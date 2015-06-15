#ifndef CG_CIECFRONT_H_
#define CG_CIECFRONT_H_

#include "cgt_CDiagMsg.h"
#include "CG_IECFront.h"
#include "fc_todebug\fc_assert.h"
#include "fc_tools\fc_hash.h"
#include "typeset.h"
#include "parse4CP.h"
#include "parseKAD.h"
#include "cgmem.h"
#include "FeatureSet.h"

class ST_CBody;
class ST_CConfig;
class ST_Expression_C;

/*
 * every IEC-function including operators can be expressed
 * using the following generalized function definition
 *
 * nOverloads = number of overloads >= 1
 * lpArgs = int[nOverloads] number of arguments
 * lpVarArg = int[nOverloads] index of variable argument.
 * lpVarOuts = int[nOverloads] number of output arguments.
 * lpOffset = int[nOverloads] index into lpArgTypes
 * lpArgTypes = int[nArgTypes] index into lpTypes
 * lpTypes = ST_TypeSet_C[nTypes] possible type sets for arguments and result.
 *
 *
 * example for simple user functions:
 *      nOverloads will be 1
 *      nTypes will be nArgs+1
 *      nArgTypes will be nArgs+1
 *      lpArgs[0] will be nArgs
 *      lpVarArg[0] will be 0
 *      lpVarOuts[0] will be 0
 *      lpOffset[0] will be 0
 *      lpArgTypes will be {0,1,2,3,4,...nArgs}
 *
 *      lpTypes will normally be fully qualified types
 *      lpTypes[0] = result type
 *      lpTypes[1] = first argument type
 *      lpTypes[2] = second argument type
 *      lpTypes[3] = third argument type
 *          ...
 *      lpTypes[nArgs] = last argument type
 *
 * NOTE: positive lpVarOuts[.] count from the left end.
 *       negative lpVarOuts[.] count from the right end.
 *
 * NOTE: lpVarArg[.] = 0 means no variable arguments.
 *       1 means first argument is variable.
 *       2 means second argument is variable etc...
 *       the variable argument can be expanded from 1..many
 *       actual arguments. they can be var_ins or var_outs.
 *
 * NOTE: if multiple overloads are used you should be sure to
 *       place more specific overloads first because the checker
 *       tries to match them first.
 */

struct ST_FUNCTION_DEF_TYP 
{
    const TCHAR*  pszId; //name of function/operator/buildin (static text or globally registered!)
    int nOverloads;     /* 1 ... */
    int nTypes;         /* 1 ... */
    int nArgTypes;      /* dont care */
    int *lpArgs;        /* int [nOverloads] 0 ... */
    int *lpVarArg;      /* int [nOverloads] 0 = no VarArg, 1 = first arg, ... nArgs */
    int *lpVarOuts;     /* int [nOverloads] 0 = no VarOuts, 1 = one VarOut, ... nArgs only VarOuts */
    int *lpOffset;      /* int [nOverloads] offset into lpArgTypes */
    int *lpArgTypes;    /* int [nArgTypes] references lpTypes */
    ST_TypeSet_C *lpTypes;      /* ST_TypeSet_C [nTypes] */
};


const ST_FUNCTION_DEF_TYP* ST_LookUpOperatorDefinition(CG_EXPRTYP op, bool isIlMode);

struct ST_CONST_REF
{
    const CG_SymEntry* pConstEntry;
    CG_Edipos          edpOfRef;
};

class ST_CMacCallDescr;
class CG_CIECFront;
struct ST_PARAMSPEC;//##AC
class ST_CSymtabPrv
{
    CG_CIECFront*             m_pFront;         //for convenience only
    FC_CHashStr<CG_SymEntry*> m_hashTable;      //hash the entries
    CG_Symtab*                m_pMySym;         //unconst pointer to owner
                                                //always true is this relation: 
                                                //pMySym->pPrivate==this
    FC_CGArray<CG_EdpSymtab>  m_ImpGVLList;     //#import list
    FC_CGArray<CG_EdpStr>     m_VarNative;      //list all var native blocks
    //function specific:
    ST_FUNCTION_DEF_TYP*      m_pFunIFDef;      //!=NULL only if FUN and no errors
    FC_CGArray
        <ST_CMacCallDescr*>   m_OverloadBodies; //contains the {} macro for every
                                                //polymorphic overload.
    CG_EXPRTYP                m_opEnum;         //if function is "inline +", undef enum else
    CG_Edipos                 m_opEdp;          //if function is "inline +", memset 0 else 
    FC_CGArray<CG_BITSET>     m_polyTypes;      //array of different polymorphic types 
                                                //used in function interface      
    int                       m_numOfVarArg;    //the ordinal number of the
                                                //VAR_IN_EXT input or 0 if none


    //used symtabs/const hashes and final result pointers:
    FC_CHashPtr
      <const CG_Symtab*, int>*m_pRefSyms;       //tmp hash to build m_pzHdrRefSyms
    const CG_Symtab**         m_pzRefSyms;      //holds final result after endOfHeader 
                                                //all directly used symtabs of FBs,SDTs,FUNs
                                                //If this is a GVL also contains PRGs, does 
                                                //not contain referenced .cst symtabs
    FC_CHashPtr
    <const CG_SymEntry*,int>* m_pRefGlobConst;  //tmp. hash to build m_pzUsedGlobConst
    const CG_SymEntry**       m_pzRefGlobConst; //holds final result after loadBody
                                                //if has body or if has no body on 
                                                //end of header.
    FC_CGArray<ST_CONST_REF>* m_pConstRefs;     //list of all local and global
                                                //const refs, only used for 
                                                //XREF gets freed after xref done.


    bool createInfoFromLinkFile();

public:

    ST_CSymtabPrv(
        CG_CIECFront*        pFront,   //must be given
        const CG_Edipos*     pEdp,     //must be given
        const CG_LIBContext* pLibCtx,  //!= if Lib defined
        const TCHAR*         pszName,  //must be given
        CG_POUTYP            pouTyp,   //can be undef if syntax error like FFUNCTION
        CG_SOURCETYPES       srcTyp,   //can be undef if syntax error like FFUNCTION
        __int64              ftLastWrite //can be NULL if error 
    );
    ~ST_CSymtabPrv();


    const CG_Symtab* getCGSym()const{return m_pMySym;}

    const ST_FUNCTION_DEF_TYP* getFunIF()const
    {
        assert(m_pFunIFDef);
        assert(m_pMySym->flags & CG_STAB_HDR_NOERR);
        return m_pFunIFDef;
    }
    //look up only in the main entry list, not the #imports:
    const CG_SymEntry* lookUpEntry(const TCHAR* pszId)const
    {
        return m_hashTable.get(pszId);
    }
    //serach for entry including all #import gvls: 
    const CG_SymEntry* lookUpEntryDeep(
        const TCHAR*         pszId,
        const CG_EdpSymtab** pGVLImp //can be NULL if not needed
    )const;

    //serach for entry also in #import gvls, if found
    //add a var_external decl.
    const CG_SymEntry* lookUpDeepAndInsertGVImp(
        const TCHAR*   pszId
    );


    bool isPolyMorph()const
    {
        return m_numOfVarArg>0 || m_polyTypes.getUsed()>0;
    }
    const FC_CGArray<CG_BITSET>* getPolyTypeArray()const{return &m_polyTypes;}
    bool isMacro()const
    {
        return m_OverloadBodies.getUsed()>0;
    }
    CG_EXPRTYP isOperator()const
    {
        return m_opEnum;
    }
    const CG_Edipos* getOperatorEdp()const
    {
        return &m_opEdp;
    }

    const CG_MacroBody* lookUpFunMacroBody
    (
        const ST_TypeSet_C&   returnType, 
        int                   nArgs,
        const ST_PARAMSPEC*   pArgs,//##AC
        const CG_DType**      ppRCast,//out 
        int*                  pnACast,//out
        const CG_DType*const**ppACast //out
    )const;
    const CG_MacroBody* getNonPolyMacroBody()const;

    void setNewHdrIFEnd(unsigned long ulByteOff, bool bIsIFPart)
    {
        assert(ulByteOff>=m_pMySym->ulHdrBytes);
        assert(ulByteOff>=m_pMySym->ulIF_Bytes);
        m_pMySym->ulHdrBytes = ulByteOff;
        if(bIsIFPart)
            m_pMySym->ulIF_Bytes = ulByteOff;
    }
    void registerAttributes(
        CG_AttribList*  pAttr
    );
    const CG_Symtab* registerGVLImp(
        const CG_Edipos*    pEdp,
        const TCHAR*        pszFile
    );
    void registerUsedType(
        const CG_Symtab* pUsed
    );
    void registerUsedGlobCONST(
        const CG_SymEntry* pGlobConstEntry
    );
    void registerCONSTRef(
        const CG_SymEntry* pGlobConstEntry, 
        const CG_Edipos*   pEdpOfRef
    );

    void registerOpAlias(
        const CG_Edipos* pEdpOp,
        CG_EXPRTYP       mactyp
    );
    void registerMacroBody(
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
    );
    void registerVarNative(
        const CG_Edipos* pEdp,
        const TCHAR*     pszText
    );
    void registerXREFFile(
        const TCHAR* pszXREFFile
    );
    const CG_SymEntry* registerDecl(
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
    );

    

    const FC_CGArray<ST_CONST_REF>* getCONSTRefList()const{return m_pConstRefs;}
    void freeCONSTRefList(){delete(m_pConstRefs);m_pConstRefs=NULL;}//called after XREF done


    //returns false if header not error free
    //input nErrorCurr is the number of errors so far in file
    bool endOfHeader(
        CG_SOURCETYPES   srcTyp,
        int              nErrorCurr,
        const CG_Edipos* pEdpEndHdr
    );

    bool endOfBody(
        bool     bHasError,
        long     lLangBits
    );

    void setBodyHasNativeBit()
    {
        m_pMySym->flags |= CG_STAB_BDY_NATIVE;
    }
};


#define ST_LHDR_OK         0x000
#define ST_LHDR_UNDEF      0x001
#define ST_LDHD_RECURSIVE  0x002
#define ST_LDHD_ERRIND     0x004
#define ST_LDHD_BAD_POUTYP 0x008
#define ST_LDHD_TRG_NS     0x010
#define ST_LDHD_ILLBI      0x020

class CG_CIECFront
{
    //global lists for literals, simple init, array init and ...
    //lists are only used to free mem on exit:
    //global strings for once create ever last strings 
    //(case sensitive!):
    FC_CHashStrTable                      m_pHashGlobalStrings;
    FC_CGArray<CG_Initialiser*>           m_listInits;//used for simple type only
    FC_CGArray<CG_Initialiser*>           m_listInitArray;
    FC_CGArray<const CG_Symtab**>         m_listSymtabLists;
    FC_CGArray<const CG_SymEntry**>       m_listEntryArray;
    FC_CGArray<long*>                     m_listLongArray;
    FC_CGArray<CG_AttribList*>            m_listAttribLists;
    FC_CGArray<CG_TaskInit*>              m_taskInitsList;
    FC_CGArray<CG_Task*>                  m_tasksList;

    //global data type list: 
    FC_CHashStr<CG_DType*>       m_hashDType;
    //global symtab list: 
    FC_CHashStr<ST_CSymtabPrv*>  m_symtabs;
    //global resource list: 
    FC_CHashStr<CG_Resource*>    m_resources;
    //CONST definitions of all cst files in the project and all libs:
    FC_CGArray<const ST_CSymtabPrv*> m_globalConstTable;


    //next is to detect recursive loading of files
    //this is used first on file level, if there is 
    //also a POU found, then the CG_Symtab* is used
    //only for better error messages.
    FC_CHashStr<ST_CSymtabPrv*>m_hashOnParsingFiles;


    //created once when first lib loaded:
    I4CLOpen*                  m_pILibOpen;


    //map lib names (without path and extension!) to lib access IF,
    //they remain open until final release:
    struct ST_LIB_TYP
    {
        I4CLAccess*          pILib;
        const CG_LIBContext* pInfo;
    };
    FC_CGArray<ST_LIB_TYP>   m_openLibs;

    FC_CHashStr<ST_FUNCTION_DEF_TYP*>* m_pBuilnInList;


    //relevant opt=XYZ options from 4cp's [compiler] section:
    bool       m_bOPT_DisableXref;

public: //public but only for internal use:

    CG_CIECFront(CGT_CDiagMsg* pdm);
    ~CG_CIECFront();
    void debugDumpState(const TCHAR* pszFile);

    //error sink, public but const ptr:
    CGT_CDiagMsg*const  m_pdm;
    //feature set:
    CFeatureSet         m_features;//will be set by back end via loadFeatures()
    //globaly avail if no idea where to store.   
    ST_CExprMem         m_exprMem;

    bool		        m_bFileCheckMode;//set true only during check single file is active
    
	const TCHAR* registerString(
        const TCHAR* psz
    );
    const CG_DType* registerDType(
        CG_DTYPES dType,   //any basic data type
        long      lStrLen  //-1 : auto size string or no string
                           //>=0: fixed size string
    );
    const CG_DType* registerDType(
        const CG_Symtab* pSym  //already globally registered symtab
    );
    const CG_DType* registerDType( //##AC
        CG_BITSET    lTypeSet, 
        const TCHAR* pszTypeName, 
        int          lStrLen
    );
    const CG_DType* registerDType(
        int               nDims,      //=0: if assumed size array
                                      //>0: otherwise
        const CG_Subrange ranges[],   //NULL or array ranges
        CG_DTYPES         dType,      //base type any simple
        long              lStrLen,    //-1 : auto size string or no string
                                      //>=0: fixed size string
        const CG_Symtab*  pSym        //NULL or struct
    );
    const CG_DType* getArrayBaseDType(
        const CG_DType* pArray
    );
    const CG_AttribList* registerAttrList(
        CG_AttribList* pAttrList
    );
    const long* registerLongArray(
        int   nCount,
        long* pLongs
    );
    const CG_Initialiser* registerInit(
        CG_Initialiser*  pInit
    );
    const CG_Initialiser* registerInitArray(
        int              nCount,
        CG_Initialiser*  pInits
    );
    const CG_Symtab*const* registerSymtabList(
        const CG_Symtab** ppSyms
    );
    const CG_SymEntry*const* registerEntryArray(
        int                 nInits,
        const CG_SymEntry** pEntries
    );
    const CG_TaskInit* registerTaskInits(
        CG_TaskInit* pInitsList
    );
    const CG_Task* registerTaskLists(
        CG_Task* pTaskList
    );
    bool loadLIBCSTs(
        const CG_LIBContext*  pInf
    );
    bool lookUpAutoDecl(
        const TCHAR*        pszId,
        ST_CSymtabPrv*      pSymPrv,
        const CG_SymEntry** ppEntry
    );


    ST_CSymtabPrv* getSymPrv(const CG_Symtab* pSym)
    {
        bool           pbRecursive; //TODO make the ST_CSymtabPrv pointer hidden part of CG_Symtab?
        ST_CSymtabPrv* pSymPrv = loadHeader(pSym->pszId, &pbRecursive);
        assert(pSymPrv && !pbRecursive);//<-cannot happen we have already a symtab.
        return pSymPrv;
    }

    ST_CSymtabPrv* getSymPrvById(const TCHAR* pszId)
    {
        return m_symtabs.get(pszId);
    }

public://public for COM interface and internal use

    //set in COM_CIECFront::COM_CIECFront used 
    //mainly for calles to cg_tools.lib:
    ICG_IECFront*  m_myCOMIF;
    
    //initialisation ICG_IECFront:
    bool loadKAD(const TCHAR* pszKADFileAndPath=NULL);
    bool load4CP(const TCHAR* psz4CPFileAndPath);


//KAD info service as public member:
    CKADInfo         m_kad;

//4CP info service as public member:
    ST_Parse4CP      m_4CP;


    I4CLAccess* getLibIFByPOUInfo(const CG_LIBContext* pInfo);  

    /*
     * lookUpBuildInFunDef 
     * looks for the generalized parametrization definition 
     * for buildin function of a given name. No inline code will be 
     * set from this function (must be done by backend)
     *
     * @param    pszFunName  input:  which function
     * @return   NULL if not found or the ST_FUNCTION_DEF_TYP defining it's interface
     */
    ST_FUNCTION_DEF_TYP* lookUpBuildInFunDef(const TCHAR* pszFunName);


    I4CLAccess* findLibOfPOU(
        const TCHAR*          pszPou, 
        const CG_LIBContext** ppInfo,
        bool                  bWarnDuply
    );


    long findPOUDefByIECName(
        const TCHAR*          pszPOUName,  //IEC POU name (FUN,STRUCT, FB, PRG)
        const TCHAR*          pszTrgType,  //NULL or the target type
        const CG_POU4CPInfo** ppPOUInf,    //receives the 4CP info of a file=
        const CG_LIBContext** ppLIBInf,    //receives the 4CP info of a lib=
        FC_CString*           pFileLocator,//will be filled with an unique locator
        bool                  bWarnDuply   //true: warning if duply defined
    );


//load source files by file name or POU name:
    bool loadLib(
        const CG_LIBContext*  pInf
    );
    ST_CSymtabPrv* loadCST(
        const TCHAR*     pszCSTFile,
        bool             bAddToCSTPool,
        bool*            pbRecursive
    );
    ST_CConfig*     loadConfig(
        const CG_POU4CPInfo* pCON
    );
    const CG_Resource* loadResource(
        const TCHAR*     pszResName
    );
    ST_CSymtabPrv* loadHeader(
        const TCHAR*     pszPOUName,
        bool*            pbRecursive
    );

    /*
     * loadHeaderEx tries to find the definition of a POU by calling
     * loadHeader and also lookUpBuildInFunDef. The wanted POU
     * typ is checked and the undefined POU error message is submitted
     * by loadHeaderEx. If a target typpe is currently set loadHeaderEx
     * checks if the found POU does support it (only if library defined).
     * The deprecated attribute of a POU header is checked too.
     *
     * @param    pEdp        edipos for all diags
     * @param    pszPOUName  name of POU to look up
     * @param    want        bitset ST_WANT_* of wanted POU types 
     * @param    ppSymPrv    receives the symtab or NULL if POU undef or is build in
     * @param    ppBIDef     receives the build in interface description or NULL
     *                       if POU undef ao has a symtab. If NULL and POU is build in
     *                       submit error: illegal call of build in function
     * @return   ST_LHDR_OK if no error: then either *ppSymPrv or *ppBIDef is set (never both)
     *           if error a set of ST_LHDR_* bits to specify the reason.
     */
    long loadHeaderEx(
        const CG_Edipos*            pEdp,
        const TCHAR*                pszPOUName,
        long                        want,
        ST_CSymtabPrv**             ppSymPrv,
        const ST_FUNCTION_DEF_TYP** ppBIDef
    );

    const ST_CSymtabPrv* loadGVL(
        const TCHAR*     pszGVLFile,
        bool*            pbRecursive
    );
    const ST_CSymtabPrv* STDMETHODCALLTYPE loadBody(
        const TCHAR*      pszPOUName,
        long              codeGenOpt,
        ST_CBody**        ppBody,
        bool*             pbRecursive
    );

    


//symbol table look up:
    const CG_SymEntry* lookUpEntry(
        const CG_Symtab* pSym, 
        const TCHAR* pszId
    );
    const CG_SymEntry* lookUpEntryDeep(
        const CG_Symtab* pSym, 
        const TCHAR* pszId
    );
    const CG_SymEntry* lookUpCONST(
        const TCHAR* pszId
    );
    const CG_KeyValPair* lookUpAttrib(
        const CG_AttribList* pAttrlist,
        const TCHAR*         pszKey
    );

    const FC_CGArray<const ST_CSymtabPrv*>* getGlobConstSyms()const
    {
        return &m_globalConstTable;
    }
};




#endif //CG_CIECFRONT_H_
