//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOAeC                              |
//                                                                            |
//  Filename    : SOAeC.h                                                     |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : OPC Alarms and Events Client Toolkit classes                |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOAEC_H_
#define _SOAEC_H_

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
#ifndef SOAEC
#ifndef SO_WINCE
	#if !defined(SOFLAG_DEMO_VERSION)
		#if defined(_DEBUG)
			#if defined(_DLL)
				#if defined(_UNICODE)
					#pragma comment(lib, "SOAeCUDD.lib")
				#else
					#pragma comment(lib, "SOAeCADD.lib")
				#endif
			#else
				#if defined(_UNICODE)
					#pragma comment(lib, "SOAeCUSD.lib")
				#else
					#pragma comment(lib, "SOAeCASD.lib")
				#endif
			#endif
		#else
			#if defined(_DLL)
				#if defined(_UNICODE)
					#pragma comment(lib, "SOAeCUD.lib")
				#else
					#pragma comment(lib, "SOAeCAD.lib")
				#endif
			#else
				#if defined(_UNICODE)
					#pragma comment(lib, "SOAeCUS.lib")
				#else
					#pragma comment(lib, "SOAeCAS.lib")
				#endif
			#endif
		#endif
	#else
		#if !defined(_UNICODE)
			#pragma comment(lib, "SOAeCASO.lib")
		#endif
	#endif
#else
	#if !defined(SOFLAG_DEMO_VERSION)
		#if defined(_DEBUG)
			#pragma comment(lib, "SOAeCD.lib")
		#else
			#pragma comment(lib, "SOAeC.lib")
		#endif
	#else
		#pragma comment(lib, "SOAeCO.lib")
	#endif
#endif
#endif
#endif


// all SOCmn, SOClt and SOAeC headers

#ifndef SOFLAG_NO_FORCE_HEADERS

#include <atlbase.h>

#include <SOClt.h>
#include <SOCltShutdown.h>

#include <opc_ae.h>
#include <opcaedef.h>
#include <SOAeCEntry.h>
#include <SOAeCServer.h>
#include <SOAeCSubscription.h>
#include <SOAeCEvent.h>
#include <SOAeCBrowse.h>

#endif

#endif
