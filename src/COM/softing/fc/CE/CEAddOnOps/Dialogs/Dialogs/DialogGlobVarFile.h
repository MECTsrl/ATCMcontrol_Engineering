
#if !defined __DIALOGGLOBVARFILE__
#define __DIALOGGLOBVARFILE__

#include "PropertySheetBase.h"
#include "DialogGen.h"
#include "DialogHelp.h"
#include "DialogInfo.h"
#include "DialogType.h"

class CDialogGlobVarFile : 
	public CPropertySheetBase,  
	public CDialogGen,
	public CDialogInfo,  
	public CDialogHelp
{
public:
	CDialogGlobVarFile(DIALOGTYPE DialogType);
	virtual ~CDialogGlobVarFile();

    CREATE_DELETE_DECL(CDialogGlobVarFile);
};

#endif 

