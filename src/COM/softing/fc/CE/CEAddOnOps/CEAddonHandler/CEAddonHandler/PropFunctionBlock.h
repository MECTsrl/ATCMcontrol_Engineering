
#ifndef __PROPFUNCTIONBLOCK_H_
#define __PROPFUNCTIONBLOCK_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
#include "DialogFunctionBlock.h"
#include "FunctionBlock.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CPropFunctionBlock : public COperation
{
public:
	CPropFunctionBlock(CPropertySheetBase *Dialog,
                       BSTR sSourceFile,
                       BSTR sId,
                       ICEProjInfo *pICEProjectInfo,
                       BSTR sAdditionalInfo);
	virtual ~CPropFunctionBlock();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void SetFileName();
    CDialogFunctionBlock* GetDialog();
    CEADDONHANDLEREXPORT void FillDialog();
    void InformProjectManager();
    void Init();
protected: 
    
    CString m_FunctionBlockText;
    FunctionBlock *m_FunctionBlock;
};



#endif // __PROPFUNCTIONBLOCK_H_

