/***********************************************************************
    created:    7th December 2013
    author:     Lukas Meindl
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
*
*   Permission is hereby granted, free of charge, to any person obtaining
*   a copy of this software and associated documentation files (the
*   "Software"), to deal in the Software without restriction, including
*   without limitation the rights to use, copy, modify, merge, publish,
*   distribute, sublicense, and/or sell copies of the Software, and to
*   permit persons to whom the Software is furnished to do so, subject to
*   the following conditions:
*
*   The above copyright notice and this permission notice shall be
*   included in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
*   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
*   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
*   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
*   OTHER DEALINGS IN THE SOFTWARE.
***************************************************************************/
#ifndef _CEGUIDirect3D9ShaderWrapper_h_
#define _CEGUIDirect3D9ShaderWrapper_h_

#include "CEGUI/ShaderWrapper.h"
#include "CEGUI/RendererModules/Direct3D9/Renderer.h"
#include <string>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
class ShaderParameterBindings;


class DIRECT3D9_GUIRENDERER_API Direct3D9ShaderWrapper : public ShaderWrapper
{
public:
    Direct3D9ShaderWrapper();

    ~Direct3D9ShaderWrapper();

    //Implementation of ShaderWrapper interface
    void prepareForRendering(const ShaderParameterBindings* shaderParameterBindings) override;
};


}

#endif

