
// XrefFind.cpp: implementation of the CXrefFind class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xref.h"
#include "XrefFind.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXrefFind::CXrefFind()
{

}

CXrefFind::CXrefFind(CXref *pXref)
{
	ASSERT(pXref);
	m_pXref = pXref;
	m_VariableName = STRING_NO_FOUND;
	m_VariableScope= STRING_NO_FOUND;
	m_VariableType= STRING_NO_FOUND;;
	m_FileScopeType= STRING_NO_FOUND;;
	m_IsGlobalDefiniton = FALSE;
	m_bWrite = FALSE;
	m_bRead = FALSE;

}

CXrefFind::~CXrefFind()
{

}

BOOL CXrefFind::Build(const CString &sName, const CString &sScope, const CString &sType, const CString &FileName)
{
	if (sScope == szAll)
		m_VariableScope = STRING_ANY;
	else
		m_VariableScope = m_pXref->GetScopes()->FindItem(sScope);

	if (sType == szAll)
		m_VariableType = STRING_ANY;
	else
		m_VariableType = m_pXref->GetTypes()->FindItem(sType);

	if (FileName == szAll)
		m_FileScopeType = STRING_ANY;
	else
		m_FileScopeType = m_pXref->GetFiles()->FindItem(FileName);

	if (sName == szAny)
	{
		m_VariableName = STRING_ANY;
		m_bIsPartial = FALSE;
	}
	else
	{
		int iIndexStar;
		iIndexStar = sName.Find(chSTAR);
		if (iIndexStar != -1)
		{
			// Use partial match
			m_PartialMatchString = sName.Left(iIndexStar);
			m_bIsPartial = TRUE;
			m_iLenPartial = iIndexStar;
		}
		else
		{
			m_VariableName = m_pXref->GetNames()->FindItem(sName);
			m_bIsPartial = FALSE;
		}
	}

	return TRUE;



}

BOOL CXrefFind::MatchName(StringId NameToMatch)
{
	if (!m_bIsPartial)
	{
		if (m_VariableName == STRING_ANY)
		{
			return TRUE;
		}
		else
		{
			return (m_VariableName == NameToMatch);
		}
	}
	else
	{
		return (!m_PartialMatchString.CompareNoCase((m_pXref->GetNames()->GetItem(NameToMatch)).Left(m_iLenPartial)));
	}

}
