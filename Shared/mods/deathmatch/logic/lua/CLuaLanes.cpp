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
#include "lua/CLuaMain.h"
#include "lua/CLuaManager.h"

extern "C"
{
#include "lanes.h"
}

/************************************/
/* CLuaLanes - per-resource manager */
/************************************/

CLuaLanes::CLuaLanes(CResource* resource)
{
    CLuaManager* manager = resource->GetLuaManager();
    m_luaMain = manager->CreateVirtualMachine(resource, resource->IsOOPEnabled());
}

CLuaLanes::~CLuaLanes()
{
}

lua_State* CLuaLanes::GetLuaVM()
{
    return m_luaMain->GetVM();
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
    return luaL_dofile(luaVM, "mta/lanes.lua");
}

void CLuaLane::EmbedLanes()
{
    if (!m_laneManager)
        return;

    lua_State* luaVM = m_laneManager->GetLuaVM();

    if (!luaVM)
        return;

    luaL_openlibs(luaVM);
    luaopen_lanes_embedded(luaVM, RunLanes, (void*)m_laneManager->GetLuaMain());
    lua_pop(luaVM, 1);

    // Test
    luaL_dostring(luaVM, "return 'test from lanes'");
    const char* str = lua_tostring(luaVM, -1);
}
