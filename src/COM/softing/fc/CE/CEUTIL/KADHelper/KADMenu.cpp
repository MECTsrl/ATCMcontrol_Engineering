
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

#include "stdafx.h"
#include "KADMenu.h"
#include "SBLLexTree.h"

//----  Static Initializations:   ----------------------------------*


///////////////////////////////////////////////////////////////////////
///////////////////// CKADMenuItem ////////////////////////////////
///////////////////////////////////////////////////////////////////////

CKADMenuItem::CKADMenuItem()
{
    m_pHandler      = NULL;
    m_iKSO          = -1;
    m_bEnabled      = TRUE;
    m_bSeparator    = FALSE;
    m_iItemNo       = -1;
    m_iKeyMouse     = -1;
    m_bAccel        = FALSE;
    m_wAccelVirt    = 0;
    m_wAccelKey     = 'a';
}

CKADMenuItem::~CKADMenuItem()
{
    if(m_pHandler)
    {
        delete m_pHandler;
        m_pHandler = NULL;
    }
}


///////////////////////////////////////////////////////////////////////
////////////////////// CKADMenu ///////////////////////////////////
///////////////////////////////////////////////////////////////////////

CKADMenu::CKADMenu()
{
    m_iItemCount    = 0;
    m_pHandler      = NULL;
    for(int i=0; i < NUM_KEY_MOUSE_EVENTS; ++i)
    {
        m_aiKeyMouseArray[i] = -1;
    }
}

CKADMenu::~CKADMenu()
{
    if(m_pHandler)
    {
        delete m_pHandler;
        m_pHandler = 0;
    }
    // delete menu items
    long                lNumItems = m_tItems.GetSize();
    CKADMenuItem*   pItem;

    for(long lItem = 0; lItem < lNumItems; ++lItem)
    {
        pItem = m_tItems.GetAt(lItem);
        delete pItem;
    }
}


int CKADMenu::GetItemCount()
{
    return m_iItemCount;
}

//------------------------------------------------------------------*
/**
 * add menu item
 *
 * @param           - CKADMenuItem* pItemNew - new item
*/
void CKADMenu::AddMenuItem(CKADMenuItem* pItemNew)
{
    if(pItemNew->m_iKeyMouse > -1)
    {
        m_aiKeyMouseArray[pItemNew->m_iKeyMouse] = m_iItemCount;
    }
    pItemNew->m_iItemNo = m_iItemCount++;
    m_tItems.Add(pItemNew);
}


//------------------------------------------------------------------*
/**
 * sort menu items according to order key and insert separators automatically.
 *
*/
void CKADMenu::SortMenu()
{
    CSBLLexTree<CKADMenuItem>   tMenuItemTree;
    CKADMenuItem*               pMenuItem;

    // run over item array and insert them into the lex tree
    int iNumItems = m_tItems.GetSize();
    for(int iItem = 0; iItem < iNumItems; ++iItem)
    {
        pMenuItem = m_tItems[iItem];
        if(!pMenuItem->m_strOrderKey.IsEmpty())
        {
            VERIFY(tMenuItemTree.insertData(pMenuItem->m_strOrderKey, pMenuItem));
        }
        else
        {
            delete pMenuItem;
        }
    }

    // now delete item array and fill it sorted
    m_tItems.RemoveAll();
    m_iItemCount = 0;

    CStringArray    astrOrderKeys;
    CString         strOrderKeyOld;
    CString         strOrderKeyNew;

    tMenuItemTree.getAllStringsFromSubstring("", astrOrderKeys);
    iNumItems = astrOrderKeys.GetSize();
    for(iItem = 0; iItem < iNumItems; ++iItem)
    {
        strOrderKeyNew = astrOrderKeys[iItem];
        pMenuItem = tMenuItemTree.getData(strOrderKeyNew);
        if(!strOrderKeyNew.IsEmpty() && !strOrderKeyOld.IsEmpty())
        {
            if(strOrderKeyNew[0] != strOrderKeyOld[0])
            {
                AddSeparator();
            }
        }
        AddMenuItem(pMenuItem);
        strOrderKeyOld = strOrderKeyNew;
    }

}

//------------------------------------------------------------------*
/**
 * add separator item
*/
void CKADMenu::AddSeparator()
{
    CKADMenuItem* pItemNew = new CKADMenuItem;
    pItemNew->m_bSeparator = TRUE;

    pItemNew->m_iItemNo = m_iItemCount++;
    m_tItems.Add(pItemNew);
}


//------------------------------------------------------------------*
/**
 * get menu item with id.
 *
 * @param           [in] iId: id of item
 * @return          pointer to menu item or NULL if not found
*/
CKADMenuItem* CKADMenu::GetMenuItemWithId(int iId)
{
    long            lNumItems = m_tItems.GetSize();
    CKADMenuItem*   pItem;

    for(long lItem = 0; lItem < lNumItems; ++lItem)
    {
        pItem = m_tItems.GetAt(lItem);
        if(pItem->m_iItemNo == iId)
        {
            return pItem;
        }
    }
    return NULL;
}

