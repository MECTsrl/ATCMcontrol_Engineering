#if !defined(AFX_XREFWINDOW_H__07293FE1_4CD1_402A_8967_0377A01C5987__INCLUDED_)
#define AFX_XREFWINDOW_H__07293FE1_4CD1_402A_8967_0377A01C5987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "CECompMan.h"
#include "CESysDef.h"

#include "XRef.h"

#include "SizedControlBar.h"



//------------------------------------------------------------------*
/**
 * CXRefCtrlWrapper
 *
 * @see             
*/
class CXRefCtrlWrapper : public CWnd
{
protected:
	DECLARE_DYNCREATE(CXRefCtrlWrapper)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xadc08007, 0x5182, 0x4324, { 0x93, 0x9a, 0xd7, 0x73, 0x83, 0x4f, 0xde, 0x93 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:

// Operations
public:
	void LinkXRef();
	void ChangeRefPosition(long bForward);
	void SetActiveQuery(LPCTSTR sVariableName);
    void InitXRef();
	// OSAI feature print XRef 24.03.04 SIS >>
	BOOL GetPrintString(CString& rstrContent, int iLineLength, int iMode = -1);	// XREFPRINT 21.07.05 SIS
	// OSAI feature print XRef 24.03.04 SIS <<
};







class CXRefWindow;



class CXRefEventSink :
        public IDispatchImpl<_IXRefCtrlEvents, 
            &IID__IXRefCtrlEvents,
            &LIBID_XREFLib>,
        public CComObjectRoot
{
public:
    CXRefEventSink(CXRefWindow* pXRefWindow = NULL)
    {
        m_pXRefWindow = pXRefWindow;
    }

    STDMETHOD(GetProjectManager)(IUnknown** ppUnk);
	STDMETHOD(ShowInEditor)(BSTR sEdiPosInfo);
	STDMETHOD(OnPrint)();						// XREFPRINT 21.07.05 SIS

	BEGIN_COM_MAP(CXRefEventSink)
	    COM_INTERFACE_ENTRY(_IXRefCtrlEvents)
   	    COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

    CXRefWindow* m_pXRefWindow;

};



class CXRefWindow  :  public CSizedControlBar //SECDialogBar //public CSizedDialogBar
{
    DECLARE_DYNCREATE(CXRefWindow)

    FC_CALLBACK(CEStateChangeCB, CXRefWindow) m_StateChangeCB;

public:
	CXRefWindow();
	virtual ~CXRefWindow();
    
    CXRefCtrlWrapper* GetWrapper();
    void InitXRef();
	// OSAI feature print XRef 24.03.04 SIS >>
	BOOL PrintFile(CString& rstrFilePathName, int iLineLength, int iMode = -1);	// XREFPRINT 21.07.05 SIS
	// OSAI feature print XRef 24.03.04 SIS <<

protected:
    CXRefCtrlWrapper m_ControlWrapper;

    void OnStateChange (CEStateNotify stateNotify, unsigned int uiStage);

public:
     //{{AFX_MSG(CPouVarSelWindow)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
//    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg BOOL OnEraseBkgnd (CDC *pDC);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    HRESULT AdviseXRefEventHandler(); 
    void UnadviseXRefEventHandler();
    
    CComObject<CXRefEventSink> *m_pXRefEventHandler;
	DWORD                       m_dwXRefEventHandlerCookie;
    DWORD                       m_dwMsgEventHandlerCookie;
};










#endif // !defined(AFX_XREFWINDOW_H__07293FE1_4CD1_402A_8967_0377A01C5987__INCLUDED_)



