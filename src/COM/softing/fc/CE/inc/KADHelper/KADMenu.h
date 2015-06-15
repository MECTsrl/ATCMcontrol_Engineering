#ifndef __KADMENU_H_
#define __KADMENU_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "kso.h"
#include <afxtempl.h>

//----  Forward Class Definitions:   -------------------------------*
class CKADMenuItem;
//----  Defines:    ------------------------------------------------*

//---- Typedefs:    ------------------------------------------------*
typedef CArray<CKADMenuItem*, CKADMenuItem*> T_ItemArray;

//----  Globals:    ------------------------------------------------*


//----  Prototypes:    ---------------------------------------------*


//------------------------------------------------------------------*
/**
 *  class CKADMenuItem - class for holding information for
 *    menu item
 *
 *  @see CKADMenu
 */
class CKADMenuItem
{
public:
    CKADMenuItem();
    ~CKADMenuItem();

public:
    int         m_iItemNo;          // number of item
    CString     m_strId;            // used for main menu items as identifier
    CString     m_strText;          // item text
    GUID*       m_pHandler;         // COM-handler
    CString     m_strExe;           // exe-handler (file + path)
    CString     m_strCommand;       // method name or command line
    int         m_iKSO;             // no. of kernel standard operation
    int         m_iKeyMouse;        // no. of key / mouse event associated with this item
    CString     m_strStatusText;    // text for status bar
    CString     m_strToolTipText;   // text for tooltip
    BOOL        m_bEnabled;         // menu item enabled?
    BOOL        m_bSeparator;       // menu item is separator?
    CString     m_strOrderKey;      // order key
    BOOL        m_bAccel;           // accelerator defined?
    WORD        m_wAccelVirt;       // FALT || FCONTROL || FSHIFT || FVIRTKEY
    WORD        m_wAccelKey;        // key value
    CString     m_strEnableExpr;    // enable expression
    CString     m_strShowExpr;      // show expression

};

// class for KAD-type menu

//------------------------------------------------------------------*
/**
 *  class CKADMenu - class for holding menu information
 *      - menu items are contained as list of CKADMenuItem
 *
 *      caution: use AddXXX methods to ensure correct numbering of items
 *
 *  @see CKADMenuItem
 */
class CKADMenu
{
public:
    CKADMenu();
    ~CKADMenu();

    int GetItemCount();

    // add menu item
    virtual void AddMenuItem(CKADMenuItem* pItemNew);

    void SortMenu();

    CKADMenuItem* GetMenuItemWithId(int iId);

protected:
    // add separator item
    void AddSeparator();
            


public:
    CString         m_strID;     // not used
    CString         m_strTitle;  // not used
    GUID*           m_pHandler; // handler guid if all items have the same

    T_ItemArray     m_tItems;   // array of menu items
    // mapping of key / mouse event to item index in array
    int             m_aiKeyMouseArray[NUM_KEY_MOUSE_EVENTS];

protected:
    int             m_iItemCount;   // counts items for setting item numbers


};

#endif // __KADMENU_H_

