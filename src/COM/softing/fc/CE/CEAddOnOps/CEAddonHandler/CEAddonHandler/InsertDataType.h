
#ifndef __INSERTDATATYPE_H_
#define __INSERTDATATYPE_H_

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


class CInsertDataType : public COperation
{
public:
	CInsertDataType(CPropertySheetBase *Dialog,
                    BSTR sSourceFile,
                    BSTR sId,
                    ICEProjInfo *pICEProjectInfo,
                    BSTR sAdditionalInfo);
	virtual ~CInsertDataType();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void SetFileName();
    CDialogDataType* GetDialog();
    void GenDataType(CString & StrOut);
    void Init();
    void FillDialog();
    int  InsertNewVar(CVar & var, CString & sPouText, POU*  Pou);
    int  InsertVarList(CString & sPouText, POU*  Pou);
protected: 
    CString m_DataTypeText;
    StStruct *m_DataType;
};



#endif // __INSERTDATATYPE_H_

