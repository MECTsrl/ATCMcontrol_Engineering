
#include "stdafx.h"
#include "ConfigDBI.h"
#include "ResourceDBI.h"
#include "XML_Wrapper.h"
#include "DBIContext.h"
#include "InstanceDBI.h"
#include "TypeDBI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DBI_XML_ID _T ("ID")
#define DBI_XML_RESOURCEREF _T ("RESOURCEREF")
#define DBI_XML_PCNAME _T ("PCNAME")
#define DBI_XML_PCADDR _T ("PCADDR")


CConfigurationDBI::CConfigurationDBI ()
: m_pType (NULL)
{
}


CConfigurationDBI::~CConfigurationDBI ()
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Resources.GetSize (); ++iIndex)
    {
        CResourceDBI *pResourceDBI;

        pResourceDBI = m_Resources[iIndex];
        ASSERT (pResourceDBI != NULL);
        if (pResourceDBI == NULL)
            continue;

        pResourceDBI->RemoveParent (this);

        pResourceDBI->Release ();
    };

    if (m_pType != NULL)
    {
        m_pType->RemoveParent (this);
        m_pType->Release ();
        m_pType = NULL;
    };
}


HRESULT CConfigurationDBI::Init (CDBIContext *pContext, const CString &strName, const CString &strMachineName, const CString &strMachineAddress)
{
    ASSERT (pContext != NULL);
    if (pContext == NULL)
    {
        return (E_POINTER);
    };

    SetContext (pContext);

    m_strName = strName;
    m_strMachineName = strMachineName;
    m_strMachineAddress = strMachineAddress;

    m_pType = m_pContext->GetConfigurationType ();
    if (m_pType == NULL)
    {
        SetContext (NULL);

        m_strName.Empty ();
        m_strMachineName.Empty ();
        m_strMachineAddress.Empty ();

        return (E_UNEXPECTED);
    };

    m_pType->AddParent (this);

    return (S_OK);
}


HRESULT CConfigurationDBI::Init (CDBIContext *pContext, CXMLArchive &ar)
{
    HRESULT hr;
    CString strName;
    CString strMachineName;
    CString strMachineAddress;

    try
    {
        if (!ar.GetAttribute (DBI_XML_ID, strName))
        {
            Corrupt ();
            return (E_FAIL);
        };

        if (!ar.GetAttribute (DBI_XML_PCNAME, strMachineName))
        {
            Corrupt ();
            return (E_FAIL);
        };

        if (!ar.GetAttribute (DBI_XML_PCADDR, strMachineAddress))
        {
            Corrupt ();
            return (E_FAIL);
        };
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    hr = Init (pContext, strName, strMachineName, strMachineAddress);
    if (FAILED (hr))
    {
        return (hr);
    };

    try
    {
        if (!ar.MoveDown (DBI_XML_RESOURCEREF))
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
                CComObject<CResourceDBI> *pResourceDBI;

                hr = CComObject<CResourceDBI>::CreateInstance (&pResourceDBI);
                if (FAILED (hr))
                {
                    throw hr;
                };
                ASSERT (pResourceDBI != NULL);
                if (pResourceDBI == NULL)
                {
                    throw hr;
                };
                pResourceDBI->AddRef ();

                hr = pResourceDBI->Init (pContext, ar);
                if (FAILED (hr))
                {
                    pResourceDBI->Release ();
                    throw hr;
                };

                pResourceDBI->AddRef ();
                pResourceDBI->AddParent (this);
                m_Resources.Add (pResourceDBI);

                pResourceDBI->Release ();
            }
            catch (HRESULT)
            {

                // Do not stop reading resources just
                // because one resource is corrupt.

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


CString CConfigurationDBI::GetName ()
{
    return (m_strName);
}


CTypeDBI *CConfigurationDBI::GetType ()
{
    return (m_pType);
}


CString CConfigurationDBI::GetMachineName ()
{
    return (m_strMachineName);
}


CString CConfigurationDBI::GetMachineAddress ()
{
    return (m_strMachineAddress);
}


unsigned long CConfigurationDBI::GetResourceCount ()
{
    return (m_Resources.GetSize ());
}


CResourceDBI *CConfigurationDBI::LookupResourceByIndex (unsigned long ulIndex)
{
    CResourceDBI *pResourceDBI;

    if (ulIndex >= (unsigned long)m_Resources.GetSize ())
    {
        throw E_INVALIDARG;
    };

    pResourceDBI = m_Resources[ulIndex];
    ASSERT (pResourceDBI != NULL);
    if (pResourceDBI == NULL)
    {
        throw E_UNEXPECTED;
    };

    return (pResourceDBI);
}


CResourceDBI *CConfigurationDBI::LookupResourceByName (const CString &strName)
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Resources.GetSize (); ++iIndex)
    {
        CResourceDBI *pResourceDBI;

        pResourceDBI = m_Resources[iIndex];
        ASSERT (pResourceDBI != NULL);
        if (pResourceDBI == NULL)
        {
            throw (E_UNEXPECTED);
        };

        if (strName.CompareNoCase (pResourceDBI->GetName ()) == 0)
        {
            return (pResourceDBI);
        };
    };

	return (NULL);
}


bool CConfigurationDBI::LookupInstanceByPath (const CString &strPath, CResourceDBI *&pResourceDBI, CInstanceDBI *&pInstanceDBI)
{
    int iPos;
    CString strResource;
    CString strRemainder;

    pResourceDBI = NULL;
    pInstanceDBI = NULL;

    iPos = strPath.Find (_T ('.'));
    if (iPos == -1)
    {
        strResource = strPath;
        strRemainder = "";
    }
    else
    {
        strResource = strPath.Left (iPos);
        strRemainder = strPath.Mid (iPos + 1);
    };

    if (strResource.IsEmpty ())
    {
        throw (E_INVALIDARG);
    };

    pResourceDBI = LookupResourceByName (strResource);
    if (pResourceDBI == NULL)
    {
        return (false);
    };

    if (strRemainder.IsEmpty ())
    {
        return (true);
    };

    pInstanceDBI = pResourceDBI->LookupInstanceByPath (strRemainder);
    if (pInstanceDBI == NULL)
    {
        return (false);
    };

    return (true);
}


//------------------------------------------------------------------*
/**
 * add instances for data type
 *
 * @param           [in] crstrDataType: data type string, e. g. "Program1"
 * @param           [in/out] rastrInstancePaths: string array of instance paths
 * @return          -
 * @exception       -
 * @see             
*/
void CConfigurationDBI::AddInstancesForDataType(const CString& crstrDataType, CStringArray& rastrInstancePaths)
{
    int             iNumRes, iRes;
    CResourceDBI*   pRes;
    CString         strInstPath = m_strMachineName;

    iNumRes = GetResourceCount();

    for(iRes = 0; iRes < iNumRes; ++iRes)
    {
        pRes = m_Resources[iRes];
        pRes->AddInstancesForDataType(crstrDataType, strInstPath, rastrInstancePaths);
    }
}


STDMETHODIMP CConfigurationDBI::get_Name(BSTR *psName)
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


STDMETHODIMP CConfigurationDBI::get_Type(IUnknown **ppType)
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


STDMETHODIMP CConfigurationDBI::get_MachineName(BSTR *psName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (psName != NULL);
    if (psName == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *psName = GetMachineName ().AllocSysString ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


STDMETHODIMP CConfigurationDBI::get_MachineAddress(BSTR *psAddress)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (psAddress != NULL);
    if (psAddress == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *psAddress = GetMachineAddress ().AllocSysString ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


STDMETHODIMP CConfigurationDBI::get_ResourceCount(unsigned long *pulCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (pulCount != NULL);
    if (pulCount == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *pulCount = GetResourceCount ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}


STDMETHODIMP CConfigurationDBI::get_ResourceByIndex(unsigned long ulIndex, IUnknown **ppResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CResourceDBI *pResourceDBI;

    ASSERT (ppResource != NULL);
    if (ppResource == NULL)
    {
        return (E_POINTER);
    };
    *ppResource = NULL;

    try
    {
        pResourceDBI = LookupResourceByIndex (ulIndex);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    *ppResource = pResourceDBI->GetUnknown ();
    ASSERT (*ppResource != NULL);
    if (*ppResource == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppResource)->AddRef ();
	return (S_OK);
}


STDMETHODIMP CConfigurationDBI::get_ResourceByName (BSTR sName, IUnknown **ppResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CResourceDBI *pResourceDBI;

    ASSERT (ppResource != NULL);
    if (ppResource == NULL)
    {
        return (E_POINTER);
    };
    *ppResource = NULL;

    ASSERT (sName != NULL);
    if (sName == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        pResourceDBI = LookupResourceByName (sName);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    if (pResourceDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppResource = pResourceDBI->GetUnknown ();
    ASSERT (*ppResource != NULL);
    if (*ppResource == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppResource)->AddRef ();
    return (S_OK);
}


STDMETHODIMP CConfigurationDBI::get_InstanceByPath(BSTR sPath, IUnknown **ppInstance)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CString strPath;
    CResourceDBI *pResourceDBI;
    CInstanceDBI *pInstanceDBI;

    ASSERT (ppInstance != NULL);
    if (ppInstance == NULL)
    {
        return (E_POINTER);
    };
    *ppInstance = NULL;

    ASSERT (sPath != NULL);
    if (sPath == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        if (!LookupInstanceByPath (sPath, pResourceDBI, pInstanceDBI))
        {
            return (S_FALSE);
        };
    }
    catch (HRESULT hr)
    {
        return (hr);
    };


    // If LookupInstanceByPath returned true at
    // least the resource must be non-NULL

    ASSERT (pResourceDBI != NULL);
    if (pResourceDBI == NULL)
    {
        return (E_UNEXPECTED);
    };

    if (pInstanceDBI != NULL)
    {
        *ppInstance = pInstanceDBI->GetUnknown ();
    }
    else
    {
        *ppInstance = pResourceDBI->GetUnknown ();
    };

    ASSERT (*ppInstance != NULL);
    if (*ppInstance == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppInstance)->AddRef ();
	return (S_OK);
}
