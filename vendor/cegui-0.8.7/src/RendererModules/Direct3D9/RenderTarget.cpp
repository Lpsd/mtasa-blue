/***********************************************************************
    created:    Tue Feb 10 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RendererModules/Direct3D9/RenderTarget.h"
#include "CEGUI/RendererModules/Direct3D9/GeometryBuffer.h"
#include "CEGUI/RenderQueue.h"
#include "CEGUI/Exceptions.h"
#include <d3dx9.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
Direct3D9RenderTarget::Direct3D9RenderTarget(Direct3D9Renderer& owner) :
    d_owner(owner),
    d_device(owner.getDevice())
{
}

//----------------------------------------------------------------------------//
void Direct3D9RenderTarget::activate()
{
    if (!d_matrixValid)
        updateMatrix();

    D3DVIEWPORT9 vp;
    setupViewport(vp);
    d_device->SetViewport(&vp);
    d_owner.setViewProjectionMatrix(d_matrix);
    
    const D3DMATRIX* matrix = new D3DXMATRIX(&d_matrix[0][0]);
    d_device->SetTransform(D3DTS_PROJECTION, matrix);

    RenderTarget::activate();
}

//----------------------------------------------------------------------------//
void Direct3D9RenderTarget::updateMatrix() const
{
    RenderTarget::updateMatrix(RenderTarget::createViewProjMatrixForDirect3D());
}

//----------------------------------------------------------------------------//
void Direct3D9RenderTarget::setupViewport(D3DVIEWPORT9& vp) const
{
    vp.X = static_cast<DWORD>(d_area.left());
    vp.Y = static_cast<DWORD>(d_area.top());
    vp.Width = static_cast<DWORD>(d_area.getWidth());
    vp.Height = static_cast<DWORD>(d_area.getHeight());
    vp.MinZ = 0.0f;
    vp.MaxZ = 1.0f;
}

//----------------------------------------------------------------------------//
Direct3D9Renderer& Direct3D9RenderTarget::getOwner()
{
    return d_owner;
}

//----------------------------------------------------------------------------//
} // End of  CEGUI namespace section
