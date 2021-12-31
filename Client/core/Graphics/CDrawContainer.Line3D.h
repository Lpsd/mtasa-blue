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

class CDrawContainerLine3D final : public CDrawContainer
{
public:
    CDrawContainerLine3D();
    ~CDrawContainerLine3D();

    virtual void OnPulse() override;

    void    SetEndPosition(CVector position) { m_endPosition = position; }
    CVector GetEndPosition() const { return m_endPosition; }

private:
    CVector m_endPosition = {};
};
