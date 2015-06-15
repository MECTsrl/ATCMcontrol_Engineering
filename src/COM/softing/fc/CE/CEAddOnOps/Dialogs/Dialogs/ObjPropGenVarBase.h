
#if !defined __OBJPROPGENVARBASE__
#define __OBJPROPGENVARBASE__

#include "ObjPropGenBase.h"

class CObjPropGenVarBase : public CObjPropGenBase
{
public:
	CObjPropGenVarBase(DIALOGTYPE DialogType,  
                       CPropertySheetBase *PropertySheet,
                       UINT nIDTemplate);

	virtual ~CObjPropGenVarBase();

    void SetTypes(CStringArray &Types);

    CString	m_Type;
	CString	m_Value;
	CString	m_VarComment;

protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnKillActive();
    CString m_SyntaxErrorMessagePrefix;

private:
    void FillTypesComboBox();
};

#endif

