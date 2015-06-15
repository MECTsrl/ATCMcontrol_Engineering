
#ifndef __INSERTTASK_H_
#define __INSERTTASK_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
#include "DialogTask.h"
#include "StResource.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CInsertTask : public COperation
{
public:
	CInsertTask(CPropertySheetBase *Dialog,
                         BSTR sSourceFile,
                         BSTR sId,
                         ICEProjInfo *pICEProjectInfo,
                         BSTR sAdditionalInfo);
	virtual ~CInsertTask();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void ReadFileContent(const CString fileName, string& buf);
    BOOL ParseResource();
    void GenTask(CString & StrOut);
    void InsertTask();
    void InformProjectManager();
    CDialogTask* GetDialog();
    void SetFileName();
    void Init();
    void FillDialog();
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
    BOOL DoesTaskExist(CString strTaskName);
    CString GetNameProposal(CString sBasis);
    void SetTaskName(const CString& crstrTaskName);
    void SetInterval(const CString& crstrInterval);
    void SetPriority(const CString& crstrPriority);
    void SetIOSync(const CString& crstrIOSync);
    void SetGenPos(BOOL bPosFirst);
    void SetUseAttributes();
    void SetAttribute(const CString& crstrAttribute);
protected: 
    string m_ResourceText;
    CString m_TaskText;
    Resource *m_Resource;

    CString m_sInterval;
    CString m_sPriority;
    CString m_sIOSync;
    CString m_sAutoStart;
	CString	m_sLoaded;		// NFTASKHANDLING 19.05.05 SIS
    CString m_sEvent;       // SIS: event driven tasks
    CString m_sWatchdog;
    BOOL m_bPosFirst;

    CArray<CString, CString> m_arrAttributes;
    BOOL m_bUseAttribute;
};



#endif // __INSERTTASK_H_


