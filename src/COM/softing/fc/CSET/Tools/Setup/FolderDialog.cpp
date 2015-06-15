/* ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: FolderDialog.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/FolderDialog.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				CSET
 *
 * =COMPONENT			Tools\Setup
 *
 * =CURRENT 	 $Date: 28.02.07 19:01 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 */

#include "stdafx.h"
#include "Setup.h"
#include "FolderDialog.h"
#include <io.h>
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFolderDialog

IMPLEMENT_DYNAMIC(CFolderDialog, CFileDialog)

WNDPROC CFolderDialog::m_fpWndProcDiag = NULL;
WNDPROC CFolderDialog::m_fpWndProcEdit = NULL;

#define DIR_MASK			_T("*..*")


/* IsDirectory
 * ----------------------------------------------------------------------------
 */
static BOOL IsDirectory(CString lpszName)
{
	DWORD dwRet;
	dwRet = GetFileAttributes(lpszName);
	return (dwRet != 0xFFFFFFFF) && (dwRet & FILE_ATTRIBUTE_DIRECTORY);
}


/* CFolderDialog
 * ----------------------------------------------------------------------------
 */
CFolderDialog::CFolderDialog(LPCTSTR szInitPath, LPCTSTR szMsgTitle) : CFileDialog(TRUE, NULL, DIR_MASK)
{
	m_sSelPath = szInitPath;

	if (m_sSelPath.GetAt(m_sSelPath.GetLength() - 1) == _T('\\'))
	{
		m_sSelPath = m_sSelPath.Left(m_sSelPath.GetLength() - 1);
	}

	m_ofn.lpstrInitialDir = m_sSelPath;

	m_sMsgTitle = szMsgTitle;
}


/* CreateDirTree
 * ----------------------------------------------------------------------------
 */
UINT CFolderDialog::CreateDirTree(CHAR CONST *pPath, UINT uPos, BOOL bDirOnly)
{
	CHAR pBuffer[MAX_PATH];
	UINT uStart = uPos;

	BOOL bUNC = FALSE;

	if (pPath[uPos] == '\\' && pPath[uPos + 1] == '\\')
	{
		uPos++;
		uPos++;

		bUNC = TRUE;
	}
	else if (pPath[uPos] == '\\')
	{
		uPos++;
		uStart++;
	}

	if (pPath[uPos] == '\0')
	{
		return TRUE;
	}

	for ( ; pPath[uPos] != '\0' && pPath[uPos] != '\\'; uPos++)
	{
		;
	}

	if (bUNC == TRUE && pPath[uPos] != '\0')
	{
		uPos++;
		for ( ; bUNC == TRUE && pPath[uPos] != '\0' && pPath[uPos] != '\\'; uPos++)
		{
			;
		}
	}

	if (pPath[uPos] == '\0' && bDirOnly == FALSE)
	{
		return TRUE;
	}

	memcpy(pBuffer, pPath + uStart, uPos - uStart);
	pBuffer[uPos - uStart] = '\0';

	if (pBuffer[1] == ':')
	{
		// Drive spec.
		pBuffer[2] = '\\';
		pBuffer[3] = '\0';
	}
	else if (bUNC == FALSE)
	{
		// Path
		_mkdir(pBuffer); 
	}

	if (_chdir(pBuffer) != 0)
	{
		return FALSE;
	}

	return CreateDirTree(pPath, uPos, bDirOnly);
}


/* WindowProcDiag
 * ----------------------------------------------------------------------------
 */
LRESULT CALLBACK WindowProcDiag(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_COMMAND)
	{
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == IDOK)
		{
			CFileDialog *pDlg = (CFileDialog *)CWnd::FromHandle(hwnd);
			if (pDlg == NULL)
			{
				return CallWindowProc(CFolderDialog::m_fpWndProcDiag, hwnd, message, wParam, lParam);
			}
			
			CString sPath;
			pDlg->GetDlgItem(edt1)->GetWindowText(sPath);

			// If the user has entered something like "c:\\windows\\\\system"
			// this is invalid but _access returns valid if this string is passed to
			// it. So before giving it to _access validate it.
			for (int i = 1; i < sPath.GetLength() - 1; i++)
			{
				if (sPath[i] == '\\' && sPath[i+1] == '\\')
				{
					CString sMsg;
					sMsg.Format(_T("The selected folder '%s' is invalid. Please select another one"), sPath);

					MessageBox(hwnd, sMsg, ((CFolderDialog*)pDlg->GetDlgItem(0))->m_sMsgTitle, MB_OK | MB_ICONHAND);
					return NULL;
				}
			}
			
			// If user has entered a trailing backslash remove it
			// e.g. "c:\\windows\\system\\" --> "c:\\windows\\system"
			if (sPath[sPath.GetLength() - 1] == '\\')
				sPath = sPath.Left (sPath.GetLength() - 1);
			
			//If path does not exist
			if(_access(sPath, 00) != 0)
			{
				CString sTempPath(sPath);
				sTempPath += _T('\\');

				CString sMsg;
				sMsg.Format(_T("The selected path '%s' does not exist.\nDo you want to create it?"), sTempPath);

				if(MessageBox(hwnd, sMsg, ((CFolderDialog*)pDlg->GetDlgItem(0))->m_sMsgTitle, MB_YESNO | MB_ICONQUESTION) == IDYES)
				{
					UINT uPos = 0;

					if (! ((CFolderDialog*)pDlg->GetDlgItem(0))->CreateDirTree(sTempPath, uPos, TRUE))
					{
						sMsg.Format(_T("Could not create folder: %s"), sTempPath);

						MessageBox(hwnd, sMsg, ((CFolderDialog*)pDlg->GetDlgItem(0))->m_sMsgTitle, MB_OK | MB_ICONHAND);
						return NULL;
					}
				}
				else
				{
					return NULL;
				}
			}
			else
			{
				TCHAR szCurrent[MAX_PATH]; 				
				GetCurrentDirectory (MAX_PATH, szCurrent);

				HWND hwndTest = GetFocus() ;
				if (hwndTest == pDlg->GetDlgItem(edt1)->m_hWnd && stricmp(sPath, szCurrent) != 0)
				{
					// OK was caused by edit control, don't close the dialog.
					return CallWindowProc(CFolderDialog::m_fpWndProcDiag, hwnd, message, wParam, lParam);
				}
			}

			((CFolderDialog*)pDlg->GetDlgItem(0))->m_sSelPath =  sPath;
			((CFolderDialog*)pDlg->GetDlgItem(0))->m_sSelPath += _T('\\');
			
			pDlg->EndDialog(IDOK);
			return NULL;
		}
	}

	return CallWindowProc(CFolderDialog::m_fpWndProcDiag, hwnd, message, wParam, lParam);
}


/* WindowProcEdit
 * ----------------------------------------------------------------------------
 */
LRESULT CALLBACK WindowProcEdit(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	if (message == WM_SETTEXT)
	{
		LPCTSTR szText = (LPCTSTR)lParam;
		if (szText && strcmp(DIR_MASK, szText) == 0)
		{
			return TRUE;
		}
	}

	return CallWindowProc(CFolderDialog::m_fpWndProcEdit, hwnd, message, wParam, lParam);
}


/* OnInitDialog
 * ----------------------------------------------------------------------------
 */
BOOL CFolderDialog::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	
	//	sets the edit box to contain our initial directory
	SetControlText(edt1, m_sSelPath);
	
	// return TRUE unless you set the focus to a control
	return TRUE;
}


/* OnFileNameChange
 * ----------------------------------------------------------------------------
 */
void CFolderDialog::OnFileNameChange()
{
	TCHAR szPath[MAX_PATH]; 
	
	GetCurrentDirectory (MAX_PATH, szPath);
	m_sSelPath = CString(szPath);
	
	// get the list control 
	CWnd *pParent = GetParent()->GetDlgItem(lst2); 
	CListCtrl *pList = (CListCtrl *)pParent->GetDlgItem(1); 
	
	// currently selected item 
	int pos = pList->GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	
	if (pos != -1)
	{ 
		// create the full path... 
		CString sSel(pList->GetItemText(pos, 0)); 
		CString sPath(szPath);
		
		if (m_sSelPath.GetAt(m_sSelPath.GetLength() - 1) == '\\')
		{
			sPath = CString (szPath) + sSel; 
		}
		else
		{
			sPath = CString (szPath) + _T ( "\\" ) + sSel; 
		}

		if (::IsDirectory(sPath))
			m_sSelPath = sPath;
	} 

	SetControlText(edt1, m_sSelPath);
}


/* DoModal
 * ----------------------------------------------------------------------------
 */
int CFolderDialog::DoModal()
{
	TCHAR path[MAX_PATH]; 	
	GetCurrentDirectory (MAX_PATH, path);

	int iRes = CFileDialog::DoModal();

	SetCurrentDirectory(path);

	return iRes;
}


/* OnInitDone
 * ----------------------------------------------------------------------------
 */
void CFolderDialog::OnInitDone()
{
	HideControl(stc2);
	HideControl(cmb1);

	CWnd* pFD = GetParent();

    LCID lcid = GetSystemDefaultLCID();

    switch(PRIMARYLANGID(LANGIDFROMLCID(lcid)))
    {
        default:
        case LANG_ENGLISH:
			SetControlText(stc3, "Folder:");
			SetControlText(IDOK, "&Select");	
			pFD->SetWindowText("Select Folder");
            break;

        case LANG_GERMAN:
			SetControlText(stc3, "Verzeichnis:");
			SetControlText(IDOK, "&Auswahl");	
			pFD->SetWindowText("Verzeichnis auswählen");
            break;
    }

	SetControlText(edt1, m_sSelPath);

	CRect rStatic; 
	pFD->GetDlgItem(stc3)->GetWindowRect(rStatic);
	pFD->ScreenToClient(rStatic);
	rStatic.right -= 10;
	pFD->GetDlgItem(stc3)->SetWindowPos(0, 0, 0, rStatic.Width(), rStatic.Height(), SWP_NOMOVE | SWP_NOZORDER);

	CRect rectCancel; 
	pFD->GetDlgItem(IDCANCEL)->GetWindowRect(rectCancel);
	pFD->ScreenToClient(rectCancel);

	CRect rectOK; 
	pFD->GetDlgItem(IDOK)->GetWindowRect(rectOK);
	pFD->ScreenToClient(rectOK);
	pFD->GetDlgItem(IDOK)->SetWindowPos(0,rectCancel.left - rectOK.Width() - 4, rectCancel.top, 0,0, SWP_NOZORDER | SWP_NOSIZE);

	CRect rList; 
	pFD->GetDlgItem(lst1)->GetWindowRect(rList);
	pFD->ScreenToClient(rList);

	CRect rEdit;
	pFD->GetDlgItem(edt1)->GetWindowRect(rEdit);
	pFD->ScreenToClient(rEdit);
	rEdit.left = rStatic.right + 10;
	pFD->GetDlgItem(edt1)->SetWindowPos(0, rEdit.left, rEdit.top, rList.right - rEdit.left, rEdit.Height(), SWP_NOZORDER);
		
	m_fpWndProcDiag = (WNDPROC)SetWindowLong(pFD->m_hWnd, GWL_WNDPROC, (long)WindowProcDiag);
	m_fpWndProcEdit = (WNDPROC)SetWindowLong(pFD->GetDlgItem(edt1)->m_hWnd, GWL_WNDPROC, (long)WindowProcEdit);
}

BEGIN_MESSAGE_MAP(CFolderDialog, CFileDialog)
	//{{AFX_MSG_MAP(CFolderDialog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/* ---------------------------------------------------------------------------- */
