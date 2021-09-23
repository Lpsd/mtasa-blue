/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        gui/CGUI_Impl.h
 *  PURPOSE:     Graphical User Interface module class
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include <gui/CGUI.h>
#include <list>
#include <windows.h>

namespace CEGUI {
    class Direct3D9Renderer;
    class System;
};

class CGUI_Impl : public CGUI
{
public:
    CGUI_Impl(IDirect3DDevice9* pDevice);

private:

    IDirect3DDevice9* m_pDevice;

    CEGUI::Direct3D9Renderer*    m_pRenderer;
    CEGUI::System*               m_pSystem;
};
