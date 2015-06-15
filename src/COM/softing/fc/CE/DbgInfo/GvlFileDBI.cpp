
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "GvlFileDBI.h"
#include "XML_Wrapper.h"
#include "DBIContext.h"
#include "TypeDBI.h"

//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DBI_XML_GVL _T("GVL")
#define DBI_XML_FILE _T("FILE")
#define DBI_XML_ID _T("ID")


CGvlFileDBI::CGvlFileDBI ()
{
}


CGvlFileDBI::~CGvlFileDBI ()
{
}


HRESULT CGvlFileDBI::Init (CDBIContext *pContext, const CString &strFileName, const CString& strId)
{
    ASSERT (pContext != NULL);
    if (pContext == NULL)
    {
        return (E_POINTER);
    };

    SetContext (pContext);

    m_strFileName = strFileName;
    m_strId = strId;

    return (S_OK);
}



HRESULT CGvlFileDBI::Init (CDBIContext *pContext, CXMLArchive &ar)
{
    HRESULT hr;
    CString strFileName;
    CString strId;

    ASSERT (pContext != NULL);
    if (pContext == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        if (!ar.GetAttribute (DBI_XML_FILE, strFileName))
        {
            Corrupt ();
            throw E_FAIL;
        };

        if (!ar.GetAttribute (DBI_XML_ID, strId))
        {
            Corrupt ();
            throw E_FAIL;
        };

        hr = Init (pContext, strFileName, strId);
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


CString CGvlFileDBI::GetFileName ()
{
    return (m_strFileName);
}


CString CGvlFileDBI::GetId ()
{
    return (m_strId);
}



STDMETHODIMP CGvlFileDBI::get_FileName(BSTR *psFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (psFileName != NULL);
    if (psFileName == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *psFileName = GetFileName ().AllocSysString ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}


STDMETHODIMP CGvlFileDBI::get_Id(BSTR *psId)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (psId != NULL);
    if (psId == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *psId = GetId().AllocSysString ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}





CGvlFileList::~CGvlFileList ()
{
    int iIndex;

    for (iIndex = 0; iIndex < m_GvlFiles.GetSize (); ++iIndex)
    {
        CGvlFileDBI *pGvlFileDBI;

        pGvlFileDBI = m_GvlFiles[iIndex];
        ASSERT (pGvlFileDBI != NULL);
        if (pGvlFileDBI == NULL)
            continue;

        pGvlFileDBI->Release ();
    };
}


HRESULT CGvlFileList::Load(CDBIContext *pContext, CXMLArchive &ar, CAnyDBI *pParent)
{
    try
    {
        if (!ar.MoveDown (DBI_XML_GVL))
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
                CComObject<CGvlFileDBI> *pGvlFileDBI;

                hr = CComObject<CGvlFileDBI>::CreateInstance (&pGvlFileDBI);
                if (FAILED (hr))
                {
                    throw hr;
                };
                ASSERT (pGvlFileDBI != NULL);
                if (pGvlFileDBI == NULL)
                {
                    throw E_UNEXPECTED;
                };
                pGvlFileDBI->AddRef ();

                hr = pGvlFileDBI->Init (pContext, ar);
                if (FAILED (hr))
                {
                    pGvlFileDBI->Release ();
                    throw hr;
                };

                pGvlFileDBI->AddRef ();
                pGvlFileDBI->AddParent (pParent);
                m_GvlFiles.Add (pGvlFileDBI);

                pGvlFileDBI->Release ();
            }
            catch (HRESULT)
            {

                // Do not stop reading variables just
                // because one GvlFileFile is corrupt.
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


void CGvlFileList::Unload (CAnyDBI *pParent)
{
    int iIndex;

    for (iIndex = 0; iIndex < m_GvlFiles.GetSize (); ++iIndex)
    {
        CGvlFileDBI *pGvlFileDBI;

        pGvlFileDBI = m_GvlFiles[iIndex];
        ASSERT (pGvlFileDBI != NULL);
        if (pGvlFileDBI == NULL)
            continue;

        pGvlFileDBI->RemoveParent (pParent);
        pGvlFileDBI->Release ();
    };

    m_GvlFiles.RemoveAll ();
}


unsigned long CGvlFileList::GetCount ()
{
    return (m_GvlFiles.GetSize ());
}


CGvlFileDBI *CGvlFileList::GetGvlFile (unsigned long ulIndex)
{
    return (m_GvlFiles[ulIndex]);
}


CGvlFileDBI *CGvlFileList::LookupByFileName (const CString &strFileName)
{
    int iIndex;

    for (iIndex = 0; iIndex < m_GvlFiles.GetSize (); ++iIndex)
    {
        CGvlFileDBI *pGvlFileDBI;

        pGvlFileDBI = m_GvlFiles[iIndex];
        ASSERT (pGvlFileDBI != NULL);
        if (pGvlFileDBI == NULL)
            continue;

        if (strFileName.CompareNoCase (pGvlFileDBI->GetFileName ()) == 0)
        {
            return (pGvlFileDBI);
        };
    };

    return (NULL);
}



HRESULT CGvlFileList::AddGvlFile (CDBIContext *pContext, const CString &strFileName, const CString& strId, CAnyDBI *pParent)
{
    HRESULT hr;
    CComObject<CGvlFileDBI> *pGvlFileDBI;

    hr = CComObject<CGvlFileDBI>::CreateInstance (&pGvlFileDBI);
    if (FAILED (hr))
    {
        return (hr);
    };
    ASSERT (pGvlFileDBI != NULL);
    if (pGvlFileDBI == NULL)
    {
        return (E_UNEXPECTED);
    };
    pGvlFileDBI->AddRef ();

    hr = pGvlFileDBI->Init (pContext, strFileName, strId);
    if (FAILED (hr))
    {
        pGvlFileDBI->Release ();
        return (hr);
    };

    pGvlFileDBI->AddRef ();
    pGvlFileDBI->AddParent (pParent);
    m_GvlFiles.Add (pGvlFileDBI);

    pGvlFileDBI->Release ();

    return (S_OK);
}

