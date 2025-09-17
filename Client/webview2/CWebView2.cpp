/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto: San Andreas (mtasa-blue)
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"

CWebView2::CWebView2(IDirect3DDevice9* pDevice)
{
    m_renderer = new CWebView2Renderer(pDevice);
    m_bridge = new CWebView2D3D9Bridge();
    m_bridge->Initialize(pDevice, 800, 600); // Default size, can be resized later
}

CWebView2::~CWebView2()
{
}

void CWebView2::Render()
{
    if (!m_renderer || !m_bridge)
        return;

    // Simple device state check
    HRESULT hr = m_renderer->GetDevice()->TestCooperativeLevel();

    if (FAILED(hr))
        return;

    m_bridge->UpdateTexture();
    auto texture = m_bridge->GetD3D9Texture();
    m_renderer->Render(texture, 0.0f, 0.0f, 800.0f, 600.0f);
    //m_renderer->RenderQuad(0.0f, 0.0f, 800.0f, 600.0f);
}


