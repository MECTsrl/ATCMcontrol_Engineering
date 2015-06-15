

#ifndef __INSERTCONSTANTFILE_H_
#define __INSERTCONSTANTFILE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
#include "DialogConstantFile.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CInsertConstantFile : public COperation
{
public:
	CInsertConstantFile(CPropertySheetBase *Dialog,
                         BSTR sSourceFile,
                         BSTR sId,
                         ICEProjInfo *pICEProjectInfo,
                         BSTR sAdditionalInfo);
	virtual ~CInsertConstantFile();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void SetFileName();
    CDialogConstantFile* GetDialog();
    void GenConstantFile(CString & StrOut);
    void Init();
    void FillDialog();
protected: 
    CString m_ConstantFileText;
};



#endif // __INSERTCONSTANTFILE_H_

