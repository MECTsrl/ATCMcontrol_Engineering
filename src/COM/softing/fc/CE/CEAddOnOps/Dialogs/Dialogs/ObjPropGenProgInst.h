
#if !defined __OBJPROPGENPROGINST__
#define __OBJPROPGENPROGINST__

#include "ObjPropGenBase.h"

class CObjPropGenProgInst : public CObjPropGenBase
{
public:
	CObjPropGenProgInst(DIALOGTYPE DialogType,
                    CPropertySheetBase *PropertySheet);

	~CObjPropGenProgInst();

	CString	m_ProgramClass;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual BOOL OnKillActive();

private:
    void FillProgClassesComboBox(const CString* pstrNewProg = NULL);

    void OnNewProg();

    DECLARE_MESSAGE_MAP()
};

#endif

