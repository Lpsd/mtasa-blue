/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"

CDrawContainerLine3D::CDrawContainerLine3D()
{
    SetType(DrawContainerType::LINE3D);

    printf("created RECT draw container");
}

CDrawContainerLine3D::~CDrawContainerLine3D()
{

}

void CDrawContainerLine3D::OnPulse()
{
    g_pGraphics->DrawLine3D(m_position, m_endPosition, m_color, m_size.fX);
}
