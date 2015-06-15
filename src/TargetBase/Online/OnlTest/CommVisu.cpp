/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/CommVisu.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CommVisu.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/CommVisu.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlTest
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
#include "onltest.h"
#include "CommVisu.h"

#include "vmSharedDef.h"
#include "visShared.h"

/* ----  Local Defines:   ----------------------------------------------------- */

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/* ----  Global Variables:   -------------------------------------------------- */

/* ----  Local Functions:   --------------------------------------------------- */

/* ----  Implementations:   --------------------------------------------------- */


IMPLEMENT_DYNAMIC(CCommVisu, CCommBase);


/* CCommVisu
 * ----------------------------------------------------------------------------
 */
CCommVisu::CCommVisu() : CCommBase(OCT_VISUCOMM_LIB)
{
	m_bLoaded = TRUE;
}


/* ~CCommVisu
 * ----------------------------------------------------------------------------
 */
CCommVisu::~CCommVisu()
{
	
}


/* Alloc
 * ----------------------------------------------------------------------------
 */
BYTE *CCommVisu::Alloc(UINT uLen)
{
	return visAlloc(uLen);
}


/* Free
 * ----------------------------------------------------------------------------
 */
void CCommVisu::Free(BYTE **ppMemory)
{
	visFree(ppMemory);
}


/* Swap16
 * ----------------------------------------------------------------------------
 */
unsigned short CCommVisu::Swap16(unsigned short	uVal)
{
	return visSwap16(uVal);
}


/* Swap32
 * ----------------------------------------------------------------------------
 */
unsigned int CCommVisu::Swap32(unsigned int	ulVal)
{
	return visSwap32(ulVal);
}


/* Swap64
 * ----------------------------------------------------------------------------
 */
unsigned __int64 CCommVisu::Swap64(unsigned __int64	ullVal)
{
	unsigned __int64 ull;

	memcpy(&ull, &ullVal, sizeof(ullVal));
	
	visSwap64((VIS_DATA *)&ull);

	memcpy(&ullVal, &ull, sizeof(ullVal));

	return ullVal;
}


/* Load
 * ----------------------------------------------------------------------------
 */
void CCommVisu::Load(CDialog *pDlg, LPCTSTR szFile, LPCTSTR szPara)
{
    if (m_bLoaded == TRUE)
    {
		Unload(pDlg);
    }

	m_bLoaded = TRUE;
}


/* Unload
 * ----------------------------------------------------------------------------
 */
void CCommVisu::Unload(CDialog *pDlg)
{
	Logout(pDlg);
	Disconnect(pDlg);

	if (m_bLoaded == FALSE)
	{
		return;
	}

}


/* Connect
 * ----------------------------------------------------------------------------
 */
void CCommVisu::Connect(CDialog *pDlg, LPCTSTR szPara)
{
	CString sPara(szPara);
	
	IEC_UINT uRes = OK;

	if (m_bConnected == TRUE)
	{
		Disconnect(pDlg);
	}

	int iPos = sPara.Find(';');
	if (iPos == -1)
	{
		uRes = visInitialize(szPara, VIS_PORT_SCAN);
	}
	else
	{
		CString sPort = sPara.Right(sPara.GetLength() - iPos - 1);

		sPara = sPara.Left(iPos);

		uRes = visInitialize(sPara, (IEC_UINT)strtoul(sPort, NULL, 10));
	}

	if (uRes != OK)
	{
		CString sMsg;
		sMsg.Format("Connect to control '%s' failed (0x%04x / %s).", sPara, uRes, visError(uRes));
		
		pDlg->MessageBox(sMsg, "OnlCommTest", MB_OK | MB_ICONSTOP);
	}

	m_bConnected = (uRes == OK);
}


/* Disconnect
 * ----------------------------------------------------------------------------
 */
void CCommVisu::Disconnect(CDialog *pDlg)
{
	Logout(pDlg);

	if (m_bConnected == FALSE)
	{
		return;
	}

	visFinalize();

	m_bConnected = FALSE;
}


/* Login
 * ----------------------------------------------------------------------------
 */
void CCommVisu::Login(CDialog *pDlg)
{
	if (m_bLogedIn == TRUE)
	{
		Logout(pDlg);
	}

	IEC_UINT uRes = visLogin();

	if (uRes != OK)
	{
		CString sMsg;
		sMsg.Format("Login to control failed (0x%04x / %s).", uRes, visError(uRes));
		
		pDlg->MessageBox(sMsg, "OnlCommTest", MB_OK | MB_ICONSTOP);
	}

	m_bLogedIn = (uRes == OK);
}


/* Logout
 * ----------------------------------------------------------------------------
 */
void CCommVisu::Logout(CDialog *pDlg)
{
	if (m_bLogedIn == FALSE)
	{
		return;
	}

	visLogout();

	m_bLogedIn = FALSE;
}


/* GetChildren
 * ----------------------------------------------------------------------------
 */
USHORT CCommVisu::GetChildren(CDialog *pDlg, BYTE *pParent, USHORT uParent, BYTE **ppChildren, USHORT *upChild)
{
	USHORT uRes = visGetChildren(&pParent, &uParent, FALSE);

	if (uRes == OK && *ppChildren != NULL)
	{
		Free(ppChildren);
		*upChild	= 0;
	}

	if (uRes != OK)
	{
		CString sMsg;
		sMsg.Format("Get children failed (0x%04x / %s).", uRes, visError(uRes));
		
		pDlg->MessageBox(sMsg, "OnlCommTest", MB_OK | MB_ICONSTOP);
	}

	if (uRes == OK)
	{
		*ppChildren = pParent;
		*upChild	= uParent;
	}

	return uRes;
}


/* GetAddress
 * ----------------------------------------------------------------------------
 */
USHORT CCommVisu::GetAddress(CDialog *pDlg, BYTE *pVar, USHORT uVar, BYTE **ppAddr, USHORT *upAddr)
{
	UCHAR  usType;

	USHORT uRes = visGetAddrType(&pVar, &uVar, &usType, FALSE);

	if (uRes == OK && *ppAddr != NULL)
	{
		Free(ppAddr);
		*upAddr = 0;
	}

	if (uRes != OK)
	{
		CString sMsg;
		sMsg.Format("Get address failed (0x%04x / %s).", uRes, visError(uRes));
		
		pDlg->MessageBox(sMsg, "OnlCommTest", MB_OK | MB_ICONSTOP);
	}

	if (uRes == OK)
	{
		*ppAddr = pVar;
		*upAddr = uVar;
	}

	return uRes;
}


/* GetValue
 * ----------------------------------------------------------------------------
 */
USHORT CCommVisu::GetValue(CDialog *pDlg, BYTE *pVar, USHORT uVar, BYTE **ppVal, USHORT *upVal)
{
	USHORT uRes = visGetValue(&pVar, &uVar, FALSE);

	if (uRes != OK)
	{
		CString sMsg;
		sMsg.Format("Get value failed (0x%04x / %s).", uRes, visError(uRes));
		
		pDlg->MessageBox(sMsg, "OnlCommTest", MB_OK | MB_ICONSTOP);

		Free(&pVar);

		pVar = NULL;
		uVar = NULL;
	}

	*ppVal = pVar;
	*upVal = uVar;

	return uRes;
}


/* SetValue
 * ----------------------------------------------------------------------------
 */
USHORT CCommVisu::SetValue(CDialog *pDlg, BYTE *pVar, USHORT uVar)
{
	USHORT uRes = visSetValue(&pVar, &uVar, FALSE);

	if (uRes != OK)
	{
		CString sMsg;
		sMsg.Format("Set value failed (0x%04x / %s).", uRes, visError(uRes));
		
		pDlg->MessageBox(sMsg, "OnlCommTest", MB_OK | MB_ICONSTOP);
	}

	return uRes;
}


/* GetState
 * ----------------------------------------------------------------------------
 */
USHORT CCommVisu::GetState(CDialog *pDlg, UINT *upState)
{
	USHORT uRes = visGetResourceState(upState);

	return uRes;
}

/* ---------------------------------------------------------------------------- */
