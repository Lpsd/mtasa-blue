/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once
#include "CDrawContainer.h"

class CDrawContainerRect final : public CDrawContainer
{
public:
    CDrawContainerRect();
    ~CDrawContainerRect();

    virtual void OnPulse() override;
};
