#include <stdio.h>
#include "fc_todebug\fc_assert.h"
#include "CG_IECFront.h"  //COM interface base
#include "CG_IECFront_i.c"  //COM interface GUID defines

#include "fc_tools\fc_registry.h"
#include "fc_tools\fc_fileIO.h"
#include "cgt_misc.h"
#include "cgt_link.h"
#include "XREFgen.h"
#include "helper.h"
#include "utilif.h"
#include "CG_CIECFront.h" 
#include "expr.h"
#include "configuration.h"
#include "parsers.h"
#include "st_ParseContext.h"
#include "CG_Messages.h"
#include "CG_IECFilter.h"
#include "CG_IECFilter_i.c"
#include "CG_4CLAccess_i.c"

//global data:
static int       g_dwRefCount=0;
static HINSTANCE g_hinstDLL;
static TCHAR     g_szModulePath[MAX_PATH];    //full module path without file name


const TCHAR* GetModulePath()
{
    assert(g_szModulePath[0]);
    return g_szModulePath;
}
HINSTANCE GetHInstance()
{
    return g_hinstDLL;
}

class COM_CExtBodyInfo : public ICG_ExtBodyInfo
{
    DWORD                      m_dwRefCount;

    ICG_IECFront*              m_pIFront;
    const CG_Symtab*           m_pSym;

    FC_CHashPtr
        <const CG_SymEntry*, 
        FC_CGArray<CG_Edipos>*> m_varExtWrtList;
    ACCESS_TREE*                m_pSMARefTree;
    FC_CHashPtr
        <const CG_Symtab*,
         COM_CExtBodyInfo*>*    m_pCurrLoadedBdyInf;
public:


    COM_CExtBodyInfo(
        ICG_IECFront*            pIFront,
        FC_CHashPtr<
           const CG_Symtab*,
           COM_CExtBodyInfo*>*   pCurrLoadedBdyInf,
        const CG_Symtab*         pSym
    )
    {
        bool  bOk;
        m_dwRefCount        = 0;
        m_pIFront           = pIFront;
        m_pSym              = pSym;
        m_pCurrLoadedBdyInf = pCurrLoadedBdyInf;
        m_pSMARefTree       = NULL;

        //as long as the brw info is alive the front end is needed too.
        m_pIFront->AddRef();

        bOk = m_pCurrLoadedBdyInf->insert(m_pSym, this);
        assert(bOk);
    }
    void createInfoFromBody(const ST_CBody* pBody)
    {
        //fill the info from the body expr list:
        //call only once diretly after new this.
        assert(m_varExtWrtList.countPairs()==0);
        pBody->m_exprMem.getVarExtWrtList(&m_varExtWrtList);
        assert(!m_pSMARefTree);
        m_pSMARefTree = CreateSMAHash(pBody);
    }


    ~COM_CExtBodyInfo()
    {
        m_pIFront->Release();
        bool bOk = m_pCurrLoadedBdyInf->remove(m_pSym);
        assert(bOk);

        m_varExtWrtList.deletePtrs();
        if(m_pSMARefTree)
            FreeSMAHash(m_pSMARefTree);
    }


//-------------------- IUnknown methods --------------------:
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppObject)
    {
	    *ppObject = NULL;
        if (riid==IID_IUnknown || riid==IID_ICG_ExtBodyInfo) 
            *ppObject=this;
	    else 
		    return E_NOINTERFACE;
	    AddRef();
	    return NO_ERROR;
    }
    ULONG STDMETHODCALLTYPE AddRef()
    {
	    g_dwRefCount++;
	    m_dwRefCount++;
	    return m_dwRefCount;
    }
    ULONG STDMETHODCALLTYPE Release()
    {
	    g_dwRefCount--;
	    m_dwRefCount--;
	    if (m_dwRefCount==0) 
        {
		    delete this;
		    return 0;
        }
	    return m_dwRefCount;
    }


    //interface :
    long STDMETHODCALLTYPE getVarExternalWrites(
        const CG_SymEntry* pEntry, 
        const CG_Edipos**  ppEdpArray
    )
    {
        long                         lCount;
        const FC_CGArray<CG_Edipos>* pEdpList;

        *ppEdpArray = NULL;
        lCount = 0;

        //assert: why trying to call with entry from other symtab ?
        if(m_pSym != pEntry->pParent)
        {
            assert(!"not an entry of this POU");
            return -1;
        }

        pEdpList = m_varExtWrtList.get(pEntry);
        if(pEdpList)
        {
           lCount = pEdpList->getUsed();
           assert(lCount>0);
           *ppEdpArray = pEdpList->getCElements();
        }

        return lCount;
    }
    
    virtual boolean STDMETHODCALLTYPE getSimpleMemberWriteInfo(
        long                     nPath,
        const CG_SymEntry*const* pPath/*[nPath]*/
    )
    {
        if(!m_pSMARefTree)
        {
            assert(!"getSimpleMemberWriteInfo: hash not allocated");
            return false;
        }
        assert(nPath>=1 && pPath && pPath[0]->pParent==m_pSym);

        return GetsSMAWritten(m_pSMARefTree, nPath, pPath);
    }

};




class COM_CConfig : public ICG_Config
{
    DWORD                    m_dwRefCount;
    ICG_IECFront*            m_pIFront;
    ST_CConfig*              m_pConfig;
public:


    COM_CConfig(
        ICG_IECFront*   pIFront,
        ST_CConfig*     pConfig
    )
    {
        m_dwRefCount     = 0;
        m_pIFront        = pIFront;

        //register the body in the loaded bdy list:
        //as long as the brw info is alive the front end is needed too.
        m_pIFront->AddRef();
        assert(pConfig);
        m_pConfig = pConfig;
    }
    ~COM_CConfig()
    {
        delete(m_pConfig);
        m_pIFront->Release();
    }

//-------------------- IUnknown methods --------------------:
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppObject)
    {
	    *ppObject = NULL;
        if (riid==IID_IUnknown || riid==IID_ICG_Config) 
            *ppObject=this;
	    else 
		    return E_NOINTERFACE;
	    AddRef();
	    return NO_ERROR;
    }
    ULONG STDMETHODCALLTYPE AddRef()
    {
	    g_dwRefCount++;
	    m_dwRefCount++;
	    return m_dwRefCount;
    }
    ULONG STDMETHODCALLTYPE Release()
    {
	    g_dwRefCount--;
	    m_dwRefCount--;
	    if (m_dwRefCount==0) 
        {
		    delete this;
		    return 0;
        }
	    return m_dwRefCount;
    }


    const TCHAR* STDMETHODCALLTYPE getName(void)
    {
        return m_pConfig->getName();
    }
    const CG_Edipos*  STDMETHODCALLTYPE getEdp(void)
    {
        return m_pConfig->getEdp();
    }
    const CG_AttribList* STDMETHODCALLTYPE getAttribs(void)
    {
        return m_pConfig->getAttribs();
    }
    const __int64 STDMETHODCALLTYPE getFileTime(void)
    {
        return m_pConfig->getFileTime();
    }

    const CG_ResImpInfo* STDMETHODCALLTYPE getResImpByName(
        const TCHAR* pszResName
    )
    {
        return m_pConfig->getResImpByName(pszResName);
    }
};











class COM_CBody : public ICG_Body
{
    DWORD                    m_dwRefCount;

    ST_CBody*                m_pSTBody;
    const CG_Symtab*         m_pSym;
    ICG_IECFront*            m_pIFront;
    FC_CHashStr<COM_CBody*>* m_pCurrLoadedBdy;//the delete(this) detach from
    FC_CHashPtr<
       const CG_Symtab*,
       COM_CExtBodyInfo*>*   m_pCurrLoadedBdyInf;//need to create bdy info

public:
    COM_CBody(
        ST_CBody*                pSTBody, 
        const CG_Symtab*         pSym,
        ICG_IECFront*            pIFront,
        FC_CHashStr<COM_CBody*>* pCurrLoadedBdy,
        FC_CHashPtr<
           const CG_Symtab*,
           COM_CExtBodyInfo*>*   pCurrLoadedBdyInf
    )
    {
    	bool bOk;

        m_dwRefCount     = 0;
        m_pSTBody        = pSTBody;
        m_pSym           = pSym;
        m_pIFront        = pIFront;
        m_pCurrLoadedBdy = pCurrLoadedBdy;
        m_pCurrLoadedBdyInf = pCurrLoadedBdyInf;
        assert(pCurrLoadedBdyInf);

        //register the body in the loaded bdy list:
        bOk = m_pCurrLoadedBdy->insert(pSym->pszId, this);
        assert(bOk);
        //as long as the body is alive the front end is needed too.
        m_pIFront->AddRef();
    }
    ~COM_CBody()
    {
    	bool bOk;
        bOk = m_pCurrLoadedBdy->remove(m_pSym->pszId);
        assert(bOk);
        delete(m_pSTBody);
        m_pIFront->Release();
    }

//-------------------- IUnknown methods --------------------:
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppObject)
    {
	    *ppObject = NULL;
        if (riid==IID_IUnknown || riid==IID_ICG_Body) 
            *ppObject=this;
	    else 
		    return E_NOINTERFACE;
	    AddRef();
	    return NO_ERROR;
    }
    ULONG STDMETHODCALLTYPE AddRef()
    {
	    g_dwRefCount++;
	    m_dwRefCount++;
	    return m_dwRefCount;
    }
    ULONG STDMETHODCALLTYPE Release()
    {
	    g_dwRefCount--;
	    m_dwRefCount--;
	    if (m_dwRefCount==0) 
        {
		    delete this;
		    return 0;
        }
	    return m_dwRefCount;
    }


    //get header for convenience...
    const CG_Symtab* STDMETHODCALLTYPE getHeader( void)
    {
        return m_pSym;
    }
    
    boolean STDMETHODCALLTYPE isSFC( void)
    {
        return (m_pSym->flags & CG_STAB_BDY_IS_SFC)!=0;
    }
    
    const CG_Edipos* STDMETHODCALLTYPE edpBeg(void)
    {
        return &m_pSTBody->m_edpBeg;
    }

    const CG_Edipos* STDMETHODCALLTYPE edpEnd(void)
    {
        return &m_pSTBody->m_edpEnd;
    }

    const CG_STStmt* STDMETHODCALLTYPE getSTStmts( void)
    {
        return m_pSTBody->getStmts();
    }
    
    //if sfc:
    const CG_SFCStruct* STDMETHODCALLTYPE getSFCStruct( void)
    {
        return m_pSTBody->getSFCStruct();
    }
    const CG_STStmt*    STDMETHODCALLTYPE getAction(long nAction)
    {
        return m_pSTBody->getAction(nAction);
    }
    const CG_STStmt*  STDMETHODCALLTYPE getTransi(long nTransi)
    {
        return m_pSTBody->getTransi(nTransi);
    }
    const CG_STStmt*  STDMETHODCALLTYPE getExceptionAction()
    {
        return m_pSTBody->getExceptionAction();
    }

    long STDMETHODCALLTYPE getUsedTriggers(void)
    {
        return m_pSTBody->getUsedTriggers();
    }

    ICG_ExtBodyInfo* STDMETHODCALLTYPE getExtBodyInfo(void)
    {
        COM_CExtBodyInfo*  pBdyInf;

        pBdyInf = m_pCurrLoadedBdyInf->get(m_pSym);
        if(!pBdyInf)
        {
            pBdyInf = new COM_CExtBodyInfo(m_pIFront, m_pCurrLoadedBdyInf, m_pSym);
            pBdyInf->createInfoFromBody(m_pSTBody);
        }
        pBdyInf->AddRef();
        return pBdyInf;
    }
};











class COM_CExpr : public ICG_Expr
{
    DWORD            m_dwRefCount;
    st_ParseContext* m_pfc;
    ST_Expression_C* m_pExpr;
    ST_CExprMem      m_exprmem;
public:
    COM_CExpr(st_ParseContext* pfc, ST_Expression_C* pExpr)
    {
    	m_dwRefCount= 0;
        m_pfc   = pfc;
        m_pExpr = pExpr;
    }
    ~COM_CExpr()
    {
        delete(m_pExpr);//order important !
        delete(m_pfc); //order important !
    }

//-------------------- IUnknown methods --------------------:
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppObject)
    {
	    *ppObject = NULL;
        if (riid==IID_IUnknown || riid==IID_ICG_Expr) 
            *ppObject=this;
	    else 
		    return E_NOINTERFACE;
	    AddRef();
	    return NO_ERROR;
    }
    ULONG STDMETHODCALLTYPE AddRef()
    {
	    g_dwRefCount++;
	    m_dwRefCount++;
	    return m_dwRefCount;
    }
    ULONG STDMETHODCALLTYPE Release()
    {
	    g_dwRefCount--;
	    m_dwRefCount--;
	    if (m_dwRefCount==0) 
        {
		    delete this;
		    return 0;
        }
	    return m_dwRefCount;
    }





    const CG_ExprTree* STDMETHODCALLTYPE getExpr(CG_DTYPES want)
    {
        CG_BITSET ltype = ST_DTypesToTBS(want);//TODO check for supported type?
        if(m_pExpr->checkTypedExpr(ltype, false))
        {
            m_pExpr->fixType();
            return m_pExpr->makeCGExpr(&m_exprmem, NULL);
        }
        else
        {
            return NULL;
        }
    }
    boolean  STDMETHODCALLTYPE isLvalue(void)
    {
        return m_pExpr->is_Lvalue();
    }
};





























class COM_CIECFront : public ICG_IECFront
{
    // Interfaces
    DWORD                   m_dwRefCount;
    CG_CIECFront            m_inst;
    CGT_CDiagMsg            m_dm;
public:
    FC_CHashStr<COM_CBody*>        m_currLoadedBdy;
    FC_CHashPtr<const CG_Symtab*,
                COM_CExtBodyInfo*> m_currLoadedBdyInf;


    COM_CIECFront() 
        : m_dm(MODULE_USER_NAME, g_hinstDLL)
        , m_inst(&m_dm)
        , m_currLoadedBdy(true)
    {
        m_inst.m_myCOMIF = this;
        m_dm.setModuleName(MODULE_USER_NAME);
        m_dwRefCount=0;
    }
    ~COM_CIECFront()
    {
        assert(m_currLoadedBdy.countPairs()==0);
    }



//-------------------- IUnknown methods --------------------:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppObject)
    {
	    *ppObject = NULL;
        if (riid==IID_IUnknown || riid==IID_ICG_IECFront) 
        {
            *ppObject=this;
        }
	    else 
        {
		    return E_NOINTERFACE;
        }
	    AddRef();
	    return NO_ERROR;
    }
    virtual ULONG STDMETHODCALLTYPE AddRef()
    {
	    g_dwRefCount++;
	    m_dwRefCount++;
	    return m_dwRefCount;
    }
    virtual ULONG STDMETHODCALLTYPE Release()
    {
	    g_dwRefCount--;
	    m_dwRefCount--;
	    if (m_dwRefCount==0) 
        {
		    delete this;
		    return 0;
        }
	    return m_dwRefCount;
    }

    
    
//-------------------- ICG_IECFront custom methods --------------------:
//--- module initialisation:
    boolean STDMETHODCALLTYPE loadKAD( 
        /* [string][in] */ const TCHAR* pszKADFileAndPath)
    {
        return m_inst.loadKAD(pszKADFileAndPath);
    }
    boolean STDMETHODCALLTYPE loadFeatures(
        /*[in]*/ const CG_Features* pFeatureSet
    )
    {
        m_inst.m_features.loadFeatures(&m_dm, pFeatureSet);

        if(pFeatureSet && pFeatureSet->pszTargetTyp)
        {
            //change the diag message module name, we are now running 
            //in a target type scope:
            m_dm.setModuleName(pFeatureSet->pszTargetTyp);
            m_dm.setNotSupportedName(pFeatureSet->pszTargetTyp);
        }
        else
        {
            m_dm.setModuleName(MODULE_USER_NAME);
            m_dm.setNotSupportedName(NULL);
        }

        return true;
    }
    boolean STDMETHODCALLTYPE load4CP( 
        /* [string][in] */ const TCHAR* psz4CPFileAndPath)
    {
        return m_inst.load4CP(psz4CPFileAndPath);
    }




//--- KAD info service, requires prior call to loadKAD:
    virtual const TCHAR* STDMETHODCALLTYPE KADgetFile(void)
    {
        return m_inst.m_kad.getFile();
    }
    virtual int STDMETHODCALLTYPE KADCountTargetTypes(void)
    {
        return m_inst.m_kad.countTargetTypes();
    }
    virtual const TCHAR* STDMETHODCALLTYPE KADgetTargetTypeExts(void)
    {
        return m_inst.m_kad.getTargetTypeExts();
    }
    virtual int STDMETHODCALLTYPE KADgetTargetTypeByExt(
        const TCHAR*  pszExt,
        const TCHAR** ppsz,  
        GUID*         puuidTrgInfo, 
        boolean*      pbMultipleTrgs
    )
    {
        return m_inst.m_kad.getTargetTypeByExt(pszExt, ppsz, puuidTrgInfo, pbMultipleTrgs);
    }
    virtual const TCHAR* STDMETHODCALLTYPE  KADgetTargetTypeByNum(
        int           iTargetType,
        const TCHAR** ppsz,
        GUID*         puuidTrgInfo,
        boolean*      pbMultipleTrgs
    )
    {
        return m_inst.m_kad.getTargetTypeByNum(iTargetType, ppsz, puuidTrgInfo, pbMultipleTrgs);
    }
    virtual const TCHAR* STDMETHODCALLTYPE KADgetSourceExts(void)
    {
        return m_inst.m_kad.getSourceExts();
    }
    virtual CG_SOURCETYPES STDMETHODCALLTYPE KADgetSourceTypeByExt(
        const TCHAR* pszExt
    )
    {
        return m_inst.m_kad.getSourceTypeByExt(pszExt);
    }
    virtual boolean STDMETHODCALLTYPE KADgetFilterByExt(
        const TCHAR* pszExt,    //source file extension to search for
        GUID*        puuidFilter//if not NULL receives the clsid of the 
                                //corresponding KAD's IECFILTER clsid.
    )
    {
        return m_inst.m_kad.getFilterByExt(pszExt, puuidFilter);
    }



//--- 4cp file's info service, requires prior call to load4CP:
    const TCHAR* STDMETHODCALLTYPE FCPgetDiagFileName(void)
    {
        return m_inst.m_4CP.getDiagFileName();
    }
    const TCHAR*const* STDMETHODCALLTYPE FCPgetSectionNames()
    {
        return m_inst.m_4CP.getSectionNames();
    }
    const CG_AttribList* STDMETHODCALLTYPE FCPgetSection(
        /*[string][in]*/ const TCHAR* pszSec
    )
    {
        return m_inst.m_4CP.getSection(pszSec);
    }
    const CG_POU4CPInfo* STDMETHODCALLTYPE FCPgetCONinfo(
        const TCHAR* pszCON
    )
    {
        return m_inst.m_4CP.getCONinfo(pszCON);
    }
    const CG_POU4CPInfo* STDMETHODCALLTYPE FCPgetTARinfo(
        /*[string][in]*/const TCHAR* pszTAR
    )
    {
        return m_inst.m_4CP.getTARinfo(pszTAR);
    }
    const CG_POU4CPInfo* STDMETHODCALLTYPE FCPgetPOUinfo(
        /*[string][in]*/const TCHAR* pszPOU
    )
    {
        return m_inst.m_4CP.getPOUinfo(pszPOU);
    }
    const CG_LIBContext* STDMETHODCALLTYPE FCPgetLIBinfo(
        /*[string][in]*/const TCHAR* pszLIB
    )
    {
        return m_inst.m_4CP.getLIBinfo(pszLIB);
    }
    const CG_LIBContext*const* 
         STDMETHODCALLTYPE FCPgetAllLIBs(void)
    {
        return m_inst.m_4CP.getAllLIBs();
    }
    const CG_POU4CPInfo*const* 
         STDMETHODCALLTYPE FCPgetAllCSTs(void)
    {
        return m_inst.m_4CP.getAllCSTs();
    }
    const CG_POU4CPInfo*const* 
         STDMETHODCALLTYPE FCPgetAllPOUs(void)
    {
        return m_inst.m_4CP.getAllPOUs();
    }


    long STDMETHODCALLTYPE FCPgetPOUDefFileLocator(
        const TCHAR* pszPOUName,
        const TCHAR* pszTrgType,
        TCHAR        szBuffer[/*nBuffer*/],
        long         nBuffer,
        boolean      bWarnDuply
    )
    {
        long                 lRet;
        FC_CString           loc(MAX_PATH);
        const CG_POU4CPInfo *pPOUinf;
        const CG_LIBContext *pLIBinf;


        if(nBuffer<1)
            return -1;

        lRet = m_inst.findPOUDefByIECName(
                        pszPOUName, 
                        pszTrgType,
                        &pPOUinf,
                        &pLIBinf,
                        &loc,
                        bWarnDuply!=0
                      );

        if(lRet==-1)
        {
            assert(pszTrgType && loc.len()>0);
            return FC_StringCopyToBuffer(szBuffer, nBuffer, loc) ? 2 : -1;
        }
        else if(lRet==0)
        {
            szBuffer[0] = 0;
            return 0;
        }
        else if(lRet==1)
        {
            return FC_StringCopyToBuffer(szBuffer, nBuffer, loc) ? 1 : -1;
        }

        CGT_IERR(&m_dm, _T("unexpected return from findPOUDefByIECName"));
        szBuffer[0] = 0;
        return 0;
     }



    const TCHAR* STDMETHODCALLTYPE FCPgetGenPath( void)
    {
        return m_inst.m_4CP.getGenPath();
    }
    const TCHAR* STDMETHODCALLTYPE FCPgetPrjPath( void)
    {
        return m_inst.m_4CP.getPrjPath();
    }
    const TCHAR* STDMETHODCALLTYPE FCPgetPrjName( void)
    {
        return m_inst.m_4CP.getPrjName();
    }
    const TCHAR* STDMETHODCALLTYPE FCPgetStandardLibPath( void)
    {
        return CGT_GetSTDLibPath(&m_dm);
    }
    const TCHAR* STDMETHODCALLTYPE FCPgetInstallationPath( void)
    {
        return CGT_GetEngBinDir(&m_dm);
    }





//--- load source files 
    boolean STDMETHODCALLTYPE loadLIBs(void)
    {
        int                        nOldErr;    
        const CG_LIBContext*const* ppInf;

        nOldErr = m_dm.getTotalErrors();

        for(ppInf = m_inst.m_4CP.getAllLIBs(); *ppInf; ppInf++)
        {
            if(m_inst.loadLib(*ppInf))
                m_inst.loadLIBCSTs(*ppInf);
            else
                assert(m_dm.hasErr());
        }
        
        return nOldErr == m_dm.getTotalErrors();;
    }
    
    boolean STDMETHODCALLTYPE loadCSTs(void)
    {
        long                       nOldErr;
        const ST_CSymtabPrv*       pSym;
        bool                       bRecursive;
        const CG_POU4CPInfo*const* ppInf;

        nOldErr = m_dm.getTotalErrors();

        for(ppInf = m_inst.m_4CP.getAllCSTs(); *ppInf; ppInf++)
        {
            pSym = m_inst.loadCST((*ppInf)->pszFile, true, &bRecursive);
            if(!pSym)
            {
                m_dm.msg1(CG_E_FILE_NOT_EXIST, 
                  &(*ppInf)->edp, (*ppInf)->pszFile);
            }
        }

        return m_dm.getTotalErrors()==nOldErr;
    }


    ICG_Config* STDMETHODCALLTYPE loadConfig(
        const CG_POU4CPInfo* pCON
    )
    {
        ST_CConfig*  pConfig;
        ICG_Config*  pIConfig;

        pIConfig = NULL;
        pConfig = m_inst.loadConfig(pCON);
        if(pConfig)
        {
            pIConfig = new COM_CConfig(this, pConfig);
            pIConfig->AddRef();
        }

        return pIConfig;
    }

    const CG_Symtab* STDMETHODCALLTYPE   loadHeader(
        const TCHAR*     pszPOUName
    )
    {
        bool                 bRecursive;
        const ST_CSymtabPrv* pSymPrv;

        pSymPrv = m_inst.loadHeader(pszPOUName, &bRecursive);
        return pSymPrv ? pSymPrv->getCGSym() : NULL;
    }
    
    
    const CG_Symtab* STDMETHODCALLTYPE   loadGVL(
        const TCHAR*     pszGVLFile
    )
    {
        const ST_CSymtabPrv* pSymPrv;
        bool                 bRecursive;

        pSymPrv = m_inst.loadGVL(pszGVLFile, &bRecursive);
        return pSymPrv ? pSymPrv->getCGSym() : NULL;
    }

    const CG_Symtab* STDMETHODCALLTYPE loadCST(
        /*[in,string]*/ const TCHAR* pszCSTFile,
        /*[in]       */ boolean      bAddToGlobCSTs
    )
    {
        const ST_CSymtabPrv* pSymPrv;
        bool                 bRecursive;

        pSymPrv = m_inst.loadCST(pszCSTFile, bAddToGlobCSTs!=0, &bRecursive);
        return pSymPrv ? pSymPrv->getCGSym() : NULL;
    }




    const CG_Resource* STDMETHODCALLTYPE loadResource(
        const TCHAR*     pszResFile
    )
    {
        return m_inst.loadResource(pszResFile);
    }

    //TODO: all loadXXX have a problem with reporting bRecursive error !!
    //may be will diag as does not exist !!
    const CG_Symtab* STDMETHODCALLTYPE loadBody(
        const TCHAR*     pszPOUName,
        long             codeGenOpt,
        ICG_Body**       pBody
    )
    {
         ST_CBody*            pBdy;
         const CG_Symtab*     pSym;
         const ST_CSymtabPrv* pSymPrv;
         bool                 bRecursive;
         
         *pBody = m_currLoadedBdy.get(pszPOUName);
         if(*pBody)
         {
             (*pBody)->AddRef();
             pSym = (*pBody)->getHeader();
             assert(!lstrcmpi(pszPOUName, pSym->pszId));
         }
         else
         {
             pSymPrv = m_inst.loadBody(pszPOUName, codeGenOpt, &pBdy, &bRecursive);
             assert(!(pBdy && !pSymPrv));

             if(pSymPrv)
                 pSym = pSymPrv->getCGSym();

             if(pBdy && pSym)
             {
                pSym = pSymPrv->getCGSym();
                 *pBody = new COM_CBody(pBdy, pSym, this, 
                     &m_currLoadedBdy,
                     &m_currLoadedBdyInf
                  );
                (*pBody)->AddRef();
             }
         }
         return pSym;
    }

    const CG_MacroBody* STDMETHODCALLTYPE getNonPolyMacroBody(const CG_Symtab* pSym)
    {
        bool                bRecursive;
        ST_CSymtabPrv*      pSymPrv;
        const CG_MacroBody* pMacBdy;

        if(!(pSym->flags & CG_STAB_POLYMORH) && (pSym->flags & CG_STAB_MACRO)
            && (pSym->flags & CG_STAB_HDR_NOERR))
        {
            pSymPrv = m_inst.loadHeader(pSym->pszId, &bRecursive);
            assert(pSymPrv && !bRecursive && pSymPrv->getCGSym()==pSym);
            pMacBdy = pSymPrv->getNonPolyMacroBody();
            assert(pMacBdy);//<-should now exits?!
            
            return pMacBdy;
        }
        else
        {
            assert(!"ICG_IECFRont::getNonPolyMacroBody: bad pSym");
            return NULL;
        }
    }

    ICG_ExtBodyInfo* STDMETHODCALLTYPE loadExtBodyInfo(const CG_Symtab* pSym)
    {
        const CG_Symtab* pSym2;
        ICG_Body*        pBody;
        ICG_ExtBodyInfo* pBdyInf;
        const long       lBSetNoInf = CG_STAB_MACRO|CG_STAB_OPERATOR;

        //simply look up first:
        pBdyInf = m_currLoadedBdyInf.get(pSym);
        if(pBdyInf)
        {
            pBdyInf->AddRef();
            return pBdyInf;
        }

        //not found must load first or reload:

        //these items don't have any kind of body, and hence no ext bdy info:
        if((pSym->flags & lBSetNoInf)!=0 || !CG_HasPOUTypBody(pSym->pouTyp))
            return NULL;
        
        //POU already has error? if so return NULL:
        if((pSym->flags & CG_STAB_HDR_NOERR)==0)
        {
            assert(m_dm.hasErr());
            return NULL;
        }
        if((pSym->flags & CG_STAB_BDY_LOADED) && (pSym->flags & CG_STAB_BDY_NOERR)==0)
        {
            assert(m_dm.hasErr());
            return NULL;
        }

        pBdyInf = NULL;
        if(pSym->flags & CG_STAB_HAS_BDY)
        {
            //load & release body, only get the ext info:
            pSym2 = this->loadBody(pSym->pszId, 0, &pBody);
            assert(!pSym2|| (pSym2==pSym));
            if(!pBody)
            {
                assert(m_dm.hasErr());
                return NULL;
            }
        
            pBdyInf = pBody->getExtBodyInfo();
            pBody->Release();
            assert(pBdyInf);
        }
        else if(pSym->pLibCtx && m_inst.m_features.getTrgTyp())
        {
            //TODO remove or put usefull data to .link
            COM_CExtBodyInfo* pCOMBdyInf;
            pCOMBdyInf = new COM_CExtBodyInfo(this, &m_currLoadedBdyInf, pSym);
            (pBdyInf = pCOMBdyInf)->AddRef();
        }
        return pBdyInf;
    }


    ICG_Expr* STDMETHODCALLTYPE parseExprFromString( 
        const CG_Edipos* pEdp,
        boolean          bIncCol,
        const TCHAR*     pszExpr,
        const TCHAR*     pszPOUCtx,
        boolean          bNoWarns
    )
    {
        COM_CExpr*       pIex;
        ST_Expression_C* pExpr;
        bool             bRecursive;
        ST_CSymtabPrv*   pSymPrv;

        pIex    = NULL;
        pSymPrv = NULL;

        if(pszPOUCtx)
        {
            pSymPrv = m_inst.loadHeader(pszPOUCtx, &bRecursive);
            //TODO: consider: check if lib POU supports target type if any?
            if(bRecursive)
                m_dm.msg1(CG_E_RECURSIV, pEdp, pszPOUCtx);
            if(!pSymPrv)
                return NULL;
        }

        {
            st_ParseContext* pfc = new st_ParseContext(&m_inst);//TODO may be to slow if called very often..
            pExpr = pfc->parseSingleExpression(pEdp, bIncCol==1, pszExpr, 
                                      pSymPrv, bNoWarns==1);
            if(pExpr)
            {
                pIex = new COM_CExpr(pfc, pExpr);
                pIex->AddRef();
            }
            else
            {
                delete(pfc);
            }
        }
       
        return pIex;
    }




//--- symbol table methods: 
    I4CLAccess* STDMETHODCALLTYPE get4CLLibIF(const CG_LIBContext* pInf)
    {
        if(!m_inst.m_4CP.is4CPLoaded())
        {
            CGT_IERR(&m_dm, _T("get4CLLibIF: 4CP not loaded"));
            return NULL;
        }

        I4CLAccess* pILib = m_inst.getLibIFByPOUInfo(pInf);
        if(pILib)
        {
            pILib->AddRef();
        }
        else
        {
            CGT_IERR(&m_dm, m_dm.jot1()<<
                     _T("get4CLLibIF: lib not found lib-id: '")<<
                     pInf->pszId<<_T("'")
                 );
            
        }

        return pILib;
    }

    const CG_SymEntry* STDMETHODCALLTYPE lookUpEntry( 
        const CG_Symtab* pSym,
        const TCHAR*     pszId
    )
    {
        return m_inst.lookUpEntry(pSym, pszId);
    }
   
    const CG_SymEntry* STDMETHODCALLTYPE lookUpGlobVar( 
        const CG_Resource*   pRes,
        const TCHAR*         pszId
    )
    {
        return m_inst.lookUpEntryDeep(pRes->pGVL, pszId);
    }

    
    const CG_SymEntry* STDMETHODCALLTYPE lookUpCONST( 
        const TCHAR*  pszId
    )
    {
        return m_inst.lookUpCONST(pszId);
    }

    long STDMETHODCALLTYPE getNumPrjCST(void)
    {
        return m_inst.getGlobConstSyms()->getUsed();
    }

    const CG_Symtab* STDMETHODCALLTYPE getPrjCSTbyNum(long n)
    {
        if(n>=0 && n<m_inst.getGlobConstSyms()->getUsed())
        {
            return m_inst.getGlobConstSyms()->get(n)->getCGSym();
        }
        else
        {
            assert(!"getPrjCSTbyNum bad index");
            return NULL;
        }
    }

    const CG_KeyValPair* STDMETHODCALLTYPE lookUpAttrib(
        const CG_AttribList* pAttrlist,
        const TCHAR*         pszKey
    )
    {
        return m_inst.lookUpAttrib(pAttrlist, pszKey);
    }

    BSTR STDMETHODCALLTYPE createIECMangeledName( 
        const CG_Symtab* pSym
    )
    {
        FC_CString mg;
        if(ST_CreateIECMangeledName(pSym, mg))
        {
            return mg.toBSTR();
        }
        else
        {
            CGT_IERR2(&m_dm, &pSym->edp, 
                _T("ICG_IECFront::createIECMangeledName: invalid symtab"));
            return NULL;
        }
    }
    
    boolean STDMETHODCALLTYPE getIECHeaderText( 
        const CG_Symtab* pSym,
        long             nBuffer,
        TCHAR*           pszBuffer
    )
    {
        bool   bOk;
        TCHAR* psz;

        psz = ST_GetIECHeaderText(&m_dm, &m_inst, pSym);
        if(!psz)
            return false;

        bOk = FC_StringCopyToBuffer(pszBuffer, nBuffer, psz)!=0;
        delete(psz);
        return bOk;
    }



    const CG_TaskInit* STDMETHODCALLTYPE lookUpTaskInit(
        const CG_Task* pTask, 
        const TCHAR*   pszInit
    )
    {
        for(int i =0; i<pTask->nInits; i++)
        {
            if(!lstrcmpi(pTask->pInits[i].pszAttr, pszInit))
                return &pTask->pInits[i];
        }
        return NULL;
    }



//--- error handling:
    boolean STDMETHODCALLTYPE edpToStr( 
        /* [ref][in] */      const CG_Edipos* pEdp,
        /* [size_is][out] */ TCHAR*           pszEdp,
        /* [in] */           long             nBuf
    )
    {
        return m_dm.edpToStrBuffer(pEdp, pszEdp, nBuf, false);        
    }

    virtual BSTR STDMETHODCALLTYPE getPredefMsgFmt(
        /*[in] */HRESULT hrmsg
    )
    {
        TCHAR buffer[2024];
        DWORD   ret;

        ret= FormatMessage(
          FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_HMODULE,
          g_hinstDLL,   // message source
          hrmsg,  // message identifier
          MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // language identifier TODO 
          buffer,    // message buffer
          FC_ARRAY_LEN(buffer)-1,        // maximum size of message buffer
          NULL  // array of message inserts
        );
        
        if(ret==0)
            return NULL;

        return FC_StringToBSTR(buffer);
    }
    long STDMETHODCALLTYPE getDisplayedErrors( void)
    {
        //wants only the displayed ones:
        return m_dm.getDisplayedErrors();
    }
    long STDMETHODCALLTYPE getDisplayedWarnings( void)
    {
        return m_dm.getDisplayedWarnings();
    }
    long STDMETHODCALLTYPE getTotalErrors( void)
    {
        return m_dm.getTotalErrors();
    }

    void STDMETHODCALLTYPE userMsg(const TCHAR* pszText)
    {
        m_dm.userMsg(pszText);
    }





//--- miscellaneous helper methods:
    HRESULT STDMETHODCALLTYPE checkIdentifier(const TCHAR* pszId)
    {
        switch(UTIL_CheckIdentifier(pszId))
        {
        case E_UTIL_ID_SYNTAX:     return CG_E_IDENT_SYNTAX;
        case E_UTIL_ID_IS_KEYWORD: return CG_E_ID_IS_KEWORD;
        case E_UTIL_ID_TOO_LONG:   return CG_E_ID_TOO_LONG;
        case S_OK:                 return S_OK;
        default:
            assert(!"unexpected return from UTIL_CheckIdentifier");
            return CG_E_IDENT_SYNTAX;
        }
    }

    const CG_Literal* STDMETHODCALLTYPE getDefaultInit(CG_DTYPES elementary)
    {
        const CG_Literal* pLit;
        if(!CG_IsElementary(elementary))
            pLit = NULL;
        else if(elementary==CG_STRING || elementary==CG_WSTRING)
            pLit = m_inst.m_exprMem.registerStringLiteral(elementary, NULL);
        else
            pLit = m_inst.m_exprMem.registerLiteral(elementary, NULL, 0, 0.0);

        return pLit;
    }


    CG_DTYPES STDMETHODCALLTYPE getEnumFromTypeKW(const TCHAR* pszSimple)
    {
        return ST_TypeKeywordToCG_DTYPES(pszSimple);
    }


    boolean STDMETHODCALLTYPE dtypeFullEqual(
        const CG_DType* pT1, 
        const CG_DType* pT2
    )
    {
        assert(pT1 && pT2);
        return  pT1==pT2;//realy hard core? TODO this works but is ola..
                         //when changing the internal type storage
                         //this might not work!
    }

    const CG_DType* STDMETHODCALLTYPE getArrayBaseType(
        const CG_DType* pArrType 
    )
    {
        return m_inst.getArrayBaseDType(pArrType);
    }


    
    
//    (implement here??):
    long STDMETHODCALLTYPE checkSourceFile(
        const TCHAR* pszFile
    )
    {
        const TCHAR*     pszExt;
        CG_SOURCETYPES   srcTyp;    
        CG_Edipos        cmdl;
        int              iOldErr;
        ST_CBody*        pBody;
        GUID             clsid;
        HRESULT          hr;
        ICG_IECFilter*   pIFilter;
        bool             bOk;
        TCHAR            canrel[2*MAX_PATH];
        const TCHAR*     pszPrjPath;


		//enter "only check" mode:
		m_inst.m_bFileCheckMode = true;

        pszPrjPath = m_inst.m_4CP.getPrjPath();
        assert(m_inst.m_4CP.is4CPLoaded() && m_inst.m_kad.isLoaded());

        //HACK ??
        //whatever file name and path comes in convert to the intenal
        //cannonical form. That also has the effect that the diag messages
        //do not contain the project path if the source happen to be underneath 
        //the project dir:
        FC_FileFullPath(pszPrjPath, pszFile, canrel, FC_ARRAY_LEN(canrel));
        if(FC_StringStripBegin(canrel, pszPrjPath)>0)
            FC_StringStripBegin(canrel, _T("\\"));


        iOldErr = m_dm.getTotalErrors();
        CG_InitCMDLEdipos(&cmdl);
        
        pszExt = FC_StringGetFileExt(canrel);
        srcTyp = m_inst.m_kad.getSourceTypeByExt(pszExt);

        switch(srcTyp)
        {
        case CG_SRC_TYPE_UNDEF:
			//special case: diag must be done by caller
        break;
        case CG_SRC_TYPE_4CP: //check 4cp, why not ??
        break;  //since 4cp already loaded just ok ...
        case CG_SRC_TYPE_CST:
        {
            //old and persistent problem:
            //don't check a .cst if already loaded during setup phase:
            if(!m_inst.getSymPrvById(canrel))
                delete(ST_ParseAnyHeader(NULL, &m_inst, canrel, NULL));
        }
        break;
        case CG_SRC_TYPE_SDT:
        case CG_SRC_TYPE_GVL:
        case CG_SRC_TYPE_HDR:
            delete(ST_ParseAnyHeader(NULL, &m_inst, canrel, NULL));
        break;
        case CG_SRC_TYPE_4GR:
        case CG_SRC_TYPE_ST:
        case CG_SRC_TYPE_IL:
            delete(ST_ParseSTIL4GR(NULL, &m_inst, canrel, NULL, &pBody, NULL, 0));
            delete(pBody);
        break;
        case CG_SRC_TYPE_RES:
        {
            ST_CSymtabPrv*   pSymPrvNew;
            delete(ST_ParseRES(NULL, &m_inst, canrel, &pSymPrvNew));
            delete(pSymPrvNew);
        }
        break;
        case CG_SRC_TYPE_CON:
            delete(ST_ParseCON(NULL, &m_inst, canrel));
        break;
        case CG_SRC_TYPE_ADDON_FILTER:
            bOk = m_inst.m_kad.getFilterByExt(pszExt, &clsid);
            assert(bOk);

            hr = CoCreateInstance(clsid,
                                  NULL, 
                                  CLSCTX_INPROC_SERVER,
                                  IID_ICG_IECFilter, 
                                  (void**)&pIFilter);
            if(FAILED(hr))
            {
                m_dm.sysErr(hr, NULL, 
                    _T("CoCreateInstance for IID_ICG_IECFilter, source file: "),
                    canrel); 
            }
            else
            {
                FC_CString jot;
                BSTR sFileName;
                BSTR sGeneratedFile = NULL;
                BSTR sOutputDirectory;
                BSTR sExpectedClass;
                long lNumberErrors = NULL;
                long lNumberWarnings = NULL;

                sFileName = jot.load(canrel).toBSTR();
                sOutputDirectory = FC_StringToBSTR(m_inst.m_4CP.getGenPath());
                sGeneratedFile = NULL;

	            hr = pIFilter->Compile(
                    sFileName,
                    this,
                    true,
                    false, //TODO [in] BOOL bIncrementalCompileFlag,
                    sOutputDirectory,
                    &sGeneratedFile,
                    &sExpectedClass,
                    &lNumberErrors,
                    &lNumberWarnings
                );
                pIFilter->Release();

                SysFreeString(sFileName);
                SysFreeString(sOutputDirectory);

                if(FAILED(hr))
                {
                    m_dm.sysErr(hr, NULL, _T("ICG_IECFilter::Compile failed source: "),canrel); 
                }
                else
                {
                    assert(sGeneratedFile);
                    m_dm.incErrors(lNumberErrors);
                    m_dm.incWarnings(lNumberWarnings);
                }
                if(sGeneratedFile)
                {
                    jot.load(sGeneratedFile);
                    SysFreeString(sGeneratedFile);
                    checkSourceFile(jot);
                }
            }

        break;
        default: 
            assert(!"bad enum");
        }

		//leave "only check" mode:
		m_inst.m_bFileCheckMode = false;

        return srcTyp==CG_SRC_TYPE_UNDEF ? -1 : m_dm.getTotalErrors() - iOldErr;
    }


};






class COM_CIECFrontFactory : public IClassFactory 
{
// Interface
public:
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppObject)
    {
	    if (riid==IID_IUnknown || riid==IID_IClassFactory) 
        {
		    *ppObject=this;
		}
	    else 
        {
		    return E_NOINTERFACE;
		}
	    AddRef();
	    return NO_ERROR;
    }
	ULONG STDMETHODCALLTYPE AddRef()
    {
	    g_dwRefCount++;
	    m_dwRefCount++;
	    return m_dwRefCount;
    }

	ULONG STDMETHODCALLTYPE Release()
    {
	    g_dwRefCount--;
	    m_dwRefCount--;
	    if(m_dwRefCount==0) 
		    delete this;
	    return m_dwRefCount;
    }


	HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown *pUnkOuter, REFIID riid, void** ppObject)
    {
        if (pUnkOuter!=NULL)
		    return CLASS_E_NOAGGREGATION;

        if(g_szModulePath[0]==0)
        {
            if(GetModuleFileName(g_hinstDLL, g_szModulePath, FC_ARRAY_LEN(g_szModulePath))==0)
            {
	           FC_ASSERT_LASTERR(_T("GetModuleFileName"));
               return false;
            }
            FC_StringCutFilePart(g_szModulePath);
        }


        COM_CIECFront* pFront = new COM_CIECFront();
	    if(FAILED(pFront->QueryInterface(riid, ppObject))) 
        {
           delete pFront;
		   *ppObject=NULL;
		   return E_NOINTERFACE;
		}

        return NO_ERROR;
    }
	HRESULT	STDMETHODCALLTYPE LockServer(BOOL fLock)
    {
	    if(fLock)
		    g_dwRefCount++;
	    else
		    g_dwRefCount--;
	    
        return NO_ERROR;
    }

// Implementation
private:
	ULONG m_dwRefCount;
public:
	COM_CIECFrontFactory()
    {
        m_dwRefCount = 0;
    }
};





//DLL Main is needed to get the hinstDLL:
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,   // handle to the DLL module
    DWORD     fdwReason,  // reason for calling function
    LPVOID    lpvReserved // reserved
)
{
    if(fdwReason==DLL_PROCESS_ATTACH)
    {
        g_hinstDLL = hinstDLL;
    }
    return TRUE;
}
//Standard COM DLL Exports:
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppObject) 
{
	*ppObject = NULL;

	if(rclsid==CLSID_CG_IECFront) 
    {
		COM_CIECFrontFactory* pFactory = new COM_CIECFrontFactory;
		if(FAILED(pFactory->QueryInterface(riid, ppObject))) 
        {
			delete pFactory;
			*ppObject = NULL;
			return E_INVALIDARG;
        }
    }
	else 
    {   // here you could check for additional CLSID's you DLL may provide
	    return CLASS_E_CLASSNOTAVAILABLE;
	}
	return NO_ERROR;
}
STDAPI DllCanUnloadNow() 
{
   return g_dwRefCount!=0 ? S_FALSE : S_OK ;
}
STDAPI DllRegisterServer(void) 
{
    LONG         lret =ERROR_SUCCESS;
    const TCHAR* pszClass = _T("4CONTROL.CG_IECFront10");
    TCHAR        szModule[MAX_PATH+1];
    FC_CString   jot;

    if(GetModuleFileName(g_hinstDLL, szModule, FC_ARRAY_LEN(szModule))==0)
    {
	    FC_ASSERT_LASTERR(_T("GetModuleFileName"));
        lret = E_UNEXPECTED;
    }
    if(lret==ERROR_SUCCESS)
    {
        jot<<_T("CLSID\\")<<CLSID_CG_IECFront;
        lret=FC_RegSetStringVal(HKEY_CLASSES_ROOT, jot, NULL, pszClass);
    }
    if(lret==ERROR_SUCCESS)
    {
        jot<<_T("\\InprocServer32");
        lret=FC_RegSetStringVal(HKEY_CLASSES_ROOT, jot, NULL, szModule);
    }
    if(lret==ERROR_SUCCESS)
    {
        lret=FC_RegSetStringVal(HKEY_CLASSES_ROOT, jot, 
            _T("ThreadingModel"), _T("Both"));
    }

    FC_ASSERT_SYSERR(lret, _T("DllRegisterServer"));
    
    return lret==ERROR_SUCCESS ? NOERROR : E_UNEXPECTED;
}
STDAPI DllUnregisterServer(void) 
{
    LONG        lret;
    FC_CString  jot;

    jot<<_T("CLSID\\")<<CLSID_CG_IECFront;
    lret = FC_RegRecursiveDelete(HKEY_CLASSES_ROOT, jot);
    FC_ASSERT_SYSERR(lret, jot);
    return lret==ERROR_SUCCESS ? NOERROR : E_UNEXPECTED;
}

