
#if !defined(AFX_OBJPROPWIZ_H__E392A687_CF67_11D4_9623_00A024399A66__INCLUDED_)
#define AFX_OBJPROPWIZ_H__E392A687_CF67_11D4_9623_00A024399A66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxcmn.h>

#include "ObjPropGenBase.h"

class CObjPropWiz : public CObjProp
{
public:
	CObjPropWiz(DIALOGTYPE DialogType, 
                CPropertySheetBase *PropertySheet,
                CObjPropGenBase *pObjPropGenBase);
	~CObjPropWiz();

    void AddWizardName(LPCTSTR WizardName, LPCTSTR Language);
    CString GetLanguageOfSelectedWizard();

	CString	m_Name;
    int m_IndexOfSelectedWizardName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual BOOL OnKillActive();

private:
    CObjPropGenBase *m_pObjPropGenBase; // PropertyPage to which m_Name must be transferred
	CListCtrl m_Wizards;
    CStringArray m_Languages;
    CImageList m_ImageList;
    CStringArray m_WizardNames;
    void OnChangeName();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_OBJPROPWIZ_H__E392A687_CF67_11D4_9623_00A024399A66__INCLUDED_)

