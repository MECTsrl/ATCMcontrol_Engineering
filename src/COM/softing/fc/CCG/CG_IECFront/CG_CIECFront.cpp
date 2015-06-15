
#include <stdio.h>
#include "fc_tools\fc_fileIO.h"
#include "cgt_misc.h"
#include "CG_AutoDecl.h"
#include "cgt_link.h"
#include "cgt_address.h"
#include "helper.h"
#include "configuration.h"
#include "literals.h"
#include "CG_CIECFront.h"
#include "CG_Messages.h"
#include "XMLparse.h"
#include "parsers.h"
#include "XREFgen.h"
#include "expr.h" //##AC need for ST_PARAMSPEC

//local helper routines (at end of file):
static int ST_InitFunctionDef(
    const TCHAR*         pszId, 
    const ST_TypeSet_C  *lpTypes,
    int                  nTypes,
    const char          *szDefStr,
    ST_FUNCTION_DEF_TYP *pDef
);
static void ST_FreeFunctionDefinition(ST_FUNCTION_DEF_TYP* pDef);
static void ST_FreeAllOperatorDefinitions();

//static data:
static bool                 DefinitionsDone_il[CG_MOVE+1];
static ST_FUNCTION_DEF_TYP  DefinitionPrepared_il[CG_MOVE+1];
static bool                 DefinitionsDone_st[CG_MOVE+1];
static ST_FUNCTION_DEF_TYP  DefinitionPrepared_st[CG_MOVE+1];

//local helper class:
//holds the info from overload,inline statements (also for FBs !)
class ST_CMacCallDescr
{
    //interface description for overloaded functions:
    int           m_iArgCount;
    bool          m_bExtended;
    ST_TypeSet_C* m_plArgTypeSets;
    FC_CStr       m_macro;  //macro text
    //code description for CG binary representation:
    CG_MacroBody  m_MacroBody;

    //optional implicite cast:
    const CG_DType*  m_pRCast;
    int              m_nACast;
    const CG_DType** m_pACast;
public:    

    ST_CMacCallDescr(
        const ST_TypeSet_C&  optRet,
        const CG_DType*      pRCast,
        int                  nArgs,
        const ST_TypeSet_C*  pArgs,
        const CG_DType*const*pACast,//NULL or size is [nArgs]
        bool                 bExt,
        const CG_Edipos*     pEdp,
        const TCHAR*         pszMacro,
        const CG_Symtab*     pSymFun
    )
    {
        m_pRCast = pRCast;
        m_nACast = 0;
        m_pACast = NULL;

        if(nArgs>=0)
        {
            int i;
            m_iArgCount  = 1 + nArgs;
            m_plArgTypeSets = new ST_TypeSet_C[m_iArgCount];
            m_plArgTypeSets[0] = optRet;
            for(i=0; i<nArgs; i++)
            {
                m_plArgTypeSets[i+1] = pArgs[i];
                if(pACast && pACast[i])
                    m_nACast = i+1;
            }

            if(m_nACast>0)
            {
                m_pACast = new CG_DType const*[m_nACast];
                for(i=0; i<m_nACast; i++)
                    m_pACast[i] = pACast[i];
            }
        }
        else
        {
            assert(nArgs==-1);
            m_iArgCount = -1; //<-signals match any
            m_plArgTypeSets = NULL;
        }

        m_bExtended = bExt;
        m_macro.load(pszMacro);
        m_MacroBody.pSym   = pSymFun;
        m_MacroBody.edp    = *pEdp;
        m_MacroBody.pszMac = m_macro;
    }
    ST_CMacCallDescr(
        const CG_Edipos* pEdp,
        const TCHAR*     pszMacro,
        const CG_Symtab* pSymFun
    )
    {
        //Match Any Args if called this ctor:
        m_iArgCount = -1; //<-signals match any
        m_plArgTypeSets = NULL;
        m_bExtended = false;
        m_macro.load(pszMacro);
        m_MacroBody.pSym   = pSymFun;
        m_MacroBody.edp    = *pEdp;
        m_MacroBody.pszMac = m_macro;
        m_pRCast  = NULL;
        m_nACast  = 0;
        m_pACast  = NULL;
    }
    ~ST_CMacCallDescr()
    {
        delete []m_plArgTypeSets;
        delete m_pACast;
    }
    bool isExtended()const    {return m_bExtended;}
    int  getArgCount()const   {assert(m_iArgCount>=0); return m_iArgCount;}
    int  matchesAnyArgs()const{return m_iArgCount==-1;}
    const ST_TypeSet_C& getArgType(int i)const
    {
        assert(i>=0 && i<m_iArgCount && m_plArgTypeSets);
        return m_plArgTypeSets[i];
    }
    const CG_MacroBody* getMacro()const
    {
        return &m_MacroBody;
    }
    const CG_DType* getOptImplicitCast(int* pnACast, const CG_DType*const** ppACast)const
    {
        *pnACast = m_nACast;
        *ppACast = m_pACast;
        return m_pRCast;
    }
};


//empty lists:
static const CG_SymEntry* g_pzRefGlobConstEmpty[] = {NULL};
static const CG_Symtab*   g_pzRefSymsEmpty[] = {NULL};

ST_CSymtabPrv::ST_CSymtabPrv(
    CG_CIECFront*        pFront,   //must be given
    const CG_Edipos*     pEdp,     //must be given
    const CG_LIBContext* pLibCtx,  //!= if Lib defined
    const TCHAR*         pszName,  //must be given
    CG_POUTYP            pouTyp,   //can be undef if syntax error like FFUNCTION
    CG_SOURCETYPES       srcTyp,   //can be undef if syntax error like FFUNCTION
    __int64              ftLastWrite //can be NULL if error 
) 
    : m_hashTable(/*bIgnoreCase=*/true, /*bAllocKey=*/false)
{
    m_pFront = pFront;
    FC_NEW_ZSTRUCT(m_pMySym, CG_Symtab);

    if(pEdp) //may be null if error created symtab
        m_pMySym->edp = *pEdp;

    m_pMySym->srcTyp  = srcTyp;
    m_pMySym->pszId   = pFront->registerString(pszName);
    m_pMySym->ftLastWrite = ftLastWrite;
    m_pMySym->pouTyp  = pouTyp;
    m_pMySym->pLibCtx = pLibCtx;

    if(pLibCtx)
    {
        //set the lib if only bit if no target type in locator:
        int lTyp = CGT_SplitFileLocator(m_pMySym->edp.pszFile, NULL);
        assert(lTyp>1);
        if(lTyp==2)
            m_pMySym->flags |= CG_STAB_LIBIF_ONLY;
    }

    m_pFunIFDef = NULL;
    m_opEnum  = CG_EXPRTYP_UNDEF;
    CG_InitCMDLEdipos(&m_opEdp);
    m_numOfVarArg = 0;

    m_pRefSyms  = NULL;
    m_pzRefSyms = NULL;
    m_pRefGlobConst  = NULL;
    m_pzRefGlobConst = NULL;

    m_pConstRefs = NULL;
}
ST_CSymtabPrv::~ST_CSymtabPrv() 
{
    delete(m_pMySym);
    
    if(m_pFunIFDef)
    {
        ST_FreeFunctionDefinition(m_pFunIFDef);
        delete(m_pFunIFDef);
    }
    m_OverloadBodies.deletePtrs();
    m_hashTable.deletePtrs();

    if(m_pzRefSyms)
    {
        assert(!m_pRefSyms);
        delete(m_pzRefSyms);
    }
    else
    {
        delete(m_pRefSyms);
    }
    if(m_pzRefGlobConst)
    {
        assert(!m_pRefGlobConst);
        delete(m_pzRefGlobConst);
    }
    else
    {
        delete(m_pRefGlobConst);
    }

    delete(m_pConstRefs);
}

const CG_SymEntry* ST_CSymtabPrv::lookUpDeepAndInsertGVImp(
    const TCHAR*        pszId
)
{
    const CG_SymEntry*  pEntry;
    const CG_EdpSymtab* pImpGVL;

    pEntry = lookUpEntryDeep(pszId, &pImpGVL);
    if(pImpGVL)
    {
        assert(pEntry);
        //auto declare: from #import gvls:
        //register the decl as if declared as var_external:
        assert(pEntry->declScope == CG_VAR_GLOBAL || pEntry->declScope == CG_CONST);
        if (pEntry->declScope == CG_CONST)
            pEntry = registerDecl(
                pszId, 
                &pImpGVL->edp, 
                CG_CONST,
                pEntry->declMod,
                NULL,
                pEntry->pType,
                NULL,
				NULL, //forosai
                NULL, 
                pEntry->pInit
            );
        else
            pEntry = registerDecl(
                pszId, 
                &pImpGVL->edp, 
                CG_VAR_EXTERNAL,
                //remove the retain attribs but leave the constant:
                pEntry->declMod & ~(CG_DMOD_ANY_RETAIN), 
                NULL,
                pEntry->pType,
                NULL, 
				NULL, //forosai
                NULL, 
                NULL
            );
    }


    if(!pEntry)
        m_pFront->lookUpAutoDecl(pszId, this, &pEntry);

    //indirect errors should be added when the fbi was
    //declared or the gvl was imported, should assert here ?
    return pEntry;
}

const CG_SymEntry* ST_CSymtabPrv::lookUpEntryDeep(
    const TCHAR*         pszId,
    const CG_EdpSymtab** ppGVLImp //can be NULL if not needed
)const
{
    int                i;
    const CG_SymEntry* pEntry;

    if(ppGVLImp)
        *ppGVLImp = NULL;

    pEntry = m_hashTable.get(pszId);
    if(!pEntry)
    {
        //warning: dont't use m_pMySym->pGVLImp (is only set
        //after called endOfHeader!!) this function is also used 
        //internally when building this object.
        for(i=0; i<m_ImpGVLList.getUsed(); i++)
        {
            pEntry = m_pFront->lookUpEntry(
                m_ImpGVLList.getAddrOf(i)->pSym, pszId);
            if(pEntry)
            {
                if(ppGVLImp)
                    *ppGVLImp = m_ImpGVLList.getAddrOf(i);
                break;
            }
        }
    }

    return pEntry;
}


const CG_MacroBody* ST_CSymtabPrv::lookUpFunMacroBody
(
    const ST_TypeSet_C&   returnType, 
    int                   nArgs,
    const ST_PARAMSPEC*   pArgs,//##AC
    const CG_DType**      ppRCast,//out 
    int*                  pnACast,//out
    const CG_DType*const**ppACast //out
)const
{
    int                     jj;
    int                     i;
    int                     nform;
    int                     nBodies;  
    int                     iArg;
    ST_TypeSet_C            tmpt;
    const ST_CMacCallDescr* pFcd;

    *ppRCast = NULL;
    *pnACast = 0;
    *ppACast = NULL;

    //##AC

    //number of overload bodies in a specific fun overload
    nBodies = m_OverloadBodies.getUsed();
    assert(nBodies>0);

    if(!isPolyMorph())
    {
        assert(nBodies==1);//assert: must be already checked.
        pFcd = m_OverloadBodies.get(0);
        assert(pFcd->matchesAnyArgs());
        return pFcd->getMacro();
    }

    for(jj=0; jj<nBodies; jj++)
    {
        pFcd = m_OverloadBodies.get(jj);
        if(pFcd->matchesAnyArgs())
            break;
        
        i = 0;
        //check return type be carefull with fun return void!!
        //the typeset class has no 'void':
        tmpt = pFcd->getArgType(i);
        i++; //ret type is read ++.
        
        if(tmpt.IsEmptyType())
        {
            if(!returnType.IsEmptyType())
                continue;
        }
        else
        {
            assert(returnType.IsMonomorphType());
            tmpt &= returnType; 
            if(tmpt.IsEmptyType())
                continue;
        }
        
        //nform includes function return (void counts 1)
        nform = pFcd->getArgCount();
        assert(nform>=1);

        //check number of params:
        bool bArgCountOk;
        if(pFcd->isExtended())
        {
            bArgCountOk = (nArgs >= nform-1);
        }
        else
        {
            bArgCountOk = (nArgs == nform-1);
        }
        if(!bArgCountOk)
            continue;


        bool bTypeErr = false;
        for(iArg=0; iArg<nArgs; iArg++)
        {
            if(i>=nform)//if ... is used also don't check the argument types same as C
                break;

            tmpt = pFcd->getArgType(i);
            i++;        
            
            const ST_TypeSet_C& actTyp = pArgs[iArg].pActual->GetType();//##AC
            assert(actTyp.IsMonomorphType() && !tmpt.IsEmptyType());//##AC
            
            tmpt &= actTyp;//##AC
            if(tmpt.IsEmptyType())
            {
                bTypeErr = true;
                break;
            }
        }
        if(!bTypeErr)
            break;
    }

    if(jj<nBodies)
    {
        *ppRCast = pFcd->getOptImplicitCast(pnACast, ppACast);
        return pFcd->getMacro();
    }
    return NULL;
}
const CG_MacroBody* ST_CSymtabPrv::getNonPolyMacroBody()const
{
    //number of overload bodies in a specific fun overload
    const ST_CMacCallDescr* pFcd;
    int                     nBodies = m_OverloadBodies.getUsed();

    if(nBodies>0)
    {
        assert(nBodies==1);//assert: must be already checked.
        pFcd = m_OverloadBodies.get(0);
        assert(pFcd->matchesAnyArgs());
        return pFcd->getMacro();
    }
    return NULL;
}


void ST_CSymtabPrv::registerAttributes(
    CG_AttribList*  pAttr
)
{
    assert(!m_pMySym->pAttribs);
    m_pMySym->pAttribs = m_pFront->registerAttrList(pAttr);
    //##AC beg
    const CG_KeyValPair* pPair;
    pPair = m_pFront->lookUpAttrib(m_pMySym->pAttribs, CG_ATTR_FORCECONTEXT);
    if(pPair && !_tcsicmp(pPair->pszVal, _T("acyclictask")))
        m_pMySym->flags |= CG_STAB_FORCEACTASK;
    //##AC end
}

const CG_Symtab* ST_CSymtabPrv::registerGVLImp(
    const CG_Edipos*    pEdp,
    const TCHAR*        pszFile
)
{
    int                  i;
    const ST_CSymtabPrv* pGVL;
    CG_EdpSymtab         edpsym;
    const CG_SymEntry*   pEntry;
    const CG_SymEntry*   pPrior;
    bool                 bRecursive;
    const CG_EdpSymtab*  pPriorGVLImp;
    TCHAR                canrel[2*MAX_PATH];


    FC_StringCanonicalPath(pszFile, canrel, FC_ARRAY_LEN(canrel),
        m_pFront->m_4CP.getPrjPath(), FC_PATHM_KEEPREL);

    //allready imported?:
    for(i=0; i<m_ImpGVLList.getUsed(); i++)
    {
        if(!lstrcmpi(m_ImpGVLList.getAddrOf(i)->pSym->pszId, canrel))
        {
            m_pFront->m_pdm->msg1(CG_W_IMPORT_FILE_TWICE, pEdp, canrel);
            return NULL;
        }
    }
    
    //parse file:
    pGVL = m_pFront->loadGVL(canrel, &bRecursive);
    if(!pGVL)
    {
        if(bRecursive)
            m_pFront->m_pdm->msg1(CG_E_RECURSIV, pEdp, pszFile);
        else
            m_pFront->m_pdm->msg1(CG_E_FILE_NOT_EXIST, pEdp, pszFile);
        return NULL;
    }
    //else if((pGVL->getCGSym()->flags & CG_STAB_HDR_NOERR)==0)
    //the caller (=parse context) must add the indirect error !
    //contnue even if ind. error:


    //check that all ids are unique even the imported ones:
    for(pEntry=pGVL->getCGSym()->pFirst; pEntry; pEntry = pEntry->pNext)
    {
        //TODO think V1.3 has not as strict as this (deep look up here
        //was not done only when var was used) can be a problem for 
        //old code !!!
        pPrior = lookUpEntryDeep(pEntry->pszId, &pPriorGVLImp);
        if(pPrior)
        {
            //if the prior def was from an import use the import edp
            //otherwise the user will maybe not understand...
            //things get more complex if nested import should be allowed.
            m_pFront->m_pdm->msg2(CG_E_REDEF_VAR, pEdp, 
                pEntry->pszId, 
                m_pFront->m_pdm->edpToStr(
                  pPriorGVLImp ? &pPriorGVLImp->edp : &pPrior->edp, 
                  m_pFront->m_pdm->jot1())
               );
        }
    }

    edpsym.edp = *pEdp;
    edpsym.pSym= pGVL->getCGSym();
    m_ImpGVLList.add(edpsym);
    return edpsym.pSym;
}

void ST_CSymtabPrv::registerUsedType(
    const CG_Symtab* pUsed
)
{
    assert(CG_HasPOUTypBody(pUsed->pouTyp) || pUsed->pouTyp==CG_POU_SDT);
    assert(!m_pzRefSyms);
    if(!m_pRefSyms)
        m_pRefSyms = new FC_CHashPtr<const CG_Symtab*, int>(256);
    m_pRefSyms->insert(pUsed, 0);
}

void ST_CSymtabPrv::registerUsedGlobCONST(
    const CG_SymEntry* pGlobConstEntry
)
{
    assert(!m_pzRefGlobConst);
    if(!m_pRefGlobConst)
        m_pRefGlobConst = new FC_CHashPtr<const CG_SymEntry*, int>(256);
    m_pRefGlobConst->insert(pGlobConstEntry,0);
}

void ST_CSymtabPrv::registerCONSTRef(
    const CG_SymEntry* pConstEntry, 
    const CG_Edipos*   pEdpOfRef
)
{
    ST_CONST_REF* pCref;
    
    if(!m_pConstRefs)
        m_pConstRefs = new FC_CGArray<ST_CONST_REF>(256);

    pCref = m_pConstRefs->addZero();
 
    pCref->pConstEntry = pConstEntry;
    pCref->edpOfRef    = *pEdpOfRef;
}

void ST_CSymtabPrv::registerOpAlias(
    const CG_Edipos* pEdpOp,
    CG_EXPRTYP       mactyp
)
{
    assert(m_opEnum==CG_EXPRTYP_UNDEF && (m_pMySym->flags & CG_STAB_OPERATOR)==0);
    assert((mactyp>=CG_NOT && mactyp<=CG_MOVE));
    assert(mactyp!=CG_UMINUS && mactyp!=CG_NOOP);

    if(m_pMySym->pouTyp!=CG_POU_FUN)
    {
        m_pFront->m_pdm->msg0(CG_E_ILLEGAL_STMT_IN_POU_TYP, pEdpOp);
        return;
    }

    m_opEnum = mactyp;
    m_opEdp = *pEdpOp;
    m_pMySym->flags |= CG_STAB_OPERATOR;
}


void ST_CSymtabPrv::registerMacroBody(
    const CG_Edipos*     pEdp,
    bool                 bOverload,
    const ST_TypeSet_C&  optRet,
    const CG_DType*      pRCast, 
    int                  nArgs,
    const ST_TypeSet_C*  pArgs,
    const CG_DType*const*pACast,
    bool                 bExt,
    const CG_Edipos*     pEdpMac,
    const TCHAR*         pszMacro
)
{
    int        nBodies;
    CG_POUTYP  pt = m_pMySym->pouTyp;

    nBodies = m_OverloadBodies.getUsed();

    if(pt==CG_POU_FB &&  !m_pFront->m_features.m_pCFeat->bMacroFB 
		
		/* LN: Hack, I want to allowd inline FB's for Embedded libraries.
		 */
		&&  m_pFront->m_features.m_pCFeat->trgTyp != NULL && _tcsicmp(_T("4cpc"), m_pFront->m_features.m_pCFeat->trgTyp) == 0)
    {
        m_pFront->m_pdm->errNS1(pEdp, m_pFront->m_pdm->res1(CG_RES_STR_INLINE_FB));
        return;
    }

    if((pt!=CG_POU_FUN && pt!=CG_POU_FB) || (bOverload && pt!=CG_POU_FUN))
    {
        m_pFront->m_pdm->msg0(CG_E_ILLEGAL_STMT_IN_POU_TYP, pEdp);
        return;
    }
    if(bOverload && !isPolyMorph())
    {
        m_pFront->m_pdm->msg1(CG_E_OVERLOAD_FUN_NOT_POLY,
            pEdp, m_pMySym->pszId);
        return;
    }
    if(!bOverload && isPolyMorph())
    {
        m_pFront->m_pdm->msg1(CG_E_POLYFUN_NEED_OVERLOAD,
            pEdp, m_pMySym->pszId);
        return;
    }
    if(!bOverload && nBodies>0)
    {
        m_pFront->m_pdm->msg0(CG_E_MACRBODY_TWICE, pEdp);
        return;
    }

    //all ok:
    if(bOverload)
    {
        m_OverloadBodies.add(new ST_CMacCallDescr(
            optRet, pRCast, nArgs, pArgs, pACast, bExt, pEdpMac, pszMacro, m_pMySym)
          ); 
    }
    else
    {
        m_OverloadBodies.add(new ST_CMacCallDescr(
            pEdpMac, pszMacro, m_pMySym)
          ); 
    }
}

void ST_CSymtabPrv::registerVarNative(
    const CG_Edipos* pEdp,
    const TCHAR*     pszText
)
{
    CG_EdpStr     edpstr;

    //no struct gvl cst or the like, if there is a body
    //native is ok. //TODO: V1.3: how are inline functions ?
    if(!CG_HasPOUTypBody(m_pMySym->pouTyp))
    {
        m_pFront->m_pdm->msg0(CG_E_ILLEGAL_USE_OF_NATIVE, pEdp);
        return;
    }
    
    edpstr.edp = *pEdp;
    edpstr.psz = m_pFront->registerString(pszText);
    m_VarNative.add(edpstr);
}

void ST_CSymtabPrv::registerXREFFile(
    const TCHAR* pszXREFFile
)
{
    assert(!m_pMySym->pszXREFFile);
    m_pMySym->pszXREFFile = m_pFront->registerString(pszXREFFile);
}


const CG_SymEntry* ST_CSymtabPrv::registerDecl(
    const TCHAR*          pszId, 
    const CG_Edipos*      pEdpId, 
    CG_DECLSCOPE          declScope,
    WORD                  declMod,
    const CG_AttribList*  pAttrStruct,
    const CG_DType*       pDType,
    const TCHAR*          pszAddr, 
	const TCHAR*          pszSysAddr, //forosai
    const CG_Edipos*      pEdpAddr, 
    const CG_Initialiser* pInit
)
{
    CG_SymEntry*     pNew;
    CG_BITSET        bitSet;
    const FC_CHashStr<CG_SymEntry*>::Pair* pLastDecl;

    assert(pszId && pEdpId && pDType);
    assert(!pszAddr || pEdpAddr);


    FC_NEW_ZSTRUCT(pNew, CG_SymEntry);

    pNew->pszId = m_pFront->registerString(pszId);
    pNew->edp   = *pEdpId;
    pNew->pType = pDType;
    if(pszAddr)
    {
       pNew->pszAddr = m_pFront->registerString(pszAddr);
	   if(pszSysAddr) //forosai
	   {
	       pNew->pszSysAddr = m_pFront->registerString(pszSysAddr);
		   declMod |= CG_DMOD_SYS_ADDR;
	   }

       pNew->edpAddr = *pEdpAddr;
    }
    pNew->declMod    = declMod;
    pNew->declScope  = declScope;
    pNew->pAttribs   = pAttrStruct;
    pNew->pInit      = pInit;
    pNew->pParent    = m_pMySym;

    //link into list:
    if(!m_pMySym->pFirst)
        m_pMySym->pFirst = pNew;
    
    pLastDecl = m_hashTable.lastPair();
    if(pLastDecl)
    {
        assert(!pLastDecl->pVal->pNext);
        pLastDecl->pVal->pNext = pNew;
    }

    if(!m_hashTable.insert(pNew->pszId, pNew))
    {
        CGT_IERR2(m_pFront->m_pdm, pEdpId, _T("ST_CSymtabPrv::registerDecl failed: duplicate entry"));
        delete(pNew);
        return false;
    }

    
    
    //update various  symtab members:
    if(pszAddr)
    {
        m_pMySym->flags |= CG_STAB_HAS_ADDR;

        if(CGT_IsIncompleteAddressRef(pszAddr))
            m_pMySym->flags |= CG_STAB_HAS_INCADDR;
    }

    //update the used type list:
    if(pDType->pSym)
    {
        m_pMySym->flags |= CG_STAB_HAS_INST;
        registerUsedType(pDType->pSym);
    }

    //inherit the deep copy err|warn attribute from every entry:
    if(pDType->pSym && (pDType->pSym->flags & CG_STAB_DIAG_ON_COPY))
    {
        m_pMySym->flags |= CG_STAB_DIAG_ON_COPY;
    }


    //get the obj numbers updated:
    pNew->lNum = m_pMySym->nEntries;
    m_pMySym->nEntries++;
    switch(declScope)
    {
    case CG_VAR:
        pNew->lInstNum = m_pMySym->nInput + m_pMySym->nOutput + m_pMySym->nVar;
		m_pMySym->nVar++;
        pNew->lParamNum = -1;
    break;
    case CG_VAR_INPUT:
        pNew->lInstNum = m_pMySym->nInput + m_pMySym->nOutput + m_pMySym->nVar;
        m_pMySym->nInput++;
        pNew->lParamNum = m_pMySym->nParam;
        m_pMySym->nParam++;
    break;
    case CG_VAR_IN_EXT:
        pNew->lInstNum = -1;
        pNew->lParamNum = m_pMySym->nParam;
        m_pMySym->nParam++;
        // ++**++ JD extInParam -->
        //assert(!m_pMySym->pVarInExt);
        if (m_pMySym->pVarInExt==NULL)
        {
            m_pMySym->pVarInExt = pNew;     
        }
        // ++**++ JD extInParam <--
    break;
    case CG_VAR_OUTPUT:
        pNew->lInstNum = m_pMySym->nInput + m_pMySym->nOutput + m_pMySym->nVar;
        m_pMySym->nOutput++;
        pNew->lParamNum = m_pMySym->nParam;
        m_pMySym->nParam++;
    break; 
    case CG_VAR_IN_OUT:
        pNew->lInstNum = m_pMySym->nInOut;
        m_pMySym->nInOut++;
        pNew->lParamNum = m_pMySym->nParam;
        m_pMySym->nParam++; 
    break;
    case CG_VAR_GLOBAL:
        pNew->lInstNum = -1;
        m_pMySym->nGlobals++;
        pNew->lParamNum= -1;
    break; 
    case CG_VAR_EXTERNAL:
        pNew->lInstNum = -1;
        m_pMySym->nExternal++;
        pNew->lParamNum= -1;
    break;
    case CG_CONST:
        pNew->lInstNum = -1;
        m_pMySym->nConst++;
        pNew->lParamNum= -1;
    break;
    case CG_VAR_TEMP:
        pNew->lInstNum = m_pMySym->nTmp;
        m_pMySym->nTmp++;
        pNew->lParamNum= -1;
    break;
    case CG_DECLSCOPE_UNDEF:
    default:
        assert(!"bad enum");
    }


    //function specials:
    if(declScope==CG_VAR_IN_EXT)
    {
        //assert: that pou typ is fun, must have been checked
        assert(m_pMySym->pouTyp==CG_POU_FUN);
        m_numOfVarArg = m_pMySym->nParam;    
    }

    if(!CG_IsMonomorph(pDType->type))
    {
        bitSet = pDType->tbs;

        //update the differentPolyTypes field:
        if(m_pMySym->pouTyp==CG_POU_FUN)
        {
            if(m_polyTypes.findElem(bitSet)<0)
                m_polyTypes.add(bitSet);
        }
        else
        {
            assert(m_pMySym->pouTyp==CG_POU_FB && declScope==CG_VAR_IN_OUT);
        }
    }

    return pNew;
}


bool ST_CSymtabPrv::endOfHeader(
    CG_SOURCETYPES   srcTyp,
    int              nErrorCurr, //error count of file so far
    const CG_Edipos* pEdpEndHdr
)
{
    assert(!m_pFunIFDef);
    assert(!(m_pMySym->flags & CG_STAB_HDR_NOERR));


    //functions need some after special care:
    
    //assert: code in next if needs m_pMySym->pFirst, check error 
    //condition, then don't enter this code, m_pFunIFDef member
    //must not be used if hdr is in error anyway...
    assert(m_pMySym->pouTyp!=CG_POU_FUN || m_pMySym->nParam>0 || nErrorCurr>0);
    if(m_pMySym->pouTyp==CG_POU_FUN && m_pMySym->nParam>0)
    { //check some special restrictions:
        int count    = 1;
        int firstIn  = 0;
        int lastIn   = 0;
        int firstOut = 0;
        int lastOut  = 0;
        int inExt    = 0;
        int last;
        const CG_SymEntry*    pEntry;

        pEntry = m_pMySym->pFirst;
        // first is return value or void
        assert(pEntry);
        pEntry=pEntry->pNext;

        for(;pEntry;pEntry=pEntry->pNext)
        {
            switch(pEntry->declScope)
            {
            case CG_VAR_INPUT:
            case CG_VAR_IN_EXT:
                if(firstIn==0)
                    firstIn = count;
                lastIn = count;

                if(pEntry->declScope == CG_VAR_IN_EXT)
                {
                    // ++**++ JD extInParam -->
                    /*if(inExt>0)
                    {
                        m_pFront->m_pdm->msg1(CG_E_VAR_IN_EXT_NOT_LAST_IN, 
                            &pEntry->edp, pEntry->pszId);
                        nErrorCurr++;
                    }*/
                    inExt = count;
                    // ++**++ JD extInParam <--
                }
            break;
            case CG_VAR_OUTPUT:

                if(srcTyp!=CG_SRC_TYPE_HDR)
                {
                    //check for not supported fun var_output of complex type if ST/FLFBD body:
                    //use m_OverloadBodies.getUsed()==0 to test if inline{<native>} 
                    if(m_OverloadBodies.getUsed()==0 && CG_IsMultiElemType(pEntry->pType))
                    {
                        m_pFront->m_pdm->msg1(CG_E_NOT_SUPPORTED, &pEntry->edp, 
                            m_pFront->m_pdm->res1(CG_RES_NS_COMPL_TYP_FUN_OUT));
                        nErrorCurr++;
                    }
                }
                //else: interface only, don't care...

            //fall th.
            case CG_VAR_IN_OUT:
                if(firstOut==0)
                    firstOut = count;
                lastOut = count;
            break;
            default:
                continue;//skip local vars
            }
            count++;
        }

        //##AC if ((firstIn<firstOut && lastIn>firstOut)
        //##AC    ||(firstOut<firstIn && lastOut>firstIn))
        //##AC {
        //##AC      m_pFront->m_pdm->msg1(CG_E_NOT_SUPPORTED, &m_pMySym->edp,
        //##AC         m_pFront->m_pdm->res1(CG_RES_STR_MIX_INOUT));
        //##AC     nErrorCurr++;
        //##AC }

        if (lastIn>lastOut)
            last = lastIn;
        else
            last = lastOut;

        if (inExt>0 && inExt!=last)
        {
            m_pFront->m_pdm->msg1(CG_E_NOT_SUPPORTED, &m_pMySym->edp,
                m_pFront->m_pdm->res1(CG_RES_STR_MIX_INOUT_EXT));
            nErrorCurr++;
        }
        //TODO document this thing and use resource strings for diag.

        if(nErrorCurr==0)
        {
            int                     nArgs;
            int                     nFirstOutPos=0;
            int                     nLastOutPos=0;
            int                     nFirstInPos=0;
            int                     nArgTypes;
            const CG_SymEntry*      pEntry;
            const CG_SymEntry*      pFunctionResultEntry;
            ST_FUNCTION_DEF_TYP*    pDef;

            nArgTypes = m_pMySym->nParam;

            pDef = new ST_FUNCTION_DEF_TYP;
            pDef->pszId      = m_pMySym->pszId;
            pDef->nOverloads = 1;
            pDef->nArgTypes  = nArgTypes;
            pDef->lpArgs     = NULL;
            pDef->lpTypes    = NULL;     
    
            pDef->lpArgs = new int[4*pDef->nOverloads+pDef->nArgTypes];
    
            pDef->lpVarArg   = pDef->lpArgs + 1*pDef->nOverloads;
            pDef->lpVarOuts  = pDef->lpArgs + 2*pDef->nOverloads;
            pDef->lpOffset   = pDef->lpArgs + 3*pDef->nOverloads;
            pDef->lpArgTypes = pDef->lpArgs + 4*pDef->nOverloads;
    
            pDef->nTypes = nArgTypes;

            pDef->lpTypes = new ST_TypeSet_C[nArgTypes];

            pDef->lpArgs[0] = nArgTypes-1;
            pDef->lpVarArg[0] = 0;
            pDef->lpVarOuts[0] = 0;
            pDef->lpOffset[0] = 0;


            for(nArgs=0, pEntry=m_pMySym->pFirst; pEntry; pEntry=pEntry->pNext) 
            {
                switch(pEntry->declScope)
                {
                case CG_VAR_INPUT:
                case CG_VAR_IN_EXT:
                case CG_VAR_OUTPUT:
                case CG_VAR_IN_OUT:
                    pDef->lpTypes[nArgs++].initFromCG_DType(pEntry->pType);
                }
            }


            //the first entry if named like the pou holds the function return,
            //note that there are also functions with no return value
            pFunctionResultEntry = m_pMySym->pFirst;
            for(nArgs=1, pEntry=m_pMySym->pFirst;pEntry;pEntry=pEntry->pNext) 
            {
                if(pEntry->declScope == CG_VAR_IN_EXT && pEntry->pParent->pVarInExt == pEntry)  // ++**++ JD extInParam
                    pDef->lpVarArg[0] = nArgs;
        
                switch(pEntry->declScope)
                {
                case CG_VAR_INPUT:
                case CG_VAR_IN_EXT:
                    if(pEntry!=pFunctionResultEntry)
                    {
                        if(nFirstInPos==0)
                            nFirstInPos = nArgs;
                    }
                break;
                case CG_VAR_IN_OUT:
                case CG_VAR_OUTPUT:
                    if(pEntry!=pFunctionResultEntry)
                    {
                        if(nFirstOutPos==0)
                            nFirstOutPos = nArgs;
                
                        nLastOutPos = nArgs;
                    }
                break;
                default:
                    continue;//skip local vars
                }

                //TODO does not work if ARRAY of ANY
                if(pEntry==pFunctionResultEntry)
                {
                    pDef->lpArgTypes[0] = 0;
                }
                else if(CG_IsMonomorph(pEntry->pType->type))//fixes the string[len] problem
                {                                           //note that tmptype==pDef->lpTypes[j]
                                                            //below is true string[len]==string !!
                    pDef->lpArgTypes[nArgs] = nArgs;
                    nArgs++;
                }
                else
                {
                    for(int j=0;j<nArgTypes;j++) //IF poly type find fisrt of that is equal
                    {
                        ST_TypeSet_C tmptype;
            
                        tmptype.initFromCG_DType(pEntry->pType);
                        if(tmptype==pDef->lpTypes[j])
                        {
                            pDef->lpArgTypes[nArgs++] = j;
                            break;
                        }
                    }
                    assert(j<nArgTypes);
                }

            }//end for

            assert(m_pMySym->nParam==nArgs);
#if 0 //##AC
            if(nFirstOutPos>0)
            {
                //check that either a) fun(out,out,out,in,in,in) or b) fun(in,in,in,out,out,out)
                //but not fun(in,out,in,out,in,out). The mixed case cannot be handled yet.
                if(nFirstOutPos==1 && (nFirstInPos==0 || nFirstInPos==nLastOutPos+1))
                {
                    //case a) or no inputs at all:
                    //positive lpVarOuts[0] counts from the feft
                    pDef->lpVarOuts[0] = nLastOutPos; 
                }
                else if(nLastOutPos+1==nArgs && nFirstInPos<nFirstOutPos)
                {
                    //case b)
                    //negative lpVarOuts[0] counts from the right
                    pDef->lpVarOuts[0] = -(nLastOutPos-nFirstOutPos+1); 
                }
                else
                {
                    m_pFront->m_pdm->msg1(CG_E_NOT_SUPPORTED, &m_pMySym->edp,
                        m_pFront->m_pdm->res1(CG_RES_STR_MIX_INOUT));
                    nErrorCurr++;
                }
            }
#endif #if 0 //##AC
            m_pFunIFDef = pDef;
        }//if(nErrorCurr==0)
    }//end of function special code





    //final actions:
    m_pMySym->edpEndHdr = *pEdpEndHdr;

    if(isPolyMorph())
        m_pMySym->flags |= CG_STAB_POLYMORH;

    if(m_OverloadBodies.getUsed()>0)
    {
        assert(!(m_pMySym->flags & CG_STAB_MACRO));
        m_pMySym->flags |= CG_STAB_MACRO;

        if(m_pMySym->pouTyp==CG_POU_FUN && m_pMySym->nVar>0)
        {
             m_pFront->m_pdm->msg1(CG_E_NOT_SUPPORTED, 
                 &m_OverloadBodies.get(0)->getMacro()->edp,
                 m_pFront->m_pdm->res1(CG_RES_STR_INLINE_LOC_VAR));
             nErrorCurr++;
        }
    }
    
    if(m_pMySym->pLibCtx && (m_pMySym->flags & CG_STAB_LIBIF_ONLY)==0)
    {
        //mainly to get the called functions:
        if(!createInfoFromLinkFile())
            nErrorCurr++;
    }


    if(m_pFront->m_myCOMIF->lookUpAttrib(m_pMySym->pAttribs, CG_ATTR_DEEPCOPY))
        m_pMySym->flags |= CG_STAB_DIAG_ON_COPY;//TODO put this code into registerAttributes


    //set the has body flag if a body is expected to exist:
    assert((m_pMySym->flags & CG_STAB_HAS_BDY)==0);
    if(CG_HasPOUTypBody(m_pMySym->pouTyp) && srcTyp!=CG_SRC_TYPE_HDR)
    {
        if((m_pMySym->flags & (CG_STAB_MACRO|CG_STAB_OPERATOR))==0)
            m_pMySym->flags |= CG_STAB_HAS_BDY; 
    }

    //set the used sym and used glob cst member now if nobody 
    //if has body do it later when endOfBody is called.
    //Also set the mebers if hdr has errors, just to give the info
    //available up to the error.
    assert(!m_pzRefSyms && !m_pMySym->pzRefSyms);
    assert(!m_pzRefGlobConst && !m_pMySym->pzRefGlbCst);
    if((m_pMySym->flags & CG_STAB_HAS_BDY)==0)
    {
        m_pMySym->pzRefGlbCst = g_pzRefGlobConstEmpty;
        if(m_pRefGlobConst)
        {
            m_pzRefGlobConst = m_pRefGlobConst->allocKeyArray(NULL);
            m_pMySym->pzRefGlbCst = m_pzRefGlobConst;
            delete(m_pRefGlobConst);
            m_pRefGlobConst = NULL;
        }

        m_pMySym->pzRefSyms = g_pzRefSymsEmpty;
        if(m_pRefSyms)
        {
            m_pzRefSyms = m_pRefSyms->allocKeyArray(NULL);
            m_pMySym->pzRefSyms = m_pzRefSyms;
            delete(m_pRefSyms);
            m_pRefSyms = NULL;
        }
    }


    //set the imp gvl list:
    assert(m_pMySym->nGVLImp==0 && !m_pMySym->pGVLImp);
    if(m_ImpGVLList.getUsed()>0)
    {
        m_ImpGVLList.sizeToFit();
        m_pMySym->nGVLImp = m_ImpGVLList.getUsed();
        m_pMySym->pGVLImp = m_ImpGVLList.getElements();
    }
    
    //set var native blocks:
    if(m_VarNative.getUsed())
    {
        m_VarNative.sizeToFit();
        m_pMySym->nVarNative = m_VarNative.getUsed();
        m_pMySym->pVarNative = m_VarNative.getElements();
    }

    assert(m_pMySym->ftLastWrite!=0);
    assert((m_pMySym->flags & CG_STAB_HDR_NOERR)==0);
    
    //finally set the noerr flag if no err:
    if(nErrorCurr==0)
       m_pMySym->flags |= CG_STAB_HDR_NOERR;

    return nErrorCurr==0;
}


bool ST_CSymtabPrv::createInfoFromLinkFile()
{
    //fill the info from the link file struct:
    //call only once diretly after new this.
    int                            i;
    bool                           bLinkErr;
    const TCHAR*                   psz;
    const CG_Symtab*               pSymFun;
    CGT_CSVFile                    csvr;
    CGT_STDLINK_INFO               lnkf;
    CG_Edipos                      edp;
    FC_CString                     jot(MAX_PATH);
    FC_CString                     errDescr;


    //precompiled lib POU,must have .link file, 
    //but do this only if there is a target type defined:
    //get and read the .link file:
    
    //build locator for link file:
    jot.load(m_pMySym->edp.pszFile).stripFilePart();

    //EF HACK ++ for sfc pous in lib for pc based: the .link file does not contain the info if
	//the POU ia a SFC or not, as a workaround test if a 4grv file exists
	//this workaround works only for PC based target, but at the moment all other targets store
	//in a lib the source code anyway and no precompiled code.
	//A realk solution would be to update the link file format (by the way the CG_STAB_BDY_NATIVE
	//bit has the same problem!)
	FC_CString sfc4GRV(MAX_PATH);
	sfc4GRV.load(jot)<<'\\'<<m_pMySym->pszId<<_T(".4grv");
    if(CGT_ExistsLocatorFile(m_pFront->m_pdm, m_pFront->m_myCOMIF, sfc4GRV, NULL)>0)
        m_pMySym->flags |= CG_STAB_BDY_IS_SFC;
    //EF HACK--
	
	jot<<_T("\\")CG_LINK_FILE;


    
    if(!CGT_LoadCSVLocatorFile(m_pFront->m_pdm, &csvr, m_pFront->m_myCOMIF, jot))
    {
        CG_InitSTEdipos(&edp, jot);
        CGT_IERR2(m_pFront->m_pdm, &edp, _T("cannot get .link form library"));
        return false;
    }
    if(!CGT_ReadLinkFile(&csvr, &lnkf, &errDescr))
    {
        CG_InitSTEdipos(&edp, jot);
        CGT_IERR2(m_pFront->m_pdm, &edp, errDescr.prepend(_T("error in .link file, reason:")));
        return false;
    }



    bLinkErr = false;
    for(i=0; i<lnkf.links.getUsed(); i++)
    {
        psz = lnkf.links.get(i);

        if(!FC_StringIncBegin(&psz, _T("FUN#")))
            continue;

        jot.clear().appendUpTo(psz, _T("("));
        
        //don't use m_pFront->loadHeaderEx, in this case the error messages must be
        //very special since there is no user visible body:
        pSymFun = m_pFront->m_myCOMIF->loadHeader(jot);
        if(pSymFun)
        {
            //test if lib POU supports a possible trg type:
            if((pSymFun->flags & CG_STAB_LIBIF_ONLY) && m_pFront->m_features.getTrgTyp())
            {
                bLinkErr = true;
                assert(pSymFun->pLibCtx);
                m_pFront->m_pdm->errNS3(&pSymFun->edp, 
                    m_pFront->m_pdm->jot1()<<_T("'")<<pSymFun->pszId<<_T("'"),
                    m_pFront->m_pdm->res1(CG_RES_STR_LIB), 
                    m_pFront->m_pdm->jot2()<<pSymFun->pLibCtx->pszId);
                
                errDescr.load(m_pFront->m_pdm->res1(CG_RES_STR_PRECOMPILED))
                    <<_T("'")<<m_pMySym->pszId<<_T("'")
                    <<m_pFront->m_pdm->res2(CG_RES_STR_LIB)
                    <<m_pMySym->pLibCtx->pszId<<_T("\n");
                m_pFront->m_pdm->userMsg(errDescr);
            }
            
            if((pSymFun->flags & CG_STAB_HDR_NOERR)==0)
            {
                m_pFront->m_pdm->addIndirectErr();
                bLinkErr = true;
            }

            //TODO: check mangeled name against header
            registerUsedType(pSymFun);
        }
        else
        {
            bLinkErr = true;
            m_pFront->m_pdm->msg1(CG_E_4CL_FUN_REF, &m_pMySym->edp, jot);
        }
    }
    return !bLinkErr;
}

bool ST_CSymtabPrv::endOfBody(
    bool     bHasError,
    long     lLangBits
)
{
    assert(m_pMySym->flags & CG_STAB_HDR_NOERR || bHasError);
    assert(m_pMySym->ulHdrBytes>0 || bHasError);
    assert(m_pMySym->flags & CG_STAB_HAS_BDY);

    long lAllLang = CG_STAB_BDY_IS_ST |CG_STAB_BDY_IS_LD|
                    CG_STAB_BDY_IS_FBD|CG_STAB_BDY_IS_IL|
                    CG_STAB_BDY_IS_SFC;
    
    assert((lLangBits & ~lAllLang)==0);

    //set even if error:
    m_pMySym->flags |= (lLangBits & lAllLang);

    assert((m_pMySym->flags & CG_STAB_BDY_LOADED)==0);
    m_pMySym->flags |= CG_STAB_BDY_LOADED; 

    //set the used sym and used glob cst member now if body is loaded:
    //Note: if hrd was in error the members have already been set with 
    //at least the header used info, but then this function should not
    //be called because the body isn't read in hdr has errors. 
    assert(!m_pzRefSyms && !m_pMySym->pzRefSyms);
    assert(!m_pzRefGlobConst && !m_pMySym->pzRefGlbCst);

    m_pMySym->pzRefGlbCst = g_pzRefGlobConstEmpty;
    if(m_pRefGlobConst)
    {
        m_pzRefGlobConst = m_pRefGlobConst->allocKeyArray(NULL);
        m_pMySym->pzRefGlbCst = m_pzRefGlobConst;
        delete(m_pRefGlobConst);
        m_pRefGlobConst = NULL;
    }

    m_pMySym->pzRefSyms = g_pzRefSymsEmpty;
    if(m_pRefSyms)
    {
        m_pzRefSyms = m_pRefSyms->allocKeyArray(NULL);
        m_pMySym->pzRefSyms = m_pzRefSyms;
        delete(m_pRefSyms);
        m_pRefSyms = NULL;
    }

    assert(bHasError==0 || m_pFront->m_pdm->getDisplayedErrors()>0);
    if(bHasError==0)
        m_pMySym->flags |= CG_STAB_BDY_NOERR;//only if no error

    return true;
}







CG_CIECFront::CG_CIECFront(CGT_CDiagMsg* pdm) :
    m_pHashGlobalStrings (/*initalSize=*/1024)
   ,m_symtabs            (/*bIgnoreCase=*/true, /*bAllocKey=*/false, /*initalSize=*/0)
   ,m_resources          (/*bIgnoreCase=*/true, /*bAllocKey=*/false, /*initalSize=*/0)
   ,m_hashDType          (/*bIgnoreCase=*/false,/*bAllocKey=*/false, /*initalSize=*/0)
   ,m_hashOnParsingFiles (/*bIgnoreCase=*/true, /*bAllocKey=*/true, /*initalSize=*/0)
   ,m_pdm(pdm)
{

    m_myCOMIF = NULL;
    m_pILibOpen = NULL;
    m_pBuilnInList = NULL;

    m_bOPT_DisableXref = false; //TODO: xref files must always be dumped: needed 
                              //for console code gen??
	m_bFileCheckMode = false;
}


CG_CIECFront::~CG_CIECFront()
{ 
    int i;
#ifdef DEBUG
    debugDumpState(_T("4CG.out"));
#endif
    m_resources.deletePtrs();
    m_symtabs.deletePtrs();
    m_listAttribLists.deletePtrs();
    m_listInits.deletePtrs();
    m_listInitArray.deletePtrs();
    m_listLongArray.deletePtrs();
    m_listSymtabLists.deletePtrs();
    m_listEntryArray.deletePtrs();
    m_taskInitsList.deletePtrs();
    m_tasksList.deletePtrs();
    m_hashDType.deletePtrs();
    
    ST_FreeAllOperatorDefinitions();
    
    for(i = 0; i<m_openLibs.getUsed(); i++)
    {
        ST_LIB_TYP lt = m_openLibs.get(i);
        m_pILibOpen->Close(lt.pILib);
        lt.pILib->Release();
    }

    FC_RELEASE_PTR(m_pILibOpen);
    
    if(m_pBuilnInList)
    {
        const FC_CHashStr<ST_FUNCTION_DEF_TYP*>::Pair* pp;
        for(pp=m_pBuilnInList->firstPair(); pp; pp=pp->pNext)
        {
            ST_FreeFunctionDefinition(pp->pVal);
            delete(pp->pVal);
        }

        delete(m_pBuilnInList);
    }
}

void CG_CIECFront::debugDumpState(const TCHAR* pszFile)
{
}



const TCHAR* CG_CIECFront::registerString(const TCHAR* psz)
{
    return m_pHashGlobalStrings.registerString(psz);
}

const CG_DType* CG_CIECFront::registerDType(
    CG_DTYPES dType, 
    long      lStrLen //-1 if no string or auto size string
                      // >=0 otherwise
)
{
    bool               bOk;
    const CG_DType*    pDType;
    CG_DType*          pDTypeNew;
    const TCHAR*       pszStr;
    TCHAR              buffer[256];

    assert(dType!=CG_DTYPES_UNDEF);
    pszStr = ST_CG_DTYPES_ToString(dType);

    if(lStrLen>=0)
    {
        assert(dType==CG_STRING || dType==CG_WSTRING || 
               dType==CG_QVT_STRING || dType==CG_QVT_WSTRING);
        wsprintf(buffer, _T("%s[%d]"), pszStr, lStrLen);
        pszStr = buffer;
    }
    else
    {
        //strlen should be always -1 if not specified:
        assert(lStrLen==-1);
        lStrLen = -1;
    }

    pDType = m_hashDType.get(pszStr);

    if(!pDType)
    {    
        FC_NEW_ZSTRUCT(pDTypeNew, CG_DType);
        pDTypeNew->type    = dType;
        pDTypeNew->tbs     = ST_DTypesToTBS(dType);
        pDTypeNew->lStrLen = lStrLen;
        pDTypeNew->pszName = registerString(pszStr);

        bOk = m_hashDType.insert(pDTypeNew->pszName, pDTypeNew);
        assert(bOk);
        pDType = pDTypeNew;
    }
    return pDType;
}
const CG_DType* CG_CIECFront::registerDType(const CG_Symtab* pSym)
{
    bool               bOk;
    const CG_DType*    pDType;
    CG_DType*          pDTypeNew;

    assert(pSym->flags & CG_STAB_HDR_NOERR);

    pDType = m_hashDType.get(pSym->pszId);
    if(!pDType)
    {    
        FC_NEW_ZSTRUCT(pDTypeNew, CG_DType);
        pDTypeNew->pSym    = pSym;
        pDTypeNew->lStrLen = -1;
        switch(pSym->pouTyp)
        {
        case CG_POU_FB:    
            pDTypeNew->type = CG_INSTANCE;
            pDTypeNew->tbs  = CG_TBS_INSTANCE;
        break;
        case CG_POU_SDT:   
            pDTypeNew->type = CG_STRUCT;   
            pDTypeNew->tbs  = CG_TBS_STRUCT;
        break; 
        case CG_POU_PRG:   
            pDTypeNew->type = CG_PROGRAM;  
            pDTypeNew->tbs  = CG_TBS_PROGRAM;
        break;
        default:
            CGT_IERR(m_pdm, _T("invalid pou typ"));
        }
        pDTypeNew->pszName = pSym->pszId;

        bOk = m_hashDType.insert(pDTypeNew->pszName, pDTypeNew);
        assert(bOk);
        pDType = pDTypeNew;
    }
    return pDType;
}

//##AC beg
const CG_DType* CG_CIECFront::registerDType(CG_BITSET lTypeSet, const TCHAR* pszTypeName, int lStrLen)
{
    const CG_DType* pDType;
    CG_DType*       pDTypeNew;
    bool            bRec;
    TCHAR           szBuff[MAX_TYPE_DUMP_BUFF];
    TCHAR           szBuff2[MAX_TYPE_DUMP_BUFF];
    int             nDisplayOff = 0;
    const TCHAR*    pszRegStr;


    if(pszTypeName)
    {
        strcpy(szBuff, pszTypeName);
        if(lTypeSet==CG_TBS_STRUCT||lTypeSet==CG_TBS_INSTANCE||lTypeSet==CG_TBS_PROGRAM||lTypeSet==CG_TBS_ENUM)
        {
            strcpy(szBuff, pszTypeName);
        }
        else if(lTypeSet==CG_TBS_ALLBITS)
        {
            strcpy(szBuff, "typdef:");
            strcat(szBuff, pszTypeName);
            pszTypeName = NULL;
        }
        else
        {
            assert(!"?");
        }
    }
    else if(lStrLen != -1)
    {
       sprintf(szBuff, "%s[%d]", ST_TypeToString(szBuff2, lTypeSet), lStrLen);
    }
    else
    {
        ST_TypeToString(szBuff, lTypeSet);
    }


    pDType = m_hashDType.get(szBuff);
    if(!pDType)
    {    
        FC_NEW_ZSTRUCT(pDTypeNew, CG_DType);
        pDTypeNew->type     = CGT_TBSToDTypes(lTypeSet);
        pDTypeNew->tbs      = lTypeSet;
        if(pszTypeName)
        {
            ST_CSymtabPrv* pSymPrv = loadHeader(pszTypeName, &bRec);
            if(pSymPrv)
                pDTypeNew->pSym = pSymPrv->getCGSym();
            else
                CGT_IERR(m_pdm, _T("CG_CIECFront: registerDType(): loadHeader failed"));
        }
        pDTypeNew->lStrLen  = lStrLen; 
        //pDTypeNew->bIsArray = false;
        //pDTypeNew->nElems   = 0; 
        //pDTypeNew->nDim     = 0;
        pszRegStr = registerString(szBuff);
        pDTypeNew->pszName = pszRegStr + nDisplayOff;

        bool bOk = m_hashDType.insert(pszRegStr, pDTypeNew);
        assert(bOk);
        pDType = pDTypeNew;
    }

    return pDType;
}
//##AC end
const CG_DType* CG_CIECFront::registerDType(
    int               nDims,      //=0: if assumed size array
                                  //>0: otherwise
    const CG_Subrange ranges[],   //NULL or array ranges
    CG_DTYPES         dType,      //array base type 
    long              lStrLen,    //-1 : auto size string or no string
                                  //>=0: fixed size string
    const CG_Symtab*  pSym        //NULL or struct (maybe FB too in future)
)
{
    bool            bOk;
    long            i;
    long            arrayLenTot;
    const CG_DType* pDType;
    CG_DType*       pDTypeNew;
    FC_CString      typeNameTmp(MAX_PATH);


    assert(!pSym || (pSym->flags & CG_STAB_HDR_NOERR));
    assert(nDims>=0 && nDims<=CG_MAX_ARRAY_DIM);
    assert((dType!=CG_DTYPES_UNDEF && dType!=CG_VOID));
    assert(dType!=CG_STRUCT || pSym);
    assert(!pSym || (dType==CG_STRUCT && pSym->pouTyp==CG_POU_SDT));

    arrayLenTot = 0;
    if(nDims==0)
    {
        typeNameTmp.load(_T("ARRAY OF "));
    }
    else
    {
        typeNameTmp.load(_T("ARRAY["));
        arrayLenTot=1;
        for(i=0;i<nDims;i++)
        {
            arrayLenTot *= (ranges[i].ulRangeLength + 1);
            if(i>0)
                typeNameTmp<<_T(",");

            typeNameTmp<<ranges[i].lRangeStart<<_T(".."); 
            //CAUTION: long + ulong == ulong, this is bad for negative array bounds
            typeNameTmp<<(long)(ranges[i].lRangeStart+ranges[i].ulRangeLength);
        }
        typeNameTmp<<_T("] OF ");
    }

    if(pSym)
    {
        typeNameTmp<<pSym->pszId;
        assert(lStrLen==-1);
        //strlen should always be -1 if not specified:
        lStrLen = -1;
    }
    else
    {
        typeNameTmp<<ST_CG_DTYPES_ToString(dType);
        if(lStrLen>=0)
        {
            assert(dType==CG_STRING || dType==CG_WSTRING || 
                   dType==CG_QVT_STRING || dType==CG_QVT_WSTRING);
            typeNameTmp<<_T("[")<<lStrLen<<_T("]");
        }
        else
        {
            //strlen should always be -1 if not specified:
            assert(lStrLen==-1);
            lStrLen = -1;
        }
    }


    pDType = m_hashDType.get(typeNameTmp);
    if(!pDType)
    {    
        FC_NEW_ZSTRUCT(pDTypeNew, CG_DType);
        pDTypeNew->type     = dType;
        pDTypeNew->tbs      = ST_DTypesToTBS(dType);
        pDTypeNew->pSym     = pSym;
        pDTypeNew->lStrLen  = lStrLen; 
        pDTypeNew->bIsArray = true;
        pDTypeNew->nElems   = arrayLenTot; 
        pDTypeNew->nDim     = nDims;

        for(i=0;i<nDims && i<CG_MAX_ARRAY_DIM;i++)
            pDTypeNew->range[i] = ranges[i];

        pDTypeNew->pszName = registerString(typeNameTmp);

        bOk = m_hashDType.insert(pDTypeNew->pszName, pDTypeNew);
        assert(bOk);
        pDType = pDTypeNew;
    }

    return pDType;
}


const CG_DType* CG_CIECFront::getArrayBaseDType(const CG_DType* pArray)
{
    assert(pArray->bIsArray);
    
    if(pArray->pSym)
    {
        assert(pArray->pSym->pouTyp==CG_POU_SDT && pArray->type==CG_STRUCT);
        return registerDType(pArray->pSym);
    }
    return registerDType(pArray->type, pArray->lStrLen);
}



const CG_AttribList* CG_CIECFront::registerAttrList(
    CG_AttribList* pAttrList
)
{
    m_listAttribLists.add(pAttrList);
    return pAttrList;
}
const long* CG_CIECFront::registerLongArray(
    int   nCount,
    long* pLongs
)
{
    m_listLongArray.add(pLongs);
    return pLongs;
}
const CG_Initialiser* CG_CIECFront::registerInit(
    CG_Initialiser*  pInit
)
{
    m_listInits.add(pInit);//TODO Hash 0,1,true,false
    return pInit;
}
const CG_Initialiser* CG_CIECFront::registerInitArray(
    int              nCount,
    CG_Initialiser*  pInits
)
{
    m_listInitArray.add(pInits);
    return pInits;
}
const CG_Symtab*const* CG_CIECFront::registerSymtabList(
    const CG_Symtab** ppSyms
)
{
    m_listSymtabLists.add(ppSyms);
    return ppSyms;
}
const CG_SymEntry*const* CG_CIECFront::registerEntryArray(
    int                 nInits,
    const CG_SymEntry** pEntries
)
{
    m_listEntryArray.add(pEntries);
    return pEntries;
}
const CG_TaskInit* CG_CIECFront::registerTaskInits(
    CG_TaskInit* pInitsList
)
{
    m_taskInitsList.add(pInitsList);
    return pInitsList;
}
const CG_Task* CG_CIECFront::registerTaskLists(
    CG_Task* pTaskList
)
{
    m_tasksList.add(pTaskList);
    return pTaskList;
}





bool CG_CIECFront::loadKAD(const TCHAR* pszKADFileAndPath)
{
    FC_CString jot(MAX_PATH);

    if(m_kad.isLoaded())
    {
        CGT_IERR(m_pdm, _T("CG_CIECFront: loadKAD(): KAD already loaded"));
        return false;
    }

    if(pszKADFileAndPath)
        jot.load(pszKADFileAndPath);
    else //get standard default kad:
        jot.load(CGT_GetEngBinDir(m_pdm)).appendDir(MODULE_KAD_NAME);

    return m_kad.load(m_pdm, jot);
}
bool CG_CIECFront::load4CP(const TCHAR* psz4CPFile)
{
    int                    i;
    const CG_AttribList*   pSec;
    const TCHAR*           pszVal;
    const CG_KeyValPair*   pPair;
    bool                   bOk;

    bOk = false;
    //HACK: avoid duplicate warnings if 4cp is loaded by 4cg and N-times by backens:
    if(m_features.getTrgTyp())
        m_pdm->disableWarnings();

    if(m_4CP.is4CPLoaded())
    {
        CGT_IERR(m_pdm, _T("CG_CIECFront: load4CP(): 4CP already loaded"));
        goto Error;
    }
    if(!m_kad.isLoaded())
    {
        CGT_IERR(m_pdm, _T("CG_CIECFront: load4CP(): KAD not loaded"));
        goto Error;
    }
    
    if(!m_4CP.load(m_pdm, m_myCOMIF->FCPgetInstallationPath(), psz4CPFile))
        goto Error;

    if(!m_4CP.buildPOUHashes(m_pdm, m_kad))
        goto Error;

    
    //get some relevant opt= 
    //check [format], only the version=<ver> is check here:
    pSec = m_4CP.getSection(CG_SEC_4CP_FORMAT);
    if(pSec)
    {
        pPair = NULL;
        for(i=0; i<pSec->nAttribs; i++)
        {
            if(!_tcsicmp(CG_KEY_4CP_VERSION, pSec->pairs[i].pszKey))
            {
                if(pPair)
                {
                    m_pdm->msg1(CG_E_DOUBLE_ATTRIB, &pSec->pairs[i].edpKey, CG_KEY_4CP_VERSION);
                    goto Error;
                }
                pPair = &pSec->pairs[i];
            }
        }

        if(pPair)
        {
            if(_tcsicmp(pPair->pszVal, CG_VAL_4CP_FORMAT))
            {
                m_pdm->msg2(CG_E_PRJ_INVALID_FORMAT, &pPair->edpVal, 
                    pPair->pszVal, CG_VAL_4CP_FORMAT);
                goto Error;
            }
        }
        else
        {
            m_pdm->msg2(CG_E_PRJ_INVALID_FORMAT, m_4CP.getEDPOfSection(pSec), 
                m_pdm->res1(CG_RES_STR_NOKEY), _T("version=")CG_VAL_4CP_FORMAT);
            goto Error;
        }
    }
    else
    {
        m_pdm->msg2(CG_E_PRJ_INVALID_FORMAT, m_4CP.getEdpBeg(), 
            m_pdm->res1(CG_RES_STR_NOFORMAT), _T(":\n    [format]\n    version=")CG_VAL_4CP_FORMAT);
        goto Error;
    }


    //checking all opts is done in 4cg.exe
    pSec = m_4CP.getSection(CG_SEC_4CP_COMPILER);
    if(pSec)
    {
        for(i=0; i<pSec->nAttribs; i++)
        {
            if(_tcsicmp(CG_BUILD_OPT, pSec->pairs[i].pszKey))
                continue;

            pszVal = pSec->pairs[i].pszVal;

            if(!_tcsicmp(CG_BUILD_OPT_XREF _T("0"), pszVal))
                m_bOPT_DisableXref = true;
        }
    }

    bOk = true;
Error:
    if(m_features.getTrgTyp())
        m_pdm->enableWarnings();
    return bOk;
}


struct BI_LIST
{
    const TCHAR* pszId;
    const char*  pszDef;
};

ST_FUNCTION_DEF_TYP* CG_CIECFront::lookUpBuildInFunDef(
    const TCHAR* pszFunName
)
{
    int                  i;
    int                  iRet;
    ST_FUNCTION_DEF_TYP* pDef;

    if(!m_pBuilnInList)
    {
        const ST_TypeSet_C types[] = {
            /* "0" */ CG_TBS_BOOL,
            /* "1" */ CG_TBS_BYTE,
            /* "2" */ CG_TBS_WORD,
            /* "3" */ CG_TBS_DWORD,
            /* "4" */ CG_TBS_DINT,
            /* "5" */ CG_TBS_LREAL,
            /* "6" */ CG_TBS_LINT,
            /* "7" */ CG_TBS_LWORD,
            /* "8" */ CG_TBS_WSTRING,
            /* "9" */ CG_TBS_DT,
            /* "A" */ CG_TBS_QVT_BOOL,
            /* "B" */ CG_TBS_QVT_BYTE,
            /* "C" */ CG_TBS_QVT_WORD,
            /* "D" */ CG_TBS_QVT_DWORD,
            /* "E" */ CG_TBS_QVT_DINT,
            /* "F" */ CG_TBS_QVT_LREAL,
            /* "G" */ CG_TBS_QVT_LINT,
            /* "H" */ CG_TBS_QVT_LWORD,
            /* "I" */ CG_TBS_QVT_WSTRING,
            /* "J" */ CG_TBS_ANY_QVT
        };
        //CAUTION: static is important, hold refs to the strings...
        static const BI_LIST bis[] = 
        {
             {_T("QualityOf"),    "3=J"}
            ,{_T("ValueOf"),      "0=A|1=B|2=C|3=D|4=E|5=F|6=G|7=H|8=I"}
            ,{_T("TimestampOf"),  "9=J"}
            ,{_T("QVT_BOOL"),     "A=309"}
            ,{_T("QVT_BYTE"),     "B=319"}
            ,{_T("QVT_WORD"),     "C=329"}
            ,{_T("QVT_DWORD"),    "D=339"}
            ,{_T("QVT_DINT"),     "E=349"}
            ,{_T("QVT_LREAL"),    "F=359"}
            ,{_T("QVT_LINT"),     "G=369"}
            ,{_T("QVT_LWORD"),    "H=379"}
            ,{_T("QVT_WSTRING"),  "I=389"}
        };
        
        m_pBuilnInList = new FC_CHashStr<ST_FUNCTION_DEF_TYP*>(true, false, 32);
        
        for(i=0; i<FC_ARRAY_LEN(bis); i++)
        {
            pDef = new ST_FUNCTION_DEF_TYP;
            iRet = ST_InitFunctionDef(bis[i].pszId, types, FC_ARRAY_LEN(types), bis[i].pszDef, pDef);
            assert(iRet);
            iRet = m_pBuilnInList->insert(bis[i].pszId, pDef);
            assert(iRet);
        }
    }

    return m_pBuilnInList->get(pszFunName);
        
}




I4CLAccess* CG_CIECFront::getLibIFByPOUInfo(const CG_LIBContext* pInfo)
{
    int        i;
    ST_LIB_TYP lt;

    assert(m_4CP.is4CPLoaded());

    for(i = 0; i<m_openLibs.getUsed(); i++)
    {
        lt = m_openLibs.get(i);
        if(!lstrcmpi(lt.pInfo->pszId, pInfo->pszId))
        {
            assert(lt.pILib && lt.pInfo==pInfo);//raw...
            return lt.pILib;
        }
    }
    return NULL;
}


I4CLAccess* CG_CIECFront::findLibOfPOU(
    const TCHAR*          pszPou, 
    const CG_LIBContext** ppInfo,
    bool                  bWarnDuply
)
{
    int             i;
    I4CLAccess*     pIFound = NULL;
    BSTR            sPOU    = FC_StringToBSTR(pszPou);


    for(i = 0; i<m_openLibs.getUsed(); i++)
    {
        ST_LIB_TYP lt = m_openLibs.get(i);
        
        if(S_FALSE == lt.pILib->ExistsPOU(sPOU, NULL))
            continue;
        
        if(pIFound)
        {
            BSTR lib1stFound;
            BSTR lib2ndFound;
            pIFound->GetLibFileName(&lib1stFound);
            lt.pILib->GetLibFileName(&lib2ndFound);

            m_pdm->msg4(CG_W_LIB_POU_ALREADY_DEF_IN_LIB, NULL,
                lib1stFound, sPOU, lib2ndFound, lib1stFound);
            
            FC_FREE_BSTR(lib1stFound);
            FC_FREE_BSTR(lib2ndFound);

            break; //stop, one warning is enough...
        }
        else 
        {
            //remember first def and continue search if bWarn is wanted:
            pIFound = lt.pILib;
            if(ppInfo)
                *ppInfo = lt.pInfo;

            if(!bWarnDuply)
                break;//no warning wanted, stop here
        }
    }

    FC_FREE_BSTR(sPOU);
    return pIFound;
}



/*
 * Syntax of returned locator is:
 * Project defined:
 *    [path\]<pouname>.[sdt|st|4gr|<filter ext>]
 *
 * Library defined and no target type specified or after error "POU does not support..":
 *     <libid>.4cl::<pouname>.[sdt|hdr]
 *
 * Library defined and target type given and supported by POU and source code lib:
 * [<libid>.4cl::]<pouname>[\<trgtype>]\<pouname>.[sdt|st|4gr|<filter ext>]
 *  
 * Library defined and target type given and supported by POU and precompiled:
 * [<libid>.4cl::]<pouname>[\<trgtype>]\<pouname>.[hdr]
 *
 * <libid> is the library file base name without path e.g 4C_IEC1131_3
 * Note: libs attached to the project must be unique in respect to their file 
 * base name.
 * @return  0  undef
 *         -1  lib POU defined but does not support target type (only if
 *             pszTrgType != NULL and not empty string.
 *          1  OK
 */
long CG_CIECFront::findPOUDefByIECName(
    const TCHAR*          pszPOUName, 
    const TCHAR*          pszTrgType,
    const CG_POU4CPInfo** ppPOUInf,
    const CG_LIBContext** ppLIBInf,
    FC_CString*           pFileLocator,
    bool                  bWarnDuply 
)
{
    long                 lRet;
    I4CLAccess*          pILib;
    const CG_POU4CPInfo* pPOUInf;
    const CG_LIBContext* pLIBInf;
    HRESULT              hr;


    *ppPOUInf = NULL;
    *ppLIBInf = NULL;
    pFileLocator->clear();

    //search project files first:
    pPOUInf = m_4CP.getPOUinfo(pszPOUName);
    if(pPOUInf)//POU found in 4cp file
    {
        if(bWarnDuply)
        {
            //warn if also in any lib:
            pILib = findLibOfPOU(pszPOUName, &pLIBInf, false);
            if(pILib)
            {
                m_pdm->msg2(CG_W_PRJ_POU_ALREADY_DEF_IN_LIB, 
                  &pPOUInf->edp, pszPOUName, pLIBInf->pszFile);
            }
        }

        //locator is simply the file name as found in 4cp in it's canonical form:
        pFileLocator->load(pPOUInf->pszFile);    
        *ppPOUInf = pPOUInf;
        return 1;
    }


    //try search libs (param #2==true: warn if defined in any second lib):
    pILib = findLibOfPOU(pszPOUName, &pLIBInf, bWarnDuply);
    if(!pILib)
        return 0; //definitely not defined.
    *ppLIBInf = pLIBInf;


    //check if supported for pszTrgType if there is a target type scope:
    lRet = 1;
    if(pszTrgType && pszTrgType[0])
    {
        BSTR sPOU = FC_StringToBSTR(pszPOUName);
        BSTR sTRG = FC_StringToBSTR(pszTrgType);
    
        hr = pILib->ExistsPOU(sPOU, sTRG);
        if(hr!=S_OK)
        {
            assert(hr==S_FALSE);
            lRet = -1;  //war doch nix...
        }

        FC_FREE_BSTR(sPOU);
        FC_FREE_BSTR(sTRG);
    }
 

    pFileLocator->load(pLIBInf->pszFile)<<_T("::")<<pszPOUName;
    if(pszTrgType && lRet==1)
        pFileLocator->addc('\\')<<pszTrgType;


    //get and read the .link file:
    CGT_CSVFile      csvr;
    CGT_STDLINK_INFO lnkf;
    FC_CString       errDescr;

    pFileLocator->addc(_T('\\'))<<CG_LINK_FILE;
    if(!CGT_LoadCSVLocatorFile(m_pdm, &csvr, this->m_myCOMIF, *pFileLocator))
    {
        CG_Edipos edp;
        CG_InitSTEdipos(&edp, *pFileLocator);
        CGT_IERR2(m_pdm, &edp, _T("cannot get .link form library"));
        return 0;
    }
    
    //remove .link again:
    pFileLocator->stripFilePart();
 
    if(CGT_ReadLinkFile(&csvr, &lnkf, &errDescr))
    {
        if(!lnkf.source.isEmpty())
        {
            pFileLocator->addc('\\')<<lnkf.source;
        }
        else if(!lnkf.header.isEmpty())
        {
            pFileLocator->addc('\\')<<lnkf.header;
        }
        else
        {
            CG_Edipos edp;
            CG_InitSTEdipos(&edp, (*pFileLocator)<<_T("\\.link"));
            CGT_IERR2(m_pdm, &edp, _T("'source;' and  'header;' : both empty, don't know how to use library POU?"));
        }
    }
    else
    {
        CG_Edipos edp;
        CG_InitSTEdipos(&edp, (*pFileLocator)<<_T("\\.link"));
        CGT_IERR2(m_pdm, &edp, errDescr);
    }
 
    return lRet;
}







//--- load source files:
bool CG_CIECFront::loadLib(
    const CG_LIBContext*  pInf
)
{
    int          i;
    HRESULT      hr;
    BSTR         bstr;
    BSTR         bstrErr;
    I4CLAccess*  pILibAcc;
    TCHAR        fullpath[MAX_PATH*2];
    ST_LIB_TYP   lt;

    
    lt.pILib = NULL;
    lt.pInfo = m_4CP.getLIBinfo(pInf->pszId);//HACK.
    assert(pInf==lt.pInfo); //HACK.


    //make full path first:
    FC_FileFullPath(m_4CP.getPrjPath(), pInf->pszFile, fullpath, FC_ARRAY_LEN(fullpath));
    
    if(!FC_Exist(fullpath))
    {
        m_pdm->msg1(CG_E_FILE_NOT_EXIST, &pInf->edp, fullpath);
        return false;
    }


    //check that not reopen lib, should not occure:
    for(i=0; i<m_openLibs.getUsed(); i++)
    {
        if(pInf == m_openLibs.getAddrOf(i)->pInfo)
        {
            CGT_IERR(m_pdm, m_pdm->jot1()<<_T("CG_CIECFront::loadLib: attempt to reload library: libid='")<<fullpath);
            return false;
        }
    }


    if(m_pILibOpen == NULL)
    {
        //must first create lib access instance:
        hr = CoCreateInstance(
                  CLSID_FC4CLAccess,
                  NULL, 
                  CLSCTX_INPROC_SERVER,
                  IID_I4CLOpen, 
                  (void**)&m_pILibOpen
             );    

        if(FAILED(hr))
        {
            m_pdm->sysErr(hr, NULL, _T("CoCreateInstance for CLSID_FC4CLAccess: lib to open: "), fullpath);
            return false;
        }
    }


    //try to open it:
    bstr = FC_StringToBSTR(fullpath);
    hr   = m_pILibOpen->Open(false, bstr, &bstrErr, &pILibAcc);
    FC_FREE_BSTR(bstr);
    if(FAILED(hr))
    {
        assert(!pILibAcc && bstrErr);
        m_pdm->msg1(CG_E_LIBRARIAN_ERROR, &pInf->edp, fullpath);
        m_pdm->userMsg(m_pdm->jot1()<<_T("    ")<<bstrErr<<_T("\r\n"));
        return false;
    }

    //register the I4CLAccess interface forever:
    lt.pILib = pILibAcc;
    m_openLibs.add(lt);

    return true;
}


//read all cst files of the lib:
bool CG_CIECFront::loadLIBCSTs(
    const CG_LIBContext*  pInf
)
{
    I4CLAccess*   pILibAcc; 
    FC_CString    cstFileLocator(MAX_PATH);
    CGT_CSVFile   csvrd;
    TCHAR*        pBuffer;
    unsigned long nChars;
    const TCHAR*  pszFile;

    pILibAcc = getLibIFByPOUInfo(pInf);
    if(!pILibAcc)
    {
        CGT_IERR(m_pdm, _T("CG_CIECFront::loadLIBCSTs: invalid CG_LIBContext* argument"));
        return false;
    }

    //check first if there are any CONST, maybe there is not even the folder CONST:
    if(!CGT_GetLIBFileData(m_pdm, pILibAcc, _T("CONST"), NULL, _T("cst"), NULL, NULL, NULL))
        return true; //CONST\cst does not exist, do nothing & return true...
     

    if(!CGT_GetLIBFileData(m_pdm, pILibAcc, _T("CONST"), NULL, _T("cst"), &pBuffer, &nChars, NULL))
        return false;

    csvrd.setStringContent(pBuffer, nChars, true);

    cstFileLocator<<pInf->pszFile<<_T("::CONST\\cst");
    while(csvrd.hasMoreFields())
    {
        if(!csvrd.getNextField(&pszFile))
        {
            CG_Edipos edptmp;
            CG_InitSTEdipos(&edptmp, cstFileLocator, csvrd.getLineno());
            m_pdm->msg0(CG_E_SYNTAX, &edptmp);
            return false;
        }

        cstFileLocator.stripFilePart();
        cstFileLocator.addc(_T('\\'))<<pszFile;
        bool bRec;
        if(!loadCST(cstFileLocator, true, &bRec) || bRec)
        {
            assert(!bRec);//TODO the recursive is a problem, can it ever happen here??
            m_pdm->msg1(CG_E_FILE_NOT_EXIST, NULL, cstFileLocator);
        }
    }

    return true;
}

bool CG_CIECFront::lookUpAutoDecl(
    const TCHAR*        pszId,
    ST_CSymtabPrv*      pSymPrv,
    const CG_SymEntry** ppEntry
)
{
    HRESULT            hr;
    const CG_DType*    pType = NULL;
    const TCHAR*       pszTrgType =  m_features.getTrgTyp();
    ICG_AutoDecl*      pIFAuto    =  m_features.getAutoDeclIF(m_pdm);
    
    
    if(ppEntry)
        *ppEntry = NULL;

    if(!pIFAuto)
        return false; //no such feature



    BSTR sId = FC_StringToBSTR(pszId);
	BSTR sIECAddr = NULL;
	BSTR sIECType = NULL;
    BOOL bReadOnly;

    hr = pIFAuto->getAddressInfo(       
				sId,        //IEC identifier to test 
				&sIECAddr,  //Target specific direct address to be used for OPC.
				&sIECType,  //The IEC data type of the predefined variable
				&bReadOnly  //true if not writable.
		 );

    if(FAILED(hr))
    {
        m_pdm->sysErr(hr, NULL, _T("IAutoDecl::getAddressInfo id=="),pszId);
        return false;
    }

    if(!pSymPrv || hr!=S_OK)
    {
        FC_FREE_BSTR(sIECAddr);
        FC_FREE_BSTR(sIECType);
        return hr==S_OK;
    }

    
    //get the data type:
    //must be simple 
    CG_DTYPES    dt;
    FC_CString   jot;
    int          lStrLen = -1;
    const TCHAR* psz;

    assert(sIECType);
    jot.load(sIECType);
    
    psz = jot.cutAtLast(_T("["));
    if(*psz)
    {
        lStrLen = atoi(psz);
        assert(lStrLen>0);
    }
    
    dt = ST_TypeKeywordToCG_DTYPES(jot);
    pType = registerDType(dt, lStrLen);

    if(pType)
    {
        CG_Edipos  edp;
        FC_CString edpFile(MAX_PATH);
        FC_CString addr;
        const CG_SymEntry* pEntry;

        if(sIECAddr)
            addr.load(sIECAddr);
        
        edpFile<<_T("<")<<pszTrgType<<_T(" system variable>");
        CG_InitSTEdipos(&edp, registerString(edpFile));
        edp.typ = CG_EDP_FILE;


        pEntry = pSymPrv->registerDecl(
            pszId, 
            &edp, 
            CG_VAR_EXTERNAL,
            bReadOnly ? CG_DMOD_CONSTANT|CG_DMOD_SYS_VAR : CG_DMOD_SYS_VAR,
            NULL,
            pType,
            sIECAddr ? addr.psz() : NULL, 
			NULL, //forosai
            sIECAddr ? &edp       : NULL, 
            NULL
        );
        
        if(ppEntry)
            *ppEntry = pEntry;
    }

    
    FC_FREE_BSTR(sIECAddr);
    FC_FREE_BSTR(sIECType);

    return true;
}


ST_CSymtabPrv* CG_CIECFront::loadCST(
    const TCHAR*     pszCSTFile,
    bool             bAddToCSTPool,
    bool*            pbRecursive
)
{
    bool                 bOk;
    ST_CSymtabPrv*       pSymPrv;
    TCHAR                canrel[MAX_PATH*2];
    const CG_LIBContext* pLIBInf;

    *pbRecursive = false;
    
    if(m_kad.getSourceTypeByExt(FC_StringGetFileExt(pszCSTFile)) != CG_SRC_TYPE_CST)
    {
        CGT_IERR(m_pdm, _T("CG_CIECFront::loadCST called for non .cst file"));
        return NULL;
    }

    FC_StringCanonicalPath(pszCSTFile, canrel, FC_ARRAY_LEN(canrel), m_4CP.getPrjPath(), FC_PATHM_KEEPREL);
    pSymPrv = m_symtabs.get(canrel);
    if(pSymPrv)
        return pSymPrv;

    
    if(CGT_ExistsLocatorFile(m_pdm, m_myCOMIF, canrel, &pLIBInf)<=0)
        return NULL; //no diag, caller must do this.

    //assert that this is a global unique pointer to this->m_4CP data:
    assert(!pLIBInf || m_4CP.getLIBinfo(pLIBInf->pszId) == pLIBInf);

    //to avoid any recursive dependencies on file level, this is not always 
    //the same than recursive POUs in IEC terms (because of e.g. #import):
    if(!m_hashOnParsingFiles.insertEx(canrel, NULL, false, &pSymPrv))
    {
        *pbRecursive = true;
        return NULL;
    }


    //don't use back end feature set on load CST!!!
    //Problem: unsupported data type must not be an error
    //before actually using such const from the .cst file.
    //safeFeatureSet will set max front end features.
    FeatureSet* pOldFt = m_features.getFeatureSet();

    pSymPrv = ST_ParseAnyHeader(NULL, this, canrel, pLIBInf);

    //restore any previouis setting. TODO should only remove type restrictions?
    m_features.restoreFeatureSet(pOldFt);

    assert(pSymPrv || m_pdm->hasErr());
    if(!pSymPrv)
    {
        //must be hard error before CONST ...,
        //Use edp NULL
        pSymPrv = new ST_CSymtabPrv(this, NULL, pLIBInf, canrel, CG_POU_CST, CG_SRC_TYPE_CST, 0);
    }

    bOk = m_symtabs.insert(pSymPrv->getCGSym()->pszId, pSymPrv);
    assert(bOk);


    if(bAddToCSTPool)
    {
        if(m_globalConstTable.findElem(pSymPrv)<0)
            m_globalConstTable.add(pSymPrv);
    }

    //release the file from active list:
    //assert: it should always work like a stack, doesn't it??
    assert(!m_hashOnParsingFiles.lastPair()->pNext);
    bOk = m_hashOnParsingFiles.remove(canrel);
    assert(bOk);

    //consider XREF:
    ST_ConsiderXREFDump(this, pSymPrv, NULL);

    return pSymPrv;
}

ST_CConfig* CG_CIECFront::loadConfig(
    const CG_POU4CPInfo* pCON
)
{
    bool            bOk;
    ST_CConfig*     pCon;

    if(m_kad.getSourceTypeByExt(FC_StringGetFileExt(pCON->pszFile)) != CG_SRC_TYPE_CON)
    {
        CGT_IERR(m_pdm, _T("CG_CIECFront::loadConfig called for non .con file"));
        return NULL;
    }
    
    if(!m_hashOnParsingFiles.insertEx(pCON->pszFile, NULL, false, NULL))
    {
        //will virtually never happen, but you never know...
        CGT_IERR2(m_pdm, &pCON->edp, _T("recursive load of con file"));
        return NULL;
    }

    pCon = ST_ParseCON(NULL, this, pCON->pszFile);
    assert(pCon || m_pdm->hasErr());
    
    //if(!pCon) must be hard error before CONFIGURATION...,

    //release the file from active list:
    //assert: it should always work like a stack, doesn't it??
    assert(!m_hashOnParsingFiles.lastPair()->pNext);
    bOk = m_hashOnParsingFiles.remove(pCON->pszFile);
    assert(bOk);

    return pCon;
}


const CG_Resource* CG_CIECFront::loadResource(
    const TCHAR*     pszResFile
)
{
    bool            bOk;
    CG_Resource*    pRes;
    ST_CSymtabPrv*  pSymRes;
    TCHAR           canrel[MAX_PATH*2];
    FC_CString      jot;

    jot.load(FC_StringGetFilePart(pszResFile));
    jot.changeFileExt(NULL);

    pRes = m_resources.get(jot);
    if(pRes)
        return pRes;


    if(m_kad.getSourceTypeByExt(FC_StringGetFileExt(pszResFile)) != CG_SRC_TYPE_RES)
    {
        CGT_IERR(m_pdm, _T("CG_CIECFront::loadResource called for non .res file"));
        return NULL;
    }
    
    //currently resources are never from libs, the unique name is the canonical
    //proj dir relative file name, even if the caller uses a full path
    //the look up in the m_symtabs should work, therefore convert to 
    //canonical but still relative file name:
    FC_StringCanonicalPath(pszResFile, canrel, FC_ARRAY_LEN(canrel), m_4CP.getPrjPath(), FC_PATHM_KEEPREL);
    
    if(!FC_FileExists(m_4CP.getPrjPath(), canrel))
        return NULL; //no diag, caller must do this.


    bOk = m_hashOnParsingFiles.insert(canrel, NULL);
    assert(bOk);

    pRes = ST_ParseRES(NULL, this, canrel, &pSymRes);
    if(!pRes)
    {
        assert(!pSymRes);
        //must be hard error before RESOURCE <name> or <name> 
        //differs from file name. Use edp of file and res name is 
        //file name base:
        FC_NEW_ZSTRUCT(pRes, CG_Resource);
        CG_InitSTEdipos(&pRes->edp, registerString(canrel));
        pRes->pszId  = registerString(jot);
    }

    bOk = m_resources.insert(pRes->pszId, pRes);
    //assert: parser must return NULL if resname!=filename:
    assert(bOk);
    
    assert(pSymRes || m_pdm->hasErr());
    if(pSymRes)
    {
        assert(pRes);
        bOk = m_symtabs.insert(pSymRes->getCGSym()->pszId, pSymRes);
        assert(bOk);
    }

    //release the file from active list:
    //assert: it should always work like a stack, doesn't it??
    assert(!m_hashOnParsingFiles.lastPair()->pNext);
    bOk = m_hashOnParsingFiles.remove(canrel);
    assert(bOk);


    //consider XREF:
	if(pSymRes)
        ST_ConsiderXREFDump(this, pSymRes, NULL);

    return pRes;
}


ST_CSymtabPrv* CG_CIECFront::loadHeader(
    const TCHAR*     pszPOUName,
    bool*            pbRecursive
)
{
    bool                 bOk;
    const CG_Symtab*     pSym;
    ST_CSymtabPrv*       pSymPrv;
    const CG_POU4CPInfo* pPOUInf;
    const CG_LIBContext* pLIBInf;
    const CG_Edipos*     pEdpFileIOerr;
    long                 lDefType;
    const TCHAR*         pszTrgType;
    FeatureSet*          pOldFt;

    assert(!_tcschr(pszPOUName, '.'));//<-dont' load pous this way!


    *pbRecursive = false;

    //look up first in the loaded POUs hash:
    pSymPrv = m_symtabs.get(pszPOUName);
    if(pSymPrv)
        return pSymPrv;

    
    //search POU definition:
    FC_CString  fileLocator(MAX_PATH);
    pszTrgType = m_features.getTrgTyp();
    lDefType = findPOUDefByIECName(pszPOUName, pszTrgType, &pPOUInf, &pLIBInf, &fileLocator, true);
    if(lDefType==0)
        return NULL; //undefined

    //to avoid any recursive dependencies on file level, this is not always 
    //the same than recursive POUs in IEC terms (because of e.g. #import):
    if(!m_hashOnParsingFiles.insert(fileLocator, NULL))
    {
        *pbRecursive = true;
        return NULL;
    }

    assert((pPOUInf || pLIBInf) && ! (pLIBInf && pPOUInf));
    if(pPOUInf)
        pEdpFileIOerr = &pPOUInf->edp;
    else
        pEdpFileIOerr = &pLIBInf->edp;

    
    //don't use back end feature set when lib does not support POU!!!
    //Problem: unsupported data type must not be an error
    //before actually submitting the not supported diag in loadHeaderEx.
    if(lDefType==-1)//HACK ???
        pOldFt = m_features.getFeatureSet();

    pSymPrv = ST_ParseAnyHeader(pEdpFileIOerr, this, fileLocator, pLIBInf);

    //restore any previouis setting. TODO should only remove type restrictions?
    if(lDefType==-1)//HACK ???
        m_features.restoreFeatureSet(pOldFt);

    assert(pSymPrv || m_pdm->hasErr());
    if(!pSymPrv)
    {
        //must be hard error before PROGRAM <name> or <name> 
        //differs from file name. Use edp of 4cp
        pSymPrv = new ST_CSymtabPrv(
            this, pEdpFileIOerr, pLIBInf, pszPOUName, CG_POUTYP_UNDEF, 
               m_kad.getSourceTypeByExt(FC_StringGetFileExt(fileLocator)), 0);
    }
    
    //insert in hash, this is the first (and only) time the POU gets loaded:
    pSym = pSymPrv->getCGSym();
    bOk = m_symtabs.insert(pSym->pszId, pSymPrv);
    assert(bOk && !lstrcmpi(pSym->pszId, pszPOUName));

    //release the file from active list:
    //assert: it should always work like a stack, doesn't it??
    assert(!m_hashOnParsingFiles.lastPair()->pNext);
    bOk = m_hashOnParsingFiles.remove(fileLocator);
    assert(bOk);

    //consider XREF:
    ST_ConsiderXREFDump(this, pSymPrv, NULL);

    return pSymPrv;
}




long CG_CIECFront::loadHeaderEx(
    const CG_Edipos*            pEdp,
    const TCHAR*                pszPOUName,
    long                        want,
    ST_CSymtabPrv**             ppSymPrv,
    const ST_FUNCTION_DEF_TYP** ppBIDef
)
{
    bool                       bRecursive;
    ST_CSymtabPrv*             pSymPrv;
    const CG_Symtab*           pSym;
    const CG_KeyValPair*       pAttr;
    const ST_FUNCTION_DEF_TYP* pBIDef;
    
    *ppSymPrv = NULL;
    if(ppBIDef)
        *ppBIDef  = NULL;

    pSymPrv = loadHeader(pszPOUName, &bRecursive);
    if(!pSymPrv)
    {
        if(bRecursive)
        {
            m_pdm->msg1(CG_E_RECURSIV, pEdp, pszPOUName);
            return ST_LDHD_RECURSIVE;
        }

        //may be buildin ?
        pBIDef = lookUpBuildInFunDef(pszPOUName);
        if(pBIDef)
        {
            if(!ST_CheckPOUType(m_pdm, pEdp, pszPOUName, CG_POU_FUN, want))
                return ST_LDHD_BAD_POUTYP;
            
            if(ppBIDef)
            {
                *ppBIDef = pBIDef;
                return ST_LHDR_OK;
            }
            else
            {
                m_pdm->msg1(CG_E_ILL_USE_BUILDIN, pEdp, pszPOUName);
                return ST_LDHD_ILLBI;
            }
        }
        
        //no, then create error msg:
        ST_CheckPOUType(m_pdm, pEdp, pszPOUName, CG_POUTYP_UNDEF, want);
        return ST_LHDR_UNDEF;
    }

    *ppSymPrv = pSymPrv;
    pSym = pSymPrv->getCGSym();
    
    if((pSym->flags & CG_STAB_HDR_NOERR)==0)
    {
        //header in error:
        //if the pou type keyword could be parsed then check it:
        if(pSym->pouTyp != CG_POUTYP_UNDEF)
        {
            if(!ST_CheckPOUType(m_pdm, pEdp, pszPOUName, pSym->pouTyp, want))
                return ST_LDHD_ERRIND|ST_LDHD_BAD_POUTYP;
        }//else: do not produce undef POU diag, caller must hanlde this problem!
        
        return ST_LDHD_ERRIND;
    }

 
    if(!ST_CheckPOUType(m_pdm, pEdp, pszPOUName, pSym->pouTyp, want))
        return ST_LDHD_BAD_POUTYP;


    pAttr = m_myCOMIF->lookUpAttrib(pSym->pAttribs, CG_ATTR_DEPRECATED);
    if(pAttr)
        m_pdm->msg2(CG_W_DEPRECATED, pEdp, pSym->pszId, pAttr->pszVal);


    //test if lib POU supports a possible trg type:
    if((pSym->flags & CG_STAB_LIBIF_ONLY) && m_features.getTrgTyp())
    {
        assert(pSym->pLibCtx);
        m_pdm->errNS3(pEdp,  m_pdm->jot1()<<_T("'")<<pSym->pszId<<_T("'"), 
            m_pdm->res1(CG_RES_STR_LIB), pSym->pLibCtx->pszId);
        return ST_LDHD_TRG_NS;
    }

    return ST_LHDR_OK;
}


const ST_CSymtabPrv* CG_CIECFront::loadGVL(
    const TCHAR*     pszGVLFile,
    bool*            pbRecursive
)
{
    bool            bOk;
    ST_CSymtabPrv*  pSymPrv;
    TCHAR           canrel[MAX_PATH*2];

    *pbRecursive = false;

    //check correct call with .gvl ext:
    if(m_kad.getSourceTypeByExt(FC_StringGetFileExt(pszGVLFile)) != CG_SRC_TYPE_GVL)
    {
        CGT_IERR(m_pdm, _T("CG_CIECFront::loadGVL called for non .gvl file"));
        return NULL;
    }
    
    //currently gvls are never from libs, the uniqu name is the canonical
    //proj dir relative file name, even if the caller uses a full path
    //the look up in the m_symtabs should work, therefore convert to 
    //canonical but still relative file name:
    FC_StringCanonicalPath(pszGVLFile, canrel, FC_ARRAY_LEN(canrel),
        m_4CP.getPrjPath(), FC_PATHM_KEEPREL);
    
    //look up first in the loaded POUs hash:
    pSymPrv = m_symtabs.get(canrel);
    if(pSymPrv)
        return pSymPrv;

    if(!FC_FileExists(m_4CP.getPrjPath(), canrel))
        return NULL; //no diag, caller must do this.

    //to avoid any recursive dependencies on file level, this is not always 
    //the same than recursive POUs in IEC terms (because of e.g. #import):
    if(!m_hashOnParsingFiles.insertEx(canrel, NULL, false, &pSymPrv))
    {
        *pbRecursive = true;
        return NULL;
    }

    pSymPrv = ST_ParseAnyHeader(NULL, this, canrel, NULL);
    assert(pSymPrv || m_pdm->hasErr());
    if(!pSymPrv)
    {
        //must be hard error before VAR_GLOBAL ...,
        //Use edp NULL
        pSymPrv = new ST_CSymtabPrv(this, NULL, NULL, canrel, CG_POU_GVL, CG_SRC_TYPE_GVL, 0);
    }

    //insert in hash, this is the first (and only) time the POU gets loaded:
    bOk = m_symtabs.insert(pSymPrv->getCGSym()->pszId, pSymPrv);
    assert(bOk);

    //release the file from active list:
    //assert: it should always work like a stack, doesn't it??
    assert(!m_hashOnParsingFiles.lastPair()->pNext);
    bOk = m_hashOnParsingFiles.remove(canrel);
    assert(bOk);

    //consider XREF:
    ST_ConsiderXREFDump(this, pSymPrv, NULL);

    return pSymPrv;
}

const ST_CSymtabPrv* CG_CIECFront::loadBody(
   const TCHAR*     pszPOUName,
   long             codeGenOpt,
   ST_CBody**       ppBody,
   bool*            pbRecursive
)
{
    bool                 bOk;
    const CG_Symtab*     pSym;
    ST_CSymtabPrv*       pSymPrv;
    const CG_POU4CPInfo* pPOUInf;
    const CG_LIBContext* pLIBInf;
    const CG_Edipos*     pEdpFileIOerr;
    long                 lDefType;
    const TCHAR*         pszTrgType;
    long                 flags;
    ST_CSymtabPrv*       pSymPrvOld;
    bool                 bIsReloadBody;
    FeatureSet*          pOldFt;

    assert(!_tcschr(pszPOUName, _T('.')));//<-dont' load pous this way!

    *ppBody = NULL;
    *pbRecursive = false;


    //look up first in the loaded POUs hash:
    bIsReloadBody = false;
    pSymPrvOld = m_symtabs.get(pszPOUName);
    if(pSymPrvOld)
    {
        flags = pSymPrvOld->getCGSym()->flags;
        bIsReloadBody = (flags & CG_STAB_BDY_LOADED)!=0;

        if((flags & CG_STAB_HAS_BDY)==0)//no bdy, pure inline (or hdr error...) 
            return pSymPrvOld;//strange, why does caller try load bdy ?

        //hdr has errors will not get better by reload...
        if((flags & CG_STAB_HDR_NOERR)==0) //hdr in error...
            return pSymPrvOld;

        //bdy has errors will not get better by reload...
        if(bIsReloadBody && !(flags & CG_STAB_BDY_NOERR))
            return pSymPrvOld;

    }


    //search POU definition:
    FC_CString  fileLocator(MAX_PATH);
    pszTrgType = m_features.getTrgTyp();
    lDefType = findPOUDefByIECName(pszPOUName, pszTrgType, &pPOUInf, &pLIBInf, &fileLocator, true);
    if(lDefType==0)
        return NULL; //undefined


    //to avoid any recursive dependencies on file level, this is not always 
    //the same than recursive POUs in IEC terms (because of e.g. #import):
    if(!m_hashOnParsingFiles.insert(fileLocator, NULL))
    {
        *pbRecursive = true;
        return NULL;
    }

    assert((pPOUInf || pLIBInf) && ! (pLIBInf && pPOUInf));
    if(pPOUInf)
        pEdpFileIOerr = &pPOUInf->edp;
    else
        pEdpFileIOerr = &pLIBInf->edp;


    //don't use back end feature set when lib does not support POU!!!
    //Problem: unsupported data type must not be an error
    //before actually submitting the not supported diag in loadHeaderEx.
    if(lDefType==-1)//HACK ???
        pOldFt = m_features.getFeatureSet();

    //FC_DEBUGPRINT1(_T("CG_CIECFront::loadBody: %s\n"), pszPOUName);
    pSymPrv = ST_ParseSTIL4GR(pEdpFileIOerr, this, fileLocator, pLIBInf,
                     ppBody, pSymPrvOld, codeGenOpt);

    //restore any previouis setting. TODO should only remove type restrictions?
    if(lDefType==-1)//HACK ???
        m_features.restoreFeatureSet(pOldFt);

    assert(!pSymPrvOld || pSymPrvOld==pSymPrv);

    assert(pSymPrv || m_pdm->hasErr());
    if(!pSymPrv)
    {
        assert(!pSymPrvOld);
        //must be hard error before PROGRAM <name> or <name> 
        //differs from file name. Use edp of 4cp
        pSymPrv = new ST_CSymtabPrv(
            this, pEdpFileIOerr, pLIBInf, pszPOUName, CG_POUTYP_UNDEF, 
             m_kad.getSourceTypeByExt(FC_StringGetFileExt(fileLocator)), 0);
    }
 
    //insert in POU hash if header was loaded the first time:
    pSym = pSymPrv->getCGSym();
    if(pSymPrv!=pSymPrvOld)
    {
        bOk = m_symtabs.insert(pSym->pszId, pSymPrv);
        assert(bOk && !lstrcmpi(pSym->pszId, pszPOUName));
    }

    flags = pSym->flags;
    if(flags & CG_STAB_HAS_BDY)//set loaded flag if has a bdy
    {
        assert(*ppBody || (flags & CG_STAB_BDY_NOERR)==0);
        assert((flags & CG_STAB_BDY_NOERR) || m_pdm->hasErr());

        //on error: initialise the symtab members that wait for end of body
        if((flags & CG_STAB_BDY_LOADED)==0)
        {
            assert(m_pdm->hasErr() && !pSym->pzRefSyms && !pSym->pzRefGlbCst);
            pSymPrv->endOfBody(true, 0);
        }
    }

    assert(pSym->pzRefSyms && pSym->pzRefGlbCst);

    //release the file from active list:
    //assert: it should always work like a stack, doesn't it??
    assert(!m_hashOnParsingFiles.lastPair()->pNext);
    bOk = m_hashOnParsingFiles.remove(fileLocator);
    assert(bOk);

    //consider XREF, but not if reload body:
    if(!bIsReloadBody) 
        ST_ConsiderXREFDump(this, pSymPrv, *ppBody);

    return pSymPrv;
}







//symbol table look up:
const CG_SymEntry* CG_CIECFront::lookUpEntry(
    const CG_Symtab* pSym, 
    const TCHAR*     pszId
)
{
    const ST_CSymtabPrv* pPrv;
    assert(pSym && pszId);

    pPrv = m_symtabs.get(pSym->pszId);
    if(!pPrv)
    {
        CGT_IERR(m_pdm, _T("symtab not found"));
        return NULL;
    }
    return pPrv->lookUpEntry(pszId);
}

const CG_SymEntry* CG_CIECFront::lookUpEntryDeep(
    const CG_Symtab* pSym, 
    const TCHAR*     pszId
)
{
    const ST_CSymtabPrv* pPrv;
    assert(pSym && pszId);

    pPrv = m_symtabs.get(pSym->pszId);
    if(!pPrv)
    {
        CGT_IERR(m_pdm, _T("symtab not found"));
        return NULL;
    }
    return pPrv->lookUpEntryDeep(pszId, NULL);
}


const CG_SymEntry* CG_CIECFront::lookUpCONST(
    const TCHAR* pszId
)
{
    int                i;
    const CG_SymEntry* pEntry;

    for(i=0;i<m_globalConstTable.getUsed();i++)
    {
        pEntry = m_globalConstTable.get(i)->lookUpEntry(pszId);
        if(pEntry)
            return pEntry;
    }
    return NULL;
}

const CG_KeyValPair* CG_CIECFront::lookUpAttrib(
    const CG_AttribList* pAttrlist,
    const TCHAR*         pszKey
)
{
    for(int i = pAttrlist ? pAttrlist->nAttribs-1 : -1; i>=0; i--)
    {
        if(!lstrcmpi(pszKey, pAttrlist->pairs[i].pszKey))
            return &pAttrlist->pairs[i];
    }
    return NULL;
}











/*
 * ST_LookUpOperatorDefinition looks for the
 * generalized parametrization definition of a function
 * INPUT:  op = which operator
 *         isIlMode  string representation is different between ST and IL
 * OUTPUT: pDef
 * RETURN: const pointer to (static) ST_FUNCTION_DEF_TYP
 *         0 otherwise
 */
const ST_FUNCTION_DEF_TYP* ST_LookUpOperatorDefinition(CG_EXPRTYP op, bool isIlMode)
{
/* OR,XOR,AND:
 *  - ANY_BIT       <- ANY_BIT,         ANY_BIT
 *
 * NOT:
 *  - ANY_BIT       <- ANY_BIT
 *
 * EQ,NE,LT,LE,GT,GE:
 *  - BOOL          <- ANY_BIT,         ANY_BIT
 *  - BOOL          <- ANY_NUM,         ANY_NUM
 *  - BOOL          <- ANY_DATE,        ANY_DATE
 *  - BOOL          <- STRING,          STRING
 *  - BOOL          <- TIME,            TIME
 *  - BOOL          <- ENUM,            ENUM
 * ====================================================
 *  - BOOL          <- ANY_SIMPLE|ENUM, ANY_SIMPLE|ENUM
 *
 * ADD:
 *  - TIME          <- TIME,            TIME
 *  - TOD           <- TOD,             TIME
 *  - DT            <- DT,              TIME
 *  - ANY_NUM  	    <- ANY_NUM,         ANY_NUM
 * ====================================================
 *  - TIME|TOD|DT   <- TIME|TOD|DT,     TIME
 *  - ANY_NUM       <- ANY_NUM,         ANY_NUM
 *
 * SUB:
 *  - TIME          <- TIME,            TIME
 *  - TIME          <- DATE,            DATE
 *  - TOD           <- TOD,             TIME
 *  - TIME          <- TOD,             TOD
 *  - DT            <- DT,              TIME
 *  - TIME          <- DT,              DT
 *  - ANY_NUM       <- ANY_NUM,         ANY_NUM
 * ====================================================
 *  - TIME|TOD|DT   <- TIME|TOD|DT,     TIME
 *  - TIME          <- ANY_DATE,        ANY_DATE
 *  - ANY_NUM       <- ANY_NUM,         ANY_NUM
 *
 * MUL,DIV:
 *  - TIME          <- TIME,            ANY_NUM
 *  - ANY_NUM       <- ANY_NUM,         ANY_NUM
 *
 * MOD:
 *  - ANY_INT       <- ANY_INT,         ANY_INT
 *
 * POW:
 *  - ANY_REAL      <- ANY_REAL,        ANY_NUM
 *
 * UMINUS:
 *  - SINT          <- SINT
 *  - INT           <- INT
 *  - DINT          <- DINT
 *  - LINT          <- LINT
 *  - ANY_REAL      <- ANY_REAL
 *  - TIME          <- TIME
 * ====================================================
 *  - ANY_SIGNED    <- ANY_SIGNED
 *
 */

    static const ST_TypeSet_C types[] = {
        /* "0" */ CG_TBS_BOOL,
        /* "1" */ CG_TBS_ANY_BIT,
        /* "2" */ CG_TBS_ANY_REAL,
        /* "3" */ CG_TBS_ANY_INT,
        /* "4" */ CG_TBS_ANY_NUM,
        /* "5" */ CG_TBS_TIME,
        /* "6" */ CG_TBS_TIME|CG_TBS_TOD|CG_TBS_DT,
        /* "7" */ CG_TBS_ANY_DATE,
        /* "8" */ CG_TBS_ANY_SIGNED,
        /* "9" */ CG_TBS_ANY_ELEMENTARY|CG_TBS_ENUM,
        /* "A" */ CG_TBS_ANY,
        /* "B" */ CG_TBS_VOID
    };
    
    
    static const char *const overloads[] = {
        /* ST_UNDEF*/   "",
        /* ST_NOT */    "1=1",
        /* ST_UMINUS */ "8=8",
        /* ST_NOOP */   "A=A",

        /* ST_OR */     "1=11",
        /* ST_XOR */    "1=11",
        /* ST_AND */    "1=11",
        /* ST_EQ */     "0=99",
        /* ST_NE */     "0=99",
        /* ST_LT */     "0=99",
        /* ST_LE */     "0=99",
        /* ST_GT */     "0=99",
        /* ST_GE */     "0=99",
        /* ST_ADD */    "6=65|4=44",
        /* ST_SUB */    "6=65|5=77|4=44",
        /* ST_MUL */    "5=54|4=44",
        /* ST_DIV */    "5=54|4=44",
        /* ST_MOD */    "3=33",
        /* ST_POW */    "2=24",
        /* ST_MOVE */   "B=A<A",
    };


	assert(op>=CG_EXPRTYP_UNDEF && op<=CG_MOVE);

    if(isIlMode) //d-2201, HACK IL has other names for the operators
                 //example: CG_MOVE in IL 'ST' in ST ':='
                 //since 'DefinitionPrepared' is static we use two 
                 //identical DefinitionPrepared_xx with xx=li or st
    {
	    if(!DefinitionsDone_il[op])
	    {
            if(!ST_InitFunctionDef(IL_ExprTypToString(op), 
                                   types, sizeof(types)/sizeof(types[0]),
			   				       overloads[op], &(DefinitionPrepared_il[op])))
            {
                return NULL;
            }
   		    DefinitionsDone_il[op] = true;
	    }
	    return &(DefinitionPrepared_il[op]);
    }
    else
    {
	    if(!DefinitionsDone_st[op])
	    {
            if(!ST_InitFunctionDef(ST_ExprTypToString(op), 
                                   types, sizeof(types)/sizeof(types[0]),
			   				       overloads[op], &(DefinitionPrepared_st[op])))
            {
                return NULL;
            }
   		    DefinitionsDone_st[op] = true;
	    }
	    return &(DefinitionPrepared_st[op]);
    }
}

























static int ST_InitFunctionDef(
    const TCHAR*         pszId, 
    const ST_TypeSet_C  *lpTypes,
    int                  nTypes,
    const char          *szDefStr,
    ST_FUNCTION_DEF_TYP *pDef
)
{
  int i, ov, offset;
  pDef->pszId = pszId;
  pDef->nOverloads = 1;
  pDef->nArgTypes = 0;
  pDef->lpArgs = NULL;
  pDef->lpTypes = NULL;

  if (!szDefStr[0])
    goto error_return;

  for (i = 0; szDefStr[i]; i++)
    {
      if (szDefStr[i] == '|')
        pDef->nOverloads++;
      else if (isalnum( szDefStr[i] ))
        pDef->nArgTypes++;
    }

  //if (pDef->nOverloads == 0)
  //  goto error_return;

  pDef->lpArgs = new int[4*pDef->nOverloads+pDef->nArgTypes];
  if (!pDef->lpArgs)
    goto error_return;

  pDef->lpVarArg   = pDef->lpArgs + 1*pDef->nOverloads;
  pDef->lpVarOuts  = pDef->lpArgs + 2*pDef->nOverloads;
  pDef->lpOffset   = pDef->lpArgs + 3*pDef->nOverloads;
  pDef->lpArgTypes = pDef->lpArgs + 4*pDef->nOverloads;

  pDef->nTypes = nTypes;
  if ((pDef->lpTypes = new ST_TypeSet_C[nTypes])==NULL)
    goto error_return;  
  for (i=0; i<nTypes; i++)
    pDef->lpTypes[i] = lpTypes[i];


  for (ov=0, offset = 0; ov<pDef->nOverloads; ov++)
    {
      pDef->lpOffset[ov] = offset;
      pDef->lpVarArg[ov] = 0;
      pDef->lpVarOuts[ov] = 0;
      pDef->lpArgs[ov] = -1;

      do {
        int c=*(szDefStr++);
        if (isalnum(c))
          {
            if (isdigit(c))
              pDef->lpArgTypes[offset] = ((c) & 15);        /* '0' -> 0, '1' -> 1 usw. */
            else
              pDef->lpArgTypes[offset] = (((c) & 31) + 9);  /* 'A' -> 10, 'B' -> 11 usw. */
            if (pDef->lpArgTypes[offset]>=nTypes)
                goto error_return;
            offset++;
            pDef->lpArgs[ov]++;
            if (pDef->lpVarOuts[ov]<0)
                pDef->lpVarOuts[ov]--;
          }
        else if (c == '*' || c == '+')
          {
            if (pDef->lpVarArg[ov] || pDef->lpArgs[ov]<=0)
                goto error_return;
            pDef->lpVarArg[ov] = pDef->lpArgs[ov];
          }
        else if (c == '<')
          {
            if (pDef->lpVarOuts[ov] || pDef->lpArgs[ov]<=0)
                goto error_return;
            pDef->lpVarOuts[ov] = pDef->lpArgs[ov];
          }
        else if (c == '>')
          {
            if (pDef->lpVarOuts[ov] || pDef->lpArgs[ov]<0)
                goto error_return;
            pDef->lpVarOuts[ov] = -1;
          }
        else if (c == '|' || c == '\0')
          {
            break;
          }
        else if (c != '=' && c != ' ')
          {
            goto error_return;
          }

      } while (1);

      if (pDef->lpArgs[ov]<0)
        goto error_return;
      if (pDef->lpVarOuts[ov]<0)
        pDef->lpVarOuts[ov]++;
    }


  return 1;

error_return:
  if (pDef->lpArgs)
    delete [] pDef->lpArgs;
  if (pDef->lpTypes)
    delete [] pDef->lpTypes;
  return 0;
}
/*
 * ST_FreeFunctionDefinition frees memory associatad with
 * pDef
 * INPUT pDef = an initialized structure
 * NOTE: pDef is not freed itself
 * because it is supposed to be an automatic variable
 */
static void ST_FreeFunctionDefinition(ST_FUNCTION_DEF_TYP    *pDef)
{
  assert(pDef);
  delete []pDef->lpTypes;
  pDef->lpTypes = NULL;
  delete []pDef->lpArgs;
  pDef->lpArgs= NULL;
}

/*
 * ST_FreeAllOperatorDefinitions frees memory associatad with
 * static DefinitionPrepared
 */
static void ST_FreeAllOperatorDefinitions()
{
	for(int i=CG_NOT;i<=CG_MOVE;i++)
    {
		if(DefinitionsDone_il[i])
        {
			ST_FreeFunctionDefinition(&(DefinitionPrepared_il[i]));
            DefinitionsDone_il[i] = false;
        }
		if(DefinitionsDone_st[i])
        {
			ST_FreeFunctionDefinition(&(DefinitionPrepared_st[i]));
            DefinitionsDone_st[i] = false;
        }
    }
}





