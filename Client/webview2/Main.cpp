/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        webview2/Main.cpp
 *  PURPOSE:     WebView2 interface init and DLL entry point
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"
#define ALLOC_STATS_MODULE_NAME "cwebview2"
#include "SharedUtil.hpp"

CWebView2* g_webView2 = NULL;

int WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, PVOID pvNothing)
{
    // Eventually destroy our GUI upon detach
    if (dwReason == DLL_PROCESS_DETACH && g_webView2)
    {
        delete g_webView2;
    }

    return TRUE;
}

MTAEXPORT CWebView2Interface* InitWebView2Interface(IDirect3DDevice9* pDevice)
{
    // Create our GUI interface if not already done
    if (!g_webView2)
    {
        SetMemoryAllocationFailureHandler();
        g_webView2 = new CWebView2(pDevice);
    }

    // Return it
    return g_webView2;
}
