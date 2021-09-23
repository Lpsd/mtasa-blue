/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        gui/CGUI_Impl.cpp
 *  PURPOSE:     Graphical User Interface module class
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"

using std::list;

CGUI_Impl::CGUI_Impl(IDirect3DDevice9* pDevice)
{
    m_pRenderer = &CEGUI::Direct3D9Renderer::bootstrapSystem(pDevice, 208, CalcMTASAPath("MTA").data());
}
