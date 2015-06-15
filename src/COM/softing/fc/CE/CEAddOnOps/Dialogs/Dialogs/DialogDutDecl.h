
#if !defined(AFX_DIALOGDUTDECL_H__CBF93A85_EC89_11D4_B8D3_002018641833__INCLUDED_)
#define AFX_DIALOGDUTDECL_H__CBF93A85_EC89_11D4_B8D3_002018641833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjPropDutDecl.h"
#include "DialogDeclBase.h"

class CDialogDutDecl : public CDialogDeclBase  
{
public:
	CDialogDutDecl(DIALOGTYPE DialogType,
        CPropertySheetBase *PropertySheet,
        int iColumnStrId);
	virtual ~CDialogDutDecl();

protected:
	CObjPropDutDecl m_ObjPropDutDecl;
};

#endif // !defined(AFX_DIALOGDUTDECL_H__CBF93A85_EC89_11D4_B8D3_002018641833__INCLUDED_)

