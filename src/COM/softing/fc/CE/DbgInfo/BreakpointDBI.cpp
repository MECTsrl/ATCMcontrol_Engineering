#include "stdafx.h"
#include "BreakpointDBI.h"
#include "XML_Wrapper.h"
#include "DBIContext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DBI_XML_NUM _T ("NUM")
#define DBI_XML_KIND _T ("KIND")
#define DBI_XML_X _T ("X")
#define DBI_XML_Y _T ("Y")
#define DBI_XML_CX   ("CX")
#define DBI_XML_CY   ("CY")
#define DBI_QUALIFIER_GRBEGIN ("GRBEGIN")
#define DBI_QUALIFIER_GREND ("GREND")
#define DBI_QUALIFIER_GRBOX ("GRBOX")
#define DBI_QUALIFIER_STLINE ("STLINE")
#define DBI_QUALIFIER_GRSTBOX ("GRSTBOX")
#define DBI_QUALIFIER_GRREGION ("GRREGION")

#define DBI_XML_BP _T ("BP")


CBreakpointDBI::CBreakpointDBI ()
: m_lNumber (0),
  m_Qualifier (EDP_STLINE),
  m_lX (-1),
  m_lY (-1),
  m_lCX(-1),
  m_lCY(-1)
{
}


CBreakpointDBI::~CBreakpointDBI ()
{
}


HRESULT CBreakpointDBI::Init (CDBIContext *pContext, long lNumber, EDP_POS_QUALIFIER qualifier, long lX, long lY, long lCX, long lCY)
{
    ASSERT (pContext != NULL);
    if (pContext == NULL)
    {
        return (E_POINTER);
    };

    SetContext (pContext);

    m_lNumber = lNumber;
    m_Qualifier = qualifier;
    m_lX = lX;
    m_lY = lY;
    m_lCX= lCX;
    m_lCY= lCY;

    return (S_OK);
}


HRESULT CBreakpointDBI::Init (CDBIContext *pContext, CXMLArchive &ar)
{
    HRESULT hr;
    long lNumber;
    CString strKind;
    EDP_POS_QUALIFIER qualifier;
    long lX;
    long lY;
    long lCX;
    long lCY;

    try
    {
        if (!ar.GetAttribute (DBI_XML_NUM, lNumber))
        {
            Corrupt ();
            return (E_FAIL);
        };

        if (!ar.GetAttribute (DBI_XML_KIND, strKind))
        {
            Corrupt ();
            return (E_FAIL);
        };

        if (strKind.CompareNoCase (DBI_QUALIFIER_GRBEGIN) == 0)
        {
            qualifier = EDP_GRBEGIN;
        }
        else if (strKind.CompareNoCase (DBI_QUALIFIER_GREND) == 0)
        {
            qualifier = EDP_GREND;
        }
        else if (strKind.CompareNoCase (DBI_QUALIFIER_GRBOX) == 0)
        {
            qualifier = EDP_GRBOX;
        }
        else if (strKind.CompareNoCase (DBI_QUALIFIER_STLINE) == 0)
        {
            qualifier = EDP_STLINE;
        }
        else if(strKind.CompareNoCase (DBI_QUALIFIER_GRSTBOX) == 0)
        {
            qualifier = EDP_GRSTBOX;
        }
        else if(strKind.CompareNoCase (DBI_QUALIFIER_GRREGION) == 0)
        {
            qualifier = EDP_GRREGION;
        }
        else
        {
            Corrupt ();
            return (E_UNEXPECTED);
        };

        if (!ar.GetAttribute (DBI_XML_X, lX))
        {
            lX = -1;
        };

        if (!ar.GetAttribute (DBI_XML_Y, lY))
        {
            lY = -1;
        };

        if (!ar.GetAttribute (DBI_XML_CX, lCX))
        {
            lCX = -1;
        };

        if (!ar.GetAttribute (DBI_XML_CY, lCY))
        {
            lCY = -1;
        };

        hr = Init (pContext, lNumber, qualifier, lX, lY, lCX, lCY);
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


long CBreakpointDBI::GetNumber ()
{
    return (m_lNumber);
}


EDP_POS_QUALIFIER CBreakpointDBI::GetQualifier ()
{
    return (m_Qualifier);
}


long CBreakpointDBI::GetX ()
{
    return (m_lX);
}

long CBreakpointDBI::GetY ()
{
    return (m_lY);
}

long CBreakpointDBI::GetCX ()
{
    return (m_lCX);
}


long CBreakpointDBI::GetCY ()
{
    return (m_lCY);
}



STDMETHODIMP CBreakpointDBI::get_Number(long *plNumber)
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


STDMETHODIMP CBreakpointDBI::get_Qualifier(EDP_POS_QUALIFIER *pQualifier)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (pQualifier != NULL);
    if (pQualifier == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *pQualifier = GetQualifier ();
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}


STDMETHODIMP CBreakpointDBI::get_X(long *plX)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (plX != NULL);
    if (plX == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *plX = m_lX;
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}


STDMETHODIMP CBreakpointDBI::get_Y(long *plY)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (plY != NULL);
    if (plY == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *plY = m_lY;
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}

STDMETHODIMP CBreakpointDBI::get_CX(long *plCX)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (plCX != NULL);
    if (plCX == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *plCX = m_lCX;
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}

STDMETHODIMP CBreakpointDBI::get_CY(long *plCY)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    ASSERT (plCY != NULL);
    if (plCY == NULL)
    {
        return (E_POINTER);
    };

    try
    {
        *plCY = m_lCY;
    }
    catch (HRESULT hr)
    {
        return (hr);
    };

	return (S_OK);
}


CBreakpointList::~CBreakpointList ()
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Breakpoints.GetSize (); ++iIndex)
    {
        CBreakpointDBI *pBreakpointDBI;

        pBreakpointDBI = m_Breakpoints[iIndex];
        ASSERT (pBreakpointDBI != NULL);
        if (pBreakpointDBI == NULL)
            continue;

        pBreakpointDBI->Release ();
    };
}


HRESULT CBreakpointList::Load (CDBIContext *pContext, CXMLArchive &ar, CAnyDBI *pParent)
{
    try
    {
        if (!ar.MoveDown (DBI_XML_BP))
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
                CComObject<CBreakpointDBI> *pBreakpointDBI;

                hr = CComObject<CBreakpointDBI>::CreateInstance (&pBreakpointDBI);
                if (FAILED (hr))
                {
                    throw hr;
                };
                ASSERT (pBreakpointDBI != NULL);
                if (pBreakpointDBI == NULL)
                {
                    throw hr;
                };
                pBreakpointDBI->AddRef ();

                hr = pBreakpointDBI->Init (pContext, ar);
                if (FAILED (hr))
                {
                    pBreakpointDBI->Release ();
                    throw hr;
                };

                pBreakpointDBI->AddRef ();
                pBreakpointDBI->AddParent (pParent);
                m_Breakpoints.Add (pBreakpointDBI);

                pBreakpointDBI->Release ();
            }
            catch (HRESULT)
            {

                // Do not stop reading breakpoints just
                // because one breakpoint is corrupt.

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


void CBreakpointList::Unload (CAnyDBI *pParent)
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Breakpoints.GetSize (); ++iIndex)
    {
        CBreakpointDBI *pBreakpointDBI;

        pBreakpointDBI = m_Breakpoints[iIndex];
        ASSERT (pBreakpointDBI != NULL);
        if (pBreakpointDBI == NULL)
            continue;

        pBreakpointDBI->RemoveParent (pParent);
        pBreakpointDBI->Release ();
    };

    m_Breakpoints.RemoveAll ();
}


unsigned long CBreakpointList::GetCount ()
{
    return (m_Breakpoints.GetSize ());
}


CBreakpointDBI *CBreakpointList::GetBreakpoint (unsigned long ulIndex)
{
    return (m_Breakpoints[ulIndex]);
}


CBreakpointDBI *CBreakpointList::LookupByNumber (long lNumber)
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Breakpoints.GetSize (); ++iIndex)
    {
        CBreakpointDBI *pBreakpointDBI;

        pBreakpointDBI = m_Breakpoints[iIndex];
        ASSERT (pBreakpointDBI != NULL);
        if (pBreakpointDBI == NULL)
            continue;

        if (lNumber == pBreakpointDBI->GetNumber ())
        {
            return (pBreakpointDBI);
        };
    };

    return (NULL);
}


CBreakpointDBI *CBreakpointList::LookupByPosition(
    EDP_POS_QUALIFIER qualifier, 
    long lX, long lY, long lCX, long lCY
)
{
    int iIndex;

    for (iIndex = 0; iIndex < m_Breakpoints.GetSize (); ++iIndex)
    {
        CBreakpointDBI*   pBreakpointDBI;
        EDP_POS_QUALIFIER BPQual;

        pBreakpointDBI = m_Breakpoints[iIndex];
        ASSERT (pBreakpointDBI != NULL);
        if (pBreakpointDBI == NULL)
            continue;

        BPQual = pBreakpointDBI->GetQualifier();

        switch(qualifier)
        {
        case EDP_STLINE:   
            if(qualifier==BPQual && pBreakpointDBI->GetY() == lY)
                return pBreakpointDBI;

        break;
        case EDP_GRBOX:    
            if(qualifier==BPQual && pBreakpointDBI->GetX() == lX 
                                 && pBreakpointDBI->GetY() == lY)
                return pBreakpointDBI;

        break;
        case EDP_GRSTBOX:  
            ASSERT(lCY>0);
            if(qualifier==BPQual && pBreakpointDBI->GetX() == lX 
                                 && pBreakpointDBI->GetY() == lY
                                 && pBreakpointDBI->GetCY()== lCY)
                return pBreakpointDBI;

        break;
        case EDP_GRREGION:
            if(qualifier==BPQual && pBreakpointDBI->GetX() == lX
                                 && pBreakpointDBI->GetY() == lY
                                 && pBreakpointDBI->GetCX()== lCX 
                                 && pBreakpointDBI->GetCY()== lCY)
                return pBreakpointDBI;

        break;
        case EDP_GRBEGIN:
            if(qualifier==BPQual)
                return pBreakpointDBI;

        break;
        case EDP_GREND:
            if(qualifier==BPQual)
                return pBreakpointDBI;

        break;
        default:
            ASSERT(!"CBreakpointList::LookupByPosition: bad EDP_POS_QUALIFIER"); 
            return NULL;
        }
    };

    return NULL;
}
