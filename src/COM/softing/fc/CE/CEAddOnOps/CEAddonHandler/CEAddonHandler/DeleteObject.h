
#ifndef __DELETEOBJECT_H_
#define __DELETEOBJECT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
//----  Forward Class Definitions:   -------------------------------*
class CCEAddonActionComObj;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CDeleteObject : public COperation  
{
public:
	CDeleteObject(CPropertySheetBase *Dialog,
                  BSTR sSourceFile,
                  BSTR sId,
                  ICEProjInfo *pICEProjectInfo,
                  BSTR sAdditionalInfo,
                  CCEAddonActionComObj* pAddonHandler);
	virtual ~CDeleteObject();
    CEADDONHANDLEREXPORT virtual BOOL Execute();
    void SetFileName();
    void InformProjectManager();
    BOOL DeleteFile(CString strFileName);
    CString GetNameFromFileName(CString strFileName);

protected:
    CCEAddonActionComObj*   m_pAddonHandler;
};

#endif // __DELETEOBJECT_H_

