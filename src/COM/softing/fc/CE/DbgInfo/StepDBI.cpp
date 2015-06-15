
#include "stdafx.h"
#include "StepDBI.h"
#include "XML_Wrapper.h"
#include "DBIContext.h"
#include "TypeDBI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DBI_XML_STEP _T ("STEP")
#define DBI_XML_ID _T ("ID")
#define DBI_XML_NUM _T ("NUM")
#define DBI_XML_X _T ("X")
#define DBI_XML_Y _T ("Y")


CStepDBI::CStepDBI ()
: m_pType (NULL),
  m_lNumber (0),
  m_lX (-1),
  m_lY (-1)
{
}


CStepDBI::~CStepDBI ()
{
    if (m_pType != NULL)
    {
        m_pType->RemoveParent (this);
        m_pType->Release ();
        m_pType = NULL;
    };
}


HRESULT CStepDBI::Init (CDBIContext *pContext, const CString &strName, long lNumber, long lX, long lY)
{
    ASSERT (pContext != NULL);
    if (pContext == NULL)
    {
        return (E_POINTER);
    };

    SetContext (pContext);

    m_strName = strName;
    m_lNumber = lNumber;
    m_lX = lX;
    m_lY = lY;

    m_pType = pContext->GetStepType ();
    if (m_pType == NULL)
    {
        SetContext (NULL);
        return (E_UNEXPECTED);
    };

    m_pType->AddParent (this);

    return (S_OK);
}


HRESULT CStepDBI::Init (CDBIContext *pContext, CXMLArchive &ar)
{
    HRESULT hr;
    CString strName;
    long lNumber;
    long lX;
    long lY;

    try
    {
        if (!ar.GetAttribute (DBI_XML_ID, strName))
        {
            Corrupt ();
            return (E_FAIL);
        };

        if (!ar.GetAttribute (DBI_XML_NUM, lNumber))
        {
            Corrupt ();
            return (E_FAIL);
        };

        if (!ar.GetAttribute (DBI_XML_X, lX))
        {
            lX = -1;
        };

        if (!ar.GetAttribute (DBI_XML_Y, lY))
        {
            lY = -1;
        };

        hr = Init (pContext, strName, lNumber, lX, lY);
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


CString CStepDBI::GetName ()
{
    return (m_strName);
}


CTypeDBI *CStepDBI::GetType ()
{
    return (m_pType);
}


long CStepDBI::GetNumber ()
{
    return (m_lNumber);
}


long CStepDBI::GetX ()
{
    return (m_lX);
}


long CStepDBI::GetY ()
{
    return (m_lY);
}


STDMETHODIMP CStepDBI::get_Name(BSTR *psName)
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


STDMETHODIMP CStepDBI::get_Type(IUnknown **ppType)
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


STDMETHODIMP CStepDBI::get_Number(long *plNumber)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (plNumber != NULL);
    if (plNumber == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *plNumber = GetNumber ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}


STDMETHODIMP CStepDBI::get_X(long *plX)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (plX != NULL);
    if (plX == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *plX = GetX ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}


STDMETHODIMP CStepDBI::get_Y(long *plY)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (plY != NULL);
    if (plY == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *plY = GetY ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}


CStepList::~CStepList ()
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Steps.GetSize (); ++iIndex)
    {
        CStepDBI *pStepDBI;

        pStepDBI = m_Steps[iIndex];
        ASSERT (pStepDBI != NULL);
        if (pStepDBI == NULL)
            continue;

        pStepDBI->Release ();
    };
}


HRESULT CStepList::Load (CDBIContext *pContext, CXMLArchive &ar, CAnyDBI *pParent)
{
    try
    {
        if (!ar.MoveDown (DBI_XML_STEP))
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
                CComObject<CStepDBI> *pStepDBI;

                hr = CComObject<CStepDBI>::CreateInstance (&pStepDBI);
                if (FAILED (hr))
                {
                    throw hr;
                };
                ASSERT (pStepDBI != NULL);
                if (pStepDBI == NULL)
                {
                    throw hr;
                };
                pStepDBI->AddRef ();

                hr = pStepDBI->Init (pContext, ar);
                if (FAILED (hr))
                {
                    pStepDBI->Release ();
                    throw hr;
                };

                pStepDBI->AddRef ();
                pStepDBI->AddParent (pParent);
                m_Steps.Add (pStepDBI);

                pStepDBI->Release ();
            }
            catch (HRESULT)
            {

                // Do not stop reading steps just
                // because one step is corrupt.

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


void CStepList::Unload (CAnyDBI *pParent)
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Steps.GetSize (); ++iIndex)
    {
        CStepDBI *pStepDBI;

        pStepDBI = m_Steps[iIndex];
        ASSERT (pStepDBI != NULL);
        if (pStepDBI == NULL)
            continue;

        pStepDBI->RemoveParent (pParent);
        pStepDBI->Release ();
    };

    m_Steps.RemoveAll ();
}


unsigned long CStepList::GetCount ()
{
    return (m_Steps.GetSize ());
}


CStepDBI *CStepList::GetStep (unsigned long ulIndex)
{
    return (m_Steps[ulIndex]);
}


CStepDBI *CStepList::LookupByNumber (long lNumber)
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Steps.GetSize (); ++iIndex)
    {
        CStepDBI *pStepDBI;

        pStepDBI = m_Steps[iIndex];
        ASSERT (pStepDBI != NULL);
        if (pStepDBI == NULL)
            continue;

        if (lNumber == pStepDBI->GetNumber ())
        {
            return (pStepDBI);
        };
    };

    return (NULL);
}


CStepDBI *CStepList::LookupByName (const CString &strName)
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Steps.GetSize (); ++iIndex)
    {
        CStepDBI *pStepDBI;

        pStepDBI = m_Steps[iIndex];
        ASSERT (pStepDBI != NULL);
        if (pStepDBI == NULL)
            continue;

        if (strName.CompareNoCase (pStepDBI->GetName ()) == 0)
        {
            return (pStepDBI);
        };
    };

    return (NULL);
}


CStepDBI *CStepList::LookupByPosition (long lX, long lY)
{
    int iIndex;
    CStepDBI *pBestStepDBI=NULL;
    long lBestDistance;

    for (iIndex = 0; iIndex < m_Steps.GetSize (); ++iIndex)
    {
        CStepDBI *pStepDBI;
        long lXDistance;
        long lYDistance;
        long lDistance;

        pStepDBI = m_Steps[iIndex];
        ASSERT (pStepDBI != NULL);
        if (pStepDBI == NULL)
            continue;

        if (lX >= 0 && lX != pStepDBI->GetX ())
            continue;

        if (lY >= 0 && lY != pStepDBI->GetY ())
            continue;

        lXDistance = lX >= 0 ? 0 : pStepDBI->GetX () + lX;
        if (lXDistance < 0)
            lXDistance = -lXDistance;

        lYDistance = lY >= 0 ? 0 : pStepDBI->GetY () + lY;
        if (lYDistance < 0)
            lYDistance = -lYDistance;

        lDistance = lXDistance * lXDistance + lYDistance * lYDistance;
        if (pBestStepDBI == NULL || lDistance < lBestDistance)
        {
            lBestDistance = lDistance;
            pBestStepDBI = pStepDBI;
        };

        if (lDistance == 0)
            break;
    };

    return (pBestStepDBI);
}
