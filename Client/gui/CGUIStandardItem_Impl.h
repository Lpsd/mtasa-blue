/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        gui/CGUIStandardItem_Impl.h
 *  PURPOSE:     List widget item class
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include <gui/CGUIStandardItem.h>
#include <CEGUI/CEGUI.h>

// Create custom ListboxItem types
// namespace CEGUI
//{
//    class ImageListboxItem : public ItemEntry
//    {
//    protected:
//    public:
//        ImageListboxItem(const String &type, const String &name);
//        virtual ~ImageListboxItem();
//        static const String WidgetTypeName;
//    };
//    CEGUI_DECLARE_WINDOW_FACTORY(ImageListboxItem);
//}

namespace CEGUI
{
    class ComboBox;
}

class CGUIStaticImage_Impl;

class CGUIStandardItem_Impl : public CGUIStandardItem
{
public:
    enum Type
    {
        TextItem = 0,
        NumberItem = 1,
        ImageItem = 2
    };

    CGUIStandardItem_Impl(CGUI_Impl* pGUI, const char* szText = "", unsigned int uiType = 0, CGUIStaticImage_Impl* pImage = NULL);
    ~CGUIStandardItem_Impl();

    std::string GetText() const;
    void        SetText(const char* pszText, const char* pszSortText = NULL);

    void* GetData() const { return m_pData; }
    void  SetData(void* pData, CGUICallback<void, void*> deleteDataCallback = NULL)
    {
        m_pData = pData;
        m_deleteDataCallback = deleteDataCallback;
    }
    void SetData(const char* pszData);

    void SetDisabled(bool bDisabled);
    void SetFont(const char* szFontName);
    void SetImage(CGUIStaticImage* Image);

    bool GetColor(unsigned char& ucRed, unsigned char& ucGreen, unsigned char& ucBlue, unsigned char& ucAlpha);
    void SetColor(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha);

    CEGUI::StandardItem* GetListItem();

    unsigned int ItemType;

private:
    CEGUI::StandardItem*      m_pListItem;
    void*                     m_pData;
    std::string               m_strData;
    CGUICallback<void, void*> m_deleteDataCallback;
    CEGUI::GenericItem*             m_comboBox;
};
