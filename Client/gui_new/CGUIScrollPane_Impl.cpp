/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        gui/CGUIScrollPane_Impl.cpp
 *  PURPOSE:     Scroll pane widget class
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"

#define CGUISCROLLPANE_NAME "ScrollablePane"

using namespace GUINew;

CGUIScrollPane_Impl::CGUIScrollPane_Impl(CGUI_Impl* pGUI, CGUIElement* pParent)
{
    m_pManager = pGUI;

    // Initialize
    m_pGUI = pGUI;

    // Get an unique identifier for CEGUI (gah, there's gotta be an another way)
    char szUnique[CGUI_CHAR_SIZE];
    pGUI->GetUniqueName(szUnique);

    // Create the window and set default settings
    m_pWindow = pGUI->GetWindowManager()->createWindow(pGUI->GetElementPrefix() + "/" + CGUISCROLLPANE_NAME, szUnique);
    m_pWindow->setDestroyedByParent(false);
    m_pWindow->setArea(CEGUI::URect(CEGUI::UDim(0.9f, 0), CEGUI::UDim(0.9f, 0), CEGUI::UDim(0.9f, 0), CEGUI::UDim(0.9f, 0)));

    // Store the pointer to this CGUI element in the CEGUI element
    m_pWindow->setUserData(reinterpret_cast<void*>(this));
    AddEvents();

    // If a parent is specified, add it to it's children list, if not, add it as a child to the m_pManager
    if (pParent)
    {
        SetParent(pParent);
    }
    else
    {
        pGUI->AddChild(this);
        SetParent(NULL);
    }
    SetHorizontalScrollStepSize(100.0f);
    SetVerticalScrollStepSize(100.0f);
}

CGUIScrollPane_Impl::~CGUIScrollPane_Impl()
{
    DestroyElement();
}

void CGUIScrollPane_Impl::SetHorizontalScrollBar(bool bEnabled)
{
    reinterpret_cast<CEGUI::ScrollablePane*>(m_pWindow)->setAlwaysShowHorizontalScrollbar(bEnabled);
}

void CGUIScrollPane_Impl::SetVerticalScrollBar(bool bEnabled)
{
    reinterpret_cast<CEGUI::ScrollablePane*>(m_pWindow)->setAlwaysShowVerticalScrollbar(bEnabled);
}

void CGUIScrollPane_Impl::SetHorizontalScrollPosition(float fPosition)
{
    try
    {
        CEGUI::ScrollablePane* pScrollPane = reinterpret_cast<CEGUI::ScrollablePane*>(m_pWindow);
        float                  fFullWidth = pScrollPane->getContentPane()->getWidth().d_offset;
        float                  fViewWidth = pScrollPane->getWidth().d_offset;

        pScrollPane->setHorizontalScrollPosition(fPosition * ((fFullWidth - fViewWidth) / fFullWidth));
    }
    catch (CEGUI::Exception)
    {
    }
}

void CGUIScrollPane_Impl::SetVerticalScrollPosition(float fPosition)
{
    try
    {
        CEGUI::ScrollablePane* pScrollPane = reinterpret_cast<CEGUI::ScrollablePane*>(m_pWindow);
        float                  fFullHeight = pScrollPane->getContentPane()->getHeight().d_offset;
        float                  fViewHeight = pScrollPane->getHeight().d_offset;

        pScrollPane->setVerticalScrollPosition(fPosition * ((fFullHeight - fViewHeight) / fFullHeight));
    }
    catch (CEGUI::Exception)
    {
    }
}

float CGUIScrollPane_Impl::GetHorizontalScrollPosition()
{
    try
    {
        CEGUI::ScrollablePane* pScrollPane = reinterpret_cast<CEGUI::ScrollablePane*>(m_pWindow);

        float fFullWidth = pScrollPane->getContentPane()->getWidth().d_offset;
        float fViewWidth = pScrollPane->getWidth().d_offset;

        // Prevent division by zero
        if (fFullWidth == fViewWidth)
            return 0.0f;

        return (pScrollPane->getHorizontalScrollPosition() / ((fFullWidth - fViewWidth) / fFullWidth));
    }
    catch (CEGUI::Exception)
    {
        return 0;
    }
}

float CGUIScrollPane_Impl::GetVerticalScrollPosition()
{
    try
    {
        CEGUI::ScrollablePane* pScrollPane = reinterpret_cast<CEGUI::ScrollablePane*>(m_pWindow);

        float fFullHeight = pScrollPane->getContentPane()->getHeight().d_offset;
        float fViewHeight = pScrollPane->getHeight().d_offset;

        // Prevent division by zero
        if (fFullHeight == fViewHeight)
            return 0.0f;

        return (pScrollPane->getVerticalScrollPosition() / ((fFullHeight - fViewHeight) / fFullHeight));
    }
    catch (CEGUI::Exception)
    {
        return 0;
    }
}

void CGUIScrollPane_Impl::SetHorizontalScrollStepSize(float fPosition)
{
    try
    {
        reinterpret_cast<CEGUI::ScrollablePane*>(m_pWindow)->setHorizontalStepSize(fPosition);
    }
    catch (CEGUI::Exception)
    {
    }
}

void CGUIScrollPane_Impl::SetVerticalScrollStepSize(float fPosition)
{
    try
    {
        reinterpret_cast<CEGUI::ScrollablePane*>(m_pWindow)->setVerticalStepSize(fPosition);
    }
    catch (CEGUI::Exception)
    {
    }
}

float CGUIScrollPane_Impl::GetHorizontalScrollStepSize()
{
    try
    {
        return reinterpret_cast<CEGUI::ScrollablePane*>(m_pWindow)->getHorizontalStepSize();
    }
    catch (CEGUI::Exception)
    {
        return 0;
    }
}

float CGUIScrollPane_Impl::GetVerticalScrollStepSize()
{
    try
    {
        return reinterpret_cast<CEGUI::ScrollablePane*>(m_pWindow)->getVerticalStepSize();
    }
    catch (CEGUI::Exception)
    {
        return 0;
    }
}
