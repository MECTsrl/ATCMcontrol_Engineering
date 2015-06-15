/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_baselib/BaseLibWrap.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BaseLibWrap.cpp $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_baselib/BaseLibWrap.cpp $
 *
 * =PROJECT             7VH102  ATCMControl
 *
 * =SWKE                4CMN
 *
 * =COMPONENT           BaseLib
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *           $Revision: 1 $
 *
 * =ENVIRONMENT             Win32:
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
 *    ATCM BaseLib COM wrapper
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  1998-02-11  Je      File created
 *  see history at end of file !
 *==
*******************************************************************************
H<<*/

#include "stdafx.h"
#include "BaseLibWrap.h"


//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*



/////////////////////////////////////////////////////////////////////////////
// CFC_BaseLib

//------------------------------------------------------------------*
CFC_BaseLib::CFC_BaseLib()
{
}

//------------------------------------------------------------------*
CFC_BaseLib::~CFC_BaseLib()
{
}

//------------------------------------------------------------------*
HRESULT
    CFC_BaseLib::FinalConstruct()
{
    return S_OK;
}

//------------------------------------------------------------------*
void
    CFC_BaseLib::FinalRelease()
{
}


//=======================================================================
//      interface   IBL_Conversion
//=======================================================================

//------------------------------------------------------------------*
STDMETHODIMP
    CFC_BaseLib::StringToVariant(BSTR sValue, BSTR sDatatype, BSTR sFormat, long options, VARIANT *pvtOut)
{
    if (sValue == NULL)
        sValue = L"";
    return ::BL_StringToVariantNamed(sValue, sDatatype, sFormat, options, pvtOut);
}

//------------------------------------------------------------------*
STDMETHODIMP
    CFC_BaseLib::VariantToString(VARIANT vtIn, BSTR sDatatype, BSTR sFormat, long options, long lMaxCharsWanted, BSTR *psOut)
{
    return ::BL_VariantToStringNamed(&vtIn, sDatatype, sFormat, options, lMaxCharsWanted, psOut);
}

//------------------------------------------------------------------*
STDMETHODIMP CFC_BaseLib::CheckString(BSTR sValue, BSTR sDatatype, BSTR sFormat, long options, long *phResult)
{
    if (phResult == NULL) {
        return E_POINTER;
    }
    *phResult = S_OK;

    VARIANT vtValue;
    HRESULT hr = StringToVariant(sValue, sDatatype, sFormat, options, &vtValue);
    VariantClear(&vtValue);
    if (HRESULT_FACILITY(hr) == E_FACILITY_BASELIB) {
        *phResult = hr;
        return S_FALSE;
    }

    return hr;
}

//------------------------------------------------------------------*
STDMETHODIMP CFC_BaseLib::ConvertSecondsToDate(long seconds, DATE *result)
{
    SYSTEMTIME sys1970 = {1970, 1, 0, 1, 0, 0, 0, 0};
    FILETIME ftUTC, ftLOCAL;
    SYSTEMTIME sysLOCAL;

    if (result == NULL) {
        return E_POINTER;
    }
    *result = 0;

    if (!SystemTimeToFileTime(&sys1970, &ftUTC))
        return E_FAIL;
    (*(__int64 *) &ftUTC) += seconds * 10000000i64;
    if (!FileTimeToLocalFileTime(&ftUTC, &ftLOCAL))
        return E_FAIL;
    if (!FileTimeToSystemTime(&ftLOCAL, &sysLOCAL))
        return E_FAIL;
    if (!SystemTimeToVariantTime(&sysLOCAL, result))
        return E_FAIL;

    return S_OK;
}


/*
 *----------------------------------------------------------------------------*
 *  $History: BaseLibWrap.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_baselib
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_baselib
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 23.01.01   Time: 17:12
 * Created in $/4Control/COM/softing/fc/CMN/fc_baselib
 * utils and baselib from V1.3
 * 
 * *****************  Version 6  *****************
 * User: Ed           Date: 5.04.00    Time: 13:16
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * add a method to convert the timestamps from CSC_simpleOPC to OLE DATEs
 * 
 * *****************  Version 5  *****************
 * User: Ed           Date: 31.03.00   Time: 14:51
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * dont strip leading whitespace on strings
 * 
 * *****************  Version 4  *****************
 * User: Ed           Date: 31.03.00   Time: 13:33
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * fix a memory leak
 * 
 * *****************  Version 3  *****************
 * User: Je           Date: 9.07.98    Time: 9:18
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * * removed DEBUG_NEW
 * 
 * *****************  Version 2  *****************
 * User: Je           Date: 8.07.98    Time: 18:47
 * Updated in $/4Control/COM/softing/fc/CMN/baselib
 * * moved BaseLib COM wrapper into BaseLib itself
 * * CheckString w/o HRESULT error if baselib syntax error
 * 
 * *****************  Version 1  *****************
 * User: Je           Date: 8.07.98    Time: 11:21
 * Created in $/4Control/COM/softing/fc/CMN/baselib
 *==
 *----------------------------------------------------------------------------*
*/

// 1998-02-11/Je  Generated by PERL5 script d:\bin\perl5/mkproto.bat Version 1.2
// ***************************  E O F  ******************************
// $Workfile: BaseLibWrap.cpp $



