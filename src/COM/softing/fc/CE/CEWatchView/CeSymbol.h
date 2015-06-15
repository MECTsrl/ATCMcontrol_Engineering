#if !defined(AFX_CESYMBOL_H__92C742A6_02C0_11D3_A01E_00A024363DFC__INCLUDED_)
#define AFX_CESYMBOL_H__92C742A6_02C0_11D3_A01E_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// #2149 27.10.04 SIS >>
#define CEWATCH_STD_ARRAY_LEN	5
// #2149 27.10.04 SIS <<


interface IDebugInfo;
interface IProjectDBI;
interface IConfigurationDBI;
interface IResourceDBI;
interface IVariableDBI;
interface ITypeDBI;

#include "CeWatchType.h"


class CCeSymbolData
{
public:
	CCeSymbolData();
	virtual ~CCeSymbolData();

public:
	unsigned long    m_nChildren;
	long             m_iMinIndex;

#define CESYMBOL_ITYPE_PROJECT       0
#define CESYMBOL_ITYPE_CONFIGURATION 1
#define CESYMBOL_ITYPE_RESOURCE      2
#define CESYMBOL_ITYPE_TYPE          3
	short              m_InterFaceType;
	IUnknown*          m_pIUnknown;	

	CString            m_strName;
	CCeWatchType       m_Type;
};

class CCeSymbol
{
public:
	CCeSymbol(bool bRoot=false);
	virtual ~CCeSymbol();
	bool Load(IDebugInfo* pIDebugInfo);

	inline CCeSymbol* GetParent() const { return m_pParent; }
	CString GetName() const;
	CString GetFullName() const;
	CCeSymbol* FindChild(LPCTSTR pName);
	CCeSymbol* FindChildRecursive(LPCTSTR pName);	// 10.10.06 SIS

	const CCeWatchType& GetType() const;
	long GetMinIndex() const;
	unsigned long GetChildrenCount() const;

	CCeSymbol* GetChild(unsigned long iIndex);

	static bool ConvertType(IUnknown* pIType, CCeWatchType &type, long& MinIndex, unsigned long& ChildrenCount);
	
	// #2149 27.10.04 SIS >>
	BOOL GetExpandFlag();
	void SetExpandFlag(BOOL bExpandFlag);
	// #2149 27.10.04 SIS <<

private:
	inline bool IsParentArray() const { return m_pParent != NULL && m_pParent->GetType().IsArray();}

	
	CCeSymbol* _FindChild(LPCTSTR pName, bool bShort);
	CCeSymbolData* GetData();
	bool LoadChildren();
	bool Load(unsigned long iIndex);
	bool InitByType(IUnknown* pIType, long* piMinIndex, unsigned long* piChildrenCount);

	static CString GetTypeName(ITypeDBI* pITypeDBI);
	static ECeWatchVarKind GetVarKind(IVariableDBI* pIVariableDBI);

private:
	long             m_Index;
	CCeSymbol*       m_pParent;
	CCeSymbol*       m_pChildrenArray;
	CCeSymbolData*   m_pData;

	BOOL			m_bExpandFlag;	// #2149 27.10.04 SIS >>
};


#endif // !defined(AFX_CESYMBOL_H__92C742A6_02C0_11D3_A01E_00A024363DFC__INCLUDED_)
