// OPCAutoServer.cpp
//
// (c) Copyright 1998 The OPC Foundation
// ALL RIGHTS RESERVED.
//
// DISCLAIMER:
//  This sample code is provided by the OPC Foundation solely to assist
//  in understanding the OPC Specifications and may be used
//  as set forth in the License Grant section of the OPC Specification.
//  This code is provided as-is and without warranty or support of any sort
//  and is subject to the Warranty and Liability Disclaimers which appear
//  in the printed OPC Specification.
//
// CREDITS:
//  This code was generously provided to the OPC Foundation by
//  Jim Hansen, FactorySoft Inc.
//
// CONTENTS:
//  This file is part of the OPC Data Access Automation 2.0 wrapper dll.
//  This is the OPCServer object.
//
//
// Modification Log:
// Vers    Date   By    Notes
// ----  -------- ---   -----
// 1.00  03/7/98  JH    First Release
//
//

#include "stdafx.h"
#include "OPCBrowser.h"
#define __GUID_DEFINED__
#include "opcda_cats.c"
const CLSID CLSID_OPCServerList = {0x13486D51,0x4821,0x11D2,{0xA4,0x94,0x3C,0xB3,0x06,0xC1,0x00,0x00}};
#define NEXT_COUNT 100  // the number of elements per Next() call

/////////////////////////////////////////////////////////////////////
// Constructor
COPCAutoServer::COPCAutoServer()
{
   m_defaultActive = VARIANT_TRUE;
   m_defaultUpdate = 1000;
   m_defaultDeadband = 0.0F;
   m_defaultLocale = m_localeID = LOCALE_USER_DEFAULT;
   m_defaultTimeBias = 0;
   m_ShutdownConnection = 0;

	// Establish security.  Everyone can connect
   HRESULT hr = CoInitializeSecurity(NULL, -1, NULL, NULL,
      RPC_C_AUTHN_LEVEL_NONE, RPC_C_IMP_LEVEL_IDENTIFY, NULL, EOAC_NONE, NULL);

}

COPCAutoServer::~COPCAutoServer()
{
   Disconnect();

	// delete the existing servers from the list
	list<OPCServerInfo*>::iterator listiter = m_servers.begin();
   for( LONG index=1; listiter!=m_servers.end(); index++ )
   {
      OPCServerInfo* pServer = *listiter;
		ATLASSERT(pServer);
		delete pServer;
		pServer = NULL;
      listiter++;
   }

}

/////////////////////////////////////////////////////////////////////
// OPCServer::ShutdownRequest callback
STDMETHODIMP COPCAutoServer::ShutdownRequest(LPCWSTR szReason)
{
   CComBSTR Reason( szReason );
   Fire_ServerShutDown(Reason);
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::StartTime property
STDMETHODIMP COPCAutoServer::get_StartTime(DATE * pStartTime)
{
   if( pStartTime == NULL )
      return E_INVALIDARG;

   if( !m_pOPCServer )
      return E_FAIL;

   OPCSERVERSTATUS* ss;

   HRESULT hr = m_pOPCServer->GetStatus( &ss );
   if( FAILED(hr) )
   {
      return hr;
   }

   *pStartTime = NULL; 
   FILETIME filetimeLocal;
   if( FileTimeToLocalFileTime( &ss->ftStartTime, &filetimeLocal) )
   {
      SYSTEMTIME systime;
      if( FileTimeToSystemTime(&filetimeLocal, &systime) )
      {
	      SystemTimeToVariantTime( &systime, pStartTime);
	   }
   }

   CoTaskMemFree( ss->szVendorInfo );
   CoTaskMemFree( ss );
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::CurrentTime property
STDMETHODIMP COPCAutoServer::get_CurrentTime(DATE * pCurrentTime)
{
   if( pCurrentTime == NULL )
      return E_INVALIDARG;

   if( !m_pOPCServer )
      return E_FAIL;

   OPCSERVERSTATUS* ss;

   HRESULT hr = m_pOPCServer->GetStatus( &ss );
   if( FAILED(hr) )
   {
      return hr;
   }

   *pCurrentTime = NULL; 
   FILETIME filetimeLocal;
   if( FileTimeToLocalFileTime( &ss->ftCurrentTime, &filetimeLocal) )
   {
      SYSTEMTIME systime;
      if( FileTimeToSystemTime(&filetimeLocal, &systime) )
      {
	      SystemTimeToVariantTime( &systime, pCurrentTime);
	   }
   }

   CoTaskMemFree( ss->szVendorInfo );
   CoTaskMemFree( ss );
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::LastUpdateTime property
STDMETHODIMP COPCAutoServer::get_LastUpdateTime(DATE * pLastUpdateTime)
{
   if( pLastUpdateTime == NULL )
      return E_INVALIDARG;

   if( !m_pOPCServer )
      return E_FAIL;

   OPCSERVERSTATUS* ss;

   HRESULT hr = m_pOPCServer->GetStatus( &ss );
   if( FAILED(hr) )
   {
      return hr;
   }

   *pLastUpdateTime = NULL; 
   FILETIME filetimeLocal;
   if( FileTimeToLocalFileTime( &ss->ftLastUpdateTime, &filetimeLocal) )
   {
      SYSTEMTIME systime;
      if( FileTimeToSystemTime(&filetimeLocal, &systime) )
      {
	      SystemTimeToVariantTime( &systime, pLastUpdateTime);
	   }
   }

   CoTaskMemFree( ss->szVendorInfo );
   CoTaskMemFree( ss );
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::MajorVersion property
STDMETHODIMP COPCAutoServer::get_MajorVersion(short * pMajorVersion)
{
   if( pMajorVersion == NULL )
      return E_INVALIDARG;

   if( !m_pOPCServer )
      return E_FAIL;

   OPCSERVERSTATUS* ss;

   HRESULT hr = m_pOPCServer->GetStatus( &ss );
   if( FAILED(hr) )
   {
      return hr;
   }

   *pMajorVersion = ss->wMajorVersion;

   CoTaskMemFree( ss->szVendorInfo );
   CoTaskMemFree( ss );
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::MinorVersion property
STDMETHODIMP COPCAutoServer::get_MinorVersion(short * pMinorVersion)
{
   if( pMinorVersion == NULL )
      return E_INVALIDARG;

   if( !m_pOPCServer )
      return E_FAIL;

   OPCSERVERSTATUS* ss;

   HRESULT hr = m_pOPCServer->GetStatus( &ss );
   if( FAILED(hr) )
   {
      return hr;
   }

   *pMinorVersion = ss->wMinorVersion;

   CoTaskMemFree( ss->szVendorInfo );
   CoTaskMemFree( ss );
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::BuildNumber property
STDMETHODIMP COPCAutoServer::get_BuildNumber(short * pBuildNumber)
{
   if( pBuildNumber == NULL )
      return E_INVALIDARG;

   if( !m_pOPCServer )
      return E_FAIL;

   OPCSERVERSTATUS* ss;

   HRESULT hr = m_pOPCServer->GetStatus( &ss );
   if( FAILED(hr) )
   {
      return hr;
   }

   *pBuildNumber = ss->wBuildNumber;

   CoTaskMemFree( ss->szVendorInfo );
   CoTaskMemFree( ss );
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::VendorInfo property
STDMETHODIMP COPCAutoServer::get_VendorInfo(BSTR * pVendorInfo)
{
   if( pVendorInfo == NULL )
      return E_INVALIDARG;

   if( !m_pOPCServer )
      return E_FAIL;

   OPCSERVERSTATUS* ss;

   HRESULT hr = m_pOPCServer->GetStatus( &ss );
   if( FAILED(hr) )
   {
      return hr;
   }

   // need to duplicate ss->szVendorInfo into a BSTR!!
   CComBSTR local( ss->szVendorInfo );
   *pVendorInfo = local.Copy();

   CoTaskMemFree( ss->szVendorInfo );
   CoTaskMemFree( ss );
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::ServerState property
STDMETHODIMP COPCAutoServer::get_ServerState(LONG * pServerState)
{
   if( pServerState == NULL )
      return E_INVALIDARG;

   if( !m_pOPCServer )
   {
      *pServerState = OPCDisconnected;
      return S_OK;
   }

   OPCSERVERSTATUS* ss;

   HRESULT hr = m_pOPCServer->GetStatus( &ss );
   if( FAILED(hr) )
   {
      return hr;
   }

   *pServerState = (LONG)ss->dwServerState;

   CoTaskMemFree( ss->szVendorInfo );
   CoTaskMemFree( ss );
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::ServerName property
STDMETHODIMP COPCAutoServer::get_ServerName(BSTR * pServerName)
{
   if( pServerName == NULL )
      return E_INVALIDARG;

   *pServerName = m_name.Copy();

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::ServerNode property
STDMETHODIMP COPCAutoServer::get_ServerNode(BSTR * pServerNode)
{
   if( pServerNode == NULL )
      return E_INVALIDARG;

   *pServerNode = m_node.Copy();

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::ClientName property
STDMETHODIMP COPCAutoServer::get_ClientName(BSTR * pClientName)
{
   if( pClientName == NULL )
      return E_INVALIDARG;
   *pClientName = m_client.Copy();

   return S_OK;
}

STDMETHODIMP COPCAutoServer::put_ClientName(BSTR ClientName)
{
   m_client = ClientName;

   if( !m_pOPCCommon )
   {
      m_pOPCCommon = m_pOPCServer;   // get OPCCommon Interface
      if( !m_pOPCCommon )
         return S_OK;
   }

   HRESULT hr = m_pOPCCommon->SetClientName(m_client);

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::LocaleID property
STDMETHODIMP COPCAutoServer::get_LocaleID(LONG * pLocaleID)
{
   if( pLocaleID == NULL )
      return E_INVALIDARG;

   *pLocaleID = m_localeID;

   if( !m_pOPCCommon )
   {
      m_pOPCCommon = m_pOPCServer;   // get OPCCommon Interface
      if( !m_pOPCCommon )
         return S_OK;
   }
   HRESULT hr = m_pOPCCommon->GetLocaleID((LCID*)&m_localeID);
   if( SUCCEEDED(hr) )
   {
      *pLocaleID = m_localeID;
   }

   return hr;
}

STDMETHODIMP COPCAutoServer::put_LocaleID(LONG LocaleID)
{
   m_localeID = LocaleID;

   if( !m_pOPCCommon )
   {
      m_pOPCCommon = m_pOPCServer;   // get OPCCommon Interface
      if( !m_pOPCCommon )
         return S_OK;
   }

   HRESULT hr = m_pOPCCommon->SetLocaleID(m_localeID);

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::Bandwidth property
STDMETHODIMP COPCAutoServer::get_Bandwidth(LONG *pBandwidth)
{
   if( pBandwidth == NULL )
      return E_INVALIDARG;

   if( !m_pOPCServer )
      return E_FAIL;

   OPCSERVERSTATUS* ss;

   HRESULT hr = m_pOPCServer->GetStatus( &ss );
   if( FAILED(hr) )
   {
      return hr;
   }

   *pBandwidth = (LONG)ss->dwBandWidth;

   CoTaskMemFree( ss->szVendorInfo );
   CoTaskMemFree( ss );
   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::OPCGroups property
STDMETHODIMP COPCAutoServer::get_OPCGroups(OPCGroups ** ppGroups)
{
   if( ppGroups == NULL )
      return E_INVALIDARG;
   *ppGroups = NULL;

   // get Interface pointer
   HRESULT hr = ((COPCAutoServerObject*)this)->QueryInterface(IID_IOPCGroups, (void**)ppGroups);
   _ASSERTE(*ppGroups);

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::PublicGroupNames property
STDMETHODIMP COPCAutoServer::get_PublicGroupNames(VARIANT * pPublicGroups)
{
   if( pPublicGroups == NULL )
      return E_INVALIDARG;

   return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::GetOPCServers method
// Find all of the registered OPC servers and return their ProgIDs in safearray
const int MAX_KEYLEN = 256;
STDMETHODIMP COPCAutoServer::GetOPCServers( VARIANT Node,
                                            VARIANT * pOPCServers)
{
USES_CONVERSION;

	// delete the existing servers from the list
	list<OPCServerInfo*>::iterator listiter = m_servers.begin();
   for( LONG index=1; listiter!=m_servers.end(); index++ )
   {
      OPCServerInfo* pServer = *listiter;
		ATLASSERT(pServer);
		delete pServer;
		pServer = NULL;
      listiter++;
   }

	// clear the list
	m_servers.clear();

   list<CComBSTR*> serverNames; // list of server names
	BOOL bFailed = FALSE;

   if( pOPCServers == NULL )
      return E_INVALIDARG;

   // New 2.0 method using component categories
   IOPCServerList* pServers=NULL;

   COSERVERINFO si;
   MULTI_QI  qi;

   si.dwReserved1 = 0;
   si.pwszName = NULL;
   if( (Node.vt == VT_BSTR) && (Node.scode != DISP_E_PARAMNOTFOUND) && (Node.bstrVal != NULL) && (Node.bstrVal[0] != 0) )
      si.pwszName = Node.bstrVal;
   si.pAuthInfo = NULL;
   si.dwReserved2 = 0;

   qi.pIID = &IID_IOPCServerList;
   qi.pItf = NULL;
   qi.hr = 0;

   HRESULT hr = CoCreateInstanceEx(CLSID_OPCServerList, NULL, CLSCTX_ALL, &si, 1, &qi);
   if (FAILED(hr) || FAILED(qi.hr))
   {
		bFailed = TRUE;
   }
   else
   {
       pServers = (IOPCServerList*)qi.pItf;

       IEnumGUID *pEnumGUID=NULL;
       CLSID catid = CATID_OPCDAServer10;
       hr = pServers->EnumClassesOfCategories(1, &catid, 1, &catid, &pEnumGUID);
       if( SUCCEEDED(hr) )
       {
          unsigned long count;
          CLSID clsid[NEXT_COUNT];

          do
          {
             hr = pEnumGUID->Next(NEXT_COUNT, clsid, &count);
             for( ULONG index=0; index<count; index++ )
             {
                LPOLESTR pszProgID;
                LPOLESTR pszUserType;
                HRESULT hr2 = pServers->GetClassDetails(clsid[index], &pszProgID, &pszUserType);
                if (SUCCEEDED(hr2))
                {
							OPCServerInfo* pServerInfo = new OPCServerInfo(pszProgID, pszUserType, clsid[index]);
							m_servers.push_back( pServerInfo );
							CComBSTR* pName = new CComBSTR( pszProgID );
							serverNames.push_back( pName);         // save in the list

							CoTaskMemFree(pszProgID);
							CoTaskMemFree(pszUserType);
                }
             }
          }
          while ( hr==S_OK );
          pEnumGUID->Release();
          pServers->Release();
       }
       else // hr failed
       {
				bFailed = TRUE;
				pServers->Release();
       }
   }

	if( bFailed )		 // If that failed use the old way
	{
		// search the registry for OPC entries
		HKEY hk = HKEY_CLASSES_ROOT;
		if( (Node.vt == VT_BSTR) && (Node.scode != DISP_E_PARAMNOTFOUND) && (Node.bstrVal != NULL) && (Node.bstrVal[0] != 0) )
		{
			DWORD dwR = RegConnectRegistry (OLE2T(Node.bstrVal), HKEY_CLASSES_ROOT, &hk);
			if( dwR != ERROR_SUCCESS )
				return E_FAIL;
		}

		TCHAR key[MAX_KEYLEN];
		DWORD size = MAX_KEYLEN;
		FILETIME ft;
		for( LONG index=0; RegEnumKeyEx(hk, index, key, &size, 0, NULL, NULL, &ft)==ERROR_SUCCESS; index++)
		{
			HKEY hProgID=0;
			if(RegOpenKeyEx(hk, key, 0, KEY_QUERY_VALUE, &hProgID )==ERROR_SUCCESS )
			{
				// Find OPC key
				HKEY hOPC=0;
				if(RegOpenKeyEx(hProgID, _T("OPC"), 0, KEY_QUERY_VALUE, &hOPC)==ERROR_SUCCESS )
				{
					// Found OPC key, save this information
					CLSID clsid;
					DWORD type=REG_SZ;
					HKEY hCLSID=0;
					if(RegOpenKeyEx(hProgID, _T("CLSID"), 0, KEY_QUERY_VALUE, &hCLSID)==ERROR_SUCCESS )
					{
						 TCHAR clsidString[MAX_KEYLEN];
						 size=MAX_KEYLEN;
						 if(RegQueryValueEx(hCLSID, NULL/*key*/, 0, &type, (BYTE*)clsidString, &size )==ERROR_SUCCESS )
						 {
							  HRESULT hr = CLSIDFromString( T2OLE(clsidString), &clsid );
							  if( FAILED(hr))
							  {
									// failed
							  }
							  else  // everything worked
							  {
									size=MAX_KEYLEN;
									OPCServerInfo* pServerInfo = new OPCServerInfo(T2OLE(key), L"", clsid );
									m_servers.push_back( pServerInfo );
									CComBSTR* pName = new CComBSTR( key ); // converts key to a BSTR
									serverNames.push_back( pName);         // save in the list
							  }
						 }
						 RegCloseKey( hCLSID );
					}
					RegCloseKey( hOPC );
				}
				RegCloseKey( hProgID );
			}
			size = MAX_KEYLEN;
		}
		RegCloseKey( hk );
	}

   SAFEARRAYBOUND bound;
   bound.lLbound = 1;
   bound.cElements = serverNames.size();

   SAFEARRAY *pArray = SafeArrayCreate(VT_BSTR, 1, &bound);
   if(pArray == NULL)
      return E_OUTOFMEMORY;

   list<CComBSTR*>::iterator listiter2 = serverNames.begin();
   for( index=1; index<=bound.cElements && listiter2!=serverNames.end(); index++ )
   {
      CComBSTR* pName = *listiter2;
      SafeArrayPutElement(pArray, &index, (void *)*pName);
      delete pName;
      listiter2++;
   }
   pOPCServers->vt = VT_BSTR | VT_ARRAY;
   pOPCServers->parray = pArray;

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::Connect method
STDMETHODIMP COPCAutoServer::Connect( BSTR ProgID,
                        VARIANT Node)
{
USES_CONVERSION;

	OPCServerInfo* pServer = NULL;
	BOOL bFound = FALSE;
	CLSID clsid;
   BOOL useNode = FALSE;

   Disconnect();

   if( (Node.vt == VT_BSTR) && (Node.scode != DISP_E_PARAMNOTFOUND) && (Node.bstrVal != NULL) && (Node.bstrVal[0] != 0) )
      useNode = TRUE;

   // Check, if the specified node is the local node
   // In this case continue as if no node has been specified.
   TCHAR	szLocalHost[MAX_COMPUTERNAME_LENGTH + 1]; // my computer name	
   DWORD	dwHostSize = sizeof(szLocalHost);

   if (useNode)
   {
      // get own computer name
      if (GetComputerName(szLocalHost, &dwHostSize) == TRUE)
         if (wcsicmp(T2W(szLocalHost), Node.bstrVal) == 0
          || wcsicmp(L"localhost", Node.bstrVal) == 0
          || wcsicmp(L"127.0.0.1", Node.bstrVal) == 0)
            useNode = FALSE;
   }

	// search through the server list and find the clsid
   list<OPCServerInfo*>::iterator listiter = m_servers.begin();
   for( long index=1; listiter!=m_servers.end(); index++ )
   {
      OPCServerInfo* pServerTemp = *listiter;
		if(pServerTemp->m_ProgID == ProgID)
		{
			clsid = pServerTemp->m_clsid;
			bFound = TRUE;
			break;
		}
      listiter++;
   }

	if( !bFound ) // server was not found, search for it using the ProgID
	{
		// If not found, Get CLSID from the ProgID using the 2.0 method
		IOPCServerList* pServers=NULL;
		BOOL bFailed = FALSE;

		COSERVERINFO si;
		MULTI_QI  qi;

		si.dwReserved1 = 0;
		si.pwszName = NULL;
		if( (Node.vt == VT_BSTR) && (Node.scode != DISP_E_PARAMNOTFOUND) && (Node.bstrVal != NULL) && (Node.bstrVal[0] != 0) )
			si.pwszName = Node.bstrVal;
		si.pAuthInfo = NULL;
		si.dwReserved2 = 0;

		qi.pIID = &IID_IOPCServerList;
		qi.pItf = NULL;
		qi.hr = 0;

		HRESULT hr = CoCreateInstanceEx(CLSID_OPCServerList, NULL, CLSCTX_ALL, &si, 1, &qi);
		if (FAILED(hr) || FAILED(qi.hr))
		{
			bFailed = TRUE;
		}
		else
		{
			pServers = (IOPCServerList*)qi.pItf;
			hr = pServers->CLSIDFromProgID(ProgID, &clsid);
			pServers->Release();
			if (FAILED(hr))
			{
				bFailed = TRUE;
			}
		}

		if(bFailed) // Search the old way...
		{			
			// Get Class ID for server
			// search the registry for OPC entries
			HKEY hk = HKEY_CLASSES_ROOT;
			HKEY hCLSID = 0;
			DWORD type = REG_SZ;
			if( useNode )
			{
				// search the registry for OPC entries
				if( (Node.vt == VT_BSTR) && (Node.scode != DISP_E_PARAMNOTFOUND) && (Node.bstrVal != NULL) && (Node.bstrVal[0] != 0) )
				{
					DWORD dwR = RegConnectRegistry (OLE2T(Node.bstrVal), HKEY_CLASSES_ROOT, &hk);
					if( dwR != ERROR_SUCCESS )
						return E_FAIL;
				}
			}

			if(!(RegOpenKeyEx(hk, OLE2T(ProgID), 0, KEY_QUERY_VALUE, &hCLSID ) == ERROR_SUCCESS))
				return E_FAIL;

			DWORD size=MAX_KEYLEN;
			TCHAR clsidString[MAX_KEYLEN];
			if(!(RegQueryValueEx(hCLSID, NULL, 0, &type, (BYTE*)clsidString, &size ) == ERROR_SUCCESS))
			{
				CLSID clsid;
				hr = CLSIDFromString( T2OLE(clsidString), &clsid );
				if( FAILED(hr))
				{
					RegCloseKey( hCLSID );
					RegCloseKey( hk );
					return hr;
				}
			}
			else
			{
				RegCloseKey( hCLSID );
				RegCloseKey( hk );
				return E_FAIL;
			}
			RegCloseKey( hCLSID );
			RegCloseKey( hk );
		}
	}

   if( !useNode )  // no node name, use local
   {
      // Create a running object from that class ID
      // (CLSCTX_ALL will allow in-proc, local and remote)
      LPUNKNOWN pUnkn = NULL;
      HRESULT hr = CoCreateInstance(clsid, NULL, CLSCTX_ALL, IID_IUnknown, (LPVOID *)&pUnkn);
      if( FAILED(hr) || pUnkn == NULL)
      {
         return hr;
      }

      // Get the IOPCServer interface.
      m_pOPCServer = pUnkn;
      pUnkn->Release();  // Don't need this anymore.
      pUnkn = NULL;
   }
   else
   {
      COSERVERINFO si;
      MULTI_QI  qi;

      si.dwReserved1 = 0;
      si.pwszName = Node.bstrVal;   // Node name
      si.pAuthInfo = NULL;
      si.dwReserved2 = 0;

      qi.pIID = &IID_IOPCServer;
      qi.pItf = NULL;
      qi.hr = 0;

      HRESULT hr = CoCreateInstanceEx(clsid, NULL, CLSCTX_REMOTE_SERVER, &si, 1, &qi);
      if (FAILED(hr))
      {
         ATLTRACE(_T("CoCreateInstanceEx failed"));
         return hr;
      }
      if (FAILED(qi.hr))
      {
         ATLTRACE(_T("Failed to connect to server"));
         return qi.hr;
      }
      m_pOPCServer = qi.pItf;
      qi.pItf->Release();
   }

   m_name = ProgID;
   if( (Node.vt == VT_BSTR) && (Node.scode != DISP_E_PARAMNOTFOUND) && (Node.bstrVal != NULL) && (Node.bstrVal[0] != 0))
   {
      m_node = Node.bstrVal;
   }

   IConnectionPointContainer *pCPC = 0;
   HRESULT hr = m_pOPCServer->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pCPC);
   if( SUCCEEDED(hr) )
   {
      IConnectionPoint *pCallbackCP = 0;
      hr = pCPC->FindConnectionPoint(IID_IOPCShutdown, &pCallbackCP);
      pCPC->Release();
      if( FAILED(hr) )
      {
         return hr;
      }

      IUnknown* pUnk = NULL;
      ((COPCAutoServerObject*)this)->QueryInterface( IID_IUnknown, (LPVOID*)&pUnk);
      hr = pCallbackCP->Advise(pUnk, &m_ShutdownConnection);
      pCallbackCP->Release();
      if( FAILED(hr) )
      {
         return hr;
      }
   }

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::Disconnect method
STDMETHODIMP COPCAutoServer::Disconnect(void)
{
   if( !m_pOPCServer )
      return S_OK;

   // OPC 2.0 ConnectionPoints
   IConnectionPointContainer *pCPC = 0;
   HRESULT hr = m_pOPCServer->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&pCPC);
   if( SUCCEEDED(hr) )
   {
      IConnectionPoint *pCallbackCP = 0;
      hr = pCPC->FindConnectionPoint(IID_IOPCShutdown, &pCallbackCP);
      if( SUCCEEDED(hr) )
      {
         hr = pCallbackCP->Unadvise(m_ShutdownConnection);
         pCallbackCP->Release();
      }
      pCPC->Release();
   }

   hr = RemoveAll();
   m_pOPCServer.Release();
   m_pOPCItemParams.Release();
   m_pOPCCommon.Release();

   m_name.Empty();
   m_node.Empty();
   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::CreateBrowser method
STDMETHODIMP COPCAutoServer::CreateBrowser(OPCBrowser ** ppBrowser)
{
   if( ppBrowser == NULL )
      return E_INVALIDARG;
   *ppBrowser = NULL;

   if( !m_pOPCServer )
      return E_FAIL;

   COPCBrowser* pBrowser = new COPCBrowser;
   pBrowser->m_pOPCBrowser = m_pOPCServer;   // QI for browse interface pointer
   if( !pBrowser->m_pOPCBrowser )   // Did QI return a pointer?
   {
      delete pBrowser;
      return E_FAIL;
   }
   // get Interface pointer
   pBrowser->QueryInterface(IID_OPCBrowser, (void**)ppBrowser);
   _ASSERTE(*ppBrowser);

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::GetErrorString method
STDMETHODIMP COPCAutoServer::GetErrorString(LONG ErrorCode,
                              BSTR *ErrorString)
{

   if( ErrorString == NULL )
      return E_INVALIDARG;
   *ErrorString = NULL;

   TCHAR msg[MAX_PATH*5];
   DWORD size = FormatMessage( FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
                              NULL,
                              ErrorCode,
                              m_localeID,
                              msg, MAX_PATH, NULL );
   if( size > 0 )
   {
      CComBSTR temp( msg );
      *ErrorString = temp.Copy();
      return S_OK;
   }

   if( !m_pOPCServer )
      return E_FAIL;

    LPWSTR retString = NULL;
    HRESULT hr = m_pOPCServer->GetErrorString(ErrorCode, m_localeID, &retString);

    if (SUCCEEDED(hr))
    {
        *ErrorString = SysAllocString( retString );

        CoTaskMemFree((void*)retString);    // free out parameter
        return hr;
    }

    return hr;
}

STDMETHODIMP COPCAutoServer::InterfaceSupportsErrorInfo(REFIID riid)
{
   static const IID* arr[] =
   {
      &IID_IOPCAutoServer,
   };

   for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
   {
      if (InlineIsEqualGUID(*arr[i],riid))
         return S_OK;
   }
   return S_FALSE;
}


/////////////////////////////////////////////////////////////////////
// OPCServer::QueryAvailableLocaleIDs method
STDMETHODIMP COPCAutoServer::QueryAvailableLocaleIDs(VARIANT * LocaleIDs)
{
   // Validate input types
   if(LocaleIDs == NULL)
      return E_INVALIDARG;

   if( !m_pOPCCommon )
   {
      m_pOPCCommon = m_pOPCServer;   // get OPCCommon Interface
      if( !m_pOPCCommon )
         return E_FAIL;
   }

   DWORD count = 0;
   LCID* locales = NULL;
   HRESULT hr = m_pOPCCommon->QueryAvailableLocaleIDs(&count, &locales);
   if( SUCCEEDED(hr) )
   {
      SAFEARRAYBOUND bound;
      bound.lLbound = 1;
      bound.cElements = count;
      SAFEARRAY *pArray = SafeArrayCreate(VT_I4, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;    // temp
      for( long index=0; index<count; index++ )
      {
         LONG vbIndex = index+1;
         SafeArrayPutElement(pArray, &vbIndex, (void *)&locales[index]);
      }
      LocaleIDs->vt = VT_ARRAY | VT_I4;
      LocaleIDs->parray = pArray;
      if( locales ) CoTaskMemFree(locales);
   }

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::QueryAvailableProperties method
STDMETHODIMP COPCAutoServer::QueryAvailableProperties(
                        BSTR ItemID,
                        LONG * pCount,
                        SAFEARRAY ** ppPropertyIDs,
                        SAFEARRAY ** ppDescriptions,
                        SAFEARRAY ** ppDataTypes)
{
   // Validate input types
   if(ItemID == NULL)
      return E_INVALIDARG;
   if(pCount == NULL)
      return E_INVALIDARG;
   if(ppPropertyIDs == NULL)
      return E_INVALIDARG;
   if(ppDescriptions == NULL)
      return E_INVALIDARG;
   if(ppDataTypes == NULL)
      return E_INVALIDARG;

   if( !m_pOPCItemParams )
   {
      m_pOPCItemParams = m_pOPCServer;   // get Item Parameters Interface
      if( !m_pOPCItemParams )
         return E_FAIL;
   }

   DWORD* pParameterIDs = NULL;
   LPWSTR* pDescriptions = NULL;
   VARTYPE* pDataTypes = NULL;
   HRESULT hr = m_pOPCItemParams->QueryAvailableProperties(ItemID,
                                     (DWORD*)pCount,
                                     &pParameterIDs,
                                     &pDescriptions,
                                     &pDataTypes);

   if( FAILED(hr) )
      return hr;

   // Return ParameterIDs in a SAFEARRAY
   {
      SAFEARRAYBOUND bound;
      bound.lLbound = 1;
      bound.cElements = *pCount;
      SAFEARRAY *pArray = SafeArrayCreate(VT_I4, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;    // temp
      for( long index=0; index<*pCount; index++ )
      {
         LONG vbIndex = index+1;
         SafeArrayPutElement(pArray, &vbIndex, (void *)&pParameterIDs[index]);
      }
      *ppPropertyIDs = pArray;
   }

   // Return Descriptions in a SAFEARRAY
   {
      SAFEARRAYBOUND bound;
      bound.lLbound = 1;
      bound.cElements = *pCount;
      SAFEARRAY *pArray = SafeArrayCreate(VT_BSTR, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;    // temp
      for( long index=0; index<*pCount; index++ )
      {
         LONG vbIndex = index+1;
         CComBSTR description( pDescriptions[index] );
         SafeArrayPutElement(pArray, &vbIndex, (void *)(BSTR)description );
         if(pDescriptions[index]) CoTaskMemFree(pDescriptions[index]);
      }
      *ppDescriptions = pArray;
   }

   // Return DataTypes in a SAFEARRAY
   {
      SAFEARRAYBOUND bound;
      bound.lLbound = 1;
      bound.cElements = *pCount;
      SAFEARRAY *pArray = SafeArrayCreate(VT_I2, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;    // temp
      for( long index=0; index<*pCount; index++ )
      {
         LONG vbIndex = index+1;
         SafeArrayPutElement(pArray, &vbIndex, (void *)&pDataTypes[index]);
      }
      *ppDataTypes = pArray;
   }

   // Free up the parameters
   if( pParameterIDs ) CoTaskMemFree(pParameterIDs);
   if( pDescriptions ) CoTaskMemFree(pDescriptions);
   if( pDataTypes ) CoTaskMemFree(pDataTypes);

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::GetItemProperties method
STDMETHODIMP COPCAutoServer::GetItemProperties(BSTR ItemID,
                        LONG Count,
                        SAFEARRAY ** ppPropertyIDs,
                        SAFEARRAY ** ppPropertyValues,
                        SAFEARRAY ** ppErrors)
{
   // Validate input types
   if(ItemID == NULL)
      return E_INVALIDARG;
   if(*ppPropertyIDs == NULL)
      return E_INVALIDARG;
   if(ppPropertyValues == NULL)
      return E_INVALIDARG;
   if(ppErrors == NULL)
      return E_INVALIDARG;

   if( !m_pOPCItemParams )
   {
      m_pOPCItemParams = m_pOPCServer;   // get Item Parameters Interface
      if( !m_pOPCItemParams )
         return E_FAIL;
   }

   HRESULT hr = S_OK;
   LONG lBound=0;
   LONG uBound=0;
   hr = SafeArrayGetLBound(*ppPropertyIDs, 1, &lBound);
   if( FAILED(hr) )
      return hr;
   hr = SafeArrayGetUBound(*ppPropertyIDs, 1, &uBound);
   if( FAILED(hr) )
      return hr;
   if( lBound > 1 )
      return E_INVALIDARG;
   if( uBound < Count )
      return E_INVALIDARG;

   // Create array of handles
   DWORD* pParameterIDs = (DWORD*)_alloca(Count*sizeof(DWORD));
   memset( pParameterIDs, 0, Count*sizeof(DWORD) );
   for( long index=0; index<Count; index++ )
      {
      LONG vbIndex = index+1;
      DWORD ID=0;
      hr = SafeArrayGetElement(*ppPropertyIDs, &vbIndex, &ID);
      if( FAILED(hr) )
         return hr;
      pParameterIDs[index] = ID;
      }

   VARIANT* pValues = NULL;
   HRESULT* pErrors = NULL;
   hr = m_pOPCItemParams->GetItemProperties(ItemID,
                                  (DWORD)Count,
                                  pParameterIDs,
                                  &pValues,
                                  &pErrors);
   if( FAILED(hr) )
      return hr;

   // Return Values in a SAFEARRAY and update each COPCItem
   {
      SAFEARRAYBOUND bound;
      bound.lLbound = 1;
      bound.cElements = Count;
      SAFEARRAY *pArray = SafeArrayCreate(VT_VARIANT, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;    // temp
      for( long index=0; index<Count; index++ )
      {
         LONG vbIndex = index+1;
         SafeArrayPutElement(pArray, &vbIndex, (void *)&pValues[index]);
         VariantClear(&pValues[index]);
      }
      *ppPropertyValues = pArray;
   }

   // Return errors in a SAFEARRAY
   {
      SAFEARRAYBOUND bound;
      bound.lLbound = 1;
      bound.cElements = Count;
      SAFEARRAY *pArray = SafeArrayCreate(VT_I4, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;    // temp
      for( long index=0; index<Count; index++ )
      {
         LONG vbIndex = index+1;
         SafeArrayPutElement(pArray, &vbIndex, (void *)&pErrors[index]);
      }
      *ppErrors = pArray;
   }

   // Free up the parameters
   if( pValues ) CoTaskMemFree(pValues);
   if( pErrors ) CoTaskMemFree(pErrors);

   return hr;
}

/////////////////////////////////////////////////////////////////////
// OPCServer::LookupItemIDs method
STDMETHODIMP COPCAutoServer::LookupItemIDs(BSTR ItemID,
                        LONG Count,
                        SAFEARRAY ** ppPropertyIDs,
                        SAFEARRAY ** ppNewItemIDs,
                        SAFEARRAY ** ppErrors)
{
   // Validate input types
   if(ItemID == NULL)
      return E_INVALIDARG;
   if(*ppPropertyIDs == NULL)
      return E_INVALIDARG;
   if(ppNewItemIDs == NULL)
      return E_INVALIDARG;
   if(ppErrors == NULL)
      return E_INVALIDARG;

   if( !m_pOPCItemParams )
   {
      m_pOPCItemParams = m_pOPCServer;   // get Item Parameters Interface
      if( !m_pOPCItemParams )
         return E_FAIL;
   }

   HRESULT hr = S_OK;
   LONG lBound=0;
   LONG uBound=0;
   hr = SafeArrayGetLBound(*ppPropertyIDs, 1, &lBound);
   if( FAILED(hr) )
      return hr;
   hr = SafeArrayGetUBound(*ppPropertyIDs, 1, &uBound);
   if( FAILED(hr) )
      return hr;
   if( lBound > 1 )
      return E_INVALIDARG;
   if( uBound < Count )
      return E_INVALIDARG;

   // Create array of handles
   DWORD* pParameterIDs = (DWORD*)_alloca(Count*sizeof(DWORD));
   memset( pParameterIDs, 0, Count*sizeof(DWORD) );
   for( long index=0; index<Count; index++ )
      {
      LONG vbIndex = index+1;
      DWORD ID=0;
      hr = SafeArrayGetElement(*ppPropertyIDs, &vbIndex, &ID);
      if( FAILED(hr) )
         return hr;
      pParameterIDs[index] = ID;
      }

   LPWSTR* pNewItemIDs = NULL;
   HRESULT* pErrors = NULL;
   hr = m_pOPCItemParams->LookupItemIDs(ItemID,
                                  (DWORD)Count,
                                  pParameterIDs,
                                  &pNewItemIDs,
                                  &pErrors);
   if( FAILED(hr) )
      return hr;

   // Return ItemIDs in a SAFEARRAY
   {
      SAFEARRAYBOUND bound;
      bound.lLbound = 1;
      bound.cElements = Count;
      SAFEARRAY *pArray = SafeArrayCreate(VT_BSTR, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;    // temp
      for( long index=0; index<Count; index++ )
      {
         LONG vbIndex = index+1;
         CComBSTR newItemID( pNewItemIDs[index] );
         SafeArrayPutElement(pArray, &vbIndex, (void *)(BSTR)newItemID );
         if(pNewItemIDs[index]) CoTaskMemFree(pNewItemIDs[index]);
      }
      *ppNewItemIDs = pArray;
   }

   // Return errors in a SAFEARRAY
   {
      SAFEARRAYBOUND bound;
      bound.lLbound = 1;
      bound.cElements = Count;
      SAFEARRAY *pArray = SafeArrayCreate(VT_I4, 1, &bound);
      if(pArray == NULL)
         return E_FAIL;    // temp
      for( long index=0; index<Count; index++ )
      {
         LONG vbIndex = index+1;
         SafeArrayPutElement(pArray, &vbIndex, (void *)&pErrors[index]);
      }
      *ppErrors = pArray;
   }

   // Free up the parameters
   if( pNewItemIDs ) CoTaskMemFree(pNewItemIDs);
   if( pErrors ) CoTaskMemFree(pErrors);

   return hr;
}

/////////////////////////////////////////////////////////////////////
// VariantToAutomation converts variant data to Automation compatible type.
HRESULT VariantToAutomation ( VARIANT* pvar )
{
   switch (pvar->vt)
   {
      case VT_I1 :
         return VariantChangeType (pvar, pvar, 0, VT_I2); break;
      case VT_UI2 :
         return VariantChangeType (pvar, pvar, 0, VT_I4); break;
      case VT_UI4 :
         return VariantChangeType (pvar, pvar, 0, VT_R8); break;
      case (VT_ARRAY | VT_I1)  :
         return ArrayToAutomation (pvar, VT_I2); break;
      case (VT_ARRAY | VT_UI2) :
         return ArrayToAutomation (pvar, VT_I4); break;
      case (VT_ARRAY | VT_UI4) :
         return ArrayToAutomation (pvar, VT_R8); break;
   }

   return S_OK;
}

/////////////////////////////////////////////////////////////////////
// ArrayToAutomation converts array data to Automation compatible type.
HRESULT ArrayToAutomation ( VARIANT* pvar, VARTYPE vtNew )
{
   // Change the type of the array elements
   long     nbEntries = pvar->parray->rgsabound[0].cElements;
   VARIANT  tmpVal;
   HRESULT  hr = S_OK;

   // determine #entries nbEntries = pvar->parray->rgsabound[0].cElements;
   // initialise the safearray
   SAFEARRAYBOUND sab[1]; sab[0].lLbound = 0; sab[0].cElements = nbEntries;
   SAFEARRAY *pArray = SafeArrayCreate (vtNew, 1, sab);
   if (pArray == NULL)
      return E_OUTOFMEMORY;

   // Fill the safearray
   tmpVal.vt = pvar->vt - VT_ARRAY;
   VARTYPE vtOld = tmpVal.vt;

   for (long Counter=0; Counter<nbEntries; Counter++)
   {
	  tmpVal.ulVal = 0;
      // extract next array element
      switch (vtOld)
      {
         case VT_I1 :
            hr = SafeArrayGetElement(pvar->parray, &Counter, &tmpVal.cVal);
			   if (FAILED(hr))
				   goto cleanupAndExit;
			   tmpVal.iVal = tmpVal.cVal;
			   hr = SafeArrayPutElement(pArray, &Counter,&tmpVal.iVal);
            break; 
         case VT_UI2 :
            hr = SafeArrayGetElement(pvar->parray, &Counter, &tmpVal.uiVal);
			   if (FAILED(hr))
				   goto cleanupAndExit;
			   tmpVal.lVal = tmpVal.uiVal;
	           hr = SafeArrayPutElement(pArray, &Counter, &tmpVal.lVal);
            break; 
         case VT_UI4 :
            hr = SafeArrayGetElement(pvar->parray, &Counter, &tmpVal.ulVal);
			   if (FAILED(hr))
				   goto cleanupAndExit;
			   tmpVal.dblVal = tmpVal.ulVal;
			   hr = SafeArrayPutElement(pArray, &Counter,&tmpVal.dblVal);
            break;
      }
      if (FAILED(hr))
         goto cleanupAndExit;
   }

   // set the VARIANT with the new array
   //
   VariantClear(pvar);
   pvar->vt = (VT_ARRAY | vtNew);
   pvar->parray = pArray;

   return S_OK;

cleanupAndExit:
   SafeArrayDestroy (pArray);
   return hr;
}