
#if !defined __OBJPROPGENGLOBVAR__
#define __OBJPROPGENGLOBVAR__

#include "ObjPropGenVarBase.h"

class CObjPropGenGlobVar : public CObjPropGenVarBase
{
public:
	CObjPropGenGlobVar(DIALOGTYPE DialogType,
                       CPropertySheetBase *PropertySheet);
	~CObjPropGenGlobVar();

	CString	m_Modifier;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
};

#endif
