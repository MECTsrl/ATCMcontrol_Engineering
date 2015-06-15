//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOCmn                              |
//                                                                            |
//  Filename    : SOCmn.h                                                     |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Common classes                                              |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOCMN_H_
#define _SOCMN_H_

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

#include <atlbase.h>

// disable warings
#ifdef _MSC_VER
#pragma warning( disable : 4250 ) 
#pragma warning( disable : 4251 ) 
#pragma warning( disable : 4275 ) 
#pragma warning( disable : 4786 ) 
#pragma warning( disable : 4996 )
#pragma warning( disable : 4183 )
#elif __BORLANDC__
#pragma warn -8071 // Conversion may loose significant digits.
#pragma warn -8027 // Functions containing some return statements are not expanded inline.
#pragma warn -8026 // Functions taking class-by-value argument(s) are not expanded inline.
#pragma warn -8022 // Functions xxx  hides virtual function yyy.
#pragma warn -8057 // xxx  is never used.
#pragma warn -8074 // Structure passed by value
#pragma warn -8060 // Possibly incorect assignment
#pragma warn -8012 // Comparing signed and unsigned values
#pragma warn -8008 // Condition is always false (for DEMO)
#pragma warn -8080 // declared but never used
#endif

// special application defines
#ifdef SOXPS
#define SOFLAG_NO_DLLS
#define SOFLAG_COMBINED_SERVER
#endif

#ifdef SOASS
#define SOFLAG_COMBINED_SERVER
#endif

#ifdef SOFLAG_NO_DLLS
#ifndef SOFLAG_NO_FORCE_LIBS
#define SOFLAG_NO_FORCE_LIBS
#endif
#endif

#ifdef DEMO_VERSION
#define SOFLAG_DEMO_VERSION
#endif

// export or import 

#ifndef SOFLAG_NO_DLLS

#ifdef SOCMN
#define SOCMN_EXPORT __declspec(dllexport)
#else
#define SOCMN_EXPORT __declspec(dllimport)
#endif

#ifdef SOSRV
#define SOSRV_EXPORT __declspec(dllexport)
#else
#define SOSRV_EXPORT __declspec(dllimport)
#endif

#ifdef SOCLT
#define SOCLT_EXPORT __declspec(dllexport)
#else
#define SOCLT_EXPORT __declspec(dllimport)
#endif

#ifdef SODAS
#define SODAS_EXPORT __declspec(dllexport)
#else
#define SODAS_EXPORT __declspec(dllimport)
#endif

#ifdef SODAC
#define SODAC_EXPORT __declspec(dllexport)
#else
#define SODAC_EXPORT __declspec(dllimport)
#endif

#ifdef SOAES
#define SOAES_EXPORT __declspec(dllexport)
#else
#define SOAES_EXPORT __declspec(dllimport)
#endif

#ifdef SOAEC
#define SOAEC_EXPORT __declspec(dllexport)
#else
#define SOAEC_EXPORT __declspec(dllimport)
#endif

#ifdef SOASS
#define SOASS_EXPORT __declspec(dllexport)
#else
#define SOASS_EXPORT __declspec(dllimport)
#endif

#else
#define SOCMN_EXPORT 
#define SOSRV_EXPORT 
#define SOAES_EXPORT 
#define SODAS_EXPORT 
#define SOCLT_EXPORT 
#define SOAEC_EXPORT 
#define SODAC_EXPORT 
#define SOASS_EXPORT 
#endif

// import libraries

#ifndef SOFLAG_NO_FORCE_LIBS
#ifndef SOCMN
#ifndef SO_WINCE
	#if !defined(SOFLAG_DEMO_VERSION)
		#if defined(_DEBUG)
			#if defined(_DLL) | defined(_RTLDLL)
				#if defined(_UNICODE)
					#pragma comment(lib, "SOCmnUDD.lib")
				#else
					#pragma comment(lib, "SOCmnADD.lib")
				#endif
			#else
				#if defined(_UNICODE)
					#pragma comment(lib, "SOCmnUSD.lib")
				#else
					#pragma comment(lib, "SOCmnASD.lib")
				#endif
			#endif
		#else
			#if defined(_DLL) | defined(_RTLDLL)
				#if defined(_UNICODE)
					#pragma comment(lib, "SOCmnUD.lib")
				#else
					#pragma comment(lib, "SOCmnAD.lib")
				#endif
			#else
				#if defined(_UNICODE)
					#pragma comment(lib, "SOCmnUS.lib")
				#else
					#pragma comment(lib, "SOCmnAS.lib")
				#endif
			#endif
		#endif
	#else
		#if !defined(_UNICODE)
			#pragma comment(lib, "SOCmnASO.lib")
		#endif
	#endif
#else
	#if !defined(SOFLAG_DEMO_VERSION)
		#if defined(_DEBUG)
			#pragma comment(lib, "SOCmnD.lib")
		#else
			#pragma comment(lib, "SOCmn.lib")
		#endif
	#else
		#pragma comment(lib, "SOCmnO.lib")
	#endif
#endif
#endif
#endif

// SOCmn defines

#define SOCMN_EVENT_END			0
#define SOCMN_EVENT_WORK		1

#ifndef SO_WINCE
#define SOCMN_ALLOCATE_OBJECT(p, creator)	\
		{										\
		p = new creator;					\
		if (p == NULL)							\
			SOCMN_THROW(SOCmnException(SOCMNEXCEPTION_ID_OUTOFMEMORY, SOCMNEXCEPTION_DESCRIPTION_OUTOFMEMORY)); \
		}
#define SOCMN_ALLOCATE_OBJECT_C(p, creator, cast)	\
		{										\
		p = cast new creator;					\
		if (cast p == NULL)							\
			SOCMN_THROW(SOCmnException(SOCMNEXCEPTION_ID_OUTOFMEMORY, SOCMNEXCEPTION_DESCRIPTION_OUTOFMEMORY)); \
		}
#else
#define SOCMN_ALLOCATE_OBJECT(p, creator)			p = new creator;
#define SOCMN_ALLOCATE_OBJECT_C(p, creator, cast)	p = cast new creator;
#endif
#define SOCMN_FREE_OBJECT(p) { if (p) { delete p; p = NULL; } }
#define SOCMN_FREE_OBJECT_C(p, cast) { if (cast p) { delete cast p; p = NULL; } }
#define SOCMN_FREE_OBJECT_THIS { delete this; }


// SOCmn functions

SOCMN_EXPORT void initSOCmn(void);
SOCMN_EXPORT void exitSOCmn(void);

// all SOCmn headers

#ifndef SOFLAG_NO_FORCE_HEADERS
#include <SOCmnString.h>
#include <SOCmnException.h>
#include <SOCmnSync.h>
#include <SOCmnVariant.h>
#include <SOCmnList.h>
#include <SOCmnTrace.h>
#include <SOCmnEventPool.h>
#include <SOCmnData.h>
#include <SOCmnObject.h>
#include <SOCmnFunctions.h>
#include <SOCmnElement.h>
#endif

#endif
