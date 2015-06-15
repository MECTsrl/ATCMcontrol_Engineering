
#ifndef __INSERTPROGRAM_H_
#define __INSERTPROGRAM_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
#include "DialogProgram.h"
#include "Program.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CInsertProgram : public COperation
{
public:
	CEADDONHANDLEREXPORT CInsertProgram(CPropertySheetBase *Dialog,
                                        BSTR sSourceFile,
                                        BSTR sId,
                                        ICEProjInfo *pICEProjectInfo,
                                        BSTR sAdditionalInfo);
	CEADDONHANDLEREXPORT virtual ~CInsertProgram();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void SetFileName();
    CEADDONHANDLEREXPORT CDialogProgram* GetDialog();
    void GenProgram(CString & StrOut);
    void Init();
    void FillDialog();
protected: 
    CString m_ProgramText;
    Program *m_Program;
    void CInsertProgram::Write4grFile();
};



#endif // __INSERTPROGRAM_H_


