
#if !defined(AFX_OBJPROPDUTDECL_H__CBF93A86_EC89_11D4_B8D3_002018641833__INCLUDED_)
#define AFX_OBJPROPDUTDECL_H__CBF93A86_EC89_11D4_B8D3_002018641833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjPropDeclBase.h"

class CObjPropDutDecl : public CObjPropDeclBase  
{
public:
	CObjPropDutDecl(DIALOGTYPE DialogType,
        CPropertySheetBase *PropertySheet,
        int iColumnStrId);
	virtual ~CObjPropDutDecl();

protected:
	void OnAddMember();
	void OnModifyMember();
    void OnUpMember();
    void OnDownMember();
	void OnClickFbmembers(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
private:
	void EnableButtons();
};

#endif // !defined(AFX_OBJPROPDUTDECL_H__CBF93A86_EC89_11D4_B8D3_002018641833__INCLUDED_)
