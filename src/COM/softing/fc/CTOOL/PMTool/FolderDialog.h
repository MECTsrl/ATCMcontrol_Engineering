/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/PMTool/FolderDialog.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FolderDialog.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/PMTool/FolderDialog.h $
 *
 * =PROJECT             ATCMControl V2.0
 *
 * =SWKE                PMTool
 *
 * =COMPONENT           PMTool
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *
 
 *

 
 *  

 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  27.02.2003, LN        File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/

#if !defined(AFX_FOLDERDIALOG_H__EF81D90E_B3A8_4C72_8C5C_38F531CC0A72__INCLUDED_)
#define AFX_FOLDERDIALOG_H__EF81D90E_B3A8_4C72_8C5C_38F531CC0A72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FolderDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFolderDialog dialog

class CFolderDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CFolderDialog)

	friend LRESULT CALLBACK WindowProcDiag(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	friend LRESULT CALLBACK WindowProcEdit(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	CFolderDialog(LPCTSTR szInitPath, LPCTSTR szMsgTitle);

public:
	CString m_sSelPath;
	CString m_sMsgTitle;

protected:
	static WNDPROC m_fpWndProcDiag;
	static WNDPROC m_fpWndProcEdit;

public:
	virtual int DoModal();

protected:
	virtual void OnInitDone();
	virtual void OnFileNameChange();

	UINT CreateDirTree(CHAR CONST *pPath, UINT uPos, BOOL bDirOnly);
    
    BOOL GetExisting(CString& rstrPath);
    BOOL StripPath(CString& rstrPath);

protected:
	//{{AFX_MSG(CFolderDialog)
	virtual BOOL OnInitDialog();
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOLDERDIALOG_H__EF81D90E_B3A8_4C72_8C5C_38F531CC0A72__INCLUDED_)

/*
 *----------------------------------------------------------------------------*
 * $History: FolderDialog.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/PMTool
 * 
 * *****************  Version 1  *****************
 * User: Sis          Date: 2.04.04    Time: 10:35
 * Created in $/4Control/COM/softing/fc/CTOOL/PMTool
 * 
 *==
 *----------------------------------------------------------------------------*
*/
