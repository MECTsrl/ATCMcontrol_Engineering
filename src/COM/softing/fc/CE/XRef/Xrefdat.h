// Xref.h: interface for the CXref class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XREF_H__53021804_75C9_11D5_9E83_00D0B77F92EB__INCLUDED_)
#define AFX_XREF_H__53021804_75C9_11D5_9E83_00D0B77F92EB__INCLUDED_

#include "SortedStringMap.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXref : public CObject  
{
friend class CXrefFileScope;
friend class CXrefItem;

public:
	BOOL GetFileScopeByLocation(CString sLocation,CString &sFilescope);
	CXref();
	virtual ~CXref();

	DECLARE_SERIAL(CXref)
	void Serialize( CArchive& archive );
	void Dump( CDumpContext& dc ) const;
	BOOL SaveToFile(CString &sDirectory);
	BOOL LoadFromFile(CString &sDirectory);
	CSortedStringMap *GetNames(void);
	CSortedStringMap *GetTypes(void);
	CSortedStringMap *GetScopes(void);
	CSortedStringMap *GetFiles(void);
	BOOL FindDefinitions(CStringArray &DefinitionList, const CString &sName, const CString &sScope, const CString &sType, const CString &sFileName);
	BOOL FindReferences(CStringArray &DeclarationList, const CString &sName, BOOL bWrite, BOOL bRead);

	// OSAI-Feature 22.03.04 SIS >>
	BOOL FindDefs(CSortedStringMap& rastrDefs, const CString &sName, const CString &sScope, const CString &sType, const CString &sFileName);
	BOOL FindRefs(CSortedStringMap& rastrRefs, const CString& crstrName, const CString& crstrFile, BOOL bRead, BOOL bWrite);	// XREFPRINT 21.07.05 SIS
	// OSAI-Feature 22.03.04 SIS <<

	CXrefDefinition * FindDefinition(CXrefFind &FindScope);

	void Clear(void);
	BOOL PerformLink(CString sPathName);
	
	// OSAI feature print XRef 16.03.04 SIS >>
	void GetContentString(CString& rstrContent, int iLineLength);
	// OSAI feature print XRef 16.03.04 SIS <<

	// XREFPRINT 21.07.05 SIS >>
	void GetSelectionString(	CString& rstrContent, 
								int iLineLength, 
								const CString& crstrSelName,
								const CString& crstrSelScope, 
								const CString& crstrSelType, 
								const CString& crstrSelFile, 
								BOOL bRead, 
								BOOL bWrite);
	// XREFPRINT 21.07.05 SIS <<

protected :
	// Data
	CSortedStringMap m_NameDictionary;
	CSortedStringMap m_TypeDictionary;
	CSortedStringMap m_ScopeDictionary;
	CSortedStringMap m_FileScopeDictionary;
	CObList			 m_Files;
};

// OSAI feature print XRef 24.03.04 SIS >>
CString GetColumnString(const CString& crstrText, int iLen, BOOL bCutFromLeft = FALSE, TCHAR cFill = _T(' '));
CString SplitField(const CString& crstrInput, CString& rstrField);
// OSAI feature print XRef 24.03.04 SIS >>

#endif // !defined(AFX_XREF_H__53021804_75C9_11D5_9E83_00D0B77F92EB__INCLUDED_)
