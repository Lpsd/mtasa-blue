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

CLuaLanes::CLuaLanes(CResource* resource) : m_resource(resource)
{
}

CLuaLanes::~CLuaLanes()
{
    for (auto lane : m_lanes)
    {
        delete lane;
    }
}

CLuaLane* CLuaLanes::CreateLane()
{
    CLuaLane* lane = new CLuaLane(this, ++m_lifetimeLanes);
    m_lanes.push_back(lane);
    return lane;
}

bool CLuaLanes::DeleteLane(int threadID)
{
    for (auto it = m_lanes.begin(); it != m_lanes.end(); ++it)
    {
        if ((*it)->GetID() == threadID)
        {
            m_lanes.erase(it);
            return true;
        }
    }

    return false;
}

CLuaLane* CLuaLanes::GetLaneFromID(int threadID)
{
    for (auto lane : m_lanes)
    {
        if (lane->GetID() == threadID)
        {
            return lane;
        }
    }

    return nullptr;
}

/*******************************/
/* CLuaLane - individual lanes */
/*******************************/

CLuaLane::CLuaLane(CLuaLanes* laneManager, const int id) : m_id(id)
{
    m_laneManager = laneManager;

    CResource*   resource = m_laneManager->GetResource();
    CLuaManager* manager = resource->GetLuaManager();
    m_luaMain = manager->CreateVirtualMachine(resource, resource->IsOOPEnabled());

    EmbedLanes();
}

CLuaLane::~CLuaLane()
{
    if (m_laneManager)
    {
        CResource* resource = m_laneManager->GetResource();

        if (resource)
        {
            CLuaManager* manager = resource->GetLuaManager();

            if (!manager)
            {
                manager->RemoveVirtualMachine(m_luaMain);
            }
        }
    }
}

int CLuaLane::RunLanes(lua_State* luaVM)
{
    return luaL_dofile(luaVM, "mta/lanes.lua");
}

void CLuaLane::EmbedLanes()
{
    if (!m_laneManager)
        return;

    lua_State* luaVM = GetLuaVM();

    if (!luaVM)
        return;

    luaL_openlibs(luaVM);
    luaopen_lanes_embedded(luaVM, RunLanes, (void*)m_luaMain);
    lua_pop(luaVM, 1);
}

const std::string CLuaLane::RunString(std::string str)
{
    if (!m_laneManager)
        return "error";

    lua_State* luaVM = GetLuaVM();

    if (!luaVM)
        return "error";

    luaL_dostring(luaVM, str.c_str());

    std::string ret{"error"};
    int         type = lua_type(luaVM, -1);

    switch (type)
    {
        case LUA_TNIL:
            ret = "nil";
            break;
        case LUA_TBOOLEAN:
            if (lua_toboolean(luaVM, -1) == 1)
                ret = "true";
            else
                ret = "false";
            break;
        case LUA_TTABLE:
            ret = "table value";
            break;
        case LUA_TSTRING:
            ret = lua_tostring(luaVM, -1);
            break;
        case LUA_TUSERDATA:
            ret = "userdata value";
            break;
        case LUA_TNUMBER:
            if (lua_tonumber(luaVM, -1) == static_cast<int>(lua_tonumber(luaVM, -1)))
                ret = SString("%d", static_cast<int>(lua_tonumber(luaVM, -1)));
            else
                ret = SString("%f", lua_tonumber(luaVM, -1));

            break;
        default:
            return ret;
            break;
    }

    return ret;
}

lua_State* CLuaLane::GetLuaVM()
{
    return m_luaMain->GetVM();
}
