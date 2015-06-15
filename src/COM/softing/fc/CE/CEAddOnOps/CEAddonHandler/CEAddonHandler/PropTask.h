
#ifndef __PROPTASK_H_
#define __PROPTASK_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
#include "DialogTask.h"
#include "TaskConfiguration.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CPropTask : public COperation
{
public:
	CPropTask(CPropertySheetBase *Dialog,
              BSTR sSourceFile,
              BSTR sId,
              ICEProjInfo *pICEProjectInfo,
              BSTR sAdditionalInfo);
	virtual ~CPropTask();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void SetFileName();
    CDialogTask* GetDialog();
    CEADDONHANDLEREXPORT void FillDialog();
    void InformProjectManager();
    BOOL IsAutoStart();
    void SetAutoStart(BOOL bAuto);

	// NFTASKHANDLING 19.05.05 SIS >>
    BOOL IsLoaded();
    void SetLoaded(BOOL bLoaded);
	// NFTASKHANDLING 19.05.05 SIS <<

    // SIS: event driven tasks >>
    CString GetEventString();
    void SetEventString(const CString& crstrEvent);
    // SIS: event driven tasks <<
    void InitFromFile(Resource* Pou);
    void GenTask(CString & StrOut);
    void InsertTask();
    void Init();
    void SetTaskName(const CString& crstrTaskName);
    void SetUseAttributes();
    void SetAttribute(const CString& crstrAttribute);
    CArray<CString, CString> m_arrAttributes;
    BOOL m_bUseAttribute;
    CString m_strNewTaskName;

protected: 
    
    //TaskConfiguration *m_Task;
    CString m_ResourceText;
    CString m_TaskText;
    Resource *m_Resource;

    CString m_sInterval;
    CString m_sPriority;
    CString m_sIOSync;
    CString m_sAutoStart;
    CString m_sLoaded;		// NFTASKHANDLING 19.05.05 SIS
    CString m_sEvent;       // SIS: event driven tasks
    CString m_sWatchdog;
    StringPos m_iPos;
    StringPos m_iLength;
};



#endif // __PROPTASK_H_


