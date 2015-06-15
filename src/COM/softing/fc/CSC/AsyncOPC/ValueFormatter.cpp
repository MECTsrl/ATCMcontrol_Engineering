/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/ValueFormatter.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: ValueFormatter.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/ValueFormatter.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           AsyncOPC
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.08.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "SyncAccessGlobals.h"
#include "ValueFormatter.h"
#include "baselib.h"
#include "opcDA.h"

//----  Static Initializations:   ----------------------------------*
/*static*/ COleDateTime     CValueFormatter::sm_timeBase = COleDateTime(1970, 1, 1, 0, 0, 0);
                                // time base of 4CC = midnight, January 1, 1970 UTC

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CValueFormatter::CValueFormatter()
{

	// get instance to baselib COM wrapper:
	IUnknown    *pUnk = NULL;
	HRESULT hrCreate = ::CoCreateInstance(CLSID_FCBaseLib, NULL, CLSCTX_SERVER, IID_IBL_Conversion, (void**) &pUnk);
	if (SUCCEEDED(hrCreate)) {
		m_pBaseLibConversion = (IBL_Conversion*) pUnk;
		pUnk->Release();
	}
}

CValueFormatter::~CValueFormatter()
{
}


/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>CValueFormatter</B>
*
*	Description = convert OPCITEMSTATEs quality, value and timestamp to string
*	<p>
*	Specification = 
*	<p>
*	@param opcItemState
*	<p>
*	@param strQuality
*	<p>
*	@param strValue
*	<p>
*	@param strTimestamp
*	<p>
*
*	@return 
*	<p>
*
*	@exception
*	<p>
*	@see
*	<p>
*/
HRESULT CValueFormatter::ValueToQVTString(VARIANT Value, WORD Quality, FILETIME Time, CString& strQvtValue)
{
	HRESULT hr=S_OK;
	CString strValue, strQuality, strTimestamp;

    hr=GetValueString(Value, strValue);
    strQuality.Format(_T("%d"), Quality);
    GetTimestampString(Time, strTimestamp);

    //
    // replace gstrSeperator in value
    //
    strValue.Replace(gchSeperator, ' ');
    strQvtValue.Format(_T("%s%c%s%c%s"), strQuality, gchSeperator, strValue, gchSeperator, strTimestamp);
	return hr;
}


/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>CValueFormatter</B>
*
*	Description = convert OPCITEMSTATEs quality, value and timestamp to string
*	<p>
*	Specification = 
*	<p>
*	@param opcItemState
*	<p>
*	@param strQuality
*	<p>
*	@param strValue
*	<p>
*	@param strTimestamp
*	<p>
*
*	@return 
*	<p>
*
*	@exception
*	<p>
*	@see
*	<p>
*/
HRESULT CValueFormatter::ValueToQVTStringIEC(VARIANT Value, WORD Quality, FILETIME Time, const CString& strType, CString& strQvtValue)
{
	HRESULT hr=S_OK;
	CString strValue, strQuality, strTimestamp;

    hr=GetValueStringIEC(Value, strType, strValue);
    strQuality.Format(_T("%d"), Quality);
    GetTimestampString(Time, strTimestamp);

    //
    // replace gstrSeperator in value
    //
    strValue.Replace(gchSeperator, ' ');
    strQvtValue.Format(_T("%s%c%s%c%s"), strQuality, gchSeperator, strValue, gchSeperator, strTimestamp);
	return hr;
}



/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>CValueFormatter</B>
*
*	Description = convert OPCITEMSTATEs quality, value and timestamp to string
*	<p>
*	Specification = 
*	<p>
*	@param opcItemState
*	<p>
*	@param strQuality
*	<p>
*	@param strValue
*	<p>
*	@param strTimestamp
*	<p>
*
*	@return 
*	<p>
*
*	@exception
*	<p>
*	@see
*	<p>
*/
HRESULT CValueFormatter::ValueToQVTStringIECandRaw(VARIANT Value, WORD Quality, FILETIME Time, const CString& strType, CString& strQvtValue)
{
	HRESULT hr=S_OK;
	CString strRawValue, strValue, strQuality, strTimestamp;

    GetValueString(Value, strRawValue);
    hr=GetValueStringIEC(Value, strType, strValue);
    strQuality.Format(_T("%d"), Quality);
    GetTimestampString(Time, strTimestamp);

    //
    // replace gstrSeperator in value
    //
    strValue.Replace(gchSeperator, ' ');
    strRawValue.Replace(gchSeperator, ' ');
    strQvtValue.Format(_T("%s%c%s%c%s%c%s"), strQuality, gchSeperator, strValue, gchSeperator, strRawValue, gchSeperator, strTimestamp);
	return hr;
}



/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>CValueFormatter</B>
*
*	Description = convert OPCITEMSTATEs quality, value and timestamp to string
*	<p>
*	Specification = 
*	<p>
*	@param opcItemState
*	<p>
*	@param strQuality
*	<p>
*	@param strValue
*	<p>
*	@param strTimestamp
*	<p>
*
*	@return 
*	<p>
*
*	@exception
*	<p>
*	@see
*	<p>
*/
void CValueFormatter::GetBadQVTString(CString& strQvtValue)
{
	CString strQuality, strValue, strTimestamp;

	strValue="";
	int iQuality = OPC_QUALITY_BAD;
	strQuality.Format(_T("%d"), iQuality);
	GetTimestampString(strTimestamp);
    strQvtValue.Format(_T("%s%c%s%c%s"), strQuality, gchSeperator, strValue, gchSeperator, strTimestamp);
}


/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>CValueFormatter</B>
*
*	Description = convert OPCITEMSTATEs quality, value and timestamp to string
*	<p>
*	Specification = 
*	<p>
*	@param opcItemState
*	<p>
*	@param strQuality
*	<p>
*	@param strValue
*	<p>
*	@param strTimestamp
*	<p>
*
*	@return 
*	<p>
*
*	@exception
*	<p>
*	@see
*	<p>
*/
void CValueFormatter::GetBadQVTStringRaw(CString& strQvtValue)
{
	CString strQuality, strValue, strTimestamp;

	strValue="";
	int iQuality = OPC_QUALITY_BAD;
	strQuality.Format(_T("%d"), iQuality);
	GetTimestampString(strTimestamp);
    strQvtValue.Format(_T("%s%c%s%c%s%c%s"), strQuality, gchSeperator, strValue, gchSeperator, strValue, gchSeperator, strTimestamp);
}


/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>CValueFormatter</B>
*
*	Description = convert OPCITEMSTATE value to string
*	<p>
*	Specification = 
*	<p>
*	@param opcItemState
*	<p>
*	@param strValue
*	<p>
*
*	@return 
*	<p>
*
*	@exception
*	<p>
*	@see
*	<p>
*/
HRESULT CValueFormatter::GetValueString(VARIANT Value, CString& strValue)
{
	HRESULT hr=S_OK;
	if ( Value.vt == VT_R4)
	{
		strValue.Format(_T("%f"),(double)Value.fltVal);
	}
	else if (Value.vt == VT_R8)
	{
		strValue.Format(_T("%f"),(double)Value.dblVal);
	}
	else
	{
		CComVariant vtValue=Value;
		hr=vtValue.ChangeType(VT_BSTR);
		if ( SUCCEEDED(hr) )
		{
			//pszValue=OLE2T(vtValue.bstrVal);
			strValue=vtValue.bstrVal;
		}
		else
		{
			strValue = _T("-- Range Error .--");
		}
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>CValueFormatter</B>
*
*	Description = convert OPCITEMSTATE value to string
*	<p>
*	Specification = 
*	<p>
*	@param opcItemState
*	<p>
*	@param strValue
*	<p>
*
*	@return 
*	<p>
*
*	@exception
*	<p>
*	@see
*	<p>
*/
HRESULT CValueFormatter::GetValueStringIEC(VARIANT Value, const CString& strType, CString& strValue)
{
	if ( !m_pBaseLibConversion)
	{
		return GetValueString(Value, strValue);
	}

	BSTR     sValue;
	CComBSTR sType(strType);
	HRESULT  hr = m_pBaseLibConversion->VariantToString(Value, sType, NULL, BL_USE_IEC_FORMAT, 0, &sValue);
	if (FAILED(hr))
	{
		strValue = _T("-- Conversion Error --");
	}
	else {
		strValue = sValue;
		::SysFreeString(sValue);
	}
	return hr;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>CValueFormatter</B>
*
*	Description = 
*		get timestamp of OPCSTATE in seconds since 1.1.1970 UTC as string
*	<p>
*	Specification = 
*	<p>
*	@param	opcItemState
*	<p>
*	@param	strTimestamp
*	<p>
*
*	@return 
*	<p>
*
*	@exception
*	<p>
*	@see
*	<p>
*/
void CValueFormatter::GetTimestampString(FILETIME Time, CString& strTimestamp)
{
	ULONG ulTimestamp=GetTimestamp(Time);
	strTimestamp.Format(_T("%ld"), ulTimestamp);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>CValueFormatter</B>
*
*	Description = 
*		get timestamp seconds since 1.1.1970 UTC as string
*	<p>
*	Specification = 
*	<p>
*	@param	opcItemState
*	<p>
*	@param	strTimestamp
*	<p>
*
*	@return 
*	<p>
*
*	@exception
*	<p>
*	@see
*	<p>
*/
void CValueFormatter::GetTimestampString(CString& strTimestamp)
{
	ULONG ulTimestamp=GetTimestamp();
	strTimestamp.Format(_T("%d"), ulTimestamp);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>CValueFormatter</B>
*
*	Description = 
*		get timestamp of OPCSTATE in seconds since 1.1.1970 UTC
*	<p>
*	Specification = 
*	<p>
*	@param opcItemState
*	<p>
*
*	@return seconds since 1.1.1970 UTC
*	<p>
*
*	@exception
*	<p>
*	@see
*	<p>
*/
ULONG CValueFormatter::GetTimestamp(FILETIME ftTimeStamp)
{
	//
	// FILETIME to SYSTEMTIME
	//
	SYSTEMTIME systemTimeTimestamp;
	::FileTimeToSystemTime(&ftTimeStamp, &systemTimeTimestamp);

	//
	// SYSTEMTIME to DATE
	//
	DATE dateTimestamp;
	SystemTimeToVariantTime(&systemTimeTimestamp, &dateTimestamp);
    
	//
	// date to ULONG
	//
	ULONG ulTimestamp = VariantTimeToSeconds(dateTimestamp);

	return ulTimestamp;
}


/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>CValueFormatter</B>
*
*	Description = 
*		get seconds since 1.1.1970 UTC
*	<p>
*	Specification = 
*	<p>
*	@param
*	<p>
*
*	@return seconds since 1.1.1970 UTC
*	<p>
*
*	@exception
*	<p>
*	@see
*	<p>
*/
ULONG CValueFormatter::GetTimestamp()
{
	SYSTEMTIME  now;
	DATE    dtNow;
	::GetSystemTime(&now);
	SystemTimeToVariantTime(&now, &dtNow);
	ULONG ulTimeNow = VariantTimeToSeconds(dtNow);
	return ulTimeNow;
}


/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>CValueFormatter</B>
*
*	Description = 
*		convert variant time to seconds since 1.1.1970 UTC
*	<p>
*	Specification = 
*	<p>
*	@param	dt	date
*	<p>
*
*	@return seconds since 1.1.1970 UTC
*	<p>
*
*	@exception
*	<p>
*	@see
*	<p>
*/
ULONG CValueFormatter::VariantTimeToSeconds(DATE dt)
{
	COleDateTimeSpan timestamp = dt - sm_timeBase;
	double dblSpan = timestamp.GetTotalSeconds();
	_ASSERTE(dblSpan < ULONG_MAX);
	ULONG   ulTime;
	if (dblSpan > ULONG_MAX) 
	{
		ulTime = 0;     // error, cannot represent time
	}
	else 
	{
		ulTime = (ULONG) dblSpan;
	}
	return ulTime;
}


/*
 *----------------------------------------------------------------------------*
 *  $History: ValueFormatter.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 8.08.01    Time: 9:41
 * Created in $/4Control/COM/softing/fc/CSC/AsyncOPC
 * copied from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
