
#if !defined __DIALOGGENTASK__
#define __DIALOGGENTASK__

#include "DialogGenBase.h"
#include "ObjPropGenTask.h"

class CDialogGenTask : public CDialogGenBase 
{
public:
	CDialogGenTask(DIALOGTYPE DialogType, CPropertySheetBase *PropertySheet);
	virtual ~CDialogGenTask();

   	DLLEXPORT void SetAutoStart(BOOL AutoStart);
	DLLEXPORT BOOL GetAutoStart();

	// NFTASKHANDLING 19.05.05 SIS >>	
   	DLLEXPORT void SetLoaded(BOOL bLoaded);
	DLLEXPORT BOOL GetLoaded();
	// NFTASKHANDLING 19.05.05 SIS <<

   	DLLEXPORT void SetInterval(CString Interval);
	DLLEXPORT CString GetInterval();
   	DLLEXPORT void SetIoSync(CString IoSync);
	DLLEXPORT CString GetIoSync();
   	DLLEXPORT void SetPriority(CString Priority);
	DLLEXPORT CString GetPriority();
    // SIS: event driven tasks >>
   	DLLEXPORT void SetEventString(CString EventString);
	DLLEXPORT CString GetEventString();
    // SIS: event driven tasks <<
   	DLLEXPORT void SetWatchdog(CString Watchdog);
	DLLEXPORT CString GetWatchdog();

protected:
	CObjPropGenTask m_ObjPropGenTask;
};

#endif

