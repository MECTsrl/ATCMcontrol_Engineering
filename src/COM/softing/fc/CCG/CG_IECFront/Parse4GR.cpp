


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

#include <malloc.h>
#include <tchar.h>
//#include <mshtml.h>
//#include <msxml.h>

#include "Parse4GR.h"
//#include "LDFBD.h"
#include "utilif.h" //XML_DecodeString
#include "Hash4GR.h"
#include "XML_4gr_names.h"



//local types:
struct XML_4GR_BSTR_TAGS
{
    BSTR ELEM_HEADER;
    BSTR ELEM_BODY;
    BSTR ELEM_LINE;

    BSTR ELEM_VARWRITE;
    BSTR ELEM_VARREAD;
    BSTR ELEM_VARFEEDBACK;
    BSTR ELEM_CONNECTOR;
    BSTR ELEM_CONTINUATION;

    BSTR ELEM_COIL;
    BSTR ELEM_CONTACT;
    BSTR ELEM_POWERRAIL;

    BSTR ELEM_FNC;
    BSTR ELEM_FB;
    BSTR ELEM_PIN;
    BSTR ELEM_PINLIST;

    BSTR ELEM_LABEL;
    BSTR ELEM_JUMP;
    BSTR ELEM_RETURN;

    BSTR ELEM_STEP;
    BSTR ELEM_TRANS;
    BSTR ELEM_ALTDIV;         
    BSTR ELEM_SIMDIV;
    BSTR ELEM_ACTION;
    BSTR ELEM_ACTBLCK;
    BSTR ELEM_ACTQUAL;
    BSTR ELEM_STACTION;

//Attribute names:
    BSTR ATTR_ORIGIN;
    BSTR ATTR_INORIGIN;
    BSTR ATTR_OUTORIGIN;
    BSTR ATTR_SIZE;
    BSTR ATTR_NAME;
    BSTR ATTR_TYPE;
    BSTR ATTR_TYPENAME;
    BSTR ATTR_CLASS;
    BSTR ATTR_COMMENT;
    BSTR ATTR_RUBBERLINE;
    BSTR ATTR_NEGATE;
    BSTR ATTR_HIDDEN;
    BSTR ATTR_CENTRE_X;
    BSTR ATTR_CENTRE_Y;
    BSTR ATTR_DISABLE;
    BSTR ATTR_R_EDGE;
    BSTR ATTR_F_EDGE;
    BSTR ATTR_FUNC;
 
    BSTR ATTR_QUALTYPE;
    BSTR ATTR_QUALTIME;
    BSTR ATTR_INITSTEP;

//Attribute Values of attribute CLASS
    BSTR ATTR_VAL_INPUT;
    BSTR ATTR_VAL_OUTPUT;
    BSTR ATTR_VAL_INOUTPUT;
    BSTR ATTR_VAL_INEXT;
    BSTR ATTR_VAL_OUTEXT;
};


//local static data:
static XML_4GR_BSTR_TAGS XML_TAGS;





//------------------ 4GR/XML READ HELPER FUNCTIONS
static void Init_TAGS()
{
#define INIT_TAG(tag)  XML_TAGS.tag = FC_StringToBSTR(XML_4GR_##tag)
    INIT_TAG(ELEM_HEADER);
    INIT_TAG(ELEM_BODY);
    INIT_TAG(ELEM_LINE);

    INIT_TAG(ELEM_VARREAD);
    INIT_TAG(ELEM_VARWRITE);
    INIT_TAG(ELEM_VARFEEDBACK);
    INIT_TAG(ELEM_CONNECTOR);
    INIT_TAG(ELEM_CONTINUATION);

    INIT_TAG(ELEM_COIL);
    INIT_TAG(ELEM_CONTACT);
    INIT_TAG(ELEM_POWERRAIL);

    INIT_TAG(ELEM_FNC);
    INIT_TAG(ELEM_FB);
    INIT_TAG(ELEM_PIN);
    INIT_TAG(ELEM_PINLIST);

    INIT_TAG(ELEM_LABEL);
    INIT_TAG(ELEM_JUMP);
    INIT_TAG(ELEM_RETURN);

    INIT_TAG(ELEM_STEP);
    INIT_TAG(ELEM_TRANS);
    INIT_TAG(ELEM_ALTDIV);         
    INIT_TAG(ELEM_SIMDIV);
    INIT_TAG(ELEM_ACTION);
    INIT_TAG(ELEM_ACTBLCK);
    INIT_TAG(ELEM_ACTQUAL);
    INIT_TAG(ELEM_STACTION);

    INIT_TAG(ATTR_ORIGIN);
    INIT_TAG(ATTR_INORIGIN);
    INIT_TAG(ATTR_OUTORIGIN);
    INIT_TAG(ATTR_SIZE);
    INIT_TAG(ATTR_NAME);
    INIT_TAG(ATTR_TYPE);
    INIT_TAG(ATTR_TYPENAME);
    INIT_TAG(ATTR_CLASS);
    INIT_TAG(ATTR_COMMENT);
    INIT_TAG(ATTR_RUBBERLINE);
    INIT_TAG(ATTR_NEGATE);
    INIT_TAG(ATTR_HIDDEN);
    INIT_TAG(ATTR_CENTRE_X);
    INIT_TAG(ATTR_CENTRE_Y);
    INIT_TAG(ATTR_DISABLE);
    INIT_TAG(ATTR_R_EDGE);
    INIT_TAG(ATTR_F_EDGE);
    INIT_TAG(ATTR_FUNC);

    INIT_TAG(ATTR_QUALTYPE);
    INIT_TAG(ATTR_QUALTIME);
    INIT_TAG(ATTR_INITSTEP);

    INIT_TAG(ATTR_VAL_INPUT);
    INIT_TAG(ATTR_VAL_OUTPUT);
    INIT_TAG(ATTR_VAL_INOUTPUT);
    INIT_TAG(ATTR_VAL_INEXT);
    INIT_TAG(ATTR_VAL_OUTEXT);
#undef INIT_TAG
}
static void Release_TAGS()
{
#define FREE_TAG(tag)  SysFreeString(XML_TAGS.tag)
    FREE_TAG(ELEM_HEADER);
    FREE_TAG(ELEM_BODY);
    FREE_TAG(ELEM_LINE);
    
    FREE_TAG(ELEM_VARREAD);
    FREE_TAG(ELEM_VARWRITE);
    FREE_TAG(ELEM_VARFEEDBACK);
    FREE_TAG(ELEM_CONNECTOR);
    FREE_TAG(ELEM_CONTINUATION);

    FREE_TAG(ELEM_COIL);
    FREE_TAG(ELEM_CONTACT);
    FREE_TAG(ELEM_POWERRAIL);

    FREE_TAG(ELEM_FNC);
    FREE_TAG(ELEM_FB);
    FREE_TAG(ELEM_PIN);
    FREE_TAG(ELEM_PINLIST);

    FREE_TAG(ELEM_LABEL);
    FREE_TAG(ELEM_JUMP);
    FREE_TAG(ELEM_RETURN);

    FREE_TAG(ELEM_STEP);
    FREE_TAG(ELEM_TRANS);
    FREE_TAG(ELEM_ALTDIV);         
    FREE_TAG(ELEM_SIMDIV);
    FREE_TAG(ELEM_ACTION);
    FREE_TAG(ELEM_ACTBLCK);
    FREE_TAG(ELEM_ACTQUAL);
    FREE_TAG(ELEM_STACTION);

    FREE_TAG(ATTR_ORIGIN);
    FREE_TAG(ATTR_INORIGIN);
    FREE_TAG(ATTR_OUTORIGIN);
    FREE_TAG(ATTR_SIZE);
    FREE_TAG(ATTR_NAME);
    FREE_TAG(ATTR_TYPE);
    FREE_TAG(ATTR_TYPENAME);
    FREE_TAG(ATTR_CLASS);
    FREE_TAG(ATTR_COMMENT);
    FREE_TAG(ATTR_RUBBERLINE);
    FREE_TAG(ATTR_NEGATE);
    FREE_TAG(ATTR_HIDDEN);
    FREE_TAG(ATTR_CENTRE_X);
    FREE_TAG(ATTR_CENTRE_Y);
    FREE_TAG(ATTR_DISABLE);
    FREE_TAG(ATTR_R_EDGE);
    FREE_TAG(ATTR_F_EDGE);
    FREE_TAG(ATTR_FUNC);

    FREE_TAG(ATTR_QUALTYPE);
    FREE_TAG(ATTR_QUALTIME);
    FREE_TAG(ATTR_INITSTEP);

    FREE_TAG(ATTR_VAL_INPUT);
    FREE_TAG(ATTR_VAL_OUTPUT);
    FREE_TAG(ATTR_VAL_INOUTPUT);
    FREE_TAG(ATTR_VAL_INEXT);
    FREE_TAG(ATTR_VAL_OUTEXT);
#undef FREE_TAG
}


static bool LoadFileToIStream(//HACK 
    CGT_CDiagMsg*  pdm,
    CG_CIECFront*  pFront,
    const TCHAR*   pszFileLocator, 
    IStream**      pIStream,
    __int64*       pftLastWrite
)
{
    DWORD                       dwSize;
    TCHAR*                      pFileContent;
    BYTE*                       pFileContentMH;
    HGLOBAL                     memHandle;

    *pIStream = NULL;

    if(!CGT_LoadLocatorFileToMem(pdm, NULL, pFront->m_myCOMIF, pszFileLocator, &pFileContent, &dwSize, pftLastWrite))
        return false;

    memHandle = GlobalAlloc(GMEM_MOVEABLE, dwSize);
    pFileContentMH = (BYTE*)GlobalLock(memHandle);
    memcpy(pFileContentMH, pFileContent, dwSize);
    delete(pFileContent);
    GlobalUnlock(memHandle);

    HRESULT hr = CreateStreamOnHGlobal(
                memHandle,   //Memory handle for the stream object
                true,        //Whether to free memory when the object is released
                pIStream);   //Address of output variable that 
                             // receives the IStream interface pointer
    
    if(SUCCEEDED(hr) && *pIStream)
    {
        ULARGE_INTEGER uli;
        LARGE_INTEGER  u0={0,0};

        uli.LowPart = dwSize;
        uli.HighPart = 0;

        if(SUCCEEDED(hr = (*pIStream)->SetSize(uli)))
            hr = (*pIStream)->Seek(u0, STREAM_SEEK_SET, NULL);

        if(FAILED(hr))
        {
            (*pIStream)->Release();
            *pIStream = NULL;
        }
    }

    if(FAILED(hr))
    {
        pdm->msg1(CG_E_FILE_READ_ERROR, NULL, pszFileLocator);
        *pIStream = NULL;
        return false;
    }

    return true;
}




static cgMSXML::IXMLDocument* XML_GetDocument(
    CGT_CDiagMsg* pdm,
    CG_CIECFront* pFront,
    const TCHAR*  pszXMLFile, 
    __int64*      pOnOpenLastWriteFileTime
)
{
    cgMSXML::IXMLDocument           *pDoc = NULL;
    IStream                *pStm = NULL;
    IPersistStreamInit     *pPSI = NULL;
    HRESULT                hr;

 
    
    // Create an empty XML document.
    hr = CoCreateInstance(cgMSXML::CLSID_XMLDocument, NULL, CLSCTX_INPROC_SERVER,
                                cgMSXML::IID_IXMLDocument, (void**)&pDoc);
    if(FAILED(hr) || !pDoc) 
        goto fatalError;

    //try to open the disk or lib file:
    if(!LoadFileToIStream(pdm, pFront, pszXMLFile, &pStm, pOnOpenLastWriteFileTime))
        return NULL;
    assert(pStm);

    
    // Get the IPersistStreamInit interface to the XML doc.
    hr = pDoc->QueryInterface(IID_IPersistStreamInit, (void **)&pPSI);
    if(FAILED(hr) || !pPSI)
        goto fatalError;

    // Init the XML doc from the stream.
    hr = pPSI->Load(pStm);
    if(FAILED(hr))
    {
        // Print error information !
        cgMSXML::IXMLError *pXMLError = NULL ;
        cgMSXML::_xml_error xmle;

        hr = pPSI->QueryInterface(cgMSXML::IID_IXMLError, (void **)&pXMLError);
        if(FAILED(hr) || !pXMLError)
            goto fatalError;

        hr = pXMLError->GetErrorInfo(&xmle);
        if(FAILED(hr))
            goto fatalError;
        pXMLError->Release();

        //different error message formats
        //if found string is ??? xmle._pszExpected seem to be a 
        //preformated message ready to dump
        if(!wcscmp(xmle._pszFound, L"???"))
        {
            pdm->msg3(CG_E_XML_PARSE_ERROR, NULL, pszXMLFile, 
                pdm->jot1()<<((int)xmle._nLine), pdm->jot2()<<xmle._pszExpected);
        }
        else
        {
            pdm->msg3(CG_E_XML_PARSE_ERROR, NULL, pszXMLFile, 
                pdm->jot1()<<((int)xmle._nLine), pdm->jot2()<<xmle._pszFound);
        }

        SysFreeString(xmle._pszFound);
        SysFreeString(xmle._pszExpected);
        SysFreeString(xmle._pchBuf);

        pDoc->Release();
        pDoc = NULL;
    }

    // Release any used interfaces.
    if(pPSI)
        pPSI->Release();
    if(pStm)
        pStm->Release();

    return pDoc;

fatalError:
    pdm->sysErr(hr, NULL, _T("CLSID_XMLDocument for file: "), pszXMLFile);
    return NULL;

}
static bool XML_GetStrAttrTrimmed(cgMSXML::IXMLElement* pElem, BSTR pszAttr, FC_CStr* pStringToLoad)
{
    VARIANT         val;
    wchar_t*        pszWchar;
    int             nMCharsNeeded;

    VariantInit(&val);
    if(FAILED(pElem->getAttribute(pszAttr, &val)) || val.vt!=VT_BSTR)
        return false;

    pszWchar = val.bstrVal;

    while(_istspace(*pszWchar))
        pszWchar++;

    nMCharsNeeded = wcslen(pszWchar);

    while(nMCharsNeeded>0 && _istspace(pszWchar[nMCharsNeeded-1]))
    {
        nMCharsNeeded--;
        pszWchar[nMCharsNeeded] = 0;
    }
    pStringToLoad->load(pszWchar);

    VariantClear(&val);
    return true;
}

static cgMSXML::IXMLElement* XML_GetChildWithName(const wchar_t* pszElemTypeName, cgMSXML::IXMLElement* pElem)
{
    bool                    bOk;
    VARIANT                 var1;
    VARIANT                 var2;
    IDispatch*              pDisp;
    cgMSXML::IXMLElement*            pChild = NULL;
    long                    aLong       = -1;
    cgMSXML::IXMLElementCollection*  pChildren   = NULL;

    
    assert(pElem);
    
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
        bOk = SUCCEEDED(pDisp->QueryInterface(cgMSXML::IID_IXMLElement, (void **)&pChild));
        pDisp->Release();
    }

    
    return pChild;
}




                                
                                
static BOX_TYPE XML_GetBoxTypeEnumFromBSTR(const BSTR bstr)
{
    if(!wcscmp(XML_TAGS.ELEM_VARREAD, bstr))       return BOX_TYPE_VARREAD;
    if(!wcscmp(XML_TAGS.ELEM_VARWRITE, bstr))      return BOX_TYPE_VARWRITE;
    if(!wcscmp(XML_TAGS.ELEM_VARFEEDBACK, bstr))   return BOX_TYPE_VARFEEDBACK;
    if(!wcscmp(XML_TAGS.ELEM_CONNECTOR, bstr))     return BOX_TYPE_CONNECTOR;
    if(!wcscmp(XML_TAGS.ELEM_CONTINUATION, bstr))  return BOX_TYPE_CONTINUATION;
    if(!wcscmp(XML_TAGS.ELEM_COIL, bstr))          return BOX_TYPE_COIL;
    if(!wcscmp(XML_TAGS.ELEM_CONTACT, bstr))       return BOX_TYPE_CONTACT;
    if(!wcscmp(XML_TAGS.ELEM_FNC, bstr))           return BOX_TYPE_FNC;
    if(!wcscmp(XML_TAGS.ELEM_FB, bstr))            return BOX_TYPE_FB;
    if(!wcscmp(XML_TAGS.ELEM_LABEL, bstr))         return BOX_TYPE_LABEL;
    if(!wcscmp(XML_TAGS.ELEM_JUMP, bstr))          return BOX_TYPE_JUMP;
    if(!wcscmp(XML_TAGS.ELEM_RETURN, bstr))        return BOX_TYPE_RETURN;
    return BOX_TYPE_UNDEF;
}
static bool XML_GetFlag(cgMSXML::IXMLElement* pChild, wchar_t* pszBoolAttr)
{
    VARIANT val;
    bool    bResult = false;

    VariantInit(&val);

    if(SUCCEEDED(pChild->getAttribute(pszBoolAttr, &val)))
    {
        assert(val.vt==VT_EMPTY || val.vt==VT_BSTR);

        if(val.vt==VT_BSTR)
        {
            if(!wcscmp(L"1", val.bstrVal))
            {
                bResult = true;
            }
            else if(!wcscmp(L"0", val.bstrVal))
            {
                bResult = false;
            }
            else
            {
                assert(!"XML_GetFlag: value string is neither 0 nor 1");
                bResult = false;
            }
            VariantClear(&val);
        }
    }
    
    return bResult;
}
static cgMSXML::IXMLElement* XML_GetNextElement(cgMSXML::IXMLElementCollection* pChildren, long* pNum)
{
    bool                    bOk;
    long                    typeId;
    VARIANT                 var1;
    VARIANT                 var2;
    IDispatch*              pDisp;
    long                    length;
    cgMSXML::IXMLElement*            pChild;
    
    
    pChildren->get_length(&length);
    var1.vt  = VT_I4;
    var2.vt  = VT_EMPTY;
    var1.lVal= *pNum;


nextChild:
    if(var1.lVal>= length)
        return NULL;


    if(FAILED(pChildren->item(var1, var2, &pDisp)) || !pDisp)
    {
        assert(!"pChildren->item failed");
        return NULL;
    }
    
    bOk = SUCCEEDED(pDisp->QueryInterface(cgMSXML::IID_IXMLElement, (void **)&pChild));
    pDisp->Release();
    
    if(!bOk)
    {
        assert(!"pDisp->QueryInterface failed");
        return NULL;
    }

    if(FAILED(pChild->get_type(&typeId)))
    {
        assert(!"pChild->get_type failed");
        pChild->Release();
        return NULL;
    }
    if(typeId!=cgMSXML::XMLELEMTYPE_ELEMENT || XML_GetFlag(pChild, XML_TAGS.ATTR_COMMENT))
    {
        pChild->Release();
        var1.lVal++;
        goto nextChild;
    }

    
    *pNum = ++var1.lVal;
    return pChild;
}
static bool XML_GetTupel(cgMSXML::IXMLElement* pElem, const wchar_t* pszAttrName, long* pa, long* pb)
{
    VARIANT     val;
    wchar_t*    pszEnd; 
    bool        bOk = true;
    long         a;
    long         b;

    VariantInit(&val);

    if(FAILED(pElem->getAttribute((wchar_t*)/*cast ok?*/pszAttrName, &val)) || val.vt!=VT_BSTR)
        return false;

    a = wcstol(val.bstrVal, &pszEnd, 10);
    if(pa)
        *pa = a;

    while(iswspace(*pszEnd))
        pszEnd++;

    if(*pszEnd == L';')
        pszEnd++;
    else
        bOk = false;

    b = wcstol(pszEnd, &pszEnd, 10);
    if(pb)
        *pb = b;

    while(iswspace(*pszEnd))
        pszEnd++;

    if(*pszEnd)
        bOk = false;

    VariantClear(&val);
    return bOk;
}
static bool XML_ReadLine(cgMSXML::IXMLElement* pLineElem, SFC_LDFBD_SignalHash* pSFCGraphicElements)
{
    bool         bOk;
    LineElement* pLine;
    long         x,y,w,h;



    bOk = XML_GetTupel(pLineElem, XML_TAGS.ATTR_ORIGIN, &x,&y) && 
          XML_GetTupel(pLineElem, XML_TAGS.ATTR_SIZE,   &w, &h);

    if(bOk)
    {
        pLine = new LineElement();

        // normalize...
        if (w<0 || (w==0 && h<0)) {
            x = x + w;
            y = y + h;
            w = -w;
            h = -h;
        }

        pLine->beg.x = x;
        pLine->beg.y = y;


        //calculate end position of line from origin and size:
        pLine->end.x = x + w;
        pLine->end.y = y + h;


        pLine->flags = 0;

        if(XML_GetFlag(pLineElem, XML_TAGS.ATTR_RUBBERLINE))
            pLine->flags |= LINE_FLAG_RUBBERLINE;

        if (w == 0)
            pLine->flags |= LINE_FLAG_VERTICAL;

        assert(x>=0 && y>=0 && x+w>=0 && y+h>=0);

        //calculate extension of line (minimal sourrounding rect:
        GetExtensionFrom2Pos(pLine->beg, pLine->end, &pLine->ext);

        pSFCGraphicElements->addLineElement(pLine);
    }

    return bOk;
}

static bool XML_ReadPowerrail(cgMSXML::IXMLElement* pLineElem, SFC_LDFBD_SignalHash* pSFCGraphicElements)
{
    bool         bOk;
    LineElement* pLine;
    long         x,y,w,h;



    bOk = XML_GetTupel(pLineElem, XML_TAGS.ATTR_ORIGIN, &x,&y) && 
          XML_GetTupel(pLineElem, XML_TAGS.ATTR_SIZE,   &w, &h);

    if(bOk)
    {
        assert(x>=0 && y>=0 && w==0 && y+h>=0);
        if (h<0) {
            y = y+h;
            h = -h;
        }
        for(;h>=0;y++,h--) {
            pLine = new LineElement();

            pLine->beg.x = x;
            pLine->beg.y = y;

            pLine->end = pLine->beg;

            pLine->flags = LINE_FLAG_POWERRAIL;
        
            //calculate extension of line (minimal sourrounding rect:
            GetExtensionFrom2Pos(pLine->beg, pLine->end, &pLine->ext);

            pSFCGraphicElements->addLineElement(pLine);
        }
    }

    return bOk;
}


static bool XML_ReadSimpleBox(cgMSXML::IXMLElement* pVarElem, 
                              BOX_TYPE boxType, 
                              SFC_LDFBD_SignalHash* pSFCGraphicElements)
{
    bool           bOk;
    BoxElement*    pBox;
    PinElement*    pPin;  
    long           x,y,w,h;
    bool           bIn;
    bool           bOut;


    //create box & append:
    pBox = new BoxElement();

    pBox->boxType = boxType;
    pBox->boxFlags = BOX_FLAG_NORMAL;

    //get the extend of the box:
    bOk = XML_GetTupel(pVarElem,   XML_TAGS.ATTR_ORIGIN, &x, &y) && 
          XML_GetTupel(pVarElem,   XML_TAGS.ATTR_SIZE,   &w, &h);
    if(bOk)
    {
        assert(x>=0 && y>=0 && w>=0 && h>=0);
        pBox->ext.left   = x;
        pBox->ext.top    = y;
        pBox->ext.right  = x + w;
        pBox->ext.bottom = y + h;
        pBox->sfcError = false;

        //get text, if not a RETURN box:
        if(boxType!=BOX_TYPE_RETURN)
            bOk = XML_GetStrAttrTrimmed(pVarElem, XML_TAGS.ATTR_NAME,   &pBox->text);

        if(boxType==BOX_TYPE_COIL || boxType==BOX_TYPE_CONTACT)
        {
            FC_CStr func;
            if (XML_GetStrAttrTrimmed(pVarElem, XML_TAGS.ATTR_FUNC, &func))
            {
                if (!stricmp(func, XML_4GR_ATTR_VAL_NORMAL))
                {
                    pBox->boxFlags |= BOX_FLAG_NORMAL;
                }
                else if (!stricmp(func, XML_4GR_ATTR_VAL_CLOSED) ||
                         !stricmp(func, XML_4GR_ATTR_VAL_NEGATIVE))
                {
                    pBox->boxFlags |= BOX_FLAG_NEGATE;
                }
                else if (!stricmp(func, XML_4GR_ATTR_VAL_POSTRANS))
                {
                    pBox->boxFlags |= BOX_FLAG_R_EDGE;
                }
                else if (!stricmp(func, XML_4GR_ATTR_VAL_NEGTRANS))
                {
                    pBox->boxFlags |= BOX_FLAG_F_EDGE;
                }
                else if (!stricmp(func, XML_4GR_ATTR_VAL_SET))
                {
                    pBox->boxFlags |= BOX_FLAG_SET;
                    assert(boxType==BOX_TYPE_COIL);
                }
                else if (!stricmp(func, XML_4GR_ATTR_VAL_RESET))
                {
                    pBox->boxFlags |= BOX_FLAG_RESET;
                    assert(boxType==BOX_TYPE_COIL);
                }
                else
                    assert(!"unknown function type");
            }
        }
    }

    if(!bOk)
    {
        delete(pBox);
        return false;
    }

    switch(boxType)
    {
    case BOX_TYPE_VARREAD:      bIn=false; bOut=true;  break;
    case BOX_TYPE_VARWRITE:     bIn=true;  bOut=false; break;
    case BOX_TYPE_VARFEEDBACK:  bIn=true;  bOut=true;  break;
    case BOX_TYPE_LABEL:        bIn=false; bOut=false; break;
    case BOX_TYPE_JUMP:         bIn=true;  bOut=false; break;  
    case BOX_TYPE_RETURN:       bIn=true;  bOut=false; break;
    case BOX_TYPE_COIL:         bIn=true;  bOut=true;  break;
    case BOX_TYPE_CONTACT:      bIn=true;  bOut=true;  break;
    default:
        assert(!"illegal enum");
    }

    if(bIn)
    {
        pPin = new PinElement();
        InitPinElement(pPin);

        pPin->lPinType = PIN_TYPE_IN;
        bOk = XML_GetTupel(pVarElem, XML_TAGS.ATTR_INORIGIN, &pPin->pos.x, &pPin->pos.y);

        pSFCGraphicElements->addPinElement(pPin);
        pBox->pins.add(pPin);
    }
    if(bOk && bOut)
    {
        pPin = new PinElement();
        InitPinElement(pPin);

        pPin->lPinType = PIN_TYPE_OUT;
        bOk = XML_GetTupel(pVarElem, XML_TAGS.ATTR_OUTORIGIN, &pPin->pos.x, &pPin->pos.y);
    
        pSFCGraphicElements->addPinElement(pPin);
        pBox->pins.add(pPin);
    }

    //check that there is no misused negate qualifier
    //there might be an editor thinking 'this is a good idea'
    assert(!XML_GetFlag(pVarElem, XML_TAGS.ATTR_NEGATE));

    if(bOk)
            pSFCGraphicElements->addBoxElement(pBox);
    else
        delete(pBox);
    return bOk;
}

static bool XML_ReadConnPair(cgMSXML::IXMLElement*      pConPairElem, 
                             BOX_TYPE          boxType, 
                             SFC_LDFBD_SignalHash* pSFCGraphicElements)
{
    bool             bOk;
    ConnPairElement* pConnPair;

    assert(boxType==BOX_TYPE_CONTINUATION || boxType==BOX_TYPE_CONNECTOR);

    //create box:
    pConnPair = new ConnPairElement();
    pConnPair->boxType = boxType;

    //extend of the box is not taken into accout for connectors/continuations
    bOk = XML_GetTupel(pConPairElem, XML_TAGS.ATTR_ORIGIN, &pConnPair->origin.x, &pConnPair->origin.y);
    
    if(bOk)
        bOk = XML_GetStrAttrTrimmed(pConPairElem, XML_TAGS.ATTR_NAME,   &pConnPair->text);

    if(bOk)
    {
        if(boxType==BOX_TYPE_CONNECTOR)
            bOk = XML_GetTupel(pConPairElem, XML_TAGS.ATTR_INORIGIN, &pConnPair->pinpos.x, &pConnPair->pinpos.y);
        else
            bOk = XML_GetTupel(pConPairElem, XML_TAGS.ATTR_OUTORIGIN, &pConnPair->pinpos.x, &pConnPair->pinpos.y);
    }
    if(bOk)
        pSFCGraphicElements->addConnectorElement(pConnPair);
    else
        delete(pConnPair);

    return bOk;
}


static long XML_GetPinType(cgMSXML::IXMLElement* pPinElem)
{
    VARIANT     val;
    long        pt;   
    
    VariantInit(&val);
    if(FAILED(pPinElem->getAttribute(XML_TAGS.ATTR_CLASS, &val)) || val.vt!=VT_BSTR)
        return PIN_TYPE_UNDEF;

    
         if(!wcsicmp(XML_TAGS.ATTR_VAL_INPUT,   val.bstrVal)) pt= PIN_TYPE_IN;
    else if(!wcsicmp(XML_TAGS.ATTR_VAL_OUTPUT,  val.bstrVal)) pt= PIN_TYPE_OUT;
    else if(!wcsicmp(XML_TAGS.ATTR_VAL_INOUTPUT,val.bstrVal)) pt= PIN_TYPE_INOUT;
    else if(!wcsicmp(XML_TAGS.ATTR_VAL_INEXT,   val.bstrVal)) pt= PIN_TYPE_INEXT;
    else if(!wcsicmp(XML_TAGS.ATTR_VAL_OUTEXT,  val.bstrVal)) pt= PIN_TYPE_OUTEXT;
    else pt = PIN_TYPE_UNDEF;

    VariantClear(&val);

    return pt;
}
static bool XML_ReadPin(cgMSXML::IXMLElement* pPinElem, 
                        BoxElement* pBox,
                        SFC_LDFBD_SignalHash* pSFCGraphicElements)
{
    bool            bOk;
    PinElement*     pPin;
   

    pPin = new PinElement();
    InitPinElement(pPin);

	//pin name is optional, don't check the return type:
	XML_GetStrAttrTrimmed(pPinElem, XML_TAGS.ATTR_NAME, &pPin->szName);
    
	pPin->lPinType = XML_GetPinType(pPinElem);
	bOk = pPin->lPinType!=PIN_TYPE_UNDEF; 

	if(bOk)
		bOk = XML_GetTupel(pPinElem,   XML_TAGS.ATTR_ORIGIN,  &pPin->pos.x, &pPin->pos.y);

    if(bOk)
    {
        //type is opt, ignore return value:
        XML_GetStrAttrTrimmed(pPinElem, XML_TAGS.ATTR_TYPE,    &pPin->szType);

        //refine the lPinType:
        //a) distinguish between inout_in and inout_out
        //b) in-pins with name EN/ENO get the PIN_TYPE_EN/ENO pin type
        if(pPin->lPinType==PIN_TYPE_INOUT)
        {
            if(pBox->ext.left==pPin->pos.x)
                pPin->lPinType = PIN_TYPE_INOUT_IN;
            else if(pBox->ext.right==pPin->pos.x)
                pPin->lPinType = PIN_TYPE_INOUT_OUT;
            else
            {
                assert(!"in?out pin");
            }
        }
        else if(pPin->lPinType==PIN_TYPE_IN)
        {
            if(!_tcsicmp(pPin->szName, _T("EN")))
                pPin->lPinType = PIN_TYPE_EN;
        }
        else if(pPin->lPinType==PIN_TYPE_OUT)
        {
            if(!_tcsicmp(pPin->szName, _T("ENO")))
                pPin->lPinType = PIN_TYPE_ENO;
        }



        if(XML_GetFlag(pPinElem, XML_TAGS.ATTR_R_EDGE))
            pPin->lPinFlags |= PIN_FLAG_R_EDGE;

        if(XML_GetFlag(pPinElem, XML_TAGS.ATTR_F_EDGE))
            pPin->lPinFlags |= PIN_FLAG_F_EDGE;

        if(XML_GetFlag(pPinElem, XML_TAGS.ATTR_NEGATE))
            pPin->lPinFlags |= PIN_FLAG_NEGATE;
    }

    
    pSFCGraphicElements->addPinElement(pPin);
    pBox->pins.add(pPin);

    return bOk;
}
static bool XML_ReadPinList(cgMSXML::IXMLElement* pPinList, 
                            BoxElement* pBox,
                            SFC_LDFBD_SignalHash* pSFCGraphicElements)
{
    bool                    bPin;
    bool                    bError = false;
    long                    lPos;
    BSTR                    bstr;
    cgMSXML::IXMLElement*            pPin;
    cgMSXML::IXMLElementCollection*  pChildren   = NULL;
    
    
    if(FAILED(pPinList->get_children(&pChildren)))
        return false;
    
    if(!pChildren)//empty list returns NULL, no error...
        return true;

    for(lPos=0; (pPin = XML_GetNextElement(pChildren, &lPos))!=NULL; )
    {
        if(FAILED(pPin->get_tagName(&bstr)))
        {
            assert(!"pPin->get_tagName failed");
            pPin->Release();
            bError = true;
            break;
        }
        bPin = !wcscmp(XML_TAGS.ELEM_PIN, bstr);
        SysFreeString(bstr);
        
        if(bPin && !XML_ReadPin(pPin, pBox, pSFCGraphicElements))
        {
            pPin->Release();
            bError = true;
            break;
        }
        pPin->Release();
    }

    pChildren->Release();
    return !bError;
}
static bool XML_ReadFunFBBox(cgMSXML::IXMLElement* pFunFBElem, 
                             BOX_TYPE boxType, 
                             SFC_LDFBD_SignalHash* pSFCGraphicElements)
{
    bool         bOk = true;
    BoxElement*  pBox;
    cgMSXML::IXMLElement* pPinList;
    long         x,y,w,h;

    
    pBox = new BoxElement();
    pBox->boxType = boxType;
    pBox->boxFlags = BOX_FLAG_NORMAL;

    pSFCGraphicElements->addBoxElement(pBox);

    bOk = XML_GetStrAttrTrimmed(pFunFBElem, XML_TAGS.ATTR_TYPENAME, &pBox->text) &&
          XML_GetTupel(pFunFBElem,   XML_TAGS.ATTR_ORIGIN,   &x, &y) && 
          XML_GetTupel(pFunFBElem,   XML_TAGS.ATTR_SIZE,     &w, &h);

    if(bOk && boxType==BOX_TYPE_FB)
        bOk = XML_GetStrAttrTrimmed(pFunFBElem, XML_TAGS.ATTR_NAME, &pBox->instName);

    if(bOk)
    {
        //get the extend of the box:
        assert(x>=0 && y>=0 && w>=0 && h>=0);
        pBox->ext.left   = x;
        pBox->ext.top    = y;
        pBox->ext.right  = x + w;
        pBox->ext.bottom = y + h;
        pBox->sfcError = false;

        pPinList = XML_GetChildWithName(XML_TAGS.ELEM_PINLIST, pFunFBElem);
        if(pPinList)
        {
            bOk = XML_ReadPinList(pPinList, pBox, pSFCGraphicElements);
            pPinList->Release();
        }
        else
            bOk = false;
    }
   
    return bOk;
}


// sfc elements
static bool XML_ReadStep(cgMSXML::IXMLElement* pStepElem, SFC_LDFBD_SignalHash* pSFCGraphicElements)
{
    bool         bOk = true;
    SFCStepElement* pStep;
    cgMSXML::IXMLElement* pPinList;
    long         x,y,w,h;

    pStep = new SFCStepElement();
    pSFCGraphicElements->addStepElement(pStep);

    bOk = XML_GetStrAttrTrimmed(pStepElem, XML_TAGS.ATTR_NAME, &pStep->box.text) &&
          XML_GetTupel(pStepElem,   XML_TAGS.ATTR_ORIGIN,   &x, &y) && 
          XML_GetTupel(pStepElem,   XML_TAGS.ATTR_SIZE,     &w, &h);

    if(bOk)
    {
        //get the extend of the box:
        assert(x>=0 && y>=0 && w>=0 && h>=0);
        pStep->box.ext.left   = x;
        pStep->box.ext.top    = y;
        pStep->box.ext.right  = x + w;
        pStep->box.ext.bottom = y + h;
        pStep->box.sfcError = false;

        pStep->initStep = XML_GetFlag(pStepElem, XML_TAGS.ATTR_INITSTEP);

        pPinList = XML_GetChildWithName(XML_TAGS.ELEM_PINLIST, pStepElem);
        if(pPinList)
        {
            bOk = XML_ReadPinList(pPinList, &(pStep->box), pSFCGraphicElements);
            pPinList->Release();
        }
        else
            bOk = false;
    }
   
    return bOk;

}

static bool XML_ReadTrans(cgMSXML::IXMLElement* pTransElem, SFC_LDFBD_SignalHash* pSFCGraphicElements)
{
    bool         bOk = true;
    SFCTransitionElement* pTrans;
    cgMSXML::IXMLElement* pPinList;
    long         x,y,w,h;

    pTrans = new SFCTransitionElement();
    pSFCGraphicElements->addTransitionElement(pTrans);

    bOk = XML_GetStrAttrTrimmed(pTransElem, XML_TAGS.ATTR_NAME, &pTrans->box.text) &&
          XML_GetTupel(pTransElem,   XML_TAGS.ATTR_ORIGIN,   &x, &y) && 
          XML_GetTupel(pTransElem,   XML_TAGS.ATTR_SIZE,     &w, &h);

    if(bOk)
    {
        //get the extend of the box:
        assert(x>=0 && y>=0 && w>=0 && h>=0);
        pTrans->box.ext.left   = x;
        pTrans->box.ext.top    = y;
        pTrans->box.ext.right  = x + w;
        pTrans->box.ext.bottom = y + h;
        pTrans->box.sfcError = false;

        pTrans->priority = 0;

        pPinList = XML_GetChildWithName(XML_TAGS.ELEM_PINLIST, pTransElem);
        if(pPinList)
        {
            bOk = XML_ReadPinList(pPinList, &(pTrans->box), pSFCGraphicElements);
            pPinList->Release();
        }
        else
            bOk = false;
    }
   
    return bOk;
}

static bool XML_ReadSTAction(cgMSXML::IXMLElement* pSTActionElem, TCHAR** pText)
{
    bool                    bRet;
    HRESULT                 hr;
    BSTR                    bstrSTAction;
    BSTR                    bstrSTActionDecoded;
    int                     iLenHdr;

    *pText = NULL;
      
    //read the st code:
    bstrSTAction = NULL;
    bstrSTActionDecoded = NULL;

    bRet = SUCCEEDED(pSTActionElem->get_text(&bstrSTAction));
    pSTActionElem->Release();
   
    if(!bRet)
    {
        *pText = new TCHAR[1];
        (*pText)[0] = 0;    
        return false;
    }

    if(bstrSTAction)
    {
        bstrSTActionDecoded = NULL;
        hr = UTIL_XMLDecodeString(bstrSTAction, &bstrSTActionDecoded, XML_ENCODE_DEFAULT);
        assert(SUCCEEDED(hr) && bstrSTActionDecoded);
        SysFreeString(bstrSTAction);
        bstrSTAction= NULL;
    }

    if (!bstrSTActionDecoded)
    {
        *pText = new TCHAR[2];
        (*pText)[0] = 0;    
        return false;
    }
        
#ifdef UNICODE
    iLenHdr = wcslen(bstrSTActionDecoded);
    *pText= new TCHAR[iLenHdr+1];
    _tcscpy(*pText, bstrSTActionDecoded);
#else
    //convert bstr into a TCHAR*
    iLenHdr = wcslen(bstrSTActionDecoded);
    *pText= new TCHAR[iLenHdr*2+1];
    FC_StringToMultiByte(bstrSTActionDecoded, *pText, iLenHdr*2+1);
#endif
    SysFreeString(bstrSTActionDecoded);
    bstrSTActionDecoded = NULL;

    return bRet;
}



static bool XML_ReadAction(cgMSXML::IXMLElement* pActionElem, SFC_LDFBD_SignalHash* pSFCGraphicElements)
{
    bool         bOk = true;
    SFCActionElement* pAction;
    long         x,y,w,h;
    cgMSXML::IXMLElement* pSTActionElem;

    pAction = new SFCActionElement();
    pSFCGraphicElements->addActionElement(pAction);

    bOk = XML_GetStrAttrTrimmed(pActionElem, XML_TAGS.ATTR_NAME, &pAction->box.text) &&
          XML_GetTupel(pActionElem,   XML_TAGS.ATTR_ORIGIN,   &x, &y) && 
          XML_GetTupel(pActionElem,   XML_TAGS.ATTR_SIZE,     &w, &h);

    if(bOk)
    {
        //get the extend of the box:
        assert(x>=0 && y>=0 && w>=0 && h>=0);
        pAction->box.ext.left   = x;
        pAction->box.ext.top    = y;
        pAction->box.ext.right  = x + w;
        pAction->box.ext.bottom = y + h;
        pAction->box.sfcError = false;
        pAction->isSTAction = false;
        pAction->ppszSTAction = NULL;

        pSTActionElem = XML_GetChildWithName(XML_TAGS.ELEM_STACTION, pActionElem);
        if (pSTActionElem)
        {
            pAction->isSTAction = true;
            TCHAR* pText;
            XML_ReadSTAction(pSTActionElem, &pText);

            pAction->ppszSTAction = pText;
            
        }
    }
   
    return bOk;
}

static bool XML_ReadActBlk(cgMSXML::IXMLElement* pActBlkElem, SFC_LDFBD_SignalHash* pSFCGraphicElements)
{
    bool         bOk = true;
    SFCActionBlockElement* pActBlk;
    cgMSXML::IXMLElement* pPinList;
    cgMSXML::IXMLElement* pActQual;
    long         x,y,w,h;
    cgMSXML::IXMLElement* pSTActionElem;

    pActBlk = new SFCActionBlockElement();
    pSFCGraphicElements->addActionBlockElement(pActBlk);

    bOk = XML_GetStrAttrTrimmed(pActBlkElem, XML_TAGS.ATTR_NAME, &pActBlk->box.text) &&
          XML_GetTupel(pActBlkElem,   XML_TAGS.ATTR_ORIGIN,   &x, &y) && 
          XML_GetTupel(pActBlkElem,   XML_TAGS.ATTR_SIZE,     &w, &h);

    if(bOk)
    {
        //get the extend of the box:
        assert(x>=0 && y>=0 && w>=0 && h>=0);
        pActBlk->box.ext.left   = x;
        pActBlk->box.ext.top    = y;
        pActBlk->box.ext.right  = x + w;
        pActBlk->box.ext.bottom = y + h;
        pActBlk->box.sfcError = false;

        if (h>2)
        {
            SFCActionElement* pAction = new SFCActionElement();
            XML_GetStrAttrTrimmed(pActBlkElem, XML_TAGS.ATTR_NAME, 
                                  &pAction->box.text);
            
            pAction->box.ext.left   = x;
            pAction->box.ext.top    = y;
            pAction->box.ext.right  = x + w;
            pAction->box.ext.bottom = y + h;
            pAction->box.sfcError = false;
            pAction->isSTAction = false;
            pAction->ppszSTAction = NULL;

            pSFCGraphicElements->addActionElement(pAction);

            pSTActionElem = XML_GetChildWithName(XML_TAGS.ELEM_STACTION, pActBlkElem);
            if (pSTActionElem)
            {
                pAction->isSTAction = true;
                TCHAR* pText;
                XML_ReadSTAction(pSTActionElem, &pText);

                pAction->ppszSTAction = pText;
                
            }
        }

        pPinList = XML_GetChildWithName(XML_TAGS.ELEM_PINLIST, pActBlkElem);
        if(pPinList)
        {
            bOk = XML_ReadPinList(pPinList, &(pActBlk->box), pSFCGraphicElements);
            pPinList->Release();
        }
        else
            bOk = false;
    }

    if(bOk)
    {
        pActQual = XML_GetChildWithName(XML_TAGS.ELEM_ACTQUAL, pActBlkElem);
        if(pActQual)
        {
            VARIANT val;

            VariantInit(&val);

            if(SUCCEEDED(pActQual->getAttribute(XML_TAGS.ATTR_QUALTYPE, &val)))
            {
                assert(val.vt==VT_EMPTY || val.vt==VT_BSTR);

                if(val.vt==VT_BSTR)
                {
                    pActBlk->qualType.load(val.bstrVal);         

                    VariantClear(&val);
                }
            }

            VariantInit(&val);

            if(SUCCEEDED(pActQual->getAttribute(XML_TAGS.ATTR_QUALTIME, &val)))
            {
                assert(val.vt==VT_EMPTY || val.vt==VT_BSTR);

                if(val.vt==VT_BSTR)
                {
                    pActBlk->qualTime.load(val.bstrVal);

                    VariantClear(&val);
                }
            }
    

        }
        else 
            bOk = false;
    }
   
    return bOk;
}

static bool XML_ReadConDiv(cgMSXML::IXMLElement* pConDivElem, SFC_LDFBD_SignalHash* pSFCGraphicElements, bool isSim)
{
    bool         bOk;
    SFCConDivElement* pConDiv;
    long         x,y,w,h;

    

    bOk = XML_GetTupel(pConDivElem, XML_TAGS.ATTR_ORIGIN, &x,&y) && 
          XML_GetTupel(pConDivElem, XML_TAGS.ATTR_SIZE,   &w, &h);
    
    if(bOk)
    {
        pConDiv = new SFCConDivElement();

        if (isSim)
            pConDiv->type = SFC_SIM_CONDIV;
        else
            pConDiv->type = SFC_ALT_CONDIV;

        pConDiv->sfcError = false;

        pConDiv->beg.x = x;
        pConDiv->beg.y = y;
                
        //calculate end position of line from origin and size:
        pConDiv->end.x = x + w;
        pConDiv->end.y = y + h;
        
        assert(x>=0 && y>=0 && x+w>=0 && y+h>=0);

        //calculate extension of line (minimal sourrounding rect:
        GetExtensionFrom2Pos(pConDiv->beg, pConDiv->end, &pConDiv->ext);

        pSFCGraphicElements->addConDivElement(pConDiv);
    }

    return bOk;
}



// sfc elements end




/////////////////////////////////////////////////////////////////////
/**
 * Reads the content of a 4gr LD/FBD body and build up a
 * pSFCGraphicElements. 
 *
 * @param pdm                error message sink
 * @param pszXMLFile input:  4gr file name, used for error messages
 * @param pBodyElem          the body element of the 4gr file
 *                           to a newly created POUContext_C
 * @param pSFCGraphicElements   pointer to signalhash object to be filled
 *                           with body information
 * @return                   true 0 Errors, false at least one error
 */
static bool XML_ReadLDFBDBodyInfo(
    CGT_CDiagMsg*         pdm,
    const TCHAR*          pszXMLFile, 
    cgMSXML::IXMLElement*          pBodyElem, 
    SFC_LDFBD_SignalHash* pSFCGraphicElements
)
{
    bool                    bOk;
    long                    lPos;
    BSTR                    bstr        = NULL;
    BOX_TYPE                boxType;
    cgMSXML::IXMLElement*            pChild      = NULL;
    cgMSXML::IXMLElementCollection*  pChildren   = NULL;

    
    assert(pBodyElem);
    
    if(!pBodyElem || FAILED(pBodyElem->get_children(&pChildren)))
    {
        assert(!"get_children failed");
        return false;
    }
    
    if(!pChildren)
        return true;
   
    for(lPos=0; (pChild = XML_GetNextElement(pChildren, &lPos))!=NULL; )
    {
        if(FAILED(pChild->get_tagName(&bstr)))
        {
            assert(!"pChild->get_tagName failed");
            break;
        }
        if(!wcscmp(XML_TAGS.ELEM_LINE, bstr))
        {
            if(!XML_ReadLine(pChild, pSFCGraphicElements))
                break;
        }
        else if(!wcscmp(XML_TAGS.ELEM_POWERRAIL, bstr))
        {
            if(!XML_ReadPowerrail(pChild, pSFCGraphicElements))
                break;
        }
        // sfc elements
        else if(!wcscmp(XML_TAGS.ELEM_STEP, bstr))
        {
            pSFCGraphicElements->setSFC();
            if(!XML_ReadStep(pChild, pSFCGraphicElements))
                break;
        }
        else if(!wcscmp(XML_TAGS.ELEM_TRANS, bstr))
        {
            pSFCGraphicElements->setSFC();
            if(!XML_ReadTrans(pChild, pSFCGraphicElements))
                break;
        }
        else if(!wcscmp(XML_TAGS.ELEM_ACTION, bstr))
        {
            pSFCGraphicElements->setSFC();
            if(!XML_ReadAction(pChild, pSFCGraphicElements))
                break;
        }
        else if(!wcscmp(XML_TAGS.ELEM_ACTBLCK, bstr))
        {
            pSFCGraphicElements->setSFC();
            if(!XML_ReadActBlk(pChild, pSFCGraphicElements))
                break;
        }
        else if(!wcscmp(XML_TAGS.ELEM_ALTDIV, bstr))
        {
            pSFCGraphicElements->setSFC();
            if(!XML_ReadConDiv(pChild, pSFCGraphicElements, false))
                break;
        }
        else if(!wcscmp(XML_TAGS.ELEM_SIMDIV, bstr))
        {
            pSFCGraphicElements->setSFC();
            if(!XML_ReadConDiv(pChild, pSFCGraphicElements, true))
                break;
        }
        // sfc elements end
        boxType = XML_GetBoxTypeEnumFromBSTR(bstr);
        switch(boxType)
        {
        case BOX_TYPE_VARREAD:
        case BOX_TYPE_VARWRITE:
        case BOX_TYPE_VARFEEDBACK:
        case BOX_TYPE_LABEL:
        case BOX_TYPE_JUMP:
        case BOX_TYPE_RETURN:
        case BOX_TYPE_COIL:
        case BOX_TYPE_CONTACT:
            bOk = XML_ReadSimpleBox(pChild, boxType, pSFCGraphicElements);
        break;
        case BOX_TYPE_FNC:
        case BOX_TYPE_FB:
            bOk = XML_ReadFunFBBox(pChild, boxType, pSFCGraphicElements);
        break;
        case BOX_TYPE_CONNECTOR:
        case BOX_TYPE_CONTINUATION:
            bOk= XML_ReadConnPair(pChild, boxType, pSFCGraphicElements);
        break;
        case BOX_TYPE_UNDEF:
            bOk = true;//we don't care about unknown element types ...
        break;
        default: assert(!"illegal enum");
        }
        if(!bOk)
            break;

        pChild->Release();
        pChild = NULL;
        
        SysFreeString(bstr);
        bstr = NULL;
    }

    
    bOk = pChild==NULL;
    if(!bOk)
    {
        CG_Edipos edpf;
        CG_InitSTEdipos(&edpf, pszXMLFile);
        TCHAR szElemType[256];
        FC_StringToMultiByte((bstr?bstr:XML_TAGS.ELEM_BODY), szElemType, sizeof(szElemType)-1);
        pdm->msg1(CG_E_XML_STRUCTURE_ERROR, &edpf, szElemType);
    }

    pChildren->Release();
    if(pChild)
    {
        pChild->Release();
    }
    if(bstr)
    {
        assert(!bOk);
        SysFreeString(bstr);
    }
    
    return bOk;
}
//------------------ END OF 4GR/XML READ HELPER FUNCTIONS












//+++++++++++++++++++++++++++++++++Entry Function to this cpp +++++++++++++++++++
bool Parse4GRFile(
    CGT_CDiagMsg*         pdm,
    CG_CIECFront*         pFront,
    const TCHAR*          pszXMLFile,
    __int64*              pOnOpenLastWriteFileTime,
    TCHAR**               ppszHeader,
    SFC_LDFBD_SignalHash* pSFCGraphicElements
)
{
    bool                    bRet;
    HRESULT                 hr;
    cgMSXML::IXMLElement*            pRoot = NULL;
    cgMSXML::IXMLElement*            pHdr  = NULL;
    cgMSXML::IXMLElement*            pBody = NULL;
    cgMSXML::IXMLDocument*           pDoc = NULL;
    BSTR                    bstrHeader;
    BSTR                    bstrHeaderDecoded;
    const TCHAR*            pszElemFailed = NULL;
    int                     iLenHdr;

    assert(pszXMLFile && pOnOpenLastWriteFileTime && ppszHeader 
          && pSFCGraphicElements);

    *ppszHeader = NULL;
    *pOnOpenLastWriteFileTime = NULL;
    

    Init_TAGS();
    
    
    pDoc  = XML_GetDocument(pdm, pFront, pszXMLFile, pOnOpenLastWriteFileTime);
    if(!pDoc)
        goto hardError;

    bRet = SUCCEEDED(pDoc->get_root(&pRoot)) && pRoot;
    pDoc->Release();
    if(!bRet)
    {
        pszElemFailed = _T("ROOT");
        goto hardError;
    }

    
    pHdr  = XML_GetChildWithName(XML_TAGS.ELEM_HEADER, pRoot);
    if(!pHdr)
    {
        pszElemFailed = _T("HEADER");
        pRoot->Release();
        goto hardError;
    }
    
    
    //parse the header:
    bstrHeader = NULL;
    bstrHeaderDecoded = NULL;

    bRet = SUCCEEDED(pHdr->get_text(&bstrHeader));
    pHdr->Release();
    if(!bRet)
    {
        pRoot->Release();
        pszElemFailed = _T("HEADER");
        goto hardError;
    }

    if(bstrHeader)
    {
        bstrHeaderDecoded = NULL;
        hr = UTIL_XMLDecodeString(bstrHeader, &bstrHeaderDecoded, XML_ENCODE_DEFAULT);
        assert(SUCCEEDED(hr) && bstrHeaderDecoded);
        SysFreeString(bstrHeader);
        bstrHeader = NULL;   
        
        if(!bstrHeaderDecoded)
        {
            pszElemFailed = _T("HEADER");
            goto hardError;
        }

    #ifdef UNICODE
        iLenHdr = wcslen(bstrHeaderDecoded);
        *ppszHeader = new TCHAR[iLenHdr+1];
        _tcscpy(*ppszHeader, bstrHeaderDecoded);
    #else
        //convert bstr into a TCHAR*
        iLenHdr = wcslen(bstrHeaderDecoded);
        *ppszHeader = new TCHAR[iLenHdr*2+1];
        FC_StringToMultiByte(bstrHeaderDecoded, *ppszHeader, iLenHdr*2+1);
    #endif
        SysFreeString(bstrHeaderDecoded);
        bstrHeaderDecoded = NULL;
    }
    else
    {
        // problem: empty CDATA section
        *ppszHeader = new TCHAR[1];
        (*ppszHeader)[0] = 0;
    }
    

    //get body and read in all graphic info needed for compilation:
    pBody = XML_GetChildWithName(XML_TAGS.ELEM_BODY, pRoot);
    pRoot->Release();
    if(!pBody)
    {
        pszElemFailed = _T("BODY");
        goto hardError;
    }

    
    bRet = XML_ReadLDFBDBodyInfo(pdm, pszXMLFile, pBody, pSFCGraphicElements);
    pBody->Release();
    if(!bRet)
    {
        pszElemFailed = _T("BODY");
        goto hardError;
    }
    Release_TAGS();

    return true;


hardError:
    Release_TAGS();
    if(pszElemFailed)
    {
        CG_Edipos edpf;
        CG_InitSTEdipos(&edpf, pszXMLFile);

        pdm->msg1(CG_E_XML_STRUCTURE_ERROR, &edpf, pszElemFailed);
    }

    delete(*ppszHeader);
    *ppszHeader = NULL;

    assert(pdm->hasErr());
    return false;
}


