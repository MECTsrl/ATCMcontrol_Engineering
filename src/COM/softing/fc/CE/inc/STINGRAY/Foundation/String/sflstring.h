/////////////////////////////////////////////////////////////////////////////
// sflstring.h : CString compatible wrapper 
// implemented via std::basic_string<TCHAR>
//
// Stingray Software Extension Classes
// Copyright (C) 1997 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Authors:      Steve Danielson
// Description:  CString compatible string class
//
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/String/sflstring.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

#pragma once

#ifndef _AFX

#include <windows.h>
#include <string>
#include <crtdbg.h>
#include <tchar.h>
#include "StringEx.h"

#ifdef _MFC_VER
	#ifndef __AFXCONV_H__
	#include <afxconv.h>
	#endif //__AFXCONV_H__
#else
	#ifndef __ATLCONV_H__
	#include <atlconv.h>
	#endif //__ATLCONV_H__
#endif //_MFC_VER



#ifdef _MFC_VER
typedef CString CMFCString;
#endif

namespace stingray {
namespace foundation {


/////////////////////////////////////////////////////////////////
// stingray::foundation::CString class
//
// Use just like MFC's CString:
//
// using foundation::stingray::CString;
// ...
// CString str;
// ...
// 
// All CString operations are present except for
// CArchive << and >>, and CDumpContext <<
//
// Note that the 1st data member is an LPCTSTR that is kept in
// synch to a pointer to the data so it isn't necessary to
// explicitly cast to LPCTSTR anytime you are passing to any 
// kind of variable argument parameters such as Format or TRACE 

class CString
{
public:
	CString();
	// copy constructor
	CString(const CString& s);
	// from a single character
	CString(TCHAR ch, int nRepeat = 1);
	// from an ANSI string (converts to TCHAR)
	CString(LPCSTR lpsz);
	// from a UNICODE string (converts to TCHAR)
	CString(LPCWSTR lpsz);
	// subset of characters from an ANSI string (converts to TCHAR)
	CString(LPCSTR lpsz, int nLength);
	// subset of characters from a UNICODE string (converts to TCHAR)
	CString(LPCWSTR lpsz, int nLength);
	// from unsigned characters
	CString(const unsigned char* lpsz);

// Attributes & Operations

	// get data length
	int GetLength() const;
	// true if zero length
	bool IsEmpty() const;
	// clear contents to empty
	void Empty();

	// return single character at zero-based index
	TCHAR GetAt(int nIndex) const;
	// return single character at zero-based index
	TCHAR operator[](int nIndex) const;
	// set a single character at zero-based index
	void SetAt(int nIndex, TCHAR ch);
	// return pointer to const string
	operator LPCTSTR() const;

	// overloaded assignment

	// copy from another CString
	const CString& operator=(const CString& stringSrc);
	// set string content to single character
	const CString& operator=(TCHAR ch);
#ifdef _UNICODE
	const CString& operator=(char ch);
#endif
	// copy string content from ANSI string (converts to TCHAR)
	const CString& operator=(LPCSTR lpsz);
	// copy string content from UNICODE string (converts to TCHAR)
	const CString& operator=(LPCWSTR lpsz);
	// copy string content from unsigned chars
	const CString& operator=(const unsigned char* lpsz);

	// string concatenation

	// concatenate from another CString
	const CString& operator+=(const CString& s);

	// concatenate a single character
	const CString& operator+=(TCHAR ch);
#ifdef _UNICODE
	// concatenate an ANSI character after converting it to TCHAR
	const CString& operator+=(char ch);
#endif
	// concatenate a UNICODE character after converting it to TCHAR
	const CString& operator+=(LPCTSTR lpsz);

	friend CString __stdcall operator+(const CString& s1, const CString& s2);
	friend CString __stdcall operator+(const CString& s, TCHAR ch);
	friend CString __stdcall operator+(TCHAR ch, const CString& s);
#ifdef _UNICODE
	friend CString __stdcall operator+(const CString& s, char ch);
	friend CString __stdcall operator+(char ch, const CString& s);
#endif
	friend CString __stdcall operator+(const CString& s, LPCTSTR lpsz);
	friend CString __stdcall operator+(LPCTSTR lpsz, const CString& s);

	// string comparison

	// straight character comparison
	int Compare(LPCTSTR lpsz) const;
	// compare ignoring case
	int CompareNoCase(LPCTSTR lpsz) const;
	// NLS aware comparison, case sensitive
	int Collate(LPCTSTR lpsz) const;
	// NLS aware comparison, case insensitive
	int CollateNoCase(LPCTSTR lpsz) const;

	// simple sub-string extraction

	// return nCount characters starting at zero-based nFirst
	CString Mid(int nFirst, int nCount) const;
	// return all characters starting at zero-based nFirst
	CString Mid(int nFirst) const;
	// return first nCount characters in string
	CString Left(int nCount) const;
	// return nCount characters from end of string
	CString Right(int nCount) const;

	//  characters from beginning that are also in passed string
	CString SpanIncluding(LPCTSTR lpszCharSet) const;
	// characters from beginning that are not also in passed string
	CString SpanExcluding(LPCTSTR lpszCharSet) const;

	// upper/lower/reverse conversion

	// NLS aware conversion to uppercase
	void MakeUpper();
	// NLS aware conversion to lowercase
	void MakeLower();
	// reverse string right-to-left
	void MakeReverse();

	// trimming whitespace (either side)

	// remove whitespace starting from right edge
	void TrimRight();
	// remove whitespace starting from left side
	void TrimLeft();

	// trimming anything (either side)

	// remove continuous occurrences of chTarget starting from right
	void TrimRight(TCHAR chTarget);
	// remove continuous occcurrences of characters in passed string,
	// starting from right
	void TrimRight(LPCTSTR lpszTargets);
	// remove continuous occurrences of chTarget starting from left
	void TrimLeft(TCHAR chTarget);
	// remove continuous occcurrences of characters in
	// passed string, starting from left
	void TrimLeft(LPCTSTR lpszTargets);

	// advanced manipulation

	// replace occurrences of chOld with chNew
	int Replace(TCHAR chOld, TCHAR chNew);
	// replace occurrences of substring lpszOld with lpszNew;
	// empty lpszNew removes instances of lpszOld
	int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew);
	// remove occurrences of chRemove
	int Remove(TCHAR chRemove);
	// insert character at zero-based index; concatenates
	// if index is past end of string
	int Insert(int nIndex, TCHAR ch);
	// insert substring at zero-based index; concatenates
	// if index is past end of string
	int Insert(int nIndex, LPCTSTR lpsz);
	// delete nCount characters starting at zero-based index
	int Delete(int nIndex, int nCount = 1);

	// searching

	// find character starting at left, -1 if not found
	int Find(TCHAR ch) const;
	// find character starting at right
	int ReverseFind(TCHAR ch) const;
	// find character starting at zero-based index and going right
	int Find(TCHAR ch, int nStart) const;
	// find first instance of any character in passed string
	int FindOneOf(LPCTSTR lpszCharSet) const;
	// find first instance of substring
	int Find(LPCTSTR lpszSub) const;
	// find first instance of substring starting at zero-based index
	int Find(LPCTSTR lpszSub, int nStart) const;

	// simple formatting

	// printf-like formatting using passed string
	void __cdecl Format(LPCTSTR lpszFormat, ...);
	// printf-like formatting using referenced string resource
	void __cdecl Format(UINT nFormatID, ...);
	// printf-like formatting using variable arguments parameter
	void FormatV(LPCTSTR lpszFormat, va_list argList);

	// formatting for localization (uses FormatMessage API)

	// format using FormatMessage API on passed string
	void __cdecl FormatMessage(LPCTSTR lpszFormat, ...);
	// format using FormatMessage API on referenced string resource
	void __cdecl FormatMessage(UINT nFormatID, ...);

	// Skipping Dump, and CArchive << and >>

	// load from string resource
	bool LoadString(UINT nID);

#ifndef _UNICODE
	// ANSI <-> OEM support (convert string in place)

	// convert string from ANSI to OEM in-place
	void AnsiToOem();
	// convert string from OEM to ANSI in-place
	void OemToAnsi();
#endif

	// OLE BSTR support (use for OLE automation)

	// return a BSTR initialized with this CString's data
	BSTR AllocSysString() const;
	// reallocates the passed BSTR, copies content of this CString to it
	BSTR SetSysString(BSTR* pbstr) const;

	// Access to string implementation buffer as "C" character array

	// get pointer to modifiable buffer at least as long as nMinBufLength
	LPTSTR GetBuffer(int nMinBufLength);
	// release buffer, setting length to nNewLength (or to first nul if -1)
	void ReleaseBuffer(int nNewLength = -1);
	// get pointer to modifiable buffer exactly as long as nNewLength
	LPTSTR GetBufferSetLength(int nNewLength);
	// release memory allocated to but unused by string
	void FreeExtra();

	// Use LockBuffer/UnlockBuffer to turn refcounting off
	// Gets the buffer
	LPTSTR LockBuffer();
	// turn refcounting off - no-op
	void UnlockBuffer();

	~CString();
	int GetAllocLength() const;


protected:

#ifdef UNICODE
	typedef stingray::foundation::wstring InternalString;
#else
	typedef stingray::foundation::string InternalString;
#endif
	// 1st data member so we don't have to cast
	// to LPCTSTR all the time
	LPCTSTR m_lpszData;

	InternalString m_str;

public:
	// Additional constructor
	CString(const InternalString& s);

};

// Compare helpers
bool __stdcall operator==(const CString& s1, const CString& s2);
bool __stdcall operator==(const CString& s1, LPCTSTR s2);
bool __stdcall operator==(LPCTSTR s1, const CString& s2);
bool __stdcall operator!=(const CString& s1, const CString& s2);
bool __stdcall operator!=(const CString& s1, LPCTSTR s2);
bool __stdcall operator!=(LPCTSTR s1, const CString& s2);
bool __stdcall operator<(const CString& s1, const CString& s2);
bool __stdcall operator<(const CString& s1, LPCTSTR s2);
bool __stdcall operator<(LPCTSTR s1, const CString& s2);
bool __stdcall operator>(const CString& s1, const CString& s2);
bool __stdcall operator>(const CString& s1, LPCTSTR s2);
bool __stdcall operator>(LPCTSTR s1, const CString& s2);
bool __stdcall operator<=(const CString& s1, const CString& s2);
bool __stdcall operator<=(const CString& s1, LPCTSTR s2);
bool __stdcall operator<=(LPCTSTR s1, const CString& s2);
bool __stdcall operator>=(const CString& s1, const CString& s2);
bool __stdcall operator>=(const CString& s1, LPCTSTR s2);
bool __stdcall operator>=(LPCTSTR s1, const CString& s2);

}	// namespace stingray::foundation
}   // namespace stingray

// Class Definition
namespace stingray {
namespace foundation {

// Private constructor used internally
inline CString::CString(const InternalString& s)
{
	m_str = s;
	m_lpszData = m_str.c_str();
}

inline CString::CString()
{
	m_lpszData = m_str.c_str();
}

// copy constructor
inline CString::CString(const CString& s)
{
	*this = s;
}

// from a single character
inline CString::CString(TCHAR ch, int nRepeat /* = 1 */)
{
	// Must cast int to size_type to resolve ambiguity
	m_str.insert(static_cast<std::basic_string<TCHAR>::size_type>(0),
				 static_cast<std::basic_string<TCHAR>::size_type>(nRepeat),
				 ch);

	m_lpszData = m_str.c_str();
}

inline CString::CString(LPCTSTR lpsz)
{
	if (lpsz != NULL && HIWORD(lpsz) == NULL)
	{
		UINT nID = LOWORD((DWORD)lpsz);
		LoadString(nID);
	}
	else
	{
		*this = lpsz;
	}
}

// from an ANSI string (converts to TCHAR)
#ifdef _UNICODE
inline CString::CString(LPCSTR lpsz)
{
	*this = lpsz;
}
#else
// from a UNICODE string (converts to TCHAR)
inline CString::CString(LPCWSTR lpsz)
{
	*this = lpsz;
}
#endif //_UNICODE

// subset of characters from lpsz
inline CString::CString(LPCTSTR lpsz, int nLength)
{
	// Can't accept NULL here
#pragma warning (disable : 4127)
	_ASSERTE(lpsz);


	m_str.insert(0, lpsz, nLength);
	m_lpszData = m_str.c_str();
}

#ifdef _UNICODE
// subset of characters from an ANSI string (converts to TCHAR)
inline CString::CString(LPCSTR lpsz, int nLength)
{
	// Can't accept NULL here
	_ASSERTE(lpsz);

	USES_CONVERSION;

	LPCTSTR lpszNewText = A2CT(lpsz);
	_ASSERTE(lpszNewText);

	m_str.insert(0, lpszNewText, nLength);
	m_lpszData = m_str.c_str();
}
#else
// subset of characters from a UNICODE string (converts to TCHAR)
inline CString::CString(LPCWSTR lpsz, int nLength)
{
	// Can't accept NULL here
	_ASSERTE(lpsz);

	USES_CONVERSION;

	LPCTSTR lpszNewText = W2CT(lpsz);
	_ASSERTE(lpszNewText);

	m_str.insert(0, lpszNewText, nLength);
	m_lpszData = m_str.c_str();
}
#endif //_UNICODE

// from unsigned characters
inline CString::CString(const unsigned char* lpsz)
{
	*this = lpsz;
}

inline CString::~CString()
{
}

// Attributes & Operations

// get data length
inline int CString::GetLength() const
{
	return (int)m_str.length();
}

// true if zero length
inline bool CString::IsEmpty() const
{
	return m_str.empty();
}

// clear contents to empty
inline void CString::Empty()
{
	// I'm sure there is a better way
	m_str = _T("");
	m_lpszData = m_str.c_str();
}

// return single character at zero-based index
inline TCHAR CString::GetAt(int nIndex) const
{
	_ASSERTE(nIndex >= 0 && nIndex < GetLength());
	return m_str.at(nIndex);
}

// return single character at zero-based index
inline TCHAR CString::operator[](int nIndex) const
{
	return GetAt(nIndex);
}

// set a single character at zero-based index
inline void CString::SetAt(int nIndex, TCHAR ch)
{
	_ASSERTE(nIndex >= 0 && nIndex < GetLength());
	m_str.at(nIndex) = ch;
}

// return pointer to const string
inline CString::operator LPCTSTR() const
{
	return m_str.c_str();
}

// overloaded assignment

// copy from another CString
inline const CString& CString::operator=(const CString& s)
{
	if(this == &s)
	{
		return const_cast<CString&>(*this);
	}

	m_str = s.m_str;
	m_lpszData = m_str.c_str();

	return *this;
}

// set string content to single character
inline const CString& CString::operator=(TCHAR ch)
{
	m_str = ch;
	m_lpszData = m_str.c_str();
	return *this;
}

#ifdef _UNICODE
inline const CString& CString::operator=(char ch)
{
	// We can promote a char to wchar_t
	m_str = static_cast<wchar_t>(ch);
	m_lpszData = m_str.c_str();
	return *this;
}
#endif

inline const CString& CString::operator=(LPCTSTR lpsz)
{
	if(lpsz)
	{
		m_str = lpsz;
		m_lpszData = m_str.c_str();
	}
	else
	{
		Empty();
	}

	return *this;
}
#ifdef _UNICODE
// copy string content from ANSI string (converts to TCHAR)
inline const CString& CString::operator=(LPCSTR lpsz)
{
	if(lpsz)
	{
		USES_CONVERSION;

		LPCWSTR lpszNewText = A2CW(lpsz);

		_ASSERTE(lpszNewText);

		m_str = lpszNewText;
		m_lpszData = m_str.c_str();
	}
	else
	{
		Empty();
	}

	return *this;
}
#else
// copy string content from UNICODE string (converts to TCHAR)
inline const CString& CString::operator=(LPCWSTR lpsz)
{
	if(lpsz)
	{
		USES_CONVERSION;

		LPCSTR lpszNewText = W2CA(lpsz);

		_ASSERTE(lpszNewText);

		m_str = lpszNewText;
		m_lpszData = m_str.c_str();
	}
	else
	{
		Empty();
	}

	return *this;
}
#endif // _UNICODE

// copy string content from unsigned chars
inline const CString& CString::operator=(const unsigned char* lpsz)
{
	// TODO: Check this out. This is how CString
	// does it, but verify
	*this = (LPCSTR)(lpsz);

	return *this;
}

// string concatenation

// concatenate from another CString
inline const CString& CString::operator+=(const CString& s)
{
	m_str += s.m_str;
	m_lpszData = m_str.c_str();

	return *this;
}

// concatenate a single character
inline const CString& CString::operator+=(TCHAR ch)
{
	m_str += ch;
	m_lpszData = m_str.c_str();

	return *this;
}

#ifdef _UNICODE
// concatenate an ANSI character after converting it to TCHAR
inline const CString& CString::operator+=(char ch)
{
	// We can promote a char to wchar_t
	m_str += static_cast<wchar_t>(ch);
	m_lpszData = m_str.c_str();

	return *this;
}

#endif
// concatenate a UNICODE character after converting it to TCHAR
inline const CString& CString::operator+=(LPCTSTR lpsz)
{
	// CString allows += with NULL
	// but not basic_string
	if(lpsz)
	{
		m_str += lpsz;
		m_lpszData = m_str.c_str();
	}

	return *this;
}

// operator +

inline CString __stdcall operator+(const CString& s1, const CString& s2)
{
	CString str = s1;
	str += s2;

	return str;
}

inline CString __stdcall operator+(const CString& s, TCHAR ch)
{
	CString str = s;
	str += ch;

	return str;
}

inline CString __stdcall operator+(TCHAR ch, const CString& s)
{
	CString str = ch;
	str += s;

	return str;
}

#ifdef _UNICODE
inline CString __stdcall operator+(const CString& s, char ch)
{
	CString str = s;
	str += static_cast<wchar_t>(ch);

	return str;
}

inline CString __stdcall operator+(char ch, const CString& s)
{
	CString str = static_cast<wchar_t>(ch);
	str += s;

	return str;
}
#endif

inline CString __stdcall operator+(const CString& s, LPCTSTR lpsz)
{
	CString str = s;
	if(lpsz)
	{
		str += lpsz;
	}

	return str;
}

inline CString __stdcall operator+(LPCTSTR lpsz, const CString& s)
{
	CString str;
	if(lpsz)
	{
		str += lpsz;
	}
	str += s;

	return str;
}

// string comparison

// straight character comparison
inline int CString::Compare(LPCTSTR lpsz) const
{
	_ASSERTE(lpsz);
	return _tcscmp((LPCTSTR)*this, lpsz);
}

// compare ignoring case
inline int CString::CompareNoCase(LPCTSTR lpsz) const
{
	_ASSERTE(lpsz);
	return _tcsicmp((LPCTSTR)*this, lpsz);
}

// NLS aware comparison, case sensitive
inline int CString::Collate(LPCTSTR lpsz) const
{
	_ASSERTE(lpsz);
	return _tcscoll((LPCTSTR)*this, lpsz);
}

// NLS aware comparison, case insensitive
inline int CString::CollateNoCase(LPCTSTR lpsz) const
{
	_ASSERTE(lpsz);
	return _tcsicoll((LPCTSTR)*this, lpsz);
}

// simple sub-string extraction

// return nCount characters starting at zero-based nFirst
inline CString CString::Mid(int nFirst, int nCount) const
{
	// out-of-bounds requests return sensible things
	int nLength = GetLength();
	if(nLength == 0)
	{
		return CString();
	}

	if(nFirst < 0)
	{
		nFirst = 0;
	}

	if(nCount < 0)
	{
		nCount = 0;
	}

	if(nFirst >= nLength)
	{
		nFirst = nLength;
	}

	if(nFirst + nCount > nLength)
	{
		nCount = nLength - nFirst;
	}

	_ASSERTE(nFirst >= 0);
	_ASSERTE(nFirst + nCount <= nLength);

	return m_str.substr(nFirst, nCount);
}

// return all characters starting at zero-based nFirst
inline CString CString::Mid(int nFirst) const
{
	return Mid(nFirst, GetLength() - nFirst);
}

// return first nCount characters in string
inline CString CString::Left(int nCount) const
{
	return m_str.substr(0, nCount);
}

// return nCount characters from end of string
inline CString CString::Right(int nCount) const
{
	int nFirst = GetLength() - nCount;
	if(nFirst < 0)
	{
		nFirst = 0;
	}

	return m_str.substr(nFirst);
}

//  characters from beginning that are also in passed string
// strspn equivalent
inline CString CString::SpanIncluding(LPCTSTR lpszCharSet) const
{
	_ASSERTE(lpszCharSet);
	return Left((int)_tcsspn((LPCTSTR)*this, lpszCharSet));
}

// characters from beginning that are not also in passed string
// strcspn equivalent
inline CString CString::SpanExcluding(LPCTSTR lpszCharSet) const
{
	_ASSERTE(lpszCharSet);
	return Left((int)_tcscspn((LPCTSTR)*this, lpszCharSet));
}

// upper/lower/reverse conversion

// NLS aware conversion to uppercase
inline void CString::MakeUpper()
{
	LPTSTR lpszBuf = const_cast<LPTSTR>(m_str.c_str());
	_ASSERTE(lpszBuf == m_str.data());
	_tcsupr(lpszBuf);
}

// NLS aware conversion to lowercase
inline void CString::MakeLower()
{
	LPTSTR lpszBuf = const_cast<LPTSTR>(m_str.c_str());
	_ASSERTE(lpszBuf == m_str.data());
	_tcslwr(lpszBuf);
}

// reverse string right-to-left
inline void CString::MakeReverse()
{
	LPTSTR lpszBuf = const_cast<LPTSTR>(m_str.c_str());
	_ASSERTE(lpszBuf == m_str.data());
	_tcsrev(lpszBuf);
}

// trimming whitespace (either side)

// remove whitespace starting from right edge
inline void CString::TrimRight()
{
	TrimRight(' ');
}

// remove whitespace starting from left side
inline void CString::TrimLeft()
{
	TrimLeft(' ');
}

// trimming anything (either side)

// remove continuous occurrences of chTarget starting from right
inline void CString::TrimRight(TCHAR chTarget)
{
	size_t nIndex = m_str.find_last_not_of(chTarget);
	if(nIndex == std::basic_string<TCHAR>::npos)
	{
	    //if the string contains ONLY the trim target (i.e. all spaces, or
		//any other character where the trim target IS that character, 
		//nIndex will be set to 0xffffffff on return from find_last_not_of.
		//...this should equal std::basic_string<TCHAR>::npos
		//in this case, return the empty string.
		*this = _T("");
		return;
	}



	_ASSERTE(nIndex  < static_cast<unsigned int>(GetLength()));

	*this = Left(static_cast<int>(nIndex) + 1);
}

// remove continuous occcurrences of characters in passed string,
// starting from right
inline void CString::TrimRight(LPCTSTR lpszTargets)
{
	if(lpszTargets)
	{
		size_t nIndex = m_str.find_last_not_of(lpszTargets);
		if(nIndex == std::basic_string<TCHAR>::npos)
		{
			//if the string contains ONLY the trim target (i.e. all spaces, or
			//any other character where the trim target IS that character, 
			//nIndex will be set to 0xffffffff on return from find_last_not_of.
			//...this should equal std::basic_string<TCHAR>::npos
			//in this case, return the empty string.
		
			*this = _T("");
			return;
		}

		_ASSERTE(nIndex < static_cast<unsigned int>(GetLength()));

		*this = Left(static_cast<int>(nIndex) + 1);
	}
}

// remove continuous occurrences of chTarget starting from left
inline void CString::TrimLeft(TCHAR chTarget)
{
	size_t nIndex = m_str.find_first_not_of(chTarget);
	if(nIndex == std::basic_string<TCHAR>::npos)
	{
		//...as for the TrimRight routines, 
		//if the string contains ONLY the trim target (i.e. all spaces, or
		//any other character where the trim target IS that character, 
		//nIndex will be set to 0xffffffff on return from find_first_not_of.
		//...this should equal std::basic_string<TCHAR>::npos
		//in this case, return the empty string.
	
		*this = _T("");
		return;
	}

	*this = Right(GetLength() - static_cast<int>(nIndex));
}

// remove continuous occcurrences of characters in
// passed string, starting from left
inline void CString::TrimLeft(LPCTSTR lpszTargets)
{
	if(lpszTargets)
	{
		size_t nIndex = m_str.find_first_not_of(lpszTargets);
		if(nIndex == std::basic_string<TCHAR>::npos)
		{
			//...as for the TrimRight routines, 
			//if the string contains ONLY the trim target (i.e. all spaces, or
			//any other character where the trim target IS that character, 
			//nIndex will be set to 0xffffffff on return from find_first_not_of.
			//...this should equal std::basic_string<TCHAR>::npos
			//in this case, return the empty string.
		
			*this = _T("");

			return;
		}

		*this = Right(GetLength() - static_cast<int>(nIndex));
	}
}

// advanced manipulation

// replace occurrences of chOld with chNew
inline int CString::Replace(TCHAR chOld, TCHAR chNew)
{
	int nCount = 0;
	if(chOld == chNew)
	{
		int nMatch = Find(chOld);
		while(nMatch != -1)
		{
			SetAt(nMatch, chNew);
			nCount++;
			nMatch = Find(chOld, nMatch + 1);
		}
	}
	return nCount;
}

// replace occurrences of substring lpszOld with lpszNew;
// empty lpszNew removes instances of lpszOld
inline int CString::Replace(LPCTSTR lpszOld, LPCTSTR lpszNew)
{
	_ASSERTE(lpszOld);
	LPCTSTR lpszReplace = _T("");
	if(lpszNew)
	{
		lpszReplace = lpszNew;
	}
	int nCount = 0;
	if(_tcslen(lpszOld) > 0 && _tcscmp(lpszOld, lpszReplace) != 0)
	{
		int nNewLength = (int)_tcslen(lpszReplace);
		int nOldLength = (int)_tcslen(lpszOld);
		int nMatch = Find(lpszOld);
		while(nMatch != -1)
		{
			Delete(nMatch, nOldLength);
			Insert(nMatch, lpszReplace);
			nCount++;
			nMatch = Find(lpszOld, nMatch + nNewLength);
		}
	}
	return nCount;
}

// remove occurrences of chRemove
inline int CString::Remove(TCHAR chRemove)
{
	int nCount = 0;
	int nMatch = Find(chRemove);
	while(nMatch != -1)
	{
		Delete(nMatch);
		nCount++;
		nMatch = Find(chRemove, nMatch);
	}
	return nCount;
}

// insert character at zero-based index; concatenates
// if index is past end of string
inline int CString::Insert(int nIndex, TCHAR ch)
{
	TCHAR chs[2] = {ch, '\0'};
	return Insert(nIndex, &chs[0]);
}

// insert substring at zero-based index; concatenates
// if index is past end of string
inline int CString::Insert(int nIndex, LPCTSTR lpsz)
{
	if(nIndex < 0)
	{
		nIndex = 0;
	}

	if(lpsz)
	{
		if(nIndex >= GetLength())
		{
			// Append
			*this += lpsz;
		}
		else
		{
			// Insert
			m_str.insert(nIndex, lpsz);
			m_lpszData = m_str.c_str();
		}
	}

	return GetLength();
}

// delete nCount characters starting at zero-based index
inline int CString::Delete(int nIndex, int nCount /* = 1 */)
{
	if(nIndex < 0)
	{
		nIndex = 0;
	}

	int nLength = GetLength();

	if(nIndex > nLength)
	{
		nIndex = nLength;
	}

	if(nIndex + nCount > nLength)
	{
		nCount = nLength - nIndex;
	}

	_ASSERTE(nIndex <= nLength);
	_ASSERTE(nIndex + nCount <= nLength);
	_ASSERTE(nIndex >= 0 && nCount >= 0);

	if(nCount > 0)
	{
		m_str.erase(nIndex, nCount);
		m_lpszData = m_str.c_str();
	}

	return GetLength();
}

// searching

// find character starting at left, -1 if not found
inline int CString::Find(TCHAR ch) const
{
	return Find(ch, 0);
}

// find character starting at right
inline int CString::ReverseFind(TCHAR ch) const
{
	unsigned int nMatch = (UINT)m_str.find_last_of(ch);
	return (nMatch == std::basic_string<TCHAR>::npos) ? -1 : nMatch;
}

// find character starting at zero-based index and going right
inline int CString::Find(TCHAR ch, int nStart) const
{
	if(nStart < 0 || nStart >= GetLength())
	{
		return -1;
	}

	unsigned int nMatch = (UINT)m_str.find(ch, nStart);

	return (nMatch == std::basic_string<TCHAR>::npos) ? -1 : nMatch;
}

// find first instance of any character in passed string
inline int CString::FindOneOf(LPCTSTR lpszCharSet) const
{
	_ASSERTE(lpszCharSet);
	unsigned int nMatch = (UINT)m_str.find_first_of(lpszCharSet);
	return (nMatch == std::basic_string<TCHAR>::npos) ? -1 : nMatch;
}

// find first instance of substring
inline int CString::Find(LPCTSTR lpszSub) const
{
	return Find(lpszSub, 0);
}

// find first instance of substring starting at zero-based index
inline int CString::Find(LPCTSTR lpszSub, int nStart) const
{
	_ASSERTE(lpszSub);
	if(nStart < 0 || nStart > GetLength())
	{
		return -1;
	}

	unsigned int nMatch = (UINT)m_str.find(lpszSub, nStart);
	return (nMatch == std::basic_string<TCHAR>::npos) ? -1 : nMatch;
}

// simple formatting

// printf-like formatting using passed string
inline void __cdecl CString::Format(LPCTSTR lpszFormat, ...)
{
	_ASSERTE((lpszFormat));

	va_list argList;
	va_start(argList, lpszFormat);
	FormatV(lpszFormat, argList);
	va_end(argList);
}

// printf-like formatting using referenced string resource
inline void __cdecl CString::Format(UINT nFormatID, ...)
{
	CString strFormat;
	if(strFormat.LoadString(nFormatID))
	{
		va_list argList;
		va_start(argList, nFormatID);
		FormatV(strFormat, argList);
		va_end(argList);
	}
}

// printf-like formatting using variable arguments parameter
inline
void CString::FormatV(LPCTSTR lpszFormat, va_list argList)
{
	// Empty this string
	Empty();

	_ASSERTE(lpszFormat);
	int nLength = (int)_tcslen(lpszFormat);
	if(nLength == 0)
	{
		// We're done
		return;
	}

	// Default buffer size 256K, see below
	int nNewBufferSize = 128;

	LPTSTR lpszNewBuffer = NULL;
	int nFormat = -1; 

	// Loop until all of our text fits in the buffer
	while(nFormat == -1)
	{
		delete[] lpszNewBuffer;
		nNewBufferSize *= 2;
		lpszNewBuffer = new TCHAR[nNewBufferSize];
		nFormat = _vsntprintf(lpszNewBuffer, nNewBufferSize, lpszFormat, argList);
	}

	if(nFormat > 0)
	{
		*this = lpszNewBuffer;
	}

	delete[] lpszNewBuffer;
}

// formatting for localization (uses FormatMessage API)

// format using FormatMessage API on passed string
inline void __cdecl CString::FormatMessage(LPCTSTR lpszFormat, ...)
{
	// format message into temporary buffer lpszTemp
	va_list argList;
	va_start(argList, lpszFormat);
	LPTSTR lpszTemp;

	if (::FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER,
		lpszFormat, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0 ||
		lpszTemp == NULL)
	{
		// Need to throw a memory exception:
		#ifdef _MFC_VER
				AfxThrowMemoryException();
		#else
				_ASSERTE(false);
		#endif
	}

	// assign lpszTemp into the resulting string and free the temporary
	*this = lpszTemp;
	LocalFree(lpszTemp);
	va_end(argList);
}

// format using FormatMessage API on referenced string resource
inline void __cdecl CString::FormatMessage(UINT nFormatID, ...)
{
	// get format string from string table
	CString strFormat;
	if(strFormat.LoadString(nFormatID))
	{
		// format message into temporary buffer lpszTemp
		va_list argList;
		va_start(argList, nFormatID);
		LPTSTR lpszTemp;
		if (::FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER,
			strFormat, 0, 0, (LPTSTR)&lpszTemp, 0, &argList) == 0 ||
			lpszTemp == NULL)
		{
			// Need to throw a memory exception:
			#ifdef _MFC_VER
					AfxThrowMemoryException();
			#else
					_ASSERTE(false);
			#endif
		}

		// assign lpszTemp into the resulting string and free lpszTemp
		*this = lpszTemp;
		LocalFree(lpszTemp);
		va_end(argList);
	}
}

// load from string resource
inline bool CString::LoadString(UINT nID)
{
	// Max size 512 from resource
	TCHAR chBuf[512];
	int nLen = ::LoadString(::GetModuleHandle(NULL), nID, &chBuf[0], 512);
	if(nLen == 0)
	{
		return false;
	}

	Empty();
	Insert(0, &chBuf[0]);

	return true;
}

#ifndef _UNICODE
// ANSI <-> OEM support (convert string in place)
// convert string from ANSI to OEM in-place
inline void CString::AnsiToOem()
{
	LPSTR lpszBuf = const_cast<LPSTR>(m_str.c_str());
	_ASSERTE(lpszBuf == m_str.data());
	::AnsiToOem(lpszBuf, lpszBuf);
}

// convert string from OEM to ANSI in-place
inline void CString::OemToAnsi()
{
	LPSTR lpszBuf = const_cast<LPSTR>(m_str.c_str());
	_ASSERTE(lpszBuf == m_str.data());
	::OemToAnsi(lpszBuf, lpszBuf);
}
#endif //_UNICODE

// OLE BSTR support (use for OLE automation)

// return a BSTR initialized with this CString's data
inline BSTR CString::AllocSysString() const
{
	USES_CONVERSION;

	LPCWSTR lpszBSTR = T2CW((LPCTSTR)*this);
	_ASSERTE(lpszBSTR);
	BSTR bstr = ::SysAllocStringLen(lpszBSTR, (UINT)wcslen(lpszBSTR));
	// memory exception
	_ASSERTE(bstr);
	return bstr;
}

// reallocates the passed BSTR, copies content of this CString to it
inline BSTR CString::SetSysString(BSTR* pbstr) const
{
	_ASSERTE(pbstr);

	USES_CONVERSION;

	LPCWSTR lpszBSTR = T2CW((LPCTSTR)*this);
	_ASSERTE(lpszBSTR);

	if(!::SysReAllocStringLen(pbstr, lpszBSTR, (UINT)wcslen(lpszBSTR)))
	{
		_ASSERTE(false);
	}
	_ASSERTE(*pbstr);
	return *pbstr;
}

// Access to string implementation buffer as "C" character array

// get pointer to modifiable buffer at least as long as nMinBufLength
inline LPTSTR CString::GetBuffer(int nMinBufLength)
{
	_ASSERTE(nMinBufLength >= 0);
	return m_str.get_buffer(nMinBufLength);
}

// release buffer, setting length to nNewLength (or to first nul if -1)
inline void CString::ReleaseBuffer(int /*nNewLength  = -1 */)
{
	m_str.release_buffer();
}

// get pointer to modifiable buffer exactly as long as nNewLength
inline LPTSTR CString::GetBufferSetLength(int nNewLength)
{
	_ASSERTE(nNewLength >= 0);
	return m_str.get_buffer_set_length(nNewLength);
}

// release memory allocated to but unused by string
inline void CString::FreeExtra()
{
	m_str.resize(GetLength());
	m_lpszData = m_str.c_str();
}

// Use LockBuffer/UnlockBuffer to turn refcounting off
// These just get the buffer, and do nothing on unlock

// turn refcounting back on
inline LPTSTR CString::LockBuffer()
{
	// return a pointer to a modifiable
	// NULL terminated buffer
	LPTSTR lpszBuf = const_cast<LPTSTR>(m_str.c_str());
	_ASSERTE(lpszBuf == m_str.data());
	return lpszBuf;
}

// turn refcounting off
inline void CString::UnlockBuffer()
{
	// No-op.
}

inline int CString::GetAllocLength() const
{
	return (int)m_str.capacity();
}

inline bool __stdcall operator==(const CString& s1, const CString& s2)
	{ return s1.Compare(s2) == 0; }
inline bool __stdcall operator==(const CString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) == 0; }
inline bool __stdcall operator==(LPCTSTR s1, const CString& s2)
	{ return s2.Compare(s1) == 0; }
inline bool __stdcall operator!=(const CString& s1, const CString& s2)
	{ return s1.Compare(s2) != 0; }
inline bool __stdcall operator!=(const CString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) != 0; }
inline bool __stdcall operator!=(LPCTSTR s1, const CString& s2)
	{ return s2.Compare(s1) != 0; }
inline bool __stdcall operator<(const CString& s1, const CString& s2)
	{ return s1.Compare(s2) < 0; }
inline bool __stdcall operator<(const CString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) < 0; }
inline bool __stdcall operator<(LPCTSTR s1, const CString& s2)
	{ return s2.Compare(s1) > 0; }
inline bool __stdcall operator>(const CString& s1, const CString& s2)
	{ return s1.Compare(s2) > 0; }
inline bool __stdcall operator>(const CString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) > 0; }
inline bool __stdcall operator>(LPCTSTR s1, const CString& s2)
	{ return s2.Compare(s1) < 0; }
inline bool __stdcall operator<=(const CString& s1, const CString& s2)
	{ return s1.Compare(s2) <= 0; }
inline bool __stdcall operator<=(const CString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) <= 0; }
inline bool __stdcall operator<=(LPCTSTR s1, const CString& s2)
	{ return s2.Compare(s1) >= 0; }
inline bool __stdcall operator>=(const CString& s1, const CString& s2)
	{ return s1.Compare(s2) >= 0; }
inline bool __stdcall operator>=(const CString& s1, LPCTSTR s2)
	{ return s1.Compare(s2) >= 0; }
inline bool __stdcall operator>=(LPCTSTR s1, const CString& s2)
	{ return s2.Compare(s1) <= 0; }

}	// namespace stingray::foundation
}   // namespace stingray

#endif
