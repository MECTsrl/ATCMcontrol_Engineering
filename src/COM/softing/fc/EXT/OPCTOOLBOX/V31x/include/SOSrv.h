//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOSrv                              |
//                                                                            |
//  Filename    : SOSrv.h                                                     |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Server Toolkit classes                                      |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOSRV_H_
#define _SOSRV_H_

#ifdef UNDER_CE
#ifndef SO_WINCE
#define SO_WINCE
#endif
#endif

#ifdef DEBUG
#ifndef _DEBUG
#define _DEBUG
#endif
#endif

// import libraries

#ifndef SOFLAG_NO_FORCE_LIBS
#ifndef SOSRV
#ifndef SO_WINCE
	#if !defined(SOFLAG_DEMO_VERSION)
		#if defined(_DEBUG)
			#if defined(_DLL)
				#if defined(_UNICODE)
					#pragma comment(lib, "SOSrvUDD.lib")
				#else
					#pragma comment(lib, "SOSrvADD.lib")
				#endif
			#else
				#if defined(_UNICODE)
					#pragma comment(lib, "SOSrvUSD.lib")
				#else
					#pragma comment(lib, "SOSrvASD.lib")
				#endif
			#endif
		#else
			#if defined(_DLL)
				#if defined(_UNICODE)
					#pragma comment(lib, "SOSrvUD.lib")
				#else
					#pragma comment(lib, "SOSrvAD.lib")
				#endif
			#else
				#if defined(_UNICODE)
					#pragma comment(lib, "SOSrvUS.lib")
				#else
					#pragma comment(lib, "SOSrvAS.lib")
				#endif
			#endif
		#endif
	#else
		#if !defined(_UNICODE)
			#pragma comment(lib, "SOSrvASO.lib")
		#endif
	#endif
#else
	#if !defined(SOFLAG_DEMO_VERSION)
		#if defined(_DEBUG)
			#pragma comment(lib, "SOSrvD.lib")
		#else
			#pragma comment(lib, "SOSrv.lib")
		#endif
	#else
		#pragma comment(lib, "SOSrvO.lib")
	#endif
#endif
#endif
#endif


// SOSrv defines

#define SOSRV_EVENT_SHUTDOWN	1

#define SOSRV_TIME_40DAYS	3456000000
#define SOSRV_TIME_1DAY		  86400000


#if _MSC_VER == 1100
#ifndef __RPCNDR_H_VERSION__
#define __RPCNDR_H_VERSION__        ( 440 )
#endif // __RPCNDR_H_VERSION__
#define MIDL_INTERFACE(x)   struct __declspec(uuid(x)) __declspec(novtable)
#endif

// all SOCmn and SOSrv headers
#define _ATL_STATIC_REGISTRY

#ifdef SO_WINCE
#include <comdef.h>
_COM_SMARTPTR_TYPEDEF(IUnknown, __uuidof(IUnknown));
_COM_SMARTPTR_TYPEDEF(IDispatch, __uuidof(IDispatch));
#define _com_issue_errorex(a,b,c) { }
#define _com_dispatch_method(a, b, c, d, e, f)
inline void __stdcall _com_issue_error(HRESULT) { };
#endif

#ifndef SOFLAG_NO_FORCE_HEADERS
#include <SOCmn.h>
#include <SOSrvEntry.h>
#include <SOSrvServer.h>
#include <SOSrvBrowser.h>
#include <SOSrvWatch.h>
#endif

#endif
