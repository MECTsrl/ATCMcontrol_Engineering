

#include "stdafx.h"
#include "IDBackparser.h"
#include "CEBackparseImpl.h"
#include "CEMsg_i.c"
#include "IDBackparserCP.h"

/////////////////////////////////////////////////////////////////////////////
// CCEBackparse


STDMETHODIMP CCEBackparseImpl::doBackparse(BSTR sFileName, ICEProjInfo* pICEProjInfo, BSTR *psXMLText)
{
    CComBSTR fileName;
    CComBSTR fileContent;
    
    fileName = sFileName;

    CComBSTR sMachine(_T("IDBackparser"));
    CComBSTR sDescr(_T(""));
    USHORT wScrFacility = 0x282;
    HRESULT hr = S_OK;
    //Fire_OnNewMessage(sMachine, wScrFacility, hr, sDescr);

    
    CComQIPtr<IXMLDOMDocument, &IID_IXMLDOMDocument> m_domDocument;
    hr = m_domDocument.CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER);
    if (hr != S_OK)
    {
        return E_FAIL;
    }
	VARIANT_BOOL vbIsSuccessful = VARIANT_FALSE;
    CComVariant vFileName = fileName;
    hr =  m_domDocument->load(vFileName, &vbIsSuccessful);
	if (hr != S_OK || (vbIsSuccessful==VARIANT_FALSE))
	{
        return E_FAIL;
    }

    m_domDocument->put_preserveWhiteSpace(VARIANT_FALSE);
    hr = m_domDocument->get_xml(&fileContent);

    if (hr != S_OK)
    {
        return E_FAIL;
    }


    *psXMLText = fileContent.Copy();

	return S_OK;
}

