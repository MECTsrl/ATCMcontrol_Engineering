
#if !defined __DIALOGGEN__
#define __DIALOGGEN__

#include "DialogGenBase.h"
#include "ObjPropGen.h"
#include "DialogType.h"

class CDialogGen : public CDialogGenBase  
{
public:
	CDialogGen(DIALOGTYPE DialogType,
               CPropertySheetBase *PropertySheet);
	virtual ~CDialogGen();

protected:
	CObjPropGen m_ObjPropGen;
};

#endif

