

//----  Includes:   -------------------------------------------*
#include "parseKAD.h"
#include "cgt_xmlparse.h"
#include "cgt_CDiagMsg.h"
#include "helper.h" //GetModulePath()




static void ReadIOComponents(
    CGT_CDiagMsg*            pdm,
    IXMLDOMNodeList*         pNList,
    FC_CGArray<IO_COMP_TYP*>*pIOcomp
);



void CKADInfo::clear()
{
    m_szKADFile.load(_T(""));
    m_IOcomp.deletePtrs();
}




bool CKADInfo::load(
    CGT_CDiagMsg*pdm,
	const TCHAR* pszKADFileAndPath
)
{
    long                i;
    long                len;
    IXMLDOMNodeList*    pNList;
    IXMLDOMNodeList*    pNList2;
    IXMLDOMNode*        pNode;
    int                 nCurrErrors;
    FC_CString          jot(MAX_PATH);
    
    nCurrErrors = pdm->getTotalErrors();
    
	//clear possible old content:
    clear();

    m_szKADFile.load(pszKADFileAndPath);

    pNode = CGT_XMLLoadFile(pdm, NULL, m_szKADFile, L"KAD4CPC");
    if(!pNode)
        return false;

    

    pNList = NULL;
    len = 0;
    pNode->selectNodes(L"IOCOMPONENTS", &pNList);
    assert(pNList);
    FC_RELEASE_PTR(pNode);

    pNList->get_length(&len);
    for(i=0;i<len;i++)
    {
        pNList->get_item(i, &pNode);
    
        pNList2 = NULL;
        pNode->selectNodes(L"FBUS", &pNList2);
        ReadIOComponents(pdm, pNList2, &m_IOcomp);
        FC_RELEASE_PTR(pNList2);

        FC_RELEASE_PTR(pNode);
    }
    FC_RELEASE_PTR(pNList);

    return nCurrErrors==pdm->getTotalErrors();
}


bool CKADInfo::getIOCompByExt(
    const TCHAR*  pszExt,
    GUID*         pClsid
)const
{
    int                i;
    const IO_COMP_TYP* pIOC;

    for(i=0;i<m_IOcomp.getUsed();i++)
    {
        pIOC = m_IOcomp.get(i);
        if(!lstrcmpi(pszExt, pIOC->szExt))
        {
            if(pClsid)
                *pClsid = pIOC->clsidFBUS;

            return true;
        }

    }
    return false;
}










//parse:  <FILE EXT="pam"  CONFIGURATOR="<uuid>" />
static void ReadIOComponents(
    CGT_CDiagMsg*            pdm,
    IXMLDOMNodeList*         pNList,
    FC_CGArray<IO_COMP_TYP*>*pIOcomp
)
{
    long                  i;
    long                  len;
    FC_CString            jot(MAX_PATH);
    IXMLDOMNode*          pNode;
    IO_COMP_TYP*          pIOC;
    bool                  bError;

    pNList->get_length(&len);
    for(i=0;i<len;i++)
    {
        pNList->get_item(i, &pNode);

        pIOC = new IO_COMP_TYP;
        bError = false;

        if(!CGT_XMLGetStrAttrTrim(pdm, pNode, L"EXT", &jot, true, false))
            bError = true;
        pIOC->szExt.load(jot);

        if(!CGT_XMLGetCLSIDAttr(pdm, pNode, L"CONFIGURATOR", &pIOC->clsidFBUS, true))
            bError = true;

        if(bError)
            delete(pIOC);
        else
            pIOcomp->add(pIOC);

        FC_RELEASE_PTR(pNode);
    }
}




