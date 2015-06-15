
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"

#include "KADToolbar.h"
#include "KADMenu.h"
#include "SBLLexTree.h"

//----  Static Initializations:   ----------------------------------*


/////////////////////////////////////////////////////////////////////
///////////////////////// CKADToolbarItem ///////////////////////////
/////////////////////////////////////////////////////////////////////
CKADToolbarItem::CKADToolbarItem()
{
    m_iMainMenuItemNo = -1;
    m_bSeparator = FALSE;
}

CKADToolbarItem::~CKADToolbarItem()
{
}



/////////////////////////////////////////////////////////////////////
/////////////////////////// CKADToolbar /////////////////////////////
/////////////////////////////////////////////////////////////////////
CKADToolbar::CKADToolbar()
{
    m_tPos = 0;
}

CKADToolbar::~CKADToolbar()
{
    CKADToolbarItem*    pToolbarItem;

    POSITION    pos = m_tItemList.GetHeadPosition();
    while(pos)
    {
        pToolbarItem = m_tItemList.GetNext(pos);
        delete pToolbarItem;
    }
    m_tItemList.RemoveAll();
}

void CKADToolbar::AddItem(const CString& crstrMainMenuId, const CString& crstrBitmapFile, const CString& crstrToolTipText)
{
    CKADToolbarItem* pItem = new CKADToolbarItem;

    pItem->m_strMainMenuId = crstrMainMenuId;
    pItem->m_strBitmapFile = crstrBitmapFile;
    pItem->m_strToolTipText = crstrToolTipText;
    m_tItemList.AddTail(pItem);
}

void CKADToolbar::AddSeparatorItem()
{
    CKADToolbarItem* pItem = new CKADToolbarItem;

    pItem->m_bSeparator = TRUE;
    m_tItemList.AddTail(pItem);
}


CKADToolbarItem* CKADToolbar::GetFirstItem()
{
    m_tPos = m_tItemList.GetHeadPosition();
    return(GetNextItem());
}

CKADToolbarItem* CKADToolbar::GetNextItem()
{
    if(!m_tPos)
    {
        return NULL;
    }
    return(m_tItemList.GetNext(m_tPos));
}

//------------------------------------------------------------------*
/**
 * sort toolbar.
 *
 *  sorts toolbar items according to the corresponding main menu.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CKADToolbar::SortToolbar(CKADMenu* pMenu)
{
    // run over all toolbar items and inser them into lex tree
    CSBLLexTree<CKADToolbarItem>    tItemTree;
    CKADToolbarItem*                pToolbarItem;
    CKADMenuItem*                   pMenuItem;
    int                             iNumItems;

    // run over item list and insert them into the lex tree
    POSITION    pos = m_tItemList.GetHeadPosition();
    while(pos)
    {
        pToolbarItem = m_tItemList.GetAt(pos);
        if(pToolbarItem->m_iMainMenuItemNo >= 0)
        {
            pMenuItem = pMenu->GetMenuItemWithId(pToolbarItem->m_iMainMenuItemNo);
            ASSERT(pMenuItem);
            if(pMenuItem)
            {
                if(!pMenuItem->m_strOrderKey.IsEmpty())
                {
                    VERIFY(tItemTree.insertData(pMenuItem->m_strOrderKey, pToolbarItem));
                }
                else
                {
                    delete pToolbarItem;
                }
            }
        }
        m_tItemList.GetNext(pos);
    }

    // now delete item array and fill it sorted
    m_tItemList.RemoveAll();

    CStringArray    astrOrderKeys;
    CString         strOrderKeyOld;
    CString         strOrderKeyNew;

    tItemTree.getAllStringsFromSubstring("", astrOrderKeys);
    iNumItems = astrOrderKeys.GetSize();
    for(int iItem = 0; iItem < iNumItems; ++iItem)
    {
        strOrderKeyNew = astrOrderKeys[iItem];
        pToolbarItem = tItemTree.getData(strOrderKeyNew);
        if(!strOrderKeyNew.IsEmpty() && !strOrderKeyOld.IsEmpty())
        {
            if(strOrderKeyNew[0] != strOrderKeyOld[0])
            {
                AddSeparatorItem();
            }
        }
        m_tItemList.AddTail(pToolbarItem);
        strOrderKeyOld = strOrderKeyNew;
    }
}
