/////////////////////////////////////////////////////////////////////////////
// DocManager.h : SFL Document management support
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.


#pragma once

#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning(push,3)
	#endif
	#pragma warning(disable:4018)
#endif

#include <foundation\sflres.h>
#include <foundation\string\stringex.h>
#include <foundation\apps\CommonDialogs.h>
#include <iomanip>

namespace stingray {
namespace foundation {


// Helper macros to define the configuration of the file dialogs

#define BEGIN_DOCUMENT_MAP()	\
	UINT* GetDocumentResources() {	\
		static UINT _aDocStrings[] = {	\


#define END_DOCUMENT_MAP()	\
		0x00}; \
		return _aDocStrings; \
	}; \

#define DOCUMENT_CLASS(resourceid)	\
	resourceid,	\

#define INCLUDE_ALL_FILES()	\
	IDS_ALLFILES,	\

#define EMPTY_DOCUMENT_MAP() \
	BEGIN_DOCUMENT_MAP() \
	END_DOCUMENT_MAP()


/////////////////////////////////////////////////////////////////////
// CDocumentManager class
//
//@doc CDocumentManager
//
// CDocumentManager
//
//@doc CDocumentManager
//
//@mfunc | CDocumentManager | CDocumentManager | Constructor
//@comm 
//
//@mfunc LRESULT | CDocumentManager | OnFileOpen | Function called when opening a document
//@rdesc Returns 0
//@parm WORD | w1 | Word parameter not used 
//@parm WORD | w2 | Word parameter not used 
//@parm HWND | hWnd | Window handle not used 
//@parm BOOL& | rBool | Boolean flag not used
//@comm Handled in response to the IDC_SFL_FILE_OPEN command ID. This function displays the 
// file open dialog box.
//
//@mfunc LRESULT | CDocumentManager | OnFileSaveAs | Function called when saving a document
//@rdesc Returns 0
//@parm WORD | w1 | Word parameter not used 
//@parm WORD | w2 | Word parameter not used 
//@parm HWND | hWnd | Window handle not used 
//@parm BOOL& | rBool | Boolean flag not used
//@comm Handled in response to the IDC_SFL_FILE_SAVEAS command ID. This function displays the 
// file SaveAs dialog box.
//
//@mfunc LRESULT | CDocumentManager | OnFileSaveAs | Function called when saving a document
//@rdesc Returns 0
//@parm WORD | w1 | Notification code 
//@parm WORD | w2 | Command ID
//@parm HWND | hWnd | Window handle 
//@parm BOOL& | rBool | Boolean flag 
//@comm Handled in response to the IDC_SFL_FILE_SAVEAS command ID. This function displays the 
// file SaveAs dialog box.
//
//@mfunc LRESULT | CDocumentManager | OnFileSave | Function called when saving a document
//@rdesc Returns 0
//@parm WORD | w1 | Notification code 
//@parm WORD | w2 | Command ID
//@parm HWND | hWnd | Window handle 
//@parm BOOL& | rBool | Boolean flag 
//@comm Handled in response to the IDC_SFL_FILE_SAVE command ID. This function displays the 
// file Save dialog box.
//
//@mfunc LRESULT | CDocumentManager | OnFileClose | Function called when closing a file 
//@rdesc Returns 0
//@parm WORD | w1 | Notification code 
//@parm WORD | w2 | Command ID
//@parm HWND | hWnd | Window handle 
//@parm BOOL& | rBool | Boolean flag 
//@comm Handled in response to the IDC_SFL_FILE_CLOSE command ID. This function displays the 
// file Save dialog box.
//
//
//@mfunc virtual bool | CDocumentManager | OpenDocument | Function for opening a document 
//@rdesc Return true if the document was opened, false otherwise
//@parm LPCTSTR | lpszFileName | File to open
//@comm Called to open a specific document 
//
//@mfunc virtual bool | CDocumentManager | SaveDocument | Function for saving a document 
//@rdesc Return true if the document was saved , false otherwise
//@parm LPCTSTR | lpszFileName | File to save 
//@comm Called to open a specific document 
//
//@mfunc virtual bool | CDocumentManager | CloseDocument | Function for closing a document 
//@rdesc Return true if the document was saved , false otherwise
//@comm Called to close a document 
//
//@class CDocumentManager | Wraps document management 
//
// CDocumentManager includes the following member functions 
//  <mf CDocumentManager::CDocumentManager>,
//  <mf CDocumentManager::OnFileOpen>,
//  <mf CDocumentManager::OnfileSave>,
//  <mf CDocumentManager::OnFileSaveAs>, 
//  <mf CDocumentManager::OnFileClose>,
//  <mf CDocumentManager::OpenDocument>,
//  <mf CDocumentManager::SaveDocument >, and 
//  <mf CDocumentManager::CloseDocument>,
//
//@tcarg typename | _Derived | The ultimately derived class
//@tcarg typename | _OpenDialog = COpenFileDialog | The dialog box to use for opening files 
//@tcarg typename | _SaveDialog = CSaveAsFileDialog | The dialog box to use for saving files 
//
//@base public | CMessageMap
//
//@comm 
//
template <typename _Derived, 
			typename _OpenDialog = COpenFileDialog, 
			typename _SaveDialog = CSaveAsFileDialog>
class CDocumentManager :
	public CMessageMap
{
	typedef CDocumentManager<_Derived> _thisClass;

protected:
	typedef _OpenDialog COpenFileDialog;
	typedef _SaveDialog CSaveAsFileDialog;

public:
	BEGIN_MSG_MAP(CDocumentManager)
        COMMAND_ID_HANDLER(IDC_SFL_FILE_OPEN, OnFileOpen)
        COMMAND_ID_HANDLER(IDC_SFL_FILE_SAVEAS, OnFileSaveAs)
        COMMAND_ID_HANDLER(IDC_SFL_FILE_SAVE, OnFileSave)
        COMMAND_ID_HANDLER(IDC_SFL_FILE_CLOSE, OnFileClose)
	END_MSG_MAP()

	EMPTY_DOCUMENT_MAP()

public:
	// Message handlers

	LRESULT OnFileOpen(WORD, WORD, HWND, BOOL&)
    {
		COpenFileDialog dlg;
		_Derived* pDerived = static_cast<_Derived*>(this);
		pDerived->PrepareDialog(dlg);
		if (dlg.DoModal() != IDCANCEL) {
			OpenDocument(dlg.GetFileName());
		}
		return 0;
	}

	LRESULT OnFileSaveAs(WORD, WORD, HWND, BOOL&)
    {
		CSaveAsFileDialog dlg;
		_Derived* pDerived = static_cast<_Derived*>(this);
		pDerived->PrepareDialog(dlg);
		if (dlg.DoModal() != IDCANCEL) {
			SaveDocument(dlg.GetFileName());
		}
		return 0;
	}

	LRESULT OnFileSave(WORD wNotifyCode, WORD wId, HWND hWnd, BOOL& bHandled)
	{
		if (m_sFileName.empty()) {
			return OnFileSaveAs(wNotifyCode, wId, hWnd, bHandled);
		}
		else {
			SaveDocument(m_sPathName + _T("\\") + m_sFileName);
		}
		return 0;
	}

	LRESULT OnFileClose(WORD, WORD, HWND, BOOL&)
	{
		CloseDocument();
		m_sFileName = "";
		m_sPathName = "";
		return 0;
	}

public:

	// Public interface overridables

	virtual bool OpenDocument(LPCTSTR /*lpszFileName*/)
	{
		return true;
	}

	virtual bool SaveDocument(LPCTSTR /*lpszFileName*/)
	{
		return true;
	}

	virtual bool CloseDocument() 
	{
		return true;
	}

protected:

	// Implementation overridables

	template <typename _Dialog>
	void PrepareDialog(_Dialog& dlg)
	{
		_Derived* pDerived = static_cast<_Derived*>(this);
		pDerived->GetFileFilter();
		dlg.m_ofn.lpstrFilter = m_sFilter.c_str();
		pDerived->GetDefaultExtension();
		dlg.m_ofn.lpstrDefExt = m_sDefExt.c_str();
	}


protected:

	void GetDefaultExtension()
	{
		GetFileFilter();
		m_sDefExt = _T("");
		int nExtPos = m_sFilter.find_first_of(foundation::string::value_type('\0'));
		if (nExtPos != foundation::string::npos) {
			int nPointPos = m_sFilter.find_first_of('.', nExtPos + 1);
			if (nPointPos != foundation::string::npos) {
				m_sDefExt = m_sFilter.substr(nPointPos + 1, 3);
			}
		}
	}

	void GetFileFilter()
	{
		// Filter is considered static information,
		// hence, once calculated, information is cached
		// and not processed again
		if (m_sFilter.length() > 0) {
			return;
		}

		_Derived* pDerived = static_cast<_Derived*>(this);
		UINT* aResIds = pDerived->GetDocumentResources();
		HINSTANCE hResInst = _Module.GetResourceInstance();
		foundation::stringstream sFilterBuf;

		bool bEndFound = (*aResIds == 0);
		foundation::string sFilterItem;
		sFilterItem.get_buffer(256);
		int nCount = 0;
		while (!bEndFound) {
			int nres;
			nres = ::LoadString(hResInst, *aResIds, sFilterItem.get_buffer(), 256);
			int nCRPos;
			nCRPos = sFilterItem.find_first_of(foundation::string::value_type('\n'));
			if (nCRPos != foundation::string::npos) {
				sFilterItem.replace(nCRPos, 1, 1, foundation::string::value_type('\0'));
			}
			sFilterBuf << sFilterItem.c_str() << std::ends << sFilterItem.c_str() + nCRPos + 1 << std::ends;
			nCount++;
			bEndFound = (*(++aResIds) == 0);
		}
		sFilterBuf << std::ends;
		m_sFilter = sFilterBuf.str();
	}

protected:
	foundation::string m_sPathName;
	foundation::string m_sFileName;

	// Implementation
	foundation::string m_sFilter;
	foundation::string m_sDefExt;

};




};	// namespace stingray::foundation
};	// namespace stingray


#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
	#pragma warning (pop)
	#endif
#endif