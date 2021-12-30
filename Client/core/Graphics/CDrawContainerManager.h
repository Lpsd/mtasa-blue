/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

class CDrawContainerManager : public CDrawContainerManagerInterface
{
public:
    CDrawContainerManager();
    ~CDrawContainerManager();

    virtual CDrawContainer* Create(DrawContainerType type);

private:
};
