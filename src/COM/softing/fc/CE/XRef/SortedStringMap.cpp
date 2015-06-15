

// SortedStringMap.cpp: implementation of the CSortedStringMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SortedStringMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


istream&  operator >>(istream& is, CStringSep & sItem)
{
	LPTSTR sTempBuffer;
	sTempBuffer = sItem.GetBuffer(1024);
	is.get(sTempBuffer,1024,sItem.m_Seperator);
	sItem.ReleaseBuffer();
	sItem.TrimLeft();
	sItem.TrimRight();
	return is;
}

CStringSep::~CStringSep()
{
}

BOOL IsGlobalName(CString &sName)
{
	if (sName == "VAR_GLOBAL" || sName == "VAR_GLOBAL_RETAIN" || sName == "CONST" )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString CSortedStringMap::sEmpty("");

IMPLEMENT_SERIAL(CSortedStringMap,CObject, 1)

CSortedStringMap::CSortedStringMap() : iGrowSize(1024)
{
	//m_HashStrings.SetSize(iGrowSize,iGrowSize);
	//m_SortedStrings.SetSize(iGrowSize,iGrowSize);
}

CSortedStringMap::~CSortedStringMap()
{

}

int		CSortedStringMap::GetCount()
{
	return m_HashStrings.GetSize();
}

void CSortedStringMap::Serialize( CArchive& archive )
{
    CObject::Serialize(archive);
	m_HashStrings.Serialize(archive);
	m_SortedStrings.Serialize(archive);
}


int		CSortedStringMap::AddItem(const CString &sNewString)
{
	int iPos = FindInsertPosition(sNewString);
	int iIndex = 0;
	if (iPos <= m_HashStrings.GetUpperBound() && m_HashStrings[m_SortedStrings[iPos]].CompareNoCase(sNewString) == 0)
		return m_SortedStrings[iPos];
	else
	{
		//append new element
		iIndex = m_HashStrings.Add(sNewString);
		m_SortedStrings.InsertAt(iPos,iIndex);
		return iIndex; 
	}
}

const CString& CSortedStringMap::GetItem(int iIndex)
{
	if (iIndex < 0 || iIndex > m_HashStrings.GetUpperBound())
		return sEmpty;
	else
		return m_HashStrings[iIndex];
}

const CString& CSortedStringMap::GetSortedItem(int iIndex)
{
	if (iIndex < 0 || iIndex > m_HashStrings.GetUpperBound())
		return sEmpty;
	else
		return m_HashStrings[m_SortedStrings[iIndex]];
}

int		CSortedStringMap::FindItem(const CString &sFindString)
{
	// returns the ID of the given String or STRING_NOT_FOUND if not existing
	if (m_SortedStrings.GetSize() == 0)
		return STRING_NO_FOUND;
	int iPos = FindInsertPosition(sFindString);
	if (iPos <= m_HashStrings.GetUpperBound() && m_HashStrings[m_SortedStrings[iPos]].CompareNoCase(sFindString) == 0)
		return m_SortedStrings[iPos];
	else
		return STRING_NO_FOUND;

}


int		CSortedStringMap::FindInsertPosition(const CString &sFindString)
{
	/*
	finds out an place to insert the string
	if it exists, returns the index og the current element
	*/
	if (m_SortedStrings.GetSize() == 0)
		return 0;
	int iLowerBound = 0;
	int iUpperBound = m_SortedStrings.GetUpperBound();
	int iCurrentIndex = 0;
	while (iLowerBound < iUpperBound)
	{
		int iCompareResult;
		iCurrentIndex = (iLowerBound + iUpperBound) / 2;
		iCompareResult = m_HashStrings[m_SortedStrings[iCurrentIndex]].CompareNoCase(sFindString);
		if (iCompareResult < 0)
		{
			iLowerBound = iCurrentIndex +1;

		}
		else if (iCompareResult > 0)
		{
			iUpperBound = iCurrentIndex - 1;
		}
		else
		{
			return iCurrentIndex;
		}
	}
	if (m_HashStrings[m_SortedStrings[iLowerBound]].CompareNoCase(sFindString) < 0)
		return iLowerBound +1;
	else
		return iLowerBound ;
}
