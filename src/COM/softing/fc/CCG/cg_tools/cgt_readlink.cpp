#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_fileIO.h"
#include "cgt_link.h"
#include "cgt_misc.h"



/* Example
version;1.20
iecif;FB$TemperatureControl(O$BOOL$STATE,O$BOOL$ERROR,O$DINT$ERROR_NR,O$WSTRING$ERROR_STRING)
pouclass;FUNCTION_BLOCK
trgtyp;4cpc
header;TemperatureControl.hdr;727
source;TemperatureControl.st;
pouattr;GUID;{0678E3F9-224D-11D4-9484-0008C779D265}
pouattr;UserURL;4cfile:TemperatureControl.htm
link; .......
link; .......
put;<fullpath>\.4cpc\.classes\temperaturecontrol.class
put;<fullpath>\.4cpc\.classes\TemperatureControl.hdr
put;<fullpath>\TemperatureControl.asp
*/


#define MAX_FIELDS   3
extern "C" bool CGT_ReadLinkFile(
    CGT_CSVFile*       pLinkFile,
    CGT_STDLINK_INFO*  pLnkInf,
    FC_CString*        pErrDescr
)
{
    const TCHAR* pszExpect;
    TCHAR*       endptr;
    const TCHAR* pszFields[MAX_FIELDS];
    FC_CString   jot(MAX_PATH);

    //cler old error of err string:
    pErrDescr->clear();

    //clear old info of struct to be filled:
    pLnkInf->version.load("");
    pLnkInf->iecif.load("");
    pLnkInf->pouclass.load("");
    pLnkInf->trgtyp.load("");
    pLnkInf->header.load("");
    pLnkInf->hbytes =0;
    pLnkInf->source.load("");
    pLnkInf->pouName.load("");
    pLnkInf->attr.clear();
    pLnkInf->links.clear();



   
//version;<vers>
    if(pLinkFile->getRecordFields(MAX_FIELDS, pszFields) != 2)
        goto ErrorSyntax;
    pszExpect = CG_LINK_LIB_VERSION;
    if(lstrcmp(pszFields[0], pszExpect))
        goto ErrorExpected;
    pLnkInf->version.load(pszFields[1]);



//iecif;<mangeled>
    if(pLinkFile->getRecordFields(MAX_FIELDS, pszFields) != 2)
        goto ErrorSyntax;
    pszExpect = CG_LINK_LIB_IECIF;
    if(lstrcmp(pszFields[0], pszExpect))
        goto ErrorExpected;
    pLnkInf->iecif.load(pszFields[1]);


    
//pouclass;<id>
    if(pLinkFile->getRecordFields(MAX_FIELDS, pszFields) != 2)
        goto ErrorSyntax;
    pszExpect = CG_LINK_LIB_POUCLASS;
    if(lstrcmp(pszFields[0], pszExpect))
        goto ErrorExpected;
    pLnkInf->pouclass.load(pszFields[1]);



//trgtyp;<id>
    if(pLinkFile->getRecordFields(MAX_FIELDS, pszFields) != 2)
        goto ErrorSyntax;
    pszExpect = CG_LINK_LIB_TRGTYP;
    if(lstrcmp(pszFields[0], pszExpect))
        goto ErrorExpected;
    pLnkInf->trgtyp.load(pszFields[1]);


//header;<file>;<hbytes>
    if(pLinkFile->getRecordFields(MAX_FIELDS, pszFields) != 3)
        goto ErrorSyntax;
    pszExpect = CG_LINK_LIB_HEADER;
    if(lstrcmp(pszFields[0], pszExpect))
        goto ErrorExpected;
    pLnkInf->header.load(pszFields[1]);

    pLnkInf->hbytes = _tcstoul(pszFields[2], &endptr, 10);
    if(*endptr)
    {
        pszExpect = _T("<hrdbytes as ulong>");
        goto ErrorExpected;
    }


//source;<file>
    if(pLinkFile->getRecordFields(MAX_FIELDS, pszFields) != 2)
        goto ErrorSyntax;
    pszExpect = CG_LINK_LIB_SOURCE;
    if(lstrcmp(pszFields[0], pszExpect))
        goto ErrorExpected;
    pLnkInf->source.load(pszFields[1]);


//pouattr;GUID;{0678E3F9-224D-11D4-9484-0008C779D265}
    while(FC_StringStartsWith(pLinkFile->getCurrChars(), CG_LINK_LIB_POUATTR)>=0)
    {
        if(pLinkFile->getRecordFields(MAX_FIELDS, pszFields) != 3)
            goto ErrorSyntax;
        jot.clear()<<pszFields[1]<<_T("=")<<pszFields[2];
        pLnkInf->attr.add(jot);
    }

//link; <mangeled>
    while(FC_StringStartsWith(pLinkFile->getCurrChars(), CG_LINK_LIB_LINK)>=0)
    {
        if(pLinkFile->getRecordFields(MAX_FIELDS, pszFields) != 2)
            goto ErrorSyntax;
        pLnkInf->links.add(pszFields[1]);
    }



    //no error so far, get the pouName from the mangeled name:
    {
    const TCHAR* psz = pLnkInf->iecif;
    psz = jot.clear().appendUpTo(psz, _T("#"));
    psz = jot.clear().appendUpTo(psz, _T("("));
    pLnkInf->pouName.load(jot);
    assert(pLnkInf->pouName[0]);
    }


    return true;

ErrorExpected:
    *pErrDescr<<_T("line ")<< pLinkFile->getLineno()-1;
    *pErrDescr<<_T(": expected '")<<pszExpect<<_T("'");
    return false;

ErrorSyntax:    
    *pErrDescr<<_T("line ")<< pLinkFile->getLineno();
    *pErrDescr<<_T(": CSV syntax error");
    return false;
}



//version;1.20
//iecif;FUN#BOOL_TO_DINT(Q#DINT#BOOL_TO_DINT,I#BOOL#IN)
//pouclass;FUNCTION
extern "C" wchar_t* CGT_IECMangeledNameFromLinkFile(
    const wchar_t* pszLink  //typically from pILib->GetFileAsBSTR
)
{
    int            iLen;
    const wchar_t* psz;
    const wchar_t* pszFirst;
    wchar_t*       pszMangeled;
    
    //skip first line  
    psz = pszLink;
    
    if(wcsncmp(psz, L"version;", 8))
        return NULL;      

    for( ;*psz; psz++)
    {
        if(*psz==_T('\n'))
        {
            psz++;
            break;
        }
    }


    if(wcsncmp(psz, L"iecif;", 6))
        return NULL;

    psz += 6;
    
    pszFirst = psz;

    for( ;*psz && *psz!=_T('\n'); psz++)
        ;
    
    iLen = psz - pszFirst;
    if(pszFirst[iLen-1] == L'\r')
        iLen--;
    
    pszMangeled = new wchar_t[iLen+1];
    memcpy(pszMangeled, pszFirst, sizeof(wchar_t)*iLen);
    pszMangeled[iLen] = 0;

    return pszMangeled;
}





#if 0 
extern "C" bool CGT_CheckLibPOULinks(
    CGT_CDiagMsg*      pdm,
    ICG_IECFront*      pIFront,
    const TCHAR*       pszTrgType,
    const CG_Symtab*   pLibSym,
    const TCHAR*       pszCodeVersion,
    const CG_Symtab*** pppCalledFuns
)
{



}
#endif




