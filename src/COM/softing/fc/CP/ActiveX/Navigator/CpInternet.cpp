/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/ActiveX/Navigator/CpInternet.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CpInternet.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/ActiveX/Navigator/CpInternet.cpp $
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
 * $History: CpInternet.cpp $
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
 * User: Scd          Date: 19.03.01   Time: 10:17
 * Created in $/4Control/COM/softing/fc/CP/ActiveX/Navigator
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:09
 * Created in $/fc/CP/ActiveX/Navigator
 * 
 * *****************  Version 7  *****************
 * User: Mkr          Date: 14.07.98   Time: 17:48
 * Updated in $/fc/CP/Main/Navigator
 * 
 * *****************  Version 6  *****************
 * User: Mkr          Date: 4.06.98    Time: 14:33
 * Updated in $/fc/CP/Main/Navigator
 * 
 * *****************  Version 5  *****************
 * User: Mkr          Date: 26.05.98   Time: 12:00
 * Updated in $/fc/CP/Main/Navigator
 * 
 * *****************  Version 4  *****************
 * User: Mkr          Date: 19.05.98   Time: 11:08
 * Updated in $/fc/CP/Main/Navigator
 * 
 * *****************  Version 3  *****************
 * User: Mkr          Date: 14.05.98   Time: 13:11
 * Updated in $/fc/CP/Main/Navigator
 *==
 ******************************************************************************
H<<*/


#include "stdafx.h"
#include "CpInternet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
/**
 * Constructor
 *
 */
//*****************************************************************************
CCpInternet::CCpInternet()
//*****************************************************************************
{
	m_pINetSession = NULL;
	m_pINetFile = NULL;
}

/////////////////////////////////////////////////////////////////////
/**
 * Destructor
 *
 */
//*****************************************************************************
CCpInternet::~CCpInternet()
//*****************************************************************************
{
	CloseFile();
}
/////////////////////////////////////////////////////////////////////
/**
 * Opens a file for reading
 *
 * @param pstrUrl			Complete URL of the file
 *							e.g. http://server/path/file
 *							e.g. ftp://server/path/file
 *							e.g. file://path/file
 *              
 * @return					TRUE: OK
 *							FALSE : Error
 *
 * @exception				no execption
 *
 * @see						
 */
//*****************************************************************************
BOOL CCpInternet::OpenFile(
	const LPCTSTR pstrUrl)
//*****************************************************************************
{
	CloseFile();

	CString URL(pstrUrl);
	CString CleanUrl;
	int index = URL.Find("///");
	if (index > 0)
		CleanUrl = URL.Left(index) + URL.Right(URL.GetLength() - index - 1);
	else
		CleanUrl = URL;

	m_pINetSession = new CInternetSession();
	if (m_pINetSession != NULL)
	{
		try 
		{
			m_pINetFile = m_pINetSession->OpenURL(
				CleanUrl, 1, 
				INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_RELOAD);
		}
		catch (...)
		{
			CloseFile();
		}
	}
	return m_pINetFile != NULL;
}
/////////////////////////////////////////////////////////////////////
/**
 * Closes previously opened file
 *
 */
//*****************************************************************************
void CCpInternet::CloseFile()
//*****************************************************************************
{
	if (m_pINetFile != NULL)
	{
		m_pINetFile->Close();
		delete m_pINetFile;
		m_pINetFile = NULL;
	}

	if (m_pINetSession != NULL)
	{
		m_pINetSession->Close();
		delete m_pINetSession;
		m_pINetSession = NULL;
	}
}


/////////////////////////////////////////////////////////////////////
/**
 * Reads a single line of text from a previously opened file
 *
 * @param TextLine			Text line to fill
 *              
 * @return					TRUE: at least one character read
 *							FALSE : no characters read
 *
 * @exception				no execption
 *
 * @see						
 */
//******************************************************************************
BOOL CCpInternet::ReadLine(
	CString& TextLine)
//******************************************************************************
{
	if (!IsFileOpen())
	{
		TextLine = "";
		ASSERT(0);
		return FALSE;
	}
	ASSERT(m_pINetFile != NULL);
	return m_pINetFile->ReadString(TextLine);	
}

/////////////////////////////////////////////////////////////////////
/**
 * Sends a single file via anonymous ftp to a server
 *
 * @param pstrSrcFileName	local source file (including path)
 *              
 * @param pstrUrl			destination URL e.g. 
 *							e.g. ftp://server/path/file
 *
 * @return					TRUE: OK
 *							FALSE : Error
 *
 * @exception				no execption
 *
 * @see        
 */
//******************************************************************************
BOOL CCpInternet::WriteFile(
	const LPCTSTR pstrSrcFileName,		
	const LPCTSTR pstrUrl) const		
//******************************************************************************
{
	BOOL rv = TRUE;			// return value
	DWORD dwServiceType;	// service type (only ftp allowed)
	CString strServer;		// destination server name
	CString strFile;		// destination file name
	INTERNET_PORT nPort;	// server port (not used)

	// parse destination URL
	if (rv)
	{
		rv = AfxParseURL(pstrUrl, dwServiceType, strServer, strFile, nPort);
		ASSERT(rv);		

		// only ftp service allowed
		if (dwServiceType != AFX_INET_SERVICE_FTP)
		{
			rv = FALSE;
			ASSERT(0);
		}
		// server required
		if (strServer.GetLength() == 0)
		{
			rv = FALSE;
			ASSERT(0);
		}
		// file required
		if (strFile.GetLength() == 0)
		{
			rv = FALSE;
			ASSERT(0);
		}
	}

	// send file via anonymous ftp
	if (rv)
	{
		CInternetSession* pINetSession = new CInternetSession();
		rv = pINetSession != NULL;
		if (rv)
		{
			// create anonymous connection
			CFtpConnection* pFtp = NULL;
			try 
			{
				pFtp = pINetSession->GetFtpConnection(strServer);
			}
			catch (...)
			{
				rv = FALSE;
			}

			// send file
			if (rv)
			{
				ASSERT(pFtp != NULL);
				rv = pFtp->PutFile(pstrSrcFileName, strFile);
			}

			delete pINetSession;
		}
	}
	return rv;
}


