
#ifndef __INSERTCONSTANT_H_
#define __INSERTCONSTANT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
#include "DialogConst.h"
#include "GlobVarConstList.h"
#include "Var.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CInsertConstant : public COperation
{
public:
	CInsertConstant(CPropertySheetBase *Dialog,
                         BSTR sSourceFile,
                         BSTR sId,
                         ICEProjInfo *pICEProjectInfo,
                         BSTR sAdditionalInfo);
	virtual ~CInsertConstant();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void ParseConstFile();
    void GenConst(CString & StrOut);
    void InsertConst();
    void InformProjectManager();
    CDialogConst* GetDialog();
    void SetFileName();
    void Init();
    void FillDialog();
    BOOL DoesConstExist(CString strConstName);
    CString GetNameProposal(CString strBasis);
private: 
    string m_ConstFileText;
    CString m_ConstText;
    GlobVarConstList *m_pConstList;
    CVar    m_Const;
};



#endif // __INSERTCONSTANT_H_


