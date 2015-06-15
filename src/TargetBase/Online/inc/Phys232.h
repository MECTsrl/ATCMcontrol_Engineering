/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/Phys232.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Phys232.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/Phys232.h $
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

#if !defined(AFX_PHYS232_H__295D9C63_299E_11D5_872B_0050DA6AA705__INCLUDED_)
#define AFX_PHYS232_H__295D9C63_299E_11D5_872B_0050DA6AA705__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _PHYSLAYER_H_
#include "PhysLayer.h"
#endif

// special characters
#define SC_DLE  0x11    // escape character
#define SC_STX  0x02    // start block character
#define SC_ETX  0x03    // end block character

/**
 *  class CPhysPipe
 *
 */
class CPhys232 : public CPhysLayer  
{
	DECLARE_DYNCREATE(CPhys232)
	protected:
		CPhys232();

	public:
		virtual ~CPhys232();

	public:
		virtual ECommError	Initialize (SCommPara *pPara, LPCTSTR cszConnect);

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
		void	SetRecvCharTimeOut(DWORD dwRecvCharTimeOut);

	protected:
		ECommError AddProtocol   (BYTE *pSrc, UINT uSrc, BYTE *pDest, UINT *upDest);
		ECommError GetByte (BYTE *pByte);

	protected:
		HANDLE	m_hComm;
        BYTE	*m_pBuffer;
        DWORD	m_dwBuffer;

		DWORD	m_dwRecvCharTimeOut;
};

enum EProtState
{
	eStart,
	eStart2,
	eData,
	eEscape,
	eFinish
};

#endif

/* ---------------------------------------------------------------------------- */
