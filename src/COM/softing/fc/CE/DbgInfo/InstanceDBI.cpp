
#include "stdafx.h"
#include "InstanceDBI.h"
#include "XML_Wrapper.h"
#include "DBIContext.h"
#include "TypeDBI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DBI_XML_PATH _T ("PATH")


CInstanceDBI::CInstanceDBI ()
: m_pType (NULL)
{
}


CInstanceDBI::~CInstanceDBI ()
{
    if (m_pType != NULL)
    {
        m_pType->RemoveParent (this);
        m_pType->Release ();
    };
}


HRESULT CInstanceDBI::Init (CDBIContext *pContext, CTypeDBI *pType, const CString &strPath)
{
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

    pType->AddRef ();
    pType->AddParent (this);
    m_pType = pType;

    m_strPath = strPath;

    return (S_OK);
}


HRESULT CInstanceDBI::Init (CDBIContext *pContext, CTypeDBI *pType, CXMLArchive &ar)
{
    CString strPath;
    HRESULT hr;

    try
    {
        if (!ar.GetAttribute (DBI_XML_PATH, strPath))
        {
            Corrupt ();
            return (E_FAIL);
        };
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    hr = Init (pContext, pType, strPath);
    if (FAILED (hr))
    {
        return (hr);
    };

    return (S_OK);
}


CTypeDBI *CInstanceDBI::GetType ()
{
    return (m_pType);
}


CString CInstanceDBI::GetPath ()
{
    return (m_strPath);
}


STDMETHODIMP CInstanceDBI::get_Type(IUnknown **ppType)
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
	return (S_OK);
}


STDMETHODIMP CInstanceDBI::get_Path(BSTR *psPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CString strPath;

    ASSERT (psPath != NULL);
    if (psPath == NULL)
    {
        return (E_UNEXPECTED);
    };

    try
    {
        *psPath = GetPath ().AllocSysString ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}
