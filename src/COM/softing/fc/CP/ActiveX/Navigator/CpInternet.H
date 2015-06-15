/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/ActiveX/Navigator/CpInternet.H 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CpInternet.H $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/ActiveX/Navigator/CpInternet.H $
 *
 * =PROJECT             7VH102  ATCMControl
 *
 * =SWKE                PAN
 *
 * =COMPONENT           Navigator
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

 

 

 *==
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATIONS
 * $History: CpInternet.H $
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
 * User: Scd          Date: 19.03.01   Time: 10:19
 * Created in $/4Control/COM/softing/fc/CP/ActiveX/Navigator
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:09
 * Created in $/fc/CP/ActiveX/Navigator
 * 
 * *****************  Version 6  *****************
 * User: Mkr          Date: 14.07.98   Time: 17:48
 * Updated in $/fc/CP/Main/Navigator
 * 
 * *****************  Version 5  *****************
 * User: Mkr          Date: 4.06.98    Time: 14:33
 * Updated in $/fc/CP/Main/Navigator
 * 
 * *****************  Version 4  *****************
 * User: Mkr          Date: 26.05.98   Time: 12:00
 * Updated in $/fc/CP/Main/Navigator
 * 
 * *****************  Version 3  *****************
 * User: Mkr          Date: 19.05.98   Time: 11:08
 * Updated in $/fc/CP/Main/Navigator
 * 
 * *****************  Version 2  *****************
 * User: Mkr          Date: 14.05.98   Time: 13:11
 * Updated in $/fc/CP/Main/Navigator
 *==
 ******************************************************************************
H<<*/

#ifndef _CPINTERNET_H_
#define _CPINTERNET_H_
#include <afxinet.h>

class CCpInternet
{
public:
	CCpInternet();
	virtual ~CCpInternet();
	BOOL OpenFile(const LPCTSTR pstrUrl);
	void CloseFile();
	BOOL ReadLine(CString& TextLine);
	inline BOOL IsFileOpen() const { return m_pINetFile != NULL; }

	BOOL WriteFile(const LPCTSTR pstrSrcFileName, const LPCTSTR pstrUrl) const;

private :
	CInternetSession*	m_pINetSession;
	CStdioFile*			m_pINetFile;
};
#endif
