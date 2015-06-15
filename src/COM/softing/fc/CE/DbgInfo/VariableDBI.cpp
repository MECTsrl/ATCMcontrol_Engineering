
#include "stdafx.h"
#include "VariableDBI.h"
#include "XML_Wrapper.h"
#include "DBIContext.h"
#include "TypeDBI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DBI_XML_ID _T ("ID")
#define DBI_XML_TYPE _T ("TYPE")
#define DBI_XML_VALUE _T ("VALUE")
#define DBI_XML_DBI _T ("DBI")
#define DBI_XML_VAR _T ("VAR")
#define DBI_XML_VARINPUT _T ("VAR_INPUT")
#define DBI_XML_VAROUTPUT _T ("VAR_OUTPUT")
#define DBI_XML_VARINOUT _T ("VAR_IN_OUT")
#define DBI_XML_VARGLOBAL _T ("VAR_GLOBAL")
#define DBI_XML_VAREXTERNAL _T ("VAR_EXTERNAL")
#define DBI_XML_CONST _T ("CONST")


CVariableDBI::CVariableDBI ()
: m_pType (NULL), 
  m_pStrOnlString (NULL)
{
}


CVariableDBI::~CVariableDBI ()
{
    if(m_pStrOnlString)
        delete(m_pStrOnlString);

    if (m_pType != NULL)
    {
        m_pType->Release ();
    };
}


HRESULT CVariableDBI::Init (CDBIContext *pContext, DBIVariableKind kind, const CString &strName, CTypeDBI *pType, const TCHAR* pszOnlString)
{
    ASSERT (pContext != NULL);
    if (pContext == NULL)
    {
        return (E_POINTER);
    };

    ASSERT (kind != DBIConst);
    if (kind == DBIConst)
    {
        return (E_INVALIDARG);
    };

    ASSERT (pType != NULL);
    if (pType == NULL)
    {
        return (E_INVALIDARG);
    };

    SetContext (pContext);

    m_Kind = kind;

    m_strName = strName;

    if(pszOnlString)
        m_pStrOnlString = new CString(pszOnlString);

    pType->AddRef ();
    m_pType = pType;

    return (S_OK);
}


HRESULT CVariableDBI::Init (CDBIContext *pContext, const CString &strName, CTypeDBI *pType, const CString &strValue)
{
    USES_CONVERSION;

    ASSERT (pContext != NULL);
    if (pContext == NULL)
    {
        return (E_POINTER);
    };

    ASSERT (pType != NULL);
    if (pType == NULL)
    {
        return (E_POINTER);
    };

    SetContext (pContext);

    m_Kind = DBIConst;
    m_strName = strName;

    pType->AddRef ();
    m_pType = pType;

    if (pType->GetKind () == DBISimpleType)
    {
        HRESULT hr;

        hr = BL_StringToVariant (T2COLE (strValue), pType->GetIECType (), NULL, BL_USE_IEC_FORMAT, &m_varValue);
        if (FAILED (hr))
        {
            pType->Release ();
            m_pType = NULL;

            SetContext (NULL);
            return (hr);
        };
    }
    else
    {
        m_varValue = (LPCTSTR)strValue;
    };

    return (S_OK);
}


HRESULT CVariableDBI::Init (CDBIContext *pContext, DBIVariableKind kind, CXMLArchive &ar)
{
    HRESULT hr;
    CString strName;
    CString strType;
    CTypeDBI *pType;

    ASSERT (pContext != NULL);
    if (pContext == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        if (!ar.GetAttribute (DBI_XML_ID, strName))
        {
            Corrupt ();
            throw E_FAIL;
        };

        if (!ar.GetAttribute (DBI_XML_TYPE, strType))
        {
            Corrupt ();
            throw E_FAIL;
        };

        if (kind == DBIConst)
        {
            CString strValue;

            if (!ar.GetAttribute (DBI_XML_VALUE, strValue))
            {
                Corrupt ();
                throw E_FAIL;
            };

            hr = pContext->BuildDataType (strType, "<POUS>", "", pType);
            if (FAILED (hr))
            {
                throw hr;
            };
            ASSERT (pType != NULL);
            if (pType == NULL)
            {
                throw E_UNEXPECTED;
            };

            hr = Init (pContext, strName, pType, strValue);
            if (FAILED (hr))
            {
                pType->Release ();
                throw hr;
            };

            pType->Release ();
        }
        else
        {
            CString strDBIFile;

            if (!ar.GetAttribute (DBI_XML_DBI, strDBIFile))
            {
                strDBIFile = "";
            };

            hr = pContext->BuildDataType (strType, "<POUS>", strDBIFile, pType);
            if (FAILED (hr))
            {
                throw hr;
            };
            ASSERT (pType != NULL);
            if (pType == NULL)
            {
                throw E_UNEXPECTED;
            };

            hr = Init (pContext, kind, strName, pType, NULL);
            if (FAILED (hr))
            {
                pType->Release ();
                throw hr;
            };

            pType->Release ();
        };
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


CString CVariableDBI::GetName ()
{
    return (m_strName);
}


CTypeDBI *CVariableDBI::GetType ()
{
    return (m_pType);
}


DBIVariableKind CVariableDBI::GetKind ()
{
    return (m_Kind);
}


CComVariant CVariableDBI::GetValue ()
{
    return (m_varValue);
}


STDMETHODIMP CVariableDBI::get_Name(BSTR *psName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (psName != NULL);
    if (psName == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *psName = GetName ().AllocSysString ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}


STDMETHODIMP CVariableDBI::get_Type(IUnknown **ppType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CTypeDBI *pType;

    ASSERT (ppType != NULL);
    if (ppType == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        pType = GetType ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };
    if (pType == NULL)
    {
        return (S_FALSE);
    };

    *ppType = pType->GetUnknown ();
    ASSERT (*ppType != NULL);
    if (*ppType == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppType)->AddRef ();
	return S_OK;
}


STDMETHODIMP CVariableDBI::get_Kind(DBIVariableKind *pKind)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (pKind != NULL);
    if (pKind == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *pKind = GetKind ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}


STDMETHODIMP CVariableDBI::get_Value(VARIANT *pvarValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (pvarValue != NULL);
    if (pvarValue == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        HRESULT hr;
        CComVariant varValue;

        varValue = GetValue ();

        hr = ::VariantCopy (pvarValue, &varValue);
        if (FAILED (hr))
        {
            return (hr);
        };
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}

STDMETHODIMP CVariableDBI::get_OnlString(BSTR *psOnlString)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (psOnlString != NULL);
    if (psOnlString == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *psOnlString = m_pStrOnlString ? m_pStrOnlString->AllocSysString() : GetName().AllocSysString();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}


CVariableList::~CVariableList ()
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Variables.GetSize (); ++iIndex)
    {
        CVariableDBI *pVariableDBI;

        pVariableDBI = m_Variables[iIndex];
        ASSERT (pVariableDBI != NULL);
        if (pVariableDBI == NULL)
            continue;

        pVariableDBI->Release ();
    };
}


HRESULT CVariableList::Load (CDBIContext *pContext, CXMLArchive &ar, CAnyDBI *pParent)
{
    HRESULT hr;

    hr = LoadVariables (pContext, ar, DBIVar, pParent);
    if (FAILED (hr))
    {
        return (hr);
    };

    hr = LoadVariables (pContext, ar, DBIVarInput, pParent);
    if (FAILED (hr))
    {
        return (hr);
    };

    hr = LoadVariables (pContext, ar, DBIVarOutput, pParent);
    if (FAILED (hr))
    {
        return (hr);
    };

    hr = LoadVariables (pContext, ar, DBIVarInOut, pParent);
    if (FAILED (hr))
    {
        return (hr);
    };

    hr = LoadVariables (pContext, ar, DBIVarGlobal, pParent);
    if (FAILED (hr))
    {
        return (hr);
    };

    hr = LoadVariables (pContext, ar, DBIVarExternal, pParent);
    if (FAILED (hr))
    {
        return (hr);
    };

    hr = LoadVariables (pContext, ar, DBIConst, pParent);
    if (FAILED (hr))
    {
        return (hr);
    };

    return (S_OK);
}


void CVariableList::Unload (CAnyDBI *pParent)
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Variables.GetSize (); ++iIndex)
    {
        CVariableDBI *pVariableDBI;

        pVariableDBI = m_Variables[iIndex];
        ASSERT (pVariableDBI != NULL);
        if (pVariableDBI == NULL)
            continue;

        pVariableDBI->RemoveParent (pParent);
        pVariableDBI->Release ();
    };

    m_Variables.RemoveAll ();
}


unsigned long CVariableList::GetCount ()
{
    return (m_Variables.GetSize ());
}


CVariableDBI *CVariableList::GetVariable (unsigned long ulIndex)
{
    return (m_Variables[ulIndex]);
}


CVariableDBI *CVariableList::LookupByName (const CString &strName)
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Variables.GetSize (); ++iIndex)
    {
        CVariableDBI *pVariableDBI;

        pVariableDBI = m_Variables[iIndex];
        ASSERT (pVariableDBI != NULL);
        if (pVariableDBI == NULL)
            continue;

        if (strName.CompareNoCase (pVariableDBI->GetName ()) == 0)
        {
            return (pVariableDBI);
        };
    };

    return (NULL);
}


HRESULT CVariableList::LoadVariables (CDBIContext *pContext, CXMLArchive &ar, DBIVariableKind kind, CAnyDBI *pParent)
{
    CString strTag;

    switch (kind)
    {
    case DBIVar        : strTag = DBI_XML_VAR;         break;
    case DBIVarInput   : strTag = DBI_XML_VARINPUT;    break;
    case DBIVarOutput  : strTag = DBI_XML_VAROUTPUT;   break;
    case DBIVarInOut   : strTag = DBI_XML_VARINOUT;    break;
    case DBIVarGlobal  : strTag = DBI_XML_VARGLOBAL;   break;
    case DBIVarExternal: strTag = DBI_XML_VAREXTERNAL; break;
    case DBIConst      : strTag = DBI_XML_CONST;       break;

    default:
        ASSERT (false);
        return (E_UNEXPECTED);
    };

    try
    {
        if (!ar.MoveDown (strTag))
        {
            return (S_OK);
        };
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    try
    {
        do
        {
            try
            {
                HRESULT hr;
                CComObject<CVariableDBI> *pVariableDBI;

                hr = CComObject<CVariableDBI>::CreateInstance (&pVariableDBI);
                if (FAILED (hr))
                {
                    throw hr;
                };
                ASSERT (pVariableDBI != NULL);
                if (pVariableDBI == NULL)
                {
                    throw E_UNEXPECTED;
                };
                pVariableDBI->AddRef ();

                hr = pVariableDBI->Init (pContext, kind, ar);
                if (FAILED (hr))
                {
                    pVariableDBI->Release ();
                    throw hr;
                };

                pVariableDBI->AddRef ();
                pVariableDBI->AddParent (pParent);
                m_Variables.Add (pVariableDBI);

                pVariableDBI->Release ();
            }
            catch (HRESULT)
            {

                // Do not stop reading variables just
                // because one variable is corrupt.

            };
        } while (ar.MoveNext ());
    }
    catch (HRESULT hr)
    {
        try
        {
            ar.MoveUp ();
        }
        catch (HRESULT)
        {
        };

        return (hr);
    };

    try
    {
        ar.MoveUp ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


HRESULT CVariableList::AddVariable (CDBIContext *pContext, DBIVariableKind kind, const CString &strName, CTypeDBI *pType, CAnyDBI *pParent)
{
    HRESULT hr;
    CComObject<CVariableDBI> *pVariableDBI;

    hr = CComObject<CVariableDBI>::CreateInstance (&pVariableDBI);
    if (FAILED (hr))
    {
        return (hr);
    };
    ASSERT (pVariableDBI != NULL);
    if (pVariableDBI == NULL)
    {
        return (E_UNEXPECTED);
    };
    pVariableDBI->AddRef ();

    hr = pVariableDBI->Init (pContext, kind, strName, pType, NULL);
    if (FAILED (hr))
    {
        pVariableDBI->Release ();
        return (hr);
    };

    pVariableDBI->AddRef ();
    pVariableDBI->AddParent (pParent);
    m_Variables.Add (pVariableDBI);

    pVariableDBI->Release ();

    return (S_OK);
}
