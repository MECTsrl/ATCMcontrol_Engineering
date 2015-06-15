
#ifndef __INSERTRESOURCE_H_
#define __INSERTRESOURCE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
#include "DialogResource.h"
#include "StResource.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CInsertResource : public COperation
{
public:
	CInsertResource(CPropertySheetBase *Dialog,
                         BSTR sSourceFile,
                         BSTR sId,
                         ICEProjInfo *pICEProjectInfo,
                         BSTR sAdditionalInfo);
	virtual ~CInsertResource();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void GenResource(CString & StrOut);
    void SetFileName();
    CDialogResource* GetDialog();
    void Init();
    void FillDialog();
    void InformProjectManager();
    void ParseConfig();
    void InsertResourceImport();
    void GenImportText(CString & sImportText);
    void WriteFile(CString sPouText);
private: 
    CString m_ResourceText;
    Resource *m_Resource;

    string m_ConfigText;
    CString m_ImportText;
    Configuration *m_Config;


    CString m_sResType;
};



#endif // __INSERTRESOURCE_H_

