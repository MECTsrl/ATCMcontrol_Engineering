
// DialogProgram.h: interface for the CDialogProgram class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOGPROGRAM_H__CBF93A83_EC89_11D4_B8D3_002018641833__INCLUDED_)
#define AFX_DIALOGPROGRAM_H__CBF93A83_EC89_11D4_B8D3_002018641833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DialogFbDecl.h"
#include "DialogGen.h"
#include "PropertySheetBase.h"
#include "DialogWiz.h"
#include "DialogInfo.h"
#include "DialogHelp.h"

class CDialogProgram : 
	public CPropertySheetBase, 
	public CDialogGen, 
	public CDialogWiz, 
	public CDialogFbDecl, 
	public CDialogInfo, 
	public CDialogHelp  
{
public:
	CDialogProgram(DIALOGTYPE DialogType);
	virtual ~CDialogProgram();

    CREATE_DELETE_DECL(CDialogProgram);

private:
    CStringArray m_PouLanguages;
    CStringArray m_WizardNames;
    CStringArray m_IecLanguages;
};

#endif // !defined(AFX_DIALOGPROGRAM_H__CBF93A83_EC89_11D4_B8D3_002018641833__INCLUDED_)
