/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibComm/LogLayer.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: LogLayer.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibComm/LogLayer.cpp $
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
#include "LogLayer.h"
#include "PhysLayer.h"

//----  Static Initializations:   ----------------------------------*

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CLogLayer, CObject)

/* CLogLayer
 * ----------------------------------------------------------------------------
 */
CLogLayer::CLogLayer()
{
	m_pPhys = NULL;
	memset(&m_Para, 0x00, sizeof(m_Para));

	srand ((unsigned)time(NULL));

	m_byCurCommand	= 0xff;
	m_byCurSequence	= (BYTE)rand();

	m_fpCallBack	= NULL;
	m_uMaxRecvProg	= MAX_RECV_PROG;
}

/* ~CLogLayer
 * ----------------------------------------------------------------------------
 */
CLogLayer::~CLogLayer()
{
	delete m_pPhys;
	m_pPhys = NULL;
}

/* Initialize
 * ----------------------------------------------------------------------------
 */
ECommError CLogLayer::Initialize (CPhysLayer *pPhys, SCommPara *pPara, LPCTSTR cszConnect)
{
	ASSERT (pPhys);
	ASSERT (pPara);

	if (pPhys == NULL || pPara == NULL)
	{
		return ceError;
	}

	m_pPhys = pPhys; 
	memcpy(&m_Para, pPara, sizeof(m_Para));
	m_sConnect.Empty();

	if (cszConnect != NULL)
	{
		m_sConnect = cszConnect;
	}

	ClearProgressCallBack();

	return ceOK; 
}

/* IsConnected
 * ----------------------------------------------------------------------------
 */
BOOL CLogLayer::IsConnected() 
{ 
	ASSERT(m_pPhys);

	return m_pPhys->IsConnected(); 
}

/* IsCommOpen
 * ----------------------------------------------------------------------------
 */
BOOL CLogLayer::IsCommOpen() 
{ 
	ASSERT(m_pPhys);

	return m_pPhys->IsCommOpen(); 
}

/* OpenComm
 * ----------------------------------------------------------------------------
 */
ECommError CLogLayer::OpenComm()
{
	return m_pPhys->OpenComm();
}

/* CloseComm
 * ----------------------------------------------------------------------------
 */
ECommError CLogLayer::CloseComm()
{
	ClearProgressCallBack();
	
	return m_pPhys->CloseComm();
}

/* Connect
 * ----------------------------------------------------------------------------
 */
ECommError CLogLayer::Connect()
{
	return m_pPhys->Connect();
}

/* Disconnect
 * ----------------------------------------------------------------------------
 */
ECommError CLogLayer::Disconnect()
{
	ClearProgressCallBack();

	return m_pPhys->Disconnect();
}

/* GetPhysLayer
 * ----------------------------------------------------------------------------
 */
CPhysLayer* CLogLayer::GetPhysLayer()
{
    return m_pPhys;
}

/* SetProgressCallback
 * ----------------------------------------------------------------------------
 */
void CLogLayer::SetProgressCallBack(PROGRESS_CALLBACK fpCallBack)
{
	m_fpCallBack = fpCallBack;
}

/* ClearProgressCallback
 * ----------------------------------------------------------------------------
 */
void CLogLayer::ClearProgressCallBack()
{
	m_fpCallBack = NULL;
}

/* ---------------------------------------------------------------------------- */
