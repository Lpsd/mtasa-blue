/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *               (Shared logic for modifications)
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        mods/shared_logic/CClientColShape.cpp
 *  PURPOSE:     Shaped collision entity base class
 *
 *****************************************************************************/

#include "StdInc.h"

CClientDrawContainer::CClientDrawContainer(CClientManager* pManager, ElementID ID, CDrawContainer* pDrawContainer) : ClassInit(this), CClientEntity(ID)
{
    // Init
    m_pManager = pManager;
    m_pDrawContainer = pDrawContainer;

    m_bIsEnabled = true;

    SetTypeName("drawcontainer");
}

CClientDrawContainer::~CClientDrawContainer()
{
    delete m_pDrawContainer;

    Unlink();
}

void CClientDrawContainer::Unlink()
{
    auto pManager = m_pManager->GetDrawContainerManager();

    if (pManager)
        pManager->Remove(this);
}

void CClientDrawContainer::DoPulse()
{
    if (m_pDrawContainer)
    {
        m_pDrawContainer->OnPulse();

        CVector position;
        GetPosition(position);

        m_pDrawContainer->SetPosition(position);
    }
        
}

void CClientDrawContainer::SetPosition(const CVector& vecPosition)
{
    if (vecPosition != m_vecPosition)
    {
        m_vecPosition = vecPosition;
    }
}

CVector2D CClientDrawContainer::GetSize() const
{
    if (!m_pDrawContainer)
        return {};

    return m_pDrawContainer->GetSize();
}

void CClientDrawContainer::SetSize(CVector2D size)
{
    if (!m_pDrawContainer)
        return;

    return m_pDrawContainer->SetSize(size);
}

DrawContainerType CClientDrawContainer::GetContainerType() const
{
    if (!m_pDrawContainer)
        return DrawContainerType::DEFAULT;

    return m_pDrawContainer->GetType();
}
