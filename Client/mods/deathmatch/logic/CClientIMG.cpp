
/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *               (Shared logic for modifications)
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        mods/shared_logic/CClientIMG.cpp
 *  PURPOSE:     IMG container class
 *
 *****************************************************************************/

#include <StdInc.h>
#include "game/CStreaming.h"

CClientIMG::CClientIMG(class CClientManager* pManager, ElementID ID)
    : ClassInit(this), CClientEntity(ID), m_pImgManager(pManager->GetIMGManager()), m_ucArchiveID(INVALID_ARCHIVE_ID), m_LargestFileSizeBlocks(0)
{
    m_pManager = pManager;
    SetTypeName("img");
    m_pImgManager->AddToList(this);
}

CClientIMG::~CClientIMG()
{
    m_pImgManager->RemoveFromList(this);
    Unlink();
}

void CClientIMG::Unlink()
{
    if (IsStreamed())
        StreamDisable();

    if (m_archive.IsLoaded())
        Unload();
}

bool CClientIMG::Load(fs::path filePath)
{
    return m_archive.Load(filePath);
}

void CClientIMG::Unload()
{
    m_archive.Unload();
}

bool CClientIMG::GetFile(size_t fileID, std::string& buffer)
{
    return m_archive.GetFile(fileID, buffer);
}

tImgFileInfo* CClientIMG::GetFileInfo(size_t fileID)
{
    return m_archive.GetFileInfo(fileID);
}

std::optional<size_t> CClientIMG::GetFileID(std::string_view filename)
{
    return m_archive.GetFileID(filename);
}

bool CClientIMG::IsStreamed()
{
    return m_ucArchiveID != INVALID_ARCHIVE_ID;
}

bool CClientIMG::StreamEnable()
{
    if (m_archive.GetFileInfos().empty())
        return false;

    if (IsStreamed())
        return false;

    if (m_LargestFileSizeBlocks == 0)
    {
        for (const auto& fileInfo : m_archive.GetFileInfos())
            m_LargestFileSizeBlocks = std::max(m_LargestFileSizeBlocks, (size_t)fileInfo.usSize);
    }

    m_ucArchiveID = g_pGame->GetStreaming()->AddArchive(m_archive.GetFilePath().wstring().c_str());

    if (IsStreamed())
    {
        m_pImgManager->UpdateStreamerBufferSize();
        return true;
    }
    return false;
}

bool CClientIMG::StreamDisable()
{
    if (!IsStreamed())
        return false;

    // Unlink all models
    for (const auto& v : m_restoreInfo)
    {
        g_pGame->GetStreaming()->SetStreamingInfo(v.uiModelID, v.ucStreamID, v.uiOffset, v.usSize);
    }
    m_restoreInfo.clear();
    m_restoreInfo.shrink_to_fit();

    // Remove archive from streaming
    g_pGame->GetStreaming()->RemoveArchive(m_ucArchiveID);
    m_ucArchiveID = INVALID_ARCHIVE_ID;

    m_pImgManager->UpdateStreamerBufferSize();

    // During session shutdown (CClientManager being destroyed), element destruction
    // order is arbitrary. Skip restreaming because earlier element cleanup may have
    // already freed TXD pool slots, and ReinitStreaming would flush pending
    // streaming channels that reference those freed parent slots.
    if (!m_pManager || !m_pManager->IsBeingDeleted())
        g_pClientGame->RestreamWorld();

    return true;
}

bool CClientIMG::LinkModel(unsigned int uiModelID, size_t uiFileID)
{
    if (!IsStreamed())
        return false;

    tImgFileInfo* pFileInfo = GetFileInfo(uiFileID);
    if (!pFileInfo)
        return false;

    CStreamingInfo* pCurrInfo = g_pGame->GetStreaming()->GetStreamingInfo(uiModelID);
    if (!pCurrInfo)
        return false;

    if (pCurrInfo->archiveId == m_ucArchiveID)
        return true;  // Already linked

    m_restoreInfo.emplace_back(uiModelID, pCurrInfo->offsetInBlocks, pCurrInfo->sizeInBlocks, pCurrInfo->archiveId);

    // Internally stream out the vehicle before calling CStreamingSA::RemoveModel
    // otherwise a crash will occur if the player is inside a vehicle that gets unloaded by the streamer
    if (CClientVehicleManager::IsValidModel(uiModelID))
        g_pClientGame->GetVehicleManager()->RestreamVehicles(static_cast<unsigned short>(uiModelID));

    // Weapon models already in a ped's hand keep their old RW clump until the weapon slot is
    // re-requested, same as vehicles above (see CClientDFF::ReplaceWeaponModel for the equivalent case)
    if (CClientPedManager::IsValidWeaponModel(uiModelID))
        g_pClientGame->GetPedManager()->RestreamWeapon(static_cast<unsigned short>(uiModelID));

    g_pGame->GetStreaming()->SetStreamingInfo(uiModelID, m_ucArchiveID, pFileInfo->uiOffset, pFileInfo->usSize);

    return true;
}

bool CClientIMG::UnlinkModel(unsigned int uiModelID)
{
    const auto it =
        std::find_if(m_restoreInfo.begin(), m_restoreInfo.end(), [uiModelID](const auto& restoreInfo) { return restoreInfo.uiModelID == uiModelID; });

    if (it == m_restoreInfo.end())
        return false;

    g_pGame->GetStreaming()->SetStreamingInfo(uiModelID, it->ucStreamID, it->uiOffset, it->usSize);

    m_restoreInfo.erase(it);

    return true;
}
