//-----------------------------------------------------------------------------
//                OPC ENGINE              OPC SESSION                         |
//                                                                            |
//  Filename    : GENERROR.CPP                                                |   
//  Version     : 2.00.0.00.release       2.00.0.00.release                   |
//  Date        : 29-March-1999           29-March-1999                       |
//                                                                            |
//  Description : Error handling class                                        |
//                - GenericError: generic error methods                       |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//                                                                            |
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GenObj.h"
#include "GenError.h"
#include "OPCTrace.h"


//-----------------------------------------------------------------------------
// GenericError                                                               |
//-----------------------------------------------------------------------------

GenericError::GenericError(
	IN GenericObject *obj,	// generic object
	IN DWORD traceGroup)	// trace group
{
	m_obj = obj;
	m_traceGroup = traceGroup;
}

//-----------------------------------------------------------------------------
// getLastError
//
// - get the last occured error
//
// returns:
//		result code for last error 
//
HRESULT GenericError::getLastError(
	OUT OPTIONAL DWORD *errorcode,		// toolbox error code 
	OUT OPTIONAL CString *description)	// error description
{ 
	if (FAILED(m_lastError))
	{
		if (description)
			*description = m_lastErrorDescription; 

		if (errorcode)
			*errorcode = m_lastErrorCode; 
	}
	else
	{
		if (description)
			*description = _T(""); 

		if (errorcode)
			*errorcode = 0; 
	}

	return m_lastError; 
} // getLastError


//-----------------------------------------------------------------------------
// setLastError
//
// - set the last occured error
//
// returns:
//		TRUE  
//
BOOL GenericError::setLastError(
	IN HRESULT res,						// system error code
	IN OPTIONAL DWORD errorcode,		// toolbox error code 
	IN OPTIONAL LPCTSTR description)	// error description
{
	if (FAILED(res))
	{
		m_lastError = res;
	
		// only if error occured
		WORD traceLevel = TL_ERR;
		if (description)
			m_lastErrorDescription = description;
		else
			m_lastErrorDescription.Empty();
		
		m_lastErrorCode = errorcode;
		onError(res, m_lastErrorCode, (LPCTSTR)m_lastErrorDescription);

		if ((0xE0000000 & errorcode) == 0x2000000)
			traceLevel = TL_INF;

		// error tracing
		_TRACEO(traceLevel, m_traceGroup, (_T("%s (0x%8.8X) res:[0x%8.8X]"), m_lastErrorDescription, m_lastErrorCode, res));
	}

	return TRUE;
} // setLastError


//-----------------------------------------------------------------------------
// onError
//
// - called if error occured
//
void GenericError::onError(
	IN HRESULT res,		// error
	IN DWORD errorcode,
	IN LPCTSTR where)	// description
{ } // onError
