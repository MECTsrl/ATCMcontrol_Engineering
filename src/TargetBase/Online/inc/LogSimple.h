/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/LogSimple.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: LogSimple.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/LogSimple.h $
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

#ifndef _LOGSIMPLE_H_
#define _LOGSIMPLE_H_

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
 *  class CLogSimple
 *
 */
class _ONLCOMM CLogSimple : public CLogLayer  
{
	DECLARE_DYNCREATE(CLogSimple)
	protected:
		CLogSimple();
	
	public:
		virtual ~CLogSimple();

	protected:
		XBlock	*m_pDataBlock;

		BYTE	*m_pRecvData;
		UINT	m_uRecvLen;

		USHORT	m_uCurBlock;

	public:
		void	TtoH(XBlock *pBlock);
		void	HtoT(XBlock *pBlock);

	public:
		virtual ECommError	Initialize(CPhysLayer *pPhys, SCommPara *pPara, LPCTSTR cszConnect);

	// Communication functions
	public:
		virtual ECommError	Send    (BYTE byCommand,   BYTE *pData,   UINT uLen);
		virtual ECommError	Receive (BYTE *bypCommand, BYTE **ppData, UINT *upLen);

	public:
		virtual ECommError  SendBlock(BYTE byCommand,   BYTE bySequence, USHORT uBlock, BYTE byLast,   BYTE *pData,   UINT uLen);
		virtual ECommError  RecvBlock(BYTE *bypCommand, BYTE bySequence, USHORT uBlock, BYTE *bypLast, BYTE **ppData, UINT *upLen);

};

#endif

/* ---------------------------------------------------------------------------- */
