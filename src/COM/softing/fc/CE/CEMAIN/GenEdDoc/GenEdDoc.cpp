

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// GenEdDoc.cpp : implementation of the CGenEdDoc class
//

#include "stdafx.h"
#include "utilif.h"
//#include "implobjhdl.h"
#include "CEMain.h"

#include "GenEdDoc.h"
#include "GenEdCont.h"
#include "GenEdView.h"
#include "GenEdCntrItem.h"
//#include "GenEdRC.h"
//#include "CeSettings.h"
#include "MainFrm.h"
//#include "PrjDoc.h"
#include "CECompMan.h"
#include "GenEdFrame.h"

#include <assert.h>

#include "gened_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGenEdDoc

IMPLEMENT_DYNCREATE(CGenEdDoc, COleDocument)

BEGIN_MESSAGE_MAP(CGenEdDoc, COleDocument)
	//{{AFX_MSG_MAP(CGenEdDoc)
	ON_COMMAND(IDM_FILE_CLOSE, OnFileClose)
	ON_COMMAND(IDM_FILE_SAVE, OnFileSave)
	ON_COMMAND(IDM_FILE_SAVE_AS, OnSaveAs)
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, COleDocument::OnUpdatePasteMenu)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINK, COleDocument::OnUpdatePasteLinkMenu)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_CONVERT, COleDocument::OnUpdateObjectVerbMenu)
	ON_COMMAND(ID_OLE_EDIT_CONVERT, COleDocument::OnEditConvert)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, COleDocument::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, COleDocument::OnEditLinks)
	ON_UPDATE_COMMAND_UI(ID_OLE_VERB_FIRST, COleDocument::OnUpdateObjectVerbMenu)
    ON_MESSAGE (MSG_PARSE_NOW, OnParseNow)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CGenEdDoc, COleDocument)
	//{{AFX_DISPATCH_MAP(CGenEdDoc)
	DISP_FUNCTION(CGenEdDoc, "GetContainer", GetContainer, VT_ERROR, VTS_PDISPATCH)
	DISP_FUNCTION(CGenEdDoc, "SetBreakpoint", SetBreakpoint,               VT_ERROR, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CGenEdDoc, "ClearBreakpoint", ClearBreakpoint,           VT_ERROR, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CGenEdDoc, "ActivateBreakpoint", ActivateBreakpoint,     VT_ERROR, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CGenEdDoc, "DeactivateBreakpoint", DeactivateBreakpoint, VT_ERROR, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CGenEdDoc, "ContinueTask", ContinueTask, VT_ERROR, VTS_NONE)
	DISP_FUNCTION(CGenEdDoc, "StepTask", StepTask, VT_ERROR, VTS_I4)
	DISP_FUNCTION(CGenEdDoc, "ClearInstanceBreakpoints", ClearInstanceBreakpoints, VT_ERROR, VTS_NONE)
	DISP_FUNCTION(CGenEdDoc, "ClearAllBreakpoints", ClearAllBreakpoints, VT_ERROR, VTS_NONE)
	DISP_FUNCTION(CGenEdDoc, "OnEditEvent", OnEditEvent, VT_ERROR, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IGenEd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {D70D1414-EBBB-11D1-8825-006097E1BE6E}
static const IID IID_ICEGenEdDocument =
{ 0xd70d1414, 0xebbb, 0x11d1, { 0x88, 0x25, 0x0, 0x60, 0x97, 0xe1, 0xbe, 0x6e } };

BEGIN_INTERFACE_MAP(CGenEdDoc, COleDocument)
	INTERFACE_PART(CGenEdDoc, IID_ICEGenEdDocument, Dispatch)
END_INTERFACE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGenEdDoc construction/destruction

CGenEdDoc::CGenEdDoc()
: m_pEditorServer(NULL),
//  m_pGCSettings(NULL),
  m_lPrintWidth(0),
  m_lPrintHeight(0),
  m_CheckedFileTime(0),
  m_bParse(TRUE),
  m_bReadOnly(FALSE),
  m_StateChangeCB(this, OnStateChange)
{
    EnableCompoundFile(TRUE);

    EnableAutomation();

    AfxOleLockApp();

    CESMRegisterCallback(&m_StateChangeCB);
	
	// SIS 22.07.04 >>
	IncreaseOpenDocumentCount();
	// SIS 22.07.04 <<
}


CGenEdDoc::~CGenEdDoc()
{
    CESMUnregisterCallback(&m_StateChangeCB);

	// SIS 22.07.04 >>
	DecreaseOpenDocumentCount();
	// SIS 22.07.04 <<
    
    AfxOleUnlockApp();
}

// SIS 22.07.04 >>
// maximum count of open documents
void CGenEdDoc::IncreaseOpenDocumentCount()
{
    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *>(AfxGetApp());
	ASSERT(pApp);

    if(pApp)
    {
		CGenEdCont *pContainer;

	    pContainer = pApp->GetContainer();
		ASSERT(pContainer);

		if(pContainer)
		{
			pContainer->IncreaseOpenDocumentCount();
		}
    }
}

void CGenEdDoc::DecreaseOpenDocumentCount()
{
    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *>(AfxGetApp());
	ASSERT(pApp);

    if(pApp)
    {
		CGenEdCont *pContainer;

	    pContainer = pApp->GetContainer();
		ASSERT(pContainer);

		if(pContainer)
		{
			pContainer->DecreaseOpenDocumentCount();
		}
    }
}
// SIS 22.07.04 <<


CString CGenEdDoc::GetDocumentFile()
{
    return(m_strDocumentFile);
}


BOOL CGenEdDoc::IsReadOnly()
{
    static  CTime statLastTime = CTime::GetCurrentTime();
    CTime   tTimeTmp = CTime::GetCurrentTime();
    CTimeSpan   tTimeSpan(0,0,0,3);
    if((tTimeTmp - tTimeSpan) < statLastTime)
    {
        return m_bReadOnly;
    }
    // look at file
    CString strFilePath = GetPathName();
    CFileStatus fileStatus;
    if (!CFile::GetStatus (strFilePath, fileStatus))
    {
        m_bReadOnly = true;
    }
    m_bReadOnly = (fileStatus.m_attribute & CFile::readOnly) != 0;
    return m_bReadOnly;
}

void CGenEdDoc::RegisterActionCB(FC_CALLBACK_BASE(GenEdActionCB) *pActionCB)
{
    m_ActionCBs.AddTail(pActionCB);
}


void CGenEdDoc::UnregisterActionCB(FC_CALLBACK_BASE(GenEdActionCB) *pActionCB)
{
    POSITION pPosition;

    pPosition = m_ActionCBs.Find(pActionCB);
    assert(pPosition != NULL);

    if(pPosition != NULL)
    {
        m_ActionCBs.RemoveAt(pPosition);
    }
}


void CGenEdDoc::SetBreakpointManager(CComPtr<IBreakPoint> pIBreakPoint)
{
    m_pIBreakPoint = pIBreakPoint;
}




//void CGenEdDoc::SetSettings(CGCDocSettings *pGCSettings)
//{
//    m_pGCSettings = pGCSettings;
//}


void CGenEdDoc::UpdateSettings()
{
    StoreWatchpoints();
}


HRESULT CGenEdDoc::SetPrintInfo(const CString &strPrinterName, long lWidth, long lHeight)
{
    HRESULT hrResult=S_OK;
//    int i;

    m_strPrinterName = strPrinterName;
    m_lPrintWidth = lWidth;
    m_lPrintHeight = lHeight;

    if(m_pEditorServer != NULL)
    {
        HRESULT hr;

        hr = m_pEditorServer->SetPrintInfo(m_strPrinterName, m_lPrintWidth, m_lPrintHeight);

        if(hrResult == S_OK && FAILED(hr))
        {
            hrResult = hr;
        }
    }

    return(hrResult);
}


void CGenEdDoc::OnFileClose()
{
    CDocument::OnFileClose();
}

void CGenEdDoc::OnFileSave()
{
    CDocument::OnFileSave();
}

void CGenEdDoc::OnSaveAs()
{
    DoSave(NULL, m_strDocumentFile == _T(""));
}



BOOL CGenEdDoc::OnNewDocument()
{

    // Do not call COleDocument::OnNewDocument because
    // we do not want a non-temporary compound docfile
    // to be created.

    if(!CDocument::OnNewDocument())
        return FALSE;


    // We always want a temporary docfile because we have
    // no file based loading and thus have to have a tem-
    // porary docfile survive throughout the documents
    // lifetime.

    if(!CreateTemporaryDocFile())
    {
        return(FALSE);
    }


    return(TRUE);
}


void CGenEdDoc::DeleteContents() 
{
    COleDocument::DeleteContents();

    // COleDocument::DeleteContents destroys all client items.
    // NULLify our member, so we run into an exception on access.

    m_pEditorServer = NULL;
}


BOOL CGenEdDoc::OnOpenDocument(LPCTSTR pszPathName)
{
    if(m_lpRootStg == NULL)
    {
        if(!CreateTemporaryDocFile())
        {
            return(FALSE);
        }
    }

    // read only?
    CFileStatus tFileStatus;
    if(CFile::GetStatus(pszPathName, tFileStatus))
    {
        if(tFileStatus.m_attribute & CFile::readOnly)
        {
            m_bReadOnly = TRUE;
        }
    }

    // Passing NULL indicates non file based opening. The temporary
    // docfile will remain in effect(otherwise OnOpenDocument would
    // try to open the given file as the docfile). Note that we have
    // to handle exceptions ourselves in this case.

    TRY
    {
        if(!COleDocument::OnOpenDocument(NULL))
        {
            return(FALSE);
        }
    }
    CATCH_ALL(e)
    {
        return(FALSE);
    }
    END_CATCH_ALL

    m_strDocumentFile = pszPathName;

    return(TRUE);
}


void CGenEdDoc::OnCloseDocument()
{
//    int iIndex;
//    CGenEdCntrItem *pServer;

    if(!m_strInstance.IsEmpty())
    {
        StoreWatchpoints();
    }

//    DeleteTemporaryFiles();

    m_ActionCBs.Execute(GenEdDocumentClosed, this);


    // Inside COleDocument::OnCloseDocument the active document servers
    // are being closed. We don't want any calls into the server to in-
    // terfere the shutdown process. Especially calls to SetClientExtent
    // from within CGenEdView::OnSize could be very annoying. So we tell
    // our client items to reject any server call requests from now on.

    if(m_pEditorServer != NULL)
    {
        m_pEditorServer->KeepOffServer();
    }

    COleDocument::OnCloseDocument();
}


BOOL CGenEdDoc::OnSaveDocument(LPCTSTR pszPathName)
{
    HRESULT hr;
//    int iServer;
    bool bSaveAs;
    CFileStatus fileStatus;

    assert(m_pEditorServer != NULL);
    if(m_pEditorServer == NULL)
    {
        return(FALSE);
    }


    // Passing NULL indicates non file based saving.

    if(!COleDocument::OnSaveDocument(NULL))
    {
        return(FALSE);
    }

    bSaveAs = m_strDocumentFile.CompareNoCase(pszPathName) != 0;

    if(m_pEditorServer != NULL)
    {
        if(m_pEditorServer->IsStandardEditor())
        {
            if(bSaveAs)
            {
                hr = m_pEditorServer->SaveDocumentAs(pszPathName);
            }
            else
            {
                hr = m_pEditorServer->SaveDocument();
            }

            if(FAILED(hr))
            {
                return(FALSE);
            }
        }
    }


    if(!bSaveAs)
    {
        if(CFile::GetStatus(m_strDocumentFile, fileStatus))
        {
            m_CheckedFileTime = fileStatus.m_mtime;
        }
        else
        {
            m_CheckedFileTime = 0;
        }
    }
    
    if(!bSaveAs)
    {
        // parse file only if it belongs to the project, 
        // else a 4cp that does not belong to the project
        // does corrupt the navigator
//        CEFILECLASS fileClass;
        
        m_ActionCBs.Execute(GenEdDocumentSaved, this);

        HRESULT hr;
        CComQIPtr<ICEProjectManager>    pProjMan;
        CComQIPtr<ICEProjInfo>          pProjInfo;
        CComPtr<IUnknown>               pTmp;
        pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
        pProjMan = pTmp;
        pProjInfo = pTmp;

        if(pProjMan && pProjInfo)
        {
            E_FV_FileCategory   eFileCat;
            CFile               tFile(m_strDocumentFile, 0);

            CComBSTR            sFileName(tFile.GetFileName());

            hr = pProjMan->GetFileCategory(sFileName, &eFileCat);
            if(hr == S_OK)
            {
                if(eFileCat != FV_FILE_UNKNOWN)
                {
                    pProjInfo->fileChangedHint(sFileName);
                    if(m_bParse)
                    {
                        //pProjInfo->fileParseNow(NULL);
                        PostMessage(NULL, MSG_PARSE_NOW, NULL, NULL);
                    }
                }
            }
        }
        ClearModified();
    }
    return(TRUE);
}


LRESULT CGenEdDoc::OnParseNow(WPARAM wParam, LPARAM lParam)
{
    CComQIPtr<ICEProjInfo>          pProjInfo;
    CComPtr<IUnknown>               pTmp;
    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjInfo = pTmp;

    pProjInfo->fileParseNow(NULL);

    return 0;
}



bool CGenEdDoc::SaveIfModified(bool bAskBeforeSave)
{
    if(bAskBeforeSave)
    {
        return(SaveModified() == TRUE ? true : false);
    }

    UpdateModifiedFlag();

    if(IsModified())
    {
        if(!DoFileSave())
        {
            return(false);
        }
    }
    return(true);
}


BOOL CGenEdDoc::DoFileSave()
{
    if(!CheckWriteable(m_strPathName))
    {
        if(AfxMessageBox(IDS_SAVE_TO_DIFFERENT_FILE_NAME, MB_OKCANCEL) == IDCANCEL)
        {
            return(FALSE);
        }
    }

    return(COleDocument::DoFileSave());
}


BOOL CGenEdDoc::DoSave(LPCTSTR pszPathName, BOOL bReplace)
{
    CString strNewName;
    CDocTemplate *pTemplate;

    strNewName = pszPathName;
    if(!strNewName.IsEmpty())
    {
        if(!COleDocument::DoSave(strNewName, bReplace))
        {
            return(FALSE);
        }

        if(bReplace)
        {
            m_strDocumentFile = strNewName;
            m_pEditorServer->SetDocumentFile(strNewName);
        }

        return(TRUE);
    }

    pTemplate = GetDocTemplate();
    assert(pTemplate != NULL);

    strNewName = m_strPathName;
    if(bReplace && strNewName.IsEmpty())
    {
        strNewName = m_strTitle;
    }

    while(TRUE)
    {
        BOOL bResult;
//        CEFILECLASS fileClass;
        bool bWriteable;

        do
        {
            bResult = AfxGetApp()->DoPromptFileName(strNewName,
                                                      bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
                                                      OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
                                                      FALSE,
                                                      pTemplate);
            if(!bResult)
            {
                return(FALSE);
            }

            bWriteable = CheckWriteable(strNewName);
            if(!bWriteable)
            {
                if(AfxMessageBox(IDS_SAVE_TO_DIFFERENT_FILE_NAME, MB_OKCANCEL) == IDCANCEL)
                {
                    return(FALSE);
                }
            }
        } while(!bWriteable);

        HRESULT hr;
        CComQIPtr<ICEProjectManager>    pProjMan;
        CComPtr<IUnknown>               pTmp;
        pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
        pProjMan = pTmp;

        if(!pProjMan)
        {
            break;
        }

        E_FV_FileCategory   eFileCat;
        CComBSTR            sFileName(strNewName);

        hr = pProjMan->GetFileCategory(sFileName, &eFileCat);
        if(hr == S_OK)
        {
            if(eFileCat == FV_FILE_UNKNOWN)
            {
                break;
            }
        }

        AfxMessageBox(IDS_CANNOT_SAVE_TO_FILE_IN_PROJECT, MB_OK);
    }

    if(!COleDocument::DoSave(strNewName, bReplace))
    {
        return(FALSE);
    }

    if(bReplace)
    {
        m_strDocumentFile = strNewName;
        m_pEditorServer->SetDocumentFile(strNewName);
    }

    return(TRUE);
}


bool CGenEdDoc::CheckWriteable(const CString &strPath)
{
    CFileStatus fileStatus;

    if(!CFile::GetStatus(strPath, fileStatus))
    {
        return(true);
    }

    return(!(fileStatus.m_attribute & CFile::readOnly));
}


CGenEdCntrItem *CGenEdDoc::CreateServer(const GUID& rtGUID)
{
    CGenEdCntrItem *pServer;

    TRY
    {
        pServer = new CGenEdCntrItem(this);
        ASSERT_VALID(pServer);
        if(pServer == NULL)
        {
            return(NULL);
        }


        if(!pServer->CreateServer(rtGUID))
        {
            delete pServer;
            return(NULL);
        }

        if(pServer->IsStandardEditor())
        {
            HRESULT hr;

            hr = pServer->SetPrintInfo(m_strPrinterName, m_lPrintWidth, m_lPrintHeight);

            if(FAILED(hr))
            {
                delete pServer;
                return(NULL);
            }
        }

        assert(m_pEditorServer == NULL);

        if(m_pEditorServer == NULL)
        {
            m_pEditorServer = pServer;
        }
    }
    CATCH_ALL(e)
    {
        if(pServer != NULL)
        {
            pServer->Delete();
            m_pEditorServer = NULL;
        }

        e->Delete();
        return(NULL);
    }
    END_CATCH_ALL

    return(pServer);
}


CGenEdCntrItem *CGenEdDoc::GetEditorServer()
{
    return(m_pEditorServer);
}


CGenEdCntrItem *CGenEdDoc::GetActiveServer()
{
    POSITION pPosition;

    pPosition = GetStartPosition();
    while(pPosition != NULL)
    {
        CGenEdCntrItem *pServer;

        pServer = dynamic_cast<CGenEdCntrItem *>(GetNextClientItem(pPosition));
        assert(pServer != NULL);
        if(pServer == NULL)
            continue;

        if(pServer->IsInPlaceActive())
        {
            return(pServer);
        }
    }

    return(NULL);
}


HRESULT CGenEdDoc::LoadServer()
{
    HRESULT hr;
    CFileStatus fileStatus;
    hr = LoadEditorServer(m_strDocumentFile);
    if(CFile::GetStatus(m_strDocumentFile, fileStatus))
    {
        m_CheckedFileTime = fileStatus.m_mtime;
    }
    else
    {
        m_CheckedFileTime = 0;
    }
    return(S_OK);
}


HRESULT CGenEdDoc::ReloadServer()
{ 
    BOOL b = FALSE;
    EDP_POS_QUALIFIER posQual;
    long lX;
    long lY;
    long lCX;
    long lCY;
    HRESULT hr;
    HRESULT hrRet;

    if(m_pEditorServer != NULL)
    {
        hr = m_pEditorServer->GetPosition(posQual, lX, lY, lCX, lCY);
        if (hr==S_OK)
        {
            b = TRUE;
        }
    }

    hrRet = LoadServer();
	  
    if (b && m_pEditorServer)
    {
        hr = m_pEditorServer->GotoPosition(posQual, lX, lY, lCX, lCY);
    }

    CGenEdFrame* pFrame = NULL;
    pFrame = GetGenEdFrame();
    if (pFrame)
    {
        pFrame->SetFocusToServer();
    }

    return hrRet;
}





//HRESULT CGenEdDoc::DeleteTemporaryFiles()
//{
//    CGenEdCntrItem *pServer;
//
//    if(m_LoadMode != SplitLoad)
//    {
//        return(S_FALSE);
//    }
//
//    pServer = GetSecondaryServer(0);
//    ASSERT(pServer != NULL);
//    if(pServer == NULL)
//    {
//        return(E_FAIL);
//    }
//
//    strSecondaryFile = pServer->GetDocumentFile();
//    ASSERT(strSecondaryFile != _T(""));
//    if(strSecondaryFile == _T(""))
//    {
//        return(E_FAIL);
//    }
//
//    if(!::DeleteFile(strSecondaryFile))
//    {
//        return(E_FAIL);
//    }
//
//    return(S_OK);
//}


//CGenEdCntrItem *CGenEdDoc::GetSecondaryServer(unsigned int uiIndex)
//{
//    CGenEdCntrItem *pServer;
//
//    if((int)uiIndex >= m_SecondaryServers.GetSize())
//    {
//        return(NULL);
//    }
//
//    pServer = m_SecondaryServers[uiIndex];
//    assert(pServer != NULL);
//    return(pServer);
//}


HRESULT CGenEdDoc::LoadEditorServer(const CString &strFile)
{
    HRESULT hr;

    if(m_pEditorServer == NULL)
    {
        return(S_FALSE);
    }

    if(!m_pEditorServer->IsStandardEditor())
    {
        return(S_FALSE);
    }

    hr = m_pEditorServer->LoadDocument(strFile);
    if(FAILED(hr))
    {
        return(hr);
    }

    hr = m_pEditorServer->SetBaseFile(m_strDocumentFile);

    return(hr);
}


//HRESULT CGenEdDoc::LoadSecondaryServer(CGenEdCntrItem *pServer, const CString &strFile)
//{
//    HRESULT hr;
//
//    assert(pServer != NULL);
//    if(pServer == NULL)
//    {
//        return(E_UNEXPECTED);
//    }
//
//    assert(pServer != m_pEditorServer);
//    if(pServer == m_pEditorServer)
//    {
//        return(E_UNEXPECTED);
//    }
//
//    if(!pServer->IsStandardEditor())
//    {
//        return(S_FALSE);
//    }
//
//    hr = pServer->LoadDocument(strFile);
//    if(FAILED(hr))
//    {
//        return(hr);
//    }
//
//    hr = pServer->SetBaseFile(m_strDocumentFile);
//
//    return(hr);
//}


HRESULT CGenEdDoc::GetSelectedText(CString &rstrText)
{
    HRESULT hr;

    if(m_pEditorServer == NULL)
    {
        return(S_FALSE);
    }
    hr = m_pEditorServer->GetSelectedText(rstrText);
    return(hr);
}


void CGenEdDoc::SetDelayedPositioning(bool bDelay)
{
//    int iIndex;

    if(m_pEditorServer != NULL)
    {
        m_pEditorServer->SetDelayedPositioning(bDelay);
    }

//    for(iIndex = 0; iIndex < m_SecondaryServers.GetSize(); ++iIndex)
//    {
//        CGenEdCntrItem *pServer;
//
//        pServer = m_SecondaryServers.GetAt(iIndex);
//        assert(pServer != NULL);
//        if(pServer == NULL)
//            continue;
//
//        pServer->SetDelayedPositioning(bDelay);
//    }
}


void CGenEdDoc::GetInstance(CString &strInstance, CString &strMachineName, CString &strMachineAddress) const
{
    strInstance = m_strInstance;
    strMachineName = m_strMachineName;
    strMachineAddress = m_strMachineAddress;
}


void CGenEdDoc::SetInstance(const CString &strInstance, const CString &strMachineName, const CString &strMachineAddress)
{
    POSITION pPosition;

    if(!m_strInstance.IsEmpty())
    {
        StoreWatchpoints();
    }

    m_strInstance = strInstance;
    m_strMachineName = strMachineName;
    m_strMachineAddress = strMachineAddress;

    // HACK HACK HACK:
    // For targets with custom connect interface, 
    // the connect information can contain charecters as . (in a IP address for example)
    // but . is used in the editors to split the instance path
    // the hack here is to pass the target name instead of the full connect address
    CString strModInstance = strInstance;
    int ipos = strModInstance.Find(_T('}'));
    if (ipos>0)
    {
        strModInstance = strModInstance.Mid(ipos+1);
        strModInstance = strMachineName + strModInstance;
    }
    m_strInstanceWithTargetName = strModInstance;

    pPosition = GetStartPosition();
    while(pPosition != NULL)
    {
        CGenEdCntrItem *pServer;

        pServer = dynamic_cast<CGenEdCntrItem *>(GetNextClientItem(pPosition));
        ASSERT(pServer != NULL);
        if(pServer == NULL)
            continue;

        if(!pServer->IsProgramEditor())
            continue;

        pServer->SetInstance(strModInstance, strMachineName, strMachineAddress);
    }

    if(!strModInstance.IsEmpty())
    {
        RestoreWatchpoints();
        m_pIBreakPoint->UpdateAllBPs(CComBSTR(strModInstance));
    }
}


void CGenEdDoc::NotifyConnect(const CString &strMachineAddress)
{
    POSITION pPosition;

    pPosition = GetStartPosition();
    while(pPosition != NULL)
    {
        CGenEdCntrItem *pServer;

        pServer = dynamic_cast<CGenEdCntrItem *>(GetNextClientItem(pPosition));
        assert(pServer != NULL);
        if(pServer == NULL)
            continue;

        if(!pServer->IsProgramEditor())
            continue;

        pServer->NotifyConnect(strMachineAddress);
    }
}


void CGenEdDoc::NotifyDisconnect(const CString &strMachineAddress)
{
    POSITION pPosition;

    pPosition = GetStartPosition();
    while(pPosition != NULL)
    {
        CGenEdCntrItem *pServer;

        pServer = dynamic_cast<CGenEdCntrItem *>(GetNextClientItem(pPosition));
        assert(pServer != NULL);
        if(pServer == NULL)
            continue;

        if(!pServer->IsProgramEditor())
            continue;

        pServer->NotifyDisconnect(strMachineAddress);
    }
}


void CGenEdDoc::NotifyDebugState(CSC_SESSION_STATE state)
{
    POSITION pPosition;

    pPosition = GetStartPosition();
    while(pPosition != NULL)
    {
        CGenEdCntrItem *pServer;

        pServer = dynamic_cast<CGenEdCntrItem *>(GetNextClientItem(pPosition));
        assert(pServer != NULL);
        if(pServer == NULL)
            continue;

        if(!pServer->IsProgramEditor())
            continue;

        pServer->NotifyDebugState(state);
    }
}


void CGenEdDoc::ForceReadOnly(bool bOn)
{
    POSITION pPosition;

    pPosition = GetStartPosition();
    while(pPosition != NULL)
    {
        CGenEdCntrItem *pServer;
        HRESULT hr;

        pServer = dynamic_cast<CGenEdCntrItem *>(GetNextClientItem(pPosition));
        assert(pServer != NULL);
        if(pServer == NULL)
            continue;

        if(!pServer->IsStandardEditor())
            continue;

        hr = pServer->ForceReadOnly(bOn);
        assert(SUCCEEDED(hr));
    }
}


HRESULT CGenEdDoc::GotoPosition(CGenEdCntrItem *pServer, 
                                EDP_POS_QUALIFIER posQual, 
                                long lX, long lY, long lCX, long lCY)
{
    HRESULT hr;

    assert(pServer != NULL);
    if(pServer == NULL)
    {
        return(E_FAIL);
    }

    hr = pServer->GotoPosition(posQual, lX, lY, lCX, lCY);
    return(hr);
}


HRESULT CGenEdDoc::GotoLocation(const CString &strPosition)
{
    HRESULT hr;
    hr = m_pEditorServer->GotoLocation(strPosition);
    return(hr);
}


bool CGenEdDoc::GetModified()
{
    POSITION pPosition;
    bool bModified = false;

    pPosition = GetStartPosition();
    while(pPosition != NULL)
    {
        CGenEdCntrItem *pServer;
        HRESULT hr;

        pServer = dynamic_cast<CGenEdCntrItem *>(GetNextClientItem(pPosition));
        assert(pServer != NULL);
        if(pServer == NULL)
            continue;

        if(!pServer->IsStandardEditor())
            continue;

        hr = pServer->GetModified();
//        assert(SUCCEEDED(hr));
        // SIS: this assertion fires when you click into the client area of 
        // an deactivated editor window. Error message is:
        // An outgoing call cannot be executed since the application 
        // performs an input synchronized call. 
        // Maybe the server item is not yet completely initialized at this time.

        if(hr == S_OK)
        {
            bModified = true;
        }
    }

    return(bModified);
}


void CGenEdDoc::ClearModified()
{
    POSITION pPosition;

    pPosition = GetStartPosition();
    while(pPosition != NULL)
    {
        CGenEdCntrItem *pServer;
        HRESULT hr;

        pServer = dynamic_cast<CGenEdCntrItem *>(GetNextClientItem(pPosition));
        assert(pServer != NULL);
        if(pServer == NULL)
            continue;

        if(!pServer->IsStandardEditor())
            continue;

        hr = pServer->SetModified(false);
        assert(SUCCEEDED(hr));
    }

    SetModifiedFlag(FALSE);
}


CString CGenEdDoc::GetPositionText()
{
//    CGenEdCntrItem *pServer;
//
//    pServer = GetActiveServer();
//    if(pServer == NULL)
//    {
//        return("");
//    }
//
    return(m_pEditorServer->GetPositionText());
}


void CGenEdDoc::UpdateModifiedFlag()
{

    // COleDocuments version of this method queries all its
    // client items whether they are dirty or not. These
    // client items in turn query their servers whether they
    // are dirty or not. Regardless if the server document
    // is set dirty or not after creation via OleCreate the
    // IPersistStorage::IsDirty method call returns "dirty".
    // I suspect that there is some instance between the client
    // item and the server document(maybe the inproc handler)
    // that caches "dirty" information. The solution to the
    // problem is to not update the modified flag on behalf of
    // the IPersistStorage::IsDirty method. Instead the modi-
    // fied flag is updated on behalf of some custom GetModified
    // method(cannot use IsModified because COleClientItem
    // already has this).

    if(GetModified())
    {
        SetModifiedFlag();
    }
}

void CGenEdDoc::OnUpdateMruList(CCmdUI *pCmdUI) 
{
//    HRESULT hr;
    CComQIPtr<ICEProjectManager>    pProjMan;
    CComPtr<IUnknown>               pTmp;
    pTmp.Attach(CEGetCOM(CE_COMPMAN_PRJMAN));
    pProjMan = pTmp;

    //pCmdUI->Enable(pProjMan == NULL || !pProjMan->IsProjectLoaded());
    pCmdUI->Enable(TRUE);
}


HRESULT CGenEdDoc::ShowBreakpoint(EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY, short sState)
{
    HRESULT hr;

    if(m_pEditorServer == NULL)
    {
        return(E_FAIL);
    }

    hr = m_pEditorServer->ShowBreakpoint(posQual, lX, lY, lCX, lCY, sState);

    return(hr);
}


HRESULT CGenEdDoc::HideBreakpoint(EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY)
{
    HRESULT hr;

    if(m_pEditorServer == NULL)
    {
        return(E_FAIL);
    }

    hr = m_pEditorServer->HideBreakpoint(posQual, lX, lY, lCX, lCY);

    return(hr);
}


HRESULT CGenEdDoc::ShowStoppoint(EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY)
{
    HRESULT hr;

    if(m_pEditorServer == NULL)
    {
        return(E_FAIL);
    }

    hr = m_pEditorServer->ShowStoppoint(posQual, lX, lY, lCX, lCY);

    return(hr);
}


HRESULT CGenEdDoc::HideStoppoint()
{
    HRESULT hr;

    if(m_pEditorServer == NULL)
    {
        return(E_FAIL);
    }

    hr = m_pEditorServer->HideStoppoint();

    return(hr);
}


HRESULT CGenEdDoc::InsertFBCall(const CString &strFB)
{
    HRESULT hr;

    if(m_pEditorServer == NULL)
    {
        return(E_FAIL);
    }

    hr = m_pEditorServer->InsertFBCall(strFB);

    return(hr);
}

HRESULT CGenEdDoc::InsertVariable(const CString &strVar)
{
    HRESULT hr;

    if(m_pEditorServer == NULL)
    {
        return(E_FAIL);
    }

    hr = m_pEditorServer->InsertVariable(strVar);

    return(hr);
}



CStringArray* CGenEdDoc::GetWatchpoints (const CString &strInstance, BOOL bCreate)
{
/*	CString strTemp=strInstance;
	CObject *pObject;
    CStringArray *pWatchpoints;

	strTemp.MakeLower ();

	if (m_WatchpointMap.Lookup (strTemp, pObject))
	{
        pWatchpoints = dynamic_cast<CStringArray*>(pObject);
		ASSERT(pWatchpoints != NULL);
		return (pWatchpoints);
	};

    pWatchpoints = new CStringArray;
	ASSERT(pWatchpoints != NULL);
	if (pWatchpoints == NULL)
	{
		return (NULL);
	}

	m_WatchpointMap.SetAt (strTemp, pWatchpoints);
	return (pWatchpoints);
*/

    CCEMAINApp* pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
    ASSERT(pApp);

    CSettings* pSettings = pApp->GetSettings();
    if (!pSettings)
    {
        return NULL;
    }

    CDocumentSettings* pDocSet = NULL;
    pDocSet = pSettings->m_containerSettings.FindDocument(m_strDocumentFile, bCreate);
    if(!pDocSet)
    {
        return NULL;
    }

    CStringArray *pWatchpoints = NULL;
    pWatchpoints = pDocSet->GetWatchpoints(strInstance, bCreate);

    return pWatchpoints;

}





HRESULT CGenEdDoc::StoreWatchpoints()
{
    CStringArray    tWatchpoints;
    CStringArray*   pWatchpoints = NULL;

    if(m_pEditorServer->IsProgramEditor())
    {
        m_pEditorServer->StoreWatchpoints(tWatchpoints);

        pWatchpoints = GetWatchpoints(m_strInstance, TRUE);
        ASSERT(pWatchpoints != NULL);
        if(pWatchpoints == NULL)
        {
            return(E_FAIL);
        } 
        pWatchpoints->Copy(tWatchpoints);
    }

    return(S_OK);
}


HRESULT CGenEdDoc::RestoreWatchpoints()
{
    CStringArray* pWatchpoints;
   
    pWatchpoints = GetWatchpoints(m_strInstance, FALSE);
    if(pWatchpoints == NULL)
    {
        return(E_FAIL);
    }

    if(m_pEditorServer->IsProgramEditor())
    {
        m_pEditorServer->RestoreWatchpoints(*pWatchpoints);
    }

    return(S_OK);
}


bool CGenEdDoc::ModifyCheck()
{
    CFileStatus fileStatus;

    if(m_strDocumentFile.IsEmpty())
    {
        return(false);
    }

    if(!CFile::GetStatus(m_strDocumentFile, fileStatus))
    {
        return(false);
    }

    // For some reason we did not get a file time
    // when loading the servers. We begin checking
    // from this point of time.

    if(m_CheckedFileTime == 0)
    {
        m_CheckedFileTime = fileStatus.m_mtime;
    }

    if(fileStatus.m_mtime == m_CheckedFileTime)
    {
        return(false);
    }

    m_CheckedFileTime = fileStatus.m_mtime;
    return(true);
}

void CGenEdDoc::ResetCheckedFileTime()
{
    CFileStatus fileStatus;

    if(m_strDocumentFile.IsEmpty())
    {
        return;
    }

    if(!CFile::GetStatus(m_strDocumentFile, fileStatus))
    {
        return;
    }

    m_CheckedFileTime = fileStatus.m_mtime;
}

CDocumentSettings* CGenEdDoc::GetDocumentSettings()
{
    CDocumentSettings*  pDocSettings = NULL;
    CCEMAINApp* pApp = (CCEMAINApp*)AfxGetApp();
    CSettings*  pSettings = pApp->GetSettings();
    ASSERT(pSettings);
    if(pSettings)
    {
        pDocSettings = pSettings->m_containerSettings.FindDocument(GetPathName(), FALSE);
    }
    return pDocSettings;
}

void CGenEdDoc::EnableParsing(BOOL bParse /* = TRUE*/)
{
    m_bParse = bParse;
}


void CGenEdDoc::OnStateChange(CEStateNotify stateNotify, unsigned int uiStage)
{
    if(stateNotify == CETargetAddressModifyNotify)
    {
        OnTargetAddressModified();
    }
}


void CGenEdDoc::OnTargetAddressModified()
{
    CTargetOnlBase *pTarget;

    pTarget = CESMGetTarget();
    if(pTarget == NULL)
    {
        return;
    }

    if(m_strMachineName.CompareNoCase(pTarget->GetId()) != 0)
    {
        return;
    }

    SetInstance(m_strInstance, m_strMachineName, pTarget->GetControlAddress());
}


/////////////////////////////////////////////////////////////////////////////
// CGenEdDoc serialization

void CGenEdDoc::Serialize(CArchive& ar) {
    if(ar.IsStoring())
    {
        // TODO: add storing code here
    }
    else
    {
        // TODO: add loading code here
    }

    COleDocument::Serialize(ar);
}


/////////////////////////////////////////////////////////////////////////////
// CGenEdDoc diagnostics

#ifdef _DEBUG
void CGenEdDoc::AssertValid() const
{
    COleDocument::AssertValid();
}

void CGenEdDoc::Dump(CDumpContext& dc) const
{
    COleDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGenEdDoc commands


/////////////////////////////////////////////////////////////////////////////
// CGenEdDoc helpers

BOOL CGenEdDoc::CreateTemporaryDocFile()
{
    LPSTORAGE pStorage;
    SCODE sc;

    ASSERT(m_lpRootStg == NULL);


    sc = ::StgCreateDocfile(NULL, STGM_DELETEONRELEASE | STGM_READWRITE | STGM_TRANSACTED | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
	                           0, &pStorage);
    if(FAILED(sc))
        return(FALSE);

    m_lpRootStg = pStorage;

    return(TRUE);
}


/////////////////////////////////////////////////////////////////////////////
// Default dispatch interface

SCODE CGenEdDoc::GetContainer(LPDISPATCH *pContainerDispatch)
{
    CCEMAINApp *pApp;
    CGenEdCont *pContainer;

    pApp = dynamic_cast<CCEMAINApp *>(AfxGetApp());
    assert(pApp != NULL);
    if(pApp == NULL)
    {
        return(E_UNEXPECTED);
    }

    pContainer = pApp->GetContainer();
    assert(pContainer != NULL);
    if(pContainer == NULL)
    {
        return(E_UNEXPECTED);
    }

    *pContainerDispatch = pContainer->GetIDispatch(TRUE);
    assert(*pContainerDispatch != NULL);
    if(*pContainerDispatch == NULL)
    {
        return(E_UNEXPECTED);
    }

    return(S_OK);
}


SCODE CGenEdDoc::SetBreakpoint(long lQualifier, long lX, long lY, long lCX, long lCY)
{
    if(m_strInstance == _T(""))
    {
        return(S_FALSE);
    }

    // perhaps it should now look like that ????
    if(m_strInstanceWithTargetName == _T(""))
    {
        return(S_FALSE);
    }

    return(m_pIBreakPoint->SetBP(CComBSTR(m_strInstanceWithTargetName),(EDP_POS_QUALIFIER)lQualifier, lX, lY, lCX, lCY, TRUE));
}


SCODE CGenEdDoc::ClearBreakpoint(long lQualifier, long lX, long lY, long lCX, long lCY)
{
    if(m_strInstance == _T(""))
    {
        return(S_FALSE);
    }

    return(m_pIBreakPoint->ResetBP(CComBSTR(m_strInstance),(EDP_POS_QUALIFIER)lQualifier, lX, lY, lCX, lCY));
}


SCODE CGenEdDoc::ActivateBreakpoint(long lQualifier, long lX, long lY, long lCX, long lCY)
{
    if(m_strInstance == _T(""))
    {
        return(S_FALSE);
    }

    return(m_pIBreakPoint->ActivateBP(CComBSTR(m_strInstance),(EDP_POS_QUALIFIER)lQualifier, lX, lY, lCX, lCY));
}


SCODE CGenEdDoc::DeactivateBreakpoint(long lQualifier, long lX, long lY, long lCX, long lCY)
{
    if(m_strInstance == _T(""))
    {
        return(S_FALSE);
    }

    return(m_pIBreakPoint->DeactivateBP(CComBSTR(m_strInstance),(EDP_POS_QUALIFIER)lQualifier, lX, lY, lCX, lCY));
}


SCODE CGenEdDoc::ContinueTask()
{
    if(m_strInstance == _T(""))
    {
        return(S_FALSE);
    }

    HRESULT hr = m_pIBreakPoint->ContinueTask(CComBSTR(m_strInstance), 0);
    if(hr != S_OK)
    {
        hr = m_pIBreakPoint->ContinueTask(CComBSTR(), 0);
    }

    return (SCODE) hr;
}


SCODE CGenEdDoc::StepTask(long mode)
{
    if(m_strInstance == _T(""))
    {
        return(S_FALSE);
    }

    HRESULT hr = m_pIBreakPoint->ContinueTask(CComBSTR(m_strInstance), mode);
    if(hr != S_OK)
    {
        hr = m_pIBreakPoint->ContinueTask(CComBSTR(), mode);
    }
    return hr;
}


SCODE CGenEdDoc::ClearInstanceBreakpoints()
{
    if(m_strInstance == _T(""))
    {
        return(S_FALSE);
    }

    return(m_pIBreakPoint->ResetAllBPs(CComBSTR(m_strInstance)));
}


SCODE CGenEdDoc::ClearAllBreakpoints()
{
    return(m_pIBreakPoint->ResetAllBPs(CComBSTR("")));
}

SCODE CGenEdDoc::OnEditEvent() 
{
    CGenEdFrame*    pFrame = GetGenEdFrame();
    if(pFrame)
    {
        pFrame->ActivateClassTab();
    }
    return S_OK;
}


CGenEdFrame* CGenEdDoc::GetGenEdFrame()
{
    CGenEdFrame*    pFrame = NULL;
    CView*          pView = NULL;

    POSITION    pos = GetFirstViewPosition();

    if(pos)
    {
        pView = GetNextView(pos);
        if(pView)
        {
            pFrame = (CGenEdFrame*) pView->GetParentFrame();
        }
    }
    return pFrame;
}

