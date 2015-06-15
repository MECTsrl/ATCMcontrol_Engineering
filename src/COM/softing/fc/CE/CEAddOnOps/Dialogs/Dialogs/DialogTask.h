
#if !defined __DIALOGTASK__
#define __DIALOGTASK__

#include "PropertySheetBase.h"
#include "DialogGenTask.h"
#include "DialogInfo.h"
#include "DialogHelp.h"
#include "DialogType.h"

class CDialogTask : 
	public CPropertySheetBase,  
	public CDialogGenTask,
	public CDialogInfo,
	public CDialogHelp
{
public:
	CDialogTask(DIALOGTYPE DialogType);
	virtual ~CDialogTask();
    
    CREATE_DELETE_DECL(CDialogTask);
};

#endif
