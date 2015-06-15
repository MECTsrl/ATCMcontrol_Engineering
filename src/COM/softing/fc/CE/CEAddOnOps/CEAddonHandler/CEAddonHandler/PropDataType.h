

#ifndef __PROPDATATYPE_H_
#define __PROPDATATYPE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "Operation.h"
#include "DialogDataType.h"
#include "StStruct.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*



class CPropDataType : public COperation
{
public:
	CPropDataType(CPropertySheetBase *Dialog,
                       BSTR sSourceFile,
                       BSTR sId,
                       ICEProjInfo *pICEProjectInfo,
                       BSTR sAdditionalInfo);
	virtual ~CPropDataType();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void SetFileName();
    CDialogDataType* GetDialog();
    CEADDONHANDLEREXPORT void FillDialog();
    void InformProjectManager();
    void Init();
    int  InsertNewVar(CVar & var, CString & sPouText, POU*  Pou);
    int  InsertVarList(CString & sPouText, POU*  Pou);
protected: 
    
    CString m_DataTypeText;
    StStruct *m_DataType;
};


#endif // __PROPDATATYPE_H_
