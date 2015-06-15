
#if !defined __OBJPROPGENPB__
#define __OBJPROPGENPB__

#include "ObjPropGenBase.h"

class CObjPropGenPB : public CObjPropGenBase
{
public:
	CObjPropGenPB(DIALOGTYPE DialogType,  
                  CPropertySheetBase *PropertySheet);
	virtual ~CObjPropGenPB();

	CString	m_ChannelNumber;

protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
};

#endif
