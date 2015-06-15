//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOClt                              |
//                                                                            |
//  Filename    : SOCltEntry.h                                                |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Entry point for OPC Toolbox                                 |
//                - SOCltEntry: Entry point class                             |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOCLTENTRY_H_
#define _SOCLTENTRY_H_

#include "SOCltElement.h"

#pragma pack(push,4)

//-----------------------------------------------------------------------------
// SOCltEntry                                                                 |
//-----------------------------------------------------------------------------

class SOCLT_EXPORT SOCltEntry : 
	public SOCltElement
{
public:
	SOCltEntry(IN DWORD objType);

	virtual BOOL initialize(IN DWORD coInit);	// initialize session
	virtual BOOL terminate(void);							// terminate session
	DWORD getCoInit(void);										// COM init mode

	virtual	LONG release(void);

	static void dispatchCallbacks();
	static void setNotifyWindow(IN HWND hwnd);
	static DWORD s_messageCallbackNotify;

protected:

	BOOL m_terminateLock;			// termination lock
	BOOL m_initialized;				// initialized flag
	DWORD m_coInit;					// COM init mode

// Overrides
  virtual HRESULT doStateTransition(IN BOOL deep);
  virtual HRESULT doRecurseChlidren(SOCmnList<SOCmnElement>* plist, IN BYTE state);

  virtual ~SOCltEntry(void);
}; // SOCltEntry

inline DWORD SOCltEntry::getCoInit(void)
{ return m_coInit; }



//-----------------------------------------------------------------------------
// Demo Version Check
//-----------------------------------------------------------------------------

#ifdef SOFLAG_DEMO_VERSION
extern DWORD SOCLT_EXPORT g_demoStamp;
inline BOOL CHECK_DEMO()
{
	if ( getTimeSpan(g_demoStamp, GetTickCount()) > 180000 ) 
	{
		_TRACEC(SOCMNTRACE_L_ERR, SOCMNTRACE_G_OPC_CALL, _T("DEMO"), (_T("Runtime (3 minutes) of Demo Version elapsed")));
		return TRUE;
	}
	return FALSE;
}
#else
#define CHECK_DEMO() (FALSE)
#endif

#pragma pack(pop)
#endif 
