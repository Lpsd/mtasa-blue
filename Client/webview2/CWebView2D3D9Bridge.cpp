/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto: San Andreas (mtasa-blue)
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"
#include <wrl.h>

CWebView2D3D9Bridge::CWebView2D3D9Bridge() : m_width(0), m_height(0), m_hwnd(nullptr)
{

}

CWebView2D3D9Bridge::~CWebView2D3D9Bridge()
{
    Shutdown();
}

bool CWebView2D3D9Bridge::Initialize(IDirect3DDevice9* d3d9Device, int width, int height)
{
    m_d3d9Device = d3d9Device;
    m_width = width;
    m_height = height;

    // Create a off-screen window for WebView2
    m_hwnd = CreateWindow("STATIC", "WebView2Host", WS_OVERLAPPEDWINDOW, 0, 0, width, height, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

    if (!m_hwnd)
        return false;

    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);

    // Create D3D11 device that shares resources with D3D9
    if (!CreateD3D11Device())
        return false;

    // Create WebView2
    if (!CreateWebView2())
        return false;

    // Create shared texture
    if (!CreateSharedTexture())
        return false;

    return true;
}

bool CWebView2D3D9Bridge::CreateD3D11Device()
{
    // Get the D3D9 device's adapter
    ComPtr<IDirect3D9> d3d9;
    if (FAILED(m_d3d9Device->GetDirect3D(&d3d9)))
        return false;

    // Get adapter info
    D3DADAPTER_IDENTIFIER9 adapterInfo;
    if (FAILED(d3d9->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &adapterInfo)))
        return false;

    // Create DXGI factory
    ComPtr<IDXGIFactory> dxgiFactory;
    if (FAILED(CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory))))
        return false;

    // Find the adapter that matches our D3D9 adapter
    ComPtr<IDXGIAdapter> adapter;
    for (UINT i = 0; dxgiFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        DXGI_ADAPTER_DESC desc;
        if (SUCCEEDED(adapter->GetDesc(&desc)))
        {
            // Compare by device name or other criteria
            std::string    adapterDesc = {adapterInfo.Description};
            std::wstring   wsDesc = std::wstring(adapterDesc.begin(), adapterDesc.end());
            const wchar_t* wcDesc = wsDesc.c_str();

            if (wcscmp(desc.Description, wcDesc) == 0)
            {
                m_dxgiAdapter = adapter;
                break;
            }
        }
        adapter = nullptr;
    }

    if (!m_dxgiAdapter)
        return false;

    // Create D3D11 device with the matching adapter
    D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_0};
    D3D_FEATURE_LEVEL featureLevel;

    HRESULT hr = D3D11CreateDevice(m_dxgiAdapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels,
                                   ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &m_d3d11Device, &featureLevel, &m_d3d11Context);

    return SUCCEEDED(hr);
}

HRESULT CWebView2D3D9Bridge::OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment)
{
    if (FAILED(result))
        return E_FAIL;

    if (!environment)
        return E_FAIL;

    // Create regular controller
    
    HRESULT hr = environment->CreateCoreWebView2Controller(m_hwnd, Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                                                                       this, &CWebView2D3D9Bridge::OnCreateCoreWebView2ControllerCompleted)
                                                                       .Get()
    );

    if (FAILED(hr))
    {
        environment->Release();
        return E_FAIL;
    }
}

HRESULT CWebView2D3D9Bridge::OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller)
{
    if (FAILED(result))
        return E_FAIL;

    if (!controller)
        return E_FAIL;

    m_webviewController = controller;
    controller->get_CoreWebView2(&m_webview);

    if (!m_webview)
        return E_FAIL;

    // Set size
    m_webviewController->put_Bounds({0, 0, m_width, m_height});

    // Navigate to a test page
    m_webview->Navigate(L"http://www.google.com");

    return S_OK;
}

bool CWebView2D3D9Bridge::CreateWebView2()
{
    // Create WebView2 environment synchronously
    auto    browserExecutableFolder = CalcMTASAPath(PathJoin("MTA", "webview2"));
    auto    userDataFolder = CalcMTASAPath(PathJoin("MTA", "webview2", "userdata"));

    HRESULT hr = CreateCoreWebView2Environment(
        Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(this, &CWebView2D3D9Bridge::OnCreateEnvironmentCompleted).Get()
    );
    
    if (FAILED(hr))
        return false;

    return true;
}

bool CWebView2D3D9Bridge::CreateSharedTexture()
{
    OutputDebugStringA("CWebView2D3D9Bridge::CreateSharedTexture - Start\n");

    // Create D3D11 texture that can be mapped for CPU access
    D3D11_TEXTURE2D_DESC d3d11Desc = {};
    d3d11Desc.Width = m_width;
    d3d11Desc.Height = m_height;
    d3d11Desc.MipLevels = 1;
    d3d11Desc.ArraySize = 1;
    d3d11Desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    d3d11Desc.SampleDesc.Count = 1;
    d3d11Desc.Usage = D3D11_USAGE_DYNAMIC;                        // Changed from DEFAULT
    d3d11Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;             // Removed RENDER_TARGET
    d3d11Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;            // Added CPU access
    d3d11Desc.MiscFlags = 0;                                      // Removed SHARED

    if (FAILED(m_d3d11Device->CreateTexture2D(&d3d11Desc, nullptr, &m_d3d11Texture)))
    {
        OutputDebugStringA("CWebView2D3D9Bridge::CreateSharedTexture - Failed to create D3D11 texture\n");
        return false;
    }
    OutputDebugStringA("CWebView2D3D9Bridge::CreateSharedTexture - D3D11 texture created\n");

    // Create staging texture for copying
    d3d11Desc.Usage = D3D11_USAGE_STAGING;
    d3d11Desc.BindFlags = 0;
    d3d11Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

    if (FAILED(m_d3d11Device->CreateTexture2D(&d3d11Desc, nullptr, &m_stagingTexture)))
    {
        OutputDebugStringA("CWebView2D3D9Bridge::CreateSharedTexture - Failed to create staging texture\n");
        return false;
    }
    OutputDebugStringA("CWebView2D3D9Bridge::CreateSharedTexture - Staging texture created\n");

    // Create D3D9 texture
    if (FAILED(m_d3d9Device->CreateTexture(m_width, m_height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_d3d9Texture, nullptr)))
    {
        OutputDebugStringA("CWebView2D3D9Bridge::CreateSharedTexture - Failed to create D3D9 texture\n");
        return false;
    }
    OutputDebugStringA("CWebView2D3D9Bridge::CreateSharedTexture - D3D9 texture created\n");

    return true;
}

bool CWebView2D3D9Bridge::UpdateTexture()
{
    if (!m_d3d11Texture || !m_d3d9Texture)
        return false;

    // Capture WebView2 content
    CaptureWebView2();

    // Copy from D3D11 to D3D9
    //CopyD3D11ToD3D9();
    return true;
}

void CWebView2D3D9Bridge::CaptureWebView2()
{
    if (!m_webviewController)
        return;

    // Use the window handle we created for WebView2
    HWND webViewHwnd = m_hwnd;

    if (!webViewHwnd)
    {
        OutputDebugStringA("CWebView2D3D9Bridge::CaptureWebView2 - No WebView2 window handle\n");
        return;
    }

    // Force the window to be visible and updated
    ShowWindow(webViewHwnd, SW_SHOW);
    UpdateWindow(webViewHwnd);

    // Get window DC
    HDC windowDC = GetDC(webViewHwnd);
    if (!windowDC)
    {
        OutputDebugStringA("CWebView2D3D9Bridge::CaptureWebView2 - Failed to get window DC\n");
        return;
    }

    // Create compatible DC and bitmap
    HDC     memDC = CreateCompatibleDC(windowDC);
    HBITMAP memBitmap = CreateCompatibleBitmap(windowDC, m_width, m_height);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

    // Use PrintWindow instead of BitBlt for better WebView2 support
    BOOL result = PrintWindow(webViewHwnd, memDC, PW_CLIENTONLY);
    if (!result)
    {
        // Fall back to BitBlt if PrintWindow fails
        result = BitBlt(memDC, 0, 0, m_width, m_height, windowDC, 0, 0, SRCCOPY);
    }

    if (!result)
    {
        OutputDebugStringA("CWebView2D3D9Bridge::CaptureWebView2 - Both PrintWindow and BitBlt failed\n");
    }
    else
    {
        // Rest of the bitmap processing code...
        // Get bitmap data
        BITMAPINFO bmpInfo = {};
        bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpInfo.bmiHeader.biWidth = m_width;
        bmpInfo.bmiHeader.biHeight = -m_height;
        bmpInfo.bmiHeader.biPlanes = 1;
        bmpInfo.bmiHeader.biBitCount = 32;
        bmpInfo.bmiHeader.biCompression = BI_RGB;

        std::vector<BYTE> bitmapData(m_width * m_height * 4);

        if (GetDIBits(windowDC, memBitmap, 0, m_height, bitmapData.data(), &bmpInfo, DIB_RGB_COLORS))
        {
            // Copy to D3D9 texture
            D3DLOCKED_RECT lockedRect;
            if (SUCCEEDED(m_d3d9Texture->LockRect(0, &lockedRect, nullptr, D3DLOCK_DISCARD)))
            {
                BYTE* textureData = static_cast<BYTE*>(lockedRect.pBits);
                BYTE* bitmapDataPtr = bitmapData.data();

                for (int y = 0; y < m_height; y++)
                {
                    for (int x = 0; x < m_width; x++)
                    {
                        int textureIndex = (y * m_width + x) * 4;
                        int bitmapIndex = (y * m_width + x) * 4;

                        // Convert BGRA to RGBA
                        textureData[textureIndex + 0] = bitmapDataPtr[bitmapIndex + 2];            // R
                        textureData[textureIndex + 1] = bitmapDataPtr[bitmapIndex + 1];            // G
                        textureData[textureIndex + 2] = bitmapDataPtr[bitmapIndex + 0];            // B
                        textureData[textureIndex + 3] = 255;            // A
                    }
                }

                m_d3d9Texture->UnlockRect(0);
                OutputDebugStringA("CWebView2D3D9Bridge::CaptureWebView2 - Successfully captured WebView2\n");
            }
        }
    }

    // Cleanup
    SelectObject(memDC, oldBitmap);
    DeleteObject(memBitmap);
    DeleteDC(memDC);
    ReleaseDC(webViewHwnd, windowDC);
}

void CWebView2D3D9Bridge::CopyD3D11ToD3D9()
{
    // Copy D3D11 texture to staging
    m_d3d11Context->CopyResource(m_stagingTexture.Get(), m_d3d11Texture.Get());

    // Map staging texture
    D3D11_MAPPED_SUBRESOURCE mapped;
    if (SUCCEEDED(m_d3d11Context->Map(m_stagingTexture.Get(), 0, D3D11_MAP_READ, 0, &mapped)))
    {
        // Lock D3D9 texture
        D3DLOCKED_RECT lockedRect;
        if (SUCCEEDED(m_d3d9Texture->LockRect(0, &lockedRect, nullptr, 0)))
        {
            // Copy pixel data
            BYTE* src = static_cast<BYTE*>(mapped.pData);
            BYTE* dst = static_cast<BYTE*>(lockedRect.pBits);

            for (int y = 0; y < m_height; y++)
            {
                memcpy(dst + y * lockedRect.Pitch, src + y * mapped.RowPitch, m_width * 4);
            }

            m_d3d9Texture->UnlockRect(0);
        }

        m_d3d11Context->Unmap(m_stagingTexture.Get(), 0);
    }
}

void CWebView2D3D9Bridge::NavigateTo(const std::wstring& url)
{
    if (m_webview)
        m_webview->Navigate(url.c_str());
}

void CWebView2D3D9Bridge::Resize(int width, int height)
{
    m_width = width;
    m_height = height;

    if (m_webviewController)
        m_webviewController->put_Bounds({0, 0, width, height});

    // Recreate textures
    CreateSharedTexture();
}

void CWebView2D3D9Bridge::SendMouseInput(int x, int y, bool leftButton, bool rightButton)
{
    if (m_webviewController)
    {
        POINT point = {x, y};
        //m_webviewController->SendMouseInput(COREWEBVIEW2_MOUSE_EVENT_KIND_LEFT_BUTTON_DOWN,
        //                                    leftButton ? COREWEBVIEW2_MOUSE_EVENT_VIRTUAL_KEYS_NONE : COREWEBVIEW2_MOUSE_EVENT_VIRTUAL_KEYS_NONE, 0, point);
    }
}

void CWebView2D3D9Bridge::SendKeyInput(const std::wstring& text)
{
    if (m_webview)
    {
        for (wchar_t c : text)
        {
            m_webview->PostWebMessageAsString(std::wstring(1, c).c_str());
        }
    }
}

void CWebView2D3D9Bridge::Shutdown()
{
    if (m_hwnd)
    {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }

    m_webview = nullptr;
    m_webviewController = nullptr;
    m_d3d11Texture = nullptr;
    m_stagingTexture = nullptr;
    m_d3d9Texture = nullptr;
    m_d3d11Device = nullptr;
    m_d3d11Context = nullptr;
}
