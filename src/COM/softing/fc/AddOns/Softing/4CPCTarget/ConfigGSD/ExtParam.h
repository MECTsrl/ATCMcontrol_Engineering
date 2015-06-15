// ExtParam.h: interface for the ExtParam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXTPARAM_H__54599BD3_43BF_11D2_822A_00104B43B0E0__INCLUDED_)
#define AFX_EXTPARAM_H__54599BD3_43BF_11D2_822A_00104B43B0E0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//---  Typedefs:  ------------------------------------------------------------+
typedef struct _tagNamedEnumValues	{
	DWORD	Value;
	CString szName;
} NamedEnumValues;

enum ParamType	{
	Array = 0,
	Byte,
	Word,
	DWord
};

//---  Class-Definition  -----------------------------------------------------+
class ExtParam : public CObject  
{
public:
	ExtParam();
		// Constructor
	ExtParam(ExtParam const& other);
		// Copy-Constructor
	virtual ~ExtParam();
		// Destructor
	ExtParam const& operator = (ExtParam const & other);
		// Assignement

//---  Inline Functions  -----------------------------------------------------+
	inline void SetByteOffset(DWORD dwOffset);
		// set byteoffset in userparamarray
	inline DWORD GetByteOffset();
		// returns byteoffset in userparamarray
	inline BOOL	IsVariableEnumParam();
		// returns if ExtParam are enumvalue(s)
	inline void SetIfBitArea(BOOL bVal);
		// set if ExtParam are enumvalue(s)
	inline BOOL IsBitArea();
		// returns if ExtParams are a bitarea
    inline void SetParamType(ParamType type);
    inline ParamType GetParamType();
	inline BOOL IsUsed();
		// returns if this ExtParam is used or must use default value
	inline void SetUsed(BOOL bVal);
		// sets the used state of an ExtParam
	inline void SetIfConstant(BOOL bVal);
		// set if ExtParam are enumvalue(s)
	inline BOOL IsConstantParam();
		// returns if ExtParams are constantvalue(s)
	void		 SetDefaultValue(DWORD dwVal);
	inline DWORD GetDefaultValue();
		// set/get default value of ExtParam
	inline void SetBitAreaStart(DWORD dwVal);
	inline DWORD GetBitAreaStart();
		// set/get bitareastart
	inline void SetBitAreaLength(DWORD dwVal);
	inline DWORD GetBitAreaLength();
		// set/get bitarealength
	inline void SetLowerValue(DWORD dwVal);
	inline DWORD GetLowerValue();
		// set/get lower value
	inline void SetUpperValue(DWORD dwVal);
	inline DWORD GetUpperValue();
		// set/get upper value
	void		 SetUsedValue(DWORD dwVal);
	inline DWORD GetUsedValue();
		// set/get used value
	inline CObList* GetNamedEnumList();
		// returns pointer to List of enumerated Values
	inline CByteArray* GetByteArray();
		// returns pointer to array of bytes

//---  Functions  -----------------------------------------------------+
	void SetDescription(CString szDescription);
		// set description of ExtUserPrm
	CString GetDescription();
		// set/get description of parameter
	void AddNamedEnumVal(NamedEnumValues* pElement);
		// adds a named enum value to array
	void SetConstantBytes(unsigned char* pusArray = NULL, DWORD dwLength = 0);
		// set constant bytes of ExtUserParam

    CString GetState();
    BOOL SetState(LPCTSTR &buffer);

// Datamembers
private:
	ParamType	m_ParamType;				// type of parameter, needed for generating Userprm string
	CString		m_szDescription;			// meaning of ExtUserParam
	DWORD		m_dwByteOffset;				// Offset of Parambytes (0-based)
	BOOL		m_bVariableEnumParam;		// TRUE if ExtParam are enumerated values
	BOOL		m_bBitArea;					// TRUE if ExtParam are a bitarea
	BOOL		m_bConstantParam;			// TRUE if ExtParam are constant values
	BOOL		m_bIsUsed;					// sign if Param is used or unused (needed for creating Userprms)

	DWORD		m_dwBitFieldStart;			// bitoffset in byte
	DWORD		m_dwBitFieldLength;			// bitfield length in byte
	DWORD		m_dwLowerVal;				// lower value of ExtParam
	DWORD		m_dwUpperVal;				// upper Value of ExtParam
	DWORD		m_dwDefaultVal;				// default if no constant value
	DWORD		m_dwUsedVal;				// used value of ExtParam
	CByteArray	m_ConstBytes;				// Constant values of ExtParam

	CObList		m_NamedEnumList;			// List of enumerated Values with their names
};

//-----------------------------------------------------------------------------+
//-------------------------------- Inline - Functions -------------------------+
inline void ExtParam::SetByteOffset(DWORD dwOffset)	{
	m_dwByteOffset = dwOffset;
}

inline DWORD ExtParam::GetByteOffset()	{
	return m_dwByteOffset;
}

inline BOOL	ExtParam::IsVariableEnumParam()	{
	return 	m_bVariableEnumParam;
}

inline void ExtParam::SetIfBitArea(BOOL bVal)	{
	m_bBitArea = bVal;
}

inline BOOL ExtParam::IsBitArea()	{
	return m_bBitArea;
}

inline void ExtParam::SetIfConstant(BOOL bVal)	{
	m_bConstantParam = bVal;
}

inline BOOL ExtParam::IsConstantParam()	{
	return m_bConstantParam;
}

inline void ExtParam::SetParamType(ParamType type) {
    m_ParamType = type;
}

inline ParamType ExtParam::GetParamType() {
    return m_ParamType;
}

inline BOOL ExtParam::IsUsed()	{
	return m_bIsUsed;
}

inline void ExtParam::SetUsed(BOOL bVal)	{
	m_bIsUsed = bVal;
}

inline DWORD ExtParam::GetDefaultValue()	{
	return m_dwDefaultVal;
}

inline void ExtParam::SetBitAreaStart(DWORD dwVal)	{
	m_dwBitFieldStart = dwVal;
}

inline DWORD ExtParam::GetBitAreaStart()	{
	return m_dwBitFieldStart;
}

inline void ExtParam::SetBitAreaLength(DWORD dwVal)	{
	m_dwBitFieldLength = dwVal;
}

inline DWORD ExtParam::GetBitAreaLength()	{
	return m_dwBitFieldLength;
}

inline void ExtParam::SetLowerValue(DWORD dwVal)	{
	m_dwLowerVal = dwVal;
}

inline DWORD ExtParam::GetLowerValue()	{
	return m_dwLowerVal;
}

inline void ExtParam::SetUpperValue(DWORD dwVal)	{
	m_dwUpperVal = dwVal;
}

inline DWORD ExtParam::GetUpperValue()	{
	return m_dwUpperVal;
}

inline DWORD ExtParam::GetUsedValue()	{
	return m_dwUsedVal;
}

inline CObList* ExtParam::GetNamedEnumList()	{
	return &m_NamedEnumList;
}

inline CByteArray* ExtParam::GetByteArray()	{
	return &m_ConstBytes;
}


LONG GetNextValue(LPCTSTR &buffer);


#endif // !defined(AFX_EXTPARAM_H__54599BD3_43BF_11D2_822A_00104B43B0E0__INCLUDED_)
