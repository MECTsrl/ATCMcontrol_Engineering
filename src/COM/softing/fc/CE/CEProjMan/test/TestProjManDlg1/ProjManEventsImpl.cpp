// ProjManEventsImpl.cpp: implementation of the CProjManEventsImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestProjManDlg1.h"
#include "ProjManEventsImpl.h"
#include "TestProjManDlg1Dlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProjManEventsImpl::CProjManEventsImpl()
{

}

CProjManEventsImpl::~CProjManEventsImpl()
{

}

void CProjManEventsImpl::SetDlgWnd (CTestProjManDlg1Dlg* pDlg)
{
    m_pDlg = pDlg;
}


/*
STDMETHODIMP CProjManEventsImpl::InvalidateClassTree()
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    if (m_pDlg != NULL)
    {
        m_pDlg->ChangeChangeState(TRUE);
    };

    return (S_OK);
}

STDMETHODIMP CProjManEventsImpl::InvalidateTargetTree()
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    if (m_pDlg != NULL)
    {
        m_pDlg->ChangeChangeState(TRUE);
    };

    return (S_OK);
}
*/

STDMETHODIMP CProjManEventsImpl::InitClassTree(IUnknown* pXMLDocument)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());
    if (m_pDlg!=NULL)
    {
        m_pDlg->AddMsgString("Init Class Tree");
    }
    return (S_OK);
}

STDMETHODIMP CProjManEventsImpl::InitTargetTree(IUnknown* pXMLDocument)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());
    if (m_pDlg!=NULL)
    {
        m_pDlg->AddMsgString("Init Target Tree");
    }
    return (S_OK);
}


STDMETHODIMP CProjManEventsImpl::UpdateClassTree(SAFEARRAY** parrChangedIdPaths)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    if (m_pDlg!=NULL)
    {
        m_pDlg->AddMsgString("Update Class Tree:");
        BSTR HUGEP *pbstr;
        BSTR bstr;
        HRESULT hr;
        unsigned long i;

        // Get a pointer to the elements of the array.
        hr = ::SafeArrayAccessData(*parrChangedIdPaths, (void HUGEP**)&pbstr);
        if (FAILED(hr))
        {
            return E_FAIL;
        }
        for (i = 0; i < ((*parrChangedIdPaths)->rgsabound[0]).cElements; i++)
        {
            CString str;
            bstr = pbstr[i];
            str = "  " + (CString)bstr;
            m_pDlg->AddMsgString(str);
        }

        ::SafeArrayUnaccessData(*parrChangedIdPaths);
    }

    ::SafeArrayDestroy(*parrChangedIdPaths);

    return (S_OK);
}

STDMETHODIMP CProjManEventsImpl::UpdateTargetTree(SAFEARRAY** parrChangedIdPaths)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    if (m_pDlg!=NULL)
    {
        m_pDlg->AddMsgString("Update Target Tree:");
        BSTR HUGEP *pbstr;
        BSTR bstr;
        HRESULT hr;
        unsigned long i;

        // Get a pointer to the elements of the array.
        hr = ::SafeArrayAccessData(*parrChangedIdPaths, (void HUGEP**)&pbstr);
        if (FAILED(hr))
        {
            return E_FAIL;
        }
        for (i = 0; i < ((*parrChangedIdPaths)->rgsabound[0]).cElements; i++)
        {
            CString str;
            bstr = pbstr[i];
            str = "  " + (CString)bstr;
            m_pDlg->AddMsgString(str);
        }

        ::SafeArrayUnaccessData(*parrChangedIdPaths);
    }

    ::SafeArrayDestroy(*parrChangedIdPaths);

    return (S_OK); 
}


STDMETHODIMP CProjManEventsImpl::InvalidateFileView()
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    if (m_pDlg!=NULL)
    {
        m_pDlg->AddMsgString("Invalidate File Tree");
    }
    return S_OK;
}