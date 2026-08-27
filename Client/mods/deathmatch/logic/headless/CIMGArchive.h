/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Client/mods/deathmatch/logic/headless/CIMGArchive.h
 *  PURPOSE:     Headless IMG container file access
 *
 *  Multi Theft Auto is available from https://multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include <filesystem>
#include <fstream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

struct tImgFileInfo
{
    unsigned int   uiOffset;
    unsigned short usSize;
    unsigned short usUnpackedSize;
    char           szFileName[24];
};

class CIMGArchive
{
public:
    bool Load(const std::filesystem::path& filePath);
    void Unload();

    bool IsLoaded() const noexcept { return m_ifs.is_open(); }

    const std::filesystem::path&     GetFilePath() const noexcept { return m_filePath; }
    const std::vector<tImgFileInfo>& GetFileInfos() const noexcept { return m_fileInfos; }
    tImgFileInfo*                    GetFileInfo(size_t fileID) noexcept;

    std::optional<size_t> GetFileID(std::string_view filename) const noexcept;
    bool                  GetFile(size_t fileID, std::string& buffer);

private:
    std::ifstream             m_ifs;
    std::filesystem::path     m_filePath;
    std::vector<tImgFileInfo> m_fileInfos;
};
