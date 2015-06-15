/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_baselib/baselib.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: baselib.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_baselib/baselib.cpp $
 *
 * =PROJECT             7VH102  ATCMControl
 *
 * =SWKE                CMN
 *
 * =COMPONENT           baselib
 *
 * =CURRENT             $Date: 28.02.07 19:00 $
 *                      $Revision: 1 $
 *
 * =ENVIRONMENT         Win32:
 *                      MS Windows NT 4.0
 *                      MSVC++ V5.0 / MFC 4.2 / ATL 2.0 / DAO 3.5
 *                      MS Java 2.0
 *                      MS Visual SourceSafe 5.0
 *
 * =REFERENCES
 *==
 *----------------------------------------------------------------------------*
 *

 

 

 *==
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATIONS
 * $History: baselib.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_baselib
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 13.06.06   Time: 15:25
 * Updated in $/4Control/COM/softing/fc/CMN/fc_baselib
 * implemented UINT, UDINT
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_baselib
 * 
 * *****************  Version 9  *****************
 * User: Ca           Date: 12.09.01   Time: 14:23
 * Updated in $/4Control/COM/softing/fc/CMN/fc_baselib
 * 
 * *****************  Version 8  *****************
 * User: Ca           Date: 10.09.01   Time: 16:28
 * Updated in $/4Control/COM/softing/fc/CMN/fc_baselib
 * 
 * *****************  Version 7  *****************
 * User: Ca           Date: 10.09.01   Time: 12:11
 * Updated in $/4Control/COM/softing/fc/CMN/fc_baselib
 * 
 * *****************  Version 6  *****************
 * User: Ca           Date: 10.09.01   Time: 10:45
 * Updated in $/4Control/COM/softing/fc/CMN/fc_baselib
 * 
 * *****************  Version 5  *****************
 * User: Ca           Date: 7.09.01    Time: 16:49
 * Updated in $/4Control/COM/softing/fc/CMN/fc_baselib
 * 
 * *****************  Version 4  *****************
 * User: Die          Date: 18.06.01   Time: 21:16
 * Updated in $/4Control/COM/softing/fc/CMN/fc_baselib
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 26.04.01   Time: 18:19
 * Updated in $/4Control/COM/softing/fc/CMN/fc_baselib
 * new types: INT,REAL,STRING
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 26.04.01   Time: 16:09
 * Updated in $/4Control/COM/softing/fc/CMN/fc_baselib
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 23.01.01   Time: 17:12
 * Created in $/4Control/COM/softing/fc/CMN/fc_baselib
 * utils and baselib from V1.3
 * 
 * *****************  Version 37  *****************
 * User: Ed           Date: 6/27/00    Time: 4:02p
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * 
 * *****************  Version 36  *****************
 * User: Ed           Date: 6/26/00    Time: 4:48p
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * add missing QVT_TYPEs
 * 
 * *****************  Version 35  *****************
 * User: Ed           Date: 31.03.00   Time: 14:51
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * dont strip leading whitespace on strings
 * 
 * *****************  Version 34  *****************
 * User: Ed           Date: 23.11.99   Time: 17:25
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * remove funny hack code
 * 
 * *****************  Version 33  *****************
 * User: Ed           Date: 9.11.99    Time: 13:48
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * fixed the following bug:
 * 
 * Die Routine 	baselib.cpp::BL_IECStrToDouble
 * 
 * liefert für alle Strings beginnend mit "lreal#" den konvertierten Wert
 * 0.0 und
 * läuft in Zeile 2068 aufs        assert(*pszE2==0);
 * 
 * 
 * *****************  Version 32  *****************
 * User: Jd           Date: 17.08.99   Time: 14:36
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * Changed variant to string : #tMIN_TIME -> #t-98... and not -#t98...
 * 
 * *****************  Version 31  *****************
 * User: Rf           Date: 28.07.99   Time: 15:37
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * Forbid padding '0' for string formats
 * 
 * *****************  Version 30  *****************
 * User: Rf           Date: 20.07.99   Time: 18:48
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * Fixed some bugs in string format handling
 * 
 * *****************  Version 29  *****************
 * User: Rf           Date: 15.06.99   Time: 18:07
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * base 2 and base 8 formats
 * 
 * *****************  Version 28  *****************
 * User: Rf           Date: 20.04.99   Time: 19:07
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * Implementation of formats
 * 
 * *****************  Version 27  *****************
 * User: Rf           Date: 18.03.99   Time: 14:47
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * bug in LINT->string conversion
 * 
 * *****************  Version 26  *****************
 * User: Ef           Date: 3/17/99    Time: 3:46p
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * added source comment
 * 
 * *****************  Version 25  *****************
 * User: Ef           Date: 3/17/99    Time: 11:26a
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * Changed comment of ST_ParseInt64
 * 
 * *****************  Version 24  *****************
 * User: Ed           Date: 16.03.99   Time: 18:42
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * 
 * *****************  Version 23  *****************
 * User: Ed           Date: 16.03.99   Time: 18:03
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * add LINT / LWORD types
 * 
 * *****************  Version 22  *****************
 * User: Ed           Date: 3.12.98    Time: 14:47
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * removed stdafx.h (weil es so besser ist)
 * 
 * *****************  Version 21  *****************
 * User: Je           Date: 9.11.98    Time: 18:41
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * * added XML encoding/decoding functions
 * * added CString from statically linked MFC lib
 * 
 * *****************  Version 20  *****************
 * User: Ef           Date: 20.08.98   Time: 15:40
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * fixed d-899
 * 
 * *****************  Version 19  *****************
 * User: Ef           Date: 14.08.98   Time: 16:58
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * fixed  bug with -t#5ms
 * 
 * *****************  Version 18  *****************
 * User: Je           Date: 5.08.98    Time: 16:50
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * * replaced all WCHAR Win32-API string functions with CRT equivalents
 * (lstrcpyW() etc. are NOT supported under Win95!!)
 * 
 * *****************  Version 17  *****************
 * User: Rf           Date: 31.07.98   Time: 13:40
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * Fixed rounding error
 * 
 * *****************  Version 16  *****************
 * User: Ef           Date: 29.07.98   Time: 14:50
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * fixed assertion if exponent  >100
 * 
 * *****************  Version 15  *****************
 * User: Ed           Date: 17.07.98   Time: 12:44
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * try to fix W95-Problems
 * 
 * *****************  Version 14  *****************
 * User: Ed           Date: 13.07.98   Time: 17:53
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * dies und das geaendert
 * 
 * *****************  Version 13  *****************
 * User: Je           Date: 8.07.98    Time: 18:47
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * * moved BaseLib COM wrapper into BaseLib itself
 * * CheckString w/o HRESULT error if baselib syntax error
 * 
 * *****************  Version 12  *****************
 * User: Je           Date: 8.07.98    Time: 11:22
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * * COM wrapper as alternate access to exported functions
 * 
 * *****************  Version 11  *****************
 * User: Je           Date: 7.07.98    Time: 16:39
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * * added BL_TypenameToDatatype
 * 
 * *****************  Version 10  *****************
 * User: Ef           Date: 6/26/98    Time: 7:20p
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * more data types
 * 
 * *****************  Version 9  *****************
 * User: Ef           Date: 6/24/98    Time: 7:44p
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * implemented ANY_DATE types
 * 
 * *****************  Version 8  *****************
 * User: Ef           Date: 6/22/98    Time: 8:12p
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * new types DT,TOD,DATE
 * 
 * *****************  Version 7  *****************
 * User: Ef           Date: 6/18/98    Time: 7:03p
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * implemented TIME
 * 
 * *****************  Version 6  *****************
 * User: Su           Date: 6/05/98    Time: 4:11p
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * Fixed a bug in BL_VariantToString - VariantChangeType will fail with
 * error DISP_E_BADVARTYPE if destination variant v is not initialized.
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 5/22/98    Time: 3:27p
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * implemented time to variant
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 5/13/98    Time: 9:05p
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * implem time to variant
 * 
 * *****************  Version 3  *****************
 * User: Ed           Date: 7.05.98    Time: 16:58
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * 
 * *****************  Version 2  *****************
 * User: Ed           Date: 5.05.98    Time: 11:48
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * 1. revision
 * 
 * *****************  Version 1  *****************
 * User: Ed           Date: 4.05.98    Time: 13:46
 * Created in $/4Control/COM/softing/fc/CMN/baselib
 * created
 *==
 ******************************************************************************
H<<*/


#define BASELIB_API __declspec(dllexport)
#define UNICODE 1
#define _UNICODE 1

// do not use PCH-File here, because UNICODE-DEFINES are different here.
// GLAUBT MIR'S EINFACH, ES MACHT EINEN UNTERSCHIED! (ED).

#include <windows.h>
#include <tchar.h>
#include <malloc.h>
#include <limits.h>
#include <stdio.h>  //sprintf
#include <math.h>   //fabs
#include <float.h>  //FLT_MAX
#include <assert.h>
#include "fc_tools\fc_string.h"
#include "baselib.h"


struct BL_StringBuffer
{
    LPTSTR       m_pszText;
    unsigned int m_uiSize;
    unsigned int m_uiLength;
};



//local helper functions:
static inline LPCTSTR BL_StripPrefixI(LPCTSTR pszInput, LPCTSTR pszPrefix)
{
    size_t s = _tcslen(pszPrefix);
    
    if(0==_tcsncicmp(pszInput, pszPrefix, s))
        return pszInput + s;
    else
        return pszInput;
}
static inline bool BL_StringEqualI(LPCTSTR psz1, LPCTSTR psz2)
{
    return 0==_tcsicmp(psz1, psz2);
}
static inline int BL_IsDigitBase10(TCHAR c)
{
    return _istdigit(c) ? (c - _T('0')) : -1;
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

static inline LPCTSTR BL_StripWhiteSpace(LPCTSTR psz)
{
    while(psz && *psz && _istspace(*psz))
        psz++;
    
    return psz;
}
static bool BL_IsOnlyWhiteSpace(LPCTSTR psz);
static bool UTCJavaToUTCWin32(__int64 UTCJava, FILETIME* pFileTime);
static __int64 ScaleFileTimeToJavaUTCmillis(const FILETIME* pFileTime);
static HRESULT MakeVT_Decimal(VARIANTARG* pDest, VARIANTARG* pSrc, __int64* pi64);
static HRESULT GetTransitionFileTimeFromSystime(const SYSTEMTIME* pTransSpec, 
                                                WORD wYearInQuestion,
                                                FILETIME* pTransTime);
static HRESULT IsInDayLightSavingPeriod(const SYSTEMTIME* pSystime, 
                                        bool* pIsInDST,
                                        const TIME_ZONE_INFORMATION* pTzi);
static HRESULT BL_UTCJavaToLocalSysTime(__int64 UTCmillis, SYSTEMTIME* lpSystime);
static HRESULT BL_LocalSysTimeToUTCJava(const SYSTEMTIME* lpSystime, __int64* pUTCmillis);
static HRESULT BL_IECStrToDouble(LPCTSTR szInput, double* lpValue, BL_IEC_TYP typ, LPCTSTR* pszEnd);

static HRESULT BL_IECStrToBool(LPCTSTR szInput, bool *pbResult, LPCTSTR* pszEnd);
static HRESULT BL_IECStrToAnyIntegral(LPCTSTR szInput, 
                                      __int64* pl64,
                                      BL_IEC_TYP typ,
                                      LPCTSTR* pszEnd);
static HRESULT BL_DoubleToStrDefault (LPTSTR pcOut, UINT wBufLen, double dWert);
static HRESULT BL_TimeToIECStrDefault(__int64 i64Value, LPTSTR buffer, int nChars);
static HRESULT BL_DateAndTimeToIECStr(__int64 i64Value, LPTSTR buffer, int nChars);
static HRESULT BL_DateToIECStr(long lDaysSince1970, LPTSTR buffer, int nChars);
static HRESULT BL_TimeOfDayToIECStr(long lMillis, LPTSTR buffer, int nChars);

static void GetFormatInfoDefaults (enum BL_IEC_TYP type, struct BL_FormatInfo *pFormatInfo);
static HRESULT CheckFormatInfo (enum BL_IEC_TYP type, const struct BL_FormatInfo *pFormatInfo);

static HRESULT FormatInteger (__int64 llValue, enum BL_IEC_TYP type, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer);
static HRESULT FormatReal (float fltValue, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer);
static HRESULT FormatLReal (double dblValue, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer);
static HRESULT FormatString (LPCTSTR pszText, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer);
static HRESULT FormatIntegerAsInteger (__int64 llValue, int iTypeSize, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer);
static HRESULT FormatIntegerAsReal (__int64 llValue, int iTypeSize, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer);
static HRESULT FormatIntegerAsString (__int64 llValue, int iTypeSize, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer);
static HRESULT FormatStringAsInteger (LPCWSTR pszText, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer);
static HRESULT FormatStringAsReal (LPCWSTR pszText, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer);
static HRESULT FormatStringAsString (LPCWSTR pszText, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer);
static HRESULT DoPadding (BL_StringBuffer *pBuffer, LPCWSTR pszText, const struct BL_FormatInfo *pFormatInfo);

static void InitStringBuffer (struct BL_StringBuffer *pBuffer);
static void CleanupStringBuffer (struct BL_StringBuffer *pBuffer);
static bool GrowByString (struct BL_StringBuffer *pBuffer, LPCWSTR pszString);
static bool GrowByCharacter (struct BL_StringBuffer *pBuffer, WCHAR ch, size_t uiCount);
static bool CheckSpace (struct BL_StringBuffer *pBuffer, size_t uiCount);


//------------------------------------------------------------------*
/**
 * BL_StringToVariant
 *
 * @param           value_in    in parameter, zero-terminated
 * @param           typ         may be BL_ANY if not known
 * @param           format      may be NULL or "" if not known
 * @param           options     BL_USE_WINDOWS_FORMAT||BL_USE_IEC_FORMAT
 * @param           out         out parameter
 * @return          
 * @exception       -
 * @see             
*/
extern "C"
HRESULT BASELIB_API __stdcall BL_StringToVariant(
                                 LPCOLESTR value_in,   // in parameter, zero-terminated
                                 BL_IEC_TYP typ,    // may be BL_ANY if not known
                                 LPCOLESTR format,  // may be NULL or "" if not known
                                 DWORD options,     // BL_USE_WINDOWS_FORMAT||BL_USE_IEC_FORMAT
                                 VARIANT *out)      // out parameter
{
    HRESULT  hr;
    LPCTSTR  pszEnd = NULL;
    LPCTSTR  value;

    
    VariantInit(out);

    //currently we don't support 'don't know Datatype'
    if(typ==BL_ANY)
        return E_BL_ERR_INVALIDARG;

    if(value_in == NULL)
        return E_BL_ERR_INVALIDARG;

    // format string not supported yet !!
    if (   (format != NULL)
        && (wcslen(format) > 0))
    {
        return E_BL_ERR_FORMATERROR;
    }


    //strip leading whitespace:
    value = BL_StripWhiteSpace(value_in);

    switch(typ)
    {
    case BL_BOOL:
        bool aBool;
        hr = BL_IECStrToBool(value, &aBool, &pszEnd);
        if (SUCCEEDED(hr))
        {
            out->boolVal = aBool ? VARIANT_TRUE : VARIANT_FALSE;
            out->vt = VT_BOOL;
        }
    break;
    case BL_BYTE:
    case BL_WORD:
    case BL_DWORD:
    case BL_INT:
    case BL_DINT:
    case BL_UINT:
    case BL_UDINT:
        __int64 val64;
        hr = BL_IECStrToAnyIntegral(value, &val64, typ, &pszEnd);
        if (SUCCEEDED(hr))
        {
            out->vt = VT_I4;
            out->lVal = (long)val64;
        }
    break;
    case BL_LWORD:
    case BL_LINT:
    case BL_ULINT:
        hr = BL_IECStrToAnyIntegral(value, &val64, typ, &pszEnd);
        if (SUCCEEDED(hr))
        {
            out->vt = VT_DECIMAL;
            
            if (val64>=0)
            {
              out->decVal.scale = 0;
              out->decVal.sign = 0;
              out->decVal.Hi32 = 0;
              out->decVal.Lo64 = val64;
            }
            else
            {
              out->decVal.scale = 0;
              out->decVal.sign = DECIMAL_NEG;
              out->decVal.Hi32 = 0;
              out->decVal.Lo64 = -val64;                
            }
        }
    break;
    case BL_REAL:
    case BL_LREAL:
        hr = BL_IECStrToDouble(value, &out->dblVal, typ, &pszEnd);
        if (SUCCEEDED(hr))
        {
            out->vt = VT_R8;
        }
    break;
    case BL_TIME:
    {
        __int64 l64time; 
        bool bNegative = false;
        if(*value==_T('+'))
        {
            value++;
        }
        else if(*value==_T('-'))
        {
            value++;
            bNegative = true;
        }

        hr = BL_IECTimeStrToMillis(value, &l64time, &pszEnd);
        
        if(bNegative && SUCCEEDED(hr))
        {
            if(!BL_safe_sub_int64(&l64time, 0, l64time))
                hr = E_BL_ERR_RANGE;
        }   
       
        if (SUCCEEDED(hr))
        {
            out->vt = VT_DECIMAL;
            
            if (l64time>=0)
            {
              out->decVal.scale = 0;
              out->decVal.sign = 0;
              out->decVal.Hi32 = 0;
              out->decVal.Lo64 = l64time;
            }
            else
            {
              out->decVal.scale = 0;
              out->decVal.sign = DECIMAL_NEG;
              out->decVal.Hi32 = 0;
              out->decVal.Lo64 = -l64time;                
            }
        }
    }        
    break;
    case BL_DT:
    {
        __int64 l64time; 
        SYSTEMTIME systime;
        hr = BL_IECDateAndTimeStrToUTCJava(value, &systime, &l64time, &pszEnd);
        if (SUCCEEDED(hr))
        {
            //assert: we have set the MIN/MAX values of DATE/DT so that no neg. int64 
            //can be produced by the convertion, an 'out of range' error
            //occurs instead
            assert(l64time>=0);

            out->vt = VT_DECIMAL;
            out->decVal.scale = 0;
            out->decVal.sign = 0;
            out->decVal.Hi32 = 0;
            out->decVal.Lo64 = l64time;
        }
    }
    break;
    case BL_DATE:
    {
        long ldaysSince1970; 
        SYSTEMTIME systime;
        hr = BL_IECDateStrToDaysSince1970(value, &systime, &ldaysSince1970, &pszEnd);
            
        if (SUCCEEDED(hr))
        {
            out->vt   = VT_I4;
            out->lVal = ldaysSince1970;
        }
    }
    break;
    case BL_TOD:
    {
        long lMillis; 
        SYSTEMTIME systime;
        hr = BL_IECTimeOfDayStrToMillis(value, &systime, &lMillis, &pszEnd);
            
        if (SUCCEEDED(hr))
        {
            out->vt   = VT_I4;
            out->lVal = lMillis;
        }
    }
    break;
    case BL_STRING:
    case BL_WSTRING:
        out->bstrVal = SysAllocString(value_in);
        if (out->bstrVal != NULL)
          {
            out->vt = VT_BSTR;
            hr = S_OK;
          }
        else
          {
            hr = E_OUTOFMEMORY;
          }
    break;
    default:
        hr = E_BL_ERR_INVALIDARG;
    }


    //ckeck that if *pszEnd!=0 there is only trailing whitespace:
    if(SUCCEEDED(hr))
    {
        if(pszEnd && *pszEnd && !BL_IsOnlyWhiteSpace(pszEnd))
            hr = E_BL_ERR_SYNTAX;
    }

    return hr;
}


//------------------------------------------------------------------*
/**
 * BL_StringToVariantNamed
 *  convert to VARIANT using IEC datatype name string
 *
 * @param           sValue      in parameter, zero-terminated
 * @param           sType       namestring of datatype
 * @param           sFormat     may be NULL or "" if not known
 * @param           options     BL_USE_WINDOWS_FORMAT||BL_USE_IEC_FORMAT
 * @param           pvtOut      out parameter
 * @return          
 * @exception       -
 * @see             
*/
extern "C"
HRESULT BASELIB_API __stdcall BL_StringToVariantNamed(
                                 LPCOLESTR sValue,      // in parameter, zero-terminated
                                 LPCOLESTR sType,       // namestring of datatype
                                 LPCOLESTR sFormat,     // may be NULL or "" if not known
                                 DWORD options,         // BL_USE_WINDOWS_FORMAT||BL_USE_IEC_FORMAT
                                 VARIANT *pvtOut)       // out parameter
{
	// convert from datatype name to baselibs enum:
    DWORD   dwMod = 0;
    enum BL_IEC_TYP type;
    HRESULT hr;
    hr = ::BL_TypenameToDatatype(sType, &type, &dwMod);
    if (FAILED(hr)) {
        return hr;
    }
    hr = ::BL_StringToVariant(sValue, type, sFormat, options, pvtOut);

	return hr;
}

//------------------------------------------------------------------*
/**
 * BL_VariantToStringNamed
 *  convert to string using IEC datatype name string
 *
 * @param           pvtIn               in value
 * @param           sType               namestring of datatype
 * @param           sFormat             may be NULL or "" if not known
 * @param           options             BL_USE_WINDOWS_FORMAT||BL_USE_IEC_FORMAT
 * @param           lMaxCharsWanted     number of characters wanted in output string
 * @param           psOut               [out] formatted string, released by caller
 * @return          
 * @exception       -
 * @see             
*/
extern "C"
HRESULT BASELIB_API __stdcall BL_VariantToStringNamed(
                                 VARIANT *pvtIn,        // in value
                                 LPCOLESTR sType,       // namestring of datatype
                                 LPCOLESTR sFormat,     // may be NULL or "" if not known
                                 DWORD options,         // BL_USE_WINDOWS_FORMAT||BL_USE_IEC_FORMAT
                                 long lMaxCharsWanted,  // number of characters wanted in output string
                                 BSTR *psOut)           // formatted string
{
    if (psOut == NULL) {
        return E_POINTER;
    }
    *psOut = NULL;

	// convert from datatype name to baselibs enum:
    DWORD   dwMod = 0;
    enum BL_IEC_TYP type;
    HRESULT hr;
    hr = ::BL_TypenameToDatatype(sType, &type, &dwMod);
    if (FAILED(hr)) {
        return hr;
    }
    size_t maxLen = lMaxCharsWanted?lMaxCharsWanted+1:_MAX_PATH;
    LPOLESTR pszBuf = ::SysAllocStringLen(NULL, maxLen);
    if (pszBuf == NULL) {
        return E_OUTOFMEMORY;
    }
    hr = ::BL_VariantToString(pvtIn, type, sFormat, options, pszBuf, maxLen);
    if (SUCCEEDED(hr)) {
        // create copy with correct length set !
        *psOut = ::SysAllocStringLen(pszBuf, wcslen(pszBuf));
        if (*psOut == NULL) {
            ::SysFreeString(pszBuf);
            return E_OUTOFMEMORY;
        }
    }
    ::SysFreeString(pszBuf);
	return hr;
}


//------------------------------------------------------------------*
/**
 * BL_VariantToString
 *
 * @param           value       in parameter
 * @param           typ         may be BL_ANY if not known
 * @param           format      may be NULL or "" if not known
 * @param           options     BL_USE_WINDOWS_FORMAT||BL_USE_IEC_FORMAT
 * @param           out         formatted string
 * @param           nChars      sizeof(out)/sizeof(OLECHAR)
 * @return          
 * @exception       -
 * @see             
*/
extern "C"
HRESULT BASELIB_API __stdcall BL_VariantToString(
                                 VARIANT *value,    // in parameter
                                 BL_IEC_TYP typ,    // may be BL_ANY if not known
                                 LPCOLESTR format,  // may be NULL or "" if not known
                                 DWORD options,     // BL_USE_WINDOWS_FORMAT||BL_USE_IEC_FORMAT
                                 LPOLESTR out,      // formatted string
                                 UINT nChars)       // sizeof(out)/sizeof(OLECHAR)
{
    VARIANT v;
    HRESULT hr;
    __int64 l64utc;
    BL_FormatInfo formatInfo;
    __int64 llValue;
    
    if (typ == BL_ANY)
        switch(value->vt)
        {
        case VT_R4: case VT_R8:
            typ = BL_LREAL; break;
        case VT_UI1: case VT_I2: case VT_I4:
            typ = BL_DINT; break;
        case VT_DATE: typ = BL_DT; break;
        case VT_BOOL: typ = BL_BOOL; break;
        case VT_BSTR: typ = BL_WSTRING; break;
        case VT_DECIMAL: typ = BL_LINT; break;
        }

    if (format == NULL)
    {
        hr = BL_ParseFormatForType (L"", typ, &formatInfo);
    }
    else
    {
        hr = BL_ParseFormatForType (format, typ, &formatInfo);
    };
    if (FAILED (hr))
    {
        return (hr);
    };

    /*	SU VariantChangeType will fail with error DISP_E_BADVARTYPE 
	    if destination variant v is not initialized - call VariantInit.
    */
    VariantInit(&v);

    switch(typ)
    {
    default:
    case BL_STRING:
    case BL_WSTRING:
        if (format == NULL)
        {
            hr = VariantChangeType(&v, value, 0, VT_BSTR);
            if (SUCCEEDED(hr))
            { 
                if(SysStringLen(v.bstrVal) >= nChars)
                {
                    hr = E_BL_ERR_BUFFER_TO_SMALL;
                }
                else
                {
                    wcsncpy(out, v.bstrVal, nChars);
                }
            }
        }
        else
        {
            hr = VariantChangeType(&v, value, 0, VT_BSTR);
            if (SUCCEEDED(hr))
            {
                struct BL_StringBuffer buffer;

                InitStringBuffer (&buffer);

                hr = FormatString (v.bstrVal, &formatInfo, &buffer);
                if (FAILED (hr))
                {
                    return (hr);
                };

                if ((UINT)buffer.m_uiLength >= nChars)
                {
                    hr = E_BL_ERR_BUFFER_TO_SMALL;
                }
                else
                {
                    if (buffer.m_pszText == NULL)
                    {
                        *out = L'\0';
                    }
                    else
                    {
                        wcscpy (out, buffer.m_pszText);
                    };
                    hr = S_OK;
                };

                CleanupStringBuffer (&buffer);
            };
        };
    break;

    case BL_BOOL:
        hr = VariantChangeType(&v, value, VARIANT_ALPHABOOL, VT_BSTR);
        if (SUCCEEDED(hr))
        {
            wcsncpy(out, v.bstrVal, nChars);
            hr = S_OK;
        }
    break;

    case BL_DINT: case BL_LINT:
    //case BL_SINT: 
    case BL_INT:
    //case BL_USINT: 
    case BL_UINT: case BL_UDINT: case BL_ULINT:
    case BL_BYTE: case BL_WORD: case BL_DWORD: case BL_LWORD:
        if (format == NULL)
        {
            TCHAR   buffer[64];

            switch (typ)
            {
            case BL_INT:
            case BL_DINT:
            // case BL_SINT:
            // case BL_USINT: 
            case BL_UINT: case BL_UDINT:
                hr = VariantChangeType(&v, value, 0, VT_I4);

                _stprintf(buffer, _T("%ld"), v.lVal);
                if((UINT)_tcslen(buffer) >= nChars)
                {
                     hr = E_BL_ERR_BUFFER_TO_SMALL;
                }
                else
                {
                    wcsncpy(out, buffer, nChars);
                    hr= S_OK;
                }
                break;

            case BL_LINT:
            case BL_ULINT:
                __int64 i64Value;
                hr = MakeVT_Decimal(&v, value, &i64Value);
                if(SUCCEEDED(hr))
                {
                  if (i64Value<0)
                    *buffer = _T('-'), _ui64tot((unsigned __int64)-i64Value, buffer+1, 10);
                  else
                    _ui64tot((unsigned __int64)i64Value, buffer, 10);

                  if((UINT)_tcslen(buffer) >= nChars)
                  {
                       hr = E_BL_ERR_BUFFER_TO_SMALL;
                  }
                  else
                  {
                      wcsncpy(out, buffer, nChars);
                      hr= S_OK;
                  }
                }
                break;

            case BL_BYTE: case BL_WORD: case BL_DWORD:
                hr = VariantChangeType(&v, value, 0, VT_I4);

                _stprintf(buffer, _T("16#%lx"), v.lVal);
                if((UINT)_tcslen(buffer) >= nChars)
                {
                     hr = E_BL_ERR_BUFFER_TO_SMALL;
                }
                else
                {
                    wcsncpy(out, buffer, nChars);
                    hr= S_OK;
                }
                break;

            case BL_LWORD:
                hr = MakeVT_Decimal (&v, value, &i64Value);
                if (SUCCEEDED (hr))
                {
                    if ((__int32)(i64Value >> 32) == 0)
                    {
                        _stprintf (buffer, _T ("16#%lx"), (__int32)i64Value);
                    }
                    else
                    {
                        _stprintf (buffer, _T ("16#%lx%08lx"), (__int32)(i64Value >> 32), (__int32)i64Value);
                    };
                    if ((UINT)_tcslen (buffer) >= nChars)
                    {
                         hr = E_BL_ERR_BUFFER_TO_SMALL;
                    }
                    else
                    {
                        wcsncpy (out, buffer, nChars);
                        hr = (S_OK);
                    };
                };
                break;
            };
        }
        else
        {
            if (typ == BL_LINT || /*type == BL_ULINT ||*/ typ == BL_LWORD)
            {
                hr = MakeVT_Decimal (&v, value, &llValue);
            }
            else
            {
                hr = VariantChangeType (&v, value, 0, VT_I4);
                llValue = v.lVal;
            };

            if (SUCCEEDED (hr))
            {
                struct BL_StringBuffer buffer;

                InitStringBuffer (&buffer);

                hr = FormatInteger (llValue, typ, &formatInfo, &buffer);
                if (FAILED (hr))
                {
                    return (hr);
                };

                if ((UINT)buffer.m_uiLength >= nChars)
                {
                    hr = E_BL_ERR_BUFFER_TO_SMALL;
                }
                else
                {
                    if (buffer.m_pszText == NULL)
                    {
                        *out = L'\0';
                    }
                    else
                    {
                        wcscpy (out, buffer.m_pszText);
                    };
                    hr = S_OK;
                };

                CleanupStringBuffer (&buffer);
            };
        };
    break;
    case BL_REAL:
    case BL_LREAL:
        if (format == NULL)
        {
            hr = VariantChangeType (&v, value, 0, VT_R8);
            if (SUCCEEDED (hr))
            {
                BL_DoubleToStrDefault (out, nChars, v.dblVal);
            };
        }
        else
        {
            struct BL_StringBuffer buffer;

            InitStringBuffer (&buffer);

            if (typ == BL_LREAL)
            {
                hr = VariantChangeType (&v, value, 0, VT_R8);
                if (SUCCEEDED (hr))
                {
                    hr = FormatLReal (v.dblVal, &formatInfo, &buffer);
                };
            }
            else
            {
                hr = VariantChangeType (&v, value, 0, VT_R4);
                if (SUCCEEDED (hr))
                {
                    hr = FormatReal (v.fltVal, &formatInfo, &buffer);
                };
            };
            if (SUCCEEDED (hr))
            {
                if ((UINT)buffer.m_uiLength >= nChars)
                {
                    hr = E_BL_ERR_BUFFER_TO_SMALL;
                }
                else
                {
                    if (buffer.m_pszText == NULL)
                    {
                        *out = L'\0';
                    }
                    else
                    {
                        wcscpy (out, buffer.m_pszText);
                    };
                    hr = S_OK;
                };

                CleanupStringBuffer (&buffer);
            };
        }
    break;
    case BL_TIME:
        hr = MakeVT_Decimal(&v, value, &l64utc);
        if(SUCCEEDED(hr))
            hr = BL_TimeToIECStrDefault(l64utc, out, nChars);
    break;
    case BL_DT:
        if(value->vt==VT_DATE)
        {
            //just in case someone uses VT_DATE , we do our best...
			// Round the value. As a side effect prevents integral
			// from being one too less because of floating point
			// epsilon unpreciseness. If rounding is not wished
			// remove the "+ 0.5" and add a sufficiently big mul-
			// tiple of the floating points epsilon value (you have
			// to guess the multiple).
            l64utc = (__int64)( (value->date - (2.0 + 70*365 + 70/4)) * (24*3600*1000.0) + 0.5);
            hr = S_OK;
        }
        else
        {
            hr = MakeVT_Decimal(&v, value, &l64utc);
        }
 
        if(SUCCEEDED(hr))
            hr = BL_DateAndTimeToIECStr(l64utc, out, nChars);
    break;
    case BL_DATE:
        
        long daysSince1970;
        if(value->vt==VT_DATE)
        {
            //just in case someone uses VT_DATE , we do our best...
            daysSince1970 = (long)(value->date - (2.0 + 70*365 + 70/4));
            hr = S_OK;
        }
        else
        {
            hr = VariantChangeType(&v, value, 0, VT_I4);
            daysSince1970 = v.lVal;
        }
        
        if(SUCCEEDED(hr))
            hr = BL_DateToIECStr(daysSince1970, out, nChars);
    break;
    case BL_TOD:
        hr = VariantChangeType(&v, value, 0, VT_I4);
        if(SUCCEEDED(hr))
            hr = BL_TimeOfDayToIECStr(v.lVal, out, nChars);
    break;
    }


    VariantClear(&v);
    return hr;
}


static HRESULT FormatInteger (__int64 llValue, enum BL_IEC_TYP type, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer)
{
    HRESULT hr;
    int iTypeSize;

    switch (type)
    {
    //case BL_SINT: case BL_USINT:
    case BL_BYTE:
        iTypeSize = 1;
        break;

    case BL_INT: 
    case BL_UINT:
    case BL_WORD:
        iTypeSize = 2;
        break;

    case BL_UDINT:
    case BL_DINT: case BL_DWORD:
        iTypeSize = 4;
        break;

    case BL_ULINT:
    case BL_LINT: case BL_LWORD:
        iTypeSize = 8;
        break;

    default:
        assert (false);
        return (E_FAIL);
    };

    switch (pFormatInfo->m_Kind)
    {
    case BL_IntegerFormat:
        hr = FormatIntegerAsInteger (llValue, iTypeSize, pFormatInfo, pBuffer);
        break;

    case BL_FloatingRealFormat:
    case BL_FixedRealFormat:
        hr = FormatIntegerAsReal (llValue, iTypeSize, pFormatInfo, pBuffer);
        break;

    case BL_StringFormat:
        hr = FormatIntegerAsString (llValue, iTypeSize, pFormatInfo, pBuffer);
        break;

    default:
        assert (false);
        hr = E_FAIL;
    };

    return (hr);
}


static HRESULT FormatReal (float fltValue, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer)
{
    HRESULT hr;
    __int64 llValue;

    llValue = *(__int32 *)&fltValue;

    switch (pFormatInfo->m_Kind)
    {
    case BL_IntegerFormat:
        hr = FormatIntegerAsInteger (llValue, 4, pFormatInfo, pBuffer);
        break;

    case BL_FloatingRealFormat:
    case BL_FixedRealFormat:
        hr = FormatIntegerAsReal (llValue, 4, pFormatInfo, pBuffer);
        break;

    case BL_StringFormat:
        hr = FormatIntegerAsString (llValue, 4, pFormatInfo, pBuffer);
        break;

    default:
        assert (false);
        hr = E_FAIL;
    };

    return (hr);
}


static HRESULT FormatLReal (double dblValue, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer)
{
    HRESULT hr;
    __int64 llValue;

    llValue = *(__int64 *)&dblValue;

    switch (pFormatInfo->m_Kind)
    {
    case BL_IntegerFormat:
        hr = FormatIntegerAsInteger (llValue, 8, pFormatInfo, pBuffer);
        break;

    case BL_FloatingRealFormat:
    case BL_FixedRealFormat:
        hr = FormatIntegerAsReal (llValue, 8, pFormatInfo, pBuffer);
        break;

    case BL_StringFormat:
        hr = FormatIntegerAsString (llValue, 8, pFormatInfo, pBuffer);
        break;

    default:
        assert (false);
        hr = E_FAIL;
    };

    return (hr);
}


static HRESULT FormatString (LPCWSTR pszText, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer)
{
    HRESULT hr;

    switch (pFormatInfo->m_Kind)
    {
    case BL_IntegerFormat:
        hr = FormatStringAsInteger (pszText, pFormatInfo, pBuffer);
        break;

    case BL_FloatingRealFormat:
    case BL_FixedRealFormat:
        hr = FormatStringAsReal (pszText, pFormatInfo, pBuffer);
        break;

    case BL_StringFormat:
        hr = FormatStringAsString (pszText, pFormatInfo, pBuffer);
        break;

    default:
        assert (false);
        hr = E_FAIL;
    };

    return (hr);
}


static HRESULT FormatIntegerAsInteger (__int64 llValue, int iTypeSize, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer)
{
    HRESULT hr;
    int iItemSize;
    int iOffset;
    WCHAR szBuffer[100];

    iItemSize = pFormatInfo->m_iBytesPerItem;
    assert (iItemSize == sizeof (__int8) || iItemSize == sizeof (__int16) ||
              iItemSize == sizeof (__int32) || iItemSize == sizeof (__int64));
    if (iItemSize != sizeof (__int8) && iItemSize != sizeof (__int16) &&
        iItemSize != sizeof (__int32) && iItemSize != sizeof (__int64))
    {
        return (E_FAIL);
    };

    assert (iTypeSize != 0 && iTypeSize % iItemSize == 0);
    if (iTypeSize == 0 || iTypeSize % iItemSize != 0)
    {
        return (E_FAIL);
    };

    if (iTypeSize != iItemSize)
    {
        for (iOffset = iTypeSize - iItemSize; iOffset >= 0; iOffset -= iItemSize)
        {
            __int64 llItemValue;

            if (iOffset != iTypeSize - iItemSize)
            {
                if (!GrowByString (pBuffer, L" "))
                {
                    return (E_OUTOFMEMORY);
                };
            };

            llItemValue = (llValue >> 8 * iOffset) & (((__int64)1 << 8 * iItemSize) - 1);
            hr = FormatIntegerAsInteger (llItemValue, iItemSize, pFormatInfo, pBuffer);
            if (FAILED (hr))
            {
                return (hr);
            };
        };

        return (S_OK);
    };


    // If signed format is requested and value would be negative
    // in a type as wide as the given type size, output the sign
    // and absolutize the value. Note that we have to sign extend
    // the value before absolutizing.

    if (pFormatInfo->m_iSignedness == 1 &&
        (llValue & ((__int64)1 << (8 * iTypeSize - 1))) != 0)
    {
        if (!GrowByString (pBuffer, L"-"))
        {
            return (E_OUTOFMEMORY);
        };

        if (iTypeSize == sizeof (__int8))
        {
            llValue = (__int8)llValue;
        }
        else if (iTypeSize == sizeof (__int16))
        {
            llValue = (__int16)llValue;
        }
        else if (iTypeSize == sizeof (__int32))
        {
            llValue = (__int32)llValue;
        };

        if (llValue < 0)
        {
            llValue = -llValue;
        };
    };


    // Mask bits that would be used in a type as wide as the given size.

    if (iTypeSize < 8)
    {
        llValue &= ((__int64)1 << (8 * iTypeSize)) - 1;
    };

    switch (pFormatInfo->m_iRadix)
    {
    case 2:
        if (!GrowByString (pBuffer, L"2#"))
        {
            return (E_OUTOFMEMORY);
        };

        _ui64tot ((unsigned __int64)llValue, szBuffer, 2);
        break;

    case 8:
        if (!GrowByString (pBuffer, L"8#"))
        {
            return (E_OUTOFMEMORY);
        };

        _ui64tot ((unsigned __int64)llValue, szBuffer, 8);
        break;

    case 10:
        _ui64tot ((unsigned __int64)llValue, szBuffer, 10);
        break;

    case 16:
        if (!GrowByString (pBuffer, L"16#"))
        {
            return (E_OUTOFMEMORY);
        };

        _ui64tot ((unsigned __int64)llValue, szBuffer, 16);
        break;
    };

    hr = DoPadding (pBuffer, szBuffer, pFormatInfo);
    if (FAILED (hr))
    {
        return (hr);
    };

    return (S_OK);
}


static HRESULT FormatIntegerAsReal (__int64 llValue, int iTypeSize, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer)
{
    HRESULT hr;
    int iItemSize;
    int iOffset;
    double dblValue;
    WCHAR szBuffer[500];
    LPWSTR pszBuffer;

    iItemSize = pFormatInfo->m_iBytesPerItem;
    assert (iItemSize == sizeof (float) || iItemSize == sizeof (double));
    if (iItemSize != sizeof (float) && iItemSize != sizeof (double))
    {
        return (E_FAIL);
    };

    assert (iTypeSize != 0 && iTypeSize % iItemSize == 0);
    if (iTypeSize == 0 || iTypeSize % iItemSize != 0)
    {
        return (E_FAIL);
    };

    if (iTypeSize != iItemSize)
    {
        for (iOffset = iTypeSize - iItemSize; iOffset >= 0; iOffset -= iItemSize)
        {
            __int64 llItemValue;

            if (iOffset != iTypeSize - iItemSize)
            {
                if (!GrowByString (pBuffer, _T (" ")))
                {
                    return (E_OUTOFMEMORY);
                };
            };

            llItemValue = (llValue >> 8 * iOffset) & (((__int64)1 << 8 * iItemSize) - 1);
            hr = FormatIntegerAsReal (llItemValue, iItemSize, pFormatInfo, pBuffer);
            if (FAILED (hr))
            {
                return (hr);
            };
        };

        return (S_OK);
    };

    if (iItemSize == sizeof (double))
    {
        assert (sizeof (double) == sizeof (__int64));
        if (sizeof (double) != sizeof (__int64))
        {
            return (E_FAIL);
        };
        dblValue = *(double *)&llValue;
    }
    else
    {
        __int32 lValue;

        assert (sizeof (float) == sizeof (__int32));
        if (sizeof (float) != sizeof (__int32))
        {        
            return (E_FAIL);
        };
        lValue = (__int32)llValue;
        dblValue = *(float *)&lValue;
    };

    if (pFormatInfo->m_Kind == BL_FloatingRealFormat)
    {

        // floating real format should not be longer than sum of
        // 1) requested fraction part digits
        // 2) some integral part/decimal point/sign/exponent stuff.

        if (pFormatInfo->m_iPrecision + 20 > sizeof (szBuffer) / sizeof (WCHAR))
        {
            pszBuffer = (LPWSTR)malloc ((pFormatInfo->m_iPrecision + 50) * sizeof (WCHAR));
            if (pszBuffer == NULL)
            {
                return (E_OUTOFMEMORY);
            };
        }
        else
        {
            pszBuffer = szBuffer;
        };

        swprintf (pszBuffer, L"%.*e", pFormatInfo->m_iPrecision, dblValue);
    }
    else if (pFormatInfo->m_Kind == BL_FixedRealFormat)
    {

        // fixed real format result should not be longer than sum of
        // 1) fabs (value) > 1 ? log10 (fabs (value)) : 1 (= length of integral part)
        // 2) requested fraction part digits
        // 3) some decimal point/sign stuff.

        if ((fabs (dblValue) > 1 ? log10 (fabs (dblValue)) : 1) + pFormatInfo->m_iPrecision + 20 > sizeof (szBuffer) / sizeof (TCHAR))
        {
            pszBuffer = (LPWSTR)malloc ((pFormatInfo->m_iPrecision + 350) * sizeof (WCHAR));
            if (pszBuffer == NULL)
            {
                return (E_OUTOFMEMORY);
            };
        }
        else
        {
            pszBuffer = szBuffer;
        };

        swprintf (pszBuffer, L"%.*f", pFormatInfo->m_iPrecision, dblValue);
    }
    else
    {
        assert (false);
        return (E_FAIL);
    };

    hr = DoPadding (pBuffer, szBuffer, pFormatInfo);

    if (pszBuffer != szBuffer)
    {
        free (pszBuffer);
    };

    return (hr);
}


static HRESULT FormatIntegerAsString (__int64 llValue, int iTypeSize, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer)
{
    HRESULT hr;
    int iItemSize;
    int iOffset;
    WCHAR wszBuffer[2];

    iItemSize = pFormatInfo->m_iBytesPerItem;
    assert (iItemSize == sizeof (CHAR) || iItemSize == sizeof (WCHAR));
    if (iItemSize != sizeof (CHAR) && iItemSize != sizeof (WCHAR))
    {
        return (E_FAIL);
    };

    assert (iTypeSize != 0 && iTypeSize % iItemSize == 0);
    if (iTypeSize == 0 || iTypeSize % iItemSize != 0)
    {
        return (E_FAIL);
    };

    if (iTypeSize != iItemSize)
    {
        for (iOffset = iTypeSize - iItemSize; iOffset >= 0; iOffset -= iItemSize)
        {
            __int64 llItemValue;

            llItemValue = (llValue >> 8 * iOffset) & (((__int64)1 << 8 * iItemSize) - 1);
            hr = FormatIntegerAsString (llItemValue, iItemSize, pFormatInfo, pBuffer);
            if (FAILED (hr))
            {
                return (hr);
            };
        };

        return (S_OK);
    };

    wszBuffer[0] = (WCHAR)(llValue & ((1 << 8 * iItemSize) - 1));
    wszBuffer[1] = L'\0';

    hr = DoPadding (pBuffer, wszBuffer, pFormatInfo);

    if (FAILED (hr))
    {
        return (hr);
    };

    return (S_OK);
}


static HRESULT FormatStringAsInteger (LPCWSTR pszText, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer)
{
    int iTypeSize;
    int iItemSize;
    int iRemainingSize;

    iTypeSize = wcslen (pszText) * sizeof (WCHAR);

    iItemSize = pFormatInfo->m_iBytesPerItem;
    assert (iItemSize == sizeof (__int8) || iItemSize == sizeof (__int16) ||
              iItemSize == sizeof (__int32) || iItemSize == sizeof (__int64));
    if (iItemSize != sizeof (__int8) && iItemSize != sizeof (__int16) &&
        iItemSize != sizeof (__int32) && iItemSize != sizeof (__int64))
    {
        return (E_FAIL);
    };

    if (iTypeSize < iItemSize)
    {
        return (S_FALSE);
    };

    for (iRemainingSize = iTypeSize; iRemainingSize >= iItemSize; iRemainingSize -= iItemSize)
    {
        __int64 llValue;
        HRESULT hr;

        if (iRemainingSize != iTypeSize)
        {
            if (!GrowByString (pBuffer, L" "))
            {
                return (E_OUTOFMEMORY);
            };
        };

        switch (iItemSize)
        {
        case sizeof (__int8) : llValue = *(__int8 *)pszText;  break;
        case sizeof (__int16): llValue = *(__int16 *)pszText; break;
        case sizeof (__int32): llValue = *(__int32 *)pszText; break;
        case sizeof (__int64): llValue = *(__int64 *)pszText; break;
        };

        hr = FormatIntegerAsInteger (llValue, iItemSize, pFormatInfo, pBuffer);
        if (FAILED (hr))
        {
            return (hr);
        };

        pszText = (LPWSTR)((LPSTR)pszText + iItemSize);
    };

    return (S_OK);
}


static HRESULT FormatStringAsReal (LPCWSTR pszText, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer)
{
    int iTypeSize;
    int iItemSize;
    int iRemainingSize;

    iTypeSize = wcslen (pszText) * sizeof (WCHAR);

    iItemSize = pFormatInfo->m_iBytesPerItem;
    assert (iItemSize == sizeof (float) || iItemSize == sizeof (double));
    if (iItemSize != sizeof (float) && iItemSize != sizeof (double))
    {
        return (E_FAIL);
    };

    if (iTypeSize < iItemSize)
    {
        return (S_FALSE);
    };

    for (iRemainingSize = iTypeSize; iRemainingSize >= iItemSize; iRemainingSize -= iItemSize)
    {
        __int64 llValue;
        HRESULT hr;

        if (iRemainingSize != iTypeSize)
        {
            if (!GrowByString (pBuffer, L" "))
            {
                return (E_OUTOFMEMORY);
            };
        };

        switch (iItemSize)
        {
        case sizeof (float):
            assert (sizeof (float) == sizeof (__int32));
            if (sizeof (float) != sizeof (__int32))
            {
                return (E_FAIL);
            };
            llValue = *(__int32 *)pszText;
            break;

        case sizeof (double):
            assert (sizeof (double) == sizeof (__int64));
            if (sizeof (double) != sizeof (__int64))
            {
                return (E_FAIL);
            };
            llValue = *(__int64 *)pszText;
            break;
        };

        hr = FormatIntegerAsReal (llValue, iItemSize, pFormatInfo, pBuffer);
        if (FAILED (hr))
        {
            return (hr);
        };

        pszText = (LPWSTR)((LPSTR)pszText + iItemSize);
    };

    return (S_OK);
}


static HRESULT FormatStringAsString (LPCWSTR pszText, const struct BL_FormatInfo *pFormatInfo, struct BL_StringBuffer *pBuffer)
{
    HRESULT hr;

    hr = DoPadding (pBuffer, pszText, pFormatInfo);
    if (FAILED (hr))
    {
        return (hr);
    };

    return (S_OK);
}


static HRESULT DoPadding (struct BL_StringBuffer *pBuffer, LPCWSTR pszText, const struct BL_FormatInfo *pFormatInfo)
{
    if (pFormatInfo->m_Alignment == BL_LeftAlign)
    {
        if (!GrowByString (pBuffer, pszText))
        {
            return (E_OUTOFMEMORY);
        };
    };

    if (pFormatInfo->m_iWidth != 0)
    {
        size_t uiLength;

        uiLength = wcslen (pszText);

        if ((int)uiLength < pFormatInfo->m_iWidth)
        {
            if (!GrowByCharacter (pBuffer, pFormatInfo->m_chPadding, pFormatInfo->m_iWidth - uiLength))
            {
                return (E_OUTOFMEMORY);
            };
        };
    };

    if (pFormatInfo->m_Alignment != BL_LeftAlign)
    {
        if (!GrowByString (pBuffer, pszText))
        {
            return (E_OUTOFMEMORY);
        };
    };

    return (S_OK);
}


static void InitStringBuffer (struct BL_StringBuffer *pBuffer)
{
    pBuffer->m_pszText = NULL;
    pBuffer->m_uiSize = 0;
    pBuffer->m_uiLength = 0;
}


static void CleanupStringBuffer (struct BL_StringBuffer *pBuffer)
{
    if (pBuffer->m_pszText != NULL)
    {
        free (pBuffer->m_pszText);
        pBuffer->m_pszText = NULL;
    };

    pBuffer->m_uiSize = 0;
    pBuffer->m_uiLength = 0;
}


static bool GrowByString (struct BL_StringBuffer *pBuffer, LPCWSTR pszString)
{
    size_t uiLength;

    uiLength = wcslen (pszString);

    if (!CheckSpace (pBuffer, uiLength))
    {
        return (false);
    };
    
    wcscpy (pBuffer->m_pszText + pBuffer->m_uiLength, pszString);
    pBuffer->m_uiLength += uiLength;

    return (true);
}


static bool GrowByCharacter (struct BL_StringBuffer *pBuffer, WCHAR ch, size_t uiCount)
{
    if (!CheckSpace (pBuffer, uiCount))
    {
        return (false);
    };

    while (uiCount-- != 0)
    {
        pBuffer->m_pszText[pBuffer->m_uiLength++] = ch;
    };
    pBuffer->m_pszText[pBuffer->m_uiLength] = '\0';

    return (true);
}


static bool CheckSpace (struct BL_StringBuffer *pBuffer, size_t uiCount)
{
    if (pBuffer->m_uiLength + uiCount + 1 < pBuffer->m_uiLength)
    {
        return (false);
    };

    if (pBuffer->m_uiLength + uiCount + 1 > pBuffer->m_uiSize)
    {
        size_t uiAdditional;

        uiAdditional = pBuffer->m_uiLength + uiCount + 1 - pBuffer->m_uiSize;
        if (uiAdditional < 100)
        {
            uiAdditional = 100;
        };

        pBuffer->m_uiSize += uiAdditional;
        if (pBuffer->m_uiSize * sizeof (WCHAR) < pBuffer->m_uiSize)
        {
            return (false);
        };

        if (pBuffer->m_pszText == NULL)
        {
            pBuffer->m_pszText = (LPWSTR)malloc (pBuffer->m_uiSize * sizeof (WCHAR));

            if (pBuffer->m_pszText != NULL)
            {
                *pBuffer->m_pszText = '\0';
            };
        }
        else
        {
            pBuffer->m_pszText = (LPWSTR)realloc (pBuffer->m_pszText, pBuffer->m_uiSize * sizeof (WCHAR));
        };

        if (pBuffer->m_pszText == NULL)
        {
            return (false);
        };
    };

    return (true);
}


static struct tagKnownTypeList {
    WCHAR               *pszTypeName;
    enum BL_IEC_TYP     type;
    DWORD               dwModifier;
} KnownTypeList[] = 
{
//  pszTypeName         type        dwModifier
//    L"SINT",            BL_SINT,    0,
    L"INT",             BL_INT,    0,
    L"DINT",            BL_DINT,    0,
    L"LINT",            BL_LINT,    0,
//    L"USINT",           BL_USINT,    0,
    L"UINT",            BL_UINT,    0,
    L"UDINT",           BL_UDINT,    0,
    L"ULINT",           BL_ULINT,    0,
    L"REAL",            BL_REAL,   0,
    L"LREAL",           BL_LREAL,   0,
    L"DATE",            BL_DATE,    0,
    L"TIME_OF_DAY",     BL_TOD,     0,
    L"TOD",             BL_TOD,     0,
    L"DATE_AND_TIME",   BL_DT,      0,
    L"DT",              BL_DT,      0,
    L"STRING",          BL_STRING,  0,
    L"WSTRING",         BL_WSTRING, 0,
    L"TIME",            BL_TIME,    0,
    L"BOOL",            BL_BOOL,    0,
    L"BYTE",            BL_BYTE,    0,
    L"WORD",            BL_WORD,    0,
    L"DWORD",           BL_DWORD,   0,
    L"LWORD",           BL_LWORD,   0,
    // QVT datatypes
    L"QVT_BOOL",        BL_BOOL,    BL_DTYPE_IS_QVT,
    L"QVT_BYTE",        BL_BYTE,    BL_DTYPE_IS_QVT,
    L"QVT_WORD",        BL_WORD,    BL_DTYPE_IS_QVT,
    L"QVT_DWORD",       BL_DWORD,   BL_DTYPE_IS_QVT,
    L"QVT_LWORD",       BL_LWORD,   BL_DTYPE_IS_QVT,
    L"QVT_INT",         BL_INT,     BL_DTYPE_IS_QVT,
    L"QVT_DINT",        BL_DINT,    BL_DTYPE_IS_QVT,
    L"QVT_LINT",        BL_LINT,    BL_DTYPE_IS_QVT,
    L"QVT_REAL",        BL_REAL,    BL_DTYPE_IS_QVT,
    L"QVT_LREAL",       BL_LREAL,   BL_DTYPE_IS_QVT,
    L"QVT_STRING",      BL_STRING,  BL_DTYPE_IS_QVT,
    L"QVT_WSTRING",     BL_WSTRING, BL_DTYPE_IS_QVT,
};


//------------------------------------------------------------------*
/**
 * BL_TypenameToDatatype
 *      convert readable string of datatype name to correct dataype enum BL_...
 *
 * @param           pszTypeName         name string of datatype as used in ST
 * @param           pDataType           [out] datatype
 * @param           pdwModifiers        [out] additional info to datatype;
 *                                      currently: QVT or non-QVT datatype
 * @return          S_OK
 *                  E_BL_ERR_SYNTAX     datatype name is unknown
 *                  E_BL_ERR_INVALIDARG bad pointer or bad string
 * @exception       -
 * @see             
*/
extern "C"
HRESULT BASELIB_API __stdcall BL_TypenameToDatatype(LPCOLESTR pszTypeName,
                                 BL_IEC_TYP *pDataType, DWORD *pdwModifiers)
{
    if (   (pszTypeName == NULL)
        || (pDataType == NULL)
        || (pdwModifiers == NULL))
    {
        return E_BL_ERR_INVALIDARG;
    }
    *pDataType = BL_ANY;
    *pdwModifiers = 0L;

    HRESULT hr = E_BL_ERR_SYNTAX;
    // this "parser" is not very clever, but should be enough for this small datatype list
    // %TBD% improve algorithm when more datatypes are added in the future !!
    for (int i = 0; i < (sizeof(KnownTypeList) / sizeof(struct tagKnownTypeList)); i++)
    {
        if (wcsicmp(KnownTypeList[i].pszTypeName, pszTypeName) == NULL) {
            // found valid type name
            *pDataType = KnownTypeList[i].type;
            *pdwModifiers = KnownTypeList[i].dwModifier;
            hr = S_OK;
            break;
        }
    }

    return hr;
}



//------------------------------------------------------------------*
/**
 * BL_DatatypeToTypename
 *      convert BL_IEC_TYP enum to WSTRING.
 *
 * @param           wDataType           [in] datatype
 * @param           dwModifiers         [in] additional info to datatype;
 *                                      currently: QVT or non-QVT datatype
 * @return          !=NULL              valid type name string
 *                  NULL                no valid type name string found
 * @exception       -
 * @see
*/
extern "C"
LPCWSTR BASELIB_API __stdcall BL_DatatypeToTypename(BL_IEC_TYP wDataType, DWORD dwModifiers)
{
    // this "parser" is not very clever, but should be enough for this small datatype list
    // %TBD% improve algorithm when more datatypes are added in the future !!
    for (int i = 0; i < (sizeof(KnownTypeList) / sizeof(struct tagKnownTypeList)); i++)
    {
        if (wDataType == KnownTypeList[i].type && dwModifiers == KnownTypeList[i].dwModifier) {
            // found valid type name
            return KnownTypeList[i].pszTypeName;
        }
    }

    return NULL;
}



/**
 * Convert a string to __int64 assuming strict IEC1131-3 format
 *
 * @param       szInput   string to convert 
 * @param       pul32      output value, may be NULL.
 *                        If ul32==NULL there is no range/type check, 
 *                        only syntax check.
 * @param       typ       BL_BYTE,BL_WORD,BL_DWORD or BL_INT,BL_DINT,BL_LINT
 *                        the expected type
 * @param       pszEnd    set to char that stopped the scan, if not NULL
 * @return      S_OK or   E_BL_ERR_SYNTAX or E_BL_ERR_RANGE (only if lpValue!=NULL)
 *                        or E_BL_ERR_INVALIDARG
 */
HRESULT BL_IECStrToAnyIntegral(LPCTSTR szInput, 
                               __int64* pl64, 
                               BL_IEC_TYP typ, 
                               LPCTSTR* pszEnd)
{
    LPCTSTR     psz = szInput;
    HRESULT     hr = S_OK;
    BL_IEC_TYP  typpfix = typ;
    int         radix =10;


    assert(szInput);
    if(szInput==NULL)
    {
        hr = E_BL_ERR_INVALIDARG;
        goto error;
    }


    //test for an optional type prefix:
    psz = BL_StripPrefixI(psz, _T("BYTE#"));
    if(psz != szInput){ typpfix = BL_BYTE;  goto parse;}

    psz = BL_StripPrefixI(psz, _T("WORD#"));
    if(psz != szInput){ typpfix = BL_WORD;  goto parse;}

    psz = BL_StripPrefixI(psz, _T("DWORD#"));
    if(psz != szInput){ typpfix = BL_DWORD; goto parse;}

    psz = BL_StripPrefixI(psz, _T("LWORD#"));
    if(psz != szInput){ typpfix = BL_LWORD; goto parse;}

    psz = BL_StripPrefixI(psz, _T("INT#"));
    if(psz != szInput){ typpfix = BL_INT; goto parse;}

    psz = BL_StripPrefixI(psz, _T("DINT#"));
    if(psz != szInput){ typpfix = BL_DINT; goto parse;}

    psz = BL_StripPrefixI(psz, _T("LINT#"));
    if(psz != szInput){ typpfix = BL_LINT; goto parse;}

    psz = BL_StripPrefixI(psz, _T("UINT#"));
    if(psz != szInput){ typpfix = BL_UINT; goto parse;}

    psz = BL_StripPrefixI(psz, _T("UDINT#"));
    if(psz != szInput){ typpfix = BL_UDINT; goto parse;}

    psz = BL_StripPrefixI(psz, _T("ULINT#"));
    if(psz != szInput){ typpfix = BL_ULINT; goto parse;}
parse:
    if(typ != typpfix)
    {
        hr = E_BL_ERR_SYNTAX;
        goto error;
    }


    //get an optional 2#,8#,16#, default is radix==10
    if(!_tcsncmp(psz, _T("2#"), 2))
    {
        radix = 2;
        psz += 2;
    }
    else if(!_tcsncmp(psz, _T("8#"), 2))
    {
        radix = 8;
        psz += 2;
    }
    else if(!_tcsncmp(psz, _T("16#"), 3))
    {
        radix = 16;
        psz += 3;

    }

    
    hr = BL_ParseInt64(psz, pl64, radix==10, radix, &psz);
    if(FAILED(hr))
        goto error;
    
    if(pl64)
    {
        //check range:
        switch(typ)
        {
        case BL_BYTE:  
            if(*pl64>0xff || *pl64<0)  
                hr = E_BL_ERR_RANGE;  
        break;
        case BL_INT:
            if(radix == 10) {
                if(*pl64>SHRT_MAX || *pl64<SHRT_MIN)  
                    hr = E_BL_ERR_RANGE;  
            break;
            }//fall th. to BL_WORD problem is int#16#ffff
        case BL_WORD:  
        case BL_UINT:  
            if(*pl64>0xffff || *pl64<0)  
                hr = E_BL_ERR_RANGE;  
        break;
        case BL_DINT:
            if(radix == 10) {
                if(*pl64>LONG_MAX || *pl64<LONG_MIN)  
                    hr = E_BL_ERR_RANGE;  
            break;
            }//fall th. to BL_DWORD problem is dint#16#ffffffff
        case BL_DWORD: 
        case BL_UDINT:  
            if(*pl64>0xffffffff || *pl64<0)  
                hr = E_BL_ERR_RANGE;  
        break;
        case BL_LWORD: //BL_LWORD and BL_LINT range check was done by BL_ParseInt64.
        case BL_LINT:  break;
        case BL_ULINT:  break;
        default:
            hr = E_BL_ERR_INVALIDARG;
        }

    }

error:
    if(pszEnd)
        *pszEnd = psz;

    return hr;
}



/**
 * Convert a string to double assuming strict IEC1131-3 format
 *
 * @param       szInput   string to convert (with a maximum length of 512 chars) 
 * @param       lpValue   output value, may be NULL.
 *                        If lpValue==NULL there is no range check, only syntax check
 * @param       pszEnd    set to char that stopped the scan, if not NULL
 * @param       typ       BL_REAL or BL_LREAL
 * @return      S_OK or  E_BL_ERR_SYNTAX or E_BL_ERR_RANGE (only if lpValue!=NULL)
 */
HRESULT BL_IECStrToDouble(LPCTSTR szInput, double* lpValue, BL_IEC_TYP typ, LPCTSTR* pszEnd)
{
    int         i;
    HRESULT     hr = S_OK;
    LPCTSTR     psz;
    LPCTSTR     pszE = szInput;
    TCHAR       buffer[512];    
    BL_IEC_TYP  typpfix = typ;


    //test for an optional 'lReaL#' type prefix:
    psz = BL_StripPrefixI(pszE, _T("LREAL#"));
    if(psz != pszE){ typpfix = BL_LREAL; goto parse;}

    psz = BL_StripPrefixI(pszE, _T("REAL#"));
    if(psz != pszE){ typpfix = BL_REAL; goto parse;}

parse:
    pszE = psz;
    if(typ != typpfix)
    {
        hr = E_BL_ERR_SYNTAX;
        goto error;
    }


    hr = BL_ParseInt64(pszE, NULL, true, 10, &pszE);
    if(FAILED(hr))
        goto error;

    //there must be a '.':
    if(*pszE!=_T('.'))
    {
        hr = E_BL_ERR_SYNTAX;
        goto error;
    }
    pszE++;

    //no +- here:
    hr = BL_ParseInt64(pszE, NULL, false, 10, &pszE);
    if(FAILED(hr))
        goto error;

    //opt exponent:
    if(*pszE==_T('E') || *pszE==_T('e'))
    {
        pszE++;

        hr = BL_ParseInt64(pszE, NULL, true, 10, &pszE);
        if(FAILED(hr))
            goto error;
    }


    //copy to buffer, strip '_':
    i=0;
    while(1)
    {
        if(*psz==_T('_'))
            psz++;

        buffer[i++] = *psz++;

        //check for too long token:
        if(i>=FC_ARRAY_LEN(buffer))
        {
            hr = E_BL_ERR_SYNTAX;
            goto error;
        }

        //end condition:
        if(psz==pszE)
            break;
    }
    //null terminate:
    buffer[i] = 0;


    //get the binary value if lpValue!=NULL:
    if(lpValue)
    {
        errno = 0;
        LPTSTR pszE2;
        *lpValue = _tcstod(buffer, &pszE2);

        //assert: no syntax error should happen, only range error possible:
        assert(*pszE2==0);

        if(errno==ERANGE)
        {
            hr = *lpValue==0.0 ? E_BL_ERR_LOSS_OF_PRECIS : E_BL_ERR_RANGE;
        }
        else if(typ==BL_REAL)
        {
            if(fabs(*lpValue)>FLT_MAX)
            {
                hr = E_BL_ERR_RANGE;
            }
            else if(fabs(*lpValue)<FLT_MIN && *lpValue != 0.0)
            {
                hr = E_BL_ERR_LOSS_OF_PRECIS;
            }
        }
    }


error:
    if(pszEnd)
        *pszEnd = pszE;

    return hr;
}


/*--------------------------------------------------------------------------*/
HRESULT BL_DoubleToStrDefault (
/*--------------------------------------------------------------------------*/
    LPTSTR pcOut,
    UINT   wBufLen,
    double dWert)
/**
 * FUNCTIONAL DESCRIPTION
 * wandelt Gleitkommazahl in Zeichenkette (bez. Basis 10) um,
 * ist interne Hilfsfunktion
 * REFERENCES
 */
{
  HRESULT result = S_OK;
  int iDez;
  int iVZ;
  int digits;
  TCHAR *pValue;
  char *pcHlp;   /* a real char * due to ecvt!*/
  int wMaxLen; /* length w/o NULL char */
  int len=0; /* length in fixed point */
  int len2=0; /* length with exponent */

  wMaxLen = min(wBufLen-1,24); /* ignore very long buffer */

  for (digits = 15; digits > 0; digits--)
    {
      pcHlp = ecvt(dWert,digits,&iDez,&iVZ);

      pValue = (TCHAR *)alloca((strlen(pcHlp)+1)*sizeof(TCHAR));
      FC_StringToWideChar(pcHlp,pValue,strlen(pcHlp)+1);

      if (pValue[0]==_T('1') && (pValue[1]==_T('#') || pValue[1] == _T('$')))
        { /* infinite values can yield strings like "1#INF" or "1#NAN" */
          if(wBufLen<4)
              return E_BL_ERR_BUFFER_TO_SMALL;

          pcOut[0]= pValue[2];
          pcOut[1]= pValue[3];
          pcOut[2]= pValue[4];
          pcOut[3]= 0;
          return result;
        }

      /*
       * sometimes ecvt() gives one digit more than it should!
       */
      assert((int)_tcslen(pValue)==digits || (int)_tcslen(pValue)==digits+1 && pValue[digits]==_T('0'));
      pValue[digits] = 0;

      for (; digits>1 && pValue[digits-1]==_T('0'); digits--)
        pValue[digits-1] = 0;

      len = digits; /* see if floating point format will fit */

      if (iDez<=0)
        len += -iDez +2;   /* < 1.0;      decimal point to the left */
      else if (iDez>=len)
        len  =  iDez +2;   /* > 999999.0; decimal point to the right */
      else
        len++;             /* decimal point inbetween */

      if (iVZ)
        len++;

      if (len <= wMaxLen)
        break;


      len2 = digits + 3;        /* scientific format: dec. point, E and one digit exponent */
      if (!pValue[1])
        len2++;                 /* need one digit after dpoint */

      assert(pValue[0]!=_T('0') || iDez==0 && !pValue[1]);
      if (pValue[0]!=_T('0') && iDez<=0)
        len2++;                 /* signed exponent */

      assert(abs(iDez-1)<500);  /* should not happen */
      if (abs(iDez-1)>=100)
        len2+=2;                 /* three-digit exponent */
      else if(abs(iDez-1)>=10)
        len2++;                 /* two-digit exponent */
          
      if (iVZ)
        len2++;                 /* signed number */

      if (len2 <= wMaxLen)
        break;


    }

  if (digits==0 && iDez<0)      /* underflow */
    { /* now we know: len>wMaxLen && len2>wMaxLen */
      iVZ=iDez=0;
      pValue = _T("0");
      len = 3; /* try if 0.0 will fit */
    }

  if (len <= wMaxLen)
    { /* format fits without loss of precision */
      if (iVZ)
        *pcOut++ = _T('-');
      if (iDez<=0)
        {
          *pcOut++ = _T('0');
          *pcOut++ = _T('.');
          for (;iDez<0;iDez++)
                *pcOut++ = _T('0');
          while (*pValue)
            *pcOut++ = *pValue++;
        }
      else
        for (;iDez >= 0 || *pValue;iDez--)
          {
            if (iDez == 0)
              *pcOut++ = _T('.');
            *pcOut++ = (TCHAR)(*pValue ? *pValue++ : _T('0'));
          }
    }
  else if (len2 <= wMaxLen)
    {
      if (iVZ)
        *pcOut++ = _T('-');
      if (pValue[0]!=_T('0'))
        iDez--;
      *pcOut++ = *pValue++;
      *pcOut++ = _T('.');
      if (!*pValue)
        *pcOut++ = _T('0');
      else
        while (*pValue)
          *pcOut++ = *pValue++;

      *pcOut++ = _T('E');
      pcOut += _stprintf(pcOut,_T("%i"), iDez);
    }
  else
    {
      result = E_BL_ERR_BUFFER_TO_SMALL;
    }
  *pcOut = _T('\0');
  return result;
}





/**
 * Convert a string to boolean assuming IEC1131-3 format
 *
 * @param       szInput   string to convert 
 * @param       pbResult  output value
 * @param       pszEnd    set to char that stopped the scan, if not NULL
 * @return      S_OK, E_BL_ERR_SYNTAX
 */
HRESULT BL_IECStrToBool(LPCTSTR szInput, bool* pbResult, LPCTSTR* pszEnd)
{
    LPCTSTR psz = szInput;
    LPCTSTR pszOld;
    HRESULT hr = S_OK;


    //test for an optional 'BoOl#' type prefix:
    psz = BL_StripPrefixI(psz, _T("BOOL#"));
    
    pszOld = psz;
    if((psz = BL_StripPrefixI(psz, _T("0")))!=pszOld || 
       (psz = BL_StripPrefixI(psz, _T("FALSE")))!=pszOld)
    {
        *pbResult = false;
    }
    else if((psz = BL_StripPrefixI(psz, _T("1")))!=pszOld || 
            (psz = BL_StripPrefixI(psz, _T("TRUE")))!=pszOld)
    {
        *pbResult = true;
    }
    else
    {
        hr = E_BL_ERR_SYNTAX;
    }

    if(pszEnd)
        *pszEnd = psz;

    return hr;
}



/**
 * Convert a TIME string to ms, assume IEC format
 *
 * Time constant syntax as lex script:
 *  integer      {digit}(_?{digit})*
 *  fixed_point  {integer}(\.{integer})?
 *  days         ({fixed_point}[dD]|{integer}[dD]_?{hours})
 *  hours        ({fixed_point}[hH]|{integer}[hH]_?{minutes})
 *  minutes      ({fixed_point}[mM]|{integer}[mM]_?{seconds})
 *  seconds      ({fixed_point}[sS]|{integer}[sS]_?{milliseconds})
 *  milliseconds {fixed_point}(ms|MS)
 *  interval     ({days}|{hours}|{minutes}|{seconds}|{milliseconds})
 *  duration     (T|t|TIME|time)#-?{interval}
 *
 * @param       szInput     string to convert
 * @param       lpi64Value  output value
 * @param       pszEnd      set to char that stopped the scan, if not NULL
 * @return      S_OK or E_BL_ERR_SYNTAX, E_BL_ERR_RANGE , E_BL_ERR_INVALIDARG
 */
HRESULT BASELIB_API __stdcall BL_IECTimeStrToMillis(const wchar_t* szInput, 
                                                    __int64* lpi64Value, 
                                                    const wchar_t** pszEnd)
{
    LPCTSTR  psz        = szInput;
    __int64  l64Result  = 0;
    int      iOrder;
    __int64  l64UnitInMS;
    int      iOrderOld;
    int      field;
    HRESULT  hr = S_OK;
    __int64  l64Integral;
    __int64  l64Fraction;
    bool     bNegative =false;
        
    
    assert(szInput && lpi64Value);
    if(!szInput || !lpi64Value)
    {
        hr = E_BL_ERR_INVALIDARG;
        goto error;
    }

    
    
    //test for the required 'Time#'|'t#' type prefix:
    psz = BL_StripPrefixI(psz, _T("T#"));
    
    if(psz==szInput)
    {
        psz = BL_StripPrefixI(psz, _T("TIME#"));
        if(psz==szInput)
        {
            hr = E_BL_ERR_SYNTAX;
            goto error;
        }
    }


    for(field=0;field<5;field++)
    {
        __int64 l64Count;
        LPCTSTR pszFracBeg = NULL;

        //the first field may have a '-' (but for strict IEC1131-3 not a '+'!)
        //all other fields (field>0) may have neither a '+' nor a '-'
        //note that BL_ParseInt64 does allow '+' and '-' if param#3 is true
        if(field>0)
        {
            if(BL_IsDigitBase10(*psz)<0)
            {
                hr = E_BL_ERR_SYNTAX;
                goto error;
            }
        }
        else if(field==0)
        {
            //CAUTION: do get an optional '-' here, but do not psz++
            //reason: if BL_ParseInt64 skips the '-' and when the
            //int before the fraction is 0 (e.g. t#-0.4456d), then 
            //you don't get the minus via l64Count (because l64Count==-0).
            //Do not psz++  on minus here, because MAX_VALUE wouldn't work!
            if(*psz==_T('+'))
            {
                hr = E_BL_ERR_SYNTAX;
                goto error;
            }
            else if(*psz==_T('-'))
            {
                bNegative = true;
            }
        }
            
        hr = BL_ParseInt64(psz, &l64Count, true, 10, &psz);
        if(hr != S_OK)
            goto error;
        
        if(l64Count>0)
        {
            //NOTE: negate l64Count has the effect of accumulating only negative
            //numbers, then we have no problems near MAX_VALUE
            l64Count = -l64Count;
        }

       
        if(*psz==_T('.'))
        {
            psz++;
            pszFracBeg = psz;

            hr = BL_ParseInt64(psz, NULL, false, 10, &psz);
            if(hr != S_OK)
                goto error;
        }
        
        
        switch(*psz)
        {
        case _T('d'): 
        case _T('D'): 
            psz++;
            iOrder = 1; 
            l64UnitInMS  = 24*3600*1000;
        break;
        case _T('h'): 
        case _T('H'): 
            psz++;
            iOrder = 2; 
            l64UnitInMS  = 3600*1000;
        break;
        case _T('m'): 
        case _T('M'): 
            if(psz[1]==_T('s') || psz[1]==_T('S'))
            {
                psz++;
                psz++;
                iOrder = 5;
                l64UnitInMS  = 1;
            }
            else
            {
                psz++;
                iOrder = 3;
                l64UnitInMS  = 60*1000;
            }
        break;
        case _T('s'): 
        case _T('S'): 
            psz++;
            iOrder = 4; 
            l64UnitInMS  = 1000;
        break;
        default:
            hr = E_BL_ERR_SYNTAX;
            goto error;
        }

        //check order of iOrder order of 
        //day(1)->hour(2)->min(3)->sec(4)->ms(5):
        if(field>0 && (iOrderOld+1 != iOrder))
        {
            hr = E_BL_ERR_SYNTAX;
            goto error;
        }
        
        iOrderOld = iOrder;    
        
               
        //add ms to total l64Result:
        l64Integral = 0;
        l64Fraction = 0;

        if(!BL_safe_mul_int64(&l64Integral, l64UnitInMS, l64Count))
        {
            hr = E_BL_ERR_RANGE;
            goto error;
        }
        
        if(pszFracBeg)
        {
            //add the fractional part:
            LPCTSTR pszTmp = pszFracBeg;
            while(1)
            {
                if(*pszTmp==_T('_'))
                    pszTmp++;
                
                int iDig = BL_IsDigitBase10(*pszTmp);
                if(iDig<0)
                    break;
                
                pszTmp++;

                l64UnitInMS /= 10;
                if(l64UnitInMS==0)
                {
                    hr = E_BL_ERR_LOSS_OF_PRECIS;
                    goto error;
                }

                //NOTE: l64Fraction -= has the effect of accumulating only negative
                //numbers, then we have no problems near MAX_VALUE
                l64Fraction -= l64UnitInMS * iDig;  
            }
        }

        //calc:  l64Result = l64Result + l64Integral + l64Fraction;
        if(!BL_safe_add_int64(&l64Result, l64Result, l64Integral))
        {
            hr = E_BL_ERR_RANGE;        
            goto error;
        }
        if(pszFracBeg && !BL_safe_add_int64(&l64Result, l64Result, l64Fraction))
        {
            hr = E_BL_ERR_RANGE;        
            goto error;
        }

        
        
        //stop scan if unit=ms or fraction was found
        if(iOrder==5 || pszFracBeg)
            break;

        //next unit, opt '_':
        if(*psz==_T('_'))
        {
            psz++;
        }
        else
        {
            //stop scan if anything else than a digit:
            if(BL_IsDigitBase10(*psz)<0)
                break;
        }
    }


    //NOTE: if(!bNegative) we negate is correct, read NOTE: above
    if(!bNegative && !BL_safe_sub_int64(&l64Result, 0, l64Result))
        hr = E_BL_ERR_RANGE;

error:
    if(pszEnd)
        *pszEnd = psz;

    *lpi64Value = l64Result;
    
    return hr;
}


/**
 * Convert a DATE_AND_TIME string to SYSTEMTIME, assume IEC format. The date and time
 * are interpreted as local date and time including any dayligth saving correction.
 * typedef struct _SYSTEMTIME {
 *   WORD wYear; 
 *   WORD wMonth; 
 *   WORD wDayOfWeek; 
 *   WORD wDay; 
 *   WORD wHour; 
 *   WORD wMinute; 
 *   WORD wSecond; 
 *   WORD wMilliseconds; 
 * } SYSTEMTIME; 
 * The day of week is not set by this function (it is set to 0xffff) 
 *
 *
 * DATE_AND_TIME constant syntax as lex script (the <type># prefix is scanned 
 * case insensitive):
 *
 * integer                 {digit}(_?{digit})*
 * year                    {integer}
 * month                   {integer}
 * day                     {integer}
 * date_literal            {year}-{month}-{day}
 * day_hour                {integer}
 * day_minute              {integer}
 * day_second              {fixed_point}
 * daytime                 {day_hour}:{day_minute}:{day_second}
 * date_and_time           (DATE_AND_TIME|DT)#{date_literal}-{daytime}
 *
 * @param       szInput     string to convert
 * @param       lpSystime   output value, if NULL only the syntax is checked
 * @param       pUTCmillis  output value, may be NULL, if not NULL then 
 *                          lpSystime must not be NULL too. 
 *                          pUTCmillis will be set to the number of milliseconds
 *                          past since 1.1.1970 in UTC time.
 * @param       pszEnd      set to char that stopped the scan, if not NULL
 * @return      S_OK or E_BL_ERR_SYNTAX, E_BL_ERR_RANGE, E_BL_ERR_INVALIDARG
 */
HRESULT BASELIB_API __stdcall BL_IECDateAndTimeStrToUTCJava(const wchar_t* szInput, 
                                                            SYSTEMTIME* lpSystime, 
                                                            __int64*    pUTCmillis,
                                                            const wchar_t** pszEnd)
{
    LPCTSTR  psz    = szInput;
    HRESULT  hr     = S_OK;
    __int64 i64year;
    __int64 i64month;
    __int64 i64day;
    __int64 i64dayHour;
    __int64 i64dayMinute;
    __int64 i64daySecond;
    WORD    wMillis = 0;//note: millis are optional, therefore init them to zero        
    
    
    assert(!(!szInput || (pUTCmillis && !lpSystime)));
    if(!szInput || (pUTCmillis && !lpSystime))
    {
        hr = E_BL_ERR_INVALIDARG;
        goto error;
    }
    
    
    //test for the required 'date_And_Time#|dt#' type prefix:
    psz = BL_StripPrefixI(psz, _T("DT#"));
    
    if(psz==szInput)
    {
        psz = BL_StripPrefixI(psz, _T("DATE_AND_TIME#"));
        if(psz==szInput)
        {
            hr = E_BL_ERR_SYNTAX;
            goto error;
        }
    }


    //year
    hr = BL_ParseInt64(psz, lpSystime ? &i64year : NULL, false, 10, &psz);
    if(hr != S_OK)
        goto error;

    //get the '-'
    if(*psz!=_T('-'))
    {
        hr = E_BL_ERR_SYNTAX;
        goto error;
    }
    psz++;

    //month
    hr = BL_ParseInt64(psz, lpSystime ? &i64month : NULL, false, 10, &psz);
    if(hr != S_OK)
        goto error;

    //get the '-'
    if(*psz!=_T('-'))
    {
        hr = E_BL_ERR_SYNTAX;
        goto error;
    }
    psz++;
    

    //day
    hr = BL_ParseInt64(psz, lpSystime ? &i64day : NULL, false, 10, &psz);
    if(hr != S_OK)
        goto error;

    //get the '-'
    if(*psz!=_T('-'))
    {
        hr = E_BL_ERR_SYNTAX;
        goto error;
    }
    psz++;
    
    
    //day hour
    hr = BL_ParseInt64(psz, lpSystime ? &i64dayHour : NULL, false, 10, &psz);
    if(hr != S_OK)
        goto error;

    //get the ':'
    if(*psz!=_T(':'))
    {
        hr = E_BL_ERR_SYNTAX;
        goto error;
    }
    psz++;
    
    
    //day minute
    hr = BL_ParseInt64(psz, lpSystime ? &i64dayMinute : NULL, false, 10, &psz);
    if(hr != S_OK)
        goto error;

    //get the ':'
    if(*psz!=_T(':'))
    {
        hr = E_BL_ERR_SYNTAX;
        goto error;
    }
    psz++;

    
    //day second
    hr = BL_ParseInt64(psz, lpSystime ? &i64daySecond : NULL, false, 10, &psz);
    if(hr != S_OK)
        goto error;

    //get the optional fraction:
    if(*psz==_T('.'))
    {
        psz++;
        
        //first digit is required:
        if(BL_IsDigitBase10(*psz)<0)
        {
            hr = E_BL_ERR_SYNTAX;
            goto error;
        }

        int iFarcBase = 100;
        while(1)
        {
            if(*psz==_T('_'))
            {
                psz++;

                //a digit must follow:
                if(BL_IsDigitBase10(*psz)<0)
                {
                    hr = E_BL_ERR_SYNTAX;
                    goto error;
                }
            }
            
            int iDig = BL_IsDigitBase10(*psz);
            if(iDig<0)
                break;
            
            psz++;

            wMillis += iFarcBase * iDig;  
            iFarcBase /= 10;
        }
    }
    
    
    
    
    //scan has finished with no error, check ranges if lpSystime!=NULL:
    if(lpSystime)
    {
        //special case DT#0001-01-01-00:00:00.000
        //this value means 'invalid date_and_time' and is used e.g. by 4CCG
        //as default initial value
        if(i64year==1 && i64month==1 && i64day==1 && i64dayHour==0 &&
            i64dayMinute==0 && i64daySecond==0 && wMillis==0)
        {
            lpSystime->wYear = 1;
            lpSystime->wDayOfWeek = 0xffff;
            lpSystime->wMonth = 1;
            lpSystime->wDay = 1;
            lpSystime->wHour = 0;
            lpSystime->wMinute = 0;
            lpSystime->wSecond = 0;
            lpSystime->wMilliseconds = 0;

            if(pUTCmillis)
                *pUTCmillis = 0;
            goto specialCase;
        }
        
        
        if(i64year>BL_MAX_DATE_YEAR || i64year<BL_MIN_DATE_YEAR)
        {
            hr = E_BL_ERR_RANGE;
            goto error;
        }
        lpSystime->wYear = (WORD)i64year;
        
        if(i64month>12 || i64month<1)
        {
            hr = E_BL_ERR_RANGE;
            goto error;
        }
        lpSystime->wMonth = (WORD)i64month;

        lpSystime->wDayOfWeek = 0xffff;
        
        if(i64day>31 || i64day<1)
        {
            hr = E_BL_ERR_RANGE;
            goto error;
        }
        lpSystime->wDay = (WORD)i64day;


        if(i64dayHour>23 || i64dayHour<0)
        {
            hr = E_BL_ERR_RANGE;
            goto error;
        }
        lpSystime->wHour = (WORD)i64dayHour;


        if(i64dayMinute>59 || i64dayMinute<0)
        {
            hr = E_BL_ERR_RANGE;
            goto error;
        }
        lpSystime->wMinute = (WORD)i64dayMinute;

        
        if(i64daySecond>59 || i64daySecond<0)
        {
            hr = E_BL_ERR_RANGE;
            goto error;
        }
        lpSystime->wSecond = (WORD)i64daySecond;

        lpSystime->wMilliseconds = wMillis;

        if(pUTCmillis)
        {
            hr = BL_LocalSysTimeToUTCJava(lpSystime, pUTCmillis);
            if(FAILED(hr))
            {
                //fake if internal error, assert instead
                if(hr==E_BL_ERR_INTERNAL)
                {
                    assert(!"E_ERR_INTERNAL from BL_LocalSysTimeToUTCJava");
                    hr = E_BL_ERR_RANGE;
                }
                goto error;
            }
        }

    }

specialCase:
error:
    if(pszEnd)
        *pszEnd = psz;

    return hr;
}





/**
 * Convert a DATE string to SYSTEMTIME, assume IEC format.
 * The date in converted to days since 1970
 *
 * DATE constant syntax as lex script (the <type># prefix is scanned 
 * case insensitive):
 *
 * integer                 {digit}(_?{digit})*
 * year                    {integer}
 * month                   {integer}
 * day                     {integer}
 * date_literal            {year}-{month}-{day}
 * date                    (DATE|D)#{date_literal}
 *
 * @param       szInput        string to convert
 * @param       lpSystime      output value, if NULL only the syntax is checked
 * @param       pDaysSince1970 output value, may be NULL, if not NULL then 
 *                             lpSystime must not be NULL too. 
 *                             pDaysSince1970 will be set to the number of 
 *                             days past since 1.1.1970. 
 * @param       pszEnd         set to char that stopped the scan, if not NULL
 * @return      S_OK or E_BL_ERR_SYNTAX, E_BL_ERR_RANGE, E_BL_ERR_INVALIDARG
 */
HRESULT BASELIB_API __stdcall BL_IECDateStrToDaysSince1970(const wchar_t* szInput, 
                                                           SYSTEMTIME* lpSystime,
                                                           long* pDaysSince1970,
                                                           const wchar_t** pszEnd)
{
    LPCTSTR  psz    = szInput;
    HRESULT  hr     = S_OK;
    __int64 i64year;
    __int64 i64month;
    __int64 i64day;
    
    
    assert(!(!szInput || (pDaysSince1970 && !lpSystime)));
    if(!szInput || (pDaysSince1970 && !lpSystime))
    {
        hr = E_BL_ERR_INVALIDARG;
        goto error;
    }
    
    
    //test for the required 'date#|d#' type prefix:
    psz = BL_StripPrefixI(psz, _T("D#"));
    
    if(psz==szInput)
    {
        psz = BL_StripPrefixI(psz, _T("DATE#"));
        if(psz==szInput)
        {
            hr = E_BL_ERR_SYNTAX;
            goto error;
        }
    }


    //year
    hr = BL_ParseInt64(psz, lpSystime ? &i64year : NULL, false, 10, &psz);
    if(hr != S_OK)
        goto error;

    //get the '-'
    if(*psz!=_T('-'))
    {
        hr = E_BL_ERR_SYNTAX;
        goto error;
    }
    psz++;

    //month
    hr = BL_ParseInt64(psz, lpSystime ? &i64month : NULL, false, 10, &psz);
    if(hr != S_OK)
        goto error;

    //get the '-'
    if(*psz!=_T('-'))
    {
        hr = E_BL_ERR_SYNTAX;
        goto error;
    }
    psz++;
    

    //day
    hr = BL_ParseInt64(psz, lpSystime ? &i64day : NULL, false, 10, &psz);
    if(hr != S_OK)
        goto error;

    
    //scan has finished with no error, check ranges if lpSystime!=NULL:
    if(lpSystime)
    {
        //special case D#0001-01-01
        //this value means 'invalid date' and is used e.g. by 4CCG
        //as default initial value
        if(i64year==1 && i64month==1 && i64day==1)
        {
            lpSystime->wYear = 1;
            lpSystime->wDayOfWeek = 0xffff;
            lpSystime->wMonth = 1;
            lpSystime->wDay = 1;
            lpSystime->wHour = 0;
            lpSystime->wMinute = 0;
            lpSystime->wSecond = 0;
            lpSystime->wMilliseconds = 0;
            
            if(pDaysSince1970)
                *pDaysSince1970 = 0;
            
            goto specialCase;
        }

        
        
        if(i64year>BL_MAX_DATE_YEAR || i64year<BL_MIN_DATE_YEAR)
        {
            hr = E_BL_ERR_RANGE;
            goto error;
        }
        lpSystime->wYear = (WORD)i64year;
        
        if(i64month>12 || i64month<1)
        {
            hr = E_BL_ERR_RANGE;
            goto error;
        }
        lpSystime->wMonth = (WORD)i64month;

        lpSystime->wDayOfWeek = 0xffff;
        
        if(i64day>31 || i64day<1)
        {
            hr = E_BL_ERR_RANGE;
            goto error;
        }
        lpSystime->wDay = (WORD)i64day;


        lpSystime->wHour = 0;
        lpSystime->wMinute = 0;
        lpSystime->wSecond = 0;
        lpSystime->wMilliseconds = 0;

        if(pDaysSince1970)
        {
            FILETIME fileTime;
            if(!SystemTimeToFileTime(lpSystime, &fileTime))
            {
                //fake if internal error, assert instead
                assert(!"SystemTimeToFileTime returned false");
                hr = E_BL_ERR_RANGE;
                goto error;
            }
            
            __int64 iMillisSince1970 = ScaleFileTimeToJavaUTCmillis(&fileTime);
            __int64  daysSince1970 = iMillisSince1970/(24*3600*1000);
            
            assert(daysSince1970>0 && daysSince1970<LONG_MAX);

            *pDaysSince1970 = (long)daysSince1970;
        }
    }

specialCase:
error:
    if(pszEnd)
        *pszEnd = psz;

    return hr;
}


/**
 * Convert a TOD string to SYSTEMTIME, assume IEC format. The TOD is
 * converted to the number of milliseconds since midnight.
 *
 * TOD constant syntax as lex script (the <type># prefix is scanned 
 * case insensitive):
 *
 * integer                 {digit}(_?{digit})*
 * day_hour                {integer}
 * day_minute              {integer}
 * day_second              {fixed_point}
 * daytime                 {day_hour}:{day_minute}:{day_second}
 * tod                     (TIME_OF_DAY|TOD)#{daytime}
 *
 * @param       szInput     string to convert
 * @param       lpSystime   output value, if NULL only the syntax is checked
 * @param       pMillis     output value, may be NULL, if not NULL then 
 *                          lpSystime must not be NULL too. 
 *                          pMillis will be set to the number of milliseconds
 * @param       pszEnd      set to char that stopped the scan, if not NULL
 * @return      S_OK or E_BL_ERR_SYNTAX, E_BL_ERR_RANGE, E_BL_ERR_INVALIDARG
 */
HRESULT BASELIB_API __stdcall BL_IECTimeOfDayStrToMillis(const wchar_t* szInput, 
                                                         SYSTEMTIME* lpSystime,
                                                         long*    pMillis,
                                                         const wchar_t** pszEnd)
{
    LPCTSTR  psz    = szInput;
    HRESULT  hr     = S_OK;
    __int64 i64dayHour;
    __int64 i64dayMinute;
    __int64 i64daySecond;
    WORD    wMillis = 0;//note: millis are optional, therefore init them to zero        
    
    
    assert(!(!szInput || (pMillis && !lpSystime)));
    if(!szInput || (pMillis && !lpSystime))
    {
        hr = E_BL_ERR_INVALIDARG;
        goto error;
    }
    
    
    //test for the required 'time_of_day#|tod#' type prefix:
    psz = BL_StripPrefixI(psz, _T("TOD#"));
    
    if(psz==szInput)
    {
        psz = BL_StripPrefixI(psz, _T("TIME_OF_DAY#"));
        if(psz==szInput)
        {
            hr = E_BL_ERR_SYNTAX;
            goto error;
        }
    }


    //day hour
    hr = BL_ParseInt64(psz, lpSystime ? &i64dayHour : NULL, false, 10, &psz);
    if(hr != S_OK)
        goto error;

    //get the ':'
    if(*psz!=_T(':'))
    {
        hr = E_BL_ERR_SYNTAX;
        goto error;
    }
    psz++;
    
    
    //day minute
    hr = BL_ParseInt64(psz, lpSystime ? &i64dayMinute : NULL, false, 10, &psz);
    if(hr != S_OK)
        goto error;

    //get the ':'
    if(*psz!=_T(':'))
    {
        hr = E_BL_ERR_SYNTAX;
        goto error;
    }
    psz++;

    
    //day second
    hr = BL_ParseInt64(psz, lpSystime ? &i64daySecond : NULL, false, 10, &psz);
    if(hr != S_OK)
        goto error;

    //get the optional fraction:
    if(*psz==_T('.'))
    {
        psz++;
        
        //first digit is required:
        if(BL_IsDigitBase10(*psz)<0)
        {
            hr = E_BL_ERR_SYNTAX;
            goto error;
        }

        int iFarcBase = 100;
        while(1)
        {
            if(*psz==_T('_'))
            {
                psz++;

                //a digit must follow:
                if(BL_IsDigitBase10(*psz)<0)
                {
                    hr = E_BL_ERR_SYNTAX;
                    goto error;
                }
            }
            
            int iDig = BL_IsDigitBase10(*psz);
            if(iDig<0)
                break;
            
            psz++;

            wMillis += iFarcBase * iDig;  
            iFarcBase /= 10;
        }
    }
    
    
    
    
    //scan has finished with no error, check ranges if lpSystime!=NULL:
    if(lpSystime)
    {
        lpSystime->wYear = 0;
        lpSystime->wMonth= 0;
        lpSystime->wDay  = 0;
        
        if(i64dayHour>23 || i64dayHour<0)
        {
            hr = E_BL_ERR_RANGE;
            goto error;
        }
        lpSystime->wHour = (WORD)i64dayHour;


        if(i64dayMinute>59 || i64dayMinute<0)
        {
            hr = E_BL_ERR_RANGE;
            goto error;
        }
        lpSystime->wMinute = (WORD)i64dayMinute;

        
        if(i64daySecond>59 || i64daySecond<0)
        {
            hr = E_BL_ERR_RANGE;
            goto error;
        }
        lpSystime->wSecond = (WORD)i64daySecond;


        lpSystime->wMilliseconds = wMillis;

        if(pMillis)
        {
            *pMillis = lpSystime->wHour   * 3600*1000 + 
                       lpSystime->wMinute *   60*1000 +
                       lpSystime->wSecond *      1000 +
                       wMillis;                      
        }
    }


error:
    if(pszEnd)
        *pszEnd = psz;

    return hr;
}












/**
 * Convert a time in ms into a IEC String
 *
 * @param       i64Value    time value in ms to convert
 * @param       pIecStr     output value
 * @return      S_OK or E_BL_ERR_BUFFER_TO_SMALL
 */
HRESULT BL_TimeToIECStrDefault(__int64 i64Value, LPTSTR buffer, int nChars)
{
    TCHAR itoaBuff[50];    
    
    assert(buffer);

    if(!buffer || nChars<64)
        return E_BL_ERR_BUFFER_TO_SMALL;


    //_i64tot does not word with negative input !!
    if(i64Value<0)
    {
        _stprintf(buffer, _T("t#-%sms"), _ui64tot((unsigned __int64)-i64Value,itoaBuff,10));
    }
    else
    {
        _stprintf(buffer, _T("t#%sms"), _ui64tot((unsigned __int64)+i64Value,itoaBuff,10));
    }
    
    assert((int)_tcslen(buffer)<nChars-1);

    return S_OK;
}


/**
 * Convert a DATE_AND_TIME value into an IEC String
 *
 * @param       i64Value    input: DT value in ms UTC since 1.1.1970
 * @param       pIecStr     output value format dt#1998-8-23-12:23:34.000
 * @return      S_OK or E_BL_ERR_BUFFER_TO_SMALL, E_BL_ERR_RANGE
 */
HRESULT BL_DateAndTimeToIECStr(__int64 i64Value, LPTSTR buffer, int nChars)
{
    SYSTEMTIME      systime;
    TCHAR           buff[256];
    HRESULT         hr;
    const TCHAR*    pszSpecial = _T("dt#0001-01-01-00:00:00.000");


    //special case DT#0001-01-01-00:00:00.000
    if(i64Value==0)
    {
        if((int)_tcslen(pszSpecial) >= nChars)
            return E_BL_ERR_BUFFER_TO_SMALL;

        _tcscpy(buffer, pszSpecial);

        return S_OK;
    }


    hr = BL_UTCJavaToLocalSysTime(i64Value, &systime);
    if(FAILED(hr))
    {
        //fake if internal error, assert instead
        if(hr==E_BL_ERR_INTERNAL)
        {
            assert(!"E_ERR_INTERNAL from BL_LocalSysTimeToUTCJava");
            hr = E_BL_ERR_RANGE;
        }
        return hr;
    }


    _stprintf(buff, _T("dt#%04d-%02d-%02d-%02d:%02d:%02d.%03d"), 
        systime.wYear,systime.wMonth, systime.wDay,
        systime.wHour,systime.wMinute, systime.wSecond, systime.wMilliseconds);


    if((int)_tcslen(buff) >= nChars)
        return E_BL_ERR_BUFFER_TO_SMALL;

    _tcscpy(buffer, buff);

    return S_OK;
}

/**
 * Convert a DATE value into an IEC String
 *
 * @param       lDaysSince1970  input: DT value in days since 1.1.1970
 * @param       buffer          output value format dt#1998-8-23-12:23:34.000
 * @param       nChars          input buffer length in chars
 * @return      S_OK or E_BL_ERR_BUFFER_TO_SMALL, E_BL_ERR_RANGE
 */
HRESULT BL_DateToIECStr(long lDaysSince1970, LPTSTR buffer, int nChars)
{

    TCHAR           buff[256];
    FILETIME        fileTime;
    SYSTEMTIME      systime;
    const TCHAR*    pszSpecial = _T("d#0001-01-01");
    
    //special case DT#0001-01-01
    if(lDaysSince1970==0)
    {
        if((int)_tcslen(pszSpecial) >= nChars)
            return E_BL_ERR_BUFFER_TO_SMALL;

        _tcscpy(buffer,pszSpecial);

        return S_OK;
    }

    __int64 i64DaysIn100ns;
    if (!BL_safe_mul_int64(&i64DaysIn100ns, lDaysSince1970, 24*3600*1000i64))
        return E_BL_ERR_RANGE;

    if(!UTCJavaToUTCWin32(i64DaysIn100ns, &fileTime))
        return E_BL_ERR_RANGE;

    if(!FileTimeToSystemTime(&fileTime, &systime))
        return E_BL_ERR_RANGE;

    assert(systime.wSecond==0);

    _stprintf(buff, _T("d#%04d-%02d-%02d"), systime.wYear, systime.wMonth, systime.wDay);

    if((int)_tcslen(buff) >= nChars)
        return E_BL_ERR_BUFFER_TO_SMALL;

    _tcscpy(buffer, buff);

    return S_OK;
}


/**
 * Convert a TOD value into an IEC String
 *
 * @param       lMillis  input: ms since midnight
 * @param       buffer   output value format dt#1998-8-23-12:23:34.000
 * @param       nChars   input buffer length in chars
 * @return      S_OK or E_BL_ERR_BUFFER_TO_SMALL
 */
HRESULT BL_TimeOfDayToIECStr(long lMillis, LPTSTR buffer, int nChars)
{
    TCHAR           buff[256];

    long lMsecs =  lMillis / 1;
    long lSecs  =  lMsecs / 1000;
    long lMins  =  lSecs / 60;
    long lHours =  lMins / 60;
    
    lMsecs =  lMsecs % 1000;
    lSecs  =  lSecs % 60;
    lMins  =  lMins % 60;

    _stprintf(buff, _T("tod#%02d:%02d:%02d.%03d"), lHours, lMins, lSecs, lMsecs);

    if((int)_tcslen(buff) >= nChars)
        return E_BL_ERR_BUFFER_TO_SMALL;

    _tcscpy(buffer, buff);

    return S_OK;
}


/**
 * Parses the string argument as a signed or unsigned __int64
 * If pOut!=NULL the range is checked for an LINT (same range as __int64)
 * If pOut==NULL only the syntax will be checked (then there is no 
 * return value 'E_BL_ERR_RANGE')
 *
 * @param      psz         the String containing the int64
 * @param      pOut        pointer to result, may be NULL
 * @param      radix       the radix one of 2,8,10 or 16
 * @param      pszEnd      set to char that stopped the scan, if not NULL
 * @param      bAllowSign  if true an optional + or - is allowed and range check is 
 *                         signed int64, if false a leading + or - results in a syntax error
 *                         and the range check is for unsigned int64.
 *                         NOTE: if bAllowSign==false the caller must interpret *pOut as 
 *                         unsigned int64.
 * @return     S_OK or E_BL_ERR_SYNTAX,E_BL_ERR_RANGE
 */
HRESULT BASELIB_API __stdcall BL_ParseInt64(const wchar_t* psz, 
                                            __int64* pOut, 
                                            bool bAllowSign,
                                            int radix,
                                            const wchar_t** pszEnd)
{
    unsigned __int64 result = 0;
    bool        negative = false;
    unsigned __int64 limit = _UI64_MAX;
    unsigned __int64 multmax;
    int         digit;
    HRESULT     hr=S_OK;

    assert(psz);


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


HRESULT BASELIB_API BL_ParseFormat (LPCOLESTR pszFormat, struct BL_FormatInfo *pFormatInfo)
{
    struct BL_FormatInfo formatInfo;
    HRESULT hr;
    HRESULT hrMinor=S_OK;
    bool bHaveBase=false;

    assert (pszFormat != NULL);
    if (pszFormat == NULL)
    {
        return (E_FAIL);
    };

    formatInfo.m_Kind = BL_UnspecifiedFormat;
    formatInfo.m_iWidth = -1;
    formatInfo.m_chPadding = '\0';
    formatInfo.m_Alignment = BL_UnspecifiedAlignment;
    formatInfo.m_iBytesPerItem = -1;
    formatInfo.m_iRadix = -1;
    formatInfo.m_iSignedness = -1;
    formatInfo.m_iPrecision = -1;

    if (*pszFormat == '\0')
    {
        if (pFormatInfo != NULL)
        {
            *pFormatInfo = formatInfo;
        };

        return (S_FALSE);
    };

    if (BL_IsDigitBase10 (*pszFormat) >= 0)
    {
        unsigned __int64 ullRadix;
        LPCOLESTR pszEnd;

        bHaveBase = true;

        hr = BL_ParseInt64 (pszFormat, (__int64 *)&ullRadix, false, 10, &pszEnd);
        if (FAILED (hr))
        {
            if (hr == E_BL_ERR_SYNTAX)
            {
                return (hr);
            };

            if (hrMinor == S_OK)
            {
                hrMinor = hr;
            };
        };
        pszFormat = pszEnd;

        if (*pszFormat != '#')
        {
            return (E_BL_ERR_SYNTAX);
        };
        ++pszFormat;

        if (SUCCEEDED (hr))
        {
            switch (ullRadix)
            {
            case 2: case 8: case 10: case 16:
                formatInfo.m_iRadix = (int)ullRadix;
                break;

            default:
                if (hrMinor == S_OK)
                {
                    hrMinor = E_BL_ERR_RADIX;
                };
                break;
            };
        };
    };

    switch (*pszFormat)
    {
    case 'b': case 'B':
        formatInfo.m_Kind = BL_IntegerFormat;
        formatInfo.m_iBytesPerItem = sizeof (__int8);
        ++pszFormat;
        break;

    case 'w': case 'W':
        formatInfo.m_Kind = BL_IntegerFormat;
        formatInfo.m_iBytesPerItem = sizeof (__int16);
        ++pszFormat;
        break;

    case 'd': case 'D':
        formatInfo.m_Kind = BL_IntegerFormat;
        formatInfo.m_iBytesPerItem = sizeof (__int32);
        ++pszFormat;
        break;

    case 'l': case 'L':
        formatInfo.m_Kind = BL_IntegerFormat;
        formatInfo.m_iBytesPerItem = sizeof (__int64);
        ++pszFormat;
        break;

    case 'e': case 'E':
        formatInfo.m_Kind = BL_FloatingRealFormat;
        formatInfo.m_iBytesPerItem = *pszFormat == 'E' ? sizeof (double) : sizeof (float);
        ++pszFormat;
        break;

    case 'f': case 'F':
        formatInfo.m_Kind = BL_FixedRealFormat;
        formatInfo.m_iBytesPerItem = *pszFormat == 'F' ? sizeof (double) : sizeof (float);
        ++pszFormat;
        break;

    case 's': case 'S':
        formatInfo.m_Kind = BL_StringFormat;
        formatInfo.m_iBytesPerItem = *pszFormat == 'S' ? sizeof (WCHAR) : sizeof (CHAR);
        ++pszFormat;
        break;

    default:
        if (!bHaveBase)
        {
            return (E_BL_ERR_SYNTAX);
        };
        break;
    };

    if (formatInfo.m_Kind == BL_IntegerFormat)
    {
        switch (*pszFormat)
        {
        case 's': case 'S':
            formatInfo.m_iSignedness = 1;
            ++pszFormat;
            break;

        case 'u': case 'U':
            formatInfo.m_iSignedness = 0;
            ++pszFormat;
            break;
        };
    };

    if (*pszFormat == ' ' || *pszFormat == '0')
    {
        if (*pszFormat == '0' && formatInfo.m_Kind == BL_StringFormat)
        {
            return (E_BL_ERR_SYNTAX);
        };

        formatInfo.m_Alignment = BL_RightAlign;
        formatInfo.m_chPadding = *pszFormat;
        ++pszFormat;
    };

    if (BL_IsDigitBase10 (*pszFormat) >= 0)
    {
        unsigned __int64 ullWidth;
        LPCOLESTR pszEnd;

        hr = BL_ParseInt64 (pszFormat, (__int64 *)&ullWidth, false, 10, &pszEnd);
        if (FAILED (hr))
        {
            if (hr == E_BL_ERR_SYNTAX)
            {
                return (hr);
            };

            if (hrMinor == S_OK)
            {
                hrMinor = hr;
            };
        };
        pszFormat = pszEnd;

        formatInfo.m_iWidth = (int)ullWidth;
        if (formatInfo.m_iWidth < 0 ||
            (unsigned __int64)formatInfo.m_iWidth != ullWidth)
        {
            if (hrMinor == S_OK)
            {
                hrMinor = E_BL_ERR_RANGE;
            };
        };
    };

    if (formatInfo.m_Kind == BL_FloatingRealFormat || formatInfo.m_Kind == BL_FixedRealFormat)
    {
        if (*pszFormat == '.')
        {
            unsigned __int64 ullDigits;
            LPCOLESTR pszEnd;

            ++pszFormat;

            hr = BL_ParseInt64 (pszFormat, (__int64 *)&ullDigits, false, 10, &pszEnd);
            if (FAILED (hr))
            {
                if (hr == E_BL_ERR_SYNTAX)
                {
                    return (hr);
                };

                if (hrMinor == S_OK)
                {
                    hrMinor = hr;
                };
            };
            pszFormat = pszEnd;

            if (SUCCEEDED (hr))
            {
                formatInfo.m_iPrecision = (int)ullDigits;

                if (formatInfo.m_iPrecision < 0 ||
                    (unsigned __int64)formatInfo.m_iPrecision != ullDigits)
                {
                    if (hrMinor == S_OK)
                    {
                        hrMinor = E_BL_ERR_RANGE;
                    };
                };
            };
        };
    };

    if (*pszFormat != '\0')
    {
        return (E_BL_ERR_SYNTAX);
    };

    if (hrMinor == S_OK && pFormatInfo != NULL)
    {
        *pFormatInfo = formatInfo;
    };

    return (hrMinor);
}


HRESULT BASELIB_API BL_ParseFormatForType (LPCOLESTR pszFormat, enum BL_IEC_TYP type, struct BL_FormatInfo *pFormatInfo)
{
    HRESULT hr;
    struct BL_FormatInfo formatInfo;
    HRESULT hrCheck;

    hr = BL_ParseFormat (pszFormat, &formatInfo);
    if (FAILED (hr))
    {
        return (hr);
    };

    hrCheck = BL_CheckFormatForType (type, &formatInfo);
    if (FAILED (hrCheck))
    {
        return (hrCheck);
    };

    if (pFormatInfo != NULL)
    {
        *pFormatInfo = formatInfo;
    };

    return (hr);
}


HRESULT BASELIB_API BL_CheckFormatForType (enum BL_IEC_TYP type, struct BL_FormatInfo *pFormatInfo)
{
    HRESULT hr;

    assert (pFormatInfo != NULL);
    if (pFormatInfo == NULL)
    {
        return (E_FAIL);
    };

    assert (type != BL_ANY);
    if (type == BL_ANY)
    {
        return (E_FAIL);
    };

    // Fill in all parameters that require default values.
    // Note that this may lead to an invalid format info
    // which will be rejected in CheckFormatInfo.

    GetFormatInfoDefaults (type, pFormatInfo);

    hr = CheckFormatInfo (type, pFormatInfo);
    if (FAILED (hr))
    {
        return (hr);
    };

    return (S_OK);
}


static void GetFormatInfoDefaults (enum BL_IEC_TYP type, struct BL_FormatInfo *pFormatInfo)
{
    if (pFormatInfo->m_Kind == BL_UnspecifiedFormat)
    {
        switch (type)
        {
        case BL_INT: case BL_DINT: case BL_LINT:
        //case BL_SINT: 
        //case BL_USINT: 
        case BL_UINT: case BL_UDINT: case BL_ULINT:
        case BL_BYTE: case BL_WORD: case BL_DWORD: case BL_LWORD:
            pFormatInfo->m_Kind = BL_IntegerFormat;
            break;

        case BL_REAL:
            pFormatInfo->m_Kind = BL_FloatingRealFormat;
            break;

        case BL_LREAL:
            pFormatInfo->m_Kind = BL_FloatingRealFormat;
            break;

        case BL_STRING:
            pFormatInfo->m_Kind = BL_StringFormat;
            break;

        case BL_WSTRING:
            pFormatInfo->m_Kind = BL_StringFormat;
            break;

        default:
            pFormatInfo->m_Kind = BL_IntegerFormat;
            break;
        };
    };

    if (pFormatInfo->m_iBytesPerItem == -1)
    {
        switch (pFormatInfo->m_Kind)
        {
        case BL_IntegerFormat:
            switch (type)
            {
            case BL_WORD: case BL_WSTRING:  case BL_INT: case BL_UINT:
                pFormatInfo->m_iBytesPerItem = 2;
                break;

            case BL_DINT: case BL_DWORD: case BL_REAL: case BL_UDINT: 
                pFormatInfo->m_iBytesPerItem = 4;
                break;

            case BL_LINT: case BL_LWORD: case BL_LREAL: case BL_ULINT:
                pFormatInfo->m_iBytesPerItem = 8;
                break;

            default:
                pFormatInfo->m_iBytesPerItem = 1;
                break;
            };
            break;

        case BL_FloatingRealFormat: case BL_FixedRealFormat:

            switch (type)
            {
            case BL_REAL: case BL_DINT: case BL_DWORD: //case BL_UDINT: 
                pFormatInfo->m_iBytesPerItem = 4;
                break;

            default:
                pFormatInfo->m_iBytesPerItem = 8;
                break;
            };
            break;

        case BL_StringFormat:
            // uncomment as soon as BL_STRING is available
            //switch (type)
            //{
            //case BL_SINT: case BL_USINT: case BL_BYTE:
            //    pFormatInfo->m_iBytesPerItem = 1;
            //    break;

            //default:
                pFormatInfo->m_iBytesPerItem = 2;
            //    break;
            //};
            break;
        };
    };

    if (pFormatInfo->m_iWidth == -1)
    {
        switch (pFormatInfo->m_Kind)
        {
        case BL_FloatingRealFormat:
            // 0.123E-10 => 9 characters
            pFormatInfo->m_iWidth = 9;
            break;

        case BL_FixedRealFormat:
            // 123456.789 => 10 characters
            pFormatInfo->m_iWidth = 10;
            break;

        default:
            // as wide as necessary
            pFormatInfo->m_iWidth = 0;
            break;
        };
    };

    if (pFormatInfo->m_Alignment == BL_UnspecifiedAlignment)
    {
        pFormatInfo->m_Alignment = BL_LeftAlign;
    };

    if (pFormatInfo->m_chPadding == '\0')
    {
        pFormatInfo->m_chPadding = ' ';
    };

    if (pFormatInfo->m_Kind == BL_IntegerFormat)
    {
        if (pFormatInfo->m_iRadix == -1)
        {
            switch (type)
            {
            //case BL_SINT: 
            case BL_INT:
            //case BL_USINT: 
            case BL_UINT: case BL_UDINT: case BL_ULINT:
            case BL_DINT: case BL_LINT:
                pFormatInfo->m_iRadix = 10;
                break;

            default:
                pFormatInfo->m_iRadix = 16;
                break;
            };
        };

        if (pFormatInfo->m_iSignedness == -1)
        {
            switch (pFormatInfo->m_iRadix)
            {
            case 10:
                if(type==BL_UINT || type==BL_UDINT || type==BL_ULINT)
                    pFormatInfo->m_iSignedness = 0;
                else
                    pFormatInfo->m_iSignedness = 1;
                break;

            default:
                pFormatInfo->m_iSignedness = 0;
                break;
            };
        };
    };

    if (pFormatInfo->m_Kind == BL_FloatingRealFormat || pFormatInfo->m_Kind == BL_FixedRealFormat)
    {
        if (pFormatInfo->m_iPrecision == -1)
        {
            pFormatInfo->m_iPrecision = 3;
        };
    };
}


static HRESULT CheckFormatInfo (enum BL_IEC_TYP type, const struct BL_FormatInfo *pFormatInfo)
{
    switch (type)
    {
    //case BL_SINT: case BL_USINT:
    case BL_BYTE:
        if (pFormatInfo->m_iBytesPerItem != 1)
            return (E_BL_ERR_FORMATERROR);
        break;

    case BL_INT: case BL_UINT:
    case BL_WORD:
        if (pFormatInfo->m_iBytesPerItem != 1 &&
            pFormatInfo->m_iBytesPerItem != 2)
            return (E_BL_ERR_FORMATERROR);
        break;

    case BL_UDINT:
    case BL_DINT:
    case BL_DWORD:
    case BL_REAL:
        if (pFormatInfo->m_iBytesPerItem != 1 &&
            pFormatInfo->m_iBytesPerItem != 2 &&
            pFormatInfo->m_iBytesPerItem != 4)
            return (E_BL_ERR_FORMATERROR);
        break;

    case BL_ULINT:
    case BL_LINT:
    case BL_LWORD:
    case BL_LREAL:
        if (pFormatInfo->m_iBytesPerItem != 1 &&
            pFormatInfo->m_iBytesPerItem != 2 &&
            pFormatInfo->m_iBytesPerItem != 4 &&
            pFormatInfo->m_iBytesPerItem != 8)
            return (E_BL_ERR_FORMATERROR);
        break;

    case BL_STRING:
    case BL_WSTRING:
        // no way to tell whether string can be
        // displayed in some other representation
        break;
    };

    if (pFormatInfo->m_Kind == BL_StringFormat && pFormatInfo->m_chPadding == '0')
    {
        return (E_BL_ERR_FORMATERROR);
    };

    return (S_OK);
}


static HRESULT MakeVT_Decimal(VARIANTARG* pDest, VARIANTARG* pSrc, __int64* pi64)
{
    HRESULT hr;
    
    
    hr = VariantChangeType(pDest, pSrc, 0, VT_DECIMAL);
    if (FAILED(hr))
        return hr;

    //watch the scale !
    while(pDest->decVal.scale != 0)
    {
        pDest->decVal.scale--;
        pDest->decVal.Lo64 /= (unsigned __int64)10;
        pDest->decVal.Lo64 += (pDest->decVal.Hi32 % 10ul) * (~(unsigned __int64)0 / 10ul);
        pDest->decVal.Hi32 /= 10ul;
    }


    if (pDest->decVal.sign==DECIMAL_NEG)
    {
        *pi64 = pDest->decVal.Lo64;
        *pi64 = - *pi64;
    }
    else
       *pi64 = +pDest->decVal.Lo64;


    return hr;
}




/**
 * Converts a 'DST transition date and time' into a file time.
 * The meaning of the members of a SYSTEMTIME struct if used
 * as 'DST transition date and time' is a defined by TIME_ZONE_INFORMATION
 *
 * @param    pTransSpec       input: a transition date and time from TIME_ZONE_INFORMATION
 * @param    wYearInQuestion  input: the wYear from a SYSTEMTIME which wants to be
 *                            compared with the file time of a transition date.
 * @return   S_OK or E_BL_ERR_INTERNAL
 */
static HRESULT GetTransitionFileTimeFromSystime(const SYSTEMTIME* pTransSpec, 
                                                WORD wYearInQuestion,
                                                FILETIME* pTransTime)
{

    //if there is an absolute period specified the wYear 
    //member tzi.StandardDate is non zero:   
    if(pTransSpec->wYear!=0)
    {
        if(!SystemTimeToFileTime(pTransSpec, pTransTime))
            return E_BL_ERR_INTERNAL;
        return S_OK;
    }


    //ok, now we must handle the Day-in-month format.
    //The task is as follows: evaluate the localDayligthDate and localStandardDate
    //for the yaer in question (that is pSystime->wYear), then proceed as in 
    //the case of an absolute format in TIME_ZONE_INFORMATION.

    //first find out what is the weekday of the first 
    //date in the transition months or what is the frist day in the month following
    //the transition month (the later is used if tzi.StandardDate.wDay equals 5, 
    //which means the 'last wDayOfWeek')
    
    SYSTEMTIME tmpST;
    FILETIME tmpFT;
    tmpST.wYear        = wYearInQuestion;
    tmpST.wMonth       = pTransSpec->wMonth;
    tmpST.wDayOfWeek   = 0xffff;//this field is not read by SystemTimeToFileTime
    tmpST.wDay         = 1;
    tmpST.wHour        = pTransSpec->wHour;
    tmpST.wMinute      = pTransSpec->wMinute;
    tmpST.wSecond      = pTransSpec->wSecond;
    tmpST.wMilliseconds= pTransSpec->wMilliseconds;

    if(pTransSpec->wDay==5)
    {
        tmpST.wMonth++;
        if(tmpST.wMonth>12)
        {
            tmpST.wMonth = 1;
            tmpST.wYear++;
        }
    }


    if(!SystemTimeToFileTime(&tmpST, &tmpFT))
        return E_BL_ERR_INTERNAL;
    
    if(!FileTimeToSystemTime(&tmpFT, &tmpST))
        return E_BL_ERR_INTERNAL;


    
    //now tmpST.wDayOfWeek is set corretly.
    //we can now just add/subtract full days as needed (note that the day time of
    //the transition is already included in tmpFT)
    LARGE_INTEGER  lint;
    const __int64 DAYin100ns = 24i64*3600*1000*10000;

    lint.LowPart = tmpFT.dwLowDateTime;
    lint.HighPart= tmpFT.dwHighDateTime;

                                  //(wanted day - first day in month(or next month + 7) % 7             
    lint.QuadPart += DAYin100ns * ((pTransSpec->wDayOfWeek - tmpST.wDayOfWeek +7) %7);

    if(pTransSpec->wDay==5)
        lint.QuadPart -= DAYin100ns*7;
    else
        lint.QuadPart += DAYin100ns*7 *(pTransSpec->wDay-1);

    
    pTransTime->dwLowDateTime = lint.LowPart;
    pTransTime->dwHighDateTime= lint.HighPart;

    return S_OK;
}


/**
 * Checks if pSystime, if interpreted as a local time, 
 * falls in the period of day ligth saving time. 
 * This is done according to the information
 * stored in the TIME_ZONE_INFORMATION struct.
 *
 * @param    pSystime   input:  pSystime a system time struct
 *                      pSystime it expected to contain a 
 *                      valid sysem time, if not E_BL_ERR_INTERNAL
 *                      will be returned.
 * @parm     bInterpretAsUTC  true if pSystime is UTC, false pSystime
 *                            is interpreted as 'Armbanduhrzeit'
 * @param    pIsInDST   output: true if pSystime is in DTS, 
 *                      false otherwise
 * @param    pTzi       input: the time zone info to use
 *                      false otherwise
 * @return   S_OK or E_BL_ERR_INTERNAL
 */
static HRESULT IsInDayLightSavingPeriod(const SYSTEMTIME* pSystime, 
                                        bool bInterpretAsUTC,
                                        bool* pIsInDST,
                                        const TIME_ZONE_INFORMATION* pTzi)
{
    FILETIME    localFileTime;
    FILETIME    localStandardDate;
    FILETIME    localDayligthDate;

    if(!pSystime || !pIsInDST || !pTzi)
    {
        assert(pSystime && pIsInDST && pTzi);
        return E_BL_ERR_INTERNAL;
    }


    //if there is no DST specified the wMonth member in tzi.StandardDate is zero:
    if(pTzi->StandardDate.wMonth==0)
    {
        *pIsInDST = false;
        return S_OK;
    }

    //in order to see if pSystime is in the period, we convert
    //all system times to file time, this way comparison is made easy. 
    //Note the SytemTimeToFileTime does not perform use any time zone 
    //correction, this function handels only the leapyear corretion.
    //convert the two transition dates and the pSystime into filetimes:
    if(SUCCEEDED(GetTransitionFileTimeFromSystime(&pTzi->StandardDate, 
                 pSystime->wYear, &localStandardDate))  &&
       SUCCEEDED(GetTransitionFileTimeFromSystime(&pTzi->DaylightDate, 
                 pSystime->wYear, &localDayligthDate))  &&
       SystemTimeToFileTime(pSystime, &localFileTime)
      )
    {
        //localStandardDate is a time value in summer time
        //localDayligthDate is a time value in winter time
        if(bInterpretAsUTC)
        {
            LARGE_INTEGER   lint;
            const __int64   MinAs100ns = 60i64*1000*10000;


            lint.LowPart = localStandardDate.dwLowDateTime;
            lint.HighPart= localStandardDate.dwHighDateTime;

            //UTC = local time + Bias + DaylightBias
            lint.QuadPart += pTzi->Bias*MinAs100ns + pTzi->DaylightBias*MinAs100ns;
            localStandardDate.dwLowDateTime = lint.LowPart;
            localStandardDate.dwHighDateTime= lint.HighPart;


            lint.LowPart = localDayligthDate.dwLowDateTime;
            lint.HighPart= localDayligthDate.dwHighDateTime;

            //UTC = local time + Bias + StandardBias
            lint.QuadPart += pTzi->Bias*MinAs100ns + pTzi->StandardBias*MinAs100ns;
            localDayligthDate.dwLowDateTime = lint.LowPart;
            localDayligthDate.dwHighDateTime= lint.HighPart;
        }
        

        //check if norther or southern hemisphere:
        if(CompareFileTime(&localStandardDate, &localDayligthDate)>0)
        {
            //northern, localStandardDate > localDayligthDate
            if(CompareFileTime(&localFileTime, &localDayligthDate)>=0 
                && CompareFileTime(&localFileTime, &localStandardDate)<0)
                *pIsInDST = true;                
            else
                *pIsInDST = false;
        }
        else
        {
            //southern, localDayligthDate > localStandardDate
            if(CompareFileTime(&localFileTime, &localStandardDate)>=0 
                && CompareFileTime(&localFileTime, &localDayligthDate)<0)
                *pIsInDST = false;                
            else
                *pIsInDST = true;

        }
        return S_OK;
    }
    else
    {
        return E_BL_ERR_INTERNAL;
    }
    
}

static HRESULT BL_UTCJavaToLocalSysTime(__int64 UTCmillis, SYSTEMTIME* lpSystime)
{
    FILETIME                fileTime;
    TIME_ZONE_INFORMATION   tzi;
    SYSTEMTIME              UTCSystemTime;
    __int64                 localMillis;
    bool                    bInDST;
    
    //ns per minute
    const __int64 MINin100ns = 60i64*1000*10000;
    

    assert(lpSystime);
    if(!lpSystime)
        return E_BL_ERR_INVALIDARG;


    if(0xFFFFFFFF == GetTimeZoneInformation(&tzi))
    {
        assert(!"bad return from GetTimeZoneInformation");
        return E_BL_ERR_INTERNAL;
    }


    if(!UTCJavaToUTCWin32(UTCmillis, &fileTime))
        return E_BL_ERR_RANGE;


    if(!FileTimeToSystemTime(&fileTime, &UTCSystemTime))
        return E_BL_ERR_RANGE;
    
    //day ligth saving ?
    if(FAILED(IsInDayLightSavingPeriod(&UTCSystemTime, true, &bInDST, &tzi)))
        return E_BL_ERR_INTERNAL;

    if(bInDST)
    {
        //DST correction:
        //formula: localDST = UTC - Bias - DayligthBias
        localMillis = UTCmillis - tzi.Bias*(60*1000i64) - tzi.DaylightBias*(60*1000i64);
    }
    else
    {
        //Standard correction:
        //formula: localDST = UTC - Bias - StandardBias
        localMillis = UTCmillis - tzi.Bias*(60*1000i64) - tzi.StandardBias*(60*1000i64);
    }


    if(!UTCJavaToUTCWin32(localMillis, &fileTime))
        return E_BL_ERR_RANGE;

    if(!FileTimeToSystemTime(&fileTime, lpSystime))
        return E_BL_ERR_RANGE;
    
    return S_OK;
}


static HRESULT BL_LocalSysTimeToUTCJava(const SYSTEMTIME* lpSystime, __int64* pUTCmillis)
{
    FILETIME                fileTime;
    TIME_ZONE_INFORMATION   tzi;
    bool                    bInDST;
    
    assert(lpSystime && pUTCmillis);
    if(!lpSystime || !pUTCmillis)
        return E_BL_ERR_INVALIDARG;

    //note: call SystemTimeToFileTime(lpSystime, &fileTime) now
    //to check for range errors. An invalid lpSystime as 
    ///argument for IsInDayLightSavingPeriod would case in internal error
    if(!SystemTimeToFileTime(lpSystime, &fileTime))
        return E_BL_ERR_RANGE;

    if(0xFFFFFFFF == GetTimeZoneInformation(&tzi))
    {
        assert(!"bad return from GetTimeZoneInformation");
        return E_BL_ERR_INTERNAL;
    }


    //day ligth saving ?
    if(FAILED(IsInDayLightSavingPeriod(lpSystime, false, &bInDST, &tzi)))
        return E_BL_ERR_INTERNAL;
    

    __int64 localMillis = ScaleFileTimeToJavaUTCmillis(&fileTime);
    
    if(bInDST)
    {
        //DST correction:
        //formula: localDST = UTC - Bias - DayligthBias
        *pUTCmillis  = localMillis + (tzi.Bias + tzi.DaylightBias)*(60*1000i64);
    }
    else
    {
        //Standard correction:
        //formula: localDST = UTC - Bias - StandardBias
        *pUTCmillis  = localMillis + (tzi.Bias + tzi.StandardBias)*(60*1000i64);
    }

    return S_OK;
}

static __int64 ScaleFileTimeToJavaUTCmillis(const FILETIME* pFileTime)
{
    LARGE_INTEGER           lint;
    lint.LowPart = pFileTime->dwLowDateTime;
    lint.HighPart= pFileTime->dwHighDateTime;

    //days between 1601 and 1970:
    const __int64 i64corrDays = (1970-1601)*365 + (1970-1601)/4 - (1970-1601)/100 + (1970-1601)/400;

    //note: The FILETIME structure is a 64-bit value representing 
    //the number of 100-nanosecond intervals since January 1, 1601. 
    //UTCmillis are msec. since 1.1.1970:
    // 1ms == 1000000ns == 10000 * (100ns)
    return  lint.QuadPart/10000 - i64corrDays*24*3600*1000;

}
static bool UTCJavaToUTCWin32(__int64 UTCJava, FILETIME* pFileTime)
{
    LARGE_INTEGER lint;
    //days between 1601 and 1970:
    const __int64 i64corrDays = (1970-1601)*365 + (1970-1601)/4 - (1970-1601)/100 + (1970-1601)/400;

    //note: The FILETIME structure is a 64-bit value representing 
    //the number of 100-nanosecond intervals since January 1, 1601. 
    //UTCmillis are msec. since 1.1.1970:
    // 1ms == 1000000ns == 10000 * (100ns)
    
    //safe calc: pUTCWin32 = (UTCJava + i64corrDays*24*3600*1000)*10000;

    if(!BL_safe_add_int64(&lint.QuadPart, UTCJava , i64corrDays*24*3600*1000))
        return false;

    if(!BL_safe_mul_int64(&lint.QuadPart, lint.QuadPart , 10000))
        return false;

    pFileTime->dwLowDateTime  = lint.LowPart;
    pFileTime->dwHighDateTime = lint.HighPart;

    return true;
}
static bool BL_IsOnlyWhiteSpace(LPCTSTR psz)
{
    for(;*psz;psz++)
    {
        if(!_istspace(*psz))
            return false;
    }

    return true;
}
