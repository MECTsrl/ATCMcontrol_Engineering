

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "parsemessage.h"
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

CParseMessage::CParseMessage()
    : m_strMessage()
{
    m_code = S_OK;
}

void CParseMessage::Initialize(HRESULT hr, LPCTSTR pszMessage)
{
    m_code = hr;
    try {
        m_strMessage = pszMessage;
    }
    catch(CMemoryException * e) {
        e->Delete();
    }
    return;
}

STDMETHODIMP CParseMessage::get_Message(BSTR * psVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(psVal,E_POINTER);
    return ::AllocSysString(psVal,m_strMessage);
}

STDMETHODIMP CParseMessage::get_Code(LONG * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);
    *pVal = (long)m_code;		
	return S_OK;
}

STDMETHODIMP CParseMessage::IsError(VARIANT_BOOL * pVal)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    FC_PCHECK_HR(pVal,E_POINTER);

//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error

    long lSev = (m_code >> 30) & 0x3; 

    if(lSev == 3) {
        *pVal = VARIANT_TRUE;
    }
    else {
        *pVal = VARIANT_FALSE;
    }
	return S_OK;
}

ParseMessage* CreateParseMessage(HRESULT hrMessage, LPCTSTR pszMessage)
{
	try {
		ParseMessage * pmsg = NULL;

		HRESULT hr = CComObject<CParseMessage>::CreateInstance(&pmsg);
		if (FAILED(hr)) {
			return NULL;
		}

		pmsg->Initialize(hrMessage,pszMessage);
		pmsg->AddRef();
        return pmsg;
	}
	catch(CMemoryException* e) {
		e->Delete();
	}

    return NULL;
}

