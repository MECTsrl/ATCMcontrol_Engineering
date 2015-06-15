
#if !defined __DIALOGGLOBVAR__
#define __DIALOGGLOBVAR__

#include "PropertySheetBase.h"
#include "DialogGenGlobVar.h"
#include "DialogType.h"

class CDialogGlobVar : 
	public CPropertySheetBase,  
	public CDialogGenGlobVar
{
public:
	CDialogGlobVar(DIALOGTYPE DialogType);
	virtual ~CDialogGlobVar();

    CREATE_DELETE_DECL(CDialogGlobVar)
};

#endif

