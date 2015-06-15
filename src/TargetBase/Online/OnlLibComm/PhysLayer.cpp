/* $Header: /4CReleased/V2.20.00/TargetBase/Online/OnlLibComm/PhysLayer.cpp 2     8.03.07 10:39 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PhysLayer.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/Online/OnlLibComm/PhysLayer.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibComm
 *
 * =CURRENT 	 $Date: 8.03.07 10:39 $
 *			 $Revision: 2 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "PhysLayer.h"
#include "CommPara.h"
#include <winsock2.h>

//----  Static Initializations:   ----------------------------------*

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CPhysLayer, CObject)

/* CPhysLayer
 * ----------------------------------------------------------------------------
 */
CPhysLayer::CPhysLayer()
{
	m_bConnected	= FALSE;
	m_bCommOpen		= FALSE;

	m_uHeader		= sizeof(USHORT);
	m_uOffset		= 0;
}

/* ~CPhysLayer
 * ----------------------------------------------------------------------------
 */
CPhysLayer::~CPhysLayer()
{

}

/* Initialize
 * ----------------------------------------------------------------------------
 */
ECommError CPhysLayer::Initialize (SCommPara *pPara, LPCTSTR cszConnect)
{
	ASSERT (pPara);

	if (pPara == NULL)
	{
	    TrcPrint(TRC_ERROR, _T("[CPhysLayer][Initialize] NULL Pointer.\n"));
		return ceError;
	}

	memcpy(&m_Para, pPara, sizeof(m_Para));
	m_sConnect.Empty();

	if (cszConnect != NULL)
	{
		m_sConnect = cszConnect;
	}

	return ceOK; 
}

/* IsConnected
 * ----------------------------------------------------------------------------
 */
BOOL CPhysLayer::IsConnected() 
{ 
	return m_bConnected; 
}

/* IsCommOpen
 * ----------------------------------------------------------------------------
 */
BOOL CPhysLayer::IsCommOpen() 
{ 
	return m_bCommOpen; 
}

/* IsWindowsNTBased
 * ----------------------------------------------------------------------------
 */
BOOL CPhysLayer::IsWindowsNTBased()
{
	OSVERSIONINFO osVersion;
	osVersion.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);

	GetVersionEx(&osVersion);

	return	osVersion.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

/* SetSendTimeOut
 * ----------------------------------------------------------------------------
 */
ECommError CPhysLayer::SetSendTimeOut(DWORD dwTimeOut)
{
	m_Para.dwSendTimeOut = dwTimeOut;

	return ceOK; 
}

/* SetRecvTimeOut
 * ----------------------------------------------------------------------------
 */
ECommError CPhysLayer::SetRecvTimeOut(DWORD dwTimeOut)
{
	m_Para.dwRecvTimeOut = dwTimeOut;

	return ceOK; 
}

/* GetDLLDir
 * ----------------------------------------------------------------------------
 */
BOOL CPhysLayer::GetDLLDir(CString *sDir)
{
	sDir->Empty();

	if (GetModuleFileName(NULL, sDir->GetBuffer(1000 + 1), 1000) == 0)
	{
	    TrcPrint(TRC_ERROR, _T("[CPhysLayer][GetDLLDir] GetModuleFileName() failed.\n"));

		sDir->ReleaseBuffer();
		return FALSE;
	}

	sDir->ReleaseBuffer();

	int iFind = sDir->ReverseFind(_T('\\'));
	if (iFind == -1)
	{
	    TrcPrint(TRC_ERROR, _T("[CPhysLayer][GetDLLDir] ReverseFind failed.\n"));

		return FALSE;
	}

	LPSTR szDummy = sDir->GetBuffer(iFind + 1);
	szDummy[iFind] = 0;
	sDir->ReleaseBuffer();

	return TRUE;
}

/* SetHeaderSize
 * ----------------------------------------------------------------------------
 */
void CPhysLayer::SetHeaderSize(UINT uHeader, UINT uOffset)
{
	ASSERT(uHeader >= uOffset + sizeof(USHORT));

	m_uHeader = uHeader;
	m_uOffset = uOffset;
}

/* HtoT
 * ----------------------------------------------------------------------------
 */
USHORT CPhysLayer::HtoT(USHORT uShort)
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
USHORT CPhysLayer::TtoH(USHORT uShort)
{
	if (m_Para.bBigEndian)
	{
		return ntohs(uShort);
	}

	return uShort;
}

/* ---------------------------------------------------------------------------- */
