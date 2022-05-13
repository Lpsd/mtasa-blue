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

class CLuaLanes
{
public:
    CLuaLanes(lua_State* luaVM);
    ~CLuaLanes();

    CLuaLane*  CreateLane();
    lua_State* GetLuaVM() { return m_luaVM; };

private:
    lua_State*             m_luaVM;
    std::vector<CLuaLane*> m_lanes = {};
};

class CLuaLane
{
public:
    CLuaLane(CLuaLanes* laneManager);
    ~CLuaLane();

    static int RunLanes(lua_State* luaVM);
    void EmbedLanes();

private:
    CLuaLanes* m_laneManager;
};
