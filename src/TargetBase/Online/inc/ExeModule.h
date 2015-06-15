/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/ExeModule.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ExeModule.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/ExeModule.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _EXEMODULE_H_
#define _EXEMODULE_H_

class CExeModule : public CComModule
{
public:
	HRESULT                 RegisterServer();
	HRESULT                 UnregisterServer();
    HRESULT                 RegisterServerInstance(LPCTSTR lpszParam);
    HRESULT                 UnregisterServerInstance(LPCTSTR lpszParam);
	void                    Init();
    DWORD                   Run();
    void                    MonitorShutdown();
    bool                    StartMonitor();
    LONG                    Unlock();

public:
    DWORD                   dwThreadID;
    bool                    bActivity;
};

#endif

/* ---------------------------------------------------------------------------- */
