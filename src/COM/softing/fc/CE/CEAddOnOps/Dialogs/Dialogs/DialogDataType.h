
// DialogDataType.h: interface for the CDialogDataType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOGDATATYPE_H__CBF93A84_EC89_11D4_B8D3_002018641833__INCLUDED_)
#define AFX_DIALOGDATATYPE_H__CBF93A84_EC89_11D4_B8D3_002018641833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DialogDutDecl.h"
#include "DialogGen.h"
#include "PropertySheetBase.h"
#include "DialogWiz.h"
#include "DialogInfo.h"
#include "DialogHelp.h"

class CDialogDataType : 
	public CPropertySheetBase, 
	public CDialogGen, 
	public CDialogWiz, 
	public CDialogDutDecl, 
	public CDialogInfo, 
	public CDialogHelp  
{
public:
	CDialogDataType(DIALOGTYPE DialogType);
	virtual ~CDialogDataType();

    CREATE_DELETE_DECL(CDialogDataType);

private:
    CStringArray m_PouLanguages;
    CStringArray m_WizardNames;
    CStringArray m_IecLanguages;
};

#endif // !defined(AFX_DIALOGDATATYPE_H__CBF93A84_EC89_11D4_B8D3_002018641833__INCLUDED_)

