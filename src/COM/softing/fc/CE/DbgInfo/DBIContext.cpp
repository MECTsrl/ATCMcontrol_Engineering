
#include "stdafx.h"
#include "XML_Wrapper.h"
#include "DBIContext.h"
#include "TypeDBI.h"
#include "VariableDBI.h"
#include "BreakpointDBI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDBIContext::CDBIContext (const CString &strDebugInfoPath)
: m_uiReferences (1),
  m_strDebugInfoPath (strDebugInfoPath),
  m_pStepType (NULL),
  m_pActionType (NULL),
  m_pTransitionType (NULL),
  m_bCorrupt (false)
{
    BL_IEC_TYP iecType;

    for (iecType = BL_ANY; iecType <= BL_LASTTYPE; iecType = (BL_IEC_TYP)(iecType + 1))
    {
        m_SimpleTypes[iecType] = NULL;
    };
}


CDBIContext::~CDBIContext ()
{
    BL_IEC_TYP iecType;


    // Can't die as long as there are types because types hold reference on ourselves.

    for (iecType = BL_ANY; iecType <= BL_LASTTYPE; iecType = (BL_IEC_TYP)(iecType + 1))
    {
        ASSERT (m_SimpleTypes[iecType] == NULL);
    };

    ASSERT (m_UserTypes.GetSize () == 0);

    ASSERT (m_pStepType == NULL);

    ASSERT (m_pActionType == NULL);

    ASSERT (m_pTransitionType == NULL);
}


void CDBIContext::Reference ()
{
    ++m_uiReferences;
}


void CDBIContext::Unreference ()
{
    if (--m_uiReferences == 0)
    {
        delete this;
    };
}


void CDBIContext::TypeDieing (CTypeDBI *pType)
{
    int iIndex;
    BL_IEC_TYP iecType;

    // ATTENTION: Do not call any methods on pType inside here.
    // pType is about to be destroyed and calling methods on it
    // could lead to rather unexpected results!!!

    ASSERT (pType != NULL);
    if (pType == NULL)
    {
        return;
    };

    if (pType == m_pStepType)
    {
        m_pStepType = NULL;
        return;
    };

    if (pType == m_pActionType)
    {
        m_pActionType = NULL;
        return;
    };

    if (pType == m_pTransitionType)
    {
        m_pTransitionType = NULL;
        return;
    };

    for (iecType = BL_ANY; iecType <= BL_LASTTYPE; iecType = (BL_IEC_TYP)(iecType + 1))
    {
        if (m_SimpleTypes[iecType] == pType)
        {
            m_SimpleTypes[iecType] = NULL;
            return;
        };
    };

    for (iIndex = 0; iIndex < m_UserTypes.GetSize (); ++iIndex)
    {
        if (m_UserTypes[iIndex] == pType)
        {
            if (iIndex + 1 < m_UserTypes.GetSize ())
            {
                m_UserTypes[iIndex] = m_UserTypes[m_UserTypes.GetSize () - 1];
            };

            m_UserTypes.SetSize (m_UserTypes.GetSize () - 1);

            return;
        };
    };

    ASSERT (false);
}


bool CDBIContext::IsCorrupt ()
{
    return (m_bCorrupt);
}


void CDBIContext::Corrupt ()
{
    m_bCorrupt = true;
}


CString CDBIContext::GetDebugInfoPath () const
{
    return (m_strDebugInfoPath);
}


CTypeDBI *CDBIContext::GetSimpleType (BL_IEC_TYP iecType)
{
    HRESULT hr;
    CComObject<CTypeDBI> *pType;

    if (m_SimpleTypes[iecType] != NULL)
    {
        CTypeDBI *pTypeDBI;

        pTypeDBI = m_SimpleTypes[iecType];
        pTypeDBI->AddRef ();
        return (pTypeDBI);
    };

    hr = CComObject<CTypeDBI>::CreateInstance (&pType);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (NULL);
    };
    ASSERT (pType != NULL);
    if (pType == NULL)
    {
        return (NULL);
    };

    pType->AddRef ();

    hr = pType->Init (this, iecType);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        pType->Release ();
        return (NULL);
    };

    m_SimpleTypes[iecType] = pType;
    return (pType);
}


CTypeDBI *CDBIContext::GetConfigurationType ()
{
    HRESULT hr;
    CComObject<CTypeDBI> *pType;

    hr = CComObject<CTypeDBI>::CreateInstance (&pType);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (NULL);
    };
    ASSERT (pType != NULL);
    if (pType == NULL)
    {
        return (NULL);
    };

    pType->AddRef ();

    hr = pType->Init (this, DBIConfigurationType);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        pType->Release ();
        return (NULL);
    };

    m_UserTypes.Add (pType);

    return (pType);
}


CTypeDBI *CDBIContext::GetStepType ()
{
    HRESULT hr;
    CComObject<CTypeDBI> *pType;

    if (m_pStepType != NULL)
    {
        m_pStepType->AddRef ();
        return (m_pStepType);
    };

    hr = CComObject<CTypeDBI>::CreateInstance (&pType);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (NULL);
    };
    ASSERT (pType != NULL);
    if (pType == NULL)
    {
        return (NULL);
    };

    pType->AddRef ();

    hr = pType->Init (this, DBIStepType);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        pType->Release ();
        return (NULL);
    };

    m_pStepType = pType;

    return (pType);
}


CTypeDBI *CDBIContext::GetActionType ()
{
    HRESULT hr;
    CComObject<CTypeDBI> *pType;

    if (m_pActionType != NULL)
    {
        m_pActionType->AddRef ();
        return (m_pActionType);
    };

    hr = CComObject<CTypeDBI>::CreateInstance (&pType);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (NULL);
    };
    ASSERT (pType != NULL);
    if (pType == NULL)
    {
        return (NULL);
    };

    pType->AddRef ();

    hr = pType->Init (this, DBIActionType);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        pType->Release ();
        return (NULL);
    };

    m_pActionType = pType;

    return (pType);
}


CTypeDBI *CDBIContext::GetTransitionType ()
{
    HRESULT hr;
    CComObject<CTypeDBI> *pType;

    if (m_pTransitionType != NULL)
    {
        m_pTransitionType->AddRef ();
        return (m_pTransitionType);
    };

    hr = CComObject<CTypeDBI>::CreateInstance (&pType);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        return (NULL);
    };
    ASSERT (pType != NULL);
    if (pType == NULL)
    {
        return (NULL);
    };

    pType->AddRef ();

    hr = pType->Init (this, DBITransitionType);
    ASSERT (SUCCEEDED (hr));
    if (FAILED (hr))
    {
        pType->Release ();
        return (NULL);
    };

    m_pTransitionType = pType;

    return (pType);
}


HRESULT CDBIContext::BuildDataType (CString strText, const CString &strNamespace, const CString &strDBIFile, CTypeDBI *&pType)
{
    int iPos;
    CString strIECType;
    HRESULT hr;
    BL_IEC_TYP iecType;
    DWORD dwModifiers;
    USES_CONVERSION;

    strText.TrimLeft ();

    for (iPos = 0; iPos < strText.GetLength (); ++iPos)
    {
        if (!_istalnum (strText[iPos]) && strText[iPos] != _T ('_'))
            break;
    };

    if (iPos == 0)
    {
        return (E_UNEXPECTED);
    };

    strIECType = strText.Left (iPos);

    if (strIECType.CompareNoCase (_T ("ARRAY")) == 0)
    {
        CString strRangeList;

        strText = strText.Mid (iPos);
        strText.TrimLeft ();

        ASSERT (strText[0] == _T ('['));
        if (strText[0] != _T ('['))
        {
            return (E_UNEXPECTED);
        };

        iPos = strText.Find (_T (']'));
        ASSERT (iPos != -1);
        if (iPos == -1)
        {
            return (E_UNEXPECTED);
        };

        strRangeList = strText.Mid (1, iPos - 1);
        strText = strText.Mid (iPos + 1);

        hr = BuildArrayType (strRangeList, strNamespace, strText, strDBIFile, pType);
        return (hr);
    };

    hr = BL_TypenameToDatatype (T2W (strIECType), &iecType, &dwModifiers);
    if (hr != E_BL_ERR_SYNTAX)
    {
        if (FAILED (hr))
        {
            return (hr);
        };

        hr = BuildSimpleType (iecType, strText.Mid (iPos), pType);
        return (hr);
    };

    hr = BuildNamedType (strText.Mid (iPos), strNamespace, strIECType, strDBIFile, pType);
    return (hr);
}


HRESULT CDBIContext::BuildArrayType (CString strRangeList, const CString &strNamespace, CString strText, const CString &strDBIFile, CTypeDBI *&pType)
{
    HRESULT hr;
    CComObject<CTypeDBI> *pTypeDBI;
    CTypeDBI *pElementType;
    int iPos;
    long lLowerBound;
    long lUpperBound;
    CString strRange;
    CString strBound;
    LPTSTR pszEnd;
    int iIndex;

    pType = NULL;

    iPos = strRangeList.Find (_T (','));
    if (iPos == -1)
    {
        strRange = strRangeList;
        strRangeList = "";
    }
    else
    {
        strRange = strRangeList.Left (iPos);
        strRangeList = strRangeList.Mid (iPos + 1);
    };

    iPos = strRange.Find (_T ('.'));
    ASSERT (iPos != -1 && iPos + 1 < strRange.GetLength () && strRange[iPos + 1] == _T ('.'));
    if (iPos == -1 || iPos + 1 >= strRange.GetLength () || strRange[iPos + 1] != _T ('.'))
    {
        return (E_UNEXPECTED);
    };

    strBound = strRange.Left (iPos);
    strBound.TrimLeft ();
    strBound.TrimRight ();
    lLowerBound = _tcstol (strBound, &pszEnd, 10);
    ASSERT (pszEnd == NULL || *pszEnd == _T ('\0'));
    if (pszEnd != NULL && *pszEnd != _T ('\0'))
    {
        return (E_UNEXPECTED);
    };

    strBound = strRange.Mid (iPos + 2);
    strBound.TrimLeft ();
    strBound.TrimRight ();
    lUpperBound = _tcstol (strBound, &pszEnd, 10);
    ASSERT (pszEnd == NULL || *pszEnd == _T ('\0'));
    if (pszEnd != NULL && *pszEnd != _T ('\0'))
    {
        return (E_UNEXPECTED);
    };

    strRangeList.TrimLeft ();
    if (!strRangeList.IsEmpty ())
    {
        hr = BuildArrayType (strRangeList, strNamespace, strText, strDBIFile, pElementType);
    }
    else
    {
        strText.TrimLeft ();
        ASSERT (strText.Left (2).CompareNoCase (_T ("OF")) == 0 && _istspace (strText[2]));
        if (strText.Left (2).CompareNoCase (_T ("OF")) != 0 || !_istspace (strText[2]))
        {
            return (E_UNEXPECTED);
        };

        hr = BuildDataType (strText.Mid (3), "<POUS>", strDBIFile, pElementType);
    };
    if (FAILED (hr))
    {
        return (hr);
    };
    ASSERT (pElementType != NULL);
    if (pElementType == NULL)
    {
        return (E_UNEXPECTED);
    };

    for (iIndex = 0; iIndex < m_UserTypes.GetSize (); ++iIndex)
    {
        CTypeDBI *pUserType;

        pUserType = m_UserTypes[iIndex];
        ASSERT (pUserType != NULL);
        if (pUserType == NULL)
        {
            pElementType->Release ();
            return (E_UNEXPECTED);
        };

        if (!pUserType->GetTypeName ().IsEmpty () ||
            pUserType->GetKind () != DBIArrayType)
            continue;

        if (pElementType == pUserType->GetElementType () &&
            lLowerBound == pUserType->GetLowerBound () &&
            lUpperBound == pUserType->GetUpperBound ())
        {
            pType = pUserType;
            pType->AddRef ();
            return (S_OK);
        };
    };

    hr = CComObject<CTypeDBI>::CreateInstance (&pTypeDBI);
    if (FAILED (hr))
    {
        pElementType->Release ();
        return (hr);
    };
    ASSERT (pTypeDBI != NULL);
    if (pTypeDBI == NULL)
    {
        pElementType->Release ();
        return (E_UNEXPECTED);
    };
    pTypeDBI->AddRef ();

    hr = pTypeDBI->Init (this, pElementType, lLowerBound, lUpperBound);
    if (FAILED (hr))
    {
        pElementType->Release ();
        pTypeDBI->Release ();
        return (hr);
    };

    m_UserTypes.Add (pTypeDBI);

    pType = pTypeDBI;
    return (S_OK);
}


HRESULT CDBIContext::BuildSimpleType (BL_IEC_TYP iecType, CString strText, CTypeDBI *&pType)
{
    HRESULT hr;
    CComObject<CTypeDBI> *pTypeDBI;
    unsigned long ulStringLength;
    LPTSTR pszEnd;
    int iIndex;

    strText.TrimLeft ();
    strText.TrimRight ();

    pType = NULL;

    if (strText.IsEmpty () || iecType != BL_WSTRING && iecType != BL_STRING)
    {
        pType = GetSimpleType (iecType);
        ASSERT (pType != NULL);
        if (pType == NULL)
        {
            return (E_UNEXPECTED);
        };

        return (S_OK);
    };

    ASSERT (strText[0] == _T ('[') && strText[strText.GetLength () - 1] == _T (']'));
    if (strText[0] != _T ('[') || strText[strText.GetLength () - 1] != _T (']'))
    {
        return (E_UNEXPECTED);
    };
    strText = strText.Mid (1, strText.GetLength () - 2);

    ulStringLength = _tcstoul (strText, &pszEnd, 10);
    ASSERT (pszEnd == NULL || *pszEnd == 0);
    if (pszEnd != NULL && *pszEnd != 0)
    {
        return (E_UNEXPECTED);
    };

    for (iIndex = 0; iIndex < m_UserTypes.GetSize (); ++iIndex)
    {
        CTypeDBI *pUserType;

        pUserType = m_UserTypes[iIndex];
        ASSERT (pUserType != NULL);
        if (pUserType == NULL)
        {
            return (E_UNEXPECTED);
        };

        if (!pUserType->GetTypeName ().IsEmpty () ||
            pUserType->GetKind () != DBISimpleType)
            continue;

        ASSERT (   pUserType->GetIECType () == BL_WSTRING
                || pUserType->GetIECType () == BL_STRING);
        if (   pUserType->GetIECType () != BL_WSTRING
            && pUserType->GetIECType () != BL_STRING)
            continue;

        if (ulStringLength == pUserType->GetStringLength ())
        {
            pType = pUserType;
            pType->AddRef ();
            return (S_OK);
        };
    };

    hr = CComObject<CTypeDBI>::CreateInstance (&pTypeDBI);
    if (FAILED (hr))
    {
        return (hr);
    };
    ASSERT (pTypeDBI != NULL);
    if (pTypeDBI == NULL)
    {
        return (E_UNEXPECTED);
    };
    pTypeDBI->AddRef ();

    hr = pTypeDBI->Init (this, iecType, ulStringLength);
    if (FAILED (hr))
    {
        pTypeDBI->Release ();
        return (hr);
    };

    m_UserTypes.Add (pTypeDBI);

    pType = pTypeDBI;
    return (S_OK);
}


HRESULT CDBIContext::BuildNamedType (CString strText, const CString &strNamespace, const CString &strName, const CString &strDBIFile, CTypeDBI *&pType)
{
    int iIndex;
    HRESULT hr;
    CComObject<CTypeDBI> *pTypeDBI;

    strText.TrimLeft ();
    strText.TrimRight ();

    pType = NULL;

    for (iIndex = 0; iIndex < m_UserTypes.GetSize (); ++iIndex)
    {
        CTypeDBI *pUserType;

        pUserType = m_UserTypes[iIndex];
        ASSERT (pUserType != NULL);
        if (pUserType == NULL)
        {
            return (E_UNEXPECTED);
        };

        if (strNamespace.CompareNoCase (pUserType->GetNamespace ()) == 0 &&
            strName.CompareNoCase (pUserType->GetTypeName ()) == 0)
        {
            pType = pUserType;
            pType->AddRef ();
            return (S_OK);
        };
    };

    hr = CComObject<CTypeDBI>::CreateInstance (&pTypeDBI);
    if (FAILED (hr))
    {
        return (hr);
    };
    ASSERT (pTypeDBI != NULL);
    if (pTypeDBI == NULL)
    {
        return (E_UNEXPECTED);
    };
    pTypeDBI->AddRef ();

    ASSERT (strText.IsEmpty ());
    if (!strText.IsEmpty ())
    {
        pTypeDBI->Release ();
        return (E_UNEXPECTED);
    };

    hr = pTypeDBI->Init (this, strName, strNamespace, strDBIFile);
    if (FAILED (hr))
    {
        pTypeDBI->Release ();
        return (hr);
    };

    m_UserTypes.Add (pTypeDBI);

    pType = pTypeDBI;
    return (S_OK);
}
