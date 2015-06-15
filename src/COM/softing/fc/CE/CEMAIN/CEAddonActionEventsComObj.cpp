
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h" 
#include "CEAddonActionEventsComObj.h"
#include "TargetSet.h"
#include "TargetOnlBase.h"
#include "CECompman.h"
#include "Resource.h"
#include "cemain.h"
#include "CEFileHandler.h"
#include "GenEdDoc.h"
#include "MainFrm.h"
#include "MsgWnd.h"

#include "utilif.h"

//----  Static Initializations:   ----------------------------------*


STDMETHODIMP CCEAddOnActionEvents::OnEnsureTargetDisconnected(BSTR sTargetId, BOOL bRebuildHint)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    HRESULT             hr = S_OK;
    CString             strTargetId(sTargetId);
    CTargetSet*         pTargetSet;
    CTargetOnlBase*     pTargetOnl;

    pTargetSet = CEGetTargetSet (_T("Project Targets"));
    ASSERT (pTargetSet != NULL);
    if(!pTargetSet)
    {
        return E_FAIL;
    }

    pTargetOnl = pTargetSet->GetTarget(strTargetId);
    if(!pTargetOnl)
    {
        // target not found --> target does not exist 
        // --> target not connected --> S_OK
        return S_OK;
    }

    CString strMessage;
    if(pTargetOnl->IsConnected() || pTargetOnl->IsConnectedExclusive())
    {
        strMessage.Format(IDS_ASK_DISCONNECT_TARGET, strTargetId);
        if(::AfxMessageBox(strMessage, MB_YESNO) == IDYES)
        {
            if(pTargetOnl->IsConnectedExclusive())
            {
                hr = pTargetOnl->DisconnectExclusive();
                if(hr != S_OK)
                {
                    return hr;
                }
            }
            if(pTargetOnl->IsConnected())
            {
                hr = pTargetOnl->Disconnect();
                if(hr != S_OK)
                {
                    return hr;
                }
            }
        }
        else
        {
            // user cancels
            return S_FALSE;
        }
    }
    if(bRebuildHint)
    {
        strMessage.Format(IDS_REBUILD_HINT, strTargetId);
        ::AfxMessageBox(strMessage);
    }

    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::OnFileRemoved(BSTR sFilePath)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    HRESULT hr = S_OK;
    CString strFilePath(sFilePath);

    // get file handler
	CCEMAINApp *pApp;
	CCEFileHandler *pFileHandler;

	pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
	ASSERT(pApp != NULL);
	if (pApp == NULL)
	{
		return(hr);
	}

	pFileHandler = pApp->GetFileHandler ();
	ASSERT(pFileHandler != NULL);
	HRESULT hrTmp = pFileHandler->CloseFile(strFilePath);

    return hr;
}

STDMETHODIMP CCEAddOnActionEvents::OnFileRenamed(BSTR sOldFilePath, BSTR sNewFilePath)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    // close old file and open new one
    HRESULT hr = S_OK;
    CString strOldFilePath(sOldFilePath);
    CString strNewFilePath(sNewFilePath);

    // get file handler
	CCEMAINApp *pApp;
	CCEFileHandler *pFileHandler;

	pApp = dynamic_cast<CCEMAINApp *> (AfxGetApp());
	ASSERT(pApp != NULL);
	if (pApp == NULL)
	{
		return(hr);
	}

	pFileHandler = pApp->GetFileHandler ();
	ASSERT(pFileHandler != NULL);
	HRESULT hrTmp = pFileHandler->CloseFile(strOldFilePath);
    if(hrTmp == S_OK)
    {
        CGenEdDoc*  pDoc;
        hrTmp = pFileHandler->ShowFile(strNewFilePath, pDoc);
    }

    return hr;
}






STDMETHODIMP CCEAddOnActionEvents::BuildAll()
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    ASSERT(pMainFrame);
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    pMainFrame->Build(eTargetAll);

    return S_OK;
}


STDMETHODIMP CCEAddOnActionEvents::BuildTarget(BSTR sTargetId)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    ASSERT(pMainFrame);
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    CString strTargetId(sTargetId);
    pMainFrame->Build(eTargetOne, strTargetId);

    return S_OK;
}


STDMETHODIMP CCEAddOnActionEvents::ShowTargetMsgTab(BSTR sTargetId)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString         strId(sTargetId);
    CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
    if(pMsgWnd)
    {
        pMsgWnd->ShowTargetWindow(strId);
    }

    return S_OK;
}


STDMETHODIMP CCEAddOnActionEvents::ShowMsgTab(enum E_FC_MsgTab msgTab)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
    if(pMsgWnd)
    {
        switch(msgTab)
        {
        case FC_MSGTAB_COMPILE:
            pMsgWnd->ShowCompileWindow();
            break;
        case FC_MSGTAB_DOWNLOAD:
            pMsgWnd->ShowDownloadWindow();
            break;
        case FC_MSGTAB_FINDINFILES:
            pMsgWnd->ShowFileFindWindow();
            break;
        case FC_MSGTAB_SYSTEM:
            pMsgWnd->ShowSystemWindow();
            break;
        }
    }

    return S_OK;
}


STDMETHODIMP CCEAddOnActionEvents::IsConnected(BSTR sTargetId, BOOL* pbStatus)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    HRESULT             hr = S_OK;
    CString             strTargetId(sTargetId);
    CTargetSet*         pTargetSet;
    CTargetOnlBase*     pTargetOnl;

    if (pbStatus == NULL)
    {
        return E_POINTER;
    }

    *pbStatus = FALSE;

    pTargetSet = CEGetTargetSet (_T("Project Targets"));
    ASSERT (pTargetSet != NULL);
    if(!pTargetSet)
    {
        return E_FAIL;
    }

    pTargetOnl = pTargetSet->GetTarget(strTargetId);
    if(!pTargetOnl)
    {
        // target not found --> target does not exist 
        // --> target not connected --> S_OK
        return S_OK;
    }

    CString strMessage;
    if(pTargetOnl->IsConnected())
    {
        *pbStatus = TRUE;
    }

    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::IsExclusiveConnected(BSTR sTargetId, BOOL* pbStatus)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    HRESULT             hr = S_OK;
    CString             strTargetId(sTargetId);
    CTargetSet*         pTargetSet;
    CTargetOnlBase*     pTargetOnl;

    if (pbStatus == NULL)
    {
        return E_POINTER;
    }

    *pbStatus = FALSE;

    pTargetSet = CEGetTargetSet (_T("Project Targets"));
    ASSERT (pTargetSet != NULL);
    if(!pTargetSet)
    {
        return E_FAIL;
    }

    pTargetOnl = pTargetSet->GetTarget(strTargetId);
    if(!pTargetOnl)
    {
        // target not found --> target does not exist 
        // --> target not connected --> S_OK
        return S_OK;
    }

    CString strMessage;
    if(pTargetOnl->IsConnectedExclusive())
    {
        *pbStatus = TRUE;
    }

    return hr;
}



STDMETHODIMP CCEAddOnActionEvents::GetResourceState(BSTR sTargetId, LONG* plStatus)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    HRESULT             hr = S_OK;
    CString             strTargetId(sTargetId);
    CTargetSet*         pTargetSet;
    CTargetOnlBase*     pTargetOnl;

    if (plStatus == NULL)
    {
        return E_POINTER;
    }

    *plStatus = FALSE;

    pTargetSet = CEGetTargetSet (_T("Project Targets"));
    ASSERT (pTargetSet != NULL);
    if(!pTargetSet)
    {
        return E_FAIL;
    }

    pTargetOnl = pTargetSet->GetTarget(strTargetId);
    if(!pTargetOnl)
    {
        // target not found --> target does not exist 
        // --> target not connected --> S_OK
        return S_OK;
    }

    CString strMessage;
    E_ResourceState eState = pTargetOnl->GetResourceState();
    *plStatus = (LONG)eState;

    return hr;
}



STDMETHODIMP CCEAddOnActionEvents::Connect(BSTR sTargetId)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());
 
    CString         strId(sTargetId);
    CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
    if(pMsgWnd)
    {
        pMsgWnd->ConnectTarget(strId);
    }

    return S_OK;
}


STDMETHODIMP CCEAddOnActionEvents::Disconnect(BSTR sTargetId)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString         strId(sTargetId);
    CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
    if(pMsgWnd)
    {
        pMsgWnd->DisconnectTarget(strId);
    }

    return S_OK;   
}


STDMETHODIMP CCEAddOnActionEvents::ConnectExclusive(BSTR sTargetId)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());
 
    CString         strId(sTargetId);
    CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
    if(pMsgWnd)
    {
        pMsgWnd->ConnectTargetExclusive(strId);
    }

    return S_OK;
}


STDMETHODIMP CCEAddOnActionEvents::DisconnectExclusive(BSTR sTargetId)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());
 
    CString         strId(sTargetId);
    CMsgWnd*        pMsgWnd = (dynamic_cast<CMsgWnd *> (CEGetDockingTool ("Message View", true)));
    if(pMsgWnd)
    {
        pMsgWnd->DisconnectTargetExclusive(strId);
    }

    return S_OK;
}


STDMETHODIMP CCEAddOnActionEvents::Download(BSTR sTargetId)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    ASSERT(pMainFrame);
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    CString strTargetId(sTargetId);
    pMainFrame->DownloadTarget(eTargetOne, CEDL_DOWNLD_ALL, 
                               strTargetId, _T(""));

    return S_OK;
}


STDMETHODIMP CCEAddOnActionEvents::Flash(BSTR sTargetId)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    ASSERT(pMainFrame);
    if (pMainFrame == NULL)
    {
        return E_FAIL;
    }

    CString strTargetId(sTargetId);
    pMainFrame->DownloadTarget(eTargetOne, CEDL_DOWNLD_ALL, 
                               strTargetId, _T(""),
                               DOWNLD_FLASH);

    return S_OK;
}


STDMETHODIMP CCEAddOnActionEvents::StartControl(BSTR sTargetId)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->StartResource();
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::StopControl(BSTR sTargetId)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->StopResource();
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::StartAllTasks(BSTR sTargetId)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->StartAllTasks();
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::StopAllTasks(BSTR sTargetId)
{
        AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->StopAllTasks();
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::ColdStart(BSTR sTargetId)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->ColdStartResource();
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::WarmStart(BSTR sTargetId)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->WarmStartResource();
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::InitClear(BSTR sTargetId)
{
AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->ClearResource(strTargetId);
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::CustomOnlCmd(BSTR sTargetId, 
                                                SAFEARRAY** ppDataDown, 
                                                SAFEARRAY** ppDataUp)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->CustomOnlCmd(ppDataDown, ppDataUp);
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::ProjectInfoUpload(BSTR sTargetId, 
                                                     BSTR* psProjVers,
	                                                 BSTR* psProjName,
	                                                 BSTR* psModified,
	                                                 BSTR* psCreated,
                                                     BSTR* psOwner,
	                                                 BSTR* psComment1,
	                                                 BSTR* psComment2,
	                                                 BSTR* psComment3)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->ProjectInfoUpload(psProjVers,
	                              psProjName,
	                              psModified,
	                              psCreated,
                                  psOwner,
	                              psComment1,
	                              psComment2,
	                              psComment3);
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::ProjectDownload(BSTR sTargetId, 
                                                   SAFEARRAY **ppDataDown)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->ProjectDownload(ppDataDown);
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::ProjectUpload(BSTR sTargetId, 
                                                 SAFEARRAY **ppDataUp)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->ProjectUpload(ppDataUp);
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::CustCmd1(BSTR sTargetId, 
                                            SAFEARRAY **ppDataDown, 
                                            SAFEARRAY **ppDataUp)
{
     AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->CustCmd1(ppDataDown, ppDataUp);
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::CustCmd2(BSTR sTargetId, 
                                            SAFEARRAY **ppDataDown, 
                                            SAFEARRAY **ppDataUp)
{
        AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->CustCmd2(ppDataDown, ppDataUp);
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::CustCmd3(BSTR sTargetId, 
                                            SAFEARRAY **ppDataDown, 
                                            SAFEARRAY **ppDataUp)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->CustCmd3(ppDataDown, ppDataUp);
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::CustCmd4(BSTR sTargetId, 
                                            SAFEARRAY **ppDataDown, 
                                            SAFEARRAY **ppDataUp)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->CustCmd4(ppDataDown, ppDataUp);
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::CustCmd5(BSTR sTargetId, 
                                            SAFEARRAY **ppDataDown, 
                                            SAFEARRAY **ppDataUp)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->CustCmd5(ppDataDown, ppDataUp);
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::LoadFile(BSTR sTargetId,
                                            BSTR sName, 
                                            SAFEARRAY **ppData)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->LoadFile(sName, ppData);
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::SaveFile(BSTR sTargetId,
                                            BSTR sName, 
                                            SAFEARRAY **ppData)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->SaveFile(sName, ppData);
    
    return hr;
}


STDMETHODIMP CCEAddOnActionEvents::Dir(BSTR sTargetId,
                                       SAFEARRAY **ppFileNames)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState());

    CString strTargetId(sTargetId);

    HRESULT hr;

    CTargetSet* pTargetSet = CEGetTargetSet("Project Targets", false);
    if(!pTargetSet)
    {
        E_FAIL;
    }

    CTargetOnlBase* pTarget = NULL;

    pTarget = pTargetSet->GetTarget(strTargetId);
    if(!pTarget)
    {
        return E_FAIL;
    }

    CTargetOnl* pTarg = dynamic_cast<CTargetOnl*> (pTarget);
    if(!pTarg)
    {
        return E_FAIL;
    }

    hr = pTarg->Dir(ppFileNames);
    
    return hr;
}
