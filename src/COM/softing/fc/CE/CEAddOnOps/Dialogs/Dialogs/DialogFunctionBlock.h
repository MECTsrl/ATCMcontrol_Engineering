#if !defined __DIALOGFUNCTIONBLOCK__
#define __DIALOGFUNCTIONBLOCK__

#include "PropertySheetBase.h"
#include "DialogWiz.h"
#include "DialogGen.h"
#include "DialogFbDecl.h"
#include "DialogHelp.h"
#include "DialogInfo.h"
#include "DialogType.h"

class CDialogFunctionBlock : 
	public CPropertySheetBase,  
	public CDialogGen,
	public CDialogWiz,
	public CDialogFbDecl,
	public CDialogInfo,  
	public CDialogHelp
{
public:
	CDialogFunctionBlock(DIALOGTYPE DialogType);
	virtual ~CDialogFunctionBlock();

    CREATE_DELETE_DECL(CDialogFunctionBlock);

private:
    CStringArray m_PouLanguages;
    CStringArray m_WizardNames;
    CStringArray m_IecLanguages;
};

#endif 
