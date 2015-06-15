

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


// GenEdCont.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "cemain.h"
#include "GenEdCont.h"
#include "GenEdDocTempl.h"
#include "GenEdFrame.h"
#include "GenEdView.h"
#include "GenEdDoc.h"
#include "GenEdCntrItem.h"
#include "GenEdRC.h"
#include "Settings.h"
#include "MainFrm.h"
#include "CECompMan.h"
#include "CEStateMan.h"
#include "utilif.h"
#include "KADHelper/KADFileExtInfo.h"
#include <assert.h>
#include "PouVarSelWindow.h"
#include "PouVarSelCtrlWrapper.h"
#include "cefilehandler.h"
#include "DbgInfo_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenEdCont

IMPLEMENT_DYNCREATE(CGenEdCont, CCmdTarget)

BEGIN_MESSAGE_MAP(CGenEdCont, CCmdTarget)
	//{{AFX_MSG_MAP(CGenEdCont)
		// HINWEIS - Der Klassen-Assistent fügt hier Zuordnungsmakros ein und entfernt diese.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CGenEdCont, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CGenEdCont)
	DISP_FUNCTION(CGenEdCont, "ShowURL", ShowURL, VT_ERROR, VTS_BSTR)
	DISP_FUNCTION(CGenEdCont, "ShowFile", ShowFile, VT_ERROR, VTS_BSTR)
	DISP_FUNCTION(CGenEdCont, "SelectFBForCall", SelectFBForCall, VT_ERROR, VTS_NONE)
    DISP_FUNCTION(CGenEdCont, "ShowVarSelection", ShowVarSelection, VT_ERROR, VTS_BSTR)
    DISP_FUNCTION(CGenEdCont, "SetVarSelectionHint", SetVarSelectionHint, VT_ERROR, VTS_BSTR VTS_BSTR VTS_BSTR)
    DISP_FUNCTION(CGenEdCont, "ShowHelp", ShowHelp, VT_ERROR, VTS_BSTR)
    DISP_FUNCTION(CGenEdCont, "ShowHeader", ShowHeader, VT_ERROR, VTS_BSTR)
    DISP_FUNCTION(CGenEdCont, "ShowHeaderFile", ShowHeaderFile, VT_ERROR, VTS_BSTR)
    
    DISP_FUNCTION(CGenEdCont, "GetLRUPous", GetLRUPous, VT_ERROR, VTS_PVARIANT)
    DISP_FUNCTION(CGenEdCont, "GetPouXML", GetPouXML, VT_ERROR, VTS_BSTR VTS_PBSTR)
    DISP_FUNCTION(CGenEdCont, "GetVarList", GetVarList, VT_ERROR, VTS_PVARIANT)
    DISP_FUNCTION(CGenEdCont, "GetPouList", GetPouList, VT_ERROR, VTS_PVARIANT)
    DISP_FUNCTION(CGenEdCont, "GetPouDefinition", GetPouDefinition, VT_ERROR, VTS_BSTR VTS_PBSTR)
    DISP_FUNCTION(CGenEdCont, "GetPouMember", GetPouMember, VT_ERROR, VTS_BSTR VTS_PVARIANT)
    DISP_FUNCTION(CGenEdCont, "XRefChangeRefPosition", XRefChangeRefPosition, VT_ERROR, VTS_BOOL)
    DISP_FUNCTION(CGenEdCont, "XRefSetActiveQuery", XRefSetActiveQuery, VT_ERROR, VTS_BSTR)
	DISP_FUNCTION(CGenEdCont, "ShowVarSelectionBox", ShowVarSelectionBox, VT_ERROR, VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_PBSTR VTS_PI4 VTS_PI4)
  	DISP_FUNCTION(CGenEdCont, "GetProjectManager", GetProjectManager, VT_ERROR, VTS_PUNKNOWN)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// {1B8AC858-5783-11D2-B9C9-006008749B3D}
/*static const IID IID_ICEGenEdContainer =
{ 0x1b8ac858, 0x5783, 0x11d2, { 0xb9, 0xc9, 0x0, 0x60, 0x8, 0x74, 0x9b, 0x3d } };*/
// 43E307E3-D43E-47cd-9390-D88A7A392B43   --> nmew for V2.0
static const IID IID_ICEGenEdContainer =
{ 0x43E307E3, 0xD43E, 0x47cd, { 0x93, 0x90, 0xD8, 0x8A, 0x7A, 0x39, 0x2B, 0x43 } };

BEGIN_INTERFACE_MAP(CGenEdCont, CCmdTarget)
	INTERFACE_PART(CGenEdCont, IID_ICEGenEdContainer, Dispatch)
END_INTERFACE_MAP()

CGenEdCont *CGenEdCont::m_pMe;


CGenEdCont::CGenEdCont ()
: m_FrameActionCB (this, OnFrameAction),
  m_ConnectActionCB (this, OnConnectAction),
  m_StateChangeCB (this, OnStateChange),
  m_pActiveFrame (NULL),
//  m_pGCSettings (NULL),
  m_pIDebugInfo(NULL),
  m_lPrintWidth (0),
  m_lPrintHeight (0),
  m_bReloadedAny (false),
  m_PrintDocPos(NULL),
  // SIS 22.07.04 >>
  m_iOpenDocumentCount(0),	
  m_iMaxOpenDocumentCount(MAXIMUM_DOCUMENT_COUNT),
  // SIS 22.07.04 <<
  // SIS 08.11.04 >>
  m_iMaxRecoverDocumentCount(MAXIMUM_RECOVER_COUNT)
  // SIS 08.11.04 <<
{
    ASSERT (m_pMe == NULL);
    if (m_pMe == NULL)
    {
        m_pMe = this;
    }

    EnableAutomation();

    m_pSimpleTemplate = new CGenEdDocTemplate (false);
    m_pSimpleTemplate->SetContainerInfo(IDR_GENEDTYPE_CNTR_IP);

    m_pTabbedTemplate = new CGenEdDocTemplate (true);
    m_pTabbedTemplate->SetContainerInfo(IDR_GENEDTYPE_CNTR_IP);

    CESMRegisterCallback (&m_StateChangeCB);
    m_bKadLoaded = false;

}


CGenEdCont::~CGenEdCont ()
{
    m_pIBreakPoint = NULL;
    m_pIDebugInfo = NULL;

    CESMUnregisterCallback (&m_StateChangeCB);

	// SIS 22.07.04 >>
	// maximum count of open documents
	ASSERT(m_iOpenDocumentCount == 0);
	// SIS 22.07.04 <<
}


// SIS 22.07.04 >>
// maximum count of open documents
void CGenEdCont::IncreaseOpenDocumentCount()
{
	++m_iOpenDocumentCount;
}

void CGenEdCont::DecreaseOpenDocumentCount()
{
	--m_iOpenDocumentCount;
}

int CGenEdCont::GetOpenDocumentCount()
{
	return m_iOpenDocumentCount;
}
// SIS 22.07.04 <<



HRESULT CGenEdCont::Init ()
{
    // load file extension information from KAD
    LoadFileInfoFromKAD();

	// SIS 22.07.04 >>
	CCEMAINApp* pApp = (CCEMAINApp*)AfxGetApp();
	ASSERT(pApp);
	CKADInfo* pKADInfo = pApp->GetKADInfoObject();
	ASSERT(pKADInfo);
	if(pKADInfo)
	{
		m_iMaxOpenDocumentCount = pKADInfo->GetMaximumNumberOfOpenDocuments();
		// SIS 08.11.04 >>
		m_iMaxRecoverDocumentCount = pKADInfo->GetMaximumNumberOfRecoverDocuments();
		// ensure that number of documents to be recovered does not exceed
		// number of open documents
		if(m_iMaxRecoverDocumentCount > m_iMaxOpenDocumentCount)
		{
			m_iMaxRecoverDocumentCount = m_iMaxOpenDocumentCount;
		}
		// SIS 08.11.04 <<
	}
	// SIS 22.07.04 <<

    return (S_OK);
}


HRESULT CGenEdCont::GetDebugInfo(IDebugInfo** ppDebugInfo)
{
    if(!m_pIDebugInfo)
    {
        // create debug info server
        HRESULT hr;

        hr = CoCreateInstance (CLSID_DebugInfoServer, NULL, CLSCTX_ALL, IID_IDebugInfo, (LPVOID *)&m_pIDebugInfo);
        if(hr != S_OK)
        {
            return (hr);
        }

        // get project gen path from proj info
        CComPtr<IUnknown>       pTmp;
        CComQIPtr<ICEProjInfo>  pProjInfo;
        pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
        pProjInfo = pTmp;
        if(!pProjInfo)
        {
            return E_FAIL;
        }

        // initialize debug info server
        CComBSTR    sGenPath;
        pProjInfo->getProjectGenPath(&sGenPath);
        hr = m_pIDebugInfo->Init(sGenPath);
        if(hr != S_OK)
        {
            return hr;
        }
    }

    // set pointer and increment reference count
    *ppDebugInfo = m_pIDebugInfo;
    (*ppDebugInfo)->AddRef();
    return S_OK;
}


void CGenEdCont::Cleanup ()
{
    m_pIBreakPoint = NULL;
}


CGenEdDoc* CGenEdCont::LoadPrintServer(const CString& crstrFileExt)
{
    // look up in print documents
    // and return if already loaded
    CGenEdDoc*  pDoc = NULL;
    CString     strFileExt;

    // get CLSID for file extension
    CString     strCLSID;
    CKADFileExtInfo* pKadFileExtInfo = m_tKadFileInfo.GetFileExtInfo(crstrFileExt);

    if(pKadFileExtInfo)
    {
        if(pKadFileExtInfo->m_bPrint && pKadFileExtInfo->m_pEditorGUID)
        {
            CGuid tGuid(*pKadFileExtInfo->m_pEditorGUID);
            strCLSID = tGuid;
        }
    }

    if(strCLSID.IsEmpty())
    {
        return pDoc;
    }

    if(m_tPrintDocsMap.Lookup(strCLSID, pDoc))
    {
        ASSERT(pDoc);
        if(pDoc)
        {
            return pDoc;
        }
    }

    // create new document
    CDocTemplate *pDocTemplate;
    pDoc = CreateTypedDocument(crstrFileExt, true, false);
    if(!pDoc)
    {
        return pDoc;
    }

    ASSERT(!m_strPrinterName.IsEmpty());
    pDoc->SetPrintInfo(m_strPrinterName, m_lPrintWidth, m_lPrintHeight);

    // don't want the document to be regarded by CloseAll or
    // SaveAll operations (CloseAll is called at project close).
    pDocTemplate = pDoc->GetDocTemplate();
    assert (pDocTemplate != NULL);
    if (pDocTemplate != NULL)
    {
        pDocTemplate->RemoveDocument(pDoc);
    }
    m_tPrintDocsMap.SetAt(strCLSID, pDoc);
    return pDoc;
}


//------------------------------------------------------------------*
/**
 * get class id string for file extension.
 *
 * @param           [in] crstrFileExt: file extension (without '.')
 * @return          class id string.
 * @exception       -
 * @see             
*/
CString CGenEdCont::GetClassIdStringForFileExtension(const CString& crstrFileExt)
{
    CString strGUID;

    CKADFileExtInfo* pKadFileExtInfo = m_tKadFileInfo.GetFileExtInfo(crstrFileExt);

    if(pKadFileExtInfo)
    {
        if(pKadFileExtInfo->m_pEditorGUID)
        {
            CGuid tGuid(*pKadFileExtInfo->m_pEditorGUID);
            strGUID = tGuid;
        }
    }
    return strGUID;
}

//------------------------------------------------------------------*
/**
 * load print servers.
 *  
 *  load all print servers.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CGenEdCont::LoadPrintServers()
{
    CStringArray        astrExt;
    CString             strExt;
    CKADFileExtInfo*    pKadFileExtInfo;

    m_tKadFileInfo.GetAllExtensions(astrExt);

    int iNumExt = astrExt.GetSize();

    for(int iExt = 0; iExt < iNumExt; ++iExt)
    {
        strExt = astrExt[iExt];
        pKadFileExtInfo = m_tKadFileInfo.GetFileExtInfo(strExt);
        ASSERT(pKadFileExtInfo);
        if(pKadFileExtInfo && pKadFileExtInfo->m_bPrint)
        {
            LoadPrintServer(strExt);
        }
    }
}

//------------------------------------------------------------------*
/**
 * unload print servers servers.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CGenEdCont::UnloadPrintServers()
{
    POSITION    pos;
    CString     strCLSID;
    CGenEdDoc*  pDoc = NULL;

    pos = m_tPrintDocsMap.GetStartPosition();
    while(pos)
    {
        m_tPrintDocsMap.GetNextAssoc(pos, strCLSID, pDoc);
        ASSERT(pDoc);
        if(pDoc)
        {
            pDoc->OnCloseDocument();
        }
    }
    m_tPrintDocsMap.RemoveAll();
}


CGenEdDocTemplate *CGenEdCont::GetDocTemplate (bool bTabbed)
{
    if (bTabbed)
    {
        return (m_pTabbedTemplate);
    }

    return (m_pSimpleTemplate);
}


CGenEdDoc *CGenEdCont::GetFirstDocument (CE_SRCDOCPOS_TYP &position, BOOL bTabbed /*=TRUE*/)
{
    CDocument *pDocument;
    CGenEdDoc *pGenEdDoc;

    if(bTabbed)
    {
        position.pTemplate = m_pTabbedTemplate;
        position.rPos = position.pTemplate->GetFirstDocPosition ();
    }
    else
    {
        position.pTemplate = m_pSimpleTemplate;
        position.rPos = position.pTemplate->GetFirstDocPosition ();
    }

    if (position.rPos == NULL)
    {
        return (NULL);
    }

    pDocument = position.pTemplate->GetNextDoc (position.rPos);
    assert (pDocument != NULL);
    if (pDocument == NULL)
    {
        return (NULL);
    }

    pGenEdDoc = dynamic_cast<CGenEdDoc *> (pDocument);
    assert (pGenEdDoc != NULL);
    if (pGenEdDoc == NULL)
    {
        return NULL;
    }

    return (pGenEdDoc);
}


CGenEdDoc *CGenEdCont::GetNextDocument (CE_SRCDOCPOS_TYP &position, BOOL bTabbed /*=TRUE*/)
{
    CDocument *pDocument;
    CGenEdDoc *pGenEdDoc;

    if (position.rPos == NULL)
    {
        return (NULL);
    }

    if(bTabbed)
    {
        position.pTemplate = m_pTabbedTemplate;
    }
    else
    {
        position.pTemplate = m_pSimpleTemplate;
    }

    pDocument = position.pTemplate->GetNextDoc (position.rPos);
    assert (pDocument != NULL);
    if (pDocument == NULL)
    {
        return (NULL);
    }

    pGenEdDoc = dynamic_cast<CGenEdDoc *> (pDocument);
    assert (pGenEdDoc != NULL);
    if (pGenEdDoc == NULL)
    {
        return NULL;
    }

    return (pGenEdDoc);
}



CGenEdDoc *CGenEdCont::GetActiveDocument ()
{
    CGenEdFrame *pFrame;
    CGenEdDoc *pDoc;


    // Get currently active container frame. If there is no
    // active frame or the active frame is not a container
    // frame there is no active container document.

    pFrame = GetActiveFrame ();
    if (pFrame == NULL)
    {
        return (NULL);
    }


    // Get active document from active container frame and
    // check if it is a container document. A container frame
    // always has to have an active document that must be a
    // container document.

    pDoc = dynamic_cast<CGenEdDoc *> (pFrame->GetActiveDocument ());
    //assert (pDoc != NULL);
    if (pDoc == NULL)
    {
        return (NULL);
    }


    return (pDoc);
}


CGenEdDoc *CGenEdCont::FindDocument (const CString &strFilePath)
{
    CString strFilePathUpper;
    CGenEdDoc *pDoc;
    CE_SRCDOCPOS_TYP docPosition;

    strFilePathUpper = strFilePath;
    strFilePathUpper.MakeUpper ();

    for (pDoc = GetFirstDocument (docPosition); pDoc != NULL; pDoc = GetNextDocument (docPosition))
    {
        CString strDocFilePath;

        strDocFilePath = pDoc->GetPathName ();
        strDocFilePath.MakeUpper ();

        if (strFilePathUpper == strDocFilePath)
        {
            return (pDoc);
        }
    }

    for (pDoc = GetFirstDocument (docPosition, FALSE); pDoc != NULL; pDoc = GetNextDocument (docPosition, FALSE))
    {
        CString strDocFilePath;

        strDocFilePath = pDoc->GetPathName ();
        strDocFilePath.MakeUpper ();

        if (strFilePathUpper == strDocFilePath)
        {
            break;
        }
    }

    return (pDoc);
}


CGenEdDoc *CGenEdCont::CreateUntypedDocument (bool bVisible)
{
    GUID    tGUID;
    CString strExtension = ".st";
    bool    bNeverTabbed = true;

    if (!GetCreateParameters(strExtension, bNeverTabbed, m_pSimpleTemplate, &tGUID))
    {
        return (NULL);
    }
    return (CreateDocument (m_pSimpleTemplate, "", bVisible, tGUID));
}


CGenEdDoc* CGenEdCont::CreateTypedDocument(const CString &strExtension, bool bNeverTabbed, bool bVisible)
{
    CGenEdDocTemplate *pDocTemplate;
    GUID    tGUID;

    if (!GetCreateParameters(strExtension, bNeverTabbed, pDocTemplate, &tGUID))
    {
        return (NULL);
    }

    CGenEdDoc* pDoc = CreateDocument(pDocTemplate, "", bVisible, tGUID);
    return (pDoc);
}




CGenEdDoc* CGenEdCont::OpenDocument (const CString &strFileName, bool bNeverTabbed, 
                                     bool bVisible /*=true*/, 
                                     E_ActivateAction eAction /*=eActivateClass*/,
                                     bool bForceAsText /*=false*/)
{
    CGenEdDocTemplate* pDocTemplate;
    GUID    tGUID;

    if (!GetCreateParameters (strFileName, bNeverTabbed, pDocTemplate, &tGUID, bForceAsText))
    {
        return (NULL);
    }

    return (CreateDocument(pDocTemplate, strFileName, bVisible, tGUID, eAction));
}



bool CGenEdCont::SaveDocument (CGenEdDoc* pDoc)
{
    if (!pDoc->GetModified ())
    {
        return (true);
    }

    return (pDoc->OnSaveDocument (pDoc->GetPathName ()) == TRUE);
}


bool CGenEdCont::LoadDocument (CGenEdDoc* pDoc)
{
    HRESULT hr;

    hr = pDoc->ReloadServer();
    return (SUCCEEDED (hr));
}


void CGenEdCont::ActivateDocument (CGenEdDoc* pDoc, E_ActivateAction eAction /*=eActivateClass*/)
{
    CGenEdFrame* pFrame;

    pFrame = FindFrame (pDoc);
    assert (pFrame != NULL);
    if (pFrame == NULL)
        return;

    // if recovering, activate instance tab
    pFrame->ActivateFrame();
    pFrame->UpdateTabs(eAction);
	pFrame->SetFocusToServer ();
}


void CGenEdCont::CloseDocument (CGenEdDoc* pDoc)
{
    pDoc->OnCloseDocument ();
}


void CGenEdCont::SetFocusToServer(CGenEdCntrItem* pServer)
{
    CGenEdDoc* pDoc;
    CGenEdFrame* pFrame;

    pDoc = pServer->GetDocument ();
    assert (pDoc != NULL);
    if (pDoc == NULL)
        return;

    pFrame = FindFrame (pDoc);
    assert (pFrame != NULL);
    if (pFrame == NULL)
        return;

    pFrame->SetFocusToServer ();
}


HRESULT CGenEdCont::ShowInstance (CGenEdDoc* pDoc, const CString &strInstance)
{
    CGenEdFrame* pFrame;
    HRESULT hr;

    pFrame = FindFrame (pDoc);
    assert (pFrame != NULL);
    if (pFrame == NULL)
        return (E_FAIL);

    hr = pFrame->ShowInstance (strInstance, false);

    return (hr);
}


HRESULT CGenEdCont::ShowLocation (CGenEdDoc* pDoc, const CString &strLocation)
{
    HRESULT hr;
    CGenEdCntrItem* pServer;

    pServer = pDoc->GetEditorServer();
    SetFocusToServer (pServer);

    hr = pDoc->GotoLocation (strLocation);
    return (hr);
}


void CGenEdCont::UpdateTabs ()
{
    CGenEdDoc* pDoc;
    CE_SRCDOCPOS_TYP docPosition;

    for (pDoc = GetFirstDocument (docPosition); pDoc != NULL; pDoc = GetNextDocument (docPosition))
    {
        UpdateDocumentTabs (pDoc);
    }
}


void CGenEdCont::UpdateDocumentTabs (CGenEdDoc* pDoc)
{
    CGenEdFrame* pFrame;

    pFrame = FindFrame (pDoc);
    assert (pFrame != NULL);
    if (pFrame == NULL)
        return;

    pFrame->UpdateTabs(eActivateInstance);
}


void CGenEdCont::CloseAll ()
{
    if (m_pSimpleTemplate != NULL)
    {
        m_pSimpleTemplate->CloseAllDocuments (FALSE);
    }

    if (m_pTabbedTemplate != NULL)
    {
        m_pTabbedTemplate->CloseAllDocuments (FALSE);
    }
}


bool CGenEdCont::SaveAll (bool bAskBeforeSave)
{
    if (m_pSimpleTemplate != NULL)
    {
        if (!m_pSimpleTemplate->SaveAll (bAskBeforeSave))
        {
            Parse();
            return (false);
        }
    }

    if (m_pTabbedTemplate != NULL)
    {
        if (!m_pTabbedTemplate->SaveAll (bAskBeforeSave))
        {
            Parse();
            return (false);
        }
    }

    Parse();
    return (true);
}



void CGenEdCont::ClearModified ()
{
    CGenEdDoc* pDoc;
    CE_SRCDOCPOS_TYP docPosition;

    for (pDoc = GetFirstDocument (docPosition); pDoc != NULL; pDoc = GetNextDocument (docPosition))
    {
        pDoc->ClearModified ();
    }
}


bool CGenEdCont::HaveModifiedDocuments ()
{
    CGenEdDoc* pDoc;
    CE_SRCDOCPOS_TYP docPosition;

    // first run over tabbed documents
    for (pDoc = GetFirstDocument (docPosition); pDoc != NULL; pDoc = GetNextDocument (docPosition))
    {
        if (pDoc->GetModified ())
        {
            return (true);
        }
    }

    // then over simple documents
    for (pDoc = GetFirstDocument (docPosition, FALSE); pDoc != NULL; pDoc = GetNextDocument (docPosition, FALSE))
    {
        if (pDoc->GetModified ())
        {
            return (true);
        }
    }

    return (false);
}


bool CGenEdCont::HaveOpenDocuments()
{
    CGenEdDoc* pDoc;
    CE_SRCDOCPOS_TYP docPosition;

    // first run over tabbed documents
    pDoc = GetFirstDocument(docPosition);
    if (pDoc)
    {
        return (true);
    }

    // then over simple documents
    pDoc = GetFirstDocument (docPosition, FALSE);
    if(pDoc)
    {
        return (true);
    }
    return (false);
}


//------------------------------------------------------------------*
/**
 * is program or standard editor available for file?
 *
 * @param           [in] rstrFileName: file name or extension
 * @return          is editor GUID available for file?
 *
*/
BOOL CGenEdCont::IsEditFile(const CString& rstrFileName)
{
    TCHAR   caExt[_MAX_EXT];

    UTIL_GetFileExt(rstrFileName, caExt, _MAX_EXT);
    CString strExt = caExt;
    // if no extension -> assume extension is given directly
    if(strExt == "")
    {
        strExt = rstrFileName;
    }

    CKADFileExtInfo* pKadFileExtInfo = m_tKadFileInfo.GetFileExtInfo(strExt);

    if(pKadFileExtInfo)
    {
        if(pKadFileExtInfo->m_pEditorGUID)
        {
            return TRUE;
        }
    }
    return FALSE;
}


void CGenEdCont::OnStateChange (CEStateNotify stateNotify, unsigned int uiStage)
{
    CCEStateManager* pStateManager;

    switch(stateNotify)
    {
    case CEPreCloseProjectNotify:
        m_pIDebugInfo = NULL;
        break;
    case CEAddTargetNotify:
        OnAddTarget(CESMGetTarget());
        break;
    case CERemoveTargetNotify:
        OnRemoveTarget (CESMGetTarget());
        break;
    case CEComponentAccessibleNotify:
        if (CESMGetComponentName () == CE_COMPMAN_BREAKPTMAN)
        {
            HRESULT hr;
            IUnknown* pUnknown = NULL;
            CComPtr<IBreakPoint> pBreakpointManager;

            ASSERT (m_pIBreakPoint == NULL);
            if (m_pIBreakPoint != NULL)
            {
                SetBreakpointManager (NULL);
            }

            pUnknown = CEGetCOM (CE_COMPMAN_BREAKPTMAN, true);
            if (pUnknown != NULL)
            {
                hr = pUnknown->QueryInterface (IID_IBreakPoint, (LPVOID* )&pBreakpointManager);
                ASSERT (SUCCEEDED (hr));
                if (SUCCEEDED (hr))
                {
                    ASSERT (pBreakpointManager != NULL);
                    SetBreakpointManager (pBreakpointManager);
                }
                pUnknown->Release();
            }
        }
        break;
    case CEHandsOffComponentNotify:
        if (CESMGetComponentName () == CE_COMPMAN_BREAKPTMAN)
        {
           SetBreakpointManager (NULL);
        }
        break;
    case CEDownloadCompleteNotify:
        pStateManager = CEGetStateManager ();
        if (pStateManager != NULL)
        {
            m_bResubscribeAnotherTime = true;
            m_uiResubscribeCookie = pStateManager->TriggerDelay (NULL);
        }
        break;
    case CEDelayNotify:
        pStateManager = CEGetStateManager ();
        if (pStateManager != NULL)
        {
            if (pStateManager->GetDelayCookie () == m_uiResubscribeCookie)
            {
                m_uiResubscribeCookie = 0;

                CE_SRCDOCPOS_TYP docPosition;
                CGenEdDoc* pDoc = NULL;
                for (pDoc = GetFirstDocument (docPosition); pDoc != NULL; pDoc = GetNextDocument (docPosition))
                {      
                    // first store actually set watchpoints
                    pDoc->StoreWatchpoints ();
                    CGenEdCntrItem *pServer = NULL;
                    pServer = pDoc->GetEditorServer();
                    if (pServer)
                    { 
                        pServer->Resubscribe();
                    }
                    // after resubscribe restore all watchpoints, 
                    // so that they wíll be updated
                    // and reconnected with the new connection
                    // see d-1327
                    pDoc->RestoreWatchpoints ();
                }

                if (m_bResubscribeAnotherTime)
                {
                    m_bResubscribeAnotherTime = false;
                    m_uiResubscribeCookie = pStateManager->TriggerDelay (2000, NULL);
                }
            }
        }
        break;
    }
}


void CGenEdCont::OnAddTarget (CTargetOnlBase* pTarget)
{
    CGenEdDoc* pDoc;
    CE_SRCDOCPOS_TYP docPosition;

    ASSERT (pTarget != NULL);
    if (pTarget == NULL)
        return;

    for (pDoc = GetFirstDocument (docPosition); pDoc != NULL; pDoc = GetNextDocument (docPosition))
    {
        CGenEdFrame* pFrame;

        pFrame = FindFrame (pDoc);
        ASSERT (pFrame != NULL);
        if (pFrame == NULL)
            continue;

        pFrame->OnAddTarget (pTarget);
    }

    pTarget->RegisterConnectActionCB (&m_ConnectActionCB);
}


void CGenEdCont::OnRemoveTarget (CTargetOnlBase* pTarget)
{
    CGenEdDoc* pDoc;
    CE_SRCDOCPOS_TYP docPosition;

    ASSERT (pTarget != NULL);
    if (pTarget == NULL)
        return;

    for (pDoc = GetFirstDocument (docPosition); pDoc != NULL; pDoc = GetNextDocument (docPosition))
    {
        CGenEdFrame* pFrame;

        pFrame = FindFrame (pDoc);
        ASSERT (pFrame != NULL);
        if (pFrame == NULL)
            continue;

        pFrame->OnRemoveTarget(pTarget);
    }

    pTarget->UnregisterConnectActionCB (&m_ConnectActionCB);
}


//void CGenEdCont::SetSettings (CGCSettings* pGCSettings)
//{
//    m_pGCSettings = pGCSettings;
//
//#if 0
//    if (pGCSettings != NULL)
//    {
//        CloseAll ();
//        OpenSettingsDocuments ();
//    }
//#endif
//}

void CGenEdCont::UpdateSettings()
{
    CE_SRCDOCPOS_TYP position;
    CGenEdDoc* pDoc;

    for (pDoc = GetFirstDocument (position); pDoc != NULL; pDoc = GetNextDocument (position))
    {
        pDoc->UpdateSettings ();
    }
}


void CGenEdCont::SetPrintInfo (const CString &strPrinterName, long lWidth, long lHeight)
{
    m_strPrinterName = strPrinterName;
    m_lPrintWidth = lWidth;
    m_lPrintHeight = lHeight;

    CGenEdDoc*  pDoc;
    POSITION    pos;
    CString     strCLSID;

    pos = m_tPrintDocsMap.GetStartPosition();
    while(pos)
    {
        m_tPrintDocsMap.GetNextAssoc(pos, strCLSID, pDoc);
        ASSERT(pDoc);
        if(pDoc)
        {
            pDoc->SetPrintInfo(strPrinterName, lWidth, lHeight);
        }
    }

    // run over all open documents
    CE_SRCDOCPOS_TYP position;
    pDoc = GetFirstDocument(position);
    while (pDoc != NULL)
    {
        pDoc->SetPrintInfo (strPrinterName, lWidth, lHeight);
        pDoc = GetNextDocument (position);
    }
    pDoc = GetFirstDocument (position, FALSE);
    while (pDoc != NULL)
    {
        pDoc->SetPrintInfo (strPrinterName, lWidth, lHeight);
        pDoc = GetNextDocument (position, FALSE);
    }
}


bool CGenEdCont::OnIdle ()
{
    if(m_pActiveFrame)
    {
        m_pActiveFrame->OnUpdateFrameTitle(1);
    }

    return (ModifyCheck ());
}


bool CGenEdCont::ModifyCheck ()
{
    CGenEdDoc* pDoc;
    CFile file;
    CString strFileName;
    CString strMessage;

    if (m_ModifyCheckDocuments.IsEmpty ())
    {
        GetAllDocuments (m_ModifyCheckDocuments);

        if (m_ModifyCheckDocuments.IsEmpty ())
        {
            return (false);
        }
    }

    pDoc = m_ModifyCheckDocuments.RemoveHead ();
    ASSERT (pDoc != NULL);
    if (pDoc != NULL)
    {
        if (FindFrame (pDoc) != NULL)
        {
            if (pDoc->ModifyCheck ())
            {
                file.SetFilePath (pDoc->GetDocumentFile ());
                strFileName = file.GetFileName ();

                if (pDoc->GetModified ())
                {
                    strMessage.Format (IDS_RELOAD_DOCUMENT_LOSE, strFileName);
                }
                else
                {
                    strMessage.Format (IDS_RELOAD_DOCUMENT, strFileName);
                }

                if (AfxMessageBox (strMessage, MB_YESNO) == IDYES)
                {
                    if (LoadDocument (pDoc))
                    {
                        m_bReloadedAny = true;
                    }
                    else
                    {
                        strMessage.Format (IDS_RELOAD_FAILED, strFileName);
                        AfxMessageBox (strMessage);
                    }
                }
            }
        }
    }

    if (!m_ModifyCheckDocuments.IsEmpty ())
    {
        return (true);
    }

    if (m_bReloadedAny)
    {
        m_bReloadedAny = false;
        m_ReloadedCBs.Execute ();
        Parse();
    }

    return (false);
}


//------------------------------------------------------------------*
/**
 * parse.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CGenEdCont::Parse()
{
    CComQIPtr<ICEProjInfo>  pProject;
    CComPtr<IUnknown>       pTmp;

    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProject = pTmp;

    ASSERT(pProject != NULL);
    if(pProject != NULL)
    {
        pProject->fileParseNow(NULL);
    }
}


void CGenEdCont::GetAllDocuments (CList<CGenEdDoc* , CGenEdDoc* > &documents)
{
    CE_SRCDOCPOS_TYP position;
    CGenEdDoc* pDoc;

    // first get tabbed documents
    for (pDoc = GetFirstDocument (position); pDoc != NULL; pDoc = GetNextDocument (position))
    {
        documents.AddTail (pDoc);
    }
    // then get simple documents
    for (pDoc = GetFirstDocument (position, FALSE); pDoc != NULL; pDoc = GetNextDocument (position, FALSE))
    {
        documents.AddTail (pDoc);
    }
}


CGenEdFrame* CGenEdCont::GetActiveFrame ()
{
    CWnd* pMainWnd;
    CMDIFrameWnd* pMainFrame;
    CGenEdFrame* pFrame;


    // Get main frame. If there is no main frame
    // there is no active container frame

    pMainWnd = AfxGetMainWnd ();
    if (pMainWnd == NULL)
    {
        return (NULL);
    }


    // The main frame has to be a CMDIFrameWnd at least.
    // Anything different would be really strange.

    pMainFrame = dynamic_cast<CMDIFrameWnd* > (pMainWnd);
    assert (pMainFrame != NULL);
    if (pMainFrame == NULL)
    {
        return (NULL);
    }


    // Get active MDI frame from main frame and check
    // if it is a container frame. If there is either
    // no active frame or it is not a container frame
    // we have no active container frame.

    pFrame = dynamic_cast<CGenEdFrame* > (pMainFrame->MDIGetActive ());
    if (pFrame == NULL)
    {
        return (NULL);
    }


    return (pFrame);
}


bool CGenEdCont::GetCreateParameters(const CString &strFileName, 
                                     bool bNeverTabbed, 
                                     CGenEdDocTemplate* &pDocTemplate, 
                                     GUID* pGUID, 
                                     bool bForceAsText /*=false*/)
{
    CString strExt;
    int     iFound = strFileName.ReverseFind(_T('.'));
    strExt = strFileName.Mid(iFound + 1);

    if (bForceAsText)
    {
        strExt = _T("st");
    }

    CKADFileExtInfo* pKadFileExtInfo = m_tKadFileInfo.GetFileExtInfo(strExt);
    pDocTemplate = m_pSimpleTemplate;

    if(pKadFileExtInfo)
    {
        if(pKadFileExtInfo->m_bProgEditor)
        {
            pDocTemplate = bNeverTabbed ? m_pSimpleTemplate : m_pTabbedTemplate;
        }
        if(pKadFileExtInfo->m_pEditorGUID)
        {
            *pGUID = *pKadFileExtInfo->m_pEditorGUID;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}


//------------------------------------------------------------------*
/**
 * create document.
 *
 * @param           pDocTemplate: document template
 * @param           strFileName: file name
 * @param           bVisible: visible?
 * @param           rtGUID: editor CLSID for CoCreateInstance()
 * @param           eAction: activation action
 * @param           bPrintOpDoc: print options document?
 *                  (shall not be visible to the user)
 * @return          -
 * @exception       -
 * @see             -
*/
CGenEdDoc* CGenEdCont::CreateDocument
(
    CGenEdDocTemplate* pDocTemplate,
    const CString &strFileName,
    bool bVisible,
    GUID& rtGUID,
    E_ActivateAction eAction /*=eActivateClass*/,
    bool bPrintOpDoc /*=false*/
)
{
    CDocument*      pDoc = NULL;
    CGenEdDoc*      pGenEdDoc = NULL;
    CGenEdFrame*    pGenEdFrame = NULL;
    CGenEdView*     pServerView = NULL;
    CGenEdCntrItem* pServerItem = NULL;
    CArray<CGenEdView *, CGenEdView *> serverViews;
//    CGCDocSettings* pGCDocSettings;
    CWaitCursor wait;

    assert (pDocTemplate != NULL);
    if (pDocTemplate == NULL)
    {
        return (NULL);
    }

    BOOL    bShow(FALSE);
    if(bVisible && !bPrintOpDoc)
    {
        bShow = TRUE;
    }

	// SIS 22.07.04 >>
	// check maximum number of open documents
	if(m_iOpenDocumentCount >= m_iMaxOpenDocumentCount)
	{
		// SIS 08.11.04 >>
		if(!TryCloseUnusedDocument())
		{
			AfxMessageBox(IDS_MAXDOCCOUNTREACHED);
			return(NULL);
		}
		// SIS 08.11.04 <<
	}
	// SIS 22.07.04 <<

	TRY
	{
		pDoc = pDocTemplate->OpenDocumentFile(strFileName.IsEmpty () ? NULL : (LPCTSTR)strFileName, bShow);
		if (pDoc == NULL)
		{
			return (NULL);
		}
	}
	CATCH_ALL(e)
	{
		return NULL;
	}
	END_CATCH_ALL


    pGenEdDoc = dynamic_cast<CGenEdDoc* > (pDoc);
    assert (pGenEdDoc != NULL);
    if (pGenEdDoc == NULL)
    {
        return (NULL);
    }

    pGenEdFrame = FindFrame (pGenEdDoc);
    assert (pGenEdFrame != NULL);
    if (pGenEdFrame == NULL)
    {
        return (NULL);
    }

    if(bPrintOpDoc)
    {
        pGenEdFrame->SetWindowPos(&CWnd::wndBottom, -100, -100, 0, 0, SWP_NOZORDER);
        pGenEdFrame->ShowWindow(SW_SHOWNA);
    }

    // Since WM_NCACTIVATE was already sent to the frame
    // at the time we register the callback setting the
    // m_pActiveFrame member which is normally done in-
    // side the callback has to be performed here as well.
    // Hope that a GenEdFrame always becomes active on
    // creation. Otherwise before setting m_pActiveFrame
    // we had to check if the frame really became active.

    m_pActiveFrame = pGenEdFrame;

    pGenEdFrame->RegisterActionCB (&m_FrameActionCB);

    pGenEdDoc->SetBreakpointManager (m_pIBreakPoint);

//    if (m_pGCSettings != NULL)
//    {
//        pGCDocSettings = m_pGCSettings->FindDocument (strFileName);
//        assert (pGCDocSettings != NULL);
//        if (pGCDocSettings == NULL)
//        {
//            pGenEdDoc->ClearModified ();
//            pGenEdFrame->PostMessage (WM_CLOSE);
//            return (NULL);
//        }
//        pGenEdDoc->SetSettings (pGCDocSettings);
//    }

    pGenEdDoc->SetPrintInfo (m_strPrinterName, m_lPrintWidth, m_lPrintHeight);

    // SIS, 27.02.01: 
    pServerItem = pGenEdDoc->CreateServer(rtGUID);
    if(!pServerItem)
    {
        pGenEdDoc->ClearModified();
        pGenEdFrame->PostMessage(WM_CLOSE);
        return (NULL);
    }
    // END SIS

    if (!strFileName.IsEmpty ())
    {
        if (!LoadServer(pGenEdDoc))
        {
            pGenEdDoc->ClearModified ();
            pGenEdFrame->PostMessage (WM_CLOSE);
            return (NULL);
        }
    }


    if (bVisible)
    {
        pGenEdFrame->GetServerViews (serverViews);
        pServerView = serverViews[0];

        pGenEdFrame->SetActiveView(pServerView);

        pServerView->ShowServer(pServerItem);

        pGenEdFrame->ActivateServer();
        pGenEdFrame->SetFocusToServer();

        pGenEdFrame->UpdateTabs(eAction);
//        if(pGenEdDoc->IsReadOnly())
//        {
//            pServerItem->ForceReadOnly(true);
//        }
    }


    return (pGenEdDoc);
}

// 08.11.04 >>
BOOL CGenEdCont::TryCloseUnusedDocument()
{
	BOOL bClosed = CloseUnusedDocument(FALSE);
	if(!bClosed)
	{
		bClosed = CloseUnusedDocument(TRUE);
	}
	return bClosed;
}


//------------------------------------------------------------------*
/**
 * CloseUnusedDocument
 *
 * @param			
 * @return			
 * @exception		-
 * @see				
*/
BOOL CGenEdCont::CloseUnusedDocument(BOOL bForce)
{
    // get parent
    CWnd*				pParentWnd = NULL;
	CE_SRCDOCPOS_TYP	docPosition;
    CGenEdDoc*			pDoc = GetFirstDocument (docPosition, TRUE);
	CGenEdFrame*		pFrame = NULL;
    if(!pDoc)
    {
        pDoc = GetFirstDocument(docPosition, FALSE);
    }

    if(pDoc)
    {
	    pFrame = FindFrame (pDoc);
        ASSERT (pFrame != NULL);
        if(pFrame)
        {
            pParentWnd = pFrame->GetParent();
        }
    }
	ASSERT(pParentWnd);

    CWnd*   pWndTmp;
	BOOL	bClosed = FALSE;
    if(pParentWnd)
    {
        pWndTmp = pParentWnd->GetWindow(GW_CHILD);
        if(!pWndTmp)
        {
            return bClosed;
        }
        pWndTmp = pWndTmp->GetWindow(GW_HWNDLAST);
        if(!pWndTmp)
        {
            return bClosed;
        }
		do
		{
			pFrame = dynamic_cast<CGenEdFrame*> (pWndTmp);
			pWndTmp = pWndTmp->GetWindow(GW_HWNDPREV);
			if(pFrame)
			{
				pDoc = pFrame->GetCurrentEditorDoc();
				if(pDoc && (!pDoc->GetModified() || bForce))
				{
					CloseDocument(pDoc);
					bClosed = TRUE;
					break;
				}
			}
		} while(pWndTmp);
	}
	return bClosed;
}
// 08.11.04 <<



CGenEdFrame* CGenEdCont::FindFrame (CGenEdDoc* pGenEdDoc)
{
    AFX_MODULE_THREAD_STATE* pState;
    CFrameWnd* pFrame;
    CGenEdFrame* pGenEdFrame;

    pState = _AFX_CMDTARGET_GETSTATE ()->m_thread;
    pFrame = pState->m_frameList;
    while (pFrame != NULL)
    {
        if (pFrame->GetActiveDocument () == pGenEdDoc)
            break;

        pFrame = pState->m_frameList.GetNext (pFrame);
    }

    if (pFrame == NULL)
    {
        return (NULL);
    }

    pGenEdFrame = dynamic_cast<CGenEdFrame* > (pFrame);
    assert (pGenEdFrame != NULL);

    return (pGenEdFrame);
}



bool CGenEdCont::LoadServer (CGenEdDoc* pDoc)
{
    HRESULT hr;

    hr = pDoc->LoadServer();
    return (SUCCEEDED (hr) ? true : false);
}


bool CGenEdCont::ShowServer(CGenEdView* pServerView, CGenEdCntrItem* pServerItem)
{

    pServerView->ShowServer(pServerItem);

    return (true);
}


void CGenEdCont::OnFrameAction (GenEdFrameAction action, CGenEdFrame* pFrame)
{
    if (action == GenEdFrameActivated)
    {
        m_pActiveFrame = pFrame;
    }
    else if (action == GenEdFrameDestroyed)
    {
        if (pFrame == m_pActiveFrame)
        {
            m_pActiveFrame = NULL;
        }
    }
}


void CGenEdCont::OnConnectAction (CTargetOnlBase* pTarget, TargetConnectAction action)
{
    CTargetOnl* pTarg = NULL;

    switch(action)
    {
    case TargetPostConnect:
        OnConnectTarget (pTarget);
        break;
    case TargetPreDisconnect:
        OnDisconnectTarget (pTarget);
        break;
    case TargetPostExclusiveConnect:
        pTarg = dynamic_cast<CTargetOnl*> (pTarget);
        if(pTarg)
        {
            OnDebugConnectTarget (pTarg);
        }
        break;
    case TargetPreExclusiveDisconnect:
        pTarg = dynamic_cast<CTargetOnl*> (pTarget);
        if(pTarg)
        {
            OnDebugDisconnectTarget (pTarg);
        }
        break;
    }
}


void CGenEdCont::OnConnectTarget (CTargetOnlBase* pTarget)
{
    CGenEdDoc* pDoc;
    CE_SRCDOCPOS_TYP docPosition;

    for (pDoc = GetFirstDocument (docPosition); pDoc != NULL; pDoc = GetNextDocument (docPosition))
    {
        CGenEdFrame* pFrame;

        // Assignment changes are propagated to the editors at
        // connect time currently. This has to change as soon
        // as download sources are kept for debugging purposes.

        pFrame = FindFrame (pDoc);
        assert (pFrame != NULL);
        if (pFrame != NULL)
        {
            pFrame->CheckAssignmentChange ();
        }

        pDoc->NotifyConnect(pTarget->GetControlAddress());
    }
}


void CGenEdCont::OnDisconnectTarget (CTargetOnlBase* pTarget)
{
    CGenEdDoc* pDoc;
    CE_SRCDOCPOS_TYP docPosition;

    for (pDoc = GetFirstDocument (docPosition); pDoc != NULL; pDoc = GetNextDocument (docPosition))
    {
        pDoc->NotifyDisconnect(pTarget->GetControlAddress());
    }
}


void CGenEdCont::OnDebugConnectTarget (CTargetOnl* pTarget)
{
    CGenEdDoc* pDoc;
    CE_SRCDOCPOS_TYP docPosition;

    for (pDoc = GetFirstDocument (docPosition); pDoc != NULL; pDoc = GetNextDocument (docPosition))
    {
        CGenEdFrame* pFrame;

        // Assignment changes are propagated to the editors at
        // connect time currently. This has to change as soon
        // as download sources are kept for debugging purposes.

        pFrame = FindFrame (pDoc);
        assert (pFrame != NULL);
        if (pFrame != NULL)
        {
            pFrame->CheckAssignmentChange ();
        }
    }
}


void CGenEdCont::OnDebugDisconnectTarget (CTargetOnl* )
{
}


//void CGenEdCont::OpenSettingsDocuments ()
//{
//    POSITION pPosition;
//
//    pPosition = m_pGCSettings->GetDocumentStartPosition ();
//    while (pPosition != NULL)
//    {
//        CString strFileName;
//        CGCDocSettings* pDocSettings;
//
//        m_pGCSettings->GetNextDocument (pPosition, strFileName, pDocSettings);
//
//        (void)OpenDocument (strFileName, false, true);
//    }
//}


void CGenEdCont::RegisterReloadedCB (FC_CALLBACK_BASE (DocumentsReloadedCB)* pReloadedCB)
{
    m_ReloadedCBs.Add (pReloadedCB);
}


void CGenEdCont::UnregisterReloadedCB (FC_CALLBACK_BASE (DocumentsReloadedCB)* pReloadedCB)
{
    m_ReloadedCBs.Remove (pReloadedCB);
}


void CGenEdCont::SetBreakpointManager (const CComPtr<IBreakPoint> &pBreakpointManager)
{
    CE_SRCDOCPOS_TYP position;
    CGenEdDoc* pDoc;

    m_pIBreakPoint = pBreakpointManager;

    for (pDoc = GetFirstDocument (position); pDoc != NULL; pDoc = GetNextDocument (position))
    {
        pDoc->SetBreakpointManager (pBreakpointManager);
    }
}

//------------------------------------------------------------------*
/**
 * get server for file.
 *
 *  this function is used to get the server item for a file without
 *  showing it (e.g. for printing or finding ...)
 *
 * @param           [in] crstrFilePath: file name incl path
 * @return          pointer to document
 * @exception       -
 * @see             -
*/
CGenEdDoc* CGenEdCont::GetServerForFile(const CString& crstrFilePath)
{
    // search for loaded document
    CGenEdDoc*      pDoc = NULL;
    CGenEdCntrItem* pServer = NULL;

    // create new print server item
    CString strFileExt;
    int     iFound;

    iFound = crstrFilePath.ReverseFind(_T('.'));
    strFileExt = crstrFilePath.Mid(iFound + 1);
    pDoc = LoadPrintServer(strFileExt);

    return pDoc;
}

//------------------------------------------------------------------*
/**
 * get server for print options.
 *
 *  create new document for file extension.
 *  document must be activated but shall not be visible to the user.
 *
 * @param           strExtension: file extension
 * @return          -
 * @exception       -
 * @see             -
*/
CGenEdDoc* CGenEdCont::GetServerForPrintOptions(const CString &strExtension)
{
    CGenEdDocTemplate *pDocTemplate;
    GUID    tGUID;

    if (!GetCreateParameters(strExtension, true, pDocTemplate, &tGUID))
    {
        return (NULL);
    }

    CGenEdDoc* pDoc = CreateDocument(pDocTemplate, "", true, tGUID, eActivateClass, true);
    return (pDoc);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CGenEdCont 

SCODE CGenEdCont::ShowURL (LPCTSTR szURL) 
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    pMainFrame->OpenURL(szURL);

    return S_OK;
}


SCODE CGenEdCont::ShowFile (LPCTSTR szFile) 
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    // INCOMPLETE: Add code to show contents of a POU
    // *********** TODO ***********
    TRACE("CGenEdCont::ShowFile not yet implemented\n");
    return E_NOTIMPL;
}



SCODE CGenEdCont::SelectFBForCall () 
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    HRESULT hr = E_FAIL;

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return hr;
    }

    pMainFrame->ShowPouVarSelection();

    CPouVarSelWindow* pVarSelWin = pMainFrame->GetPouVarSel(false);
    if (pVarSelWin)
    {
        CPouVarSelCtrlWrapper* pVarSelWrapper = pVarSelWin->GetWrapper();
        if (pVarSelWrapper)
        {
            hr = pVarSelWrapper->ShowPouSelection(); 
        }
    }
    return hr;
}


SCODE CGenEdCont::ShowVarSelection(LPCTSTR szPouName)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    HRESULT hr;

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    pMainFrame->ShowPouVarSelection();

    CPouVarSelWindow* pVarSelWin = pMainFrame->GetPouVarSel(false);
    if (pVarSelWin)
    {
        CPouVarSelCtrlWrapper* pVarSelWrapper = pVarSelWin->GetWrapper();
        if (pVarSelWrapper)
        {
            hr = pVarSelWrapper->ShowVarSelection(szPouName); 
        }
    }
    return hr;
}



SCODE CGenEdCont::SetVarSelectionHint(LPCTSTR szPouName,
                                      LPCTSTR szVarNamePrefix,
                                      LPCTSTR szDataType)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    HRESULT hr = S_OK;
    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    CPouVarSelWindow* pVarSelWin = pMainFrame->GetPouVarSel(false);
    if (pVarSelWin)
    {
        CPouVarSelCtrlWrapper* pVarSelWrapper = pVarSelWin->GetWrapper();
        if (pVarSelWrapper)
        {
            hr = pVarSelWrapper->SetVarSelectionHint(szPouName, szVarNamePrefix, szDataType);
        }
    }
    return hr;
}




SCODE CGenEdCont::ShowHelp(LPCTSTR szPOU)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    HRESULT hr = S_OK;
    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    CPouVarSelWindow* pVarSelWin = pMainFrame->GetPouVarSel(false);
    if (pVarSelWin)
    {
        CPouVarSelCtrlWrapper* pVarSelWrapper = pVarSelWin->GetWrapper();
        if (pVarSelWrapper)
        {
            CString strFileName;
            hr = pVarSelWrapper->LookupUserHelp(szPOU, strFileName);
            if (hr==S_OK)
            {
                pMainFrame->OpenURL(strFileName);
            }
        }
    }
    return hr;
}



SCODE CGenEdCont::ShowHeader(LPCTSTR szPOU)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    HRESULT hr = S_OK;
    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    CPouVarSelWindow* pVarSelWin = pMainFrame->GetPouVarSel(false);
    if (pVarSelWin)
    {
        CPouVarSelCtrlWrapper* pVarSelWrapper = pVarSelWin->GetWrapper();
        if (pVarSelWrapper)
        {
            CString strHeaderFile;
            hr = pVarSelWrapper->LookupHeader(szPOU, strHeaderFile);
            if (hr==S_OK)
            {
                ShowHeaderFile(strHeaderFile);
            }
        }
    }
    return hr;
}

SCODE CGenEdCont::ShowHeaderFile(LPCTSTR szFileName)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());
    HRESULT hr = S_OK;
    CCEMAINApp *pApp = (CCEMAINApp *)AfxGetApp();
    ASSERT(pApp);
    if (pApp == NULL)
    {
        return E_FAIL;
    }

    CCEFileHandler* pFileHandler = pApp->m_pFileHandler;
    ASSERT(pFileHandler);
    if(!pFileHandler)
    {
        return E_FAIL;
    }
    CGenEdDoc* pDocument;
    pFileHandler->ShowFile(szFileName, pDocument);
    //Set the header files readonly
    if(pDocument)
    {
        pDocument->ForceReadOnly(true);
    }
    
    return hr;
}




SCODE CGenEdCont::GetLRUPous(VARIANT FAR* parrPous)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    HRESULT hr = S_OK;

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    CPouVarSelWindow* pVarSelWin = pMainFrame->GetPouVarSel(false);
    if (pVarSelWin)
    {
        CPouVarSelCtrlWrapper* pVarSelWrapper = pVarSelWin->GetWrapper();
        if (pVarSelWrapper)
        {
            SAFEARRAY* pSArr;
            hr = pVarSelWrapper->GetLRUPous(&pSArr);
            if (hr==S_OK)
            {
                parrPous->vt = VT_ARRAY | VT_BSTR;
                parrPous->parray = pSArr;
            }
        }
    }    
    return hr;
}

SCODE CGenEdCont::GetPouXML(LPCTSTR szPouName, BSTR FAR* pszPouDef)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    HRESULT hr = S_OK;

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    CPouVarSelWindow* pVarSelWin = pMainFrame->GetPouVarSel(false);
    if (pVarSelWin)
    {
        CPouVarSelCtrlWrapper* pVarSelWrapper = pVarSelWin->GetWrapper();
        if (pVarSelWrapper)
        {
            CComBSTR sPouName = szPouName;
            hr = pVarSelWrapper->GetPouXML(sPouName, pszPouDef);
        }
    }    
    return hr;
}

SCODE CGenEdCont::GetVarList(VARIANT FAR* parrVars)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    HRESULT hr = S_OK;

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    CPouVarSelWindow* pVarSelWin = pMainFrame->GetPouVarSel(false);
    if (pVarSelWin)
    {
        CPouVarSelCtrlWrapper* pVarSelWrapper = pVarSelWin->GetWrapper();
        if (pVarSelWrapper)
        {
            SAFEARRAY* pSArr;
            hr = pVarSelWrapper->GetVarList(&pSArr);
            if (hr==S_OK)
            {
                parrVars->vt = VT_ARRAY;
                parrVars->parray = pSArr;
            }
        }
    }    
    return hr;
}

SCODE CGenEdCont::GetPouList(VARIANT FAR* parrPous)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    HRESULT hr = S_OK;

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    CPouVarSelWindow* pVarSelWin = pMainFrame->GetPouVarSel(false);
    if (pVarSelWin)
    {
        CPouVarSelCtrlWrapper* pVarSelWrapper = pVarSelWin->GetWrapper();
        if (pVarSelWrapper)
        {
            SAFEARRAY* pSArr;
            hr = pVarSelWrapper->GetPouList(&pSArr);
            if (hr==S_OK)
            {
                parrPous->vt = VT_ARRAY;
                parrPous->parray = pSArr;
            }
        }
    }    
    return hr;
}

SCODE CGenEdCont::GetPouDefinition(LPCTSTR szPouName, BSTR FAR* pszPouDef)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    HRESULT hr = S_OK;

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    CPouVarSelWindow* pVarSelWin = pMainFrame->GetPouVarSel(false);
    if (pVarSelWin)
    {
        CPouVarSelCtrlWrapper* pVarSelWrapper = pVarSelWin->GetWrapper();
        if (pVarSelWrapper)
        {
            CComBSTR sPouName = szPouName;
            hr = pVarSelWrapper->GetPouDefinition(sPouName, pszPouDef);
        }
    }    
    return hr;
}

SCODE CGenEdCont::GetPouMember(LPCTSTR szPouName, VARIANT FAR* parrMembers)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    HRESULT hr = S_OK;

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    CPouVarSelWindow* pVarSelWin = pMainFrame->GetPouVarSel(false);
    if (pVarSelWin)
    {
        CPouVarSelCtrlWrapper* pVarSelWrapper = pVarSelWin->GetWrapper();
        if (pVarSelWrapper)
        {
            SAFEARRAY* pSArr;
            CComBSTR sPouName = szPouName;
            hr = pVarSelWrapper->GetPouMember(sPouName, &pSArr);
            if (hr==S_OK)
            {
                parrMembers->vt = VT_ARRAY;
                parrMembers->parray = pSArr;
            }
        }
    }    
    return hr;
}



SCODE CGenEdCont::XRefChangeRefPosition(BOOL bForward)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    HRESULT hr = S_OK;

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    CXRefWindow* pXRef = pMainFrame->GetXRef(false);
    if (pXRef)
    {
        CXRefCtrlWrapper* pXRefWrapper = pXRef->GetWrapper();
        if (pXRefWrapper)
        {
            pXRefWrapper->ChangeRefPosition(bForward);
        }
    }    
    return hr;
}



SCODE CGenEdCont::XRefSetActiveQuery(LPCTSTR sVariableName)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    HRESULT hr = S_OK;

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    CXRefWindow* pXRef = pMainFrame->GetXRef(false);
    if (pXRef)
    {
        pMainFrame->ShowXRef();
        CXRefCtrlWrapper* pXRefWrapper = pXRef->GetWrapper();
        if (pXRefWrapper)
        {
            pXRefWrapper->SetActiveQuery(sVariableName);
        }
    }    
    return hr;
}



SCODE CGenEdCont::ShowVarSelectionBox(LONG lXPos, LONG lYPos,
                                      LPCTSTR szLine, LONG lColumn,
                                      LPCTSTR szTypeHint,
                                      LPCTSTR szPouName,
                                      BSTR FAR* pszSelection,
                                      LONG* plStartColumn,
                                      LONG* plEndColumn)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    HRESULT hr = S_OK;

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    CPouVarSelWindow* pVarSelWin = pMainFrame->GetPouVarSel(false);
    if (pVarSelWin)
    {
        CPouVarSelCtrlWrapper* pVarSelWrapper = pVarSelWin->GetWrapper();
        if (pVarSelWrapper)
        {
            CComBSTR sLine = szLine;
            CComBSTR sPouName = szPouName;
            CComBSTR sTypeHint = szTypeHint;
            hr = pVarSelWrapper->ShowVarSelectionBox(lXPos, lYPos,
                                               sLine, lColumn,
                                               sTypeHint,
                                               sPouName,
                                               pszSelection,
                                               plStartColumn,
                                               plEndColumn);
        }
    }    
    return hr;
}


SCODE CGenEdCont::GetProjectManager(IUnknown** pProjManUnk)
{
    *pProjManUnk = NULL;
    *pProjManUnk = CEGetCOM(CE_COMPMAN_PRJMAN);
    if (!(*pProjManUnk))
    {
        return E_FAIL;
    }
    return S_OK;
}



// <<<--- Behandlungsroutinen für Nachrichten CGenEdCont 
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////




//------------------------------------------------------------------*
/**
 * load file info from KAD
 *
 *  called from Init()
 *  
 * @see             Init()
*/
bool CGenEdCont::LoadFileInfoFromKAD()
{
    CComPtr<IUnknown>   pKadMan;
    pKadMan.Attach(CEGetCOM(CE_COMPMAN_KADMAN));
    if(m_tKadFileInfo.Load(pKadMan))
    {
        return true;
    }
    return false;
}


CGenEdCont* CGenEdCont::GetMe ()
{
    return (m_pMe);
}


CGenEdCont* CEGetContainer ()
{
    CGenEdCont* pContainer;

    pContainer = CGenEdCont::GetMe ();
    ASSERT (pContainer != NULL);
    return (pContainer);
}



//------------------------------------------------------------------*
/**
 * save settings.
 *
 *  run over all documents and SaveSettings of the document.
 *  this will store the windows placement of the frame.
 *
 * @param           [in] pSettings: pointer to settings object
 * @return          -
 * @exception       -
 * @see             -
*/
void CGenEdCont::SaveSettings(CContainerSettings& rtContSettings)
{
    CGenEdFrame*        pFrame;
    CGenEdDoc*          pDoc;
    int                 iZOrder = 0;
    CE_SRCDOCPOS_TYP    docPosition;

    // update settings: remove not existing documents and set all documents to invisible
    rtContSettings.UpdateSettings();

    // get parent
    CWnd* pParentWnd = NULL;
    pDoc = GetFirstDocument (docPosition, TRUE);
    if(!pDoc)
    {
        pDoc = GetFirstDocument(docPosition, FALSE);
    }

    if(pDoc)
    {
        pFrame = FindFrame (pDoc);
        ASSERT (pFrame != NULL);
        if(pFrame)
        {
            pParentWnd = pFrame->GetParent();
        }
    }

    CWnd*   pWndTmp;
    if(pParentWnd)
    {
        pWndTmp = pParentWnd->GetWindow(GW_CHILD);
        if(!pWndTmp)
        {
            return;
        }
        pWndTmp = pWndTmp->GetWindow(GW_HWNDFIRST);
        if(!pWndTmp)
        {
            return;
        }

        // check, if maximized
        WINDOWPLACEMENT wndPlc;
        BOOL bMaximized(FALSE);
        pFrame = dynamic_cast<CGenEdFrame*> (pWndTmp);
        if(pFrame)
        {
            if(pFrame->GetWindowPlacement(&wndPlc))
            {
                if(wndPlc.showCmd == SW_SHOWMAXIMIZED)
                {
                    bMaximized = TRUE;
                }
            }
        }

        // now save settings for each frame
        do
        {
            pFrame = dynamic_cast<CGenEdFrame*> (pWndTmp);
            if(pFrame && pFrame->IsWindowVisible())
            {
                SaveFrameSettings(pFrame, rtContSettings, iZOrder, bMaximized);
            }
            pWndTmp = pWndTmp->GetWindow(GW_HWNDNEXT);
        } while(pWndTmp);
    }
}


void CGenEdCont::SaveFrameSettings(CGenEdFrame* pFrame, CContainerSettings& rtContSettings, int& riZOrder, BOOL bMaximized /*=FALSE*/)
{
    WINDOWPLACEMENT     wndPlc;
    CDocumentSettings*  pDocSettings;
    CString             strDocFilePath;
    CGenEdDoc*          pDoc;

    pDoc = pFrame->GetCurrentEditorDoc();
    if(pDoc)
    {
        if(pFrame->GetWindowPlacement(&wndPlc))
        {
            strDocFilePath = pDoc->GetPathName();
            pDocSettings = rtContSettings.FindDocument(strDocFilePath);
            if(pDocSettings)
            {
                if(bMaximized)
                {
                    wndPlc.showCmd = SW_SHOWMAXIMIZED;
                }
                pDocSettings->SetWindowPlacement(wndPlc);
                pDocSettings->SetDocumentVisible(TRUE);
                pDocSettings->SetTabbed(pFrame->IsTabbed());
                pDocSettings->SetZOrder(riZOrder++);
                pFrame->SaveInstanceInfo(pDocSettings, TRUE);
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * load settings.
 *
 * @param           [in] rtContSettings: container settings object
 * @return          -
 * @exception       -
 * @see             -
*/
void CGenEdCont::LoadSettings(CContainerSettings& rtContSettings)
{
    CDocumentSettings*      pDocSettings;
    CString                 strDocFilePath;
    POSITION                pos;
    CCEWindowPlacement      tWindowPlacement;
    CStringArray            astrInstances;
    BOOL                    bTabbed;
    CArray<CDocumentSettings*, CDocumentSettings*>  tDocSetArray;

    int iNumVisible = rtContSettings.GetNumVisibleDocs();
    try
    {
        tDocSetArray.SetSize(iNumVisible);
    }
    catch(CMemoryException* pE)
    {
        pE->Delete();
        return;
    }

    int iDoc;
    pos = rtContSettings.GetDocumentStartPosition();
    while(pos)
    {
        rtContSettings.GetNextDocument(pos, strDocFilePath, pDocSettings);
        if(pDocSettings && pDocSettings->IsDocumentVisible())
        {
            iDoc = pDocSettings->GetZOrder();
            if(iDoc >= 0 && iDoc < iNumVisible)
            {
                tDocSetArray[iDoc] = pDocSettings;
            }
        }
    }
    int iNumDocs = tDocSetArray.GetSize();
	// SIS 22.07.04 >>
	// check for maximum open document count
	int	iCount = 0;
	// SIS 22.07.04 <<

	// SIS 08.11.04 >>
	if(iNumDocs > m_iMaxRecoverDocumentCount)
	{
		iNumDocs = m_iMaxRecoverDocumentCount;
	}
	// SIS 08.11.04 <<

    for(iDoc = iNumDocs-1; iDoc >= 0; --iDoc)
    {
        pDocSettings = tDocSetArray[iDoc];
        if(pDocSettings)
        {
            bTabbed = pDocSettings->IsTabbed();
            strDocFilePath = pDocSettings->GetFilePath();
            if(!strDocFilePath.IsEmpty())
            {
                RecoverDocument(strDocFilePath, !bTabbed);
				++iCount;
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * recover document.
 *
 *  document must be in project directory.
 *
 * @param           [in] crstrDocFilePath: file name with or without path
 * @param           [in] bNeverTabbed: never tabbed?
 * @return          document pointer or NULL if not found
 * @exception       -
 * @see             -
*/
CGenEdDoc* CGenEdCont::RecoverDocument(const CString& crstrDocFilePath, bool bNeverTabbed)
{
    CGenEdDocTemplate*  pDocTemplate;
    GUID                tGUID;
    
    // check, if file exists
    CCEFileHandler* pFileHandler = ((CCEMAINApp*)AfxGetApp())->GetFileHandler();
    ASSERT(pFileHandler);

    if(!pFileHandler)
    {
        return(NULL);
    }

    CString strFilePath = pFileHandler->GetFilePathName(crstrDocFilePath);
    CFileStatus tFileStatus;
    if(!CFile::GetStatus(strFilePath, tFileStatus))
    {
        return (NULL);
    }

    if (!GetCreateParameters (strFilePath, bNeverTabbed, pDocTemplate, &tGUID))
    {
        return (NULL);
    }

    return CreateDocument(pDocTemplate, strFilePath, true, tGUID, eActivateInstance);
}


//------------------------------------------------------------------*
/**
 * reload project file.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CGenEdCont::ReloadProjectFile()
{
    // get project gen path from proj info
    HRESULT hr;
    CComPtr<IUnknown>       pTmp;
    CComQIPtr<ICEProjInfo>  pProjInfo;
    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjInfo = pTmp;
    if(!pProjInfo)
    {
        return;
    }

    CComBSTR    sProjName;
    CComBSTR    sProjPath;

    hr = pProjInfo->getProjectName(&sProjName);
    if(hr != S_OK)
    {
        return;
    }

    hr = pProjInfo->getProjectPath(&sProjPath);
    if(hr != S_OK)
    {
        return;
    }

    CString strProjName(sProjName);
    CString strProjPath(sProjPath);
    CString strProjectFilePathName;

    strProjectFilePathName.Format("%s\\%s.4cp", strProjPath, strProjName);

    CGenEdDoc*  pDoc = FindDocument(strProjectFilePathName);
    if(pDoc)
    {
        LoadDocument(pDoc);
    }
}

CGenEdDoc* CGenEdCont::GetFirstPrintServer()
{
    CGenEdDoc*  pDoc = NULL;
    CString strCLSID;
    m_PrintDocPos = m_tPrintDocsMap.GetStartPosition();
    m_tPrintDocsMap.GetNextAssoc(m_PrintDocPos, strCLSID, pDoc);
    return pDoc;
}

CGenEdDoc* CGenEdCont::GetNextPrintServer()
{
    CGenEdDoc*  pDoc = NULL;
    if(!m_PrintDocPos)
    {
        return pDoc;
    }

    CString strCLSID;
    m_tPrintDocsMap.GetNextAssoc(m_PrintDocPos, strCLSID, pDoc);
    return pDoc;
}


