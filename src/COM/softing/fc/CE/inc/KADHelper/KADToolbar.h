#ifndef __KADTOOLBAR_H_
#define __KADTOOLBAR_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*
class CKADMenu;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////
///////////////////////// CKADToolbarItem ///////////////////////////
/////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------*
/**
 *  class CKADToolbarItem
 *
 *  @see    ....
 */
class CKADToolbarItem
{
public:
    CKADToolbarItem();
    ~CKADToolbarItem();

public:
    int         m_iMainMenuItemNo;
    BOOL        m_bSeparator;
    CString     m_strMainMenuId;
    CString     m_strBitmapFile;
    CString     m_strToolTipText;
};


/////////////////////////////////////////////////////////////////////
/////////////////////////// CKADToolbar /////////////////////////////
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 *  class CKADToolbar
 *
 *  @see    ....
 */
class CKADToolbar
{
public:
    CKADToolbar();
    ~CKADToolbar();

    void AddItem(const CString& crstMainMenuId, const CString& crstrBitmapFile, const CString& crstrToolTipText);

    CKADToolbarItem* GetFirstItem();
    CKADToolbarItem* GetNextItem();

    void SortToolbar(CKADMenu* pMenu);

protected:
    void AddSeparatorItem();

public:
    CString     m_strId;
    POSITION    m_tPos;
    CList<CKADToolbarItem*, CKADToolbarItem*>   m_tItemList;
};

#endif // __KADTOOLBAR_H_

