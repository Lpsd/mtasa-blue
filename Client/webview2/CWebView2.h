/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto: San Andreas (mtasa-blue)
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include "WebView2.h"
#include <webview2/CWebView2Interface.h>

#include "CWebView2D3D9Bridge.h"
#include "CWebView2Renderer.h"

class CWebView2;

class CWebView2 : public CWebView2Interface
{
    public:
        CWebView2(IDirect3DDevice9* pDevice);
        ~CWebView2();

        void Render();
        
        CWebView2D3D9Bridge* GetBridge() const { return m_bridge; }

    private:
        CWebView2D3D9Bridge* m_bridge;
        CWebView2Renderer*   m_renderer;
};
