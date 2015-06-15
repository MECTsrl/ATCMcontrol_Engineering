/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibComm/LogSimple.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: LogSimple.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibComm/LogSimple.cpp $
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

#include <WinSock2.h>

#include "LogSimple.h"
#include "PhysLayer.h"

//----  Static Initializations:   ----------------------------------*

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static LPCTSTR szCommandText[] = CMD_TEXT;

IMPLEMENT_DYNCREATE(CLogSimple, CLogLayer)


/* CLogSimple
 * ----------------------------------------------------------------------------
 */
CLogSimple::CLogSimple()
{
	m_pDataBlock	= NULL;

	m_pRecvData		= NULL;
	m_uRecvLen		= 0;
}

/* ~CLogSimple
 * ----------------------------------------------------------------------------
 */
CLogSimple::~CLogSimple()
{
	delete m_pDataBlock;

	HEAP_FREE(m_pRecvData, m_uRecvLen);
}

/* Initialize
 * ----------------------------------------------------------------------------
 */
ECommError CLogSimple::Initialize (CPhysLayer *pPhys, SCommPara *pPara, LPCTSTR cszConnect)
{
	ASSERT (pPhys);
	ASSERT (pPara);

	if (m_pDataBlock == NULL)
	{
		m_pDataBlock = (XBlock *)new char [HD_BLOCK + pPara->uBlockLen];
	}

	pPhys->SetHeaderSize(HD_BLOCK, (BYTE *)&m_pDataBlock->uLen - (BYTE *)m_pDataBlock);

	return CLogLayer::Initialize(pPhys, pPara, cszConnect);
}

/* Send
 * ----------------------------------------------------------------------------
 */
ECommError CLogSimple::Send (BYTE byCommand, BYTE *pData, UINT uLen)
{
	HEAP_FREE(m_pRecvData, m_uRecvLen);

	ECommError eError;

	// Create a new sequence number
	m_byCurSequence++;
	m_byCurCommand	= byCommand;
	m_uCurBlock		= 1;

	if (uLen <= m_Para.uBlockLen)
	{
		// Command fits in one communication frame

		PROGRESS(0,1);
		
		eError = SendBlock(m_byCurCommand, m_byCurSequence, m_uCurBlock, TRUE, pData, uLen);

		PROGRESS(1,1);

		return eError;
	}

	UINT	uCount		= 0;
	UINT	uToSend		= 0;
	
	BYTE	bySendLast	= TRUE;
	BYTE	byRecvLast	= TRUE;

	UINT	uProgMax	= uLen / m_Para.uBlockLen + ((uLen % m_Para.uBlockLen) > 0 ? 1 : 0);
	UINT	uProgCur	= 0;

	do
	{
		// ---- Send ----

		PROGRESS(uProgCur, uProgMax);

		if (uLen - uCount <= m_Para.uBlockLen)
		{
			bySendLast	= TRUE;
			uToSend		= uLen - uCount;
		}
		else
		{
			bySendLast	= FALSE;
			uToSend		= m_Para.uBlockLen;
		}

		eError = SendBlock(m_byCurCommand, m_byCurSequence, m_uCurBlock, bySendLast, pData + uCount, uToSend);
		if (eError != ceOK)
		{
			// Communication error
			PROGRESS_LAST(uProgMax);
			return eError;
		}

		uCount += uToSend;

		// ---- Recv ----

		if (bySendLast == FALSE)
		{
			eError = RecvBlock(&m_byCurCommand, m_byCurSequence, m_uCurBlock, &byRecvLast, &m_pRecvData, &m_uRecvLen);
			if (eError != ceOK)
			{
				// Communication error
				PROGRESS_LAST(uProgMax);
				return eError;
			}

			if (m_byCurCommand < 0x7fu && m_pRecvData != NULL)
			{
				// RTS must not send data if OK and bySendLast == FALSE! --> ignore

				HEAP_FREE(m_pRecvData, m_uRecvLen);
			}

			m_uCurBlock++;
		}

		uProgCur++;

	} while (uCount < uLen && m_byCurCommand < 0x7fu);

	PROGRESS(uProgCur, uProgMax);

	return ceOK;
}



/* Receive
 * ----------------------------------------------------------------------------
 */
ECommError CLogSimple::Receive (BYTE *bypCommand, BYTE **ppData, UINT *upLen)
{
	HEAP_FREE(*ppData, *upLen);

	BYTE	byLast = TRUE;

	ECommError eError;

	BYTE	*pRecvData = NULL;
	UINT	uRecvLen   = 0;

	if (m_byCurCommand >= 0x80)
	{
		// RTS error
		*bypCommand	= m_byCurCommand;
		*ppData		= m_pRecvData;
		*upLen		= m_uRecvLen;

		return ceOK;
	}

	UINT	uProgMax	= m_uMaxRecvProg;
	UINT	uProgCur	= 0;
	
	HEAP_FREE(m_pRecvData, m_uRecvLen);

	do
	{
		PROGRESS(uProgCur,uProgMax);

		eError = RecvBlock(&m_byCurCommand, m_byCurSequence, m_uCurBlock, &byLast, &pRecvData, &uRecvLen);
		if (eError != ceOK)
		{
			// Communication error
			PROGRESS_LAST(uProgMax);
			return eError;
		}

		if (m_byCurCommand >= 0x80u)
		{
			// RTS error
			HEAP_FREE(*ppData, *upLen);
			byLast = TRUE;
		}

		if (*ppData == NULL)
		{
			*ppData = pRecvData;
			*upLen  = uRecvLen;

            pRecvData = NULL;
            uRecvLen  = 0;
		}
		else
		{
			HEAP_RE_ALLOC(*ppData, *upLen + uRecvLen);

			memcpy(*ppData + *upLen, pRecvData, uRecvLen);
			*upLen += uRecvLen;

			HEAP_FREE(pRecvData, uRecvLen);
		}

		m_uCurBlock++;

		if (byLast == FALSE)
		{
		    eError = SendBlock(m_byCurCommand, m_byCurSequence, m_uCurBlock, TRUE, NULL, 0);
		    if (eError != ceOK)
		    {
			    // Communication error
				PROGRESS_LAST(uProgMax);
			    return eError;
		    }
        }

		if (++uProgCur == uProgMax)
		{
			uProgMax += uProgMax;
		}

	} while (byLast == FALSE);

	*bypCommand = m_byCurCommand;

	PROGRESS_LAST(uProgMax);

	return eError;

}

/* SendBlock
 * ----------------------------------------------------------------------------
 */
ECommError CLogSimple::SendBlock(BYTE byCommand, BYTE bySequence, USHORT uBlock, BYTE byLast, BYTE *pData, UINT uLen)
{
	if (uLen > m_Para.uBlockLen)
	{
		TRACE(_T("ERROR: To much data!\n"));
	    TrcPrint(TRC_ERROR, _T("[CLogSimple][SendBlock] To much data.\n"));

		return ceError;
	}

	// Build block frame
	if (m_pDataBlock == NULL)
	{
		m_pDataBlock = (XBlock *)new char [HD_BLOCK + m_Para.uBlockLen];
	}

	m_pDataBlock->uLen			= (USHORT)uLen;
	m_pDataBlock->byLast		= byLast;
	m_pDataBlock->uBlock		= uBlock;

	m_pDataBlock->CMD.byCommand		= byCommand;			
	m_pDataBlock->CMD.bySequence	= bySequence;
	memcpy(m_pDataBlock->CMD.pData, pData, uLen);

	// Convert Little -> Big Endian
	HtoT(m_pDataBlock);

	TRACE(_T(">>> %-8s %3d     "), byCommand <= LAST_CMD_VALUE ? szCommandText[byCommand] : _T("Invalid"), uLen);
    TrcPrint(TRC_INTERFACE, _T(">>> %-8s %3d     "), byCommand <= LAST_CMD_VALUE ? szCommandText[byCommand] : _T("Invalid"), uLen);

	// Send block
	return m_pPhys->Send((BYTE *)m_pDataBlock, HD_BLOCK + uLen);
}

/* RecvBlock
 * ----------------------------------------------------------------------------
 */
ECommError CLogSimple::RecvBlock(BYTE *bypCommand, BYTE bySequence, USHORT uBlock, BYTE *bypLast, BYTE **ppData, UINT *upLen)
{
	// Build block frame
	if (m_pDataBlock == NULL)
	{
		m_pDataBlock = (XBlock *)new char [HD_BLOCK + m_Para.uBlockLen];
	}

	HEAP_FREE(*ppData, *upLen);

	UINT uLen = m_Para.uBlockLen + HD_BLOCK;

	// Receive block
	ECommError eError = m_pPhys->Receive((BYTE *)m_pDataBlock, &uLen);

	if (eError != ceOK)
	{
		// Communication error
		m_pPhys->Flush();
		return eError;
	}

	// Check block size
	if (uLen < HD_BLOCK)
	{
		m_pPhys->Flush();
	    TRACE(_T("[CLogSimple][Receive] Invalid block length (1).\n"));
	    TrcPrint(TRC_ERROR, _T("[CLogSimple][Receive] Invalid block length (1).\n"));
		return ceError;
	}

	// Convert Big -> Little Endian
	TtoH(m_pDataBlock);

	// Check block size
	if (uLen != m_pDataBlock->uLen + HD_BLOCK)
	{
		m_pPhys->Flush();
	    TRACE(_T("[CLogSimple][Receive] Invalid block length (2).\n"));
	    TrcPrint(TRC_ERROR, _T("[CLogSimple][Receive] Invalid block length (2).\n"));
		return ceError;
	}

	// Check block sequence
	if ((m_pDataBlock->CMD.byCommand & 0x7f) != *bypCommand)
	{
	    TRACE(_T("[CLogSimple][Receive] Invalid command, synchronization lost.\n"));
		TrcPrint(TRC_ERROR, _T("[CLogSimple][Receive] Invalid command, synchronization lost.\n"));
		return ceError;
	}

	if (m_pDataBlock->CMD.bySequence != bySequence)
	{
	    TRACE(_T("[CLogSimple][Receive] Invalid sequence number, synchronization lost.\n"));
	    TrcPrint(TRC_ERROR, _T("[CLogSimple][Receive] Invalid sequence number, synchronization lost.\n"));
		return ceError;
	}

	if (m_pDataBlock->uBlock != uBlock)
	{
	    TRACE(_T("[CLogSimple][Receive] Invalid block number, synchronization lost.\n"));
	    TrcPrint(TRC_ERROR, _T("[CLogSimple][Receive] Invalid block number, synchronization lost.\n"));
		return ceError;
	}

	// Copy Data
	*upLen		= m_pDataBlock->uLen;
	*bypLast	= (BYTE)(m_pDataBlock->CMD.byCommand < 0x7fu ? m_pDataBlock->byLast : TRUE);
	*bypCommand	= m_pDataBlock->CMD.byCommand;

	HEAP_ALLOC(*ppData, uLen);

	memcpy(*ppData, m_pDataBlock->CMD.pData, uLen);

	if (m_pDataBlock->CMD.byCommand >= 0x80u)
	{
		TRACE(_T("<<<  %-8s %3d  \n"), _T("FAILED"), uLen);
		TrcPrint(TRC_INTERFACE, _T("<<<  %-8s %3d  \n"), _T("FAILED"), uLen);
	}
	else
	{
		TRACE(_T("<<<  %-8s %3d  \n"), m_pDataBlock->CMD.byCommand <= LAST_CMD_VALUE ? szCommandText[m_pDataBlock->CMD.byCommand] : _T("Invalid"), uLen);
	    TrcPrint(TRC_INTERFACE, _T("<<<  %-8s %3d  \n"), m_pDataBlock->CMD.byCommand <= LAST_CMD_VALUE ? szCommandText[m_pDataBlock->CMD.byCommand] : _T("Invalid"), uLen);
	}

	return ceOK;
}


/* HtoT
 * ----------------------------------------------------------------------------
 */
void CLogSimple::HtoT(XBlock *pBlock)
{
	if (m_Para.bBigEndian)
	{
		pBlock->uLen	= htons(pBlock->uLen);
		pBlock->uBlock	= htons(pBlock->uBlock);
	}
}

/* TtoH
 * ----------------------------------------------------------------------------
 */
void CLogSimple::TtoH(XBlock *pBlock)
{
	if (m_Para.bBigEndian)
	{
		pBlock->uLen	= ntohs(pBlock->uLen);
		pBlock->uBlock	= ntohs(pBlock->uBlock);
	}
}

/* ---------------------------------------------------------------------------- */
