/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once
#include "CClientEntity.h"

class CDrawContainer;

class CClientDrawContainer : public CClientEntity
{
    DECLARE_CLASS(CClientDrawContainer, CClientEntity);

public:
    CClientDrawContainer(class CClientManager* pManager, ElementID ID, CDrawContainer* pDrawContainer);
    ~CClientDrawContainer();

    void DoPulse();
    void Unlink();
    
    CResource* GetResource() const { return m_pResource; }
    void       SetResource(CResource* pResource) { m_pResource = pResource; }

    CDrawContainer* GetDrawContainer() const { return m_pDrawContainer; }

    DrawContainerType GetContainerType() const;
    eClientEntityType GetType() const { return CCLIENTDRAWCONTAINER; }

    virtual void GetPosition(CVector& vecPosition) const { vecPosition = m_vecPosition; };
    virtual void SetPosition(const CVector& vecPosition);

    CVector2D GetSize() const;
    void      SetSize(CVector2D size);

    bool IsEnabled() { return m_bIsEnabled; };
    void SetEnabled(bool bEnabled) { m_bIsEnabled = bEnabled; };

protected:
    CVector m_vecPosition;

private:
    CResource*      m_pResource = nullptr;
    CDrawContainer* m_pDrawContainer = nullptr;

    bool m_bIsEnabled;
};
