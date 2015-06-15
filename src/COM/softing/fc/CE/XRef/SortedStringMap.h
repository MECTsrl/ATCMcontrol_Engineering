// SortedStringMap.h: interface for the CSortedStringMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SORTEDSTRINGMAP_H__53021807_75C9_11D5_9E83_00D0B77F92EB__INCLUDED_)
#define AFX_SORTEDSTRINGMAP_H__53021807_75C9_11D5_9E83_00D0B77F92EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef int StringId;
const StringId	INVALID_STRING = -1;

const char	szInfoSection[] = "[INFO]";
const char	szDefinitionSection[] = "[DEFINITION]";
const char	szDeclarationSection[] = "[DECLARATION]";
const char	szReferenceSection[] = "[REFERENCE]";
const char	szAll[] = "<All>";
const char	szAny[] = "*";
const char	szDot[] = ".";

const TCHAR chSEMICOLON = _T(';');
const TCHAR chBRACKETOPEN = _T('[');
const TCHAR chDOT = _T('.');
const TCHAR chDOUBLECOLON = _T(':');
const TCHAR chSTAR = _T('*');

class CStringSep : public CString
{
friend istream&  operator >>(istream& is, CStringSep & sItem);

public:
	CStringSep(TCHAR cSep = _T('\n')) : m_Seperator(cSep){;}
	~CStringSep();
protected:
	TCHAR m_Seperator;
};
const int	STRING_NO_FOUND = -1;
const int	STRING_ANY = -2;


BOOL IsGlobalName(CString &sName);

class CSortedStringMap : public CObject  
{
public:
	CSortedStringMap();
	virtual ~CSortedStringMap();

	DECLARE_SERIAL(CSortedStringMap)
	void Serialize( CArchive& archive );
	
	int		AddItem(const CString &sNewString);
	int		FindItem(const CString &sFindString);
	const CString &GetItem(int iIndex);
	const CString &GetSortedItem(int iIndex);
	int		GetCount();
public:	
	const	int iGrowSize;

protected:
	// functions
	int		FindInsertPosition(const CString &sFindString);

protected:
	// data
	CStringArray	m_HashStrings;
	CDWordArray		m_SortedStrings;
	static	CString sEmpty;
};

#endif // !defined(AFX_SORTEDSTRINGMAP_H__53021807_75C9_11D5_9E83_00D0B77F92EB__INCLUDED_)
