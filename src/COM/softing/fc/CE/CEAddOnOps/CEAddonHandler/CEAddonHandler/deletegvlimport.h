
#ifndef __DELETEGVLIMPORT_H_
#define __DELETEGVLIMPORT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
#include "FileSelDlg.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CDeleteGvlImport : public COperation  
{
public:
	CDeleteGvlImport(CPropertySheetBase *Dialog,
                             BSTR sSourceFile,
                             BSTR sId,
                             ICEProjInfo *pICEProjectInfo,
                             BSTR sAdditionalInfo);
	virtual ~CDeleteGvlImport();
    CEADDONHANDLEREXPORT virtual BOOL Execute();
    void SetFileName();
    void InformProjectManager();
    BOOL ParseResource();
    void Init();
    void GetImportRange(CString strFileName);
    BOOL DeleteImport(CString strFileName);
    void AddDeleteFile(const CString& crstrImportFile);

// SIS d-113 >>
protected:
	BOOL FindImportStatement(const CString& crstrResourceText, int& riPos, int& riLength);
// SIS d-113 <<

protected:
    CString m_ResourceText;
    Resource *m_Resource;

    StringPos m_iPos;
    StringPos m_iLength;
    CStringArray m_aStrGvlFilesSel;
};



#endif // __DELETEGVLIMPORT_H_


