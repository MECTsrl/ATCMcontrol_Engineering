/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/javautil.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: javautil.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/javautil.h $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CC
 *
 * =COMPONENT           javautil
 *
 * =CURRENT             $Date: 28.02.07 19:01 $
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
 * $History: javautil.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ed           Date: 3/26/01    Time: 5:00p
 * Created in $/4Control/COM/softing/fc/inc
 *==
 ******************************************************************************
H<<*/

#ifndef JAVAUTIL_H
#define JAVAUTIL_H


#ifndef JAVAUTIL_API
#define JAVAUTIL_API __declspec(dllimport)
#endif

JAVAUTIL_API HRESULT javautilInitDll();
JAVAUTIL_API HRESULT javautilExitDll();


JAVAUTIL_API HRESULT connectBreakpointNotify(IBreakpointNotify *pNotify, BOOL bConnect);
JAVAUTIL_API HRESULT connectDebugOutput(IDebugOutput *pOutput, BOOL bConnect);

JAVAUTIL_API HRESULT getLogfileMaxLength(long *len);
JAVAUTIL_API HRESULT setLogfileMaxLength(long len);
JAVAUTIL_API HRESULT getLogfileHistory(long *first, long *last);
JAVAUTIL_API HRESULT getLogfileMessage(long id, BSTR *pMessage, DATE *pDate);
JAVAUTIL_API HRESULT addLogfileMessage(BSTR pMessage);


#endif /* JAVAUTIL_H */