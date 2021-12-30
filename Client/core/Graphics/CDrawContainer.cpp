/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"

CDrawContainer::CDrawContainer()
{

}

CDrawContainer::~CDrawContainer()
{

}

void CDrawContainer::OnPulse()
{
    printf(SString("CDrawContainer::OnPulse %i \n", static_cast<int>(GetTickCount32())).c_str());
}

void CDrawContainer::SetType(DrawContainerType type)
{
    m_type = type;
}
