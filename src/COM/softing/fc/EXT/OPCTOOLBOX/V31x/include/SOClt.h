//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOClt                              |
//                                                                            |
//  Filename    : SOClt.h                                                     |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Client Toolkit classes                                      |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOCLT_H_
#define _SOCLT_H_

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
#ifndef SOCLT
#ifndef SO_WINCE
	#if !defined(SOFLAG_DEMO_VERSION)
		#if defined(_DEBUG)
			#if defined(_DLL) | defined(_RTLDLL)
				#if defined(_UNICODE)
					#pragma comment(lib, "SOCltUDD.lib")
				#else
					#pragma comment(lib, "SOCltADD.lib")
				#endif
			#else
				#if defined(_UNICODE)
					#pragma comment(lib, "SOCltUSD.lib")
				#else
					#pragma comment(lib, "SOCltASD.lib")
				#endif
			#endif
		#else
			#if defined(_DLL) | defined(_RTLDLL)
				#if defined(_UNICODE)
					#pragma comment(lib, "SOCltUD.lib")
				#else
					#pragma comment(lib, "SOCltAD.lib")
				#endif
			#else
				#if defined(_UNICODE)
					#pragma comment(lib, "SOCltUS.lib")
				#else
					#pragma comment(lib, "SOCltAS.lib")
				#endif
			#endif
		#endif
	#else
		#if !defined(_UNICODE)
			#pragma comment(lib, "SOCltASO.lib")
		#endif
	#endif
#else
	#if !defined(SOFLAG_DEMO_VERSION)
		#if defined(_DEBUG)
			#pragma comment(lib, "SOCltD.lib")
		#else
			#pragma comment(lib, "SOClt.lib")
		#endif
	#else
		#pragma comment(lib, "SOCltO.lib")
	#endif
#endif
#endif
#endif


#if _MSC_VER == 1100
#ifndef __RPCNDR_H_VERSION__
#define __RPCNDR_H_VERSION__        ( 440 )
#endif // __RPCNDR_H_VERSION__
#define MIDL_INTERFACE(x)   struct __declspec(uuid(x)) __declspec(novtable)
#endif

// all SOCmn and SOClt headers

#ifndef SOFLAG_NO_FORCE_HEADERS

#include <SOCmn.h>
#include <opccomn.h>

#include <SOCltElement.h>
#include <SOCltEntry.h>
#include <SOCltStorage.h>
#include <SOCltTask.h>
#include <SOCltThread.h>
#include <SOCltShutdown.h>
#include <SOCltServerBrowser.h>
#include <SOCltServer.h>
#include <SOCltError.h>
#include <SOCltBrowse.h>

#endif

#endif

