// AddOnOps_Test.cpp: implementation of the AddOnOps_Test class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dialog.h"
#include "AddOnOps_Test.h"

#include "..\CEAddonHandler\CEAddonHandler_i.c"
#include "CEAddonAction_i.c"
#include "..\ProjInfoDummy\ProjInfoDummy\ProjInfoDummy_i.c"
#include "CEProjInfo_i.c"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AddOnOps_Test::AddOnOps_Test()
{
	HRESULT hRes = 0;

	m_pIAddOnAction = NULL;
    m_pIUnkProjInfo = NULL;

	hRes = CoInitialize(NULL);
	if(FAILED(hRes))
	{
		DispMessage(hRes);
	}

}

AddOnOps_Test::~AddOnOps_Test()
{
	if(m_pIAddOnAction != NULL)
	{
		disconnect();
	}
	if(m_pIUnkProjInfo != NULL)
	{
		disconnectProjInfo();
	}
	CoUninitialize();
}

BOOL AddOnOps_Test::connect()
{
	HRESULT hRes = 0;

	if(m_pIAddOnAction != NULL)
	{
		AfxMessageBox(_T("Already connected ..."));
		return FALSE;
	}

	// create the COM object
	hRes = CoCreateInstance(CLSID_CEAddonActionComObj,
							NULL,
							CLSCTX_INPROC_SERVER,
							//IID_ICEAddonAction,
                            IID_IUnknown,
							(LPVOID *) &m_pIAddOnAction);
	if(FAILED(hRes))
	{
		DispMessage(hRes);
		return FALSE;
	}
    connectProjInfo();

	return TRUE;
}

BOOL AddOnOps_Test::disconnect()
{
	if(m_pIAddOnAction != NULL)
	{
		m_pIAddOnAction->Release();
		m_pIAddOnAction = NULL;
		return TRUE;
        disconnectProjInfo();
	}
	return FALSE;
}

BOOL AddOnOps_Test::connectProjInfo()
{
	HRESULT hRes = 0;

	if(m_pIUnkProjInfo != NULL)
	{
		//AfxMessageBox(_T("Already connected ..."));
		return FALSE;
	}

	// create the COM object
	hRes = CoCreateInstance(CLSID_ProjInfoDummyComObj,
							NULL,
							CLSCTX_INPROC_SERVER,
							//IID_ICEProjInfo,
							IID_IUnknown,
							(LPVOID *) &m_pIUnkProjInfo);
	if(FAILED(hRes))
	{
		DispMessage(hRes);
		return FALSE;
	}
	return TRUE;
}

BOOL AddOnOps_Test::disconnectProjInfo()
{
	if(m_pIUnkProjInfo != NULL)
	{
		m_pIUnkProjInfo->Release();
		m_pIUnkProjInfo = NULL;
		return TRUE;
	}
	return FALSE;
}

void AddOnOps_Test::invoke(LPCTSTR pszDispId,
                           LPCTSTR pszSourceFile,
                           LPCTSTR pszId,
                           LPCTSTR pszAdditionalInfo)
{
	HRESULT hRes = 0;

	bstr_t * psMethodName = new bstr_t(pszDispId);
	bstr_t * psSourceFile = new bstr_t(pszSourceFile);
	bstr_t * psId = new bstr_t(pszId);
	bstr_t * psAdditionalInfo = new bstr_t(pszAdditionalInfo);
	ICEProjInfo * pICEProjectInfo = NULL;

	if(m_pIAddOnAction == NULL)
	{
		AfxMessageBox(_T("Not connected ..."));
		return;
	}

	hRes = m_pIAddOnAction->doAction(*psMethodName,
        *psSourceFile,
        *psId,
        m_pIUnkProjInfo,
        *psAdditionalInfo);
	if(FAILED(hRes))
	{
		DispMessage(hRes);
		return;
	}

    delete psMethodName;
    delete psSourceFile;
    delete psId;
    delete psAdditionalInfo;
}

void AddOnOps_Test::DispMessage(DWORD dwErrCode)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	CString strMessage;
	strMessage.Format("Fehler:\n%s\nHRESULT: %d", lpMsgBuf, dwErrCode);
	AfxMessageBox((LPCTSTR)strMessage);
}

