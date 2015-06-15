/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/PhysLayer.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PhysLayer.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/PhysLayer.h $
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

#ifndef _PHYSLAYER_H_
#define _PHYSLAYER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _COMMPARA_H_
#include "CommPara.h"
#endif

/**
 *  class CPhysLayer
 *
 */
class _ONLCOMM CPhysLayer : public CObject  
{
	DECLARE_DYNAMIC(CPhysLayer)

	protected:
		CPhysLayer();

	public:
		virtual ~CPhysLayer();

	protected:
		SCommPara	m_Para;
		CString		m_sConnect;

		BOOL		m_bConnected;		// Flag if logical connected to PLC
		BOOL		m_bCommOpen;		// Flag if physical connected to PLC

		UINT		m_uHeader;			// Size of protocol header
		UINT		m_uOffset;			// Offset of data size in protocol header

	public:
		virtual ECommError	Initialize (SCommPara *pPara, LPCTSTR cszConnect);

	public:
		BOOL	IsWindowsNTBased();
		void	SetHeaderSize(UINT uHeader, UINT uOffset);

	public:
		virtual BOOL		IsConnected();
		virtual BOOL		IsCommOpen();

	// Communication functions
	public:
		virtual ECommError	OpenComm()	 = 0;
		virtual ECommError	CloseComm()  = 0;
		virtual ECommError	Flush()		 = 0;

		virtual ECommError	Connect()	 = 0;
		virtual ECommError	Disconnect() = 0;

		virtual ECommError	Send	(BYTE *pData, UINT uLen)	= 0;
		virtual ECommError	Receive (BYTE *pData, UINT *upLen)	= 0;
		
	public:
		virtual ECommError	SetSendTimeOut(DWORD dwTimeOut);
		virtual ECommError	SetRecvTimeOut(DWORD dwTimeOut);

	public:
		static  BOOL GetDLLDir(CString *sDir);

        USHORT	TtoH(USHORT uShort);
		USHORT	HtoT(USHORT uShort);
};

#endif

/* ---------------------------------------------------------------------------- */
