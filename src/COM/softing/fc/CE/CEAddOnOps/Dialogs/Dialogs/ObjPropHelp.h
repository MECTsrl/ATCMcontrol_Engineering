
#if !defined(AFX_OBJPROPHELP_H__E392A681_CF67_11D4_9623_00A024399A66__INCLUDED_)
#define AFX_OBJPROPHELP_H__E392A681_CF67_11D4_9623_00A024399A66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxcmn.h>

#include "ObjPropGenBase.h"
#include "ObjPropInfo.h"

class CObjPropHelp : public CObjProp
{
public:
	CObjPropHelp(DIALOGTYPE DialogType,
                 CPropertySheetBase *PropertySheet,
                 CObjPropGenBase *pObjPropGenBase, 
                 CObjPropInfo *pObjPropInfo);
	~CObjPropHelp();

	BOOL	m_GenerateHelp;
	CString	m_PathOfSelectedTemplate;
	CButton	m_BrowseTemplate;
	CString	m_EditTemplate;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnKillActive();
    virtual BOOL OnInitDialog();

private:
    void OnBrowseTemplateClicked();
    void OnGenerateHelpClicked();
    CObjPropGenBase *m_pObjPropGenBase;   // PropertyPage from which object name must be transferred
    CObjPropInfo *m_pObjPropInfo;         // PropertyPage to which object name must be transferred
    CPropertySheetBase *m_pPropertySheet;

	DECLARE_MESSAGE_MAP()
};

#endif

