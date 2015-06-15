// XrefFileScope.h: interface for the CXrefFileScope class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XREFFILESCOPE_H__53021809_75C9_11D5_9E83_00D0B77F92EB__INCLUDED_)
#define AFX_XREFFILESCOPE_H__53021809_75C9_11D5_9E83_00D0B77F92EB__INCLUDED_

#include "SortedStringMap.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXref;
class CXrefFind;

class CXrefFileScope : public CObList  
{
friend class CXrefItem;
public:
	CXrefFileScope();
	virtual ~CXrefFileScope();

	DECLARE_SERIAL(CXrefFileScope)
	void Serialize( CArchive& archive );

	BOOL ReadFromFile(ifstream &StreamIn,CXref &XrefScope);
	BOOL FindDefinitions(CStringArray &DefinitionList,CXrefFind &FindScope);
	CXrefDefinition * FindDefinition(CXrefFind &FindScope);
	StringId MatchLocation(const CString &sLocation);
	BOOL FindReferences(CStringArray &DefinitionList,CXrefFind &FindScope);

	// OSAI-Feature 22.03.04 SIS >>
	BOOL FindDefs(CSortedStringMap& rastrDefs, CXrefFind &FindScope);
	BOOL FindRefs(CSortedStringMap& rastrRefs, CXrefFind &FindScope);
	// OSAI-Feature 22.03.04 SIS <<
	
	StringId GetScopeNameId();

	CString GetLocation();

protected:
	CXrefItem *FindItem(StringId ItemName);
	int	m_iVersion;
	StringId m_ScopeName;
	int	m_iDate;
	CString	m_sGuid;
	CString m_sLanguage;
	CString m_sLocation;

};

#endif // !defined(AFX_XREFFILESCOPE_H__53021809_75C9_11D5_9E83_00D0B77F92EB__INCLUDED_)
