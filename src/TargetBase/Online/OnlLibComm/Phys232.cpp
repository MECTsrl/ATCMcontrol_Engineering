/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibComm/Phys232.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Phys232.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibComm/Phys232.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibComm
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

#include "Phys232.h"

//----  Static Initializations:   ----------------------------------*

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CPhys232, CPhysLayer)

static void TraceError(LPCTSTR szText);


/* CPhys232
 * ----------------------------------------------------------------------------
 */
CPhys232::CPhys232()
{
	m_hComm			= NULL;

    m_pBuffer		= NULL;
    m_dwBuffer		= 0;

	m_dwRecvCharTimeOut = 100;
}

/* ~CPhys232
 * ----------------------------------------------------------------------------
 */
CPhys232::~CPhys232()
{
	if (m_hComm)
	{
		CloseComm();
	}

	delete m_pBuffer;
	m_pBuffer = NULL;
}

/* Initialize
 * ----------------------------------------------------------------------------
 */
ECommError CPhys232::Initialize(SCommPara *pPara, LPCTSTR cszConnect)
{
    ECommError eCommErr = CPhysLayer::Initialize(pPara, cszConnect);
    if(eCommErr != ceOK)
    {
        return eCommErr;
    }

	if (m_pBuffer != NULL)
	{
		delete m_pBuffer;
	}

    m_dwBuffer	= m_Para.uBlockLen * 2 + 4;
    m_pBuffer	= new BYTE[m_dwBuffer];

    return ceOK;
}

/* SetRecvCharTimeOut
 * ----------------------------------------------------------------------------
 */
void CPhys232::SetRecvCharTimeOut(DWORD dwRecvCharTimeOut)
{
	m_dwRecvCharTimeOut = dwRecvCharTimeOut;
}

/* OpenComm
 * ----------------------------------------------------------------------------
 */
ECommError CPhys232::OpenComm()
{
	ASSERT(! m_sConnect.IsEmpty());

	if (m_hComm)
	{
		CloseComm();
	}

	CString sPort = m_sConnect.Left(m_sConnect.Find(_T(':')));

	// Open serial device
	m_hComm = CreateFile (sPort,
		 				GENERIC_READ | GENERIC_WRITE,
		 			    0,
					    NULL,
					    OPEN_EXISTING,
					    0, 
					    NULL);

	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		TraceError(_T("[CPhys232][OpenComm][CreateFile] %d.\n"));

		m_hComm = NULL;
		return ceError;
	}

	// Configure serial devive
	DCB dcb;
	if (! GetCommState (m_hComm, &dcb))
	{
		TraceError(_T("[CPhys232][OpenComm][GetCommState] %d.\n"));

		CloseHandle (m_hComm);
		m_hComm = 0;
		return ceConfigure;
	}

	if (! BuildCommDCB (m_sConnect, &dcb))
    {
		TraceError(_T("[CPhys232][OpenComm][BuildCommDCB] %d.\n"));

		CloseHandle (m_hComm);
		m_hComm = NULL;
		return ceConfigure;
    }   

	if (! SetCommState (m_hComm, &dcb))
	{
		TraceError(_T("[CPhys232][OpenComm][SetCommState] %d.\n"));

		CloseHandle (m_hComm);
		m_hComm = NULL;
		return ceConfigure;
	}
	
	// Configure time out values
	COMMTIMEOUTS TimeOut;

	TimeOut.ReadIntervalTimeout			= MAXDWORD;
	TimeOut.ReadTotalTimeoutConstant	= 0;
	TimeOut.ReadTotalTimeoutMultiplier	= 0;
	TimeOut.WriteTotalTimeoutConstant	= 0;
	TimeOut.WriteTotalTimeoutMultiplier	= 0;

	if (! SetCommTimeouts (m_hComm, &TimeOut))
	{
		TraceError(_T("[CPhys232][OpenComm][SetCommTimeouts] %d.\n"));

		CloseHandle (m_hComm);
		m_hComm = NULL;
		return ceConfigure;
	}

    TrcPrint(TRC_WARNING, _T("[CPhys232][OpenComm]\n"));

	m_bCommOpen	= TRUE;

	return ceOK;
}

/* CloseComm
 * ----------------------------------------------------------------------------
 */
ECommError CPhys232::CloseComm()
{
	if (! m_hComm)
	{
	    TrcPrint(TRC_ERROR, _T("[CPhys232][CloseComm] Not Open.\n"));
		return ceNotOpen;
	}

	BOOL bRes = CloseHandle (m_hComm);

	m_hComm		 = NULL;
	
	m_bCommOpen	 = FALSE;
	m_bConnected = FALSE;

    TrcPrint(TRC_WARNING, _T("[CPhys232][CloseComm]\n"));

	return bRes ? ceOK : ceError;
}

/* Flush
 * ----------------------------------------------------------------------------
 */
ECommError CPhys232::Flush()
{
	if (! ::PurgeComm (m_hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR))
	{
		TraceError(_T("[CPhys232][OpenComm][PurgeComm] %d.\n"));

		return ceError;
	}
	
	// Give the OS a chance to call the completion routine.
	// (Otherwise, for the OS the IO is still active!)
	SleepEx (0, TRUE);

	return ceOK;
}

/* Connect
 * ----------------------------------------------------------------------------
 */
ECommError CPhys232::Connect()
{
	m_bConnected = TRUE;

	return ceOK;
}

/* Disconnect
 * ----------------------------------------------------------------------------
 */
ECommError CPhys232::Disconnect()
{
	m_bConnected = FALSE;

	return ceOK;
}

/* Send
 * ----------------------------------------------------------------------------
 */
ECommError CPhys232::Send(BYTE *pData, UINT uLen)
{
	ASSERT(pData);
    ASSERT(m_pBuffer);

	if (! m_hComm)
	{
	    TrcPrint(TRC_ERROR, _T("[CPhys232Sync][Send] Not Open.\n"));
		return ceNotOpen;
	}

	AddProtocol(pData, uLen, m_pBuffer, &uLen);

	DWORD dwTransfered = 0;

	if (WriteFile (m_hComm, m_pBuffer, uLen, &dwTransfered, NULL) == FALSE)
	{
		// Write failed.
		TraceError(_T("[CPhys232Sync][Send][WriteFile] %d.\n"));
		return ceErrorSendingData;
	}

	if (dwTransfered != uLen)
	{
		// Write failed.
		TraceError(_T("[CPhys232Sync][Send][WriteFile][No] %d.\n"));
		return ceErrorSendingData;
	}

	// Everything OK
	return ceOK;
}

/* Receive
 * ----------------------------------------------------------------------------
 */
ECommError CPhys232::Receive (BYTE *pData, UINT *upLen)
{
	ASSERT(pData);
    ASSERT(m_pBuffer);

	if (! m_hComm)
	{
	    TrcPrint(TRC_ERROR, _T("[CPhys232][Receive] Not Open.\n"));
		return ceNotOpen;
	}

	EProtState uState = eStart;
	ECommError eError = ceOK;

	BYTE byCurr;

	UINT uMax	= *upLen;
	*upLen		= 0;

	DWORD dwStartTime = timeGetTime();
	DWORD dwCurrTime  = 0;
	DWORD dwLastTime  = 0;

	BOOL bFirst = TRUE;

	while (*upLen <= uMax)
	{
		dwCurrTime = timeGetTime();

		/* Check the absolut time out value for the receiving of the 
		 * first byte.
		 */
		if (bFirst && dwCurrTime - dwStartTime > m_Para.dwRecvTimeOut)
		{
			TraceError(_T("[CPhys232Sync][Receive] Time Out (absolut).\n"));

			*upLen = 0;
			return ceTimeOut;
		}

		eError = GetByte(&byCurr);
		
		if (eError != ceOK)
		{
			if (eError == ceTimeOut)
			{
				/* Time out between receiving of two characters
				 */
				if (bFirst == FALSE && dwCurrTime - dwLastTime > m_dwRecvCharTimeOut)
				{
					TraceError(_T("[CPhys232Sync][Receive] Time Out (dynamic).\n"));

					*upLen = 0;
					return ceTimeOut;
				}

				SleepEx(25, TRUE);
				continue;
			}

			return eError;
		}

		bFirst = FALSE;
		dwLastTime = dwCurrTime;

		switch (uState)
		{
			case eStart:	// ------------------------------------------------
			{
				if (byCurr == SC_DLE)
				{
					uState = eStart2;
				}
				break;
			}

			case eStart2:	// ------------------------------------------------
			{
				if (byCurr == SC_STX)
				{
					uState = eData;
				}
				else
				{
					uState = eStart;
				}
				break;
			}

			case eData:		// ------------------------------------------------
			{
				if (byCurr == SC_DLE)
				{
					uState = eEscape;
				}
				else
				{
					if (*upLen < uMax)
					{
						pData[*upLen] = byCurr;
					}
					(*upLen)++;
				}
				break;
			}

			case eEscape:	// ------------------------------------------------
			{
				switch (byCurr)
				{
					case SC_DLE:
					{
						if (*upLen < uMax)
						{
							pData[*upLen] = byCurr;
						}
						(*upLen)++;

						uState = eData;
						break;
					}

					case SC_STX:
					{
						*upLen = 0;
						
						uState = eData;
						break;
					}

					case SC_ETX:
					{
						return ceOK;
					}

					default:
					{
						*upLen = 0;
					
						uState = eStart;
						break;
					}
				}
				break;
			}

			default:		// ------------------------------------------------
			{
				*upLen = 0;
				uState = eStart;
				break;
			}
		
		} /* switch (uState) */

	} /* while (*upLen < uMax) */


	TRACE1(_T("[CPhys232][Receive] Block synchronization.\n"), uState);
	TrcPrint(TRC_ERROR, _T("[CPhys232][Receive] Block synchronization.\n"), uState);

    return ceErrorReadingData;
}


/* AddProtocol
 * ----------------------------------------------------------------------------
 */
ECommError CPhys232::AddProtocol(BYTE *pSrc, UINT uSrc, BYTE *pDest, UINT *upDest)
{
    *upDest = 0;

    pDest[(*upDest)++] = SC_DLE;
    pDest[(*upDest)++] = SC_STX;

    for(UINT i = 0; i < uSrc; i++)
    {
        if(pSrc[i] == SC_DLE)
        {
            pDest[(*upDest)++] = SC_DLE;
        }
        pDest[(*upDest)++] = pSrc[i];
    }

    pDest[(*upDest)++] = SC_DLE;
    pDest[(*upDest)++] = SC_ETX;

    return ceOK;
}

/* GetByte
 * ----------------------------------------------------------------------------
 */
ECommError CPhys232::GetByte(BYTE *pByte)
{
	DWORD dwTransfered = 0;

	if (ReadFile (m_hComm, pByte, sizeof(BYTE), &dwTransfered, NULL) == FALSE)
	{
		// Read failed.
		TraceError(_T("[CPhys232Sync][Receive][GetByte] %d.\n"));
		return ceErrorSendingData;
	}

	if (dwTransfered != sizeof(BYTE))
	{
		// Time out, no data
		return ceTimeOut;
	}

	return ceOK;
}

/* TraceError
 * ----------------------------------------------------------------------------
 */
static void TraceError(LPCTSTR szText)
{
	int iError = GetLastError();

	TRACE1(szText, iError);
	TrcPrint(TRC_ERROR, szText, iError);
}

/* ---------------------------------------------------------------------------- */
