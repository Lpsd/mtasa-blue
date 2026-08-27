/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Tests/client/deathmatch/CIMGArchive_Tests.cpp
 *  PURPOSE:     Google Test suite for headless IMG archive access
 *
 *  Multi Theft Auto is available from https://multitheftauto.com/
 *
 *****************************************************************************/

#include <CIMGArchive.h>
#include <gtest/gtest.h>

#include <array>
#include <chrono>
#include <cstring>

namespace
{
    struct SImgHeader
    {
        char         szMagic[4];
        unsigned int uiFilesCount;
    };

    class CIMGArchiveTest : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            const auto uniqueValue = std::chrono::steady_clock::now().time_since_epoch().count();
            m_path = std::filesystem::temp_directory_path() / ("mta-img-test-" + std::to_string(uniqueValue) + ".img");
        }

        void TearDown() override
        {
            std::error_code error;
            std::filesystem::remove(m_path, error);
        }

        void WriteArchive(const std::vector<tImgFileInfo>& fileInfos, std::string_view payload = {})
        {
            std::ofstream    file(m_path, std::ios::binary);
            const SImgHeader header{{'V', 'E', 'R', '2'}, static_cast<unsigned int>(fileInfos.size())};
            file.write(reinterpret_cast<const char*>(&header), sizeof(header));
            file.write(reinterpret_cast<const char*>(fileInfos.data()), static_cast<std::streamsize>(sizeof(tImgFileInfo) * fileInfos.size()));

            if (!payload.empty())
            {
                file.seekp(2048);
                file.write(payload.data(), static_cast<std::streamsize>(payload.size()));
            }
        }

        static tImgFileInfo MakeFileInfo(unsigned int offset, unsigned short size, std::string_view name)
        {
            tImgFileInfo fileInfo{};
            fileInfo.uiOffset = offset;
            fileInfo.usSize = size;
            std::memcpy(fileInfo.szFileName, name.data(), std::min(name.size(), sizeof(fileInfo.szFileName)));
            return fileInfo;
        }

        std::filesystem::path m_path;
    };
}

TEST_F(CIMGArchiveTest, RejectsMissingAndInvalidArchives)
{
    CIMGArchive archive;
    EXPECT_FALSE(archive.Load({}));
    EXPECT_FALSE(archive.Load(m_path));

    std::ofstream(m_path, std::ios::binary) << "invalid";
    EXPECT_FALSE(archive.Load(m_path));
    EXPECT_FALSE(archive.IsLoaded());
}

TEST_F(CIMGArchiveTest, LoadsEntriesAndFindsExactNames)
{
    WriteArchive({MakeFileInfo(1, 1, "first.dff"), MakeFileInfo(2, 3, "second.txd")});

    CIMGArchive archive;
    ASSERT_TRUE(archive.Load(m_path));
    ASSERT_EQ(archive.GetFileInfos().size(), 2u);
    EXPECT_EQ(archive.GetFileInfo(1)->usSize, 3u);
    EXPECT_EQ(archive.GetFileID("first.dff"), 0u);
    EXPECT_EQ(archive.GetFileID("second.txd"), 1u);
    EXPECT_FALSE(archive.GetFileID("FIRST.DFF").has_value());
    EXPECT_EQ(archive.GetFileInfo(2), nullptr);
    EXPECT_FALSE(archive.Load(m_path));
}

TEST_F(CIMGArchiveTest, ReadsSectorPayload)
{
    const std::string payload(2048, 'A');
    WriteArchive({MakeFileInfo(1, 1, "payload.bin")}, payload);

    CIMGArchive archive;
    ASSERT_TRUE(archive.Load(m_path));

    std::string buffer;
    ASSERT_TRUE(archive.GetFile(0, buffer));
    EXPECT_EQ(buffer, payload);
    EXPECT_THROW(archive.GetFile(1, buffer), std::invalid_argument);
}

TEST_F(CIMGArchiveTest, FailedLoadCanBeRetriedAndUnloadAllowsReload)
{
    std::ofstream(m_path, std::ios::binary) << "VER2";

    CIMGArchive archive;
    EXPECT_FALSE(archive.Load(m_path));

    WriteArchive({MakeFileInfo(1, 1, "retry.dff")});
    ASSERT_TRUE(archive.Load(m_path));

    archive.Unload();
    EXPECT_FALSE(archive.IsLoaded());
    EXPECT_TRUE(archive.GetFileInfos().empty());
    EXPECT_TRUE(archive.Load(m_path));
}

TEST_F(CIMGArchiveTest, ReportsTruncatedPayload)
{
    WriteArchive({MakeFileInfo(1, 1, "short.bin")}, "short");

    CIMGArchive archive;
    ASSERT_TRUE(archive.Load(m_path));

    std::string buffer;
    EXPECT_FALSE(archive.GetFile(0, buffer));
}
