#if !defined(AFX_CEWATCHTYPE_H__D6848BD6_EC03_11D2_A00E_00A024363DFC__INCLUDED_)
#define AFX_CEWATCHTYPE_H__D6848BD6_EC03_11D2_A00E_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "baselib.h"

#define CEWATCH_BASETYPE_PROJECT        0
#define CEWATCH_BASETYPE_CONFIGURATION  1
#define CEWATCH_BASETYPE_RESOURCE       2
#define CEWATCH_BASETYPE_TASK           3
#define CEWATCH_BASETYPE_PROGRAM        4
#define CEWATCH_BASETYPE_STEP           5
#define CEWATCH_BASETYPE_ACTION         6
#define CEWATCH_BASETYPE_TRANSITION     7
#define CEWATCH_BASETYPE_FUNCTIONBLOCK  8
#define CEWATCH_BASETYPE_ARRAY          9
#define CEWATCH_BASETYPE_STRUCTURE     10
#define CEWATCH_BASETYPE_SIMPLE        11


typedef enum tagVarKind
{	
	NoVar	    = 0,
	Var	        = 1,
	VarInput    = 2,
	VarOutput   = 3,
	VarInOut    = 4,
	VarGlobal   = 5,
	VarExternal = 6,
	Const	    = 7,
} ECeWatchVarKind;


class CCeWatchType  
{
public:
	CCeWatchType();
	virtual ~CCeWatchType();

	bool SetSimpleType(const CString& strName);
	bool SetArrayType(const CString& strName, long iArrayLower, long iArrayUpper);
	bool Set(int iBaseType, const CString& strName);
	bool SetByName(const CString& strName);
    bool SetUnknown();

	void Init();

	const CString& GetName() const { return m_strName; }
	CString GetArrayTypeName() const;
	inline bool IsVarType() const { return m_iBaseType >= CEWATCH_BASETYPE_ARRAY; }
	inline bool IsIECType() const { return GetIECType() >= 0; }
	inline int GetIECType() const { return m_IECType; }
	inline bool IsSimpleType() const { return m_iBaseType == CEWATCH_BASETYPE_SIMPLE; }
	inline bool IsStructure() const { return m_iBaseType == CEWATCH_BASETYPE_STRUCTURE; }
	inline bool IsArray() const { return m_iBaseType == CEWATCH_BASETYPE_ARRAY; }
	inline bool IsFunctionBlock() const { return m_iBaseType == CEWATCH_BASETYPE_FUNCTIONBLOCK; }
	inline bool IsProgram() const { return m_iBaseType == CEWATCH_BASETYPE_PROGRAM; }
	inline bool IsStep() const { return m_iBaseType == CEWATCH_BASETYPE_STEP; }
	inline bool IsAction() const { return m_iBaseType == CEWATCH_BASETYPE_ACTION; }
	inline bool IsTransition() const { return m_iBaseType == CEWATCH_BASETYPE_TRANSITION; }
	inline bool IsTask() const { return m_iBaseType == CEWATCH_BASETYPE_TASK; }
	inline bool IsResource() const { return m_iBaseType ==  CEWATCH_BASETYPE_RESOURCE; }
	inline bool IsConfiguration() const { return m_iBaseType == CEWATCH_BASETYPE_CONFIGURATION; }
	inline bool IsProject() const { return m_iBaseType == CEWATCH_BASETYPE_PROJECT; }
	inline unsigned GetBaseType() const { return m_iBaseType; }

	inline long GetArrayUpperBound() const { ASSERT(IsArray()); return m_ArrayUpper; }
	inline long GetArrayLowerBound() const { ASSERT(IsArray()); return m_ArrayLower; }

	inline void SetVarKind(ECeWatchVarKind iKind) { m_VarKind = iKind; }
	inline ECeWatchVarKind GetVarKind() const { return m_VarKind; }

private:
	CString     m_strName;
	short       m_IECType;
	short       m_IECModifier;
	short       m_iBaseType;
	long        m_ArrayLower; // >= 0 if array
	long        m_ArrayUpper; // >= 0 if array

	ECeWatchVarKind m_VarKind;
};

#endif // !defined(AFX_CEWATCHTYPE_H__D6848BD6_EC03_11D2_A00E_00A024363DFC__INCLUDED_)
