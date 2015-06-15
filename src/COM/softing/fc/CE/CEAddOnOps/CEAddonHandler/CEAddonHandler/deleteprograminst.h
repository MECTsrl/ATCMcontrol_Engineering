
#ifndef __DELETEPROGRAMINST_H_
#define __DELETEPROGRAMINST_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CDeleteProgramInst : public COperation  
{
public:
	CDeleteProgramInst(CPropertySheetBase *Dialog,
                       BSTR sSourceFile,
                       BSTR sId,
                       ICEProjInfo *pICEProjectInfo,
                       BSTR sAdditionalInfo);
	virtual ~CDeleteProgramInst();
    CEADDONHANDLEREXPORT virtual BOOL Execute();
    void SetFileName();
    void InformProjectManager();
    BOOL ParseResource();
    void GetProgramInstRange(CString strProgramInstName);
    BOOL DeleteProgramInstConfig();
protected:
    CString m_ResourceText;
    Resource *m_Resource;

    //CString m_sInterval;
    //CString m_sPriority;
    //CString m_sIOSync;
    //CString m_sAutoStart;
    StringPos m_iPos;
    StringPos m_iLength;
};




#endif // __DELETEPROGRAMINST_H_

