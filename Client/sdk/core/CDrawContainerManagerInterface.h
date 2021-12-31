/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once 
#include <array>

#include <../Client/core/Graphics/CDrawContainer.h>
#include <../Client/core/Graphics/CDrawContainer.Rect.h>
#include <../Client/core/Graphics/CDrawContainer.Line3D.h>

class CDrawContainerManagerInterface
{
public:
    virtual ~CDrawContainerManagerInterface() {}

    virtual CDrawContainer* Create(DrawContainerType type) = 0;
};
