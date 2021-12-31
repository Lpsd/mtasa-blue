/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"
#include "lua/CLuaFunctionParser.h"
#include "CLuaDrawContainerDefs.h"

void CLuaDrawContainerDefs::LoadFunctions()
{
    constexpr static const std::pair<const char*, lua_CFunction> functions[]{
        {"dxCreateContainer", ArgumentParser<DxCreateContainer>},
        {"dxCreateRect", ArgumentParser<DxCreateRect>},
        {"dxCreateLine3D", ArgumentParser<DxCreateLine3D>},
        {"dxGetContainerSize", ArgumentParser<DxGetContainerSize>},
        {"dxSetContainerSize", ArgumentParser<DxSetContainerSize>},
    };

    // Add functions
    for (const auto& [name, func] : functions)
        CLuaCFunctions::AddFunction(name, func);
}

void CLuaDrawContainerDefs::AddClass(lua_State* luaVM)
{
    // Create main container class
    lua_newclass(luaVM);

    lua_classfunction(luaVM, "create", "dxCreateContainer");

    lua_classfunction(luaVM, "getSize", "dxGetContainerSize");
    lua_classfunction(luaVM, "setSize", "dxSetContainerSize");

    lua_classvariable(luaVM, "size", "dxSetContainerSize", "dxGetContainerSize");

    lua_registerclass(luaVM, "DxContainer", "Element");

    // Add other clases
    AddRectClass(luaVM);
    AddLine3DClass(luaVM);
}

void CLuaDrawContainerDefs::AddRectClass(lua_State* luaVM)
{
    lua_newclass(luaVM);

    lua_classfunction(luaVM, "create", "dxCreateRect");

    lua_registerclass(luaVM, "DxRect", "DxContainer");
}

void CLuaDrawContainerDefs::AddLine3DClass(lua_State* luaVM)
{
    lua_newclass(luaVM);

    lua_classfunction(luaVM, "create", "dxCreateLine3D");

    lua_registerclass(luaVM, "DxLine3D", "DxContainer");
}

CClientDrawContainer* CLuaDrawContainerDefs::CreateFromType(DrawContainerType type)
{
    auto pInterface = m_pManager->GetDrawContainerManager();

    if (!pInterface)
        return nullptr;

    return pInterface->Create(type);
}

CClientDrawContainer* CLuaDrawContainerDefs::DxCreateContainer(lua_State* luaVM, CVector2D position, CVector2D size)
{
    CLuaMain&             pLuaMain = lua_getownercluamain(luaVM);
    CResource*            pParentResource = pLuaMain.GetResource();

    CClientDrawContainer* pElement = CreateFromType(DrawContainerType::DEFAULT);

    if (!pParentResource || !pElement)
        return nullptr;

    pElement->SetPosition({position.fX, position.fY, 0});
    pElement->SetSize(size);

    pElement->SetParent(pParentResource->GetResourceDynamicEntity());
    pElement->SetResource(pParentResource);

    return pElement;
}

CClientDrawContainer* CLuaDrawContainerDefs::DxCreateRect(lua_State* luaVM, CVector2D position, CVector2D size, SColor color)
{
    CLuaMain&             pLuaMain = lua_getownercluamain(luaVM);
    CResource*            pParentResource = pLuaMain.GetResource();

    CClientDrawContainer* pElement = CreateFromType(DrawContainerType::RECT);

    if (!pParentResource || !pElement)
        return nullptr;

    pElement->SetPosition({position.fX, position.fY, 0});
    pElement->SetSize(size);

    CDrawContainerRect* pContainer = reinterpret_cast<CDrawContainerRect*>(pElement->GetDrawContainer());

    if (!pContainer)
    {
        auto pDeleter = m_pClientGame->GetElementDeleter();

        if (pDeleter)
            pDeleter->Delete(pElement);

        return nullptr;
    }

    pContainer->SetColor(color);

    pElement->SetParent(pParentResource->GetResourceDynamicEntity());
    pElement->SetResource(pParentResource);

    return pElement;
}

CClientDrawContainer* CLuaDrawContainerDefs::DxCreateLine3D(lua_State* luaVM, CVector startPosition, CVector endPosition, float width, SColor color)
{
    CLuaMain&             pLuaMain = lua_getownercluamain(luaVM);
    CResource*            pParentResource = pLuaMain.GetResource();

    CClientDrawContainer* pElement = CreateFromType(DrawContainerType::LINE3D);

    if (!pParentResource || !pElement)
        return nullptr;

    pElement->SetPosition(startPosition);
    pElement->SetSize({width, 0});

    CDrawContainerLine3D* pContainer = reinterpret_cast<CDrawContainerLine3D*>(pElement->GetDrawContainer());

    if (!pContainer)
    {
        auto pDeleter = m_pClientGame->GetElementDeleter();

        if (pDeleter)
            pDeleter->Delete(pElement);

        return nullptr;
    }

    pContainer->SetEndPosition(endPosition);
    pContainer->SetColor(color);

    pElement->SetParent(pParentResource->GetResourceDynamicEntity());
    pElement->SetResource(pParentResource);

    return pElement;
}

CVector2D CLuaDrawContainerDefs::DxGetContainerSize(CClientDrawContainer* pContainer)
{
    if (!pContainer)
        return {};

    return pContainer->GetSize();
}

void CLuaDrawContainerDefs::DxSetContainerSize(CClientDrawContainer* pContainer, CVector2D size)
{
    if (!pContainer)
        return;

    pContainer->SetSize(size);
}
