
#ifndef __RENAMEOBJECT1_H_
#define __RENAMEOBJECT1_H_

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

class CRenameObject : public COperation  
{
public:
	CRenameObject(CPropertySheetBase *Dialog,
                             BSTR sSourceFile,
                             BSTR sId,
                             ICEProjInfo *pICEProjectInfo,
                             BSTR sAdditionalInfo,
                             CCEAddonActionComObj* pAddonHandler);
	virtual ~CRenameObject();
    CEADDONHANDLEREXPORT virtual BOOL Execute();
    void SetFileName();
    void InformProjectManager();
    void RenameFile(CString strFileName, CString strNewName);
    //CString GetExtentionFromFileName(CString strFileName);
    void RenameInText();
protected:
    CString m_sNewName;
    CString m_sObjectText;
    POU* m_pPouObject;
    CCEAddonActionComObj*   m_pAddonHandler;
};


#endif // __RENAMEOBJECT1_H_


