/////////////////////////////////////////////////////////////////////////////
// plugin.h : Declaration of SECWndPlugIn
//
// Stingray Software Extension Classes
// Copyright (C) 1997 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Authors:      Stefan Hoenig, Dean Hallman, Justin Rudd
// Description:  Declaration of SECWndPlugIn
//

#ifdef WIN32

#ifndef __SFL_PLUGIN_H__
#define __SFL_PLUGIN_H__

//
// CMN Extension DLL
// Initialize declaration context
//
#ifdef _SFLDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SFL_DATAEXT
#endif //_SFLDLL

// Constants

// Macros

// Plugin specific message map macro that returns NULL for CWnd::messageMap queries

#if (_MFC_VER <= 0x0421)
#ifndef AFX_COMDAT
#define AFX_COMDAT
#endif
#endif

#ifdef _AFXDLL
#define BEGIN_NOCWND_MESSAGE_MAP(theClass, baseClass) \
	const AFX_MSGMAP* PASCAL theClass::_GetBaseMessageMap() \
		{ return NULL; } \
	const AFX_MSGMAP* theClass::GetMessageMap() const \
		{ return &theClass::messageMap; } \
	AFX_COMDAT AFX_DATADEF const AFX_MSGMAP theClass::messageMap = \
	{ &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] }; \
	AFX_COMDAT const AFX_MSGMAP_ENTRY theClass::_messageEntries[] = \
	{ \

#else
#define BEGIN_NOCWND_MESSAGE_MAP(theClass, baseClass) \
	const AFX_MSGMAP* theClass::GetMessageMap() const \
		{ return &theClass::messageMap; } \
	AFX_COMDAT AFX_DATADEF const AFX_MSGMAP theClass::messageMap = \
	{ NULL, &theClass::_messageEntries[0] }; \
	AFX_COMDAT const AFX_MSGMAP_ENTRY theClass::_messageEntries[] = \
	{ \

#endif

namespace stingray {
namespace foundation {

// Types

// Forward Declarations

/////////////////////////////////////////////////////////////////////////////
// SECWndPlugIn


//@doc SECWndPlugIn 
//@mdata BOOL | SECWndPlugIn  | m_bNeedDefaultProcessing | Normally, SECWndPlugIn will not forward default processing of a message to CWnd::DefWindowProc.
// This is because you could have multiple plug-ins doing default processing for the same message. However, it is necessary in some cases to allow default
// processing. In this case, set m_bNeedDefaultProcessing to TRUE in the message handler before calling the base class handler. For example, if your
// plug-in has a handler for WM_SETCURSOR ( OnSetCursor ) you will want default processing or the cursor will not behave properly. Simply set m_bNeedDefaultProcessing = TRUE
// before calling SECWndPlugIn::OnSetCursor(...) and the SECWndPlugIn::DefWindowProc will delegate to CWnd::DefWindowProc. m_bNeedDefaultProcessing is
// reset to FALSE in every OnWndMsg.
//@xref <c SECWndPlugIn>


//@doc SECWndPlugIn 
//@class Window "plugin" for tapping into a particular hWnd's message map.
//@base public | CWnd
//@comm An object of this class will attach to a particular hWnd to hook into the message map.
class SECWndPlugIn : public CWnd
{
	DECLARE_DYNAMIC(SECWndPlugIn);

	//---------------------------------------------------------------------//
	// 						   Public Interface							   //
	//---------------------------------------------------------------------//
public:

	// Initialization/Destruction

	//@cmember
	/* Constructor*/
	SECWndPlugIn();
	virtual ~SECWndPlugIn();

	//@access Attributes

	//@cmember
	/* Set to TRUE inside your message handler if you don't want any other plug ins or default message handling done*/
	BOOL m_bExitMessage;

	//@cmember
	/* Set to TRUE inside your message handler to enable CWnd::DefWindowProc processing.*/
	BOOL m_bNeedDefaultProcessing;
	
	//@access Operations

	//@cmember
	/* Specify intercepted message was handled, and set return code for this message. */
	virtual void SetMessageHandled(BOOL bRC=TRUE);
	
	//---------------------------------------------------------------------//
	// 							 Overridables							   //
	//---------------------------------------------------------------------//
public:
	//@cmember
	/* Plug in to the specific window passed in.*/
	virtual BOOL PlugInTo( CWnd* pWnd );
	BOOL PlugIn(CWnd* pWnd) { return PlugInTo(pWnd); }
	virtual void UnPlug();
	virtual BOOL IsPlugged() { return m_bPluggedIn; } 

protected:

	//---------------------------------------------------------------------//
	// 						Protected Implementation					   //
	//---------------------------------------------------------------------//
protected:

	// Implementation Member Variables
	BOOL m_bMsgHandled,m_bMsgRC,m_bPluggedIn;

	// Implementation Member Functions

	//---------------------------------------------------------------------//
	// 						  Base Class Overrides						   //
	//---------------------------------------------------------------------//

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECWndPlugIn)
	public:
	virtual BOOL OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult );
	virtual BOOL OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );
	protected:
	virtual LRESULT DefWindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	//}}AFX_VIRTUAL
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

	//---------------------------------------------------------------------//
	// 						Message Map Declarations					   //
	//---------------------------------------------------------------------//

	// Generated message map functions
	//{{AFX_MSG(SECWndPlugIn)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//---------------------------------------------------------------------//
	// 						Private Implementation						   //
	//---------------------------------------------------------------------//
private:

};

};  // namespace stingray::foundation
};  // namespace stingray

/////////////////////////////////////////////////////////////////////////////

//
// CMN Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR


#endif //__SFL_PLUGIN_H__

#endif // WIN32


