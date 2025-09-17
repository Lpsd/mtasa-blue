/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto: San Andreas (mtasa-blue)
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"
#include "png.h"

using Microsoft::WRL::Callback;

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
    m_hwnd = CreateWindow("STATIC", "MTAWebView2Host", WS_OVERLAPPEDWINDOW, 0, 0, width, height, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

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
                                                                       .Get());

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
    auto browserExecutableFolder = CalcMTASAPath(PathJoin("MTA", "webview2"));
    auto userDataFolder = CalcMTASAPath(PathJoin("MTA", "webview2", "userdata"));

    HRESULT hr = CreateCoreWebView2Environment(
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(this, &CWebView2D3D9Bridge::OnCreateEnvironmentCompleted).Get());

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
    // CopyD3D11ToD3D9();
    return true;
}

void CWebView2D3D9Bridge::ProcessPNGStream()
{
    if (!m_imageDataStream || !m_d3d9Texture)
    {
        OutputDebugStringA("CWebView2D3D9Bridge::ProcessPNGStream - Invalid parameters\n");
        return;
    }

    // Reset stream position to beginning
    LARGE_INTEGER zero = {0};
    if (FAILED(m_imageDataStream->Seek(zero, STREAM_SEEK_SET, nullptr)))
    {
        OutputDebugStringA("CWebView2D3D9Bridge::ProcessPNGStream - Failed to seek to beginning\n");
        return;
    }

    // Get stream size
    STATSTG stat;
    if (FAILED(m_imageDataStream->Stat(&stat, STATFLAG_NONAME)))
    {
        OutputDebugStringA("CWebView2D3D9Bridge::ProcessPNGStream - Failed to get stream size\n");
        return;
    }

    ULARGE_INTEGER streamSize = stat.cbSize;
    if (streamSize.QuadPart == 0)
    {
        OutputDebugStringA("CWebView2D3D9Bridge::ProcessPNGStream - Stream is empty\n");
        return;
    }

    // Read the entire stream into memory
    std::vector<BYTE> pngData(static_cast<size_t>(streamSize.QuadPart));
    ULONG             bytesRead;
    if (FAILED(m_imageDataStream->Read(pngData.data(), static_cast<ULONG>(pngData.size()), &bytesRead)))
    {
        OutputDebugStringA("CWebView2D3D9Bridge::ProcessPNGStream - Failed to read stream\n");
        return;
    }

    // Check PNG signature
    if (pngData.size() < 8)
    {
        OutputDebugStringA("CWebView2D3D9Bridge::ProcessPNGStream - Data too small for PNG\n");
        return;
    }

    // Check PNG signature (first 8 bytes)
    const BYTE pngSignature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    bool       isPNG = true;
    for (int i = 0; i < 8; i++)
    {
        if (pngData[i] != pngSignature[i])
        {
            isPNG = false;
            break;
        }
    }

    if (!isPNG)
    {
        OutputDebugStringA("CWebView2D3D9Bridge::ProcessPNGStream - Invalid PNG signature\n");
        return;
    }

    // Use libpng to decode
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr)
    {
        OutputDebugStringA("CWebView2D3D9Bridge::ProcessPNGStream - Failed to create PNG read struct\n");
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        OutputDebugStringA("CWebView2D3D9Bridge::ProcessPNGStream - Failed to create PNG info struct\n");
        return;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        OutputDebugStringA("CWebView2D3D9Bridge::ProcessPNGStream - PNG decode error\n");
        return;
    }

    // Create a simple memory reader
    struct PNGReader
    {
        BYTE*  data;
        size_t size;
        size_t offset;
    };

    PNGReader reader = {pngData.data(), pngData.size(), 0};

    // Set up custom read function
    png_set_read_fn(png_ptr, &reader,
                    [](png_structp png_ptr, png_bytep data, png_size_t length)
                    {
                        PNGReader* reader = (PNGReader*)png_get_io_ptr(png_ptr);

                        if (reader->offset + length > reader->size)
                        {
                            png_error(png_ptr, "Read beyond end of data");
                            return;
                        }

                        memcpy(data, reader->data + reader->offset, length);
                        reader->offset += length;
                    });

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    char debugMsg[256];
    sprintf_s(debugMsg, "PNG: %dx%d, color_type=%d, bit_depth=%d\n", width, height, color_type, bit_depth);
    OutputDebugStringA(debugMsg);

    // Convert to RGBA if needed
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);
    if (bit_depth == 16)
        png_set_strip_16(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);

    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);

    // Get the actual row bytes after transformations
    int rowBytes = png_get_rowbytes(png_ptr, info_ptr);
    sprintf_s(debugMsg, "Row bytes: %d\n", rowBytes);
    OutputDebugStringA(debugMsg);

    // Allocate row pointers
    png_bytep* row_pointers = (png_bytep*)malloc(height * sizeof(png_bytep));
    if (!row_pointers)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        OutputDebugStringA("CWebView2D3D9Bridge::ProcessPNGStream - Failed to allocate row pointers\n");
        return;
    }

    for (int y = 0; y < height; y++)
    {
        row_pointers[y] = (png_bytep)malloc(rowBytes);
        if (!row_pointers[y])
        {
            // Cleanup already allocated rows
            for (int i = 0; i < y; i++)
                free(row_pointers[i]);
            free(row_pointers);
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            OutputDebugStringA("CWebView2D3D9Bridge::ProcessPNGStream - Failed to allocate row data\n");
            return;
        }
    }

    // Read the image
    png_read_image(png_ptr, row_pointers);

    // Copy to D3D9 texture
    D3DLOCKED_RECT lockedRect;
    if (SUCCEEDED(m_d3d9Texture->LockRect(0, &lockedRect, nullptr, D3DLOCK_DISCARD)))
    {
        BYTE* textureData = static_cast<BYTE*>(lockedRect.pBits);

        // Clear the texture first
        memset(textureData, 0, m_width * m_height * 4);

        // Copy PNG data to texture with proper bounds checking
        for (int y = 0; y < height && y < m_height; y++)
        {
            for (int x = 0; x < width && x < m_width; x++)
            {
                // Calculate texture index with bounds checking
                int textureIndex = (y * m_width + x) * 4;
                if (textureIndex + 3 >= m_width * m_height * 4)
                {
                    sprintf_s(debugMsg, "Texture bounds exceeded at y=%d, x=%d\n", y, x);
                    OutputDebugStringA(debugMsg);
                    break;
                }

                // Calculate image index with bounds checking
                int imageIndex = x * 4;            // RGBA format
                if (imageIndex + 3 >= rowBytes)
                {
                    sprintf_s(debugMsg, "Image bounds exceeded at y=%d, x=%d\n", y, x);
                    OutputDebugStringA(debugMsg);
                    break;
                }

                // Copy RGBA data with bounds checking
                textureData[textureIndex + 0] = row_pointers[y][imageIndex + 0];            // R
                textureData[textureIndex + 1] = row_pointers[y][imageIndex + 1];            // G
                textureData[textureIndex + 2] = row_pointers[y][imageIndex + 2];            // B
                textureData[textureIndex + 3] = row_pointers[y][imageIndex + 3];            // A
            }
        }

        m_d3d9Texture->UnlockRect(0);
        OutputDebugStringA("CWebView2D3D9Bridge::ProcessPNGStream - Successfully processed PNG\n");
    }

    // Cleanup
    for (int y = 0; y < height; y++)
    {
        free(row_pointers[y]);
    }
    free(row_pointers);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
}

void CWebView2D3D9Bridge::CaptureWebView2()
{
    if (!m_webview || m_isCapturePending)
        return;

    // Create a new memory stream for each capture
    if (m_imageDataStream)
    {
        m_imageDataStream.Reset();
    }

    m_isCapturePending = true;

    // Create a memory stream for the PNG data
    if (FAILED(CreateStreamOnHGlobal(nullptr, TRUE, &m_imageDataStream)))
    {
        OutputDebugStringA("CWebView2D3D9Bridge::CaptureWebView2 - Failed to create stream\n");
        return;
    }

    // Use WebView2's built-in capture method
    HRESULT hr = m_webview->CapturePreview(COREWEBVIEW2_CAPTURE_PREVIEW_IMAGE_FORMAT_PNG, m_imageDataStream.Get(),
                                           Callback<ICoreWebView2CapturePreviewCompletedHandler>(
                                               [this](HRESULT result) -> HRESULT
                                               {
                                                   m_isCapturePending = false;

                                                   if (SUCCEEDED(result))
                                                   {
                                                       OutputDebugStringA("CWebView2D3D9Bridge::CaptureWebView2 - Capture succeeded\n");
                                                       ProcessPNGStream();
                                                   }
                                                   else
                                                   {
                                                       char errorMsg[256];
                                                       sprintf_s(errorMsg, "CWebView2D3D9Bridge::CaptureWebView2 - Capture failed: 0x%08X\n", result);
                                                       OutputDebugStringA(errorMsg);
                                                   }
                                                   return S_OK;
                                               })
                                               .Get());

    if (FAILED(hr))
    {
        OutputDebugStringA("CWebView2D3D9Bridge::CaptureWebView2 - CapturePreview call failed\n");
        m_isCapturePending = false;
    }
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
        // m_webviewController->SendMouseInput(COREWEBVIEW2_MOUSE_EVENT_KIND_LEFT_BUTTON_DOWN,
        //                                     leftButton ? COREWEBVIEW2_MOUSE_EVENT_VIRTUAL_KEYS_NONE : COREWEBVIEW2_MOUSE_EVENT_VIRTUAL_KEYS_NONE, 0, point);
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
