

// XrefDefinition.cpp: implementation of the CXrefItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XrefDefinition.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CXrefItem,CObList, 1)
IMPLEMENT_SERIAL(CXrefDefinition,CXrefItem, 1)
IMPLEMENT_SERIAL(CXrefDeclaration,CXrefItem, 1)
IMPLEMENT_SERIAL(CXrefReference,CObject, 2)

CXrefItem::CXrefItem()
{

}

CXrefItem::~CXrefItem()
{
	POSITION pos = GetHeadPosition();
	while (pos != NULL)
		delete GetNext(pos);

}

void CXrefItem::Serialize( CArchive& archive )
{
    CObList::Serialize(archive);

    if (archive.IsStoring())
    {
		archive  << m_VariableName << m_VariableScope << m_VariableType << m_FileScopeType << m_sEditorPosition << m_XrefScope;
		//archive  << m_VariableName << m_VariableScope << m_VariableType << m_sEditorPosition << m_FileScopeType;

    }
    else
    {
		archive  >> m_VariableName >> m_VariableScope >> m_VariableType >> m_FileScopeType >> m_sEditorPosition >> m_XrefScope;
		//archive  >> m_VariableName >> m_VariableScope >> m_VariableType  >> m_sEditorPosition >> m_FileScopeType;
    }

}
void CXrefItem::Dump( CDumpContext& dc ) const
{
	dc << 
		"  NameIndex :" << m_VariableName <<
		"  VariableScope :" << m_VariableScope <<
		"  VariableType :" << m_VariableType <<
		"  FileScope :" << m_FileScopeType <<
		"  Editor Position :" << m_sEditorPosition;

}

BOOL CXrefItem::ReadFromFile(ifstream &StreamIn)
{
	CStringSep sName(chSEMICOLON);
	CStringSep sScope(chSEMICOLON);
	CStringSep sType(chSEMICOLON);
	CStringSep sEditpos;
	char		chSep;


	StreamIn >> sName >> chSep >> sScope >> chSep >> sType >> chSep >> sEditpos >> ws;
	if(sName.IsEmpty())
		return FALSE;
		
	m_VariableName = m_XrefScope->m_NameDictionary.AddItem(sName);
	m_VariableScope = m_XrefScope->m_ScopeDictionary.AddItem(sScope);
	m_VariableType = m_XrefScope->m_TypeDictionary.AddItem(sType);
	m_sEditorPosition = sEditpos;
	return TRUE;
}

BOOL CXrefItem::FindDefinitions(CStringArray &DefinitionList,CXrefFind &FindScope)
{
	BOOL fIsMatching = TRUE;
	CString sMatchingDefinition;
	if (!FindScope.MatchName (m_VariableName))
		fIsMatching = FALSE;
	if (FindScope.m_VariableScope != STRING_ANY && FindScope.m_VariableScope != m_VariableScope)
		fIsMatching = FALSE;
	if (FindScope.m_VariableType != STRING_ANY && FindScope.m_VariableType != m_VariableType)
		fIsMatching = FALSE;
	if (fIsMatching)
	{
		sMatchingDefinition = FindScope.m_pXref->m_NameDictionary.GetItem(m_VariableName);
		sMatchingDefinition += "\t";
		sMatchingDefinition += FindScope.m_pXref->m_FileScopeDictionary.GetItem(m_FileScopeType);
		DefinitionList.Add(sMatchingDefinition);
	}
	return fIsMatching;
}

// OSAI-Feature 22.03.04 SIS >>
BOOL CXrefItem::FindDefs(CSortedStringMap& rastrDefs, CXrefFind &FindScope, const CString& crstrLocation)
{
	BOOL fIsMatching = TRUE;
	CString sMatchingDefinition;
	if (!FindScope.MatchName (m_VariableName))
		fIsMatching = FALSE;
	if (FindScope.m_VariableScope != STRING_ANY && FindScope.m_VariableScope != m_VariableScope)
		fIsMatching = FALSE;
	if (FindScope.m_VariableType != STRING_ANY && FindScope.m_VariableType != m_VariableType)
		fIsMatching = FALSE;
	if (fIsMatching)
	{
		sMatchingDefinition = FindScope.m_pXref->m_NameDictionary.GetItem(m_VariableName);
		sMatchingDefinition += "\t";
		sMatchingDefinition += FindScope.m_pXref->m_FileScopeDictionary.GetItem(m_FileScopeType);
		sMatchingDefinition += "\t";
		sMatchingDefinition += FindScope.m_pXref->m_TypeDictionary.GetItem(m_VariableType);
		sMatchingDefinition += "\t";
        sMatchingDefinition += crstrLocation + m_sEditorPosition;
        sMatchingDefinition += "\t";        // no access
		rastrDefs.AddItem(sMatchingDefinition);
	}
	return fIsMatching;
}
// OSAI-Feature 22.03.04 SIS <<

BOOL CXrefItem::FindReferences(CStringArray &DefinitionList,CXrefFind &FindScope,CString &sLocation)
{
	BOOL fIsMatching = TRUE;
	CString sMatchingDefinition;
	if (FindScope.m_VariableName != STRING_ANY && FindScope.m_VariableName != m_VariableName)
		fIsMatching = FALSE;
	if (FindScope.m_VariableScope != STRING_ANY && FindScope.m_VariableScope != m_VariableScope)
		fIsMatching = FALSE;
	if (FindScope.m_VariableType != STRING_ANY && FindScope.m_VariableType != m_VariableType)
		fIsMatching = FALSE;
	if (fIsMatching)
	{
		sMatchingDefinition = sLocation;
		sMatchingDefinition += m_sEditorPosition;
		DefinitionList.Add(sMatchingDefinition);
		if (FindScope.m_bRead || FindScope.m_bWrite)
		{
			POSITION pos = GetHeadPosition();
			while (pos != NULL)
			{
				CXrefReference * Item = dynamic_cast<CXrefReference*> (GetNext(pos));
				if (Item && (Item->m_bRead == FindScope.m_bRead || Item->m_bWritten == FindScope.m_bWrite))
				{
					// adding the subname if available
					sMatchingDefinition = Item->GetSuffix();
					if (!sMatchingDefinition.IsEmpty())
						sMatchingDefinition += _T(" :");
					sMatchingDefinition += sLocation;
					sMatchingDefinition += Item->GetEdipos();
					DefinitionList.Add(sMatchingDefinition);
				}
			}
		}
	}
	return fIsMatching;
}


// OSAI-Feature 22.03.04 SIS >>
BOOL CXrefItem::FindRefs(CSortedStringMap& rastrRefs, CXrefFind &FindScope, CString &sLocation)
{
	BOOL fIsMatching = TRUE;
	CString sMatchingDefinition;
	if (FindScope.m_VariableName != STRING_ANY && FindScope.m_VariableName != m_VariableName)
		fIsMatching = FALSE;
	if (FindScope.m_VariableScope != STRING_ANY && FindScope.m_VariableScope != m_VariableScope)
		fIsMatching = FALSE;
	if (FindScope.m_VariableType != STRING_ANY && FindScope.m_VariableType != m_VariableType)
		fIsMatching = FALSE;
	if (fIsMatching)
	{
//		sMatchingDefinition = sLocation;
//		sMatchingDefinition += m_sEditorPosition;
//      sMatchingDefinition += _T("\t");
//		DefinitionList.Add(sMatchingDefinition);
		if (FindScope.m_bRead || FindScope.m_bWrite)
		{
			POSITION pos = GetHeadPosition();
			while (pos != NULL)
			{
				CXrefReference * Item = dynamic_cast<CXrefReference*> (GetNext(pos));
				if (Item && (Item->m_bRead == FindScope.m_bRead || Item->m_bWritten == FindScope.m_bWrite))
				{
					// adding the subname if available
					sMatchingDefinition = Item->GetSuffix();
					if (!sMatchingDefinition.IsEmpty())
						sMatchingDefinition += _T(" :");
					sMatchingDefinition += sLocation;
					sMatchingDefinition += Item->GetEdipos();
                    CString strAccess;
                    if(Item->m_bRead)
                    {
                        strAccess = _T("R");
                    }
                    if(Item->m_bWritten)
                    {
                        strAccess += _T("W");
                    }
                    sMatchingDefinition += _T("\t") + strAccess;
					rastrRefs.AddItem(sMatchingDefinition);
				}
			}
		}
	}
	return fIsMatching;
}
// OSAI-Feature 22.03.04 SIS <<

StringId CXrefItem::GetVariableNameId() 
{
	return m_VariableName; 
}

StringId CXrefItem::GetVariableScopeId() 
{
	return m_VariableScope; 
}

StringId CXrefItem::GetVariableTypeId() 
{
	return m_VariableType; 
}

StringId CXrefItem::GetFileScopeTypeId() 
{
	return m_FileScopeType; 
}

CString CXrefItem::GetEditorPosition() 
{
	return m_sEditorPosition; 
}


CXrefDefinition::CXrefDefinition()
{

}

CXrefDefinition::~CXrefDefinition()
{

}
void CXrefDefinition::Serialize( CArchive& archive )
{
	CXrefItem::Serialize( archive );
}

BOOL	CXrefDefinition::IsGlobal(void)
{
	CString sVariableScope;
	sVariableScope = m_XrefScope->GetScopes()->GetItem(m_VariableScope);
	return IsGlobalName(sVariableScope);
}



CXrefDeclaration::CXrefDeclaration()
{

}

CXrefDeclaration::~CXrefDeclaration()
{

}
void CXrefDeclaration::Serialize( CArchive& archive )
{
	CXrefItem::Serialize( archive );
}

BOOL CXrefDeclaration::FindReferences(CStringArray &DefinitionList,CXrefFind &FindScope,CString &sLocation)
{
	BOOL fIsMatching = TRUE;
	CString sMatchingDefinition;
	if (FindScope.m_VariableName != STRING_ANY && FindScope.m_VariableName != m_VariableName)
		fIsMatching = FALSE;
	if (!FindScope.m_IsGlobalDefiniton && FindScope.m_VariableScope != STRING_ANY && FindScope.m_VariableScope != m_VariableScope)
		fIsMatching = FALSE;
	if (FindScope.m_VariableType != STRING_ANY && FindScope.m_VariableType != m_VariableType)
		fIsMatching = FALSE;
	if (fIsMatching)
	{
		sMatchingDefinition = sLocation;
		sMatchingDefinition += m_sEditorPosition;
		DefinitionList.Add(sMatchingDefinition);
		if (FindScope.m_bRead || FindScope.m_bWrite)
		{
			POSITION pos = GetHeadPosition();
			while (pos != NULL)
			{
				CXrefReference * Item = dynamic_cast<CXrefReference*> (GetNext(pos));
				if (Item && (Item->m_bRead == FindScope.m_bRead || Item->m_bWritten == FindScope.m_bWrite))
				{
					// adding the subname if available
					sMatchingDefinition = Item->GetSuffix();
					if (!sMatchingDefinition.IsEmpty())
						sMatchingDefinition += _T(" :");
					sMatchingDefinition += sLocation;
					sMatchingDefinition += Item->GetEdipos();
					DefinitionList.Add(sMatchingDefinition);

				}
			}
		}
	}
	return fIsMatching;

}


// OSAI-Feature 22.03.04 SIS >>
//------------------------------------------------------------------*
/**
 * FindRefs
 *    search for references and add information to string array rastrRefs
 *    each string consists of a location part and a an access part
 *    separator is a tab ('\t')
 *
 * @param           rastrRefs: reference information (in/out)
 * @param           FindScope: find scope (in)
 * @param           sLocation: location string (in)
 * @return          matching?
 * @exception       -
 * @see             -
*/
BOOL CXrefDeclaration::FindRefs(CSortedStringMap& rastrRefs,CXrefFind &FindScope, CString &sLocation)
{
	BOOL fIsMatching = TRUE;
	CString sMatchingDefinition;
	if (FindScope.m_VariableName != STRING_ANY && FindScope.m_VariableName != m_VariableName)
		fIsMatching = FALSE;
	if (!FindScope.m_IsGlobalDefiniton && FindScope.m_VariableScope != STRING_ANY && FindScope.m_VariableScope != m_VariableScope)
		fIsMatching = FALSE;
	if (FindScope.m_VariableType != STRING_ANY && FindScope.m_VariableType != m_VariableType)
		fIsMatching = FALSE;
	if (fIsMatching)
	{
		sMatchingDefinition = sLocation;
		sMatchingDefinition += m_sEditorPosition;
        sMatchingDefinition += _T("\t");
		rastrRefs.AddItem(sMatchingDefinition);
		if (FindScope.m_bRead || FindScope.m_bWrite)
		{
			POSITION pos = GetHeadPosition();
			while (pos != NULL)
			{
				CXrefReference * Item = dynamic_cast<CXrefReference*> (GetNext(pos));
				if (Item && (Item->m_bRead == FindScope.m_bRead || Item->m_bWritten == FindScope.m_bWrite))
				{
					// adding the subname if available
					sMatchingDefinition = Item->GetSuffix();
					if (!sMatchingDefinition.IsEmpty())
						sMatchingDefinition += _T(" :");
					sMatchingDefinition += sLocation;
					sMatchingDefinition += Item->GetEdipos();
                    CString strAccess;
                    if(Item->m_bRead)
                    {
                        strAccess = _T("R");
                    }
                    if(Item->m_bWritten)
                    {
                        strAccess += _T("W");
                    }
                    sMatchingDefinition += _T("\t") + strAccess;
					rastrRefs.AddItem(sMatchingDefinition);
				}
			}
		}
	}
	return fIsMatching;
}
// OSAI-Feature 22.03.04 SIS <<


CXrefReference::CXrefReference()
{
	m_bWritten = FALSE;
	m_bRead = FALSE;
}
CXrefReference::CXrefReference(CXref	*XrefScope):m_XrefScope(XrefScope) 
{
	m_bWritten = FALSE;
	m_bRead = FALSE;
}

CXrefReference::~CXrefReference()
{

}

void CXrefReference::Serialize( CArchive& archive )
{
    CObject::Serialize(archive);
    if (archive.IsStoring())
    {
		archive  << m_sEditorPosition << m_sSuffix << m_bWritten << m_bRead ;
    }
    else
    {
		archive  >> m_sEditorPosition >> m_sSuffix >> m_bWritten >> m_bRead ;
	}

}

BOOL CXrefReference::ReadFromFile(ifstream &StreamIn,CString &sReferenceName)
{
	CStringSep sName(chSEMICOLON);
	CStringSep sAccess(chSEMICOLON);
	CStringSep sEditpos;
	char		chSep;


	StreamIn >> sName >> chSep >> sAccess >> chSep >> sEditpos >> ws;
	if(sName.IsEmpty())
		return FALSE;
		
	sReferenceName = sName;
	m_sEditorPosition = sEditpos;
	if (sAccess == "READ")
	{
		m_bRead = TRUE;
	}
	else if (sAccess == "WRITE")
	{
		m_bWritten = TRUE;
	}
	else if (sAccess == "READWRITE")
	{
		m_bRead = TRUE;
		m_bWritten = TRUE;
	}

	return TRUE;

}

BOOL CXrefReference::IsWriteAccess()
{
	return m_bWritten;
}

BOOL CXrefReference::IsReadAccess()
{
	return m_bRead;
}
