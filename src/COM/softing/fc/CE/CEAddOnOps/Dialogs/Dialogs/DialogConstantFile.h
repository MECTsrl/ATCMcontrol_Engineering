
#if !defined __DIALOGCONSTANTFILE__
#define __DIALOGCONSTANTFILE__

#include "PropertySheetBase.h"
#include "DialogGen.h"
#include "DialogHelp.h"
#include "DialogInfo.h"
#include "DialogType.h"

class CDialogConstantFile : 
	public CPropertySheetBase,  
	public CDialogGen,
	public CDialogInfo,  
	public CDialogHelp
{
public:
	CDialogConstantFile(DIALOGTYPE DialogType);
	virtual ~CDialogConstantFile();

    CREATE_DELETE_DECL(CDialogConstantFile);
};

#endif 

