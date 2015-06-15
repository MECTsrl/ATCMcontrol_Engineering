
#ifndef __INSERTFUNCTION_H_
#define __INSERTFUNCTION_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
#include "DialogFunction.h"
#include "Function.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CInsertFunction : public COperation
{
public:
	CInsertFunction(CPropertySheetBase *Dialog,
                    BSTR sSourceFile,
                    BSTR sId,
                    ICEProjInfo *pICEProjectInfo,
                    BSTR sAdditionalInfo);
	virtual ~CInsertFunction();

    CEADDONHANDLEREXPORT virtual BOOL Execute();
    
    void SetFileName();
    CDialogFunction* GetDialog();
    void GenFunction(CString & StrOut);
    void Init();
    void FillDialog();
protected: 
    CString m_FunctionText;
    Function *m_Function;
    CString m_sFunctionType;
    void Write4grFile();
};

#endif // __INSERTFUNCTION_H_

