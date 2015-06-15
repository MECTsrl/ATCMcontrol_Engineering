
#if !defined(AFX_DIALOGFBDECL_H__97FA71D9_CF6C_11D4_9624_00A024399A66__INCLUDED_)
#define AFX_DIALOGFBDECL_H__97FA71D9_CF6C_11D4_9624_00A024399A66__INCLUDED_

#include "ObjPropFbDecl.h"
#include "DialogDeclBase.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDialogFbDecl : public CDialogDeclBase
{
public:
	CDialogFbDecl(DIALOGTYPE DialogType,
                  CPropertySheetBase *PropertySheet,
                  int iColumnStrId,
                  LPCTSTR pszSClass[]);
	virtual ~CDialogFbDecl();

protected:
	CObjPropFbDecl m_ObjPropFbDecl;
};

#endif // !defined(AFX_DIALOGFBDECL_H__97FA71D9_CF6C_11D4_9624_00A024399A66__INCLUDED_)
