/* $Header: /4CReleased/V2.20.00/TargetBase/Online/OnlLibServ/CommObj.cpp 2     19.04.07 15:45 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CommObj.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/Online/OnlLibServ/CommObj.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 19.04.07 15:45 $
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
#include "Connection.h"

#include "Security.h"
#include "ControlMgt.h"

#import <msxml.dll> raw_interfaces_only named_guids rename_namespace("MSXML_4C")
using namespace MSXML_4C;

#include "XMLHelper\XMLDocument.h"
#include "XMLHelper\XMLNode.h"
#include "XMLHelper\XMLNodeList.h"
#include "XMLHelper\IdPathHelp.h"
#include "XMLHelper\XMLNodeIter.h"


/* CCommObj
 * ----------------------------------------------------------------------------
 */
CCommObj::CCommObj()
{
	m_pLayer = NULL;

	m_fpConstruct4CLayer = NULL;
	m_fpDispose4CLayer	 = NULL;

	m_hModule			= NULL;
	m_szCommParam[0]	= '\0';
}


/* ~CCommObj
 * ----------------------------------------------------------------------------
 */
CCommObj::~CCommObj()
{
	m_RegKey.Close();

	Dispose();
}


BOOL CCommObj::Construct(void)
{
	BOOL bDestruct;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		bDestruct = m_pLayer != NULL;
	
	} m_CritSec.Unlock();	// ------------------------------------------------

	if (bDestruct == TRUE)
	{
		Dispose();
	}

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		SendNLToEng();

		CString sMsg;

		sMsg.Format(_T("<initialize> %s"), g_StartParam);
		SendMsgToEng(sMsg);

		m_hModule = LoadLibrary(_T("OnlComm.dll"));
		if (m_hModule == NULL)
		{
			FC_DEBUGPRINT1(_T("FATAL> LoadLibrary(\"OnlComm.dll\") fails (%d)\n"), GetLastError());
			TrcPrint(TRC_ERROR, _T("FATAL> LoadLibrary(\"OnlComm.dll\") fails (%d)\n"), GetLastError());
			SendErrToEng(OPC_ERR_LOAD_ONLCOMM);
	
			m_CritSec.Unlock();

			return FALSE;
		}

		FARPROC lpConstruct4CLayer	= GetProcAddress(m_hModule, "Construct4CLayer");
		FARPROC lpDispose4CLayer	= GetProcAddress(m_hModule, "Dispose4CLayer");

		if (lpConstruct4CLayer == NULL)
		{
			FC_DEBUGPRINT1(_T("FATAL> GetProcAddress(\"OnlComm.dll\", \"Construct4CLayer\") fails (%d)\n"), GetLastError());
			TrcPrint(TRC_ERROR, _T("FATAL> GetProcAddress(\"OnlComm.dll\", \"Construct4CLayer\") fails (%d)\n"), GetLastError());
			SendErrToEng(OPC_ERR_INIT_ONLCOMM);
			FreeLibrary(m_hModule);
			m_hModule = NULL;

			m_CritSec.Unlock();

			return FALSE;
		}
		if (lpDispose4CLayer == NULL)
		{
			FC_DEBUGPRINT1(_T("FATAL> GetProcAddress(\"OnlComm.dll\", \"Dispose4CLayer\") fails (%d)\n"), GetLastError());
			TrcPrint(TRC_ERROR, _T("FATAL> GetProcAddress(\"OnlComm.dll\", \"Dispose4CLayer\") fails (%d)\n"), GetLastError());
			SendErrToEng(OPC_ERR_INIT_ONLCOMM);
			FreeLibrary(m_hModule);
			m_hModule = NULL;

			m_CritSec.Unlock();

			return FALSE;
		}
		
		m_fpConstruct4CLayer = (LPCONSTRUCT4CLAYER)lpConstruct4CLayer;
		m_fpDispose4CLayer	 = (LPDISPOSE4CLAYER)lpDispose4CLayer;
			

		CRegKey keySW;
		LONG lRes = keySW.Open(HKEY_LOCAL_MACHINE, _T("Software"), KEY_READ);
		if (lRes == ERROR_SUCCESS)
		{
			CRegKey key4C;
			lRes = key4C.Open(keySW, g_cs4Control, KEY_READ);
			if (lRes == ERROR_SUCCESS)
			{
				CRegKey keyApp;
				lRes = keyApp.Open(key4C, g_csApp, KEY_READ);
				if (lRes == ERROR_SUCCESS)
				{
					CRegKey keySysVar;
					lRes = keySysVar.Open(keyApp, g_csSysVar, KEY_READ);
					if (lRes == ERROR_SUCCESS)
					{
						lRes = m_RegKey.Open(keySysVar, g_StartParam, KEY_READ);
						if (lRes == ERROR_SUCCESS)
						{
							DWORD dwCommParam = sizeof(m_szCommParam);
							lRes = m_RegKey.QueryValue(m_szCommParam, _T("CommParam"), &dwCommParam);
						}
					}
				}
			}
		}

		if (lRes != ERROR_SUCCESS)
		{
			lstrcpy(m_szCommParam, g_StartParam);
		}

		CString sPara(m_szCommParam);
		sPara = sPara.Right(sPara.GetLength() - sPara.Find(_T(',')) - 1);

		sMsg.Format(_T("<connect>    %s"), sPara);
		SendMsgToEng(sMsg);

		m_pLayer = m_fpConstruct4CLayer(m_szCommParam);
		if (m_pLayer == NULL)
		{
			FC_DEBUGPRINT(_T("FATAL> OnlComm::Construct4CLayer fails\n"));
			TrcPrint(TRC_ERROR, _T("FATAL> OnlComm::Construct4CLayer fails\n"));
			SendErrToEng(OPC_ERR_CONS_ONLCOMM);
			FreeLibrary(m_hModule);
			m_hModule = NULL;

			m_CritSec.Unlock();

			return FALSE;
		}
	
	    ReadAdaptParameter();

	} m_CritSec.Unlock();	// ------------------------------------------------

	return TRUE;
}


/* Dispose
 * ----------------------------------------------------------------------------
 */
BOOL CCommObj::Dispose(void)
{
	m_CritSec.Lock();	// ----------------------------------------------------
	{
		if (m_pLayer != NULL && m_fpDispose4CLayer != NULL)
		{
			m_fpDispose4CLayer(m_pLayer);
			m_pLayer = NULL;
		}

		if (m_hModule != NULL)
		{
			FreeLibrary(m_hModule);
			m_hModule = NULL;
		}
		
		m_fpConstruct4CLayer = NULL;
		m_fpDispose4CLayer	 = NULL;
	
		m_RegKey.Close();

	} m_CritSec.Unlock();	// ------------------------------------------------
	
	return TRUE;
}


/* CheckCommParams
 * ----------------------------------------------------------------------------
 */
BOOL CCommObj::CheckCommParams(void)
{	 
	TCHAR szCommParam[256];
	DWORD dwCommParam = sizeof(szCommParam);
	LONG lRes = m_RegKey.QueryValue(szCommParam, _T("CommParam"), &dwCommParam);
	
	if (lRes == ERROR_SUCCESS && _tcscmp(m_szCommParam, szCommParam) != NULL)
	{
		FC_DEBUGPRINT(_T("CONNECT> CommParam changed\n"));
		TrcPrint(TRC_COMM_EVENT, _T("CONNECT> CommParam changed\n"));
		
		Dispose();

		if (! Construct())
		{
			return FALSE;
		}
		
		return TRUE;
	}

	return FALSE;
}


/* OpenComm
 * ----------------------------------------------------------------------------
 */
BOOL CCommObj::OpenComm(void)
{
	BOOL bOK = FALSE;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		if (m_pLayer != NULL)
		{
			bOK = m_pLayer->OpenComm();
		}

	} m_CritSec.Unlock();	// ------------------------------------------------

	return bOK;
}


/* CloseComm
 * ----------------------------------------------------------------------------
 */
BOOL CCommObj::CloseComm(void)
{
	BOOL bOK = FALSE;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		if (m_pLayer != NULL)
		{
			bOK = m_pLayer->CloseComm();
		}

	} m_CritSec.Unlock();	// ------------------------------------------------

	return bOK;
}


/* Connect
 * ----------------------------------------------------------------------------
 */
BOOL CCommObj::Connect(void)
{
	BOOL bOK = FALSE;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		if (m_pLayer != NULL)
		{
			bOK = m_pLayer->Connect();
		}

	} m_CritSec.Unlock();	// ------------------------------------------------

	return bOK;
}


/* Disconnect
 * ----------------------------------------------------------------------------
 */
BOOL CCommObj::Disconnect(void)
{
	BOOL bOK = FALSE;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		if (m_pLayer != NULL)
		{
			bOK = m_pLayer->Disconnect();
		}

	} m_CritSec.Unlock();	// ------------------------------------------------

	return bOK;
}


/* ReadAdaptParameter
 * ----------------------------------------------------------------------------
 */
BOOL CCommObj::ReadAdaptParameter(void)
{
	// search for a file in the same directory as the executable of this module
	// the used kad file is the same as the AddOn handler kad file
	// the name should match the following id: *_Addon.kad
	// the root xml tag is ADDON
	// the root node has sub nodes called FIRMWARE
	// these nodes have sub nodes called ONLSRV, 
	// these contain the attributes containing the adaptable parameter

	// attributes used: DEBUGDL=0/1  debug download enabled

	CString strModulePath;

	HMODULE hMod = GetModuleHandle(NULL);

	GetModuleFileName(hMod, strModulePath.GetBuffer(MAX_PATH), MAX_PATH);
	strModulePath.ReleaseBuffer();

	int i = strModulePath.ReverseFind(_T('\\'));
	if (i>0)
	{
		strModulePath = strModulePath.Left(i);
	}

	strModulePath += _T("\\*_Addon.kad");

	
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(strModulePath, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return FALSE;
	} 
	  
	FindClose(hFind);

	CString strKADFileName = FindFileData.cFileName;

	CXMLDocument doc;
	
	if (!doc.Load(strKADFileName))
	{
		return FALSE;
	}

	CXMLNode rootNode;

	if (!doc.GetRootNode(rootNode))
	{
		return FALSE;
	}

	CString str;
	rootNode.GetTagName(str);
	if (str.Compare(_T("ADDON"))!=0)
	{
		return FALSE;
	}

	CXMLNodeList nodeList;

	rootNode.GetChildNodeList(nodeList);
	nodeList.Reset();

	CXMLNode nodeChild;
	while (nodeList.Next(nodeChild))
	{
		nodeChild.GetTagName(str);
		if (str.Compare(_T("FIRMWARE"))!=0)
		{
			continue;
		}

		CString strVersion;
		if (!nodeChild.GetAttribute(_T("VERSION"), strVersion))
		{
			continue;
		}

		DWORD dwKADFirmware = _ttoi(strVersion);
		if (m_pLayer->GetCommPara()->dwFirmware != dwKADFirmware)
		{
			continue;
		}

		CXMLNodeList nodeListFW;

		nodeChild.GetChildNodeList(nodeListFW);
		nodeListFW.Reset();

		CXMLNode nodeFWChild;
		while (nodeListFW.Next(nodeFWChild))
		{
			nodeFWChild.GetTagName(str);
			if (str.Compare(_T("ONLSRV"))!=0)
			{
				continue;
			}

			CString strDebugDL;
			if (!nodeFWChild.GetAttribute(_T("DEBUGDL"), strDebugDL))
			{
				continue;
			}

			if (strDebugDL.CompareNoCase(_T("0"))==0)
			{
				g_bSupportDebugDownload = FALSE;
			}
			else if (strDebugDL.CompareNoCase(_T("1"))==0)
			{
				g_bSupportDebugDownload = TRUE;
			}
		}
	}

	return TRUE;
}


/* TransferNT
 * ----------------------------------------------------------------------------
 */
BOOL CCommObj::TransferNT(BYTE *bypCmd, BYTE **ppData, UINT *upLen)
{
	BOOL bOK = FALSE;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		if (m_pLayer != NULL)
		{
			bOK = m_pLayer->Transfer(bypCmd, ppData, upLen);
		}

	} m_CritSec.Unlock();	// ------------------------------------------------

	return bOK;
}


/* TransferNT
 * ----------------------------------------------------------------------------
 */
BOOL CCommObj::TransferNT(BYTE *bypCmd, LPCTSTR cszFile, BYTE **ppData, UINT *upLen)
{
	BOOL bOK = FALSE;

	m_CritSec.Lock();		// ------------------------------------------------
	{
		if (m_pLayer != NULL) 
		{
			bOK = m_pLayer->Transfer(bypCmd, cszFile, ppData, upLen);
		}

	} m_CritSec.Unlock();	// ------------------------------------------------

	return bOK;
}


/* GetFirmware
 * ----------------------------------------------------------------------------
 */
DWORD CCommObj::GetFirmware(void)
{
	DWORD dwFirmware = 0;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		if (m_pLayer != NULL)
		{
			dwFirmware = m_pLayer->GetCommPara()->dwFirmware;
		}
	
	} m_CritSec.Unlock();	// ------------------------------------------------

	return dwFirmware;
}


/* SendMsgToEng
 * ----------------------------------------------------------------------------
 */
void CCommObj::SendMsgToEng(BSTR bMsg)
{
	CControlMgt::SendMsgToEng(bMsg);
	SysFreeString(bMsg);
}


/* SendMsgToEng
 * ----------------------------------------------------------------------------
 */
void CCommObj::SendMsgToEng(LPCTSTR szMsg)
{
	CString sMsg;
	sMsg.Format(OPC_MSG, szMsg);

	BSTR bMsg = sMsg.AllocSysString();
	CControlMgt::SendMsgToEng(bMsg);
	SysFreeString(bMsg);
}


/* SendNLToEng
 * ----------------------------------------------------------------------------
 */
void CCommObj::SendNLToEng(void)
{
	CString sMsg(_T("\n"));

	BSTR bMsg = sMsg.AllocSysString();
	CControlMgt::SendMsgToEng(bMsg);
	SysFreeString(bMsg);
}


/* SendErrToEng
 * ----------------------------------------------------------------------------
 */
void CCommObj::SendErrToEng(UINT uErrNo)
{
	CString sRes;

	if (!Adapt_SendErrToEng(uErrNo))
	{
		return;
	}

	switch(uErrNo)
	{
		case OPC_ERR_LOAD_ONLCOMM		:	sRes.Format(OPC_ERR_DEF, uErrNo, _T("Failed to load communication module"));			break;				
		case OPC_ERR_INIT_ONLCOMM		:	sRes.Format(OPC_ERR_DEF, uErrNo, _T("Failed to initialize communication module"));		break;				
		case OPC_ERR_CONS_ONLCOMM		:	sRes.Format(OPC_ERR_DEF, uErrNo, _T("Failed to create communication object"));			break;				
		case OPC_ERR_OPENCOMM			:	sRes.Format(OPC_ERR_DEF, uErrNo, _T("Unable to establish communication with control")); break;				
		case OPC_ERR_CONNECT			:	sRes.Format(OPC_ERR_DEF, uErrNo, _T("Unable to establish connection to control"));		break;				
					
		default 	:	sRes.Format(OPC_ERR_DEF, uErrNo, _T("Unkown error"));													break;					
	
	}

	if (! sRes.IsEmpty())
	{
		BSTR pMessage = sRes.AllocSysString();
		SendMsgToEng(pMessage);
		SysFreeString(pMessage);
	}
}


/* GetMsgString
 * ----------------------------------------------------------------------------
 */
BOOL CCommObj::GetMsgString(BYTE byCommand, BYTE *pData, UINT uLen, BSTR *pMessage)
{
	BOOL bOK = FALSE;

	m_CritSec.Lock();	// ----------------------------------------------------
	{
		if (m_pLayer != NULL)
		{
			bOK = m_pLayer->GetMsgString(byCommand, pData, uLen, pMessage);
		}

	} m_CritSec.Unlock();	// ------------------------------------------------

	return bOK;
}


/* Get4CLayer
 * ----------------------------------------------------------------------------
 * Danger, experts only!
 */
C4CLayer *CCommObj::Get4CLayer(void)
{
	// Sync & pray!
	m_CritSec.Lock();
	m_CritSec.Unlock();

	return m_pLayer;
}

/* ---------------------------------------------------------------------------- */
