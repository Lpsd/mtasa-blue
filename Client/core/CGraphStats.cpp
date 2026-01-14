/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        core/CGraphStats.cpp
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include <StdInc.h>

namespace
{
    struct SGraphStatLine
    {
        TIMEUS              prevData;
        std::vector<TIMEUS> dataHistory;
        int                 iDataPos;
        SColor              color;
        std::string         strName;
    };
}            // namespace

///////////////////////////////////////////////////////////////
//
// CGraphStats class
//
///////////////////////////////////////////////////////////////
class CGraphStats : public CGraphStatsInterface
{
public:
    ZERO_ON_NEW
    CGraphStats();
    ~CGraphStats();

    // CGraphStatsInterface methods
    virtual void Draw();
    virtual void SetEnabled(bool bEnabled);
    virtual bool IsEnabled();
    virtual void AddTimingPoint(const char* szName);
    virtual void RemoveTimingPoint(const char* szName);

protected:
    bool                              m_bEnabled;
    std::map<SString, SGraphStatLine> m_LineList;
    TIMEUS                            m_StartTime;
    std::vector<PrimitiveVertice>     m_VertexBuffer;            // Pre-allocated buffer for batched line drawing
};

///////////////////////////////////////////////////////////////
//
// CGraphStats instantiation
//
///////////////////////////////////////////////////////////////
CGraphStats* g_pGraphStats = NULL;

CGraphStatsInterface* GetGraphStats()
{
    if (!g_pGraphStats)
        g_pGraphStats = new CGraphStats();
    return g_pGraphStats;
}

///////////////////////////////////////////////////////////////
//
// CGraphStats implementation
//
///////////////////////////////////////////////////////////////
CGraphStats::CGraphStats()
{
}

CGraphStats::~CGraphStats()
{
}

///////////////////////////////////////////////////////////////
//
// CGraphStats::SetEnabled
//
//
//
///////////////////////////////////////////////////////////////
void CGraphStats::SetEnabled(bool bEnabled)
{
    m_bEnabled = bEnabled;
    m_LineList.clear();
    m_StartTime = 0;
}

///////////////////////////////////////////////////////////////
//
// CGraphStats::IsEnabled
//
//
//
///////////////////////////////////////////////////////////////
bool CGraphStats::IsEnabled()
{
    return m_bEnabled;
}

///////////////////////////////////////////////////////////////
//
// CGraphStats::AddTimingPoint
//
//
//
///////////////////////////////////////////////////////////////
void CGraphStats::AddTimingPoint(const char* szName)
{
    if (!IsEnabled())
        return;

    CGraphicsInterface* pGraphics = g_pCore->GetGraphics();

    std::uint32_t viewportWidth = pGraphics->GetViewportWidth();
    std::uint32_t sizeX = viewportWidth / 4; // one quarter of screen width

    // Start of next frame?
    if (szName[0] == 0)
    {
        TIMEUS endTime = GetTimeUs();
        TIMEUS frameTime = endTime - m_StartTime;
        m_StartTime = endTime;

        // Duplicate data points to make up for missed time
        int Dups = frameTime / 33000;
        if (Dups > 0)
        {
            Dups = std::min(100, Dups);
            for (std::map<SString, SGraphStatLine>::iterator iter = m_LineList.begin(); iter != m_LineList.end(); ++iter)
            {
                SGraphStatLine* pLine = &iter->second;

                float Data = pLine->dataHistory[pLine->iDataPos];
                for (int i = 0; i < Dups; i++)
                {
                    pLine->iDataPos++;
                    if (pLine->iDataPos > static_cast<int>(sizeX - 1))
                        pLine->iDataPos = 0;
                    pLine->dataHistory[pLine->iDataPos] = Data;
                }
            }
        }
        return;
    }

    if (m_StartTime == 0)
        return;

    // Find existing line
    SGraphStatLine* pLine = MapFind(m_LineList, szName);

    if (!pLine)
    {
        // Add new line
        MapSet(m_LineList, szName, SGraphStatLine());
        pLine = MapFind(m_LineList, szName);
        pLine->dataHistory.resize(sizeX);
        memset(&pLine->dataHistory[0], 0, pLine->dataHistory.size());
        pLine->iDataPos = 0;
        pLine->prevData = 0;
        pLine->strName = szName;

        // Deterministic color based on line name hash
        MD5 md5;
        CMD5Hasher().Calculate(szName, strlen(szName), md5);
        uchar* p = md5.data;
        // Use hash bytes to deterministically brighten colors if too dark
        int brightenIndex = 0;
        while (p[0] + p[1] + p[2] < 128)
        {
            int f = md5.data[brightenIndex % NUMELMS(md5.data)];
            int t = (brightenIndex / NUMELMS(md5.data)) % 3;
            p[t] = std::min<unsigned char>(255, p[t] + (f & 0x3F) + 32);
            brightenIndex++;
        }
        pLine->color = SColorRGBA(p[0], p[1], p[2], 255);
    }

    // Calc timing averaged with previous frame
    TIMEUS NewData = GetTimeUs() - m_StartTime;
    TIMEUS AvgData = (NewData + pLine->prevData) / 2;
    pLine->prevData = NewData;

    // Inc position
    pLine->iDataPos++;
    if (pLine->iDataPos > static_cast<int>(sizeX - 1))
        pLine->iDataPos = 0;

    // Insert data point
    pLine->dataHistory[pLine->iDataPos] = AvgData;
}

///////////////////////////////////////////////////////////////
//
// CGraphStats::RemoveTimingPoint
//
//
//
///////////////////////////////////////////////////////////////
void CGraphStats::RemoveTimingPoint(const char* szName)
{
    if (!IsEnabled() || szName[0] == 0)
        return;

    MapRemove(m_LineList, szName);
}

///////////////////////////////////////////////////////////////
//
// CGraphStats::Draw
//
//
//
///////////////////////////////////////////////////////////////
void CGraphStats::Draw()
{
    if (!m_bEnabled)
        return;

    CGraphicsInterface* pGraphics = g_pCore->GetGraphics();
    CLocalGUI*          pLocalGUI = g_pCore->GetLocalGUI();

    std::uint32_t viewportWidth = pGraphics->GetViewportWidth();    // get width of current resolution
    std::uint32_t viewportHeight = pGraphics->GetViewportHeight();  // get height of current resolution
    std::uint32_t originX = 10;                                     // offset the graph by 10 pixels from left side of screen
    std::uint32_t originY = pLocalGUI->GetChatBottomPosition();     // get chat bottom screen position
    std::uint32_t sizeX = viewportWidth / 4;                        // set the width of graph to 1/4 of current resolution
    std::uint32_t sizeY = viewportHeight / 4;                       // set the height of graph to 1/4 of current resolution
    std::uint32_t rangeY = 100;                                     // 100ms

    originY = originY + sizeY + 30;  // add graph height plus a little gap to the overall Y position

    float fLineScale = 1 / 1000.f / rangeY * sizeY;
    float fLineHeight = pGraphics->GetDXFontHeight();

    // Background box
    pGraphics->DrawRectQueued(originX, originY - sizeY, sizeX, sizeY, SColorRGBA(0, 0, 0, 128), true);

    // Pre-calculate total vertices needed: (sizeX-1)*2 vertices per line + 2 for connector line
    std::size_t verticesPerLine = (sizeX - 1) * 2 + 2;
    std::size_t totalVertices = m_LineList.size() * verticesPerLine;

    // Reuse pre-allocated vertex buffer
    m_VertexBuffer.clear();
    m_VertexBuffer.reserve(totalVertices);

    // Build vertex data for all lines
    float fLabelY = originY - m_LineList.size() * fLineHeight;
    for (const auto& dataLine : m_LineList)
    {
        const SGraphStatLine& line = dataLine.second;
        int                   iDataPos = line.iDataPos;
        int                   iDataPosPrev = iDataPos;
        D3DCOLOR              d3dColor = D3DCOLOR_ARGB(line.color.A, line.color.R, line.color.G, line.color.B);
        float                 fLabelX = originX + sizeX + 22;
        float                 fFirstY0 = 0;

        // Add graph line segments as line list (2 vertices per segment)
        for (int i = sizeX - 1; i > 0; i--)
        {
            float fY0 = line.dataHistory[iDataPos] * fLineScale;
            float fY1 = line.dataHistory[iDataPosPrev] * fLineScale;

            // Store first point Y for connector line
            if (i == sizeX - 1)
                fFirstY0 = fY0;

            iDataPosPrev = iDataPos;
            iDataPos--;
            if (iDataPos == -1)
                iDataPos = sizeX - 1;

            // Add line segment vertices
            PrimitiveVertice v1 = {static_cast<float>(originX + i - 1), static_cast<float>(originY) - fY0, 0.0f, d3dColor};
            PrimitiveVertice v2 = {static_cast<float>(originX + i), static_cast<float>(originY) - fY1, 0.0f, d3dColor};
            m_VertexBuffer.push_back(v1);
            m_VertexBuffer.push_back(v2);
        }

        // Add connector line from graph to label
        PrimitiveVertice connStart = {static_cast<float>(originX + sizeX - 2), static_cast<float>(originY) - fFirstY0, 0.0f, d3dColor};
        PrimitiveVertice connEnd = {fLabelX - 2, fLabelY + fLineHeight / 2, 0.0f, d3dColor};
        m_VertexBuffer.push_back(connStart);
        m_VertexBuffer.push_back(connEnd);

        fLabelY += fLineHeight;
    }

    // Draw all lines in a single batched call
    if (!m_VertexBuffer.empty())
    {
        auto* pVecVertices = new std::vector<PrimitiveVertice>(std::move(m_VertexBuffer));
        pGraphics->DrawPrimitiveQueued(pVecVertices, D3DPT_LINELIST, true);
    }

    // Draw labels (text rendering is already efficient)
    float fLabelX = originX + sizeX + 22;
    fLabelY = originY - m_LineList.size() * fLineHeight;
    for (const auto& dataLine : m_LineList)
    {
        const SGraphStatLine& line = dataLine.second;
        pGraphics->DrawStringQueued(fLabelX, fLabelY, fLabelX, fLabelY, line.color, line.strName.c_str(), 1.0f, 1.0f, DT_NOCLIP, nullptr, true);
        fLabelY += fLineHeight;
    }
}
