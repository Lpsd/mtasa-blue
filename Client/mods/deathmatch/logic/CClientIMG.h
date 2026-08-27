/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/shared_logic/CClientIMG.h
*  PURPOSE:     IMG container class header

*
*****************************************************************************/

#pragma once

#include "CClientEntity.h"
#include "../headless/CIMGArchive.h"
#include <vector>
#include <optional>
#include <variant>
#include <string_view>
#include <filesystem>
namespace fs = std::filesystem;

struct tLinkedModelRestoreInfo
{
    constexpr tLinkedModelRestoreInfo(unsigned int uiModelID, unsigned int uiOffset, unsigned short usSize, unsigned char ucStreamID)
        : uiModelID(uiModelID), uiOffset(uiOffset), usSize(usSize), ucStreamID(ucStreamID)
    {
    }

    unsigned int   uiModelID;
    unsigned int   uiOffset;
    unsigned short usSize;
    unsigned char  ucStreamID;
};

class CClientIMG : public CClientEntity
{
    DECLARE_CLASS(CClientIMG, CClientEntity)
    friend class CClientIMGManager;

public:
    CClientIMG(class CClientManager* pManager, ElementID ID);
    ~CClientIMG();

    void Unlink();
    void GetPosition(CVector& vecPosition) const {};
    void SetPosition(const CVector& vecPosition) {};

    eClientEntityType GetType() const { return CCLIENTIMG; }
    unsigned char     GetArchiveID() const { return m_ucArchiveID; }
    unsigned int      GetFilesCount() const { return m_archive.GetFileInfos().size(); }
    const auto&       GetFileInfos() const noexcept { return m_archive.GetFileInfos(); }
    auto              GetLargestFileSizeBlocks() const { return m_LargestFileSizeBlocks; }

    bool Load(fs::path filePath);
    void Unload();

    tImgFileInfo*         GetFileInfo(size_t fileID);
    std::optional<size_t> GetFileID(std::string_view filename);
    bool                  GetFile(size_t uiFileID, std::string& buffer);

    bool StreamEnable();
    bool StreamDisable();
    bool IsStreamed();

    bool LinkModel(unsigned int usModelID, size_t fileID);
    bool UnlinkModel(unsigned int usModelID);

private:
    class CClientIMGManager* m_pImgManager;

    CIMGArchive   m_archive;
    unsigned char m_ucArchiveID;
    size_t        m_LargestFileSizeBlocks;  // The size of the largest file [in streaming blocks/sectors]

    std::vector<tLinkedModelRestoreInfo> m_restoreInfo;
};
