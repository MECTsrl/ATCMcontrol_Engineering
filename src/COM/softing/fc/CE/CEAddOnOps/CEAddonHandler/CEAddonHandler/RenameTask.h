
#ifndef __RENAMETASK_H_
#define __RENAMETASK_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "RenameObject1.h"

//----  Forward Class Definitions:   -------------------------------*
class CCEAddonActionComObj;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CRenameTask : public CRenameObject  
{
public:
	CRenameTask(CPropertySheetBase *Dialog,
                BSTR sSourceFile,
                BSTR sId,
                ICEProjInfo *pICEProjectInfo,
                BSTR sAdditionalInfo,
                CCEAddonActionComObj* pAddonHandler);
	virtual ~CRenameTask();
    CEADDONHANDLEREXPORT virtual BOOL Execute();
    void InformProjectManager();
    BOOL ParseResource();
    void RenameInText();
    BOOL DoesTaskExist(CString strTaskName);
protected:
    CString m_ResourceText;
    Resource *m_Resource;
};



#endif // __RENAMETASK_H_


