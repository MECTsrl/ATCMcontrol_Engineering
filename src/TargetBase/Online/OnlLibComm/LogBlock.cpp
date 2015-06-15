/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibComm/LogBlock.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: LogBlock.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibComm/LogBlock.cpp $
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

#include "LogBlock.h"
#include "PhysLayer.h"

//----  Static Initializations:   ----------------------------------*

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static LPCTSTR szCommandText[] = CMD_TEXT;

IMPLEMENT_DYNCREATE(CLogBlock, CLogLayer)

/* CLogBlock
 * ----------------------------------------------------------------------------
 */
CLogBlock::CLogBlock()
{
	m_pDataFrame	= NULL;
	m_pInfoFrame	= NULL;

	m_uWaitResend	= 50;
	m_uRepeatOnError= 1;
}

/* ~CLogBlock
 * ----------------------------------------------------------------------------
 */
CLogBlock::~CLogBlock()
{
	delete m_pDataFrame;
	delete m_pInfoFrame;
}

/* Initialize
 * ----------------------------------------------------------------------------
 */
ECommError CLogBlock::Initialize (CPhysLayer *pPhys, SCommPara *pPara, LPCTSTR cszConnect)
{
	ASSERT (pPhys);
	ASSERT (pPara);

	if (m_pDataFrame == NULL)
	{
		m_pDataFrame = (XFrame *)new char [HD_FRAME + pPara->uBlockLen];
	}

	pPhys->SetHeaderSize(HD_FRAME, (BYTE *)&m_pDataFrame->BLK.uLen - (BYTE *)m_pDataFrame);

	return CLogLayer::Initialize(pPhys, pPara, cszConnect);
}

/* Send
 * ----------------------------------------------------------------------------
 */
ECommError CLogBlock::Send (BYTE byCommand, BYTE *pData, UINT uLen)
{
	ASSERT(m_pPhys);

	// Build block frame
	if (m_pDataFrame == NULL)
	{
		m_pDataFrame = (XFrame *)new char [HD_FRAME + m_Para.uBlockLen];
	}

	memset (m_pDataFrame, 0x00, HD_FRAME);

	m_pDataFrame->BLK.CMD.byCommand	= byCommand;

	USHORT	uBlock		= 0;
	UINT	uBlockLen	= 0;
	UINT	uDataLen	= 0;
	BOOL	bLast		= FALSE;
	BYTE	byType		= BT_DATA;

	UINT	uProgMax	= uLen / m_Para.uBlockLen + ((uLen % m_Para.uBlockLen) > 0 ? 1 : 0);
	UINT	uProgCur	= 0;

	while (! bLast)
	{
		PROGRESS(uProgCur, uProgMax);
		
		// Block count starts with 1!
		uBlock++;
		
		m_pDataFrame->byType = byType;

		// Build data length
		uDataLen = (uLen - (uBlock - 1) * m_Para.uBlockLen) <= m_Para.uBlockLen ? 
				   (uLen - (uBlock - 1) * m_Para.uBlockLen) : m_Para.uBlockLen;
		
		// Build block length
		uBlockLen	= uDataLen + HD_FRAME;
		bLast		= (uBlock * m_Para.uBlockLen >= uLen);

		m_pDataFrame->BLK.byLast	= (uBlock * m_Para.uBlockLen >= uLen);
		m_pDataFrame->BLK.uBlock	= uBlock;			
		m_pDataFrame->BLK.uLen		= (USHORT)uDataLen;

		if (pData != NULL)
		{
			memcpy(m_pDataFrame->BLK.CMD.pData, pData + m_Para.uBlockLen * (uBlock - 1), uDataLen);
		}

		BuildCRC(m_pDataFrame);

		// Convert Little -> Big Endian
		HtoT(m_pDataFrame);

		ECommError eError = ceError;

		for (UINT i = 0; (i < m_uRepeatOnError && eError != ceAck) || i == 0; i++)
		{

			TRACE(_T(">>> %-8s %3d %3d     "), byCommand <= LAST_CMD_VALUE ? szCommandText[byCommand] : _T("Invalid"), uBlock, uDataLen);
			TrcPrint(TRC_INTERFACE, _T(">>> %-8s %3d %3d     "), byCommand <= LAST_CMD_VALUE ? szCommandText[byCommand] : _T("Invalid"), uBlock, uDataLen);

			// Send block
			eError = m_pPhys->Send((BYTE *)m_pDataFrame, HD_FRAME + uDataLen);

			if (eError != ceOK)
			{
				// Communication error sending data block.
				SleepEx (m_uWaitResend, FALSE);
				m_pPhys->Flush();
				
				continue;
			}

			// Wait for acknowledge
			eError = RecvAck (uBlock);

			if (eError != ceAck && eError != ceNAck)
			{
				// Communication Error receiving ACK / NACK.
				SleepEx (m_uWaitResend, FALSE);
				m_pPhys->Flush();
				
				continue;
			}
		
		} // end for 

		if (eError != ceAck)
		{
			// A block could not be transmitted correctly. 
			PROGRESS_LAST(uProgMax);
			return eError;
		}
	
		uProgCur++;
		
	} // end while

	PROGRESS(uProgCur, uProgMax);

	return ceOK;
}

/* Receive
 * ----------------------------------------------------------------------------
 */
ECommError CLogBlock::Receive (BYTE *bypCommand, BYTE **ppData, UINT *upLen)
{
	HEAP_FREE(*ppData, *upLen);

	// Build block frame
	if (m_pDataFrame == NULL)
	{
		m_pDataFrame = (XFrame *)new char [HD_FRAME + m_Para.uBlockLen];
	}

	memset (m_pDataFrame, 0x00, HD_FRAME);

	*upLen	= 0;

	USHORT	uBlock		= 0;
	UINT	uBlockLen	= 0;
	UINT	uDataLen	= 0;
	BOOL	bLast		= FALSE;

	ECommError eError;

	UINT	uProgMax	= m_uMaxRecvProg;
	UINT	uProgCur	= 0;

	while (! bLast)
	{
		PROGRESS(uProgCur,uProgMax);
		
		// Block count starts with 1!
		uBlock++;

		eError = ceError;

		for (UINT i = 0; (i < m_uRepeatOnError && eError != ceOK) || i == 0; i++)
		{
			bLast = FALSE;

			// Send request for response block
			eError = SendInfoBlock (uBlock, BT_REQ);

			if (eError != ceOK)
			{
				// Communication error sending block
				SleepEx (m_uWaitResend, FALSE);
				m_pPhys->Flush();
	
				continue;			
			}

			uBlockLen = m_Para.uBlockLen + HD_FRAME;

			// Receive block
			eError = m_pPhys->Receive((BYTE *)m_pDataFrame, &uBlockLen);

			if (eError != ceOK)
			{
				// Communication error or time out; try again
				SleepEx (m_uWaitResend, FALSE);
				m_pPhys->Flush();

				continue;
			}

			// Check block size
			if (uBlockLen < HD_FRAME)
			{
				// Received block could not be a valid block; try again
				SleepEx (m_uWaitResend, FALSE);
				m_pPhys->Flush();

				TrcPrint(TRC_ERROR, _T("[CLogBlock][Receive] Invalid block length.\n"));

				eError = ceError;
				continue;
			}

			// Convert Big -> Little Endian
			TtoH(m_pDataFrame);

            if (HD_FRAME + m_pDataFrame->BLK.uLen != uBlockLen)
            {
		        TrcPrint(TRC_ERROR, _T("[CLogBlock][Receive] CRC frame data length check failed.\n"));
				PROGRESS_LAST(uProgMax);
		        return ceCRC;
            }

			// Check CRC
			if (CheckCRC (m_pDataFrame) != ceOK)
			{
				TRACE (_T("CRC_CRC_CRC_CRC_CRC_CRC_CRC_CRC_CRC_CRC_CRC_CRC_CRC_CRC_CRC_\n"));
				TrcPrint(TRC_ERROR, _T("[CLogBlock][Receive] CRC check failed.\n"));

				// Wrong checksum; send NACK and try again
				SleepEx (m_uWaitResend, FALSE);
				m_pPhys->Flush();

				eError = ceCRC;
				continue;
			}

			// Check block type
			if (m_pDataFrame->byType != BT_DATA)
			{
				// Received block could not be a valid block; try again
				SleepEx (m_uWaitResend, FALSE);
				m_pPhys->Flush();

				TrcPrint(TRC_ERROR, _T("[CLogBlock][Receive] Invalid block type (data expected).\n"));

				eError = ceError;
				continue;
			}

			bLast = m_pDataFrame->BLK.byLast;

			// Check block number
			if (m_pDataFrame->BLK.uBlock != uBlock)
			{
				// Received block number does not match
				SleepEx (m_uWaitResend, FALSE);
				m_pPhys->Flush();

				TrcPrint(TRC_ERROR, _T("[CLogBlock][Receive] Invalid block number.\n"));

				eError = ceError;
				continue;
			}

			*bypCommand = m_pDataFrame->BLK.CMD.byCommand;
			uDataLen	= m_pDataFrame->BLK.uLen;

			// Create Command Buffer
			if (uBlock == 1)
			{
				HEAP_ALLOC(*ppData, uDataLen);
			}
			else
			{
				HEAP_RE_ALLOC(*ppData, *upLen + uDataLen);
			}

			if (*ppData == NULL)
			{
				TrcPrint(TRC_ERROR, _T("[CLogBlock][Receive] Alloc Memory.\n"));

				eError = ceError;
				continue;
			}

			// Copy data
			memcpy(*ppData + *upLen, m_pDataFrame->BLK.CMD.pData, uDataLen);
			*upLen += uDataLen;

			if (m_pDataFrame->BLK.CMD.byCommand > 0x80u)
			{
				// Error Message from VMM
				TRACE(_T("<<<  %-8s %3d %4d  \n"), _T("FAILED"), uBlock, m_pDataFrame->BLK.uLen);
				TrcPrint(TRC_INTERFACE, _T("<<<  %-8s %3d %4d  \n"), _T("FAILED"), uBlock, m_pDataFrame->BLK.uLen);

				PROGRESS_LAST(uProgMax);

				return ceOK;
			}
			else
			{
				TRACE(_T("<<<  %-8s %3d %3d  \n"), m_pDataFrame->BLK.CMD.byCommand <= LAST_CMD_VALUE ? szCommandText[m_pDataFrame->BLK.CMD.byCommand] : _T("Invalid"), uBlock, m_pDataFrame->BLK.uLen);
				TrcPrint(TRC_INTERFACE, _T("<<<  %-8s %3d %4d  \n"), m_pDataFrame->BLK.CMD.byCommand <= LAST_CMD_VALUE ? szCommandText[m_pDataFrame->BLK.CMD.byCommand] : _T("Invalid"), uBlock, m_pDataFrame->BLK.uLen);
			}

		} // end for 

		if (eError != ceOK)
		{
			HEAP_FREE(*ppData, *upLen);

			PROGRESS_LAST(uProgMax);
			return eError;
		}

		if (++uProgCur == uProgMax)
		{
			uProgMax += uProgMax;
		}
		
	} // end while

	PROGRESS_LAST(uProgMax);
	
	return ceOK;
}

/* RecvAck
 * ----------------------------------------------------------------------------
 */
ECommError CLogBlock::RecvAck (USHORT uBlock)
{
	ASSERT(m_pPhys);

	if (m_pInfoFrame == NULL)
	{
		m_pInfoFrame = (XFrame *)new char [HD_FRAME + m_Para.uBlockLen];
	}

	memset (m_pInfoFrame, 0x00, HD_FRAME);

	UINT uLen = HD_FRAME + m_Para.uBlockLen;

	ECommError eError = m_pPhys->Receive((BYTE *)m_pInfoFrame, &uLen);

	// Error receiving BLK
	if (eError != ceOK)
	{
		return eError;
	}

	// Check data length
	if (uLen != HD_FRAME)
	{
		TrcPrint(TRC_ERROR, _T("[CLogBlock][RecvAck] Invalid data length.\n"));
		return ceError;
	}

	// Convert Big -> Little Endian
	TtoH(m_pInfoFrame);

    if (HD_FRAME + m_pInfoFrame->BLK.uLen != uLen)
    {
		TrcPrint(TRC_ERROR, _T("[CLogBlock][RecvAck] CRC frame data length check failed.\n"));
		return ceCRC;
    }

	if (CheckCRC (m_pInfoFrame) != ceOK)
	{
		TrcPrint(TRC_ERROR, _T("[CLogBlock][RecvAck] CRC check failed.\n"));
		return ceCRC;
	}

	// Check block number
	if (uBlock != m_pInfoFrame->BLK.uBlock)
	{
		TrcPrint(TRC_ERROR, _T("[CLogBlock][RecvAck] Invalid block number.\n"));
		return ceError;
	}

	// Get ACK / NACK
	if (m_pInfoFrame->byType == BT_ACK)
	{
		TRACE(_T("<<<  %-8s %3d %3d\n"), _T("ACK"), uBlock, m_pInfoFrame->BLK.uLen);
		TrcPrint(TRC_INTERFACE, _T("<<<  %-8s %3d %4d\n"), _T("ACK"), uBlock, m_pInfoFrame->BLK.uLen);

		return ceAck;
	}

	if (m_pInfoFrame->byType == BT_NACK)
	{
		TRACE(_T("<<<  %-8s %3d %3d\n"), _T("NACK"), uBlock, m_pInfoFrame->BLK.uLen);
		TrcPrint(TRC_INTERFACE, _T("<<<  %-8s %3d %4d\n"), _T("NACK"), uBlock, m_pInfoFrame->BLK.uLen);

		return ceNAck;
	}

	return ceError;
}

/* SendInfoBlock
 * ----------------------------------------------------------------------------
 */
ECommError CLogBlock::SendInfoBlock (USHORT uBlock, BYTE byType)
{
	ASSERT(m_pPhys);

	if (m_pInfoFrame == NULL)
	{
		m_pInfoFrame = (XFrame *)new char [HD_FRAME + m_Para.uBlockLen];
	}

	memset (m_pInfoFrame, 0x00, HD_FRAME);

	m_pInfoFrame->byType		= byType;
	m_pInfoFrame->BLK.uBlock	= uBlock;

	BuildCRC(m_pInfoFrame);

	// Convert Little -> Big Endian
	HtoT(m_pInfoFrame);
	
	switch (byType)
	{
		case BT_ACK:
		{
			TRACE (_T(">>> %-8s %3d %3d     "), _T("ACK"), uBlock, 0);
			TrcPrint(TRC_INTERFACE, _T(">>> %-8s %3d %3d     "), _T("ACK"), uBlock, 0);
			break;
		}

		case BT_NACK:
		{
			TRACE (_T(">>> %-8s %3d %3d     "), _T("NACK"), uBlock, 0);
			TrcPrint(TRC_INTERFACE, _T(">>> %-8s %3d %3d     "), _T("NACK"), uBlock, 0);
			break;
		}

		case BT_REQ:
		{
			TRACE (_T(">>> %-8s %3d %3d     "), _T("REQ"), uBlock, 0);
			TrcPrint(TRC_INTERFACE, _T(">>> %-8s %3d %3d     "), _T("REQ"), uBlock, 0);
			break;
		}
	}

	return m_pPhys->Send((BYTE *)m_pInfoFrame, HD_FRAME);
}

/* BuildCRC
 * ----------------------------------------------------------------------------
 */
ECommError CLogBlock::BuildCRC (XFrame *pFrame)
{
	BYTE *pData = (BYTE *)pFrame;
	BYTE byCRC = 0;

	pFrame->byCRC = 0;

	for (UINT i = 0; i < HD_FRAME + pFrame->BLK.uLen; i++)
	{
		byCRC = (BYTE)(byCRC + pData[i]);
	}

	pFrame->byCRC = byCRC;

	return ceOK;
}

/* CheckCRC
 * ----------------------------------------------------------------------------
 */
ECommError CLogBlock::CheckCRC(XFrame *pFrame)
{
	BYTE *pData	= (BYTE *)pFrame;
	BYTE byCheck= pFrame->byCRC;
	BYTE byCRC	= 0;

	pFrame->byCRC = 0;

	for (UINT i = 0; i < HD_FRAME + pFrame->BLK.uLen; i++)
	{
		byCRC = (BYTE)(byCRC + pData[i]);
	}

	return byCheck == byCRC ? ceOK : ceError;
}

/* HtoT
 * ----------------------------------------------------------------------------
 */
void CLogBlock::HtoT(XFrame *pFrame)
{
	ASSERT(m_pPhys);

	if (m_Para.bBigEndian)
	{
		pFrame->BLK.uBlock	= htons(pFrame->BLK.uBlock);
		pFrame->BLK.uLen	= htons(pFrame->BLK.uLen);
	}
}

/* TtoH
 * ----------------------------------------------------------------------------
 */
void CLogBlock::TtoH(XFrame *pFrame)
{
	ASSERT(m_pPhys);

	if (m_Para.bBigEndian)
	{
		pFrame->BLK.uBlock	= ntohs(pFrame->BLK.uBlock);
		pFrame->BLK.uLen	= ntohs(pFrame->BLK.uLen);
	}
}

/* SetParam
 * ----------------------------------------------------------------------------
 */
void CLogBlock::SetParam(UINT uWaitResend, UINT uRepeatOnError)
{
	m_uWaitResend	= uWaitResend;
	m_uRepeatOnError= uRepeatOnError;
}

/* ---------------------------------------------------------------------------- */
