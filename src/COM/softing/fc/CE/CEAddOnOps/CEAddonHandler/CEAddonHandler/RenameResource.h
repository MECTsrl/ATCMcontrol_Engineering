
#ifndef __RENAMERESOURCE_H_
#define __RENAMERESOURCE_H_

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

class CRenameResource : public CRenameObject  
{
public:
	CRenameResource(CPropertySheetBase *Dialog,
                    BSTR sSourceFile,
                    BSTR sId,
                    ICEProjInfo *pICEProjectInfo,
                    BSTR sAdditionalInfo,
                    CCEAddonActionComObj* pAddonHandler);
	virtual ~CRenameResource();
    CEADDONHANDLEREXPORT virtual BOOL Execute();
    void RenameResourceImport();
    void ParseConfig();
    StringPosRange GetImportRange(CString strFileName);
    void InformProjectManager();
protected:
    string m_ConfigText;
    Configuration *m_Config;
};




#endif // __RENAMERESOURCE_H_


