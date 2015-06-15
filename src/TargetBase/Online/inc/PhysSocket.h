/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/PhysSocket.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PhysSocket.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/PhysSocket.h $
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

#ifndef _PHYSSOCKET_H_
#define _PHYSSOCKET_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <winsock2.h>

#ifndef _PHYSLAYER_H_
#include "PhysLayer.h"
#endif

/**
 *  class CPhysSocket
 *
 */
class _ONLCOMM CPhysSocket : public CPhysLayer  
{
	DECLARE_DYNCREATE(CPhysSocket)
	protected:
		CPhysSocket();

	public:
		virtual ~CPhysSocket();

	protected:
        SOCKET	m_Socket;
		USHORT	m_uPort;

	public:
		virtual ECommError	SetParam(USHORT uPort);

    // Communication functions
	public:
		virtual ECommError	OpenComm();
		virtual ECommError	CloseComm();
		
		virtual ECommError	Flush();

		virtual ECommError	Connect();
		virtual ECommError	Disconnect();

		virtual ECommError	Send	(BYTE *pData, UINT uLen);
		virtual ECommError	Receive (BYTE *pData, UINT *upLen);

	public:
		virtual ECommError	SetSendTimeOut(DWORD dwTimeOut);
		virtual ECommError	SetRecvTimeOut(DWORD dwTimeOut);
		
	// Socket Functions
	protected:
		ULONG	GetAddress();

};

#endif

/* ---------------------------------------------------------------------------- */
