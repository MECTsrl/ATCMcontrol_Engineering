
#include "stdafx.h"
#include "ProjectDBI.h"
#include "XML_Wrapper.h"
#include "ConfigDBI.h"
#include "DBIContext.h"
#include "ResourceDBI.h"
#include "InstanceDBI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define DBI_XML_PROJECT  _T("PROJECT")
#define DBI_XML_ID       _T("ID")
#define DBI_XML_POUS     _T("POUS")
#define DBI_XML_TARGET   _T("TARGET")


CProjectDBI::CProjectDBI ()
: m_bLoaded (false),
  m_bLoading (false),
  m_Timestamp (0)
{
}


CProjectDBI::~CProjectDBI ()
{
    Unload ();
}


HRESULT CProjectDBI::Init (CDBIContext *pContext)
{
    CString strName;

    ASSERT (pContext != NULL);
    if (pContext == NULL)
    {
        return (E_POINTER);
    };

    SetContext (pContext);

    return (S_OK);
}


CString CProjectDBI::GetName ()
{
    HRESULT hr;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    return (m_strName);
}


unsigned long CProjectDBI::GetConfigurationCount ()
{
    HRESULT hr;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    return (m_Configurations.GetSize ());
}


CConfigurationDBI *CProjectDBI::LookupConfigurationByIndex (unsigned long ulIndex)
{
    HRESULT hr;
    CConfigurationDBI *pConfigurationDBI;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    if (ulIndex >= (unsigned long)m_Configurations.GetSize ())
    {
        throw E_INVALIDARG;
    };

    pConfigurationDBI = m_Configurations[ulIndex];
    ASSERT (pConfigurationDBI != NULL);
    if (pConfigurationDBI == NULL)
    {
        throw E_UNEXPECTED;
    };

    return (pConfigurationDBI);
}


CConfigurationDBI *CProjectDBI::LookupConfigurationByName (const CString &strName)
{
    HRESULT hr;
    int iIndex;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    for (iIndex = 0; iIndex < m_Configurations.GetSize (); ++iIndex)
    {
        CConfigurationDBI *pConfigurationDBI;

        pConfigurationDBI = m_Configurations[iIndex];
        ASSERT (pConfigurationDBI != NULL);
        if (pConfigurationDBI == NULL)
        {
            throw (E_UNEXPECTED);
        };

        if (strName.CompareNoCase (pConfigurationDBI->GetName ()) == 0)
        {
            return (pConfigurationDBI);
        };
    };

	return (NULL);
}


CConfigurationDBI *CProjectDBI::LookupConfigurationByMachine (const CString &strMachine)
{
    HRESULT hr;
    int iIndex;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    for (iIndex = 0; iIndex < m_Configurations.GetSize (); ++iIndex)
    {
        CConfigurationDBI *pConfigurationDBI;

        pConfigurationDBI = m_Configurations[iIndex];
        ASSERT (pConfigurationDBI != NULL);
        if (pConfigurationDBI == NULL)
            continue;

        if (strMachine.CompareNoCase (pConfigurationDBI->GetMachineName ()) == 0)
        {
            return (pConfigurationDBI);
        };
    };

	return (NULL);
}


CConfigurationDBI *CProjectDBI::LookupConfigurationByAddress (const CString &strAddress)
{
    HRESULT hr;
    int iIndex;
    CString str = strAddress;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    //it is possible that the address passed here contains {} encapsulation.
    str.TrimLeft();
    str.TrimRight();
    str.TrimLeft(_T('{'));
    str.TrimRight(_T('}'));

    for (iIndex = 0; iIndex < m_Configurations.GetSize (); ++iIndex)
    {
        CConfigurationDBI *pConfigurationDBI;

        pConfigurationDBI = m_Configurations[iIndex];
        ASSERT (pConfigurationDBI != NULL);
        if (pConfigurationDBI == NULL)
            continue;

        if (str.CompareNoCase (pConfigurationDBI->GetMachineAddress ()) == 0)
        {
            return (pConfigurationDBI);
        };
    };

	return (NULL);
}


bool CProjectDBI::LookupInstanceByPath (const CString &strPath, CConfigurationDBI *&pConfigurationDBI, CResourceDBI *&pResourceDBI, CInstanceDBI *&pInstanceDBI)
{
    HRESULT hr;
    CString strConfiguration;
    CString strRemainder=strPath;
    int iPos;

    hr = Load ();
    if (FAILED (hr))
    {
        throw hr;
    };

    pConfigurationDBI = NULL;
    pResourceDBI = NULL;
    pInstanceDBI = NULL;

    strConfiguration = StripConfiguration (strPath, strRemainder);
    if (strConfiguration.IsEmpty ())
    {
        throw (E_INVALIDARG);
    };

    if (strConfiguration[0] == _T ('{'))
    {
        iPos = strConfiguration.Find (_T ('}'));
        if (iPos == -1)
        {
            throw (E_INVALIDARG);
        };

        //it's an address - hence lookup by address
        pConfigurationDBI = LookupConfigurationByAddress (strConfiguration.Mid (1, iPos - 1));
    }
    else
    {
        pConfigurationDBI = LookupConfigurationByName (strConfiguration);
    };

    if (pConfigurationDBI == NULL)
    {
        return (false);
    };

    if (strRemainder.IsEmpty ())
    {
        return (true);
    };

    return (pConfigurationDBI->LookupInstanceByPath (strRemainder, pResourceDBI, pInstanceDBI));
}


CString CProjectDBI::StripConfiguration (const CString &strPath, CString &strRemainder)
{
    int iPos;

    if (strPath[0] == _T ('{'))
    {
        iPos = strPath.Find (_T ('}'));
        if (iPos == -1)
        {
            strRemainder = strPath;
            return ("");
        };

        strRemainder = strPath.Mid (iPos + 1);
        strRemainder.TrimLeft(_T('.'));
        return (strPath.Left (iPos+1));
    };

    iPos = strPath.Find (_T ('.'));
    if (iPos == -1)
    {
        strRemainder = "";
        return (strPath);
    };

    strRemainder = strPath.Mid (iPos + 1);
    return (strPath.Left (iPos));
}


HRESULT CProjectDBI::Load ()
{
    HRESULT hr;
    CString strDBIFile;
    CFile file;
    CString strName;
    CFileStatus fileStatus;

    if (m_bLoaded)
    {
        return (S_OK);
    };

    // Currently loading? Recursive call!!!
    if (m_bLoading)
    {
        return (E_FAIL);
    };
    m_bLoading = true;

    strDBIFile.Format ("%s\\%s", (LPCTSTR)m_pContext->GetDebugInfoPath (), PROJECT_DBI_FILE);

    if (!file.Open (strDBIFile, CFile::modeRead | CFile::shareDenyWrite))
    {
        m_bLoading = false;
        return (S_FALSE);
    };

    try
    {
        CXMLArchive ar (&file, CArchive::load);

        try
        {
            ar.Load ();

            if (!ar.MoveDown (DBI_XML_PROJECT))
            {
                Corrupt ();
                throw E_FAIL;
            };
        }
        catch (HRESULT hr)
        {
            try
            {
                ar.Close ();
            }
            catch (HRESULT)
            {
            };

            m_bLoading = false;
            return (hr);
        };

        try
        {
            if (!ar.GetAttribute (DBI_XML_ID, strName))
            {
                Corrupt ();
                throw E_FAIL;
            };
            m_strName = strName;

            hr = LoadPOUs (ar);
            if (FAILED (hr))
            {
                throw hr;
            };
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

            try
            {
                ar.Close ();
            }
            catch (HRESULT)
            {
            };

            m_bLoading = false;
            return (hr);
        };

        try
        {
            ar.MoveUp ();
        }
        catch (HRESULT hr)
        {
            try
            {
                ar.Close ();
            }
            catch (HRESULT)
            {
            };

            return (hr);
        };

        ar.Close ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    m_bLoaded = true;
    m_bLoading = false;

    if (!CFile::GetStatus (strDBIFile, fileStatus))
    {
        fileStatus.m_mtime = 0;
    }
    else
    {
        m_Timestamp = fileStatus.m_mtime;
    };

    return (S_OK);
}


HRESULT CProjectDBI::LoadPOUs (CXMLArchive &ar)
{
    try
    {
        if (!ar.MoveDown (DBI_XML_TARGET))
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
                CComObject<CConfigurationDBI> *pConfigurationDBI;
                CComBSTR sName;

                hr = CComObject<CConfigurationDBI>::CreateInstance (&pConfigurationDBI);
                if (FAILED (hr))
                {
                    throw hr;
                };
                ASSERT (pConfigurationDBI != NULL);
                if (pConfigurationDBI == NULL)
                {
                    throw E_UNEXPECTED;
                };
                pConfigurationDBI->AddRef ();

                hr = pConfigurationDBI->Init (m_pContext, ar);
                if (FAILED (hr))
                {
                    pConfigurationDBI->Release ();
                    throw hr;
                };

                pConfigurationDBI->AddRef ();
                pConfigurationDBI->AddParent (this);
                m_Configurations.Add (pConfigurationDBI);

                pConfigurationDBI->Release ();
            }
            catch (HRESULT)
            {

                // Do not stop reading configurations just
                // because one configuration is corrupt.

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


void CProjectDBI::Unload ()
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Configurations.GetSize (); ++iIndex)
    {
        CConfigurationDBI *pConfigurationDBI;

        pConfigurationDBI = m_Configurations[iIndex];
        ASSERT (pConfigurationDBI != NULL);
        if (pConfigurationDBI == NULL)
            continue;

        pConfigurationDBI->RemoveParent (this);
        pConfigurationDBI->Release ();
    };

    m_bLoaded = false;
}


STDMETHODIMP CProjectDBI::get_Name(BSTR *psName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CString strName;

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


STDMETHODIMP CProjectDBI::get_ConfigurationCount (unsigned long *pulCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (pulCount != NULL);
    if (pulCount == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *pulCount = GetConfigurationCount ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}


STDMETHODIMP CProjectDBI::get_ConfigurationByIndex(unsigned long ulIndex, IUnknown **ppConfiguration)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CConfigurationDBI *pConfigurationDBI;

    ASSERT (ppConfiguration != NULL);
    if (ppConfiguration == NULL)
    {
        return (E_POINTER);
    };
    *ppConfiguration = NULL;

    try
    {
        pConfigurationDBI = LookupConfigurationByIndex (ulIndex);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    *ppConfiguration = pConfigurationDBI->GetUnknown ();
    ASSERT (*ppConfiguration != NULL);
    if (*ppConfiguration == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppConfiguration)->AddRef ();
	return (S_OK);
}


STDMETHODIMP CProjectDBI::get_ConfigurationByName(BSTR sName, IUnknown **ppConfiguration)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CConfigurationDBI *pConfigurationDBI;

    ASSERT (ppConfiguration != NULL);
    if (ppConfiguration == NULL)
    {
        return (E_POINTER);
    };
    *ppConfiguration = NULL;

    ASSERT (sName != NULL);
    if (sName == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        pConfigurationDBI = LookupConfigurationByName (sName);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    if (pConfigurationDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppConfiguration = pConfigurationDBI->GetUnknown ();
    ASSERT (*ppConfiguration != NULL);
    if (*ppConfiguration == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppConfiguration)->AddRef ();
    return (S_OK);
}


STDMETHODIMP CProjectDBI::get_ConfigurationByMachine(BSTR sMachine, IUnknown **ppConfiguration)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CConfigurationDBI *pConfigurationDBI;

    ASSERT (ppConfiguration != NULL);
    if (ppConfiguration == NULL)
    {
        return (E_POINTER);
    };
    *ppConfiguration = NULL;

    ASSERT (sMachine != NULL);
    if (sMachine == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        pConfigurationDBI = LookupConfigurationByMachine (sMachine);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    if (pConfigurationDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppConfiguration = pConfigurationDBI->GetUnknown ();
    ASSERT (*ppConfiguration != NULL);
    if (*ppConfiguration == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppConfiguration)->AddRef ();
    return (S_OK);
}


STDMETHODIMP CProjectDBI::get_ConfigurationByAddress(BSTR sAddress, IUnknown **ppConfiguration)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CConfigurationDBI *pConfigurationDBI;

    ASSERT (ppConfiguration != NULL);
    if (ppConfiguration == NULL)
    {
        return (E_POINTER);
    };
    *ppConfiguration = NULL;

    ASSERT (sAddress != NULL);
    if (sAddress == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        pConfigurationDBI = LookupConfigurationByAddress (sAddress);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    if (pConfigurationDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppConfiguration = pConfigurationDBI->GetUnknown ();
    ASSERT (*ppConfiguration != NULL);
    if (*ppConfiguration == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppConfiguration)->AddRef ();
    return (S_OK);
}


STDMETHODIMP CProjectDBI::get_InstanceByPath(BSTR sPath, IUnknown **ppInstance)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CConfigurationDBI *pConfigurationDBI;
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
        if (!LookupInstanceByPath (sPath, pConfigurationDBI, pResourceDBI, pInstanceDBI))
        {
            return (S_FALSE);
        };
    }
    catch (HRESULT hr)
    {
        return (hr);
    };


    // If LookupInstanceByPath returned true at
    // least the configuration must be non-NULL

    ASSERT (pConfigurationDBI != NULL);
    if (pConfigurationDBI == NULL)
    {
        return (E_UNEXPECTED);
    };

    if (pInstanceDBI != NULL)
    {
        *ppInstance = pInstanceDBI->GetUnknown ();
    }
    else if (pResourceDBI != NULL)
    {
        *ppInstance = pResourceDBI->GetUnknown ();
    }
    else
    {
        *ppInstance = pConfigurationDBI->GetUnknown ();
    };

    ASSERT (*ppInstance != NULL);
    if (*ppInstance == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppInstance)->AddRef ();
	return (S_OK);
}


STDMETHODIMP CProjectDBI::GetInstancesForDataType
(
    /*[in, string]*/ BSTR   sDataType,
    /*[out]*/ SAFEARRAY**   pparrInstancePaths
)
{
    if (pparrInstancePaths == NULL)
    {
        return E_POINTER;
    }

    CString         strDataType(sDataType); // data type of instances
    CStringArray    astrInstancePaths;      // array to collect instance paths

    // run over all configurations and resources
    int iNumConf, iConf;
    CConfigurationDBI*  pConfig;

    HRESULT hr;

    hr = Load();
    if(hr != S_OK)
    {
        return hr;
    }

    iNumConf = m_Configurations.GetSize();

    for(iConf = 0; iConf < iNumConf; ++iConf)
    {
        pConfig = m_Configurations[iConf];
        pConfig->AddInstancesForDataType(strDataType, astrInstancePaths);
    }
    
    long lNumInstances = astrInstancePaths.GetSize();

    // now fill safe array with instances
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
            CComBSTR sInstancePath(astrInstancePaths[lInstance]);
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
