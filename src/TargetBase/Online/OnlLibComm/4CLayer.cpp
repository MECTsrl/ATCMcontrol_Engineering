/* $Header: /4CReleased/V2.20.00/TargetBase/Online/OnlLibComm/4CLayer.cpp 3     19.04.07 13:14 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: 4CLayer.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/Online/OnlLibComm/4CLayer.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibComm
 *
 * =CURRENT 	 $Date: 19.04.07 13:14 $
 *			 $Revision: 3 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "4CLayer.h"
#include <WinSock2.h>

#include "CommPara.h"
#include "LogLayer.h"
#include "PhysLayer.h"

#include "vmSharedDef.h"

//----  Static Initializations:   ----------------------------------*

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(C4CLayer, CObject)

static LPCTSTR szCmd[] = CMD_TEXT_LARGE;


/* C4CLayer
 * ----------------------------------------------------------------------------
 */
C4CLayer::C4CLayer()
{
	m_pLog  = NULL;
	m_pCrit = new CCriticalSection();

	memset(&m_Para, 0x00, sizeof(m_Para));
}

/* ~C4CLayer
 * ----------------------------------------------------------------------------
 */
C4CLayer::~C4CLayer()
{
	delete m_pLog;
	m_pLog = NULL;

	delete m_pCrit;
	m_pCrit = NULL;
}

/* Initialize
 * ----------------------------------------------------------------------------
 */
BOOL C4CLayer::Initialize(CLogLayer *pLog, SCommPara *pPara, LPCTSTR cszConnect)
{
	if (pLog == NULL || pPara == NULL)
	{
	    TrcPrint(TRC_ERROR, _T("[C4CLayer][Initialize] Null Pointer.\n"));
		return FALSE;
	}

	m_pLog = pLog;
	memcpy(&m_Para, pPara, sizeof(m_Para));
	m_sConnect.Empty();

	if (cszConnect != NULL)
	{
		m_sConnect = cszConnect;
	}

	return TRUE;
}

/* IsConnected
 * ----------------------------------------------------------------------------
 */
BOOL C4CLayer::IsConnected()
{
	ASSERT(m_pLog);

	return m_pLog->IsConnected() == ceOK;
}

/* IsCommOpen
 * ----------------------------------------------------------------------------
 */
BOOL C4CLayer::IsCommOpen()
{
	ASSERT(m_pLog);

	return m_pLog->IsCommOpen() == ceOK;
}

/* GetCommPara
 * ----------------------------------------------------------------------------
 */
SCommPara *C4CLayer::GetCommPara()
{
	return &m_Para;
}

/* OpenComm
 * ----------------------------------------------------------------------------
 */
BOOL C4CLayer::OpenComm()
{
	ASSERT(m_pLog);

	return m_pLog->OpenComm() == ceOK;
}

/* CloseComm
 * ----------------------------------------------------------------------------
 */
BOOL C4CLayer::CloseComm()
{
	ASSERT(m_pLog);

	ECommError eError = m_pLog->CloseComm(); 
	
	SleepEx(500, TRUE);

	return eError == ceOK;
}

/* Connect
 * ----------------------------------------------------------------------------
 */
BOOL C4CLayer::Connect()
{
	ASSERT(m_pLog);

	BOOL bRes = m_pLog->Connect() == ceOK;

	SleepEx(500,TRUE);

	return bRes;
}

/* Disconnect
 * ----------------------------------------------------------------------------
 */
BOOL C4CLayer::Disconnect()
{
	ASSERT(m_pLog);

	return m_pLog->Disconnect() == ceOK;
}

/* Transfer
 * ----------------------------------------------------------------------------
 */
BOOL C4CLayer::TransferData(BYTE *bypCommand, BYTE **ppData, UINT *upLen)
{
	ASSERT(m_pLog);

	ECommError eError = ceOK;

	m_pCrit->Lock();
	{
		// Only one communication at one time!

		eError = m_pLog->Send(*bypCommand, *ppData, *upLen);

		HEAP_FREE(*ppData, *upLen);

		if (eError == ceOK)
		{
			eError = m_pLog->Receive(bypCommand, ppData, upLen);
		}
	
	} m_pCrit->Unlock();

	return eError == ceOK;
}

/* Transfer
 * ----------------------------------------------------------------------------
 */
BOOL C4CLayer::Transfer(BYTE *bypCommand, BYTE **ppData, UINT *upLen)
{
	ASSERT(m_pLog);

	BOOL bRes = FALSE;

	switch (*bypCommand)
	{
		case CMD_GET_STATE :
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_NOT_IMPLEMENTED :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_LOGIN :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_LOGOUT :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_WARM_START :			
			m_pLog->GetPhysLayer()->SetRecvTimeOut(m_Para.dwColdWarmTO);
			bRes = TransferData(bypCommand, ppData, upLen);
			m_pLog->GetPhysLayer()->SetRecvTimeOut(m_Para.dwRecvTimeOut);
			break;

		case CMD_COLD_START :			
			m_pLog->GetPhysLayer()->SetRecvTimeOut(m_Para.dwColdWarmTO);
			bRes = TransferData(bypCommand, ppData, upLen);
			m_pLog->GetPhysLayer()->SetRecvTimeOut(m_Para.dwRecvTimeOut);
			break;

		case CMD_START_ALL_TASKS :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_STOP_ALL_TASKS :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_START_TASK :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_STOP_TASK :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_OPEN_DEBUG_SESSION :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_CLOSE_DEBUG_SESSION :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_SET_BREAKPOINT :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_CLEAR_BREAKPOINT:			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_CLEAR_ALL_BREAKPOINTS :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_SINGLE_STEP :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_CONTINUE :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_GET_VALUE :			
		{
			if (m_Para.dwFirmware < 21000)
			{
				bRes = GetVal20(bypCommand, ppData, upLen);
			}
			else
			{
				bRes = GetVal21(bypCommand, ppData, upLen);
			}

			break;
		}

		case CMD_SET_VALUE :			
		{
			UINT uLen		= 0;
			BYTE *pData		= NULL;

			UINT uToCopy	= 0;
			UINT uCount		= 0;

			XValue *pVal    = NULL;

			if (m_Para.dwFirmware <= 20500)
			{
				// Split to single commands

				for (UINT i = 0; uCount < *upLen; i++)
				{
					pVal = (XValue *)(*ppData + uCount);

					uLen	= HD_VALUE + TtoH(pVal->VAR.uLen);
					uToCopy	= uLen;
					
					HEAP_ALLOC(pData, uLen);
					memcpy(pData, pVal, uLen);

					if ((bRes = TransferData(bypCommand, &pData, &uLen)) == FALSE)
					{
						// Communication error
						HEAP_FREE(pData, uLen);
						break;
					}

					if (*bypCommand & 0x80)
					{
						// RTS error
						break;
					}
					
					HEAP_FREE(pData, uLen);

					uCount += uToCopy;
				}
			
			} /* if (m_Para.dwFirmware <= 20500) */
			else
			{				
				// VMM can handle multiple SetValue's

				if (*upLen <= m_Para.uBlockLen)
				{
					// Command and response fits in one block
					return TransferData(bypCommand, ppData, upLen);
				}

				while (uCount < *upLen)
				{
					uLen = 0;

					HEAP_ALLOC(pData, m_Para.uBlockLen);

					while (uLen < m_Para.uBlockLen && uCount < *upLen)
					{
						/* Build block
						 */

						pVal = (XValue *)(*ppData + uCount);

						uToCopy = HD_VALUE + TtoH(pVal->VAR.uLen);

						if (uLen + uToCopy > m_Para.uBlockLen)
						{
							break;
						}

						memcpy(pData + uLen, *ppData + uCount, uToCopy);

						uLen	+= uToCopy;
						uCount	+= uToCopy;
					}

					/* Transfer block
					 */
					if ((bRes = TransferData(bypCommand, &pData, &uLen)) == FALSE)
					{
						// Communication error
						HEAP_FREE(pData, uLen);
						break;
					}

					if (*bypCommand & 0x80u)
					{
						// RTS error
						break;
					}

					HEAP_FREE(pData, uLen);

				} /* while (uCount < *upLen) */

			} /* else (m_Para.dwFirmware <= 20500) */

			HEAP_FREE(*ppData, *upLen);

			*ppData = pData;
			*upLen	= uLen;

			break;
		}

		case CMD_DOWNLOAD_CONFIG :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_DOWNLOAD_INITIAL:			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_DOWNLOAD_CODE  :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_DOWNLOAD_CUSTOM :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_DOWNLOAD_FINISH:			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_START_RESOURCE :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_STOP_RESOURCE :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_DOWNLOAD_END :			
			m_pLog->GetPhysLayer()->SetRecvTimeOut(m_Para.dwDownEndRecvTO);
			bRes = TransferData(bypCommand, ppData, upLen);
			m_pLog->GetPhysLayer()->SetRecvTimeOut(m_Para.dwRecvTimeOut);
			break;

		case CMD_DOWNLOAD_BEGIN :
			m_pLog->GetPhysLayer()->SetRecvTimeOut(m_Para.dwDownBegRecvTO);
			bRes = TransferData(bypCommand, ppData, upLen);
			m_pLog->GetPhysLayer()->SetRecvTimeOut(m_Para.dwRecvTimeOut);
			break;

		case CMD_INITIALIZE_CLEAR :			
			m_pLog->GetPhysLayer()->SetRecvTimeOut(m_Para.dwClearResRecvTO);
			bRes = TransferData(bypCommand, ppData, upLen);
			m_pLog->GetPhysLayer()->SetRecvTimeOut(m_Para.dwRecvTimeOut);
			break;

		case CMD_CLEAR_FLASH :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_DBI_GET_CHILDREN :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_DBI_GET_ADDRESS  :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_DOWNLOAD_DEBUG :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_DBI_GET_TASKNR :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_GET_PROJ_VERSION :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_GET_PROJ_INFO :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_LOAD_PROJECT :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_SAVE_PROJECT :	
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_CUSTOM :
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_LOAD_FILE :
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_SAVE_FILE :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_DIR_CONTENT :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_OC_BEGIN :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_OC_CODE :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_OC_DEBUG :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_OC_CUSTOM :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_OC_INITIAL :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_OC_COMMIT :			
			m_pLog->GetPhysLayer()->SetRecvTimeOut(m_Para.dwOCCommitRecvTO);
			bRes = TransferData(bypCommand, ppData, upLen);
			m_pLog->GetPhysLayer()->SetRecvTimeOut(m_Para.dwRecvTimeOut);
			break;

		case CMD_OC_END :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_OC_CONFIG :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_OC_FINISH  :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_FLASH :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_DEL_FILE :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_GET_CONFIG :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_RET_WRITE :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;

		case CMD_RET_CYCLE :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;
	
		case CMD_DOWNLOAD_IOL :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;
	
		case CMD_DOWNLOAD_CLEAR :			
			bRes = TransferData(bypCommand, ppData, upLen);
			break;
	
		default:
			bRes = TransferData(bypCommand, ppData, upLen);
			break;
	
	} /* switch (*bypCommand) */

	return bRes;
}


/* GetVal20
 * ----------------------------------------------------------------------------
 */
BOOL C4CLayer::GetVal20(BYTE *bypCommand, BYTE **ppData, UINT *upLen)
{
	UINT uResultLen = 0;
	XVariable *pSrc = (XVariable *)*ppData;
	
	for (UINT i = 0; i < *upLen / sizeof(XVariable); i++)
	{
		uResultLen += TtoH(pSrc->uLen);
		pSrc++;
	}
	
	if (*upLen <= m_Para.uBlockLen && uResultLen <= m_Para.uBlockLen)
	{
		// Command and response fits in one block
		return TransferData(bypCommand, ppData, upLen);
	}
	
	BYTE *pResult = NULL;
	UINT uResult  = 0;
	
	UINT uResLen = 0;
	UINT uCmdLen = 0;
	pSrc = (XVariable *)*ppData;
	
	BYTE	  *pData = NULL;
	XVariable *pDes  = NULL;
	
	BOOL bOK = OK;
	
	for (i = 0; i < *upLen / sizeof(XVariable); i++)
	{
		if (pData == NULL)
		{
			// Alloc new buffer
			HEAP_ALLOC(pData, m_Para.uBlockLen);
			pDes = (XVariable *)pData;
		}
		
		uCmdLen += sizeof(XVariable);
		uResLen += TtoH(pSrc->uLen);
		
		memcpy(pDes, pSrc, sizeof(XVariable));
		
		pDes++;
		pSrc++;
		
		if (   uCmdLen + sizeof(XVariable) > m_Para.uBlockLen	// Block full for download
			|| uResLen + TtoH(pSrc->uLen)  > m_Para.uBlockLen	// Block full for upload
			|| i == (*upLen / sizeof(XVariable) - 1))			// Last block (must always fit)
		{
			bOK = TransferData(bypCommand, &pData, &uCmdLen);
			if (! bOK || uResult + uCmdLen > uResultLen)
			{
				// Communication error
				HEAP_FREE(pData, uCmdLen);
				HEAP_FREE(pResult, uResult);
				break;
			}
			
			if (*bypCommand & 0x80)
			{
				// RTS error
				HEAP_FREE(pResult, uResult);
				
				pResult = pData;
				uResult = uCmdLen;
				
				break;
			}
			
			if (pResult == NULL)
			{
				// Alloc result buffer (for all values)
				HEAP_ALLOC(pResult, uResultLen);
			}
			
			memcpy(pResult + uResult, pData, uCmdLen);
			uResult += uCmdLen;
			
			HEAP_FREE(pData, uCmdLen);
			
			uResLen = 0;
			uCmdLen = 0;
		}
	}
	
	HEAP_FREE(*ppData, *upLen);
	
	*ppData = pResult;
	*upLen	= uResult;
	
	return bOK;
}


/* GetVal21
 * ----------------------------------------------------------------------------
 */
BOOL C4CLayer::GetVal21(BYTE *bypCommand, BYTE **ppData, UINT *upLen)
{
	UINT uVarCount	= 0;
	UINT uResultLen = 0;

	BYTE *pData	= *ppData;
	UINT uLen	= *upLen;

	while(uLen > 0)
	{
		/* Compute expected result data size
		 */
		switch(*pData & VMM_XV_TYPEMASK)
		{
			case VMM_XV_SMALL:
			{
				XVariableS *pxVar = (XVariableS *)pData;

				if (uLen < sizeof(XVariableS))
				{
					return FALSE;
				}
				uResultLen	+= pxVar->usLen == 0 ? 1 : pxVar->usLen;
				pData		+= sizeof(XVariableS);
				uLen		-= sizeof(XVariableS);
				break;
			}

			case VMM_XV_MEDIUM:
			{
				XVariableM *pxVar = (XVariableM *)pData;

				if (uLen < sizeof(XVariableM))
				{
					return FALSE;
				}
				uResultLen	+= pxVar->uLen == 0 ? 1 : TtoH(pxVar->uLen);
				pData		+= sizeof(XVariableM);
				uLen		-= sizeof(XVariableM);
				break;
			}

			case VMM_XV_LARGE:
			{
				XVariableL *pxVar = (XVariableL *)pData;

				if (uLen < sizeof(XVariableL))
				{
					return FALSE;
				}
				uResultLen	+= pxVar->uLen == 0 ? 1 : TtoH(pxVar->uLen);
				pData		+= sizeof(XVariableL);
				uLen		-= sizeof(XVariableL);
				break;
			}

			default:
			{
				return FALSE;
			}
		}

		uVarCount++;
	}
	
	if (*upLen <= m_Para.uBlockLen && uResultLen <= m_Para.uBlockLen)
	{
		/* Command and response fits into one block
		 */
		return TransferData(bypCommand, ppData, upLen);
	}
	
	
	BYTE *pResult = NULL;
	UINT uResult  = 0;
	
	UINT uResLen = 0;
	UINT uCmdLen = 0;
	
	BYTE *pBuffer = NULL;
	
	BOOL bOK = OK;

	pData = *ppData;
	
	for (UINT i = 0; i < uVarCount; i++)
	{
		if (pBuffer == NULL)
		{
			// Alloc new buffer
			HEAP_ALLOC(pBuffer, m_Para.uBlockLen);
		}

		switch(*pData & VMM_XV_TYPEMASK)
		{
			case VMM_XV_SMALL:
			{		
				XVariableS *pxVar = (XVariableS *)pData;
				
				memcpy(pBuffer + uCmdLen, pData, sizeof(XVariableS));
				
				uCmdLen += sizeof(XVariableS);
				uResLen += pxVar->usLen == 0 ? 1 : pxVar->usLen;
				pData	+= sizeof(XVariableS);
				
				break;
			}

			case VMM_XV_MEDIUM:
			{		
				XVariableM *pxVar = (XVariableM *)pData;
				
				memcpy(pBuffer + uCmdLen, pData, sizeof(XVariableM));
				
				uCmdLen += sizeof(XVariableM);
				uResLen += pxVar->uLen == 0 ? 1 : TtoH(pxVar->uLen);
				pData	+= sizeof(XVariableM);
				
				break;
			}

			case VMM_XV_LARGE:
			{		
				XVariableL *pxVar = (XVariableL *)pData;
				
				memcpy(pBuffer + uCmdLen, pData, sizeof(XVariableL));
				
				uCmdLen += sizeof(XVariableL);
				uResLen += pxVar->uLen == 0 ? 1 : TtoH(pxVar->uLen);
				pData	+= sizeof(XVariableL);
				
				break;
			}

			default:
			{
				return FALSE;
			}
		}

		UINT uNextCmdLen = 0;
		UINT uNextResLen = 0;

		if (i < uVarCount - 1)
		{
			switch(*pData & VMM_XV_TYPEMASK)
			{
				case VMM_XV_SMALL:
				{		
					XVariableS *pxVar = (XVariableS *)pData;

					uNextCmdLen = sizeof(XVariableS);
					uNextResLen = pxVar->usLen == 0 ? 1 : pxVar->usLen;
									
					break;
				}

				case VMM_XV_MEDIUM:
				{		
					XVariableM *pxVar = (XVariableM *)pData;

					uNextCmdLen = sizeof(XVariableM);
					uNextResLen = pxVar->uLen == 0 ? 1 : TtoH(pxVar->uLen);
					
					break;
				}

				case VMM_XV_LARGE:
				{		
					XVariableL *pxVar = (XVariableL *)pData;

					uNextCmdLen = sizeof(XVariableL);
					uNextResLen = pxVar->uLen == 0 ? 1 : TtoH(pxVar->uLen);
					
					break;
				}

				default:
				{
					return FALSE;
				}
			}
		}

		if (   uCmdLen + uNextCmdLen  > m_Para.uBlockLen	// Block full for download
			|| uResLen + uNextResLen  > m_Para.uBlockLen	// Block full for upload
			|| i == uVarCount - 1)							// Last block (must always fit)
		{
			bOK = TransferData(bypCommand, &pBuffer, &uCmdLen);
			if (! bOK || uResult + uCmdLen > uResultLen)
			{
				// Communication error
				HEAP_FREE(pBuffer, uCmdLen);
				HEAP_FREE(pResult, uResult);
				break;
			}
			
			if (*bypCommand & 0x80)
			{
				// RTS error
				HEAP_FREE(pResult, uResult);
				
				pResult = pBuffer;
				uResult = uCmdLen;
				
				break;
			}
			
			if (pResult == NULL)
			{
				// Alloc result buffer (for all values)
				HEAP_ALLOC(pResult, uResultLen);
			}
			
			memcpy(pResult + uResult, pBuffer, uCmdLen);
			uResult += uCmdLen;
			
			HEAP_FREE(pBuffer, uCmdLen);
			
			uResLen = 0;
			uCmdLen = 0;
		}
	}
	
	HEAP_FREE(*ppData, *upLen);
	
	*ppData = pResult;
	*upLen	= uResult;
	
	return bOK;
}


/* Transfer
 * ----------------------------------------------------------------------------
 */
BOOL C4CLayer::Transfer(BYTE *bypCommand, LPCTSTR cszFile, BYTE **ppData, UINT *upLen)
{
	ASSERT(m_pLog);

	HANDLE hFile = CreateFile(cszFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if (*ppData != NULL)
	{
		HeapFree(GetProcessHeap(), 0, *ppData);
	}

	*upLen  = GetFileSize(hFile, NULL);
	*ppData = (BYTE *)HeapAlloc(GetProcessHeap(), 0, *upLen);

	DWORD dwRead;

	if (! ReadFile(hFile, *ppData, *upLen, &dwRead, NULL))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	CloseHandle(hFile);

	if (*upLen != dwRead)
	{
		return FALSE;
	}

	return Transfer(bypCommand, ppData, upLen);
}

/* GetMsgString
 * ----------------------------------------------------------------------------
 */
BOOL C4CLayer::GetMsgString(BYTE byCmd, BYTE *pData, UINT uLen, BSTR *pMessage)
{
	if (m_Para.dwFirmware < 20500)
	{
		/* Only for firmware starting with V2.05
		 */
		return FALSE;
	}

    BOOL bFail = (byCmd & 0x80) != 0;

    byCmd &= 0x7F;

    USHORT uErrNo = 0;
	if (bFail && uLen >= sizeof(USHORT) && pData != NULL) 
	{
        uErrNo = *(USHORT*)pData;
        if (m_Para.bBigEndian)
            uErrNo = (USHORT)(uErrNo >> 8 | uErrNo << 8);
    }

	CString sRes;

	if (! bFail)
	{
		switch(byCmd)
		{
			case CMD_INITIALIZE_CLEAR:
			case CMD_CLEAR_FLASH:
			case CMD_OC_COMMIT:
			{
				if (m_Para.dwFirmware < 22000 || uLen != sizeof(USHORT) || *(USHORT *)pData == 0)
				{
					FormatOK(byCmd, pData, uLen, sRes);
				}
				else
				{
					FormatContinued(byCmd, pData, uLen, sRes);
				}

				break;
			}

			case CMD_DOWNLOAD_IOL:
			case CMD_DOWNLOAD_CLEAR:
			{
				if (uLen != sizeof(USHORT) || *(USHORT *)pData == 0)
				{
					FormatOK(byCmd, pData, uLen, sRes);
				}
				else
				{
					FormatContinued(byCmd, pData, uLen, sRes);
				}

				break;
			}

			default:
			{
				FormatOK(byCmd, pData, uLen, sRes);
				break;
			}
		}
	}
	else
	{
		FormatError(byCmd, uErrNo, pData, uLen, sRes);
	}

	if (! sRes.IsEmpty())
	{
		if (bFail)
		{
			TRACE(sRes);
			TrcPrint(TRC_ERROR, sRes);
		}

		*pMessage = sRes.AllocSysString();
		return TRUE;
	}

	return FALSE; // not yet implemented...
}

/* FormatOK
 * ----------------------------------------------------------------------------
 */
void C4CLayer::FormatOK(BYTE byCmd, BYTE *pData, UINT uLen, CString &sRes)
{
	if (byCmd > LAST_CMD_VALUE)
	{
		byCmd = CMD_NOT_IMPLEMENTED;
	}

	switch(byCmd)
	{
		case CMD_GET_STATE:
		case CMD_SET_VALUE:
		case CMD_GET_VALUE:
		{
			break;
		}

		default:
		{
			if (byCmd <= LAST_CMD_VALUE)
			{
				sRes.Format(FOK_CMD, szCmd[byCmd]);
			}
			else
			{
				sRes.Format(FOK_CMD, _T("<unkown>"));
			}
			break;
		}
	}
}

/* FormatContinued
 * ----------------------------------------------------------------------------
 */
void C4CLayer::FormatContinued(BYTE byCmd, BYTE *pData, UINT uLen, CString &sRes)
{
	if (byCmd > LAST_CMD_VALUE)
	{
		byCmd = CMD_NOT_IMPLEMENTED;
	}

	switch(byCmd)
	{
		case CMD_GET_STATE:
		case CMD_SET_VALUE:
		case CMD_GET_VALUE:
		{
			break;
		}

		default:
		{
			if (byCmd <= LAST_CMD_VALUE)
			{
				sRes.Format(FCT_CMD, szCmd[byCmd]);
			}
			else
			{
				sRes.Format(FCT_CMD, _T("<unkown>"));
			}
			break;
		}
	}
}

/* FormatError
 * ----------------------------------------------------------------------------
 */
void C4CLayer::FormatError(BYTE byCmd, UINT uErrNo, BYTE *pData, UINT uLen, CString &sRes)
{
	if (byCmd > LAST_CMD_VALUE)
	{
		byCmd = CMD_NOT_IMPLEMENTED;
	}

	switch(byCmd)
	{
		case CMD_GET_VALUE:
		{
			if (uErrNo == ERR_NO_PROJECT)
			{
				return;
			}

			break;
		}

		default:
			break;
	}

	switch(uErrNo)
	{
	/* Warnings
	 */
	case WRN_TIME_OUT			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Time Out"));												break;				
	case WRN_NO_CONNECTION		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("No connection to OPC server"));								break;
	case WRN_HANDLED			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Command already handled"));									break;
	case WRN_BREAK				:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Task is in breakpoint"));									break;
	case ERR_CRC				:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Checksum calculation failed"));								break;
	case WRN_DIVIDED			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Command divided"));											break;
	case WRN_TRUNCATED			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Data truncated"));											break;
	case WRN_NO_MESSAGE			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("No message pending in message queue"));						break;
	case WRN_RETAIN_INVALID		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Retain memory doesn't match current project"));				break;
	case WRN_TASK_HALTED		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Task halted immediately"));									break;
	case WRN_IN_PROGRESS 		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Process still in progress"));								break;
	case WRN_NOT_HANDLED 		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Object not yet handled"));									break;
	case WRN_FILE_NOT_EXIST		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("File does not exist"));										break;

	/* Common error messages
	 */
	case ERR_INIT				:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Task initializations failed"));								break;
	case ERR_INVALID_TASK		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid task number"));										break;
	case ERR_INVALID_COMMAND	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid command received"));								break;
	case ERR_INVALID_DATA		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid data received"));									break;
	case ERR_INVALID_OFFSET		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid object offset"));									break;
	case ERR_INVALID_INSTANCE	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid instance object"));									break;
	case ERR_INVALID_CLASS		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid class ID"));										break;
	case ERR_INVALID_DATA_SIZE	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Data size not valid"));										break;
	case ERR_QUEUE_FULL			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Message or breakpoint queue full"));						break;
	case ERR_OUT_OF_MEMORY		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Out of memory"));											break;
	case ERR_DEBUG_MODE			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Not in debug mode"));										break;
	case ERR_LOGIN				:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Not logged in"));											break;
	case ERR_NO_PROJECT			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("No project loaded"));										break;
	case ERR_CRITICAL_SECTION	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error entering a critical section"));						break;
	case ERR_ACTIVATE_TIMER		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error activating a VM timer"));								break;
	case ERR_WRONG_PROJECT		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Project conflict. New download required"));					break;
	case ERR_TO_MANY_VARS		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("To many simultanous variables. Buffer overrun"));			break;
	case ERR_INVALID_PARAM		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid parameter"));										break;
	case ERR_CREATE_QUEUE		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Unable to create IPC queue"));								break;
	case ERR_CREATE_TIMER		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Unable to create VM timer"));								break;
	case ERR_IPC_RECV_FAILED	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Unable to receive a message on a IPC queue"));				break;
	case ERR_IPC_SEND_FAILED	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Unable to send a message on a IPC queue"));					break;
	case ERR_IPC_VMM_TIMEOUT	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Time Out waiting for VMM"));								break;
	case ERR_ALREADY_IN_DEBUG	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Already in Debug Mode. Only one Debug Connection possible"));	break;
	case ERR_INVALID_SEGMENT	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid object segment received"));							break;
	case ERR_IO_READ			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error reading from communication device"));					break;
	case ERR_IO_WRITE			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error writing to communication device"));					break;
	case ERR_NOT_CONNECTED		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Not connected"));											break;
	case ERR_INVALID_MSG		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid message received"));								break;
	case ERR_WRITE_TO_INPUT		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Write access to input segment not supported"));				break;
	case ERR_TO_MUCH_DATA		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("To much data received - unable to handle"));				break;
	case ERR_NOT_SUPPORTED		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Requested function/command/operation not supported"));		break;
	case ERR_UNEXPECTED_MSG		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Unexpected message (out of order) received"));				break;
	case ERR_ERRMSG_RECEIVED	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("(Unexpected) Error message received"));						break;
	case ERR_INVALID_QUEUE		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid message queue specified"));							break;
	case ERR_UNKNOWN_PRODUCT	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Unknown ATCMControl product ID provided"));					break;
	case ERR_NOT_READY			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Not ready to execute function/command/operation now"));		break;
	case ERR_INVALID_IOLAYER	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid IO layer ID provided"));							break;
	case ERR_NOT_LICENSED		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Feature not licensed."));									break;
	case ERR_INVALID_LICENSE	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid license key received"));							break;
	case ERR_INVALID_PRODUCT	: 	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid product number received"));							break;
	case ERR_NULL_POINTER		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Null pointer as parameter"));								break;
	case ERR_NOT_CONFIGURED		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Not configured"));											break;


	/* Download
	 */
	case ERR_INVALID_DOMAIN		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid download domain"));									break;
	case ERR_INVALID_VERSION	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid version"));											break;
	case ERR_OVERRUN_TASK		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("To many tasks specified"));									break;
	case ERR_OVERRUN_INSTANCE	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("To many instance objects"));								break;
	case ERR_OVERRUN_CLASS		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("To many classes (POU's)"));									break;
	case ERR_OVERRUN_PROGRAM	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("To many programs per task"));								break;
	case ERR_OVERRUN_CODESEG	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Code segment full"));										break;
	case ERR_OVERRUN_OBJSEG		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Data segment full"));										break;
	case ERR_OVERRUN_REGION		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("To many read/write memory regions"));						break;
	case ERR_ENTRIES_MISSING	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Not all expected items received"));							break;
	case ERR_INVALID_SEG_INIT	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Data segment initializations out of order"));				break;
	case ERR_CREATE_TASK		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error creating a task or thread"));							break;
	case ERR_OVERRUN_COPYSEG	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("To many local retain copy segments"));						break;
	case ERR_BUFFER_TOO_SMALL	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Temporary buffer is too small"));							break;

	/* Breakpoints
	 */
	case ERR_INVALID_CODE_POS	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid code position"));									break;
	case ERR_BP_NOT_FOUND		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Breakpoint not found in breakpoint list"));					break;
	case ERR_BP_LIST_FULL		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Breakpoint list full"));									break;
	case ERR_TASK_NOT_IN_BREAK	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Task is not in breakpoint state"));							break;

	/* File I/O
	 */
	case ERR_FILE_INIT			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("File library not initialized"));							break;
	case ERR_FILE_MAX			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("To many open files"));										break;
	case ERR_FILE_WRONG_ID		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid file ID used"));									break;
	case ERR_FILE_EOF			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("End of file reached"));										break;
	case ERR_FILE_OPEN			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error opening a file"));									break;
	case ERR_FILE_CLOSE			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error closing a file"));									break;
	case ERR_FILE_READ			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error reading from file"));									break;
	case ERR_FILE_WRITE			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error writing to file"));									break;
	case ERR_FILE_SYNC			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Temporary file closed by another process"));				break;
	case ERR_FILE_NOT_EXIST		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("File does not exist"));										break;
	case ERR_FILE_SEEK			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error setting file position"));								break;
	case ERR_FILE_RENAME		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error renaming a file"));									break;
	case ERR_FILE_REMOVE		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error deleting a file"));									break;
	case ERR_INVALID_DRIVE		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid drive specification"));								break;
	case ERR_CHANGE_DRIVE		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error changing drive"));									break;
	case ERR_CHANGE_DIR			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error changing directory"));								break;
	case ERR_INVALID_PATH		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid directory name"));									break;
	case ERR_INVALID_FILE_NAME	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid file name"));										break;
	case ERR_PATH_IS_NOT_8_3	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("File path is not in 8.3 format"));							break;
	case ERR_PATH_TO_LONG		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("File path to long"));										break;
	case ERR_FILE_INVALID		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("The given file is invalid"));								break;
	case ERR_FILE_NOT_OPEN		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("File is not open"));										break;
											   
	/* Debug Interface
	 */
	case ERR_DBI_INIT			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Debug library not initialized"));							break;
	case ERR_DBI_PARAM			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Parameter not valid"));										break;
	case ERR_DBI_OBJ_NOT_FOUND	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Object not found"));										break;
	case ERR_DBI_FILE_FORMAT	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Debug file invalid formatted"));							break;
	case ERR_DBI_RETAIN_MIXED	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Complex variable has mixed with retentive and non retentive memory"));	break;
	case ERR_DBI_BUF_TOO_SMALL	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Communication buffer to small"));							break;

	/* Flash interface
	 */
	case ERR_FLASH				:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Common flash error"));										break;
	case ERR_FLASH_INIT			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error initializing the flash memory"));						break;
	case ERR_FLASH_WRITE		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error writing into the flash memory"));						break;
	case ERR_FLASH_READ			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error reading from the flash memory"));						break;
	case ERR_FLASH_CLEAR		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error erasing the flash memory"));							break;
	case ERR_FLASH_WRONG_BLOCK	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Flash domain does not match"));								break;
		
	/* Online Change
	 */
	case ERR_OC_TASK_CHANGED	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Number of tasks or tasks attributes changed. Online Change not possible"));	break;
	case ERR_OC_PROJ_CHANGED	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Project changed. Online Change not possible"));				break;
	case ERR_OC_INVALID_CODE	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid code object index received"));						break;
	case ERR_OC_INVALID_INST	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid data object index received"));						break;
	case ERR_OC_COPY_LIST		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid data copy list received"));							break;
	case ERR_OC_COPY_NEW		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid new data object index specified"));					break;
	case ERR_OC_COPY_OLD		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid old data object index specified"));					break;
	case ERR_OC_TO_MANY_CODE	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("To many changed code objects received"));					break;
	case ERR_OC_TO_MANY_INST	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("To many changed data objects received"));					break;
	case ERR_OC_TO_MANY_CL		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("To many entries in data object copy list"));				break;
	case ERR_OC_TEMP_CODE		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("To many temporary code objects"));							break;
	case ERR_OC_TEMP_INST		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("To many temporary data objects"));							break;
	case ERR_OC_RETAIN_CHANGED	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Online Change is not supported for retain variables"));		break;

	/* Field Bus
	 */
	case ERR_FB_INIT			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Failed to initialize field bus driver"));					break;
	case ERR_FB_INIT_DATA		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Field bus initialization failed - invalid configuration"));	break;
	case WRN_FB_NO_INIT_DATA	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Field bus not initialized, no configuration data"));		break;
	case ERR_FB_TERM			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Failed to terminate field bus"));							break;
	case ERR_FB_NOT_INITIALIZED	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Field bus is not initialized"));							break;
	case ERR_FB_NOT_OPERATING	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Field bus is not operating"));								break;
	case ERR_FB_UNKNOWN_FBUS	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Field bus type unknow"));									break;
	case ERR_FB_NOT_LICENSED	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Field bus is not licensed"));								break;
	case ERR_FB_NOT_ENABLED		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Given field bus is not activated"));						break;

	/* BACnet
	 */
	case ERR_BAC_OBJ_NOT_INIT	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Object is not initialized"));								break;
	case ERR_BAC_OBJ_CREATE		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Failed to create object in BACnet stack"));					break;
	case ERR_INVALID_PRIORITY	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid priority value for object"));						break;
	case ERR_PROP_NOT_SUPPORTED	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Property is not supported"));								break;
	case ERR_SET_PROP_LOCAL		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error setting local property"));							break;
	case ERR_SET_PROP_REMOTE 	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error setting remote property"));							break;
	case ERR_COV_SUBSCRIBE		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("COV subscription failed"));									break;
	case ERR_CFG_FILE_NOT_FOUND	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Configuration file not found"));							break;
	case ERR_LIST_FULL			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Object list full"));										break;
	case ERR_INVALID_CONFIG		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid configuration file"));								break;
	case ERR_INVALID_OBJECT_TYPE:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid object type"));										break;
	case ERR_INVALID_PROP_TYPE	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid property type"));									break;
	case ERR_INVALID_OBJECT_ID	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid object ID"));										break;
	case ERR_API_CALL_FAILED 	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("BACnet API call failed"));									break;
	case ERR_OBJ_NOT_FOUND		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Object not found"));										break;
	case ERR_BACNET				:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Common BACnet error"));										break;
	case ERR_DEVICE_NOT_FOUND	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("BACnet Device not found"));									break;
	case ERR_GET_PROP_LOCAL		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error getting local property"));							break;
	case ERR_GET_PROP_REMOTE 	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error getting remote property"));							break;
	case ERR_INVALID_DATA_TYPE	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Invalid data type"));										break;
	case ERR_READ_ONLY			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Error Read Only"));											break;
	case ERR_COV_NOT_SUPPORTED	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("COV notifications not supported"));							break;
	case ERR_NO_EVENTS			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("No events queued"));										break;
	case ERR_TASK_NOT_ACTIVE 	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Task is not active"));										break;
	case ERR_DUPLICATE_OBJECT	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Duplicate BACnet object specified"));						break;
	case ERR_TOO_MANY_OBJECTS	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Too many BACnet objects downloaded"));						break;
	case ERR_NO_LOCAL_DEVICE	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("No local BACnet device defined"));							break;

	case WRN_BAC_OBJ_NOT_EXIST	:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("BACnet object does not exist"));							break;
	case WRN_PROPVAL_BAD 		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Property value is BAD"));									break;
	case WRN_OBJECT_BAD			:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Object is BAD"));											break;
	case WRN_NO_PROP_VALUE		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("No property value available (yet)"));						break;
	case WRN_OBJ_NOT_EXIST		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Object does not exist"));									break;

	/* Profibus DP
	 */
	case ERR_MASTER_ERROR		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Profibus Master switch to error state"));					break;
	case ERR_INIT_TIMEOUT		:	sRes.Format(FERR_CMD, uErrNo, szCmd[byCmd], _T("Profibus Master failed to init in time"));					break;

	/* Default
	 */
	default :	
		if (byCmd <= LAST_CMD_VALUE)
		{
			sRes.Format(FERR_DEF, uErrNo, szCmd[byCmd]);
		}
		else
		{
			sRes.Format(FERR_DEF, uErrNo, _T("<unkown>"));
		}
		break;
	}
}

/* GetTime64
 * ----------------------------------------------------------------------------
 */
__int64 GetTime64()
{
    static LARGE_INTEGER pf;
    static BOOL pf_valid = FALSE;
    LARGE_INTEGER pc;
	__int64 llResult;

	if (QueryPerformanceCounter (&pc))
	{
		if (!pf_valid)
		{
			pf_valid = QueryPerformanceFrequency( &pf );
		}

		if (pf_valid && (pf.LowPart | pf.HighPart) != 0)
		{
			llResult = *(__int64 *)&pc * 1000 / *(__int64 *)&pf;
			return llResult;
		}
	}

	llResult = GetTickCount();
	return llResult;
}

/* HtoT
 * ----------------------------------------------------------------------------
 */
USHORT C4CLayer::HtoT(USHORT uShort)
{
	if (m_Para.bBigEndian)
	{
		return htons(uShort);
	}

	return uShort;
}

/* TtoH
 * ----------------------------------------------------------------------------
 */
USHORT C4CLayer::TtoH(USHORT uShort)
{
	if (m_Para.bBigEndian)
	{
		return ntohs(uShort);
	}

	return uShort;
}


/* GetLogLayer
 * ----------------------------------------------------------------------------
 */
CLogLayer* C4CLayer::GetLogLayer()
{
    return m_pLog;
}

/* SetProgressCallback
 * ----------------------------------------------------------------------------
 */
void C4CLayer::SetProgressCallBack(PROGRESS_CALLBACK fpCallBack)
{
	ASSERT(m_pLog);

	m_pLog->SetProgressCallBack(fpCallBack);

}

/* ClearProgressCallback
 * ----------------------------------------------------------------------------
 */
void C4CLayer::ClearProgressCallBack()
{
	ASSERT(m_pLog);

	m_pLog->ClearProgressCallBack();
}


/* ---------------------------------------------------------------------------- */
