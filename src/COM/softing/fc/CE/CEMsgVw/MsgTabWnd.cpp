
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*



// MsgTabWnd.cpp: implementation of the CMsgTabWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MsgTabWnd.h"
#include "MsgTab.h"
#include "MsgTabCtr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * Function name			CMsgTabWnd::CMsgTabWnd
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CMsgTabWnd::CMsgTabWnd()
{

}

//------------------------------------------------------------------*
/**
 * Function name			CMsgTabWnd::~CMsgTabWnd
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CMsgTabWnd::~CMsgTabWnd()
{

}

//------------------------------------------------------------------*
/**
 * Function name			CMsgTabWnd::CreateTabCtrl
 * Description			
 * @param			DWORD dwStyle
 * @param			UINT nID
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL CMsgTabWnd::CreateTabCtrl(DWORD dwStyle, UINT nID)
{
    ASSERT_VALID(this);

    CMsgTabCtr* pTabCtr = new CMsgTabCtr;
	m_pTabCtrl = pTabCtr;
    ASSERT_VALID(this);
	VERIFY(m_pTabCtrl->Create(WS_VISIBLE | m_dwTabCtrlStyle,
		CRect(0, 0, 0, 0), this, nID));

    pTabCtr->SetContextMenuOwner(this);

	dwStyle; // UNUSED
	return (m_pTabCtrl != NULL);
}


//------------------------------------------------------------------*
/**
 * Function name	CMsgTabWnd::SetTabIcon
 * Description			
 * @param			int nIndex
 * @param			HICON hIcon
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgTabWnd::SetTabIcon(int nIndex, HICON hIcon)
{
    m_pTabCtrl->SetTabIcon(nIndex, hIcon);
}


//------------------------------------------------------------------*
/**
 * Function name			CMsgTabWnd::SetTabColor
 * Description			
 * @param			int nIndex
 * @param			COLORREF Color
 * @return			void 
 * @exception			
 * @see			
*/
void CMsgTabWnd::SetTabColor(int nIndex, COLORREF Color)
{
    CMsgTabCtr* pTabCtrl = (CMsgTabCtr*)m_pTabCtrl;
    pTabCtrl->SetTabColor(nIndex, Color);
}

