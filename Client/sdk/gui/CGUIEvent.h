/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        sdk/gui/CGUIEvent.h
 *  PURPOSE:     Event interface
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include "CGUI.h"
#include "CGUITypes.h"

#include <bitset>

struct CGUIEventArgs
{
    CGUIElement* pWindow;
};

struct CGUIKeyEventArgs
{
    CGUIElement* pWindow;

    unsigned int   codepoint;
    CGUIKeys::Scan scancode;
    std::bitset<7> modifiers;
    unsigned int   sysKeys;
};

struct CGUIMouseEventArgs
{
    CGUIElement* pWindow;

    CGUIPosition           position;
    CVector2D              moveDelta;
    CGUIMouse::MouseButton button;
    std::bitset<7>         modifiers;
    float                  wheelChange;
    unsigned int           sysKeys;
    unsigned int           clickCount;
    CGUIElement*           pSwitchedWindow;
};

struct CGUIFocusEventArgs
{
    CGUIElement* pActivatedWindow;
    CGUIElement* pDeactivatedWindow;
};
