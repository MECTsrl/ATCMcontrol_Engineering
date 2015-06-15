/////////////////////////////////////////////////////////////////////////////
// CommonDialogs.h : Wrappers around the Windows common dialog library
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  Wrappers around the Windows common dialog library
//

#pragma once


#include <commdlg.h>
#include <shlobj.h>
#include <Foundation\Apps\Application.h>


namespace stingray {
namespace foundation {


////////////////////////////////////////////////////////////////////////////
//@doc Common Dialogs 
//@topic Common Dialogs | SFL wrapps the common Windows dialog boxes
//@comm Classes include <c CFileDialogImpl>, <c COpenFileDialog>, <c CSaveAsFileDialog>,
// <c CFontDialogImpl>, <c CColorDialogImpl>, <c CFontDialog>, <c CColorDialog>,
// <c CFindDialogImpl>, <c CReplaceDialogImpl>, <c CFindDialog>, and <c CReplaceDialog>
//


/////////////////////////////////////////////////////////////////////////////
// CFileDialogImpl - used for File Open or File Save As
//
//@doc CFileDialogImpl
//
//@mfunc | CFileDialogImpl | CFileDialogImpl | Constructor
//@parm bool  | bOpenFileDialog | true for FileOpen, false for FileSaveAs
//@parmopt LPCTSTR | lpszDefExt | NULL | Default extension
//@parmopt LPCTSTR | lpszFilter | NULL | Filter for file name extenstions,
//@parmopt LPCTSTR | lpszFileName | NULL | File Name 
//@parmopt DWORD | dwFlags | OFN_HIDEREADONLY + OFN_OVERWRITEPROMPT | Hide or show certain files.
//@parmopt DWORD | dwExFlags | OFN_EX_NOPLACESBAR | Extended Windows 2000 flags. 
//@parmopt HWND | hWndParent | NULL | Parent window handle 
//@comm Initialize the OPENFILENAME structure 
//
//@mfunc int | CFileDialogImpl | DoModal | Invoke the dialog
//@rdesc Returns the value of the button pressed to dismiss the dialog box
//@parmopt HWND | hWndParent | NULL | Parent window handle 
//@comm Invoke the dialog box by calling one of the File Dialog box APIs
//
//@mfunc CWindow | CFileDialogImpl | GetFileDialogWindow | Get the dialog's window
//@rdesc Returns the dialog's window
//@comm Call this function to retrieve the window handle of the file dialog. 
//
//@mfunc LPCTSTR | CFileDialogImpl | GetFileName | Get the name of the file
//@rdesc Returns the selected file name 
//@comm Call this function to retrieve the name of the filename entered in the 
// dialog box. The name of the file includes both the prefix and the extension. 
// For example, GetFileName will return "TEXT.DAT" for the file C:\FILES\TEXT.DAT.
//
//@mfunc LPCTSTR | CFileDialogImpl | GetFileTitle | Get the title of the file
//@rdesc Returns the selected file title.
//@comm Call this function to retrieve the title of the file entered 
// in the dialog box. The title of the file includes only its prefix, without 
// the path or the extension. For example, GetFileTitle will return "TEXT" 
// for the file C:\FILES\TEXT.DAT.
//
//@mfunc int | CFileDialogImpl | GetFilePath | Get the whole path of the file
//@rdesc Returns the selected file path
//@parm LPTSTR | lpstrFilePath | Path of the file
//@parm int | nLength | Length of the path
//@comm Call this function to retrieve the full path of the file entered in the 
// dialog box. The path of the filename includes the file’s title plus the entire 
// directory path. For example, GetPathName will return "C:\FILES\TEXT.DAT"
// for the file C:\FILES\TEXT.DAT.
//
//@mfunc int | CFileDialogImpl | GetFolderIDList | Get the whole path of the file
//@rdesc Returns the selected file path
//@parm LPVOID | lpBuff | Buffer to hold the list
//@parm int | nLength | Length of the list
//@comm retrieves the address of the item identifier list corresponding to the 
// folder that an Explorer-style Open or Save As common dialog box currently has open.
//
//@mfunc int | CFileDialogImpl | GetFolderPath | Get the whole path of the folder
//@rdesc Returns the length of selected folder path
//@parm LPTSTR | lpstrFolderPath | Path of the folder
//@parm int | nLength | Length of the path
//@comm Retrieves the path of the currently open folder or directory 
//
//@mfunc int | CFileDialogImpl | GetSpec | Get the spec of the folder
//@rdesc Returns the length of selected folder path
//@parm LPTSTR | lpstrSpec | Path of the folder
//@parm int | nLength | Length of the spec
//@comm Retrieves the file name (not including the path) of the currently selected file 
//
//@mfunc int | CFileDialogImpl | SetControlText | Set the text for a control 
//@parm int | nCtrlID | ID of the control to set
//@parm LPTSTR | lpstrText | Text to set
//@comm Sets the text for the specified control 
//
//@mfunc int | CFileDialogImpl | SetDefExt | Set the default extension
//@parm LPTSTR | lpstrText | Text to set
//@comm Message sets the default file name extension 
//
//@mfunc BOOL | CFileDialogImpl | GetReadOnlyPref | Get the read-only preference
//@rdesc TRUE if the file is read-only, file otherwise. 
//@comm 
//
//@mfunc void | CFileDialogImpl | HideControl | Hide a control on the dialog box
//@parm int | nCtrlID | Control to hide
//@comm 
//
//@mfunc BOOL | CFileDialogImpl | EndDialog | End the dialog 
//@rdesc TRUE if the file is read-only, file otherwise. 
//@parm int | nRetCode | Code to return to caller 
//@comm 
//
//@base public | CDialogImplBase
//
//@comm Base class for SFL's common dialogs
//
//@class CFileDialogImpl | This class represents the base class for SFL's common
// file dialog boxes. You set up the common file dialog box with a starting path, 
// a default file extension, and file extension filters and then show the dialog box.
// 
// The dialog box shows a tree control allowing the user to find the file. 
//
// CFileDialogImpl includes the following member functions 
//  <mf CFileDialogImpl::CFileDialogImpl>,
//  <mf CFileDialogImpl::DoModal>, 
//  <mf CFileDialogImpl::GetFileDialogWindow>. 
//  <mf CFileDialogImpl::GetFileName>,
//  <mf CFileDialogImpl::GetFileTitle>,
//  <mf CFileDialogImpl::GetFilePath>,
//  <mf CFileDialogImpl::GetFolderIDList>,
//  <mf CFileDialogImpl::GetFolderPath>,
//  <mf CFileDialogImpl::GetSpec>,
//  <mf CFileDialogImpl::SetControlText>,
//  <mf CFileDialogImpl::SetDefExt>,
//  <mf CFileDialogImpl::GetReadOnlyPref>,
//  <mf CFileDialogImpl::HideControl>, and 
//  <mf CFileDialogImpl::EndDialog>
//
//@tcarg typename | _Derived | The ultimately-derived class
//
//@comm SFL contains implementations of the common file management dialog boxes. 
// Both <c COpenFileDialog> and <c CSaveAsFileDialog> inherit from CFileDialogImpl. 
// CFileDialogImpl serves as the base class for SFL's common file dialog boxes. 
// CFileDialogImpl Inherits from ATL's CDialogImplBase and so has the 
// normal dialog box functionality. CFileDialogImpl has a member variable of 
// type OPENFILENAME named m_ofn, which the file open and file save dialog boxes 
// use to pass to the file dialog API functions. In addition, CFileDialogImpl has a 
// boolean member variable named m_bOpenFileDialog. DoModal simply checks this flag to 
// decide whether to call GetOpenFileName or GetSaveFileName API functions. 
//
template <typename _Derived>
class ATL_NO_VTABLE CFileDialogImpl : 
	public CDialogImplBase
{
public:
	OPENFILENAME m_ofn;
	bool m_bOpenFileDialog;				// true for file open, false for file save
	TCHAR m_szFileTitle[_MAX_FNAME];	// contains file title after return
	TCHAR m_szFileName[_MAX_PATH];		// contains full path name after return

	//@cmember
	/* Constructor */
	CFileDialogImpl (
		bool bOpenFileDialog, // true for FileOpen, false for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFilter = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
#if (_WIN32_WINNT >= 0x0500)
		DWORD dwExFlags = OFN_EX_NOPLACESBAR,
#else
		DWORD dwExFlags = 0,
#endif
		HWND hWndParent = NULL
	)
	{
		memset(&m_ofn, 0, sizeof(m_ofn)); // initialize structure to 0/NULL
		m_szFileName[0] = '\0';
		m_szFileTitle[0] = '\0';

		m_bOpenFileDialog = bOpenFileDialog;

		m_ofn.lStructSize = sizeof(m_ofn);
		m_ofn.lpstrFile = m_szFileName;
		m_ofn.nMaxFile = _MAX_PATH;
		m_ofn.lpstrDefExt = lpszDefExt;
		m_ofn.lpstrFileTitle = (LPTSTR)m_szFileTitle;
		m_ofn.nMaxFileTitle = _MAX_FNAME;
		m_ofn.Flags |= dwFlags | OFN_EXPLORER | OFN_ENABLEHOOK | OFN_ENABLESIZING;
#if (_WIN32_WINNT >= 0x0500)
		m_ofn.FlagsEx |= dwExFlags;
#else
		dwExFlags;	// Avoid "unreferenced formal parameter" warning.
#endif // (_WIN32_WINNT >= 0x0500)

		m_ofn.lpstrFilter = lpszFilter;
		m_ofn.hInstance = _Module.GetResourceInstance();
		m_ofn.lpfnHook = (LPOFNHOOKPROC)_Derived::StartDialogProc;
		m_ofn.hwndOwner = hWndParent;

		// setup initial file name
		if (lpszFileName != NULL) {
			lstrcpyn(m_szFileName, lpszFileName, _MAX_PATH);
		}
	}

	//@cmember
	/* Invoke the dialog box */
	int DoModal (
		HWND hWndParent = ::GetActiveWindow()
	)
	{
		ATLASSERT(m_ofn.Flags & OFN_ENABLEHOOK);
		ATLASSERT(m_ofn.lpfnHook != NULL);
		ATLASSERT(m_ofn.Flags & OFN_EXPLORER);

		if (m_ofn.hwndOwner == NULL) {
			m_ofn.hwndOwner = hWndParent;
		}

		ATLASSERT(m_hWnd == NULL);
		_Module.AddCreateWndData(&m_thunk.cd, static_cast<CDialogImplBase*>(this));

		int nResult;
		if (m_bOpenFileDialog) {
			nResult = ::GetOpenFileName(&m_ofn);
		}
		else {
			nResult = ::GetSaveFileName(&m_ofn);
		}
		m_hWnd = NULL;
		return nResult ? nResult : IDCANCEL;
	}

	//@cmember
	/* Get the dialog's window handle */
	CWindow GetFileDialogWindow() const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		return CWindow(GetParent());
	}

	//@cmember
	/* Get the selected file name */	
	LPCTSTR GetFileName() const
	{
		return m_szFileName;
	}

	//@cmember
	/* Get the selected file title  */	
	LPCTSTR GetFileTitle() const
	{
		return m_szFileTitle;
	}

	//@cmember
	/* Get the entire path for the selected file */
	int GetFilePath(LPTSTR lpstrFilePath, int nLength) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(m_ofn.Flags & OFN_EXPLORER);

		return (int)GetFileDialogWindow().SendMessage(CDM_GETFILEPATH, nLength, (LPARAM)lpstrFilePath);
	}

	//@cmember
	/* Get the IDs of the folders*/
	int GetFolderIDList(LPVOID lpBuff, int nLength) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(m_ofn.Flags & OFN_EXPLORER);

		return (int)GetFileDialogWindow().SendMessage(CDM_GETFOLDERIDLIST, nLength, (LPARAM)lpBuff);
	}

	//@cmember
	/* Get the selected folder's path */
	int GetFolderPath(LPTSTR lpstrFolderPath, int nLength) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(m_ofn.Flags & OFN_EXPLORER);

		return (int)GetFileDialogWindow().SendMessage(CDM_GETFOLDERPATH, nLength, (LPARAM)lpstrFolderPath);
	}

	//@cmember
	/* Retrieves the file name (not including the path) of the currently selected file */
	int GetSpec(LPTSTR lpstrSpec, int nLength) const
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(m_ofn.Flags & OFN_EXPLORER);

		return (int)GetFileDialogWindow().SendMessage(CDM_GETSPEC, nLength, (LPARAM)lpstrSpec);
	}

	//@cmember
	/* Set the text for a specific control */
	void SetControlText(int nCtrlID, LPCTSTR lpstrText)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(m_ofn.Flags & OFN_EXPLORER);

		GetFileDialogWindow().SendMessage(CDM_SETCONTROLTEXT, nCtrlID, (LPARAM)lpstrText);
	}

	//@cmember
	/* Set the default extension */
	void SetDefExt(LPCTSTR lpstrExt)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(m_ofn.Flags & OFN_EXPLORER);

		GetFileDialogWindow().SendMessage(CDM_SETDEFEXT, 0, (LPARAM)lpstrExt);
	}

	//@cmember
	/* Find out if the files include read-only files. */
	BOOL GetReadOnlyPref() const	
	{
		// return TRUE if readonly checked
		return m_ofn.Flags & OFN_READONLY ? TRUE : FALSE;
	}

	//@cmember
	/* Hide a specific control */
	void HideControl(int nCtrlID)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		ATLASSERT(m_ofn.Flags & OFN_EXPLORER);

		GetFileDialogWindow().SendMessage(CDM_HIDECONTROL, nCtrlID);
	}

// Special override for common dialogs
	//@cmember
	/* End the dialog  */
	BOOL EndDialog(int nRetCode = 0)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		GetFileDialogWindow().SendMessage(WM_COMMAND, MAKEWPARAM(IDCANCEL, 0));
		return TRUE;
	}

// Message map and handlers
	BEGIN_MSG_MAP(CFileDialogImpl< T >)
		NOTIFY_CODE_HANDLER(CDN_FILEOK, _OnFileOK)
		NOTIFY_CODE_HANDLER(CDN_FOLDERCHANGE, _OnFolderChange)
		NOTIFY_CODE_HANDLER(CDN_HELP, _OnHelp)
		NOTIFY_CODE_HANDLER(CDN_INITDONE, _OnInitDone)
		NOTIFY_CODE_HANDLER(CDN_SELCHANGE, _OnSelChange)
		NOTIFY_CODE_HANDLER(CDN_SHAREVIOLATION, _OnShareViolation)
		NOTIFY_CODE_HANDLER(CDN_TYPECHANGE, _OnTypeChange)
	END_MSG_MAP()

	LRESULT _OnFileOK(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		_Derived* pDerived = static_cast<_Derived*>(this);
		return !pDerived->OnFileOK((LPOFNOTIFY)pnmh);
	}
	LRESULT _OnFolderChange(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		_Derived* pDerived = static_cast<_Derived*>(this);
		pDerived->OnFolderChange((LPOFNOTIFY)pnmh);
		return 0;
	}
	LRESULT _OnHelp(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		_Derived* pDerived = static_cast<_Derived*>(this);
		pDerived->OnHelp((LPOFNOTIFY)pnmh);
		return 0;
	}
	LRESULT _OnInitDone(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		_Derived* pDerived = static_cast<_Derived*>(this);
		pDerived->OnInitDone((LPOFNOTIFY)pnmh);
		return 0;
	}
	LRESULT _OnSelChange(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		_Derived* pDerived = static_cast<_Derived*>(this);
		pDerived->OnSelChange((LPOFNOTIFY)pnmh);
		return 0;
	}
	LRESULT _OnShareViolation(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		_Derived* pDerived = static_cast<_Derived*>(this);
		return pDerived->OnShareViolation((LPOFNOTIFY)pnmh);
	}
	LRESULT _OnTypeChange(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		_Derived* pDerived = static_cast<_Derived*>(this);
		pDerived->OnSelChange((LPOFNOTIFY)pnmh);
		return 0;
	}

	// Overridables

	BOOL OnFileOK(LPOFNOTIFY /*lpon*/)
	{
		return TRUE;
	}
	void OnFolderChange(LPOFNOTIFY /*lpon*/)
	{
	}
	void OnHelp(LPOFNOTIFY /*lpon*/)
	{
	}
	void OnInitDone(LPOFNOTIFY /*lpon*/)
	{
	}
	void OnSelChange(LPOFNOTIFY /*lpon*/)
	{
	}
	int OnShareViolation(LPOFNOTIFY /*lpon*/)
	{
		return 0;
	}
	void OnTypeChange(LPOFNOTIFY /*lpon*/)
	{
	}
};


//@doc COpenFileDialog
//
//@class COpenFileDialog | This class presents the standard Windows Open file dialog box
// by calling GetOpenFileName on your behalf.
//
// COpenFileDialog includes the following member functions 
//  <mf COpenFileDialog::COpenFileDialog>,
//
//@mfunc | COpenFileDialog | COpenFileDialog | Constructor
//@parmopt LPCTSTR | lpszDefExt | NULL | Default extension
//@parmopt LPCTSTR | lpszFilter | NULL | Filter for file name extenstions,
//@parmopt LPCTSTR | lpszFileName | NULL | File Name 
//@parmopt DWORD | dwFlags | OFN_HIDEREADONLY + OFN_OVERWRITEPROMPT | Hide or show certain files.
//@parmopt DWORD | dwExFlags | OFN_EX_NOPLACESBAR | Extended Windows 2000 flags. 
//@parmopt HWND | hWndParent | NULL | Parent window handle 
//
//@base public | CFileDialogImpl<COpenFileDialog>
//
//
class COpenFileDialog : 
	public CFileDialogImpl<COpenFileDialog>
{
	typedef COpenFileDialog _thisClass;
	typedef CFileDialogImpl<COpenFileDialog> _baseClass;
public:

	//@cmember
	/* Constructor */
	COpenFileDialog (
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFilter = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
#if (_WIN32_WINNT >= 0x0500)
		DWORD dwExFlags = OFN_EX_NOPLACESBAR,
#else
		DWORD dwExFlags = 0,
#endif
		HWND hWndParent = NULL
	)
		: _baseClass(true, lpszDefExt, lpszFilter, lpszFileName, dwFlags, dwExFlags, hWndParent)
	{}


	// override base class map, ignore hook
	DECLARE_EMPTY_MSG_MAP()
};

//@doc CSaveAsFileDialog
//
//@class CSaveFileDialog | This class presents the standard Windows save file dialog box
// by calling GetSaveFileName on your behalf.
//
//@mfunc | CSaveAsFileDialog | CSaveAsFileDialog | Constructor
//@parmopt LPCTSTR | lpszDefExt | NULL | Default extension
//@parmopt LPCTSTR | lpszFilter | NULL | Filter for file name extenstions,
//@parmopt LPCTSTR | lpszFileName | NULL | File Name 
//@parmopt DWORD | dwFlags | OFN_HIDEREADONLY + OFN_OVERWRITEPROMPT | Hide or show certain files.
//@parmopt DWORD | dwExFlags | OFN_EX_NOPLACESBAR | Extended Windows 2000 flags. 
//@parmopt HWND | hWndParent | NULL | Parent window handle 
//@comm 
//
//@base public | CFileDialogImpl<CSaveAsFileDialog>
//
//
class CSaveAsFileDialog : 
	public CFileDialogImpl<CSaveAsFileDialog>
{
	typedef CSaveAsFileDialog _thisClass;
	typedef CFileDialogImpl<CSaveAsFileDialog> _baseClass;
public:

	CSaveAsFileDialog (
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFilter = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT,
#if (_WIN32_WINNT >= 0x0500)
		DWORD dwExFlags = OFN_EX_NOPLACESBAR,
#else
		DWORD dwExFlags = 0,
#endif
		HWND hWndParent = NULL
	)
		: _baseClass(false, lpszDefExt, lpszFilter, lpszFileName, dwFlags, dwExFlags, hWndParent)
	{}


	// override base class map, ignore hook
	DECLARE_EMPTY_MSG_MAP()
};




/////////////////////////////////////////////////////////////////////////////
// CCommonDialogImplBase - base class for other common dialog classes


class ATL_NO_VTABLE CCommonDialogImplBase : 
	public CWindowImplBase
{
public:
	BOOL EndDialog(int /*nRetCode*/ = 0)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		SendMessage(WM_COMMAND, MAKEWPARAM(IDABORT, 0));
		return TRUE;
	}

protected:

	static UINT APIENTRY HookProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg != WM_INITDIALOG) return 0;
		CCommonDialogImplBase* pT = (CCommonDialogImplBase*)_Module.ExtractCreateWndData();
		ATLASSERT(pT != NULL);
		ATLASSERT(pT->m_hWnd == NULL);
		ATLASSERT(::IsWindow(hWnd));
		// subclass dialog's window
		if (!pT->SubclassWindow(hWnd)) {
			ATLTRACE(_T("Subclassing a common dialog failed\n\r"));
			return 0;
		}
		// check message map for WM_INITDIALOG handler
		LRESULT lRes;
		if (pT->ProcessWindowMessage(pT->m_hWnd, uMsg, wParam, lParam, lRes, 0) == FALSE)
			return 0;
		return lRes;
	}

	static LRESULT CALLBACK StartWindowProc(HWND /*hWnd*/, UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		ATLASSERT(FALSE);	// should not be called
		return 0;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CFontDialogImpl - font selection dialog
//
//
//@doc CFontDialogImpl
//
//@mfunc | CFontDialogImpl | CFontDialogImpl | Constructor
//@parmopt LPLOGFONT | lplfInitial | NULL | pointer to logical font
//@parmopt DWORD | dwFlags | CF_EFFECTS + CF_SCREENFONTS,| What kind of fonts to use?
//@parmopt HDC | hDCPrinter | NULL | Printer DC
//@parmopt HWND | hWndParent | NULL | Parent window handle 
//
//@mfunc int | CFontDialogImpl | DoModal | Invoke the dialog
//@rdesc Returns the value of the button pressed to dismiss the dialog box
//@parmopt HWND | hWndParent | NULL | Parent window handle 
//
//@mfunc void | CFontDialogImpl | GetCurrentFont | Get the currently selected font
//@parm LPLOGFONT | lplf | Pointer to logical font structure 
//
//@mfunc LPCTSTR | CFontDialogImpl | GetFaceName | Get the name of the font face like Ariel
// or Courier)
//@rdesc Returns the selected face name 
//
//@mfunc LPCTSTR | CFontDialogImpl | GetStyleName | Get the name of the style of the 
// currently selected font. 
//@rdesc Returns the selected style name.
//
//@mfunc int | CFontDialogImpl | GetSize | Get the point size of the font
//@rdesc Returns the point size of the selected font
//
//@mfunc COLORREF | CFontDialogImpl | GetColor | Get the color of the currently-selected font
//@rdesc Returns the color of the selected font
//
//@mfunc int | CFontDialogImpl | GetWeight | Get the weight of the currently-selected font
//@rdesc Returns the weight of selected	font.
//
//@mfunc BOOL | CFontDialogImpl | IsStrikeOut | Is the font a strike-out font?
//@rdesc Returns TRUE if the font is a strike-out font, FALSE otherwise
//
//@mfunc BOOL | CFontDialogImpl | IsUnderline | Is the font a underline font?
//@rdesc Returns TRUE if the font is an underline font, FALSE otherwise
//
//@mfunc BOOL | CFontDialogImpl | IsItalic | Is the font an italic font?
//@rdesc Returns TRUE if the font is an italic font, FALSE otherwise
//
//@mfunc BOOL | CFontDialogImpl | IsBold | Is the font bold face
//@rdesc Returns TRUE if the font is a bold face font, FALSE otherwise
//
//@class CFontDialogImpl | This class presents the standard Windows font dialog box
// by calling ChooseFont on your behalf.
//
// CFontDialogImpl includes the following member functions 
//  <mf CFontDialogImpl::CFontDialogImpl>, 
//  <mf CFontDialogImpl::DoModal>, 
//  <mf CFontDialogImpl::GetCurrentFont>. 
//  <mf CFontDialogImpl::GetFaceName>,
//  <mf CFontDialogImpl::GetStyleName>, 
//  <mf CFontDialogImpl::GetSize>, 
//  <mf CFontDialogImpl::GetColor>, 
//  <mf CFontDialogImpl::GetWeight>, 
//  <mf CFontDialogImpl::IsUnderline>, 
//  <mf CFontDialogImpl::IsStrikeOut>, 
//  <mf CFontDialogImpl::IsItalic>, and
//  <mf CFontDialogImpl::IsBold>
//
//@tcarg typename | _Derived | The ultimately-derived class
//
//@comm The CFontDialogImpl and its finalized implementation <c CFontDialog> wrap the standard Windows font-selection dialog box 
// into your application. A CFontDialog object is a dialog box with a list of fonts 
// that are currently installed in the system. The user can select a particular font 
// from the list.
// 
//@comm To construct a CFontDialog object, use the provided constructor 
// or derive a new subclass and use your own custom constructor. 
//
//@comm The CFontDialog contains a data member of type CHOOSEFONT named m_cf. Once a 
// CFontDialog object has been constructed, you can use the m_cf structure to 
// initialize the values or states of controls in the dialog box. For more information 
// on this structure, see the Win32 SDK documentation. 
//
//@comm After initializing the dialog object's controls, call the DoModal 
// member function to display the dialog box and allow the user to select a font. 
// DoModal calls the standard Widnows ChooseFont API function. DoModal returns whether 
// the user selected the OK (IDOK) or Cancel (IDCANCEL) button.
// 
//@comm If DoModal returns IDOK, you can use one of CFontDialog's member functions 
// to retrieve the information input by the user. You may also use m_cf directly. 
//
//@base public | CCommonDialogImplBase
//
//
//
template <typename _Derived>
class ATL_NO_VTABLE CFontDialogImpl : public CCommonDialogImplBase
{
public:
	CHOOSEFONT m_cf;
	TCHAR m_szStyleName[64];	// contains style name after return
	LOGFONT m_lf;			// default LOGFONT to store the info

// Constructors
	CFontDialogImpl(LPLOGFONT lplfInitial = NULL,
			DWORD dwFlags = CF_EFFECTS | CF_SCREENFONTS,
			HDC hDCPrinter = NULL,
			HWND hWndParent = NULL)
	{
		memset(&m_cf, 0, sizeof(m_cf));
		memset(&m_lf, 0, sizeof(m_lf));
		memset(&m_szStyleName, 0, sizeof(m_szStyleName));

		m_cf.lStructSize = sizeof(m_cf);
		m_cf.hwndOwner = hWndParent;
		m_cf.rgbColors = RGB(0, 0, 0);
		m_cf.lpszStyle = (LPTSTR)&m_szStyleName;
		m_cf.Flags = dwFlags | CF_ENABLEHOOK;
		m_cf.lpfnHook = (LPCFHOOKPROC)_Derived::HookProc;

		if (lplfInitial != NULL) {
			m_cf.lpLogFont = lplfInitial;
			memcpy(&m_lf, m_cf.lpLogFont, sizeof(m_lf));
		}
		else {
			m_cf.lpLogFont = &m_lf;
		}

		if (hDCPrinter != NULL) {
			m_cf.hDC = hDCPrinter;
			m_cf.Flags |= CF_PRINTERFONTS;
		}
	}

// Operations
	int DoModal(HWND hWndParent = ::GetActiveWindow())
	{
		ATLASSERT(m_cf.Flags & CF_ENABLEHOOK);
		ATLASSERT(m_cf.lpfnHook != NULL);	// can still be a user hook

		if(m_cf.hwndOwner == NULL)		// set only if not specified before
			m_cf.hwndOwner = hWndParent;

		ATLASSERT(m_hWnd == NULL);
		_Module.AddCreateWndData(&m_thunk.cd, (CCommonDialogImplBase*)this);

		int nResult = ::ChooseFont(&m_cf);

		m_hWnd = NULL;

		if(nResult == IDOK)
		{
			// copy logical font from user's initialization buffer (if needed)
			memcpy(&m_lf, m_cf.lpLogFont, sizeof(m_lf));
			return IDOK;
		}

		return nResult ? nResult : IDCANCEL;
	}

	// Get the selected font (works during DoModal displayed or after)
	void GetCurrentFont(LPLOGFONT lplf) const
	{
		ATLASSERT(lplf != NULL);

		if(m_hWnd != NULL)
			::SendMessage(m_hWnd, WM_CHOOSEFONT_GETLOGFONT, 0, (DWORD)(LPVOID)lplf);
		else
			*lplf = m_lf;
	}

	// Helpers for parsing information after successful return
	LPCTSTR GetFaceName() const   // return the face name of the font
	{
		return (LPCTSTR)m_cf.lpLogFont->lfFaceName;
	}
	LPCTSTR GetStyleName() const  // return the style name of the font
	{
		return m_cf.lpszStyle;
	}
	int GetSize() const           // return the pt size of the font
	{
		return m_cf.iPointSize;
	}
	COLORREF GetColor() const     // return the color of the font
	{
		return m_cf.rgbColors;
	}
	int GetWeight() const         // return the chosen font weight
	{
		return (int)m_cf.lpLogFont->lfWeight;
	}
	BOOL IsStrikeOut() const      // return TRUE if strikeout
	{
		return m_cf.lpLogFont->lfStrikeOut ? TRUE : FALSE;
	}
	BOOL IsUnderline() const      // return TRUE if underline
	{
		return m_cf.lpLogFont->lfUnderline ? TRUE : FALSE;
	}
	BOOL IsBold() const           // return TRUE if bold font
	{
		return m_cf.lpLogFont->lfWeight == FW_BOLD ? TRUE : FALSE;
	}
	BOOL IsItalic() const         // return TRUE if italic font
	{
		return m_cf.lpLogFont->lfItalic ? TRUE : FALSE;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CFontDialog - font selection dialog
//
//@doc CFontDialog
//
//@class CFontDialog | This class is a complete finalized implementation of the font dialog
//
//@base public | CFontDialogImpl<lt>CFontDialog<gt>
//
//@comm This is a convenient finalized implementation of the font dialog
//
class CFontDialog : public CFontDialogImpl<CFontDialog>
{
public:
	CFontDialog(LPLOGFONT lplfInitial = NULL,
		DWORD dwFlags = CF_EFFECTS | CF_SCREENFONTS,
		HDC hDCPrinter = NULL,
		HWND hWndParent = NULL)
		: CFontDialogImpl<CFontDialog>(lplfInitial, dwFlags, hDCPrinter, hWndParent)
	{ }

	DECLARE_EMPTY_MSG_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CColorDialogImpl - color selection
//
//@doc CColorDialogImpl | Shows the standard Windows color dialog box
//
//@mfunc | CColorDialogImpl | CColorDialogImpl | Constructor
//@parm COLORREF | clrInit = 0 | Initial color selected 
//@parm DWORD | dwFlags = 0,| Color selection flags
//@parm HWND | hWndParent = NULL | Parent window handle 
//@comm 
//
//@mfunc int | CColorDialogImpl | DoModal | Invoke the dialog
//@rdesc Returns the value of the button pressed to dismiss the dialog box
//@parm HWND | hWndParent = NULL | Parent window handle 
//@comm 
//
//@mfunc void | CColorDialogImpl | SetCurrentColor | Set the current color
//@parm COLORREF | clr | Color to set
//@comm 
//
//@mfunc COLORREF | CColorDialogImpl | GetColor | Get the currently selected color
//@rdesc Returns the selected color 
//@comm 
//
//@mfunc COLORREF* | CColorDialogImpl | GetCustomColors | Get the array of custom colors created by teh user
//@rdesc Returns a pointer to an array of COLORREFs
//@comm 
//
//@class CColorDialogImpl | This class presents the standard Windows color dialog box
// by calling ChooseColor on your behalf.
//
// CColorDialogImpl includes the following member functions 
//  <mf CColorDialogImpl::CColorDialogImpl>, 
//  <mf CColorDialogImpl::DoModal>, 
//  <mf CColorDialogImpl::GetCurrentColor>. 
//  <mf CColorDialogImpl::GetColor>, and
//  <mf CColorDialogImpl::GetCustomColors>, 
//
//@tcarg typename | _Derived | The ultimately-derived class
//
//@base public | CCommonDialogImplBase
//
//@comm The CColorDialogImpl and <c CColorDialog> class wraps the standard Windows color-selection dialog box. 
// A CColorDialog object is a dialog box with a list of colors that are defined for the 
// display system. The user can select or create a particular color from the list, 
// which is then reported back to the application when the dialog box exits.
//
//@comm To construct a CColorDialog object, use the provided constructor or derive a 
// new class and use your own custom constructor. 
//
//@comm The CColorDialog has a member variable of type CHOOSECOLOR named m_cc. Once the 
// dialog box has been constructed, you can set or modify any values in the m_cc 
// structure to initialize the values of the dialog box's controls. 
//
//@comm After initializing the dialog box's controls, call the DoModal member function 
// to display the dialog box and allow the user to select a color. DoModal simply calls 
// the ChooseColor Windows API to show the standard color dialog box.
//
//@comm If DoModal returns IDOK, CColorDialog's m_cc member contains the information 
// input by the user.
//
template <typename _Derived>
class ATL_NO_VTABLE CColorDialogImpl : 
	public CCommonDialogImplBase
{
public:
	CHOOSECOLOR m_cc;

// Constructors
	CColorDialogImpl(COLORREF clrInit = 0, DWORD dwFlags = 0, HWND hWndParent = NULL)
	{
		memset(&m_cc, 0, sizeof(m_cc));

		m_cc.lStructSize = sizeof(m_cc);
		m_cc.lpCustColors = GetCustomColors();
		m_cc.hwndOwner = hWndParent;
		m_cc.Flags = dwFlags | CC_ENABLEHOOK;
		m_cc.lpfnHook = reinterpret_cast<LPCCHOOKPROC>(_Derived::HookProc);

		if (clrInit != 0) {
			m_cc.rgbResult = clrInit;
			m_cc.Flags |= CC_RGBINIT;
		}
	}

// Operations
	int DoModal(HWND hWndParent = ::GetActiveWindow())
	{
		ATLASSERT(m_cc.Flags & CC_ENABLEHOOK);
		ATLASSERT(m_cc.lpfnHook != NULL);	// can still be a user hook

		if (m_cc.hwndOwner == NULL)	{
			// set only if not specified before
			m_cc.hwndOwner = hWndParent;
		}

		ATLASSERT(m_hWnd == NULL);
		_Module.AddCreateWndData(&m_thunk.cd, (CCommonDialogImplBase*)this);

		int nResult = ::ChooseColor(&m_cc);

		m_hWnd = NULL;

		return nResult ? nResult : IDCANCEL;
	}

	// Set the current color while dialog is displayed
	void SetCurrentColor(COLORREF clr)
	{
		ATLASSERT(::IsWindow(m_hWnd));
		SendMessage(_nMsgSETRGB, 0, (DWORD)clr);
	}

	// Helpers for parsing information after successful return
	COLORREF GetColor() const
	{
		return m_cc.rgbResult;
	}

	static COLORREF* GetCustomColors()
	{
		static COLORREF rgbCustomColors[16] =
		{
			RGB(255, 255, 255), RGB(255, 255, 255), 
			RGB(255, 255, 255), RGB(255, 255, 255), 
			RGB(255, 255, 255), RGB(255, 255, 255), 
			RGB(255, 255, 255), RGB(255, 255, 255), 
			RGB(255, 255, 255), RGB(255, 255, 255), 
			RGB(255, 255, 255), RGB(255, 255, 255), 
			RGB(255, 255, 255), RGB(255, 255, 255), 
			RGB(255, 255, 255), RGB(255, 255, 255), 
		};

		return rgbCustomColors;
	}


public:
// Registered messages
	static const UINT nMsgColorOk;
	static const UINT nMsgSetRGB;

public:
// Overridable callbacks
	BEGIN_MSG_MAP(CColorDialogImpl< T >)
		MESSAGE_HANDLER(nMsgColorOk, _OnColorOK)
	END_MSG_MAP()

	LRESULT _OnColorOK(UINT, WPARAM, LPARAM, BOOL&)
	{
		_Derived* pDerived = static_cast<_Derived*>(this);
		return pDerived->OnColorOK();
	}

	BOOL OnColorOK()        // validate color
	{
		return FALSE;
	}
};


template <typename _Derived>
const UINT CColorDialogImpl<_Derived>::nMsgColorOk = ::RegisterWindowMessage(COLOROKSTRING);
template <typename _Derived>
const UINT CColorDialogImpl<_Derived>::nMsgSetRGB = ::RegisterWindowMessage(SETRGBSTRING);

///////////////////////////////////////////////////////////////////////////
//
//@doc CColorDialog
//
//@class CColorDialog | This class is a complete finalized implementation of the color dialog
//
//@base public | CColorDialogImpl
//
//@comm Convenient finalized implementation of the color selection dialog box
//
class CColorDialog : public CColorDialogImpl<CColorDialog>
{
public:
	CColorDialog(COLORREF clrInit = 0, DWORD dwFlags = 0, HWND hWndParent = NULL)
		: CColorDialogImpl<CColorDialog>(clrInit, dwFlags, hWndParent)
	{ }

	// override base class map and references to handlers
	DECLARE_EMPTY_MSG_MAP()
};

///////////////////////////////////////////////////////////////////////////
//
//@doc CFindDialogImpl 
//
//@mfunc virtual void | CFindDialogImpl | OnFinalMessage | Perform any cleanup here
//@parm HWND | hWnd | Window handle receiving the final message
//@comm Most of the time Find Dialogs are created on the heap and destroy themselves on
// the final message. If you decide to put the dialog box on the stack, be sure to override
// this function (the default version delete the object this pointer, which as you know
// results in painful consequences. 
//
//@mfunc HWND | CFindDialogImpl | Create | Create the find dialog box
//@rdesc Returns the dialog's window handle
//@parm LPCTSTR | lpszFindWhat | String to search for
//@parm DWORD | dwFlags = FR_DOWN | Creation flags
//@parm HWND | hWnd | Window handle of the parent.
//
//@mfunc static _Derived* PASCAL | CFindDialogImpl | GetNotifier | This is called while handling the FINDMSGSTRING registered message
// to retreive the object
//@rdesc Returns a pointer to the most derived class
//@parm LPARAM | lParam | User defined parameter
//
//@mfunc LPCTSTR | CFindDialogImpl | GetFindString | Get the string to find
//@rdesc Returns the selected string being searched for 
//
//@mfunc BOOL | CFindDialogImpl | SearchDown | Direction of search
//@rdesc Returns TRUE if searching down hte document, FALSE if searching up
//
//@mfunc BOOL | CFindDialogImpl | FindNext | Is command "find next"?
//@rdesc Returns TRUE if command is "find next", FALSE otherwise
//
//@mfunc BOOL | CFindDialogImpl | MatchCase | Should search match case?
//@rdesc Returns TRUE if the search should match case, FALSE otherwise
//
//@mfunc BOOL | CFindDialogImpl | MatchWholeWord | Should the match search for the whole word?
//@rdesc Returns TRUE if the match should search for the whole word, FALSE otherwise
//
//@mfunc void | CFindDialogImpl | PrepareCreation | Prepare the dialog for creation
//@parm DWORD | dwFlags | Creation flags 
//@parm HWND | hWndParent = NULL | Window handle of parent 
//
//@base public | CDialogImplBase
//
//@comm Encapsulates the standard find dialog functionality  
// The <c CFindDialog> lets you type in a string for which to search. The dialog box then calls back to the parent window every time the user clicks on the "Find" button. The application then takes the string typed in by the user and tries to find it within the document. 
//
//@class CFindDialogImpl | This class encapsulates the standard FindDialog. 
//
// CFindDialogImpl includes the following member functions 
//  <mf CFindDialogImpl::OnFinalMessage>. 
//  <mf CFindDialogImpl::Create>,
//  <mf CFindDialogImpl::GetNotifier>,
//  <mf CFindDialogImpl::GetFindString>,
//  <mf CFindDialogImpl::SearchDown>,
//  <mf CFindDialogImpl::FindNext>,
//  <mf CFindDialogImpl::MatchCase>, and 
//  <mf CFindDialogImpl::MatchWholeWord>,
//
//@tcarg typename | _Derived | The ultimately-derived class
//
template <typename _Derived>
class ATL_NO_VTABLE CFindDialogImpl : 
	public CCommonDialogImplBase,
	public CDialogMessagePreTranslator<_Derived>
{
public:
	FINDREPLACE m_fr;
	TCHAR m_szFindWhat[128];

	//@cmember
	/* Constructor */
	CFindDialogImpl()
	{
		memset(&m_fr, 0, sizeof(m_fr));
		m_szFindWhat[0] = '\0';

		m_fr.lStructSize = sizeof(m_fr);
		m_fr.Flags = FR_ENABLEHOOK;
		m_fr.lpfnHook = reinterpret_cast<LPFRHOOKPROC>(_Derived::HookProc);
		m_fr.lpstrFindWhat = m_szFindWhat;
		m_fr.wFindWhatLen = sizeof(m_szFindWhat)/sizeof(TCHAR);
	}

	//@cmember
	/* Final message handler */
	virtual void OnFinalMessage(HWND hWnd)
	{
		hWnd;
		// Note: You must allocate the object on the heap.
		//       If you do not, you must override OnFinalMessage()
		_Module.GetMessageLoop()->RemoveMessagePreTranslator(this);
		delete this;
	}

	//@cmember
	/* Create the find dialog */
	HWND Create (
		LPCTSTR lpszFindWhat,
		DWORD dwFlags = FR_DOWN,
		HWND hWndParent = NULL)
	{
		PrepareCreation(dwFlags, hWndParent);
		if (lpszFindWhat != NULL) {
			lstrcpyn(m_szFindWhat, lpszFindWhat, sizeof(m_szFindWhat)/sizeof(TCHAR));
		}

		HWND hWnd;
		hWnd = ::FindText(&m_fr);

		ATLASSERT(m_hWnd == hWnd);
		return hWnd;
	}

	//@cmember
	/* Get notification class. Called while handling FINDMSGSTRING message */
	static _Derived* PASCAL GetNotifier(LPARAM lParam)
	{
		ATLASSERT(lParam != NULL);
		_Derived* pDlg = reinterpret_cast<_Derived*>(lParam - offsetof(_Derived, m_fr));
		return pDlg;
	}

	//@cmember
	/* Get the string being searched for. */
	LPCTSTR GetFindString() const
	{
		return m_fr.lpstrFindWhat;
	}

	//@cmember
	/* Searching down or up */
	BOOL SearchDown() const
	{
         // TRUE if search down, FALSE is up
		return m_fr.Flags & FR_DOWN ? TRUE : FALSE;
	}

	//@cmember
	/* Find the next item. */
	BOOL FindNext() const            
	{
		// TRUE if command is find next
		return m_fr.Flags & FR_FINDNEXT ? TRUE : FALSE;
	}

	//@cmember
	/* Match case */
	BOOL MatchCase() const           
	{
		// TRUE if matching case
		return m_fr.Flags & FR_MATCHCASE ? TRUE : FALSE;
	}

	//@cmember
	/* Match whole word */
	BOOL MatchWholeWord() const      
	{
		// TRUE if matching whole words only
		return m_fr.Flags & FR_WHOLEWORD ? TRUE : FALSE;
	}

	//@cmember
	/* Is the dialog terminating  */
	BOOL IsTerminating() const       
	{
		// TRUE if terminating dialog
		return m_fr.Flags & FR_DIALOGTERM ? TRUE : FALSE ;
	}

public:
	static const UINT nMsgFindReplace;


protected:
	static UINT APIENTRY HookProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		UINT lres = CCommonDialogImplBase::HookProc(hWnd, uMsg, wParam, lParam);
		if (uMsg == WM_INITDIALOG) {
			// Show the dialog box
			lres = TRUE;
		}
		return lres;
	}

protected:
	//@cmember
	/* Prepare the dialog box for creation   */
	void PrepareCreation (
		DWORD dwFlags,
		HWND hWndParent = NULL
	)
	{
		ATLASSERT(m_fr.Flags & FR_ENABLEHOOK);
		ATLASSERT(m_fr.lpfnHook != NULL);

		m_fr.Flags |= dwFlags;

		if (hWndParent == NULL) {
			m_fr.hwndOwner = ::GetActiveWindow();
		}
		else {
			m_fr.hwndOwner = hWndParent;
		}
		ATLASSERT(m_fr.hwndOwner != NULL); // must have an owner for modeless dialog

		// ATL Window creation
		ATLASSERT(m_hWnd == NULL);
		_Module.AddCreateWndData(&m_thunk.cd, (CCommonDialogImplBase*)this);

		// Add to the message pretranslators list
		// in order to IsDialogMessage be called
		_Module.GetMessageLoop()->AddMessagePreTranslator(this);

	}
};

template <typename _Derived>
const UINT CFindDialogImpl<_Derived>::nMsgFindReplace = ::RegisterWindowMessage(FINDMSGSTRING);

///////////////////////////////////////////////////////////////////////////
//@topic CReplaceDialogImpl Class Overview | The CReplaceDialogImpl
// class provides a convenient implementation of the standard Find/Replace
// dialog box. The Find/Replace dialog box is different than the other 
// dialog boxes as they are normally created as modeless dialog boxes. 
//
//@doc CReplaceDialogImpl
//
//@mfunc HWND | CReplaceDialogImpl | Create | Create the find dialog box
//@rdesc Returns the dialog's window handle
//@parm LPCTSTR | lpszFindWhat | String to search for
//@parm LPCTSTR | lpszReplaceWith | String to search for
//@parm DWORD | dwFlags = FR_DOWN | Creation flags
//@parm HWND | hWnd | Window handle of the parent.
//@comm 
//
//@mfunc LPCTSTR | CReplaceDialogImpl | GetReplaceString | Get the string to replace
//@rdesc Returns the replacement string 
//@comm 
//
//@mfunc BOOL | CReplaceDialogImpl | ReplaceCurrent | Replace the current string
//@rdesc Returns TRUE if replacing the current string, FALSE otherwise
//@comm 
//
//@mfunc BOOL | CReplaceDialogImpl | ReplaceAll | Replace all strings found in the document?
//@rdesc Returns TRUE if replacing all strings, FALSE otherwise
//@comm 
//
//@base public | CFindDialogImpl<_Derived>
//
//@comm 
//
//@class CReplaceDialogImpl | Wrapps the replace dialog functionality 
//
// CReplaceDialogImpl includes the following member functions 
//  <mf CReplaceDialogImpl::Create>,
//  <mf CReplaceDialogImpl::GetReplaceString>,
//  <mf CReplaceDialogImpl::ReplaceCurrent>, and
//  <mf CReplaceDialogImpl::ReplaceAll>
//
//@tcarg typename | _Derived | The ultimately-derived class
//
template <typename _Derived>
class CReplaceDialogImpl :
	public CFindDialogImpl<_Derived>
{
	friend class CFindDialogImpl<_Derived>;

public:
	TCHAR m_szReplaceWith[128];

	CReplaceDialogImpl() 
	{
		m_szReplaceWith[0] = '\0';
		m_fr.lpstrReplaceWith = m_szReplaceWith;
		m_fr.wReplaceWithLen = sizeof(m_szReplaceWith)/sizeof(TCHAR);
	}

	//@cmember
	/* Create the dialog dialog */ 
	HWND Create (
		LPCTSTR lpszFindWhat,
		LPCTSTR lpszReplaceWith,
		DWORD dwFlags = FR_DOWN,
		HWND hWndParent = NULL
	)
	{
		PrepareCreation(dwFlags, hWndParent);

		if(lpszFindWhat != NULL) {
			lstrcpyn(m_szFindWhat, lpszFindWhat, sizeof(m_szFindWhat)/sizeof(TCHAR));
		}
		if (lpszReplaceWith != NULL) {
			lstrcpyn(m_szReplaceWith, lpszReplaceWith, sizeof(m_szReplaceWith)/sizeof(TCHAR));
		}
		HWND hWnd;
		hWnd = ::ReplaceText(&m_fr);

		ATLASSERT(m_hWnd == hWnd);
		return hWnd;
	}

	//@cmember
	/* Get the replacement string */ 
	LPCTSTR GetReplaceString() const 
	{
		return m_fr.lpstrReplaceWith;
	}
	//@cmember
	/* Check for replacing the current string */
	BOOL ReplaceCurrent() const      
	{
		// TRUE if replacing current string
		return m_fr.Flags & FR_REPLACE ? TRUE : FALSE;
	}

	//@cmember
	/* Check for replacing all strings */
	BOOL ReplaceAll() const
	{
        // TRUE if replacing all occurrences
		return m_fr.Flags & FR_REPLACEALL ? TRUE : FALSE;
	}
};


///////////////////////////////////////////////////////////////////////////
//@topic CFindDialog Class Overview | The CFindDialog
// class provides a convenient implementation of the standard Find dialog box. 
// CFindDialog provides an instantiable class based upon CFindDialogImpl.
//
//@doc CFindDialog
//
//@class CFindDialog | Wraps the find dialog box functionality 
//
//@base public | CFindDialogImpl<lt>CFindDialog<gt>
//
class CFindDialog : 
	public CFindDialogImpl<CFindDialog>
{
public:
	DECLARE_EMPTY_MSG_MAP()
};

///////////////////////////////////////////////////////////////////////////
//@topic CReplaceDialog Class Overview | The CReplaceDialog
// class provides a convenient implementation of the standard Reolace dialog box. 
// CReplaceDialog provides an instantiable class based upon CReplaceDialogImpl.
//
//@doc CReplaceDialog
//
//@class CReplaceDialog | Convenient implementation of the replace dialog 
//
//@base public | CReplaceDialogImpl<lt>CReplaceDialog<gt>
//
//@comm CReplaceDialog works in the same fashion, except the 
// replace dialog also includes a field for typing a replacement string. 
//
//
class CReplaceDialog :
	public CReplaceDialogImpl<CReplaceDialog>
{
public:
	DECLARE_EMPTY_MSG_MAP()
};


};	// namespace stingray::foundation
};	// namespace stingray
