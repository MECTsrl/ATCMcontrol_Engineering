
#include "stdafx.h"
#include "TransitionDBI.h"
#include "XML_Wrapper.h"
#include "DBIContext.h"
#include "TypeDBI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DBI_XML_TRANSITION _T ("TRANSITION")
#define DBI_XML_ID _T ("ID")
#define DBI_XML_NUM _T ("NUM")
#define DBI_XML_X _T ("X")
#define DBI_XML_Y _T ("Y")


CTransitionDBI::CTransitionDBI ()
: m_pType (NULL),
  m_lNumber (0),
  m_lX (-1),
  m_lY (-1)
{
}


CTransitionDBI::~CTransitionDBI ()
{
    if (m_pType != NULL)
    {
        m_pType->RemoveParent (this);
        m_pType->Release ();
        m_pType = NULL;
    };
}


HRESULT CTransitionDBI::Init (CDBIContext *pContext, const CString &strName, long lNumber, long lX, long lY)
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

    m_pType = pContext->GetTransitionType ();
    if (m_pType == NULL)
    {
        SetContext (NULL);
        return (E_UNEXPECTED);
    };

    m_pType->AddParent (this);

    return (S_OK);
}


HRESULT CTransitionDBI::Init (CDBIContext *pContext, CXMLArchive &ar)
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


CString CTransitionDBI::GetName ()
{
    return (m_strName);
}


CTypeDBI *CTransitionDBI::GetType ()
{
    return (m_pType);
}


long CTransitionDBI::GetNumber ()
{
    return (m_lNumber);
}


long CTransitionDBI::GetX ()
{
    return (m_lX);
}


long CTransitionDBI::GetY ()
{
    return (m_lY);
}


STDMETHODIMP CTransitionDBI::get_Name(BSTR *psName)
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


STDMETHODIMP CTransitionDBI::get_Type(IUnknown **ppType)
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


STDMETHODIMP CTransitionDBI::get_Number(long *plNumber)
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


STDMETHODIMP CTransitionDBI::get_X(long *plX)
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


STDMETHODIMP CTransitionDBI::get_Y(long *plY)
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


CTransitionList::~CTransitionList ()
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Transitions.GetSize (); ++iIndex)
    {
        CTransitionDBI *pTransitionDBI;

        pTransitionDBI = m_Transitions[iIndex];
        ASSERT (pTransitionDBI != NULL);
        if (pTransitionDBI == NULL)
            continue;

        pTransitionDBI->Release ();
    };
}


HRESULT CTransitionList::Load (CDBIContext *pContext, CXMLArchive &ar, CAnyDBI *pParent)
{
    try
    {
        if (!ar.MoveDown (DBI_XML_TRANSITION))
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
                CComObject<CTransitionDBI> *pTransitionDBI;

                hr = CComObject<CTransitionDBI>::CreateInstance (&pTransitionDBI);
                if (FAILED (hr))
                {
                    throw hr;
                };
                ASSERT (pTransitionDBI != NULL);
                if (pTransitionDBI == NULL)
                {
                    throw hr;
                };
                pTransitionDBI->AddRef ();

                hr = pTransitionDBI->Init (pContext, ar);
                if (FAILED (hr))
                {
                    pTransitionDBI->Release ();
                    throw hr;
                };

                pTransitionDBI->AddRef ();
                pTransitionDBI->AddParent (pParent);
                m_Transitions.Add (pTransitionDBI);

                pTransitionDBI->Release ();
            }
            catch (HRESULT)
            {

                // Do not stop reading transitions just
                // because one transition is corrupt.

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


void CTransitionList::Unload (CAnyDBI *pParent)
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Transitions.GetSize (); ++iIndex)
    {
        CTransitionDBI *pTransitionDBI;

        pTransitionDBI = m_Transitions[iIndex];
        ASSERT (pTransitionDBI != NULL);
        if (pTransitionDBI == NULL)
            continue;

        pTransitionDBI->RemoveParent (pParent);
        pTransitionDBI->Release ();
    };

    m_Transitions.RemoveAll ();
}


unsigned long CTransitionList::GetCount ()
{
    return (m_Transitions.GetSize ());
}


CTransitionDBI *CTransitionList::GetTransition (unsigned long ulIndex)
{
    return (m_Transitions[ulIndex]);
}


CTransitionDBI *CTransitionList::LookupByNumber (long lNumber)
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Transitions.GetSize (); ++iIndex)
    {
        CTransitionDBI *pTransitionDBI;

        pTransitionDBI = m_Transitions[iIndex];
        ASSERT (pTransitionDBI != NULL);
        if (pTransitionDBI == NULL)
            continue;

        if (lNumber == pTransitionDBI->GetNumber ())
        {
            return (pTransitionDBI);
        };
    };

    return (NULL);
}


CTransitionDBI *CTransitionList::LookupByName (const CString &strName)
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Transitions.GetSize (); ++iIndex)
    {
        CTransitionDBI *pTransitionDBI;

        pTransitionDBI = m_Transitions[iIndex];
        ASSERT (pTransitionDBI != NULL);
        if (pTransitionDBI == NULL)
            continue;

        if (strName.CompareNoCase (pTransitionDBI->GetName ()) == 0)
        {
            return (pTransitionDBI);
        };
    };

    return (NULL);
}


CTransitionDBI *CTransitionList::LookupByPosition (long lX, long lY)
{
    int iIndex;
    CTransitionDBI *pBestTransitionDBI=NULL;
    long lBestDistance;

    for (iIndex = 0; iIndex < m_Transitions.GetSize (); ++iIndex)
    {
        CTransitionDBI *pTransitionDBI;
        long lXDistance;
        long lYDistance;
        long lDistance;

        pTransitionDBI = m_Transitions[iIndex];
        ASSERT (pTransitionDBI != NULL);
        if (pTransitionDBI == NULL)
            continue;

        if (lX >= 0 && lX != pTransitionDBI->GetX ())
            continue;

        if (lY >= 0 && lY != pTransitionDBI->GetY ())
            continue;

        lXDistance = lX >= 0 ? 0 : pTransitionDBI->GetX () + lX;
        if (lXDistance < 0)
            lXDistance = -lXDistance;

        lYDistance = lY >= 0 ? 0 : pTransitionDBI->GetY () + lY;
        if (lYDistance < 0)
            lYDistance = -lYDistance;

        lDistance = lXDistance * lXDistance + lYDistance * lYDistance;
        if (pBestTransitionDBI == NULL || lDistance < lBestDistance)
        {
            lBestDistance = lDistance;
            pBestTransitionDBI = pTransitionDBI;
        };

        if (lDistance == 0)
            break;
    };

    return (pBestTransitionDBI);
}
