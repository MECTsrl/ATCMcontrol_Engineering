
#ifndef __INSERTGVLIMPORT_H_
#define __INSERTGVLIMPORT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
#include "StResource.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CInsertGvlImport : public COperation
{
public:
	CInsertGvlImport(CPropertySheetBase *Dialog,
                         BSTR sSourceFile,
                         BSTR sId,
                         ICEProjInfo *pICEProjectInfo,
                         BSTR sAdditionalInfo);
	virtual ~CInsertGvlImport();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void ReadFileContent(const CString fileName, string& buf);
    BOOL ParseResource();
    void GenImport(CString & StrOut, CString strFileName);
    void InsertImport();
    void SetFileName();
    void InformProjectManager();
    void Init();
    BOOL DoesImportExist(CString strImportName);
    void AddImportFile(const CString& crstrImportFile);
private: 
    string          m_ResourceText;
    CString         m_ImportText;
    Resource*       m_Resource;
    CStringArray    m_aStrGvlFilesSel;
};


#endif // __INSERTGVLIMPORT_H_


