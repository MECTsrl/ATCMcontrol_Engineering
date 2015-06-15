//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SODaC                              |
//                                                                            |
//  Filename    : SODaC.h                                                     |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : OPC Data Access Client Toolkit classes                      |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SODAC_H_
#define _SODAC_H_

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
#ifndef SODAC
#ifndef SO_WINCE
	#if !defined(SOFLAG_DEMO_VERSION)
		#if defined(_DEBUG)
			#if defined(_DLL) | defined(_RTLDLL)
				#if defined(_UNICODE)
					#pragma comment(lib, "SODaCUDD.lib")
				#else
					#pragma comment(lib, "SODaCADD.lib")
				#endif
			#else
				#if defined(_UNICODE)
					#pragma comment(lib, "SODaCUSD.lib")
				#else
					#pragma comment(lib, "SODaCASD.lib")
				#endif
			#endif
		#else
			#if defined(_DLL) | defined(_RTLDLL)
				#if defined(_UNICODE)
					#pragma comment(lib, "SODaCUD.lib")
				#else
					#pragma comment(lib, "SODaCAD.lib")
				#endif
			#else
				#if defined(_UNICODE)
					#pragma comment(lib, "SODaCUS.lib")
				#else
					#pragma comment(lib, "SODaCAS.lib")
				#endif
			#endif
		#endif
	#else
		#if !defined(_UNICODE)
			#pragma comment(lib, "SODaCASO.lib")
		#endif
	#endif
#else
	#if !defined(SOFLAG_DEMO_VERSION)
		#if defined(_DEBUG)
			#pragma comment(lib, "SODaCD.lib")
		#else
			#pragma comment(lib, "SODaC.lib")
		#endif
	#else
		#pragma comment(lib, "SODaCO.lib")
	#endif
#endif
#endif
#endif


// all SOCmn, SOClt and SODaC headers

#ifndef SOFLAG_NO_FORCE_HEADERS

#include <SOClt.h>
#include <opcda.h>

#include <SODaCCallback.h>
#include <SODaCEntry.h>
#include <SODaCGroup.h>
#include <SODaCItem.h>
#include <SODaCServer.h>
#include <SODaCBrowse.h>
#endif

#endif
