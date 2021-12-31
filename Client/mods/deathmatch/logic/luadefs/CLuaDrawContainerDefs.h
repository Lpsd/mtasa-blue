/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once
#include "CLuaDefs.h"
#include "CClientDrawContainer.h"

class CLuaDrawContainerDefs : public CLuaDefs
{
public:
    static void LoadFunctions();

    static void AddClass(lua_State* luaVM);
    static void AddRectClass(lua_State* luaVM);
    static void AddLine3DClass(lua_State* luaVM);

    static CClientDrawContainer* DxCreateContainer(lua_State* luaVM, CVector2D position, CVector2D size);
    static CClientDrawContainer* DxCreateRect(lua_State* luaVM, CVector2D position, CVector2D size, SColor color = 0xFFFFFFFF);
    static CClientDrawContainer* DxCreateLine3D(lua_State* luaVM, CVector startPosition, CVector endPosition, float width = 1, SColor color = 0xFFFFFFFF);

    static CVector2D DxGetContainerSize(CClientDrawContainer* pContainer);
    static void      DxSetContainerSize(CClientDrawContainer* pContainer, CVector2D size);

private:
    static CClientDrawContainer* CreateFromType(DrawContainerType type);
};
