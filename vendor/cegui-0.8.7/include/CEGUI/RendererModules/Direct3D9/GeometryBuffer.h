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
#ifndef _CEGUIDirect3D9GeometryBuffer_h_
#define _CEGUIDirect3D9GeometryBuffer_h_

#include "../../GeometryBuffer.h"
#include "CEGUI/RendererModules/Direct3D9/Renderer.h"
#include "../../Rectf.h"
#include "../../Quaternion.h"
#include <d3dx9.h>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

#if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
    class Direct3D9Texture;

    /*!
    \brief
        Direct3D9 based implementation of the GeometryBuffer interface.
    */
    class DIRECT3D9_GUIRENDERER_API Direct3D9GeometryBuffer : public GeometryBuffer
    {
    public:
        //! Constructor
        Direct3D9GeometryBuffer(Direct3D9Renderer& owner, LPDIRECT3DDEVICE9 device, CEGUI::RefCounted<RenderMaterial> renderMaterial);

        //! return pointer to D3DXMATRIX used as world transform.
        const D3DXMATRIX* getMatrix() const;

        // implementation of abstract members from GeometryBuffer
        void                    draw(uint32_t drawModeMask = DrawModeMaskAll) const override;

    protected:
        void                    updateVertexBuffer();
        void                    onGeometryChanged() override;
        void                    setTranslation(const glm::vec3& t);
        void                    setRotation(const glm::quat& r);
        void                    setPivot(const glm::vec3& p);
        void                    setClippingRegion(const Rectf& region);
        const Direct3D9Texture* getActiveTexture() const;
        unsigned int            getVertexCount() const;
        unsigned int            getBatchCount() const;
        void                    setRenderEffect(RenderEffect* effect);
        RenderEffect*           getRenderEffect();
        void                    setClippingActive(const bool active);
        bool                    isClippingActive() const;

        //! perform batch management operations prior to adding new geometry.
        void performBatchManagement() const;
        //! update cached matrix
        void updateMatrix() const;
        //! Allocates a hardware vertex buffer of size 'vertexSize' (size in bytes).
        void allocateVertexBuffer(const unsigned int vertexSize) const;
        //! cleanup the hardware vertex buffer.
        void cleanupVertexBuffer() const;

        //! internal Vertex structure used for Direct3D based geometry.
        struct D3DVertex
        {
            //! The transformed position for the vertex.
            FLOAT x, y, z;
            //! colour of the vertex.
            DWORD diffuse;
            //! texture coordinates.
            float tu, tv;
        };

        //! type to track info for per-texture sub batches of geometry
        struct BatchInfo
        {
            LPDIRECT3DTEXTURE9 texture;
            unsigned int       vertexCount;
            bool               clip;
        };

        //! Owning Direct3D9Renderer object
        Direct3D9Renderer& d_owner;
        //! last texture that was set as active
        const Direct3D9Texture* d_activeTexture;
        //! type of container that tracks BatchInfos.
        typedef std::vector<BatchInfo> BatchList;
        //! list of texture batches added to the geometry buffer
        mutable BatchList d_batches;
        //! type of container used to queue the geometry
        typedef std::vector<D3DVertex> VertexList;
        //! container where added geometry is stored.
        mutable VertexList d_vertices;
        //! rectangular clip region
        Rectf d_clipRect;
        //! whether clipping will be active for the current batch
        bool d_clippingActive;
        //! translation vector
        glm::vec3 d_translation;
        //! rotation vector
        glm::quat d_rotation;
        //! pivot point for rotation
        glm::vec3 d_pivot;
        //! RenderEffect that will be used by the GeometryBuffer
        RenderEffect* d_effect;
        //! The D3D Device
        IDirect3DDevice9* d_device;
        //! model matrix cache
        mutable D3DXMATRIX d_matrix;
        //! true when d_matrix is valid and up to date
        mutable bool d_matrixValid;
        mutable IDirect3DVertexBuffer9* d_vertexBuffer = nullptr;
        mutable unsigned int            d_vertexBufferSize = 0;
    };

}            // namespace CEGUI

#if defined(_MSC_VER)
    #pragma warning(pop)
#endif

#endif            // end of guard _CEGUIDirect3D9GeometryBuffer_h_
