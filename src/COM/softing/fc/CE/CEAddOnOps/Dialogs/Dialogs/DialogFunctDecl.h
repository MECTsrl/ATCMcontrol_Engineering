
#if !defined(AFX_DIALOGFUNCTDECL_H__9AD5ACC7_EC57_11D4_B8D3_002018641833__INCLUDED_)
#define AFX_DIALOGFUNCTDECL_H__9AD5ACC7_EC57_11D4_B8D3_002018641833__INCLUDED_

#include "ObjPropFunctDecl.h"
#include "DialogDeclBase.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDialogFunctDecl : public CDialogDeclBase  
{
public:
	CDialogFunctDecl(DIALOGTYPE DialogType,
                     CPropertySheetBase *PropertySheet,
                     int iColumnStrId,
                     LPCTSTR pszSClass[]);
	virtual ~CDialogFunctDecl();

	DLLEXPORT void SetReturnType(CString strType);
	DLLEXPORT CString GetReturnType();

protected:
	CObjPropFunctDecl m_ObjPropFunctDecl;
};

#endif // !defined(AFX_DIALOGFUNCTDECL_H__9AD5ACC7_EC57_11D4_B8D3_002018641833__INCLUDED_)

