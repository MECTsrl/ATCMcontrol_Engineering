
#ifndef __PROPPROGRAM_H_
#define __PROPPROGRAM_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
#include "DialogProgram.h"
#include "Program.h"
#include <fstream>
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CPropProgram : public COperation
{
public:
	CPropProgram(CPropertySheetBase *Dialog,
                       BSTR sSourceFile,
                       BSTR sId,
                       ICEProjInfo *pICEProjectInfo,
                       BSTR sAdditionalInfo);
	virtual ~CPropProgram();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void SetFileName();
    CDialogProgram* GetDialog();
    void Init();
    CEADDONHANDLEREXPORT void FillDialog();
    void InformProjectManager();
protected: 
    
    CString m_ProgramText;
    Program *m_Program;
};



#endif // __PROPPROGRAM_H_

