/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        sdk/webview2/CWebView2Interface.h
 *  PURPOSE:     WebView2 interface class
 *
 *****************************************************************************/
#pragma once

class CWebView2Interface;
class CWebView2D3D9Bridge;

class CWebView2Interface
{
public:
    virtual CWebView2D3D9Bridge* GetBridge() const = 0;

    virtual void Render() = 0;
};
