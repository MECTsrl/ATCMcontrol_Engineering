
#ifndef __RENAMEPROGRAMINST_H_
#define __RENAMEPROGRAMINST_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "RenameObject1.h"
//----  Forward Class Definitions:   -------------------------------*
class CCEAddonActionComObj;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CRenameProgramInst : public CRenameObject  
{
public:
	CRenameProgramInst(CPropertySheetBase *Dialog,
                       BSTR sSourceFile,
                       BSTR sId,
                       ICEProjInfo *pICEProjectInfo,
                       BSTR sAdditionalInfo,
                       CCEAddonActionComObj* pAddonHandler);
	virtual ~CRenameProgramInst();
    CEADDONHANDLEREXPORT virtual BOOL Execute();
    void InformProjectManager();
    BOOL ParseResource();
    void RenameInText();
    BOOL DoesProgramInstExist(CString strProgramInstName);
protected:
    CString m_ResourceText;
    Resource *m_Resource;
};


#endif // __RENAMEPROGRAMINST_H_

