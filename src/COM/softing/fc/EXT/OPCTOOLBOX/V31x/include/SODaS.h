//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SODaS                              |
//                                                                            |
//  Filename    : SODaS.h                                                     |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : OPC Data Access Server Toolkit classes                      |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SODAS_H_
#define _SODAS_H_

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

// private function called on public group		
#define SODAS_WRN_GRP_PUBLIC		0xA0800101
// connection parameter of COM interface function is not valid
#define SODAS_WRN_BAD_CONNECTION	0xA0800102


// import libraries

#ifndef SOFLAG_NO_FORCE_LIBS
#ifndef SODAS
#ifndef SO_WINCE
	#if !defined(SOFLAG_DEMO_VERSION)
		#if defined(_DEBUG)
			#if defined(_DLL)
				#if defined(_UNICODE)
					#pragma comment(lib, "SODaSUDD.lib")
				#else
					#pragma comment(lib, "SODaSADD.lib")
				#endif
			#else
				#if defined(_UNICODE)
					#pragma comment(lib, "SODaSUSD.lib")
				#else
					#pragma comment(lib, "SODaSASD.lib")
				#endif
			#endif
		#else
			#if defined(_DLL)
				#if defined(_UNICODE)
					#pragma comment(lib, "SODaSUD.lib")
				#else
					#pragma comment(lib, "SODaSAD.lib")
				#endif
			#else
				#if defined(_UNICODE)
					#pragma comment(lib, "SODaSUS.lib")
				#else
					#pragma comment(lib, "SODaSAS.lib")
				#endif
			#endif
		#endif
	#else
		#if !defined(_UNICODE)
			#pragma comment(lib, "SODaSASO.lib")
		#endif
	#endif
#else
	#if !defined(SOFLAG_DEMO_VERSION)
		#if defined(_DEBUG)
			#pragma comment(lib, "SODaSD.lib")
		#else
			#pragma comment(lib, "SODaS.lib")
		#endif
	#else
		#pragma comment(lib, "SODaSO.lib")
	#endif
#endif
#endif
#endif


// all SOCmn, SOSrv and SODaS headers

#ifndef SOFLAG_NO_FORCE_HEADERS
#include <SOSrv.h>
#include <SODaSEntry.h>
#include <SODaSCache.h>
#include <SODaSGroup.h>
#include <SODaSItem.h>
#include <SODaSItemTag.h>
#include <SODaSNode.h>
#include <SODaSProperty.h>
#include <SODaSRequest.h>
#include <SODaSServer.h>
#include <SODaSTag.h>
#include <SODaSTransaction.h>
#include <SODaSWatch.h>
#endif

#endif
