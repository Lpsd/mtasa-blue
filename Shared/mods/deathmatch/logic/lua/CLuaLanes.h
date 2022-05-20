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

class CLuaLane;
class CLuaMain;
class CResource;

class CLuaLanes
{
public:
    CLuaLanes(CResource* resource);
    ~CLuaLanes();

    CLuaLane* CreateLane();
    bool      DeleteLane(int threadID);

    CLuaLane* GetLaneFromID(int threadID);

    CResource* GetResource() { return m_resource; }

private:
    CResource*             m_resource;
    std::vector<CLuaLane*> m_lanes = {};

    int m_lifetimeLanes = 0;
};

class CLuaLane
{
public:
    CLuaLane(CLuaLanes* laneManager, const int id);
    ~CLuaLane();

    static int RunLanes(lua_State* luaVM);
    void       EmbedLanes();

    const std::string RunString(std::string str);

    const int GetID() const { return m_id; }

    CLuaMain*  GetLuaMain() { return m_luaMain; }
    lua_State* GetLuaVM();

private:
    CLuaLanes* m_laneManager;
    CLuaMain*  m_luaMain;
    const int  m_id;
};
