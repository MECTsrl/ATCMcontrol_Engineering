
#if !defined __DIALOGPROJ__
#define __DIALOGPROJ__

#include "PropertySheetBase.h"
#include "DialogGenProj.h"
#include "DialogInfo.h"
#include "DialogType.h"

class CDialogProj : 
	public CPropertySheetBase,  
	public CDialogGenProj,
	public CDialogInfo
{
public:
	CDialogProj(DIALOGTYPE DialogType);
	virtual ~CDialogProj();

    CREATE_DELETE_DECL(CDialogProj);

private:
    CStringArray m_PouLanguages;
    CStringArray m_WizardNames;
};

#endif 
