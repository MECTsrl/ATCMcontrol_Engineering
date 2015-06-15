
#if !defined(AFX_DIALOGDECLBASE_H__9AD5ACC3_EC57_11D4_B8D3_002018641833__INCLUDED_)
#define AFX_DIALOGDECLBASE_H__9AD5ACC3_EC57_11D4_B8D3_002018641833__INCLUDED_

#include "ObjPropDeclBase.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDialogDeclBase  
{
public:
	CDialogDeclBase(CObjPropDeclBase &CObjPropDeclBase);
	virtual ~CDialogDeclBase();

    DLLEXPORT void SetMembers(SAFEARRAY * pMembers);
    DLLEXPORT SAFEARRAY * GetMembers();

	CObjPropDeclBase &m_ObjPropDeclBase;
};

#endif // !defined(AFX_DIALOGDECLBASE_H__9AD5ACC3_EC57_11D4_B8D3_002018641833__INCLUDED_)
