/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto: San Andreas (mtasa-blue)
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include <d3d9.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class CWebView2D3D9Bridge
{
public:
    CWebView2D3D9Bridge();
    ~CWebView2D3D9Bridge();

    bool Initialize(IDirect3DDevice9* d3d9Device, int width, int height);
    void Shutdown();

    bool               UpdateTexture();            // Call this each frame
    IDirect3DTexture9* GetD3D9Texture() const { return m_d3d9Texture.Get(); }

    // WebView2 control methods
    void NavigateTo(const std::wstring& url);
    void Resize(int width, int height);
    void SendMouseInput(int x, int y, bool leftButton, bool rightButton);
    void SendKeyInput(const std::wstring& text);

private:
    // D3D9 references
    ComPtr<IDirect3DDevice9>  m_d3d9Device;
    ComPtr<IDirect3DTexture9> m_d3d9Texture;

    // D3D11 bridge device
    ComPtr<ID3D11Device>        m_d3d11Device;
    ComPtr<ID3D11DeviceContext> m_d3d11Context;
    ComPtr<IDXGIDevice>         m_dxgiDevice;
    ComPtr<IDXGIAdapter>        m_dxgiAdapter;

    // WebView2
    ComPtr<ICoreWebView2Controller> m_webviewController;
    ComPtr<ICoreWebView2>           m_webview;

    // Capture
    ComPtr<ID3D11Texture2D> m_d3d11Texture;
    ComPtr<ID3D11Texture2D> m_stagingTexture;

    // Dimensions
    int  m_width, m_height;
    HWND m_hwnd;

    // Internal methods
    bool CreateD3D11Device();
    bool CreateWebView2();
    bool CreateSharedTexture();
    void CopyD3D11ToD3D9();
    void CaptureWebView2();

    HRESULT OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment);
    HRESULT OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller);
};
