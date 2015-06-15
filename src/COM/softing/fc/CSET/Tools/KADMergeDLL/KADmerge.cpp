/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL/KADmerge.cpp 2     7.08.07 13:21 Ef $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: KADmerge.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL/KADmerge.cpp $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CE
 *
 * =COMPONENT           CSET
 *
 * =CURRENT      $Date: 7.08.07 13:21 $
 *           $Revision: 2 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  15.03.2000  KT      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/

// KADmerge.cpp: implementation of the CKADmerge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KADmergeDLL.h"
#include "KADmerge.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef struct PrimaryKey {
    LPCSTR Element;
    LPCSTR PrimKey;
} PrimaryKey;

LPSTR End = "&END";

/*PrimaryKey pPrimaryKeys[] =
{
    "KERNEL", "VERSION",
    "ADDON", "NAME",
    "FILE", "EXT",
    "CLASSES", "TYPE",
    "CLASS", "TYPE",
    "TARGETS", "TYPE",
    "TARGET", "TYPE",
    "MENU", "ID",
    "MENUITEM", "TEXT",
    "TOOLBAR", "ID",
    "TOOLBARITEM", "ICON",
    "TYPE", "ID",
    End, End
};*/

LPSTR pOverwriteable[] =
{
    "GENERAL",
    "ADDON",
    "FILE",
    "CLASS",
    "TARGET",
    //"MENU",
    "MENUITEM",
    //"TOOLBAR",
    "TOOLBARITEM",
    //"TYPE",
    "TARGETONLINE",
    End
};

string sDescription = "4C KAD Merge Tool";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKADmerge::CKADmerge()
{
	HRESULT hRes = CoInitialize(NULL);
    if(FAILED(hRes))
    {
        throw string("CoInitialize() failed");
    }
    m_pXMLKad = NULL;
}

CKADmerge::CKADmerge(string sFileName)
{
    m_bIsDirty = FALSE;

	HRESULT hRes = CoInitialize(NULL);
    if(FAILED(hRes))
    {
        throw string("CoInitialize() failed");
    }
    m_sFileName = sFileName;

    LoadXMLFile(sFileName, &m_pXMLKad);

    // init
    int i = 0;
    /*for(i = 0; pPrimaryKeys[i].Element != End; i++)
    {
        m_PrimaryKeys[pPrimaryKeys[i].Element] = pPrimaryKeys[i].PrimKey;
    }*/
    for(i = 0; pOverwriteable[i] != End; i++)
    {
        m_Overwriteable[pOverwriteable[i]] = TRUE;
    }
}

CKADmerge::~CKADmerge()
{
    if(m_pXMLKad)
    {
	   //HACK: for some reason the msxml3.dll crashes in DllMain if this is the last reference to the DLL
	   //we simply allow some mem leak and forget this problem untill we change to msxml4.dll
	   //m_pXMLKad->Release();
    }
    CoUninitialize();
}

HRESULT CKADmerge::CopyNode(IXMLDOMNode ** pXMLDest, IXMLDOMNode ** pXMLSrc, BOOL bOverwrite, string sPath)
{
    HRESULT hRes = 0;

    IXMLDOMNodeList * pNodeList = NULL;

    if(*pXMLDest == NULL || *pXMLSrc == NULL)
    {
        Log("ERROR: CopyNode " + GetName(*pXMLSrc) + " to " + GetName(*pXMLDest));
        hRes = S_FALSE;
        return hRes;
    }

    hRes = (*pXMLSrc)->get_childNodes(&pNodeList);

    if(SUCCEEDED(hRes))
    {
        LONG lListLength = 0;
        hRes = pNodeList->get_length(&lListLength);

        IXMLDOMNode * pCurrentNode = NULL;
        hRes = pNodeList->nextNode(&pCurrentNode);

        while(pCurrentNode != NULL)
        {
            // iterate through the xml tree
            string sPatternString = GetPatternString(pCurrentNode, TRUE);

            IXMLDOMNode * pDestNode = NULL;
            BOOL bHasNode = HasNode(*pXMLDest, sPatternString, &pDestNode);
            if(bHasNode && (!m_Overwriteable[GetName(pCurrentNode)] || !bOverwrite ))
            {
                // copy child elements
                CopyNode(&pDestNode, &pCurrentNode, bOverwrite, sPath + "/" + sPatternString);
            }
            else if(bHasNode && m_Overwriteable[GetName(pCurrentNode)] && bOverwrite)
            {
                // compare element
                sPatternString = GetPatternString(pCurrentNode, TRUE);
                bHasNode = HasNode(*pXMLDest, sPatternString, &pDestNode);

                if(bHasNode)
                {
                    sPatternString = GetPatternString(pCurrentNode, FALSE);
                    bHasNode = HasNode(*pXMLDest, sPatternString, &pDestNode);

                    if(!bHasNode)
                    {
                        // replace element
                        IXMLDOMNode * pNewNode = NULL;
                        IXMLDOMNode * pXMLOldChild = NULL;

                        hRes = pCurrentNode->cloneNode(VARIANT_TRUE, &pNewNode);
                        hRes = (*pXMLDest)->replaceChild(pNewNode,          // new child
                                                         pDestNode,         // old child
                                                         &pXMLOldChild);    // out old child

                        // log changes
                        Log("Replace Element: " + sPath + "/" + sPatternString);

                        if(pNewNode)
                            pNewNode->Release();
                        if(pXMLOldChild)
                            pXMLOldChild->Release();
                        m_bIsDirty = TRUE;
                    }
                }
            }
            else if(!bHasNode)
            {
                // attach entire sub tree
                IXMLDOMNode * pDestNode = NULL;
                IXMLDOMNode * pXMLNewChild = NULL;
                hRes = pCurrentNode->cloneNode(VARIANT_TRUE, &pDestNode);
                hRes = (*pXMLDest)->appendChild(pDestNode,          // new child
                                                &pXMLNewChild);     // out new child

                // log changes
                sPatternString = GetPatternString(pCurrentNode, FALSE);
                Log("Attach Element: " + sPath + "/" + sPatternString);

                if(pDestNode)
                    pDestNode->Release();
                if(pXMLNewChild)
                    pXMLNewChild->Release();
                m_bIsDirty = TRUE;
            }

            hRes = pNodeList->nextNode(&pCurrentNode);
        }
    }

    return hRes;
}

string CKADmerge::ExpandBackslash(string str)
{
    // Expand '\' to '\\'
    int iPos = str.find("\\");;
    while(iPos >= 0)
    {
        str.replace(iPos, 1, "\\\\");
        iPos = str.find("\\", iPos + 2);
    }
    return str;
}

HRESULT CKADmerge::LoadXMLFile(string sFileName, IXMLDOMDocument ** pXMLDoc)
{
	HRESULT hRes = CoCreateInstance(CLSID_DOMDocument,
                                    NULL,
                                    CLSCTX_INPROC_SERVER,
                                    IID_IXMLDOMDocument,
                                    (void**)pXMLDoc);
	if(FAILED(hRes))
    {
		throw string("CoCreateInstance() failed");
    }

	_variant_t   vXmlSource(sFileName.c_str());
	VARIANT_BOOL vbIsSuccessful = VARIANT_FALSE;

    (*pXMLDoc)->put_async(VARIANT_FALSE);

	hRes = (*pXMLDoc)->load(vXmlSource, &vbIsSuccessful);
	if(vbIsSuccessful != VARIANT_TRUE)
	{
        (*pXMLDoc)->Release();
	    hRes = CoCreateInstance(CLSID_DOMDocument,
                                NULL,
                                CLSCTX_INPROC_SERVER,
                                IID_IXMLDOMDocument,
                                (void**)pXMLDoc);
    }

	return hRes;
}

HRESULT CKADmerge::RemoveKADFile(string &sFilename, string sLogFile)
{
    HRESULT hRes = 0;

    if(sLogFile != "")
    {
        // open log file
        OpenLog(sLogFile, sDescription + " " + m_sFileName);
    }

    // set new filename
    time_t ltime;
    struct tm * stime;
    char * pszValue = new char[MAX_PATH];

    time(&ltime);
    stime = gmtime(&ltime);

    string sNewFilename = sFilename;
    sprintf(pszValue, ".%d-%d-%d_%d-%d-%d", stime->tm_mday,
        stime->tm_mon+1,
        stime->tm_year+1900,
        stime->tm_hour+1,
        stime->tm_min,
        stime->tm_sec);
    sNewFilename += pszValue;
    delete pszValue;

    if( 0 == MoveFile(sFilename.c_str(), sNewFilename.c_str()) )
    {
        hRes = S_FALSE;
    }
    else
    {
        // log changes
        Log("Delete File: " + sFilename + " Backup: " + sNewFilename);

        hRes = S_OK;
        sFilename = sNewFilename;
    }

    return hRes;
}

HRESULT CKADmerge::SaveXMLFile(string sFileName, IXMLDOMDocument * pXMLDoc)
{
	_variant_t vDestFile(sFileName.c_str());
	HRESULT hRes = pXMLDoc->save(vDestFile);
	if(FAILED(hRes))
	{
        Log("ERROR: Could not save file: " + sFileName);    // save failed
		pXMLDoc->Release();
	}

    return hRes;
}

HRESULT CKADmerge::GetKADFilenames(string * sFilenames[], LONG &lLength, string sLogFile)
{
    HRESULT hRes = S_FALSE;

    if(sLogFile != "")
    {
        // open log file
        OpenLog(sLogFile, sDescription + " GetKADFileNames");
    }

    IXMLDOMNode * pXMLRoot = NULL;
    hRes = GetRootElement(m_pXMLKad, &pXMLRoot);

    if(hRes == S_OK)
    {
        // find elements
        IXMLDOMNodeList * pElemList = NULL;
        _bstr_t bPatternString("/KAD4CE/KERNEL/ADDON[@KADFILE]");
        hRes = pXMLRoot->selectNodes(bPatternString, &pElemList);

        if(SUCCEEDED(hRes))
        {
            LONG lElements = 0;
            pElemList->get_length(&lElements);

            (*sFilenames) = new string[lElements];

            IXMLDOMNode * pCurrentElem = NULL;
            hRes = pElemList->nextNode(&pCurrentElem);
            int i = 0;
            while(pCurrentElem != NULL)
            {
                // find 'KADFILE' attribute
                IXMLDOMNodeList * pAttrList = NULL;
                _bstr_t bPatternString("@KADFILE");
                hRes = pCurrentElem->selectNodes(bPatternString, &pAttrList);

                if(SUCCEEDED(hRes))
                {
                    LONG lAtributes = 0;
                    pAttrList->get_length(&lAtributes);

                    IXMLDOMNode * pCurrentAttr = NULL;
                    hRes = pAttrList->nextNode(&pCurrentAttr);
                    if(lAtributes == 1)
                    {
                        (*sFilenames)[i++] = GetValue(pCurrentAttr);
                    }
                    if(pCurrentAttr)
                        pCurrentAttr->Release();
                }
                hRes = pElemList->nextNode(&pCurrentElem);
                if(pAttrList)
                    pAttrList->Release();
            }
            lLength = i;
            hRes = S_OK;

            if(pCurrentElem)
                pCurrentElem->Release();
        }

        if(pXMLRoot)
            pXMLRoot->Release();
        if(pElemList)
            pElemList->Release();
    }

    if(hRes == S_OK)
    {
        Log("GetKADFilenames:");
        for(int i = 0; i < (int)lLength; i++)
        {
            Log((*sFilenames)[i]);
        }
    }

    // close log file
    CloseLog();

    return hRes;
}

//DEL BOOL CKADmerge::GetLanguageAttribute(string & sLanguageID)
//DEL {
//DEL     BOOL bRet = FALSE;
//DEL     IXMLDOMNode * pAttrLanguage = NULL;
//DEL     _bstr_t patternString("/KAD4CE/KERNEL/ADDON/@LANGUAGE");
//DEL 
//DEL     try
//DEL 	{
//DEL         HRESULT hRes = 0;
//DEL         hRes = m_pXMLKad->selectSingleNode(patternString, &pAttrLanguage);
//DEL         if(hRes == S_FALSE)
//DEL         {
//DEL             throw string("node access error");
//DEL         }
//DEL     	VARIANT vAttlanguage;
//DEL         hRes = pAttrLanguage->get_nodeValue(&vAttlanguage);
//DEL         if(FAILED(hRes) || vAttlanguage.vt != VT_BSTR)
//DEL         {
//DEL             throw string("value error");
//DEL         }
//DEL         _bstr_t bAttlanguage(vAttlanguage.bstrVal);
//DEL         sLanguageID = bAttlanguage;
//DEL         VariantClear(&vAttlanguage);
//DEL         bRet = TRUE;
//DEL 	}
//DEL 	catch(string str)
//DEL 	{
//DEL         sLanguageID = "0";
//DEL         bRet = FALSE;
//DEL 	}
//DEL 
//DEL 	if(pAttrLanguage != NULL)
//DEL 		pAttrLanguage->Release();
//DEL 
//DEL     return bRet;
//DEL }

//DEL BOOL CKADmerge::SetLanguageAttribute(string sLanguageID, string sLogFile)
//DEL {
//DEL     BOOL bRet = FALSE;
//DEL     IXMLDOMNode * pNodeKernel = NULL;
//DEL     _bstr_t patternString("/KAD4CE/KERNEL/@LANGUAGE");
//DEL 
//DEL     if(sLogFile != "")
//DEL     {
//DEL         // open log file
//DEL         OpenLog(sLogFile, sDescription + " " + m_sFileName);
//DEL     }
//DEL 
//DEL     try
//DEL 	{
//DEL         // select kernel node
//DEL         HRESULT hRes = 0;
//DEL         hRes = m_pXMLKad->selectSingleNode(patternString, &pNodeKernel);
//DEL         if(hRes == S_FALSE)
//DEL         {
//DEL             bRet = FALSE;
//DEL             throw string("node access error");
//DEL         }
//DEL 
//DEL         _bstr_t bAttlanguage(sLanguageID.c_str());
//DEL         VARIANT vAttlanguage;
//DEL         vAttlanguage.vt = VT_BSTR;
//DEL         V_BSTR(&vAttlanguage) = bAttlanguage.copy();
//DEL 
//DEL         hRes = pNodeKernel->put_nodeValue(vAttlanguage);
//DEL         VariantClear(&vAttlanguage);
//DEL 
//DEL         SaveXMLFile(m_sFileName, m_pXMLKad);
//DEL         bRet = TRUE;
//DEL 
//DEL         //delete pwAttlanguage;
//DEL         if(FAILED(hRes))
//DEL         {
//DEL             bRet = FALSE;
//DEL             throw string("value error");
//DEL         }
//DEL         // log changes
//DEL         Log("Set Language Attribute to: " + sLanguageID);
//DEL 	}
//DEL 	catch(string str)
//DEL 	{
//DEL         bRet = FALSE;
//DEL 	}
//DEL 
//DEL 	if(pNodeKernel != NULL)
//DEL 		pNodeKernel->Release();
//DEL 
//DEL     // close log file
//DEL     CloseLog();
//DEL 
//DEL     return bRet;
//DEL }

HRESULT CKADmerge::Merge(string sAddOnFileName, BOOL bOverwrite, string sLogFile)
{
    HRESULT hRes = 0;

	IXMLDOMDocument * pXMLAddOn = NULL;

    if(sLogFile != "")
    {
        // open log file
        OpenLog(sLogFile, sDescription + " " + m_sFileName + " with " + sAddOnFileName);
    }

    try
	{
        // load AddOn file
        hRes = LoadXMLFile(sAddOnFileName, &pXMLAddOn);

        IXMLDOMElement * pKadRoot = NULL;
        hRes = GetRootElement(m_pXMLKad, &pKadRoot);
        if(hRes == S_FALSE)
        {
            // create root element
            hRes = GetRootElement(pXMLAddOn, &pKadRoot);
            if(hRes == S_FALSE)
            {
                throw string("ERROR: could not get addon kad root element: " + sAddOnFileName);
            }

            _bstr_t bTagName(GetName(pKadRoot).c_str());
            hRes = m_pXMLKad->createElement(bTagName, &pKadRoot);
            hRes = m_pXMLKad->putref_documentElement(pKadRoot);

            // log changes
            Log("Create Root-Element: " + GetName(pKadRoot) );

            m_bIsDirty = TRUE;
        }

        if(pKadRoot)
            pKadRoot->Release();

	    IXMLDOMNode * pXMLKadNode = NULL;
	    IXMLDOMNode * pXMLAddOnNode = NULL;

        hRes = GetRootElement(m_pXMLKad, &pXMLKadNode);
        hRes = GetRootElement(pXMLAddOn, &pXMLAddOnNode);

        // copy nodes
        hRes = CopyNode(&pXMLKadNode, &pXMLAddOnNode, bOverwrite, "");

	    if(pXMLKadNode != NULL)
		    pXMLKadNode->Release();
	    if(pXMLAddOnNode != NULL)
		    pXMLAddOnNode->Release();

        if(m_bIsDirty)
        {
            hRes = SaveXMLFile(m_sFileName, m_pXMLKad);
            m_bIsDirty = FALSE;
        }
	}
	catch(string str)
	{
        Log(str);
        hRes = S_FALSE;
	}

	if(pXMLAddOn != NULL)
		pXMLAddOn->Release();

    // close log file
    CloseLog();

    return hRes;
}

string CKADmerge::GetName(IXMLDOMNode * pXMLNode)
{
    HRESULT hRes = 0;
    string str = "";
    BSTR bName;

    if(!pXMLNode)
        return str;

    hRes = pXMLNode->get_nodeName(&bName);
    if(SUCCEEDED(hRes))
    {
        _bstr_t bName(bName);
        str = bName;
    }
    SysFreeString(bName);

    return str;
}

string CKADmerge::GetPatternString(IXMLDOMNode * pXMLNode, BOOL bQuery)
{
    HRESULT hRes = 0;
    string sPatternString = "";
    string sElementName;

    if(pXMLNode)
    {
        // retrieve node name
        sElementName = GetName(pXMLNode);
        sPatternString = sElementName;

        if(SUCCEEDED(hRes))
        {
            // retrieve all attributes
            IXMLDOMNodeList * pNodeList = NULL;
            BSTR bPatternString = NULL;
            bPatternString = ::SysAllocString(L"@*");
            _bstr_t b2PatternString(bPatternString);
            hRes = pXMLNode->selectNodes(bPatternString, &pNodeList);
            ::SysFreeString(bPatternString);

            LONG lListLength = 0;
            hRes = pNodeList->get_length(&lListLength);

            if(SUCCEEDED(hRes) && lListLength)
            {
                // retrive attribute names
                IXMLDOMNode * pCurrentNode = NULL;
                hRes = pNodeList->nextNode(&pCurrentNode);
                string sAttributeName;
                while(pCurrentNode != NULL)
                {
                    // attach attibute names and values to pattern string
                    sAttributeName = GetName(pCurrentNode);

                    if(!bQuery)
                    {
                        // all attributes
                        if(sPatternString.find("[") == -1)
                        {
                            sPatternString += "[";
                        }
                        else
                        {
                            sPatternString += " and " ;
                        }
                        sPatternString += "@" + sAttributeName;
                        sPatternString += "=\"" + ExpandBackslash(GetValue(pCurrentNode)) + "\"";
                    }
                    else
                    {
                        // just "primary key" (only first attribute)
                        if(sPatternString.find("[") == -1)
                        {
                            sPatternString += "[";
                        }
                        sPatternString += "@" + sAttributeName;
                        sPatternString += "=\"" + ExpandBackslash(GetValue(pCurrentNode)) + "\"";

                        break;
                    }
                    hRes = pNodeList->nextNode(&pCurrentNode);
                }
                if(sPatternString.find("[") != -1)
                {
                    sPatternString += "]";
                }
            }
            if(pNodeList)
                pNodeList->Release();
        }
    }

    return sPatternString;
}

HRESULT CKADmerge::GetRootElement(IXMLDOMDocument * pXMLDoc, IXMLDOMNode ** pXMLNode)
{
    IXMLDOMElement * pXMLElem = NULL;

    HRESULT hRes = 0;

    if(!pXMLDoc)
    {
        hRes = S_FALSE;
    }
    else
    {
        hRes = pXMLDoc->get_documentElement(&pXMLElem);
        if(pXMLElem)
        {
            hRes = pXMLElem->QueryInterface(IID_IXMLDOMNode, (void**)pXMLNode);
        }
    }

    if(pXMLElem)
        pXMLElem->Release();

    return hRes;
}

HRESULT CKADmerge::GetRootElement(IXMLDOMDocument * pXMLDoc, IXMLDOMElement ** pXMLElem)
{
    HRESULT hRes = 0;

    if(!pXMLDoc)
    {
        hRes = S_FALSE;
    }
    else
    {
        hRes = pXMLDoc->get_documentElement(pXMLElem);
    }

    return hRes;
}

string CKADmerge::GetValue(IXMLDOMNode * pXMLNode)
{
    HRESULT hRes = 0;
    string str = "";

    if(!pXMLNode)
        return str;

    /*VARIANT vValue;
    hRes = pXMLNode->get_nodeValue(&vValue);
    if(hRes == S_OK && vValue.vt != VT_BSTR)
    {
        HRESULT hRes = VariantChangeType(&vValue, &vValue, 0, VT_BSTR);
    }
    if(vValue.vt == VT_BSTR)
    {
        _bstr_t bValue(vValue.bstrVal);
        str = bValue;
    }*/

    // use 'get_text()' instead of 'get_nodeValue()'!
    // This method will trim trim leading and trailing white spaces.
    BSTR bValue = NULL;
    hRes = pXMLNode->get_text(&bValue);
    if(hRes == S_OK)
    {
        _bstr_t b2Value(bValue);
        str = b2Value;
    }
    SysFreeString(bValue);

    return str;
}

HRESULT CKADmerge::GetValueByPattern(string sElementPattern,
                                     string sAttributePattern,
                                     string & sValue,
                                     string sLogFile)
{
    HRESULT hRes = 0;

    if(sLogFile != "")
    {
        // open log file
        OpenLog(sLogFile, sDescription + " " + m_sFileName);
    }

    IXMLDOMNode * pElemNode = NULL;
    IXMLDOMNode * pAttrNode = NULL;
    _bstr_t bElementPattern(sElementPattern.c_str());

    try
	{
        // get element node by pattern string

        hRes = m_pXMLKad->selectSingleNode(bElementPattern, &pElemNode);
        if(hRes == S_FALSE)
        {
            throw string("node access error");
        }

        if(sAttributePattern.length())
        {
            // get attribute node by pattern string
            _bstr_t bAttributePattern(sAttributePattern.c_str());

            hRes = pElemNode->selectSingleNode(bAttributePattern, &pAttrNode);
            if(hRes == S_FALSE)
            {
                throw string("node access error");
            }
            // get attribute value
            sValue = GetValue(pAttrNode);
        }
        else
        {
            // get element value
            sValue = GetValue(pElemNode);
        }
        hRes = S_OK;

        // log changes
        Log("GetValueByPattern - Get value succeeded: Element '" + sElementPattern + " - Attribute " + sAttributePattern + "' is: " + sValue);
	}
	catch(string str)
	{
        sValue = "";
        hRes = S_FALSE;
        // log changes
        Log("GetValueByPattern - Get value succeeded: Element '" + sElementPattern + " - Attribute " + sAttributePattern);
        Log("ERROR: " + str);
	}

	if(pElemNode != NULL)
		pElemNode->Release();
	if(pAttrNode != NULL)
		pAttrNode->Release();

    // close log file
    CloseLog();

    return hRes;
}

HRESULT CKADmerge::SetValue(IXMLDOMNode * pXMLNode, string sValue)
{
    HRESULT hRes = 0;

    /*VARIANT vValue;
    hRes = pXMLNode->put_nodeValue(vValue);
    if(hRes == S_OK && vValue.vt != VT_BSTR)
    {
        HRESULT hRes = VariantChangeType(&vValue, &vValue, 0, VT_BSTR);
    }
    if(vValue.vt == VT_BSTR)
    {
        _bstr_t bValue(vValue.bstrVal);
        str = bValue;
    }*/

    // use 'get_text()' instead of 'get_nodeValue()'!
    // This method will trim trim leading and trailing white spaces.
    _bstr_t b2Value(sValue.c_str());
    BSTR bValue = ::SysAllocString(b2Value);
    hRes = pXMLNode->put_text(bValue);
    SysFreeString(bValue);

    return hRes;
}

HRESULT CKADmerge::SetValueByPattern(string sElementPattern,
                                     string sAttributePattern,
                                     string sValue,
                                     string sLogFile)
{
    HRESULT hRes = 0;

    if(sLogFile != "")
    {
        // open log file
        OpenLog(sLogFile, sDescription + " " + m_sFileName);
    }

    IXMLDOMNode * pElemNode = NULL;
    IXMLDOMNode * pAttrNode = NULL;
    _bstr_t bElementPattern(sElementPattern.c_str());

    try
	{
        // get element node
        hRes = m_pXMLKad->selectSingleNode(bElementPattern, &pElemNode);
        if(hRes == S_FALSE)
        {
            hRes = S_FALSE;
            throw string("node access error");
        }

        if(sAttributePattern.length())
        {
            // get attribute node by pattern string
            _bstr_t bAttributePattern(sAttributePattern.c_str());

            hRes = pElemNode->selectSingleNode(bAttributePattern, &pAttrNode);
            if(hRes == S_FALSE)
            {
                throw string("node access error");
            }

            // set attribute value
            _bstr_t bAttlanguage(sValue.c_str());
            VARIANT vAttlanguage;
            vAttlanguage.vt = VT_BSTR;
            V_BSTR(&vAttlanguage) = bAttlanguage.copy();

            hRes = pAttrNode->put_nodeValue(vAttlanguage);
            VariantClear(&vAttlanguage);
        }
        else
        {
            // set element value
            hRes = SetValue(pElemNode, sValue);
        }


        if(hRes == S_OK)
        {
            SaveXMLFile(m_sFileName, m_pXMLKad);
            // log changes
            Log("SetValueByPattern - Set value succeeded: Element '" + sElementPattern + " - Attribute " + sAttributePattern + "' to: " + sValue);
        }

        //delete pwAttlanguage;
        if(FAILED(hRes))
        {
            hRes = S_FALSE;
            throw string("value error");
        }
	}
	catch(string str)
	{
        hRes = S_FALSE;
        // log changes
        Log("SetValueByPattern - Set value failed: Element '" + sElementPattern + " - Attribute " + sAttributePattern + "' to: " + sValue);
        Log("ERROR: " + str);
	}

	if(pElemNode != NULL)
		pElemNode->Release();
	if(pAttrNode != NULL)
		pAttrNode->Release();

    // close log file
    CloseLog();

    return hRes;
}

BOOL CKADmerge::HasNode(IXMLDOMNode * pXMLNode, string sPatternString, IXMLDOMNode ** pXMLTarget)
{
    BOOL bRet = FALSE;
    HRESULT hRes = 0;
    LONG lListLength = 0;

    IXMLDOMNodeList * pXMLNodeList = NULL;
    _bstr_t bPatternString(sPatternString.c_str());
    hRes = pXMLNode->selectNodes(bPatternString, &pXMLNodeList);

    if(SUCCEEDED(hRes))
    {
        hRes = pXMLNodeList->get_length(&lListLength);
        if(lListLength)
        {
            hRes = pXMLNodeList->nextNode(pXMLTarget);
            bRet = TRUE;
        }
        else
        {
            string str01 = GetName(pXMLNode);
            string str02 = GetName(*pXMLTarget);
            bRet = FALSE;
        }
    }

    if(pXMLNodeList)
        pXMLNodeList->Release();

    return bRet;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: KADmerge.cpp $
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 7.08.07    Time: 13:21
 * Updated in $/4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL
 * fixed: msxml3.dll crashes in DllMain
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSET/Tools/KADmergeDLL
 * 
 * *****************  Version 6  *****************
 * User: Kt           Date: 20.06.01   Time: 10:42
 * Updated in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * changed 'HRESULT CopyNode(...)' prototype;
 * changed methods 'GetPatternString()' and 'CopyNode()'
 * no "Primary Keys" anymore
 * 
 * *****************  Version 5  *****************
 * User: Kt           Date: 15.05.01   Time: 11:45
 * Updated in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * changed 'pOverwriteable[]'
 * 
 * *****************  Version 4  *****************
 * User: Kt           Date: 9.04.01    Time: 14:47
 * Updated in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * changed 'overwriteable' flag for 'MENU' and 'TYPE' to false
 * 
 * *****************  Version 3  *****************
 * User: Kt           Date: 22.03.01   Time: 16:59
 * Updated in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * included 'Installed4CKernel()' and 'InstalledIECAddOn()'
 * 
 * *****************  Version 2  *****************
 * User: Kt           Date: 21.03.01   Time: 13:56
 * Updated in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 15.03.01   Time: 14:08
 * Created in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * New Project
 * 
 *----------------------------------------------------------------------------*
*/

