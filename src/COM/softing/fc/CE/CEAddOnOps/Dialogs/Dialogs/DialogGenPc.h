
#if !defined __DIALOGGENPC__
#define __DIALOGGENPC__

#include "DialogGenBase.h"
#include "ObjPropGenPc.h"

class CDialogGenPc : public CDialogGenBase 
{
public:
	CDialogGenPc(DIALOGTYPE DialogType, CPropertySheetBase *PropertySheet);
	virtual ~CDialogGenPc();

   	DLLEXPORT void SetAddress(CString Address);
	DLLEXPORT CString GetAddress();
	DLLEXPORT void SetResources(CStringArray &Resources);
   	DLLEXPORT void SetAssignment(CString Assignment);
	DLLEXPORT CString GetAssignment();

protected:
	CObjPropGenPc m_ObjPropGenPc;
};

#endif

