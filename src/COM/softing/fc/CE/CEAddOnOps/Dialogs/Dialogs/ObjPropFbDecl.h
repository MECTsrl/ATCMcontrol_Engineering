
#if !defined(AFX_OBJPROPFBDECL_H__E392A686_CF67_11D4_9623_00A024399A66__INCLUDED_)
#define AFX_OBJPROPFBDECL_H__E392A686_CF67_11D4_9623_00A024399A66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjPropFbDecl.h : header file
//

#include "ObjPropDeclBase.h"

/////////////////////////////////////////////////////////////////////////////
// CObjPropFbDecl dialog

class CObjPropFbDecl : public CObjPropDeclBase
{
public:
	CObjPropFbDecl(DIALOGTYPE DialogType,
        CPropertySheetBase *PropertySheet,
        int iColumnStrId,
        LPCTSTR pszSClass[]);
	~CObjPropFbDecl();

protected:
};


#endif // !defined(AFX_OBJPROPFBDECL_H__E392A686_CF67_11D4_9623_00A024399A66__INCLUDED_)
