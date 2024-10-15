/***********************************************************************
    created:    Mon Feb 9 2009
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
#include "CEGUI/RendererModules/Direct3D9/GeometryBuffer.h"
#include "CEGUI/RendererModules/Direct3D9/Texture.h"
#include "CEGUI/RenderEffect.h"
#include "CEGUI/RenderMaterial.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/Colour.h"
#include <d3d9.h>

static std::uint32_t calculateArgb(std::uint8_t alpha, std::uint8_t red, std::uint8_t green, std::uint8_t blue)
{
    return (static_cast<std::uint32_t>(alpha) << 24 | static_cast<std::uint32_t>(red) << 16 | static_cast<std::uint32_t>(green) << 8 |
            static_cast<std::uint32_t>(blue));
}

// Start of CEGUI namespace section
namespace CEGUI
{
    //----------------------------------------------------------------------------//
    Direct3D9GeometryBuffer::Direct3D9GeometryBuffer(Direct3D9Renderer& owner, LPDIRECT3DDEVICE9 device, CEGUI::RefCounted<RenderMaterial> renderMaterial)
        : GeometryBuffer(renderMaterial),
          d_owner(owner),
          d_activeTexture(0),
          d_clipRect(0, 0, 0, 0),
          d_clippingActive(true),
          d_translation(0, 0, 0),
          d_rotation(.0f, .0f, .0f, .0f),
          d_pivot(0, 0, 0),
          d_effect(0),
          d_device(device),
          d_matrixValid(false),
          d_vertexBufferSize(0)
    {
    }

    //----------------------------------------------------------------------------//
    void Direct3D9GeometryBuffer::draw(uint32_t drawModeMask) const
    {
        RECT saved_clip;
        d_device->GetScissorRect(&saved_clip);

        // setup clip region
        RECT clip;
        clip.left = static_cast<LONG>(d_clipRect.left());
        clip.top = static_cast<LONG>(d_clipRect.top());
        clip.right = static_cast<LONG>(d_clipRect.right());
        clip.bottom = static_cast<LONG>(d_clipRect.bottom());

        // apply the transformations we need to use.
        if (!d_matrixValid)
            updateMatrix();

        d_device->SetTransform(D3DTS_WORLD, &d_matrix);
        d_device->BeginScene();

        d_owner.setupRenderingBlendMode(d_blendMode);

        const int pass_count = d_effect ? d_effect->getPassCount() : 1;

        for (int pass = 0; pass < pass_count; ++pass)
        {
            // set up RenderEffect
            if (d_effect)
                d_effect->performPreRenderFunctions(pass);

            d_renderMaterial->prepareForRendering();

            // draw the batches
            size_t                    pos = 0;
            BatchList::const_iterator i = d_batches.begin();
            for (; i != d_batches.end(); ++i)
            {
                if (i->clip)
                    d_device->SetScissorRect(&clip);

                d_device->SetTexture(0, i->texture);
                d_device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, i->vertexCount / 3, (void*)&d_vertices[pos], sizeof(D3DVertex));

                pos += i->vertexCount;

                if (i->clip)
                    d_device->SetScissorRect(&saved_clip);
            }
        }

        // clean up RenderEffect
        if (d_effect)
            d_effect->performPostRenderFunctions();

        d_device->EndScene();

        updateRenderTargetData(d_owner.getActiveRenderTarget());
    }

    //----------------------------------------------------------------------------//
    void Direct3D9GeometryBuffer::onGeometryChanged()
    {
        updateVertexBuffer();
    }

    //----------------------------------------------------------------------------//
    void Direct3D9GeometryBuffer::updateVertexBuffer()
    {
        if (d_vertexData.empty())
            return;

        performBatchManagement();

        const unsigned int vertexItemCount = d_vertexData.size();

        if (d_vertexBufferSize < d_vertexCount)
        {
            cleanupVertexBuffer();
            allocateVertexBuffer(d_vertexCount);
            d_vertexBufferSize = d_vertexCount;
        }

        // update size of current batch
        d_batches.back().vertexCount += d_vertexCount;

        // buffer these vertices
        D3DVertex vertex;
        for (const float& data : d_vertexData)
        {
            auto& cv = reinterpret_cast<const TexturedColouredVertex&>(data);
            // copy vertex info the buffer, converting from CEGUI::Vertex to
            // something directly usable by D3D as needed.
            vertex.x = cv.d_position.x - 0.5f;
            vertex.y = cv.d_position.y - 0.5f;
            vertex.z = cv.d_position.z;
            vertex.diffuse = calculateArgb(cv.d_colour.a, cv.d_colour.r, cv.d_colour.g, cv.d_colour.b);
            vertex.tu = cv.d_texCoords.x;
            vertex.tv = cv.d_texCoords.y;
            d_vertices.push_back(vertex);
        }
    }

    //----------------------------------------------------------------------------//
    void Direct3D9GeometryBuffer::cleanupVertexBuffer() const
    {
        if (d_vertexBuffer)
        {
            d_vertexBuffer->Release();
            d_vertexBuffer = 0;
            d_vertexBufferSize = 0;
        }
    }

    void Direct3D9GeometryBuffer::allocateVertexBuffer(unsigned int vertexSize) const
    {
        d_device->CreateVertexBuffer(vertexSize, 0, 0,
                                     D3DPOOL_MANAGED, &d_vertexBuffer, NULL);
        d_device->SetStreamSource(0, d_vertexBuffer, 0, sizeof(D3DVertex));
    }


    //----------------------------------------------------------------------------//
    void Direct3D9GeometryBuffer::setTranslation(const glm::vec3& t)
    {
        d_translation = t;
        d_matrixValid = false;
    }

    //----------------------------------------------------------------------------//
    void Direct3D9GeometryBuffer::setRotation(const glm::quat& r)
    {
        d_rotation = r;
        d_matrixValid = false;
    }

    //----------------------------------------------------------------------------//
    void Direct3D9GeometryBuffer::setPivot(const glm::vec3& p)
    {
        d_pivot = p;
        d_matrixValid = false;
    }

    //----------------------------------------------------------------------------//
    void Direct3D9GeometryBuffer::setClippingRegion(const Rectf& region)
    {
        d_clipRect.top(std::max(0.0f, region.top()));
        d_clipRect.bottom(std::max(0.0f, region.bottom()));
        d_clipRect.left(std::max(0.0f, region.left()));
        d_clipRect.right(std::max(0.0f, region.right()));
    }

    //----------------------------------------------------------------------------//
    const Direct3D9Texture* Direct3D9GeometryBuffer::getActiveTexture() const
    {
        return static_cast<const Direct3D9Texture*>(d_renderMaterial->getMainTexture());
    }

    //----------------------------------------------------------------------------//
    unsigned int Direct3D9GeometryBuffer::getVertexCount() const
    {
        return d_vertices.size();
    }

    //----------------------------------------------------------------------------//
    unsigned int Direct3D9GeometryBuffer::getBatchCount() const
    {
        return d_batches.size();
    }

    //----------------------------------------------------------------------------//
    void Direct3D9GeometryBuffer::setRenderEffect(RenderEffect* effect)
    {
        d_effect = effect;
    }

    //----------------------------------------------------------------------------//
    RenderEffect* Direct3D9GeometryBuffer::getRenderEffect()
    {
        return d_effect;
    }

    //----------------------------------------------------------------------------//
    void Direct3D9GeometryBuffer::performBatchManagement() const
    {
        auto texture = getActiveTexture();

        const LPDIRECT3DTEXTURE9 t = texture ? texture->getDirect3D9Texture() : 0;

        // create a new batch if there are no batches yet, or if the active texture
        // differs from that used by the current batch.
        if (d_batches.empty() || t != d_batches.back().texture || d_clippingActive != d_batches.back().clip)
        {
            BatchInfo batch = {t, 0, d_clippingActive};
            d_batches.push_back(batch);
        }
    }

    //----------------------------------------------------------------------------//
    void Direct3D9GeometryBuffer::updateMatrix() const
    {
        const D3DXVECTOR3 p(d_pivot.x, d_pivot.y, d_pivot.z);
        const D3DXVECTOR3 t(d_translation.x, d_translation.y, d_translation.z);

        D3DXQUATERNION r;
        r.x = d_rotation.x;
        r.y = d_rotation.y;
        r.z = d_rotation.z;
        r.w = d_rotation.w;

        D3DXMatrixTransformation(&d_matrix, 0, 0, 0, &p, &r, &t);

        d_matrixValid = true;
    }

    //----------------------------------------------------------------------------//
    const D3DXMATRIX* Direct3D9GeometryBuffer::getMatrix() const
    {
        if (!d_matrixValid)
            updateMatrix();

        return &d_matrix;
    }

    //----------------------------------------------------------------------------//
    void Direct3D9GeometryBuffer::setClippingActive(const bool active)
    {
        d_clippingActive = active;
    }

    //----------------------------------------------------------------------------//
    bool Direct3D9GeometryBuffer::isClippingActive() const
    {
        return d_clippingActive;
    }

    //----------------------------------------------------------------------------//

}            // namespace CEGUI
