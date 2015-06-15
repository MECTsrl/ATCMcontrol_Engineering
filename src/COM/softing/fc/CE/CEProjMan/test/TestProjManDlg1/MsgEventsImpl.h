// MsgEventsImpl.h: interface for the CMsgEventsImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGEVENTSIMPL_H__1E9D2499_990D_4D27_9780_7F377BBB42F1__INCLUDED_)
#define AFX_MSGEVENTSIMPL_H__1E9D2499_990D_4D27_9780_7F377BBB42F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CEMsg.h"
#include "CEProjMan.h"
#include <atlbase.h>
#include <atlcom.h>

class CTestProjManDlg1Dlg;


class CMsgEventsImpl : public IDispatchImpl<_ICEMessageEvent, 
                                            &IID__ICEMessageEvent, 
                                            &LIBID_CEPROJMANLib>,
						   public CComObjectRoot
{
public:
	CMsgEventsImpl();
	virtual ~CMsgEventsImpl();

public:
    void SetDlgWnd (CTestProjManDlg1Dlg* pDlg);

protected:
    CTestProjManDlg1Dlg* m_pDlg;

private:
	STDMETHOD (OnNewMessage)(BSTR sTargetId, WORD wScrFacility, HRESULT hr, BSTR sMsgText);

	BEGIN_COM_MAP(CMsgEventsImpl)
	    COM_INTERFACE_ENTRY(_ICEMessageEvent)
   	    COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

};



#endif // !defined(AFX_MSGEVENTSIMPL_H__1E9D2499_990D_4D27_9780_7F377BBB42F1__INCLUDED_)
