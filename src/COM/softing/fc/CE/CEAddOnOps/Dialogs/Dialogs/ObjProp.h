
#if !defined __OBJPROP__
#define __OBJPROP__

#include "DialogType.h"
#include "PropertySheetBase.h"

void Trim(CString &String);

class CObjProp : public CPropertyPage
{
public:
	CObjProp(DIALOGTYPE DialogType, 
             CPropertySheetBase *PropertySheet,
             UINT nIDTemplate);
	virtual ~CObjProp();
    CPropertySheetBase *m_PropertySheet;
    CWnd* DisableDialogItem(UINT DialogItemId);
    
protected:
    virtual BOOL OnApply();
    virtual BOOL OnSetActive(); 
    DIALOGTYPE m_DialogType;
    void ErrorBox(DWORD Error, CString ErrorMessageSuffix);
    int GetPageIndex();
    void SetReadOnly();

private:
	//OH: CWnd* DisableDialogItem(UINT DialogItemId);
};

#endif

