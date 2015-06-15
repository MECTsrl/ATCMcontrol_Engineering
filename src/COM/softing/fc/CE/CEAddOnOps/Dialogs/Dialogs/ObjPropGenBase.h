
#if !defined(AFX_OBJPROPGENBASE_H__5A1CB2C4_D591_11D4_962B_00A024399A66__INCLUDED_)
#define AFX_OBJPROPGENBASE_H__5A1CB2C4_D591_11D4_962B_00A024399A66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjProp.h"

class CObjPropWiz;

class CObjPropGenBase : public CObjProp
{
public:
	CObjPropGenBase(DIALOGTYPE DialogType, 
        UINT nIDTemplate, 
        CPropertySheetBase *PropertySheet);
	virtual ~CObjPropGenBase();

    CString	m_Alias;
	CString	m_Name;
	CString	m_CreateDate;
	CString	m_CreateUser;
	CString	m_LastModDate;
	CString	m_LastModUser;
    CObjPropWiz *m_pObjPropWiz; // PropertyPage to which m_Name must be transferred

protected:
	virtual BOOL OnInitDialog();
    virtual BOOL OnKillActive();
	virtual void DoDataExchange(CDataExchange* pDX);

private:
    void OnChangeName();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_OBJPROPGENBASE_H__5A1CB2C4_D591_11D4_962B_00A024399A66__INCLUDED_)

