/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/CommDLL.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CommDLL.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlTest/CommDLL.cpp $
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
#include "CommDLL.h"
#include "4CLayer.h"

#include "vmSharedDef.h"

/* ----  Local Defines:   ----------------------------------------------------- */

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/* ----  Global Variables:   -------------------------------------------------- */

/* ----  Local Functions:   --------------------------------------------------- */

/* ----  Implementations:   --------------------------------------------------- */


IMPLEMENT_DYNAMIC(CCommDLL, CCommBase);


/* CCommDLL
 * ----------------------------------------------------------------------------
 */
CCommDLL::CCommDLL() : CCommBase(OCT_ONLCOMM_DLL)
{
	m_hModule	= NULL;
	m_pLayer	= NULL;
}


/* ~CCommDLL
 * ----------------------------------------------------------------------------
 */
CCommDLL::~CCommDLL()
{

}


/* Alloc
 * ----------------------------------------------------------------------------
 */
BYTE *CCommDLL::Alloc(UINT uLen)
{
	return (BYTE *)HeapAlloc(GetProcessHeap(), 0, uLen);
}


/* Free
 * ----------------------------------------------------------------------------
 */
void CCommDLL::Free(BYTE **ppMemory)
{
	HeapFree(GetProcessHeap(), 0, *ppMemory);
	*ppMemory = NULL;
}


/* SetSwap
 * ----------------------------------------------------------------------------
 */
void CCommDLL::SetSwap(BOOL bSwap)
{
	m_bSwap = bSwap;
}


/* Swap16
 * ----------------------------------------------------------------------------
 */
unsigned short CCommDLL::Swap16(unsigned short uVal)
{
	unsigned short uRet = uVal;

    if (m_bSwap)
    {
		IEC_BYTE *pVal = (IEC_BYTE *)&uVal;
		IEC_BYTE *pRet = (IEC_BYTE *)&uRet;

		pRet[0] = pVal[1];
		pRet[1] = pVal[0];
    }

    return uRet;
}


/* Swap32
 * ----------------------------------------------------------------------------
 */
unsigned int CCommDLL::Swap32(unsigned int ulVal)
{
	unsigned int ulRet = ulVal;

    if (m_bSwap)
    {
		IEC_BYTE *pVal = (IEC_BYTE *)&ulVal;
		IEC_BYTE *pRet = (IEC_BYTE *)&ulRet;

		pRet[0] = pVal[3];
		pRet[1] = pVal[2];
		pRet[2] = pVal[1];
		pRet[3] = pVal[0];
    }

    return ulRet;
}


/* Swap64
 * ----------------------------------------------------------------------------
 */
unsigned __int64 CCommDLL::Swap64(unsigned __int64	ullVal)
{
	unsigned __int64 ullRet = ullVal;

    if (m_bSwap)
    {
		IEC_BYTE *pVal = (IEC_BYTE *)&ullVal;
		IEC_BYTE *pRet = (IEC_BYTE *)&ullRet;

		pRet[0] = pVal[7];
		pRet[1] = pVal[6];
		pRet[2] = pVal[5];
		pRet[3] = pVal[4];
		pRet[4] = pVal[3];
		pRet[5] = pVal[2];
		pRet[6] = pVal[1];
		pRet[7] = pVal[0];
    }

    return ullRet;
}


/* Transfer
 * ----------------------------------------------------------------------------
 */
BOOL CCommDLL::Transfer(BYTE *byCommand, BYTE **ppData, UINT *upLen)
{
	BOOL bOK = m_pLayer->Transfer(byCommand, ppData, upLen);

	return bOK;
}


/* ShowError
 * ----------------------------------------------------------------------------
 */
void CCommDLL::ShowError(LPCTSTR szMessage, BYTE *pData)
{
	CString s;

	s.Format("Error: %s.\nError Number: 0x%04x", szMessage, *(unsigned short *)pData);
	AfxMessageBox(s, MB_OK | MB_ICONEXCLAMATION);
}


/* Load
 * ----------------------------------------------------------------------------
 */
void CCommDLL::Load(CDialog *pDlg, LPCTSTR szFile, LPCTSTR szPara)
{
    if (m_bLoaded == TRUE)
    {
		Unload(pDlg);
    }
   
	m_hModule = LoadLibrary(szFile);
    if (m_hModule == NULL)
    {
        pDlg->MessageBox("Error: Loading library OnlComm.dll.", "OnlCommTest", MB_OK | MB_ICONEXCLAMATION);
        return;
    }

    LPCONSTRUCT4CLAYER lpConstruct4CLayer = (LPCONSTRUCT4CLAYER)GetProcAddress(m_hModule, "Construct4CLayer");
    if (lpConstruct4CLayer == NULL)
    {
        pDlg->MessageBox("Error: Getting ProcAddress Construct4CLayer.", "OnlCommTest", MB_OK | MB_ICONEXCLAMATION);
        FreeLibrary(m_hModule);
        m_hModule = NULL;
        return;
    }

    m_pLayer = lpConstruct4CLayer(szPara);
    if (m_pLayer == NULL)
    {
        pDlg->MessageBox("Error: Creating 4CLayer.", "OnlCommTest", MB_OK | MB_ICONEXCLAMATION);
        FreeLibrary(m_hModule);
        m_hModule = NULL;
        return;
    }

	m_bLoaded = TRUE;
}


/* Unload
 * ----------------------------------------------------------------------------
 */
void CCommDLL::Unload(CDialog *pDlg)
{
	Logout(pDlg);
	Disconnect(pDlg);

	if (m_bLoaded == FALSE)
	{
		return;
	}

    if (m_pLayer != NULL)
	{
		LPDISPOSE4CLAYER lpDispose4CLayer = (LPDISPOSE4CLAYER)GetProcAddress(m_hModule, "Dispose4CLayer");
		if (lpDispose4CLayer == NULL)
		{
			pDlg->MessageBox("Error: Getting ProcAddress Dispose4CLayer.", "OnlCommTest", MB_OK | MB_ICONEXCLAMATION);  
		}
		else
		{
			lpDispose4CLayer(m_pLayer);
			m_pLayer = NULL;
		}
	}

	if (m_hModule != NULL)
	{
        FreeLibrary(m_hModule);
        m_hModule = NULL;
	}

	m_bLoaded = FALSE;
}


/* Connect
 * ----------------------------------------------------------------------------
 */
void CCommDLL::Connect(CDialog *pDlg, LPCTSTR szPara)
{
    if (m_bConnected == TRUE)
	{
		Disconnect(pDlg);
	}

	if (!m_pLayer->OpenComm())
    {
        pDlg->MessageBox("Error: OpenComm failed.", "OnlCommTest", MB_OK | MB_ICONEXCLAMATION);
        return;
    }	
    if (!m_pLayer->Connect())
    {
        pDlg->MessageBox("Error: Connect failed.", "OnlCommTest", MB_OK | MB_ICONEXCLAMATION);
        return;
    }	

	m_bConnected = TRUE;
}


/* Disconnect
 * ----------------------------------------------------------------------------
 */
void CCommDLL::Disconnect(CDialog *pDlg)
{
	Logout(pDlg);

	if (m_bConnected == FALSE)
	{
		return;
	}

    if (m_pLayer->IsConnected())
    {
        if (!m_pLayer->Disconnect())
        {
            pDlg->MessageBox("Error: Disconnect failed.", "OnlCommTest", MB_OK | MB_ICONEXCLAMATION);
        }	
    }
    if (!m_pLayer->CloseComm())
    {
        pDlg->MessageBox("Error: CloseComm failed.", "OnlCommTest", MB_OK | MB_ICONEXCLAMATION);
    }	

	m_bConnected = FALSE;
}


/* Login
 * ----------------------------------------------------------------------------
 */
void CCommDLL::Login(CDialog *pDlg)
{
	if (m_bLogedIn == TRUE)
	{
		Logout(pDlg);
	}

	UINT uLen		= 0;
	BYTE *pData 	= Alloc(uLen);
	BYTE byCommand  = CMD_GET_PROJ_VERSION;
	   
	BOOL bOK = Transfer(&byCommand, &pData, &uLen);

	if (! bOK)
	{
		pDlg->MessageBox("Error: Get Project Version", "OnlCommTest", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	else if (bOK && byCommand != CMD_GET_PROJ_VERSION)
	{
		ShowError("Get Project Version", pData);
		return;
	}

	if (uLen != 16)
	{
		pDlg->MessageBox("Error: Login failed unexpected data size returned.", "OnlCommTest", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	byCommand = CMD_LOGIN;
	bOK = Transfer(&byCommand, &pData, &uLen);

	if (! bOK)
	{
		pDlg->MessageBox("Error: Login failed.", "OnlCommTest", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	else if (bOK && byCommand != CMD_LOGIN)
	{
		ShowError("Login failed", pData);
		return;
	}  

	Free(&pData);

	m_bLogedIn = TRUE;
}


/* Logout
 * ----------------------------------------------------------------------------
 */
void CCommDLL::Logout(CDialog *pDlg)
{
	if (m_bLogedIn == FALSE)
	{
		return;
	}

	BYTE byCommand	= CMD_LOGOUT;
	UINT uLen		= 0;
	BYTE *pData		= Alloc(uLen);

    if (! Transfer(&byCommand, &pData, &uLen))
    {
        pDlg->MessageBox("Error: Logout failed.", "OnlCommTest", MB_OK | MB_ICONEXCLAMATION);
    }

	Free(&pData);

	m_bLogedIn = FALSE;
}


/* GetChildren
 * ----------------------------------------------------------------------------
 */
USHORT CCommDLL::GetChildren(CDialog *pDlg, BYTE *pParent, USHORT uParent, BYTE **ppChildren, USHORT *upChild)
{
	BYTE byCommand	= CMD_DBI_GET_CHILDREN;
	UINT uLen		= uParent;
	BYTE *pData		= Alloc(uLen);

	memcpy(pData, pParent, uLen);
	
	BOOL bOK = Transfer(&byCommand, &pData, &uLen);
	
	IEC_UINT uRes = OK;

	if (! bOK)
	{
		uRes = ERR_ERROR;
	}
	if ((byCommand & 0x80u) != 0)
	{
		uRes = *(USHORT *)pData;
	}
	
	if (uRes == OK && *ppChildren != NULL)
	{
		Free(ppChildren);
		*upChild	= 0;
	}

	if (uRes != OK)
	{
		CString sMsg;
		sMsg.Format("Get children failed (0x%04x).", uLen);
		
		pDlg->MessageBox(sMsg, "OnlCommTest", MB_OK | MB_ICONSTOP);

		if (pData != NULL)
		{
			Free(&pData);
		}
		uLen = 0;
	}

	if (uRes == OK)
	{
		*ppChildren = pData;
		*upChild	= uLen;
	}

	return uRes;
}


/* GetAddress
 * ----------------------------------------------------------------------------
 */
USHORT CCommDLL::GetAddress(CDialog *pDlg, BYTE *pVar, USHORT uVar, BYTE **ppAddr, USHORT *upAddr)
{
	BYTE byCommand	= CMD_DBI_GET_ADDRESS;
	UINT uLen		= uVar;
	BYTE *pData		= Alloc(uLen);

	memcpy(pData, pVar, uLen);
	
	BOOL bOK = Transfer(&byCommand, &pData, &uLen);
	
	IEC_UINT uRes = OK;

	if (! bOK)
	{
		uRes = ERR_ERROR;
	}
	if ((byCommand & 0x80u) != 0)
	{
		uRes = *(USHORT *)pData;
	}

	if (uRes != OK)
	{
		CString sMsg;
		sMsg.Format("Get address failed (0x%04x).", uRes);
		
		pDlg->MessageBox(sMsg, "OnlCommTest", MB_OK | MB_ICONSTOP);

		if (pData != NULL)
		{
			Free(&pData);
		}
		uLen = 0;
	}

	if (uRes == OK)
	{
		*ppAddr = pData;
		*upAddr = uLen;
	}

	return uRes;
}


/* GetValue
 * ----------------------------------------------------------------------------
 */
USHORT CCommDLL::GetValue(CDialog *pDlg, BYTE *pVar, USHORT uVar, BYTE **ppVal, USHORT *upVal)
{
	BYTE byCommand	= CMD_GET_VALUE;
	UINT uLen		= uVar;
	BYTE *pData		= Alloc(uLen);

	memcpy(pData, pVar, uLen);
	
	BOOL bOK = Transfer(&byCommand, &pData, &uLen);
	
	IEC_UINT uRes = OK;

	if (! bOK)
	{
		uRes = ERR_ERROR;
	}
	if ((byCommand & 0x80u) != 0)
	{
		uRes = *(USHORT *)pData;
	}

	if (uRes != OK)
	{
		CString sMsg;
		sMsg.Format("Get value failed (0x%04x.", uRes);
		
		pDlg->MessageBox(sMsg, "OnlCommTest", MB_OK | MB_ICONSTOP);

		Free(&pData);

		pData = NULL;
		uLen  = NULL;
	}

	*ppVal = pData;
	*upVal = uLen;

	return uRes;
}


/* SetValue
 * ----------------------------------------------------------------------------
 */
USHORT CCommDLL::SetValue(CDialog *pDlg, BYTE *pVar, USHORT uVar)
{
	BYTE byCommand	= CMD_SET_VALUE;
	UINT uLen		= uVar;
	BYTE *pData		= Alloc(uLen);

	memcpy(pData, pVar, uLen);
	
	BOOL bOK = Transfer(&byCommand, &pData, &uLen);
	
	IEC_UINT uRes = OK;

	if (! bOK)
	{
		uRes = ERR_ERROR;
	}
	if ((byCommand & 0x80u) != 0)
	{
		uRes = *(USHORT *)pData;
	}

	if (uRes != OK)
	{
		CString sMsg;
		sMsg.Format("Set value failed (0x%04x).", uRes);
		
		pDlg->MessageBox(sMsg, "OnlCommTest", MB_OK | MB_ICONSTOP);
	}

	Free(&pData);
	
	return uRes;
}


/* GetState
 * ----------------------------------------------------------------------------
 */
USHORT CCommDLL::GetState(CDialog *pDlg, UINT *upState)
{
	return OK;
}

/* ---------------------------------------------------------------------------- */
