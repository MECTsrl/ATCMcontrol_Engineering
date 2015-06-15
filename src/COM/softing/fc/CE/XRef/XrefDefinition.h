// XrefDefinition.h: interface for the CXrefItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XREFDEFINITION_H__53021808_75C9_11D5_9E83_00D0B77F92EB__INCLUDED_)
#define AFX_XREFDEFINITION_H__53021808_75C9_11D5_9E83_00D0B77F92EB__INCLUDED_

#include "SortedStringMap.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CXrefItem : public CObList  
{
public:
	CXrefItem();
	CXrefItem(StringId CurrFileScope,CXref	*XrefScope) : m_FileScopeType(CurrFileScope),m_XrefScope(XrefScope) {;}
	virtual ~CXrefItem();

	DECLARE_SERIAL(CXrefItem)
	virtual void Serialize( CArchive& archive );
	virtual BOOL ReadFromFile(ifstream &StreamIn);
	virtual void Dump( CDumpContext& dc ) const;
	BOOL FindDefinitions(CStringArray &DefinitionList,CXrefFind &FindScope);
	virtual	BOOL FindReferences(CStringArray &DefinitionList,CXrefFind &FindScope,CString &m_sLocation);

	// OSAI-Feature 22.03.04 SIS >>
	BOOL FindDefs(CSortedStringMap& rastrDefs,CXrefFind &FindScope, const CString& crstrLocation);
	virtual	BOOL FindRefs(CSortedStringMap& rastrRefs, CXrefFind &FindScope, CString &m_sLocation);
	// OSAI-Feature 22.03.04 SIS <<

	BOOL IsCorrectName(StringId ItemName){ return ItemName == m_VariableName;}

	StringId GetVariableNameId();
	StringId GetVariableScopeId();
	StringId GetVariableTypeId();
	StringId GetFileScopeTypeId();
	CString GetEditorPosition();

protected:
	StringId	m_VariableName;
	StringId	m_VariableScope;
	StringId	m_VariableType;
	StringId	m_FileScopeType;
	CString		m_sEditorPosition;
	CXref	*	m_XrefScope;
};

class CXrefDefinition : public CXrefItem
{
public :
	CXrefDefinition();
	virtual ~CXrefDefinition();
	CXrefDefinition(StringId CurrFileScope,CXref	*XrefScope) : CXrefItem(CurrFileScope,XrefScope){;}
	virtual void Serialize( CArchive& archive );
	BOOL	IsGlobal(void);
	DECLARE_SERIAL(CXrefDefinition)
};

class CXrefDeclaration : public CXrefItem
{
public :
	CXrefDeclaration();
	virtual ~CXrefDeclaration();
	CXrefDeclaration(StringId CurrFileScope,CXref	*XrefScope) : CXrefItem(CurrFileScope,XrefScope){;}
	virtual void Serialize( CArchive& archive );
	virtual	BOOL FindReferences(CStringArray &DefinitionList,CXrefFind &FindScope,CString &m_sLocation);

	// OSAI-Feature 22.03.04 SIS >>
	virtual	BOOL FindRefs(CSortedStringMap& rstrRefs ,CXrefFind &FindScope,CString &m_sLocation);
	// OSAI-Feature 22.03.04 SIS <<

	DECLARE_SERIAL(CXrefDeclaration)

};

class CXrefReference : public CObject  
{
friend class CXrefItem;
friend class CXrefDefinition;
friend class CXrefDeclaration;
public:
	CXrefReference();
	virtual ~CXrefReference();
	CXrefReference(CXref	*XrefScope);
	DECLARE_SERIAL(CXrefReference)
	virtual void Serialize( CArchive& archive );
	BOOL ReadFromFile(ifstream &StreamIn,CString &sReferenceName);
	//virtual void Dump( CDumpContext& dc ) const;
	//BOOL FindDefinitions(CStringArray &DefinitionList,CXrefFind &FindScope);
	CString &GetEdipos(){ return m_sEditorPosition;}
	CString &GetSuffix(){ return m_sSuffix;}

	BOOL IsWriteAccess();
	BOOL IsReadAccess();

protected:
	CString 	m_sEditorPosition;
	CString		m_sSuffix;
	BOOL		m_bWritten;
	BOOL		m_bRead;
	CXref	*	m_XrefScope;

};


#endif // !defined(AFX_XREFDEFINITION_H__53021808_75C9_11D5_9E83_00D0B77F92EB__INCLUDED_)
