
#if !defined __DIALOGPB__
#define __DIALOGPB__

#include "PropertySheetBase.h"
#include "DialogGenPB.h"
#include "DialogInfo.h"
#include "DialogHelp.h"
#include "DialogType.h"

class CDialogPB : 
	public CPropertySheetBase,  
	public CDialogGenPB,
	public CDialogInfo,
	public CDialogHelp
{
public:
	CDialogPB(DIALOGTYPE DialogType);
	virtual ~CDialogPB();

    CREATE_DELETE_DECL(CDialogPB);
};

#endif

