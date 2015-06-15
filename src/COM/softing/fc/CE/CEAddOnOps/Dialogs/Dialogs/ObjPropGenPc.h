
#if !defined __OBJPROPGENPC__
#define __OBJPROPGENPC__

#include "ObjPropGenBase.h"

class CObjPropGenPc : public CObjPropGenBase
{
public:
	CObjPropGenPc(DIALOGTYPE DialogType,
                  CPropertySheetBase *PropertySheet);
	~CObjPropGenPc();

    void SetResources(CStringArray &Resources);

    CString	m_Address;
    CStringArray m_Resources;
	CString	m_Assignment;

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual BOOL OnKillActive();
    
private:
    void FillAssignmentComboBox();
};

#endif

