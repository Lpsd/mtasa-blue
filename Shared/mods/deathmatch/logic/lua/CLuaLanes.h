/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include "lua/LuaCommon.h"
#include <vector>

class CLuaMain;
class CLuaLane;

class CLuaLanes
{
public:
    CLuaLanes(CResource* resource);
    ~CLuaLanes();

    CLuaLane* CreateLane();

    CLuaMain*  GetLuaMain() { return m_luaMain; }
    lua_State* GetLuaVM();

private:
    CLuaMain*              m_luaMain;
    std::vector<CLuaLane*> m_lanes = {};
};

class CLuaLane
{
public:
    CLuaLane(CLuaLanes* laneManager);
    ~CLuaLane();

    static int RunLanes(lua_State* luaVM);
    void       EmbedLanes();

private:
    CLuaLanes* m_laneManager;
};
