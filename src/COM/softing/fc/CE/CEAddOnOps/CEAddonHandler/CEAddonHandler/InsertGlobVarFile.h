
#ifndef __INSERTGLOBVARFILE_H_
#define __INSERTGLOBVARFILE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
#include "DialogGlobVarFile.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*



class CInsertGlobVarFile : public COperation
{
public:
	CInsertGlobVarFile(CPropertySheetBase *Dialog,
                         BSTR sSourceFile,
                         BSTR sId,
                         ICEProjInfo *pICEProjectInfo,
                         BSTR sAdditionalInfo);
	virtual ~CInsertGlobVarFile();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void SetFileName();
    CDialogGlobVarFile* GetDialog();
    void GenGlobVarFile(CString & StrOut);
    void Init();
    void FillDialog();
protected: 
    CString m_GlobVarFileText;
};




#endif // __INSERTGLOBVARFILE_H_


