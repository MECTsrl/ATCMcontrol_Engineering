// NavTreeDropTarget.cpp : implementation file
//

#include "stdafx.h"
#include "NavTreeDropTarget.h"
#include "NavTreeControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNavTreeDropTarget

//IMPLEMENT_DYNCREATE(CNavTreeDropTarget, COleDropTarget)


//------------------------------------------------------------------*
/**
 * constructor
 *
 *  pointer to tree control can be set later using SetTreeCtrl()
 *
 * @param           [in] pNavTreeCtrl: pointer to tree control
 *                                      default is NULL
 * @see             SetTreeCtrl()
*/
CNavTreeDropTarget::CNavTreeDropTarget(CNavTreeCtrl* pNavTreeCtrl /*=NULL*/)
{
    m_pNavTreeCtrl = pNavTreeCtrl;
}

CNavTreeDropTarget::~CNavTreeDropTarget()
{
}


//------------------------------------------------------------------*
/**
 * set pointer to tree control
 *
 * @param           [in] pNavTreeCtrl: pointer to tree control
 *
*/
void CNavTreeDropTarget::SetTreeCtrl(CNavTreeCtrl* pNavTreeCtrl)
{
    m_pNavTreeCtrl = pNavTreeCtrl;
}


BEGIN_MESSAGE_MAP(CNavTreeDropTarget, COleDropTarget)
	//{{AFX_MSG_MAP(CNavTreeDropTarget)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNavTreeDropTarget commands

//------------------------------------------------------------------*
/**
 * OnDrop
 *
 *  just calls CNavTreeCtrl::OnDrop() if available.
 *  else COleDataObject::OnDrop()
 *
 * @see             CNavTreeCtrl::OnDrop(), COleDataObject::OnDrop()
*/
BOOL CNavTreeDropTarget::OnDrop( CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point ) 
{
    if(m_pNavTreeCtrl)
    {
        m_pNavTreeCtrl->OnDrop(pWnd, pDataObject, dropEffect, point);
    }
	return COleDropTarget::OnDrop(pWnd, pDataObject, dropEffect, point );
}


//------------------------------------------------------------------*
/**
 * OnDropEx
 *
 *  just calls CNavTreeCtrl::OnDropEx() if available.
 *  else COleDataObject::OnDropEx()
 *  first one not used yet
 *
 * @see             CNavTreeCtrl::OnDropEx(), COleDataObject::OnDropEx()
*/
DROPEFFECT CNavTreeDropTarget::OnDropEx(CWnd* pWnd, COleDataObject* pDataObject, 
                  DROPEFFECT dropEffect, DROPEFFECT dropEffectList, CPoint point)
{
//    if(m_pNavTreeCtrl)
//    {
//        return m_pNavTreeCtrl->OnDropEx(pWnd, pDataObject, dropEffect, dropEffectList, point);
//    }
	return COleDropTarget::OnDropEx(pWnd, pDataObject, dropEffect, dropEffectList, point );
}


//------------------------------------------------------------------*
/**
 * OnDragEnter
 *
 *  just calls CNavTreeCtrl::OnDragEnter() if available.
 *  else COleDataObject::OnDragEnter()
 *
 * @see             CNavTreeCtrl::OnDragEnter(), COleDataObject::OnDragEnter()
*/
DROPEFFECT CNavTreeDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
    if(m_pNavTreeCtrl)
    {
    	m_pNavTreeCtrl->OnDragEnter(pWnd, pDataObject, dwKeyState, point);
    }
	return COleDropTarget::OnDragEnter(pWnd, pDataObject, dwKeyState, point);
}

//------------------------------------------------------------------*
/**
 * OnDragLeave
 *
 *  just calls CNavTreeCtrl::OnDragLeave() if available.
 *  else COleDataObject::OnDragLeave()
 *
 * @see             CNavTreeCtrl::OnDragLeave(), COleDataObject::OnDragLeave()
*/
void CNavTreeDropTarget::OnDragLeave(CWnd* pWnd) 
{
    if(m_pNavTreeCtrl)
    {
    	m_pNavTreeCtrl->OnDragLeave(pWnd);
    }
	COleDropTarget::OnDragLeave(pWnd);
}

//------------------------------------------------------------------*
/**
 * OnDragOver
 *
 *  just calls CNavTreeCtrl::OnDragOver() if available.
 *  else COleDataObject::OnDragOver()
 *
 * @see             CNavTreeCtrl::OnDragOver(), COleDataObject::OnDragOver()
*/
DROPEFFECT CNavTreeDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
    if(m_pNavTreeCtrl)
    {
    	return m_pNavTreeCtrl->OnDragOver(pWnd, pDataObject, dwKeyState, point);
    }
	return COleDropTarget::OnDragOver(pWnd, pDataObject, dwKeyState, point);
}

//------------------------------------------------------------------*
/**
 * OnDragScroll
 *
 *  just calls CNavTreeCtrl::OnDragScroll() if available.
 *  else COleDropTarget::OnDragScroll()
 *
 * @see             CNavTreeCtrl::OnDragScroll(), COleDataObject::OnDragScroll()
*/
DROPEFFECT CNavTreeDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point) 
{
    if(m_pNavTreeCtrl)
    {
    	return m_pNavTreeCtrl->OnDragScroll(pWnd, dwKeyState, point);
    }
	return COleDropTarget::OnDragScroll(pWnd, dwKeyState, point);
}


