#if !defined(AFX_IMPLMSGSINK_H__D2113BF4_0CB9_11D2_8845_006097E1BE6E__INCLUDED_)
#define AFX_IMPLMSGSINK_H__D2113BF4_0CB9_11D2_8845_006097E1BE6E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ImplMsgSink.h : header file
//

#include "CEMsg.h"
#include "CEMsgViewer.h"
#include "msgdoc.h"

class CMsgData;

/////////////////////////////////////////////////////////////////////////////
// CImplMsgSink command target

class CImplMsgSink : public CCmdTarget
{
	DECLARE_DYNCREATE(CImplMsgSink)

	CImplMsgSink();           // protected constructor used by dynamic creation
	virtual ~CImplMsgSink();
// Operations
public:
	void    AddFacilityCodeToList(WORD wScrFacility);
    HRESULT RegisterMsgSource(LPUNKNOWN lpUnkMsgSource, DWORD* lpdwCookie);
    HRESULT UnregisterMsgSource(LPUNKNOWN lpUnkMsgSource, DWORD dwCookie);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImplMsgSink)
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CImplMsgSink)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
    
    BEGIN_INTERFACE_PART(MsgEvents, _ICEMessageEvent)
        STDMETHOD(GetTypeInfoCount)(UINT* pctinfo);
        STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid,ITypeInfo** pptinfo);
        STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid);
        STDMETHOD(Invoke)(DISPID dispid, REFIID riid, LCID lcid,WORD wFlags, DISPPARAMS* pDispParams, LPVARIANT pvarResult,
	        LPEXCEPINFO pexcepinfo, UINT* puArgErr);
        STDMETHOD(OnNewMessage)(BSTR sMachine, WORD wScrFacility, HRESULT hr, BSTR sDescr);
    END_INTERFACE_PART(MsgEvents)

    DECLARE_INTERFACE_MAP()
private:
	CList<WORD, WORD>    m_listFacilityCodeList;

//  private Methods
    bool IsMessageData(BSTR,CMsgData &,MSG_VIEWER_TYPE);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPLMSGSINK_H__D2113BF4_0CB9_11D2_8845_006097E1BE6E__INCLUDED_)


