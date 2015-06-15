#include <errno.h>
#include <limits.h>
#include <math.h> //fabs
#include <float.h> //FLT_MIN
#include "utilsts.h"
#include "helper.h"
#include "literals.h"
#include "expr.h"
#include "baselib.h"
#include "CG_Messages.h"
#include "st_ParseContext.h"
#include "FeatureSet.h"
#include "cgt_literal.h"

static CG_BITSET ST_GetPossibleTypeSetFromInt65(unsigned __int64 val, bool bNegative);

static TCHAR* PSZ_Empty_String = _T("");

void ST_InitBinValString(ST_BINVAL_TYP* pInit, const TCHAR* pText)
{
    pInit->iLen = _tcslen(pText);
    if(pInit->iLen>0)
    {
        pInit->pText= new TCHAR[pInit->iLen+1];
        memcpy(pInit->pText, pText, (pInit->iLen+1)*sizeof(TCHAR));
    }
    else
    {
        pInit->pText = PSZ_Empty_String;
    }
}
void ST_InitBinValFrom(ST_BINVAL_TYP* pInit, const ST_BINVAL_TYP* pFrom)
{
    if(pFrom->iLen>=0)
        ST_InitBinValString(pInit, pFrom->pText);
    else
        ST_InitBinValI64(pInit, pFrom->any64);
}
void ST_FreeInitBinVal(ST_BINVAL_TYP* pFree)
{
    assert(pFree->iLen!=0 || PSZ_Empty_String==pFree->pText);

    if(pFree->iLen>0)
        delete(pFree->pText);
}

//init pInit from CG_Literal: HACK scheibse lit handling !
void ST_InitBinValFrom(ST_BINVAL_TYP* pInit, const CG_Literal* pFrom)
{
    if(!CG_IsElementary(pFrom->type))
    {
        assert(!"bad cg lit");
        return;
    }
    

    switch(pFrom->ut)
    {
    case CG_LIT_NONE:
		assert(pFrom->type == CG_STRING || pFrom->type == CG_WSTRING);
        if(!pFrom->psz[0])
        {
            pInit->iLen = 0;
            pInit->pText = PSZ_Empty_String;
        }
        else
        {
            pInit->iLen  = _tcslen(pFrom->psz);
            pInit->pText = FC_StringDup(pFrom->psz);
        }
    break;
    case CG_LIT_FLT:
        if(pFrom->psz[0]!=_T('-') && pFrom->psz[0]!=_T('+'))
        {
            TCHAR buffer[MAX_PATH];
            buffer[0] = _T('+');
            lstrcat(buffer+1, pFrom->psz);
            pInit->pText = FC_StringDup(buffer);
        }
        else
        {
            pInit->pText = FC_StringDup(pFrom->psz);
        }
        pInit->iLen = lstrlen(pInit->pText);
    break;
    case CG_LIT_I64:
        pInit->iLen = -1;
        pInit->any64= pFrom->i64;
    break;
    default:
        assert(!"bad lit ut");
    }
}


ST_BINVAL_TYP* ST_LitBinValEval
(
    CGT_CDiagMsg*            pdm,
    const CG_Edipos*         pEdp,
    CG_EXPRTYP               op,
    const ST_Literal_C*      pLit1,
    const ST_Literal_C*      pLit2,
    CG_BITSET*               pResultType
)
{
    const ST_BINVAL_TYP* pArg1 = pLit1->getLitValue();
    const ST_BINVAL_TYP* pArg2 = pLit2 ? pLit2->getLitValue() : NULL;
    ST_BINVAL_TYP*       pResult = NULL;
    __int64              result64;
    bool                 bNegative;
    unsigned __int64     ulVal;
    bool                 bI64opOk;


    switch(op)
    {
    case CG_OR:
    case CG_XOR:
    case CG_AND:
    case CG_EQ:
    case CG_NE:

    case CG_LT:
    case CG_LE:
    case CG_GT:
    case CG_GE:
    return NULL;
    
    
    case CG_ADD:
    case CG_SUB:
    case CG_MUL:
    case CG_DIV:
        if(*pResultType & CG_TBS_ANY_INT)
        {
            assert((*pResultType & ~CG_TBS_ANY_INT)==0);
            switch(op)
            {
            case CG_ADD: bI64opOk = BL_safe_add_int64(&result64, pArg1->any64, pArg2->any64);break;
            case CG_SUB: bI64opOk = BL_safe_sub_int64(&result64, pArg1->any64, pArg2->any64);break;
            case CG_MUL: bI64opOk = BL_safe_mul_int64(&result64, pArg1->any64, pArg2->any64);break;
            case CG_DIV: 
                if(pArg2->any64==0L)
                { 
                    pdm->msg0(CG_E_DIVIDE_BY_ZERO, pEdp); 
                    return NULL;
                }
                bI64opOk = true;  // divide is always safe...
                result64 = pArg1->any64 / pArg2->any64;
            }
            if(!bI64opOk)
                goto overflow;

            //get result type after operation:
            bNegative   = result64<0;
            ulVal       = bNegative ? -result64 : result64;
            *pResultType &= ST_GetPossibleTypeSetFromInt65(ulVal, bNegative);
            if(*pResultType == CG_TBS_EMPTY)
                goto overflow;

            
            pResult = new ST_BINVAL_TYP;
            ST_InitBinValI64(pResult, result64);
            return pResult;
        }       
        
    return NULL;

    case CG_MOD:
    case CG_POW:
    case CG_NOT:
    return NULL;
    
    case CG_UMINUS:

        if(*pResultType & CG_TBS_ANY_INT)
        {
            assert((*pResultType & ~CG_TBS_ANY_INT)==0);
            if(pLit1->isPureLiteral())
            {
                // -128 Problem:
                switch(pArg1->any64)
                {
                case 0x8000:
                    *pResultType |= CG_TBS_INT;
                break;
                case 0x80000000:
                    *pResultType |= CG_TBS_DINT;
                break;
                case 0x8000000000000000:
                    *pResultType |= CG_TBS_LINT;
                break;
                }
            }            
    
            
            if(!BL_safe_sub_int64(&result64, 0l, pArg1->any64))
                goto overflow;

            //get result type after operation:
            bNegative   = result64<0;
            ulVal       = bNegative ? -result64 : result64;
            *pResultType &= ST_GetPossibleTypeSetFromInt65(ulVal, bNegative);
            if(*pResultType == CG_TBS_EMPTY)
                goto overflow;

            pResult = new ST_BINVAL_TYP;
            ST_InitBinValI64(pResult, result64);
            return pResult;
        }
        if(*pResultType & CG_TBS_ANY_REAL)
        {
            assert((*pResultType & ~CG_TBS_ANY_REAL)==0);
            pResult = new ST_BINVAL_TYP;
            ST_InitBinValFrom(pResult, pArg1);

            assert(pResult->pText && pResult->iLen>0);
            
            if(pResult->pText[0]==_T('+'))
            {
                pResult->pText[0]= _T('-');
            }
            else if(pResult->pText[0]==_T('-')) 
            {
                pResult->pText[0]= _T('+');
            }
            else
            {
                assert(!"invalid internal float format");
            }
            return pResult;
        }
        if(*pResultType & CG_TBS_TIME)
        {
            assert((*pResultType & ~CG_TBS_TIME)==0);
            if(!BL_safe_sub_int64(&result64, 0l, pArg1->any64))
                goto overflow;

            pResult = new ST_BINVAL_TYP;
            ST_InitBinValI64(pResult, result64);
            return pResult;
        }

        assert(!"invalid data type for '-'");

    case CG_MOVE:    /* VOID ":=" (ANY_LVALUE, ANY) */
    return NULL;
    
    case CG_NOOP:     /* ANY ( ANY ) */
        pResult = new ST_BINVAL_TYP;
        ST_InitBinValFrom(pResult, pArg1);
    return pResult;

    default:
        assert(!"invalid operator enum");
        return NULL;
    }

overflow:
    pdm->msg1(CG_E_CONSTFOLDING_OVERFLOW, pEdp, ST_ExprTypToString(op));
    return NULL;
}








CG_BITSET AnyFloatToBinVal(
    CGT_CDiagMsg*    pdm,
    const CG_Edipos* pEdp, 
    const TCHAR*     szLiteral, 
    ST_BINVAL_TYP**  ppBinVal,
    CG_BITSET        supportedTypes
)
{
    int           i;
    int           j;
    TCHAR*        lpBuffer = new TCHAR[_tcslen(szLiteral)+2];//CAUTION +2 because lpC[-1]= below
    TCHAR*        lpC = lpBuffer+1;
    TCHAR*        lpEnd;
    const TCHAR*  lpType;
    CG_BITSET     lTypedLitType = CG_TBS_EMPTY;
    CG_BITSET     lTypeSetRet  = CG_TBS_EMPTY;


    //remove '_'
    for (j=0, i=0; szLiteral[i]; i++)
    {
       if (szLiteral[i]!=_T('_'))
         lpC[j++] = szLiteral[i];
    }
    lpC[j] = 0;


    //typed literal ? 
    lpType = lpC;
    while(_istalpha(*lpC))
        *lpC++;

    if(lpType != lpC)
    {
        assert(*lpC==_T('#'));
        *lpC = 0;
        lpC++;
    
        //HACK ?????????????????????????
        if(       !_tcsicmp(_T("BOOL"), lpType))
            lTypedLitType = CG_TBS_BOOL;
        else if( !_tcsicmp(_T("BYTE"), lpType))
            lTypedLitType = CG_TBS_BYTE;
        else if( !_tcsicmp(_T("WORD"), lpType))
            lTypedLitType = CG_TBS_WORD;
        else if( !_tcsicmp(_T("DWORD"), lpType))
            lTypedLitType = CG_TBS_DWORD;
        else if( !_tcsicmp(_T("INT"), lpType))
            lTypedLitType = CG_TBS_INT;
        else if( !_tcsicmp(_T("DINT"), lpType))
            lTypedLitType = CG_TBS_DINT;
        else if( !_tcsicmp(_T("REAL"), lpType))
            lTypedLitType = CG_TBS_REAL;
        else if( !_tcsicmp(_T("LREAL"), lpType))
            lTypedLitType = CG_TBS_LREAL;
        else
        {
            pdm->errNS2(pEdp, pdm->res1(CG_RES_STR_DATATYPE), lpType);
        }
    }


    errno = 0;
    double aDouble =  _tcstod(lpC, &lpEnd);
    
    if (*lpEnd!='\0' || errno == ERANGE)
    {
        pdm->msg1(CG_E_FLOAT_LITERAL, pEdp, szLiteral);
        aDouble = 0.0;
    }

    
    //normalize syntax to leading + or -, ST_BINVAL_TYP need this:
    if(*lpC != _T('-') && *lpC != _T('+'))
    {
        lpC--;
        *lpC = _T('+');
    }


    *ppBinVal = new ST_BINVAL_TYP;
    ST_InitBinValString(*ppBinVal, lpC);

    //get possible types:
    if(lTypedLitType != CG_TBS_EMPTY)
    {
        //check if supported type:
        if((lTypedLitType & supportedTypes)==0)
        {
            pdm->errNS3(pEdp,pdm->res1(CG_RES_STR_DATATYPE),
                  ST_BitsetToStr(pdm->jot2(), lTypedLitType), 
                  pdm->res3(CG_RES_STR_USEDLIT)<<_T("'")<<szLiteral<<_T("'"));
            //carry on...
        }


        //ckeck if compatible with <type># if any
        if(lTypedLitType==CG_TBS_REAL)
        {
            //check range for flt:
            if(fabs(aDouble)>FLT_MAX)
            {
                pdm->msg1(CG_E_FLOAT_LITERAL, pEdp, szLiteral);
            }
            else if(aDouble!=0.0 && fabs(aDouble)<FLT_MIN)
            {
                pdm->msg2(CG_W_FLOAT_LIT_UNDERFLOW, pEdp, szLiteral, _T("1.175494351e-38"));
            }
            lTypeSetRet = CG_TBS_REAL;
        }
        else if(lTypedLitType==CG_TBS_LREAL)
        {
            //check range for dbl:
            //done above with  ERRNO: if(fabs(aDouble)>DBL_MAX)
            //TODO ??? if(fabs(aDouble)<DBL_MIN)
            lTypeSetRet = CG_TBS_LREAL;
        }
        else
        {
            pdm->msg2(CG_E_TYPED_LIT_TYPE_MISMATCH, pEdp,
                    szLiteral, pdm->jot1()<<_T("LREAL|REAL"));
            lTypeSetRet = CG_TBS_ANY_REAL; //after error should beeter use the lit type, but the lit bin val struct is filled with dbl!!!
        }
    }
    else if(aDouble!=0.0 && (fabs(aDouble)>FLT_MAX || fabs(aDouble)<FLT_MIN))
    {
        lTypeSetRet = CG_TBS_LREAL;
    }
    else
    {
        lTypeSetRet = CG_TBS_ANY_REAL;
    }

    delete lpBuffer;
    return lTypeSetRet;
}


static CG_BITSET ST_GetPossibleTypeSetFromInt65(unsigned __int64 val, bool bNegative)
{

    CG_BITSET lTypeSet = CG_TBS_LWORD;

    if (val<=0x7FFFFFFFFFFFFFFF)
        lTypeSet |= CG_TBS_LINT;
    if (val<=0x7FFFFFFF)
        lTypeSet |= CG_TBS_DINT;
    if (val<=0x7FFF)
        lTypeSet |= CG_TBS_INT;
    if (val<=0x7F)
        lTypeSet |= CG_TBS_SINT;

    if(bNegative)
    {
        if(val==0x80)
            lTypeSet |= CG_TBS_SINT;
        if(val==0x8000)
            lTypeSet |= CG_TBS_INT;
        else if(val==0x80000000)
            lTypeSet |= CG_TBS_DINT;
        else if(val==0x8000000000000000)
            lTypeSet |= CG_TBS_LINT;
    }
    else
    {
        if (val<=0xFFFFFFFF)
            lTypeSet |= (CG_TBS_DWORD|CG_TBS_UDINT);
        if (val<=0xFFFF)
            lTypeSet |= (CG_TBS_WORD|CG_TBS_UINT);
        if (val<=0xFF)
            lTypeSet |= (CG_TBS_BYTE|CG_TBS_USINT);
        if (val<=0x1)
            lTypeSet |= CG_TBS_BOOL;
    }

    return lTypeSet;
}


static CG_BITSET AnyIntOrBitToBinVal(
    CGT_CDiagMsg*    pdm,
    const CG_Edipos* pEdp, 
    const TCHAR*     pszToken, 
    __int64*         pResult64,
    CG_BITSET        supportedTypes   
)
{
    const wchar_t* pszWtoken;
    const wchar_t* pszEnd;
    const wchar_t* psz;
    bool           bNegative = false;
    bool           bSigned = false;
    CG_BITSET      lTypeSet;
    CG_BITSET      lTypedLitType = CG_TBS_EMPTY;
    int            base = 10;


#ifdef UNICODE  
    pszWtoken = pszToken;
#else        
    wchar_t buffer[2*UTIL_STS_MAX_TOKEN_LENGTH];
    int iRet = FC_StringToWideChar(pszToken, buffer, 2*UTIL_STS_MAX_TOKEN_LENGTH);
    assert(iRet>0 && iRet<2*UTIL_STS_MAX_TOKEN_LENGTH);
    pszWtoken = buffer;
#endif
    
    //typed literal ? 
    psz = pszWtoken;
    while(iswalpha(*psz))
        psz++;

    if(pszWtoken!=psz)
    {
        assert(*psz==L'#');
        psz++;

        
        if(      !_wcsnicmp(L"BOOL", pszWtoken, 4))
        {
            lTypedLitType = CG_TBS_BOOL;
        }
        else if( !_wcsnicmp(L"BYTE", pszWtoken, 4))
        {
            lTypedLitType = CG_TBS_BYTE;
        }
        else if( !_wcsnicmp(L"WORD", pszWtoken, 4))
        {
            lTypedLitType = CG_TBS_WORD;
        }
        else if( !_wcsnicmp(L"DWORD", pszWtoken, 5))
        {
            lTypedLitType = CG_TBS_DWORD;
        }
        else if( !_wcsnicmp(L"LWORD", pszWtoken, 5))
        {
            lTypedLitType = CG_TBS_LWORD;
        }
        else if( !_wcsnicmp(L"INT", pszWtoken, 3))
        {
            lTypedLitType = CG_TBS_INT;
        }
        else if( !_wcsnicmp(L"UINT", pszWtoken, 3))
        {
            lTypedLitType = CG_TBS_UINT;
        }
        else if( !_wcsnicmp(L"DINT", pszWtoken, 4))
        {
            lTypedLitType = CG_TBS_DINT;
        }
        else if( !_wcsnicmp(L"UDINT", pszWtoken, 3))
        {
            lTypedLitType = CG_TBS_UDINT;
        }
        else if( !_wcsnicmp(L"LINT", pszWtoken, 4))
        {
            lTypedLitType = CG_TBS_LINT;
        }
        else if( !_wcsnicmp(L"ULINT", pszWtoken, 4))
        {
            lTypedLitType = CG_TBS_ULINT;
        }
        else if( !_wcsnicmp(L"REAL", pszWtoken, 4))
        {
            lTypedLitType = CG_TBS_REAL;
        }
        else if( !_wcsnicmp(L"LREAL", pszWtoken, 5))
        {
            lTypedLitType = CG_TBS_LREAL;
        }
        else
        {
            pdm->errNS2(pEdp, pdm->res1(CG_RES_STR_DATATYPE), pszToken);
        }
    }


    //has sign +- or base 2# 8# or 16# ?
    if(psz[0]==_T('-') || psz[0]==_T('+'))
    {
        bSigned = true;
        if(psz[0]==_T('-'))
            bNegative = true;
        psz++;
    }
    if(psz[0]==_T('2') && psz[1]==_T('#'))
    {
        base = 2;
        psz +=2;
    }
    else if(psz[0]==_T('8') && psz[1]==_T('#'))
    {
        base = 8;
        psz +=2;
    }
    else if(psz[0]==_T('1') && psz[1]==_T('6') && psz[2]==_T('#'))
    {
        base = 16;
        psz +=3;
    }


    //get unsigned 64 bit value:
    if(FAILED(BL_ParseInt64(psz, pResult64, false, base, &pszEnd)) || *pszEnd!=0)
    {
        pdm->msg1(CG_E_INT_LITERAL, pEdp, pszToken);
        *pResult64 = 0;
    }
    
    
    lTypeSet = ST_GetPossibleTypeSetFromInt65(*pResult64, bNegative);
    
    //handle implicite type cast feature int#16#ffff
    if(base!=10 && *pResult64>=0x8000i64 && *pResult64<=0xFFFFi64)
        lTypeSet |= CG_TBS_INT;

    //handle implicite type cast feature dint#16#ffffffff
    if(base!=10 && *pResult64>=0x80000000i64 && *pResult64<=0xFFFFFFFFi64)
        lTypeSet |= CG_TBS_DINT;

    //handle implicite type cast feature lint#16#ffffffff_ffffffff
    if(base!=10 && *pResult64>=0x8000000000000000i64 && *pResult64<=0xFFFFFFFFFFFFFFFFi64)
        lTypeSet |= CG_TBS_LINT;

    if(bSigned)
        lTypeSet &= CG_TBS_ANY_INT;
    

    //ckeck if compatible with <type># if any
    if(lTypedLitType!=CG_TBS_EMPTY)
    {
        //check supported types:
        if((lTypedLitType & supportedTypes)==0)
        {
            pdm->errNS3(pEdp, pdm->res1(CG_RES_STR_DATATYPE),
                  ST_BitsetToStr(pdm->jot2(), lTypedLitType), 
                  pdm->res3(CG_RES_STR_USEDLIT)<<_T("'")<<pszToken<<_T("'"));
            //carry on...
        }
        
        if((lTypedLitType & lTypeSet)==0)
        {
            pdm->msg2(CG_E_TYPED_LIT_TYPE_MISMATCH, pEdp,
                pszToken, ST_TypeSetToErrMsgString(lTypeSet));
            //fake: use lTypedLitType even after error but set value to zero
            bNegative = false;
            *pResult64 = 0;
        }

        lTypeSet = lTypedLitType;
    }
    else if(base!=10)
    {
        //16#ffff is never an INT type but maybe an unsigned int type:
        lTypeSet &= (CG_TBS_ANY_BIT|CG_TBS_ANY_USIGN_INT);
    }

    if(bNegative)
        *pResult64 = - (*pResult64);

    return lTypeSet;
}






FC_CStr ST_BinValToIECString(const ST_BINVAL_TYP* pBinVal, CG_BITSET lTypSet)
{
    VARIANT    va;
    BL_IEC_TYP blt;
    HRESULT    hr;
    LPOLESTR   pOut;

    
    if(!pBinVal)
    {
        assert(!"??<compiler error>");    
        return _T("??<compiler error>");
    }

    switch(lTypSet)
    {
    case CG_TBS_BOOL:
         va.boolVal = pBinVal->any64!=0 ? VARIANT_TRUE : VARIANT_FALSE;
         va.vt = VT_BOOL;
    break;
    case CG_TBS_BYTE:
    case CG_TBS_WORD:
    case CG_TBS_DWORD:
    case CG_TBS_INT:
    case CG_TBS_UINT:
    case CG_TBS_DINT:
    case CG_TBS_UDINT:
    case CG_TBS_DATE:
    case CG_TBS_TOD:
        va.vt = VT_I4;
        va.lVal = (long)pBinVal->any64;
    break;

    case CG_TBS_DT:
    case CG_TBS_TIME:
    case CG_TBS_LINT:
    case CG_TBS_LWORD:
        va.vt = VT_DECIMAL;

        if (pBinVal->any64>=0)
        {
            va.decVal.scale = 0;
            va.decVal.sign = 0;
            va.decVal.Hi32 = 0;
            va.decVal.Lo64 = pBinVal->any64;
        }
        else
        {
            va.decVal.scale = 0;
            va.decVal.sign = DECIMAL_NEG;
            va.decVal.Hi32 = 0;
            va.decVal.Lo64 = -pBinVal->any64;                
        }
    break;
    case CG_TBS_REAL:
    case CG_TBS_LREAL:
        return pBinVal->pText;

    case CG_TBS_STRING:
    case CG_TBS_WSTRING:
        return pBinVal->pText;

    default:
        assert(!"illegal data type");        
        return _T("??<compiler error>");
    }


    switch(lTypSet)
    {
    case CG_TBS_BOOL:   blt = BL_BOOL;   break;
    case CG_TBS_BYTE:   blt = BL_BYTE;   break;
    case CG_TBS_WORD:   blt = BL_WORD;   break;
    case CG_TBS_DWORD:  blt = BL_DWORD;  break;
    case CG_TBS_INT:    blt = BL_INT;    break;
    case CG_TBS_UINT:   blt = BL_UINT;   break;
    case CG_TBS_DINT:   blt = BL_DINT;   break;
    case CG_TBS_UDINT:  blt = BL_UDINT;  break;
    case CG_TBS_DATE:   blt = BL_DATE;   break;
    case CG_TBS_TOD:    blt = BL_TOD;    break;
    case CG_TBS_DT:     blt = BL_DT;     break;
    case CG_TBS_TIME:   blt = BL_TIME;   break;
    case CG_TBS_LINT:   blt = BL_LINT;   break;
    case CG_TBS_LWORD:  blt = BL_LWORD;  break;
    case CG_TBS_REAL:   blt = BL_REAL;   break;
    case CG_TBS_LREAL:  blt = BL_LREAL;  break;
    case CG_TBS_STRING: blt = BL_STRING; break;
    case CG_TBS_WSTRING:blt = BL_WSTRING;break;
    default:
        assert(!"illegal data type");        
        return _T("??<compiler error>");
    }

    pOut = (LPOLESTR)_alloca(512*sizeof(OLECHAR));

    hr = BL_VariantToString(&va, blt, NULL, BL_USE_IEC_FORMAT, pOut, 512);
    assert(SUCCEEDED(hr));

    return pOut;  // this return is ok here:
                  // return value is of type ST_String_C, 
                  // implicitly called constructor ST_String_C(char*)
                  // does a deep copy of the string, so the stack storage no longer needed
}

static CG_BITSET MakeAnyTimeToBinVal(
    CGT_CDiagMsg*    pdm,
    const CG_Edipos* pEdp, 
    const TCHAR*     pszToken, 
    __int64*         pResult64, 
    int              tok
)
{
    HRESULT     hr;
    HRESULT     err_id = 0;
    SYSTEMTIME  systime;
    wchar_t*    pszWtoken;
    CG_BITSET   lTypeSet;
    long        val32;

#ifdef UNICODE  
    pszWtoken = pszToken;
#else        
    wchar_t buffer[2*UTIL_STS_MAX_TOKEN_LENGTH];
    int iRet = FC_StringToWideChar(pszToken, buffer, 2*UTIL_STS_MAX_TOKEN_LENGTH);
    assert(iRet>0 && iRet<2*UTIL_STS_MAX_TOKEN_LENGTH);
    pszWtoken = buffer;
#endif

    
    switch(tok)
    {
    case UTIL_STS_TIME_LITERAL: 
        lTypeSet = CG_TBS_TIME;
        hr = BL_IECTimeStrToMillis(pszWtoken, pResult64, NULL);
        //E_BL_ERR_LOSS_OF_PRECIS ????????
        if(FAILED(hr))
            err_id = CG_E_TIME_LITERAL;
    break;
    case UTIL_STS_DATE_LITERAL: 
        lTypeSet = CG_TBS_DATE;
        hr = BL_IECDateStrToDaysSince1970(pszWtoken, &systime, &val32, NULL);
        *pResult64 = val32;
        if(FAILED(hr))
            err_id = CG_E_DATE_LITERAL;
    break;
    case UTIL_STS_TOD_LITERAL: 
        lTypeSet = CG_TBS_TOD;
        hr = BL_IECTimeOfDayStrToMillis(pszWtoken, &systime, &val32, NULL);
        *pResult64 = val32;
        if(FAILED(hr))
            err_id = CG_E_TOD_LITERAL;
    break;
    case UTIL_STS_DT_LITERAL: 
        lTypeSet = CG_TBS_DT;
        hr = BL_IECDateAndTimeStrToUTCJava(pszWtoken, &systime, pResult64, NULL);
        if(FAILED(hr))
            err_id = CG_E_DT_LITERAL;
    break;
    default:
        assert(!"invalid type");
    }

    if(err_id)
        pdm->msg1(err_id, pEdp, pszToken);

    //assert: no syntax error must occur:
    assert(hr==S_OK || hr==E_BL_ERR_RANGE);

    return lTypeSet;
}


ST_Literal_C* ST_LitTokenToLiteral(
    CGT_CDiagMsg*      pdm,
    const CG_Edipos*   pEdp, 
    st_ParseContext*   pfc,
    const CFeatureSet* pFeat,
    int                tok, 
    const TCHAR*       pText
)
{
    CG_BITSET          typeSet;
    ST_Literal_C*      pLit;
    ST_BINVAL_TYP*     pBinVal = NULL;
    __int64            result64;

    switch(tok)
    {
    case UTIL_STS_TRUE_LITERAL:
        result64 = 1;
        typeSet= CG_TBS_BOOL;
        pBinVal = new ST_BINVAL_TYP;
        ST_InitBinValI64(pBinVal, result64);
    break;
    case UTIL_STS_FALSE_LITERAL:
        result64 = 0;
        typeSet= CG_TBS_BOOL;
        pBinVal = new ST_BINVAL_TYP;
        ST_InitBinValI64(pBinVal, result64);
    break;
    case UTIL_STS_INT_LITERAL:
        typeSet = AnyIntOrBitToBinVal(pdm, pEdp, pText, &result64, pFeat->m_pCFeat->allTypes);
        pBinVal = new ST_BINVAL_TYP;
        ST_InitBinValI64(pBinVal, result64);
    break;
    case UTIL_STS_FLOAT_LITERAL:
        typeSet= AnyFloatToBinVal(pdm, pEdp, pText, &pBinVal, pFeat->m_pCFeat->allTypes);
    break;
    case UTIL_STS_DATE_LITERAL:
    case UTIL_STS_DT_LITERAL:
    case UTIL_STS_TOD_LITERAL:
    case UTIL_STS_TIME_LITERAL:
        typeSet = MakeAnyTimeToBinVal(pdm, pEdp, 
            pText, &result64, tok);

        if((typeSet & pFeat->m_pCFeat->allTypes)==0)
        {
            pdm->errNS3(pEdp, pdm->res1(CG_RES_STR_DATATYPE),
                  ST_BitsetToStr(pdm->jot2(), typeSet), 
                  pdm->res3(CG_RES_STR_USEDLIT)<<_T("'")<<pText<<_T("'"));
            //carry on...
        }

        pBinVal = new ST_BINVAL_TYP;
        ST_InitBinValI64(pBinVal, result64);

    break;
    case UTIL_STS_STRING_LITERAL_BEGIN:
    {
		long   iIECStrLen=-1;

	    //called only to get the error messages and the length of the 
        //binary representation of the string (we pass pBIN==NULL)
        bool bRes = CGT_IECStringToBinary(pdm, pEdp, pText, /*pBin*/NULL, &iIECStrLen);
        assert(bRes || pdm->getTotalErrors()>0);

        pBinVal = new ST_BINVAL_TYP;
        ST_InitBinValString(pBinVal, pText);
    
        typeSet = pText[0]==_T('"') ? CG_TBS_WSTRING : CG_TBS_STRING;

        //check supported types: TODO max string length
        if((typeSet & pFeat->m_pCFeat->allTypes)==0)
        {
            pdm->errNS3(pEdp, pdm->res1(CG_RES_STR_DATATYPE),
              ST_BitsetToStr(pdm->jot2(), typeSet), _T(""));
        }
        
        if(pFeat->m_pCFeat->nMaxStrLen && pFeat->m_pCFeat->nMaxStrLen < iIECStrLen)
        {
            pdm->errNS3(pEdp, ST_BitsetToStr(pdm->jot1(), typeSet), 
                pdm->res2(CG_RES_STR_LONGERTHAN)<<pFeat->m_pCFeat->nMaxStrLen,
                pdm->res3(CG_RES_STR_CHARS));
        }

#ifdef UNICODE
#error "rework and rethink this code"
#endif
        pLit = new ST_Literal_C(
                pEdp, 
                pfc, 
                pBinVal, 
                ST_TypeSet_C(typeSet, iIECStrLen),
                true, 
                NULL
               ); 
        return pLit;
    }
    break;
    default:
        assert(!"illegal literal");
        return NULL;
    }

    pLit = new ST_Literal_C(pEdp, pfc, pBinVal, typeSet, true, NULL);
    return pLit;
}








bool ST_GotoEndOfString(const TCHAR** ppsz)
{
    const TCHAR* psz = *ppsz;
    TCHAR delim = *psz;
    
    assert(delim==_T('"') || delim==_T('\''));
    
    psz = _tcsinc(psz);
    while(*psz)
    {
        if(*psz==_T('$'))
        {
            psz = _tcsinc(psz);
            if(*psz==0)
                break;
        }
        else if(*psz==delim)
        {
            break;
        }

        psz = _tcsinc(psz);
    }

    if(*psz==delim) 
    {
        psz = _tcsinc(psz);
        *ppsz = psz;
        return true;
    }
    *ppsz = psz;
    return false;
}



