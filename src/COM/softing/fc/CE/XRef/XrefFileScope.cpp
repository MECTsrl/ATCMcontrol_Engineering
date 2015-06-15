

// XrefFileScope.cpp: implementation of the CXrefFileScope class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XrefFileScope.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CXrefFileScope,CObList, 1)

CXrefFileScope::CXrefFileScope()
{

}

CXrefFileScope::~CXrefFileScope()
{
	POSITION pos = GetHeadPosition();
	while (pos != NULL)
		delete GetNext(pos);
}
CXrefItem *CXrefFileScope::FindItem(StringId ItemName)
{
		POSITION pos = GetHeadPosition();
		while (pos != NULL)
		{
			CXrefItem * Item = dynamic_cast<CXrefItem*> (GetNext(pos));
			if (Item->IsCorrectName(ItemName))
			{
				return Item;
			}
		}
		return NULL;
}

void CXrefFileScope::Serialize( CArchive& archive )
{
    CObList::Serialize(archive);
    if (archive.IsStoring())
    {
		archive  << m_iVersion << m_ScopeName << m_iDate << m_sGuid << m_sLanguage << m_sLocation;
    }
    else
    {
		archive  >> m_iVersion >> m_ScopeName >> m_iDate >> m_sGuid >> m_sLanguage >> m_sLocation;
    }
}

BOOL CXrefFileScope::ReadFromFile(ifstream &StreamIn,CXref &XrefScope)
{
	TCHAR		chSep;
	CStringSep	sBuffer;

	while ( StreamIn.get(chSep) && chSep != chBRACKETOPEN)
	{
		StreamIn.putback(chSep);
		StreamIn >> sBuffer >> ws ;
		int iIndexEqual = sBuffer.Find(_T('='));
		if (iIndexEqual)
		{
			CString sItemName = sBuffer.Left(iIndexEqual);
			CString sItemValue = sBuffer.Mid(iIndexEqual+1);
			if (sItemName == _T("VERSION"))
			{
				m_iVersion = _ttoi(sItemValue);
			}
			else if (sItemName == _T("SCOPE"))
			{
				m_ScopeName = XrefScope.m_FileScopeDictionary.AddItem(sItemValue);
			}
			else if (sItemName == _T("DATE"))
			{
				m_iDate = _ttoi(sItemValue);
			}
			else if (sItemName == _T("GUID"))
			{
				m_sGuid = sItemValue;
			}
			else if (sItemName == _T("LANG"))
			{
				m_sLanguage = sItemValue;
			}
			else if (sItemName == _T("LOCATION"))
			{
				m_sLocation = sItemValue;
			}

		}

	}
	StreamIn.putback(chSep);
	// switch to next section..
	while ( StreamIn >> sBuffer >> ws && sBuffer.CompareNoCase(szDefinitionSection))
		;
	while ( StreamIn.get(chSep) && chSep != chBRACKETOPEN)
	{
		StreamIn.putback(chSep);
		CXrefDefinition *pNewDefinition = new CXrefDefinition(m_ScopeName,&XrefScope);
		if (pNewDefinition->ReadFromFile(StreamIn))
		{
			AddTail((CObject*)pNewDefinition);
		}
		else
		{
			delete pNewDefinition;
		}

	}
	StreamIn.putback(chSep);

	while ( StreamIn >> sBuffer >> ws && sBuffer.CompareNoCase(szDeclarationSection))
		;
	while ( StreamIn.get(chSep) && chSep != chBRACKETOPEN)
	{
		StreamIn.putback(chSep);
		CXrefDeclaration *pNewDeclaration = new CXrefDeclaration(m_ScopeName,&XrefScope);
		if (pNewDeclaration->ReadFromFile(StreamIn))
		{
			AddTail((CObject*)pNewDeclaration);
		}
		else
		{
			delete pNewDeclaration;
		}

	}
	StreamIn.putback(chSep);
	while ( StreamIn >> sBuffer >> ws && sBuffer.CompareNoCase(szReferenceSection))
		;
	while ( StreamIn.get(chSep) && chSep != chBRACKETOPEN)
	{
		CString sReferenceName;
		StreamIn.putback(chSep);
		CXrefReference *pNewDeclaration = new CXrefReference(&XrefScope);
		if (pNewDeclaration->ReadFromFile(StreamIn,sReferenceName))
		{
			StringId NameId = XrefScope.m_NameDictionary.FindItem(sReferenceName);
			if (NameId != STRING_NO_FOUND)
			{
				// append to existing Definition or Declaration
				CXrefItem *pItem = FindItem(NameId);
				if (pItem != NULL)
				{
					pItem->AddTail(pNewDeclaration);
				}
				else
				{
					delete pNewDeclaration;
					// Create an empty declaration
				}

				
			}
			else
			{
				// If it is an structured variable, add references to base variable..
				CString sBaseName;
				sBaseName = sReferenceName.SpanExcluding(szDot);
				NameId = XrefScope.m_NameDictionary.FindItem(sBaseName);
				if (NameId != STRING_NO_FOUND)
				{
					// append to existing Definition or Declaration
					CString sSuffix ;
					int iPosDot = sReferenceName.Find(szDot);
					if (iPosDot != -1)
					{
						sSuffix = sReferenceName.Mid(iPosDot+1);
						pNewDeclaration->GetSuffix() = sSuffix;
					}
					CXrefItem *pItem = FindItem(NameId);
					if (pItem != NULL)
					{
						pItem->AddTail(pNewDeclaration);
					}
					else
					{
						delete pNewDeclaration;
						// Create an empty declaration
					}
				}
			}

		}
		else
		{
			delete pNewDeclaration;
		}

	}

	return TRUE;

}

BOOL CXrefFileScope::FindDefinitions(CStringArray &DefinitionList,CXrefFind &FindScope)
{
	if (FindScope.m_FileScopeType == STRING_ANY || FindScope.m_FileScopeType == m_ScopeName)
	{
		POSITION pos = GetHeadPosition();
		while (pos != NULL)
		{
			CXrefItem * Item = dynamic_cast<CXrefItem*> (GetNext(pos));
			if (Item->IsKindOf(RUNTIME_CLASS(CXrefDefinition))|| FindScope.m_FileScopeType == m_ScopeName
				|| ( FindScope.m_FileScopeType == STRING_ANY && FindScope.m_VariableScope != STRING_ANY))
			{
				Item->FindDefinitions(DefinitionList,FindScope);
			}
		}
// recurse on list..
		return TRUE;
	}
	else
		return FALSE;

}

// OSAI-Feature 22.03.04 SIS >>
BOOL CXrefFileScope::FindDefs(CSortedStringMap& rastrDefs, CXrefFind &FindScope)
{
	if (FindScope.m_FileScopeType == STRING_ANY || FindScope.m_FileScopeType == m_ScopeName)
	{
		POSITION pos = GetHeadPosition();
		while (pos != NULL)
		{
			CXrefItem * Item = dynamic_cast<CXrefItem*> (GetNext(pos));
			if (Item->IsKindOf(RUNTIME_CLASS(CXrefDefinition))|| FindScope.m_FileScopeType == m_ScopeName
				|| ( FindScope.m_FileScopeType == STRING_ANY && FindScope.m_VariableScope != STRING_ANY))
			{
				Item->FindDefs(rastrDefs, FindScope, m_sLocation);
			}
		}
// recurse on list..
		return TRUE;
	}
	else
		return FALSE;

}
// OSAI-Feature 22.03.04 SIS <<

CXrefDefinition * CXrefFileScope::FindDefinition(CXrefFind &FindScope)
{
	if (FindScope.m_FileScopeType == m_ScopeName)
	{
		POSITION pos = GetHeadPosition();
		while (pos != NULL)
		{
			CXrefItem * Item = dynamic_cast<CXrefItem*> (GetNext(pos));
			if (Item->IsKindOf(RUNTIME_CLASS(CXrefDefinition)))
			{
				if (Item->IsCorrectName(FindScope.m_VariableName))
				{
					return dynamic_cast<CXrefDefinition*>(Item);
				}
			}
		}

	}
	return NULL;
}

BOOL CXrefFileScope::FindReferences(CStringArray &DefinitionList,CXrefFind &FindScope)
{
	if (FindScope.m_FileScopeType == STRING_ANY || FindScope.m_FileScopeType == m_ScopeName || FindScope.m_IsGlobalDefiniton)
	{
		POSITION pos = GetHeadPosition();
		while (pos != NULL)
		{
			CXrefItem * Item = dynamic_cast<CXrefItem*> (GetNext(pos));
			if (Item)
			{
				Item->FindReferences(DefinitionList,FindScope,m_sLocation);
			}
		}
// recurse on list..
		return TRUE;
	}
	else
		return FALSE;

}


// OSAI-Feature 22.03.04 SIS >>
BOOL CXrefFileScope::FindRefs(CSortedStringMap& rastrRefs, CXrefFind &FindScope)
{
	if (FindScope.m_FileScopeType == STRING_ANY || FindScope.m_FileScopeType == m_ScopeName || FindScope.m_IsGlobalDefiniton)
	{
		POSITION pos = GetHeadPosition();
		while (pos != NULL)
		{
			CXrefItem * Item = dynamic_cast<CXrefItem*> (GetNext(pos));
			if (Item)
			{
				Item->FindRefs(rastrRefs, FindScope, m_sLocation);
			}
		}
// recurse on list..
		return TRUE;
	}
	else
		return FALSE;

}
// OSAI-Feature 22.03.04 SIS <<

CString CXrefFileScope::GetLocation()
{
	return m_sLocation;
}

StringId CXrefFileScope::GetScopeNameId()
{
	return m_ScopeName;
}


StringId CXrefFileScope::MatchLocation(const CString &sLocation)
{
	if (m_sLocation.CompareNoCase(sLocation) == 0)
		return m_ScopeName;
	else
		return STRING_NO_FOUND;
}