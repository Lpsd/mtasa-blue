/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/


#include "StdInc.h"
#include "CLuaLaneDefs.h"
#include "lua/CLuaFunctionParser.h"

void CLuaLaneDefs::LoadFunctions()
{
    constexpr static const std::pair<const char*, lua_CFunction> functions[]{
        {"threadCreate", ArgumentParser<ThreadCreate>},
        {"threadDestroy", ArgumentParser<ThreadDestroy>},
        {"threadRun", ArgumentParser<ThreadRun>},
    };

    for (const auto& [name, func] : functions)
        CLuaCFunctions::AddFunction(name, func);
}

std::variant<int, bool> CLuaLaneDefs::ThreadCreate(lua_State* luaVM)
{
    CResource& resource{lua_getownerresource(luaVM)};
    CLuaLanes* lanes = resource.GetLuaLanes();

    if (!lanes)
        return false;

    CLuaLane* lane = lanes->CreateLane();

    if (!lane)
        return false;

    return lane->GetID();
}

bool CLuaLaneDefs::ThreadDestroy(lua_State* luaVM, int threadID)
{
    CResource& resource{lua_getownerresource(luaVM)};
    CLuaLanes* lanes = resource.GetLuaLanes();

    if (!lanes)
        return false;

    return lanes->DeleteLane(threadID);
}

std::string CLuaLaneDefs::ThreadRun(lua_State* luaVM, int threadID, std::string code)
{
    CResource& resource{lua_getownerresource(luaVM)};
    CLuaLanes* lanes = resource.GetLuaLanes();

    if (!lanes)
        return "error: no lanes manager";

    CLuaLane* lane = lanes->GetLaneFromID(threadID);

    if (!lane)
        return "error: no lane";

    return lane->RunString(code);
}
