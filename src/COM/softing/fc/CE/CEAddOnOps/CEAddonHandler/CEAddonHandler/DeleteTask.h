
#ifndef __DELETETASK_H_
#define __DELETETASK_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CDeleteTask : public COperation  
{
public:
	CDeleteTask(CPropertySheetBase *Dialog,
                             BSTR sSourceFile,
                             BSTR sId,
                             ICEProjInfo *pICEProjectInfo,
                             BSTR sAdditionalInfo);
	virtual ~CDeleteTask();
    CEADDONHANDLEREXPORT virtual BOOL Execute();
    void SetFileName();
    void InformProjectManager();
    BOOL ParseResource();
    void GetTaskRange(CString strTaskName);
    BOOL DeleteTaskConfig();
    void DeleteProgramInstances(CString strTaskName);
protected:
    CString m_ResourceText;
    Resource *m_Resource;

    CString m_sInterval;
    CString m_sPriority;
    CString m_sIOSync;
    CString m_sAutoStart;
    StringPos m_iPos;
    StringPos m_iLength;
};



#endif // __DELETETASK_H_

