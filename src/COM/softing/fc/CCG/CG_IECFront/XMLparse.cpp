



#include <stdio.h>
#include "XMLParse.h"
#include "CG_Messages.h"
#include "CGT_CDiagMsg.h"

IXMLDOMNodeList* XML_LoadXMLFile(
    CGT_CDiagMsg*  pdm,
    const TCHAR*   pszFile, 
    const wchar_t* pszwRoot
)
{
    HRESULT          hr;
    BSTR             bstr;
    FC_CString       jot;
    IXMLDOMElement*  pRoot = NULL;
    IXMLDOMDocument* pXMLDoc = NULL;
    IXMLDOMNodeList* pNList = NULL;
    CG_Edipos        edp;

    CG_InitSTEdipos(&edp, pszFile);

    hr = CoCreateInstance(CLSID_DOMDocument,
	                      NULL,
	                      CLSCTX_INPROC_SERVER,
	                      IID_IXMLDOMDocument,
	     			      (void**)&pXMLDoc);
    	
    if(FAILED(hr))
    {
        pdm->sysErr(hr, &edp, _T("CoCreateInstance(CLSID_DOMDocument,..."), NULL);
        return NULL;
    }

	_variant_t   vSrc(pszFile);
	VARIANT_BOOL vBool = VARIANT_FALSE;
    
	hr =  pXMLDoc->load(vSrc, &vBool);
    if(FAILED(hr))
    {
        pdm->sysErr(hr, &edp, 
            _T("IID_IXMLDOMDocument::load for file: "), pszFile);
		pXMLDoc->Release();
        return NULL;
    }
    else if(vBool!=VARIANT_TRUE)
	{
        FC_CString reason;

        IXMLDOMParseError * pXMLError;
        hr = pXMLDoc->get_parseError(&pXMLError);
        FC_ASSERT_SYSERR(hr,0);
        if(SUCCEEDED(hr))
        {
            long l=0,c=0;
            BSTR bs;
            pXMLError->get_line(&l);
            pXMLError->get_linepos(&c);
            CG_InitSTEdipos(&edp, pszFile, l, c);

            hr = pXMLError->get_reason(&bs);
            FC_ASSERT_SYSERR(hr,0);
            if(SUCCEEDED(hr))
                jot.load(bs);

            pXMLError->Release();
        }
        pdm->msg1(CG_E_XML_PARSE_ERROR, &edp, jot);
		pXMLDoc->Release();
        return NULL;
	}

    hr = pXMLDoc->get_documentElement(&pRoot);
    pXMLDoc->Release();
    if(FAILED(hr))
    {
        pdm->sysErr(hr, &edp, _T("IID_IXMLDOMDocument::get_documentElement"), NULL);
        return NULL;
    }

    if(pszwRoot)
    {
        pRoot->get_tagName(&bstr);
        if(wcscmp(pszwRoot, bstr))
        {
            pdm->msg2(CG_E_XML_STRUCTURE_ROOT_NAME, &edp, pszwRoot, bstr);
            pRoot->Release();
            return NULL;
        }
        SysFreeString(bstr);bstr=NULL;
    }


    hr = pRoot->get_childNodes(&pNList);
    pRoot->Release();
    if(FAILED(hr))
    {
        pdm->sysErr(hr, &edp, _T("IID_IXMLDOMDocument::get_childNodes"), NULL);
        return NULL;
    }

    return pNList;
}

bool XML_GetStrAttrTrim(
    CGT_CDiagMsg*       pdm,
    const CG_Edipos*    pEdp,
    const wchar_t*      pszwElem,
    IXMLDOMNamedNodeMap*pAList,
    const wchar_t*      pszwAttr,
    FC_CString*         pStringToLoad,
    bool                bWarnOnTrimm,
    bool                bOptional
)
{
    bool            bDid = false;
    HRESULT         hr;
    VARIANT         val;
    wchar_t*        pszWchar;
    int             nLen;
    IXMLDOMNode*    pNode;

    pNode = NULL;
    if(FAILED(pAList->getNamedItem((wchar_t*)pszwAttr, &pNode)) || !pNode)
    {
        pStringToLoad->clear();
        if(!bOptional)
            pdm->msg2(CG_E_XML_STRUCTURE_ERROR_ATTR, pEdp, pszwAttr, pszwElem);
        return false;
    }

    VariantInit(&val);
    hr = pNode->get_nodeValue(&val);
    pNode->Release();
    if(FAILED(hr) || val.vt!=VT_BSTR)
    {
        VariantChangeType(&val, &val, VARIANT_NOVALUEPROP, VT_BSTR);
        if(val.vt==VT_BSTR)
            pszWchar = val.bstrVal;
        else
            pszWchar = L"?? <cannot convert to string> ??";

        pdm->msg3(CG_E_XML_STRUCTURE_ERROR_VALUE, pEdp, 
            pszwAttr, pszwElem, pszWchar);
        pNode->Release();
        VariantClear(&val);
        return false;
    }

    //trim the string and warn if trimmed:
    pszWchar = val.bstrVal;
    while(_istspace(*pszWchar))
    {
        bDid = true;
        pszWchar++;
    }
    for(nLen=wcslen(pszWchar);nLen>0 && _istspace(pszWchar[nLen-1]);nLen--)
        bDid = true;

    if(bDid && bWarnOnTrimm)
    {
        pdm->msg1(CG_W_XML_ATTRVAL_WHITESPACE, pEdp, val.bstrVal);
    }

    if(bDid)
        pszWchar[nLen] = 0;

    pStringToLoad->load(pszWchar);
    VariantClear(&val);
    return true;
}

bool XML_CLSIDFromString(
    CGT_CDiagMsg*    pdm,
    const CG_Edipos* pEdp,
    const TCHAR*     pszClsid,
    GUID*            pUUID
)
{
    HRESULT  hr;
#ifndef UNICODE
    wchar_t  wbuff[MAX_PATH];
    FC_StringToWideChar(pszClsid, wbuff, FC_ARRAY_LEN(wbuff));
    hr = CLSIDFromString(wbuff, pUUID);
#else
    hr = CLSIDFromString(pszClsid, pUUID);
#endif
    if(FAILED(hr))
    {
        pdm->sysErr(hr, pEdp, _T("CLSIDFromString: "), pszClsid);
        return false;
    }
    return true;
}


#if 0
IXMLElement* XML_GetChildWithName(const wchar_t* pszElemTypeName, IXMLElement* pElem)
{
    bool                    bOk;
    VARIANT                 var1;
    VARIANT                 var2;
    IDispatch*              pDisp;
    IXMLElement*            pChild = NULL;
    long                    aLong       = -1;
    IXMLElementCollection*  pChildren   = NULL;

    
    toassert(pElem);
    
    bOk = pElem && SUCCEEDED(pElem->get_children(&pChildren));
    
    if(!bOk || !pChildren)
    {
        if(pChildren)
            pChildren->Release();

        return NULL;
    }
    
   
    pChildren->get_length(&aLong);
    var1.vt  = VT_BSTR;
    var1.bstrVal  = (wchar_t*)pszElemTypeName;
    var2.vt  = VT_I4;
    var2.lVal= 0;


    bOk = SUCCEEDED(pChildren->item(var1, var2, &pDisp)) && pDisp;
    pChildren->Release();
    
    if(bOk)
    {
        bOk = SUCCEEDED(pDisp->QueryInterface(IID_IXMLElement, (void **)&pChild));
        pDisp->Release();
    }

    
    return pChild;
}
#endif



