
#ifndef __PARSE4GR_H_
#define __PARSE4GR_H_


//----  Aggregate Includes:   --------------------------------------*
#include "CGT_CDiagMsg.h"

//----  Forward Class Definitions:   -------------------------------*
class LDFBD_SignalHash; 
class SFC_LDFBD_SignalHash;
class CG_CIECFront;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*




/////////////////////////////////////////////////////////////////////
/**
 * Read an an 4GR File
 *
 * @param pdm                error message sink
 * @param pszXMLFile
 * @param pOnOpenLastWriteFileTime     
 * @param ppszHeader  
 * @param pGraphicElements
 * @param pIsSFC
 *   
 * @return  true 0 Errors, false at least one error
 */
bool Parse4GRFile(
    CGT_CDiagMsg*         pdm,
    CG_CIECFront*         pFront,
    const TCHAR*          pszXMLFile,
    __int64*              pOnOpenLastWriteFileTime,
    TCHAR**               ppszHeader,
    SFC_LDFBD_SignalHash* pSFCGraphicElements
);






#endif // __PARSE4GR_H_


