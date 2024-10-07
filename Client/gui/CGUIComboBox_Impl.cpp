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

#include "StdInc.h"

#define CGUICOMBOBOX_NAME "Combobox"

CGUIComboBox_Impl::CGUIComboBox_Impl(CGUI_Impl* pGUI, CGUIElement* pParent, const char* szCaption)
{
    m_pManager = pGUI;

    // Get an unique identifier for CEGUI (gah, there's gotta be an another way)
    char szUnique[CGUI_CHAR_SIZE];
    pGUI->GetUniqueName(szUnique);

    // Create the window and set default settings
    m_pWindow = pGUI->GetWindowManager()->createWindow(pGUI->GetElementPrefix() + "/" + CGUICOMBOBOX_NAME, szUnique);
    m_pWindow->setDestroyedByParent(false);

    // This needs a better alternative, so changing comboBox will change this - Jyrno42
    storedCaption = CGUI_Impl::GetUTFString(szCaption);

    m_pWindow->setText(storedCaption);

    m_pWindow->setSize(pGUI->CreateAbsoluteSize(128.0f, 24.0f));
    m_pWindow->setVisible(true);

    // Store the pointer to this CGUI element in the CEGUI element
    m_pWindow->setUserData(reinterpret_cast<void*>(this));

    // Add out changed event
    m_pWindow->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&CGUIComboBox_Impl::Event_OnSelectionAccepted, this));
    m_pWindow->subscribeEvent(CEGUI::Combobox::EventDropListRemoved, CEGUI::Event::Subscriber(&CGUIComboBox_Impl::Event_OnDropListRemoved, this));

    AddEvents();

    // If a parent is specified, add it to it's children list, if not, add it as a child to the pManager
    if (pParent)
    {
        SetParent(pParent);
    }
    else
    {
        pGUI->AddChild(this);
        SetParent(NULL);
    }
}

CGUIComboBox_Impl::~CGUIComboBox_Impl()
{
    Clear();
    DestroyElement();
}

CGUIStandardItem* CGUIComboBox_Impl::AddItem(const char* szText)
{
    CGUIStandardItem_Impl* pNewItem = new CGUIStandardItem_Impl(m_pManager, szText, CGUIStandardItem_Impl::TextItem, NULL);
    CEGUI::StandardItem* pListboxItem = pNewItem->GetListItem();
    reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->addItem(pListboxItem);
    m_Items[pNewItem->GetListItem()] = pNewItem;
    return pNewItem;
}

CGUIStandardItem* CGUIComboBox_Impl::AddItem(CGUIStaticImage* pImage)
{
    CGUIStandardItem_Impl*   pNewItem = new CGUIStandardItem_Impl(m_pManager, "", CGUIStandardItem_Impl::ImageItem, (CGUIStaticImage_Impl*)pImage);
    CEGUI::StandardItem*   pListboxItem = pNewItem->GetListItem();
    reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->addItem(pListboxItem);
    m_Items[pNewItem->GetListItem()] = pNewItem;
    return pNewItem;
}

bool CGUIComboBox_Impl::RemoveItem(int index)
{
    try
    {
        auto                 window = reinterpret_cast<CEGUI::Combobox*>(m_pWindow);
        CEGUI::StandardItem* pItem = reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->getItemFromIndex(index);
        if (pItem && window->isItemSelected(index))            // if this is currently selected, let's update the editbox.
        {
            m_pWindow->setText(storedCaption);
        }
        reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->removeItem(pItem);
        return true;
    }
    catch (...)
    {
        return false;
    }
    return false;
}

CGUIStandardItem* CGUIComboBox_Impl::GetSelectedItem()
{
    return GetListItem(reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->getSelectedItem());
}

int CGUIComboBox_Impl::GetSelectedItemIndex()
{
    CEGUI::StandardItem* pItem = reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->getSelectedItem();

    CFastHashMap<CEGUI::StandardItem*, CGUIStandardItem_Impl*>::iterator it;
    it = m_Items.find(pItem);
    if (it == m_Items.end())
        return -1;

    try
    {
        return reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->getItemIndex(it->first);
    }
    catch (...)
    {
        return -1;
    }
}

int CGUIComboBox_Impl::GetItemIndex(CGUIStandardItem* pItem)
{
    CFastHashMap<CEGUI::StandardItem*, CGUIStandardItem_Impl*>::iterator it;

    bool found = false;

    for (it = m_Items.begin(); it != m_Items.end(); it++)
    {
        if (it->second == pItem)
        {
            found = true;
            break;
        }
    }
    if (found)
    {
        try
        {
            return reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->getItemIndex((CEGUI::StandardItem*)it->first);
        }
        catch (...)
        {
            return -1;
        }
    }

    return -1;
}

const char* CGUIComboBox_Impl::GetItemText(int index)
{
    try
    {
        if (index == -1)
        {
            return (const char*)m_pWindow->getText().c_str();
        }
        else
        {
            CEGUI::ListboxItem* pItem = (CEGUI::ListboxItem*)reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->getItemFromIndex(index);
            if (pItem != NULL)
            {
                return (const char*)pItem->getText().c_str();
            }
        }
    }
    catch (...)
    {
        return "";
    }
    return "";
}

bool CGUIComboBox_Impl::SetItemText(int index, const char* szText)
{
    try
    {
        CEGUI::ListboxItem* pItem = (CEGUI::ListboxItem*)reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->getItemFromIndex(index);
        pItem->setText(CGUI_Impl::GetUTFString(szText));
        if (pItem->isSelected())            // if this is currently selected, let's update the editbox.
        {
            m_pWindow->setText(CGUI_Impl::GetUTFString(szText));
        }
        return true;
    }
    catch (...)
    {
        return false;
    }
    return false;
}

CGUIStandardItem* CGUIComboBox_Impl::GetItemByIndex(int index)
{
    CEGUI::StandardItem* pCEGUIItem = reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->getItemFromIndex(index);
    CGUIStandardItem*        pItem = GetListItem(pCEGUIItem);
    return pItem;
}

bool CGUIComboBox_Impl::SetSelectedItemByIndex(int index)
{
    try
    {
        auto combobox = reinterpret_cast<CEGUI::Combobox*>(m_pWindow);
        combobox->clearAllSelections();

        if (index == -1)
        {
            m_pWindow->setText(storedCaption);
            return true;
        }
        else
        {
            CEGUI::StandardItem* pItem = combobox->getItemFromIndex(index);
            if (pItem != NULL)
            {
                combobox->setItemSelectState(index, true);
                combobox->setText(pItem->getText());
                return true;
            }
        }
    }
    catch (...)
    {
        return false;
    }
    return false;
}

void CGUIComboBox_Impl::Clear()
{
    reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->getDropList()->clearList();

    CFastHashMap<CEGUI::StandardItem*, CGUIStandardItem_Impl*>::iterator it;
    for (it = m_Items.begin(); it != m_Items.end(); it++)
    {
        delete it->second;
    }

    m_Items.clear();
    m_pWindow->setText(storedCaption);
}

bool CGUIComboBox_Impl::IsOpen()
{
    return reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->isDropDownListVisible();
}

void CGUIComboBox_Impl::SetReadOnly(bool bReadonly)
{
    reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->setReadOnly(bReadonly);
}

CGUIStandardItem_Impl* CGUIComboBox_Impl::GetListItem(CEGUI::StandardItem* pItem)
{
    CFastHashMap<CEGUI::StandardItem*, CGUIStandardItem_Impl*>::iterator it;
    it = m_Items.find(pItem);
    if (it == m_Items.end())
        return NULL;

    return it->second;
}

size_t CGUIComboBox_Impl::GetItemCount()
{
    return reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->getItemCount();
}

void CGUIComboBox_Impl::SetSelectionHandler(GUI_CALLBACK Callback)
{
    m_OnSelectChange = Callback;
}

void CGUIComboBox_Impl::SetDropListRemoveHandler(GUI_CALLBACK Callback)
{
    m_OnDropListRemoved = Callback;
}

bool CGUIComboBox_Impl::Event_OnSelectionAccepted(const CEGUI::EventArgs& e)
{
    if (m_OnSelectChange)
        m_OnSelectChange(reinterpret_cast<CGUIElement*>(this));
    return true;
}

bool CGUIComboBox_Impl::Event_OnDropListRemoved(const CEGUI::EventArgs& e)
{
    if (m_OnDropListRemoved)
        m_OnDropListRemoved(reinterpret_cast<CGUIElement*>(this));
    return true;
}

void CGUIComboBox_Impl::ShowDropList()
{
    reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->showDropList();
    reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->setSingleCursorActivationEnabled(true);
}

void CGUIComboBox_Impl::HideDropList()
{
    reinterpret_cast<CEGUI::Combobox*>(m_pWindow)->hideDropList();
}
