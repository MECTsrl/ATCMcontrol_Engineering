#if !defined __OBJPROPGENCONST__
#define __OBJPROPGENCONST__

#include "ObjPropGenVarBase.h"

class CObjPropGenConst : public CObjPropGenVarBase
{
public:
	CObjPropGenConst(DIALOGTYPE DialogType,
                     CPropertySheetBase *PropertySheet);
	virtual ~CObjPropGenConst();

protected:
    virtual BOOL OnKillActive();
};

#endif

