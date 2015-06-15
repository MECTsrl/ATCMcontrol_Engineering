

#include "stdafx.h"
#include "DbgInfo.h"
#include "DebugInfoServer.h"
#include "DBIHolder.h"
#include "ProjectDBI.h"
#include "TypeDBI.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CDBIHolder *DBILookupHolder (const CString &strDebugInfoPath);

// DebugInfoServer.cpp : Implementation of CDebugInfoServer

/////////////////////////////////////////////////////////////////////////////
// CDebugInfoServer





CDebugInfoServer::CDebugInfoServer () 
: m_bInitialized (false),
  m_ModifiedCB (this, OnDebugInfoModified)
{
}


CDebugInfoServer::~CDebugInfoServer ()
{
    if (m_bInitialized)
    {
        Cleanup ();
    };
}


void CDebugInfoServer::OnDebugInfoModified ()
{
    Fire_OnDebugInfoModified ();
}


STDMETHODIMP CDebugInfoServer::Init (BSTR sDebugInfoPath)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr;
    CString strDebugInfoPath;

    strDebugInfoPath = sDebugInfoPath;

    // Strip backslashes from end of pathname
    while (!strDebugInfoPath.IsEmpty () && 
           strDebugInfoPath[strDebugInfoPath.GetLength () - 1] == _T ('\\'))
    {
        strDebugInfoPath = strDebugInfoPath.Left (strDebugInfoPath.GetLength () - 1);
    };

    m_pHolder = DBILookupHolder (strDebugInfoPath);
    if (m_pHolder == NULL)
    {
        m_pHolder = new CDBIHolder (strDebugInfoPath);

        hr = m_pHolder->Init ();
        if (FAILED (hr))
        {
            m_pHolder->Unreference ();
            m_pHolder = NULL;
            return (hr);
        };

        m_pHolder->RegisterModifiedCB (&m_ModifiedCB);
    };

    m_bInitialized = true;
    return (S_OK);
}


STDMETHODIMP CDebugInfoServer::Cleanup ()
{
    m_bInitialized = false;

    if (m_pHolder != NULL)
    {
        m_pHolder->UnregisterModifiedCB (&m_ModifiedCB);
        m_pHolder->Unreference ();
    };

    return (S_OK);
}


STDMETHODIMP CDebugInfoServer::get_Project (IUnknown **ppProject)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CProjectDBI *pProjectDBI;

    ASSERT (ppProject != NULL);
    if (ppProject == NULL)
    {
        return (E_POINTER);
    };
    *ppProject = NULL;

    if (!m_bInitialized)
    {
        return (E_FAIL);
    };

    try
    {
        pProjectDBI = m_pHolder->GetProject ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    if (pProjectDBI == NULL)
    {
        return (S_FALSE);
    };

    *ppProject = pProjectDBI->GetUnknown ();
    ASSERT (*ppProject != NULL);
    if (*ppProject == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppProject)->AddRef ();
    return (S_OK);
}


STDMETHODIMP CDebugInfoServer::GetSimpleType(short sIECType, IUnknown **ppType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CTypeDBI *pTypeDBI;

    ASSERT (ppType != NULL);
    if (ppType == NULL)
    {
        return (E_POINTER);
    };

    ASSERT (sIECType >= 0 && (BL_IEC_TYP)sIECType <= BL_LASTTYPE);
    if (sIECType < 0 || (BL_IEC_TYP)sIECType > BL_LASTTYPE)
    {
        return (E_INVALIDARG);
    };

    try
    {
        pTypeDBI = m_pHolder->GetSimpleType ((BL_IEC_TYP)sIECType);
    }
    catch (HRESULT hr)
    {
        return (hr);
    };
    ASSERT (pTypeDBI != NULL);
    if (pTypeDBI == NULL)
    {
        return (E_UNEXPECTED);
    };

    *ppType = pTypeDBI->GetUnknown ();
    ASSERT (*ppType != NULL);
    if (*ppType == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppType)->AddRef ();
	return (S_OK);
}


STDMETHODIMP CDebugInfoServer::GetConfigurationType(IUnknown **ppType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CTypeDBI *pTypeDBI;

    ASSERT (ppType != NULL);
    if (ppType == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        pTypeDBI = m_pHolder->GetConfigurationType ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };
    ASSERT (pTypeDBI != NULL);
    if (pTypeDBI == NULL)
    {
        return (E_UNEXPECTED);
    };

    *ppType = pTypeDBI->GetUnknown ();
    ASSERT (*ppType != NULL);
    if (*ppType == NULL)
    {
        return (E_UNEXPECTED);
    };

    (*ppType)->AddRef ();
	return (S_OK);
}


STDMETHODIMP CDebugInfoServer::IsCorrupt (BOOL *pbCorrupt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (pbCorrupt != NULL);
    if (pbCorrupt == NULL)
    {
        return (E_POINTER);
    };
    *pbCorrupt = false;

    if (!m_bInitialized)
    {
        return (E_FAIL);
    };

    try
    {
        *pbCorrupt = m_pHolder->IsCorrupt ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

    return (S_OK);
}
