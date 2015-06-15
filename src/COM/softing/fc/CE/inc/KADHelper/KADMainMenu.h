#ifndef __KADMAINMENU_H_
#define __KADMAINMENU_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "kso.h"
#include "KADMenu.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
typedef CArray<CKADMenu*, CKADMenu*>    T_MenuArray;
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//------------------------------------------------------------------*
/**
 *  class CKADMainMenu - class for holding main menu information
 *
 *  KAD description of submenus "Create", "Tools" and "Help"
 *
*/

class CKADMainMenu
{
public:
    CKADMainMenu();
    ~CKADMainMenu();

    void AddMenu(CKADMenu* pMenu);

    int GetAdaptMenuCount();
    int GetAddMenuCount();

    CKADMenu* GetAdaptMenu(int iIndex);
    CKADMenu* GetAddMenu(int iIndex);

    CKADMenu* GetMenuById(const CString& crstrId);

protected:
    BOOL IsAdaptMenu(const CString& crstrMenuId);

protected:
    T_MenuArray m_tAdaptMenus;
    T_MenuArray m_tAddMenus;
};

#endif // __KADMAINMENU_H_

