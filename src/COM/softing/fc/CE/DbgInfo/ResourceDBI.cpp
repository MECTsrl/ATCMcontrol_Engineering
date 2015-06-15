
#include "stdafx.h"
#include "ResourceDBI.h"
#include "XML_Wrapper.h"
#include "VariableDBI.h"
#include "InstanceDBI.h"
#include "DBIContext.h"
#include "TypeDBI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DBI_XML_ID _T ("ID")
#define DBI_XML_DBI _T ("DBI")


CResourceDBI::CResourceDBI ()
: m_pType (NULL)
{
}


CResourceDBI::~CResourceDBI ()
{
    if (m_pType != NULL)
    {
        m_pType->RemoveParent (this);
        m_pType->Release ();
    };
}


HRESULT CResourceDBI::Init (CDBIContext *pContext, const CString &strName, CTypeDBI *pType)
{
    ASSERT (pContext != NULL);
    if (pContext == NULL)
    {
        return (E_POINTER);
    };

    SetContext (pContext);

    m_strName = strName;

    pType->AddParent (this);
    m_pType = pType;

    return (S_OK);
}


HRESULT CResourceDBI::Init (CDBIContext *pContext, CXMLArchive &ar)
{
    CString strName;
    CString strDBIFile;
    HRESULT hr;
    CTypeDBI *pType;

    try
    {
        if (!ar.GetAttribute (DBI_XML_ID, strName))
        {
            Corrupt ();
            throw E_FAIL;
        };

        if (!ar.GetAttribute (DBI_XML_DBI, strDBIFile))
        {
            Corrupt ();
            throw E_FAIL;
        };

        hr = pContext->BuildDataType (strName, strDBIFile, strDBIFile, pType);
        if (FAILED (hr))
        {
            throw hr;
        };

        hr = Init (pContext, strName, pType);
        if (FAILED (hr))
        {
            throw hr;
        };
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}


CString CResourceDBI::GetName ()
{
    return (m_strName);
}


CTypeDBI *CResourceDBI::GetType ()
{
    return (m_pType);
}


CInstanceDBI *CResourceDBI::LookupInstanceByPath (const CString &strPath)
{
    if (strPath.IsEmpty ())
    {
        throw (E_INVALIDARG);
    };

    if (m_pType == NULL)
    {
        return (NULL);
    };

    return (m_pType->LookupInstanceByPath (strPath));
}

//------------------------------------------------------------------*
/**
 * add instances for data type
 *
 * @param           [in] crstrDataType: data type string, e. g. "Program1"
 * @param           [in/out] rstrInstPath: instance path up to resource, e. g. "PC1.Resource1"
 * @param           [in/out] rastrInstancePaths: string array of instance paths
 * @return          -
 * @exception       -
 * @see             
*/
void CResourceDBI::AddInstancesForDataType(const CString& crstrDataType, CString& rstrInstPath, CStringArray& rastrInstancePaths)
{
    HRESULT hr;
    hr = m_pType->Load();
    if(hr != S_OK)
    {
        TRACE("CResourceDBI: Error in Load()\n");
        return;
    }

    // special way for gvls
    if(crstrDataType.Right(4).CompareNoCase(_T(".gvl")) == 0)
    {
        AddGvlInstance(crstrDataType, rstrInstPath, rastrInstancePaths);
        return;
    }

    CTypeDBI::InstanceList* pInstanceList = m_pType->LookupInstanceList(crstrDataType);

    if(!pInstanceList)
    {
        return;
    }

    long lNumInstances = pInstanceList->m_Instances.GetSize();
    CInstanceDBI*   pInstanceDBI;
    CString         strInstPath;

    for (long lInstance = 0; lInstance < lNumInstances; ++lInstance)
    {
        pInstanceDBI = pInstanceList->m_Instances[lInstance];
        strInstPath = rstrInstPath + _T(".") + m_strName + _T(".") + pInstanceDBI->m_strPath;
        rastrInstancePaths.Add(strInstPath);
    }
}


//------------------------------------------------------------------*
/**
 * add gvl instance.
 *
 *  search, whether gvl import exists and add instance path.
 *
 * @param           [in] crstrFileName: gvl file name
 * @param           [in/out] rstrInstPath: instance path up to resource, e. g. "PC1.Resource1"
 * @param           [in/out] rastrInstancePaths: string array of instance paths
 * @return          -
 * @exception       -
 * @see             -
*/
void CResourceDBI::AddGvlInstance(const CString& crstrFileName, CString& rstrInstPath, CStringArray& rastrInstancePaths)
{
    CGvlFileList* pGvlFileList = m_pType->GetGvlFileList();
    if(!pGvlFileList)
    {
        return;
    }

    long lNumGvlFiles = pGvlFileList->GetCount();
    CGvlFileDBI*    pGvlFileDBI;
    CString         strGvlInstPath;

    for(long lGvlFile = 0; lGvlFile < lNumGvlFiles; ++lGvlFile)
    {
        pGvlFileDBI = pGvlFileList->GetGvlFile(lGvlFile);
        ASSERT(pGvlFileDBI);
        if(pGvlFileDBI)
        {
            if(crstrFileName.CompareNoCase(pGvlFileDBI->GetFileName()) == 0)
            {
                strGvlInstPath = rstrInstPath + _T(".") + m_strName;
                rastrInstancePaths.Add(strGvlInstPath);
                break;
            }
        }
    }
}

STDMETHODIMP CResourceDBI::get_Name(BSTR *psName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (psName != NULL);
    if (psName == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *psName = m_strName.AllocSysString ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}


STDMETHODIMP CResourceDBI::get_Type(IUnknown **ppType)
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


STDMETHODIMP CResourceDBI::get_VariableCount(unsigned long *pulCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_pType == NULL)
    {
        return (S_FALSE);
    };

    return (m_pType->get_VariableCount (pulCount));
}


STDMETHODIMP CResourceDBI::get_VariableByIndex(unsigned long ulIndex, IUnknown **ppVariable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_pType == NULL)
    {
        return (S_FALSE);
    };

    return (m_pType->get_VariableByIndex (ulIndex, ppVariable));
}


STDMETHODIMP CResourceDBI::get_VariableByName(BSTR sName, IUnknown **ppVariable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_pType == NULL)
    {
        return (S_FALSE);
    };

    return (m_pType->get_VariableByName (sName, ppVariable));
}


STDMETHODIMP CResourceDBI::get_InstanceCount(IUnknown *pType, unsigned long *pulCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_pType == NULL)
    {
        return (S_FALSE);
    };

    return (m_pType->get_InstanceCount (pType, pulCount));
}


STDMETHODIMP CResourceDBI::get_InstanceByIndex(IUnknown *pType, unsigned long ulIndex, IUnknown **ppInstance)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (m_pType == NULL)
    {
        return (S_FALSE);
    };

    return (m_pType->get_InstanceByIndex (pType, ulIndex, ppInstance));
}


STDMETHODIMP CResourceDBI::get_InstanceByPath(BSTR sPath, IUnknown **ppInstance)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
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
        pInstanceDBI = LookupInstanceByPath (sPath);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };
    if (pInstanceDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppInstance = pInstanceDBI->GetUnknown ();
    ASSERT (*ppInstance != NULL);
    if (*ppInstance == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppInstance)->AddRef ();
    return (S_OK);
}


STDMETHODIMP CResourceDBI::GetInstancesForDataType
(
    /*[in, string]*/ BSTR   sDataType,
    /*[out]*/ SAFEARRAY**   pparrInstancePaths
)
{
    if (pparrInstancePaths == NULL)
    {
        return E_POINTER;
    }

    // run over instance list
    CString strDataType(sDataType);
    CTypeDBI::InstanceList* pInstanceList = m_pType->LookupInstanceList(strDataType);

    if(!pInstanceList)
    {
        return E_FAIL;
    }

    long lNumInstances = pInstanceList->m_Instances.GetSize();
    CInstanceDBI*   pInstanceDBI;

    // now fill safe array with instances
    HRESULT         hr;
    SAFEARRAYBOUND  Bound;

    Bound.lLbound = 0;
    Bound.cElements = lNumInstances;

    *pparrInstancePaths = ::SafeArrayCreate(VT_BSTR, 1, &Bound);
    if (NULL == (*pparrInstancePaths)) 
    {
        return E_FAIL;    
    }

    try
    {
        for (long lInstance = 0; lInstance < lNumInstances; ++lInstance)
        {
            pInstanceDBI = pInstanceList->m_Instances[lInstance];

            CComBSTR sInstancePath(pInstanceDBI->m_strPath);
            hr = ::SafeArrayPutElement(*pparrInstancePaths, &lInstance, 
                                        (void*)sInstancePath);
            if (FAILED(hr))
            {
                ::SafeArrayDestroy(*pparrInstancePaths);
                return E_FAIL;
            }
        }
    }
    catch (CMemoryException* e)
    {
        e->Delete();
        return E_OUTOFMEMORY;
    }
    return S_OK;
}

