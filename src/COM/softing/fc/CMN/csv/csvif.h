/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/csv/csvif.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: csvif.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/csv/csvif.h $
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
 *==  Library interface header for csv.lib
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  27.03.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#ifndef CSVIF_H
#define CSVIF_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#define CSV_TAB _T('\t')
#define CSV_NL _T('\n')
#define CSV_CR _T('\r')
#define CSV_FSEP _T(';')
#define CSV_DQUOTE _T('"')
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CCsvExp  
{
	CString m_strFile;
	CFile m_File;
	BOOL m_bEOL;

public:
	CCsvExp(LPCTSTR pszFileName = NULL);
	virtual ~CCsvExp(void);

	void Open(LPCTSTR pszFileName = NULL);
	void Close(void);
	BOOL IsOpen(void);

	void SetActiveDelimiter(BOOL bEOL = FALSE); 
	//only relevant in conjunction with the operators

	void WriteField(LPCTSTR pszField, BOOL bEOL = FALSE);
	void WriteDelimiter(BOOL bEOL = FALSE);

	CCsvExp & operator << (int);
    CCsvExp & operator << (short);
    CCsvExp & operator << (unsigned int);
    CCsvExp & operator << (unsigned short);
    CCsvExp & operator << (unsigned long);
    CCsvExp & operator << (long);
    CCsvExp & operator << (LPCTSTR psz);
	CCsvExp & operator << (COleDateTime dt);

    void WriteHex(unsigned long ulValue);
    void WriteHex(int iValue);
    void WriteHex(long lValue);
private:
	void AppendDelimiter(CString & strOut, BOOL bEOL);
    void WriteString(CString & strOut);
};

class CCsvParse
{
	CString m_strFile;
	CStdioFile m_File;
	CString m_strChunk;

	int m_iLineNo;
	int m_iLinePos;

public:
	CCsvParse(LPCTSTR pszFileName = NULL);
	virtual ~CCsvParse(void);
	
	HRESULT Open(LPCTSTR pszFileName = NULL);
	void Close(void);
	
	inline BOOL IsOpen(void)
	{
		return m_File.m_hFile != CFile::hFileNull;
	}

    BOOL ParseNextField(CString & strField);

/////////////////////////////////////////////////////////////////////////////////////////////
/**
*
*	Tests, if the cursor is at the end of a line.
*	@return     BOOL - true => EOL detected.
*			    false => otherwise
*/
	inline BOOL IsEndOfLine()
	{
		return m_iLinePos >= m_strChunk.GetLength();
	}

	BOOL ParseIdIntValueLine(CString & strId, int & iValue);

    //all these operators throw CFileException::endofFile 
    //if they can't read
	CCsvParse & operator >> (int &);
    CCsvParse & operator >> (short &);
    CCsvParse & operator >> (unsigned int &);
    CCsvParse & operator >> (unsigned short &);
    CCsvParse & operator >> (unsigned long &);
    CCsvParse & operator >> (long &);

    inline const int GetLineNo() const
    {
        return m_iLineNo;
    }

    inline const int GetLinePos() const
    {
        return m_iLinePos;
    }

    inline LPCTSTR GetFileName() const
    {
        return m_strFile;
    }

private: //private methods
    unsigned long CCsvParse::ReadULong();
};


#endif //CSVIF_H

/*
 *----------------------------------------------------------------------------*
 *  $History: csvif.h $
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
 *==
 *----------------------------------------------------------------------------*
*/
