/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"

CDrawContainerRect::CDrawContainerRect()
{
    SetType(DrawContainerType::RECT);

    printf("created RECT draw container");
}

CDrawContainerRect::~CDrawContainerRect()
{

}

void CDrawContainerRect::OnPulse()
{
    printf(SString("CDrawContainerRect::OnPulse %i \n", static_cast<int>(GetTickCount32())).c_str());

    g_pGraphics->DrawRectangle(m_position.fX, m_position.fY, m_size.fX, m_size.fY, 0xFF000000);
}
