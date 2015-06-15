/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/ValueFormatter.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: ValueFormatter.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/ValueFormatter.h $
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
 *==
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

#ifndef __VALUEFORMATTER_H_
#define __VALUEFORMATTER_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "opcDA.h"
#include "BaseLibCOM.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
class CValueFormatter  
{
public:
	CValueFormatter();
	virtual ~CValueFormatter();

	HRESULT ValueToQVTString(const OPCITEMSTATE& opcItemState, CString& strQuality, CString& strValue, CString& strTimestamp);
	HRESULT ValueToQVTStringIEC(const OPCITEMSTATE& opcItemState, const CString& strType, CString& strQuality, CString& strValue, CString& strTimestamp);
	HRESULT ValuesToQVTString(int iCount, const OPCITEMSTATE* opcItemStates, HRESULT* errors, CString& strValues);
	HRESULT ValuesToQVTStringIEC(int iCount, const OPCITEMSTATE* opcItemStates, HRESULT* errors,const CStringArray& strTypes, CString& strValues);
	void GetQVTBad(CString& strQuality, CString& strValue, CString& strTimestamp);
	void GetBadQVTs(int iCount, CString& strValues);
	HRESULT GetValueString(const OPCITEMSTATE& opcItemState, CString& strValue);
	HRESULT GetValueStringIEC(const OPCITEMSTATE& opcItemState, const CString& strType, CString& strValue);
	void GetTimestampString(const OPCITEMSTATE& opcItemState, CString& strTimestamp);
	void GetTimestampString(CString& strTimestamp);
	ULONG GetTimestamp(const OPCITEMSTATE& opcItemState);
	ULONG GetTimestamp();
	ULONG VariantTimeToSeconds(DATE dt);

private:
	static COleDateTime     sm_timeBase;
	CComPtr<IBL_Conversion> m_pBaseLibConversion;

};

#endif // __VALUEFORMATTER_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: ValueFormatter.h $
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
