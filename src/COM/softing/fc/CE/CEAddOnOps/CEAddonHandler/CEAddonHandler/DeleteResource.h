
#ifndef __DELETERESOURCE_H_
#define __DELETERESOURCE_H_

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


class CDeleteResource : public COperation  
{
public:
	CDeleteResource(CPropertySheetBase *Dialog,
                             BSTR sSourceFile,
                             BSTR sId,
                             ICEProjInfo *pICEProjectInfo,
                             BSTR sAdditionalInfo,
                             CCEAddonActionComObj* pAddonHandler);
	virtual ~CDeleteResource();
    CEADDONHANDLEREXPORT virtual BOOL Execute();
    void SetFileName();
    BOOL DeleteFile(CString strFileName);
    CString GetNameFromFileName(CString strFileName);
    void ParseConfig();
    void DeleteResourceImport();
    StringPosRange GetRangeOfResourceImport(CString strResourceFileName);
    void WriteFile(CString sPouText);
    void InformProjectManager();
protected:

    CCEAddonActionComObj*   m_pAddonHandler;
    string m_ConfigText;
    Configuration *m_Config;
};



#endif // __DELETERESOURCE_H_


