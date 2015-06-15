
#if !defined(AFX_DIALOGHELP_H__97FA71D6_CF6C_11D4_9624_00A024399A66__INCLUDED_)
#define AFX_DIALOGHELP_H__97FA71D6_CF6C_11D4_9624_00A024399A66__INCLUDED_

#include "ObjPropHelp.h"
#include "DialogGenBase.h"
#include "DialogInfo.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDialogHelp  
{
public:
	CDialogHelp(DIALOGTYPE DialogType, 
                CPropertySheetBase *PropertySheet,
                CDialogGenBase *pDialogGenBase, 
                CDialogInfo *pDialogInfo);
	virtual ~CDialogHelp();

	DLLEXPORT CString GetPathOfSelectedTemplate();

protected:
	CObjPropHelp m_ObjPropHelp;
};

#endif // !defined(AFX_DIALOGHELP_H__97FA71D6_CF6C_11D4_9624_00A024399A66__INCLUDED_)
