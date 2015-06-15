/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/baselib.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: baselib.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/baselib.h $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           BASELIB
 *
 * =CURRENT             $Date: 28.02.07 19:01 $
 *                      $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==Baselib DLL interface, functions to convert IEC string literals and 
 *  formatted output of binary values
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  23.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#ifndef __BASELIB_H_
#define __BASELIB_H_


#ifndef BASELIB_API
#define BASELIB_API __declspec(dllexport)
#endif

#include "cFacility.h"      // 4C global facility codes


// HRESULTS of BL functions:
#define E_BL_ERR_SYNTAX          MAKE_HRESULT(E_SEVERITY_ERROR, E_FACILITY_BASELIB, 0x0001)
#define E_BL_ERR_RANGE           MAKE_HRESULT(E_SEVERITY_ERROR, E_FACILITY_BASELIB, 0x0002)
#define E_BL_ERR_FORMATERROR     MAKE_HRESULT(E_SEVERITY_ERROR, E_FACILITY_BASELIB, 0x0003)
#define E_BL_ERR_INVALIDARG      MAKE_HRESULT(E_SEVERITY_ERROR, E_FACILITY_BASELIB, 0x0004)
#define E_BL_ERR_FUN_NOT_IMPL    MAKE_HRESULT(E_SEVERITY_ERROR, E_FACILITY_BASELIB, 0x0005)
#define E_BL_ERR_BUFFER_TO_SMALL MAKE_HRESULT(E_SEVERITY_ERROR, E_FACILITY_BASELIB, 0x0006)
#define E_BL_ERR_INTERNAL        MAKE_HRESULT(E_SEVERITY_ERROR, E_FACILITY_BASELIB, 0x0007)
#define E_BL_ERR_LOSS_OF_PRECIS  MAKE_HRESULT(E_SEVERITY_ERROR, E_FACILITY_BASELIB, 0x0008)
#define E_BL_ERR_RADIX           MAKE_HRESULT(E_SEVERITY_ERROR, E_FACILITY_BASELIB, 0x0009)
#define E_BL_ERR_INVALID_IEC_STR MAKE_HRESULT(E_SEVERITY_ERROR, E_FACILITY_BASELIB, 0x000A)


enum BL_IEC_TYP{
    BL_ANY = 0,
    BL_BOOL,
    BL_BYTE,
    BL_WORD,
    BL_DWORD,
    BL_LWORD,
    BL_INT,
    BL_DINT,
    BL_LINT,
    BL_REAL,
    BL_LREAL,
    BL_TIME,
    BL_DT,
    BL_DATE,
    BL_TOD,
    BL_STRING,
    BL_WSTRING,
    BL_UINT,
    BL_UDINT,
    BL_ULINT,
    BL_LASTTYPE = BL_ULINT
  };


// datatype additional info:
#define BL_DTYPE_IS_QVT         0x00001L

//limits:
#define BL_MIN_DATE_YEAR 1971
#define BL_MAX_DATE_YEAR 9999


#define BL_USE_WINDOWS_FORMAT     0x0001UL
#define BL_USE_IEC_FORMAT         0x0002UL
#define BL_USE_TYPED_LITERALS     0x0004UL

/*
Date/Time:
YY              -> 2-Stelliges Jahr
YYYY            -> 4-Stelliges Jahr
M               -> 1-Stelliger Monat (numerisch)
MM              -> 2-Stelliger Monat (numerisch)
MMM             -> 3-Stellig abgek. Monat
MMMM            -> Ausgeschrieber Monat
D               -> 1-Stelliger Tag (numerisch)
DD              -> 2-Stelliger Tag (numerisch)
DDD             -> 3-Stellig abgek. Wochentag
DDDD            -> Ausgeschriebener Wochentag
h               -> 1-Stellige Stunde (1-12)
hh              -> 2-Stellige Stunde (1-12)
H               -> 1-Stellige Stunde (1-24)
HH              -> 2-Stellige Stunde (1-24)
m               -> 1-Stellige Minuten
mm              -> 2-Stellige Minuten
s               -> 1-Stellige Sekunden
ss              -> 2-Stellige Sekunden
t               -> A oder P
tt              -> AM oder PM
'text'          -> text
.,;:/' '        -> Trennzeichen

Integer-Formate:
d               -> 7, -7
-dd             -> 007, -07
+dddd           -> +0007, -0007
'2#'bbbb        -> 2#0111, ...
'8#'oooo        -> 8#0007, ...
'16#'xxxx       -> 16#0007, 16#fff9
'16#'XXXX       -> 16#0007, 16#FFF9
+ddd,ddd.ddd    -> +000,000.007, -000,000.007
+d.ddddddd+ee   -> "+1.2345678+99"
-d.ddddEee      -> " 1.2345E12"
d.ddddE+ee      -> "1.2345E+12"

Boolean:
[true|false]    -> "true", "false"
*/

enum BL_FormatKind
{
    BL_UnspecifiedFormat,
    BL_IntegerFormat,
    BL_FloatingRealFormat,
    BL_FixedRealFormat,
    BL_StringFormat
};

enum BL_Alignment
{
    BL_UnspecifiedAlignment,
    BL_LeftAlign,
    BL_RightAlign
};

struct BL_FormatInfo
{
    BL_FormatKind m_Kind;
    int           m_iBytesPerItem;
    BL_Alignment  m_Alignment;
    int           m_iWidth;
    TCHAR         m_chPadding;

    int m_iSignedness;
    int m_iRadix;

    int m_iPrecision;
};


#ifdef __cplusplus
extern "C" 
{
#endif // __cplusplus

//high level conversion routines: VARIANT to STRING and vice versa
HRESULT BASELIB_API __stdcall BL_StringToVariant(
                                 LPCOLESTR value,   // in parameter, zero-terminated
                                 BL_IEC_TYP typ,    // may be BL_ANY if not known
                                 LPCOLESTR format,  // may be NULL or "" if not known
                                 DWORD options,     // BL_USE_WINDOWS_FORMAT||BL_USE_IEC_FORMAT
                                 VARIANT *out);     // out parameter

HRESULT BASELIB_API __stdcall BL_VariantToString(
                                 VARIANT *value,    // in parameter
                                 BL_IEC_TYP typ,    // may be BL_ANY if not known
                                 LPCOLESTR format,  // may be NULL or "" if not known
                                 DWORD options,     // BL_USE_WINDOWS_FORMAT||BL_USE_IEC_FORMAT||
                                                    // BL_USE_TYPED_LITERALS
                                 LPOLESTR out,      // formatted string
                                 UINT nChars);      // sizeof(out)/sizeof(OLECHAR)

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
                                 VARIANT *pvtOut);      // out parameter

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
HRESULT BASELIB_API __stdcall BL_VariantToStringNamed(
                                 VARIANT *pvtIn,        // in parameter
                                 LPCOLESTR sType,       // namestring of datatype
                                 LPCOLESTR sFormat,     // may be NULL or "" if not known
                                 DWORD options,         // BL_USE_WINDOWS_FORMAT||BL_USE_IEC_FORMAT
                                 long lMaxCharsWanted,  // number of characters wanted in output string
                                 BSTR *psOut);          // formatted string

//------------------------------------------------------------------*
/**
 * BL_TypenameToDatatype
 *      convert readable string of datatype name to correct dataype enum BL_...
 *
 * @param           pszTypeName         name string of datatype as used in ST
 * @param           pDataType           [out] datatype
 * @param           pModifiers          [out] additional info to datatype;
 *                                      currently: QVT or non-QVT datatype
 * @return          S_OK
 *                  E_BL_ERR_SYNTAX     datatype name is unknown
 *                  E_BL_ERR_INVALIDARG bad pointer or bad string
 * @exception       -
 * @see             
*/
HRESULT BASELIB_API __stdcall BL_TypenameToDatatype(
                                 LPCOLESTR   pszTypeName,   // in parameter, 
                                 BL_IEC_TYP *pDataType,     // [out]
                                 DWORD      *pModifiers);   // BL_DTYPE_IS_QVT


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
LPCWSTR BASELIB_API __stdcall BL_DatatypeToTypename(BL_IEC_TYP wDataType, DWORD dwModifiers);


//low level conversion routines 
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
                                            const wchar_t** pszEnd);

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
                                                    const wchar_t** pszEnd);


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
                                                            const wchar_t** pszEnd);



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
                                                           const wchar_t** pszEnd);




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
                                                         const wchar_t** pszEnd);


//------------------------------------------------------------------*
/**
 * BL_SystemTimeToVariantTime
 *  corrects the Win32-supplied SystemTimeToVariantTime: no truncation of
 *  fractions below seconds
 *
 * @param           pSystemTime     contains system time to convert to DATE
 * @param           pvTime          converted time *WITH* milliseconds in fraction!
 * @return          S_OK if function succeeds
 * @exception       -
 * @see             
*/
HRESULT BASELIB_API __stdcall
    BL_SystemTimeToVariantTime(SYSTEMTIME const * pSystemTime, OUT DOUBLE *pvTime);



/**
 * BL_IECStringCB callback function
 *  for displaing error/warrnig messages when creating binary represention of a file 
 *
 * @param           pCtx        pointer to context information (ex. CG_CDiagMsg*, CG_EdiPos)
 *                              provided by the caller of BL_IECStringToBinary or BL_IECWStringToBinary
 *                              the rest of parameters are set by the BL_IECStringToBinary or BL_IECWStringToBinary
 * @param           offset      offset in (TCHARs) in string where error/warning occured
 * @param           bBadEscape  true if a non standard escape sequence was provided
 * @param           nBadUnicode int value of caracter that was unable to convert to user code page representation
 * @param           bBadNewLine true if newline caracter in string (not newline escaped sequence)
 * @return          void
 * @exception       -
 * @see             BL_IECStringToBinary and BL_IECWStringToBinary
*/
typedef void (*BL_IECStringCB)(void* pCtx, int offset, int bBadEscape, int nBadUnicode, int bBadNewLine);

//------------------------------------------------------------------*
/**
 * BL_IECWStringToBinary
 *  converts a UNICODE string to binary representation 
 *
 * @param           pszIECStr       contains string to convert in folowing format
 *                                  wcharacter_string         ["]{wcharacter_representation}*["]
 *                                  wcharacter_representation ([^$"\n]|"$"({hex_digit}{hex_digit}{hex_digit}{hex_digit}|[$"LlNnPpRrTt]))
 *                                  hex_digit                 [0-9A-Fa-f]
 * @param           pBinBuffer      output buffer containing binary representation if no error ocurred
 * @param           pBufferLen      in/out parameter buffer length in TCHARs (without final '\0')
 *                                  if buffer is too small E_BL_ERR_BUFFER_TO_SMALL is returned
 *                                  and pBufferLen contains the necesary buffer length
 * @param           pCB             callback function called when errors, NULL if no error/warning should be produced
 * @param           pCtx            parameter send to callback function, NULL if no error/warning should be produced
 * @return          S_OK or E_BL_ERR_INVALID_IEC_STR, E_BL_ERR_BUFFER_TO_SMALL 
 * @exception       -
 * @see             BL_IECStringCB
*/
HRESULT BASELIB_API BL_IECWStringToBinary(
    const TCHAR*    pszIECStr, 
    TCHAR*          pBinBuffer,
    long*           pBufferLen,
    BL_IECStringCB  pCB,
    void*           pCtx
);


//------------------------------------------------------------------*
/**
 * BL_IECStringToBinary
 *  converts a ASCII string to binary representation 
 *
 * @param           pszIECStr       contains string to convert in folowing format
 *                                  character_string         [']{character_representation}*[']
 *                                  character_representation ([^$'\n]|"$"({hex_digit}{hex_digit}|[$'LlNnPpRrTt]))
 *                                  hex_digit                [0-9A-Fa-f]
 * @param           pBinBuffer      output buffer containing binary representation if no error ocurred
 * @param           pBufferLen      in/out parameter buffer length in chars (without final '\0')
 *                                  if buffer is too small E_BL_ERR_BUFFER_TO_SMALL is returned
 *                                  and pBufferLen contains the necesary buffer length
 * @param           pCB             callback function called when errors, NULL if no error/warning should be produced
 * @param           pCtx            parameter send to callback function, NULL if no error/warning should be produced
 * @return          S_OK or E_BL_ERR_INVALID_IEC_STR, E_BL_ERR_BUFFER_TO_SMALL
 * @exception       -
 * @see             BL_IECStringCB
*/
HRESULT BASELIB_API BL_IECStringToBinary(
    const TCHAR*    pszIECStr, 
    char*           pBinBuffer,
    long*           pBufferLen,
    BL_IECStringCB  pCB,
    void*           pCtx
);









HRESULT BASELIB_API BL_ParseFormat (LPCOLESTR pszFormat, struct BL_FormatInfo *pFormatInfo);
HRESULT BASELIB_API BL_ParseFormatForType (LPCOLESTR pszFormat, enum BL_IEC_TYP type, struct BL_FormatInfo *pFormatInfo);
HRESULT BASELIB_API BL_CheckFormatForType (enum BL_IEC_TYP type, struct BL_FormatInfo *pFormatInfo);


//overflow checking __int64 arithmetic routines:
inline bool BL_safe_add_int64(__int64* pRes, __int64 op1, __int64 op2)
{
    *pRes = op1 + op2;

    bool sign1 = op1 < 0;
    bool sign2 = op2 < 0;
    bool signR = *pRes < 0;

    return ! ((sign1==sign2) && (signR!=sign1));
}

inline bool BL_safe_sub_int64(__int64* pRes, __int64 op1, __int64 op2)
{
    
    *pRes = op1 - op2;
    bool sign1 = op1 < 0;
    bool sign2 = op2 > 0;
    bool signR = *pRes < 0;

    return ! ((sign1==sign2) && (signR!=sign1));
}

inline bool BL_safe_mul_int64(__int64* pRes, __int64 op1, __int64 op2)
{
    *pRes = op1 * op2;
    return op1==0 || op2==0 || ((*pRes/op1==op2) && (*pRes/op2==op1));
}




#ifdef __cplusplus
}//extern "C" 
#endif // __cplusplus



#endif // __BASELIB_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: baselib.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 13.06.06   Time: 15:25
 * Updated in $/4Control/COM/softing/fc/inc
 * implemented UINT, UDINT
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 7  *****************
 * User: Ca           Date: 10.10.01   Time: 15:56
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 6  *****************
 * User: Ca           Date: 12.09.01   Time: 14:23
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 5  *****************
 * User: Ca           Date: 10.09.01   Time: 16:28
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 10.09.01   Time: 10:52
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 3  *****************
 * User: Ca           Date: 7.09.01    Time: 16:50
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 26.04.01   Time: 18:18
 * Updated in $/4Control/COM/softing/fc/inc
 * new types: INT,REAL,STRING
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 23.01.01   Time: 17:09
 * Created in $/4Control/COM/softing/fc/inc
 * util and baselib from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
