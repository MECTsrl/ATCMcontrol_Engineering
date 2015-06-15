
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

#include "stdafx.h"
#include "MainDropTarget.h"
#include "mainfrm.h"


//------------------------------------------------------------------*
/**
 * constructor
 *
 *  pointer to main frame can be set later using SetMainFrame()
 *
 * @param           [in] pMainFrame: pointer to main frame
 *                                      default is NULL
 * @see             SetMainFrame()
*/
CMainDropTarget::CMainDropTarget(CMainFrame* pMainFrame /*=NULL*/)
{
    m_pMainFrame = pMainFrame;
}

CMainDropTarget::~CMainDropTarget()
{
}


//------------------------------------------------------------------*
/**
 * set pointer to main frame
 *
 * @param           [in] pMainFrame: pointer to main frame
 *
*/
void CMainDropTarget::SetMainFrame(CMainFrame* pMainFrame)
{
    m_pMainFrame = pMainFrame;
}


BEGIN_MESSAGE_MAP(CMainDropTarget, COleDropTarget)
	//{{AFX_MSG_MAP(CMainDropTarget)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMainDropTarget commands



//------------------------------------------------------------------*
/**
 * OnDragEnter
 *
 *  just calls CMainFrame::OnDragEnter() if available.
 *  else COleDataObject::OnDragEnter()
 *
 * @see             CMainFrame::OnDragEnter(), COleDataObject::OnDragEnter()
*/
DROPEFFECT CMainDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
    if(m_pMainFrame)
    {
    	m_pMainFrame->OnDragEnter(pWnd, pDataObject, dwKeyState, point);
    }
	return COleDropTarget::OnDragEnter(pWnd, pDataObject, dwKeyState, point);
}
