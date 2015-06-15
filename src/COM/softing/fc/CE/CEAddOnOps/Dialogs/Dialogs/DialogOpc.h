
#if !defined __DIALOGOPC__
#define __DIALOGOPC__

#include "PropertySheetBase.h"
#include "DialogGen.h"
#include "DialogInfo.h"
#include "DialogHelp.h"
#include "DialogType.h"

class CDialogOpc : 
	public CPropertySheetBase,  
	public CDialogGen,
	public CDialogInfo,
	public CDialogHelp
{
public:
	CDialogOpc(DIALOGTYPE DialogType);
	virtual ~CDialogOpc();

    CREATE_DELETE_DECL(CDialogOpc);
};

#endif

