#include "cgt_misc.h"
#include "helper.h"
#include "parsers.h"
#include "st_ParseContext.h"
#include "CG_Messages.h"
#include "Hash4GR.h"
#include "Parse4GR.h"
#include "LDFBD.h"
#include "fc_tools\fc_fileIO.h" //FC_FileFullPath









ST_CSymtabPrv* ST_ParseAnyHeader(
    const CG_Edipos*     pEdp,
    CG_CIECFront*        pFront,
    const TCHAR*         pszFileLocator,
    const CG_LIBContext* pLibCtx
)
{
    unsigned long        ulLength;
    TCHAR*               pszFileContent;
    ST_CSymtabPrv*       pSym;
    __int64              ftLastWrite;
    st_ParseContext      parser(pFront);
    CG_SOURCETYPES       styp;
    CGT_CDiagMsg*        pdm;

    pdm = pFront->m_pdm;
    styp = pFront->m_kad.getSourceTypeByExt(FC_StringGetFileExt(pszFileLocator));

    if(styp==CG_SRC_TYPE_4GR)
    {
        if(!ST_Get4GRHeaderAndBody(pdm, pEdp, pFront, pszFileLocator, &pszFileContent, 
            NULL/*<-don't want the xml-body*/, &ulLength, &ftLastWrite)
          )
            return NULL;
    }
    else if(styp==CG_SRC_TYPE_ST || styp==CG_SRC_TYPE_IL || styp==CG_SRC_TYPE_SDT || styp==CG_SRC_TYPE_GVL 
            || styp==CG_SRC_TYPE_HDR || styp==CG_SRC_TYPE_CST)
    {
        //try to open the disk or lib text file:
        if(!CGT_LoadLocatorFileToMem(pdm, pEdp, pFront->m_myCOMIF, 
            pszFileLocator, &pszFileContent, &ulLength, &ftLastWrite)
          )
            return NULL;
    }
    else
    {
        CGT_IERR2(pdm, pEdp, pdm->jot1()<<_T("illegal source file type of inout: ")<<pszFileLocator);
        return NULL;
    }



    pSym = parser.parseHeader(
             pszFileLocator, 
             pLibCtx,
             pszFileContent, 
             ulLength,
             ftLastWrite
           );

    delete(pszFileContent);
    
    //assert: that there was an error msg:
    assert(pSym || pFront->m_pdm->hasErr());
    return pSym;
}



ST_CSymtabPrv* ST_ParseSTIL4GR(
    const CG_Edipos*     pEdp,
    CG_CIECFront*        pFront,
    const TCHAR*         pszFileLocator,
    const CG_LIBContext* pLibCtx,
    ST_CBody**           ppBody,   
    ST_CSymtabPrv*       pSymOld,
    long                 codeGenOpt
)
{
    ST_CSymtabPrv*       pSym;
    __int64              ftLastWrite;
    CG_SOURCETYPES       styp;
    unsigned long        ulLength;
    TCHAR*               pszFileContent;
    st_ParseContext      parser(pFront);


    *ppBody= NULL;
    styp = pFront->m_kad.getSourceTypeByExt(FC_StringGetFileExt(pszFileLocator));

    if(styp==CG_SRC_TYPE_4GR)
    {
        SFC_LDFBD_SignalHash  grElements(&parser);
        TCHAR                 fullPath[MAX_PATH];

        //HACK rework Parse4GRFile for new XML control.
        FC_FileFullPath(pFront->m_4CP.getPrjPath(), pszFileLocator, fullPath, FC_ARRAY_LEN(fullPath));
        if(!Parse4GRFile(
            pFront->m_pdm,
            pFront,
            fullPath, 
            &ftLastWrite,
            &pszFileContent,
            &grElements)
        )
            return pSymOld;


        if(pSymOld)
        {
            ulLength = 0;
            delete(pszFileContent);//don't need the text
            pszFileContent = NULL;

            assert(pSymOld->getCGSym()->flags & CG_STAB_HDR_NOERR);

            if(pSymOld->getCGSym()->ftLastWrite!=ftLastWrite)
            {
                pFront->m_pdm->msg1(CG_E_FILE_MODIFIED, NULL, pszFileLocator);
                return pSymOld;
            }
        }
        else
        {
            ulLength = lstrlen(pszFileContent);
        }

        pSym = parser.parse4GRFile(
            pszFileLocator,
            pLibCtx,            
            pszFileContent, 
            ulLength,
            grElements,
            ppBody,
            ftLastWrite,
            pSymOld,
            codeGenOpt
        );
        delete(pszFileContent);
    }
    else if(styp==CG_SRC_TYPE_ST)
    {
        //try to open the disk or lib text file:
        if(!CGT_LoadLocatorFileToMem(pFront->m_pdm, pEdp, pFront->m_myCOMIF, 
            pszFileLocator, &pszFileContent, &ulLength, &ftLastWrite)
          )
            return pSymOld;

        if(pSymOld)
        {
            assert(pSymOld->getCGSym()->flags & CG_STAB_HDR_NOERR);

            if(pSymOld->getCGSym()->ftLastWrite!=ftLastWrite)
            {
                pFront->m_pdm->msg1(CG_E_FILE_MODIFIED, NULL, pszFileLocator);
                delete(pszFileContent);
                return pSymOld;
            }
        }


        pSym = parser.parseWholeSTFile(
                 pszFileLocator, 
                 pLibCtx,
                 pszFileContent, 
                 ulLength,
                 ppBody,
                 ftLastWrite,
                 pSymOld
               );
        assert(pSym==pSymOld || !pSymOld);
        delete(pszFileContent);
    }
    else if(styp==CG_SRC_TYPE_IL)
    {
        //try to open the disk or lib text file:
        if(!CGT_LoadLocatorFileToMem(pFront->m_pdm, pEdp, pFront->m_myCOMIF, 
            pszFileLocator, &pszFileContent, &ulLength, &ftLastWrite)
          )
            return pSymOld;

        if(pSymOld)
        {
            assert(pSymOld->getCGSym()->flags & CG_STAB_HDR_NOERR);

            if(pSymOld->getCGSym()->ftLastWrite!=ftLastWrite)
            {
                pFront->m_pdm->msg1(CG_E_FILE_MODIFIED, NULL, pszFileLocator);
                delete(pszFileContent);
                return pSymOld;
            }
        }


        pSym = parser.parseWholeILFile(
                 pszFileLocator, 
                 pLibCtx,
                 pszFileContent, 
                 ulLength,
                 ppBody,
                 ftLastWrite,
                 pSymOld
               );
        assert(pSym==pSymOld || !pSymOld);
        delete(pszFileContent);
    }
    else
    {
        CGT_IERR2(pFront->m_pdm, pEdp, pFront->m_pdm->jot1()<<_T("illegal source file type of inout: ")<<pszFileLocator);
        return pSymOld;
    }

    
    //assert: that there was an error msg:
    assert(pSym || pFront->m_pdm->hasErr());
    return pSym;
}


ST_CConfig* ST_ParseCON(
    const CG_Edipos* pEdp,
    CG_CIECFront*    pFront,
    const TCHAR*     pszFileAndRelPath
)
{
    unsigned long        ulLength;
    TCHAR*               pszFileContent;
    ST_CConfig*          pCon;
    __int64              ftLastWrite;
    st_ParseContext      parser(pFront);


    //try to open the disk file:
    if(!ST_LoadTextFileToMem(pFront->m_pdm, pFront->m_4CP.getPrjPath(), 
        pEdp, pszFileAndRelPath, &pszFileContent, &ulLength, &ftLastWrite)
      )
        return NULL;


    pCon = parser.parseConfiguration(
             pszFileAndRelPath, 
             pszFileContent, 
             ulLength,
             ftLastWrite
           );

    delete(pszFileContent);
    
    //assert: that there was an error msg:
    assert(pCon || pFront->m_pdm->hasErr());
    return pCon;
}




CG_Resource* ST_ParseRES(
    const CG_Edipos* pEdp,
    CG_CIECFront*    pFront,
    const TCHAR*     pszFileAndRelPath,
    ST_CSymtabPrv**  ppSymPrvNew
)
{
    unsigned long        ulLength;
    TCHAR*               pszFileContent;
    CG_Resource*         pRes;
    __int64              ftLastWrite;
    st_ParseContext      parser(pFront);

    *ppSymPrvNew = NULL;

    //try to open the disk file:
    if(!ST_LoadTextFileToMem(pFront->m_pdm, pFront->m_4CP.getPrjPath(), 
        pEdp, pszFileAndRelPath, &pszFileContent, &ulLength, &ftLastWrite)
      )
        return NULL;

    pRes = parser.parseResource(
             pszFileAndRelPath, 
             pszFileContent, 
             ulLength,
             ppSymPrvNew,
             ftLastWrite
           );

    delete(pszFileContent);
    
    //assert: that there was an error msg:
    assert((pRes && *ppSymPrvNew) || pFront->m_pdm->hasErr());
    return pRes;
}

