/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto: San Andreas (mtasa-blue)
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"

CWebView2Renderer::CWebView2Renderer(IDirect3DDevice9* device) : m_d3dDevice(device)
{
}

CWebView2Renderer::~CWebView2Renderer()
{
    Cleanup();
}

void CWebView2Renderer::Render(IDirect3DTexture9* texture, float x, float y, float width, float height)
{
    if (!m_d3dDevice || !texture)
        return;

    // Set up render states
    m_d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    // Set texture
    m_d3dDevice->SetTexture(0, texture);
    m_d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

    // Set vertex format
    m_d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

    // Use a high Z value (close to 1.0) to ensure it renders on top
    float z = 0.999f;            // Very close to 1.0 (screen)

    Vertex vertices[4] = {{x, y, z, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f},
                          {x + width, y, z, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f},
                          {x, y + height, z, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f},
                          {x + width, y + height, z, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f}};

    // Draw the quad
    m_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(Vertex));

    // Reset render states
    m_d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_d3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_d3dDevice->SetTexture(0, nullptr);
}

void CWebView2Renderer::RenderQuad(float x, float y, float width, float height)
{
    if (!m_d3dDevice)
        return;

    // Set render states
    m_d3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    m_d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    m_d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    // Don't set any texture - just draw a colored quad
    m_d3dDevice->SetTexture(0, nullptr);
    m_d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

    // Set vertex format
    m_d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

    // Create vertices for a bright red quad
    struct Vertex
    {
        float x, y, z, rhw;
        DWORD color;
    };

    float z = 0.999f;
    DWORD redColor = 0xFFFF0000;            // Bright red

    Vertex vertices[4] = {
        {x, y, z, 1.0f, redColor}, {x + width, y, z, 1.0f, redColor}, {x, y + height, z, 1.0f, redColor}, {x + width, y + height, z, 1.0f, redColor}};

    // Draw the quad
    m_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(Vertex));

    // Reset render states
    m_d3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    m_d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_d3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CWebView2Renderer::Cleanup()
{
    m_d3dDevice = nullptr;
}
