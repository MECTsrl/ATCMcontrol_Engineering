
// DialogWiz.h: interface for the CDialogWiz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOGWIZ_H__97FA71D8_CF6C_11D4_9624_00A024399A66__INCLUDED_)
#define AFX_DIALOGWIZ_H__97FA71D8_CF6C_11D4_9624_00A024399A66__INCLUDED_

#include "ObjPropWiz.h"
#include "DialogGenBase.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDialogWiz  
{
public:
	CDialogWiz(DIALOGTYPE DialogType, 
               CPropertySheetBase *PropertySheet,
               CDialogGenBase *pDialogGenBase);
	virtual ~CDialogWiz();

    DLLEXPORT int GetIndexOfSelectedWizardName();
    DLLEXPORT CString GetLanguageOfSelectedWizard();

	CObjPropWiz m_ObjPropWiz;

protected:
    void InitializeDialogWiz(CStringArray& IecLanguages, 
                             CStringArray& PouLanguages, 
                             CStringArray& WizardNames);

private:
    void AddWizardName(LPCTSTR lpszWizardname, LPCTSTR lpszLanguage);
};

#endif // !defined(AFX_DIALOGWIZ_H__97FA71D8_CF6C_11D4_9624_00A024399A66__INCLUDED_)

