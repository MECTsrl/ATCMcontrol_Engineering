/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/LogBlock.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: LogBlock.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/LogBlock.h $
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

#ifndef _LOGBLOCK_H_
#define _LOGBLOCK_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _LOGLAYER_H_
#include "LogLayer.h"
#endif

#ifndef _VMSHAREDDEF_H_
#include "vmSharedDef.h"
#endif

/**
 *  class CLogBlock
 *
 */
class _ONLCOMM CLogBlock : public CLogLayer  
{
	DECLARE_DYNCREATE(CLogBlock)
	protected:
		CLogBlock();
	
	public:
		virtual ~CLogBlock();

	protected:
		XFrame	*m_pDataFrame;
		XFrame	*m_pInfoFrame;

		UINT	m_uWaitResend;
		UINT	m_uRepeatOnError;

	public:
		void	TtoH(XFrame *pFrame);
		void	HtoT(XFrame *pFrame);

		void	SetParam(UINT uWaitResend, UINT uRepeatOnError);

	public:
		virtual ECommError	Initialize(CPhysLayer *pPhys, SCommPara *pPara, LPCTSTR cszConnect);

	// Communication functions
	public:
		virtual ECommError	Send    (BYTE byCommand,   BYTE *pData,   UINT uLen);
		virtual ECommError	Receive (BYTE *bypCommand, BYTE **ppData, UINT *upLen);

	protected:
		virtual ECommError BuildCRC(XFrame *pFrame);
		virtual ECommError CheckCRC(XFrame *pFrame);

		virtual ECommError RecvAck(USHORT uBlock);
		virtual ECommError SendInfoBlock(USHORT uBlock, BYTE byType);
};

#endif

/* ---------------------------------------------------------------------------- */
