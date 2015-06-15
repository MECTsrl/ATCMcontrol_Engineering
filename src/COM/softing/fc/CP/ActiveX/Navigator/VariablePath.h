/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/ActiveX/Navigator/VariablePath.h 1     28.02.07 19:00 Ln $
*----------------------------------------------------------------------------*
*
* =FILENAME            $Workfile: VariablePath.h $
*                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/ActiveX/Navigator/VariablePath.h $
*
* =PROJECT             7VH102  ATCMControl
*
* =SWKE                4CSC
*
* =COMPONENT           simple sync OPC client
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
*----------------------------------------------------------------------------*
* =DESCRIPTION
*==
*----------------------------------------------------------------------------*
* =MODIFICATIONS
* $History: VariablePath.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP/ActiveX/Navigator
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CP/ActiveX/Navigator
 * 
 * *****************  Version 1  *****************
 * User: Scd          Date: 19.03.01   Time: 11:52
 * Created in $/4Control/COM/softing/fc/CP/ActiveX/Navigator
 * 
 * *****************  Version 1  *****************
 * User: So           Date: 28.09.99   Time: 9:41
 * Created in $/4Control/COM/softing/fc/CP/ActiveX/Navigator
******************************************************************************
H<<*/

#if !defined(_VARIABLEPATH_H)
#define _VARIABLEPATH_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const CString gstrSeperator = _T(".");
const TCHAR gchSeperator = _T('.');

class CVariablePath  
{
public:
	CVariablePath(CString strPath);
	virtual ~CVariablePath();
	int GetSize() const;
	CString GetAt( int nIndex ) const;
	CString Pop();

private:
	void Initialize();

private:
	CString m_strPath;
	CStringArray m_astrList;
};

#endif // !defined(_VARIABLEPATH_H)
