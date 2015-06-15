
#if !defined(AFX_DIALOGINFO_H__97FA71D5_CF6C_11D4_9624_00A024399A66__INCLUDED_)
#define AFX_DIALOGINFO_H__97FA71D5_CF6C_11D4_9624_00A024399A66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjPropInfo.h"
#include "DialogType.h"

class CDialogInfo  
{
public:
	CDialogInfo(DIALOGTYPE DialogType, 
                CPropertySheetBase *PropertySheet);
	virtual ~CDialogInfo();

    DLLEXPORT void SetUrl(CString Url);
    DLLEXPORT CString GetUrl();
    DLLEXPORT void SetHelpUrl(CString HelpUrl);
    DLLEXPORT CString GetHelpUrl();
    DLLEXPORT void SetText1(CString Text1);
    DLLEXPORT CString GetText1();
    DLLEXPORT void SetText2(CString Text2);
    DLLEXPORT CString GetText2();
    DLLEXPORT void SetText3(CString Text3);
    DLLEXPORT CString GetText3();

	CObjPropInfo m_ObjPropInfo;
};

#endif // !defined(AFX_DIALOGINFO_H__97FA71D5_CF6C_11D4_9624_00A024399A66__INCLUDED_)

