//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOCmn                              |
//                                                                            |
//  Filename    : SOCmnString.h                                               |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : String class                                                |
//                - SOCmnString: String class                                 |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOCMNSTRING_H_
#define _SOCMNSTRING_H_

#pragma pack(push,4)

//-----------------------------------------------------------------------------
// SOCmnString                                                                |
//-----------------------------------------------------------------------------

class SOCMN_EXPORT SOCmnString
{
public:
	SOCmnString();
	SOCmnString(IN TCHAR c);
	SOCmnString(IN LPCTSTR sz);
	SOCmnString(IN const SOCmnString *str);
	SOCmnString(IN const SOCmnString &str);

	~SOCmnString();

	operator LPCTSTR() const;	// get string buffer
	operator LPTSTR();			// get string buffer

	// add string to the existing string
	const SOCmnString& operator+=(IN const SOCmnString &str);
	const SOCmnString& operator+=(IN LPCTSTR sz);
	const SOCmnString& operator+=(IN TCHAR c);

	// set string data
	const SOCmnString& operator=(IN const SOCmnString& str);
	const SOCmnString& operator=(IN LPCTSTR sz);
	const SOCmnString& operator=(IN TCHAR c);

	// add string to another string
	SOCmnString operator+(IN const SOCmnString& str) const;
	SOCmnString operator+(IN LPCTSTR sz) const;
	SOCmnString operator+(IN TCHAR c) const;

	// get length of the string
	DWORD GetLength(void) const;
	DWORD getLength(void) const;

	// check if string is empty
	BOOL IsEmpty(void) const;
	BOOL isEmpty(void) const;

	// empty string
	void Empty(void);
	void empty(void);

	// set string with format
	void Format(IN LPCTSTR format, ...);
	LPCTSTR format(IN LPCTSTR format, ...);
	
	// replace indicated characters with other characters
	int Replace(IN LPCTSTR pszOld, IN LPCTSTR pszNew);
	int replace(IN LPCTSTR pszOld, IN LPCTSTR pszNew);

	void MakeLower(void);
	void makeLower(IN OPTIONAL DWORD startIdx = 0);
	void MakeUpper(void);
	void makeUpper(IN OPTIONAL DWORD startIdx = 0);

	// get string buffer
	LPTSTR GetBuffer(IN DWORD minBufferLen); 
	LPTSTR getBuffer(IN DWORD minBufferLen); 

	// check if two strings are equal
	static BOOL areEqual(IN LPCTSTR sz1, IN LPCTSTR sz2);

	// search in the string
	int Find(IN TCHAR ch, IN OPTIONAL int start = 0) const;
	int find(IN TCHAR ch, IN OPTIONAL int start = 0) const;
	int Find(IN LPCTSTR pszSub, IN OPTIONAL int start = 0) const;
	int find(IN LPCTSTR pszSub, IN OPTIONAL int start = 0) const;

	int FindOneOf(LPCTSTR pszCharset);
	int findOneOf(LPCTSTR pszCharset);


	// create wide character string
	LPWSTR cloneToWCHAR(void);
	static LPWSTR cloneToWCHAR(IN SOCmnString &str);

  // Init from WCHAR
  void fromWCHAR(LPCWSTR wszSrc);

protected:
	TCHAR *m_string;	// string data

	void setData(IN LPTSTR data);
	LPTSTR addData(IN LPCTSTR data, IN DWORD numChars) const;
}; // SOCmnString

inline SOCmnString::operator LPCTSTR() const
{ return m_string; }

inline SOCmnString::operator LPTSTR()
{ return m_string; }


bool operator==(const SOCmnString& s1, const SOCmnString& s2);
bool operator==(const SOCmnString& s1, LPCTSTR s2);
bool operator==(LPCTSTR s1, const SOCmnString& s2);
bool operator!=(const SOCmnString& s1, const SOCmnString& s2);
bool operator!=(const SOCmnString& s1, LPCTSTR s2);
bool operator!=(LPCTSTR s1, const SOCmnString& s2);

inline bool operator==(const SOCmnString& s1, const SOCmnString& s2)
{ return (SOCmnString::areEqual((LPCTSTR)s1, (LPCTSTR)s2) == TRUE); }

inline bool operator==(const SOCmnString& s1, LPCTSTR s2)
{ return (SOCmnString::areEqual((LPCTSTR)s1, s2) == TRUE); }

inline bool operator==(LPCTSTR s1, const SOCmnString& s2)
{ return (SOCmnString::areEqual(s1, (LPCTSTR)s2) == TRUE); }

inline bool operator!=(const SOCmnString& s1, const SOCmnString& s2)
{ return (SOCmnString::areEqual((LPCTSTR)s1, (LPCTSTR)s2) == FALSE); }

inline bool operator!=(const SOCmnString& s1, LPCTSTR s2)
{ return (SOCmnString::areEqual((LPCTSTR)s1, s2) == FALSE); }

inline bool operator!=(LPCTSTR s1, const SOCmnString& s2)
{ return (SOCmnString::areEqual(s1, (LPCTSTR)s2) == FALSE); }

inline DWORD SOCmnString::GetLength(void) const
{ return getLength(); }

inline BOOL SOCmnString::IsEmpty(void) const
{ return isEmpty(); }

inline int SOCmnString::Replace(IN LPCTSTR pszOld, IN LPCTSTR pszNew)
{ return replace(pszOld, pszNew); }

inline void SOCmnString::MakeLower(void)
{ makeLower(); }

inline void SOCmnString::MakeUpper(void)
{ makeUpper(); }

inline void SOCmnString::Empty(void)
{ empty(); }

inline LPTSTR SOCmnString::GetBuffer(IN DWORD minBufferLen)
{ return getBuffer(minBufferLen); }

inline int SOCmnString::Find(TCHAR ch, int start) const
{ return find(ch, start); };

// find a sub-string (like strstr)
inline int SOCmnString::Find(LPCTSTR pszSub, int start) const
{ return find(pszSub, start); };

inline int SOCmnString::FindOneOf(LPCTSTR pszCharset)
{ return findOneOf(pszCharset); }

#pragma pack(pop)
#endif
