/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/Connection.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Connection.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/Connection.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "connection.h"
#include "security.h"
#include "ControlMgt.h"
#include "CommissioningMgt.h"
#include "CSC_OnlineErr.h"




CConnection myConnection;

DWORD CConnection::m_dwFirmware = 0;


static LPCTSTR szCmd[] = CMD_TEXT_SYMBOL;


/* CConnection
 * ----------------------------------------------------------------------------
 */
CConnection::CConnection()
{
    m_bConnected = FALSE;
	m_bConfigChanged = FALSE;
    m_bShutdown  = FALSE;
    m_hThread = NULL;
    m_ThreadID = 0;
    m_resourceState = 0;
    m_bLogon = FALSE;
    m_GUID = GUID_NULL;
    m_bDebugSession = FALSE;
	m_bRequestBPSync = TRUE;
}


/* ~CConnection
 * ----------------------------------------------------------------------------
 */
CConnection::~CConnection()
{

}


/* TransferNT
 * ----------------------------------------------------------------------------
 */
BOOL CConnection::TransferNT(BYTE *bypCmd, BYTE **ppData, UINT *upLen)
{
	BOOL bOK;
	BYTE byCmd = *bypCmd;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		bOK = m_CommObj.TransferNT(bypCmd, ppData, upLen);

		if (! bOK) 
		{
			// Hard communication error, force a reconnect to the control.
			m_bConnected = FALSE;
		}

	} m_CritSec.Unlock();	// ------------------------------------------------

    if (bOK) 
	{
		BSTR bMsg;
		if (m_CommObj.GetMsgString(*bypCmd, *ppData, *upLen, &bMsg)) {
			m_CommObj.SendMsgToEng(bMsg);
		}
    }

    if (bOK && byCmd != *bypCmd) {
		myConnection.TraceCommError(*bypCmd, *ppData, *upLen);
	}

	return bOK;
}


/* TransferNT
 * ----------------------------------------------------------------------------
 * Must always be called within m_CritSec!
 */
BOOL CConnection::TransferNT(BYTE *bypCmd, LPCTSTR cszFile, BYTE **ppData, UINT *upLen)
{
	BOOL bOK;
	BYTE byCmd = *bypCmd;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		bOK = m_CommObj.TransferNT(bypCmd, cszFile, ppData, upLen);

		if (! bOK) 
		{
			// Hard communication error, force a reconnect to the control.
			m_bConnected = FALSE;
		}

	} m_CritSec.Unlock();	// ------------------------------------------------

    if (bOK) 
	{
		BSTR bMsg;
		if (m_CommObj.GetMsgString(*bypCmd, *ppData, *upLen, &bMsg)) {
			m_CommObj.SendMsgToEng(bMsg);
		}
    }

    if (bOK && byCmd != *bypCmd) {
		myConnection.TraceCommError(*bypCmd, *ppData, *upLen);
	}

	return bOK;
}


/* IsSafeNT
 * ----------------------------------------------------------------------------
 * Must always be called within m_CritSec!
 */
HRESULT CConnection::IsSafeNT(BOOL bLogon)
{
	HRESULT hr = S_OK;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		if (! m_bConnected || m_bShutdown) {
			hr = E_CSC_NO_CONNECT_TO_CONTROL;
		}

		if (bLogon && ! m_bLogon) {
			hr = E_CSC_NO_CONNECT_TO_CONTROL;
		}

	} m_CritSec.Unlock();	// ------------------------------------------------

	return hr;
}


/* SendCommand
 * ----------------------------------------------------------------------------
 */
HRESULT CConnection::SendCommand(BOOL bLogon, BYTE byCmd, BYTE *pData, UINT uLen)
{
	BOOL bOK;
	BYTE byOrg;

	if (GetFirmware() >= 22000 && byCmd == CMD_DOWNLOAD_BEGIN)
	{
		m_CommObj.SendNLToEng();
		m_CommObj.SendMsgToEng("<--------->  Starting Download");
	}

	if (GetFirmware() >= 22000 && byCmd == CMD_OC_BEGIN)
	{
		m_CommObj.SendNLToEng();
		m_CommObj.SendMsgToEng("<--------->  Starting Online Change");
	}

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		if (FAILED(IsSafeNT(bLogon))) 
		{
			m_CritSec.Unlock();

			return E_CSC_NO_CONNECT_TO_CONTROL;
		}
		
		BYTE *pLocData	= NULL;
		UINT uLocLen	= uLen;

		HEAP_ALLOC(pLocData, uLocLen);

		memcpy(pLocData, pData, uLocLen);

		byOrg = byCmd;

		bOK = TransferNT(&byCmd, &pLocData, &uLocLen);

		HEAP_FREE(pLocData, uLocLen);

	} m_CritSec.Unlock();	// ------------------------------------------------

	if (GetFirmware() >= 22000 && byCmd == CMD_DOWNLOAD_END)
	{
		m_CommObj.SendMsgToEng("<--------->  Download Finished");
	}

	if (GetFirmware() >= 22000 && byCmd == CMD_OC_END)
	{
		m_CommObj.SendMsgToEng("<--------->  Online Changed Finished");
	}

	return bOK && byOrg == byCmd ? S_OK : E_FAIL;
}


/* SendCommand
 * ----------------------------------------------------------------------------
 */
HRESULT CConnection::SendCommand(BOOL bLogon, BYTE byCmd, LPCTSTR cszFile)
{
	BOOL bOK;
	BYTE byOrg;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		if (FAILED(IsSafeNT(bLogon))) 
		{
			m_CritSec.Unlock();

			return E_CSC_NO_CONNECT_TO_CONTROL;
		}

		BYTE *pLocData	= NULL;
		UINT uLocLen	= 0;

		HEAP_ALLOC(pLocData, uLocLen);

		byOrg = byCmd;

		bOK = TransferNT(&byCmd, cszFile, &pLocData, &uLocLen);

		HEAP_FREE(pLocData, uLocLen);

	} m_CritSec.Unlock();	// ------------------------------------------------

	return bOK && byOrg == byCmd ? S_OK : E_FAIL;
}


/* TransferData
 * ----------------------------------------------------------------------------
 */
HRESULT CConnection::TransferData(BOOL bLogon, BYTE byCmd, BYTE **ppData, UINT *upLen)
{
	BOOL bOK;
	BYTE byOrg;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		if (FAILED(IsSafeNT(bLogon))) 
		{
			m_CritSec.Unlock();

			return E_CSC_NO_CONNECT_TO_CONTROL;
		}

		byOrg = byCmd;

		bOK = TransferNT(&byCmd, ppData, upLen);

	} m_CritSec.Unlock();	// ------------------------------------------------

	return bOK && byOrg == byCmd ? S_OK : E_FAIL;
}


/* TransferData
 * ----------------------------------------------------------------------------
 */
HRESULT CConnection::TransferData(BYTE byCmd, BYTE **ppData, UINT *upLen)
{
	BOOL bOK;
	BYTE byOrg;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		byOrg = byCmd;

		bOK = TransferNT(&byCmd, ppData, upLen);

	} m_CritSec.Unlock();	// ------------------------------------------------

	return bOK && byOrg == byCmd ? S_OK : E_FAIL;
}


/* SendDirect
 * ----------------------------------------------------------------------------
 */
HRESULT CConnection::SendDirect(BYTE *bypCmd, BYTE **ppData, UINT *upLen)
{
	BOOL bOK;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		bOK = TransferNT(bypCmd, ppData, upLen);

	} m_CritSec.Unlock();	// ------------------------------------------------

	return bOK ? S_OK : E_FAIL;
}


/* CheckMessages
 * ----------------------------------------------------------------------------
 */
HRESULT CConnection::CheckMessages(void)
{
	UINT uLen	= 0;
	BYTE *pData = NULL;

	HRESULT hr = TransferData(FALSE, CMD_GET_STATE, &pData, &uLen);

	if (SUCCEEDED(hr) && uLen < sizeof(XInfo)) {
		hr = E_FAIL;
	}

	if (SUCCEEDED(hr))
	{
        XInfo *pInfo = (XInfo *)pData;
        XBPNotification *pBPNotificationTags = (XBPNotification *)(pInfo + 1);
        LPSTR lpMessageStrings = (LPSTR)(pBPNotificationTags + TRANSLATE_SHORT(pInfo->uBPCount));
        UINT  cbMessageStrings = uLen - sizeof(XInfo) - TRANSLATE_SHORT(pInfo->uBPCount)*sizeof(XBPNotification);
        assert(uLen>=sizeof(XInfo) + TRANSLATE_SHORT(pInfo->uBPCount)*sizeof(XBPNotification));
        m_resourceState = TRANSLATE_LONG(pInfo->ulResState);
        HandleBreakpoints(TRANSLATE_SHORT(pInfo->uBPCount), pBPNotificationTags);
        HandleMessages(TRANSLATE_SHORT(pInfo->uStrCount), lpMessageStrings, cbMessageStrings );
    }
    
	HEAP_FREE(pData, uLen);

    return hr;
}


/* CheckMessages
 * ----------------------------------------------------------------------------
 */
HRESULT CConnection::CheckVersion()
{
	HRESULT hr;
		
	m_CritSec.Lock();	// ----------------------------------------------------
	{
		hr = SendCommand(FALSE, CMD_LOGIN, (BYTE *)&m_GUID, sizeof(m_GUID));

		m_bLogon = SUCCEEDED(hr);

	} m_CritSec.Unlock();	// ------------------------------------------------

	return hr;
}


/* IsConnected
 * ----------------------------------------------------------------------------
 */
BOOL CConnection::IsConnected()
{
	BOOL bIsConnected;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		bIsConnected = m_bConnected && ! m_bShutdown;

	} m_CritSec.Unlock();	// ------------------------------------------------

	return bIsConnected;
}


/* IsLogon
 * ----------------------------------------------------------------------------
 */
BOOL CConnection::IsLogon()
{
	BOOL bIsLogon;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		bIsLogon = m_bLogon;

	} m_CritSec.Unlock();	// ------------------------------------------------

	return bIsLogon;
}


/* Disconnect
 * ----------------------------------------------------------------------------
 */
void CConnection::Disconnect(BOOL bConfigChanged)
{
	m_CritSec.Lock();	// ----------------------------------------------------
	{
		m_bConnected		= FALSE;
		m_bConfigChanged	= bConfigChanged;
	
	} m_CritSec.Unlock();	// ------------------------------------------------
}


/* Lock
 * ----------------------------------------------------------------------------
 */
void CConnection::Lock(void)
{
	m_CritSec.Lock();
}


/* Unlock
 * ----------------------------------------------------------------------------
 */
void CConnection::Unlock(void)
{
	m_CritSec.Unlock();
}


/* SetGUID
 * ----------------------------------------------------------------------------
 */
void CConnection::SetGUID(const GUID &guid)
{
	m_CritSec.Lock();	// ----------------------------------------------------
	{
		m_GUID	 = guid;
		m_bLogon = TRUE;
	
	} m_CritSec.Unlock();	// ------------------------------------------------
}


/* SetDebugSession
 * ----------------------------------------------------------------------------
 */
BOOL CConnection::SetDebugSession(BOOL bON)
{
	m_bDebugSession = bON;

    return TRUE;
}


/* CheckCommParams
 * ----------------------------------------------------------------------------
 */
BOOL CConnection::CheckCommParams(void)
{
    static DWORD idle;

    if (GetTickCount()-idle < 5000)
	{
        return FALSE;
	}
    idle = GetTickCount();
    
	BOOL bRes;
	
	m_CritSec.Lock();	// ----------------------------------------------------
	{
		bRes = m_CommObj.CheckCommParams();    

	} m_CritSec.Unlock();	// ------------------------------------------------

	return bRes;
}


/* GetFirmware
 * ----------------------------------------------------------------------------
 */
DWORD CConnection::GetFirmware(void)
{
	return m_dwFirmware;
}


/* RequestBPSync
 * ----------------------------------------------------------------------------
 */
void CConnection::RequestBPSync(BOOL bRequestBPSync)
{
	m_bRequestBPSync = bRequestBPSync;
}


/* GetResourceState
 * ----------------------------------------------------------------------------
 */
DWORD CConnection::GetResourceState(void)
{
    return m_resourceState;
}


/* TraceCommError
 * ----------------------------------------------------------------------------
 */
void CConnection::TraceCommError(UINT uCmd, LPBYTE lpData, UINT cbData)
{
    IEC_UINT uErrNo = (IEC_UINT)-1;
    if (cbData>=sizeof(IEC_UINT) && lpData != NULL)
        uErrNo = TRANSLATE_SHORT( *(IEC_UINT*)lpData);

	uCmd &= 0x7fu;

    FC_DEBUGPRINT2(_T("TRANSFER> %s: error=%04x\n"), uCmd <= LAST_CMD_VALUE ? szCmd[uCmd] : _T("Invalid"), uErrNo);
    TrcPrint(TRC_COMM_EVENT, _T("TRANSFER> %s: error=%04x\n"), uCmd <= LAST_CMD_VALUE ? szCmd[uCmd] : _T("Invalid"), uErrNo);
}


/* TraceComm
 * ----------------------------------------------------------------------------
 */
void CConnection::TraceComm(LPCTSTR lpMsg)
{
    FC_DEBUGPRINT(lpMsg);
    TrcPrint(TRC_COMM_EVENT,lpMsg);
}


/* StartConnectThread
 * ----------------------------------------------------------------------------
 */
BOOL CConnection::StartConnectThread()
{
	assert(g_StartParam != NULL);
	
	BOOL bRes;
	
	Sleep(500);

	m_dwFirmware = 0;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		bRes = m_CommObj.Construct();
	
		if (bRes != TRUE)
		{
			m_CritSec.Unlock();

			return FALSE;
		}

		m_dwFirmware	= m_CommObj.GetFirmware();

		m_bShutdown		= FALSE;
		m_bConnected	= FALSE;
    
	} m_CritSec.Unlock();	// ------------------------------------------------

	InitGUID();
    
	m_hThread = CreateThread(NULL, 0, ThreadProc, this, 0, &m_ThreadID);
            
    return m_hThread != NULL;
}


/* StartConnectThread
 * ----------------------------------------------------------------------------
 */
BOOL CConnection::StopConnectThread()
{
	m_CritSec.Lock();	// ----------------------------------------------------
	{
		m_bShutdown	 = TRUE;
		m_bConnected = FALSE;

	} m_CritSec.Unlock();	// ------------------------------------------------

    if (m_hThread == NULL)
	{
        return TRUE;
	}

    WaitForSingleObject(m_hThread, 10000);

    CloseHandle(m_hThread);
    
	m_hThread	= NULL;
    m_ThreadID	= 0;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		m_dwFirmware = 0;

		m_CommObj.Dispose();
		
	} m_CritSec.Unlock();	// ------------------------------------------------

    return TRUE;
}


/* ThreadProc
 * ----------------------------------------------------------------------------
 */
DWORD WINAPI CConnection::ThreadProc(LPVOID lpParam)
{
    CConnection *_this = (CConnection *)lpParam;
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    _this->ThreadMain();
    
	CoUninitialize();

    return 0;
}


/* ThreadMain
 * ----------------------------------------------------------------------------
 */
void CConnection::ThreadMain()
{
    TraceComm(_T("CONNECT> ConnectionThread starting...\n"));

	static bFirst = TRUE;
	
    while (! m_bShutdown)
    {
		TraceComm(_T("CONNECT> Trying to connect...\n"));
		
		if (m_CommObj.OpenComm()) 
		{
			if (m_CommObj.Connect()) 
			{
				TraceComm(_T("CONNECT> Connection established\n"));
				
				// Request BP from Eng. is default ...
				m_bRequestBPSync = TRUE;

				if (bFirst) 
				{
					m_CommObj.SendMsgToEng(_T("<done>       Connected to control"));
					bFirst			 = FALSE;
					m_bRequestBPSync = FALSE; // ... except on the first time!
				}

				m_CritSec.Lock();	// ----------------------------------------
				{
					UINT cbData = sizeof(GUID);
					BYTE *pData = NULL;
					
					HEAP_ALLOC(pData, cbData);
					memcpy(pData, &m_GUID, sizeof(GUID));
					
					BYTE byCommand = CMD_LOGIN;
					HRESULT hOK = SendDirect(&byCommand, &pData, &cbData);
					
					HEAP_FREE(pData, cbData);
				
					m_bLogon	 = SUCCEEDED(hOK) && byCommand == CMD_LOGIN;
					m_bConnected = SUCCEEDED(hOK);
				
				} m_CritSec.Unlock();	// ------------------------------------
				
				BOOL bDebugSession = FALSE;
				
				while (m_bConnected && !m_bShutdown)
				{
					if (m_bDebugSession != bDebugSession) 
					{
						// Debug connection state has changed.

						bDebugSession = m_bDebugSession;
						SendCommand(FALSE, (BYTE)(bDebugSession ? CMD_OPEN_DEBUG_SESSION : CMD_CLOSE_DEBUG_SESSION), NULL, 0);

						if (bDebugSession == TRUE && m_bRequestBPSync == TRUE)
						{
							// Only when a debug session is created and sync. requested.
							// (Normally only when a existing debug connection is 
							// re-established.)
							m_bRequestBPSync = TRUE;
						}
						else
						{
							m_bRequestBPSync = FALSE;
						}
					}
					
					if (m_bConnected == TRUE && m_bConfigChanged == TRUE)
					{
						m_bConfigChanged = FALSE;
						m_bRequestBPSync = FALSE;

						CCommissioningMgt::ibnConfigurationChanged();
					}

					CheckMessages();
					
					if (m_bRequestBPSync == TRUE)
					{
						m_bRequestBPSync = FALSE;

						CCommissioningMgt::ibnConfigurationChanged();
					}

					Sleep(1000);

					if (CheckCommParams()) 
					{
						break;
					}
				
				} /* while (m_bConnected && !m_bShutdown) */
				
				TraceComm(_T("CONNECT> DisConnecting...\n"));
				
				m_CritSec.Lock();	// ----------------------------------------
				{
					m_bConnected = FALSE;
					m_CommObj.Disconnect();
				
				} m_CritSec.Unlock();	// ------------------------------------
				
			} /* if (m_CommObj.Connect()) */

			else 
			{
				m_CommObj.SendErrToEng(OPC_ERR_CONNECT);
				Sleep(1000);
			}

			m_CritSec.Lock();	// --------------------------------------------
			{
				m_CommObj.CloseComm();
			
			} m_CritSec.Unlock();	// ----------------------------------------
			
		} /* if (m_CommObj.OpenComm()) */

		else 
		{
			m_CommObj.SendErrToEng(OPC_ERR_OPENCOMM);
			Sleep(1000);
		}
		
		CheckCommParams();
    
	} /* while (! m_bShutdown) */

    TraceComm(_T("CONNECT> ConnectionThread terminating\n"));
}


/* Get4CLayer
 * ----------------------------------------------------------------------------
 * Danger, experts only!
 */
C4CLayer *CConnection::Get4CLayer(void)
{
	// Sync & pray!
	m_CritSec.Lock();
	m_CritSec.Unlock();
	
	return m_CommObj.Get4CLayer();
}

/* ---------------------------------------------------------------------------- */
