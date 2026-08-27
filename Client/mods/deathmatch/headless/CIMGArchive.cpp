/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Client/mods/deathmatch/headless/CIMGArchive.cpp
 *  PURPOSE:     Headless IMG container file access
 *
 *  Multi Theft Auto is available from https://multitheftauto.com/
 *
 *****************************************************************************/

#include "CIMGArchive.h"

#include <algorithm>
#include <cstring>
#include <limits>
#include <new>
#include <stdexcept>

namespace
{
    struct SImgHeader
    {
        char         szMagic[4];
        unsigned int uiFilesCount;
    };
}

bool CIMGArchive::Load(const std::filesystem::path& filePath)
{
    if (IsLoaded() || !m_fileInfos.empty() || filePath.empty() || !std::filesystem::exists(filePath))
        return false;

    std::ifstream file(filePath, std::ios::binary);
    if (!file)
        return false;

    SImgHeader fileHeader{};
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    if (!file || std::memcmp(fileHeader.szMagic, "VER2", sizeof(fileHeader.szMagic)) != 0)
        return false;

    if (fileHeader.uiFilesCount > std::numeric_limits<size_t>::max() / sizeof(tImgFileInfo))
        return false;

    std::vector<tImgFileInfo> fileInfos;
    try
    {
        fileInfos.resize(fileHeader.uiFilesCount);
    }
    catch (const std::bad_alloc&)
    {
        return false;
    }

    file.read(reinterpret_cast<char*>(fileInfos.data()), static_cast<std::streamsize>(sizeof(tImgFileInfo) * fileInfos.size()));
    if (!file)
        return false;

    // Commit only after complete validation so a failed load can be retried.
    m_filePath = filePath;
    m_fileInfos = std::move(fileInfos);
    m_ifs = std::move(file);
    return true;
}

void CIMGArchive::Unload()
{
    m_fileInfos.clear();
    m_fileInfos.shrink_to_fit();
    m_ifs.close();
    m_filePath.clear();
}

bool CIMGArchive::GetFile(size_t fileID, std::string& buffer)
{
    const tImgFileInfo* fileInfo = GetFileInfo(fileID);
    if (!fileInfo)
        throw std::invalid_argument("Invalid file id");

    const size_t bytesToRead = static_cast<size_t>(fileInfo->usSize) * 2048u;

    try
    {
        buffer.resize(bytesToRead);
    }
    catch (const std::bad_alloc&)
    {
        throw std::invalid_argument("Out of memory");
    }

    m_ifs.clear();
    m_ifs.seekg(static_cast<std::streamoff>(fileInfo->uiOffset) * 2048);
    m_ifs.read(buffer.data(), static_cast<std::streamsize>(bytesToRead));
    return static_cast<bool>(m_ifs);
}

tImgFileInfo* CIMGArchive::GetFileInfo(size_t fileID) noexcept
{
    if (fileID >= m_fileInfos.size())
        return nullptr;

    return &m_fileInfos[fileID];
}

std::optional<size_t> CIMGArchive::GetFileID(std::string_view filename) const noexcept
{
    const auto iter = std::find_if(m_fileInfos.begin(), m_fileInfos.end(),
                                   [filename](const tImgFileInfo& fileInfo)
                                   {
                                       const char* nameEnd = std::find(std::begin(fileInfo.szFileName), std::end(fileInfo.szFileName), '\0');
                                       return filename == std::string_view(fileInfo.szFileName, static_cast<size_t>(nameEnd - fileInfo.szFileName));
                                   });

    if (iter == m_fileInfos.end())
        return std::nullopt;

    return static_cast<size_t>(std::distance(m_fileInfos.begin(), iter));
}
