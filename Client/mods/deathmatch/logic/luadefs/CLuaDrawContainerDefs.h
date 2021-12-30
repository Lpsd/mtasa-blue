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

    static CClientDrawContainer* DxCreateContainer(lua_State* luaVM, CVector2D position, CVector2D size);
    static CClientDrawContainer* DxCreateRect(lua_State* luaVM, CVector2D position, CVector2D size);

    static CVector2D DxGetContainerSize(CClientDrawContainer* pContainer);
    static void      DxSetContainerSize(CClientDrawContainer* pContainer, CVector2D size);

private:
    static CClientDrawContainer* CreateFromType(DrawContainerType type);
};
