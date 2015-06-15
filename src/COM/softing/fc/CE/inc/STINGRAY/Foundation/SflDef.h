/////////////////////////////////////////////////////////////////////////////
// SflDef.h : SFL External Routines
//
// Stingray Software Windows Template Library
// Copyright (C) 2000 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  SFL External Routines declarations and definitions
//

#if !defined(_SFL)
#define _SFL
#endif

#include <foundation\sflcommon.h>

/////////////////////////////////////////////////////////////////////////////
// Library support

#if !defined(_SFL_MFC_SUPPORT) && defined(_DEFINE_ATL_ROUTINES) && !defined(_SFLLIB)
#if !defined(_LIBFUNCTIONS_DEFINED)
#define _LIBFUNCTIONS_DEFINED
namespace stingray {
namespace foundation {

#if defined(_SFL_ATL_SUPPORT)
HINSTANCE __stdcall GetResourceHandle()
{return _Module.m_hInstResource;}
#endif	// defined(_SFL_ATL_SUPPORT)

};
};


#endif	// defined(_LIBFUNCTIONS_DEFINED)

#else 
namespace stingray {
namespace foundation {

#if defined(_SFLDLL) && !defined(_SFL_MFC_SUPPORT)
	extern HINSTANCE _hinstDll;
#endif

// Just declare the symbol, it will be defined elsewhere
	extern HINSTANCE __stdcall GetResourceHandle();
};
};

#endif	// 

