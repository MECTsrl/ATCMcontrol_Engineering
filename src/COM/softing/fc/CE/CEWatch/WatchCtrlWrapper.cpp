
#include "stdafx.h"
#include "WatchCtrlWrapper.h"
#include "atlbase.h"
#include "WatchRC.h"


//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNCREATE(CWatchControlWrapper, CWnd)



BOOL CWatchControlWrapper::Create
(
    LPCTSTR lpszWindowName, 
    DWORD   dwStyle,
    const   RECT& rect, 
    CWnd*   pParentWnd, 
    UINT    nID,
	CFile*  pPersist,
    BOOL    bStorage,
	BSTR    bstrLicKey
)
{ 
    if(!CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey))
    {
        ::AfxMessageBox(IDS_ERR_NOWATCH);
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * CEWatchView's SetProjectPath method wrapper. Propagates project
 * path to CEWatchView.
 *
 * @param           path: path to project directory
 * @return          Error code
 * @exception       -
 * @see             CEWatchView default dispatch interface
*/
HRESULT CWatchControlWrapper::SetProjectPath (LPCTSTR pszPath)
{
    HRESULT hr;
    static BYTE parms[] = VTS_BSTR;

    if(this == NULL)
    {
        return -1;
    }

    try
    {
        InvokeHelper (0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pszPath);
    }
    catch (CMemoryException *e)
    {
        e->Delete ();
        return (E_OUTOFMEMORY);
    }
    catch (COleException *e)
    {
        hr = e->m_sc;
        e->Delete ();
        return (hr);
    }
    catch (COleDispatchException *e)
    {
        hr = e->m_scError;
        e->Delete ();
        return (hr);
    };

    return (S_OK);
}


//------------------------------------------------------------------*
/**
 * CEWatchView's Store method wrapper. Stores CEWatchView's content
 * into a file.
 *
 * @param           file: path to target file
 * @return          Error code
 * @exception       -
 * @see             CEWatchView default dispatch interface
*/
HRESULT CWatchControlWrapper::Store (LPCTSTR pszFile)
{
    HRESULT hr;
    BOOL bResult;
    static BYTE parms[] = VTS_BSTR;

    if(this == NULL)
    {
        return -1;
    }

    try
    {
        InvokeHelper (0x2, DISPATCH_METHOD, VT_BOOL, (void*)&bResult, parms, pszFile);
    }
    catch (CMemoryException *e)
    {
        e->Delete ();
        return (E_OUTOFMEMORY);
    }
    catch (COleException *e)
    {
        hr = e->m_sc;
        e->Delete ();
        return (hr);
    }
    catch (COleDispatchException *e)
    {
        hr = e->m_scError;
        e->Delete ();
        return (hr);
    };

    return (bResult ? S_OK : E_FAIL);
}


//------------------------------------------------------------------*
/**
 * CEWatchView's Load method wrapper. Loads content of a file into
 * CEWatchView.
 *
 * @param           file: path to source file
 * @return          Error code
 * @exception       -
 * @see             CEWatchView default dispatch interface
*/
HRESULT CWatchControlWrapper::Load (LPCTSTR pszFile)
{
    HRESULT hr;
    BOOL bResult;
    static BYTE parms[] = VTS_BSTR;

    if(this == NULL)
    {
        return -1;
    }

    try
    {
        InvokeHelper (0x3, DISPATCH_METHOD, VT_BOOL, (void*)&bResult, parms, pszFile);
    }
    catch (CMemoryException *e)
    {
        e->Delete ();
        return (E_OUTOFMEMORY);
    }
    catch (COleException *e)
    {
        hr = e->m_sc;
        e->Delete ();
        return (hr);
    }
    catch (COleDispatchException *e)
    {
        hr = e->m_scError;
        e->Delete ();
        return (hr);
    };

    return (bResult ? S_OK : E_FAIL);
}


//------------------------------------------------------------------*
/**
 * CEWatchView's ForceResubscribe method wrapper. Forces CEWatchView
 * to resubscribe displayed variables/expressions.
 *
 * @param           -
 * @return          Error code
 * @exception       -
 * @see             CEWatchView default dispatch interface
*/
HRESULT CWatchControlWrapper::ForceResubscribe ()
{
    HRESULT hr;

    if(this == NULL)
    {
        return -1;
    }

    try
    {
        InvokeHelper (0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    catch (CMemoryException *e)
    {
        e->Delete ();
        return (E_OUTOFMEMORY);
    }
    catch (COleException *e)
    {
        hr = e->m_sc;
        e->Delete ();
        return (hr);
    }
    catch (COleDispatchException *e)
    {
        hr = e->m_scError;
        e->Delete ();
        return (hr);
    };

    return (S_OK);
}

HRESULT CWatchControlWrapper::SaveToXMLString(CString& rstrXMLString)
{
    HRESULT hr;
    SCODE   sc;
	static BYTE parms[] = VTS_PBSTR;

    if(this == NULL)
    {
        return -1;
    }

    try
    {
        CComBSTR    sXMLString;
        InvokeHelper (0x5, DISPATCH_METHOD, VT_ERROR, &sc, parms, &sXMLString);
        rstrXMLString = sXMLString;
    }
    catch (CMemoryException *e)
    {
        e->Delete ();
        return (E_OUTOFMEMORY);
    }
    catch (COleException *e)
    {
        hr = e->m_sc;
        e->Delete ();
        return (hr);
    }
    catch (COleDispatchException *e)
    {
        hr = e->m_scError;
        e->Delete ();
        return (hr);
    }

    return sc;
}

HRESULT CWatchControlWrapper::LoadFromXMLString(const CString& crstrXMLString)
{
    HRESULT hr;
    SCODE   sc;
	static BYTE parms[] = VTS_PBSTR;

    if(this == NULL)
    {
        return -1;
    }

    try
    {
        CComBSTR    sXMLString(crstrXMLString);
        InvokeHelper (0x7, DISPATCH_METHOD, VT_ERROR, &sc, parms, &sXMLString);
    }
    catch (CMemoryException *e)
    {
        e->Delete ();
        return (E_OUTOFMEMORY);
    }
    catch (COleException *e)
    {
        hr = e->m_sc;
        e->Delete ();
        return (hr);
    }
    catch (COleDispatchException *e)
    {
        hr = e->m_scError;
        e->Delete ();
        return (hr);
    }

    return sc;
}


HRESULT CWatchControlWrapper::Deactivate()
{
    HRESULT hr;

    if(this == NULL)
    {
        return -1;
    }

    try
    {
        InvokeHelper (0x8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    catch (CMemoryException *e)
    {
        e->Delete ();
        return (E_OUTOFMEMORY);
    }
    catch (COleException *e)
    {
        hr = e->m_sc;
        e->Delete ();
        return (hr);
    }
    catch (COleDispatchException *e)
    {
        hr = e->m_scError;
        e->Delete ();
        return (hr);
    };

    return (S_OK);
}

