
#ifndef __XMLPARSE_H_
#define __XMLPARSE_H_

//----  Aggregate Includes:   --------------------------------------*
#include <tchar.h>
#import "msxml.dll" raw_interfaces_only named_guids rename_namespace("cgMSXML")
using namespace cgMSXML;
#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_cstring.h"
#include "CG_IECFront.h" //CG_Edipos

//----  Forward Class Definitions:   -------------------------------*
class CGT_CDiagMsg;
//----  Defines:    ------------------------------------------------*



//----  Prototypes:    ---------------------------------------------*
IXMLDOMNodeList* XML_LoadXMLFile(
    CGT_CDiagMsg*  pdm,
    const TCHAR*   pszFile, 
    const wchar_t* pszwRoot
);

bool XML_GetStrAttrTrim(
    CGT_CDiagMsg*       pdm,
    const CG_Edipos*    pEdp,
    const wchar_t*      pszwElem,
    IXMLDOMNamedNodeMap*pAList,
    const wchar_t*      pszwAttr,
    FC_CString*         pStringToLoad,
    bool                bWarnOnTrimm,
    bool                bOptional
);

bool XML_CLSIDFromString(
    CGT_CDiagMsg*    pdm,
    const CG_Edipos* pEdp,
    const TCHAR*     pszClsid,
    GUID*            pUUID
);








#endif // __XMLPARSE_H_


