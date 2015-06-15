/////////////////////////////////////////////////////////////////////////////
// MvcMsgHandler.h : AFX message handler interface.
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
// Author:       Jeff Boenig and Dean Hallman
// Description:  AFX message handler interface.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/Compatibility/MVC/MvcMsgHandler.h 1     28.02.07 18:58 Ln $

#ifndef __COMPAT_MVCMSGHANDLER_H__
#define __COMPAT_MVCMSGHANDLER_H__

//
// MVC Extension DLL
// Initialize declaration context
//
#ifdef _MVCDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    MVC_DATAEXT
#endif //_MVCDLL

#ifndef _DECORATE_VISUAL_H_NAMES
#define _DECORATE_VISUAL_H_NAMES
#endif

#ifndef __COMPAT_MVCIFCS_H__
#include "Foundation\Compatibility\MVC\MvcIfcs.h"
#endif

//@doc IMvcMsgHandler
//@mfunc BOOL | IMvcMsgHandler | OnWndMsg | A windows message is being delegated to this component for handling
//@parm UINT | message | The windows message being handled
//@parm WPARAM | wParam | Specifies additional message-dependent information.
//@parm LPARAM | lParam | Specifies additional message-dependent information.
//@parm LRESULT* | pResult | The return value
//@xref <c IMvcMsgHandler>

//@doc IMvcMsgHandler
//@mfunc BOOL | IMvcMsgHandler | OnCmdMsg | A command message is being delegated to this component for handling
//@parm UINT | nID | Contains the command ID.
//@parm int | nCode | Identifies the command notification code.
//@parm void* | pExtra | Used according to the value of nCode.
//@parm AFX_CMDHANDLERINFO* | pHandlerInfo | If not NULL, OnCmdMsg fills in the pTarget and pmf members of the pHandlerInfo structure instead of dispatching the command. Typically, this parameter should be NULL.
//@xref <c IMvcMsgHandler>

//@class Interface that supports message handling. Classes that support this interface
// can route and process Windows messages. Basically, this is an interface to a small
// subset of CCmdTarget. Classes derived from CCmdTarget can easily support this interface.
class IMvcMsgHandler
{
public:
	//@cmember
	/* A windows message is being delegated to this component for handling*/
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) = 0;

	//@cmember
	/* The command message is being delegated to this component for handling*/
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) = 0;
};

///////////////////////////////////////////////////////////////////////////

//
// MVC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif  // __COMPAT_MVCMSGHANDLER_H__

