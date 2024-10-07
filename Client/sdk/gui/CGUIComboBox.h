/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        gui/CGUIComboBox.h
 *  PURPOSE:     Combobox widget class
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include "CGUIElement.h"
#include "CGUICallback.h"

class CGUIStandardItem;

class CGUIComboBox : public CGUIElement
{
public:
    virtual ~CGUIComboBox(){};

    virtual void              SetReadOnly(bool bRead) = 0;
    virtual CGUIStandardItem* AddItem(const char* szText) = 0;
    virtual CGUIStandardItem* AddItem(CGUIStaticImage* pImage) = 0;
    virtual bool              RemoveItem(int index) = 0;
    virtual CGUIStandardItem* GetItemByIndex(int index) = 0;
    virtual CGUIStandardItem* GetSelectedItem() = 0;
    virtual int               GetSelectedItemIndex() = 0;
    virtual size_t            GetItemCount() = 0;
    virtual const char*       GetItemText(int index) = 0;
    virtual bool              SetItemText(int index, const char* szText) = 0;
    virtual bool              SetSelectedItemByIndex(int index) = 0;
    virtual int               GetItemIndex(CGUIStandardItem* pItem) = 0;
    virtual void              Clear() = 0;
    virtual void              SetSelectionHandler(GUI_CALLBACK Callback) = 0;
    virtual void              SetDropListRemoveHandler(GUI_CALLBACK Callback) = 0;
    virtual void              ShowDropList() = 0;
    virtual void              HideDropList() = 0;
    virtual bool              IsOpen() = 0;
};
