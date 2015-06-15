
#if !defined __DIALOGRESOURCE__
#define __DIALOGRESOURCE__

#include "PropertySheetBase.h"
#include "DialogGenResource.h"
#include "DialogInfo.h"
#include "DialogHelp.h"
#include "DialogType.h"

class CDialogResource : 
	public CPropertySheetBase,  
	public CDialogGenResource,
	public CDialogInfo,
	public CDialogHelp
{
public:
	CDialogResource(DIALOGTYPE DialogType);
	virtual ~CDialogResource();

    CREATE_DELETE_DECL(CDialogResource);
};

#endif

