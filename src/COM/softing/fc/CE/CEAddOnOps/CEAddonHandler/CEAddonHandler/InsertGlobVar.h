
#ifndef __INSERTGLOBVAR_H_
#define __INSERTGLOBVAR_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
#include "DialogGlobVar.h"
#include "GlobVarConstList.h"
#include "Var.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


class CInsertGlobVar : public COperation
{
public:
	CInsertGlobVar(CPropertySheetBase *Dialog,
                   BSTR sSourceFile,
                   BSTR sId,
                   ICEProjInfo *pICEProjectInfo,
                   BSTR sAdditionalInfo);
	virtual ~CInsertGlobVar();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void ParseGlobVarFile();
    void GenGlobVar(CString & StrOut);
    void InsertGlobVar();
    void InformProjectManager();
    CDialogGlobVar* GetDialog();
    void SetFileName();
    void Init();
    void FillDialog();
    BOOL DoesGVarExist(CString strGlobVarName);
    CString GetNameProposal(CString strBasis);
private: 
    string m_GlobVarFileText;
    CString m_GlobVarText;
    GlobVarConstList *m_pGlobVarList;
    CVar    m_GlobVar;
};



#endif // __INSERTGLOBVAR_H_

