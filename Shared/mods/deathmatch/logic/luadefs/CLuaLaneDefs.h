/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once
#include "luadefs/CLuaDefs.h"

class CLuaLaneDefs : public CLuaDefs
{
public:
    static void LoadFunctions();
    
private:
    static std::variant<int, bool> ThreadCreate(lua_State* luaVM);
    static bool                    ThreadDestroy(lua_State* luaVM, int threadID);

    static std::string ThreadRun(lua_State* luaVM, int threadID, std::string code);
};
