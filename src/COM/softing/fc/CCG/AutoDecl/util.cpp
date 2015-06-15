
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include <stdio.h>
#include <errno.h>
#include "fc_tools\fc_defines.h"
#include "util.h"
//----  Static Initializations:   ----------------------------------*


LU_CDiag::set(const TCHAR* pszFmt, ...)
{
    va_list va;
    va_start(va, pszFmt); 
    _vsntprintf(szDetails, MAX_PATH, pszFmt, va);
    va_end(va);
    szDetails[MAX_PATH-1] = 0;
}


BOOL LU_ParseIECDirectAddress(
    LU_CDiag*        pDiag,
    const TCHAR*     pszAddr, 
    LU_IECADDRESS*   pAdr,
    const TCHAR**    ppszEnd
)
{
    int          icpy;
    const TCHAR* psz;
    const TCHAR* pszEnd;
    

    icpy = 0;
    psz = pszAddr;
    pAdr->nFields = 0;
    pAdr->nMods   = 0;

    if(*psz != _T('%'))
    {
        pDiag->set(_T("expected '%'"));
        goto ErrExit;
    }
    pAdr->szSplit[icpy++] = *psz++;

    if(*psz != _T('I') && *psz != _T('Q') && *psz != _T('M'))
    {
        pDiag->set(_T("expected [I|Q|M]"));
        goto ErrExit;
    }
    pAdr->cLoc = *psz;
    pAdr->szSplit[icpy++] = *psz++;

    pAdr->cSize = *psz;
    pAdr->szSplit[icpy++] = *psz++;
    switch(pAdr->cSize)
    {
    case _T('X'):  pAdr->nBits = 1;  break;
    case _T('B'):  pAdr->nBits = 8;  break;
    case _T('W'):  pAdr->nBits = 16; break;
    case _T('D'):  pAdr->nBits = 32; break;
    case _T('L'):  pAdr->nBits = 64; break;
    case _T('S'):  pAdr->nBits = 0;  break;
    default:
        pAdr->cSize = 'X';
        pAdr->nBits = 1;
        icpy--;psz--;
    }

    
    //nummeric fields:
    while(_istdigit(*psz))
    {
        if(pAdr->nFields >= LU_MAX_ADDR_FIELDS)
            goto ErrorToLong;
        
        pAdr->ulFields[pAdr->nFields] = _tcstoul(psz, (TCHAR**)&pszEnd, 10);
        if(errno == ERANGE)
        {
            pDiag->set(_T("integer overflow"));
            goto ErrExit;
        }

        pAdr->pszFields[pAdr->nFields] = &pAdr->szSplit[icpy];
        while(psz < pszEnd)
        {
            if(icpy>=FC_ARRAY_LEN(pAdr->szSplit))
                goto ErrorToLong;
            pAdr->szSplit[icpy++] = *psz++;
        }
        
        pAdr->nFields++;
        if(*psz == _T('.'))
        {
            if(icpy>=FC_ARRAY_LEN(pAdr->szSplit))
                goto ErrorToLong;
            pAdr->szSplit[icpy++] = 0;
            psz++;
        }
    }

    while(*psz == _T('!'))
    {
        if(icpy>=FC_ARRAY_LEN(pAdr->szSplit))
            goto ErrorToLong;
        pAdr->szSplit[icpy++] = 0;
        psz++;

        if(pAdr->nMods >= LU_MAX_ADDR_FIELDS)
            goto ErrorToLong;
        pAdr->pszMods[pAdr->nMods] = &pAdr->szSplit[icpy];

        while(_istalnum(*psz))
        {
            if(icpy>=FC_ARRAY_LEN(pAdr->szSplit))
                goto ErrorToLong;
            pAdr->szSplit[icpy++] = *psz++;
        }
        pAdr->nMods++;
    }

    if(icpy>=FC_ARRAY_LEN(pAdr->szSplit))
        goto ErrorToLong;
    pAdr->szSplit[icpy] = 0;

    if(ppszEnd)
       *ppszEnd = psz;
    return true; 


ErrorToLong:
    pDiag->set(_T("address too long: max chars: %d max fields %d"), 
        FC_ARRAY_LEN(pAdr->szSplit)-1, LU_MAX_ADDR_FIELDS);

ErrExit:
    pAdr->szSplit[0]= 0;
    pAdr->nFields = 0;
    pAdr->nMods   = 0;
    if(ppszEnd)
        *ppszEnd = psz;
    return false; 
}





//HACKy functions to read the compiler kad to get channel info:
//the XML is NOT SIMPLE to parse !!!, there is no cool lib to help me with XML !!!
#include "fc_tools\fc_string.h"
#import "msxml.dll" raw_interfaces_only named_guids rename_namespace("cgMSXML")
using namespace cgMSXML;

static IXMLDOMNode* XMLLoadFile(const TCHAR* pszFile)
{
    HRESULT          hr;
    IXMLDOMElement*  pRoot;
    IXMLDOMDocument* pXMLDoc;

    hr = CoCreateInstance(CLSID_DOMDocument,
	                      NULL,
	                      CLSCTX_INPROC_SERVER,
	                      IID_IXMLDOMDocument,
	     			      (void**)&pXMLDoc);
    	
    if(FAILED(hr))
        return NULL;

	_variant_t   vSrc(pszFile);
	VARIANT_BOOL vBool = VARIANT_FALSE;
    
	hr =  pXMLDoc->load(vSrc, &vBool);
    if(FAILED(hr))
    {
        FC_RELEASE_PTR(pXMLDoc);
        return NULL;
    }
    
    if(vBool!=VARIANT_TRUE)
	{
        FC_RELEASE_PTR(pXMLDoc);
        return NULL;
	}

    pRoot = NULL;
    hr = pXMLDoc->get_documentElement(&pRoot);
    FC_RELEASE_PTR(pXMLDoc);
    if(FAILED(hr) || !pRoot)
        return NULL;

    return pRoot;
}



static bool XMLGetAttrTrim( //return true only if an attribute with name pszwAttr found
    IXMLDOMNode*        pNode, //node where to get attribute from
    const wchar_t*      pszwAttr, //Name of attribute to get
    wchar_t             szAttrVal[MAX_PATH] //buffer for result
)
{
    HRESULT              hr;
    VARIANT              val;
    wchar_t*             pszWchar;
    int                  nLen;
    IXMLDOMNode*         pAttr;
    IXMLDOMNamedNodeMap* pAList;

    szAttrVal[0] = 0;

    pAList = NULL;
    pAttr  = NULL;
    pNode->get_attributes(&pAList);
    if(pAList)
    {
        pAList->getNamedItem((wchar_t*)pszwAttr, &pAttr);
        FC_RELEASE_PTR(pAList);
    }

    if(!pAttr)
        return false;

    VariantInit(&val);
    hr = pAttr->get_nodeValue(&val);
    FC_RELEASE_PTR(pAttr);
    
    if(FAILED(hr) || val.vt!=VT_BSTR)
    {
        VariantClear(&val);
        return false;
    }

    //trim the string:
    pszWchar = val.bstrVal;
    while(_istspace(*pszWchar))
        pszWchar++;

    for(nLen=wcslen(pszWchar);nLen>0 && _istspace(pszWchar[nLen-1]);nLen--)
        pszWchar[nLen-1] = 0;

    wcsncpy(szAttrVal, pszWchar, MAX_PATH);
    szAttrVal[MAX_PATH-1] = 0;

    VariantClear(&val);
    return true;
}



//parse al list of <CHANNEL NUM="<num>"  [I|O|GM|GMR]SEG_OFFS="<hexval>"/>
static IO_COMP_TYP* ReadIOComponents(IXMLDOMNodeList* pNList)
{
    long                  i;
    long                  len;
    wchar_t               szAttrVal[MAX_PATH];
    IXMLDOMNode*          pNode;
    IO_COMP_TYP*          pIOC;
    IO_COMP_TYP*          pIOC_first = NULL;
    IO_COMP_TYP*          pIOC_last  = NULL;
    wchar_t*              psz;

    if(!pNList)
        return NULL;

    pNList->get_length(&len);
    for(i=0;i<len;i++)
    {
        pNList->get_item(i, &pNode);

        pIOC = new IO_COMP_TYP;
        if(pIOC_last)
            pIOC_last->pNext = pIOC;
        if(!pIOC_first)
            pIOC_first = pIOC;
        pIOC_last = pIOC;

        // NUM is not optional
        XMLGetAttrTrim(pNode, L"NUM", szAttrVal);
        if(szAttrVal[0])
            pIOC->lChannelNum = _wtol(szAttrVal);

        // ISEG_OFFS is optional
        XMLGetAttrTrim(pNode, L"ISEG_OFFS", szAttrVal);
        pIOC->ulISegOffs = szAttrVal[0] ? wcstoul(szAttrVal, &psz, 16) : ULONG_MAX;

        if(pIOC->ulISegOffs != ULONG_MAX)
        {
            //ISEG_OFFS given ==> ICHN_SIZE not optional
            XMLGetAttrTrim(pNode, L"ICHN_SIZE",szAttrVal);
            pIOC->ulIChnSize = szAttrVal[0] ? wcstoul(szAttrVal, &psz, 16) : ULONG_MAX;
        }

        // OSEG_OFFS is optional
        XMLGetAttrTrim(pNode, L"OSEG_OFFS", szAttrVal);
        pIOC->ulOSegOffs = szAttrVal[0] ? wcstoul(szAttrVal, &psz, 16) : ULONG_MAX;

        if (pIOC->ulOSegOffs != ULONG_MAX)
        {
            // OSEG_OFFS given ==> OCHN_SIZE not optional
            XMLGetAttrTrim(pNode, L"OCHN_SIZE", szAttrVal);
            pIOC->ulOChnSize = szAttrVal[0] ? wcstoul(szAttrVal, &psz, 16) : ULONG_MAX;
        }

        // GMSEG_OFFS is optional
        XMLGetAttrTrim(pNode, L"GMSEG_OFFS", szAttrVal);
        pIOC->ulGMSegOffs = szAttrVal[0] ? wcstoul(szAttrVal, &psz, 16) : ULONG_MAX;

        if (pIOC->ulGMSegOffs != ULONG_MAX)
        {
            // GMSEG_OFFS given ==> GMCHN_SIZE not optional
            XMLGetAttrTrim(pNode, L"GMCHN_SIZE", szAttrVal);
            pIOC->ulGMChnSize = szAttrVal[0] ? wcstoul(szAttrVal, &psz, 16) : ULONG_MAX;
        }

        // GMRSEG_OFFS is optional
        XMLGetAttrTrim(pNode, L"GMRSEG_OFFS", szAttrVal);
        pIOC->ulGMRSegOffs = szAttrVal[0] ? wcstoul(szAttrVal, &psz, 16) : ULONG_MAX;

        if (pIOC->ulGMRSegOffs != ULONG_MAX)
        {
            // GMRSEG_OFFS given ==> GMRCHN_SIZE not optional
            XMLGetAttrTrim(pNode, L"GMRCHN_SIZE", szAttrVal);
            pIOC->ulGMRChnSize = szAttrVal[0] ? wcstoul(szAttrVal, &psz, 16) : ULONG_MAX;
        }

        FC_RELEASE_PTR(pNode);
    }

    return pIOC_first;
}





/*
<?xml version="1.0" encoding="UTF-8"?>
<KAD4VM> 
    <TRGTYP EXT="4cwin" USECHANNELNUM="0">
       <IEC_FEATURES 
         DATA_TYPES="BOOL, INT, DINT, REAL, LREAL, BYTE, WORD, DWORD, TIME, STRING"
         ARR_IDX_DATA_TYPES="INT, DINT"
         LOCAL_RETAIN="1"
         LANGUAGES="ST, IL, FBD, LD, SFC" />
       <TASKIMAGE
         USE_FOR_SEGMENT="INPUT, OUTPUT"
         OPTIMIZE_FOR_SEGMENT="INPUT, OUTPUT"
         MAX_READ_REGIONS_PER_SEGM="INPUT:25, OUTPUT:25"
         MAX_WRITE_REGIONS_PER_SEGM="OUTPUT:75" />
       <IOCOMPONENTS>     
         <CHANNEL NUM="0"  INIT="1"   
           ISEG_OFFS="0x00000000"  ICHN_SIZE="0x00004000"
           OSEG_OFFS="0x00000000"  OCHN_SIZE="0x00004000"
           GMSEG_OFFS="0x00000000" GMCHN_SIZE="0x00002000"/>
         <CHANNEL NUM="1"  INIT="1"   
           GMSEG_OFFS="0x00002000" GMCHN_SIZE="0x00002000"/>
       </IOCOMPONENTS>            

*/

BOOL Parse4CVMKADForChannels(       //return false if file could not be parsed
    const TCHAR*    pszTrgTypeName, //4cwin, 4cosai, ...
    const TCHAR*    pszAutoDeclIni, //full path and file name of AutoDecl.ini
    IO_COMP_TYP**   ppIOCmps        //return, maqy be NULL if no channel def found
)
{
    TCHAR           szBuffer[2*MAX_PATH];
    IXMLDOMNode*    pNode;
    IXMLDOMNode*    pNode2;
    IXMLDOMNodeList*pNList;
    IXMLDOMNodeList*pNList2;

    *ppIOCmps = NULL;

    strcpy(szBuffer, pszAutoDeclIni);

    FC_StringCutFilePart(szBuffer);
    _tcscat(szBuffer, _T("\\"));
    _tcscat(szBuffer, pszTrgTypeName);
    _tcscat(szBuffer, _T("_4cvm.kad"));

    pNode = XMLLoadFile(szBuffer);
    if(!pNode)
        return FALSE;

    pNode2 = NULL;
    pNode->get_firstChild(&pNode2); //assume first child is <TRGTYP EXT="4cwin" USECHANNELNUM="0">
    FC_RELEASE_PTR(pNode)
    if(!pNode2)
        return FALSE;
    
    pNList = NULL;
    pNode2->selectNodes(L"IOCOMPONENTS", &pNList);
    FC_RELEASE_PTR(pNode2)
    if(!pNList)
        return FALSE;

    long len = 0;
    pNList->get_length(&len);
    ATLASSERT(len<=1);
    if(len>0)
    {
        pNList->get_item(0, &pNode);
        // <CHANNEL>
        pNList2 = NULL;
        pNode->selectNodes(L"CHANNEL", &pNList2);
        *ppIOCmps = ReadIOComponents(pNList2);
        FC_RELEASE_PTR(pNList2);
        FC_RELEASE_PTR(pNode);
    }
    FC_RELEASE_PTR(pNList);



    return TRUE;
}



BOOL LookUpChannelDef( //return true if a channel was found, false if failed
    const IO_COMP_TYP*  pChannelDefs, //list of known channels
    TCHAR               cLoc,         //I,Q or M
    long                nChannel,     //number of channel (first field of %-addr)
    unsigned long*      pulOff,       //output, set to ULONG_MAX if function failed
    unsigned long*      pulSize       //output, set to ULONG_MAX if function failed
)
{
    const IO_COMP_TYP* pCD;

    *pulOff = *pulSize = ULONG_MAX;

    
    for(pCD = pChannelDefs; pCD; pCD = pCD->pNext)
    {
        if(pCD->lChannelNum != nChannel)
            continue;

        switch(cLoc)
        {
        case 'I':
            if(pCD->ulISegOffs != ULONG_MAX)
            {
                *pulOff  = pCD->ulISegOffs;
                *pulSize = pCD->ulIChnSize;
                return TRUE;
            }
        break;
        case 'Q':
            if(pCD->ulOSegOffs != ULONG_MAX)
            {
                *pulOff  = pCD->ulOSegOffs;
                *pulSize = pCD->ulOChnSize;
                return TRUE;
            }
        break;
        case 'M':
            if(pCD->ulGMSegOffs != ULONG_MAX)
            {
                *pulOff  = pCD->ulGMSegOffs;
                *pulSize = pCD->ulGMChnSize;
                return TRUE;
            }
        break;
        default:
            ATLASSERT(!"bad addr location");
        }
    }

    return FALSE;

}



//------------------------------------------------------------------*
/**
 * StringIncBeginIW test if a string start with another
 * [ignoring case]. If *ppsz begins with pszWith *ppsz is incremented
 * to point to the first char right after pszWith.
 * Note: if pszWith=="" the function always returns true.
 * @param   ppsz    : address of string to work on
 * @param   pszWith : assumed begin of psz
 * @return  true if begins with false if not
*/
__inline int StringIncBeginIW(const wchar_t** ppsz, const wchar_t* pszWith)
{
    int iLen = wcslen(pszWith);
    return wcsnicmp(*ppsz, pszWith, iLen)==0 ? *ppsz+=iLen, 1 : 0;
}


BOOL ParseDataType( //return true if type could be parsed, false if not
    const wchar_t* pszIECType,  //simple type or at most complex (W)STRING[<len>] (no arrays!)
    BL_IEC_TYP*    piecTyp,     //output: baselib enum of basic type.
    long*          pstrLength   //output: string length if any sort of string, 0 otherwise 
)
{
    const wchar_t* psz;

    *pstrLength = 0;
    *piecTyp = BL_ANY;

    while(iswspace(*pszIECType))
        pszIECType++;

    //check for string[len] syntax:
    psz = pszIECType;
    if(StringIncBeginIW(&psz, L"STRING") || StringIncBeginIW(&psz, L"WSTRING"))
    {
        *piecTyp = *pszIECType=='S' ? BL_STRING : BL_WSTRING;

        while(iswspace(*psz))
            psz++;
        
        if(*psz=='[')
        {
            psz++;
            
            *pstrLength = wcstol(psz, (wchar_t**)&psz, 10);
            
            if(*pstrLength<0)
                return FALSE;
    
            while(iswspace(*psz))
                psz++;

            if(*psz!=']')
                return FALSE;
            psz++;
            while(iswspace(*psz))
                psz++;

            return *psz==0;

        }
        else if(*psz==0)
        {
            //string without length ?? assume default length 256 (TODO: default length 
            //should be from compiler KAD?):
            *pstrLength = 256;
            return TRUE;
        }
        else
        {
            //other char that '[' following (W)STRING:
            return FALSE;
        }
    }
    else
    {
        HRESULT hr;
        DWORD   modifiers;

        *pstrLength = 0;
        hr = BL_TypenameToDatatype(pszIECType,   // in parameter, 
                                 piecTyp,        // [out]
                                 &modifiers);    // BL_DTYPE_IS_QVT


        return hr==S_OK;
    }

}



//build a string that can be interpreted by the OPC Server (see also OPC Server's 
//implementaion -> function 'IsDirectVarAddress')
BOOL BuildOnlString(
    TCHAR                   szOnlString[MAX_PATH], //output: example: L"__system.direct.W.2.2"
    const LU_IECADDRESS*    pAddr,                 //parsed IEC address
    unsigned long           ulChannelOffset,       //channel offset
    BL_IEC_TYP              iecTyp,                //iec type to be used
    long                    strLength              //str len is type is STRING
)
{
    int           lSegment;
    unsigned long ulByteOff;
    unsigned long ulBitOff;

    szOnlString[0] = 0;

    switch(pAddr->cLoc)
    {
    case 'I':  lSegment = 0; break;
    case 'Q':  lSegment = 1; break;
    case 'M':  lSegment = 2; break; 
    default:
        ATLASSERT(!"bad location");
        return FALSE;
    }

    
    switch(iecTyp)
    {
    case BL_BOOL:
        ATLASSERT(pAddr->nFields==3);
        ulByteOff = pAddr->ulFields[1] + ulChannelOffset + pAddr->ulFields[2] / 8;
        ulBitOff  = pAddr->ulFields[2] % 8;//normalize bit off to be 0..7, if >7 add some bytes!
        _stprintf(szOnlString, _T("__system.direct.X.%d.%d.%d"), lSegment, ulByteOff, ulBitOff);
    break;
    
    case BL_BYTE:
        ulByteOff = pAddr->ulFields[1] + ulChannelOffset;
        _stprintf(szOnlString, _T("__system.direct.B.%d.%d"), lSegment, ulByteOff);
    break;
    
    case BL_INT: //fall thr.
    case BL_UINT:
    case BL_WORD:
        ulByteOff = pAddr->ulFields[1] + ulChannelOffset;
        _stprintf(szOnlString, _T("__system.direct.W.%d.%d"), lSegment, ulByteOff);
    break;
    
    case BL_DINT: //fall thr.
    case BL_UDINT:
    case BL_DWORD:
        ulByteOff = pAddr->ulFields[1] + ulChannelOffset;
        _stprintf(szOnlString, _T("__system.direct.D.%d.%d"), lSegment, ulByteOff);
    break;
    
    case BL_LINT:
    case BL_LWORD:
        ulByteOff = pAddr->ulFields[1] + ulChannelOffset;
        _stprintf(szOnlString, _T("__system.direct.L.%d.%d"), lSegment, ulByteOff);
    break;
    
    case BL_REAL:
        ulByteOff = pAddr->ulFields[1] + ulChannelOffset;
        _stprintf(szOnlString, _T("__system.direct.F.%d.%d"), lSegment, ulByteOff);
    break;

    case BL_LREAL:
        ulByteOff = pAddr->ulFields[1] + ulChannelOffset;
        _stprintf(szOnlString, _T("__system.direct.G.%d.%d"), lSegment, ulByteOff);
    break;

    case BL_STRING:
        ulByteOff = pAddr->ulFields[1] + ulChannelOffset;
        _stprintf(szOnlString, _T("__system.direct.S.%d.%d.%d"), lSegment, ulByteOff, strLength);
    break;

    default:
        ATLASSERT(!"bad iec type");
        return FALSE;
    }

    return TRUE;
}


