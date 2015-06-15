
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*



// ImplMsgSink.cpp : implementation file
//

#include "stdafx.h"
#include "afxctl.h"
#include "ImplMsgSink.h"
#include "cemain.h"
#include "CFacility.h"
#include "utilif.h"
#include "CESysDef.h"
#include "CEMsg_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImplMsgSink

BEGIN_INTERFACE_MAP(CImplMsgSink, CCmdTarget)
    INTERFACE_PART(CImplMsgSink, IID__ICEMessageEvent, MsgEvents)
END_INTERFACE_MAP()

IMPLEMENT_DYNCREATE(CImplMsgSink, CCmdTarget)


//------------------------------------------------------------------*
/**
 * Function name			CImplMsgSink::CImplMsgSink
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CImplMsgSink::CImplMsgSink() 
{
}


//------------------------------------------------------------------*
/**
 * Function name			CImplMsgSink::~CImplMsgSink
 * Description			
 * @return			
 * @exception			
 * @see			
*/
CImplMsgSink::~CImplMsgSink() 
{
    m_listFacilityCodeList.RemoveAll();
}


//------------------------------------------------------------------*
/**
 * Function name    CImplMsgSink::RegisterMsgSource
 * Description			
 * @param           LPUNKNOWN lpUnkMsgSource
 * @param			DWORD* lpdwCookie
 * @return			HRESULT 
 * @exception			
 * @see			    CMsgWnd
*/
HRESULT CImplMsgSink::RegisterMsgSource(LPUNKNOWN lpUnkMsgSource, DWORD* lpdwCookie) 
{
    ASSERT(lpUnkMsgSource != NULL);
    ASSERT(lpdwCookie != NULL);

    LPUNKNOWN           pUnkSink;
    BOOL                bResult;

    HRESULT hr = InternalQueryInterface((IID*)&IID_IUnknown, (void**)&pUnkSink);
    if(FAILED(hr)) 
    {
        return(hr);
    }

    bResult = AfxConnectionAdvise(lpUnkMsgSource, IID__ICEMessageEvent, pUnkSink, TRUE, lpdwCookie);

    pUnkSink->Release();
    return(bResult ? S_OK : E_FAIL);
}


//------------------------------------------------------------------*
/**
 * Function name	CImplMsgSink::UnregisterMsgSource
 * Description			
 * @param			LPUNKNOWN lpUnkMsgSource
 * @param			DWORD dwCookie
 * @return			HRESULT 
 * @exception			
 * @see			    CMsgWnd
*/
HRESULT CImplMsgSink::UnregisterMsgSource(LPUNKNOWN lpUnkMsgSource, DWORD dwCookie) 
{
    ASSERT(lpUnkMsgSource != NULL);

    LPUNKNOWN           pUnkSink;
    BOOL                bResult;

    HRESULT hr = InternalQueryInterface((IID*)&IID_IUnknown, (void**)&pUnkSink);
    if(FAILED(hr)) 
    {
        return(hr);
    }

    bResult = AfxConnectionUnadvise(lpUnkMsgSource, IID__ICEMessageEvent, pUnkSink, TRUE, dwCookie);
    pUnkSink->Release();
    return(bResult ? S_OK : E_FAIL);
}


/////////////////////////////////////////////////////////////////////////////
// embedded interface XMsgEvents methods

STDMETHODIMP_(ULONG) CImplMsgSink::XMsgEvents::AddRef() 
{
	METHOD_PROLOGUE_EX (CImplMsgSink, MsgEvents)
    
	return (pThis->ExternalAddRef());
}

STDMETHODIMP_(ULONG) CImplMsgSink::XMsgEvents::Release() 
{
	METHOD_PROLOGUE_EX (CImplMsgSink, MsgEvents)

	return (pThis->ExternalRelease());
}

STDMETHODIMP CImplMsgSink::XMsgEvents::QueryInterface(REFIID iid, LPVOID *ppvObj) 
{
	METHOD_PROLOGUE_EX (CImplMsgSink, MsgEvents)

	return ((HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj));
}

STDMETHODIMP CImplMsgSink::XMsgEvents::GetTypeInfoCount(UINT* pctinfo) 
{
    return E_NOTIMPL;
}

STDMETHODIMP CImplMsgSink::XMsgEvents::GetTypeInfo(UINT itinfo, LCID lcid,ITypeInfo** pptinfo) 
{
    return E_NOTIMPL;   
}

STDMETHODIMP CImplMsgSink::XMsgEvents::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid) 
{
    return E_NOTIMPL;
}

STDMETHODIMP CImplMsgSink::XMsgEvents::Invoke (DISPID dispid, REFIID riid, LCID lcid,WORD wFlags, DISPPARAMS* pDispParams, LPVARIANT pvarResult,
	LPEXCEPINFO pexcepinfo, UINT* puArgErr) 
{
    return E_NOTIMPL;
}


//------------------------------------------------------------------*
/**
 * Function name	CImplMsgSink::XMsgEvents::OnNewMessage
 * Description			
 * @param			BSTR sMachine
 * @param			WORD wScrFacility
 * @param			HRESULT hr
 * @param			BSTR sDescr
 * @return			STDMETHODIMP 
 * @exception			
 * @see			
*/
STDMETHODIMP CImplMsgSink::XMsgEvents::OnNewMessage(BSTR sMachine, WORD wScrFacility, HRESULT hr, BSTR sDescr) 
{
    METHOD_PROLOGUE_EX(CImplMsgSink, MsgEvents)

    CString strMessage;
    MSG_VIEWER_TYPE RequestedViewer;
    
    assert(pThis);
    //Ask if the facility code is a ATCM facility code
    if(IS_4C_FACILITY(wScrFacility))
    {
        // first build groups by wScrFacility
        switch(wScrFacility) 
        {
        case E_FACILITY_CONTROL:
        case E_FACILITY_IO:
        case E_FACILITY_SYSCOM:
            RequestedViewer = MSG_ControlMsgViewer;
            break;
        case E_FACILITY_CODEGEN:
            RequestedViewer = MSG_CompileMsgViewer;
            break;
        case E_FACILITY_CEDOWNLOAD:
            RequestedViewer = MSG_DownLoadMsgViewer;
            break;
        case E_FACILITY_CEFINDINFILES:
            RequestedViewer = MSG_FindInFilesMsgViewer;
            break;
        case E_FACILITY_XML:
        case E_FACILITY_UTIL:
        case E_FACILITY_BASELIB:
        case E_FACILITY_WIZARD:
        case E_FACILITY_KADMAN:
        case E_FACILITY_ENGINEERING:
        case E_FACILITY_SETUP:
        case E_FACILITY_LIBRARIAN:
        case E_FACILITY_CESYSTEM:
        case E_FACILITY_CEBPMAN:
        case E_FACILITY_CEGR:
        case E_FACILITY_CEST:
        case E_FACILITY_PRJMAN:
            RequestedViewer = MSG_SystemMsgViewer;
            break;
		case E_FACILITY_CEXREF:
            RequestedViewer = MSG_SystemMsgViewer;
            break;
        default:
            RequestedViewer = MSG_SystemMsgViewer;
            break;
        }
    }
    //Else ask if it's an addon facility code
    else if(IS_4C_ADDON_FACILITY(wScrFacility))
    {
        //Look for it in the Target facility code list
        //use pThis to get members from the over class
        //To get the pThis pointer use METHOD_PROLOGUE_EX
        if(pThis->m_listFacilityCodeList.Find(wScrFacility))
        {
            RequestedViewer = MSG_ControlMsgViewer;
        }
        else
        {
            RequestedViewer = MSG_SystemMsgViewer;
        }
    }
    //Otherwise return FALSE
    else
    {
        return(S_FALSE);
    }

    // the ckeck CMsgData for a valid MsgViewer
    CCEMAINApp *pApp = (CCEMAINApp *)AfxGetApp();
	ASSERT(pApp != NULL);
	if(pApp != NULL) 
    {
		CMsgDoc *pDoc = pApp->GetMsgDoc();
		ASSERT(pDoc != NULL);
		if(pDoc != NULL) 
        {
			int iSize = pDoc->m_arrMsgData.GetSize();
			for(int i = 0; i < iSize; i++) 
            {
				CMsgData& MsgData = pDoc->m_arrMsgData[i];

                if(!pThis->IsMessageData(sMachine,MsgData,RequestedViewer)) {
                    //not the correct message data.
                    continue;
                }

                strMessage = sDescr;
                strMessage.TrimRight();
                if (NULL != MsgData.m_hwndClient)
                {
					::SendMessage(MsgData.m_hwndClient, MSGTHRD_NOTIFY, 
                                  i, (LPARAM)(LPCTSTR)strMessage);
                }
			}	
		}
	}
    return(S_OK);
}

BEGIN_MESSAGE_MAP(CImplMsgSink, CCmdTarget)
	//{{AFX_MSG_MAP(CImplMsgSink)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImplMsgSink message handlers


//------------------------------------------------------------------*
/**
 * Function name	CImplMsgSink::AddFacilityCodeToList
 * Description			
 * @param			WORD wScrFacility
 * @return			void 
 * @exception			
 * @see			
*/
void CImplMsgSink::AddFacilityCodeToList(WORD wScrFacility)
{
    m_listFacilityCodeList.AddTail(wScrFacility);   
}

bool CImplMsgSink::IsMessageData(BSTR sMachine,CMsgData & MsgData,MSG_VIEWER_TYPE ReqViewer)
{
    //int iPos;
    CString strAddr;
    CString str;

    if(MsgData.m_MsgViewerType != ReqViewer) 
        return false;

    if(ReqViewer != MSG_ControlMsgViewer) {
        return true;
    }

    //ReqViewer == MSG_ControlMsgViewer
    strAddr = sMachine;
    strAddr.TrimLeft(_T("{["));
    strAddr.TrimRight(_T("]}"));

    str = MsgData.m_pTarget->GetControlAddress();
   

    // old buggy code: addr is more than machine address now
    //iPos = str.Find(strAddr);
    //if(iPos != -1) {
    //    return true;
    //}
    // perhaps this will be better: to decide when discussed what we really get as machine
    // see DCS error entry
    str.TrimLeft(_T("{["));
    str.TrimRight(_T("]}"));
    if (str.CompareNoCase(strAddr)==0)
    {
       return true;
    }

    //sMachine may also be the targets instance name
    str = MsgData.m_pTarget->GetId();
    if(!str.CompareNoCase(strAddr)) { //it's the targets instance name
        return true;
    }

    return false;
}
