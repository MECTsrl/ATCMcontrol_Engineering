
#ifndef __INSERTFUNCTIONBLOCK_H_
#define __INSERTFUNCTIONBLOCK_H_

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

class CInsertFunctionBlock : public COperation
{
public:
	CInsertFunctionBlock(CPropertySheetBase *Dialog,
                         BSTR sSourceFile,
                         BSTR sId,
                         ICEProjInfo *pICEProjectInfo,
                         BSTR sAdditionalInfo);
	virtual ~CInsertFunctionBlock();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void SetFileName();
    CDialogFunctionBlock* GetDialog();
    void GenFunctionBlock(CString & StrOut);
    void Init();
    void FillDialog();
protected: 
    CString m_FunctionBlockText;
    FunctionBlock *m_FunctionBlock;
    void Write4grFile();
};



#endif // __INSERTFUNCTIONBLOCK_H_

