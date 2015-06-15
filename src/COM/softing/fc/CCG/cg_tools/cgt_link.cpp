#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_fileIO.h"
#include "cgt_link.h"
#include "cgt_misc.h"



//TODO: error if src file is unicode file 


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

extern "C" bool CGT_DumpLinkFile(
    CGT_CDiagMsg*      pdm,   
    ICG_IECFront*      pIF,   
    const CG_Symtab*   pSym,  
    bool               bSourceCode,
    const TCHAR*       pszTrgType, 
    const TCHAR*       pszVersion, 
    const TCHAR*       pszClass,
    const TCHAR*       pszBackend
)
{
    int                     i;
    FC_CString              lfile(MAX_PATH);
    CGT_CDmp                dmp;
    BSTR                    mangeld;      
    const CG_Symtab*const*  ppSym;
    long                    nHdr;
    TCHAR*                  pszHdr;
    bool                    bOk;
    FC_CString              genHdrFullPath(MAX_PATH);
    FC_CString              prjSourceFullPath(MAX_PATH);


    //makes no sense at all:
    assert(!pSym->pLibCtx);

    if((pSym->flags & CG_STAB_HDR_NOERR)==0 || pSym->pLibCtx)
    {
        CGT_IERR2(pdm, &pSym->edp, _T("CGT_DumpLinkFile: symtab invalid or is lib POU")); 
        return false;
    }   

//CG_LINK_LIB_VERSION:
    //version string:
    dmp<<CG_LINK_LIB_VERSION _T(";")<<(pszVersion?pszVersion:_T(""))<< _T('\n');


//CG_LINK_LIB_IECIF:
    mangeld = pIF->createIECMangeledName(pSym);
    assert(mangeld); //cannot fail, return false above if sym was bad
    dmp<<CG_LINK_LIB_IECIF _T(";")<<mangeld<<_T('\n');
    FC_FREESYSTRING(mangeld);


//CG_LINK_LIB_POUCLASS:
    //display type for libraian, may be custom defined:
    if(!pszClass)
    {
        switch(pSym->pouTyp)//use default..
        {
        case CG_POU_FUN: pszClass = _T("FUNCTION");break;
        case CG_POU_FB:  pszClass = _T("FUNCTION_BLOCK");break;
        case CG_POU_SDT: pszClass = _T("STRUCT");break;
        case CG_POU_PRG: pszClass = _T("PROGRAM");break;
        default:
            CGT_IERR2(pdm, &pSym->edp, _T("CGT_DumpLinkFile: invalid POU type for lib insert")); 
            return false;
        }
    }
    dmp<<CG_LINK_LIB_POUCLASS _T(";")<<pszClass<<_T('\n');



//CG_LINK_LIB_TRGTYP:
    dmp<<CG_LINK_LIB_TRGTYP _T(";")<<(pszTrgType?pszTrgType:_T(""))<<_T('\n');


//CG_LINK_LIB_HEADER
    //get header portion and dump ..\.link\<pou name>.hdr:
    //+ 1 for 0-termination:
    nHdr   = pSym->ulHdrBytes/sizeof(TCHAR);
    pszHdr = new TCHAR[nHdr+1];
    bOk = pIF->getIECHeaderText(pSym, nHdr+1, pszHdr)!=0;
    if(bOk)
    {
        assert(pSym->srcTyp != CG_SRC_TYPE_ADDON_FILTER);//<-TODO ?

        genHdrFullPath.load(pIF->FCPgetGenPath());

        if(pszTrgType && pszTrgType[0])
            genHdrFullPath<<_T("\\.")<<pszTrgType<<_T("\\.link\\");
        else
            genHdrFullPath<<_T("\\.link\\");

        genHdrFullPath<<FC_StringGetFilePart(pSym->pszId);

        if(pSym->pouTyp==CG_POU_SDT)
            genHdrFullPath<<_T(".")CG_PREDEF_SOURCE_SDT;
        else
            genHdrFullPath<<_T(".")CG_PREDEF_SOURCE_HDR;

        //assert: must not exist, directory should be clean:
        assert(!FC_FileExists(pIF->FCPgetGenPath(), genHdrFullPath));

        bOk = CGT_WriteFile(pdm, NULL, genHdrFullPath,
                 pszHdr, nHdr*sizeof(TCHAR), CGT_WF_STD);
    } 

    delete(pszHdr);
    if(!bOk)
        return false;

    assert(pSym->ulIF_Bytes<=pSym->ulHdrBytes && pSym->ulIF_Bytes>0);

    dmp<<CG_LINK_LIB_HEADER _T(";")<<FC_StringGetFilePart(genHdrFullPath)<<_T(';')
        <<pSym->ulIF_Bytes<<_T('\n');


//CG_LINK_LIB_SOURCE:
    dmp<<CG_LINK_LIB_SOURCE _T(";");

    //need source ?
    if(pszTrgType && pszTrgType[0])
    {
        if(bSourceCode || (pSym->flags & (CG_STAB_MACRO|CG_STAB_OPERATOR))!=0 ||
            pSym->pouTyp==CG_POU_SDT
          )
        {
            if(!FC_StringIsAbsPath(pSym->edp.pszFile))
                prjSourceFullPath<<pIF->FCPgetPrjPath();
        
            prjSourceFullPath<<_T("\\")<<pSym->edp.pszFile;
            dmp<<FC_StringGetFilePart(prjSourceFullPath);
        }
    }//else: only template if no trg typ
    dmp<<_T('\n');

//CG_LINK_LIB_POUATTR:
    if(pSym->pAttribs)
    {
        for(i=0; i<pSym->pAttribs->nAttribs; i++)
        {
            dmp<<CG_LINK_LIB_POUATTR _T(";");
            dmp<<pSym->pAttribs->pairs[i].pszKey<<_T(';');
            dmp.dumpAsCSV(pSym->pAttribs->pairs[i].pszVal,-1,true)<<_T('\n');
        }
    }

//CG_LINK_LIB_LINK
    if(pSym->pzRefSyms)
    {
        for(ppSym=pSym->pzRefSyms; *ppSym; ppSym++)
        {
            mangeld = pIF->createIECMangeledName(*ppSym);
            dmp<<CG_LINK_LIB_LINK _T(";")<<mangeld<<_T('\n');
            FC_FREESYSTRING(mangeld);
        }
    }
    else
    {
        CGT_IERR2(pdm, &pSym->edp, _T("CGT_DumpLinkFile: POU has body but no CG_Symtab::pzHdrRefSyms==NULL")); 
        return false;    
    }

    //TODO var_external and CONST use ?
    if(pSym->nExternal>0)
    {
    }


//CG_LINK_LIB_PUT  the IEC header dumped above and opt. source:
    if(pszTrgType && pszTrgType[0])
    {
        dmp<<CG_LINK_LIB_PUT _T(";")<<genHdrFullPath<<_T('\n');
        if(!prjSourceFullPath.isEmpty())
            dmp<<CG_LINK_LIB_PUT _T(";")<<prjSourceFullPath<<_T('\n');


        //append back end domain list:
        if(pszBackend)
            dmp<<pszBackend;
    }
    else
    {
        assert(!pszBackend);
    }


    //write link file to disk:
    if(pszTrgType && pszTrgType[0])
        lfile.load(_T("."))<<pszTrgType<<_T("\\.link\\");
    else
        lfile.load(_T("\\.link\\"));

    lfile<<FC_StringGetFilePart(pSym->edp.pszFile)<<_T(".link");
    
    //assert: must not exist, directory should be clean:
    assert(!FC_FileExists(pIF->FCPgetGenPath(), lfile));

    return CGT_DmpToFile(pdm, pIF->FCPgetGenPath(), lfile, dmp, false); 
}
