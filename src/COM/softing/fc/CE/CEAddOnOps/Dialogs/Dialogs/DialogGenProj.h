
#if !defined __DIALOGGENPROJ__
#define __DIALOGGENPROJ__

#include "DialogGenBase.h"
#include "ObjPropGenProj.h"

class CDialogGenProj : public CDialogGenBase  
{
public:
	CDialogGenProj(DIALOGTYPE DialogType, CPropertySheetBase *PropertySheet);
	virtual ~CDialogGenProj();

    DLLEXPORT CString GetLocation();
    DLLEXPORT void SetLocation(CString Location);

protected:
	CObjPropGenProj m_ObjPropGenProj;
};

#endif

