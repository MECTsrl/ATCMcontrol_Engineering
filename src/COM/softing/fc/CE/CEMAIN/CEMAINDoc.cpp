// 18.12.2000, SIS: Dokument-Klasse, die vom MFC-Applikationswizard angelegt wurde
//                  Erhaelt keinen Header, da sie spaeter nicht mehr gebraucht wird
// CEMAINDoc.cpp : implementation of the CCEMAINDoc class
//

#include "stdafx.h"
#include "CEMAIN.h"

#include "CEMAINDoc.h"
#include "ExeDelegator.h"

//#include "CntrItem.h"

// Only for test purposes
#include "MainFrm.h"
#include "CECompMan.h"
#include "ProjectWizardSelection.h"
#include "GenEdCont.h"
#include "TargetSet.h"
#include "Settings.h"
#include "utilif.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCEMAINDoc

IMPLEMENT_DYNCREATE(CCEMAINDoc, CDocument)

BEGIN_MESSAGE_MAP(CCEMAINDoc, CDocument)
	//{{AFX_MSG_MAP(CCEMAINDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	ON_COMMAND(IDM_FILE_CLOSE, CDocument::OnFileClose)
    ON_UPDATE_COMMAND_UI(IDM_FILE_CLOSE, OnUpdateFileClose)
    ON_UPDATE_COMMAND_UI(IDM_PROJECT_CLOSE, OnUpdateDocumentOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCEMAINDoc construction/destruction

CCEMAINDoc::CCEMAINDoc() :
   m_StateChangeCB (this, OnStateChange)
{
//	EnableCompoundFile();

    // CCmdTarget constructor initially sets its m_dwRef member to 1
    // as this is the correct value for CCmdTarget interface reference
    // count if the represented COM object is cocreated. However the
    // project class currently cannot be cocreated and we have to have
    // a correct interface count for to get to know when the last inter-
    // face reference is released.
    // NOTE: If ever the project class is to be cocreateable somebody
    // has to tell whether it has been cocreated or C++ created. Only
    // in the latter case the m_dwRef has to be decremented by one. The
    // best solution would be to supply a method that is called only if
    // the instance is C++ created. Inside this method the m_dwRef would
    // have to be decremented by one.

    --m_dwRef;

	// TODO: add one-time construction code here
    m_bOpen = FALSE;
    m_bLockFileOpen = FALSE;

    CCEStateManager *pStateManager;

    pStateManager = CEGetStateManager ();
    if (pStateManager != NULL)
    {
        pStateManager->RegisterCallback (&m_StateChangeCB);
    }
}

CCEMAINDoc::~CCEMAINDoc()
{
    CCEStateManager *pStateManager;

    pStateManager = CEGetStateManager ();
    if (pStateManager != NULL)
    {
        pStateManager->UnregisterCallback (&m_StateChangeCB);
    }
}



void CCEMAINDoc::OnStateChange (CEStateNotify stateNotify, unsigned int uiStage)
{
    //IUnknown *pUnknown;
    //CCEStateManager *pStateManager;

    switch (stateNotify)
    {

        case CEPreCloseProjectNotify:
            //UnregisterMsgSource();
            break;

        case CECreateComponentNotify:
            if (CESMGetComponentName () == CE_COMPMAN_BREAKPTMAN)
            {
                CComPtr<IBreakPoint> pBreakpointManager;

                GetBreakpointManager(pBreakpointManager);
                if (pBreakpointManager != NULL)
                {
                    pBreakpointManager->Init (CComBSTR (m_StrPath));

                    // Currently cannot sync on debug connect because breakpoint
                    // manager is not clever enough to not subscribe breakpoints
                    // if the project being downloaded does not match the enginee-
                    // ring project. Instead syncing is done manually where project
                    // matching is done (CMainFrame::OnIdle).
                    pBreakpointManager->SetSyncMode (FALSE, TRUE);
                };
            }

            break;

        case CEDestroyComponentNotify:
            if (CESMGetComponentName () == CE_COMPMAN_BREAKPTMAN)
            {
                CComPtr<IBreakPoint> pBreakpointManager;

                GetBreakpointManager (pBreakpointManager);
                if (pBreakpointManager != NULL)
                {
                    pBreakpointManager->Cleanup ();
                };
            }
            break;

        case CEDownloadCompleteNotify:
          /*  pStateManager = CEGetStateManager ();
            if (pStateManager != NULL)
            {
                m_bResubscribeAnotherTime = true;
                m_uiResubscribeCookie = pStateManager->TriggerDelay (NULL);
            };*/
            break;

        case CEDelayNotify:
          /*  pStateManager = CEGetStateManager ();
            if (pStateManager != NULL)
            {
                if (pStateManager->GetDelayCookie () == m_uiResubscribeCookie)
                {
                    m_uiResubscribeCookie = 0;

                    pUnknown = CEGetCOM (_T ("WatchAdmin"));
                    if (pUnknown != NULL)
                    {
                        HRESULT hr;
                        ICEWatchAdmin *pWatchAdmin;

                        hr = pUnknown->QueryInterface (IID_ICEWatchAdmin, (LPVOID *)&pWatchAdmin);
                        if (SUCCEEDED (hr))
                        {
                            ASSERT (pWatchAdmin != NULL);
                            if (pWatchAdmin != NULL)
                            {
                                pWatchAdmin->Resubscribe ();
                                pWatchAdmin->Release ();
                            };
                        };
                    };
                   

                    if (m_bResubscribeAnotherTime)
                    {
                        m_bResubscribeAnotherTime = false;
                        m_uiResubscribeCookie = pStateManager->TriggerDelay (2000, NULL);
                    };
                };
            };*/
            break;

        default:
            break;
    };
}



HRESULT CCEMAINDoc::GetBreakpointManager (CComPtr<IBreakPoint>& pIBreakPoint)
{
    HRESULT hr;
    IUnknown* pUnknown;

    pUnknown = CEGetCOM (CE_COMPMAN_BREAKPTMAN, false);
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (E_FAIL);
    };

    hr = pUnknown->QueryInterface (IID_IBreakPoint, (LPVOID *)&pIBreakPoint);
    ASSERT (SUCCEEDED (hr));
    pUnknown->Release();
    if (FAILED (hr))
    {
        return (hr);
    }
    ASSERT (pIBreakPoint != NULL);
    return (hr);
}



BOOL CCEMAINDoc::OnNewDocument()
{

	if (!CDocument::OnNewDocument())
    {
		return FALSE;
    }
    
    //Muss das auch hin ???
    /*if (!CopyDatabase(StrProjectPath,dlg.m_StrProject))
		return FALSE;*/

    BSTR bstrFileName;
    HRESULT hr = ProjectWizardSelection(bstrFileName);
    CString strFileName;

    if(hr == S_OK)
    {
        strFileName = bstrFileName;
        SetPathName(strFileName);
        CCEMAINApp *pApp;
        pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
        ASSERT (pApp != NULL);
	    
        
        if(!OnOpenDocument(strFileName))
        {
            return FALSE;
        }
        else
        {
            AfxGetApp()->AddToRecentFileList((CString)bstrFileName);
            AfxGetApp()->WriteProfileString(REG_SECTION_ENGINEERING, REG_KEY_LAST_PROJECT, strFileName);
            if (pApp != NULL)
            {
	            pApp->LoadSettings(_T(""));
            }
            return TRUE;
        }
    }
	return FALSE;
}



/////////////////////////////////////////////////////////////////////////////
// CCEMAINDoc serialization

void CCEMAINDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
        ASSERT(pApp);

		if (pApp != NULL)
		{
			CGenEdCont *pContainer;

			pContainer = pApp->GetContainer ();
			assert (pContainer != NULL);
			if (pContainer != NULL)
			{
				pContainer->UpdateSettings ();
			};
		};
	}
	else
	{
		// TODO: add loading code here
	}

	// Calling the base class CDocument enables serialization
	//  of the container document's COleClientItem objects.
	CDocument::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CCEMAINDoc diagnostics

#ifdef _DEBUG
void CCEMAINDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCEMAINDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCEMAINDoc commands

void CCEMAINDoc::OnCloseDocument() 
{
	CCEMAINApp *pApp;
	CGenEdCont *pContainer;
    CCEStateManager *pStateManager;
    //CCEComponentManager *pComponentManager;
    //CMultiDocTemplate* pFileTemplate;
    //CMainFrame *pFrame;

    // If the project document is logically open destroy
    // anything belonging to this project and set it
    // logically closed.

	pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
	ASSERT (pApp != NULL);
	if (pApp == NULL)
		return;

    if (m_bOpen)
    {
//        CCEObjectManager *pObjectManager;

//        pObjectManager = CEGetObjectManager();
//        if (pObjectManager != NULL && pObjectManager->IsConfiguring ())
//        {
//            if (AfxMessageBox (IDS_CLOSE_PROJECT_WHILE_CONFIGURING, MB_OKCANCEL) == IDCANCEL)
//                return;
//        }

        pStateManager = CEGetStateManager();

        if (pStateManager != NULL)
        {
            pStateManager->TriggerFillSettings ();
        }

//        pComponentManager = CEGetComponentManager ();
	    pContainer = pApp->GetContainer ();
	    ASSERT(pContainer != NULL);

	    if (pContainer != NULL)
	    {
		    pContainer->CloseAll ();
//		    pContainer->SetSettings (NULL);
	    }

        pApp->CloseAllPanelViews();

        // save settings file
        CSettings* pSettings = pApp->GetSettings();
        ASSERT(pSettings);

        if (pSettings)
        {
            pSettings->Save(m_StrPath);
        }

        if (pStateManager != NULL)
        {
            pStateManager->TriggerPreCloseProject ();
        }

        pApp->DeleteSettings();

        RemoveAllTargets();

//	    pFrame = (CMainFrame *) AfxGetMainWnd();
//	    ASSERT(pFrame);
//
//        pFrame->TerminateFindInFilesThread();

// close project
        CComPtr<IUnknown>               pTmp;
        CComQIPtr<ICEProjectManager>    pProjMan;
        HRESULT hr;


        pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
        pProjMan = pTmp;
        if(pProjMan)
        {
            hr = pProjMan->CloseProject();
        }


        if (pStateManager != NULL)
        {
            pStateManager->TriggerPostCloseProject ();
        }

	    m_bOpen = FALSE;

	    // clear project open flag
	    pApp->m_bPrjOpen = FALSE;
    }

	// As soon as the reference count is zero we are allowed to really destroy
	// the document. The difference between m_bAutoDelete set to TRUE and
	// m_bAutoDelete set to FALSE is that CDocument::OnCloseDocument does not
	// destroy the document in the second case (nor does it remove the document
	// from the document template). In both cases the views are destroyed.

    // now close and remove lock file
    if(m_bLockFileOpen)
    {
        m_tLockFile.Close();
    }
    RemoveLockFile(GetPathName(), TRUE);

	if (m_dwRef == 0)
	{
	    CDocument::OnCloseDocument();
		pApp->m_bPrjLocked = FALSE;
	}
	else
	{
		int bSavedAutoDelete;

		bSavedAutoDelete = m_bAutoDelete;
		m_bAutoDelete = FALSE;

	    CDocument::OnCloseDocument();

		m_bAutoDelete = bSavedAutoDelete;

		// We do not want the doc template to reuse this document
		// if we reopen right the same file immediately after having
		// closed it.

		m_strPathName = "";
	}

	//CDocument::OnCloseDocument();
}

BOOL CCEMAINDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	//return CDocument::OnSaveDocument(lpszPathName);
    return TRUE;
}

BOOL CCEMAINDoc::SaveModified ()
{
//	if (!CloseDownloader ())
//	{
//		return (FALSE);
//	}

	if (!SaveDocuments ())
	{
		return (FALSE);
	}

	return (TRUE);
}

//bool CCEMAINDoc::CloseDownloader()
///////////////////////////////////////////////////////////////////////////////////////////////
///**
//*
//*	<B>CloseDownloader</B>.
//*
//*	Description = closes the downloader if project closed or project deleted.
//*	If a current download is pending, the function queries wether it should abort
//*	the pending download and destroy the downloader or wether it should not inter-
//*	rupt the current pending download. In the latter case the calling code should
//*	not close the document.
//*	<p>
//*	@return - TRUE - the downloader has been closed successfully. FALSE - there
//*	is a pending download and the project should not be closed !
//*	<p>
//*	@exception - No
//*	<p>
//*	@see
//*	<p>
//*/
//{
//	CMainFrame *pMainFrame;
//	CDownload *pDownloader;
//
//	pMainFrame = (CMainFrame *)AfxGetMainWnd ();
//	assert (pMainFrame != NULL);
//	if (pMainFrame == NULL)
//	{
//		return (TRUE);
//	}
//
//	pDownloader = pMainFrame->GetDownloader ();
//	if (pDownloader == NULL)
//	{
//		return (TRUE);
//	}
//
//	if (!pDownloader->IsDownloading ())
//	{
//		pDownloader->Destroy();
//		return (TRUE);
//	}
//
//	if (AfxMessageBox (IDS_CLOSE_DURING_DOWNLOAD, MB_YESNO) == IDYES)
//	{
//		pDownloader->AbortAllDownloads ();
//		pDownloader->Destroy();
//		return (TRUE);
//	}
//
//	return (FALSE);
//}


bool CCEMAINDoc::SaveDocuments ()
{
	CCEMAINApp *pApp;
	CGenEdCont *pContainer;
	CMultiDocTemplate *pDocTemplate;

	pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp ());
	assert (pApp != NULL);
	if (pApp == NULL)
	{
		return (TRUE);
	}

	pContainer = pApp->GetContainer ();
	assert (pContainer != NULL);
	if (pContainer != NULL)
	{
		if (!pContainer->SaveAll ())
		{
			return (FALSE);
		}
	}

	// save panel documents
	pDocTemplate = pApp->m_pPanelTemplate;
	assert (pDocTemplate != NULL);
	if (pDocTemplate != NULL && !pDocTemplate->SaveAllModified ())
	{
		return (FALSE);
	}

	return (TRUE);
}


BOOL CCEMAINDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
    // original code commented out
    //if (!CDocument::OnOpenDocument(lpszPathName))
	//	return FALSE;

    CWaitCursor                     tWaitCursor;
    CComPtr<IUnknown>               pTmp;
    CComQIPtr<ICEProjectManager>    pProjMan;
    CComQIPtr<ICEProjInfo>          pProjInfo;
    CCEStateManager*                pStateManager;
    BOOL                            bRet = TRUE;
    CComBSTR                        bstrPathName(lpszPathName);
    CString                         strStatusText;
    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());

    strStatusText.LoadString(IDS_LOAD_PROJECT_INIT);
    pMainFrame->UpdateStatusBar(strStatusText);

    //check if a project is already open
	CCEMAINApp*         pApp;
	pApp = dynamic_cast<CCEMAINApp*> (AfxGetApp());
	ASSERT(pApp != NULL);
	if (pApp == NULL)
	{
		return (FALSE);
	}

    if (pApp->m_bPrjOpen)
    {
        AfxMessageBox(IDS_CLOSE_CURRENT_PROJECT);
        return FALSE;
    }
    PathFromFile(m_StrPath, lpszPathName);
    SetCurrentDirectory(m_StrPath);

	if(!ProjectFromFile(m_StrProjectTitle, lpszPathName))
    {
		return FALSE;
	}

    // SIS: check if project file exists
    CFileStatus tFileStatus;
    if(!CFile::GetStatus(lpszPathName, tFileStatus))
    {
        CString strMessage;
        strMessage.Format(IDS_FILE_NOT_EXISTING, lpszPathName);
        AfxMessageBox(strMessage);
        return FALSE;
    }
//    else
//    {
//        if(tFileStatus.m_attribute & CFile::readOnly)
//        {
//            CString strMessage;
//            strMessage.Format(IDS_PROJ_READ_ONLY);
//            ::AfxMessageBox(strMessage);
//        }
//    }

    // refuse opening, if project is already opened in other engineering
    if(!CheckProjectOpened(lpszPathName))
    {
        return FALSE;
    }

    // get projman and load project
    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjMan = pTmp;
    pProjInfo = pTmp;

    HRESULT hr;

    CComBSTR sVersion;

    hr = pProjInfo->GetVersionOfProjectFile(bstrPathName, &sVersion);

    if (hr == S_OK)
    {
        CString strVersion = sVersion;
        CString str0;
        if (strVersion.CompareNoCase(CE_FORMAT_VERSION)!=0)
        {
            if (strVersion.CompareNoCase(CE_FORMAT_VERSION_CONVERT)==0)
            {
                str0.Format(IDS_FORMATVERSION_ERR, lpszPathName, strVersion);
                if (IDYES == AfxMessageBox(str0,MB_ICONSTOP|MB_YESNO))
                {
                    strStatusText.LoadString(IDS_LOAD_PROJECT_CONVERT);
                    pMainFrame->UpdateStatusBar(strStatusText);
                    ConvertProject((CString)lpszPathName);
                    strStatusText.LoadString(IDS_LOAD_PROJECT_CONVERT_DONE);
                    pMainFrame->UpdateStatusBar(strStatusText);
                }
                else
                {
                    return FALSE;
                }
            }
            else
            {
                str0.Format(IDS_FORMATVERSION_ERR, lpszPathName, strVersion);
                AfxMessageBox(str0,MB_ICONSTOP|MB_OK);
                return FALSE;
            }
        }
    }
    
    // now lock project by writing lock file
    WriteLockFile(lpszPathName);

    pStateManager = CEGetStateManager();
    if (pStateManager != NULL)
    {
        pStateManager->TriggerPreOpenProject();
    }

    strStatusText.LoadString(IDS_LOAD_PROJECT_LOAD);
    pMainFrame->UpdateStatusBar(strStatusText);

    hr = pProjMan->OpenProject(bstrPathName);
    //SIS, TODO: evaluate HRESULT and do error output
    if(!SUCCEEDED(hr))
    {
        RemoveLockFile(lpszPathName, TRUE);
        return(FALSE);
    }

    m_bOpen = TRUE;
    AfxGetApp()->WriteProfileString(REG_SECTION_ENGINEERING, REG_KEY_LAST_PROJECT, lpszPathName);

// now create the PCAssigner
//    if(bRet)
//    {
//        StrHelp = lpszPathName;
//        m_pPCAssign = new CPCAssign(StrHelp);
//        ASSERT(m_pPCAssign != NULL);
//        // clear the PCList if any and load it from DB
//        bRet &= GetPCsFromDB(pcList);       
//    }
    if (bRet)
    {
//		CGenEdCont *pContainer;

		m_bOpen = TRUE;

        // set project open flag
        pApp->m_bPrjOpen = TRUE;
        pApp->m_bPrjLocked = TRUE;


//        AddPCsFromDB (pcList);

        if (pStateManager != NULL)
        {
            pStateManager->TriggerPostOpenProject ();
        }
    }
//
//    FreePCsFromDB (pcList);
//
    strStatusText.LoadString(IDS_LOAD_PROJECT_LOAD_DONE);
    pMainFrame->UpdateStatusBar(strStatusText);

    return bRet;
}


//------------------------------------------------------------------*
/**
 * load project list view settings.
 *
 * @param           [in] pSettings: pointer to settings
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEMAINDoc::LoadProjectListViewSettings(CSettings* pSettings)
{
    if(!pSettings)
    {
        return;
    }

    POSITION pos = GetFirstViewPosition();
    while (pos != NULL)
    {
        CView* pView = GetNextView(pos);
        ASSERT_VALID(pView);
        CCEMAINFrame* pFrame = dynamic_cast<CCEMAINFrame*> (pView->GetParentFrame());
        if(pFrame)
        {
            pFrame->LoadWindowPlacement(pSettings);
        }
    }
}


//------------------------------------------------------------------*
/**
 * save project list view settings.
 *
 * @param           [in] pSettings: pointer to settings
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEMAINDoc::SaveProjectListViewSettings(CSettings* pSettings)
{
    if(!pSettings)
    {
        return;
    }

    POSITION pos = GetFirstViewPosition();
    while (pos != NULL)
    {
        CView* pView = GetNextView(pos);
        ASSERT_VALID(pView);
        CCEMAINFrame* pFrame = dynamic_cast<CCEMAINFrame*> (pView->GetParentFrame());
        if(pFrame)
        {
            pFrame->SaveWindowPlacement(pSettings);
        }
    }
}


//------------------------------------------------------------------*
/**
 * check if project is opened by other engineering.
 *
 * @param           [in] crstrProjectFilePath: project file name incl path
 * @return          project may be opened?
 * @exception       -
 * @see             
*/
BOOL CCEMAINDoc::CheckProjectOpened(const CString& crstrProjectFilePath)
{
    CString strUser;
    CString strOpenTime;

    // first try to remove lock file
    // this only works, if file is not opened by other user
    RemoveLockFile(crstrProjectFilePath);
    if(ReadLockFile(crstrProjectFilePath, strUser))
    {
        CString strMessage;
        CString strOpenTime;
        CFileStatus tStatus;
        
        VERIFY(CFile::GetStatus(crstrProjectFilePath, tStatus));
        strOpenTime = tStatus.m_atime.Format(IDS_DATE_TIME_FORMAT);
        strMessage.Format(IDS_PROJECT_IS_OPEN, crstrProjectFilePath, strUser, strOpenTime);
        ::AfxMessageBox(strMessage);
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * write lock file.
 *
 * @param           [in] crstrProjectFilePath: project file name incl path
 * @return          successful?
 * @exception       -
 * @see             
*/
BOOL CCEMAINDoc::WriteLockFile(const CString crstrProjectFilePath)
{
    CFileStatus tStatus;
    CString strLockFilePathName = GetLockFilePathName(crstrProjectFilePath);
    try
    {
        if(m_tLockFile.Open(strLockFilePathName, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
        {
            m_tLockFile.WriteString(GetCurrentUser());
//            m_tLockFile.Close();
            CFile::GetStatus(strLockFilePathName, tStatus);
            tStatus.m_attribute |= CFile::hidden;
            tStatus.m_mtime = NULL;
            CFile::SetStatus(strLockFilePathName, tStatus);
            m_tLockFile.Flush();
            m_bLockFileOpen = TRUE;
            return TRUE;
        }
        else
        {
            DWORD   dwLastError = ::GetLastError();
        }
    }
    catch(CFileException* pFileException)
    {
        pFileException->Delete();
        return FALSE;
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * read lock file.
 *
 * @param           [in] crstrProjectFilePath: project file name incl path
 * @param           [out] rstrUser: name of the current user of the project
 * @return          successful?
 * @exception       -
 * @see             
*/
BOOL CCEMAINDoc::ReadLockFile(const CString crstrProjectFilePath, CString& rstrUser)
{
    CString strLockFilePathName = GetLockFilePathName(crstrProjectFilePath);
    CStdioFile  tFile;
    try
    {
        if(tFile.Open(strLockFilePathName, CFile::modeRead | CFile::shareDenyNone))
        {
            tFile.ReadString(rstrUser);
            tFile.Close();
            return TRUE;
        }
    }
    catch(CFileException* pFileException)
    {
        pFileException->Delete();
        return FALSE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * remove lock file.
 *
 * @param           [in] crstrProjectFilePath: project file name incl path
 * @param           [in] bForce: force delete?
 * @return          successful?
 * @exception       -
 * @see             
*/
BOOL CCEMAINDoc::RemoveLockFile(const CString& crstrProjectFilePath, BOOL bForce /*=FALSE*/)
{
    BOOL    bReturn = FALSE;
    CString strLockFilePathName = GetLockFilePathName(crstrProjectFilePath);
    if(bForce)
    {
        bReturn = ::DeleteFile(strLockFilePathName);
        if(!bReturn)
        {
            DWORD   dwLastError = ::GetLastError();
        }
        return bReturn;
    }

    CFile   tFile;
    try
    {
        CFile::Remove(strLockFilePathName);
        bReturn = TRUE;
    }
    catch (CFileException* pE)
    {
        pE->Delete();
    }
    return bReturn;
}


//------------------------------------------------------------------*
/**
 * get lock file path name.
 *
 * @param           [in] crstrProjectFilePath: file name incl path of 4cp file
 * @return          file name incl path of project lock file
 * @exception       -
 * @see             
*/
CString CCEMAINDoc::GetLockFilePathName(const CString& crstrProjectFilePath)
{
    CString strFileName;
    CString strFilePath;
    int     iFound;

    // split in file name and path
    iFound = crstrProjectFilePath.ReverseFind(_T('\\'));
    if(iFound >= 0)
    {
        strFilePath = crstrProjectFilePath.Left(iFound + 1);
    }
    strFileName = crstrProjectFilePath.Mid(iFound + 1);

    // remove file extension
    iFound = strFileName.ReverseFind(_T('.'));
    if(iFound >= 0)
    {
        strFileName = strFileName.Left(iFound);
    }

    // create lock file name by add preceeding "~$"
    strFileName = _T("~$") + strFileName;

    return(strFilePath + strFileName);
}


//------------------------------------------------------------------*
/**
 * get current user.
 *
 * @return          current user name
*/
CString CCEMAINDoc::GetCurrentUser()
{
    // determine user name from system
    DWORD   dwLength = _MAX_PATH + 1;
    CString strUser;

    ::WNetGetUser(NULL, strUser.GetBuffer(_MAX_PATH + 1), &dwLength);
    strUser.ReleaseBuffer();
    return(strUser);
}

void CCEMAINDoc::OnUpdateDocumentOpen(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(m_bOpen);
}

void CCEMAINDoc::OnUpdateFileClose(CCmdUI* pCmdUI) 
{
    // only close file if editor is open
    CGenEdCont* pContainer = NULL;
	CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp ());
    BOOL        bOpen = FALSE;

	assert (pApp != NULL);
	if (pApp == NULL)
	{
        bOpen = TRUE;
	}

	pContainer = pApp->GetContainer();
	assert (pContainer != NULL);
	if (pContainer != NULL)
	{
        if(pContainer->GetActiveDocument())
        {
            bOpen = TRUE;
        }
    }
    pCmdUI->Enable(bOpen);
}



//SIS, 06.02.01: copy from V1.3
void PathFromFile(CString& StrPath,LPCTSTR lpszFileName)
{
    CString StrFile;
    int iPos;

    StrPath.Empty();
    StrFile = lpszFileName;

    if(StrFile.IsEmpty())
        return;

    iPos = StrFile.ReverseFind(_T('\\'));
    StrPath = StrFile.Left(iPos+1);
    return;
}


//SIS, 06.02.01: in analogy of PathFromFile (avoided UTIL_GetFileTitle)
BOOL ProjectFromFile(CString & StrProject, LPCTSTR lpszFileName)
{
    CString StrFile;
    int iPos, iLen;

    StrProject.Empty();
    StrFile = lpszFileName;

    if(StrFile.IsEmpty())
        return FALSE;

    iLen = StrFile.GetLength();
    iPos = StrFile.ReverseFind(_T('\\'));
    StrProject = StrFile.Right(iLen - iPos - 1);
    return TRUE;
}

//void CCEMAINDoc::RemoveAllTargets(bool bKeepAssignments /*=false*/)
void CCEMAINDoc::RemoveAllTargets()
{
    CTargetSet *pProjectTargets;
//    bool bSavedKeep;

    pProjectTargets = CEGetTargetSet ("Project Targets");
    ASSERT (pProjectTargets != NULL);
    if (pProjectTargets == NULL)
        return;

//    bSavedKeep = m_bKeepAssignments;
//    m_bKeepAssignments = bKeepAssignments;

    pProjectTargets->RemoveAllTargets ();

//    m_bKeepAssignments = bSavedKeep;
}





BOOL ConvertProject(CString& strProject)
{
    IUnknown *pUnk = NULL;
    pUnk = CEGetCOM (CE_COMPMAN_EXE, true);
    
    if (!pUnk)
    {
        AfxMessageBox(IDS_ERR_EXECUTE, MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    CComQIPtr<ICEExecute> pExeDelegator;
    pExeDelegator = pUnk;
    pUnk->Release();
    if (!pExeDelegator)
    {
        AfxMessageBox(IDS_ERR_EXECUTE, MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    ASSERT(pMainFrame);
    if (pMainFrame != NULL)
    {
        CMsgWnd* pMsgWnd = NULL;
        pMsgWnd = pMainFrame->GetMsgView();
        if (pMsgWnd)
        {
            pMsgWnd->ShowSystemWindow();
        }
    }

    CString strCmd;
    CString strConverter;

    TCHAR pszInstPath[1000];   
    size_t sizeChars = 1000;
    CString strInstPath;

    HRESULT hr = UTIL_GetInstallPath(pszInstPath, &sizeChars);
    if (hr==S_OK)
    {
        strInstPath = pszInstPath;
    }

    strConverter = strInstPath + _T("\\") + CE_CONVERTER;

    strCmd.Format(_T("%s \"%s\""), strConverter, strProject);

    CComBSTR sCommand(strCmd);
    DWORD dwReturnVal;
    
    hr = pExeDelegator->ExecuteCommand(sCommand, NULL,
                                       0, TRUE, FALSE, 
                                       &dwReturnVal);

    if (!SUCCEEDED(hr))
    {
        AfxMessageBox(IDS_ERR_EXECUTE, MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    if (dwReturnVal!=0)
    {
        AfxMessageBox(IDS_ERR_EXECUTE, MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
    }

    return TRUE;
}