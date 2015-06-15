
#ifndef __ST_PARSECONTEXT_H_
#define __ST_PARSECONTEXT_H_


//----  Aggregate Includes:   --------------------------------------*
#include "expr.h"
#include "Hash4GR.h"
#include "il_ParseContext.h"

//----  Forward Class Definitions:   -------------------------------*
class ST_CBody;
class st_ParseContext;
class ST_CSFCBody;
struct SFC_CHECKER;
struct UTIL_STS_ScannerStruct;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*



class ST_CJumpLabel
{
public:
    CG_STStmt*              m_pDef;    //definition of label always pDef->styp==CG_LABEL
    FC_CGArray<CG_STStmt*>  m_gotos;   //the uses of the label
    FC_CGArray<CG_ExprTree*>m_lblrefex;//the uses of the label as expr label:, always extyp CG_LABELREF  ##AC

    ST_CJumpLabel()
    {
        m_pDef = NULL;    
    }
};

class ST_CBodyTMP
{
public:
    int for_stmts,  while_stmts, repeat_stmts;
    int case_stmts, if_stmts,    sync_stmts;

    bool bHasNative;
    FC_CGArray<ST_Identifier_C*>FORVars;
    FC_CHashStr<ST_CJumpLabel*> labels;
    int nlabels; //gets ++ for evely label stmt.
    
    struct ST_TMPVAR
    {
        bool      bUsed;
        CG_BITSET tbs;
        bool      bRef;
    };
    FC_CGArray<ST_TMPVAR> m_tmpVars;



    
    ST_CBodyTMP() : FORVars(true), labels(true)
    {
       for_stmts = while_stmts = repeat_stmts = 0;
       case_stmts= if_stmts    = sync_stmts   = 0;
       bHasNative = false;
       nlabels = 0;
    }
    ~ST_CBodyTMP()
    {
        FORVars.deletePtrs();
        labels.deletePtrs();
    }
    const CG_Edipos* isActiveForVar(const TCHAR* psz)
    {
        for(int i=0;i<FORVars.getUsed();i++)
        {
            if(!lstrcmpi(psz, FORVars.get(i)->getName()))
                return FORVars.get(i)->getEdp();
        }
        return NULL;
    }

    //returnes >0:
    int getFreeTmp(CG_BITSET tbs, bool bIsReference)
    {
        int i;

        for(i=m_tmpVars.getUsed()-1; i>=0; i--)
        {
            ST_TMPVAR* pTmp = m_tmpVars.getAddrOf(i);
            if(!pTmp->bUsed && pTmp->tbs==tbs && pTmp->bRef==bIsReference)
            {
                pTmp->bUsed = true;
                return i+1;
            }
        }
        ST_TMPVAR tmp;
        tmp.bUsed = true;
        tmp.tbs   = tbs;
        tmp.bRef  = bIsReference;
        return m_tmpVars.add(tmp)+1;
    }
    void freeTmpVar(int i)
    {
        ST_TMPVAR* pTmp = m_tmpVars.getAddrOf(i-1);
        assert(pTmp->bUsed);
        pTmp->bUsed = false;
    }
    void freeAllTmpVars()
    {
        m_tmpVars.resetUsed();
    }

};




class st_parse;//foreward the yacc generated parser class
class st_ParseContext //name of this class must be st_ParseContext
                      //see the yacc templates gentools\yyparse.cpp
                      //and the gentools\yyreplace.pl
{
friend class st_parse;
friend class ST_CParseTree;
friend CG_STStmt* il_ParseContext::transformToST();
    ST_CParseTree* m_pFirstHeapElement;
    ST_CParseTree* m_pLastHeapElement;


    //remember the toterr count at begin of file (only for assertions..)
    int                      m_errCountBeginFile;    
    CG_CIECFront*            m_pFront;
    CGT_CDiagMsg*            m_pdm;      //diagnostic msg. sink
    st_parse*                m_pyacc;
	UTIL_STS_ScannerStruct*  m_pScanner; //must be named 'pScanner'(see gentools\yyparse.cpp)
    CG_Edipos                m_currEdp;  //edp of current input token
    CG_SOURCETYPES           m_srcType;
    __int64                  m_ftLastWrite;//set when parse function called
    const TCHAR*             m_pszFile; //a global string from registerString with 
                                        //infinite lifetime this is the file name on 
                                        //start parsing a file and is used for 
                                        //the CG_Edipos::pszFile member
                                        //can be used to check that POU name 
                                        //equals file name. m_pszFile may have a path
                                        //and and usually an extension.
                                        //if a single expression or ST-action
                                        //is parsed it must be set to the 4gr file name
                                        //if parse is done it's reset to NULL
    const CG_LIBContext*     m_pLibCtx; //!=NULL if lib context

    //pure tmp helpers that cannot not be put into the union (is a class):
    const ST_CAttrList*      m_pSysCommentTMP;
    ST_TypeSet_C             m_typeorTMP;
    ST_TypeSet_C             m_typeorRetTMP;


    //pou type and decl scope set when VAR_XXX [CONSTANT|RETAIN|NON_RETAIN] seen:
    bool                     m_bDontCheckBackendSupportedTypes;
    CG_DECLSCOPE             m_currDeclScope;
    WORD                     m_currDeclBitSet;
    bool                     m_bDeclSecHasAddr;//remember if there was a AT %addr 
                                              //this is needed to find out the 
                                              //interface portion of the header
    unsigned long            m_ulhdrEndTMP;   //remeber the fpos of the last possible 
                                              //end of header.

    ST_CTaskList*            m_pCurrTaskList; //task of current resource
    //##AC m_pCurrFBICall;  //current FB or void fun call

    //always used for all parse actions:
    const CG_Symtab*         pSymtab; 
    ST_CSymtabPrv*           m_pSymtabPrv;


    //pase body specific members:
public: //HACK for SFC_LDFBD_SignalHash
    ST_CSFCBody*             m_pSFC;  //allocated only for SFC bodies
                                      //used as helper class.
private:

    ST_CBody*                m_pBody; //holds memory for body COM IF
                                      //also if SFC
    ST_CBodyTMP*             m_pbt;   //helper class for ST body creation
                                      //conatins a label list and some counters
                                      //must be cleared for every ST-Action

    //parse resource helpers:
    CG_Resource*             m_pResource; //final result
    FC_CGArray<CG_Task*>     m_Tasks;

    //parse configuration helpers:
    ST_CConfig*              m_pConfig;

    //for parse single expr. and ST-action:
    int                      m_startToken;
    int                      m_lLineOffset;
    int                      m_lColOffset;
    ST_Expression_C*         m_pSingleExpression;//holds result of a single expr. parse
    CG_STStmt*               m_pSTAction;        //points to the result of ST action parse

    ST_Expression_C* expandInline(
        const CG_Edipos*     pEdpOP,
        CG_EXPRTYP           openum,
        int                  nArgs,
        ST_PARAMSPEC*        pArgs
    );
    
    il_ParseContext          m_IlContext;
public:
    FC_CGArray<TCHAR>        m_addrExpansionResult;//public for convenience..

    //these methods shall only be called by yacc (friend class st_parse):
    //called for every getToken to update the m_curredp:
    const CG_Edipos* setCurrEdp(int nLine, int nCol);


    //data type creation helpers:
    const CG_Symtab* getSymtabForDType(
        const CG_Edipos* pEdp, 
        const TCHAR*     pszId,
        long             want
    );
    int  checkArrayDims(
        ST_ExpressionList_C* pDims, 
        CG_Subrange ranges[CG_MAX_ARRAY_DIM]
    );
    long checkTypeStrLen(
        CG_DTYPES dType, 
        ST_Expression_C* pExpr
    );

    //helper routines to get a string with good error response:
    //getStringLiteral: used if real expression context, never 
    //returns NULL, does a fake with empty string 
    ST_Literal_C* getStringLiteral(
        const CG_Edipos* pEdp, 
        TCHAR            delim
    );
    //getAttribVal: used for e.g. #import  and sys comments
    //we don't want the $ stuff to be escaped, returns NULL on error
    ST_Identifier_C* getAttribVal(
        const CG_Edipos* pEdp 
    );

    //get native block of text as psz, return NULL if EOF in block
    //returned psz only valid as long no other skip block operation made:
    const TCHAR* getSkippedText(
        const CG_Edipos* pEdp, 
        const TCHAR*     pszEnd
    );
    //get inline {..} block of text as psz, return NULL if EOF in block
    //returned psz only valid as long no other skip block operation made:
    const TCHAR* getSkippedBraceText(
        const CG_Edipos* pEdp, 
        TCHAR            cOpenBr
    );
    //special string handling after #import, returns NULL if error 
    TCHAR* getImportFile(
        const CG_Edipos* pEdp
    );

    //newDeclScope called when reach a VAR_XX CONSTANT|[NON_]RETAIN declscope start:
    void newDeclScope(
        const CG_Edipos* pEdp,
        CG_DECLSCOPE     s, 
        WORD             declBitSet
    );
    void appendDeclListToSymTab(
        ST_CDeclList*    pIdList, 
        ST_DirectVar_C*  pAddr,
		const TCHAR*     pszSysAddr, //forosai
        const CG_DType*  pDType,
        WORD             wStructConstantDecl, //IEC Extension CONSTANT Modifier in Struct decl
        ST_Expression_C* pInit
    );
    void GVLImport(
        const CG_Edipos* pEdp, 
        const TCHAR*     pszFile
    );
    void RESImport(
        const CG_Edipos* pEdp, 
        const TCHAR*     pszFile
    );
    
    //called whenever an end delc syntax found
    //end_var, end_const, var_native, #import and pou name
    //or last token of fun return type :
    void endDeclSection(unsigned long ulEndSectionFpos);




private:
    CG_POUTYP checkFileNameToPOUTyp(
        const CG_Edipos*  pEdp,
        CG_POUTYP         pouTypFound //only for PRG,FB,FUN
    );
    bool checkPOUNameToFileName(
        const CG_Edipos* pEdp,
        const TCHAR*     pszPouName
    );

    //beginn a POU scope:
    void newSymtab(
        const CG_Edipos* pEdp,
        CG_POUTYP        pouTyp,
        const TCHAR*     pszPouName
    );
    bool newPOU(
        const ST_CAttrList*pAttr,
        CG_POUTYP          pouTypFound, 
        ST_Identifier_C*   pId,
        const CG_DType*    pFunTyp
    );

    //beginn a Configuration scope:
    bool newCON(
        const CG_Edipos* pEdpName,
        const TCHAR*     pszConName,
        CG_AttribList*   pAttribs
    );

    //beginn a resource scope:
    bool newRES(
        ST_CAttrList*    pAttr,
        ST_Identifier_C* pId,
        ST_CAttrList*    pAttrId,
        ST_Identifier_C* pOptResType
    );

    void attachPrgToTask(
        WORD             retBitSet,
        ST_Identifier_C* pPrgInstName, 
        ST_CAttrList*    pOptAttr,
        ST_Identifier_C* pOptTaskName,
        ST_Identifier_C* pPrgTypeName
    );

    //return false if body shall not be processed
    bool endOfDecls(bool bBodyFollows); 
    void endOfResource(ST_CTaskList* pTasks);

    void handleEmptyFile();

    
    void endOfPou(
        const CG_Edipos*  pEdp,
        CG_POUTYP         endMarkerFound
    );
    
    //declarations:
    ST_DirectVar_C* getDirectAddr(const TCHAR* pszDirectAddr);
	ST_DirectVar_C* directAddrFromSysVar(const CG_Edipos* pEdp, const TCHAR* pszSysvar); //forosai

public:
    //con/destruction:
    st_ParseContext(
        CG_CIECFront* pIFront
    );
    void initTmpMembers();
    ~st_ParseContext();
    
    int getErrorsInFile()
    {
        return m_pdm->getTotalErrors()-m_errCountBeginFile;
    }
    void indirectError()
    {
        m_pdm->addIndirectErr();
    }
    const CG_Edipos* getCurrEdp()const
    {
        return &m_currEdp;
    }
    //returns the globally registerd source file name:
    const TCHAR* getEDPSrcFile()const
    {
        return m_pszFile;
    }
    CG_CIECFront* getFront()const
    {
        return m_pFront;
    }
    CGT_CDiagMsg* getDM()const
    {
        return m_pdm;
    }
    const CG_Symtab* getCGSym()const
    {
        return pSymtab;
    }
    ST_CSymtabPrv* getSym()const
    {
        assert(m_pSymtabPrv);
        return m_pSymtabPrv;
    }
    
    ST_CBodyTMP* getBodyTMP(){return m_pbt;}
    
    ST_CBody*   getCBody(){return m_pBody;}
    
    ST_CSymtabPrv* parseHeader(
        const TCHAR*         pszFileName,//used for initial edipos
        const CG_LIBContext* pLibCtx,    //!=NULL if from lib 
        const TCHAR*         pszText,    //text, either a complete file or 
                                         //a 4gr header part or from a lib a .hdr
        unsigned             ulsize,     //size in TCHARs without 
                                         //the terminating 0
        __int64              ftLastWrite //file time of file to parse
    );
    ST_CSymtabPrv* parseWholeSTFile(
        const TCHAR*         pszFileName,//used for initial edipos, 
        const CG_LIBContext* pLibCtx,    //!=NULL if from lib 
        const TCHAR*         pszText,    //text, either a complete file or 
                                         //a 4gr header part or from a lib a .hdr
        unsigned             ulsize,     //size in TCHARs without the 
                                         //terminating 0
        ST_CBody**           ppBody,     //the body to be returned
        __int64              ftLastWrite,//file time of file to parse
        ST_CSymtabPrv*       pSymOld     //!=NULL if load body after only hdr was loaded
    );
    ST_CSymtabPrv* parseWholeILFile(
        const TCHAR*         pszFileName,//used for initial edipos, 
        const CG_LIBContext* pLibCtx,    //!=NULL if from lib 
        const TCHAR*         pszText,    //text, either a complete file or 
                                         //a 4gr header part or from a lib a .hdr
        unsigned             ulsize,     //size in TCHARs without the 
                                         //terminating 0
        ST_CBody**           ppBody,     //the body to be returned
        __int64              ftLastWrite,//file time of file to parse
        ST_CSymtabPrv*       pSymOld     //!=NULL if load body after only hdr was loaded
    );
    ST_CSymtabPrv* parse4GRFile(
        const TCHAR*          pszFileName,//used for initial edipos, 
        const CG_LIBContext*  pLibCtx,    //!=NULL if from lib 
        const TCHAR*          pszText,    //text, a complete ST file
        unsigned              ulsize,     //size in TCHARs without the 
                                          //terminating 0
        SFC_LDFBD_SignalHash& grElements, //full 4gr body also if SFC,FBD,LD
        ST_CBody**            ppBody,     //the body to be returned
        __int64               ftLastWrite,//file time of file to parse
        ST_CSymtabPrv*        pSymOld,    //!=NULL if load body after only hdr was loaded
        long                  codeGenOpt  //CG_LOADBDY_* bits
    );
    ST_Expression_C* parse4GRBox(//use only after call parse4GRFile
      const CG_Edipos* pEdp,     //must not be NULL
      const TCHAR*     pszExpr,  //expression as string, should not have newlines.
      bool             bNoWarns  //if true no warnings, usefull for reparse same expr. 
    );
    CG_STStmt* parseSTAction(      //use only after call parse4GRFile
        const CG_Edipos* pEdp,     //must not be NULL, the edp of the Action box
        const TCHAR*     pszAction,//ST action text as string
        bool             bNoWarns  //if true no warnings, usefull for reparse
    );
    ST_CConfig* parseConfiguration(
        const TCHAR*     pszFileName,//used for initial edipos, 
        const TCHAR*     pszText,    //a complete .res file
        unsigned         ulsize,     //size in TCHARs without the 
                                     //terminating 0
        __int64          ftLastWrite //file time of file to parse
    );
    CG_Resource* parseResource(
        const TCHAR*     pszFileName,//used for initial edipos, 
        const TCHAR*     pszText,    //a complete .res file
        unsigned         ulsize,     //size in TCHARs without the 
                                     //terminating 0
        ST_CSymtabPrv**  ppSymPrvNew,//OUT: if 0 errors the glob var table
        __int64          ftLastWrite //file time of file to parse
    );
    ST_Expression_C* parseSingleExpression(
      const CG_Edipos* pEdp,     //must not be NULL
      bool             bIncCol,  //pEdp's line,column info is updated for every diag msg
      const TCHAR*     pszExpr,  //expression as string, should not have newlines.
      ST_CSymtabPrv*   pSymPrv,  //may be NULL, if only constant expr. should be evaluated.
      bool             bNoWarns  //if true no warnings, usefull for reparse same expr. 
    );


//call these methodes from the graphics front end or yacc:
    ST_Expression_C* createVar(
        ST_Identifier_C*  pId
    );
    ST_Expression_C* checkUPlusMinus(
        const CG_Edipos* pEdp,
        ST_Expression_C* pExpr, 
        bool             bMinus
    );
    ST_Expression_C* checkDirectBodyVar(
        ST_Expression_C* pDirect
    );
    ST_Expression_C* createOp(
        const CG_Edipos* pEdp,
        CG_EXPRTYP       op,
        ST_Expression_C* pFirstArg,
        ST_Expression_C* pSecondArg
    );
    //##AC beg
    ST_Expression_C* newCallExpr(
        const CG_Edipos* pEdp,
        const TCHAR*     pszCalee,
        CG_POUTYP        caleeTyp, 
        ST_CParamList*   pParList 
    );
    //##AC end


    ST_CSFCBody* addSFC();
    ST_CSFCBody* getSFC(){return m_pSFC;}
    //call this to clean up for next Action body
    void resetSTBodyTMP()
    {
        delete(m_pbt);
        m_pbt = new ST_CBodyTMP();
    }
    int getFreeTmp(CG_BITSET tbs, bool bIsReference)
    {
        return m_pbt->getFreeTmp(tbs, bIsReference);
    }
    
    int getFreeTriggerTmpNum()
    {
        return m_pBody->incUsedTriggers();
    }

    void freeTmpVar(int i)
    {
        m_pbt->freeTmpVar(i);
    }
 


    //statement create helpers:
    CG_STStmt* newEmptyStmt(
        const CG_Edipos*  pEdp
    );
    CG_STStmt* newLabel(
        const CG_Edipos*  pEdp,
        const TCHAR*      pszLbl, 
        const CG_STStmt*  pStmt
    );
    CG_STStmt* newAssignStmt(
        const CG_Edipos*  pEdp,  //id NULL it's called for an internal generated assign
        ST_Expression_C*  pLval, 
        ST_Expression_C*  pExpr
    );
    /* IL specific operation used to produce something like: i:=j:=3 */
    CG_STStmt* newAssignAccStmt(
        const CG_Edipos*   pEdp,  //id NULL it's called for an internal generated assign
        ST_Expression_C*   pLval, 
        const CG_ExprTree* pExpr
    );
    CG_STStmt* newIfElseStmt(
        const CG_Edipos*  pEdp,
        ST_Expression_C*  pIfExpr, 
        const CG_STStmt*        pThen,
        const CG_STStmt*        pElse
    );
    CG_STStmt* newCaseStmt(
        const CG_Edipos*  pEdp,
        ST_Expression_C*  pExpr, 
        const CG_STCaseList*    pList, 
        const CG_STStmt*        pElse
    );
    CG_STCaseList* newCase(
        ST_CCaseExpr*     pCaseList,
        CG_STStmt*        pFirstStmt
    );
    CG_STStmt* newCallStmt(
        ST_Expression_C* pCallExpr //##AC
    );
    CG_STStmt* newExitStmt(
        const CG_Edipos* pEdp
    );
    CG_STStmt* newReturnStmt(
        const CG_Edipos* pEdp
    );
    CG_STStmt* newTransAssignStmt(
        const CG_Edipos*  pEdp,
        ST_Expression_C*  pExpr
    );
    CG_STStmt* newNativeStmt(
        const CG_Edipos* pEdp,
        const TCHAR*     pszNative
    );
    CG_STStmt* newSynchronizedStmt(
        const CG_Edipos* pEdp,
        CG_STStmt*       pStmts
    );
    CG_STStmt* newGotoStmt(
        const CG_Edipos* pEdp,
        const TCHAR*     pszLbl
    );
    CG_STStmt* newForStmt(
        const CG_Edipos* pEdps, 
        ST_Identifier_C* pId, 
        ST_Expression_C* pFrom, 
        ST_Expression_C* pTo, 
        ST_Expression_C* pBy
    );
    CG_STStmt* newWhileStmt(
        const CG_Edipos* pEdps, 
        ST_Expression_C* pWhile, 
        CG_STStmt*       pStmts
    );
    CG_STStmt* newRepeatStmt(
        const CG_Edipos* pEdps, 
        CG_STStmt*       pStmts,
        ST_Expression_C* pUntil 
    );
    
    void checkControlStmts(
        const CG_Edipos* pEdp
    );

    void foundStmts(
        bool       bFound
    );
    void endOfSTBody(const CG_STStmt* pFirstStmt);
};





class ST_CSFCBody //helper class for SFC input 
{
private:
    st_ParseContext*       m_pBodyCon;
    CGT_CDiagMsg*          m_pdm;
    CG_CIECFront*          m_pFront;
    ST_CSymtabPrv*         m_pSymPrv; //sym tab of the sfc pou.
    ST_CBody*              m_pBody;   //this is the object of the outer st_ParseContext
    SFC_CHECKER*           m_pSFCChecker;
    bool                   m_inTransitionFrom;
    bool                   m_inTransitionTo;
    FC_CStrList*           m_pTransFromList;
    FC_CGArray <CG_Edipos> m_TransFromPos;
    FC_CStrList*           m_pTransToList;
    FC_CGArray <CG_Edipos> m_TransToPos;
    bool                   m_transSingleStepFrom;
    bool                   m_transSingleStepTo;
    CG_Edipos              m_transEdiPos;
    int                    m_transPrio;
    bool                   m_isNamedTrans;
    const ST_Identifier_C* m_transName;
    const ST_Identifier_C* m_stepName;
    const ST_Identifier_C* m_acbActionName;
    const ST_Identifier_C* m_acbQualifierName;
    long                   m_acbTime;
    int                    m_stPrio;
    FC_CStrList            m_boolActions;

public:
    ST_CSFCBody(
        st_ParseContext* pfc, 
        ST_CSymtabPrv*   pSymPrv,
        ST_CBody*        pBody
    );
    ~ST_CSFCBody();

    void sfcAddInitialStep(const ST_Identifier_C* pName);
    void sfcAddStep(const ST_Identifier_C* pName);
    void sfcStartStep();
    void sfcEndStep();
    void sfcAddActionAssociation(ST_Identifier_C* pName);
    void sfcAddActionAssociationQualifier(const ST_Identifier_C* pName);
    void sfcAddActionAssociationTime(const ST_Literal_C* pTime);
    void sfcAddAction(const ST_Identifier_C* pName);
    void sfcStartAction();
    void sfcEndAction(
        const CG_Edipos* pEdp,
        const CG_STStmt* pFirstActionStmt,
        const TCHAR* pszName,
        const CG_Edipos* pEdpId
    );
    void sfcStartTransition(const CG_Edipos* ppos);
    void sfcStartTransitionNamed(const ST_Identifier_C* pName);
    void sfcEndTransition(
        const CG_Edipos* pEdp,
        const CG_STStmt* pFirstTransiStmt
    );
    void sfcStartTransitionFrom();
    void sfcStartTransitionTo();
    void sfcAddTransitionStep(const ST_Identifier_C* pName, bool singleStep);
    void sfcAddTransitionPrio(int prio);

    int  isStep(const TCHAR* pname);
    int  isAction(const TCHAR* pname);
    SFC_CHECKER* getSFCChecker() {return m_pSFCChecker;}

    void endOfSFC(const CG_Edipos* pEndOfFile);
};




#endif // __ST_PARSECONTEXT_H_


