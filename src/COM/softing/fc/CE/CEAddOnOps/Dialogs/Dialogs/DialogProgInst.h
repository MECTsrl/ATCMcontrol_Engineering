
#if !defined __DIALOGPROGINST__
#define __DIALOGPROGINST__

#include "PropertySheetBase.h"
#include "DialogGenProgInst.h"
#include "DialogInfo.h"
#include "DialogHelp.h"
#include "DialogType.h"

class CDialogProgInst : 
	public CPropertySheetBase,  
	public CDialogGenProgInst,
	public CDialogInfo,
	public CDialogHelp
{
public:
	CDialogProgInst(DIALOGTYPE DialogType);
	virtual ~CDialogProgInst();
    
    CREATE_DELETE_DECL(CDialogProgInst);
};

#endif

