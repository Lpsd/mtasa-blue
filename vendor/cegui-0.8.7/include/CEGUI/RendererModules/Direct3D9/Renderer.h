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
#ifndef _CEGUIDirect3D9Renderer_h_
#define _CEGUIDirect3D9Renderer_h_

#include "CEGUI/Base.h"
#include "CEGUI/Renderer.h"
#include "CEGUI/Sizef.h"
#include "CEGUI/UVector.h"

#include <d3d9.h>
#include <vector>
#include <unordered_map>

#if (defined(__WIN32__) || defined(_WIN32)) && !defined(CEGUI_STATIC)
    #ifdef CEGUIDIRECT3D9RENDERER_EXPORTS
        #define DIRECT3D9_GUIRENDERER_API __declspec(dllexport)
    #else
        #define DIRECT3D9_GUIRENDERER_API __declspec(dllimport)
    #endif
#else
    #define DIRECT3D9_GUIRENDERER_API
#endif

#if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
    class Direct3D9Texture;
    class Direct3D9GeometryBuffer;
    class Direct3D9ShaderWrapper;

    /*!
    \brief
        Renderer class to interface with Direct3D 9.
    */
    class DIRECT3D9_GUIRENDERER_API Direct3D9Renderer : public Renderer
    {
    public:
        /*!
        \brief
            Convenience function that creates the required objects to initialise the
            CEGUI system.

            This will create and initialise the following objects for you:
            - CEGUI::Direct3D9Renderer
            - CEGUI::DefaultResourceProvider
            - CEGUI::System

        \param device
            LPDIRECT3DDEVICE9 of the device that is to be used for CEGUI
            rendering operations.

        \param abi
            This must be set to CEGUI_VERSION_ABI

        \return
            Reference to the CEGUI::Direct3D9Renderer object that was created.
        */
        static Direct3D9Renderer& bootstrapSystem(LPDIRECT3DDEVICE9 device, const int abi = CEGUI_VERSION_ABI, std::string moduleDir = "");

        /*!
        \brief
            Convenience function to cleanup the CEGUI system and related objects
            that were created by calling the bootstrapSystem function.

            This function will destroy the following objects for you:
            - CEGUI::System
            - CEGUI::DefaultResourceProvider
            - CEGUI::Direct3D9Renderer

        \note
            If you did not initialise CEGUI by calling the bootstrapSystem function,
            you should \e not call this, but rather delete any objects you created
            manually.
        */
        static void destroySystem();

        /*!
        \brief
            Create an Direct3D9Renderer object.
        */
        static Direct3D9Renderer& create(LPDIRECT3DDEVICE9 device, const int abi = CEGUI_VERSION_ABI);

        /*!
        \brief
            Destroy an Direct3D9Renderer object.

        \param renderer
            The Direct3D9Renderer object to be destroyed.
        */
        static void destroy(Direct3D9Renderer& renderer);

        /*!
        \brief
            Returns if the texture coordinate system is vertically flipped or not. The original of a
            texture coordinate system is typically located either at the the top-left or the bottom-left.
            CEGUI, Direct3D and most rendering engines assume it to be on the top-left. OpenGL assumes it to
            be at the bottom left.

            This function is intended to be used when generating geometry for rendering the TextureTarget
            onto another surface. It is also intended to be used when trying to use a custom texture (RTT)
            inside CEGUI using the Image class, in order to determine the Image coordinates correctly.

        \return
            - true if flipping is required: the texture coordinate origin is at the bottom left
            - false if flipping is not required: the texture coordinate origin is at the top left
        */
        bool isTexCoordSystemFlipped() const { return false; }

        //! support function to be called prior to a Reset on the Direct3DDevice9.
        void preD3DReset();

        //! support function to be called after a Reset on the Direct3DDevice9.
        void postD3DReset();

        //! return the Direct3D 9 Device interface used by this renderer object.
        LPDIRECT3DDEVICE9 getDevice() const;

        //! create a CEGUI::texture from an existing D3D texture
        Texture& createTexture(const String& name, LPDIRECT3DTEXTURE9 tex);

        //! return true if we can use non square textures.
        bool supportsNonSquareTexture();

        //! return true if we can use NPOT texture dimensions.
        bool supportsNPOTTextures();

        //! returns Size object from \a sz adjusted for hardware capabilities.
        Sizef getAdjustedSize(const Sizef& sz);

        //! set the render states for the specified BlendMode.
        void setupRenderingBlendMode(const BlendMode mode, const bool force = false);

        // Implementation of Renderer interface.
        virtual RenderTarget&              getDefaultRenderTarget();
        virtual RefCounted<RenderMaterial> createRenderMaterial(const DefaultShaderType shaderType) const;
        virtual GeometryBuffer&            createGeometryBufferColoured(RefCounted<RenderMaterial> renderMaterial);
        virtual GeometryBuffer&            createGeometryBufferTextured(RefCounted<RenderMaterial> renderMaterial);
        virtual void                       destroyGeometryBuffer(const GeometryBuffer& buffer);
        virtual void                       destroyAllGeometryBuffers();
        virtual TextureTarget*             createTextureTarget(bool addStencilBuffer);
        virtual void                       destroyTextureTarget(TextureTarget* target);
        virtual void                       destroyAllTextureTargets();
        virtual Texture&                   createTexture(const CEGUI::String& name);
        virtual Texture&                   createTexture(const CEGUI::String& name, const String& filename, const String& resourceGroup);
        virtual Texture&                   createTexture(const CEGUI::String& name, const Sizef& size);
        virtual void                       destroyTexture(Texture& texture);
        virtual void                       destroyTexture(const CEGUI::String& name);
        virtual void                       destroyAllTextures();
        virtual Texture&                   getTexture(const String&) const;
        virtual bool                       isTextureDefined(const String& name) const;
        virtual void                       beginRendering();
        virtual void                       endRendering();
        virtual void                       setDisplaySize(const Sizef& sz);
        virtual const Sizef&               getDisplaySize() const;
        virtual unsigned int               getMaxTextureSize() const;
        virtual const String&              getIdentifierString() const;
        const glm::vec2&                   getDisplayDPI() const;

    private:
        //! Constructor for Direct3D9 Renderer objects.
        Direct3D9Renderer(LPDIRECT3DDEVICE9 device);

        //! Destructor for Direct3D9Renderer objects.
        virtual ~Direct3D9Renderer();

        //! helper to throw exception if name is already used.
        void throwIfNameExists(const String& name) const;
        //! helper to safely log the creation of a named texture
        static void logTextureCreation(const String& name);
        //! helper to safely log the destruction of a named texture
        static void logTextureDestruction(const String& name);

        //! return size of device view port (if possible).
        Sizef getViewportSize();
        //! returns next power of 2 size if \a size is not power of 2
        float getSizeNextPOT(float sz) const;

        //! Initialises the ShaderManager and the required D3D9 shaders
        void initialiseShaders();
        //! Initialises the D3D9 ShaderWrapper for textured objects
        void initialiseStandardTexturedShaderWrapper();
        //! Initialises the D3D9 ShaderWrapper for coloured objects
        void initialiseStandardColouredShaderWrapper();
        //! Wrapper of the OpenGL shader we will use for textured geometry
        Direct3D9ShaderWrapper* d_shaderWrapperTextured;
        //! Wrapper of the OpenGL shader we will use for solid geometry
        Direct3D9ShaderWrapper* d_shaderWrapperSolid;

        //! String holding the renderer identification text.
        static String d_rendererID;
        //! Direct3DDevice9 interface we were given when constructed.
        LPDIRECT3DDEVICE9 d_device;
        //! What the renderer considers to be the current display size.
        Sizef d_displaySize;
        //! What the renderer considers to be the current display DPI resolution.
        glm::vec2 d_displayDPI;
        //! The default RenderTarget
        RenderTarget* d_defaultTarget;
        //! container type used to hold TextureTargets we create.
        typedef std::vector<TextureTarget*> TextureTargetList;
        //! Container used to track texture targets.
        TextureTargetList d_textureTargets;
        //! container type used to hold GeometryBuffers we create.
        typedef std::vector<Direct3D9GeometryBuffer*> GeometryBufferList;
        //! Container used to track geometry buffers.
        GeometryBufferList d_geometryBuffers;
        //! container type used to hold Textures we create.
        typedef std::unordered_map<String, Direct3D9Texture*> TextureMap;
        //! Container used to track textures.
        TextureMap d_textures;
        //! What the renderer thinks the max texture size is.
        unsigned int d_maxTextureSize;
        //! whether the hardware supports non-power of two textures
        bool d_supportNPOTTex;
        //! whether the hardware supports non-square textures.
        bool d_supportNonSquareTex;
        //! What we think is the active blendine mode
        BlendMode d_activeBlendMode;
    };

}            // namespace CEGUI

#if defined(_MSC_VER)
    #pragma warning(pop)
#endif

#endif            // end of guard _CEGUIDirect3D9Renderer_h_
