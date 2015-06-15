
#if !defined __DIALOGGENGLOBVAR__
#define __DIALOGGENGLOBVAR__

#include "DialogGenVarBase.h"
#include "ObjPropGenGlobVar.h"
#include "GlobConst.h"

class CDialogGenGlobVar : public CDialogGenVarBase 
{
public:
	CDialogGenGlobVar(DIALOGTYPE DialogType, CPropertySheetBase *PropertySheet);
	virtual ~CDialogGenGlobVar();

   	DLLEXPORT void SetModifier(CString Modifier);
	DLLEXPORT CString GetModifier();

protected:
	CObjPropGenGlobVar m_ObjPropGenGlobVar;
};

#endif

