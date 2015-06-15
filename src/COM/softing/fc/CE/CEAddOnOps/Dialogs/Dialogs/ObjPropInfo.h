
#if !defined __OBJPROPINFO__
#define __OBJPROPINFO__

#include "ObjProp.h"
#include "DialogType.h"

class CObjPropInfo : public CObjProp
{
public:
	CObjPropInfo(DIALOGTYPE DialogType,
                 CPropertySheetBase *PropertySheet);
	~CObjPropInfo();

    virtual BOOL OnSetActive();

	CButton	m_BrowseHelpUrl;
	CButton	m_BrowseUrl;
	CString	m_EditText1;
	CString	m_EditText2;
	CString	m_EditText3;
	CString	m_EditHelpUrl;
	CString	m_EditUrl;
    BOOL    m_bShowUserHelp;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

private:
    void OnBrowseUrlClicked();
    void OnBrowseHelpUrlClicked();
    CPropertySheetBase *m_pPropertySheet;

	DECLARE_MESSAGE_MAP()
};

#endif

