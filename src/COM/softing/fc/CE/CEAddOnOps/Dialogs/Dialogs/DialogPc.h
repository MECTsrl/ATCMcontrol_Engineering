
#if !defined __DIALOGPC__
#define __DIALOGPC__

#include "PropertySheetBase.h"
#include "DialogGenPc.h"
#include "DialogInfo.h"
#include "DialogHelp.h"
#include "DialogType.h"

class CDialogPc : 
	public CPropertySheetBase,  
	public CDialogGenPc,
	public CDialogInfo,
	public CDialogHelp
{
public:
	CDialogPc(DIALOGTYPE DialogType);
	virtual ~CDialogPc();

    CREATE_DELETE_DECL(CDialogPc);
};

#endif

