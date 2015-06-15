
#ifndef __PROPPROJECT_H_
#define __PROPPROJECT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
#include "DialogProj.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CPropProject : public COperation
{
public:
	CPropProject(CPropertySheetBase *Dialog,
                 BSTR sSourceFile,
                 BSTR sId,
                 ICEProjInfo *pICEProjectInfo,
                 BSTR sAdditionalInfo);
	virtual ~CPropProject();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void SetFileName();
    CDialogProj* GetDialog();
    CEADDONHANDLEREXPORT void FillDialog();
    void InformProjectManager();
    void Init();    
    void InitFromFile();
    void WriteIntoFile();
protected:
    CString m_strLocation;
};




#endif // __PROPPROJECT_H_

