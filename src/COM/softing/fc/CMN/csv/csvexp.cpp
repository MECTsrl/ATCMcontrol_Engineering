/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/csv/csvexp.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: csvexp.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/csv/csvexp.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           CSV
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
 *== CsvExp.cpp: implementation of the CCsvExp class. Performs csv like export
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  27.03.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "csvif.h"
#include "csvexp.h"
#include "baselib.h"

//----  Static Initializations:   ----------------------------------*


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCsvExp::CCsvExp(LPCTSTR pszFileName)
	: m_strFile() , m_File()
{
	if(pszFileName) {
		m_strFile = pszFileName;
	}

	m_bEOL = FALSE;
}

CCsvExp::~CCsvExp()
{
	Close();
}


/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*   Opens file for csv export/import.
*   The file can be specified by a file name as @param or has already
*   been set in the ctor. If the argument pszFileName is not null it overwrites the
*   current setting in the object (last file specification wins !).
*
*   @param      pszFileName - points to the asciiz-string determining the full path of
*               the file to open.
*   @exception  COleException, CFileException
*   @see        Close(), IsOpen()
*/
void CCsvExp::Open(LPCTSTR pszFileName)
{
	CFileException e;

	if(m_File.m_hFile != CFile::hFileNull) {
		assert(m_File.m_hFile == CFile::hFileNull);
		AfxThrowOleException(E_FAIL);
	} 

	//pszFileName always overwrites the current config in m_strFile
	if(m_strFile.IsEmpty()) {
		if(!pszFileName) {
			AfxThrowOleException(E_INVALIDARG);
		}

		m_strFile = pszFileName;
	}
	else {
		if(pszFileName) {
			m_strFile = pszFileName;
		}
	}

	if(!m_File.Open(m_strFile,CFile::modeCreate | CFile::modeReadWrite,&e)) {
        AfxThrowFileException(e.m_cause,e.m_lOsError,m_strFile);
	}
	
}

void CCsvExp::Close(void)
//throws CFileException !!
{
	if(m_File.m_hFile == CFile::hFileNull)
		return;

	m_File.Close();
}

BOOL CCsvExp::IsOpen(void)
{
	return m_File.m_hFile != CFile::hFileNull;
}

void CCsvExp::WriteField(LPCTSTR pszField, BOOL bEOL)
/*	throws CFileException; 
	writes field to file if bEOL set ,appends a newline
	iff not set appends a default delimiter ';'
*/
{
	assert(IsOpen());
	assert(pszField);
	
	CString strOut = pszField;
	AppendDelimiter(strOut,bEOL);
	
	WriteString(strOut);

}

void CCsvExp::AppendDelimiter(CString & strOut, BOOL bEOL)
{
	if(!bEOL) { 
	    strOut = strOut + CSV_FSEP;
    }
    else {
        strOut = strOut + CSV_CR;
        strOut = strOut + CSV_NL;
    } 
}

void CCsvExp::WriteDelimiter(BOOL bEOL)
/*	throws CFileException; 
	writes only a delimiter to file - if bEOL set,newline
	iff not set default delimiter ';'
*/
{
	assert(IsOpen());
	CString strOut;

	AppendDelimiter(strOut,bEOL);
	
	WriteString(strOut);
}

CCsvExp & CCsvExp::operator << (int i)
/*	throws CFileException;
*/
{
	assert(IsOpen());
	CString strOut;

	strOut.Format(_T("%d"),i);
	AppendDelimiter(strOut,m_bEOL);

	WriteString(strOut);
	return *this;
}

CCsvExp & CCsvExp::operator << (short i)
/*	throws CFileException;
*/
{
	assert(IsOpen());
	CString strOut;

	strOut.Format(_T("%d"),i);
	AppendDelimiter(strOut,m_bEOL);

	WriteString(strOut);
	return *this;
}

CCsvExp & CCsvExp::operator << (unsigned int i)
/*	throws CFileException;
*/
{
	assert(IsOpen());
	CString strOut;

	strOut.Format(_T("%u"),i);
	AppendDelimiter(strOut,m_bEOL);

	WriteString(strOut);
	return *this;
}

CCsvExp & CCsvExp::operator << (unsigned short i)
/*	throws CFileException;
*/
{
	assert(IsOpen());
	CString strOut;

	strOut.Format(_T("%u"),i);
	AppendDelimiter(strOut,m_bEOL);

	WriteString(strOut);
	return *this;
}

CCsvExp & CCsvExp::operator << (unsigned long i)
/*	throws CFileException;
*/
{
	assert(IsOpen());
	CString strOut;

	strOut.Format(_T("%lu"),i);
	AppendDelimiter(strOut,m_bEOL);

	WriteString(strOut);
	return *this;
}

CCsvExp & CCsvExp::operator << (long i)
/*	throws CFileException;
*/
{
	assert(IsOpen());
	CString strOut;

	strOut.Format(_T("%ld"),i);
	AppendDelimiter(strOut,m_bEOL);

	WriteString(strOut);
	return *this;
}

CCsvExp & CCsvExp::operator << (LPCTSTR psz)
/*	throws CFileException;
*/
{
	assert(IsOpen());
	CString strOut;

	strOut = psz;
	AppendDelimiter(strOut,m_bEOL);

	WriteString(strOut);
	return *this;
}

CCsvExp & CCsvExp::operator << (COleDateTime dt)
/*	throws CFileException;
*/
{
	COleVariant vt;
	CString strOut;
	HRESULT hr;

	assert(IsOpen());
	vt = dt;

/*
    SYSTEMTIME LocalTime;

    hr = VariantTimeToSystemTime(vt.date, &LocalTime);

	if(SUCCEEDED(hr)) {
		strOut.Format(_T("DT#%04d-%02d-%02d-%02d:%02d:%02d.%03d"),
			LocalTime.wYear,
			LocalTime.wMonth,
			LocalTime.wDay,
			LocalTime.wHour,
			LocalTime.wMinute,
			LocalTime.wSecond,
			LocalTime.wMilliseconds);
	}
*/
	OLECHAR sOut[DT_STRING_LEN];

	hr = BL_VariantToString(&vt,BL_DT,NULL,BL_USE_IEC_FORMAT,
		sOut,FC_ARRAY_LEN(sOut));

	if(SUCCEEDED(hr)) {
		strOut = sOut;
	}

	AppendDelimiter(strOut,m_bEOL);
	WriteString(strOut);
	return *this;
}

void CCsvExp::SetActiveDelimiter(BOOL bEOL)
//used only for the operators
{
	m_bEOL = bEOL;
}

void CCsvExp::WriteString(CString & strOut)
{
    LPCTSTR pszOut = strOut;
    m_File.Write(pszOut,lstrlen(pszOut));
}


void CCsvExp::WriteHex(unsigned long ulValue)
{
	assert(IsOpen());
	CString strOut;

	strOut.Format(_T("0x%lx"),ulValue);
	AppendDelimiter(strOut,m_bEOL);

	WriteString(strOut);
}

void CCsvExp::WriteHex(long lValue)
{
	assert(IsOpen());
	CString strOut;

	strOut.Format(_T("0x%lx"),lValue);
	AppendDelimiter(strOut,m_bEOL);

	WriteString(strOut);
}

void CCsvExp::WriteHex(int iValue)
{
	assert(IsOpen());
	CString strOut;

	strOut.Format(_T("0x%x"),iValue);
	AppendDelimiter(strOut,m_bEOL);

	WriteString(strOut);
}

/*
 *----------------------------------------------------------------------------*
 *  $History: csvexp.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/csv
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/csv
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 27.03.01   Time: 18:42
 * Created in $/4Control/COM/softing/fc/CMN/csv
 * *****************  Version 3  *****************
 * User: Su           Date: 9/22/99    Time: 3:58p
 * Updated in $/4Control/COM/softing/fc/CMN/Csv
 * overloaded function WriteHex for data types int,long,ulong.
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 6/29/99    Time: 7:59p
 * Updated in $/4Control/COM/softing/fc/CMN/Csv
 * New operators for export
 *==
 *----------------------------------------------------------------------------*
*/
