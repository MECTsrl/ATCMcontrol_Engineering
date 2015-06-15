
#if !defined __DIALOGGENCONST__
#define __DIALOGGENCONST__

#include "DialogGenVarBase.h"
#include "ObjPropGenConst.h"

class CDialogGenConst : public CDialogGenVarBase  
{
public:
	CDialogGenConst(DIALOGTYPE DialogType, CPropertySheetBase *PropertySheet);
	virtual ~CDialogGenConst();

protected:
	CObjPropGenConst m_ObjPropGenConst;
};

#endif

