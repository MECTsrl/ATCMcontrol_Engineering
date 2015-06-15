
#ifndef __CGT_XMLPARSE_H_
#define __CGT_XMLPARSE_H_

//----  Aggregate Includes:   --------------------------------------*
#include <tchar.h>
#import "msxml.dll" raw_interfaces_only named_guids rename_namespace("cgMSXML")
using namespace cgMSXML;
#include "CGT_CDiagMsg.h"



//----  Prototypes:    ---------------------------------------------*
extern "C"{


IXMLDOMNode* CGT_XMLLoadFile(
    CGT_CDiagMsg*        pdm,
    const TCHAR*         pszPrjPath,
    const TCHAR*         pszFile, 
    const wchar_t*       pszwRoot
);
IXMLDOMNode* CGT_XMLLoadLocatorFile(
    CGT_CDiagMsg*    pdm,           //for err msg
    ICG_IECFront*    pIFront,       //used if locator points to a 4cl lib
    const TCHAR*     pszFileLocator,//file locator
    const wchar_t*   pszwRoot,      //expected name of the root node,
                                    //if not NULL this is validated and
                                    //an error is submitted if the actual 
                                    //root node name differs form pszwRoot.
    __int64*         pftLastWrite   //NULL or receives the last file write time
);


bool CGT_XMLGetStrAttrTrim(
    CGT_CDiagMsg*       pdm,
    IXMLDOMNode*        pNode,
    const wchar_t*      pszwAttr,
    FC_CString*         pStringToLoad,
    bool                bWarnOnTrimm,
    bool                bOptional
);

//return <0 if any error
//        0: no such attribute
//        1: attribute found an ok 
int CGT_XMLGetBoolAttr( 
    CGT_CDiagMsg*       pdm,
    IXMLDOMNode*        pNode,
    const wchar_t*      pszwAttr,
    bool*               pbAttr,   //set only if return > 0
    bool                bOptional 
);

bool CGT_XMLGetCLSIDAttr(
    CGT_CDiagMsg*    pdm,
    IXMLDOMNode*     pNode,
    const wchar_t*   pszwAttr,
    GUID*            pUUID,
    bool             bOptional
);




//Diag messages on a node:
void CGT_XMLDiagMsg(
    CGT_CDiagMsg*pdm, 
    IXMLDOMNode* pNode, 
    HRESULT      errId, 
    const TCHAR* ppsz[]
);
void CGT_XMLDiagMsg0(
    CGT_CDiagMsg*  pdm, 
    IXMLDOMNode*   pNode, 
    HRESULT        errId 
);
void CGT_XMLDiagMsg1(
    CGT_CDiagMsg*  pdm, 
    IXMLDOMNode*   pNode, 
    HRESULT        errId, 
    const wchar_t* psz
);
void CGT_XMLDiagMsg2(
    CGT_CDiagMsg*  pdm, 
    IXMLDOMNode*   pNode, 
    HRESULT        errId, 
    const wchar_t* psz,
    const wchar_t* psz2
);
void CGT_XMLDiagMsg3(
    CGT_CDiagMsg*  pdm, 
    IXMLDOMNode*   pNode, 
    HRESULT        errId, 
    const wchar_t* psz,
    const wchar_t* psz2,
    const wchar_t* psz3
);

} //extern "C"



#endif // __CGT_XMLPARSE_H_


