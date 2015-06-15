
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include <limits.h>
#include <errno.h>
#include "helper.h"

bool LoadTxtFileToMem(const TCHAR* pszFile, TCHAR** ppsz, unsigned long* pulChars)
{
    int iRet = FC_LoadDiskTextFileToMem(pszFile, ppsz, pulChars, NULL);
    if(iRet == 1)
    {
        //simple error:
        _tprintf(_T("4dl: error: file '%s' does not exist\n"), pszFile);
        return false;
    }
    else if(iRet != 0)
    {
        //complex error:
        FC_CString jot;
        _tprintf(_T("4dl: error: cannot open file '%s' for reading: %s\n"), 
            pszFile, jot.fmtSysErr(GetLastError()));
        return false;
    }

    return true;
}


BL_IEC_TYP ParseSimpleType(const TCHAR* pszType, long* pnArrDim)
{
    TCHAR        buffer[MAX_PATH];
    const TCHAR* psz;
    TCHAR*       pszEnd;
    int          i;
    HRESULT      hr;
    BL_IEC_TYP   blTyp;
    DWORD        modifiers;


    *pnArrDim = 0;
    
    for(i=0, psz=pszType; *psz; psz++)
    {
        if(i>=MAX_PATH-1)
        {
            *pnArrDim = -2;
            return BL_ANY;
        }

        if(*psz == _T('['))
        {
            *pnArrDim = _tcstol(psz+1, &pszEnd, 10);
            if(*pnArrDim<=0)
                *pnArrDim = -2;
            if(pszEnd[0] != _T(']') || pszEnd[1])
            {
                *pnArrDim = -1;
                return BL_ANY;
            }

            break;
        }
        else
        {
            buffer[i++] = *psz;
        }
    }
    buffer[i] = 0;

#ifdef UNICODE
    hr = BL_TypenameToDatatype(buffer, &blTyp, &modifiers);
#else
    wchar_t bufferW[MAX_PATH];
    FC_StringToWideChar(buffer, bufferW, FC_ARRAY_LEN(bufferW));
    hr = BL_TypenameToDatatype(bufferW, &blTyp, &modifiers);
#endif
    return hr==S_OK ? blTyp : BL_ANY;
}


VARTYPE BLTypToVariantTyp(BL_IEC_TYP blTyp)
{
    switch(blTyp)
    {
    case BL_BOOL:    return VT_BOOL;
    case BL_BYTE:    return VT_I4;
    case BL_WORD:    return VT_I4;
    case BL_DWORD:   return VT_I4;
    case BL_LWORD:   return VT_DECIMAL;
    case BL_INT:     return VT_I4;
    case BL_DINT:    return VT_I4;
    case BL_LINT:    return VT_DECIMAL;
    case BL_REAL:    return VT_R4;
    case BL_LREAL:   return VT_R8;
    case BL_TIME:    return VT_DECIMAL;
    case BL_DT:      return VT_DECIMAL;
    case BL_DATE:    return VT_I4;
    case BL_TOD:     return VT_I4;
    case BL_STRING:  return VT_BSTR;
    case BL_WSTRING: return VT_BSTR; 
    case BL_UINT:    return VT_I4;
    case BL_UDINT:   return VT_I4;
    default:
        assert(!"bad BL_IEC_TYP enum");
        return VT_EMPTY;
    }
}
const TCHAR* BLTypToString(BL_IEC_TYP blTyp)
{
    switch(blTyp)
    {
    case BL_BOOL:    return _T("bool");
    case BL_BYTE:    return _T("byte");
    case BL_WORD:    return _T("word");
    case BL_DWORD:   return _T("dword");
    case BL_LWORD:   return _T("lword");
    case BL_INT:     return _T("int");
    case BL_DINT:    return _T("dint");
    case BL_LINT:    return _T("lint");
    case BL_REAL:    return _T("real");
    case BL_LREAL:   return _T("lreal");
    case BL_TIME:    return _T("time");
    case BL_DT:      return _T("dt");
    case BL_DATE:    return _T("date");
    case BL_TOD:     return _T("tod");
    case BL_STRING:  return _T("string");
    case BL_WSTRING: return _T("wstring"); 
    case BL_UINT:    return _T("uint");
    case BL_UDINT:   return _T("udint");
    default:
        assert(!"bad BL_IEC_TYP enum");
        return _T("<bad type>");
    }
}


bool StringToVariant(const TCHAR* pszTxt, int nChars, VARIANT* pvd, BL_IEC_TYP want)
{
    HRESULT        hr;
    wchar_t        buffer[2*MAX_PATH];

    if((want==BL_STRING || want==BL_WSTRING))
    {
        pvd->vt      = VT_BSTR;
        pvd->bstrVal = FC_StringToBSTR(pszTxt); //embedded 0-chars TODO
        return true;
    }
    
    FC_StringToWideChar(pszTxt, buffer, FC_ARRAY_LEN(buffer));
    hr = BL_StringToVariant(buffer, want, NULL, BL_USE_IEC_FORMAT, pvd);
    return SUCCEEDED(hr);
}

bool VariantToString(const VARIANT* pvd, BL_IEC_TYP want, FC_CString* pJot, bool bForDisplay)
{
    HRESULT        hr;
    VARIANT        tmp;
    wchar_t        buffer[2*MAX_PATH];
    const wchar_t* pszw;

    pJot->clear();

    if(pvd->vt==VT_EMPTY)
        return false;


    if((want==BL_STRING || want==BL_WSTRING))
    {
        if(pvd->vt == VT_BSTR)
        {
            pszw = pvd->bstrVal;
            hr = S_OK;
        }
        else
        {
            VariantInit(&tmp);
            hr = VariantChangeType(&tmp, (VARIANT*)pvd, VARIANT_NOVALUEPROP, VT_BSTR);
                return false;
            pszw = tmp.bstrVal;
        }
        
        if(SUCCEEDED(hr))
        {
            if(bForDisplay)
            {
                int            i;
                wchar_t        tmp[2];
                const wchar_t* psz;
                
                tmp[1] = 0;
                pJot->addc(want==BL_STRING? _T('\'') : _T('"'));
                
                for(i=0,psz=pszw; *psz && i<64; psz++,i++)
                {
                    tmp[0] = *psz; 
                    if(i>64)
                    {
                        (*pJot)<<_T("  ...");
                        break;
                    }
                    else if(iswprint(*psz) && iswascii(*psz))
                    {
                        (*pJot)<<tmp;
                    }
                    else
                    {
                        pJot->addc(_T('?'));
                    }
                }

                if(*psz==0)
                    pJot->addc(want==BL_STRING? _T('\'') : _T('"'));
            }
            else
            {
                //TODO  embedded 0-chars ? 
                pJot->load(pszw);
            }

            if(pvd->vt != VT_BSTR)
                VariantClear(&tmp);
        }
    }
    else
    {
        //DWORD fmt = bForDisplay ? BL_USE_IEC_FORMAT|BL_USE_TYPED_LITERALS : BL_USE_IEC_FORMAT;
        //TODO BL_USE_TYPED_LITERALS does not work??
        hr = BL_VariantToString((VARIANT*)pvd, want, NULL, BL_USE_IEC_FORMAT, buffer, FC_ARRAY_LEN(buffer));
        if(SUCCEEDED(hr))
        {
            if(bForDisplay && want!=BL_BOOL && !wcschr(buffer, _T('#')))//HACK
                pJot->load(BLTypToString(want))<<_T("#");
            
            (*pJot)<<buffer;
            pJot->toLower();
        }
    }

    return hr == S_OK;
}



extern "C" void FC_CSVRecordToFILEA(
    FILE*        pFile,
    const TCHAR* pBinStr, 
    int          iChars
)
{
    int   i;
    TCHAR cc;

    //first check if there are non print " or ; chars in there:
    if(FC_CSVNeedEscape(pBinStr, iChars))
    {
        _fputtc('"', pFile);
        for(i=0; ;i++)
        {
            if(iChars>=0 && i>=iChars)
                break;

            cc = pBinStr[i];
            if(iChars<0 && cc==0)
                break;
        
            if(cc==_T('"'))
                _fputtc('"', pFile);

            _fputtc(cc, pFile);
        }
        _fputtc('"', pFile);
    }
    else if(iChars<0)
    {
        _fputts(pBinStr, pFile);
    }
    else
    {
        for(i=0; i<iChars; i++)
            _fputtc(pBinStr[i], pFile);
    }
}





extern "C" void FC_CSVRecordToFILEW(
    FILE*          pFile,
    const wchar_t* pBinStr, 
    int            iChars
)
{
    int     i;
    wchar_t cc;

    //first check if there are non print " or ; chars in there:
    if(FC_CSVNeedEscapeW(pBinStr, iChars))
    {
        fputwc('"', pFile);
        for(i=0; ;i++)
        {
            if(iChars>=0 && i>=iChars)
                break;

            cc = pBinStr[i];
            if(iChars<0 && cc==0)
                break;
        
            if(cc==_T('"'))
                fputwc('"', pFile);

            fputwc(cc, pFile);
        }
        fputwc('"', pFile);
    }
    else if(iChars<0)
    {
        fputws(pBinStr, pFile);
    }
    else
    {
        for(i=0; i<iChars; i++)
            fputwc(pBinStr[i], pFile);
    }
}



bool GetConnectStringFromLoaderList(
    const TCHAR* pszListFile, 
    const TCHAR* pszTrgName,
    FC_CString*  pStrConnect
)
{
    FILE*           pLDList;
    TCHAR           szLine[1024];
    const TCHAR*    psz;


    pStrConnect->clear();

    pLDList = _tfopen(pszListFile, _T("r"));
    if(!pLDList)
    {
        _tprintf(_T("4dl: error: cannot open file %s (target '%s' not compiled?)"),pszListFile,pszTrgName);
        return false;
    }


    szLine[0] = 0;
    fgets(szLine, FC_ARRAY_LEN(szLine), pLDList);
    fclose(pLDList);

    FC_StringTrim(szLine);
    psz = szLine;

    if(FC_StringIncBegin(&psz, _T("control;")))
    {
        pStrConnect->load(psz);
    }
    else if(FC_StringIncBegin(&psz, _T("console;")))
    {
        pStrConnect->load(psz);
    }
    else
    {
        _tprintf(_T("4dl: fatal error: cannot extract target address from %s"), pszListFile);
        return false;
    }

    return true;
}





extern "C" int FC_CSVNeedEscape(
    const TCHAR* pBinStr, 
    int          iChars
)
{
    for(int i=0; ;i++)
    {
        if(iChars>=0 && i>=iChars)
            break;
        else if(iChars<0 && pBinStr[i]==0)
            break;


#ifdef UNICODE
        if(pBinStr[i]<32 || pBinStr[i]==L'"' || pBinStr[i]==L';')
            return 1;
#else
        if((unsigned char)pBinStr[i]<32 || pBinStr[i]=='"' || pBinStr[i]==';')
            return 1;
#endif
    }
    
    return 0;
}



extern "C" int FC_CSVNeedEscapeW(
    const wchar_t* pBinStr, 
    int            iChars
)
{
    for(int i=0; ;i++)
    {
        if(iChars>=0 && i>=iChars)
            break;
        else if(iChars<0 && pBinStr[i]==0)
            break;

        if(pBinStr[i]<32 || pBinStr[i]==L'"' || pBinStr[i]==L';')
            return 1;
    }
    return 0;
}





void CopyBinVal(
    BINVAL_TYP*       pDest, //must be initialised !
    const BINVAL_TYP* pSrc //must be initialised !
)
{
    FreeBinVal(pDest);
    pDest->typ = pSrc->typ; 

    if(pSrc->typ==BL_STRING || pSrc->typ==BL_WSTRING)
    {
        pDest->nStrlen = pSrc->nStrlen;
        pDest->pszStr = new TCHAR[pSrc->nStrlen+1];
        memcpy(pDest->pszStr, pSrc->pszStr, (pSrc->nStrlen+1)*sizeof(TCHAR));
    }
    else if(pSrc->typ != BL_ANY)
    {
        pDest->aLWORD = pSrc->aLWORD;
    }
    else
    {
        pDest->aLWORD = 0;
    }
}



static void SetVTDecimalVal(VARIANT* pVar, __int64 i64)
{
    pVar->vt      = VT_DECIMAL;
    if(i64>=0)
    {
        pVar->decVal.scale = 0;
        pVar->decVal.sign = 0;
        pVar->decVal.Hi32 = 0;
        pVar->decVal.Lo64 = i64;
    }
    else
    {
        pVar->decVal.scale = 0;
        pVar->decVal.sign = DECIMAL_NEG;
        pVar->decVal.Hi32 = 0;
        pVar->decVal.Lo64 = -i64;
    }
}
static inline void GetVTDecimalAsSI64(const VARIANT* pVar, __int64* pi64)
{
    __int64 i64 = pVar->decVal.Lo64;
    if(pVar->decVal.sign & DECIMAL_NEG)
      i64 = - i64;

    *pi64 = i64;
}
static inline void GetVTDecimalAsUI64(const VARIANT* pVar, unsigned __int64* pui64)
{
    __int64 i64 = pVar->decVal.Lo64;
    if(pVar->decVal.sign & DECIMAL_NEG)
       i64 = - i64;

    *pui64 = i64;
}


bool BinValToVariant(
    const BINVAL_TYP*    pVal,
    VARIANT*             pVar   //gets always initialised (if error by VariantInit)
)
{
    switch(pVal->typ) 
    {
    case BL_BOOL:
        pVar->vt  = VT_BOOL;
        if(pVal->aBOOL)
            pVar->boolVal = VARIANT_TRUE;
        else
            pVar->boolVal = VARIANT_FALSE;
    break;
    case BL_BYTE:
        pVar->vt = VT_I4;
        pVar->lVal = pVal->aBYTE & 0xff;
    break;
    case BL_WORD:
    case BL_UINT:
        pVar->vt = VT_I4;
        pVar->lVal = pVal->aWORD & 0xffff;
    break;
    case BL_INT:
        pVar->vt = VT_I4;
        pVar->lVal = pVal->aINT;
    break;
    case BL_DWORD:
    case BL_UDINT:
        pVar->vt = VT_I4;
        pVar->lVal = pVal->aDWORD;
    break;
    case BL_DINT: 
        pVar->vt = VT_I4;
        pVar->lVal = pVal->aDINT;
    break;
    case BL_LWORD:    
        SetVTDecimalVal(pVar, pVal->aLWORD);
    break;
    case BL_LINT:
        SetVTDecimalVal(pVar, pVal->aLINT);
    break;
    case BL_REAL:
        pVar->vt = VT_R4;
        pVar->fltVal = pVal->aREAL;
    break;
    case BL_LREAL:    
        pVar->vt = VT_R8;
        pVar->dblVal = pVal->aLREAL;
    break;
    case  BL_TOD:
        pVar->vt = VT_I4;
        pVar->lVal = pVal->aTOD;
    break;
    case  BL_DATE:
        pVar->vt = VT_I4;
        pVar->lVal = pVal->aDATE;
    break;
    case  BL_TIME:
        SetVTDecimalVal(pVar, pVal->aTIME);
    break;
    case  BL_DT:
        SetVTDecimalVal(pVar, pVal->aDT);
    break;
    case BL_STRING:
    case BL_WSTRING:
        pVar->vt = VT_BSTR;
        pVar->bstrVal = FC_StringToBSTR(pVal->pszStr);//TODO embedded 0-chars
    break;
    default:
        assert(!"data type not supported");
        VariantInit(pVar);
        return false;
    }

    //check that we are internally consistent:
    assert(pVar->vt == BLTypToVariantTyp(pVal->typ));
    return true;
}



bool VariantToBinVal(
    const VARIANT*       pVar,  
    BL_IEC_TYP           want,
    BINVAL_TYP*          pVal
)
{
    VARIANT  tmp;
    VARTYPE  vtwant = BLTypToVariantTyp(want);

    FreeBinVal(pVal);

    if(pVar->vt != vtwant)
    {
        //force type, convert in place:
        VariantInit(&tmp);
        HRESULT hr = VariantChangeType(&tmp, (VARIANT*)pVar, VARIANT_NOVALUEPROP, vtwant);
        if(FAILED(hr) || vtwant!=tmp.vt)
            return false;
        pVar = &tmp;
    }


    switch(want) 
    {
    case BL_BOOL:
        if(pVar->boolVal)
            pVal->aBOOL = 1;
        else
            pVal->aBOOL = 0;
    break;
    case BL_BYTE:
        pVal->aBYTE = pVar->lVal & 0xff;
    break;
    case BL_WORD:
    case BL_UINT:
        pVal->aWORD = pVar->lVal & 0xffff;
    break;
    case BL_INT:
        pVal->aINT = (short)pVar->lVal;
    break;
    case BL_DWORD:
    case BL_UDINT:
        pVal->aDWORD = pVar->lVal;
    break;
    case BL_DINT: 
        pVal->aDINT = pVar->lVal;
    break;
    case BL_LWORD:    
        GetVTDecimalAsUI64(pVar, &pVal->aLWORD);
    break;
    case BL_LINT:
        GetVTDecimalAsSI64(pVar, &pVal->aLINT);
    break;
    case BL_REAL:
        pVal->aREAL = pVar->fltVal;
    break;
    case BL_LREAL:    
        pVal->aLREAL = pVar->dblVal;
    break;
    case BL_TOD:
        pVal->aTOD = pVar->lVal;
    break;
    case  BL_DATE:
        pVal->aDATE = pVar->lVal;
    break;
    case  BL_TIME:
        GetVTDecimalAsSI64(pVar, &pVal->aTIME);
    break;
    case  BL_DT:
        GetVTDecimalAsSI64(pVar, &pVal->aDT);
    break;
    case BL_STRING:
    case BL_WSTRING:
    {
        FC_CString hack;
        hack.load(pVar->bstrVal);
    
        pVal->typ = want; 
        pVal->nStrlen = hack.len();
        pVal->pszStr = new TCHAR[pVal->nStrlen+1];
        memcpy(pVal->pszStr, hack, (pVal->nStrlen+1)*sizeof(TCHAR));
    }
    break;
    default:
        assert(!"data type not supported");
        if(pVar == &tmp)
            VariantClear(&tmp);
        return false;
    }

    pVal->typ = want; 
    if(pVar == &tmp)
        VariantClear(&tmp);

    return true;
}








void BinValToStrSimle(
    const BINVAL_TYP*    pVal,
    FC_CString*          pStr          //c-like value string
)
{
    TCHAR buffer[256];

    switch(pVal->typ) 
    {
    case BL_BOOL:
        buffer[0] = pVal->aBOOL ? _T('1') : _T('0');
        buffer[1] = 0;
    break;
    case BL_BYTE:
        buffer[0] = '0';
        buffer[1] = 'x';
        _ltot(pVal->aBYTE & 0xff, buffer+2, 16);
    break;
    case BL_WORD:
        buffer[0] = '0';
        buffer[1] = 'x';
        _ltot(pVal->aWORD & 0xffff, buffer+2, 16);
    break;
    case BL_UINT:
        _ultot(pVal->aWORD & 0xffff, buffer, 10);
    break;
    case BL_INT:
        _ltot(pVal->aINT, buffer, 10);
    break;
    case BL_DWORD:
        buffer[0] = '0';
        buffer[1] = 'x';
        _ltot(pVal->aDWORD, buffer+2, 16);
    break;
    case BL_UDINT:
        _ultot(pVal->aDWORD, buffer, 10);
    break;
    case BL_DINT: 
        _ltot(pVal->aDINT, buffer, 10);
    break;
    case BL_LWORD:
        buffer[0] = '0';
        buffer[1] = 'x';
        _ui64tot(pVal->aLWORD, buffer+2, 16);
    break;
    case BL_LINT:
        _ui64tot(pVal->aLINT, buffer, 10);
    break;
    case BL_REAL:
        if(pVal->aREAL != 0.0)
            _stprintf(buffer, _T("%.8e"), (double)pVal->aREAL);                  
        else
            _tcscpy(buffer, _T("0"));
    break;
    case BL_LREAL:    
        if(pVal->aLREAL != 0.0)
            _stprintf(buffer, _T("%.17e"), pVal->aLREAL);
        else
            _tcscpy(buffer, _T("0"));
    break;
    case BL_TOD:
        _ltot(pVal->aTOD, buffer, 10);
    break;
    case  BL_DATE:
        _ltot(pVal->aDATE, buffer, 10);
    break;
    case  BL_TIME:
        _ui64tot(pVal->aTIME, buffer, 10);
    break;
    case  BL_DT:
        _ui64tot(pVal->aDT, buffer, 10);
    break;
    case BL_STRING:
    case BL_WSTRING:
        pStr->load(pVal->pszStr); //HACK
        return;
    break;
    default:
        assert(!"data type not supported");
        pStr->load(_T("<data type not supported>"));
        return;
    }

    pStr->load(buffer);
}




static unsigned long AToULong(const TCHAR* psz, const TCHAR** ppszEnd, bool* pbRange)
{
    unsigned long ulVal;

    errno = 0;

    if(psz[0]== _T('0') && (psz[1]== _T('x') || psz[1]== _T('X')))
        ulVal = _tcstoul(psz, (TCHAR**)ppszEnd, 16);
    else
        ulVal = _tcstoul(psz, (TCHAR**)ppszEnd, 10);

    if(pbRange)
        *pbRange = errno==ERANGE;

    return ulVal;
}
static unsigned long AToLong(const TCHAR* psz, const TCHAR** ppszEnd, bool* pbRange)
{
    long lVal;
    
    errno = 0;

    if(psz[0]== _T('0') && (psz[1]== _T('x') || psz[1]== _T('X')))
        lVal = _tcstoul(psz, (TCHAR**)ppszEnd, 16);
    else
        lVal = _tcstol(psz, (TCHAR**)ppszEnd, 10);

    if(pbRange)
        *pbRange = errno==ERANGE;

    return lVal;
}

static int BL_IsDigit(TCHAR c, int base)
{
    int i;

    switch(c)
    {
    case _T('0'): i=0; break;
    case _T('1'): i=1; break;
    case _T('2'): i=2; break;
    case _T('3'): i=3; break;
    case _T('4'): i=4; break;
    case _T('5'): i=5; break;
    case _T('6'): i=6; break;
    case _T('7'): i=7; break;
    case _T('8'): i=8; break;
    case _T('9'): i=9; break;
    case _T('a'): case _T('A'): i=10; break;
    case _T('b'): case _T('B'): i=11; break;
    case _T('c'): case _T('C'): i=12; break;
    case _T('d'): case _T('D'): i=13; break;
    case _T('e'): case _T('E'): i=14; break;
    case _T('f'): case _T('F'): i=15; break;
    default:
        return -1;
    }

    return i>=base ? -1 : i;
}
static HRESULT /*BL_*/ParseInt64( //HACK+ TODO taken from baselib, the baselib should export also Ansi functions !
    const TCHAR*    psz, 
    __int64*        pOut, 
    bool            bAllowSign,
    int             radix,
    const TCHAR**   pszEnd
)
{
    unsigned __int64 result = 0;
    bool        negative = false;
    unsigned __int64 limit = _UI64_MAX;
    unsigned __int64 multmax;
    int         digit;
    HRESULT     hr=S_OK;


    if(psz==0 || *psz==0)
    {
        hr = E_BL_ERR_SYNTAX;
        goto error;
    }

    if(bAllowSign)
    {
        limit = _I64_MAX;
	    if(*psz == _T('-')) 
        {
	        negative = true;
	        limit = -_I64_MIN;
	        psz++;
	    }
        else if(*psz == _T('+'))
        {
	        psz++;
        }
    }
	

    multmax = limit / radix;

    //first digit is required:
    if ((digit = BL_IsDigit(*psz,radix))>=0) 
    {
        psz++;
        result = digit;
	}
    else
    {
        hr = E_BL_ERR_SYNTAX;    
        goto error;
    }
	
    while(1) 
    {
#if 0  //don't want _ syntax here! 
	    if(*psz==_T('_'))
        {
            psz++;

            //after an opt '_' a digit must follow:
            if(BL_IsDigit(*psz,radix)<0)
            {
                hr = E_BL_ERR_SYNTAX;    
                goto error;
            }

        }
#endif
        if((digit = BL_IsDigit(*psz,radix))>=0)
        {
	        psz++;
	        
            //if pOut==NULL syntax check only
        	if(pOut)
            {
                // Accumulating negatively avoids surprises near MAX_VALUE
                if (result > multmax) 
                {
                    hr = E_BL_ERR_RANGE;    
                    //stop accumulating, continue syntax check only, 
                    //syntax error will override this range error
                    pOut = NULL;
	            }

                result *= radix;
	            
                if (result > limit - digit) 
                {
                    hr = E_BL_ERR_RANGE;    
                    //stop accumulating, continue syntax check only, 
                    //syntax error will override this range error
                    pOut = NULL;
	            }
            	        
                result += digit;
            }
        }
        else
        {
            //stop scan now:
            break;
        }
	}

error:
    if(pszEnd)
        *pszEnd = psz;
	if(pOut)
        *pOut= negative ? -(__int64)result : result;
    
    return hr;
}
static unsigned __int64 AToUI64(const TCHAR* psz, const TCHAR** ppszEnd, bool* pbRange)
{
    __int64 i64;
    HRESULT hr;
    
    if(psz[0]== _T('0') && (psz[1]== _T('x') || psz[1]== _T('X')))
        hr = ParseInt64(psz+2,  &i64, false, 16, ppszEnd);
    else
        hr = ParseInt64(psz,  &i64, true, 10, ppszEnd);

    if(*pbRange)
        *pbRange = hr == E_BL_ERR_RANGE;
    return (unsigned __int64)i64;
}
static __int64 AToI64(const TCHAR* psz, const TCHAR** ppszEnd, bool* pbRange)
{
    __int64 i64;
    HRESULT hr;
    
    if(psz[0]== _T('0') && (psz[1]== _T('x') || psz[1]== _T('X')))
        hr = ParseInt64(psz+2,  &i64, false, 16, ppszEnd);
    else
        hr = ParseInt64(psz,  &i64, true, 10, ppszEnd);

    if(pbRange)
        *pbRange = hr == E_BL_ERR_RANGE;
    return i64;
}


bool StrSimleToBinVal(
    const TCHAR*   psz,
    BL_IEC_TYP     want,
    BINVAL_TYP*    pVal,
    TCHAR          szErrDescr[MAX_PATH]
)
{
    const TCHAR*   pszEnd     = _T("");
    bool           bWarnRange = false;
    long           l;
    unsigned long  ul;


    FreeBinVal(pVal);
    if(szErrDescr)
        szErrDescr[0] = 0;

    if(want == BL_STRING || want ==BL_WSTRING)
    {
        pVal->typ = want;    
        pVal->nStrlen = _tcslen(psz);
        pVal->pszStr = new TCHAR[pVal->nStrlen+1];
        memcpy(pVal->pszStr, psz, (pVal->nStrlen+1)*sizeof(TCHAR));
        return true;
    }

    FC_StringSkipWhite(&psz);

    switch(want) 
    {
    case BL_BOOL:
        if(*psz==_T('1'))
        {
            pVal->aBOOL = 1;
            pszEnd = psz+1;
        }
        else if(*psz==_T('0'))
        {
            pVal->aBOOL = 0;
            pszEnd = psz+1;
        }
        else
            pszEnd = psz;
    break;
    case BL_BYTE:
        ul = AToULong(psz, &pszEnd, &bWarnRange);
        pVal->aBYTE = (BYTE) (ul & 0xff);
        bWarnRange = bWarnRange || ul != pVal->aBYTE;
    break;
    case BL_WORD:
	case BL_UINT:
        ul = AToULong(psz, &pszEnd, &bWarnRange);
        pVal->aWORD = (WORD)(ul & 0xffff);
        bWarnRange = bWarnRange || ul != pVal->aWORD;
    break;
    case BL_INT:
        l = AToLong(psz, &pszEnd, &bWarnRange);
        pVal->aINT = (short)l;
        bWarnRange = bWarnRange || l>SHRT_MAX || l<SHRT_MIN;
    break;
    case BL_DWORD:
    case BL_UDINT:
        pVal->aDWORD = AToULong(psz, &pszEnd, &bWarnRange);
    break;
    case BL_DINT: 
        pVal->aDINT = AToLong(psz, &pszEnd, &bWarnRange);
    break;
    case BL_LWORD:
        pVal->aLWORD = AToUI64(psz, &pszEnd, &bWarnRange);
    break;
    case BL_LINT:
        pVal->aLINT = AToI64(psz, &pszEnd, &bWarnRange);
    break;
    case BL_REAL:
        pVal->aREAL = (float)_tcstod(psz, (TCHAR**)&pszEnd);
    break;
    case BL_LREAL:    
        pVal->aLREAL = _tcstod(psz, (TCHAR**)&pszEnd);
    break;
    case BL_TOD:
        pVal->aTOD = AToLong(psz, &pszEnd, &bWarnRange);
    break;
    case  BL_DATE:
        pVal->aDATE = AToLong(psz, &pszEnd, &bWarnRange);
    break;
    case  BL_TIME:
        pVal->aTIME = AToI64(psz, &pszEnd, &bWarnRange);
    break;
    case  BL_DT:
        pVal->aDT = AToI64(psz, &pszEnd, &bWarnRange);
    break;
    default:
        if(szErrDescr)
        {
            _sntprintf(szErrDescr, MAX_PATH, _T("data type '%s' not supported"), 
                BLTypToString(want));
            szErrDescr[MAX_PATH-1] = 0;
        }
        return false;
    }


    FC_StringSkipWhite(&pszEnd);
    if(*pszEnd || pszEnd == psz)
    {
        if(szErrDescr)
        {
            if(pszEnd == psz)
                _sntprintf(szErrDescr, MAX_PATH, _T("empty string or only white space"));
            else
                _sntprintf(szErrDescr, MAX_PATH, _T("parse error at '%s'"), pszEnd);

            szErrDescr[MAX_PATH-1] = 0;
        }
        pVal->aLWORD = 0;
        return false;
    }


    if(bWarnRange)
    {
        if(szErrDescr)
        {
            _sntprintf(szErrDescr, MAX_PATH, _T("out of range of type '%s'"), 
                BLTypToString(want));
            szErrDescr[MAX_PATH-1] = 0;
        }
    }

    pVal->typ = want;    

    return true;
}

