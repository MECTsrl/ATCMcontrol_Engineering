
#if !defined __OBJPROPGENPROJ__
#define __OBJPROPGENPROJ__

#include "ObjPropGenBase.h"

class CObjPropGenProj : public CObjPropGenBase
{
public:
	CObjPropGenProj(DIALOGTYPE DialogType,
                    CPropertySheetBase *PropertySheet);
	virtual ~CObjPropGenProj();

    CString	m_Location;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

};

#endif

