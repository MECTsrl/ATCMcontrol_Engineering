
#ifndef __PROPPROGRAMINST_H_
#define __PROPPROGRAMINST_H_

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


class CPropProgramInst : public COperation
{
public:
	CPropProgramInst(CPropertySheetBase *Dialog,
                     BSTR sSourceFile,
                     BSTR sId,
                     ICEProjInfo *pICEProjectInfo,
                     BSTR sAdditionalInfo);
	virtual ~CPropProgramInst();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void SetFileName();
    CDialogProgInst* GetDialog();
    CEADDONHANDLEREXPORT void FillDialog();
    void InformProjectManager();
    void InitFromFile(Resource* Pou);
    void GenProgramInst(CString & StrOut);
    void InsertProgramInst();
    void Init();
    void SetProgramClass(const CString& strClass);
    void SetProgramInstName(const CString& strName);
    void SetTaskName(const CString& strTask);
protected: 
    
    CString m_ResourceText;
    Resource *m_Resource;
    StringPos m_iPos;
    StringPos m_iLength;
    CString m_ProgramInstText;
    CString m_sTaskName;
    CString m_sClass;
    CString m_sProgInstName;
};


#endif // __PROPPROGRAMINST_H_


