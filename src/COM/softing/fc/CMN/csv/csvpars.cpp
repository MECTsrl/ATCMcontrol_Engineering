/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/csv/csvpars.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: csvpars.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/csv/csvpars.cpp $
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
 *== *.csv format parser class
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  27.03.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "atlconv.h"
#include "csvif.h"
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*



CCsvParse::CCsvParse(LPCTSTR pszFileName)
	: m_strFile(),
	  m_strChunk(),
	  m_iLineNo(0),	
	  m_iLinePos(0)
{
	if(pszFileName) {
		m_strFile = pszFileName;
	}
}

CCsvParse::~CCsvParse(void)
{
	Close();
}

HRESULT CCsvParse::Open(LPCTSTR pszFileName)
{
	CFileException e;
	HRESULT hr;

	if(m_File.m_hFile != CFile::hFileNull) {
		assert(m_File.m_hFile == CFile::hFileNull);
		return E_FAIL;
	} 

	//pszFileName always overwrites the current config in m_strFile
	if(m_strFile.IsEmpty()) {
		if(!pszFileName) {
			return E_INVALIDARG;
		}

		m_strFile = pszFileName;
	}
	else {
		if(pszFileName) {
			m_strFile = pszFileName;
		}
	}

	if(!m_File.Open(m_strFile,CFile::modeRead,&e)) {
		hr = HRESULT_FROM_WIN32(e.m_lOsError);
		return hr;
	}
	
	return S_OK;
}

void CCsvParse::Close(void)
//throws CFileException !!
{
	if(m_File.m_hFile == CFile::hFileNull)
		return;

	m_File.Close();
}




BOOL CCsvParse::ParseNextField(CString & strField)
/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	ParseNextField() returns the next input field. On success
*	the passed argument strField will be filled with the read field. Escape sequences
*	(double-'"') will be replaced by original char.
*
*	@param	    strField - output parameter; will be filled with the CSV-Field read from
*			    file.
*	@return     TRUE - success; FALSE: error (unexpected end of file, I/O errors).
*	@see        NextChar()
*/
{
	assert(IsOpen());
	if(!IsOpen())
		return false;
	//clear the fild
	strField.Empty();

	//if needed, get a new line from the file
	if(m_strChunk.IsEmpty() || m_iLinePos >= m_strChunk.GetLength())
	{
		if(!m_File.ReadString(m_strChunk))
			return false;
		m_iLinePos = 0;
		m_iLineNo++;
		//no chars on the line
		if(m_strChunk.IsEmpty())
			return true;
	}
	else
	{
		m_iLinePos++;
	}
	//see if there is a double quote zone
	TCHAR separator;
	if(m_strChunk.GetAt(m_iLinePos) == CSV_DQUOTE)
	{
		//the separator is the ending quote
		separator  = CSV_DQUOTE;
	}
	else
	{
		//separator is the ; char
		separator = CSV_FSEP;
	}
	
	//find the next separator or the end of line
	int stopIndex = m_strChunk.Find(separator, m_iLinePos);

	//no separator found, read until the end of line
	if(stopIndex == -1 )
	{
		//if there was suposed to be a quote block and no ending quote was found, rise an error
		if( separator == CSV_DQUOTE)
		{
			FC_DEBUGPRINT3(_T("CSV> %s(%d:%d): illegal csv file format, \" without \"\n"),m_strFile,m_iLineNo,m_iLinePos);
			return false;
		}
		else
		{
			stopIndex = m_strChunk.GetLength();
		}
	}

	if(separator == CSV_DQUOTE)
	{
		//get the text inside the quote block, without the " char
		strField = m_strChunk.Mid(m_iLinePos + 1, stopIndex - m_iLinePos - 1);
		
	}
	else
	{
		//get the next token
		strField = m_strChunk.Mid(m_iLinePos, stopIndex - m_iLinePos);
	}

	//update the line position
	m_iLinePos = stopIndex;
	
	//successfull
	return true;
}



BOOL CCsvParse::ParseIdIntValueLine(CString & strId, int & iValue)
/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*   ParseIdIntValueLine parses a tagged line. The line has the syntax id;int-value. 
*	@param	    strId - the tag.
*	@param	    iValue - will be set on output to the numeric value
*	@return	    BOOL - TRUE: ok, FALSE: syntax error
*	@see        ParseNextField()
*/
{
    CString strToken;
	BOOL bRet; 
	
	assert(IsOpen());		
	
	/*set to a weird value to avoid use after a false return.
	  people never check the return value but use the out param
	*/
	iValue = 0xEEEEEEEE;

	if (IsEndOfLine()) {
	
		bRet = ParseNextField(strToken);
      
		if (bRet && strToken == strId && !IsEndOfLine()) {
        
			bRet = ParseNextField(strToken);

	        if (bRet && IsEndOfLine()) {
				_ttoi(strToken);
				return TRUE;
			}
		}
    }

	FC_DEBUGPRINT3(_T("\nCSV> %s(%d:%d): syntax error"),m_strFile,m_iLineNo,m_iLinePos);
	return FALSE;
}

CCsvParse & CCsvParse::operator >> (int & iValue)
{
    unsigned long ulValue = ReadULong();
    iValue = (int)ulValue;
   	return *this;
}

CCsvParse & CCsvParse::operator >> (short & nValue)
{
    unsigned long ulValue = ReadULong();
    nValue = (short)ulValue;
	return *this;
}

CCsvParse & CCsvParse::operator >> (unsigned int & uiValue)
{
    unsigned long ulValue = ReadULong();
    uiValue = (unsigned int)ulValue;
	return *this;
}

CCsvParse & CCsvParse::operator >> (unsigned short & unValue)
{
    unsigned long ulValue = ReadULong();
    unValue = (unsigned short)ulValue;
	return *this;
}

CCsvParse & CCsvParse::operator >> (unsigned long & ulValue)
{
    ulValue = ReadULong();
	return *this;
}

CCsvParse & CCsvParse::operator >> (long & lValue)
{
    unsigned long ulValue = ReadULong();
    lValue = (long)ulValue;
	return *this;
}

unsigned long CCsvParse::ReadULong()
{
    CString strField;

    BOOL bRet = ParseNextField(strField);
    if(!bRet) {
        AfxThrowFileException(CFileException::endOfFile,-1,m_strFile);
    }

    /*  set base to 0 - means the base will be detected by string strField
        strField = '0' [x|X] [0-9a-fA-F]+ - hex integer
        strField = '0' [0-7]+ - octal integer
        strField = [1-9][0-9]* - decimal

        see description of strtoul
    */
    unsigned long ulValue = _tcstoul(strField,NULL,0);
    return ulValue;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: csvpars.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/csv
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 6.06.06    Time: 16:44
 * Updated in $/4Control/COM/softing/fc/CMN/csv
 * changes from CA: downloader performance optimizations
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/csv
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 27.03.01   Time: 18:42
 * Created in $/4Control/COM/softing/fc/CMN/csv
 * *****************  Version 4  *****************
 * User: Su           Date: 9/22/99    Time: 3:57p
 * Updated in $/4Control/COM/softing/fc/CMN/Csv
 * CCsvParse::ReadULong - use standard detection mechanism of _tcstoul
 * (strtoul).
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 7/01/99    Time: 8:46p
 * Updated in $/4Control/COM/softing/fc/CMN/Csv
 * CCsvParse: added operators.
 * 
 *==
 *----------------------------------------------------------------------------*
*/
