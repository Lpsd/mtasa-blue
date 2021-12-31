/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"

CDrawContainerManager::CDrawContainerManager()
{

}

CDrawContainerManager::~CDrawContainerManager()
{

}

CDrawContainer* CDrawContainerManager::Create(DrawContainerType type)
{
    switch (type)
    {
        case DrawContainerType::DEFAULT:
            return new CDrawContainer();
        case DrawContainerType::RECT:
            return new CDrawContainerRect();
        case DrawContainerType::LINE3D:
            return new CDrawContainerLine3D();
    }

    return nullptr;
}
