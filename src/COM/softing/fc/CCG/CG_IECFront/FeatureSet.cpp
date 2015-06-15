
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "FeatureSet.h"
#include "CG_AutoDecl.h"
#include "helper.h"

//----  Static Initializations:   ----------------------------------*
#include "CG_AutoDecl_i.c"



CFeatureSet::CFeatureSet()
{
    m_pFeat = new FeatureSet;
    m_pCFeat= m_pFeat;
}
CFeatureSet::~CFeatureSet()
{
    assert(m_pCFeat==m_pFeat && m_pFeat->magic==0x4711);
    delete(m_pFeat);
}



void CFeatureSet::loadFeatures(
    CGT_CDiagMsg*      pdm,
    const CG_Features* pFeatureSet
)
{
    delete(m_pFeat);
    m_pFeat = new FeatureSet;
    m_pCFeat= m_pFeat;

    if(!pFeatureSet)
        return; //leave the max. features...

    if(pFeatureSet->pszTargetTyp)
        m_pFeat->trgTyp.load(pFeatureSet->pszTargetTyp);

    m_pFeat->bLadder = m_pFeat->bLadder && pFeatureSet->bLadder;
    m_pFeat->bFBD    = m_pFeat->bFBD    && pFeatureSet->bFBD;
    m_pFeat->bSFC    = m_pFeat->bSFC    && pFeatureSet->bSFC;

    //declaration (header) features:
    m_pFeat->bIncompleteAddr     = m_pFeat->bIncompleteAddr    && pFeatureSet->bIncompleteAddr;
    m_pFeat->bAssumedSizeArrays  = m_pFeat->bAssumedSizeArrays && pFeatureSet->bAssumedSizeArrays;
    m_pFeat->declModifiers       = m_pFeat->declModifiers      && pFeatureSet->declModifiers;
    m_pFeat->bMemoryDirectAddr   = m_pFeat->bMemoryDirectAddr  && pFeatureSet->bMemoryDirectAddr;


    //intersetct all type sets with the front end supported types:
    m_pFeat->allTypes         &= pFeatureSet->allTypes; 
    m_pFeat->arrayTypes       &= pFeatureSet->arrayTypes;
    m_pFeat->structMemberTypes&= pFeatureSet->structMemberTypes;
    m_pFeat->funRetTypes      &= pFeatureSet->funRetTypes;
    m_pFeat->funInputTypes    &= pFeatureSet->funInputTypes;
    m_pFeat->funOutputTypes   &= pFeatureSet->funOutputTypes;
    m_pFeat->funInOutTypes    &= pFeatureSet->funInOutTypes;
    m_pFeat->funLocalTypes    &= pFeatureSet->funLocalTypes;
    m_pFeat->FBInOutTypes     &= pFeatureSet->FBInOutTypes;
    m_pFeat->gvlTypes         &= pFeatureSet->gvlTypes;

    assert(pFeatureSet->preferedInt==16 || pFeatureSet->preferedInt==32);
    m_pFeat->preferedInt       = pFeatureSet->preferedInt;
    assert(pFeatureSet->preferedFloat==32 || pFeatureSet->preferedFloat==64);
    m_pFeat->preferedFloat     = pFeatureSet->preferedFloat;

    //allowed decl scopes:
    m_pFeat->bVarNativeFun   = pFeatureSet->bVarNativeFun;
    m_pFeat->bVarNativeFBPrg = pFeatureSet->bVarNativeFBPrg;
    m_pFeat->bVarInOutFun    = pFeatureSet->bVarInOutFun; 
    m_pFeat->bVarInOutFB     = pFeatureSet->bVarInOutFB; 
    //TODO m_pFeat->bVarTmpFun      = pFeatureSet->bVarTmpFun;
    //TODO m_pFeat->bVarTmpFBPrg    = pFeatureSet->bVarTmpFBPrg;
    //TODO m_pFeat->bVarGlobPrg     = pFeatureSet->bVarGlobPrg;
    //TODO m_pFeat->bVarExtFun      = pFeatureSet->bVarExtFun;

    //task features:
	m_pFeat->nTIPairs = pFeatureSet->nTIPairs;
	
	if(m_pFeat->psTI)
		delete[] m_pFeat->psTI;
	if(m_pFeat->pTITypes)
		delete[] m_pFeat->pTITypes;

	m_pFeat->psTI     = new FC_CStr[pFeatureSet->nTIPairs];
	m_pFeat->pTITypes = new CG_BITSET[pFeatureSet->nTIPairs];

	for(int i = 0; i < pFeatureSet->nTIPairs; i++)
	{
		m_pFeat->psTI[i].load(pFeatureSet->ppszTI[i]);
		m_pFeat->pTITypes[i] = pFeatureSet->pTITypes[i];
	}

    m_pFeat->bMacroFB   = pFeatureSet->bMacroFB;
    m_pFeat->nMaxStrLen = pFeatureSet->nMaxStrLen;

    if(pFeatureSet->pAutodecl)
    {
        m_pFeat->autodecl = *pFeatureSet->pAutodecl;
        assert(pFeatureSet->pszAutoDeclConfig);
        if(pFeatureSet->pszAutoDeclConfig)
        {
            if(FC_StringIsAbsPath(pFeatureSet->pszAutoDeclConfig))
            {
                m_pFeat->autoDeclConfigFile.load(pFeatureSet->pszAutoDeclConfig);
            }
            else
            {
                FC_CString jot(MAX_PATH);
                
                m_pFeat->autoDeclConfigFile.load(
                    jot<<CGT_GetEngBinDir(pdm)<<_T("\\")<<pFeatureSet->pszAutoDeclConfig);
            }
        }
    }
}


FeatureSet* CFeatureSet::getFeatureSet()
{
    FeatureSet* pCurr;
    
    pCurr = m_pFeat;
    m_pFeat = new FeatureSet;
    m_pCFeat= m_pFeat;

    assert(pCurr->magic == 0x4711);
    return pCurr;
}

void CFeatureSet::restoreFeatureSet(FeatureSet* pFeat)
{
    assert(pFeat->magic == 0x4711);
    delete(m_pFeat);
    m_pFeat = pFeat;
    m_pCFeat= m_pFeat;
}

 

ICG_AutoDecl* CFeatureSet::getAutoDeclIF(CGT_CDiagMsg* pdm)
{
    if(m_pFeat->autoDeclConfigFile.isEmpty())
        return NULL;

    if(!m_pFeat->pIAutoDecl)
    {
        //must first create lib access instance:
        HRESULT hr = CoCreateInstance(
                  m_pFeat->autodecl,
                  NULL, 
                  CLSCTX_INPROC_SERVER,
                  IID_ICG_AutoDecl, 
                  (void**)&(m_pFeat->pIAutoDecl)
             );

        if(FAILED(hr))
        {
            pdm->sysErr(hr, NULL, _T("CoCreateInstance for IID_ICG_AutoDecl: target type: "),  m_pFeat->trgTyp);
            return NULL;
        }
        
        //call the init method to load the definitions:
        BSTR sTrgName = FC_StringToBSTR(m_pFeat->trgTyp);
        BSTR sIniFile = FC_StringToBSTR(m_pFeat->autoDeclConfigFile);

        hr = m_pFeat->pIAutoDecl->init(sTrgName, sIniFile);  
        
        FC_FREE_BSTR(sTrgName);
        FC_FREE_BSTR(sIniFile);
        
        if(FAILED(hr))
        {
            pdm->sysErr(hr, NULL, _T("Auto declaraction initialisation failed, attemp to load file: "),  
                m_pFeat->autoDeclConfigFile);
            return NULL;
        }


    }

    return m_pFeat->pIAutoDecl;
}

FeatureSet::FeatureSet()
{
    trgTyp.load("");
    bLadder      = 1;
    bFBD         = 1;
    bSFC         = 1;
  
    bIncompleteAddr    = 1;
    bAssumedSizeArrays = 1;
    declModifiers      = 0xffffffff;
    bMemoryDirectAddr  = 1;
 
    allTypes          = CG_FRONT_END_TYPES;
    arrayTypes        = CG_FRONT_END_TYPES;
    structMemberTypes = CG_FRONT_END_TYPES;
    funRetTypes       = CG_FRONT_END_TYPES;
    funInputTypes     = CG_FRONT_END_TYPES;
    funOutputTypes    = CG_FRONT_END_TYPES;
    funInOutTypes     = CG_FRONT_END_TYPES;
    funLocalTypes     = CG_FRONT_END_TYPES;
    FBInOutTypes      = CG_FRONT_END_TYPES;
    gvlTypes          = CG_FRONT_END_TYPES;

    preferedInt       = 32;
    preferedFloat     = 64;
    
    bVarNativeFun     = 1;
    bVarNativeFBPrg   = 1;
    bVarInOutFun      = 1;
    bVarInOutFB       = 1;
    bVarTmpFun        = 0;
    bVarTmpFBPrg      = 0;
    bVarGlobPrg       = 0;
    bVarExtFun        = 0;
    
	nTIPairs = 1;
	psTI     = new FC_CStr[1];
	pTITypes = new CG_BITSET[1];

    bMacroFB   = false;
    nMaxStrLen = 0;

	psTI[0].load(_T("*"));
	pTITypes[0] = CG_TBS_ANY;
    
    memset(&autodecl, 0, sizeof(autodecl));
    pIAutoDecl = NULL;

    magic = 0x4711;
}
FeatureSet::~FeatureSet()
{
	if(psTI)
		delete[] psTI;
	if(pTITypes)
		delete[] pTITypes;

    FC_RELEASE_PTR(pIAutoDecl);
}

