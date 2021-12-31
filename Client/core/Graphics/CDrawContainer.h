/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include <CVector.h>
#include <CVector2D.h>

enum class DrawContainerType
{
    DEFAULT,
    RECT,
    LINE3D,
};

const std::array<DrawContainerType, 3> DrawContainerTypes = {
    DrawContainerType::DEFAULT,
    DrawContainerType::RECT,
    DrawContainerType::LINE3D,
};

class CDrawContainer
{
public:
    CDrawContainer();
    virtual ~CDrawContainer();

    virtual void OnPulse();

    void              SetType(DrawContainerType type);
    DrawContainerType GetType() const { return m_type; }

    void    SetPosition(CVector position) { m_position = position; }
    CVector GetPosition() const { return m_position; }

    void      SetSize(CVector2D size) { m_size = size; }
    CVector2D GetSize() const { return m_size; }

    void SetColor(SColor color) { m_color = color; }
    void SetColor(int r, int g, int b, int a) { m_color = SColorRGBA(r, g, b, a); }

    SColor GetColor() const { return m_color; }

protected:
    CVector   m_position = {};
    CVector2D m_size = {};
    SColor    m_color = 0xFFFFFFFF;

private:
    DrawContainerType m_type = DrawContainerType::DEFAULT;
};
