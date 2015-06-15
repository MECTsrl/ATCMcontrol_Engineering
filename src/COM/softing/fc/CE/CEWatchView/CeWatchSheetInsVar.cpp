#include "stdafx.h"
#include "cewatchview.h"
#include "CeWatchSheetInsVar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCeWatchSheetInsVar

IMPLEMENT_DYNAMIC(CCeWatchSheetInsVar, CPropertySheet)

CCeWatchSheetInsVar::CCeWatchSheetInsVar(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CCeWatchSheetInsVar::CCeWatchSheetInsVar(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CCeWatchSheetInsVar::~CCeWatchSheetInsVar()
{
}


BEGIN_MESSAGE_MAP(CCeWatchSheetInsVar, CPropertySheet)
	//{{AFX_MSG_MAP(CCeWatchSheetInsVar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCeWatchSheetInsVar message handlers

BOOL CCeWatchSheetInsVar::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	

	CWnd* pOk = GetDlgItem(IDOK);
	CWnd* pApply = GetDlgItem(ID_APPLY_NOW);
	CWnd* pCancel = GetDlgItem(IDCANCEL);
	
	ASSERT(pOk != NULL);
	ASSERT(pApply != NULL);
	ASSERT(pCancel != NULL);

	// hide OK buttton
	// rename Cancel and Apply button
	if (pOk && pCancel && pApply)
	{
		pOk->ShowWindow(SW_HIDE);

		CString str;
		str.LoadString(IDS_CLOSE);
		pCancel->SetWindowText(str);

		str.LoadString(IDS_ADD);
		pApply->SetWindowText(str);
	}

	return bResult;
}

void CCeWatchSheetInsVar::SetModified(bool bModified) 
{
	CWnd* pApply = GetDlgItem(ID_APPLY_NOW);
	if (pApply)
		pApply->EnableWindow(bModified);
}

