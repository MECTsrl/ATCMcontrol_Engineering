
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


#include "stdafx.h"
#include "dbginfo_i.c"
#include "watchexprman.h"


extern CExpressionFactory *CEGetExpressionFactory ();


CExpressionHolder::CExpressionHolder (CComPtr<IDebugInfo> pDebugInfo, long lHandle)
: CExpressionNode (pDebugInfo),
  m_lHandle (lHandle),
  m_lClientData (0),
  m_ItemState (CEGoodWatchItem),
  m_pRoot (NULL),
  m_pChangedCB (NULL),
  m_pFormatInfo (NULL),
  m_pRanges (NULL),
  m_bRootSet (false),
  m_FormatState (CEGoodFormat),
  m_RangesState (CEGoodRanges)
{
}


CExpressionHolder::~CExpressionHolder ()
{
    DisposeNodes (NULL);

    delete m_pFormatInfo;
    delete m_pRanges;
}


void CExpressionHolder::AddNode (CExpressionNode *pNode)
{
    m_Nodes.AddTail (pNode);
    pNode->AddParent (this);
}


void CExpressionHolder::SetChangedCB (FC_CALLBACK_BASE (ExpressionChangedCB) *pChangedCB)
{
    m_pChangedCB = pChangedCB;
}


void CExpressionHolder::SetText (const CString &strText, CEWatchItemState state)
{
    m_strItemText = strText;
    m_strItemText.TrimLeft ();
    m_strItemText.TrimRight ();

    m_ItemState = state;
}


void CExpressionHolder::SetExpression (const CString &strText, CExpressionNode *pRoot)
{
    m_strExpressionText = strText;
    m_strExpressionText.TrimLeft ();
    m_strExpressionText.TrimRight ();

    ASSERT (!m_bRootSet);
    if (m_bRootSet)
    {
        return;
    };

    DisposeNodes (pRoot);

    if (pRoot == NULL)
    {
        return;
    };


    // Now the root node must be the only one we are parent of.

    ASSERT (m_Nodes.GetCount () == 1 && pRoot == m_Nodes.GetHead ());

    m_pRoot = pRoot;
    m_pType = pRoot->GetType ();
}


void CExpressionHolder::SetFormat (const CString &strText, BL_FormatInfo *pFormatInfo)
{
    m_strFormatText = strText;
    m_strFormatText.TrimLeft ();
    m_strFormatText.TrimRight ();

    if (pFormatInfo != NULL)
    {
        delete m_pFormatInfo;
        m_pFormatInfo = new BL_FormatInfo (*pFormatInfo);
    };
}


void CExpressionHolder::SetRanges (const CString &strText, const CArray<Range, const Range &> *pRanges)
{
    int iIndex;

    m_strRangesText = strText;
    m_strRangesText.TrimLeft ();
    m_strRangesText.TrimRight ();

    if (pRanges != NULL)
    {
        m_pRanges = new CArray<Range, const Range &>;

        for (iIndex = 0; iIndex < pRanges->GetSize (); ++iIndex)
        {
            m_pRanges->Add (pRanges->GetAt (iIndex));
        };
    };
}


long CExpressionHolder::GetHandle ()
{
    return (m_lHandle);
}


CExpressionNode *CExpressionHolder::GetRoot ()
{
    return (m_pRoot);
}


long CExpressionHolder::GetClientData ()
{
    return (m_lClientData);
}


void CExpressionHolder::SetClientData (long lClientData)
{
    m_lClientData = lClientData;
}


CString CExpressionHolder::GetItemText ()
{
    return (m_strItemText);
}


CEWatchItemState CExpressionHolder::GetItemState ()
{
    return (m_ItemState);
}


CString CExpressionHolder::GetExpressionText ()
{
    return (m_strExpressionText);
}


CEExpressionState CExpressionHolder::GetExpressionState ()
{
    return (m_State);
}


CString CExpressionHolder::GetFormatText ()
{
    return (m_strFormatText);
}


CEFormatState CExpressionHolder::GetFormatState ()
{
    return (m_FormatState);
}


CENumberBase CExpressionHolder::GetFormatBase ()
{
    CENumberBase base=CEBase10;

    if (m_pFormatInfo == NULL || m_pFormatInfo->m_Kind != BL_IntegerFormat || m_pFormatInfo->m_iRadix == -1)
    {
        return (CEBase10);
    };

    switch (m_pFormatInfo->m_iRadix)
    {
    case 10: base = CEBase10; break;
    case 16: base = CEBase16; break;
    default: base = CEBase10; break;
    };

    return (base);
}


CString CExpressionHolder::GetRangesText ()
{
    return (m_strRangesText);
}


CERangesState CExpressionHolder::GetRangesState ()
{
    return (m_RangesState);
}


HRESULT CExpressionHolder::GetRangeCount (unsigned long &ulCount)
{
    if (m_pRanges == NULL)
    {
        ulCount = 0;
        return (S_FALSE);
    };

    ulCount = m_pRanges->GetSize ();
    return (S_OK);
}


HRESULT CExpressionHolder::GetRange (unsigned long ulIndex, Range &range)
{
    if (m_pRanges == NULL)
    {
        return (E_FAIL);
    };

    if (ulIndex >= (unsigned long)m_pRanges->GetSize ())
    {
        return (E_FAIL);
    };

    range = m_pRanges->GetAt (ulIndex);
    return (S_OK);
}


unsigned long CExpressionHolder::GetSubItemCount ()
{
    CComPtr<ITypeDBI> pElementType;
    long lLowerBound;
    long lUpperBound;

    if (m_ItemState != CEGoodWatchItem)
    {
        return (0);
    };

    if (m_pType == NULL)
    {
        return (0);
    };

    if (m_pType != NULL && IsArrayType (pElementType, lLowerBound, lUpperBound))
    {
        return (GetArraySubItemCount ());
    };

    return (0);
}


unsigned long CExpressionHolder::GetArraySubItemCount ()
{
    CComPtr<ITypeDBI> pElementType;
    long lLowerBound;
    long lUpperBound;
    unsigned long ulCount=0;
    long lIndex;

    ASSERT (m_pType != NULL && IsArrayType (pElementType, lLowerBound, lUpperBound));
    if (m_pType == NULL || !IsArrayType (pElementType, lLowerBound, lUpperBound))
    {
        return (0);
    };

    if (m_pRanges == NULL)
    {
        return (lUpperBound - lLowerBound + 1);
    };

    for (lIndex = 0; lIndex < m_pRanges->GetSize (); ++lIndex)
    {
        Range range;

        range = m_pRanges->GetAt (lIndex);

        if (range.m_lUpper < range.m_lLower)
        {
            ulCount += range.m_lLower - range.m_lUpper + 1;
        }
        else
        {
            ulCount += range.m_lUpper - range.m_lLower + 1;
        };
    };

    return (ulCount);
}


HRESULT CExpressionHolder::GetSubItem (unsigned long ulIndex, CExpressionHolder *pSubItem)
{
    CComPtr<ITypeDBI> pElementType;
    long lLowerBound;
    long lUpperBound;

    if (m_ItemState != CEGoodWatchItem)
    {
        return (E_FAIL);
    };

    if (m_pType == NULL)
    {
        return (E_FAIL);
    };

    if (m_pType != NULL && IsArrayType (pElementType, lLowerBound, lUpperBound))
    {
        return (GetArraySubItem (ulIndex, pSubItem));
    };

    return (E_FAIL);
}


HRESULT CExpressionHolder::GetArraySubItem (unsigned long ulIndex, CExpressionHolder *pSubItem)
{
    CComPtr<ITypeDBI> pElementType;
    long lLowerBound;
    long lUpperBound;
    long lArrayIndex;
    CExpressionLiteral *pIndex;
    CExpressionIndexing *pIndexing;
    CString strText;

    ASSERT (m_pType != NULL && IsArrayType (pElementType, lLowerBound, lUpperBound));
    if (m_pType == NULL || !IsArrayType (pElementType, lLowerBound, lUpperBound))
    {
        return (E_FAIL);
    };

    if (m_pRanges == NULL)
    {
        if (ulIndex > (unsigned long)(lUpperBound - lLowerBound))
        {
            return (S_FALSE);
        };

        lArrayIndex = lLowerBound + ulIndex;
    }
    else
    {
        bool bInside=false;
        long lIndex;

        for (lIndex = 0; lIndex < m_pRanges->GetSize (); ++lIndex)
        {
            Range range;
            long rangeDiff;

            range = m_pRanges->GetAt (lIndex);
            rangeDiff = range.m_lUpper - range.m_lLower;

            if (ulIndex <= (unsigned long)(rangeDiff >= 0 ? rangeDiff : -rangeDiff))
            {
                bInside = true;
                if (rangeDiff < 0)
                {
                    lArrayIndex = range.m_lLower - ulIndex;
                }
                else
                {
                    lArrayIndex = range.m_lLower + ulIndex;
                };
                break;
            };

            ulIndex -= (rangeDiff >= 0 ? rangeDiff : -rangeDiff) + 1;
        };

        if (!bInside)
        {
            return (S_FALSE);
        };
    };

    ASSERT (m_pRoot != NULL);
    if (m_pRoot == NULL)
    {
        return (E_FAIL);
    };

    pIndex = new CExpressionLiteral (m_pDebugInfo, (__int64)lArrayIndex);
    ASSERT (pSubItem != NULL);
    if (pSubItem == NULL)
    {
        return (E_OUTOFMEMORY);
    };
    pSubItem->AddNode (pIndex);

    pIndexing = new CExpressionIndexing (m_pDebugInfo, m_pRoot, pIndex);
    ASSERT (pSubItem != NULL);
    if (pSubItem == NULL)
    {
        return (E_OUTOFMEMORY);
    };
    pSubItem->AddNode (pIndexing);

    if (m_strFormatText.IsEmpty ())
    {
        strText.Format ("%s[%ld]", (LPCTSTR)m_strExpressionText, lArrayIndex);
    }
    else
    {
        strText.Format ("%s[%ld],%s", (LPCTSTR)m_strExpressionText, lArrayIndex, (LPCTSTR)m_strFormatText);
    };

    pSubItem->SetText (strText, CEGoodWatchItem);
    pSubItem->SetExpression (strText, pIndexing);
    pSubItem->SetFormat (m_strFormatText, m_pFormatInfo);

    return (S_OK);
}


HRESULT CExpressionHolder::GetValueText (bool bFormatted, CString &strText)
{
    CComVariant varValue;
    BL_IEC_TYP iecType;

    varValue = GetValue ();

	if (m_pType == NULL)
    {
        return (GuessingConversion (varValue, bFormatted, strText));
    };

    if (!IsSimpleType (iecType))
    {
        strText = "";
        return (S_FALSE);
    };

    return (BaseLibToString (varValue, iecType, bFormatted, strText));
}


HRESULT CExpressionHolder::GuessingConversion (const CComVariant &varValue, bool bFormatted, CString &strValue)
{
	BL_IEC_TYP iecType;

	switch (varValue.vt)
	{
    case VT_BOOL   : iecType = BL_BOOL;  break;
	case VT_I1     : iecType = BL_BYTE;  break;
	case VT_I2     : iecType = BL_WORD;  break;
    case VT_I4     : iecType = BL_DINT;  break;
	case VT_R8     : iecType = BL_LREAL; break;
	case VT_DECIMAL: iecType = BL_LINT;  break;

	default:
        iecType = BL_WSTRING;
        break;
	};

	return (BaseLibToString (varValue, iecType, bFormatted, strValue));
}


HRESULT CExpressionHolder::BaseLibToString (const CComVariant &varValue, BL_IEC_TYP iecType, bool bFormatted, CString &strValue)
{
	HRESULT hr;
	OLECHAR szValue[500];
    CComVariant varTemp;
    USES_CONVERSION;

    if ((iecType == BL_WSTRING || iecType == BL_STRING) && varValue.vt == VT_BSTR && m_strFormatText.IsEmpty())
    {
        strValue = varValue.bstrVal;
        return S_OK;
    }

    varTemp = varValue;   // oops, BL_VariantToString expects non-const pointer
    if (bFormatted)
    {
	    hr = BL_VariantToString (&varTemp, iecType, T2COLE (m_strFormatText), BL_USE_IEC_FORMAT, szValue, sizeof (szValue) / sizeof (OLECHAR));
    }
    else
    {
	    hr = BL_VariantToString (&varTemp, iecType, NULL, BL_USE_IEC_FORMAT, szValue, sizeof (szValue) / sizeof (OLECHAR));
    };

	if (FAILED (hr))
	{
        return (hr);
    };

	strValue = szValue;
	return (S_OK);
}


CEWriteState CExpressionHolder::GetWriteState ()
{
    CSubscription *pSubscription;
    BL_IEC_TYP iecType;

    if (m_pRoot == NULL)
    {
        return (CENotWriteable);
    };

    pSubscription = m_pRoot->GetSubscription ();
    if (pSubscription == NULL)
    {
        return (CENotWriteable);
    };

    if (pSubscription->IsKnown ())
    {
        return (CEWriteable);
    };

    if (m_pType == NULL)
    {
        return (CEMaybeWriteable);
    };

    if (!IsSimpleType (iecType))
    {
        return (CENotWriteable);
    };

    return (CEMaybeWriteable);
}


HRESULT CExpressionHolder::Write (const CComVariant &varValue)
{
    CSubscription *pSubscription;

    if (GetWriteState () == CENotWriteable)
    {
        return (E_FAIL);
    };

    ASSERT (m_pRoot != NULL);
    if (m_pRoot == NULL)
    {
        return (E_FAIL);
    };

    pSubscription = m_pRoot->GetSubscription ();
    ASSERT (pSubscription != NULL);
    if (pSubscription == NULL)
    {
        return (E_FAIL);
    };

    return (pSubscription->Write (varValue));
}


bool CExpressionHolder::IsConnected ()
{
    return (m_uiConnectCount != 0);
}


void CExpressionHolder::Connect ()
{
    bool bConnected;

    bConnected = IsConnected ();
    ASSERT (!bConnected);
    if (bConnected)
    {
        return;
    };

    CExpressionNode::Connect ();

    if (m_pRoot != NULL)
    {
        m_pRoot->Connect ();
    };
}


void CExpressionHolder::Disconnect ()
{
    bool bConnected;

    bConnected = IsConnected ();
    ASSERT (bConnected);
    if (!bConnected)
    {
        return;
    };

    if (m_pRoot != NULL)
    {
        m_pRoot->Disconnect ();
    };

    CExpressionNode::Disconnect ();
}


void CExpressionHolder::Evaluate ()
{
    if (m_pRoot == NULL)
    {
        return;
    };

    SetState (m_pRoot->GetState ());
    SetValue (m_pRoot->GetValue ());
    SetQuality (m_pRoot->GetQuality ());
    SetTimestamp (m_pRoot->GetTimestamp ());
}


void CExpressionHolder::Changed ()
{
    if (IsConnected ())
    {
        m_pChangedCB->Execute (this);
    };
}


void CExpressionHolder::DisposeNodes (CExpressionNode *pNotMe)
{
    bool bFoundMe=false;

    while (!m_Nodes.IsEmpty ())
    {
        CExpressionNode *pNode;

        pNode = m_Nodes.RemoveHead ();
        ASSERT (pNode != NULL);
        if (pNode == NULL)
            continue;

        if (pNode != pNotMe)
        {
            pNode->RemoveParent (this);
        }
        else
        {
            bFoundMe = true;
        };
    };

    ASSERT (pNotMe == NULL || bFoundMe);

    if (pNotMe != NULL)
    {
        m_Nodes.AddTail (pNotMe);

        if (!bFoundMe)
        {
            pNotMe->AddParent (this);
        };
    };
}


CExpressionNode *CExpressionHolder::NewLiteral (__int64 llValue)
{
    CExpressionFactory *pFactory;
    CExpressionNode *pExpression;

    pFactory = CEGetExpressionFactory ();
    if (pFactory == NULL)
    {
        return (NULL);
    };

    pExpression = pFactory->NewLiteral (m_pDebugInfo, llValue);
    AddNode (pExpression);
    return (pExpression);
}


CExpressionNode *CExpressionHolder::NewLiteral (__int64 llValue, BL_IEC_TYP iecType)
{
    CExpressionFactory *pFactory;
    CExpressionNode *pExpression;

    pFactory = CEGetExpressionFactory ();
    if (pFactory == NULL)
    {
        return (NULL);
    };

    pExpression = pFactory->NewLiteral (m_pDebugInfo, llValue, iecType);
    AddNode (pExpression);
    return (pExpression);
}


CExpressionNode *CExpressionHolder::NewLiteral (double dblValue)
{
    CExpressionFactory *pFactory;
    CExpressionNode *pExpression;

    pFactory = CEGetExpressionFactory ();
    if (pFactory == NULL)
    {
        return (NULL);
    };

    pExpression = pFactory->NewLiteral (m_pDebugInfo, dblValue);
    AddNode (pExpression);
    return (pExpression);
}


CExpressionNode *CExpressionHolder::NewLiteral (double dblValue, BL_IEC_TYP iecType)
{
    CExpressionFactory *pFactory;
    CExpressionNode *pExpression;

    pFactory = CEGetExpressionFactory ();
    if (pFactory == NULL)
    {
        return (NULL);
    };

    pExpression = pFactory->NewLiteral (m_pDebugInfo, dblValue, iecType);
    AddNode (pExpression);
    return (pExpression);
}


CExpressionNode *CExpressionHolder::NewLiteral (bool bValue)
{
    CExpressionFactory *pFactory;
    CExpressionNode *pExpression;

    pFactory = CEGetExpressionFactory ();
    if (pFactory == NULL)
    {
        return (NULL);
    };

    pExpression = pFactory->NewLiteral (m_pDebugInfo, bValue);
    AddNode (pExpression);
    return (pExpression);
}


CExpressionNode *CExpressionHolder::NewLiteral (const CString &strValue)
{
    CExpressionFactory *pFactory;
    CExpressionNode *pExpression;

    pFactory = CEGetExpressionFactory ();
    if (pFactory == NULL)
    {
        return (NULL);
    };

    pExpression = pFactory->NewLiteral (m_pDebugInfo, strValue);
    AddNode (pExpression);
    return (pExpression);
}


CExpressionNode *CExpressionHolder::NewScopedName (CEExpandMode expandMode, const CString &strScope, const CString &strName, CString &strPrefix)
{
    HRESULT hr;
    CExpressionNode *pExpression;

    strPrefix.Empty ();


    if (!strScope.IsEmpty ())
    {
        hr = GetScopedName (strName, strScope, pExpression, strPrefix);

        if (hr == S_OK)
        {
            return (pExpression);
        };
    };


    if (expandMode != CEExpandNever)
    {
        hr = GetExpandedName (strName, expandMode, pExpression, strPrefix);

        if (hr == S_OK)
        {
            return (pExpression);
        };
    };

    if (!strScope.IsEmpty ())
    {
        pExpression = BuildPrefixedExpression (strName, strScope);

        if (pExpression != NULL)
        {
            return (pExpression);
        };
    };

    return (NewConfiguration (strName, CheckAddress(strName)));
}


HRESULT CExpressionHolder::GetScopedName (const CString &strName, const CString &strScope, CExpressionNode *&pExpression, CString &strPrefix)
{
    HRESULT hr;
    int iPos;
    CString strGlobalScope;
    CComPtr<ITypeDBI> pTypeDBI;
    CComPtr<IUnknown> pUnknown;
    CComPtr<IVariableDBI> pVariableDBI;
    DBIVariableKind kind;

    iPos = strScope.Find (_T ('.'));
    if (iPos != -1)
    {
        iPos = strScope.Find (_T ('.'), iPos + 1);
        if (iPos != -1)
        {
            strGlobalScope = strScope.Left (iPos);
        };
    };

    hr = GetTypeDBIForScope (strScope, pTypeDBI);
    if (hr != S_OK)
    {
        return (hr);
    };
    ASSERT (pTypeDBI != NULL);
    if (pTypeDBI == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pTypeDBI->get_VariableByName (CComBSTR (strName), &pUnknown);
    if (hr != S_OK)
    {
        return (hr);
    };
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pUnknown->QueryInterface (IID_IVariableDBI, (LPVOID *)&pVariableDBI);
    pUnknown = NULL;
    if (FAILED (hr))
    {
        return (E_UNEXPECTED);
    };
    ASSERT (pVariableDBI != NULL);
    if (pVariableDBI == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pVariableDBI->get_Kind (&kind);
    if (hr != S_OK)
    {
        return (hr);
    };

    strPrefix = kind == DBIVarExternal ? strGlobalScope : strScope;

    pExpression = BuildPrefixedExpression (strName, strPrefix);
    if (pExpression == NULL)
    {
        return (E_UNEXPECTED);
    };

    return (S_OK);
}


HRESULT CExpressionHolder::GetExpandedName (const CString &strName, CEExpandMode expandMode, CExpressionNode *&pExpression, CString &strPrefix)
{
    HRESULT hr;
    CComPtr<IProjectDBI> pProjectDBI;

    hr = GetProjectDBI (pProjectDBI);
    if (hr != S_OK)
    {
        return (hr);
    };

    hr = LookupChild (strName, pProjectDBI.p, strPrefix);
    if (hr != S_OK)
    {
        return (hr);
    };

    pExpression = BuildPrefixedExpression (strName, strPrefix);
    if (pExpression == NULL)
    {
        return (E_UNEXPECTED);
    };

    return (S_OK);
}


HRESULT CExpressionHolder::LookupChild (const CString &strName, const CComPtr<IUnknown> &pParent, CString &strPrefix)
{
    HRESULT hr;
    CString strParentName;
    unsigned long ulChildrenCount;
    unsigned long ulIndex;

    hr = GetDBIItemInfo (pParent, strParentName, ulChildrenCount);
    if (hr != S_OK)
    {
        return (hr);
    };

    if (strName.CompareNoCase (strParentName) == 0)
    {
        strPrefix = "";
        return (S_OK);
    };

    for (ulIndex = 0; ulIndex < ulChildrenCount; ++ulIndex)
    {
        CComPtr<IUnknown> pChild;

        hr = GetChildDBI (pParent, ulIndex, pChild);
        if (hr == S_OK)
        {
            hr = LookupChild (strName, pChild, strPrefix);
            if (hr == S_OK)
            {
                if (!strParentName.IsEmpty ())
                {
                    if (!strPrefix.IsEmpty ())
                    {
                        strPrefix = _T (".") + strPrefix;
                    };

                    strPrefix = strParentName + strPrefix;
                };

                return (S_OK);
            };
        };
    };

    return (S_FALSE);
}


HRESULT CExpressionHolder::GetDBIItemInfo (const CComPtr<IUnknown> &pDBI, CString &strName, unsigned long &ulChildrenCount)
{
    HRESULT hr;
    CComPtr<IProjectDBI> pProjectDBI;
    CComPtr<IConfigurationDBI> pConfigurationDBI;
    CComPtr<IResourceDBI> pResourceDBI;
    CComPtr<IVariableDBI> pVariableDBI;

    hr = pDBI->QueryInterface (IID_IProjectDBI, (LPVOID *)&pProjectDBI);
    if (SUCCEEDED (hr))
    {
        strName = "";

        hr = pProjectDBI->get_ConfigurationCount (&ulChildrenCount);
        return (hr);
    };

    hr = pDBI->QueryInterface (IID_IConfigurationDBI, (LPVOID *)&pConfigurationDBI);
    if (SUCCEEDED (hr))
    {
        CComBSTR sName;

        hr = pConfigurationDBI->get_Name (&sName);
        if (hr != S_OK)
        {
            return (hr);
        };
        strName = sName;

        hr = pConfigurationDBI->get_ResourceCount (&ulChildrenCount);
        return (hr);
    };

    hr = pDBI->QueryInterface (IID_IResourceDBI, (LPVOID *)&pResourceDBI);
    if (SUCCEEDED (hr))
    {
        CComBSTR sName;

        hr = pResourceDBI->get_Name (&sName);
        if (hr != S_OK)
        {
            return (hr);
        };
        strName = sName;

        hr = pResourceDBI->get_VariableCount (&ulChildrenCount);
        return (hr);
    };

    hr = pDBI->QueryInterface (IID_IVariableDBI, (LPVOID *)&pVariableDBI);
    if (SUCCEEDED (hr))
    {
        CComBSTR sName;
        CComPtr<IUnknown> pUnknown;
        CComPtr<ITypeDBI> pTypeDBI;

        hr = pVariableDBI->get_Name (&sName);
        if (hr != S_OK)
        {
            return (hr);
        };
        strName = sName;

        hr = pVariableDBI->get_Type (&pUnknown);
        if (hr != S_OK)
        {
            return (hr);
        };

        hr = pUnknown->QueryInterface (IID_ITypeDBI, (LPVOID *)&pTypeDBI);
        if (FAILED (hr))
        {
            return (hr);
        };

        hr = pTypeDBI->get_VariableCount (&ulChildrenCount);
        return (hr);
    };

    return (S_FALSE);
}


HRESULT CExpressionHolder::GetChildDBI (CComPtr<IUnknown> pDBI, unsigned long ulIndex, CComPtr<IUnknown> &pChildDBI)
{
    HRESULT hr;
    CComPtr<IProjectDBI> pProjectDBI;
    CComPtr<IConfigurationDBI> pConfigurationDBI;
    CComPtr<IResourceDBI> pResourceDBI;
    CComPtr<IVariableDBI> pVariableDBI;

    hr = pDBI->QueryInterface (IID_IProjectDBI, (LPVOID *)&pProjectDBI);
    if (SUCCEEDED (hr))
    {
        hr = pProjectDBI->get_ConfigurationByIndex (ulIndex, &pChildDBI);
        return (hr);
    };

    hr = pDBI->QueryInterface (IID_IConfigurationDBI, (LPVOID *)&pConfigurationDBI);
    if (SUCCEEDED (hr))
    {
        hr = pConfigurationDBI->get_ResourceByIndex (ulIndex, &pChildDBI);
        return (hr);
    };

    hr = pDBI->QueryInterface (IID_IResourceDBI, (LPVOID *)&pResourceDBI);
    if (SUCCEEDED (hr))
    {
        hr = pResourceDBI->get_VariableByIndex (ulIndex, &pChildDBI);
        return (hr);
    };

    hr = pDBI->QueryInterface (IID_IVariableDBI, (LPVOID *)&pVariableDBI);
    if (SUCCEEDED (hr))
    {
        CComPtr<IUnknown> pUnknown;
        CComPtr<ITypeDBI> pTypeDBI;

        hr = pVariableDBI->get_Type (&pUnknown);
        if (hr != S_OK)
        {
            return (hr);
        };

        hr = pUnknown->QueryInterface (IID_ITypeDBI, (LPVOID *)&pTypeDBI);
        if (FAILED (hr))
        {
            return (hr);
        };

        hr = pTypeDBI->get_VariableByIndex (ulIndex, &pChildDBI);
        return (hr);
    };

    return (S_FALSE);
}


CExpressionNode *CExpressionHolder::BuildPrefixedExpression (const CString &strName, const CString &strPrefix)
{
    CExpressionFactory *pFactory;
    int iPos;
    CString strTemp;
    CString strConfiguration;
    CExpressionNode *pExpression;

    pFactory = CEGetExpressionFactory ();
    if (pFactory == NULL)
    {
        return (NULL);
    };

	if (!strPrefix.IsEmpty ())
	{
		strTemp = strPrefix;
		strTemp += _T (".");
	};

	strTemp += strName;

    iPos = strTemp.Find (_T ('.'));
    if (iPos < 0)
    {
        strConfiguration = strTemp;
        strTemp = "";
    }
    else
    {
        strConfiguration = strTemp.Left (iPos);
        strTemp = strTemp.Mid (iPos + 1);
    };

    pExpression = NewConfiguration (strConfiguration, CheckAddress(strPrefix));

    while (!strTemp.IsEmpty ())
    {
        CString strField;

        iPos = strTemp.Find (_T ('.'));
        if (iPos < 0)
        {
            strField = strTemp;
            strTemp = "";
        }
        else
        {
            strField = strTemp.Left (iPos);
            strTemp = strTemp.Mid (iPos + 1);
        };

        pExpression = NewComponent (pExpression, strField);
    };

    return (pExpression);
}


//------------------------------------------------------------------*
/**
 * Checks whether the passed string constitutes a machine address
 * string encapsulated in { and } characters or propably a target instance
 * name.
 * @param			const CString & strName - name to parse
 * @return			bool - true if name is an address, false - otherwise.
*/
bool CExpressionHolder::CheckAddress(const CString & strName)
{
    CString str = strName;
    
    if(str.IsEmpty()) {
        return false;
    }

    str.TrimLeft();
    str.TrimRight();

    if(str[0] != _T('{')) {
        return false;
    }

    if(str.Find(_T('}')) == -1) {
        return false;
    }

    return true;
}

CExpressionNode *CExpressionHolder::NewConfiguration (const CString &strName, bool bAddress)
{
    CExpressionFactory *pFactory;
    CExpressionNode *pExpression;

    pFactory = CEGetExpressionFactory ();
    if (pFactory == NULL)
    {
        return (NULL);
    };

    pExpression = pFactory->NewConfiguration (m_pDebugInfo, strName, bAddress);
    AddNode (pExpression);
    return (pExpression);
}


CExpressionNode *CExpressionHolder::NewComponent (CExpressionNode *pAggregate, const CString &strFieldName)
{
    CExpressionFactory *pFactory;
    CExpressionNode *pExpression;

    pFactory = CEGetExpressionFactory ();
    if (pFactory == NULL)
    {
        return (NULL);
    };

    pExpression = pFactory->NewComponent (m_pDebugInfo, pAggregate, strFieldName);
    AddNode (pExpression);
    return (pExpression);
}


CExpressionNode *CExpressionHolder::NewIndexing (CExpressionNode *pArray, CExpressionNode *pIndex)
{
    CExpressionFactory *pFactory;
    CExpressionNode *pExpression;

    pFactory = CEGetExpressionFactory ();
    if (pFactory == NULL)
    {
        return (NULL);
    };

    pExpression = pFactory->NewIndexing (m_pDebugInfo, pArray, pIndex);
    AddNode (pExpression);
    return (pExpression);
}


CExpressionNode *CExpressionHolder::NewConversion (CExpressionNode *pOperand, BL_IEC_TYP sourceType, BL_IEC_TYP targetType)
{
    CExpressionFactory *pFactory;
    CExpressionNode *pExpression;

    pFactory = CEGetExpressionFactory ();
    if (pFactory == NULL)
    {
        return (NULL);
    };

    pExpression = pFactory->NewConversion (m_pDebugInfo, pOperand, sourceType, targetType);
    AddNode (pExpression);
    return (pExpression);
}


CExpressionNode *CExpressionHolder::NewUnaryArithmetic (CExpressionUnaryArithmetic::Arithmetic arithmetic, CExpressionNode *pOperand)
{
    CExpressionFactory *pFactory;
    CExpressionNode *pExpression;

    pFactory = CEGetExpressionFactory ();
    if (pFactory == NULL)
    {
        return (NULL);
    };

    pExpression = pFactory->NewUnaryArithmetic (m_pDebugInfo, arithmetic, pOperand);
    AddNode (pExpression);
    return (pExpression);
}


CExpressionNode *CExpressionHolder::NewUnaryLogical (CExpressionUnaryLogical::Logical logical, CExpressionNode *pOperand)
{
    CExpressionFactory *pFactory;
    CExpressionNode *pExpression;

    pFactory = CEGetExpressionFactory ();
    if (pFactory == NULL)
    {
        return (NULL);
    };

    pExpression = pFactory->NewUnaryLogical (m_pDebugInfo, logical, pOperand);
    AddNode (pExpression);
    return (pExpression);
}


CExpressionNode *CExpressionHolder::NewBinaryArithmetic (CExpressionBinaryArithmetic::Arithmetic arithmetic, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand)
{
    CExpressionFactory *pFactory;
    CExpressionNode *pExpression;

    pFactory = CEGetExpressionFactory ();
    if (pFactory == NULL)
    {
        return (NULL);
    };

    pExpression = pFactory->NewBinaryArithmetic (m_pDebugInfo, arithmetic, pLeftOperand, pRightOperand);
    AddNode (pExpression);
    return (pExpression);
}


CExpressionNode *CExpressionHolder::NewFunctionalArithmetic (CExpressionBinaryArithmetic::Arithmetic arithmetic, CList<CExpressionNode *, CExpressionNode *> *pOperands)
{
    CExpressionNode *pLeftOperand;
    CExpressionNode *pRightOperand;

    ASSERT (pOperands != NULL);
    if (pOperands == NULL)
    {
        return (NULL);
    };

    ASSERT (pOperands->GetCount () >= 2);
    if (pOperands->GetCount () < 2)
    {
        return (NULL);
    };

    pLeftOperand = pOperands->RemoveHead ();

    while (!pOperands->IsEmpty ())
    {
        pRightOperand = pOperands->RemoveHead ();
        pLeftOperand = NewBinaryArithmetic (arithmetic, pLeftOperand, pRightOperand);
    };

    return (pLeftOperand);
}


CExpressionNode *CExpressionHolder::NewRelation (CExpressionRelation::Relation relation, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand)
{
    CExpressionFactory *pFactory;
    CExpressionNode *pExpression;

    pFactory = CEGetExpressionFactory ();
    if (pFactory == NULL)
    {
        return (NULL);
    };

    pExpression = pFactory->NewRelation (m_pDebugInfo, relation, pLeftOperand, pRightOperand);
    AddNode (pExpression);
    return (pExpression);
}


CExpressionNode *CExpressionHolder::NewBinaryLogical (CExpressionBinaryLogical::Logical logical, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand)
{
    CExpressionFactory *pFactory;
    CExpressionNode *pExpression;

    pFactory = CEGetExpressionFactory ();
    if (pFactory == NULL)
    {
        return (NULL);
    };

    pExpression = pFactory->NewBinaryLogical (m_pDebugInfo, logical, pLeftOperand, pRightOperand);
    AddNode (pExpression);
    return (pExpression);
}


CExpressionNode *CExpressionHolder::NewFunctionalLogical (CExpressionBinaryLogical::Logical logical, CList<CExpressionNode *, CExpressionNode *> *pOperands)
{
    CExpressionNode *pLeftOperand;
    CExpressionNode *pRightOperand;

    ASSERT (pOperands != NULL);
    if (pOperands == NULL)
    {
        return (NULL);
    };

    ASSERT (pOperands->GetCount () >= 2);
    if (pOperands->GetCount () < 2)
    {
        return (NULL);
    };

    pLeftOperand = pOperands->RemoveHead ();

    while (!pOperands->IsEmpty ())
    {
        pRightOperand = pOperands->RemoveHead ();
        pLeftOperand = NewBinaryLogical (logical, pLeftOperand, pRightOperand);
    };

    return (pLeftOperand);
}


CExpressionNode *CExpressionHolder::NewPower (CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand)
{
    CExpressionFactory *pFactory;
    CExpressionNode *pExpression;

    pFactory = CEGetExpressionFactory ();
    if (pFactory == NULL)
    {
        return (NULL);
    };

    pExpression = pFactory->NewPower (m_pDebugInfo, pLeftOperand, pRightOperand);
    AddNode (pExpression);
    return (pExpression);
}


HRESULT CExpressionHolder::GetTypeDBIForScope (const CString &strScope, CComPtr<ITypeDBI> &pTypeDBI)
{
    HRESULT hr;
    CComPtr<IUnknown> pUnknown;
    CComPtr<IProjectDBI> pProjectDBI;
    CComPtr<IInstanceDBI> pInstanceDBI;
    CComPtr<IResourceDBI> pResourceDBI;

    pTypeDBI = NULL;

    hr = GetProjectDBI (pProjectDBI);
    if (hr != S_OK)
    {
        return (hr);
    };

	hr = pProjectDBI->get_InstanceByPath (CComBSTR (strScope), &pUnknown);
    if (hr != S_OK)
    {
        return (hr);
    };
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pUnknown->QueryInterface (IID_IInstanceDBI, (LPVOID *)&pInstanceDBI);
    if (SUCCEEDED (hr))
    {
        pUnknown = NULL;

        ASSERT (pInstanceDBI != NULL);
        if (pInstanceDBI == NULL)
        {
            return (E_UNEXPECTED);
        };

        hr = pInstanceDBI->get_Type (&pUnknown);
        if (hr != S_OK)
        {
            return (hr);
        };
    }
    else
    {
        hr = pUnknown->QueryInterface (IID_IResourceDBI, (LPVOID *)&pResourceDBI);
        if (SUCCEEDED (hr))
        {
            pUnknown = NULL;

            ASSERT (pResourceDBI != NULL);
            if (pResourceDBI == NULL)
            {
                return (E_UNEXPECTED);
            };

            hr = pResourceDBI->get_Type (&pUnknown);
            if (hr != S_OK)
            {
                return (hr);
            };
        }
        else
        {
            return (hr);
        };
    };

    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pUnknown->QueryInterface (IID_ITypeDBI, (LPVOID *)&pTypeDBI);
    pUnknown = NULL;
    if (FAILED (hr))
    {
        return (hr);
    };
    ASSERT (pTypeDBI != NULL);
    if (pTypeDBI == NULL)
    {
        return (E_UNEXPECTED);
    };

    return (S_OK);
}


HRESULT CExpressionHolder::GetProjectDBI (CComPtr<IProjectDBI> &pProjectDBI)
{
    HRESULT hr;
    CComPtr<IUnknown> pUnknown;

    ASSERT (m_pDebugInfo != NULL);
    if (m_pDebugInfo == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = m_pDebugInfo->get_Project (&pUnknown);
    if (hr != S_OK)
    {
        return (hr);
    };
    ASSERT (pUnknown != NULL);
    if (pUnknown == NULL)
    {
        return (E_UNEXPECTED);
    };

    hr = pUnknown->QueryInterface (IID_IProjectDBI, (LPVOID *)&pProjectDBI);
    pUnknown = NULL;
    if (FAILED (hr))
    {
        return (hr);
    };
    ASSERT (pProjectDBI != NULL);
    if (pProjectDBI == NULL)
    {
        return (E_UNEXPECTED);
    };

    return (S_OK);
}


CExpressionFactory::CExpressionFactory ()
: m_lNextHandle (1)
{
}


CExpressionHolder *CExpressionFactory::NewHolder (CComPtr<IDebugInfo> pDebugInfo)
{
    CExpressionHolder *pHolder;

    pHolder = new CExpressionHolder (pDebugInfo, m_lNextHandle++);
    ASSERT (pHolder != NULL);
    return (pHolder);
}


CExpressionNode *CExpressionFactory::NewLiteral (CComPtr<IDebugInfo> pDebugInfo, __int64 llValue)
{
    CExpressionLiteral *pExpression;

    pExpression = new CExpressionLiteral (pDebugInfo, llValue);
    return (pExpression);
}


CExpressionNode *CExpressionFactory::NewLiteral (CComPtr<IDebugInfo> pDebugInfo, __int64 llValue, BL_IEC_TYP iecType)
{
    CExpressionLiteral *pExpression;

    pExpression = new CExpressionLiteral (pDebugInfo, llValue, iecType);
    return (pExpression);
}


CExpressionNode *CExpressionFactory::NewLiteral (CComPtr<IDebugInfo> pDebugInfo, double dblValue)
{
    CExpressionLiteral *pExpression;

    pExpression = new CExpressionLiteral (pDebugInfo, dblValue);
    return (pExpression);
}


CExpressionNode *CExpressionFactory::NewLiteral (CComPtr<IDebugInfo> pDebugInfo, double dblValue, BL_IEC_TYP iecType)
{
    CExpressionLiteral *pExpression;

    pExpression = new CExpressionLiteral (pDebugInfo, dblValue, iecType);
    return (pExpression);
}


CExpressionNode *CExpressionFactory::NewLiteral (CComPtr<IDebugInfo> pDebugInfo, bool bValue)
{
    CExpressionLiteral *pExpression;

    pExpression = new CExpressionLiteral (pDebugInfo, bValue);
    return (pExpression);
}


CExpressionNode *CExpressionFactory::NewLiteral (CComPtr<IDebugInfo> pDebugInfo, const CString &strValue)
{
    CExpressionLiteral *pExpression;

    pExpression = new CExpressionLiteral (pDebugInfo, strValue);
    return (pExpression);
}


CExpressionNode *CExpressionFactory::NewConfiguration (CComPtr<IDebugInfo> pDebugInfo, const CString &strName, bool bAddress)
{
    CExpressionConfiguration *pExpression;

    pExpression = new CExpressionConfiguration (pDebugInfo, strName, bAddress);
    return (pExpression);
}


CExpressionNode *CExpressionFactory::NewComponent (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pAggregate, const CString &strFieldName)
{
    CExpressionComponent *pExpression;

    pExpression = new CExpressionComponent (pDebugInfo, pAggregate, strFieldName);
    return (pExpression);
}


CExpressionNode *CExpressionFactory::NewIndexing (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pArray, CExpressionNode *pIndex)
{
    CExpressionIndexing *pExpression;

    pExpression = new CExpressionIndexing (pDebugInfo, pArray, pIndex);
    return (pExpression);
}


CExpressionNode *CExpressionFactory::NewConversion (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pOperand, BL_IEC_TYP sourceType, BL_IEC_TYP targetType)
{
    CExpressionConversion *pExpression;

    pExpression = new CExpressionConversion (pDebugInfo, pOperand, sourceType, targetType);
    return (pExpression);
}


CExpressionNode *CExpressionFactory::NewUnaryArithmetic (CComPtr<IDebugInfo> pDebugInfo, CExpressionUnaryArithmetic::Arithmetic arithmetic, CExpressionNode *pOperand)
{
    CExpressionUnaryArithmetic *pExpression;

    pExpression = new CExpressionUnaryArithmetic (pDebugInfo, arithmetic, pOperand);
    return (pExpression);
}


CExpressionNode *CExpressionFactory::NewUnaryLogical (CComPtr<IDebugInfo> pDebugInfo, CExpressionUnaryLogical::Logical logical, CExpressionNode *pOperand)
{
    CExpressionUnaryLogical *pExpression;

    pExpression = new CExpressionUnaryLogical (pDebugInfo, logical, pOperand);
    return (pExpression);
}


CExpressionNode *CExpressionFactory::NewBinaryArithmetic (CComPtr<IDebugInfo> pDebugInfo, CExpressionBinaryArithmetic::Arithmetic arithmetic, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand)
{
    CExpressionBinaryArithmetic *pExpression;

    pExpression = new CExpressionBinaryArithmetic (pDebugInfo, arithmetic, pLeftOperand, pRightOperand);
    return (pExpression);
}


CExpressionNode *CExpressionFactory::NewRelation (CComPtr<IDebugInfo> pDebugInfo, CExpressionRelation::Relation relation, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand)
{
    CExpressionRelation *pExpression;

    pExpression = new CExpressionRelation (pDebugInfo, relation, pLeftOperand, pRightOperand);
    return (pExpression);
}


CExpressionNode *CExpressionFactory::NewBinaryLogical (CComPtr<IDebugInfo> pDebugInfo, CExpressionBinaryLogical::Logical logical, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand)
{
    CExpressionBinaryLogical *pExpression;

    pExpression = new CExpressionBinaryLogical (pDebugInfo, logical, pLeftOperand, pRightOperand);
    return (pExpression);
}


CExpressionNode *CExpressionFactory::NewPower (CComPtr<IDebugInfo> pDebugInfo, CExpressionNode *pLeftOperand, CExpressionNode *pRightOperand)
{
    CExpressionPower *pExpression;

    pExpression = new CExpressionPower (pDebugInfo, pLeftOperand, pRightOperand);
    return (pExpression);
}


