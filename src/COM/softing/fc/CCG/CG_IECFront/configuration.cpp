#include "configuration.h"


ST_CConfig::ST_CConfig(
    CG_CIECFront*    pFront,
    const CG_Edipos* pEdp, 
    const TCHAR*     pszId,
    CG_AttribList*   pAttribs,
    __int64          ftLastWrite
)
{
    assert(ftLastWrite!=0 && pFront);

    m_pFront = pFront;
    m_edp = *pEdp; 
    m_id.load(pszId);
    m_pAttribs = pAttribs;
    m_ftLastWrite = ftLastWrite;

}

ST_CConfig::~ST_CConfig()
{
    delete(m_pAttribs);
}

const CG_ResImpInfo* ST_CConfig::getResImpByName(const TCHAR* pszRES)
{
    int                  i;
    const CG_ResImpInfo* pImp;

    for(i=0; i<m_resImp.getUsed(); i++)
    {
        pImp = m_resImp.getAddrOf(i);
        if(!lstrcmpi(pszRES, pImp->pszRes))
            return pImp;
    }
    return NULL;
}

bool ST_CConfig::addResImp(
    const CG_Edipos* pEdp, 
    const TCHAR*     pszFile //already canonical path
)
{
    CG_ResImpInfo* pImp;
    FC_CString     jot(MAX_PATH);

    jot.load(FC_StringGetFilePart(pszFile));
    jot.changeFileExt(NULL);

    if(getResImpByName(jot))
        return false;

    pImp = m_resImp.addZero();
    pImp->edp = *pEdp;
    pImp->pszImpFile = m_pFront->registerString(pszFile);
    pImp->pszRes = m_pFront->registerString(jot);
    return true;
}




