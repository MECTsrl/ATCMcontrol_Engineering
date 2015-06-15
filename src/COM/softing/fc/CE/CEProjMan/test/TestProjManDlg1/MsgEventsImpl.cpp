// MsgEventsImpl.cpp: implementation of the CMsgEventsImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestProjManDlg1.h"
#include "MsgEventsImpl.h"
#include "TestProjManDlg1Dlg.h"
#include "CEMsg_i.c"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgEventsImpl::CMsgEventsImpl()
{

}

CMsgEventsImpl::~CMsgEventsImpl()
{

}

void CMsgEventsImpl::SetDlgWnd (CTestProjManDlg1Dlg* pDlg)
{
    m_pDlg = pDlg;
}


STDMETHODIMP CMsgEventsImpl::OnNewMessage(BSTR sTargetId, WORD wScrFacility, HRESULT hr, BSTR sMsgText)
{
    AFX_MANAGE_STATE (AfxGetAppModuleState ());

    if (m_pDlg!=NULL)
    {
        CString str;
        CString strTarget = sTargetId;
        str.Format("%s  %i  %x", strTarget, wScrFacility, hr);
        m_pDlg->AddMsgViewString(str);
        str = sMsgText;
        str = "   " + str;
        m_pDlg->AddMsgViewString(str);
    }
    return S_OK;
}


