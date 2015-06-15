
#ifndef __INSERTPROGRAMINST_H_
#define __INSERTPROGRAMINST_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
#include "DialogProgInst.h"
#include "StResource.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CInsertProgramInst : public COperation
{
public:
	CInsertProgramInst(CPropertySheetBase *Dialog,
                       BSTR sSourceFile,
                       BSTR sId,
                       ICEProjInfo *pICEProjectInfo,
                       BSTR sAdditionalInfo);
	virtual ~CInsertProgramInst();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void SetFileName();
    CDialogProgInst* GetDialog();
    void GenProgramInst(CString & StrOut);
    void Init();
    void FillDialog();
    BOOL ParseResource();
    BOOL DoesProgInstExist(CString strProgInstName);
    void InsertProgInst();
    void InformProjectManager();
    CString GetNameProposal(CString sBasis);
    void SetProgramClass(const CString& crstrProgramClass);
    void SetProgramInstName(const CString& crstrProgramInstName);
    void SetProgramPos(BOOL bPos);

protected: 
    CString m_ProgramInstText;
    
    string m_ResourceText;
    Resource* m_Resource;
    CString m_sTaskName;
    CString m_sClass;
    CString m_strInstName;
    BOOL    m_bPosFirst;
};


#endif // __INSERTPROGRAMINST_H_


