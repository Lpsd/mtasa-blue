/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include <vector>

class CClientDrawContainer;

class CClientDrawContainerManager
{
public:
    CClientDrawContainerManager(CClientManager* pClientManager);
    ~CClientDrawContainerManager();

    void DoPulse();

    CClientDrawContainer* Create(DrawContainerType type);
    void                  Remove(CClientDrawContainer* pElement);

private:
    CClientManager* m_pClientManager = nullptr;

    std::vector<CClientDrawContainer*> m_vecElements;
};
