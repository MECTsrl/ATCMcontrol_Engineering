


#include <stdio.h>
#include "fc_tools\fc_fileIO.h"
#include "fc_tools\fc_libmem.h"
#include "fc_tools\fc_registry.h"
#include "helper.h"
#include "utilif.h" //fuckn UTIL_XMLDecodeString
#include "cgt_misc.h"
#include "cgt_xmlparse.h"
#include "CG_Messages.h"


static TCHAR     g_szSetupDir[MAX_PATH] = {0};
static TCHAR     g_szEngBinDir[MAX_PATH] = {0};
static TCHAR     g_szSTDLibPath[MAX_PATH] = {0};

//generall helper functions:

bool ST_CheckIdentifier(
    CGT_CDiagMsg*    pdm,
    const CG_Edipos* pEdp, 
    const TCHAR*     pszIdToCheck
)
{
    if(*pszIdToCheck==0)
    {
        FC_CString jot;
        pdm->msg1(CG_E_SYNTAX_MISSING, pEdp, pdm->res1(ST_RES_STR_IDENTIFIER));
        return false;
    }
    
    switch(UTIL_CheckIdentifier(pszIdToCheck))
    {
    case E_UTIL_ID_SYNTAX:
        pdm->msg1(CG_E_IDENT_SYNTAX, pEdp, pszIdToCheck);
    break;
    case E_UTIL_ID_IS_KEYWORD:
        pdm->msg1(CG_E_ID_IS_KEWORD, pEdp, pszIdToCheck);
    break;    
    case E_UTIL_ID_TOO_LONG:
        pdm->msg2(CG_E_ID_TOO_LONG, pEdp, pszIdToCheck, 
            pdm->jot1()<<UTIL_MAX_ID_LENGTH);
    break;
    case S_OK:
        return true;
    default:
        assert(!"unexpected return from UTIL_CheckIdentifier");
    }
   
    return false;
}




bool ST_LoadTextFileToMem( //TODO replace by CGT_LoadLocator ...
    CGT_CDiagMsg*       pdm, 
    const TCHAR*        pszPrjPath,
    const CG_Edipos*    pEdp,
    const TCHAR*        pszFileName, 
    TCHAR**             ppCharBuffer, 
    unsigned long*      pulLength, 
    __int64*            pftLastWrite
)
{
    TCHAR  szFileAndFullPath[2*MAX_PATH];

    int iRet;
    //printf("load->%s\r\n",  (const char*)pszFileName);
    if(ppCharBuffer)
        *ppCharBuffer = NULL;
    if(pulLength)
        *pulLength = 0;

    FC_StringCanonicalPath(
        pszFileName, 
        szFileAndFullPath,
        FC_ARRAY_LEN(szFileAndFullPath), 
        pszPrjPath,
        FC_PATHM_FULL
    );


    *pulLength = ST_MAX_FILE_SIZE;
    iRet = FC_LoadDiskTextFileToMem(
            szFileAndFullPath, 
            ppCharBuffer, 
            pulLength,    
            pftLastWrite
           );
    /*
        0: no error
        1: file does simply not exist.
        2: file cannot be opened or read (possibly missing permission)
          Use win32 GetLastError() for more information.
        3: file is larger than *pulLength (>0)
        4: Unicode to Multibyte conversion error (error currently not supported)
        -1: Out of memory
    */
    switch(iRet)
    {
    case  0: 
    break;
    case -1: 
        pdm->msg0(CG_E_OUTOFMEM, pEdp); 
    break;
    case  1: 
        pdm->msg1(CG_E_FILE_NOT_EXIST, pEdp, pszFileName); 
    break;
    case  2: 
    {
        FC_CString jot;
        pdm->msg2(CG_E_FILE_OPEN, pEdp, pszFileName, jot.fmtSysErr(GetLastError())); 
    }
    break;
    case  3: 
        pdm->msg1(CG_E_FILE_TO_LARGE, pEdp, pszFileName); 
    break;
    case  4: 
    default:
        assert(!"unexpected return");
    }
 
    return iRet==0;
}



FC_CStr  ST_TypeSetToErrMsgString(const ST_TypeSet_C& type)
{
    TCHAR szType[MAX_TYPE_DUMP_BUFF];
    
    //NOTE: before you try to get the function block, struct, program type name
    //      check that 'type' has a detailed type description, just see if 
    //      type.GetTypeName() != NULL. Same problem with ARRAY check 
    //      type.GetArrayDim()>0. There are situations like: 
    //      ST_TypeSetToErrMsgString(ST_INSTANCE), then ST_TypeSet_C(long)
    //      converts the typeset long into a ST_TypeSet_C, but no instance type
    //      description is set.

    //##AC beg
    CG_BITSET lTypeSet = type.GetTypeSet();
    if((lTypeSet & (CG_TBS_INSTANCE|CG_TBS_STRUCT|CG_TBS_PROGRAM)) && type.GetTypeName())
    {
        return type.GetTypeName();
    }
    else if(lTypeSet==CG_TBS_ARRAY)
    {
        int              i;
        ST_SUBRANGE_TYP  range;
        int              nDims = type.GetArrayDim();
        FC_CString       str(64);
        
        str.load(nDims>0 ? _T("ARRAY[") : _T("ARRAY"));
        
        for(i=0;i<nDims;i++)
        {
            type.GetArrayRange(i, &range);
            if(i>0)
                str<<_T(",");

            str<<range.lRangeStart<<_T(".."); 
            //CAUTION: long + ulong == ulong, tthis is bad for negative array bounds
            str<<(long)(range.lRangeStart+range.ulRangeLength);
        }
        
        if(type.GetArrayBase() != CG_TBS_EMPTY)
        {
            if(nDims>0)
                str<<_T("] OF ");
            else
                str<<_T(" OF ");

            str<<ST_TypeSetToErrMsgString(type.GetArrayBase());
        }

        return str; 
    }
    else if((lTypeSet & CG_TBS_ANY_STRING) && type.GetStringLength()>=0)
    {
        ST_TypeToString(szType, type.GetTypeSet());
        FC_CString      str(64);
        str<<szType<<_T("[")<<type.GetStringLength()<<_T("]");
        return str;
    }
    else
    {
        return ST_TypeToString(szType, lTypeSet);
    }
    //##AC end
}

 
const TCHAR* ST_CG_DTYPES_ToString(CG_DTYPES dt)
{
    return CGT_DTYPES_ToString(dt); //##AC
}
CG_DTYPES ST_TypeKeywordToCG_DTYPES(const TCHAR* pszTypeKW)
{
    return CGT_TypeKeywordToCG_DTYPES(pszTypeKW); //##AC
}




const TCHAR* ST_ExprTypToString(CG_EXPRTYP etyp)
{
    switch(etyp)
    {
    case CG_EXPRTYP_UNDEF: 
        assert(!"bad expr typ enum");
        return _T("<internal error, no expr. type");
    //unary ops:
    case CG_NOT:        return _T("NOT");
    case CG_UMINUS:     return _T("-");
    case CG_NOOP:       return _T("()");
    //binary ops:
    case CG_OR:         return _T("OR"); 
    case CG_XOR:        return _T("XOR"); 
    case CG_AND:        return _T("AND"); 
    case CG_EQ:         return _T("="); 
    case CG_NE:         return _T("<>"); 
    case CG_LT:         return _T("<");
    case CG_LE:         return _T("<=");
    case CG_GT:         return _T(">");
    case CG_GE:         return _T(">=");
    case CG_ADD:        return _T("+");
    case CG_SUB:        return _T("-");
    case CG_MUL:        return _T("*");
    case CG_DIV:        return _T("/");
    case CG_MOD:        return _T("MOD");
    case CG_POW:        return _T("**");
    case CG_MOVE:       return _T(":=");
    //Primary:
    case CG_LITERAL:    return _T("<literal>");
    case CG_VARREF:     return _T("<variable>");
    case CG_STRUCTREF:  return _T(".");
    case CG_ARRAYREF:   return _T("[]");
     //function call:
    case CG_FUNCALL:    
    case CG_FUNMACRO:   return _T("<function>");
    case CG_BUILDIN:    return _T("<buildin>");
    //tmp vars:
    case CG_TMPVAR:     return _T("<tmp>");
    case CG_TRIGVAR:    return _T("<trig tmp>");
    //special:
    case CG_STEPACTION: return _T("<step/action ref>");
    case CG_USEREXPR:   return _T("<user node>");
    case CG_FBCALLEXPR: return _T("<FB call expr>");
    default:
        assert(!"bad expr typ enum");
        return _T("<internal error, no expr. type");
    }
}


const TCHAR* IL_ExprTypToString(CG_EXPRTYP etyp)
{
    switch(etyp)
    {
    case CG_EXPRTYP_UNDEF: 
        assert(!"bad expr typ enum");
        return _T("<internal error, no expr. type");
    //unary ops:
    case CG_NOT:        return _T("NOT");
    //binary ops:
    case CG_OR:         return _T("OR"); 
    case CG_XOR:        return _T("XOR"); 
    case CG_AND:        return _T("AND"); 
    case CG_EQ:         return _T("EQ"); 
    case CG_NE:         return _T("NE"); 
    case CG_LT:         return _T("LT");
    case CG_LE:         return _T("LE");
    case CG_GT:         return _T("GT");
    case CG_GE:         return _T("GE");
    case CG_ADD:        return _T("ADD");
    case CG_SUB:        return _T("SUB");
    case CG_MUL:        return _T("MUL");
    case CG_DIV:        return _T("DIV");
    case CG_MOD:        return _T("MOD");
    case CG_MOVE:       return _T("ST");
    case CG_UMINUS:     return _T("<unary minus>");
    //Primary:
    case CG_LITERAL:    return _T("<literal>");
    case CG_VARREF:     return _T("<variable>");
    case CG_STRUCTREF:  return _T(".");
    case CG_ARRAYREF:   return _T("[]");
     //function call:
    case CG_FUNCALL:    
    case CG_FUNMACRO:   return _T("<function>");
    case CG_BUILDIN:    return _T("<buildin>");
    //tmp vars:
    case CG_TMPVAR:     return _T("<tmp>");
    case CG_TRIGVAR:    return _T("<trig tmp>");
    //special:
    case CG_STEPACTION: return _T("<step/action ref>");
    case CG_USEREXPR:   return _T("<user node>");
    case CG_NOOP:       return _T("()");
    default:
        assert(!"bad expr typ enum");
        return _T("<internal error, no expr. type");
    }
}




const TCHAR* ST_StoreClassName(CG_DECLSCOPE s)
{
    return CGT_StoreClassName(s); //##AC
}

const TCHAR* ST_PouTypeName(CG_POUTYP pt, bool bEndMaker)
{
    return CGT_PouTypeName(pt, bEndMaker); //##AC
}


bool ST_CheckPOUType(
    CGT_CDiagMsg*        pdm, 
    const CG_Edipos*     pEdp, 
    const TCHAR*         pszPouName,
    CG_POUTYP            pouTypFound,
    long                 want
)
{
    bool bOk;

    //check for wanted POU type:
    switch(pouTypFound)
    {
    case CG_POU_SDT:  bOk = (want & ST_WANT_SDT)!=0; break;
    case CG_POU_FB:   bOk = (want & ST_WANT_FB )!=0; break;
    case CG_POU_FUN:  bOk = (want & ST_WANT_FUN)!=0; break;
    case CG_POU_PRG:  bOk = (want & ST_WANT_PRG)!=0; break;
    case CG_POUTYP_UNDEF: 
    {
        //create undef err msg:
        const TCHAR* pszWant;
        if(want==ST_WANT_SDT)
            pszWant = _T("STRUCT");
        else if(want==ST_WANT_FUN)
            pszWant = _T("FUNCTION");
        else if(want==ST_WANT_FB)
            pszWant = _T("FUNCTION_BLOCK");
        else if(want==ST_WANT_PRG)
            pszWant = _T("PROGRAM");
        else 
            pszWant = _T("POU");

        pdm->msg2(CG_E_UNDEF_POU_TYP, pEdp, pszWant, pszPouName);
    }
    return false;
    default:
        assert(!"bad enum");
    }

    if(!bOk)
    {
        FC_CString ptyps;
        if(want & ST_WANT_SDT)
            ptyps<<_T("STRUCT|");
        if(want & ST_WANT_FUN)
            ptyps<<_T("FUNCTION|");
        if(want & ST_WANT_FB)
            ptyps<<_T("FUNCTION_BLOCK|");
        if(want & ST_WANT_PRG)
            ptyps<<_T("PROGRAM|");

        ptyps.removeLastChar(NULL);
        //error C1332: expected type '%1', but '%2' is a '%3'
        pdm->msg3(CG_E_INVALID_POU_TYPE, pEdp, 
            ptyps, pszPouName, ST_PouTypeName(pouTypFound)
          );
    }

    return bOk;
}





bool ST_CreateIECMangeledName(
    const CG_Symtab* pSym, 
    FC_CString&      mgName
)
{
    const CG_SymEntry* pEntry;
    bool               bFirst;
    const TCHAR*       pszScope;


    mgName.clear();

    if((pSym->flags & CG_STAB_HDR_NOERR)==0)
        return false;

    switch(pSym->pouTyp)
    {
    case CG_POU_FUN: mgName<<_T("FUN#");break;
    case CG_POU_FB:  mgName<<_T("FB#"); break;
    case CG_POU_SDT: mgName<<_T("SDT#");break;
    case CG_POU_PRG: mgName<<_T("PRG#");break;
    case CG_POU_CST: mgName<<_T("CST#");break;
    default:
        return false;
    }

    mgName<<pSym->pszId<<_T("(");  
    
    bFirst = true;
    for(pEntry = pSym->pFirst; pEntry; pEntry = pEntry->pNext)
    {
        if(pSym->pouTyp==CG_POU_SDT)
        {
            pszScope = _T("V");
        }
        else if(pSym->pouTyp==CG_POU_CST)
        {
            pszScope = _T("C");
        }
        else
        {
            //TODO CONST as interface???
            switch(pEntry->declScope)
            {
            case CG_VAR_INPUT:  pszScope = _T("I"); break;
            case CG_VAR_OUTPUT: pszScope = _T("Q"); break;
            case CG_VAR_IN_OUT: pszScope = _T("X"); break;
            case CG_VAR_IN_EXT: pszScope = _T("IE"); break;
            default:
                continue;
            }
        }

        if(!bFirst)
            mgName.addc(_T(','));
        bFirst = false;
        
        mgName<<pszScope;

        if(pEntry->declMod & CG_DMOD_RETAIN)
            mgName.addc(_T('R'));
        else if(pEntry->declMod & CG_DMOD_RETAIN)
            mgName.addc(_T('N'));

        if(pEntry->declMod & CG_DMOD_CONSTANT)
            mgName.addc(_T('C'));
        
        mgName.addc(_T('#'));
        mgName<<pEntry->pType->pszName;
        mgName.addc(_T('#'));
        mgName<<pEntry->pszId;

        if(pSym->pouTyp==CG_POU_CST)
        {
            if(pEntry->pInit && pEntry->pInit->ut==CG_INIT_SIMPLE)
            {
                mgName.addc(_T('='));
                mgName<<pEntry->pInit->sim.pLit->psz;
            }
            else
            {
                assert(!"bad CONST entry init val");
            }
        }

    }
    mgName.addc(_T(')'));

    return true;
}


TCHAR* ST_GetIECHeaderText(
    CGT_CDiagMsg*    pdm, 
    CG_CIECFront*    pFront,
    const CG_Symtab* pSym
)
{
    TCHAR*          pszFile;
    unsigned long   ulLength;
    __int64         ftLastWrite;
    
    if((pSym->flags & CG_STAB_HDR_NOERR)==0)
    {
        CGT_IERR2(pdm, &pSym->edp, _T("ST_GetIECInterfaceHDR: symtab in error"));
        return NULL;
    }
    
    assert(pSym->srcTyp != CG_SRC_TYPE_UNDEF);
    assert(pSym->ulIF_Bytes>0 && pSym->ulIF_Bytes<=pSym->ulHdrBytes);


    if(pSym->srcTyp == CG_SRC_TYPE_4GR)
    {
        if(!ST_Get4GRHeaderAndBody(
            pdm, 
            NULL, 
            pFront, 
            pSym->edp.pszFile, 
            &pszFile, 
            NULL, //don't want the XML body
            &ulLength, 
            &ftLastWrite)
          ) 
          return NULL;
    }
    else if(pSym->srcTyp == CG_SRC_TYPE_ADDON_FILTER)
    {
        assert(!"IEC-filter not implem"); //TODO;
    }
    else
    {
        if(!CGT_LoadLocatorFileToMem(
            pdm,
            NULL,
            pFront->m_myCOMIF,
            pSym->edp.pszFile, 
            &pszFile, 
            &ulLength, 
            &ftLastWrite)
        )
            return NULL;
    }


    if(ftLastWrite != pSym->ftLastWrite || ulLength<pSym->ulHdrBytes)
    {
        pdm->msg1(CG_E_FILE_MODIFIED, &pSym->edp, pSym->edp.pszFile);
        delete(pszFile);
        return NULL;
    }

    //cut at end of header:
    pszFile[pSym->ulHdrBytes] = 0;

#ifdef UNICODE 
    #error "rethink and rework this code and all other code too ..."
#endif

    if(!FC_LibResizeMemory((void**)&pszFile, pSym->ulHdrBytes+1))
    {
        CGT_IERR(pdm, _T("ST_GetIECInterfaceHDR: out of mem on shrik memory?"));
        return false;
    }

    return pszFile;
}


bool ST_Get4GRHeaderAndBody(
    CGT_CDiagMsg*    pdm, 
    const CG_Edipos* pEdp,
    CG_CIECFront*    pFront,
    const TCHAR*     pszLocator, 
    TCHAR**          ppszHeader,
    IXMLDOMNode**    ppXMLBody,
    unsigned long*   pulLength,
    __int64*         pftLastWrite
)
{
    BSTR                bstr;
    BSTR                bstr2;
    HRESULT             hr;
    long                len;
    IXMLDOMNode*        pNodeRoot;
    IXMLDOMNode*        pNode;
    IXMLDOMNodeList*    pNList;
    TCHAR*              pszHeader;
    int                 nBytes;
    int                 nBytes2;

    if(pulLength)
        *pulLength  = 0;
    if(ppszHeader)
        *ppszHeader = NULL;

    
    assert(pFront->m_kad.getSourceTypeByExt(FC_StringGetFileExt(pszLocator))==CG_SRC_TYPE_4GR);


    //try to open the disk or lib xml file:
    pNodeRoot = CGT_XMLLoadLocatorFile(pdm, pFront->m_myCOMIF, //TODO pass pEdp for file open error
                     pszLocator, L"FCPOU", pftLastWrite);
    
    if(!pNodeRoot)
        return false;

    if(ppXMLBody)
    {
        pNList = NULL;
        len = 0;
        *ppXMLBody = NULL;
        hr = pNodeRoot->selectNodes(L"BODY", &pNList);
        if(hr==S_OK && pNList)
        {
            hr = pNList->get_length(&len);
            assert(hr==S_OK && len==1);
            if(hr == S_OK)
            {
                hr = pNList->get_item(0, &pNode);
                assert(hr==S_OK);
                FC_RELEASE_PTR(pNList);
            }
        }

        if(hr!=S_OK || !*ppXMLBody)
        {
            CG_Edipos edp;
            CG_InitSTEdipos(&edp, pszLocator);
            pdm->msg1(CG_E_XML_STRUCTURE_ERROR, &edp, _T("<BODY>"));
            return false;
        }


        //*ppXMLBody = pNode;

    }

    pNList = NULL;
    bstr   = NULL;
    len = 0;
    hr = pNodeRoot->selectNodes(L"HEADER", &pNList);

    if(hr == S_OK)
    {
        hr = pNList->get_length(&len);
        assert(hr==S_OK && len==1);
        if(hr == S_OK)
        {
            hr = pNList->get_item(0, &pNode);
            assert(hr==S_OK);
            FC_RELEASE_PTR(pNList);

            if(hr == S_OK)
            {
                hr = pNode->get_text(&bstr);
                assert(hr==S_OK);
                FC_RELEASE_PTR(pNode);
            }        

        }
    }

    FC_RELEASE_PTR(pNodeRoot);


    if(!bstr || hr!=S_OK)
    {
        CG_Edipos edp;
        CG_InitSTEdipos(&edp, pszLocator);
        pdm->msg1(CG_E_XML_STRUCTURE_ERROR, &edp, _T("<HEADER>"));
        return false;
    }

    bstr2 = NULL;
    hr = UTIL_XMLDecodeString(bstr, &bstr2, XML_ENCODE_DEFAULT);
    FC_FREE_BSTR(bstr);
    if(hr!=S_OK || bstr2==NULL)
    {
        CG_Edipos edp;
        CG_InitSTEdipos(&edp, pszLocator);
        pdm->msg1(CG_E_XML_STRUCTURE_ERROR_PCDATA, &edp, _T("<HEADER>"));
        return false;
    }



#ifdef UNICODE
#error "UNICODE: rework and rethink this code"
#endif
    
    nBytes = wcslen(bstr2)+1;
    nBytes2;

    pszHeader = new char[nBytes];
    nBytes2 = FC_StringToMultiByte(bstr2, pszHeader, nBytes);

    if(nBytes2<nBytes)
    {
        //enter this code if there was realy a multibyte char (will not happen...)
        delete(pszHeader);
        nBytes *= 2;
        pszHeader = new char[nBytes];
        nBytes2 = FC_StringToMultiByte(bstr2, pszHeader, nBytes);
        if(nBytes2<nBytes)
        {
            assert(!"more than two bytes per wide char?");
            CG_Edipos edp;
            CG_InitSTEdipos(&edp, pszLocator);
            pdm->msg1(CG_E_XML_STRUCTURE_ERROR, &edp, _T("<HEADER>"));
            delete(pszHeader);
            return false;
        }
    }
    FC_FREE_BSTR(bstr2);
    
    assert(nBytes2<=nBytes);
    if(pulLength)
    {
        *pulLength  = nBytes2-1;
#ifdef DEBUG
        assert(*pulLength == (unsigned)lstrlen(pszHeader));
#endif
    }

    if(ppszHeader)
        *ppszHeader = pszHeader;
    else
        delete(pszHeader);

    return true;
}



void ST_OnDeepCopy(
    CG_CIECFront*    pFront,
    CGT_CDiagMsg*    pdm,
    const CG_Edipos* pEdp,
    const CG_Symtab* pSym
)
{
    //checks if a struct/FB copy is allowed
    const CG_SymEntry*   pEntry; 
    const CG_KeyValPair* pAttr;   

    pAttr = pFront->lookUpAttrib(pSym->pAttribs, CG_ATTR_DEEPCOPY);

    if(pAttr)
    {
        if(!_tcsicmp(pAttr->pszVal, _T("disable")))
            pdm->msg1(CG_E_NO_DEEPCOPY, pEdp, pSym->pszId);
        else if(!_tcsicmp(pAttr->pszVal, _T("warn")))
            pdm->msg1(CG_W_NO_DEEPCOPY, pEdp, pSym->pszId);
        else
            assert(!"deepcopy unexpected attr val");
    }
    else
    {
        for(pEntry=pSym->pFirst; pEntry; pEntry=pEntry->pNext)
        {
            if(!pEntry->pType->pSym)
                continue;
            if(!(pEntry->pType->pSym->flags & CG_STAB_DIAG_ON_COPY))
                continue;
            
             //TODO???? better err msg if nested struct can not be assigned?
            if(pSym!=pEntry->pType->pSym)
                ST_OnDeepCopy(pFront, pdm, pEdp, pEntry->pType->pSym);
            else
                assert(!"unexpected recusrion");//no recursion here any more!
        }
    }
}

