#ifndef __CGT_LINK_H_
#define __CGT_LINK_H_



//----  Aggregate Includes:   --------------------------------------*
#include "fc_tools\fc_cgarray.h"
#include "fc_tools\fc_cstring.h"

//----  Forward Class Definitions:   -------------------------------*
class CGT_CSVFile;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*















/**
 * CGT_SDTLINK_INFO struct contains all required fields of a link file.
 * These fields must exist in both link file places (inside a lib)
 *  a) "<POU name>\.link"  
 *  b) "<POU name>\<target type>\.link"
 */
typedef struct _CGT_STDLINK_INFO
{
    FC_CStr version;  //a) version of front end
                      //b) back end
    FC_CStr iecif;    //mangeled name of POU
    FC_CStr pouclass; //user display name of POU 
    FC_CStr trgtyp;   //a) empty string 
                      //b) target type name
    FC_CStr header;   //file name containing the complete header
    long    hbytes;   //size in bytes of the interface portion of header

    FC_CStr source;   //"" or a source file check into the lib must have hdr & body
                      //typically if fun was inline{...} r a real source code lib.

    FC_CStrList  attr;  //list of all attr;<key>;<val>
                        //stored as <key>=<val>
    FC_CStrList  links; //list of all link mangeled names 

    FC_CStr pouName;  //not from the .link, extracted from iecif mangeled name

} CGT_STDLINK_INFO;



//------------------------------------------------------------------*
/** TODO Comment incorrect !
 * CGT_GetStdLinkInfo reads a .link file from an initialised CGT_CSVFile
 * reader and converts the standard default data into a CGT_STDLINK_INFO
 * structure.  
 * The 'standard default' data are the fields:
 *  TODO
 * Appended by back end, precompiled source and console stuff:
 *  TODO
 *
 * @return          true if no error
 * @exception       -
*/
extern "C" bool CGT_ReadLinkFile(
    CGT_CSVFile*       pLinkFile,
    CGT_STDLINK_INFO*  pStdInfo,
    FC_CString*        pErrDescr
);


//------------------------------------------------------------------*
/**
 * CGT_IECMangeledNameFromLinkFile extracts the IEC mangeld name
 * from a link file. 
 *
 * @param      pszLink    the link file typically from pILib->GetFileAsBSTR
 * @return     0-terminated IEC mangeld name, allocated with new, caller 
 *             must free
 *             returns NULL if any format error.
 * @exception       -
*/
extern "C" wchar_t* CGT_IECMangeledNameFromLinkFile(
    const wchar_t* pszLink
);



#if 0
extern "C" bool CGT_CheckLibPOULinks(
    CGT_CDiagMsg*      pdm,
    ICG_IECFront*      pIFront,
    const TCHAR*       pszTrgType,
    const CG_Symtab*   pLibSym,
    const TCHAR*       pszCodeVersion,
    const CG_Symtab*** pppCalledFuns
);
#endif



#endif // __CGT_LINK_H_


