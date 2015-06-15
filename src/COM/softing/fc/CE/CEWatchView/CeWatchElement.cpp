

#include "stdafx.h"
#include "resource.h"
#include "CeWatchElement.h"
#include "CeWatchBackEnd.h"
#include "CESysDef.h"
#include "DbgInfo.h"
#include "CeSymbol.h"
#include "XMLHelper/XMLNodeList.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//*****************************************************************************	
CCeWatchElement::CCeWatchElement()
//*****************************************************************************	
{
	m_pParent = NULL;
	m_Level = 0;
	m_pRoot = this;
    m_bInitExpanded = false;
	Init();
}

//*****************************************************************************	
CCeWatchElement::CCeWatchElement(LPCTSTR pName, const CCeWatchType& Type)
//*****************************************************************************	
{
	SetParent(NULL);

	m_pRoot = this;
    m_bInitExpanded = false;

	Init();
	// 14.12.05 SIS >>
	// remove array range from name
	// otherwise it will be shown twice
	CString	strName(pName);
	int iFound = strName.Find(_T(", ["));
	if(iFound != -1)
	{
		m_strIndexRange = strName.Mid(iFound);
		strName = strName.Left(iFound);
	}
	SetName(strName);
	// 14.12.05 SIS <<

	SetType(Type);

    PresetHistory();
}

//*****************************************************************************	
CCeWatchElement::CCeWatchElement(CCeSymbol *pSymbol)
//*****************************************************************************
{
	ASSERT(pSymbol != NULL);
	SetParent(NULL);

	m_pRoot = this;
    m_bInitExpanded = false;

	Init();

	if (pSymbol != NULL)
	{
		SetName(pSymbol->GetFullName());
		SetType(pSymbol->GetType());

		// #2149 27.10.04 SIS >>
		// TODO optimize insertion:
		// do not insert all children when parent is inserted
		// delay until expanded
		unsigned long ulCount = pSymbol->GetChildrenCount();

		for (unsigned long ii = 0; ii < ulCount; ii++)
		// #2149 27.10.04 SIS <<
		{
			CCeSymbol *pChildSymbol = pSymbol->GetChild(ii);
            if (pChildSymbol != NULL && CCeWatchElement::IsVarKindVisible(pChildSymbol->GetType().GetVarKind())) 
			{
				CCeWatchElement* pVar = new CCeWatchElement(pChildSymbol);
				AddChild(pVar);
			}
		}
	}

    PresetHistory();
}

//*****************************************************************************	
CCeWatchElement::~CCeWatchElement()
//*****************************************************************************	
{
	DeleteAllChildren();
	m_DebugId = 0;
}

//*****************************************************************************	
void CCeWatchElement::SetParent(CCeWatchElement* pParent)
//*****************************************************************************	
{
	m_pParent = pParent;	
	if (pParent != NULL)
		m_Level = pParent->GetLevel() + 1;
	else
		m_Level = 0;

	for (int ii = 0; ii < GetChildrenCount(); ii++)
		GetChild(ii)->SetParent(this);
}

//*****************************************************************************	
void CCeWatchElement::Init()
//*****************************************************************************	
{
	DeleteAllChildren();

	m_Name.Empty();
	m_strFormat.Empty();
	m_Type.Init();

	SetQuality(OPC_QUALITY_BAD | OPC_QUALITY_NOT_CONNECTED);
	m_strValue = CCeWatchBackEnd::GetStateText(CEEmptyValue);
	m_strTime.Empty();

	m_HistoryList.RemoveAll();
	
	m_DebugId = 4711;
}

//*****************************************************************************	
void CCeWatchElement::SetType(const CCeWatchType& Type) 
//*****************************************************************************	
{ 
	m_Type = Type;
}

//*****************************************************************************	
CString CCeWatchElement::GetShortName() const
//*****************************************************************************	
{
	int i1 = m_Name.ReverseFind('[');
	int i2 = m_Name.ReverseFind('.') + 1;
	int ii = max(i1, i2);
	if (ii > 0 && ii < m_Name.GetLength())
		return m_Name.Mid(ii);
	else
		return m_Name;
}


//*****************************************************************************	
CCeWatchElement* CCeWatchElement::AddChild(LPCTSTR pChildName, const CCeWatchType& ChildType)
//*****************************************************************************	
{
	ASSERT(AfxIsValidString(pChildName)); 
	
	CCeWatchElement* pChild = new CCeWatchElement(pChildName, ChildType);
	if (AddChild(pChild))
		return pChild;
	else
		return NULL;
}

//*****************************************************************************	
bool CCeWatchElement::AddChild(CCeWatchElement* pChild)
//*****************************************************************************	
{
	ASSERT(IsValidWatchElement(pChild)); 	
	if (pChild != NULL)
	{
		m_ChildrenArray.Add(pChild);
		pChild->m_pRoot = m_pRoot;
		pChild->SetParent(this);
	}
	return true;
}

// 11.10.06 SIS >>
void CCeWatchElement::AddChildren(CCeSymbol* pSymbol)
{
	unsigned long ulCount = pSymbol->GetChildrenCount();

	for (unsigned long ii = 0; ii < ulCount; ii++)
	// #2149 27.10.04 SIS <<
	{
		CCeSymbol *pChildSymbol = pSymbol->GetChild(ii);
        if (pChildSymbol != NULL && CCeWatchElement::IsVarKindVisible(pChildSymbol->GetType().GetVarKind())) 
		{
			CCeWatchElement* pVar = new CCeWatchElement(pChildSymbol);
			AddChild(pVar);
		}
	}
}
// 11.10.06 SIS <<


//*****************************************************************************	
bool CCeWatchElement::SetChild(int ii, CCeWatchElement* pChild)
//*****************************************************************************	
{
	ASSERT(IsValidWatchElement(pChild)); 	
	ASSERT(ii >= 0 && ii < GetChildrenCount());
	if (pChild != NULL && ii >= 0 && ii < GetChildrenCount())
	{
		m_ChildrenArray.SetAt(ii, pChild);
		pChild->SetParent(this);
		pChild->m_pRoot = m_pRoot;
	}
	return true;
}


//*****************************************************************************	
bool CCeWatchElement::DeleteChild(CCeWatchElement* pChild)
//*****************************************************************************	
{
	ASSERT(IsValidWatchElement(pChild)); 
	for (int ii = 0; ii < m_ChildrenArray.GetSize(); ii++)
	{
		if (m_ChildrenArray.GetAt(ii) == pChild)
		{
			delete pChild;
			m_ChildrenArray.RemoveAt(ii);
			return true;
		}
	}
	return false;
}

//*****************************************************************************	
bool CCeWatchElement::DeleteAllChildren()
//*****************************************************************************	
{
	bool rv = m_ChildrenArray.GetSize() > 0;
	for (int ii = 0; ii < m_ChildrenArray.GetSize(); ii++)
	{
		CCeWatchElement* pVar = m_ChildrenArray.GetAt(ii);
		ASSERT(IsValidWatchElement(pVar)); 
		if (pVar != NULL)
			delete pVar;
	}
	m_ChildrenArray.RemoveAll();
	return rv;
}

//*****************************************************************************	
bool IsValidWatchElement(const CCeWatchElement* pElem) 
//*****************************************************************************	
{
	return 
		AfxIsValidAddress(pElem, sizeof(CCeWatchElement)) && 
		(pElem->m_DebugId == 4711);
}


//*****************************************************************************	
bool CCeWatchElement::WriteData(CStdioFile& file) const
//*****************************************************************************	
{
	// don't forget to increment the version string if you change anything here
	bool rv = false;
	CString str;

    str = CEWATCH_EXP_NAME + m_Name + CEWATCH_EXP_CRLF;
	file.WriteString(str);

	if (m_Type.GetName().GetLength() > 0)
	{
		str.Format(";%d", (int)m_Type.GetVarKind());
		file.WriteString(m_Type.GetName() + str);
	}
	file.WriteString(CEWATCH_EXP_CRLF);

    str = CEWATCH_EXP_VAL + m_strValue + CEWATCH_EXP_CRLF;
    file.WriteString(str);

	if (m_strFormat.GetLength() > 0)
		file.WriteString(m_strFormat);
	file.WriteString(CEWATCH_EXP_CRLF);

	str.Format("%d", GetHistoryCount());
    str += CEWATCH_EXP_CRLF;
	file.WriteString(str);
	for (int ii = 0; ii < GetHistoryCount(); ii++)
		file.WriteString(GetHistory(ii) + CEWATCH_EXP_CRLF);

	str.Format("%d", GetChildrenCount());
    str += CEWATCH_EXP_CRLF;

	file.WriteString(str);
	rv = true;
	for (ii = 0; ii < GetChildrenCount() && rv; ii++)
		rv &= GetChild(ii)->WriteData(file);

	return rv;;
}

//*****************************************************************************	
bool CCeWatchElement::ReadData(CStdioFile& file)
//*****************************************************************************	
{
	CString str;
    bool rv = false;
    int iPos = -1;

	Init();

	if (file.ReadString(str))
	{
        iPos = str.Find(CEWATCH_EXP_EQ);
        if(iPos == -1) {
            m_Name = str;
        }
        else {
            m_Name = str.Mid(iPos+1);
        }

        m_Name.TrimLeft();
        m_Name.TrimRight();

		file.ReadString(str);
		CCeWatchType type;
		if (type.SetByName(str))
		{
			SetType(type);

            file.ReadString(str);
            m_strValue = str.Mid(str.Find(_T("=")) + 1);

			file.ReadString(m_strFormat);

			file.ReadString(str);
			int nHistoryCount = atoi(str);			
			if ((nHistoryCount > 0) || (str == "0"))
			{
				for (int ii = 0; ii < nHistoryCount; ii++)
				{
					file.ReadString(str);
					AddToHistory(str);
				}
		
				file.ReadString(str);
				int iChildrenCount = atoi(str);			
				if ((iChildrenCount > 0) || (str == "0"))
				{
					rv = true;
					for (ii = 0; ii < iChildrenCount; ii++)
					{
						CCeWatchElement* pChild = new CCeWatchElement();
						ASSERT(pChild != NULL);
						if (pChild != NULL)
						{
							AddChild(pChild);
							rv &= pChild->ReadData(file);
						}
					}
				}
			}
		}
	}
	if (!rv)
		Init();

	return rv;;
}

// 15.12.05 SIS >>
CString	CCeWatchElement::GetIndexRange()
{
	return m_strIndexRange;
}
// 15.12.05 SIS <<

// 10.10.06 SIS >>
void CCeWatchElement::SetIndexRange(const CString& crsRange)
{
	m_strIndexRange = crsRange;
}

BOOL CCeWatchElement::ExtractArrayIndices(const CString& crsIndexRange, int& riStartIndex, int& riStopIndex)
{
	int		iStart;
	int		iStop;
	CString	sIdx;

	// extract start index
	iStart = crsIndexRange.Find(_T('[')) + 1;
	if(iStart == -1)
	{
		return FALSE;
	}
	iStop = crsIndexRange.Find(_T(".."), iStart);
	if(iStop == -1)
	{
		return FALSE;
	}
	sIdx = crsIndexRange.Mid(iStart, iStop-iStart);
	riStartIndex = _ttoi(sIdx);

	// extract stop index
	iStart = iStop + 2;
	iStop = crsIndexRange.Find(_T(']'));
	if(iStop == -1)
	{
		return FALSE;
	}
	sIdx = crsIndexRange.Mid(iStart, iStop-iStart);
	riStopIndex = _ttoi(sIdx);

	return TRUE;
}
// 11.10.06 SIS >>
BOOL CCeWatchElement::CheckArrayIndices()
{
	int	iStartIndex;
	int iStopIndex;

	if(!ExtractArrayIndices(m_strIndexRange, iStartIndex, iStopIndex))
	{
		return FALSE;
	}
	int iLowerBound = m_Type.GetArrayLowerBound();
	int	iUpperBound = m_Type.GetArrayUpperBound();

	if(iStartIndex < iLowerBound)
	{
		iStopIndex = min((iStopIndex-iStartIndex) + iLowerBound, iUpperBound);
		iStartIndex = iLowerBound;
	}
	if(iStopIndex > iUpperBound)
	{
		iStartIndex = max(iUpperBound - (iStopIndex-iStartIndex), iLowerBound);
		iStopIndex = iUpperBound;
	}
	ASSERT(iStartIndex >= iLowerBound && iStartIndex <= iUpperBound);
	ASSERT(iStopIndex >= iLowerBound && iStopIndex <= iUpperBound);
	ASSERT(iStartIndex <= iStopIndex);
	m_strIndexRange.Format(_T(", [%d..%d]"), iStartIndex, iStopIndex);
	return TRUE;
}
// 10.10.06 SIS <<

bool CCeWatchElement::LoadFromNode(CXMLNode& rtNode, CCeSymbol* pRootSymbol)
{
    bool bReturn = true;
	CString str;
    int iPos = -1;

	Init();

    rtNode.GetAttribute(CEWATCH_XMLATTR_EXPRESSION, str);
    iPos = str.Find(CEWATCH_EXP_EQ);
    if(iPos == -1) 
    {
        m_Name = str;
    }
    else
    {
        m_Name = str.Mid(iPos+1);
    }
	// 15.12.05 SIS >>
	// remove array range from name
	// otherwise it will be shown twice
	int iFound = m_Name.Find(_T(", ["));
	if(iFound != -1)
	{
		m_strIndexRange = m_Name.Mid(iFound);
		m_Name = m_Name.Left(iFound);
	}
	// 14.12.05 SIS <<

    m_Name.TrimLeft();
    m_Name.TrimRight();

    rtNode.GetAttribute(CEWATCH_XMLATTR_TYPE, str);

	CCeWatchType type;
	if (type.SetByName(str))
	{
		SetType(type);

        PresetHistory();

        //last value - don't read it
        rtNode.GetAttribute(CEWATCH_XMLATTR_VALUE, m_strValue);
        rtNode.GetAttribute(CEWATCH_XMLATTR_FORMAT, m_strFormat);
        rtNode.GetAttribute(CEWATCH_XMLATTR_EXPANDED, str);
        if(atoi(str) == 1)
        {
            m_bInitExpanded = true;
        }
		
        CXMLNodeList    tChildList;
        CXMLNode        tNodeChild;
		CCeWatchElement* pChild = NULL;

        if(rtNode.GetChildNodeList(tChildList))
        {
            long lNumChildren = tChildList.GetLength();
            for(long lChild = 0; lChild < lNumChildren; ++lChild)
            {
                tChildList.Item(lChild, tNodeChild);
				pChild = new CCeWatchElement();
				AddChild(pChild);
                bReturn &= pChild->LoadFromNode(tNodeChild, pRootSymbol);
            }
		}
	}
	if(!bReturn)
    {
		Init();
    }
    return bReturn;
}

// 15.12.05 SIS >>
// added parameter crstrIndex
// to save index range
bool CCeWatchElement::SaveToNode(CXMLNode& rtNode, CXMLDocument& rtDocument, const CString& crstrIndexRange)
// 15.12.05 SIS <<
{
    bool bReturn = true;
	// don't forget to increment the version string if you change anything here
	CString str;

    if(m_bInitExpanded)
    {
        rtNode.SetAttribute(CEWATCH_XMLATTR_EXPANDED, _T("1"));
    }

	CString strExpression = m_Name;
	if(!crstrIndexRange.IsEmpty())
	{
		strExpression.Format(_T("%s, %s"), m_Name, crstrIndexRange);
	}
	rtNode.SetAttribute(CEWATCH_XMLATTR_EXPRESSION, strExpression);

	if (m_Type.GetName().GetLength() > 0)
	{
		str.Format("%s;%d", m_Type.GetName(), (int)m_Type.GetVarKind());
        rtNode.SetAttribute(CEWATCH_XMLATTR_TYPE, str);
	}
    rtNode.SetAttribute(CEWATCH_XMLATTR_VALUE, m_strValue);

	if (m_strFormat.GetLength() > 0)
    {
        rtNode.SetAttribute(CEWATCH_XMLATTR_FORMAT, m_strFormat);
    }

	// #2149 10.10.06 SIS >>
	// only save visible array elements
    int iNumChildren = GetChildrenCount();
	int	iChildStart = 0;
	int iChildStop = iNumChildren-1;
	if(m_Type.IsArray())
	{
		VERIFY(ExtractArrayIndices(crstrIndexRange, iChildStart, iChildStop));
		iChildStart = max(iChildStart - m_Type.GetArrayLowerBound(), 0);
		iChildStop = min(iChildStop - m_Type.GetArrayLowerBound(), iNumChildren-1);
	}
	CCeWatchElement* pElem = NULL;
	for (int iChild = iChildStart; iChild <= iChildStop; iChild++)
	// #2149 10.10.06 SIS <<
    {
        CXMLNode    tNodeChild;
        if(rtDocument.CreateNode(tNodeChild, CEWATCH_XMLTAG_VARNODE))
        {
			pElem = GetChild(iChild);
			if(!pElem)
			{
				break;
			}
		    bReturn &= pElem->SaveToNode(tNodeChild, rtDocument, _T(""));
            if(!bReturn)
            {
                break;
            }
            rtNode.AppendChild(tNodeChild);
        }
    }

    return bReturn;
}


//*****************************************************************************	
void CCeWatchElement::AddToHistory(const CString& value)
//*****************************************************************************	
{
    CString valNormal = value;


 	// insert new element at the beginning
    if(m_Type.GetIECType() == BL_BOOL)
    {
        valNormal.MakeLower();
        valNormal.TrimLeft();
        valNormal.TrimRight();
        valNormal.Replace(_T("bool#"), _T(""));
            
        if(!value.CompareNoCase(_T("0")) || value.CompareNoCase(_T("false")))
            valNormal =  _T("False");
        else if(value.CompareNoCase(_T("1")) || value.CompareNoCase(_T("true")))
            valNormal =  _T("True");
        else
            valNormal =  value;
    }
    else
    {
        valNormal = value;
    }

	m_HistoryList.InsertAt(0, valNormal);

	// remove identical elements
	int n = m_HistoryList.GetSize();
	for (int ii = n-1; ii > 0; ii--)
	{
		if (valNormal == m_HistoryList.GetAt(ii))
			m_HistoryList.RemoveAt(ii);
	}

	// limit number of entries to CEWATCH_MAX_HISTORY
	if (m_HistoryList.GetSize() > CEWATCH_MAX_HISTORY)
		m_HistoryList.SetSize(CEWATCH_MAX_HISTORY);

	m_HistoryList.FreeExtra();
}

//*****************************************************************************	
void CCeWatchElement::SetQuality(short Quality)
//*****************************************************************************	
{
	m_iQuality = Quality; 	
	m_strQuality = CCeWatchBackEnd::GetQualityText(m_iQuality);
}

//*****************************************************************************	
bool CCeWatchElement::IsConnected() const 
//*****************************************************************************	
{ 
	if ((m_iQuality & OPC_QUALITY_MASK) == OPC_QUALITY_BAD)
		return (m_iQuality & OPC_STATUS_MASK) != OPC_QUALITY_NOT_CONNECTED;
	else
		return true;
}

void CCeWatchElement::PresetHistory()
{
    HRESULT hr = S_OK;
	OLECHAR szBuffer[100];
    VARIANT vt;
    CString str;
    
    if(m_Type.IsSimpleType() && m_Type.GetIECType() == BL_BOOL) {
        ::VariantInit(&vt);
	    vt.vt = VT_BOOL;
	    vt.boolVal = VARIANT_TRUE;

	    HRESULT hr = BL_VariantToString (&vt, BL_BOOL, NULL, BL_USE_WINDOWS_FORMAT, szBuffer, FC_ARRAY_LEN(szBuffer));
        if (FAILED (hr)) {
            return;
        }

        str = szBuffer;
        AddToHistory(str);

        vt.boolVal = VARIANT_FALSE;

	    hr = BL_VariantToString (&vt, BL_BOOL, NULL, BL_USE_WINDOWS_FORMAT, szBuffer, FC_ARRAY_LEN(szBuffer));
        if (FAILED (hr)) {
            return;
        }

        str = szBuffer;
        AddToHistory(str);

    }
    
}

//*****************************************************************************	
bool CCeWatchElement::IsVarKindVisible(ECeWatchVarKind varKind)
//*****************************************************************************	
{
    switch(varKind)
    {
    case NoVar:     // non variable nodes are visible
    case Var:
    case VarInput:
    case VarOutput:
    case VarGlobal:
        return true;
    }
    return false;
}