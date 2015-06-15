// XrefFind.h: interface for the CXrefFind class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XREFFIND_H__09AB1C49_818D_11D5_9E9A_00D0B77F92EB__INCLUDED_)
#define AFX_XREFFIND_H__09AB1C49_818D_11D5_9E9A_00D0B77F92EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXrefFind : public CObject  
{
public:
	BOOL Build(const CString &sName, const CString &sScope, const CString &sType, const CString &FileName);
	CXrefFind(CXref *pXref);
	BOOL MatchName(StringId NameToMatch);
	virtual ~CXrefFind();
	StringId	m_VariableName;
	StringId	m_VariableScope;
	StringId	m_VariableType;
	StringId	m_FileScopeType;
	CXref *		m_pXref;
	BOOL		m_IsGlobalDefiniton;
	BOOL		m_bWrite;
	BOOL		m_bRead;
	CString		m_PartialMatchString;
	BOOL		m_bIsPartial;
	int			m_iLenPartial;
protected:
	CXrefFind();

};

#endif // !defined(AFX_XREFFIND_H__09AB1C49_818D_11D5_9E9A_00D0B77F92EB__INCLUDED_)
