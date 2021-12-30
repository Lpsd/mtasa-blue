/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"

CClientDrawContainerManager::CClientDrawContainerManager(CClientManager* pClientManager)
{
    m_pClientManager = pClientManager;
}

CClientDrawContainerManager::~CClientDrawContainerManager()
{
}

CClientDrawContainer* CClientDrawContainerManager::Create(DrawContainerType type)
{
    CDrawContainerManagerInterface* pInterface = g_pCore->GetGraphics()->GetDrawContainerManager();

    if (!pInterface)
        return nullptr;

    CDrawContainer* pDrawContainer = pInterface->Create(type);

    if (!pDrawContainer)
        return nullptr;

    CClientDrawContainer* pElement = new CClientDrawContainer(m_pClientManager, INVALID_ELEMENT_ID, pDrawContainer);
    m_vecElements.push_back(pElement);

    return pElement;
}

void CClientDrawContainerManager::Remove(CClientDrawContainer* pElement)
{
    m_vecElements.erase(std::remove(m_vecElements.begin(), m_vecElements.end(), pElement), m_vecElements.end());
}

void CClientDrawContainerManager::DoPulse()
{
    for (auto& element : m_vecElements)
    {
        element->DoPulse();
    }
}
