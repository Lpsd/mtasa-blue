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

CGUI_Impl::CGUI_Impl(IDirect3DDevice9* pDevice)
{
    // Init
    m_pDevice = pDevice;
}

CGUI_Impl::~CGUI_Impl()
{

}