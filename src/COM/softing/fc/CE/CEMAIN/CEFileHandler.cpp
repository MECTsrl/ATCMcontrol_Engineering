


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

#include "stdafx.h"
#include "CEFileHandler.h"
#include "GenEdCntrItem.h"
#include "resource.h"
#include "cedb.h"
#include "cemain.h"
#include "navigator.h"
#include "CECompMan.h"
#include "utilif.h"
#include "fc_tools/fc_string.h"
#include "pouvarselwindow.h"
#include "pouvarselctrlwrapper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCEFileHandler

IMPLEMENT_DYNCREATE(CCEFileHandler, CCmdTarget)

BEGIN_MESSAGE_MAP(CCEFileHandler, CCmdTarget)
	//{{AFX_MSG_MAP(CCEFileHandler)
		// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//BEGIN_DISPATCH_MAP(CCEFileHandler, CCmdTarget)
//	//{{AFX_DISPATCH_MAP(CGenEdCont)
//	//}}AFX_DISPATCH_MAP
//END_DISPATCH_MAP()

// {160D82B0-7004-11d2-B9CE-006008749B3D}
static const IID IID_ICEFileHandler =
{ 0x160d82b0, 0x7004, 0x11d2, { 0xb9, 0xce, 0x0, 0x60, 0x8, 0x74, 0x9b, 0x3d } };

BEGIN_INTERFACE_MAP(CCEFileHandler, CCmdTarget)
	INTERFACE_PART(CCEFileHandler, IID_ICEFileHandler, Dispatch)
	INTERFACE_PART(CCEFileHandler, IID__IBreakPointNotify, _IBreakPointNotifyObj)
END_INTERFACE_MAP()


CCEFileHandler::CCEFileHandler()
: m_pContainer(NULL),
  m_pIBreakPoint(NULL),
  m_dwBPNotifyCookie(0),
  m_bInitialized(false),
  m_bProjectLoaded(false),
  m_bParsingEnabled(true),
//  m_ReloadedCB(this, OnDocumentsReloaded),
  m_StateChangeCB(this, OnStateChange),
//  m_ActionCB(this, OnGenEdAction),
  m_uiStoppointDelayCookie(0)
{
}


CCEFileHandler::~CCEFileHandler()
{
    if(m_bInitialized)
    {
        Cleanup();
    }
}


HRESULT CCEFileHandler::Init(CGenEdCont *pContainer)
{
    CCEStateManager *pStateManager;

    if(m_bInitialized)
    {
        return(S_FALSE);
    }

    ASSERT(pContainer != NULL);
    if(pContainer == NULL)
    {
        return(E_INVALIDARG);
    }

    pStateManager = CEGetStateManager();
    if(pStateManager == NULL)
    {
        return(E_UNEXPECTED);
    }

    pStateManager->RegisterCallback(&m_StateChangeCB);

    m_pContainer = pContainer;
//    pContainer->RegisterReloadedCB(&m_ReloadedCB);

    m_bInitialized = true;
    return(S_OK);
}


void CCEFileHandler::Cleanup()
{
    CCEStateManager *pStateManager;

    if(!m_bInitialized)
    {
        return;
    }

    SetBreakpointManager(NULL);
//    if(m_pContainer != NULL)
//    {
//        m_pContainer->UnregisterReloadedCB(&m_ReloadedCB);
//    }

    pStateManager = CEGetStateManager();
    if(pStateManager != NULL)
    {
        pStateManager->UnregisterCallback(&m_StateChangeCB);
    }

    while(!m_StoppointDatas.IsEmpty())
    {
        StoppointData *pData;

        pData = m_StoppointDatas.RemoveHead();
        delete pData;
    }
}

void CCEFileHandler::EnableParsing(bool bEnable) 
{
	m_bParsingEnabled = bEnable;
}

bool CCEFileHandler::IsParseEnabled() 
{
	return(m_bParsingEnabled);
}


//------------------------------------------------------------------*
/**
 * show file.
 *
 * @param           [in] strPath: file name, file name incl. rel path or
 *                                  file name incl full path
 * @param           [out] pDoc: document pointer to opened document
 * @param           [in] bActivateInst: activate instance tab?
 *                                   if true, the document is not activated
 *                                   a subsequent call of ShowInstance() is required
 *                                   default false
 * @return          S_OK:   ok
 *                  E_FAIL: not ok
 *                  return values from ShowDocument()
 * @exception       -
 * @see             
*/
HRESULT CCEFileHandler::ShowFile(const CString &strPath, CGenEdDoc *&pDoc, bool bActivateInst /*=false*/, bool bForceAsText /* = false */)
{
    HRESULT hr;
    CString strFullPath;
    E_ActivateAction    eAction = bActivateInst ? eActivateInstance : eActivateClass;

    if(!m_bInitialized)
    {
        return(E_FAIL);
    }

    strFullPath = GetFilePathName(strPath);

    hr = ShowDocument(strFullPath, pDoc, eAction, bForceAsText);
    if(FAILED(hr))
    {
        CString strMessage;

        strMessage.LoadString(IDS_CANNOT_OPEN_FILE);
        AfxMessageBox(strMessage);

        return(hr);
    }

    return(S_OK);
}


HRESULT CCEFileHandler::ShowFileAt(const CString& strPath, const CString& crstrLocation, CGenEdDoc *&pDoc, bool bActivateInst /*= false*/)
{
    HRESULT hr;
    CString strFullPath;
    E_ActivateAction    eAction = bActivateInst ? eActivateInstance : eActivateClass;

    if(!m_bInitialized)
    {
        return(E_FAIL);
    }

    strFullPath = GetFilePathName(strPath);
    hr = ShowDocument(strFullPath, pDoc, eActivateClass);
    if(FAILED(hr))
    {
        CString strMessage;

        strMessage.LoadString(IDS_CANNOT_OPEN_FILE);
        AfxMessageBox(strMessage);

        return(hr);
    }

    if(m_pContainer && pDoc)
    {
        m_pContainer->ShowLocation (pDoc, crstrLocation);
    }
    return(S_OK);
}


HRESULT CCEFileHandler::LoadFile(const CString &strPath)
{
    HRESULT hr;
    CString strFullPath;

    if(!m_bInitialized)
    {
        return(E_FAIL);
    }

    strFullPath = GetFilePathName(strPath);

    hr = LoadDocument(strFullPath);
    if(FAILED(hr))
    {
        return(hr);
    }

    return(S_OK);
}


HRESULT CCEFileHandler::SaveFile(const CString &strPath)
{
    HRESULT hr;
    CString strFullPath;

    if(!m_bInitialized)
    {
        return(E_FAIL);
    }

    strFullPath = GetFilePathName(strPath);

    hr = SaveDocument(strFullPath);
    if(FAILED(hr))
    {
        return(hr);
    }

    return(S_OK);
}


HRESULT CCEFileHandler::CloseFile(const CString &strPath)
{
    HRESULT hr;
    CString strFullPath;

    if(!m_bInitialized)
    {
        return(E_FAIL);
    }

    strFullPath = GetFilePathName(strPath);

    hr = CloseDocument(strFullPath);
    return(hr);
}


HRESULT CCEFileHandler::IsFileOpen(const CString &strPath, CGenEdDoc *&pDoc)
{
    CString strFullPath;

    if(!m_bInitialized)
    {
        return(E_FAIL);
    }

    pDoc = FindFileDocument(strPath);
    if(pDoc == NULL)
    {
        return(S_FALSE);
    }

    return(S_OK);
}




/////////////////////////////////////////////////////////////////////////////
// State change handling for breakpoints/stoppoints

HRESULT CCEFileHandler::OnBPStateChanged(BPM_BRKPT_STATE newState, const CString &strInstance, CComPtr<IBreakPointInfo>& pBPInfo, bool bUpdate)
{
    HRESULT hr;
    CString strPath;
    long lX;
    long lY;
    long lCX;
    long lCY;
    EDP_POS_QUALIFIER posQual;
    CString strFullPath;
    CGenEdDoc* pDoc;
    short sState;


    // If no project open just forget notification.

    if(!m_bProjectLoaded)
        return(S_OK);

    ASSERT(pBPInfo != NULL);
    if(pBPInfo == NULL)
    {
        return(E_POINTER);
    }

    hr = ReadBreakpointInfo(pBPInfo, strPath, posQual, lX, lY, lCX, lCY);
    if(FAILED(hr))
    {
        return(hr);
    }

    strFullPath = GetFilePathName(strPath);

    pDoc = FindInstanceDocument(strFullPath, strInstance);
    if(pDoc == NULL)
    {
        return(S_OK);
    }

    if(newState == BPM_NOBP)
    {
        hr = pDoc->HideBreakpoint(posQual, lX, lY, lCX, lCY);
    }
    else
    {
        switch(newState)
        {
        case BPM_BPINACTIVE   : sState = 0;  break;
        case BPM_BPACTIVE     : sState = 1;  break;
        case BPM_BPACTIVEUNSET: sState = -1; break;

        default:
            return(E_FAIL);
        }

        hr = pDoc->ShowBreakpoint(posQual, lX, lY, lCX, lCY, sState);
    }

    return(hr);
}


HRESULT CCEFileHandler::OnSPStateChanged(BPM_STPPT_STATE newState, const CString &strInstance, CComPtr<IBreakPointInfo>& pBPInfo, bool bUpdate)
{
    CCEStateManager *pStateManager;
    StoppointData *pData;

    ASSERT(pBPInfo != NULL);
    if(pBPInfo == NULL)
        return(E_POINTER);

    pStateManager = CEGetStateManager();
    if(pStateManager == NULL)
        return(E_FAIL);

    pData = new StoppointData;
    ASSERT(pData != NULL);
    if(pData == NULL)
        return(E_OUTOFMEMORY);

    pData->m_State = newState;
    pData->m_strInstance = strInstance;
    pData->m_pBPInfo = pBPInfo;
    pData->m_bUpdate = bUpdate;

    m_StoppointDatas.AddTail(pData);

    if(m_uiStoppointDelayCookie == 0)
    {
        m_uiStoppointDelayCookie = pStateManager->TriggerDelay(NULL);
    }

    return(S_OK);
}


void CCEFileHandler::HandleStoppoint(StoppointData *pData)
{
    HRESULT hr;

    ASSERT(pData != NULL);
    if(pData == NULL)
        return;

    switch(pData->m_State)
    {
    case BPM_STOPPED:
        hr = OnTaskStopped(pData->m_strInstance, pData->m_pBPInfo, pData->m_bUpdate);
        break;

    case BPM_CONTINUED:
        hr = OnTaskContinued(pData->m_strInstance, pData->m_pBPInfo, pData->m_bUpdate);
        break;

    default:
        ASSERT(false);
        break;
    }
}


HRESULT CCEFileHandler::OnTaskStopped(const CString &strInstance, CComPtr<IBreakPointInfo>& pBPInfo, bool bUpdate)
{
    HRESULT hr;
    long lQuality;
    CString strPath;
    long lX;
    long lY;
    long lCX;
    long lCY;
    EDP_POS_QUALIFIER posQual;
    CString strFullPath;
    CGenEdDoc *pDoc;
    CGenEdCntrItem *pServer;
    bool bUserBreakpoint;


    // If no project open just forget notification.

    if(!m_bProjectLoaded)
        return(S_OK);

    hr = pBPInfo->get_Quality(&lQuality);
    if(SUCCEEDED(hr))
    {
        hr = ReadBreakpointInfo(pBPInfo, strPath, posQual, lX, lY, lCX, lCY);
    }

    if(FAILED(hr))
    {
        FC_DEBUGPRINT2("CEMAIN> CCEFileHandler::OnTaskStopped: failed to get stoppoint information(path=%s, hr=0x%08lx)\n",
                       (LPCTSTR)strInstance, hr);

        if(!bUpdate)
        {
            CString strMessage;

            strMessage.Format(IDS_TASK_STOPPED_NO_BPINFO,(LPCTSTR)strInstance);
            AfxMessageBox(strMessage);
        }

        return(hr);
    }

    bUserBreakpoint = lQuality == 1;

    if(bUpdate)
    {
        if(bUserBreakpoint)
        {
            return(S_OK);
        }

        if(strPath.IsEmpty())
        {
            return(S_OK);
        }

        strFullPath = GetFilePathName(strPath);

        pDoc = FindInstanceDocument(strFullPath, strInstance);
        if(pDoc == NULL)
        {
            return(S_OK);
        }

        hr = pDoc->ShowStoppoint(posQual, lX, lY, lCX, lCY);
        if(FAILED(hr))
        {
            FC_DEBUGPRINT2("CEMAIN> CCEFileHandler::OnTaskStopped: failed to update stoppoint(path=%s, hr=0x%08lx)\n",
                           (LPCTSTR)strInstance, hr);
        }

        return(hr);
    }

    if(strPath.IsEmpty())
    {
        hr = E_FAIL;
    }
    else
    {
        CString strFullPath;

        strFullPath = GetFilePathName(strPath);

        hr = ShowDocument(strFullPath, pDoc, eActivateNone);

        if(SUCCEEDED(hr))
        {
            hr = m_pContainer->ShowInstance(pDoc, strInstance);
        }
    }

    if(FAILED(hr))
    {
        CString strMessage;

        if(bUserBreakpoint)
        {
            strMessage.Format(IDS_USER_BREAKPOINT_NO_SOURCE,(LPCTSTR)strInstance);
        }
        else
        {
            strMessage.Format(IDS_TASK_STOPPED_NO_SOURCE,(LPCTSTR)strInstance);
        }

        AfxMessageBox(strMessage);

        return(hr);
    }

    if(bUserBreakpoint)
    {
        CString strMessage;

        strMessage.Format(IDS_USER_BREAKPOINT_REACHED,(LPCTSTR)strInstance);
        AfxMessageBox(strMessage);

        return(S_OK);
    }

    pServer = pDoc->GetEditorServer();
    if(pServer != NULL)
    {
        pDoc->GotoPosition(pServer, posQual, lX, lY, lCX, lCY);
    }

    hr = pDoc->ShowStoppoint(posQual, lX, lY, lCX, lCY);

    return(hr);
}


HRESULT CCEFileHandler::OnTaskContinued(const CString &strInstance, CComPtr<IBreakPointInfo>& pBPInfo, bool bUpdate)
{
    HRESULT hr;
    CString strPath;
    long lX;
    long lY;
    long lCX;
    long lCY;
    EDP_POS_QUALIFIER posQual;
    CString strFullPath;
    CGenEdDoc *pDoc;


    // If no project open just forget notification.

    if(!m_bProjectLoaded)
        return(S_OK);

    hr = ReadBreakpointInfo(pBPInfo, strPath, posQual, lX, lY, lCX, lCY);
    if(FAILED(hr))
    {
        return(hr);
    }

    strFullPath = GetFilePathName(strPath);

    pDoc = FindInstanceDocument(strFullPath, strInstance);
    if(pDoc == NULL)
    {
        return(S_OK);
    }

    hr = pDoc->HideStoppoint();
    if(FAILED(hr))
    {
        return(hr);
    }

    return(S_OK);
}


HRESULT CCEFileHandler::OnShowBP(const CString &strInstance, CComPtr<IBreakPointInfo>& pBPInfo)
{
    HRESULT hr;
    long lQuality;
    CString strPath;
    long lX;
    long lY;
    long lCX;
    long lCY;
    EDP_POS_QUALIFIER posQual;
    CGenEdDoc *pDoc;
    CGenEdCntrItem *pServer;
    bool bUserBreakpoint;


    // If no project open just forget notification.

    if(!m_bProjectLoaded)
        return(S_OK);

    hr = pBPInfo->get_Quality(&lQuality);
    if(SUCCEEDED(hr))
    {
        hr = ReadBreakpointInfo(pBPInfo, strPath, posQual, lX, lY, lCX, lCY);
    }

    if(FAILED(hr))
    {
        FC_DEBUGPRINT2("CEMAIN> CCEFileHandler::OnShowBP: failed to get breakpoint information(path=%s, hr=0x%08lx)\n",
                       (LPCTSTR)strInstance, hr);


        AfxMessageBox(IDS_GOTO_BREAKPOINT_NO_BPINFO);
        return(hr);
    }

    bUserBreakpoint = lQuality == 1;

    if(strPath.IsEmpty())
    {
        hr = E_FAIL;
    }
    else
    {
        CString strFullPath;

        strFullPath = GetFilePathName(strPath);

        hr = ShowDocument(strFullPath, pDoc, eActivateNone);

        if(SUCCEEDED(hr))
        {
            hr = m_pContainer->ShowInstance(pDoc, strInstance);
        }
    }

    if(FAILED(hr))
    {
        AfxMessageBox(IDS_GOTO_BREAKPOINT_NO_SOURCE);
        return(hr);
    }

    if(!bUserBreakpoint)
    {
        pServer = pDoc->GetEditorServer();
        if(pServer != NULL)
        {
            pDoc->GotoPosition(pServer, posQual, lX, lY, lCX, lCY);
        }
    }

    return(S_OK);
}


//------------------------------------------------------------------*
/**
 * show document.
 *
 * @param            [in] strPath: full file path
 * @param            [out] pDoc: document pointer
 * @param            [in] eAction: eActivateNone, eActivateClass or eActivateInstance
 *                                   if true, the document is not activated
 *                                   a subsequent call of ShowInstance() is required
 *                                   default false
 * @return           
 * @exception        -
 * @see              
*/
HRESULT CCEFileHandler::ShowDocument(const CString &strPath, CGenEdDoc *&pDoc, E_ActivateAction eAction /*=eActivateClass*/, bool bForceText /*=false*/)
{
    HRESULT             hr;
    CComQIPtr<ICEProjectManager> pProject;
    bool                bSourceFile = false;
    E_FV_FileCategory   eCategory;
    bool                bInProject(false);
    bool                bExternEditor(false);
    CString             strFilePathName;
    CString             strFileName(GetFileName(strPath));

    // initialize pDoc with NULL
    pDoc = NULL;

    strFilePathName = GetFilePathName(strPath);

    if(IsLibraryHeader(strFilePathName))
    {
        ShowLibraryHeader(strFilePathName, pDoc);
        if(!pDoc)
        {
            return E_FAIL;
        }
        return S_OK;
    }

    pDoc = m_pContainer->FindDocument(strFilePathName);

    if (bForceText)
    {
        if (pDoc)
        {
            m_pContainer->ActivateDocument(pDoc, eAction);
            return S_OK;
        }
        else
        {
            pDoc = m_pContainer->OpenDocument(strFilePathName, true, true, eActivateClass, true);
            return S_OK;
        }
    }
    
    if(!Is4CEEditFile(strFileName))      // look if the file can be opened to edit in ATCM 
    {
        pDoc = NULL;
        // try to open in an extern editor
        //HINSTANCE 
        ShellExecute(
            NULL,			//HWND hwnd, 
            "open",			//LPCTSTR lpVerb,
            strPath,		//LPCTSTR lpFile, 
            NULL,			//LPCTSTR lpParameters, 
            NULL,			//LPCTSTR lpDirectory,
            SW_SHOWNORMAL); //INT nShowCmd
        return S_FALSE;     // don't return TRUE since pDoc is NULL
    }


    // file is within project:
    if(IsWithinProjectPath(strFilePathName))
    {
        if(!GetProject(pProject))
        {
            return(E_FAIL);
        }

        CComBSTR    sFileName = strFileName;
        hr = pProject->GetFileCategory(sFileName, &eCategory);
        if(hr == S_OK)
        {
            if(eCategory == FV_FILE_SOURCE)
            {
                bSourceFile = true;
            }
        }

        // document not found -> open it
        if(!pDoc)
        {
            // try to open file
            CFile file;
            if(!file.Open(strFilePathName, CFile::modeRead | CFile::shareDenyWrite))
            {
                return(E_FAIL);
            }
            file.Close();

            pDoc = m_pContainer->OpenDocument(strFilePathName, !bSourceFile, true, eAction);

            // if document is not valid -> return error
            if(pDoc == NULL)
            {
                return(E_FAIL);
            }
//            pDoc->RegisterActionCB(&m_ActionCB);
        }
        // document found -> just activate it
        else
        {
            m_pContainer->ActivateDocument(pDoc, eAction);
        }
    }
    // file is not within project -> no instance tabs
    else
    {
        if(!pDoc)
        {
            pDoc = m_pContainer->OpenDocument(strFilePathName, true);
        }
        else
        {
            m_pContainer->ActivateDocument(pDoc, eAction);
        }
    }

    return(S_OK);
}







//------------------------------------------------------------------*
/**
 * is library header.
 *
 *  look for "::"
 *
 * @param           crstrFileString: library name :: header name
 * @return          is library header?
 * @exception       -
 * @see             -
*/
BOOL CCEFileHandler::IsLibraryHeader(const CString& crstrFileString)
{
    if(crstrFileString.Find(_T("::")) >= 0)
    {
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
/**
 * show library header.
 *
 *  use var selection to open library header file.
 *
 * @param           crstrFileString: library name :: header name
 * @param           pDoc: document pointer
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CCEFileHandler::ShowLibraryHeader(const CString& crstrFileString, CGenEdDoc*& pDoc)
{
    CString strHeaderFilePathName;
    CString strHeaderFilePathNameOut;
    HRESULT hr = E_FAIL;

    strHeaderFilePathName = GetFilePathName(crstrFileString);

    // get pou selection
    CPouVarSelWindow* pPouVarSelWnd = dynamic_cast<CPouVarSelWindow *>(CEGetDockingTool (CE_COMPMAN_POUVARSEL, false));
    if(pPouVarSelWnd)
    {
        CPouVarSelCtrlWrapper*  pPouVarSel = pPouVarSelWnd->GetWrapper();
        hr = pPouVarSel->LookupHeader(strHeaderFilePathName, strHeaderFilePathNameOut);
        if(hr == S_OK)
        {
            pDoc = m_pContainer->FindDocument(strHeaderFilePathNameOut);
            if(!pDoc)
            {
                pDoc = m_pContainer->OpenDocument(strHeaderFilePathNameOut, false, true);
                if(pDoc)
                {
                    pDoc->ForceReadOnly(true);
                }
                else
                {
                    return E_FAIL;
                }
            }
            else
            {
                m_pContainer->ActivateDocument(pDoc);
            }
        }
    }
    return hr;
}


//------------------------------------------------------------------*
/**
 * is library header existing?
 *
 * @param           crstrFileString: header incl library path.
 * @return          is library header existing?
 * @exception       -
 * @see             -
*/
BOOL CCEFileHandler::IsLibraryHeaderExisting(const CString& crstrFileString)
{
    CString strHeaderFilePathName;
    CString strHeaderFilePathNameOut;
    HRESULT hr = E_FAIL;

    strHeaderFilePathName = GetFilePathName(crstrFileString);

    // get pou selection
    CPouVarSelWindow* pPouVarSelWnd = dynamic_cast<CPouVarSelWindow *>(CEGetDockingTool (CE_COMPMAN_POUVARSEL, false));
    if(pPouVarSelWnd)
    {
        CPouVarSelCtrlWrapper*  pPouVarSel = pPouVarSelWnd->GetWrapper();
        hr = pPouVarSel->LookupHeader(strHeaderFilePathName, strHeaderFilePathNameOut);
        if(hr == S_OK)
        {
            return TRUE;
        }
    }
    return FALSE;
}



HRESULT CCEFileHandler::LoadDocument(const CString &strPath)
{
    CGenEdDoc *pDoc;

    pDoc = m_pContainer->FindDocument(strPath);
    if(pDoc == NULL)
    {
        return(S_OK);
    }

    if(!m_pContainer->LoadDocument(pDoc))
    {
        return(E_FAIL);
    }

    return(S_OK);
}


HRESULT CCEFileHandler::SaveDocument(const CString &strPath)
{
    CGenEdDoc *pDoc;
    bool bSuccess;

    pDoc = m_pContainer->FindDocument(strPath);
    if(pDoc == NULL)
    {
        return(S_OK);
    }

    bSuccess = m_pContainer->SaveDocument(pDoc);

    if(!bSuccess)
    {
        return(E_FAIL);
    }

    // parse now
    Parse(NULL);

    return(S_OK);
}


HRESULT CCEFileHandler::CloseDocument(const CString &strPath)
{
    CGenEdDoc *pDoc;

    pDoc = m_pContainer->FindDocument(strPath);
    if(pDoc == NULL)
    {
        return(S_FALSE);
    }

    m_pContainer->CloseDocument(pDoc);

    return(S_OK);
}


//void CCEFileHandler::OnGenEdAction(GenEdAction action, CGenEdDoc *pDoc)
//{
//    if(action == GenEdDocumentSaved)
//    {
//	    Parse(pDoc);
//    }
//}

//void CCEFileHandler::OnDocumentsReloaded()
//{
//    Parse(NULL);
//}
//


void CCEFileHandler::Parse(CGenEdDoc *pDoc)
{
    CString strExt;
    bool bHWFile=false;
    CComQIPtr<ICEProjInfo> pProject;
    CString str4CHFile;

    if(!m_bParsingEnabled)
    {
        return;
    }

    GetProjectInfo(pProject);
    ASSERT(pProject != NULL);
    if(pProject != NULL)
    {
        CComBSTR    sFileName;
        if(pDoc)
        {
            // use CFile to get the file name from path
            CFile       tFile;
            tFile.SetFilePath(pDoc->GetPathName());
            sFileName = tFile.GetFileName();
            pProject->fileChangedHint(sFileName);  // if sFileName is empty project file will be parsed
        }
        else
        {
            pProject->fileParseNow(NULL);
        }
    }
}


CGenEdDoc *CCEFileHandler::FindFileDocument(const CString &strPath)
{
    CString strFullPath;

    strFullPath = GetFilePathName(strPath);

    return(m_pContainer->FindDocument(strFullPath));
}




BOOL CCEFileHandler::GetProject(CComQIPtr<ICEProjectManager>& pProjMan)
{
    CComPtr<IUnknown>   pTmp;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjMan = pTmp;
    if(!pProjMan)
    {
        return FALSE;
    }
    return TRUE;
}



BOOL CCEFileHandler::GetProjectInfo(CComQIPtr<ICEProjInfo>& pProjInfo)
{
    CComPtr<IUnknown>   pTmp;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjInfo = pTmp;
    if(!pProjInfo)
    {
        return FALSE;
    }
    return TRUE;
}

HRESULT CCEFileHandler::CheckRelative(const CString &strPath)
{
    HRESULT hr;

    hr = UTIL_IsAbsolutePath(strPath);
    if(FAILED(hr))
    {
        return(hr);
    }

    if(hr == S_OK)
    {
        return(E_FAIL);
    }

    return(S_OK);
}


CGenEdDoc *CCEFileHandler::FindInstanceDocument(const CString &strPath, const CString &strInstance)
{
    CGenEdDoc *pDoc;
    CString strLowerInstance;
    CString strDocInstance;
    CString strDocMachineName;
    CString strDocMachineAddress;

    pDoc = m_pContainer->FindDocument(strPath);
    if(pDoc == NULL)
    {
        return(NULL);
    }

    strLowerInstance = strInstance;
    strLowerInstance.MakeLower();

    pDoc->GetInstance(strDocInstance, strDocMachineName, strDocMachineAddress);
    strDocInstance.MakeLower();

    if(strLowerInstance != strDocInstance)
    {
        return(NULL);
    }

    return(pDoc);
}




HRESULT CCEFileHandler::ReadBreakpointInfo(
    CComPtr<IBreakPointInfo>& pInfo, 
    CString&                  strPath, 
    EDP_POS_QUALIFIER&        posQual,
    long&                     lX, 
    long&                     lY, 
    long&                     lCX, 
    long&                     lCY
)
{
    HRESULT hr;
    BSTR sPath;

    hr = pInfo->get_FileName(&sPath);
    if(FAILED(hr))
    {
        return(hr);
    }
    strPath = sPath;
    ::SysFreeString(sPath);

    hr = pInfo->get_PosQualifier(&posQual);
    if(FAILED(hr))
    {
        return(hr);
    }

    hr = pInfo->get_X(&lX);
    if(FAILED(hr))
    {
        return(hr);
    }

    hr = pInfo->get_Y(&lY);
    if(FAILED(hr))
    {
        return(hr);
    }

    hr = pInfo->get_CX(&lCX);
    if(FAILED(hr))
    {
        return(hr);
    }

    hr = pInfo->get_CY(&lCY);
    if(FAILED(hr))
    {
        return(hr);
    }

    return(S_OK);
}


void CCEFileHandler::OnStateChange(CEStateNotify stateNotify, unsigned int uiStage)
{
    switch(stateNotify)
    {
    case CEPreOpenProjectNotify:
        {
            CString strLibPath;
            UTIL_GetStdLibPath(strLibPath.GetBuffer(_MAX_PATH), _MAX_PATH);
            strLibPath.ReleaseBuffer();
            m_strLibPath = strLibPath;
        }
        break;
    case CEPostOpenProjectNotify:
        m_bProjectLoaded = TRUE;
        {
            // get project path
            CComQIPtr<ICEProjInfo> pProjInfo;

            GetProjectInfo(pProjInfo);
            ASSERT(pProjInfo);
            if(pProjInfo)
            {
                CComBSTR    sProjPath;
                pProjInfo->getProjectPath(&sProjPath);
                m_strProjPath = sProjPath;
            }
        }
        break;
    case CEPreCloseProjectNotify:
        m_bProjectLoaded = FALSE;
        m_strProjPath.Empty();
        break;
    case CEComponentAccessibleNotify:
        if(CESMGetComponentName() == CE_COMPMAN_BREAKPTMAN)
        {
            HRESULT hr;
            CComPtr<IUnknown> pUnknown;
            CComPtr<IBreakPoint> pBreakpointManager;

            SetBreakpointManager(NULL);

            pUnknown.Attach(CEGetCOM(CE_COMPMAN_BREAKPTMAN, true));
            if(pUnknown != NULL)
            {
                hr = pUnknown->QueryInterface(IID_IBreakPoint,(LPVOID *)&pBreakpointManager);
                ASSERT(SUCCEEDED(hr));
                if(SUCCEEDED(hr))
                {
                    SetBreakpointManager(pBreakpointManager);
                }
            }
        }
        break;
    case CEHandsOffComponentNotify:
        if(CESMGetComponentName() == CE_COMPMAN_BREAKPTMAN)
        {
            SetBreakpointManager(NULL);
        }
        break;
    case CEDelayNotify:
        {
            CList<StoppointData *, StoppointData *> stoppointDatas;

            if(m_uiStoppointDelayCookie == 0 || CESMGetDelayCookie() != m_uiStoppointDelayCookie)
                return;

            while(!m_StoppointDatas.IsEmpty())
            {
                stoppointDatas.AddTail(m_StoppointDatas.RemoveHead());
            }
            m_uiStoppointDelayCookie = 0;

            while(!stoppointDatas.IsEmpty())
            {
                StoppointData *pData;

                pData = stoppointDatas.RemoveHead();
                HandleStoppoint(pData);
                delete pData;
            }
        }
        break;
    }
}

//------------------------------------------------------------------*
/**
 * show instance.
 *
 * @param           [in] crstrFileString: file string
 * @return          
 * @exception       -
 * @see             
*/
BOOL CCEFileHandler::ShowInstance(const CString& crstrFileString, const CString& crstrInst)
{
    CString strFullPath;
    CGenEdDoc* pDoc;
    HRESULT hr;

    strFullPath = GetFilePathName(crstrFileString);

    pDoc = FindInstanceDocument(strFullPath, crstrInst);
    // document is not open currently -> open it.
    if(pDoc == NULL)
    {
        // 
        hr = ShowFile(strFullPath, pDoc, false);
        if(hr != S_OK)
        {
            return FALSE;
        }
    }
    // document is open -> just show it
    else
    {
        hr = ShowDocument(strFullPath, pDoc, eActivateNone);
        if(hr != S_OK)
        {
            return FALSE;
        }
    }

    // show instance
    hr = m_pContainer->ShowInstance(pDoc, crstrInst);

    return TRUE;
}


void CCEFileHandler::SetBreakpointManager(const CComPtr<IBreakPoint>& pBreakpointManager)
{
    HRESULT hr;

    if(pBreakpointManager == m_pIBreakPoint)
    {
        return;
    }

    ASSERT(pBreakpointManager == NULL || m_pIBreakPoint == NULL);

	if(m_dwBPNotifyCookie != 0)
	{
		AtlUnadvise(m_pIBreakPoint, IID__IBreakPointNotify, m_dwBPNotifyCookie);
        m_dwBPNotifyCookie = 0;
	}

    m_pIBreakPoint = pBreakpointManager;

    if(pBreakpointManager == NULL)
    {
        return;
    }

    hr = AtlAdvise(pBreakpointManager, GetControllingUnknown(), IID__IBreakPointNotify, &m_dwBPNotifyCookie);
    if(FAILED(hr))
    {
        FC_DEBUGPRINT1(_T("Failed to advise breakpoint manager(hr=0x%08lx)\n"),(long)hr);
        return;
    }
}


/////////////////////////////////////////////////////////////////////////////
// Interface _IBreakPointNotify

STDMETHODIMP_(ULONG) CCEFileHandler::X_IBreakPointNotifyObj::AddRef()
{
    METHOD_PROLOGUE_EX(CCEFileHandler, _IBreakPointNotifyObj)

    return(pThis->ExternalAddRef());
}


STDMETHODIMP_(ULONG) CCEFileHandler::X_IBreakPointNotifyObj::Release()
{
    METHOD_PROLOGUE_EX(CCEFileHandler, _IBreakPointNotifyObj)

    return(pThis->ExternalRelease());
}


STDMETHODIMP CCEFileHandler::X_IBreakPointNotifyObj::QueryInterface(REFIID iid, LPVOID *ppvObj)
{
    METHOD_PROLOGUE_EX(CCEFileHandler, _IBreakPointNotifyObj)

    return((HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj));
}


STDMETHODIMP CCEFileHandler::X_IBreakPointNotifyObj::OnBPStateChanged(BPM_BRKPT_STATE newState, BSTR sInstance, IUnknown *pUnk, BOOL bUpdate)
{
    METHOD_PROLOGUE_EX(CCEFileHandler, _IBreakPointNotifyObj)

    CComPtr<IBreakPointInfo> pBP;

    if(pUnk != NULL)
    {
        HRESULT hr;

        hr = pUnk->QueryInterface(IID_IBreakPointInfo,(LPVOID *)&pBP);

        if(FAILED(hr))
        {
            return(hr);
        }
    }

    return(pThis->OnBPStateChanged(newState, sInstance, pBP, bUpdate == TRUE));
}


STDMETHODIMP CCEFileHandler::X_IBreakPointNotifyObj::OnSPStateChanged(BPM_STPPT_STATE newState, BSTR sInstance, IUnknown *pUnk, BOOL bUpdate)
{
    METHOD_PROLOGUE_EX(CCEFileHandler, _IBreakPointNotifyObj)

    CComPtr<IBreakPointInfo> pBP;

    FC_DEBUGPRINT2("Stoppoint notification received(state=%d, instance=%s)\n",(int)newState,(LPCTSTR)CString(sInstance));

    if(pUnk != NULL)
    {
        HRESULT hr;

        hr = pUnk->QueryInterface(IID_IBreakPointInfo,(LPVOID *)&pBP);

        if(FAILED(hr))
        {
            return(hr);
        }
    }

    return(pThis->OnSPStateChanged(newState, sInstance, pBP, bUpdate == TRUE));
}


STDMETHODIMP CCEFileHandler::X_IBreakPointNotifyObj::OnShowBP(BSTR sInstance, IUnknown *pUnk)
{
    METHOD_PROLOGUE_EX(CCEFileHandler, _IBreakPointNotifyObj)

    CComPtr<IBreakPointInfo> pBP;

    if(pUnk != NULL)
    {
        HRESULT hr;

        hr = pUnk->QueryInterface(IID_IBreakPointInfo,(LPVOID *)&pBP);

        if(FAILED(hr))
        {
            return(hr);
        }
    }

    return(pThis->OnShowBP(sInstance, pBP));
}



BOOL CCEFileHandler::Is4CEEditFile(const CString &strPath)
{
    return(m_pContainer->IsEditFile(strPath));
}


//------------------------------------------------------------------*
/**
 * get project path.
 *
 * @param           -
 * @return          project path
 * @exception       -
 * @see             -
*/
CString CCEFileHandler::GetProjectPath()
{
    return m_strProjPath;
}

//------------------------------------------------------------------*
/**
 * get file name.
 *
 * @param           [in] crstrFileString: file name, rel path or abs path
 * @return          file name
 * @exception       -
 * @see             -
*/
CString CCEFileHandler::GetFileName(const CString& crstrFileString)
{
    CString strFileName;

    CString strTmp(crstrFileString);
    strTmp.Replace(_T('/'), _T('\\'));

    int iFound = strTmp.ReverseFind(_T('\\'));
    strFileName = strTmp.Mid(iFound + 1);
    return strFileName;
}


//------------------------------------------------------------------*
/**
 * get file path name.
 *
 *  get file name incl. full path
 *  if input is file name or rel path, project scope is assumed
 *  uses GetFilePathName(2)
 *
 * @param           [in] crstrFileString: file name, rel path or abs path
 * @return          file path name
 * @exception       -
 * @see             GetFilePathName(2)
*/
CString CCEFileHandler::GetFilePathName(const CString& crstrFileString)
{
    CString strFilePathName(crstrFileString);
    if(strFilePathName.Mid(0, 9).CompareNoCase(_T("$(LibDir)")) == 0)
    {
        strFilePathName.Format("%s\\%s", m_strLibPath, strFilePathName.Mid(10));
        return strFilePathName;
    }
    // analyse file string
    E_CE_FileStringType eType = AnalyseFileString(crstrFileString);
    switch(eType)
    {
    // if only file name or relative path -> in project
    case eCEFileName:
    case eCERelPath:
        strFilePathName = GetFilePathName(crstrFileString, m_strProjPath);
        break;
    // absolute path -> return input
    case eCEAbsPath:
        strFilePathName = crstrFileString;
        break;
    }
    return strFilePathName;
}


//------------------------------------------------------------------*
/**
 * get file path name.
 *
 *  make absolute path from relative path or file name: 
 *  concatenate path and relative file path to an absolute path.
 *
 * @param           [in] crstrFileString: file name, rel path or abs path
 * @param           [in] crstrPath: file path with or without trailing backslash
 * @return          file path name (file name incl full path)
 * @exception       -
 * @see             -
*/
CString CCEFileHandler::GetFilePathName(const CString& crstrFileString, const CString& crstrPath)
{
    BOOL bAppendBackslash = TRUE;
    CString strFilePathName;
    CString strFileName;
    CString strPath(crstrPath);

    int iLen = strPath.GetLength();
    if(iLen == 0)
    {
        return crstrFileString;
    }

    // if last character of path is backslash -> do not append backslash
    if(strPath[iLen - 1] == _T('\\'))
    {
        bAppendBackslash = FALSE;
    }

    strFileName = crstrFileString;

    if(bAppendBackslash)
    {
        strFilePathName = crstrPath + _T('\\') + strFileName;
    }
    else
    {
        strFilePathName = crstrPath + strFileName;
    }
    return strFilePathName;
}

//------------------------------------------------------------------*
/**
 * get file path optimized.
 *
 * @param           [in] crstrPath: file name incl full path
 * @return          if crstrPath is within project path -> relative path
 *                  else full path.
 * @exception       -
 * @see             -
*/
CString CCEFileHandler::GetFilePathOpt(const CString& crstrPath)
{
    CString     strProjectPath;
    CString     strRelPath;

    strRelPath = GetFilePathRelative(crstrPath, m_strProjPath);

    // if file is not within project path
    if(strRelPath.IsEmpty())
    {
        // return path
        return crstrPath;
    }
    // return relative path
    return strRelPath;
}

//------------------------------------------------------------------*
/**
 * get file path relative to some other path.
 *
 *  example: file path name:    C:\4Control\Engineering\Projects\test\blabla\xyz.st
 *           other path:        C:\4Control\Engineering\Projects\test
 *           rel path returned: blabla\xyz.st
 *
 * @param           [in] crstrFilePathName: file name incl full path
 * @param           [in] crstrPath: path
 * @return          relative path of file or empty string if file is not within this path
 * @exception       -
 * @see             -
*/
CString CCEFileHandler::GetFilePathRelative(const CString& crstrFilePathName, const CString& crstrPath)
{
    CString strRelPath;
    CString strPath(crstrPath);
    int iPathLen = strPath.GetLength();

    strPath.Replace(_T('/'), _T('\\'));
    if(iPathLen > 0)
    {
        // if last character of path isn't backslash -> append backslash
        if(strPath[iPathLen - 1] != _T('\\'))
        {
            strPath += _T('\\');
            ++iPathLen;
        }
    }
    else
    {
        // crstrPath is empty -> return full path name
        return crstrFilePathName;
    }

    if(crstrFilePathName.Left(iPathLen).CompareNoCase(strPath) != 0)
    {
        return strRelPath;
    }

    strRelPath = crstrFilePathName.Mid(iPathLen);
    return strRelPath;
}


//------------------------------------------------------------------*
/**
 * is within project path?.
 *
 * @param           [in] crstrFilePathName: file name incl full path or just path
 * @return          is path within project path?
 * @exception       -
 * @see             -
*/
BOOL CCEFileHandler::IsWithinProjectPath(const CString& crstrFilePathName)
{
    int  iProjPathLen = m_strProjPath.GetLength();
    if(iProjPathLen == 0)
    {
        ASSERT(FALSE);
        return FALSE;   // project not yet loaded
    }

    if(crstrFilePathName.GetLength() < iProjPathLen)
    {
        return FALSE;
    }

    if(m_strProjPath.CompareNoCase(crstrFilePathName.Left(iProjPathLen)) == 0)
    {
        return TRUE;
    }
    return FALSE;
}


//------------------------------------------------------------------*
/**
 * analyse file string.
 *
 * @param           [in] crstrFileString: file name, rel path or absolute path
 * @return          
 * @exception       -
 * @see             
*/
E_CE_FileStringType CCEFileHandler::AnalyseFileString(const CString& crstrFileString)
{
    CString strTmp(crstrFileString);
    strTmp.Replace(_T('/'), _T('\\'));

    int iFoundBackslash = strTmp.Find(_T('\\'));
    int iFoundColon = strTmp.Find(_T(':'));

    if(iFoundBackslash == -1 && iFoundColon == -1)
    {
        return eCEFileName;
    }
    // now use fc function:
    if(FC_StringIsAbsPath(crstrFileString)==0)
    {
        return eCERelPath;
    }
    return eCEAbsPath;
}


//------------------------------------------------------------------*
/**
 * get file status.
 *
 * @param           crstrFileString: file string
 * @return          file status value
 * @exception       -
 * @see             -
*/
EFileStatus CCEFileHandler::GetFileStatus(const CString& crstrFileString)
{
    BOOL    bLibrary = FALSE;
    CString strFilePathName = GetFilePathName(crstrFileString);
    if(IsLibraryHeader(strFilePathName))
    {
        return GetLibraryHeaderStatus(strFilePathName);
    }

    CFileStatus tFileStatus;
    if(CFile::GetStatus(strFilePathName, tFileStatus))
    {
        if(tFileStatus.m_attribute & CFile::readOnly)
        {
            return eFSReadOnly;
        }
        return eFSNormal;
    }
    return eFSMissing;
}


//------------------------------------------------------------------*
/**
 * get library header file status.
 *
 *  normal: library is existing and not read only and contains specified header
 *  readonly: library is read only and contains specified header
 *  missing: library file is missing or does not contain specified header
 *
 * @param           crstrFileString: header file string.
 * @return          file status value.
 * @exception       -
 * @see             -
*/
EFileStatus CCEFileHandler::GetLibraryHeaderStatus(const CString& crstrFileString)
{
    CString strFilePathName;
    int     iFound;
    BOOL    bHeaderExists = FALSE;

    strFilePathName = GetFilePathName(crstrFileString);
    bHeaderExists = IsLibraryHeaderExisting(strFilePathName);

    iFound = strFilePathName.Find(_T("::"));
    strFilePathName = strFilePathName.Left(iFound);
    EFileStatus eLibStatus = GetFileStatus(strFilePathName);

    if(bHeaderExists)
    {
        return eLibStatus;
    }
    return eFSMissing;
}

