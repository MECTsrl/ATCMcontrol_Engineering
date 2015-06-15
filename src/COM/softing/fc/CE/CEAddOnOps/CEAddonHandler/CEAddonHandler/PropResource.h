
#ifndef __PROPRESOURCE_H_
#define __PROPRESOURCE_H_

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


class CPropResource : public COperation
{
public:
	CPropResource(CPropertySheetBase *Dialog,
                  BSTR sSourceFile,
                  BSTR sId,
                  ICEProjInfo *pICEProjectInfo,
                  BSTR sAdditionalInfo);
	virtual ~CPropResource();

    CEADDONHANDLEREXPORT virtual BOOL Execute();

    void SetFileName();
    CDialogResource* GetDialog();
    CEADDONHANDLEREXPORT void FillDialog();
    void InformProjectManager();
    void Init();
    void InitFromFile(Resource* Pou);
    void InsertResourceType(CString & ResourceText);
protected: 
    
    CString m_ResourceText;
    Resource *m_Resource;

//    CString m_sResType;
//    StringPos m_iResTypePos;
//    StringPos m_iResTypeLength;
};




#endif // __PROPRESOURCE_H_

