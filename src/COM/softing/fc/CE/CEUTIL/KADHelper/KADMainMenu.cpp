
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

#include "stdafx.h"
#include "KADMainMenu.h"

//----  Static Initializations:   ----------------------------------*

const int       con_iAdaptMenuCount = 3;
const char*     con_caAdaptMenuIds[] = { "file", "tools", "help" };

///////////////////////////////////////////////////////////////////////
////////////////////// CKADMainMenu ///////////////////////////////////
///////////////////////////////////////////////////////////////////////

CKADMainMenu::CKADMainMenu()
{
}

CKADMainMenu::~CKADMainMenu()
{
    // delete all adapt menus
    int iMenuCount = m_tAdaptMenus.GetSize();
    CKADMenu*   pKADMenu;

    for(int iMenu=0; iMenu < iMenuCount; ++iMenu)
    {
        pKADMenu = m_tAdaptMenus[iMenu];
        ASSERT(pKADMenu);
        if(pKADMenu)
        {
            delete pKADMenu;
        }
    }
    
    // delete all add menus
    iMenuCount = m_tAddMenus.GetSize();
    for(iMenu=0; iMenu < iMenuCount; ++iMenu)
    {
        pKADMenu = m_tAddMenus[iMenu];
        ASSERT(pKADMenu);
        if(pKADMenu)
        {
            delete pKADMenu;
        }
    }
}

//------------------------------------------------------------------*
/**
 * add menu.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CKADMainMenu::AddMenu(CKADMenu* pMenu)
{
    // if id is in adapt menu list -> add it to the adapt menu array
    if(IsAdaptMenu(pMenu->m_strID))
    {
        m_tAdaptMenus.Add(pMenu);
    }
    else
    {
        m_tAddMenus.Add(pMenu);
    }
}


//------------------------------------------------------------------*
/**
 * is adapt menu.
 *
 * @param           [in] crstrMenuId: menu id
 * @return          is adapt menu?
 * @see             
*/
BOOL CKADMainMenu::IsAdaptMenu(const CString& crstrMenuId)
{
    CString strAdaptMenuId;

    for(int iMenu = 0; iMenu < con_iAdaptMenuCount; ++iMenu)
    {
        strAdaptMenuId = con_caAdaptMenuIds[iMenu];
        if(strAdaptMenuId.CompareNoCase(crstrMenuId) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * get adapt menu count.
 *
 * @return          adapt menu count (number of adapt menus)
*/
int CKADMainMenu::GetAdaptMenuCount()
{
    return(m_tAdaptMenus.GetSize());
}

//------------------------------------------------------------------*
/**
 * get add menu count.
 *
 * @return          add menu count (number of add menus)
*/
int CKADMainMenu::GetAddMenuCount()
{
    return(m_tAddMenus.GetSize());
}

//------------------------------------------------------------------*
/**
 * get adapt menu with certain index.
 *
 * @param           [in] iIndex: index of menu
 * @return          menu pointer or NULL if not found
 * @see             
*/
CKADMenu* CKADMainMenu::GetAdaptMenu(int iIndex)
{
    int iMenuCount = m_tAdaptMenus.GetSize();
    if(iIndex >= 0 && iIndex < iMenuCount)
    {
        return(m_tAdaptMenus[iIndex]);
    }
    return 0;
}

//------------------------------------------------------------------*
/**
 * get add menu with certain index.
 *
 * @param           [in] iIndex: index of menu
 * @return          menu pointer or NULL if not found
 * @see             
*/
CKADMenu* CKADMainMenu::GetAddMenu(int iIndex)
{
    int iMenuCount = m_tAddMenus.GetSize();
    if(iIndex >= 0 && iIndex < iMenuCount)
    {
        return(m_tAddMenus[iIndex]);
    }
    return 0;
}


//------------------------------------------------------------------*
/**
 * get menu by id.
 *
 * @param           [in] crstrId: ID of menu
 * @return          pointer to KAD menu
*/
CKADMenu* CKADMainMenu::GetMenuById(const CString& crstrId)
{
    int iMenuCount = m_tAdaptMenus.GetSize();
    CKADMenu*   pKADMenu;

    for(int iMenu=0; iMenu < iMenuCount; ++iMenu)
    {
        pKADMenu = m_tAdaptMenus[iMenu];
        ASSERT(pKADMenu);
        if(pKADMenu->m_strID.CompareNoCase(crstrId) == 0)
        {
            return pKADMenu;
        }
    }
    
    iMenuCount = m_tAddMenus.GetSize();
    for(iMenu=0; iMenu < iMenuCount; ++iMenu)
    {
        pKADMenu = m_tAddMenus[iMenu];
        ASSERT(pKADMenu);
        if(pKADMenu->m_strID.CompareNoCase(crstrId) == 0)
        {
            return pKADMenu;
        }
    }
    return NULL;
}