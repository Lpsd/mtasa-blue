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

class CGUI_Impl;

#pragma once

class CGUI_Impl
{
public:
    CGUI_Impl(IDirect3DDevice9* pDevice);
    ~CGUI_Impl();
private:
    IDirect3DDevice9* m_pDevice;
};
