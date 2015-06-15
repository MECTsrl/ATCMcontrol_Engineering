
#if !defined __DIALOGGENPB__
#define __DIALOGGENPB__

#include "DialogGenBase.h"
#include "ObjPropGenPB.h"

class CDialogGenPB : public CDialogGenBase 
{
public:
	CDialogGenPB(DIALOGTYPE DialogType, CPropertySheetBase *PropertySheet);
	virtual ~CDialogGenPB();

   	DLLEXPORT void SetChannelNumber(CString Channelnumber);
	DLLEXPORT CString GetChannelNumber();

protected:
	CObjPropGenPB m_ObjPropGenPB;
};

#endif

