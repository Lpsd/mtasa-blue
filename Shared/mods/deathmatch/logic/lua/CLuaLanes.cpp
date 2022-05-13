/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"
#include "CLuaLanes.h"
#include "lanes.h"

/************************************/
/* CLuaLanes - per-resource manager */
/************************************/

CLuaLanes::CLuaLanes(lua_State* luaVM)
{
}

CLuaLanes::~CLuaLanes()
{
}

CLuaLane* CLuaLanes::CreateLane()
{
    CLuaLane* lane = new CLuaLane(this);
    m_lanes.push_back(lane);
    return lane;
}

/*******************************/
/* CLuaLane - individual lanes */
/*******************************/

CLuaLane::CLuaLane(CLuaLanes* laneManager)
{
    m_laneManager = laneManager;

    EmbedLanes();
}

CLuaLane::~CLuaLane()
{
}

int CLuaLane::RunLanes(lua_State* luaVM)
{
    return luaL_dofile(luaVM, CalcMTASAPath("MTA/lanes.lua").data());
}

void CLuaLane::EmbedLanes()
{
    if (!m_laneManager)
        return;

    lua_State* luaVM = m_laneManager->GetLuaVM();

    luaL_openlibs(luaVM);
    luaopen_lanes_embedded(luaVM, RunLanes);
    lua_pop(luaVM, 1);
}
