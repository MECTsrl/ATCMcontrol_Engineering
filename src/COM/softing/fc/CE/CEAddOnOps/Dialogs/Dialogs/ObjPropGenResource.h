
#if !defined __OBJPROPGENRESOURCE__
#define __OBJPROPGENRESOURCE__

#include "ObjPropGenBase.h"

class CObjPropGenResource : public CObjPropGenBase
{
public:
	CObjPropGenResource(DIALOGTYPE DialogType,
                        CPropertySheetBase *PropertySheet);
	~CObjPropGenResource();

//	CString	m_Type;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
    virtual BOOL OnInitDialog();
    virtual BOOL OnKillActive();
};

#endif
