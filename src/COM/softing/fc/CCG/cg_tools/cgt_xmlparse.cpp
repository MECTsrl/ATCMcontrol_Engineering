


//----  Includes:   -------------------------------------------*
#include <stdio.h>
#include "fc_tools\fc_fileIO.h"
#include "cgt_xmlparse.h"
#include "cgt_misc.h"



static void XMLDisplayParseError(
    CGT_CDiagMsg*    pdm,
    IXMLDOMDocument* pXMLDoc,
    const TCHAR*     pszFile
)
{
    CG_Edipos          edp;
    HRESULT            hr;
    FC_CString         reason;
    IXMLDOMParseError* pXMLError = NULL;

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
            reason.load(bs);

        FC_RELEASE_PTR(pXMLError);
    }

    pdm->msg1(CG_E_XML_PARSE_ERROR, &edp, reason);
}




IXMLDOMNode* CGT_XMLLoadFile(
    CGT_CDiagMsg*        pdm,
    const TCHAR*         pszPrjPath,
    const TCHAR*         pszFile, 
    const wchar_t*       pszwRoot
)
{
    HRESULT          hr;
    BSTR             bstr;
    IXMLDOMElement*  pRoot;
    IXMLDOMDocument* pXMLDoc;
    CG_Edipos        edp;
    TCHAR            fullPath[2*MAX_PATH];



    FC_FileFullPath(pszPrjPath, pszFile, fullPath, FC_ARRAY_LEN(fullPath));

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

	_variant_t   vSrc(fullPath);
	VARIANT_BOOL vBool = VARIANT_FALSE;
    
	hr =  pXMLDoc->load(vSrc, &vBool);
    if(FAILED(hr))
    {
        pdm->sysErr(hr, &edp, 
            _T("IID_IXMLDOMDocument::load for file: "), pszFile);
        FC_RELEASE_PTR(pXMLDoc);
        return NULL;
    }
    
    if(vBool!=VARIANT_TRUE)
	{
        FC_CString reason;

        IXMLDOMParseError * pXMLError = NULL;
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
                reason.load(bs);
        }

        pdm->msg1(CG_E_XML_PARSE_ERROR, &edp, reason);
 
        FC_RELEASE_PTR(pXMLError);
        FC_RELEASE_PTR(pXMLDoc);

        return NULL;
	}

    pRoot = NULL;
    hr = pXMLDoc->get_documentElement(&pRoot);
    FC_RELEASE_PTR(pXMLDoc);
    if(FAILED(hr) || !pRoot)
    {
        pdm->sysErr(hr, &edp, _T("IID_IXMLDOMDocument::get_documentElement"), NULL);
        return NULL;
    }

    if(pszwRoot)
    {
        bstr = NULL;
        pRoot->get_tagName(&bstr);
        if(!bstr || wcscmp(pszwRoot, bstr))
        {
            pdm->msg2(CG_E_XML_STRUCTURE_ROOT_NAME, &edp, pszwRoot, bstr);
            FC_FREESYSTRING(bstr);
            FC_RELEASE_PTR(pRoot);
            return NULL;
        }
        FC_FREESYSTRING(bstr);
    }

    return pRoot;
}




IXMLDOMNode* CGT_XMLLoadLocatorFile(
    CGT_CDiagMsg*    pdm,           //for err msg
    ICG_IECFront*    pIFront,       //used if locator points to a 4cl lib
    const TCHAR*     pszFileLocator,//file locator
    const wchar_t*   pszwRoot,
    __int64*         pftLastWrite   //NULL or receives the last file write time
)
{
    HRESULT          hr;
    BSTR             bstr;
    IXMLDOMElement*  pRoot;
    IXMLDOMDocument* pXMLDoc;
    CG_Edipos        edp;
    long             ltyp;
    CGT_FileLocator  splitted;
    TCHAR            fullPath[2*MAX_PATH];
    VARIANT_BOOL     vBoolXMLSyntaxOK = VARIANT_FALSE;
    
    if(pftLastWrite)
        *pftLastWrite = 0;

    CG_InitSTEdipos(&edp, pszFileLocator);

    ltyp = CGT_SplitFileLocator(pszFileLocator, &splitted);
    if(ltyp<=0)
    {
        CGT_IERR(pdm, pdm->jot1()<<_T("invalid locator '")<<pszFileLocator<<_T("'"));
        return NULL;
    }
    FC_FileFullPath(pIFront->FCPgetPrjPath(), splitted.pszFile, fullPath, FC_ARRAY_LEN(fullPath));


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



    if(ltyp==1)
    {
	    _variant_t   vSrc(fullPath);
	    hr =  pXMLDoc->load(vSrc, &vBoolXMLSyntaxOK);
        if(pftLastWrite)
            *pftLastWrite = FC_FileGetLastWriteTime(fullPath);
    }
    else //2 or 3 a library file:
    {
        const CG_LIBContext*    pLibCtx;
        I4CLAccess*             pLibAcc;
        
        pLibCtx = pIFront->FCPgetLIBinfo(FC_StringGetFilePart(fullPath));
        pLibAcc = NULL;

        if(pLibCtx)
            pLibAcc = pIFront->get4CLLibIF(pLibCtx);

        BSTR sFileContent = NULL; 
        if(pLibAcc)
        {

            BSTR sPOU = FC_StringToBSTR(splitted.pszPOUName);
            BSTR sTRG = FC_StringToBSTR(splitted.pszTrgType);
            BSTR sFIL = FC_StringToBSTR(splitted.pszLibFile);
            pLibAcc->GetFileAsBSTR(sPOU, sTRG, sFIL, &sFileContent);
            SysFreeString(sPOU);
            SysFreeString(sTRG);
            SysFreeString(sFIL);
        }


        if(sFileContent)
        {
            hr =  pXMLDoc->loadXML(sFileContent, &vBoolXMLSyntaxOK);
            if(pftLastWrite)
                *pftLastWrite = FC_FileGetLastWriteTime(fullPath);
            
            SysFreeString(sFileContent);
        }
        else
        {
            pdm->msg2(CG_E_FILE_OPEN, NULL, pszFileLocator, _T("?"));
            FC_RELEASE_PTR(pXMLDoc);
            return NULL;
        }
    }

    if(FAILED(hr))
    {
        pdm->sysErr(hr, &edp, _T("IID_IXMLDOMDocument::load failed "), NULL);
        FC_RELEASE_PTR(pXMLDoc);
        return NULL;
    }

    if(!vBoolXMLSyntaxOK)
    {
        XMLDisplayParseError(pdm, pXMLDoc, pszFileLocator);
        FC_RELEASE_PTR(pXMLDoc);
        return NULL;
    }




    pRoot = NULL;
    hr = pXMLDoc->get_documentElement(&pRoot);
    FC_RELEASE_PTR(pXMLDoc);
    if(FAILED(hr) || !pRoot)
    {
        pdm->sysErr(hr, &edp, _T("IID_IXMLDOMDocument::get_documentElement"), NULL);
        return NULL;
    }

    if(pszwRoot)
    {
        bstr = NULL;
        pRoot->get_tagName(&bstr);
        if(!bstr || wcscmp(pszwRoot, bstr))
        {
            pdm->msg2(CG_E_XML_STRUCTURE_ROOT_NAME, &edp, pszwRoot, bstr);
            FC_FREESYSTRING(bstr);
            FC_RELEASE_PTR(pRoot);
            return NULL;
        }
        FC_FREESYSTRING(bstr);
    }

    return pRoot;
}




bool CGT_XMLGetStrAttrTrim(
    CGT_CDiagMsg*       pdm,
    IXMLDOMNode*        pNode,
    const wchar_t*      pszwAttr,
    FC_CString*         pStringToLoad,
    bool                bWarnOnTrimm,
    bool                bOptional
)
{
    bool                 bDid;
    HRESULT              hr;
    VARIANT              val;
    wchar_t*             pszWchar;
    int                  nLen;
    BSTR                 nodename;
    IXMLDOMNode*         pAttr;
    IXMLDOMNamedNodeMap* pAList;

    pStringToLoad->clear();
    pAList = NULL;
    pAttr  = NULL;
    
    assert(pNode);
    if(!pNode)
        return false;

    pNode->get_attributes(&pAList);
    if(pAList)
    {
        pAList->getNamedItem((wchar_t*)pszwAttr, &pAttr);
        FC_RELEASE_PTR(pAList);
    }

    if(!pAttr)
    {
        if(!bOptional)
        {
            pNode->get_nodeName(&nodename);
            CGT_XMLDiagMsg2(pdm, pNode, CG_E_XML_STRUCTURE_ERROR_ATTR, 
                pszwAttr, nodename);
            FC_FREESYSTRING(nodename);
        }
        return false;
    }

    VariantInit(&val);
    hr = pAttr->get_nodeValue(&val);
    FC_RELEASE_PTR(pAttr);
    
    if(FAILED(hr) || val.vt!=VT_BSTR)
    {
        VariantChangeType(&val, &val, VARIANT_NOVALUEPROP, VT_BSTR);
        if(val.vt==VT_BSTR)
            pszWchar = val.bstrVal;
        else
            pszWchar = L"<cannot convert to string!!>";

        pNode->get_nodeName(&nodename);
        CGT_XMLDiagMsg3(pdm, pNode, CG_E_XML_STRUCTURE_ERROR_VALUE,  
            nodename, pszWchar, pszwAttr);
        FC_FREESYSTRING(nodename);
        VariantClear(&val);
        return false;
    }

    //trim the string and warn if trimmed:
    bDid = false;
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
        CGT_XMLDiagMsg1(pdm, pNode, CG_W_XML_ATTRVAL_WHITESPACE, val.bstrVal);
    }

    if(bDid)
        pszWchar[nLen] = 0;

    pStringToLoad->load(pszWchar);
    VariantClear(&val);

    return true;
}


int CGT_XMLGetBoolAttr(
    CGT_CDiagMsg*       pdm,
    IXMLDOMNode*        pNode,
    const wchar_t*      pszwAttr,
    bool*               pbAttr,
    bool                bOptional
)
{
    bool                 bDid;
    HRESULT              hr;
    VARIANT              val;
    wchar_t*             pszWchar;
    int                  nLen;
    BSTR                 nodename;
    IXMLDOMNode*         pAttr;
    IXMLDOMNamedNodeMap* pAList;

    pAList = NULL;
    pAttr  = NULL;
    
    assert(pNode);
    if(!pNode)
        return -1;

    pNode->get_attributes(&pAList);
    if(pAList)
    {
        pAList->getNamedItem((wchar_t*)pszwAttr, &pAttr);
        FC_RELEASE_PTR(pAList);
    }

    if(!pAttr)
    {
        if(!bOptional)
        {
            pNode->get_nodeName(&nodename);
            CGT_XMLDiagMsg2(pdm, pNode, CG_E_XML_STRUCTURE_ERROR_ATTR, 
                pszwAttr, nodename);
            FC_FREESYSTRING(nodename);
            return -1;
        }
        return 0;
    }

    VariantInit(&val);
    hr = pAttr->get_nodeValue(&val);
    FC_RELEASE_PTR(pAttr);
    
    if(FAILED(hr) || val.vt!=VT_BSTR)
    {
        VariantChangeType(&val, &val, VARIANT_NOVALUEPROP, VT_BSTR);
        if(val.vt==VT_BSTR)
            pszWchar = val.bstrVal;
        else
            pszWchar = L"<cannot convert to string!!>";

        pNode->get_nodeName(&nodename);
        CGT_XMLDiagMsg3(pdm, pNode, CG_E_XML_STRUCTURE_ERROR_VALUE,  
            nodename, pszWchar, pszwAttr);
        FC_FREESYSTRING(nodename);
        VariantClear(&val);
        return -1;
    }

    //trim the string and warn if trimmed:
    bDid = false;
    pszWchar = val.bstrVal;
    while(_istspace(*pszWchar))
    {
        bDid = true;
        pszWchar++;
    }
    for(nLen=wcslen(pszWchar);nLen>0 && _istspace(pszWchar[nLen-1]);nLen--)
        bDid = true;

    if(bDid)
        CGT_XMLDiagMsg1(pdm, pNode, CG_W_XML_ATTRVAL_WHITESPACE, val.bstrVal);

    if(bDid)
        pszWchar[nLen] = 0;

    if(!wcscmp(pszWchar, L"0"))
        *pbAttr = false;
    else if(!wcscmp(pszWchar, L"1"))
        *pbAttr = true;
    else
    {
        pNode->get_nodeName(&nodename);
        CGT_XMLDiagMsg3(pdm, pNode, CG_E_XML_STRUCTURE_ERROR_VALUE,  
            nodename, pszWchar, pszwAttr);
        FC_FREESYSTRING(nodename);
        VariantClear(&val);
        return -1;
    }

    VariantClear(&val);

    return 1;
}




bool CGT_XMLGetCLSIDAttr(
    CGT_CDiagMsg*    pdm,
    IXMLDOMNode*     pNode,
    const wchar_t*   pszwAttr,
    GUID*            pUUID,
    bool             bOptional
)
{
    HRESULT    hr;
    BSTR       bstrClsid;
    BSTR       nodeName;
    FC_CString jot;

    memset(pUUID, 0, sizeof(*pUUID));
    if(!CGT_XMLGetStrAttrTrim(pdm, pNode, pszwAttr, &jot, true, bOptional))
        return false;

    bstrClsid = jot.toBSTR();
    hr = CLSIDFromString(bstrClsid, pUUID);

    if(FAILED(hr))
    {
        pNode->get_nodeName(&nodeName);
        CGT_XMLDiagMsg3(pdm, pNode, CG_E_XML_STRUCTURE_ERROR_VALUE,
            nodeName, bstrClsid, pszwAttr);
        FC_FREESYSTRING(nodeName);
        FC_FREESYSTRING(bstrClsid);
        return false;
    }

    FC_FREESYSTRING(bstrClsid);
    return true;
}




void CGT_XMLDiagMsg(
    CGT_CDiagMsg*  pdm, 
    IXMLDOMNode*   pNode, 
    HRESULT        errId, 
    const TCHAR*   ppsz[]
)
{
    FC_CString       jot;
    const TCHAR*     pszFile;
    CG_Edipos        edp;
    IXMLDOMDocument* pdoc;
    BSTR             bstr;
 
    pNode->get_ownerDocument(&pdoc);
    pdoc->get_url(&bstr);
    jot.load(bstr);
    FC_RELEASE_PTR(pdoc);
    FC_FREESYSTRING(bstr);

    jot.stripBeginI(_T("file:"));
    pszFile = jot;
    while(*pszFile==_T('\\') || *pszFile==_T('/'))
        pszFile++;

    CG_InitSTEdipos(&edp, pszFile);//TODO impossible to get (line,col)??
    pdm->msg(errId, &edp, ppsz);

}

void CGT_XMLDiagMsg0(
    CGT_CDiagMsg*  pdm, 
    IXMLDOMNode*   pNode, 
    HRESULT        errId 
)
{
    CGT_XMLDiagMsg(pdm, pNode, errId, NULL);
}

void CGT_XMLDiagMsg1(
    CGT_CDiagMsg*  pdm, 
    IXMLDOMNode*   pNode, 
    HRESULT        errId, 
    const wchar_t* psz
)
{
    FC_CString jot;
    jot.load(psz);

    const TCHAR* ppsz[] = {jot};
    CGT_XMLDiagMsg(pdm, pNode, errId, ppsz);
}


void CGT_XMLDiagMsg2(
    CGT_CDiagMsg*  pdm, 
    IXMLDOMNode*   pNode, 
    HRESULT        errId, 
    const wchar_t* psz,
    const wchar_t* psz2
)
{
    FC_CString jot,jot2;
    jot.load(psz);
    jot2.load(psz2);

    const TCHAR* ppsz[] = {jot, jot2};
    CGT_XMLDiagMsg(pdm, pNode, errId, ppsz);
}

void CGT_XMLDiagMsg3(
    CGT_CDiagMsg*  pdm, 
    IXMLDOMNode*   pNode, 
    HRESULT        errId, 
    const wchar_t* psz,
    const wchar_t* psz2,
    const wchar_t* psz3
)
{
    FC_CString jot,jot2,jot3;
    jot.load(psz);
    jot2.load(psz2);
    jot3.load(psz3);

    const TCHAR* ppsz[] = {jot, jot2, jot3};
    CGT_XMLDiagMsg(pdm, pNode, errId, ppsz);
}



