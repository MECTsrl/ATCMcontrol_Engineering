#include "stdafx.h"
#include "ChildDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildDialog implementation

CChildDialog::CChildDialog ()
{
#ifndef _AFX_NO_OCC_SUPPORT
	AfxEnableControlContainer ();
#endif // !_AFX_NO_OCC_SUPPORT
}

void CChildDialog::OnOK ()
{
	//
	// Prevent CDialog::OnOK from calling EndDialog.
	//
}

void CChildDialog::OnCancel ()
{
	//
	// Prevent CDialog::OnCancel from calling EndDialog.
	//
}

BOOL CChildDialog::OnCommand (WPARAM wParam, LPARAM lParam) 
{
	//
	// Forward WM_COMMAND messages to the dialog's parent.
	//
	return GetParent ()->SendMessage (WM_COMMAND, wParam, lParam);
}

BOOL CChildDialog::OnNotify (WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	//
	// Forward WM_NOTIFY messages to the dialog's parent.
	//
	return GetParent ()->SendMessage (WM_NOTIFY, wParam, lParam);
}

BOOL CChildDialog::OnCmdMsg (UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	//
	// Forward ActiveX control events to the dialog's parent.
	//
#ifndef _AFX_NO_OCC_SUPPORT
	if (nCode == CN_EVENT)
		return GetParent ()->OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
#endif // !_AFX_NO_OCC_SUPPORT

	return CDialog::OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
}
