
#if !defined(AFX_OBJPROPFUNCTDECL_H__9AD5ACC6_EC57_11D4_B8D3_002018641833__INCLUDED_)
#define AFX_OBJPROPFUNCTDECL_H__9AD5ACC6_EC57_11D4_B8D3_002018641833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjPropDeclBase.h"

class CObjPropFunctDecl : public CObjPropDeclBase  
{
public:
	CObjPropFunctDecl(DIALOGTYPE DialogType,
        CPropertySheetBase *PropertySheet,
        int iColumnStrId,
        LPCTSTR pszSClass[]);
	virtual ~CObjPropFunctDecl();
    CString m_FunctionType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	BOOL OnInitDialog();

private:
	void FillComboBox(int iCBId, const char * const pszItems[], int iDefault = -1);
};

#endif // !defined(AFX_OBJPROPFUNCTDECL_H__9AD5ACC6_EC57_11D4_B8D3_002018641833__INCLUDED_)

