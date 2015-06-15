
#ifndef __PROPFUNCTION_H_
#define __PROPFUNCTION_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
#include "DialogFunction.h"
#include "Function.h"
#include <fstream>
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CPropFunction : public COperation
{
public:
	CPropFunction(CPropertySheetBase *Dialog,
                       BSTR sSourceFile,
                       BSTR sId,
                       ICEProjInfo *pICEProjectInfo,
                       BSTR sAdditionalInfo);
	virtual ~CPropFunction();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void SetFileName();
    CDialogFunction* GetDialog();
    void Init();
    void InformProjectManager();
    CEADDONHANDLEREXPORT void FillDialog();
    void InitFromFile(POU* Pou);
    void InsertFunctionType(CString & FunctionText);
protected: 
    
    CString m_FunctionText;
    Function *m_Function;

    CString m_sFunctionType;
    StringPos m_iFunctionTypePos;
    StringPos m_iFunctionTypeLength;
};



#endif // __PROPFUNCTION_H_


