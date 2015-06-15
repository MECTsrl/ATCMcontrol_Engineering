


//----  Includes:   -------------------------------------------*
#include <time.h>
#include "helper.h"
#include "BuildNr.h"
#include "Product.h"


CParamList::CParamList()
 : m_strbag(false, 1023),
   m_instHash(true, false, 513)
{
    m_maxPathLen = 0;
}
CParamList::~CParamList()
{
    empty();
}

void CParamList::empty()
{
    INST_HASH_IT ii;

    m_fileName.load("");
    m_targetTyp.load("");
    m_targetVer.load("");
    m_projectName.load("");
    m_projectVer.load("");
    m_resName.load("");
    m_maxPathLen = 0;
    //TODO: m_strbag.clean() ??;

    for(ii=m_instHash.firstPair(); ii; ii=ii->pNext)
        ii->pVal->deletePtrs();
    m_instHash.deletePtrs();
}



bool CParamList::load(
    const TCHAR*  pszFileName, 
    TCHAR*        pszContent, 
    unsigned long ulChars
)
{
    bool           bOk         = false;
    FC_CSVMemFile* pCSV        = NULL;
    TCHAR*         pszCSVdata  = NULL;
    unsigned long  ulCSVChars  = 0;
    CParamList*    pIL         = NULL;

    empty();
    m_fileName.load(pszFileName);

    if(pszContent)
    {
        pszCSVdata  = pszContent;
        ulCSVChars  = ulChars;
    }
    else
    {
        if(!LoadTxtFileToMem(pszFileName, &pszCSVdata, &ulCSVChars))
            goto error;
    }


    pCSV = FC_CSVSplit(pszCSVdata, ulCSVChars);
    if(!pCSV)
    {
        _tprintf(_T("4dl: error: out of memory\n"));
        goto error;
    }
    else if(pCSV->lErrReason!=0)
    {
        _tprintf(_T("%s(%d): syntax error: bad CSV syntax\n"),pszFileName, pCSV->lErrReason);
        goto error;
    }

    bOk = parseHelp(pCSV);

error:
    if(pCSV)
        FC_CSVFreeMemFile(pCSV);
    if(pszCSVdata && !pszContent)
        FC_LibFreeMemory(pszCSVdata);

    if(!bOk)
        empty();

    return bOk;
}



int CParamList::merge(const CParamList* pOldPL, bool bWarn, long flags)
{
    INST_HASH_IT     ii;
    MEMB_HASH_IT     im;
    const MEMB_HASH* pMemOld;
    MEMB_HASH*       pMemNew;
    const CVarItem*  pItemOld;
          CVarItem*  pItemNew;
    const TCHAR*     pszOldFile;
    FC_CString       fullPath(MAX_PATH);


    pszOldFile = pOldPL->getFileName();

    if(_tcsicmp(pOldPL->getResName(), this->getResName()))
    {
        if(bWarn)
        {
            _tprintf(_T("%s(%d): warning: merge 4PL: resource name has changed from '%s' to '%s'"),
                pszOldFile, 1, pOldPL->getResName(), this->getResName());
        }
        return -1;
    }


    for(ii=pOldPL->m_instHash.firstPair(); ii; ii=ii->pNext)
    {
        if(g_bForceShutdown)
        {
            return 0;
            break;
        }

        pMemOld = ii->pVal;
        pMemNew = m_instHash.get(ii->pszKey);
        if(!pMemNew)
        {
            if(bWarn)
            {
                im = pMemOld->firstPair();
                assert(im);
                _tprintf(_T("%s(%d): warning: merge 4PL: instance '%s' does not exist any more\n"),
                    pszOldFile, im?im->pVal->nLineNo:1, ii->pszKey);
            }
            continue;
        }
        
        for(im=pMemOld->firstPair(); im; im=im->pNext)
        {
            if(g_bForceShutdown)
            {
                return 0;
                break;
            }

            pItemOld = im->pVal;
            pItemNew = pMemNew->get(im->pszKey);
            if(!pItemNew)
            {
                if(bWarn)
                {
                    pOldPL->buildFullItemPath(pItemOld, &fullPath, NULL);
                    _tprintf(_T("%s(%d): warning: merge 4PL: '%s' does not exist any more\n"),
                        pszOldFile, pItemOld->nLineNo, fullPath.psz());
                }
                continue;
            }


            if(pItemNew->m_typ != pItemOld->m_typ)
            {
                if(bWarn)
                {
                    pOldPL->buildFullItemPath(pItemOld, &fullPath, NULL);
                    _tprintf(_T("%s(%d): warning: merge 4PL: '%s' changed type from '%s to '%s'\n"),
                        pszOldFile, pItemOld->nLineNo, fullPath.psz(), 
                        BLTypToString(pItemOld->value.typ), BLTypToString(pItemNew->value.typ));
                }
                continue;
            }

            if(pItemOld->value.typ == BL_ANY)
            {
                if(bWarn)
                {
                    pOldPL->buildFullItemPath(pItemOld, &fullPath, NULL);
                    _tprintf(_T("%s(%d): error: merge 4PL: '%s' does not have a value\n"),
                        pszOldFile, pItemOld->nLineNo, fullPath.psz());
                    return 0;
                }
                continue;
            }
            CopyBinVal(&pItemNew->value, &pItemOld->value);
        }
    }


    //remove items that have no value:
    for(ii=m_instHash.firstPair(); ii; )
    {
        if(g_bForceShutdown)
        {
            return 0;
            break;
        }

        pMemNew = ii->pVal;
        for(im=pMemNew->firstPair(); im; )
        {
            if(g_bForceShutdown)
            {
                return 0;
                break;
            }

            pItemNew = im->pVal;
            im = im->pNext;

            if(pItemNew->value.typ == BL_ANY)
            {
                if(bWarn)
                {
                    buildFullItemPath(pItemNew, &fullPath, NULL);
                    _tprintf(_T("%s(%d): warning: merge 4PL: new parameter '%s'\n"),
                        m_fileName.psz(), pItemNew->nLineNo, fullPath.psz());
                }

                ii->pVal->remove(pItemNew->pszMember);
                delete(pItemNew);
            }
            else if(pItemNew->value.typ != pItemNew->m_typ)
            {
                _tprintf(_T("%s(%d): internal error: merge 4PL: type mismatch\n"),
                    m_fileName.psz(), pItemNew->nLineNo);
                return false;
            }
        }

        if(pMemNew->countPairs()==0)
        {
            const TCHAR* pszInst = ii->pszKey;
            ii = ii->pNext;
            delete(pMemNew);
            m_instHash.remove(pszInst);
        }
        else
        {
            ii = ii->pNext;
        }
    }

    return 1;
}




int CParamList::getItemCount()const
{
    int          nItems;
    INST_HASH_IT ii;

    nItems = 0;
    for(ii=m_instHash.firstPair(); ii; ii=ii->pNext)
        nItems += ii->pVal->countPairs();

    return nItems;
}
CVarItem* CParamList::getFirstVar(ITEMPOS_IT* pPos)
{
    pPos->pInst = m_instHash.firstPair();
    pPos->pMemb = pPos->pInst ? pPos->pInst->pVal->firstPair() : NULL;   
    return pPos->pMemb ? pPos->pMemb->pVal : NULL;
}
CVarItem* CParamList::getNextVar(ITEMPOS_IT* pPos)
{
    if(pPos->pMemb)
        pPos->pMemb = pPos->pMemb->pNext;

    if(!pPos->pMemb && pPos->pInst)
    {
        pPos->pInst = pPos->pInst->pNext;
        pPos->pMemb = pPos->pInst ? pPos->pInst->pVal->firstPair() : NULL;   
    }
    return pPos->pMemb ? pPos->pMemb->pVal : NULL;
}




bool CParamList::dump(const TCHAR* pszFile, bool bHeader, const TCHAR* pszControlAddr)
{
    FILE*               pFile;
    FC_CString          jot(MAX_PATH);
    INST_HASH_IT        ii;
    MEMB_HASH_IT        im;


    pFile = _tfopen(pszFile, _T("wt"));
    if(!pFile)
    {
        //try to create path on first fopen error:
        jot.load(pszFile).stripFilePart();
        FC_CreateSubDir(jot);
        pFile = _tfopen(pszFile, _T("wt"));
    }

    if(!pFile)
    {
        _tprintf(_T("4dl: error: cannot open file '%s' for writing\n"),pszFile);
        return false;
    }



    //dump magic header:
    //$format;1
    //$genver;2.05.2606.0
    //$resource;Resource1

    //if(bHeader)
        //$target;4cwin;20500
        //$created
        //$project;simpleLocRet
        //$projver;
        //$resource;Resource1
        //$control;

    _fputts(_T("$format;1\n"), pFile);
    _fputts(_T("$genver;")PROD_VERSION_STR _T("\n"), pFile);
    _fputts(_T("$resource;"), pFile);
    _fputts(m_resName, pFile);
    _fputtc(_T('\n'), pFile);

    if(bHeader)
    {
        struct tm* newtime;
        time_t     aclock;
        
        time(&aclock);
        newtime = localtime(&aclock);

        jot.load(_tasctime(newtime)).trim();  
        _fputts(_T("$created;"), pFile);
        _fputts(jot, pFile);
        _fputtc(_T('\n'), pFile);

        _fputts(_T("$target;"), pFile);
        _fputts(m_targetTyp, pFile);
        _fputtc(_T(';'), pFile);
        _fputts(m_targetVer, pFile);
        _fputtc(_T('\n'), pFile);

        _fputts(_T("$project;"), pFile);
        _fputts(m_projectName, pFile);
        _fputtc(_T('\n'), pFile);

        _fputts(_T("$projver;"), pFile);
        FC_CSVRecordToFILE(pFile, m_projectVer, -1);
        _fputtc(_T('\n'), pFile);

        _fputts(_T("$uploaded;"), pFile);
        if(pszControlAddr)
            FC_CSVRecordToFILE(pFile, pszControlAddr, -1);
        _fputtc(_T('\n'), pFile);
        _fputtc(_T('\n'), pFile);
        _fputtc(_T('\n'), pFile);
    }

   
    //dump item list:
    _fputts(_T("$list\n"), pFile);
    for(ii=m_instHash.firstPair(); ii; ii=ii->pNext)
    {
        if(ii->pszKey[0])
        {
            _fputts(ii->pszKey, pFile);
            _fputtc(_T('\n'), pFile);
        }

        for(im=ii->pVal->firstPair(); im; im=im->pNext)
        {
            const BINVAL_TYP* pValue = &im->pVal->value;
            
            if(ii->pszKey[0])
                _fputtc(_T('.'), pFile);

            _fputts(im->pszKey, pFile);
            _fputtc(_T(';'), pFile);
            _fputts(BLTypToString(im->pVal->m_typ), pFile);

            if(pValue->typ != BL_ANY)
            {
                _fputtc(_T(';'), pFile);
                if(pValue->typ==BL_STRING || pValue->typ==BL_WSTRING)
                {
                    FC_CSVRecordToFILE(pFile, pValue->pszStr, pValue->nStrlen);
                }
                else
                {
                    BinValToStrSimle(pValue, &jot);
                    _fputts(jot, pFile);
                }
            }
             _fputtc(_T('\n'), pFile);
        }
    }

    fclose(pFile);
    return true;
}




void CParamList::buildFullItemPath(const CVarItem* pItem, FC_CString* pJot, wchar_t** ppszNew)const
{
    pJot->load(m_resName).addc(_T('.'));
    assert(!m_resName.isEmpty());

    if(pItem->pszInst[0])
        pJot->addpsz(pItem->pszInst).addc(_T('.'));

    pJot->addpsz(pItem->pszMember);

    if(ppszNew)
    {
        int iChars = pJot->len()+1;
        *ppszNew = new wchar_t[iChars];
    #ifdef UNICODE
        memcpy(*ppszNew, pJot->psz(), sizeof(wchar_t)*iChars);
    #else
        FC_StringToWideChar(pJot->psz(), *ppszNew, iChars);
    #endif
    }
}



























//private helper functions:
MEMB_HASH* CParamList::insertInst(const TCHAR* pszInstReg)
{
    MEMB_HASH* pInst;
    
    assert(m_strbag.registerString(pszInstReg) == pszInstReg);
    
    pInst = m_instHash.get(pszInstReg);
    if(!pInst)
    {
        pInst = new MEMB_HASH(true, false);
        m_instHash.insert(pszInstReg, pInst);
    }
    return pInst;
}

bool CParamList::registerItem(
    long         nLineNo, 
    const TCHAR* pszPath, 
    const TCHAR* pszMember, 
    const TCHAR* pszType,
    const TCHAR* pszValue
)
{
    int                 i;
    FC_CString          jot(MAX_PATH);
    CVarItem*           pIt;
    BL_IEC_TYP          blTyp;
    long                nArrDim;
    int                 iLenPath;
    MEMB_HASH*          pInst;
    const TCHAR*        pszPathReg;
    const TCHAR*        pszMemberReg;
    TCHAR               errDescr[MAX_PATH];
    

    blTyp = ParseSimpleType(pszType, &nArrDim);
    if(blTyp == BL_ANY || nArrDim<0)
    {
        _tprintf(_T("%s(%d): error: data type '%s' not a simple type or invalid\n"),
            m_fileName.psz(), nLineNo, pszType);
        return false;
    }

    
    if(nArrDim==0)
    {
        pszPathReg   = m_strbag.registerString(pszPath);
        pszMemberReg = m_strbag.registerString(pszMember);
        pIt = new CVarItem(nLineNo, pszPathReg, pszMemberReg, blTyp);
        
        if(pszValue)
        {
            if(!StrSimleToBinVal(pszValue, blTyp, &pIt->value, errDescr))
            {
                _tprintf(_T("%s(%d): error: value format error '%s': %s\n"),
                    m_fileName.psz(), nLineNo, pszValue, errDescr);
                return false;
            }
            if(errDescr[0])
            {
                _tprintf(_T("%s(%d): warning: value '%s': %s\n"),
                    m_fileName.psz(), nLineNo, pszValue, errDescr);
            }
        }

        pInst = insertInst(pszPathReg);
        if(!pInst->insertEx(pszMemberReg, pIt, false, NULL))
        {
            buildFullItemPath(pIt, &jot, NULL);
            _tprintf(_T("%s(%d): error: second definition of '%s'\n"),
                m_fileName.psz(), nLineNo, jot.psz());
            delete(pIt);
            return false;
        }
        iLenPath = (pszPathReg[0]?_tcslen(pszPathReg)+1:0)+_tcslen(pszMember);
    }
    else
    {
        if(pszValue)
        {
            _tprintf(_T("%s(%d): error: item with array type '[]' must not have value field\n"),
                m_fileName.psz(), nLineNo);
            return false;
        }

        if(pszPath[0])
        {
            jot<<pszPath<<_T(".")<<pszMember;
            pszPathReg = m_strbag.registerString(jot);
        }
        else
        {
            pszPathReg = m_strbag.registerString(pszMember);
        }

        pInst = insertInst(pszPathReg);
        for(i=0; i<nArrDim; i++)
        {
            pszMemberReg = m_strbag.registerString(jot.clear()<<i);
            pIt = new CVarItem(nLineNo, pszPathReg, pszMemberReg, blTyp);
            bool bOk = pInst->insert(pszMemberReg, pIt);
            assert(bOk);
        }
        iLenPath = _tcslen(pszPathReg)+1+(jot.clear()<<nArrDim).len();
    }

    if(iLenPath > m_maxPathLen)
        m_maxPathLen = iLenPath;

    return true;
}


bool CParamList::parseHelp(FC_CSVMemFile* pCSV)
{
    int                 i;
    bool                bInList           = false;
    bool                bFirstNonEmptyRec = true;
    const TCHAR*        pszLastInst       = _T("");
    TCHAR*              pszF1;
    TCHAR*              pszF2;
    TCHAR*              pszF3;
    long                nLineNo;
    FC_CString          jot(MAX_PATH);
    const TCHAR*        pszFileName = m_fileName;
    bool                bError = false;


    //trim all strings first and ignore empty records:
    for(i=0; i<pCSV->nRec; i++)
    {
        FC_CSVRecord* pRec = pCSV->ppRec[i];
        //there is always one field:
        assert(pRec->nFields>0);
        //trim the first record:
        pRec->fields[0].lLength = FC_StringTrim(pRec->fields[0].pszField);

        if(pRec->fields[0].lLength == 0)
            continue; //empty first record is a comment ...

        nLineNo   = pRec->fields[0].lLineNo; 
        pszF1 = pRec->fields[0].pszField;
        pszF2 = pRec->nFields>=2 ? pRec->fields[1].pszField : NULL;
        pszF3 = pRec->nFields>=3 ? pRec->fields[2].pszField : NULL;

        if(bFirstNonEmptyRec)
        {
            bFirstNonEmptyRec = false;
            if(pRec->nFields==2 && !_tcscmp(pszF1, _T("$format")) && !_tcscmp(pszF2, _T("1")))
                continue;

            _tprintf(_T("%s(%d): error: expected '$format;1'\n"), pszFileName, nLineNo);
            goto error;
        }

        if(*pszF1==_T('$'))
        {
            //$format;1
            //$created
            //$target;4cwin;20500
            //$genver;2.05.2606.0
            //$project;simpleLocRet
            //$projver;
            //$resource;Resource1
            //$control;
            //$list
            //gr4;string
            //gr5;string
            //gr1;bool

            if(!_tcscmp(pszF1, _T("$resource")))
            {
                if(pszF2)
                {
                    m_resName.load(pszF2);
                    continue;
                }
                _tprintf(_T("%s(%d): error: missing resource name after '$resource;'\n"), 
                    pszFileName, nLineNo);
                goto error;
            }
            else if(!_tcscmp(pszF1, _T("$target")))
            {
                if(pszF3)
                {
                    m_targetTyp.load(pszF2);
                    m_targetVer.load(pszF3);
                    continue;
                }
                _tprintf(_T("%s(%d): error: missing <target type> or <version after> '$target;'\n"), 
                    pszFileName, nLineNo);
                goto error;
            }
            else if(!_tcscmp(pszF1, _T("$genver")))
            {
                //ignore
                continue;
            }
            else if(!_tcscmp(pszF1, _T("$project")))
            {
                if(pszF2)
                {
                    m_projectName.load(pszF2);
                    continue;
                }
                _tprintf(_T("%s(%d): error: missing <project name> after '$project;'\n"), 
                    pszFileName, nLineNo);
                goto error;
            }
            else if(!_tcscmp(pszF1, _T("$projver")))
            {
                if(pszF2)
                {
                    m_projectVer.load(pszF2);
                    continue;
                }
                _tprintf(_T("%s(%d): error: missing <project verion> after '$projver;'\n"), 
                    pszFileName, nLineNo);
                goto error;
            }
            else if(!_tcscmp(pszF1, _T("$control")) || !_tcscmp(pszF1, _T("$uploaded")) || !_tcscmp(pszF1, _T("$created")))
            {
                //ignore
                continue;
            }
            else if(!_tcscmp(pszF1, _T("$list")))
            {
                if(m_resName.isEmpty())
                {
                    _tprintf(_T("%s(%d): error: missing $resource;<resource name>\n"), 
                        pszFileName, nLineNo);
                    goto error;
                }

                bInList = true;
                continue;
            }
            else if(!_tcscmp(pszF1, _T("$end")))
            {
                bInList = false;
                continue;
            }
            else
            {
                _tprintf(_T("%s(%d): warning: unknown $<id> tag '%s'\n"), 
                    pszFileName, nLineNo, pszF1);
                continue;
            }

            if(bInList)
            {
                _tprintf(_T("%s(%d): error: unexpected $<id> syntax inside list body\n"), 
                    pszFileName, nLineNo);
                goto error;
            }
            else
            {
                continue;
            }
        }
        else if(!bInList)
        {
            continue;
        }

        if(pRec->nFields>3)
        {
            _tprintf(_T("%s(%d): error: unexpected field #%d '%s'\n"), 
                pszFileName, nLineNo, 4, pRec->fields[3].pszField);
            bError = true;
            continue;
        }

        if(*pszF1 == _T('.'))
        {
            if(pszLastInst[0]==0)
            {
                _tprintf(_T("%s(%d): error: unexpected '%s': no preceeding instance record\n"), 
                    pszFileName, nLineNo, pszF1);
                bError = true;
                continue;
            }
            //.m2;bool       => no  value => pszF3 is NULL
            //.m2;bool;true  => has value => pszF3 is !NULL
            if(!registerItem(nLineNo, pszLastInst, pszF1+1, pszF2, pszF3))
                bError = true;
        }
        else if(pRec->nFields==1)
        {
            pszLastInst = pszF1;
        }
        else
        {
            //program1.struct1.m2;bool[3]
            TCHAR*  pszMember = _tcsrchr(pszF1, _T('.'));
            if(pszMember)
            {
                //convert string inplace:
                *pszMember++ = 0;
                pszLastInst = pszF1;
            }
            else
            {
                pszMember   = pszF1;
                pszLastInst = _T("");
            }
            if(!registerItem(nLineNo, pszLastInst, pszMember, pszF2, pszF3))
                bError = true;
            //now the last inst is not set any longer:
            pszLastInst = _T("");
        }
    }

    return !bError;
error:
    return false;
}


