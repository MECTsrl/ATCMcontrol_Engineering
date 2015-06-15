
#if !defined(AFX_DIALOGFUNCTION_H__9AD5ACC9_EC57_11D4_B8D3_002018641833__INCLUDED_)
#define AFX_DIALOGFUNCTION_H__9AD5ACC9_EC57_11D4_B8D3_002018641833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DialogGen.h"
#include "PropertySheetBase.h"
#include "DialogFunctDecl.h"
#include "DialogWiz.h"
#include "DialogInfo.h"
#include "DialogHelp.h"

class CDialogFunction : 
	public CPropertySheetBase, 
	public CDialogGen, 
	public CDialogWiz, 
	public CDialogFunctDecl, 
	public CDialogInfo, 
	public CDialogHelp  
{
public:
	CDialogFunction(DIALOGTYPE DialogType);
	virtual ~CDialogFunction();

    CREATE_DELETE_DECL(CDialogFunction);

private:
    CStringArray m_PouLanguages;
    CStringArray m_WizardNames;
    CStringArray m_IecLanguages;
};

#endif // !defined(AFX_DIALOGFUNCTION_H__9AD5ACC9_EC57_11D4_B8D3_002018641833__INCLUDED_)

