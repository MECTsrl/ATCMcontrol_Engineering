
#if !defined __DIALOGGENRESOURCE__
#define __DIALOGGENRESOURCE__

#include "DialogGenBase.h"
#include "ObjPropGenResource.h"

class CDialogGenResource : public CDialogGenBase 
{
public:
	CDialogGenResource(DIALOGTYPE DialogType, CPropertySheetBase *PropertySheet);
	virtual ~CDialogGenResource();

//  DLLEXPORT void SetType(CString Type);
//	DLLEXPORT  CString GetType();

protected:
	CObjPropGenResource m_ObjPropGenResource;
};

#endif

