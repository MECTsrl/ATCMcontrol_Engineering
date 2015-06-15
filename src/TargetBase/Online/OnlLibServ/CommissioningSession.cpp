/* $Header: /4CReleased/V2.20.00/TargetBase/Online/OnlLibServ/CommissioningSession.cpp 4     19.04.07 15:45 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CommissioningSession.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/Online/OnlLibServ/CommissioningSession.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 19.04.07 15:45 $
 *			 $Revision: 4 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "Security.h"
#include "opcItem.h"
#include "SysVar.h"
#include "SysVarPool.h"
#include "iecResource.h"
#include "CommissioningMgt.h"
#include "CommissioningSession.h"
#include "NSpaceHolder.h"
#include "CSC_OnlineErr.h"
#include "OPCError.h"

#include "VisTransferLocal.h"
#include "config.h"
#include "connection.h"

#include "ControlMgt.h"

#define	  F4C_FLASH_FILE			_T("_flash.f4c")


/////////////////////////////////////////////////////////////////////////////
// CCommissioningSession

//------------------------------------------------------------------*
CCommissioningSession::CCommissioningSession()
  : m_lSessionKey(0),
    m_bCtrlDebugSessionOn(FALSE),
    m_bFlashResource(FALSE),
    m_pResource(NULL),
    m_pVisTransfer(NULL),
	m_bIsFullDownload(FALSE),
	m_pOCHlp(NULL)
{
}

//------------------------------------------------------------------*
CCommissioningSession::~CCommissioningSession()
{
    CloseControl();
    CloseConsole();
	if(m_pOCHlp)
		delete m_pOCHlp;
}



//=======================================================================
//      interface   ITransfer
//=======================================================================

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::StartOnlineChange(long lKey, CSC_TRANSFER_TARGET target)
{
    TrcPrint(TRC_INTERFACE, _T("StartOnlineChange for target %s\n"), target == ProgramDomains ? _T("Prog") : _T("Vis"));

    BOOL bHandled = FALSE;
	m_bIsFullDownload = FALSE;
	if(!m_pOCHlp)
		m_pOCHlp = new COnlineChangesHelper;

	HRESULT hr = Adapt_StartOnlineChange(lKey, target, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }
    switch (target) {
    case ProgramDomains:
        if (!m_pResource) {
            return E_CSC_NO_CONNECT_TO_CONTROL;
        }

        //check if connection available...
        if (!myConnection.IsConnected())
            return E_CSC_NO_CONNECT_TO_CONTROL;

        m_bFlashResource = FALSE;

		{
			CString filename = GetConfigPath() + pszOnlineDomainName;
			_tremove(filename);
		}

        hr = m_pResource->StartOnlineChange();
        break;
    case VisualDomains:
        if (!m_pVisTransfer) {
            return E_CSC_NO_FTP_SESSION;
        }
        hr = m_pVisTransfer->StartOnlineChange();
        break;
    default:
        return E_FAIL;
        break;
    }

	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::CommitOnlineChange(long lKey, CSC_TRANSFER_TARGET target, BSTR sResource, BSTR sProjectName, BSTR sProjectGUID, BSTR sDownloadGUID)
{
    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][CommitOnlineChange] For target %s\n"), target == ProgramDomains ? _T("Prog") : _T("Vis"));

    BOOL bHandled = FALSE;

	HRESULT hr = Adapt_CommitOnlineChange(lKey, target, sResource, sProjectName, sProjectGUID, sDownloadGUID, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) 
	{
	    TrcPrint(TRC_ERROR, _T("[CCommissioningSession][CommitOnlineChange] Adapt_CommitOnlineChange() failed (0x%04x)\n"), hr);
		return hr;
	}

    hr = CheckKey(lKey);
    if (FAILED(hr)) 
	{
	    TrcPrint(TRC_ERROR, _T("[CCommissioningSession][CommitOnlineChange] CheckKey() failed (0x%04x)\n"), hr);
        return hr;
    }
    switch (target) 
	{
    case ProgramDomains:
        if (!m_pResource)
        {
		    TrcPrint(TRC_ERROR, _T("[CCommissioningSession][CommitOnlineChange] Null Pointer\n"));
            return E_CSC_NO_CONNECT_TO_CONTROL;
        }
        hr = m_pResource->CommitOnlineChange();
        if (SUCCEEDED(hr)) 
		{
            CSC_InitHashTables();
            hr = DownloadToControl(sDownloadGUID);

			if (FAILED(hr))
			{
			    TrcPrint(TRC_ERROR, _T("[CCommissioningSession][CommitOnlineChange] DownloadToControl() failed (0x%04x)\n"), hr);
			}	
        }
		else
		{
			TrcPrint(TRC_ERROR, _T("[CCommissioningSession][CommitOnlineChange] m_pResource->CommitOnlineChange() failed (0x%04x)\n"), hr);
		}
        break;
    case VisualDomains:
        if (!m_pVisTransfer) 
		{
		    TrcPrint(TRC_ERROR, _T("[CCommissioningSession][CommitOnlineChange] Null Pointer\n"));
            return E_CSC_NO_FTP_SESSION;
        }
        hr = m_pVisTransfer->CommitOnlineChange();
		if (FAILED(hr))
		{
			TrcPrint(TRC_ERROR, _T("[CCommissioningSession][CommitOnlineChange] m_pVisTransfer->CommitOnlineChange() failed (0x%04x)\n"), hr);
		}	
        break;
    default:
        return E_FAIL;
        break;
    }
    if (SUCCEEDED(hr)) 
	{
        UpdateSystemVariables(target, FALSE, CString(sResource), CString(sProjectName), CString(sProjectGUID), CString(sDownloadGUID));
		if (FAILED(hr))
		{
			TrcPrint(TRC_ERROR, _T("[CCommissioningSession][CommitOnlineChange] UpdateSystemVariables() failed (0x%04x)\n"), hr);
		}	
    }

	if(m_pOCHlp)
	{
		delete m_pOCHlp;
		m_pOCHlp = NULL;
	}

	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::CancelOnlineChange(long lKey, CSC_TRANSFER_TARGET target)
{
    TrcPrint(TRC_INTERFACE, _T("CancelOnlineChange for target %s\n"), target == ProgramDomains ? _T("Prog") : _T("Vis"));

    BOOL bHandled = FALSE;

	HRESULT hr = Adapt_CancelOnlineChange(lKey, target, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }
    switch (target) {
    case ProgramDomains:
        if (!m_pResource) {
            return E_CSC_NO_CONNECT_TO_CONTROL;
        }
        hr = m_pResource->CancelOnlineChange();
        break;
    case VisualDomains:
        if (!m_pVisTransfer) {
            return E_CSC_NO_FTP_SESSION;
        }
        hr = m_pVisTransfer->CancelOnlineChange();
        break;
    default:
        return E_FAIL;
        break;
    }

	if(m_pOCHlp)
	{
		delete m_pOCHlp;
		m_pOCHlp = NULL;
	}

	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::DownloadDomain(long lKey, CSC_TRANSFER_TARGET target,
                                     BSTR sDomainName, SAFEARRAY /*(unsigned char)*/ **ppContent)
{
    TrcPrint(TRC_INTERFACE, _T("DownloadDomain for target %s, domain='%s'\n"),
        target == ProgramDomains ? _T("Prog") : _T("Vis"), (LPCTSTR)CString(sDomainName));

    BOOL bHandled = FALSE;

	HRESULT hr = Adapt_DownloadDomain(lKey, target, sDomainName, ppContent, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }
    switch (target) {
    case ProgramDomains:
        if (!m_pResource) {
            return E_CSC_NO_CONNECT_TO_CONTROL;
        }
		if (!m_bIsFullDownload)
		{
			CString strDomain = sDomainName;
		    CString strType, strFile;
			DWORD   size = 1;

			if(!m_pOCHlp->isOCConfigInit())
				m_pOCHlp->startOC();
			
			hr = m_pOCHlp->processFileOC(strDomain, ppContent, strFile, &size);
			if (FAILED(hr)) {
				return hr;
			}

			if(size > 0 && CSC_GetFileType(strDomain, strType))
			{
				CString filename = GetConfigPath() + pszOnlineDomainName;
				FILE* pFile = _tfopen(filename, "a");
				if (FAILED(hr)) {
					FC_DEBUGPRINT1(_T("CSVPARS> file not found: %s\n"), filename);
					TrcPrint(TRC_ERROR, _T("CSVPARS> file not found: %s\n"), filename);
				}
				else
				{
					_ftprintf(pFile, _T("%s;%s\n"), strType, strFile);
					fclose(pFile);
				}
			}
		}

        hr = m_pResource->DownloadDomain(sDomainName, ppContent);
        break;
    case VisualDomains:
        if (!m_pVisTransfer) {
            return E_CSC_NO_FTP_SESSION;
        }
        hr = m_pVisTransfer->DownloadDomain(sDomainName, ppContent);
        break;
    default:
        return E_FAIL;
        break;
    }

	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::UploadDomain(long lKey, CSC_TRANSFER_TARGET target,
                                   BSTR sDomainName, SAFEARRAY /*(unsigned char)*/ **ppContent)
{
    TrcPrint(TRC_INTERFACE, _T("UploadDomain for target %s, domain='%s'\n"),
        target == ProgramDomains ? _T("Prog") : _T("Vis"), (LPCTSTR)CString(sDomainName));

    BOOL bHandled = FALSE;

	HRESULT hr = Adapt_UploadDomain(lKey, target, sDomainName, ppContent, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }
    switch (target) {
    case ProgramDomains:
        if (!m_pResource) {
            return E_CSC_NO_CONNECT_TO_CONTROL;
        }
        hr = m_pResource->UploadDomain(sDomainName, ppContent);
        break;
    case VisualDomains:
        if (!m_pVisTransfer) {
            return E_CSC_NO_FTP_SESSION;
        }
        hr = m_pVisTransfer->UploadDomain(sDomainName, ppContent);
        break;
    default:
        return E_FAIL;
        break;
    }

	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::DeleteAllDomains(long lKey, CSC_TRANSFER_TARGET target)
{
    TrcPrint(TRC_INTERFACE, _T("DeleteAllDomains for target %s\n"),
        target == ProgramDomains ? _T("Prog") : _T("Vis"));

    BOOL bHandled = FALSE;
	m_bIsFullDownload = TRUE;

	HRESULT hr = Adapt_DeleteAllDomains(lKey, target, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }
    switch (target) {
    case ProgramDomains:
        if (!m_pResource) {
            return E_CSC_NO_CONNECT_TO_CONTROL;
        }
        hr = m_pResource->DeleteAllDomains();
        break;
    case VisualDomains:
        if (!m_pVisTransfer) {
            return E_CSC_NO_FTP_SESSION;
        }
        hr = m_pVisTransfer->DeleteAllDomains();
        break;
    default:
        return E_FAIL;
        break;
    }

	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::DeleteDomain(long lKey, CSC_TRANSFER_TARGET target, BSTR sDomainName)
{
    TrcPrint(TRC_INTERFACE, _T("DeleteDomain for target %s, domain='%s'\n"),
        target == ProgramDomains ? _T("Prog") : _T("Vis"), (LPCTSTR)CString(sDomainName));

    BOOL bHandled = FALSE;

	HRESULT hr = Adapt_DeleteDomain(lKey, target, sDomainName, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }
    switch (target) {
    case ProgramDomains:
        if (!m_pResource) {
            return E_CSC_NO_CONNECT_TO_CONTROL;
        }
        hr = m_pResource->DeleteDomain(sDomainName);
        break;
    case VisualDomains:
        if (!m_pVisTransfer) {
            return E_CSC_NO_FTP_SESSION;
        }
        hr = m_pVisTransfer->DeleteDomain(sDomainName);
        break;
    default:
        return E_FAIL;
        break;
    }

	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::GetDomainList(long lKey, CSC_TRANSFER_TARGET target, SAFEARRAY /*( BSTR )*/ **ppListDomainNames)
{
    TrcPrint(TRC_INTERFACE, _T("GetDomainList for target %s\n"),
        target == ProgramDomains ? _T("Prog") : _T("Vis"));

    BOOL bHandled = FALSE;

	HRESULT hr = Adapt_GetDomainList(lKey, target, ppListDomainNames, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }
    switch (target) {
    case ProgramDomains:
        if (!m_pResource) {
            return E_CSC_NO_CONNECT_TO_CONTROL;
        }
        hr = m_pResource->GetDomainList(ppListDomainNames);
        break;
    case VisualDomains:
        if (!m_pVisTransfer) {
            return E_CSC_NO_FTP_SESSION;
        }
        hr = m_pVisTransfer->GetDomainList(ppListDomainNames);
        break;
    default:
        return E_FAIL;
        break;
    }


	return hr;
}


//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::get_DLoadAttribute(long lKey, CSC_TRANSFER_TARGET target,
                                         BSTR sAtributeName, VARIANT *pVal)
{
    CString strAttr = sAtributeName;
    TrcPrint(TRC_INTERFACE, _T("get_DLoadAttribute for target %s, name='%s'\n"),
        target == ProgramDomains ? _T("Prog") : _T("Vis"), (LPCTSTR) strAttr);

    BOOL bHandled = FALSE;

	HRESULT hr = Adapt_get_DLoadAttribute(lKey, target, sAtributeName, pVal, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }
    if (pVal == NULL) {
        return E_POINTER;
    }

    CSysVarPool *pPool = NULL;
    hr = OPC_E_UNKNOWNITEMID;
    // lookup program/control variables:
    if (target == ProgramDomains)
    {
        // global system variables:
        pPool = (CSysVarPool*) NameSpaceHolder::GetGlobalSysVars();
        if (pPool != NULL)
        {
            // lookup in project and resource names
            GenericPointer<CSysVar> pVar = pPool->LookupSysVar(strAttr);
            pPool->release();
            if (!!pVar)
            {
                GenericPointer<GenericCache> pCache = pVar->getCache();
                if (!pCache->getValue(pVal)) {
                    hr = E_FAIL;
                }
                else {
                    hr = S_OK;
                }
            }
        }
        
        // not found yet, try in resource sys vars:
        if (FAILED(hr))
        {
            pPool = (CSysVarPool*) NameSpaceHolder::GetResourceDLoadSysVars(0);
            if (pPool != NULL)
            {
                // lookup variable name:
                GenericPointer<CSysVar> pVar = pPool->LookupSysVar(strAttr);
                pPool->release();
                if (!!pVar)
                {
                    GenericPointer<GenericCache> pCache = pVar->getCache();
                    if (!pCache->getValue(pVal)) {
                        hr = E_FAIL;
                    }
                    else {
                        hr = S_OK;
                    }
                }
            }
        }
    }

    // lookup console variables :
    if (target == VisualDomains)
    {
        // console system variable
        pPool = (CSysVarPool*) NameSpaceHolder::GetConsoleDLoadSysVars();
        if (pPool != NULL)
        {
            // lookup variable name:
            GenericPointer<CSysVar> pVar = pPool->LookupSysVar(strAttr);
            pPool->release();
            if (!!pVar)
            {
                GenericPointer<GenericCache> pCache = pVar->getCache();
                if (!pCache->getValue(pVal)) {
                    hr = E_FAIL;
                }
                else {
                    hr = S_OK;
                }
            }
        }
    }

	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::ClearResource(long lKey, CSC_TRANSFER_TARGET target)
{
    TrcPrint(TRC_INTERFACE, _T("ClearResource for target %s\n"),
        target == ProgramDomains ? _T("Prog") : _T("Vis"));

    BOOL bHandled = FALSE;

	HRESULT hr = Adapt_ClearResource(lKey, target, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }
    switch (target) {
    case ProgramDomains:
        if (!m_pResource) {
            return E_CSC_NO_CONNECT_TO_CONTROL;
        }
		if (CConnection::GetFirmware() < 22000) {
			// to clear control, we start a online change, delete all domains
			// and then commit this empty configuration:
			hr = m_pResource->StartOnlineChange();
			if (FAILED(hr)) {
				return hr;
			}
			hr = m_pResource->DeleteAllDomains();
			if (FAILED(hr)) {
				return hr;
			}
			hr = m_pResource->CommitOnlineChange();
		}
        if (SUCCEEDED(hr)) 
		{
			if (CConnection::GetFirmware() < 22000) {
				CSC_InitHashTables();
			}
            
			if (CConnection::GetFirmware() < 22000)
			{
				hr = myConnection.SendCommand(FALSE, CMD_INITIALIZE_CLEAR, NULL, 0);
			}
			else
			{
				hr = PollControl(FALSE, 50, CMD_INITIALIZE_CLEAR);
			}

            ibnConfigurationChanged();
        }
        break;
    case VisualDomains:
        if (!m_pVisTransfer) {
            return E_CSC_NO_FTP_SESSION;
        }
        hr = m_pVisTransfer->DeleteAllDomains();
        break;
    default:
        return E_FAIL;
        break;
    }

	if (CConnection::GetFirmware() < 22000) {
		if (SUCCEEDED(hr)) {
			// update DownloadGUIDs of control or console
			GUID guid;
			CString strNullGUID;
			::memset(&guid, 0, sizeof(GUID));
			LPOLESTR poszGUID = NULL;
			if (SUCCEEDED(StringFromCLSID(guid, &poszGUID))) {
				strNullGUID = poszGUID;
				CoTaskMemFree(poszGUID);
			}
			UpdateSystemVariables(target, TRUE, "", "", strNullGUID, strNullGUID);
		}
	}
	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::FlashResource(long lKey)
{
    TrcPrint(TRC_INTERFACE, _T("FlashResource\n"));

    BOOL bHandled = FALSE;

	HRESULT hr = Adapt_FlashResource(lKey, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }
    if (!m_pResource) {
        return E_CSC_NO_CONNECT_TO_CONTROL;
    }

    m_bFlashResource = TRUE;

	if (CConnection::GetFirmware() >= 21000)
	{
		hr = DL_ClearFlash();
		if (FAILED(hr)) {
			return hr;
		}

		hr = DL_Flash();
		if (FAILED(hr)) {
			return hr;
		}
	}

	return S_OK;
}







//=======================================================================
//      interface   IBreakpoint
//=======================================================================
//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::SetBreakpoint(long lKey, BSTR sInstanceName, long lBreakpointNumber, long *phBP)
{
    TrcPrint(TRC_INTERFACE, _T("SetBreakpoint for instance %s, BP#=%d\n"), (LPCTSTR)CString(sInstanceName), lBreakpointNumber);

	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_SetBreakpoint(lKey, sInstanceName, lBreakpointNumber, phBP, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    if (phBP == NULL) {
		return E_POINTER;
    }
    *phBP = 0;

    hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }
    if (!m_pResource) {
        return E_CSC_NO_CONNECT_TO_CONTROL;
    }
    if (!m_bCtrlDebugSessionOn) {
        return E_CSC_NO_DEBUG_SESSION_TO_CONTROL;  
    }

    // strip configuration- and resource name from instance name!
    CString strName = sInstanceName;
    CString strStripped;

    LPTSTR pszSplit = _tcstok((LPTSTR)(LPCTSTR) strName, NameSpaceHolder::sm_pszSeparatorList);
    if (pszSplit != NULL)
    {
        m_strConfigName = pszSplit;

        // have config, go past resource
        pszSplit = _tcstok(NULL, NameSpaceHolder::sm_pszSeparatorList);
        if (pszSplit != NULL)
        {
            m_strResourceName = pszSplit;

            // have resource, remainder must be task name!
            pszSplit = _tcstok(NULL, _T(""));
            if (pszSplit != NULL) {
                strStripped = pszSplit;
            }
        }
    }
    
	strStripped.MakeLower();
    CSC_BREAKPOINT_INFO info;
    hr = E_FAIL;

    if (CSC_LookupBreakpoint(strStripped, lBreakpointNumber, info)) 
	{
		UINT uLen	= sizeof(XBreakpoint);
		BYTE *pData = NULL;

		HEAP_ALLOC(pData, uLen);

		XBreakpoint *pdescr = (XBreakpoint *)pData;
		pdescr->uCode     = TRANSLATE_SHORT((IEC_UINT)info.cls_id);
		pdescr->uInst     = TRANSLATE_SHORT((IEC_UINT)info.obj   );
		pdescr->ulCodePos = TRANSLATE_LONG (          info.offset);
		pdescr->uType     = TRANSLATE_SHORT((IEC_UINT)info.type  );

		hr = myConnection.TransferData(TRUE, CMD_SET_BREAKPOINT, &pData, &uLen);

		if (uLen < sizeof(IEC_UDINT))
		{
			hr = E_FAIL;
		}

		if (SUCCEEDED(hr))
		{
			*phBP = TRANSLATE_LONG(*(IEC_UDINT *)pData);
		}

		HEAP_FREE(pData, uLen);
    }

	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::ClearBreakpoint(long lKey, long hBP)
{
    TrcPrint(TRC_INTERFACE, _T("ClearBreakpoint handle=0x%08X\n"), hBP);

	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_ClearBreakpoint(lKey, hBP, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }
    if (!m_pResource) {
        return E_CSC_NO_CONNECT_TO_CONTROL;
    }
    if (!m_bCtrlDebugSessionOn) {
        return E_CSC_NO_DEBUG_SESSION_TO_CONTROL;  
    }

	hBP = TRANSLATE_LONG(hBP);

	return myConnection.SendCommand(TRUE, CMD_CLEAR_BREAKPOINT, (BYTE *)&hBP, sizeof(hBP));
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::ClearAllBreakpoints(long lKey)
{
    TrcPrint(TRC_INTERFACE, _T("ClearAllBreakpoints\n"));

	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_ClearAllBreakpoints(lKey, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }
    if (!m_pResource) {
        return E_CSC_NO_CONNECT_TO_CONTROL;
    }
    if (!m_bCtrlDebugSessionOn) {
        return E_CSC_NO_DEBUG_SESSION_TO_CONTROL;  
    }

	return myConnection.SendCommand(TRUE, CMD_CLEAR_ALL_BREAKPOINTS, NULL, 0);
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::ContinueTask(long lKey, BSTR sTaskName)
{
    TrcPrint(TRC_INTERFACE, _T("ContinueTask for task='%s'\n"), (LPCTSTR)CString(sTaskName));

	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_ContinueTask(lKey, sTaskName, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }
    if (!m_pResource) {
        return E_CSC_NO_CONNECT_TO_CONTROL;
    }
    if (!m_bCtrlDebugSessionOn) {
        return E_CSC_NO_DEBUG_SESSION_TO_CONTROL;  
    }
    // QI for breakpoint manager!
    // strip configuration- and resource name from task name!
    CString strName = sTaskName;
    CString strStripped;
    
    LPTSTR pszSplit = _tcstok((LPTSTR)(LPCTSTR) strName, NameSpaceHolder::sm_pszSeparatorList);
    if (pszSplit != NULL)
    {
        // have config, go past resource
        pszSplit = _tcstok(NULL, NameSpaceHolder::sm_pszSeparatorList);
        if (pszSplit != NULL)
        {
            // have resource, remainder must be task name!
            pszSplit = _tcstok(NULL, _T(""));
            if (pszSplit != NULL) {
                strStripped = pszSplit;
            }
        }
    }

    strStripped.MakeLower();
    long taskid = 0;
    
	if (CSC_LookupTask(strStripped, taskid))
	{
		IEC_UINT uTask = TRANSLATE_SHORT((IEC_UINT)taskid);

		hr = myConnection.SendCommand(TRUE, CMD_CONTINUE, (BYTE *)&uTask, sizeof(uTask));

		if (SUCCEEDED(hr)) {
			hr = myConnection.CheckMessages();
        }
    }


	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::SingleStep(long lKey, BSTR sTaskName)
{
    TrcPrint(TRC_INTERFACE, _T("SingleStep for task='%s'\n"), (LPCTSTR)CString(sTaskName));
    
	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_SingleStep(lKey, sTaskName, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

	hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }
    if (!m_pResource) {
        return E_CSC_NO_CONNECT_TO_CONTROL;
    }
    if (!m_bCtrlDebugSessionOn) {
        return E_CSC_NO_DEBUG_SESSION_TO_CONTROL;  
    }

    // strip configuration- and resource name from task name!
    CString strName = sTaskName;
    CString strStripped;
    
    LPTSTR pszSplit = _tcstok((LPTSTR)(LPCTSTR) strName, NameSpaceHolder::sm_pszSeparatorList);
    if (pszSplit != NULL)
    {
        // have config, go past resource
        pszSplit = _tcstok(NULL, NameSpaceHolder::sm_pszSeparatorList);
        if (pszSplit != NULL)
        {
            // have resource, remainder must be task name!
            pszSplit = _tcstok(NULL, _T(""));
            if (pszSplit != NULL) {
                strStripped = pszSplit;
            }
        }
    }

    strStripped.MakeLower();
    long taskid = 0;
    
	if (CSC_LookupTask(strStripped, taskid))
	{
		IEC_UINT uTask = TRANSLATE_SHORT((IEC_UINT)taskid);

		hr = myConnection.SendCommand(TRUE, CMD_SINGLE_STEP, (BYTE *)&uTask, sizeof(uTask));

		if (SUCCEEDED(hr)) {
			hr = myConnection.CheckMessages();
        }
    }


	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::StepOut(long lKey, BSTR sTaskName)
{
    TrcPrint(TRC_INTERFACE, _T("StepOut for task='%s'\n"), (LPCTSTR)CString(sTaskName));

	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_StepOut(lKey, sTaskName, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }
    if (!m_pResource) {
        return E_CSC_NO_CONNECT_TO_CONTROL;
    }
    if (!m_bCtrlDebugSessionOn) {
        return E_CSC_NO_DEBUG_SESSION_TO_CONTROL;  
    }

    //27.11.06 EF: copied the following lines from SingleStep: 
    //as long as we don't support StepOut/Over treat as SingleStep
    // strip configuration- and resource name from task name!
    CString strName = sTaskName;
    CString strStripped;
    
    LPTSTR pszSplit = _tcstok((LPTSTR)(LPCTSTR) strName, NameSpaceHolder::sm_pszSeparatorList);
    if (pszSplit != NULL)
    {
        // have config, go past resource
        pszSplit = _tcstok(NULL, NameSpaceHolder::sm_pszSeparatorList);
        if (pszSplit != NULL)
        {
            // have resource, remainder must be task name!
            pszSplit = _tcstok(NULL, _T(""));
            if (pszSplit != NULL) {
                strStripped = pszSplit;
            }
        }
    }

    strStripped.MakeLower();
    long taskid = 0;
    
	if (CSC_LookupTask(strStripped, taskid))
	{
		IEC_UINT uTask = TRANSLATE_SHORT((IEC_UINT)taskid);

		hr = myConnection.SendCommand(TRUE, CMD_SINGLE_STEP, (BYTE *)&uTask, sizeof(uTask));

		if (SUCCEEDED(hr)) {
			hr = myConnection.CheckMessages();
        }
    }

	return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::StepOver(long lKey, BSTR sTaskName)
{
    TrcPrint(TRC_INTERFACE, _T("StepOver for task='%s'\n"), (LPCTSTR)CString(sTaskName));

	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_StepOver(lKey, sTaskName, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

    hr = CheckKey(lKey);
    if (FAILED(hr)) {
        return hr;
    }
    if (!m_pResource) {
        return E_CSC_NO_CONNECT_TO_CONTROL;
    }
    if (!m_bCtrlDebugSessionOn) {
        return E_CSC_NO_DEBUG_SESSION_TO_CONTROL;  
    }

    //27.11.06 EF: copied the following lines from SingleStep: 
    //as long as we don't support StepOut/Over treat as SingleStep
    // strip configuration- and resource name from task name!
    CString strName = sTaskName;
    CString strStripped;
    
    LPTSTR pszSplit = _tcstok((LPTSTR)(LPCTSTR) strName, NameSpaceHolder::sm_pszSeparatorList);
    if (pszSplit != NULL)
    {
        // have config, go past resource
        pszSplit = _tcstok(NULL, NameSpaceHolder::sm_pszSeparatorList);
        if (pszSplit != NULL)
        {
            // have resource, remainder must be task name!
            pszSplit = _tcstok(NULL, _T(""));
            if (pszSplit != NULL) {
                strStripped = pszSplit;
            }
        }
    }

    strStripped.MakeLower();
    long taskid = 0;
    
	if (CSC_LookupTask(strStripped, taskid))
	{
		IEC_UINT uTask = TRANSLATE_SHORT((IEC_UINT)taskid);

		hr = myConnection.SendCommand(TRUE, CMD_SINGLE_STEP, (BYTE *)&uTask, sizeof(uTask));

		if (SUCCEEDED(hr)) {
			hr = myConnection.CheckMessages();
        }
    }

	return hr;
}

//=======================================================================
//      interface   IBreakpointNotify (sink!)
//=======================================================================
//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::ibnBreakPointNotify(BSTR sTaskName, LONG bpQuality, BSTR sInstanceName, LONG lBreakpointNumber)
{
    if (m_lSessionKey == 0) {
        return E_FAIL; //already disconnected!
    }

    // must complete instance name to full path
    if (m_strResourceName.IsEmpty()) {
        TrcPrint(TRC_ERROR, _T("ibnBreakPointNotify: empty resource name!\n"));
    }
    CString strPath;
    strPath.Format(_T("%s%c%s%c"), (LPCTSTR)m_strConfigName, NameSpaceHolder::sm_cStdSeparatorChar, (LPCTSTR)m_strResourceName, NameSpaceHolder::sm_cStdSeparatorChar);
    CString strFullInstance = strPath;
    strFullInstance += sInstanceName;
    CString strFullTask = strPath;
    strFullTask += sTaskName;

    TrcPrint(TRC_INTERFACE, _T("ibnBreakPointNotify: task='%s', qual=%d, inst='%s', BP#=%d\n"),
        (LPCTSTR)CString(sTaskName), bpQuality, (LPCTSTR)strFullInstance, lBreakpointNumber);
    LONG index = 0;
    SAFEARRAY *arrsInstances = SafeArrayCreateVector(VT_BSTR, 0, 1);
    SafeArrayPutElement(arrsInstances, &index, CComBSTR(strFullInstance));
    SAFEARRAY *arrlBreakpointNumber = SafeArrayCreateVector(VT_I4, 0, 1);
    SafeArrayPutElement(arrlBreakpointNumber, &index, &lBreakpointNumber);
    Fire_OnBreakpointReached(CComBSTR(strFullTask), bpQuality, arrsInstances, arrlBreakpointNumber);
    SafeArrayDestroy(arrsInstances);
    SafeArrayDestroy(arrlBreakpointNumber);
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::ibnConfigurationChanged()
{
    if (m_lSessionKey == 0) {
        return E_FAIL; //already disconnected!
    }

    TrcPrint(TRC_INTERFACE, _T("ibnConfigurationChanged\n"));
    Fire_OnConfigurationChanged();
    return S_OK;
}


//=======================================================================
//      interface   IConnection
//=======================================================================
STDMETHODIMP
    CCommissioningSession::CheckConnection()
{
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::DropSession()
{
	Disconnect();
    CoDisconnectObject(GetUnknown(), 0);
    return S_OK;
}

//------------------------------------------------------------------*
STDMETHODIMP
    CCommissioningSession::CheckSession()
{
    if (m_pResource == NULL)
        return E_CSC_NO_CONNECT_TO_CONTROL;
    return S_OK;
}

//=======================================================================
//      helpers
//=======================================================================

//------------------------------------------------------------------*
HRESULT
    CCommissioningSession::Connect()
{
    HRESULT hr;
    CSingleLock guard(&m_cs, TRUE);

    CloseControl();
    hr = ConnectToControlResource();
    if (FAILED(hr)) {
        return hr;
    }
    hr = ConnectToControlService();
    if (FAILED(hr)) {
        return hr;
    }

/*
    hr = ConnectToConsole();
    if (FAILED(hr)) {
        CloseControl();
        return hr;
    }
*/

    // set resource name
    m_strResourceName.Empty();
    m_strConfigName.Empty();

    // also connect as debug session to control!
    // QI for breakpoint manager!

	// BP's are written by the Eng. automatically - don't request 
	// syncronisation from Engineering. (Must be done before
	// SetDebugSession() because of parallel threads!
	//
	myConnection.RequestBPSync(FALSE);
    myConnection.SetDebugSession(TRUE);
	
    
	m_bCtrlDebugSessionOn = TRUE;
    
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT
    CCommissioningSession::Disconnect()
{
    CSingleLock guard(&m_cs, TRUE);
    HRESULT hr = S_OK;

    // also disconnect debug session from control!

    myConnection.SetDebugSession(FALSE);

    m_bCtrlDebugSessionOn = FALSE;
    m_lSessionKey = 0;

    return hr;
}

//------------------------------------------------------------------*
HRESULT
    CCommissioningSession::ConnectToControlResource()
{
    HRESULT hr;
    if (m_pResource != NULL) {
        hr = m_pResource->Close();
        delete m_pResource;
    }

    assert(g_StartParam != NULL);
    CString config = _T("\\config\\_");
    config += g_StartParam;
    config += _T("_\\");
    m_pResource = new CVisTransferLocal(config);
    if (m_pResource == NULL) {
        return E_OUTOFMEMORY;
    }
    hr = m_pResource->Connect();
    return hr;
}

//------------------------------------------------------------------*
HRESULT
    CCommissioningSession::ConnectToControlService()
{
	return S_OK;
}

//------------------------------------------------------------------*
HRESULT
    CCommissioningSession::ConnectToConsole()
{
    HRESULT hr;
    if (m_pVisTransfer != NULL) {
        hr = m_pVisTransfer->Close();
        delete m_pVisTransfer;
    }

    m_pVisTransfer = new CVisTransferLocal(_T("\\config.vis\\"));

    if (m_pVisTransfer == NULL) {
        return E_OUTOFMEMORY;
    }
    hr = m_pVisTransfer->Connect();
    return hr;
}



//------------------------------------------------------------------*
HRESULT
    CCommissioningSession::CloseControl()
{
    HRESULT hr = S_OK;
    if (m_pResource != NULL) {
        hr = m_pResource->Close();
        delete m_pResource;
        m_pResource = NULL;
    }
    return hr;
}

//------------------------------------------------------------------*
HRESULT
    CCommissioningSession::CloseConsole()
{
    HRESULT hr = S_OK;
    if (m_pVisTransfer != NULL) {
        hr = m_pVisTransfer->Close();
        delete m_pVisTransfer;
        m_pVisTransfer = NULL;
    }
    return hr;
}


//------------------------------------------------------------------*
HRESULT
    CCommissioningSession::CheckKey(long lKey)
{
    if (m_lSessionKey == 0) {
        return E_CSC_NO_CONNECT_TO_CONTROL;
    }
    if (m_lSessionKey != lKey) {
        return E_CSC_TRANSFER_SINGLE_DOWNLOADER_ALLOWED;
    }
    return S_OK;
}


//------------------------------------------------------------------*
HRESULT
    CCommissioningSession::UpdateSystemVariables(CSC_TRANSFER_TARGET target, BOOL bClearedResource, LPCTSTR pszResource, LPCTSTR pszProjectName, LPCTSTR pszProjectGUID, LPCTSTR pszDownloadGUID)
{
    HRESULT hr;
    CSysVarPool *pPool = NULL;

    FILETIME    now;
    ::CoFileTimeNow(&now);
    SYSTEMTIME sysNow;
    ::FileTimeToSystemTime(&now, &sysNow);
    DATE    dtNow;
    ::SystemTimeToVariantTime(&sysNow, &dtNow);
    CComVariant vtNow = dtNow;
    vtNow.vt = VT_DATE;

    LPOLESTR pszBuf = ::SysAllocStringLen(NULL, _MAX_PATH);
    if (pszBuf == NULL) {
        return E_OUTOFMEMORY;
    }
    CString strNow;
    hr = ::BL_VariantToString(&vtNow, BL_DT, L"", 0, pszBuf, _MAX_PATH);
    if (SUCCEEDED(hr)) {
        // create copy with correct length set !
        strNow = pszBuf;
        ::SysFreeString(pszBuf);
    }
    
    // update variables on commit of program domains:
    if (target == ProgramDomains)
    {
        // global system variables:
        pPool = (CSysVarPool*) NameSpaceHolder::GetGlobalSysVars();
        if (pPool != NULL)
        {
            // set project and resource names
            // get resource name:
            CString strResName = pszResource;
            strResName.MakeLower();
            // get project name:
            CString strPrjName = pszProjectName;
            hr = pPool->UpdateValue(g_csSVarProjectName, strPrjName);
            hr = pPool->UpdateValue(g_csSVarRes0Name, strResName);
            pPool->release();
        }
        
        // RESTRICTION: 
        //      NOTE, to deliver proper trigger for console, the DownloadGUID must be updated last !!!!!!!!!!!!!!!!
        
        // security variables
        pPool = (CSysVarPool*) NameSpaceHolder::GetGlobalSecuritySysVars();
        if (pPool != NULL)
        {
            // update string does not matter, security info will be reread from security.txt
            hr = pPool->UpdateValue(g_csSVarSecurityIsEnabled, _T(""));
            pPool->release();
        }
        
        // resource system variable
        pPool = (CSysVarPool*) NameSpaceHolder::GetResourceDLoadSysVars(0);
        if (pPool != NULL)
        {
            hr = pPool->UpdateValue(g_csSVarDate, strNow);
            hr = pPool->UpdateValue(g_csSVarUser, m_strLoggedInUserName);
            hr = pPool->UpdateValue(g_csSVarHost, m_strEngMachineName);
            hr = pPool->UpdateValue(g_csSVarPrjGUID, pszProjectGUID);
            hr = pPool->UpdateValue(g_csSVarCleared, bClearedResource ? _T("-1") : _T("0"));
            hr = pPool->UpdateValue(g_csSVarDLoadGUID, pszDownloadGUID);
            pPool->release();
        }

    }
    // update variables on commit of console domains:
    if (target == VisualDomains)
    {
        // console system variable
        pPool = (CSysVarPool*) NameSpaceHolder::GetConsoleDLoadSysVars();
        if (pPool != NULL)
        {
            hr = pPool->UpdateValue(g_csSVarDate, strNow);
            hr = pPool->UpdateValue(g_csSVarUser, m_strLoggedInUserName);
            hr = pPool->UpdateValue(g_csSVarHost, m_strEngMachineName);
            hr = pPool->UpdateValue(g_csSVarPrjGUID, pszProjectGUID);
            hr = pPool->UpdateValue(g_csSVarDLoadGUID, pszDownloadGUID);
            hr = pPool->UpdateValue(g_csSVarCleared, bClearedResource ? _T("-1") : _T("0"));
            pPool->release();
        }
    }

    if (target == ProgramDomains)
    {
        NameSpaceHolder *pNSpace;
        GetGlobalNamespace(&pNSpace);
        if (pNSpace) {
            GenericPointer<IEC_Resource>    pRes;
            HRESULT hr = pNSpace->GetDefaultResource(&pRes);
            if (SUCCEEDED(hr)) {
                hr = pRes->OpenResourceInstance();
            }
    
            // need only to reconnect with control!
            pNSpace->release();
        }
    }

    return S_OK;
}


#define FDOMAIN_200_BEGIN		0x0000u
#define FDOMAIN_200_CONFIG		0x0001u
#define FDOMAIN_200_CODE		0x0002u
#define FDOMAIN_200_INIT		0x0003u
#define FDOMAIN_200_CUSTOM		0x0004u
#define FDOMAIN_200_DEBUG		0x0005u
#define FDOMAIN_200_FINISH		0x0006u
#define FDOMAIN_200_END			0x0007u

#define FDOMAIN_300_BEGIN		0x0000u
#define FDOMAIN_300_CLEAR		0x0001u
#define FDOMAIN_300_CONFIG		0x0002u
#define FDOMAIN_300_CODE		0x0003u
#define FDOMAIN_300_INIT		0x0004u
#define FDOMAIN_300_CUSTOM		0x0005u
#define FDOMAIN_300_DEBUG		0x0006u
#define FDOMAIN_300_FINISH		0x0007u
#define FDOMAIN_300_CFGIOL		0x0008u
#define FDOMAIN_300_END			0x0009u

#define FDOMAIN_BEGIN			0x0000u
#define FDOMAIN_CONFIG			0x0001u
#define FDOMAIN_CODE			0x0002u
#define FDOMAIN_INIT			0x0003u
#define FDOMAIN_CUSTOM			0x0004u
#define FDOMAIN_DEBUG			0x0005u
#define FDOMAIN_FINISH			0x0006u
#define FDOMAIN_END				0x0007u
#define FDOMAIN_CLEAR			0x0008u
#define FDOMAIN_CFGIOL			0x0009u



//------------------------------------------------------------------*
BOOL AppendFlashFile(FILE *pFlashFile, BYTE byCommand, BYTE *pData, DWORD uLen)
{
	if (CConnection::GetFirmware() < 22000)
	{
		switch(byCommand)
		{
		case FDOMAIN_BEGIN:			byCommand = FDOMAIN_200_BEGIN;		break;
		case FDOMAIN_CONFIG:		byCommand = FDOMAIN_200_CONFIG;		break;
		case FDOMAIN_CODE:			byCommand = FDOMAIN_200_CODE;		break;
		case FDOMAIN_INIT:			byCommand = FDOMAIN_200_INIT;		break;
		case FDOMAIN_CUSTOM:		byCommand = FDOMAIN_200_CUSTOM;		break;
		case FDOMAIN_DEBUG:			byCommand = FDOMAIN_200_DEBUG;		break;
		case FDOMAIN_FINISH:		byCommand = FDOMAIN_200_FINISH;		break;
		case FDOMAIN_END:			byCommand = FDOMAIN_200_END;		break;
		case FDOMAIN_CLEAR:			byCommand = 0xffu;					break;
		case FDOMAIN_CFGIOL:		byCommand = 0xffu;					break;
		default:					byCommand = 0xffu;					break;
		}
	}
	else
	{
		switch(byCommand)
		{
		case FDOMAIN_BEGIN:			byCommand = FDOMAIN_300_BEGIN;		break;
		case FDOMAIN_CONFIG:		byCommand = FDOMAIN_300_CONFIG;		break;
		case FDOMAIN_CODE:			byCommand = FDOMAIN_300_CODE;		break;
		case FDOMAIN_INIT:			byCommand = FDOMAIN_300_INIT;		break;
		case FDOMAIN_CUSTOM:		byCommand = FDOMAIN_300_CUSTOM;		break;
		case FDOMAIN_DEBUG:			byCommand = FDOMAIN_300_DEBUG;		break;
		case FDOMAIN_FINISH:		byCommand = FDOMAIN_300_FINISH;		break;
		case FDOMAIN_END:			byCommand = FDOMAIN_300_END;		break;
		case FDOMAIN_CLEAR:			byCommand = FDOMAIN_300_CLEAR;		break;
		case FDOMAIN_CFGIOL:		byCommand = FDOMAIN_300_CFGIOL;		break;
		default:					byCommand = 0xffu;					break;
		}
	}

    USHORT uCommand = TRANSLATE_SHORT(byCommand);
	fwrite(&uCommand, sizeof(USHORT), 1, pFlashFile);
    uLen = TRANSLATE_LONG(uLen);
    fwrite(&uLen, sizeof(DWORD), 1, pFlashFile);
    uLen = TRANSLATE_LONG(uLen);
    fwrite(pData, 1, uLen, pFlashFile);
    return TRUE;
}


//------------------------------------------------------------------*
BOOL AppendFlashFile(FILE *pFlashFile, BYTE byCommand, LPCTSTR lpFileName)
{
    HANDLE h = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
                          OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE)
          return FALSE;

    DWORD dwFileLen = GetFileSize(h, NULL);
    if (dwFileLen == (DWORD)-1)
    {
          CloseHandle(h);
          return FALSE;
    }
    BYTE *pFileData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, dwFileLen);
    if (pFileData == NULL)
    {
          CloseHandle(h);
          return FALSE;
    }
    if (!ReadFile(h, pFileData, dwFileLen, &dwFileLen, NULL))
    {
          HeapFree(GetProcessHeap(), 0, pFileData);
          CloseHandle(h);
          return FALSE;
    }
    CloseHandle(h);

    BOOL retval = AppendFlashFile(pFlashFile, byCommand, pFileData, dwFileLen);
    HeapFree(GetProcessHeap(), 0, pFileData);
    return retval;
}

//------------------------------------------------------------------*
HRESULT CCommissioningSession::DownloadToControl(BSTR sDownloadGuid)
{
    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DownloadToControl]\n"));

	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_DownloadToControl(sDownloadGuid, &bHandled);
	if (FAILED(hr) || bHandled == TRUE) {
		return hr;
	}

	if (CConnection::GetFirmware() < 21000)
	{
		/* Old download
		 */
		return DL_Download_20x(sDownloadGuid);
	}

	if (m_bFlashResource == TRUE)
	{
		/* Clear the flash memory on control
		 */
		hr = DL_ClearFlash();
		if (FAILED(hr)) 
		{
		    TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DownloadToControl] DL_ClearFlash() failed (0x%04x)\n"), hr);
			return hr;
		}
	}

	/* Build flash file (_flash.f4c)
	 */
	hr = DL_BuildFlashFile(sDownloadGuid);
	if (FAILED(hr)) 
	{
	    TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DownloadToControl] DL_BuildFlashFile() failed (0x%04x)\n"), hr);
		return hr;
	}

	/* Download
	 */
	if (m_bIsFullDownload == TRUE) 
	{
		// Full 	
		hr = DL_Download_210(sDownloadGuid);
		if (FAILED(hr)) 
		{
		    TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DownloadToControl] DL_Download_210() failed (0x%04x)\n"), hr);
			return hr;
		}
	}
	else 
	{
		// Incremental 
		hr = DL_Changes(sDownloadGuid);
		if (FAILED(hr)) 
		{
		    TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DownloadToControl] DL_Changes() failed (0x%04x)\n"), hr);
			return hr;
		}
	}

	/* Flash
	 */
	if (m_bFlashResource == TRUE) 
	{
		hr = DL_Flash();
		if (FAILED(hr)) 
		{
		    TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DownloadToControl] DL_Flash() failed (0x%04x)\n"), hr);
			return hr;
		}
	}

	return hr;
}

//------------------------------------------------------------------*
HRESULT	CCommissioningSession::DL_BuildFlashFile(BSTR sDownloadGuid)
{
    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_BuildFlashFile]\n"));

	FILE *pFlashFile = pFlashFile = fopen(GetConfigPath() + F4C_FLASH_FILE, "wb");

	if (pFlashFile == NULL) 
	{
	    TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_BuildFlashFile] fopen() failed\n"));
		return E_FAIL;
	}

	if (CConnection::GetFirmware() < 21030)
	{
		/* < 21030
		 */
		IEC_UINT uFlag = TRANSLATE_SHORT(DOWN_MODE_COLD);
   		AppendFlashFile(pFlashFile, FDOMAIN_BEGIN, (BYTE *)&uFlag, sizeof(uFlag));
	}
	else if (CConnection::GetFirmware() < 22000)
	{
		/* < 22000 (Append GUID already here in order to verify retain memory in VMM.)
		 */
		UINT uLen	= sizeof(IEC_UINT) + sizeof(GUID);
		BYTE *pData = NULL;

		HEAP_ALLOC(pData, uLen);

		*(IEC_UINT *)pData = TRANSLATE_SHORT(DOWN_MODE_COLD);

		CLSIDFromString(sDownloadGuid, (GUID *)(pData + sizeof(IEC_UINT)));

   		AppendFlashFile(pFlashFile, FDOMAIN_BEGIN, pData, uLen);

		HEAP_FREE(pData, uLen);
	}
	else
	{
		/* >= 22000 (Append CG Firmware version already here in order to verify CG version earlier.)
		 */
		UINT uLen	= sizeof(IEC_UINT) + sizeof(GUID) + sizeof(IEC_UDINT);
		BYTE *pData = NULL;

		HEAP_ALLOC(pData, uLen);

		*(IEC_UINT *)pData = TRANSLATE_SHORT(DOWN_MODE_COLD);

		CLSIDFromString(sDownloadGuid, (GUID *)(pData + sizeof(IEC_UINT)));

		GetCompiledFWVersion((UINT *)(pData + sizeof(IEC_UINT) + sizeof(GUID)));

		AppendFlashFile(pFlashFile, FDOMAIN_BEGIN, pData, uLen);

		HEAP_FREE(pData, uLen);
	}

	if (CConnection::GetFirmware() >= 22000)
	{
		/* >= 22000 Explicitly clear resource on download.
		 */
		USHORT uVal = 0;

		AppendFlashFile(pFlashFile, FDOMAIN_CLEAR, (BYTE *)&uVal, sizeof(USHORT));
	}

	CDomainList *pDomainList = CSC_ParseDomainList(TRUE);

	for (POSITION pos = pDomainList->GetHeadPosition(); pos != NULL ; )
	{
		CSC_DOMAINENTRY domain = pDomainList->GetNext(pos);

        BYTE byFlashCommand;
		switch(domain.type) 
		{
			case CSC_DOMAINTYPE_CODE:
                byFlashCommand	= FDOMAIN_CODE;
				break;
			case CSC_DOMAINTYPE_INITIAL:
                byFlashCommand	= FDOMAIN_INIT;
				break;
			case CSC_DOMAINTYPE_CONFIG:
                byFlashCommand	= FDOMAIN_CONFIG;
				break;
			case CSC_DOMAINTYPE_CUSTOM:
                byFlashCommand	= FDOMAIN_CUSTOM;
				break;
			default:
				continue;      
		}
			
		AppendFlashFile(pFlashFile, byFlashCommand, domain.file);
	}

	delete pDomainList;

	if (g_bSupportDebugDownload)
	{
		CString strDBIFile = CSC_CreateDBIDownloadDomain(TRUE);
		AppendFlashFile(pFlashFile, FDOMAIN_DEBUG, strDBIFile);
	}

	GUID guid;
	if (FAILED(CLSIDFromString(sDownloadGuid, &guid))) {
		guid = GUID_NULL;
	}

	AppendFlashFile(pFlashFile, FDOMAIN_FINISH, (BYTE *)&guid, sizeof(guid));

	if (CConnection::GetFirmware() < 21030)
	{
		AppendFlashFile(pFlashFile, FDOMAIN_END, NULL, 0);
	}
	else if (CConnection::GetFirmware() < 22000)
	{
		USHORT uVal = 0;

		AppendFlashFile(pFlashFile, FDOMAIN_END, (BYTE *)&uVal, sizeof(USHORT));
	}
	else
	{
		USHORT uVal = 0;

		AppendFlashFile(pFlashFile, FDOMAIN_CFGIOL, (BYTE *)&uVal, sizeof(USHORT));
		AppendFlashFile(pFlashFile, FDOMAIN_END,    NULL, 0);
	}

  	fclose(pFlashFile);

    return S_OK;
}

//------------------------------------------------------------------*
HRESULT	CCommissioningSession::DL_Download_210(BSTR sDownloadGuid)
{
    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Download_210]\n"));

	HRESULT hr = S_OK;

	myConnection.Lock();	// --------------------------------------------
	{
	    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Download_210] CMD_DOWNLOAD_BEGIN\n"));

		if (CConnection::GetFirmware() < 21030)
		{
			/* < 21030
			 */
			IEC_UINT uFlag = TRANSLATE_SHORT(DOWN_MODE_COLD);
			hr = myConnection.SendCommand(FALSE, CMD_DOWNLOAD_BEGIN, (BYTE *)&uFlag, sizeof(uFlag));
		}
		else if (CConnection::GetFirmware() < 22000)
		{
			/* < 22000 (Append GUID already here in order to verify retain memory in VMM.)
			 */
			UINT uLen	= sizeof(IEC_UINT) + sizeof(GUID);
			BYTE *pData = NULL;

			HEAP_ALLOC(pData, uLen);

			*(IEC_UINT *)pData = TRANSLATE_SHORT(DOWN_MODE_COLD);

			CLSIDFromString(sDownloadGuid, (GUID *)(pData + sizeof(IEC_UINT)));

			hr = myConnection.SendCommand(FALSE, CMD_DOWNLOAD_BEGIN, pData, uLen);

			HEAP_FREE(pData, uLen);
		}
		else
		{
			/* >= 22000 (Append CG Firmware version already here in order to verify CG version earlier.)
			 */
			UINT uLen	= sizeof(IEC_UINT) + sizeof(GUID) + sizeof(IEC_UDINT);
			BYTE *pData = NULL;

			HEAP_ALLOC(pData, uLen);

			*(IEC_UINT *)pData = TRANSLATE_SHORT(DOWN_MODE_COLD);

			CLSIDFromString(sDownloadGuid, (GUID *)(pData + sizeof(IEC_UINT)));

			hr = GetCompiledFWVersion((UINT *)(pData + sizeof(IEC_UINT) + sizeof(GUID)));

			if (SUCCEEDED(hr))
			{
				hr = myConnection.SendCommand(FALSE, CMD_DOWNLOAD_BEGIN, pData, uLen);
			}

			HEAP_FREE(pData, uLen);
		}

		if(FAILED(hr))
		{
			TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Download_210] CMD_DOWNLOAD_BEGIN failed (0x%04x)\n"), hr);
		}

		if (SUCCEEDED(hr) && CConnection::GetFirmware() >= 22000)
		{
			/* >= 22000 Explicitly clear resource on download.
			 */
		    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Download_210] CMD_DOWNLOAD_CLEAR\n"));

			hr = PollControl(FALSE, 50, CMD_DOWNLOAD_CLEAR);

			if(FAILED(hr))
			{
				TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Download_210] CMD_DOWNLOAD_CLEAR failed (0x%04x)\n"), hr);
			}
		}

		CDomainList *pDomainList = CSC_ParseDomainList(TRUE);

		for (POSITION pos = pDomainList->GetHeadPosition(); SUCCEEDED(hr) && pos != NULL ; )
		{
			CSC_DOMAINENTRY domain = pDomainList->GetNext(pos);

			BYTE byCommand		= CMD_NOT_IMPLEMENTED;
			LPCTSTR szCommand	= NULL;

			switch(domain.type) 
			{
				case CSC_DOMAINTYPE_CODE:
					byCommand		= CMD_DOWNLOAD_CODE;
					szCommand		= _T("CMD_DOWNLOAD_CODE");
					break;
				case CSC_DOMAINTYPE_INITIAL:
					byCommand		= CMD_DOWNLOAD_INITIAL;
					szCommand		= _T("CMD_DOWNLOAD_INITIAL");
					break;
				case CSC_DOMAINTYPE_CONFIG:
					byCommand		= CMD_DOWNLOAD_CONFIG;
					szCommand		= _T("CMD_DOWNLOAD_CONFIG");
					break;
				case CSC_DOMAINTYPE_CUSTOM:
					byCommand		= CMD_DOWNLOAD_CUSTOM;
					szCommand		= _T("CMD_DOWNLOAD_CUSTOM");
					break;
				default:
					szCommand		= _T("<nix>");
					continue;      
			}
			
		    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Download_210] %s\n"), szCommand);

			hr = myConnection.SendCommand(FALSE, byCommand, domain.file);

			if(FAILED(hr))
			{
				TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Download_210] %s failed (0x%04x)\n"), szCommand, hr);
			}
		}

		delete pDomainList;

		if (SUCCEEDED(hr) && g_bSupportDebugDownload)
		{
		    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Download_210] CMD_DOWNLOAD_DEBUG\n"));

			CString strDBIFile = CSC_CreateDBIDownloadDomain(FALSE);
			
			if (strDBIFile.IsEmpty())
			{
				CString sMsg ;
				BSTR bMsg;

				sMsg.Format(OPC_MSG, "--------------------------------------------------------------------------------------------");
				bMsg = sMsg.AllocSysString();

				CControlMgt::SendMsgToEng(bMsg);
				SysFreeString(bMsg);

				sMsg.Format(OPC_MSG, "ERROR: Failed to create DBI download domain. You probably have an older Engineering version.");
				bMsg = sMsg.AllocSysString();

				CControlMgt::SendMsgToEng(bMsg);
				SysFreeString(bMsg);

				sMsg.Format(OPC_MSG, "You need at least version V2.2.0 or V2.1.5 (B3808) in order to connect to this OPC server.");
				bMsg = sMsg.AllocSysString();

				CControlMgt::SendMsgToEng(bMsg);
				SysFreeString(bMsg);

				sMsg.Format(OPC_MSG, "--------------------------------------------------------------------------------------------");
				bMsg = sMsg.AllocSysString();

				CControlMgt::SendMsgToEng(bMsg);
				SysFreeString(bMsg);

				hr = E_FAIL;
			}
			else
			{
				hr = myConnection.SendCommand(FALSE, CMD_DOWNLOAD_DEBUG, strDBIFile);
			}

			if(FAILED(hr))
			{
				TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Download_210] CMD_DOWNLOAD_DEBUG failed (0x%04x)\n"), hr);
			}
		}

		if (SUCCEEDED(hr))
		{
		    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Download_210] CMD_DOWNLOAD_FINISH.\n"));

			GUID guid;
			if (FAILED(CLSIDFromString(sDownloadGuid, &guid))) 
			{
				guid = GUID_NULL;
			}

			myConnection.SetGUID(guid);

			hr = myConnection.SendCommand(FALSE, CMD_DOWNLOAD_FINISH, (BYTE *)&guid, sizeof(guid));

			if(FAILED(hr))
			{
				TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Download_210] CMD_DOWNLOAD_FINISH failed (0x%04x)\n"), hr);
			}

			if (SUCCEEDED(hr)) 
			{
				hr = myConnection.CheckVersion();
			}
			else
			{
				TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Download_210] myConnection.CheckVersion() failed (0x%04x)\n"), hr);
			}
		}

	} myConnection.Unlock();	// ----------------------------------------

	if (SUCCEEDED(hr)) 
	{
		// Now we call back to the Engineering!
	    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Download_210] ibnConfigurationChanged()\n"));

		HRESULT hr = ibnConfigurationChanged();

		if(FAILED(hr))
		{
			TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Download_210] ibnConfigurationChanged() failed (0x%04x)\n"), hr);
		}
	}

	if (SUCCEEDED(hr)) 
	{
		if (CConnection::GetFirmware() < 21030) 
		{
		    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Download_210] CMD_DOWNLOAD_END\n"));

			hr = myConnection.SendCommand(TRUE, CMD_DOWNLOAD_END, NULL, 0);

			if(FAILED(hr))
			{
				TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Download_210] CMD_DOWNLOAD_END failed (0x%04x)\n"), hr);
			}
		}
		else if (CConnection::GetFirmware() < 22000) 
		{
		    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Download_210] CMD_DOWNLOAD_END\n"));

			hr = PollControl(TRUE, 10, CMD_DOWNLOAD_END);

			if(FAILED(hr))
			{
				TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Download_210] CMD_DOWNLOAD_END failed (0x%04x)\n"), hr);
			}
		}
		else 
		{
		    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Download_210] CMD_DOWNLOAD_IOL\n"));

			hr = PollControl(TRUE, 50, CMD_DOWNLOAD_IOL);

			if(FAILED(hr))
			{
				TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Download_210] CMD_DOWNLOAD_IOL failed (0x%04x)\n"), hr);
			}

			if (SUCCEEDED(hr)) 
			{
			    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Download_210] CMD_DOWNLOAD_END\n"));

				hr = myConnection.SendCommand(TRUE, CMD_DOWNLOAD_END, NULL, 0);

				if(FAILED(hr))
				{
					TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Download_210] CMD_DOWNLOAD_END failed (0x%04x)\n"), hr);
				}
			}
		}	 
	}

    return hr;
}

//------------------------------------------------------------------*
HRESULT CCommissioningSession::GetCompiledFWVersion(UINT *upFirmware)
{
    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][GetCompiledFWVersion]\n"));

	HRESULT hr = S_OK;

	if (CConnection::GetFirmware() < 22000)
	{
		return E_FAIL;
	}

	CDomainList *pDomainList = CSC_ParseDomainList(TRUE);

	for (POSITION pos = pDomainList->GetHeadPosition(); SUCCEEDED(hr) && pos != NULL ; )
	{
		CSC_DOMAINENTRY domain = pDomainList->GetNext(pos);

		if (domain.type == CSC_DOMAINTYPE_CONFIG)
		{
			HANDLE hFile = CreateFile(domain.file, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				return E_FAIL;
			}

			UINT uLen   = sizeof(XProject_213);
			BYTE *pData = (BYTE *)HeapAlloc(GetProcessHeap(), 0, uLen);

			DWORD dwRead;

			if (! ReadFile(hFile, pData, uLen, &dwRead, NULL))
			{
				HeapFree(GetProcessHeap(), 0, pData);
				CloseHandle(hFile);

				return E_FAIL;
			}

			CloseHandle(hFile);

			if (uLen != dwRead)
			{
				HeapFree(GetProcessHeap(), 0, pData);
				
				return E_FAIL;
			}

			XProject_213 *pProj = (XProject_213 *)pData;

			*upFirmware = pProj->ulBinDLVersion;

			HeapFree(GetProcessHeap(), 0, pData);

			break;
		}
	}

	delete pDomainList;

	return hr;
}

//------------------------------------------------------------------*
HRESULT	CCommissioningSession::DL_Changes(BSTR sDownloadGuid)
{
    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Changes]\n"));

	HRESULT hr = S_OK;

	myConnection.Lock();	// --------------------------------------------
	{
		m_pOCHlp->finishOC();

		hr = m_pOCHlp->createOCConfig();

		if (SUCCEEDED(hr))
		{
		    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Changes] CMD_OC_BEGIN\n"));

			hr = myConnection.SendCommand(FALSE, CMD_OC_BEGIN, m_pOCHlp->getOCConfig(), m_pOCHlp->getOCConfigSize());
			m_pOCHlp->destroyOCConfig();

			if(FAILED(hr))
			{
				TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Changes] CMD_OC_BEGIN failed (0x%04x)\n"), hr);
			}
		}

		CDomainList *pDomainList = CSC_ParseDomainList(FALSE);

		for (POSITION pos = pDomainList->GetHeadPosition(); SUCCEEDED(hr) && pos != NULL ; )
		{
			CSC_DOMAINENTRY domain = pDomainList->GetNext(pos);

			BYTE byCommand		= CMD_NOT_IMPLEMENTED;
			LPCTSTR szCommand	= NULL;

			switch(domain.type) 
			{
				case CSC_DOMAINTYPE_CODE:
					byCommand = CMD_OC_CODE;
					szCommand = _T("CMD_OC_CODE");
					break;
				case CSC_DOMAINTYPE_INITIAL:
					byCommand = CMD_OC_INITIAL;
					szCommand = _T("CMD_OC_INITIAL");
					break;
				case CSC_DOMAINTYPE_CUSTOM:
					byCommand = CMD_OC_CUSTOM;
					szCommand = _T("CMD_OC_CUSTOM");
					break;
				case CSC_DOMAINTYPE_CONFIG:
					if (m_pOCHlp->m_OCConfig.bTaskInfo)
					{
						szCommand = _T("CMD_OC_CONFIG");
						byCommand = CMD_OC_CONFIG;
					}
					else
					{
						szCommand = _T("<nix>");
						continue;
					}
					break;
				default:
					szCommand = _T("<nix>");
					continue;          
			}
			
		    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Changes] %s\n"), szCommand);

			hr = myConnection.SendCommand(FALSE, byCommand, domain.file);

			if(FAILED(hr))
			{
				TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Changes] %s failed (0x%04x)\n"), szCommand, hr);
			}
		}

		delete pDomainList;

		if (SUCCEEDED(hr) && g_bSupportDebugDownload)
		{
		    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Changes] CMD_OC_DEBUG\n"));

			CString strDBIFile = CSC_CreateDBIDownloadDomain(FALSE);

			if (strDBIFile.IsEmpty())
			{
				CString sMsg ;
				BSTR bMsg;

				sMsg.Format(OPC_MSG, "--------------------------------------------------------------------------------------------");
				bMsg = sMsg.AllocSysString();

				CControlMgt::SendMsgToEng(bMsg);
				SysFreeString(bMsg);

				sMsg.Format(OPC_MSG, "ERROR: Failed to create DBI download domain. You probably have an older Engineering version.");
				bMsg = sMsg.AllocSysString();

				CControlMgt::SendMsgToEng(bMsg);
				SysFreeString(bMsg);

				sMsg.Format(OPC_MSG, "You need at least version V2.2.0 or V2.1.5 (B3808) in order to connect to this OPC server.");
				bMsg = sMsg.AllocSysString();

				CControlMgt::SendMsgToEng(bMsg);
				SysFreeString(bMsg);

				sMsg.Format(OPC_MSG, "--------------------------------------------------------------------------------------------");
				bMsg = sMsg.AllocSysString();

				CControlMgt::SendMsgToEng(bMsg);
				SysFreeString(bMsg);

				hr = E_FAIL;
			}
			else
			{
				hr = myConnection.SendCommand(FALSE, CMD_OC_DEBUG, strDBIFile);
			}	

			if(FAILED(hr))
			{
				TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Changes] CMD_OC_DEBUG failed (0x%04x)\n"), hr);
			}
		}

		if (SUCCEEDED(hr))
		{
		    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Changes] CMD_OC_FINISH\n"));

			GUID guid;
			if (FAILED(CLSIDFromString(sDownloadGuid, &guid))) 
			{
				guid = GUID_NULL;
			}

			myConnection.SetGUID(guid);

			hr = myConnection.SendCommand(FALSE, CMD_OC_FINISH, (BYTE *)&guid, sizeof(guid));

			if(FAILED(hr))
			{
				TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Changes] CMD_OC_FINISH failed (0x%04x)\n"), hr);
			}

			if (SUCCEEDED(hr)) 
			{
				hr = myConnection.CheckVersion();

				if(FAILED(hr))
				{
					TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Changes] CheckVersion() (0x%04x)\n"), hr);
				}
			}			
		}


	} myConnection.Unlock();	// ----------------------------------------

	if (SUCCEEDED(hr)) 
	{
		// Now we call back to the Engineering!
	    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Changes] ibnConfigurationChanged()\n"));

		ibnConfigurationChanged();

		if(FAILED(hr))
		{
			TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Changes] ibnConfigurationChanged() failed (0x%04x)\n"), hr);
		}
	}

	if (SUCCEEDED(hr)) 
	{
		// Commit the changes
	    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Changes] CMD_OC_COMMIT\n"));

		hr = PollControl(FALSE, 20, CMD_OC_COMMIT);

		if(FAILED(hr))
		{
			TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Changes] CMD_OC_COMMIT failed (0x%04x)\n"), hr);
		}
	}

	if (SUCCEEDED(hr)) 
	{
	    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Changes] CMD_OC_END\n"));

		hr = myConnection.SendCommand(TRUE, CMD_OC_END, NULL, 0);

		if(FAILED(hr))
		{
			TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Changes] CMD_OC_END failed (0x%04x)\n"), hr);
		}
	}

    return hr;
}

//------------------------------------------------------------------*
HRESULT CCommissioningSession::DL_Download_20x(BSTR sDownloadGuid)
{
    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Download_20x]\n"));

	/* Download prior to version V2.1.00
	 */
	BOOL bHandled = FALSE;

	HRESULT hr = Adapt_DownloadToControl(sDownloadGuid, &bHandled);
	if (FAILED(hr) || bHandled == TRUE)
	{
		return hr;
	}

	if (m_bFlashResource) 
	{
		hr = PollControl(FALSE, 100, CMD_CLEAR_FLASH);
	}

	FILE* pFlashFile = fopen(GetConfigPath() + _T("_flash.f4c"), "wb");

	myConnection.Lock();	// --------------------------------------------
	{
		if (SUCCEEDED(hr)) 
		{
			IEC_UINT uFlag;

			if (m_bFlashResource)
				uFlag = TRANSLATE_SHORT(DOWN_MODE_COLD | DOWN_MODE_FLASH);
			else
				uFlag = TRANSLATE_SHORT(DOWN_MODE_COLD | DOWN_MODE_NORMAL);

			hr = myConnection.SendCommand(FALSE, CMD_DOWNLOAD_BEGIN, (BYTE *)&uFlag, sizeof(uFlag));
   			AppendFlashFile(pFlashFile, FDOMAIN_BEGIN, (BYTE *)&uFlag, sizeof(uFlag));
		}

		CDomainList *pDomainList = CSC_ParseDomainList(TRUE);

		for (POSITION pos = pDomainList->GetHeadPosition(); SUCCEEDED(hr) && pos != NULL ; )
		{
			CSC_DOMAINENTRY domain = pDomainList->GetNext(pos);

			BYTE byCommand = CMD_NOT_IMPLEMENTED;
            BYTE byFlashCommand;
			switch(domain.type) {
				case CSC_DOMAINTYPE_CODE:
					byCommand = CMD_DOWNLOAD_CODE;
                    byFlashCommand = FDOMAIN_CODE;
					break;
				case CSC_DOMAINTYPE_INITIAL:
					byCommand = CMD_DOWNLOAD_INITIAL;
                    byFlashCommand = FDOMAIN_INIT;
					break;
				case CSC_DOMAINTYPE_CONFIG:
					byCommand = CMD_DOWNLOAD_CONFIG;
                    byFlashCommand = FDOMAIN_CONFIG;
					break;
				case CSC_DOMAINTYPE_CUSTOM:
					byCommand = CMD_DOWNLOAD_CUSTOM;
                    byFlashCommand = FDOMAIN_CUSTOM;
					break;
				default:
					continue;
          
			}
			
			hr = myConnection.SendCommand(FALSE, byCommand, domain.file);
			AppendFlashFile(pFlashFile, byFlashCommand, domain.file);
		}

		if (SUCCEEDED(hr) && g_bSupportDebugDownload)
		{
			CString strDBIFile = CSC_CreateDBIDownloadDomain(FALSE);

			hr = myConnection.SendCommand(FALSE, CMD_DOWNLOAD_DEBUG, strDBIFile);
			AppendFlashFile(pFlashFile, FDOMAIN_DEBUG, strDBIFile);
		}

		if (SUCCEEDED(hr)) 
		{
			GUID guid;
			if (FAILED(CLSIDFromString(sDownloadGuid, &guid)))
				  guid = GUID_NULL;

			myConnection.SetGUID(guid);

			hr = myConnection.SendCommand(FALSE, CMD_DOWNLOAD_FINISH, (BYTE *)&guid, sizeof(guid));
			AppendFlashFile(pFlashFile, FDOMAIN_FINISH, (BYTE *)&guid, sizeof(guid));

			if (SUCCEEDED(hr)) {
				hr = myConnection.CheckVersion();
			}			
		}

		delete pDomainList;

	} myConnection.Unlock();	// ----------------------------------------

	if (SUCCEEDED(hr)) 
	{
		// Now we call back to the Engineering!
		ibnConfigurationChanged();

		hr = myConnection.SendCommand(TRUE, CMD_DOWNLOAD_END, NULL, 0);
        AppendFlashFile(pFlashFile, FDOMAIN_END, NULL, 0);
	}

  	fclose(pFlashFile);

    return hr;
}

//------------------------------------------------------------------*
HRESULT CCommissioningSession::DL_Flash()
{
    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_Flash]\n"));

	HRESULT hr = myConnection.SendCommand(FALSE, CMD_FLASH, GetConfigPath() + F4C_FLASH_FILE);

	if (FAILED(hr))
	{
	    TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_Flash] SendCommand() failed (0x%04x)\n"), hr);
	}

	return hr;
}

//------------------------------------------------------------------*
HRESULT CCommissioningSession::DL_ClearFlash()
{
    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][DL_ClearFlash]\n"));

	HRESULT hr = PollControl(FALSE, 100, CMD_CLEAR_FLASH);

	if (FAILED(hr))
	{
	    TrcPrint(TRC_ERROR, _T("[CCommissioningSession][DL_ClearFlash] PollControl() failed (0x%04x)\n"), hr);
	}

	return hr;
}

//------------------------------------------------------------------*
HRESULT CCommissioningSession::PollControl(BOOL bLogon, UINT uRetry, UINT uCmd)
{
    TrcPrint(TRC_INTERFACE, _T("[CCommissioningSession][PollControl]\n"));

	HRESULT hr = S_OK;

	UINT uResult = (UINT)-1;

	for (IEC_UINT uCount = 0; SUCCEEDED(hr) && uResult != 0 && uCount < uRetry; uCount++) 
	{
		UINT uLen	= sizeof(IEC_UINT);
		BYTE *pData = NULL;

		if (uCount > 0) 
		{
			switch(uCmd)
			{	
				case CMD_INITIALIZE_CLEAR:
				case CMD_DOWNLOAD_IOL:
				case CMD_DOWNLOAD_CLEAR:
				case CMD_CLEAR_FLASH:
					SleepEx(750, TRUE);

				default:
					SleepEx(250, TRUE);
			}
		}

		HEAP_ALLOC(pData, uLen);

		*(IEC_UINT*)pData = TRANSLATE_SHORT(uCount);

		hr = myConnection.TransferData(bLogon, (BYTE)uCmd, &pData, &uLen);

		if (FAILED(hr))
		{
			TrcPrint(TRC_ERROR, _T("[CCommissioningSession][PollControl] TransferData() failed\n"));
		}

		if (SUCCEEDED(hr) && uLen < sizeof(IEC_UINT)) 
		{
			TrcPrint(TRC_ERROR, _T("[CCommissioningSession][PollControl] uLen < sizeof(IEC_UINT)\n"));
			hr = E_FAIL;
		}
		
		if (SUCCEEDED(hr)) 
		{
			uResult = TRANSLATE_SHORT(*(IEC_UINT *)pData);
		}

		HEAP_FREE(pData, uLen);
	}						

	if (uCount == uRetry) 
	{
		TrcPrint(TRC_ERROR, _T("[CCommissioningSession][PollControl] uCount == uRetry\n"));
		hr = E_FAIL;
	}

	return hr;
}

//------------------------------------------------------------------*
BOOL CCommissioningSession::AppendData(BYTE **ppData, UINT *pLen, BYTE *pAppendData, UINT uAppendLen)
{
    VOID *pNewData = HeapReAlloc(GetProcessHeap(), 0, *ppData, *pLen + uAppendLen);
    if (pNewData == NULL)
	{
		TrcPrint(TRC_ERROR, _T("[CCommissioningSession][AppendData] HeapReAlloc() failed\n"));
        return FALSE;
	}

    *ppData = (BYTE *)pNewData;
    memcpy(*ppData + *pLen, pAppendData, uAppendLen);
    *pLen += uAppendLen;
    return TRUE;
}

//------------------------------------------------------------------*
BOOL CCommissioningSession::AppendFile(BYTE **ppData, UINT *pLen, LPCTSTR lpFileName)
{
    HANDLE h = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE)
	{
		TrcPrint(TRC_ERROR, _T("[CCommissioningSession][AppendFile] CreateFile() failed\n"));
		return FALSE;
	}
	
    DWORD dwFileLen = GetFileSize(h, NULL);
    if (dwFileLen == (DWORD)-1)
    {
		TrcPrint(TRC_ERROR, _T("[CCommissioningSession][AppendFile] GetFileSize() failed\n"));
		CloseHandle(h);
		return FALSE;
    }
	
    BYTE *pFileData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, dwFileLen);
    if (pFileData == NULL)
    {
		TrcPrint(TRC_ERROR, _T("[CCommissioningSession][AppendFile] HeapAlloc() failed\n"));
		CloseHandle(h);
		return FALSE;
    }
	
    if (!ReadFile(h, pFileData, dwFileLen, &dwFileLen, NULL))
    {
		TrcPrint(TRC_ERROR, _T("[CCommissioningSession][AppendFile] ReadFile() failed\n"));
		HeapFree(GetProcessHeap(), 0, pFileData);
		CloseHandle(h);
		return FALSE;
    }
    CloseHandle(h);
	
    if (!AppendData(ppData, pLen, (BYTE*)&dwFileLen, sizeof(DWORD)))
    {
		TrcPrint(TRC_ERROR, _T("[CCommissioningSession][AppendFile] AppendData() failed\n"));
		HeapFree(GetProcessHeap(), 0, pFileData);
		return FALSE;
    }
	
    if (!AppendData(ppData, pLen, pFileData, dwFileLen))
    {
		TrcPrint(TRC_ERROR, _T("[CCommissioningSession][AppendFile] AppendData() failed\n"));
		HeapFree(GetProcessHeap(), 0, pFileData);
		return FALSE;
    }
	
    HeapFree(GetProcessHeap(), 0, pFileData);
    return TRUE;
}

//------------------------------------------------------------------*
BOOL CCommissioningSession::LoadFile(BYTE **ppData, UINT *pLen, LPCTSTR lpFileName)
{
    HANDLE h = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE)
	{
		TrcPrint(TRC_ERROR, _T("[CCommissioningSession][LoadFile] CreateFile() failed\n"));
        return FALSE;
	}
	
    DWORD dwFileLen = GetFileSize(h, NULL);
    if (dwFileLen == (DWORD)-1)
    {
		TrcPrint(TRC_ERROR, _T("[CCommissioningSession][LoadFile] GetFileSize() failed\n"));
		CloseHandle(h);
		return FALSE;
    }
	
    BYTE *pFileData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, dwFileLen);
    if (pFileData == NULL)
    {
		TrcPrint(TRC_ERROR, _T("[CCommissioningSession][LoadFile] HeapAlloc() failed\n"));
		CloseHandle(h);
		return FALSE;
    }
	
    if (!ReadFile(h, pFileData, dwFileLen, &dwFileLen, NULL))
    {
		TrcPrint(TRC_ERROR, _T("[CCommissioningSession][LoadFile] ReadFile() failed\n"));
		HeapFree(GetProcessHeap(), 0, pFileData);
		CloseHandle(h);
		return FALSE;
    }
    CloseHandle(h);
	
    if (!AppendData(ppData, pLen, pFileData, dwFileLen))
    {
		TrcPrint(TRC_ERROR, _T("[CCommissioningSession][LoadFile] AppendData() failed\n"));
		HeapFree(GetProcessHeap(), 0, pFileData);
		return FALSE;
    }
	
    HeapFree(GetProcessHeap(), 0, pFileData);
    return TRUE;
}

//------------------------------------------------------------------*
int CCommissioningSession::GetNumberOfTasks()
{
    int result = 0;
    CString tasks_list = GetConfigPath() + pszTasksFileName;
    FILE *f = _tfopen(tasks_list, _T("r"));
    if (f != NULL) {
        TCHAR line[100];
        while (_fgetts(line, 100, f) != NULL) {
            result++;
        }
        fclose(f);
    }
    return result;
}

//------------------------------------------------------------------*
HRESULT CCommissioningSession::SendMsgToEng(BSTR pszMessage)
{
	return CControlMgt::SendMsgToEng(pszMessage);
}

/* ---------------------------------------------------------------------------- */
