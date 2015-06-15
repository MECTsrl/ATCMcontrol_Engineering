
#if !defined __DIALOGCONST__
#define __DIALOGCONST__

#include "PropertySheetBase.h"
#include "DialogGenConst.h"
#include "DialogType.h"
#include "GlobConst.h"

class CDialogConst : 
	public CPropertySheetBase,  
	public CDialogGenConst
{
public:
    CDialogConst(DIALOGTYPE DialogType);
	virtual ~CDialogConst();

    CREATE_DELETE_DECL(CDialogConst);
};

#endif

