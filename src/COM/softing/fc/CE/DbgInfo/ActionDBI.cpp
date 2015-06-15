#include "stdafx.h"
#include "ActionDBI.h"
#include "XML_Wrapper.h"
#include "DBIContext.h"
#include "TypeDBI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DBI_XML_ACTION _T ("ACTION")
#define DBI_XML_ID     _T ("ID")
#define DBI_XML_NUM    _T ("NUM")
#define DBI_XML_X      _T ("X")
#define DBI_XML_Y      _T ("Y")



CActionDBI::CActionDBI ()
: m_pType (NULL),
  m_lNumber (0),
  m_lX (-1),
  m_lY (-1)
{
}


CActionDBI::~CActionDBI ()
{
    if (m_pType != NULL)
    {
        m_pType->RemoveParent (this);
        m_pType->Release ();
        m_pType = NULL;
    };
}


HRESULT CActionDBI::Init (CDBIContext *pContext, const CString &strName, long lNumber, long lX, long lY)
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

    m_pType = pContext->GetActionType ();
    if (m_pType == NULL)
    {
        SetContext (NULL);
        return (E_UNEXPECTED);
    };

    m_pType->AddParent (this);

    return (S_OK);
}


HRESULT CActionDBI::Init (CDBIContext *pContext, CXMLArchive &ar)
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


CString CActionDBI::GetName ()
{
    return (m_strName);
}


CTypeDBI *CActionDBI::GetType ()
{
    return (m_pType);
}


long CActionDBI::GetNumber ()
{
    return (m_lNumber);
}


long CActionDBI::GetX ()
{
    return (m_lX);
}


long CActionDBI::GetY ()
{
    return (m_lY);
}


STDMETHODIMP CActionDBI::get_Name(BSTR *psName)
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


STDMETHODIMP CActionDBI::get_Type(IUnknown **ppType)
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


STDMETHODIMP CActionDBI::get_Number(long *plNumber)
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


STDMETHODIMP CActionDBI::get_X(long *plX)
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


STDMETHODIMP CActionDBI::get_Y(long *plY)
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


CActionList::~CActionList ()
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Actions.GetSize (); ++iIndex)
    {
        CActionDBI *pActionDBI;

        pActionDBI = m_Actions[iIndex];
        ASSERT (pActionDBI != NULL);
        if (pActionDBI == NULL)
            continue;

        pActionDBI->Release ();
    };
}


HRESULT CActionList::Load (CDBIContext *pContext, CXMLArchive &ar, CAnyDBI *pParent)
{
    try
    {
        if (!ar.MoveDown (DBI_XML_ACTION))
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
                CComObject<CActionDBI> *pActionDBI;

                hr = CComObject<CActionDBI>::CreateInstance (&pActionDBI);
                if (FAILED (hr))
                {
                    throw hr;
                };
                ASSERT (pActionDBI != NULL);
                if (pActionDBI == NULL)
                {
                    throw hr;
                };
                pActionDBI->AddRef ();

                hr = pActionDBI->Init (pContext, ar);
                if (FAILED (hr))
                {
                    pActionDBI->Release ();
                    throw hr;
                };

                pActionDBI->AddRef ();
                pActionDBI->AddParent (pParent);
                m_Actions.Add (pActionDBI);

                pActionDBI->Release ();
            }
            catch (HRESULT)
            {

                // Do not stop reading actions just
                // because one action is corrupt.

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


void CActionList::Unload (CAnyDBI *pParent)
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Actions.GetSize (); ++iIndex)
    {
        CActionDBI *pActionDBI;

        pActionDBI = m_Actions[iIndex];
        ASSERT (pActionDBI != NULL);
        if (pActionDBI == NULL)
            continue;

        pActionDBI->RemoveParent (pParent);
        pActionDBI->Release ();
    };

    m_Actions.RemoveAll ();
}


unsigned long CActionList::GetCount ()
{
    return (m_Actions.GetSize ());
}


CActionDBI *CActionList::GetAction (unsigned long ulIndex)
{
    return (m_Actions[ulIndex]);
}


CActionDBI *CActionList::LookupByNumber (long lNumber)
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Actions.GetSize (); ++iIndex)
    {
        CActionDBI *pActionDBI;

        pActionDBI = m_Actions[iIndex];
        ASSERT (pActionDBI != NULL);
        if (pActionDBI == NULL)
            continue;

        if (lNumber == pActionDBI->GetNumber ())
        {
            return (pActionDBI);
        };
    };

    return (NULL);
}


CActionDBI *CActionList::LookupByName (const CString &strName)
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Actions.GetSize (); ++iIndex)
    {
        CActionDBI *pActionDBI;

        pActionDBI = m_Actions[iIndex];
        ASSERT (pActionDBI != NULL);
        if (pActionDBI == NULL)
            continue;

        if (strName.CompareNoCase (pActionDBI->GetName ()) == 0)
        {
            return (pActionDBI);
        };
    };

    return (NULL);
}


CActionDBI *CActionList::LookupByPosition (long lX, long lY)
{
    int iIndex;
    CActionDBI *pBestActionDBI=NULL;
    long lBestDistance;

    for (iIndex = 0; iIndex < m_Actions.GetSize (); ++iIndex)
    {
        CActionDBI *pActionDBI;
        long lXDistance;
        long lYDistance;
        long lDistance;

        pActionDBI = m_Actions[iIndex];
        ASSERT (pActionDBI != NULL);
        if (pActionDBI == NULL)
            continue;

        if (lX >= 0 && lX != pActionDBI->GetX ())
            continue;

        if (lY >= 0 && lY != pActionDBI->GetY ())
            continue;

        lXDistance = lX >= 0 ? 0 : pActionDBI->GetX () + lX;
        if (lXDistance < 0)
            lXDistance = -lXDistance;

        lYDistance = lY >= 0 ? 0 : pActionDBI->GetY () + lY;
        if (lYDistance < 0)
            lYDistance = -lYDistance;

        lDistance = lXDistance * lXDistance + lYDistance * lYDistance;
        if (pBestActionDBI == NULL || lDistance < lBestDistance)
        {
            lBestDistance = lDistance;
            pBestActionDBI = pActionDBI;
        };

        if (lDistance == 0)
            break;
    };

    return (pBestActionDBI);
}
