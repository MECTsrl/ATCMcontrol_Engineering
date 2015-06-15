/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/LogLayer.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: LogLayer.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/LogLayer.h $
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

#ifndef _LOGLAYER_H_
#define _LOGLAYER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _COMMPARA_H_
#include "CommPara.h"
#endif

#define MAX_RECV_PROG 25

#define PROGRESS(c,m)								\
						if (m_fpCallBack != NULL)	\
						{							\
							m_fpCallBack((c),(m));	\
						}

#define PROGRESS_LAST(m)							\
						if (m_fpCallBack != NULL)	\
						{							\
							m_fpCallBack((m),(m));	\
							m_fpCallBack = NULL;	\
						}


class CPhysLayer;

/**
 *  class CLogLayer
 *
 */
class _ONLCOMM CLogLayer : public CObject  
{
	DECLARE_DYNAMIC(CLogLayer)

	protected:
		CLogLayer();

	public:
		virtual ~CLogLayer();

	protected:
		CPhysLayer	*m_pPhys;		// Physical layer
		SCommPara	m_Para;
		CString		m_sConnect;

		BYTE		m_byCurCommand;
		BYTE		m_byCurSequence;

		PROGRESS_CALLBACK m_fpCallBack;
		UINT		m_uMaxRecvProg;

	public:
		virtual ECommError	Initialize(CPhysLayer *pPhys, SCommPara *pPara, LPCTSTR cszConnect);
        virtual CPhysLayer* GetPhysLayer();

	public:
		virtual BOOL		IsConnected();
		virtual BOOL		IsCommOpen();

	// Communication functions
	public:
		virtual ECommError	OpenComm();
		virtual ECommError	CloseComm();

		virtual ECommError	Connect();
		virtual ECommError	Disconnect();

		virtual ECommError	Send    (BYTE byCommand,   BYTE *pData,   UINT uLen)	= 0;
		virtual ECommError	Receive (BYTE *bypCommand, BYTE **ppData, UINT *upLen)	= 0;

	// Progress bar for data transfer
	public:
		virtual void SetProgressCallBack(PROGRESS_CALLBACK fpCallBack);
		virtual void ClearProgressCallBack();
};

#endif

/* ---------------------------------------------------------------------------- */
