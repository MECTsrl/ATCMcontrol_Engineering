
#if !defined __DIALOGGENPROGINST__
#define __DIALOGGENPROGINST__

#include "DialogGenBase.h"
#include "ObjPropGenProgInst.h"

class CDialogGenProgInst : public CDialogGenBase 
{
public:
	CDialogGenProgInst(DIALOGTYPE DialogType, CPropertySheetBase *PropertySheet);
	virtual ~CDialogGenProgInst();

   	DLLEXPORT void SetProgClass(CString ProgClass);
	DLLEXPORT CString GetProgClass();

protected:
	CObjPropGenProgInst m_ObjPropGenProgInst;
};

#endif

