#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include "helper.h"

class CG_CIECFront;
class ST_CConfig
{
    FC_CGArray<CG_ResImpInfo> m_resImp;
    CG_CIECFront*    m_pFront;
    CG_Edipos        m_edp;
    FC_CStr          m_id;
    CG_AttribList*   m_pAttribs;
    __int64          m_ftLastWrite;

public:
    ST_CConfig(
        CG_CIECFront*    pFront,
        const CG_Edipos* pEdp, 
        const TCHAR*     pszId,
        CG_AttribList*   pAttribs,
        __int64          ftLastWrite
    );
    ~ST_CConfig();

    bool addResImp(
        const CG_Edipos* pEdp, 
        const TCHAR* pszFile
    );

    const TCHAR*         getName()const    {return m_id;}
    const CG_Edipos*     getEdp()const     {return &m_edp;}
    const CG_AttribList* getAttribs()const {return m_pAttribs;}
    const __int64        getFileTime()const{return m_ftLastWrite;}

    const CG_ResImpInfo* getResImpByName(
        const TCHAR* pszRES
    );

};



#endif