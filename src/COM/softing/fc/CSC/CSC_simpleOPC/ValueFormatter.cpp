/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/ValueFormatter.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: ValueFormatter.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/ValueFormatter.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           simple sync OPC client
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
*	Description = convert OPCITEMSTATE quality, value and timestamp to string
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
HRESULT CValueFormatter::ValueToQVTString(const OPCITEMSTATE& opcItemState, CString& strQuality, CString& strValue, CString& strTimestamp)
{
	HRESULT hr;
	hr=GetValueString(opcItemState, strValue);
	int iQuality = opcItemState.wQuality;
	strQuality.Format(_T("%d"), iQuality);
	GetTimestampString(opcItemState, strTimestamp);
	return hr;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>CValueFormatter</B>
*
*	Description = convert OPCITEMSTATE quality, value and timestamp to string
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
HRESULT CValueFormatter::ValueToQVTStringIEC(const OPCITEMSTATE& opcItemState, const CString& strType, CString& strQuality, CString& strValue, CString& strTimestamp)
{
	HRESULT hr;
	hr=GetValueStringIEC(opcItemState, strType, strValue);
	int iQuality = opcItemState.wQuality;
	strQuality.Format(_T("%d"), iQuality);
	GetTimestampString(opcItemState, strTimestamp);
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
HRESULT CValueFormatter::ValuesToQVTString(int iCount, const OPCITEMSTATE* opcItemStates, HRESULT* errors, CString& strValues)
{
	HRESULT hr=S_OK;
	CString strValue, strQuality, strTimestamp, strEntry;

	strValues="";
	
	for (int ii=0; ii< iCount; ii++)
	{
		if ( errors!=0 && opcItemStates!=0 && SUCCEEDED(errors[ii]) )
		{
			ValueToQVTString(opcItemStates[ii], strQuality, strValue, strTimestamp);
		}
		else
		{
			GetQVTBad(strQuality, strValue, strTimestamp);			
		}
		//
		// replace gstrSeperator in value
		//
		strValue.Replace(gchSeperator, ' ');
        strEntry.Format(_T("%s%s%s%s%s"), strQuality, gstrSeperator, strValue, gstrSeperator, strTimestamp);
		strValues+=strEntry;
		if (ii<iCount-1)
			strValues+=gstrSeperator;
	}
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
HRESULT CValueFormatter::ValuesToQVTStringIEC(int iCount, const OPCITEMSTATE* opcItemStates, HRESULT* errors,const CStringArray& strTypes, CString& strValues)
{
	HRESULT hr=S_OK;
	CString strValue, strQuality, strTimestamp, strEntry;

	strValues="";
	
	for (int ii=0; ii< iCount; ii++)
	{
		if ( errors!=0 && opcItemStates!=0 && SUCCEEDED(errors[ii]) )
		{
			ValueToQVTStringIEC(opcItemStates[ii], strTypes.GetAt(ii), strQuality, strValue, strTimestamp);
		}
		else
		{
			GetQVTBad(strQuality, strValue, strTimestamp);			
		}
		strValue.Replace(gchSeperator, ' ');
        strEntry.Format(_T("%s%s%s%s%s"), strQuality, gstrSeperator, strValue, gstrSeperator, strTimestamp);
		strValues+=strEntry;
		if (ii<iCount-1)
			strValues+=gstrSeperator;
	}
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
void CValueFormatter::GetBadQVTs(int iCount, CString& strValues)
{
	CString strQuality, strValue, strTimestamp, strEntry ;

	strValues="";

	GetQVTBad(strQuality, strValue, strTimestamp);			
	for (int ii=0; ii< iCount; ii++)
	{
        strEntry.Format(_T("%s%s%s%s%s"), strQuality, gstrSeperator, strValue, gstrSeperator, strTimestamp);
		strValues+=strEntry;
		if (ii<iCount-1)
			strValues+=gstrSeperator;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	<B>CValueFormatter</B>
*
*	Description = convert bad OPCITEMSTATE quality, value and timestamp to string
*	<p>
*	Specification = 
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
void CValueFormatter::GetQVTBad(CString& strQuality, CString& strValue, CString& strTimestamp)
{
	strValue="0";
	int iQuality = OPC_QUALITY_BAD | OPC_QUALITY_OUT_OF_SERVICE;
	strQuality.Format(_T("%d"), iQuality);
	GetTimestampString(strTimestamp);
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
HRESULT CValueFormatter::GetValueString(const OPCITEMSTATE& opcItemState, CString& strValue)
{
	HRESULT hr=S_OK;
	if ( opcItemState.vDataValue.vt == VT_R4)
	{
		strValue.Format(_T("%f"),(double)opcItemState.vDataValue.fltVal);
	}
	else if (opcItemState.vDataValue.vt == VT_R8)
	{
		strValue.Format(_T("%f"),(double)opcItemState.vDataValue.dblVal);
	}
	else
	{
		CComVariant vtValue=opcItemState.vDataValue;
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
HRESULT CValueFormatter::GetValueStringIEC(const OPCITEMSTATE& opcItemState, const CString& strType, CString& strValue)
{
	HRESULT hr;

    _ASSERTE( m_pBaseLibConversion );
	if ( !m_pBaseLibConversion)
	{
		CComVariant v;
        v = opcItemState.vDataValue;
        hr = v.ChangeType(VT_BSTR);
        if (FAILED(hr))
          {
             strValue = _T("-- Conversion Error --");
          }
        else
          {
            strValue  =  v.bstrVal;
          }
        return hr;
	}

	BSTR    sValue;
	CComBSTR sType(strType);
	hr = m_pBaseLibConversion->VariantToString(opcItemState.vDataValue, sType, NULL, BL_USE_IEC_FORMAT, 0, &sValue);
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
void CValueFormatter::GetTimestampString(const OPCITEMSTATE& opcItemState, CString& strTimestamp)
{
	ULONG ulTimestamp=GetTimestamp(opcItemState);
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
ULONG CValueFormatter::GetTimestamp(const OPCITEMSTATE& opcItemState)
{
	//
	// FILETIME to SYSTEMTIME
	//
	SYSTEMTIME systemTimeTimestamp;
	::FileTimeToSystemTime(&opcItemState.ftTimeStamp, &systemTimeTimestamp);

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
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 8.08.01    Time: 9:59
 * Created in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * copied from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
