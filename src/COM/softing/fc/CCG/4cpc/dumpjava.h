#ifndef DUMPJAVA_H_
#define DUMPJAVA_H_

#include "cgt_CDiagMsg.h"
#include "cgt_tools.h"
class CGlobCtx;

class CCodeInfo
{
public:
    CCodeInfo(const CG_Symtab* pSym) 
        : m_pSym(pSym)
    {
        m_bErrors = false;
        m_jitmenot= false;
        m_pIExtBdyInf = NULL;
    }
    ~CCodeInfo()
    {
        if(m_pIExtBdyInf)
            m_pIExtBdyInf->Release();
    }

    bool                  m_bErrors;
    const CG_Symtab*const m_pSym;
    ICG_ExtBodyInfo*      m_pIExtBdyInf;
    
    FC_CStr               m_classFile;//relative to gen
    FC_CStr               m_jifFile;  //relative to gen

    bool                  m_jitmenot;//jitmenot attr for this POU
};

//get precompiled code from lib:
void GenCodeLibPOU(
    CGlobCtx*               pgctx,
    const CG_Symtab*        pSym,
    CCodeInfo*              pInfo,
    FC_CGArray<CGT_BP_TYP>* pbpList
);
extern "C" void GenCodeStruct(
    CGlobCtx*                 pgctx,
    CCodeInfo*                pInfo
);
extern "C" void GenCodeSTPrgFB(
    CGlobCtx*                 pgctx,
    ICG_Body*                 pIBdy,
    const CCodeInfo*const*    pzCalledFuns,
    CCodeInfo*                pInfo,
    FC_CGArray<CGT_BP_TYP>*   pbpList
);
extern "C" void GenCodeSFCPrgFB(
    CGlobCtx*                 pgctx,
    ICG_Body*                 pIBdy,
    const CCodeInfo*const*    pzCalledFuns,
    CCodeInfo*                pInfo,
    FC_CGArray<CGT_BP_TYP>*   pbpList
);
extern "C" void GenCodeFun(
    CGlobCtx*                 pgctx,
    ICG_Body*                 pIBdy,
    CCodeInfo*                pInfo,
    FC_CGArray<CGT_BP_TYP>*   pbpList
);




#endif