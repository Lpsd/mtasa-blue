/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto: San Andreas (mtasa-blue)
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include <d3dx9.h>

struct Vertex
{
    float x, y, z, rhw;
    DWORD color;
    float u, v;
};

class CWebView2Renderer
{
public:
    CWebView2Renderer(IDirect3DDevice9* device);
    ~CWebView2Renderer();

    void Render(IDirect3DTexture9* texture, float x, float y, float width, float height);
    void RenderQuad(float x, float y, float width, float height);
    void Cleanup();

    IDirect3DDevice9* GetDevice() const { return m_d3dDevice; }

private:
    IDirect3DDevice9*  m_d3dDevice;
};
