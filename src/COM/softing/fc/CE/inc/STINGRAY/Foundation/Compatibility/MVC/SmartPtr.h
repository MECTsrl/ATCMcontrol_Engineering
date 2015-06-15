
// MVC Extension DLL
// Initialize declaration context
#ifdef _MVCDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA MVC_DATAEXT
#endif // _MVCDLL

#include <Foundation\SmartPtr.h>

// MVC Extension DLL
// Reset declaration context
#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

