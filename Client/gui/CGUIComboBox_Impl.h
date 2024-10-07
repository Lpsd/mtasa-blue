/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        gui/CGUIComboBox_Impl.h
 *  PURPOSE:     Combobox widget class
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include "CGUIElement_Impl.h"
#include "CGUIGridListItem_Impl.h"

class CGUIComboBox_Impl : public CGUIComboBox, public CGUIElement_Impl
{
public:
    CGUIComboBox_Impl(class CGUI_Impl* pGUI, CGUIElement* pParent = NULL, const char* szCaption = "");
    ~CGUIComboBox_Impl();

    eCGUIType GetType() { return CGUI_COMBOBOX; };

    CGUIStandardItem* AddItem(const char* szText);
    CGUIStandardItem* AddItem(CGUIStaticImage* pImage);
    bool              RemoveItem(int index);
    CGUIStandardItem* GetItemByIndex(int index);
    CGUIStandardItem* GetSelectedItem();
    int               GetSelectedItemIndex();
    size_t            GetItemCount();
    int               GetItemIndex(CGUIStandardItem* pItem);
    const char*       GetItemText(int index);
    bool              SetItemText(int index, const char* szText);
    bool              SetSelectedItemByIndex(int index);
    void              Clear();
    bool              IsOpen();

    void SetReadOnly(bool bReadonly);

    void SetSelectionHandler(GUI_CALLBACK Callback);
    void SetDropListRemoveHandler(GUI_CALLBACK Callback);

    void ShowDropList();
    void HideDropList();

#include "CGUIElement_Inc.h"

protected:
    CFastHashMap<CEGUI::StandardItem*, CGUIStandardItem_Impl*> m_Items;

    bool                   Event_OnSelectionAccepted(const CEGUI::EventArgs& e);
    bool                   Event_OnDropListRemoved(const CEGUI::EventArgs& e);
    CGUIStandardItem_Impl* GetListItem(CEGUI::StandardItem* pItem);
    CEGUI::String          storedCaption;
    GUI_CALLBACK           m_OnSelectChange;
    GUI_CALLBACK           m_OnDropListRemoved;
};
