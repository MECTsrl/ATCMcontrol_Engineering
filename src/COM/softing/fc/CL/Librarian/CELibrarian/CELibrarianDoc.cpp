/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrarianDoc.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CELibrarianDoc.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrarianDoc.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CL
 *
 * =COMPONENT           CELibrarian
 *
 * =CURRENT      $Date: 28.02.07 18:59 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  6/25/2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "utilif.h"
#include "CELibrarian.h"
//#include "CELibraryException.h" //OH: was in 4cdll
#include "CEFile.h" //OH: was in 4cdll

#include "CELibrarianDoc.h"
#include "CSrvrItem.h"
#include "CELibrarianIF_i.c"

#include "CELibrarianView.h"

#include "cemaindrivers.h"

//----  Static Initializations:   ----------------------------------*



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CELibrarianDoc

IMPLEMENT_DYNCREATE(CELibrarianDoc, CStdEditorDoc)

BEGIN_MESSAGE_MAP(CELibrarianDoc, CStdEditorDoc)
	//{{AFX_MSG_MAP(CELibrarianDoc)
	ON_COMMAND(ID_CL_FILE_IMPORT, OnFileImport)
	ON_COMMAND(ID_CL_LIBRARY_OPEN, OnLibraryOpen)
	//ON_COMMAND(ID_CL_LIBRARY_SAVE, OnLibrarySave)
	//ON_COMMAND(ID_CL_LIBRARY_SAVEAS, OnLibrarySaveas)
	//ON_COMMAND(ID_CL_LIBRARY_NEW, OnLibraryNew)
	//}}AFX_MSG_MAP
	// Zulassen der Standardimplementierung für OLE-Container
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, COleServerDoc::OnUpdatePasteMenu)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINK, COleServerDoc::OnUpdatePasteLinkMenu)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_CONVERT, COleServerDoc::OnUpdateObjectVerbMenu)
	ON_COMMAND(ID_OLE_EDIT_CONVERT, COleServerDoc::OnEditConvert)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, COleServerDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, COleServerDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI(ID_OLE_VERB_FIRST, COleServerDoc::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()

BEGIN_OLECMD_MAP(CELibrarianDoc, CStdEditorDoc)
	ON_OLECMD_PRINT()
END_OLECMD_MAP()

// automation
BEGIN_DISPATCH_MAP(CELibrarianDoc, CStdEditorDoc)
	//{{AFX_DISPATCH_MAP(CELibrarianDoc)
		// HINWEIS - Hier werden Mapping-Makros vom Klassen-Assistenten eingefügt und entfernt.
		//      Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


// Hinweis: Wir stellen Unterstützung für IID_ICELibrarian zur Verfügung, um typsicheres Binden 
//  von VBA zu unterstützen. Diese IID muss mit der GUID übereinstimmen, die bei der 
//  Disp-Schnittstelle in der .ODL-Datei angegeben ist.

// {D56600B5-D42E-11D1-B273-006008736562}
static const IID IID_ICELibrarian =
{ 0xd56600b5, 0xd42e, 0x11d1, { 0xb2, 0x73, 0x0, 0x60, 0x8, 0x73, 0x65, 0x62 } };

BEGIN_INTERFACE_MAP(CELibrarianDoc, CStdEditorDoc)
	INTERFACE_PART(CELibrarianDoc, IID_ICELibrarian, Dispatch)
	INTERFACE_PART(CELibrarianDoc, IID_ICELibrarianUser,CELibrarianUserObj)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CELibrarianDoc Konstruktion/Destruktion

CELibrarianDoc::CELibrarianDoc()
{
	// Verwenden Sie OLE-Compound-Dateien
	EnableCompoundFile();

	// automation ?
	// ZU ERLEDIGEN: Hier Code für One-Time-Konstruktion einfügen
	EnableAutomation();
	AfxOleLockApp();

	//
	// own code
	//
	m_documentState = none;
	//AfxMessageBox("Hallo !");
}

CELibrarianDoc::~CELibrarianDoc()
{
	// automation ?
	AfxOleUnlockApp();
}

CDocObjectServer* CELibrarianDoc::GetDocObjectServer(LPOLEDOCUMENTSITE pSite)
{
	return new CDocObjectServer(this, pSite);
}

BOOL CELibrarianDoc::OnNewDocument()
{
	//
	// HACK !!!!!
	//
	if (m_bEmbedded==TRUE)
		SetModified(FALSE);
	
	//OnLibraryNew();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CELibrarianDoc Server-Implementierung

COleServerItem* CELibrarianDoc::OnGetEmbeddedItem()
{
	// OnGetEmbeddedItem wird automatisch aufgerufen, um COleServerItem zu erhalten,
	//  das mit dem Dokument verknüpft ist. Die Funktion wird nur bei Bedarf aufgerufen.

	CELibrarianSrvrItem* pItem = new CELibrarianSrvrItem(this);
	ASSERT_VALID(pItem);
	return pItem;
}

/////////////////////////////////////////////////////////////////////////////
// CELibrarianDoc Serialisierung

void CELibrarianDoc::Serialize(CArchive& ar)
{
	ASSERT(0);

	if (ar.IsStoring())
	{
		// ZU ERLEDIGEN: Hier Code zum Speichern einfügen
	}
	else
	{
		// ZU ERLEDIGEN: Hier Code zum Laden einfügen
	}


	/* umstellung auf server
	// Ein Aufruf der Basisklasse COleServerDoc ermöglicht die Serialisierung
	//  des Objekts COleClientItem des Container-Dokuments.
	COleServerDoc::Serialize(ar);
	*/
}

void CELibrarianDoc::OnSerializeDocument (CArchive &ar)
{
	ASSERT(0);
}

/////////////////////////////////////////////////////////////////////////////
// CELibrarianDoc Diagnose

#ifdef _DEBUG
void CELibrarianDoc::AssertValid() const
{
	COleServerDoc::AssertValid();
}

void CELibrarianDoc::Dump(CDumpContext& dc) const
{
	COleServerDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CELibrarianDoc Befehle

/////////////////////////////////////////////////////////////////////
/**
 * get a pointer to the library
 *
 * @return		pointer to CELibrary
 * @exception   
 * @see         
 */
CELibrary* CELibrarianDoc::GetCELibrary()
{
	return &m_CELibrary;
}

/////////////////////////////////////////////////////////////////////
/**
 * intitialize the document, i.e. the CELibrary
 *
 * @return		void               
 * @exception   
 * @see         
 */
void CELibrarianDoc::InitDocument()
{
	m_actualItem = "";
	m_passWordProtected = FALSE;
	m_CELibrary.DeleteAllPOU();		// 13.12.06 SIS: in stand allone modus this is necessary
									// else after a second open -> crash on close
}

/////////////////////////////////////////////////////////////////////
/**
 *
 * @param lpszPathName	path name of document
 * @return		TRUE ok, FALSE not ok
 * @exception   
 * @see         
 */
BOOL CELibrarianDoc::OnOpenDocument(LPCTSTR fileName) 
{
    if ( ! CEFile::IsFile(fileName) )
	{
		AfxMessageBox(IDS_CE_ERR_FILENOTFOUND,MB_ICONEXCLAMATION |MB_OK);
		return OnNewDocument();
	}
	else
	{
		//try
		//{
			CString openMode;
			//
			// read only flag
			//
			if ( CEFile::IsReadOnly(fileName) )
			{
				SetReadOnlyFlag(TRUE);
				openMode="rt";
			}
			else
			{
				SetReadOnlyFlag(FALSE);
				openMode="rwt";
			}
			InitDocument();
			//if ( GetCELibrary()->IsOpen() )
			//	GetCELibrary()->Close();
			GetCELibrary()->Open(fileName,openMode);
			SetPathName(fileName,TRUE);
			m_documentState=loaded;
			m_passWordProtected=GetCELibrary()->IsPassWordProtected();
			UpdateAllViews(NULL);
			SetModifiedFlag(FALSE);

		//}
		//catch (CELibraryException ex)
		//{
		//	ex.ReportError();
		//	OnNewDocument();
		//}
	}

	return TRUE;
}

void CELibrarianDoc::OnCloseDocument() 
{

	/*if ( GetCELibrary()->IsOpen() )
		GetCELibrary()->Close();*/
	COleServerDoc::OnCloseDocument();
}

/////////////////////////////////////////////////////////////////////
/**
 * set read only flag 
 *
 * @param flag, TRUE document is read only
 * @return	 
 * @exception   
 * @see
 */
void CELibrarianDoc::SetReadOnlyFlag(BOOL flag)
{
	m_readOnly=flag;
}

/////////////////////////////////////////////////////////////////////
/**
 * get read only flag 
 *
 * @return true if lib is read only	 
 * @exception   
 * @see
 */
BOOL CELibrarianDoc::IsReadOnly()
{
	return m_readOnly;
}

void CELibrarianDoc::SetActualItem(const CString & actItem)
{
	m_actualItem = actItem;
}

CString CELibrarianDoc::GetActualItem()
{
	return m_actualItem;
}

void CELibrarianDoc::OnFileImport() 
{
	/*try
	{
		CString fileName;
		int answer = -1;

		//
		// ask to save current library
		//
		if ( IsModified() )
		{
			// get name/title of document
			CString name;
			if (m_strPathName.IsEmpty())
			{
				// get name based on caption
				name = m_strTitle;
				if (name.IsEmpty())
					VERIFY(name.LoadString(AFX_IDS_UNTITLED));
			}
			else
			{
				// get name based on file title of path name
				name = CEFile::GetFileName(m_strPathName);
			}

			CString prompt;
			AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, name);
			answer=AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE);
			//
			// cancel -> return
			//
			if (answer == IDCANCEL)
			{
				return;
			}
			else if (answer == IDYES)
			{
				//
				// save it, ask for new name, if no name given, or file write protected
				//
				if ( m_documentState==newdoc )
				{
					if ( DoFileDialog(fileName, FALSE,
					OFN_NOREADONLYRETURN | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY))
					{
						CWaitCursor wait;
						GetCELibrary()->SaveAs(fileName);
						GetCELibrary()->Close();
					}
					else
					{
						GetCELibrary()->Close();
						SetModifiedFlag(FALSE);
					}
				}
				else if (m_documentState==loaded)
				{
					CWaitCursor wait;
					GetCELibrary()->Commit();
					GetCELibrary()->Close();
					SetModifiedFlag(FALSE);
				}
			}
			else if ( answer == IDNO )
			{
				GetCELibrary()->Close();
				SetModifiedFlag(FALSE);
			}
		}

		if ( DoFileDialog(fileName, TRUE, OFN_FILEMUSTEXIST) )
		{
			//
			// open new document
			//
			InitDocument();
			if ( GetCELibrary()->IsOpen() )
				GetCELibrary()->Close();
			m_documentState=none;
			InitDocument();
			OnNewDocument();
			CWaitCursor wait;
			GetCELibrary()->Import(fileName);
			UpdateAllViews(NULL);
		}
		else
		{
			//
			// new document
			//
			InitDocument();
			if ( GetCELibrary()->IsOpen() )
				GetCELibrary()->Close();
			m_documentState=none;
			OnNewDocument();
		}
	}
	catch (CELibraryException ex)
	{
		ex.ReportError();
	}*/
}

void CELibrarianDoc::OnLibraryOpen() 
{
	//try
	//{
		CString fileName;
		int answer = -1;

		//
		// ask to save current library
		//
		if ( IsModified() )
		{
			// get name/title of document
			CString name;
			if (m_strPathName.IsEmpty())
			{
				// get name based on caption
				name = m_strTitle;
				if (name.IsEmpty())
					VERIFY(name.LoadString(AFX_IDS_UNTITLED));
			}
			else
			{
				// get name based on file title of path name
				name = CEFile::GetFileName(m_strPathName);
			}

			CString prompt;
			AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, name);
			answer=AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE);
			//
			// cancel -> return
			//
			if (answer == IDCANCEL)
			{
				return;
			}
			else if (answer == IDYES)
			{
				//
				// save it, ask for new name, if no name given, or file write protected
				//
				if ( m_documentState==newdoc )
				{
					if ( DoFileDialog(fileName, FALSE,
					OFN_NOREADONLYRETURN | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY))
					{
						CWaitCursor wait;
						//GetCELibrary()->SaveAs(fileName);
						//GetCELibrary()->Close();
					}
					else
					{
						//GetCELibrary()->Close();
						SetModifiedFlag(FALSE);
					}
				}
				else if (m_documentState==loaded)
				{
					CWaitCursor wait;
					//GetCELibrary()->Commit();
					//GetCELibrary()->Close();
					SetModifiedFlag(FALSE);
				}
			}
			else if ( answer == IDNO )
			{
				//GetCELibrary()->Close();
				SetModifiedFlag(FALSE);
			}
		}

		if ( DoFileDialog(fileName, TRUE, OFN_FILEMUSTEXIST) )
		{
			//
			// open new document
			//
			InitDocument();
			//if ( GetCELibrary()->IsOpen() )
			//	GetCELibrary()->Close();
			OnOpenDocument(fileName);
		}
		else
		{
			//
			// new document
			//
			
			InitDocument();
			//if ( GetCELibrary()->IsOpen() )
			//	GetCELibrary()->Close();
			m_documentState=none;
			OnNewDocument();
			
			//nix
		}
	//}
	//catch (CELibraryException ex)
	//{
	//	ex.ReportError();
	//}
}

/*void CELibrarianDoc::OnLibrarySave() 
{	
	try
	{
		CString fileName;
		//
		// if new lib ask for name
		//
		if (m_documentState==newdoc)
		{
			if ( DoFileDialog(fileName, FALSE,
				OFN_NOREADONLYRETURN | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY) )
			{
				CWaitCursor wait;
				GetCELibrary()->SaveAs(fileName);
				m_documentState=loaded;
				SetPathName(fileName,TRUE);
				SetModifiedFlag(FALSE);
			}
			return;
		}
		else if (m_documentState==loaded && !IsReadOnly())
		{
			CWaitCursor wait;
			GetCELibrary()->Commit();
			SetModifiedFlag(FALSE);
		}
		else if (m_documentState==loaded && IsReadOnly())
		{
			AfxMessageBox(IDS_CE_SAVEDIFFERENT,MB_ICONEXCLAMATION |MB_OK);
			OnLibrarySaveas();
		}
	}
	catch (CELibraryException ex)
	{
		ex.ReportError();
	}
}*/

/*void CELibrarianDoc::OnLibrarySaveas() 
{
	try
	{
		CString fileName;
		//
		// ask for a new file, not write protected
		//
		if ( DoFileDialog(fileName, FALSE,
				OFN_NOREADONLYRETURN | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY) )
		{
			//
			// if new file = old file name -> Commit
			//
			if ( fileName == GetPathName() )
			{
				CWaitCursor wait;
				GetCELibrary()->Commit();
			}
			else
			{
				//
				// save library as file name
				//
				CWaitCursor wait;
				GetCELibrary()->SaveAs(fileName);
				SetPathName(fileName,TRUE);
				SetReadOnlyFlag(FALSE);
			}
			SetModifiedFlag(FALSE);
		}
	}
	catch (CELibraryException ex)
	{
		ex.ReportError();
	}
}*/

/*void CELibrarianDoc::OnLibraryNew() 
{
	try
	{
		CString fileName;
		//
		// if document loaded and modified, ask if to save old document
		//
		if ( (m_documentState==loaded || m_documentState==newdoc) && IsModified())
		{
			// get name/title of document
			CString name;
			if (m_strPathName.IsEmpty())
			{
				// get name based on caption
				name = m_strTitle;
				if (name.IsEmpty())
					VERIFY(name.LoadString(AFX_IDS_UNTITLED));
			}
			else
			{
				// get name based on file title of path name
				name = CEFile::GetFileName(m_strPathName);
			}

			CString prompt;
			AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, name);
			int rv=AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE);
			//
			// cancel -> return
			//
			if (rv == IDCANCEL)
			{
				return;
			}
			else if ( rv == IDYES )
			{
				//
				// save it, ask for new name, if no name given, or file write protected
				//
				if ( m_documentState==newdoc )
				{
					if ( DoFileDialog(fileName, FALSE,
						OFN_NOREADONLYRETURN | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY) )
					{
						CWaitCursor wait;
						GetCELibrary()->SaveAs(fileName);
						GetCELibrary()->Close();
						InitDocument();
						GetCELibrary()->Open("","ct");
					}
				}
				else if (m_documentState==loaded)
				{
					CWaitCursor wait;
					GetCELibrary()->Commit();
					GetCELibrary()->Close();
					InitDocument();
					GetCELibrary()->Open("","ct");
					m_documentState=newdoc;
				}
			}
			else if ( rv == IDNO )
			{
				GetCELibrary()->Close();
				InitDocument();
				GetCELibrary()->Open("","ct");
				m_documentState=newdoc;
			}
		}
		else if (m_documentState==none)
		{
			//
			// create a new document
			//
			InitDocument();
			GetCELibrary()->Open("","ct");
			m_documentState = newdoc;
		}
		else if (m_documentState==loaded)
		{
			GetCELibrary()->Close();
			InitDocument();
			GetCELibrary()->Open("","ct");
			m_documentState = newdoc;
		}

		SetPathName("Untitled",FALSE);
		SetModifiedFlag(FALSE);
		SetReadOnlyFlag(FALSE);
		UpdateAllViews(NULL);
		return;
	}
	catch (CELibraryException ex)
	{
		ex.ReportError();
	}
}*/

BOOL CELibrarianDoc::DoFileDialog(CString& fileName, BOOL bOpenFileDialog, DWORD lFlags)
{
	CFileDialog dlgFile(bOpenFileDialog);
	static int initPath = 0;
	
	//CString title;
	//VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;

	//
	// standard lib path
	//
	TCHAR pszPath[MAX_PATH];
	if ( initPath==0)
	{
		HRESULT hr=UTIL_GetStdLibPath(pszPath, MAX_PATH);	
		dlgFile.m_ofn.lpstrInitialDir = pszPath;
		initPath=1;
	}	

	// 4cl filter
	CString strFilter;

	strFilter="ATCMControl library (*.4cl)";
	strFilter += (TCHAR)'\0';
	strFilter += _T("*.4cl");
	strFilter += (TCHAR)'\0';

	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please

	strFilter += _T("*.*");

	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;

	//dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	
	BOOL bResult = dlgFile.DoModal() == IDOK ? TRUE : FALSE;

	//
	// add 4cl extension if 
	//
	fileName.ReleaseBuffer();
	/*if ( !(dlgFile.m_ofn.Flags & OFN_FILEMUSTEXIST) )
	{
		CString ext=CEFile::GetFileExt(fileName);
		if (ext=="")
			fileName += ".4cl";
	}*/
	return bResult;
}

BOOL CELibrarianDoc::SaveModified() 
{
	if (!IsModified())
		return TRUE;        // ok to continue

	// get name/title of document
	CString name;
	if (m_strPathName.IsEmpty())
	{
		// get name based on caption
		name = m_strTitle;
		if (name.IsEmpty())
			VERIFY(name.LoadString(AFX_IDS_UNTITLED));
	}
	else
	{
		// get name based on file title of path name
		//name = CEFile::GetFileName(m_strPathName);
	}

	CString prompt;
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, name);
	int answer=AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE);
	
	if ( answer==IDCANCEL )
	{
		return FALSE;
	}
	else if ( answer==IDYES )
	{
		//OnLibrarySave();
	}

	return TRUE;    // keep going
}

/////////////////////////////////////////////////////////////////////
/**
 * show error message from CELibrary
 *
 * @param errId errorId
 * @return		void
 * @exception
 * @see
 */
void CELibrarianDoc::ErrorMessage(long errId)
{
	/*if (errId != CELibrary::Err_Ok)
	{
		
		CString message=CELibrary::GetErrorMessage(errId);
		AfxMessageBox( message, MB_ICONEXCLAMATION|MB_OK);
	}*/
}

void CELibrarianDoc::DebugHook()
{
	int xx=1;
}

BOOL CELibrarianDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	//
	// is only called in embedded mode via automation interface
	// save to a different file name means save to a copy
	//
	//return COleServerDoc::OnSaveDocument(lpszPathName);
	CString strLibName=GetPathName();
	if (strLibName.CompareNoCase(lpszPathName) == 0)
	{
		//OnLibrarySave();
	}
	else
	{
		/*try
		{
			//
			// save library as file name
			//
			CWaitCursor wait;
			GetCELibrary()->SaveAs(lpszPathName, FALSE);
			//SetPathName(fileName,TRUE);
			//SetReadOnlyFlag(FALSE);
		}
		catch (CELibraryException ex)
		{
			ex.ReportError();
		}*/
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
/**
 *
 * @param strFileName	path name of document
 * @return		TRUE ok, FALSE not ok
 * @exception   
 * @see         
 */
bool CELibrarianDoc::LoadDocument (const CString &strFileName)
{
	int bb=OnOpenDocument(strFileName);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
/**
 *
 * @param strFileName	path name of document
 * @return		TRUE ok, FALSE not ok
 * @exception   
 * @see         
 */
bool CELibrarianDoc::SaveDocument (const CString &strFileName)
{
	OnSaveDocument(strFileName);
	return TRUE;
}

HRESULT CELibrarianDoc::GotoLocation (const CString &strPouName)
{
	//
	// get Librarian (list) View
	//
	POSITION pos = GetFirstViewPosition();
	CView* pView = GetNextView(pos);
	if (pView == NULL)
		return E_FAIL;
		
	if ( ! pView->IsKindOf( RUNTIME_CLASS( CELibrarianView ) ) )
		return E_FAIL;

	CELibrarianView* pCELibrarianView = (CELibrarianView*)pView;
	
	HRESULT hr=pCELibrarianView->SelectPou(strPouName);

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// embedded interface XCELibrarianUserObj commands

STDMETHODIMP_(ULONG) CELibrarianDoc::XCELibrarianUserObj::AddRef ()
{
	METHOD_PROLOGUE_EX (CELibrarianDoc, CELibrarianUserObj)
    
	return (pThis->ExternalAddRef ());
}

STDMETHODIMP_(ULONG) CELibrarianDoc::XCELibrarianUserObj::Release ()
{
	METHOD_PROLOGUE_EX (CELibrarianDoc, CELibrarianUserObj)

	return (pThis->ExternalRelease ());
}

STDMETHODIMP CELibrarianDoc::XCELibrarianUserObj::QueryInterface (REFIID iid, LPVOID *ppvObj)
{
	METHOD_PROLOGUE_EX (CELibrarianDoc, CELibrarianUserObj)

	return ((HRESULT)pThis->ExternalQueryInterface (&iid, ppvObj));
}


STDMETHODIMP CELibrarianDoc::XCELibrarianUserObj::ShowPouHeader (BSTR szPouName)
{
	METHOD_PROLOGUE_EX (CELibrarianDoc, CELibrarianUserObj)

    return (pThis->ShowPouHeader(szPouName));
}

/////////////////////////////////////////////////////////////////////
/**
 *	show header of pou 
 * @param strPouName	pou name
 * @return		
 * @exception   
 * @see         
 */
HRESULT CELibrarianDoc::ShowPouHeader (const CString &strPouName)
{
	POSITION pos = GetFirstViewPosition();
	CView* pView = GetNextView(pos);
	
	if (pView==NULL)
		return E_FAIL;
	/*
	if (pView != NULL)
	{
		CFrameWnd* pFrameWnd = pView->GetParentFrame();
		pFrameWnd->ActivateFrame(SW_SHOW);
		pFrameWnd = pFrameWnd->GetParentFrame();
		if (pFrameWnd != NULL)
			pFrameWnd->ActivateFrame(SW_SHOW);
	}
	*/
	
	// ab hier gehts los
	if ( ! pView->IsKindOf( RUNTIME_CLASS( CELibrarianView ) ) )
		return E_FAIL;

	CELibrarianView* pCELibrarianView = (CELibrarianView*)pView;
	HRESULT hr=pCELibrarianView->ShowPouHeader(strPouName);

	return hr;
}



HRESULT CELibrarianDoc::ShowHeader(const CString &strFileName)
{
    HRESULT hr;
    CComPtr<IDispatch> pGenEdCont;
    CGenEdContDriver driver;

    hr = GetGenEdCont (pGenEdCont);
    if (FAILED (hr))
    {
        return (hr);
    };

    ASSERT (pGenEdCont != NULL);
    if (pGenEdCont == NULL)
    {
        return (E_UNEXPECTED);
    };

    driver.AttachDispatch (pGenEdCont, FALSE);

    TRY
    {
        hr = driver.ShowHeaderFile((LPCTSTR)strFileName);
    }
    CATCH (COleException, e)
    {
        hr = COleException::Process (e);
    }
    END_CATCH

    return (hr);
}



HRESULT CELibrarianDoc::ShowUserHelp(const CString &strPouName)
{
    HRESULT hr;
    CComPtr<IDispatch> pGenEdCont;
    CGenEdContDriver driver;

    hr = GetGenEdCont (pGenEdCont);
    if (FAILED (hr))
    {
        return (hr);
    };

    ASSERT (pGenEdCont != NULL);
    if (pGenEdCont == NULL)
    {
        return (E_UNEXPECTED);
    };

    driver.AttachDispatch (pGenEdCont, FALSE);

    TRY
    {
        hr = driver.ShowHelp((LPCTSTR)strPouName);
    }
    CATCH (COleException, e)
    {
        hr = COleException::Process (e);
    }
    END_CATCH

    return (hr);
}


BOOL CELibrarianDoc::CanShowUserHelp()
{
    return (m_lpClientSite!=NULL);
}


/*
 *----------------------------------------------------------------------------*
 *  $History: CELibrarianDoc.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 13.12.06   Time: 15:19
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 13.12.06 SIS sort by (1) type and (2) name
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 5  *****************
 * User: Oh           Date: 7/12/01    Time: 11:55a
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 4  *****************
 * User: Oh           Date: 7/12/01    Time: 10:32a
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 3  *****************
 * User: Oh           Date: 7/10/01    Time: 3:59p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:32p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/
