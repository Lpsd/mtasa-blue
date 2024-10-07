/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        gui/CGUIFont_Impl.cpp
 *  PURPOSE:     Font type class
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"

CGUIFont_Impl::CGUIFont_Impl(CGUI_Impl* pGUI, const char* szFontName, const char* szFontFile, unsigned int uSize, unsigned int uFlags, bool bAutoScale,
                             bool isWinFont)
{
    // Store the fontmanager and create a font with the given attributes
    m_pFontManager = pGUI->GetFontManager();
    m_pFont = NULL;

    // Store the renderer's geometry buffer
    m_pGeometryBuffer = pGUI->GetGeometryBuffer();

    CEGUI::AutoScaledMode scaleMode = bAutoScale ? CEGUI::AutoScaledMode::Both : CEGUI::AutoScaledMode::Disabled;

    while (!m_pFont)
    {
        try
        {
            m_pFont = &m_pFontManager->createFreeTypeFont(szFontName, uSize, CEGUI::FontSizeUnit::Pixels, true, szFontFile,
                                                          isWinFont ? "absolute" : "mta_fonts", scaleMode, CEGUI::Sizef(1024, 768));
        }
        catch (CEGUI::RendererException)
        {
            // Reduce size until it can fit into a texture
            if (--uSize == 1)
                throw;
        }
    }

    // Define our glyphs (no longer required)
    // m_pFont->setInitialFontGlyphs(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");

    // Set default attributes
    SetNativeResolution(1024, 768);
    SetAutoScalingEnabled(bAutoScale);
}

CGUIFont_Impl::~CGUIFont_Impl()
{
    m_pFontManager->destroy(*m_pFont);
}

void CGUIFont_Impl::DrawTextString(const char* szText, CRect2D DrawArea, float fZ, CRect2D ClipRect, unsigned long ulFormat, unsigned long ulColor,
                                   float fScaleX, float fScaleY)
{
    m_pRenderedText->renderText(szText);
    m_pRenderedText->createRenderGeometry(std::vector{m_pGeometryBuffer}, glm::vec2(DrawArea.fX1, DrawArea.fY1),
                                          &CEGUI::ColourRect(static_cast<CEGUI::argb_t>(ulColor)),
                                          &CEGUI::Rectf(glm::vec2(ClipRect.fX1, ClipRect.fY1), glm::vec2(ClipRect.fX2, ClipRect.fY2)));
}

void CGUIFont_Impl::SetAntiAliasingEnabled(bool bAntialiased)
{
    // m_pFont->setAntiAliased(bAntialiased);
}

bool CGUIFont_Impl::IsAntiAliasingEnabled()
{
    // return m_pFont->isAntiAliased();
    return false;
}

void CGUIFont_Impl::SetAutoScalingEnabled(bool bAutoScaled)
{
    CEGUI::AutoScaledMode scaleMode = bAutoScaled ? CEGUI::AutoScaledMode::Both : CEGUI::AutoScaledMode::Disabled;
    m_pFont->setAutoScaled(scaleMode);
}

bool CGUIFont_Impl::IsAutoScalingEnabled()
{
    CEGUI::AutoScaledMode scaledMode = m_pFont->getAutoScaled();
    return (scaledMode == CEGUI::AutoScaledMode::Disabled) ? false : true;
}

void CGUIFont_Impl::SetNativeResolution(int iX, int iY)
{
    m_pFont->setNativeResolution(CEGUI::Sizef(static_cast<float>(iX), static_cast<float>(iY)));
}

float CGUIFont_Impl::GetCharacterWidth(int iChar, float fScale)
{
    char szBuf[2];
    szBuf[0] = iChar;
    szBuf[1] = 0;

    return m_pFont->getTextExtent(szBuf);
}

float CGUIFont_Impl::GetFontHeight(float fScale)
{
    float fHeight = m_pFont->getFontHeight();            // average height.. not the maximum height for long characters such as 'g' or 'j'
    fHeight += 2.0f;                                           // so hack it

    return fHeight;
}

float CGUIFont_Impl::GetTextExtent(const char* szText, float fScale)
{
    return m_pFont->getTextExtent(CGUI_Impl::GetUTFString(szText));
}

CEGUI::Font* CGUIFont_Impl::GetFont()
{
    return m_pFont;
}
