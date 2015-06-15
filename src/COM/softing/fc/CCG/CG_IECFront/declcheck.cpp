#include "cgt_address.h"
#include "declcheck.h"
#include "CG_Messages.h"


void ST_CheckNewDeclScope(
    CG_CIECFront*    pFront,
    const CG_Edipos* pEdp, 
    const CG_Symtab* pSym,
    CG_DECLSCOPE*    pS, 
    WORD*            pdeclMod
)
{

#ifdef ST_STRICT_IEC
    //Check order of var decl sections:
    if(s==CG_input || s==CG_output || s==CG_in_out)
    {
        if(m_onAppendStoreClass!=CG_undefined && 
            m_onAppendStoreClass!=CG_input &&
            m_onAppendStoreClass!=CG_output && 
            m_onAppendStoreClass!=CG_in_out
          )
        {
            pdm->msg0(CG_W_DECL_SEC, pEdp TION_ORDER);
        }
    } 
#endif

    CG_POUTYP        pouTyp;
    CG_DECLSCOPE     s;
    WORD             declMod;
    CGT_CDiagMsg*    pdm;
    
    
    pdm = pFront->m_pdm;


    
    pouTyp = pSym->pouTyp;    
    
    s = *pS;
    declMod = *pdeclMod;

    //Check restrictions according to POU typ:
    switch(pouTyp)
    {
    case CG_POU_PRG:
        switch(s)
        {
        case CG_VAR:
        break;
        case CG_VAR_TEMP:
        case CG_CONST:
        case CG_VAR_EXTERNAL:
            if(declMod & CG_DMOD_ANY_RETAIN)
            {
        	    pdm->msg1(CG_E_TEXT, pEdp, pdm->jot1()<<ST_StoreClassName(s)<<
                    pdm->res1(CG_RES_STR_DECL_RETAIN));
                declMod &= ~CG_DMOD_ANY_RETAIN;
            }
        break;
        case CG_VAR_IN_OUT:
            //not supported:
            pdm->msg1(CG_E_NOT_SUPPORTED, pEdp, pdm->res1(CG_RES_STR_PRG_USING)<<_T("VAR_IN_OUT"));
            s = CG_VAR;
        break;
        case CG_VAR_INPUT:
        case CG_VAR_OUTPUT:
            //not supported:
            // #### FF
            pdm->msg1(CG_E_NOT_SUPPORTED, pEdp, pdm->res1(CG_RES_STR_PRG_USING)<<_T("VAR_INPUT, VAR_OUTPUT, VAR_IN_OUT"));
            s = CG_VAR;
        break;
        case CG_VAR_NATIVE:
        break;
        default:
            //simply error:
            pdm->msg2(CG_E_INVALID_DECL_SCOPE, pEdp, ST_StoreClassName(s), ST_PouTypeName(pouTyp));
            s = CG_VAR;
        }
    break;
    case CG_POU_FUN:
        // ++**++ JD extInParam -->
        /*if(s==CG_VAR_IN_EXT && pSym->pVarInExt)
        {
            pdm->msg1(CG_E_VAR_IN_EXT_NOT_LAST_IN, 
                            pEdp, _T("VAR_IN_EXT"));
            s = CG_VAR;
                         
        }*/
        // ++**++ JD extInParam <--
        switch(s)
        {
        case CG_VAR:
        case CG_CONST:
        case CG_VAR_INPUT:
        case CG_VAR_OUTPUT:
        case CG_VAR_IN_OUT:
        case CG_VAR_IN_EXT:
            if(declMod & CG_DMOD_ANY_RETAIN)
            {
                pdm->msg0(CG_E_INVALID_RETAIN_FUN, pEdp);
                declMod &= ~CG_DMOD_ANY_RETAIN;
            }
        break;    
        default:
        case CG_VAR_NATIVE:
        break;
        //simply error:
            pdm->msg2(CG_E_INVALID_DECL_SCOPE, pEdp, 
                ST_StoreClassName(s), ST_PouTypeName(pouTyp));
            s = CG_VAR;
        }
    break;
    case CG_POU_FB:
        switch(s)
        {
        case CG_VAR:
        case CG_VAR_INPUT:
        case CG_VAR_OUTPUT: //TODO check every thing, e.g. not CONSTANT (avoid doing it with syntax)
        break;
        case CG_CONST:
        case CG_VAR_IN_OUT:
        case CG_VAR_TEMP:
        case CG_VAR_EXTERNAL:
        assert((declMod & CG_DMOD_ANY_RETAIN)==0); //TODO can happen assert
        break;
        case CG_VAR_NATIVE:
        break;
        default:
        //simply error:
            pdm->msg2(CG_E_INVALID_DECL_SCOPE, pEdp, 
                ST_StoreClassName(s), ST_PouTypeName(pouTyp));
            s = CG_VAR;
        }
    break;
    case CG_POU_GVL:
        if(s != CG_VAR_GLOBAL && s != CG_CONST)
        {
            pdm->msg2(CG_E_INVALID_DECL_SCOPE, pEdp, 
                ST_StoreClassName(s), ST_PouTypeName(pouTyp));
            s=CG_VAR_GLOBAL;
        }
    break;
    case CG_POU_CST:
        if(s != CG_CONST)
        {
            pdm->msg2(CG_E_INVALID_DECL_SCOPE, pEdp, 
                ST_StoreClassName(s), ST_PouTypeName(pouTyp));
            s=CG_CONST;
            declMod = 0;
        }
        else if(declMod & (CG_DMOD_ANY_RETAIN|CG_DMOD_CONSTANT))
        {
            pdm->msg2(CG_E_MODI_ILL_USE, pEdp, 
                ST_StoreClassName(s), ST_PouTypeName(pouTyp));
            declMod = 0;
        }
    break;
    case CG_POU_SDT:
        if(s != CG_VAR)
        {
            pdm->msg2(CG_E_INVALID_DECL_SCOPE, pEdp, 
                ST_StoreClassName(s), ST_PouTypeName(pouTyp));
            s=CG_VAR;//TODO declMod as new feature 
        }
    break;
    default:
        assert(!"bad pou typ");
    }


    //Check for (currently) not supported stuff:    
    if(s==CG_VAR_TEMP)
    {
        pdm->msg1(CG_E_NOT_SUPPORTED, pEdp, _T("VAR_TEMP"));
        s = CG_VAR;
    }

    *pS = s;
    *pdeclMod = declMod;
}
    






bool ST_CheckAndAppendDecl(
    CG_CIECFront*    pFront,
    ST_CSymtabPrv*   pSymPrv,
    CG_DECLSCOPE     declScope,
    WORD             declMod,

    const TCHAR*     pszId, 
    const CG_Edipos* pEdpId, 

    const CG_DType*  pDType,  //never NULL error in type are do not pass here
    const CG_Edipos* pEdpTyp,

    const TCHAR*     pszAddr, 
	const TCHAR*     pszSysAddr, //forosai
    const CG_Edipos* pEdpAddr, 

    const CG_AttribList*   pAttr, 
    const CG_Initialiser*  pInit
)
{
    bool                bOk;
    CG_POUTYP           pouTyp;
    const CG_Symtab*    pSym;
    const CG_SymEntry*  pPrior;
    const CG_EdpSymtab* pImpGVL;
    CGT_CDiagMsg*       pdm;
    
    
    pdm = pFront->m_pdm;

    pSym   = pSymPrv->getCGSym();
    pouTyp = pSym->pouTyp;    
    
    assert(declScope!=CG_DECLSCOPE_UNDEF);//<-must already been set!
    assert(pouTyp   !=CG_POUTYP_UNDEF);//<-must already been set!
    assert(pDType->type!=CG_PROGRAM); //not done with this function



    //First check name scopes:
    //---------------------------------------------------
    //check #IMPORT gvls: 
    pPrior = pSymPrv->lookUpEntryDeep(pszId, &pImpGVL);
    if(pImpGVL)
    {
        pdm->msg3(CG_E_IMPORT_NAME_REDEFINED,
                    &pImpGVL->edp,
                    pImpGVL->pSym->edp.pszFile, pszId, 
                    pdm->edpToStr(pEdpId, pdm->jot1()));
        //fake: not needed: the imported name gets overwritten by the local name
        //see implementation of findEntryByName. This way further results are 
        //predictable. We also could change this from error to warning.
    }
    else if(pPrior)
    {
        pdm->msg2(CG_E_REDEF_VAR, pEdpId, pszId, pdm->edpToStr(&pPrior->edp, pdm->jot1()));
        return true; //ret true will not stop a possible Id,Id,Id,Id list to be inserted.
    }

    //check that it is not named as any of the 'super global' CONST:
    //CAUTION: this works only because this table is added to the 
    //global const pool (if it's a .cst) after the CST File has been parsed
    pPrior = pFront->lookUpCONST(pszId);
    if(pPrior)
    {
        pdm->msg2(CG_E_REDEF_VAR_CONST, pEdpId, pszId, pdm->edpToStr(&pPrior->edp, pdm->jot1()));
        //fake: not needed: the global const name gets overwritten by the local name
        //This way further results are predictable. 
        //We also could change this from error to warning.
    }

    //d-273
    if(pFront->lookUpAutoDecl(pszId, NULL, NULL))
    {
        pdm->msg2(CG_E_REDEF_VAR, pEdpId, pszId, _T("<auto declaration>"));
        return true; //ret true will not stop a possible Id,Id,Id,Id list to be inserted.
    }



    //check usage of ANY-types
    if(!CG_IsMonomorph(pDType->type))
    {
        if(pDType->bIsArray!=0)
        {
            pdm->msg1(CG_E_ANYTYPE_BAD_USE, pEdpTyp, ST_CG_DTYPES_ToString(pDType->type));
            return false;
        }
        else if(pouTyp==CG_POU_FUN)
        {
            //possible if function in,out,inout,in_ext but not local scope:
            switch(declScope)
            {
            case CG_VAR_INPUT:
            case CG_VAR_IN_EXT:
            case CG_VAR_OUTPUT:
            case CG_VAR_IN_OUT:
            break;
            default:
                //error:
                pdm->msg1(CG_E_ANYTYPE_BAD_USE, pEdpTyp, ST_CG_DTYPES_ToString(pDType->type));
                return false;
            }
        }
        else if(pouTyp==CG_POU_FB && declScope!=CG_VAR_IN_OUT)
        {
            //possible only (as ATCM IEC extension) if FB inout:
            pdm->msg1(CG_E_ANYTYPE_BAD_USE, pEdpTyp, ST_CG_DTYPES_ToString(pDType->type));
            return false;
        }
    }
    
    

    //check usage of assumed size array:
    if(CG_IsAssumedSizeArray(pDType))
    {
        bOk = false;
        switch(declScope)
        {
        case CG_VAR_INPUT:
        case CG_VAR_IN_EXT:
        case CG_VAR_OUTPUT:
            if(pouTyp==CG_POU_FUN)
                bOk = true;
        break;    
        case CG_VAR_IN_OUT:
            bOk = true;
        }
        if(!bOk)
        {
            pdm->msg0(CG_E_DYNARRAY_BAD_USE, pEdpTyp);
            return false;
        }
    }

    //check usage of type together with CONST:
    if(declScope==CG_CONST && (!CG_IsElementary(pDType->type) || pDType->bIsArray))
    {
        pdm->msg1(CG_E_CONST_NOT_SIMPLE_TYPE, pEdpTyp, pDType->pszName);
        return false;
    }


    //check the pouTyp of the pDType->pSym if any
    if(pDType->pSym)
    {
        //a) inside a struct only struct members, 
        //   array only of struct.
        if((pouTyp==CG_POU_SDT || pDType->bIsArray) && pDType->pSym->pouTyp!=CG_POU_SDT) 
        {
            assert(!pDType->bIsArray); //ARRAY of non SDT should be already checked
                                       //and not pass here!
            //error C1332: expected type '%1', but '%2' is a '%3'
            pdm->msg3(CG_E_INVALID_POU_TYPE, pEdpTyp, _T("STRUCT"), 
                pDType->pSym->pszId, ST_PouTypeName(pDType->pSym->pouTyp));
            return false;
        }

        const CG_KeyValPair* pDep = pFront->lookUpAttrib(pDType->pSym->pAttribs, CG_ATTR_DEPRECATED);
        if(pDep)
            pdm->msg2(CG_W_DEPRECATED, pEdpTyp, pDType->pSym->pszId, pDep->pszVal);
    }

    //check that array base type is supported:
    if(pDType->bIsArray && (pDType->tbs & pFront->m_features.m_pCFeat->arrayTypes)==0)
    {
        pdm->errNS3(pEdpTyp, pdm->res1(CG_RES_STR_ARRAY_BASE)<<_T(" '"),
            ST_BitsetToStr(pdm->jot1(), pDType->tbs), _T("'")); 
    }
   

 


    //check if direct address allowed for this data type and scope:
    //-------------------------------------------------------------
    if(pszAddr)
    {
        HRESULT      errId = 0;
        bool         bAddrHasStar = false;
        bool         bIsIncompleteDef = false;
        const TCHAR* pszErrorAddr1 = pszAddr;
        const TCHAR* pszErrorAddrHint = NULL;
        FC_CString   helpString;
         
        if(pszId[0]==_T('%'))
        {
            pdm->msg1(CG_E_NOT_SUPPORTED, pEdpAddr, pdm->res1(CG_RES_STR_DADDR_WOUT_NAME));
        }

        //check: no retain spec for IO-Inputs:
        TCHAR chLoc = CGT_GetAddrLocation(pszAddr);
        if((declMod & CG_DMOD_ANY_RETAIN) && chLoc==_T('I'))
        {
            pdm->msg1(CG_W_INVALID_RETAIN_SPEC_IOI, pEdpAddr, pszId);
            //fake:
            declMod &= ~CG_DMOD_ANY_RETAIN;
        }

        if(chLoc==_T('M') && !pFront->m_features.m_pCFeat->bMemoryDirectAddr)
        {
            pdm->errNS2(pEdpAddr, pdm->jot1()<<_T("'")<<pszAddr<<_T("': "),
                                pdm->res1(CG_RES_STR_DADDR_WITH_MEM)
               );
        }
        else if(CGT_IsVarConfigAddressRef(pszAddr))
        {
            errId = CG_E_NOT_SUPPORTED; 
            helpString<<_T("'")<<pszAddr<<_T("': ")<<pdm->res1(CG_RES_STR_DADDR_VARCONFIG);
            pszErrorAddr1 = helpString;
        }
        else if(CGT_IsFullBasicAddress(pszAddr))
        {
            //[IQM] is present and no '*':
            //if full addr: only VAR_GLOBAL and PROGRAM VAR
            if(pouTyp!=CG_POU_GVL && pouTyp!=CG_POU_PRG)
            {
                pszErrorAddrHint = ST_PouTypeName(pouTyp);
                errId = CG_E_ADDR_ILL_USE; 
            }
            else if(pouTyp==CG_POU_PRG && declScope!=CG_VAR)
            {
                pszErrorAddrHint = ST_StoreClassName(declScope);
                errId = CG_E_ADDR_ILL_USE; 
            }
        }
        else if(_tcsstr(pszAddr, _T("\"")))
        {
            errId = CG_E_NOT_SUPPORTED; //HACK for "" OPC Addr.
            helpString<<_T("'")<<pszAddr<<_T("': ")<<pdm->res1(CG_RES_STR_DADDR_OPC);
            pszErrorAddr1 = helpString;
        }
        else            
        {
            bAddrHasStar = CGT_IsIncompleteAddressRef(pszAddr);
            if(bAddrHasStar)
            {
                //pszAddr contains a '*':
                //if incomplete Reference: only struct,FB and only VAR
                if(pouTyp!=CG_POU_SDT && pouTyp!=CG_POU_FB)
                {
                    errId = CG_E_IREF_ADDR_ILL_USE; 
                    pszErrorAddrHint = ST_PouTypeName(pouTyp);
                }
                else if(declScope!=CG_VAR)
                {
                    errId = CG_E_IREF_ADDR_ILL_USE; 
                    pszErrorAddrHint = ST_StoreClassName(declScope);
                }
            }
        
            if(CGT_IsIncompleteAddressDef(pszAddr))
            {
                bIsIncompleteDef = true;
                //no [IQM] after %, pszAddr may or may not contain a '*':
                //1. if incomplete Definition without '*': only Program or VAR_GLOBAL.
                //2. if program then only VAR
                if(!bAddrHasStar && pouTyp!=CG_POU_GVL && pouTyp!=CG_POU_PRG)
                {
                    errId = CG_E_IDEF_ADDR_ILL_USE; 
                    pszErrorAddrHint = ST_PouTypeName(pouTyp);
                }
                else if(pouTyp==CG_POU_PRG && declScope!=CG_VAR)
                {
                    errId = CG_E_IDEF_ADDR_ILL_USE; 
                    pszErrorAddrHint = ST_StoreClassName(declScope);
                }
                else if(!pDType->pSym || (pDType->pSym->flags & CG_STAB_HAS_INCADDR)==0)
                {
                    errId = CG_E_IDEF_ADDR_ILL_USE2;
                    helpString.load(pDType->pszName);
                    pszErrorAddrHint = helpString;
                }
            }
            
            //at least one attrib must be true:
            assert(bIsIncompleteDef || bAddrHasStar);
        }

        if(errId==0)//check data type
        {
            if(bIsIncompleteDef)
            {
                //type of variable must be FB or struct (but no array of struct)
                //with incomplete refererence
                if(!pDType->pSym || (pDType->pSym->flags & CG_STAB_HAS_INCADDR)==0)
                {
                    errId = CG_E_IDEF_ADDR_ILL_USE2;
                    helpString.load(pDType->pszName);
                    pszErrorAddrHint = helpString;
                }
            }
            else if(pDType->pSym && (pDType->pSym->flags & CG_STAB_HAS_INCADDR))
            {
                //for better err msg: special message if struct/FB needs an incomplete addr def addr
                if(!bIsIncompleteDef)
                {
                    errId = CG_E_NEED_INCOMPLETE_ADDR;
                    helpString.load(pDType->pszName);
                    pszErrorAddrHint = helpString;
                }
            }
            //##d-1393  do not check %-address restrictions in front end !
            //else 
            //%[IQM] address format and IEC type is 
            //simple or a struct at %M without '*'-Addr.
            //all checks must be done by backend/IO-Configurator...
        }        
        

        if(errId)
        {
            pdm->msg2(errId, pEdpAddr, pszErrorAddr1, pszErrorAddrHint);
            //fake: don't check any further...
            pszAddr = NULL;
            bAddrHasStar = false;
        }
    }
    else
    {
        //if no error so far, force addr if type has incomplete 
        //addr reference and is not inout or var ext:
        if(pDType->pSym && (pDType->pSym->flags & CG_STAB_HAS_INCADDR))
        {
            bOk = declScope==CG_VAR_IN_OUT || declScope==CG_VAR_EXTERNAL ||
                  (pouTyp==CG_POU_FUN && (declScope==CG_VAR_INPUT||declScope==CG_VAR_OUTPUT));
         
            if(!bOk)
                pdm->msg2(CG_E_NEED_AT, pEdpId, pszId, pDType->pszName);
        }
    }



    //Check edge detection usage:
    //---------------------------------------------------
    //only allowed for boolean FB VAR_INPUT:
    if(pDType->type==CG_BOOL_R_EDGE || pDType->type==CG_BOOL_F_EDGE)
    {
        if(pouTyp!=CG_POU_FB || declScope!=CG_VAR_INPUT)
            pdm->msg1(CG_E_ILLUSE_EDGE, pEdpId, pszId);
    }





    //Check usage of Initializer:
    //---------------------------------------------------
    //force init is CONST:
    if(declScope==CG_CONST && !pInit)
    {
        //warn only if no errors so far, other wise you will get 
        //this warning only because type mismatch in init value and the like:
        //TODO --->? was this so needed ? if(pFront->getErrorsInFile(pEdpId->psz)==0)
            pdm->msg1(CG_W_CONST_NO_INIT, pEdpId, pszId);

        CG_Initialiser* pInitNew; 
        FC_NEW_ZSTRUCT(pInitNew, CG_Initialiser);
        pInitNew->ut = CG_INIT_SIMPLE;
        if(pDType->type==CG_STRING || pDType->type==CG_WSTRING)
        {
            pInitNew->sim.pLit = pFront->m_exprMem.registerStringLiteral(pDType->type, NULL);
        }
        else
        {
            pInitNew->sim.pLit = pFront->m_exprMem.registerLiteral(pDType->type, NULL, 0, 0.0);
        }
        pInit = pFront->registerInit(pInitNew);
    }


    if(pInit)
    {
        assert(!CG_IsAnyQVT(pDType->type));//<-must be checkec already in ST_Expression::checkInit
        
        if(declScope==CG_VAR_EXTERNAL)
        {
            //no inits for VAR_EXTERNAL allowed:
            pdm->msg1(CG_E_INIT_ON_VAR_EXTERN, pEdpId, pszId);
        }
        else if(declScope==CG_VAR_IN_OUT)
        {
            // no inits for VAR_IN_OUT and VAR_INPUT if POU is a function:
            pdm->msg1(CG_E_INIT_AT_INOUT_VAR, pEdpId, pszId);   
        }
        else if(pouTyp==CG_POU_FUN && (declScope==CG_VAR_INPUT|| declScope==CG_VAR_IN_EXT))
        {
            pdm->msg1(CG_E_INIT_AT_FUN_INPUT, pEdpId, pszId);   
        }
        else if(pszAddr && CGT_GetAddrLocation(pszAddr)==_T('I'))
        {
            //no Inits if direct address is not writable
            pdm->msg2(CG_E_INIT_AT_NON_LVALUE_ADR, pEdpId, pszId, pszAddr);   
        }
    }
    
   
#if 0  //TODO    
    //Check usage of object attributes:
    //---------------------------------------------------
    //1. no attribs for var_external and var_in_out
    if(pAttr)
    {
        bool bAttrErr = false;
        if(declScope==CG_VAR_EXTERNAL || declScope==CG_VAR_IN_OUT)
        {
            pdm->msg2(pEdp, E_4CCG_WARN_ATTR_VAR_IGNORED, 
                ST_StoreClassName(declScope), pszId);
            bAttrErr = true;
        }
        else
        {
            const TCHAR* pszDeclId    = pAttr->getAttr(sts_atr_declid);
            const TCHAR* pszJavaClass = pAttr->getAttr(sts_atr_javaclass);

            //2. declid only for ANY_SIMPLE
            if(pszDeclId && (m_onAppendType.GetTypeSet() & ~ST_ANY_SIMPLE)!=0)
            {
                pdm->msg3(pEdp, E_4CCG_WARN_ATTR_BAD_TYPE, 
                    pszId, STS_GetAttrTagNameFromEnum(sts_atr_declid), pszId);
                bAttrErr = true;
            }
            //3. javaclass only for ANY_SIMPLE
            if(pszJavaClass && (m_onAppendType.GetTypeSet() & ~ST_ANY_SIMPLE)!=0)
            {
                pdm->msg3(pEdp, E_4CCG_WARN_ATTR_BAD_TYPE, 
                    pszId, STS_GetAttrTagNameFromEnum(sts_atr_javaclass), pszId);
                bAttrErr = true;
            }
        }
        if(bAttrErr)
        {
            delete(pAttr);
            pAttr = NULL;
        }
    }
#endif  

    
    
    //Check usage of CONSTANT qualifier:
    //---------------------------------------------------
    if((declMod & CG_DMOD_CONSTANT) && (pDType->type==CG_INSTANCE))
    {
        pdm->msg1(CG_E_FBI_CANNOT_CONSTANT, pEdpId, pszId);
        declMod &= ~(CG_DMOD_CONSTANT);
    }


    //usage of var extended feature done on end of symtab

    //register the decl:
    pSymPrv->registerDecl(
        pszId, 
        pEdpId, 
        declScope,
        declMod,
        pAttr,
        pDType,
        pszAddr,
		pszAddr ? pszSysAddr : NULL, //forosai
        pEdpAddr, 
        pInit
    );
    return true;
}







