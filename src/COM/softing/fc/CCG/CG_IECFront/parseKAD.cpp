

//----  Includes:   -------------------------------------------*
#include "parseKAD.h"
#include "fc_tools\fc_fileIO.h"
#include "XMLparse.h"
#include "cgt_CDiagMsg.h"
#include "helper.h"



#define WARNTRIMM true


//parse helper methods:
static bool ReadTargets(
    CGT_CDiagMsg*           pdm,
    const CG_Edipos*        pEdp,
    IXMLDOMNodeList*        pNList,
    FC_CGArray<CKADTarget*>*pTargets
);
static bool ReadLanguages(
    CGT_CDiagMsg*           pdm,
    const CG_Edipos*        pEdp,
    IXMLDOMNodeList*        pNList,
    FC_CGArray<CKADFilter*>*pFilters
);




void CKADInfo::clear()
{
    m_szKADFile.load(_T(""));
    m_szKADTargetExts.load(_T(""));
    m_szKADSourcesExts.load(_T(""));
    m_szKADLangExts.load(_T(""));
    m_targets.deletePtrs();
    m_filters.deletePtrs();
}



bool CKADInfo::load(
    CGT_CDiagMsg* pdm,
	const TCHAR*  pszKADFileAndPath
)
{
    long                i;
    long                len;
    IXMLDOMNodeList*    pNList;
    IXMLDOMNodeList*    pNList2;
    IXMLDOMNode*        pNode;
    BSTR                bstr;
    CG_Edipos           edp;
    FC_CString          jot(MAX_PATH);

    
	//clear possible old content:
    clear();

    m_szKADSourcesExts.load(CG_PREDEF_SOURCE_EXTS);
    m_szKADLangExts.load(CG_PREDEF_LANG_EXTS);
    m_szKADFile.load(pszKADFileAndPath);


    CG_InitSTEdipos(&edp, m_szKADFile);
    pNList = XML_LoadXMLFile(pdm, m_szKADFile, L"KADCGFRONT");
    if(!pNList)
        return false;


    pNList->get_length(&len);
    for(i=0;i<len;i++)
    {
        pNList->get_item(i, &pNode);
        pNode->get_nodeName(&bstr);

        if(!wcscmp(bstr, L"TARGETS"))
        {
            pNode->get_childNodes(&pNList2);
            ReadTargets(pdm, &edp, pNList2, &m_targets);
            pNList2->Release();

            for(int j=0; j<m_targets.getUsed(); j++)
            {
                //update extension list:
                //TODO: check duply defined ! this must also be done for the filters 
                if(j>0)
                    m_szKADTargetExts<<_T(",");
                m_szKADTargetExts<<m_targets.get(j)->szExt;
            }

        }
        else if(!wcscmp(bstr, L"LANGUAGES"))
        {
            pNode->get_childNodes(&pNList2);
            ReadLanguages(pdm, &edp, pNList2, &m_filters);
            pNList2->Release();

            for(int j=0; j<m_filters.getUsed(); j++)
            {
                CKADFilter* pFil;
                pFil = m_filters.get(j);
                m_szKADSourcesExts<<_T(",")<<pFil->szExt;
                m_szKADLangExts<<_T(",")<<pFil->szExt;
            }
        }
        else
        {
            pdm->msg1(CG_W_XML_STRUCTURE_ELEM_UNKNOWN, &edp, bstr);
            return false;
        }
    }

    return true;
}




//--- KAD info:
const TCHAR* CKADInfo::getFile(void)const
{
    return m_szKADFile;
}
const TCHAR* CKADInfo::getTargetTypeExts(void)const
{
    return m_szKADTargetExts;
}
int CKADInfo::countTargetTypes(void)const
{
    return m_targets.getUsed();
}
int CKADInfo::getTargetTypeByExt(
    const TCHAR*  pszExt,
    const TCHAR** ppsz,
    GUID*         puuidTrgInfo,
    boolean*      pbMultipleTrgs
)const
{
    int               i;
    const CKADTarget* pTrg;

    for(i=0;i<m_targets.getUsed();i++)
    {
        pTrg = m_targets.get(i);
        if(!lstrcmpi(pszExt, pTrg->szExt))
        {
            if(ppsz)
                *ppsz = pTrg->szBackend;
            if(puuidTrgInfo)
                *puuidTrgInfo = pTrg->clsidTargetInfo;
            if(pbMultipleTrgs)
                *pbMultipleTrgs = pTrg->bMultipleTrgs;

            return i+1;
        }

    }
    return 0;
}
const TCHAR* CKADInfo::getTargetTypeByNum(
    int           iTargetType,
    const TCHAR** ppsz,
    GUID*         puuidTrgInfo,
    boolean*      pbMultipleTrgs
)const
{
    const CKADTarget* pTrg;
    
    iTargetType--;

    if(iTargetType>=0 && iTargetType<m_targets.getUsed())
    {
        pTrg = m_targets.get(iTargetType);
        if(ppsz)
            *ppsz = pTrg->szBackend;
        if(puuidTrgInfo)
            *puuidTrgInfo = pTrg->clsidTargetInfo;
        if(pbMultipleTrgs)
            *pbMultipleTrgs = pTrg->bMultipleTrgs;
        return pTrg->szExt;
    }
    else
    {
        assert(!"CKADInfo::getTargetTypeByNum() iTargetType out of range");
    }

    return NULL;
}
const TCHAR* CKADInfo::getSourceExts()const
{
    return m_szKADSourcesExts;
}
const TCHAR* CKADInfo::getLangExts()const
{
    return m_szKADLangExts;
}

CG_SOURCETYPES CKADInfo::getSourceTypeByExt(
    const TCHAR* pszExt
)const
{
    if(!lstrcmpi(pszExt, CG_PREDEF_SOURCE_ST))
        return CG_SRC_TYPE_ST;
    else if(!lstrcmpi(pszExt, CG_PREDEF_SOURCE_IL))
        return CG_SRC_TYPE_IL;
    else if(!lstrcmpi(pszExt, CG_PREDEF_SOURCE_SDT))
        return CG_SRC_TYPE_SDT;
    else if(!lstrcmpi(pszExt, CG_PREDEF_SOURCE_HDR))
        return CG_SRC_TYPE_HDR;
    else if(!lstrcmpi(pszExt, CG_PREDEF_SOURCE_4GR))
        return CG_SRC_TYPE_4GR;
    else if(!lstrcmpi(pszExt, CG_PREDEF_SOURCE_GVL))
        return CG_SRC_TYPE_GVL;
    else if(!lstrcmpi(pszExt, CG_PREDEF_SOURCE_CST))
        return CG_SRC_TYPE_CST;
    else if(!lstrcmpi(pszExt, CG_PREDEF_SOURCE_RES))
        return CG_SRC_TYPE_RES;
    else if(!lstrcmpi(pszExt, CG_PREDEF_SOURCE_CON))
        return CG_SRC_TYPE_CON;
    else if(getFilterByExt(pszExt, NULL))
        return CG_SRC_TYPE_ADDON_FILTER;
    else if(!lstrcmpi(pszExt, CG_4CP_EXTENSION))
        return CG_SRC_TYPE_4CP;
    else
        return CG_SRC_TYPE_UNDEF;
}
bool CKADInfo::getFilterByExt(
    const TCHAR* pszExt, 
    GUID*        puuidFilter
)const
{
    int               i;
    const CKADFilter* pFil;

    for(i=0;i<m_filters.getUsed();i++)
    {
        pFil = m_filters.get(i);
        if(!lstrcmpi(pszExt, pFil->szExt))
        {
            if(puuidFilter)
                *puuidFilter = pFil->clsidFilter;
            return true;
        }
    }
    return false;
}


































//parse: <TARGET EXT="pc" BACKEND="pc\4PCCode.exe" GETTARGETINFO="uuid1"/>
static bool ReadTargets(
    CGT_CDiagMsg*           pdm,
    const CG_Edipos*        pEdp,
    IXMLDOMNodeList*        pNList,
    FC_CGArray<CKADTarget*>*pTargets
)
{
    long                  i;
    long                  len;
    BSTR                  bstr;
    FC_CString            jot(MAX_PATH);
    IXMLDOMNode*          pNode;
    IXMLDOMNamedNodeMap*  pAList;
    CKADTarget*           pTrg;
    int                   nOldErr;

    
    nOldErr = pdm->getTotalErrors();

    pNList->get_length(&len);
    for(i=0;i<len;i++)
    {
        pNList->get_item(i, &pNode);
        pNode->get_nodeName(&bstr);

        pNList->get_item(i, &pNode);
        pNode->get_nodeName(&bstr);

        if(!wcscmp(bstr, L"TARGET"))
        {
            pTrg = new CKADTarget();
            pNode->get_attributes(&pAList);

            XML_GetStrAttrTrim(pdm, pEdp,bstr,pAList, L"EXT", &jot, WARNTRIMM, false);

            pTrg->szExt.load(jot);

            XML_GetStrAttrTrim(pdm,pEdp,bstr,pAList, L"BACKEND", &jot, WARNTRIMM, false);

            //make a full path relative to "<setupdir>\Engineering\bin"
            //if back end path is relative, that is a commitment of kad:
            TCHAR tmp[2*MAX_PATH];
            FC_FileFullPath(CGT_GetEngBinDir(pdm), jot, tmp, FC_ARRAY_LEN(tmp));
            pTrg->szBackend.load(tmp);

            if(XML_GetStrAttrTrim(pdm,pEdp,bstr,pAList, L"GETTARGETINFO", &jot, WARNTRIMM, true))
                XML_CLSIDFromString(pdm,pEdp, jot, &pTrg->clsidTargetInfo);

            pTrg->bMultipleTrgs = false;
            if(XML_GetStrAttrTrim(pdm,pEdp,bstr,pAList, L"BUILD_MULTIPLE", &jot, WARNTRIMM, true))
            {
                if(!_tcscmp(_T("0"), jot))
                    pTrg->bMultipleTrgs = 0;
                else if(!_tcscmp(_T("1"), jot))
                    pTrg->bMultipleTrgs = 1;
                else
                    pdm->msg3(CG_E_XML_STRUCTURE_ERROR_VALUE, pEdp, _T("TARGET"), jot, _T("BUILD_MULTIBLE"));
            }

            pAList->Release();
            pTargets->add(pTrg);
        }
        else
        {
            pdm->msg1(CG_W_XML_STRUCTURE_ELEM_UNKNOWN, pEdp, bstr);
        }
        
        pNode->Release();
        SysFreeString(bstr);
    }

    return nOldErr==pdm->getTotalErrors();
}




//parse:     <FILETYP EXT="pn" IECFILTER="uuid3" />
static bool ReadLanguages(
    CGT_CDiagMsg*           pdm,
    const CG_Edipos*        pEdp,
    IXMLDOMNodeList*        pNList,
    FC_CGArray<CKADFilter*>*pFilters
)
{
    long                  i;
    long                  len;
    BSTR                  bstr;
    FC_CString            jot(MAX_PATH);
    IXMLDOMNode*          pNode;
    IXMLDOMNamedNodeMap*  pAList;
    CKADFilter*           pFil;
    bool                  bError = false;

    pNList->get_length(&len);
    for(i=0;i<len;i++)
    {
        pNList->get_item(i, &pNode);
        pNode->get_nodeName(&bstr);

        pNList->get_item(i, &pNode);
        pNode->get_nodeName(&bstr);

        if(!wcscmp(bstr, L"FILETYP"))
        {
            pFil = new CKADFilter();
            pNode->get_attributes(&pAList);

            if(!XML_GetStrAttrTrim(pdm,pEdp,bstr,pAList, L"EXT", &jot, WARNTRIMM, false))
                bError = true;
            pFil->szExt.load(jot);


            if(!XML_GetStrAttrTrim(pdm,pEdp,bstr,pAList, L"IECFILTER", &jot, WARNTRIMM, false))
                bError = true;
            
            if(!XML_CLSIDFromString(pdm,pEdp, jot, &pFil->clsidFilter))
                bError = true;

            pAList->Release();

            if(bError)
                delete(pFil);
            else
                pFilters->add(pFil);
        }
        else
        {
            pdm->msg1(CG_W_XML_STRUCTURE_ELEM_UNKNOWN, pEdp, bstr);
        }
        
        pNode->Release();
        SysFreeString(bstr);
    }

    return !bError;
}






