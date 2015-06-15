/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/ValueFormatter.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: ValueFormatter.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/AsyncOPC/ValueFormatter.h $
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

	HRESULT ValueToQVTString(VARIANT Value, WORD Quality, FILETIME Time, CString& strQvtValue);
	HRESULT ValueToQVTStringIEC(VARIANT Value, WORD Quality, FILETIME Time, const CString& strType, CString& strQvtValue);
	HRESULT ValueToQVTStringIECandRaw(VARIANT Value, WORD Quality, FILETIME Time, const CString& strType, CString& strQvtValue);
    void GetBadQVTString(CString& strQvtValue);
    void GetBadQVTStringRaw(CString& strQvtValue);

private:
	HRESULT GetValueString(VARIANT Value, CString& strQvtValue);
	HRESULT GetValueStringIEC(VARIANT Value, const CString& strType, CString& strQvtValue);
	void GetTimestampString(FILETIME Time, CString& strTimestamp);
	void GetTimestampString(CString& strTimestamp);
    ULONG GetTimestamp(FILETIME ftTimeStamp);
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
