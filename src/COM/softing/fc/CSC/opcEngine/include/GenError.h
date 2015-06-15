//-----------------------------------------------------------------------------
//                OPC ENGINE              OPC SESSION                         |
//                                                                            |
//  Filename    : GENERROR.H                                                  |   
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

#ifndef _GENERROR_H_
#define _GENERROR_H_

#include "GenObj.h"

//-----------------------------------------------------------------------------
// GenericError                                                               |
//-----------------------------------------------------------------------------

class GenericObject;

class OPCENGINE_EXPORT GenericError
{
public:	
	GenericError(IN GenericObject *obj, IN DWORD traceGroup);

	// last error
	HRESULT getLastError(OUT OPTIONAL DWORD *errorcode = NULL, 
			OUT OPTIONAL CString *where = NULL);

protected:
	HRESULT	m_lastError;			// includes the last system error
	CString m_lastErrorDescription; // description of last error
	DWORD m_lastErrorCode;			// toolbox error code of last error
	GenericObject *m_obj;			// generic object pointer of me
	DWORD m_traceGroup;				// my trace group

	// last error
	BOOL setLastError(IN HRESULT res, IN OPTIONAL DWORD errorcode = 0xFFFFFFFF, 
			IN OPTIONAL LPCTSTR where = NULL);
	virtual void onError(IN HRESULT res, IN DWORD errorcode, IN LPCTSTR where);
}; // GenericError

#endif
