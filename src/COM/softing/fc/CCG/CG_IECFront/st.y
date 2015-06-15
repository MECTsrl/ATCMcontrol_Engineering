/* Structured Text & Common Elements Grammar according to IEC 1131-3 
 * (corrigenda 3 and Amendment included) 
 */

/*
 * the following features are implemented differently:
 * 1. array subscripts can be expressions of type ANY_INT
 * 2. function parameters can only be expressions (no assignments)
 */
%{


#include "st_ParseContext.h"
#include "il_ParseContext.h"
#include "configuration.h"
#include "CG_Messages.h"
#include "IECKeywords.h"
#include "utilif.h"
#include "utilsts.h"
#include "LDFBD.h"


#yy_movetop struct ST_TagVal //helper for sys comment tag="val" pairs 
#yy_movetop {
#yy_movetop     ST_Identifier_C* pTag;
#yy_movetop     ST_Identifier_C* pVal;
#yy_movetop };



%}

%union 
{
    CG_Edipos              edp;
    ST_Literal_C*          lit;
    ST_Identifier_C*       ident;
    ST_DirectVar_C*        direct;
    ST_Expression_C*       expr;
    ST_ExpressionList_C*   list;
    ST_CParamList*         paramlist; /*##AC*/
    CG_EXPRTYP             exprtyp;
    CG_DTYPES              btype;
    const CG_DType*        dtype;
    CG_BITSET              bitset; 
    WORD                   declmod;//Bit field of CG_DMOD_RETAIN,..
    ST_TagVal              tagval;
    ST_CAttrList*          attrlist;
    ST_CDeclList*          decllist; //list of ident and opt. attributes
    ST_StructInit_C*       structInit;
    ST_ArrayInit_C*        arrayInit;
    CG_POUTYP              poutyp;
    ST_CTaskInits*         taskinit;
    ST_CTaskList*          tasklist;
    CG_STStmt*             stmt;
    struct{
       CG_STStmt*          plistbegn;
       CG_STStmt*          plistlast;
    } stmtlist;
    struct{
       CG_STCaseList*      pcasebegn;
       CG_STStmt*          plistlast;
    } caselist;
    ST_CCaseExpr*          caserange;
    ST_CTypeOrList*        typeor;
    /* IL stuff */
    struct{
       IL_Instr*           pinstrbegn;
       IL_Instr*           pinstrlast;
    } instrlist;
    IL_Instr*              instr;
	struct {
	    ST_DirectVar_C*    optdirect;
	    ST_Identifier_C*   optsysvar;
	} optaddr;
};

%token<edp> '|' '+' '-' '*' '/' '(' ')' ':' '{' '}' '.'
%token<edp> MOD POW AND OR XOR NOT EQ NE LT LE GT GE 
%token<edp> BECOMES DOTDOT IF THEN ELSIF ELSE END_IF CASE OF END_CASE RETURN
%token<edp> WHILE DO END_WHILE REPEAT UNTIL END_REPEAT FOR TO BY END_FOR EXIT
%token<edp> ON TYPE END_TYPE STRUCT END_STRUCT PROGRAM END_PROGRAM FUNCTION
%token<edp> END_FUNCTION FUNCTION_BLOCK END_FUNCTION_BLOCK AT RIGHT_ARROW
%token<edp> CONFIGURATION END_CONFIGURATION RESOURCE END_RESOURCE TASK 
%token<edp> F_EDGE R_EDGE
%token<edp> WITH VAR_ACCESS READ_ONLY READ_WRITE PERIOD
%token<edp> ARRAY VAR VAR_INPUT VAR_OUTPUT VAR_IN_OUT VAR_EXTERNAL VAR_GLOBAL
%token<edp> VAR_TEMP VAR_IN_EXT VAR_CONFIG END_VAR RETAIN NON_RETAIN CONSTANT
%token<edp> STEP END_STEP INITIAL_STEP TRANSITION END_TRANSITION ACTION END_ACTION
%token<edp> FROM SYS_COMMENT_BEGIN SYS_COMMENT_END GOTO SYNCHRONIZED END_SYNCHRONIZED
%token<edp> DECL_CONST DECL_END_CONST END_NATIVE 
%token<edp> NATIVE VAR_NATIVE OVERLOAD KW_INLINE IMPORT_DIRECTIVE
%token<edp> WSTRING_BEG STRING_BEG
%token<ident>  IDENTIFIER
%token<direct> DIRECT_VARIABLE
%token<lit>    SIMPLE_LITERAL
%token<btype>  BASIC_TYP QVT_TYP

/* special start tokens */  
%token GVLCST_START
%token HDRFILE_START
%token EXPR_START
%token STACTION_START
%token BODY_START
%token IL_BODY_START

/* IL operators */
%token<instr> IL_OP_LD  IL_OP_LDN  IL_OP_ST  IL_OP_STN
%token<instr> IL_OP_NOT IL_OP_RPAR
%token<instr> IL_OP_S   IL_OP_R    IL_OP_S1  IL_OP_R1
%token<instr> IL_OP_CLK IL_OP_CU   IL_OP_CD  IL_OP_PV   IL_OP_IN  IL_OP_PT
%token<instr> IL_OP_AND IL_OP_OR   IL_OP_XOR IL_OP_ANDN IL_OP_ORN IL_OP_XORN
%token<instr> IL_OP_ADD IL_OP_SUB  IL_OP_MUL IL_OP_DIV  IL_OP_MOD
%token<instr> IL_OP_GT  IL_OP_GE   IL_OP_EQ  IL_OP_LT   IL_OP_LE  IL_OP_NE
%token<instr> IL_OP_CAL IL_OP_CALC IL_OP_CALCN
%token<instr> IL_OP_RET IL_OP_RETC IL_OP_RETCN
%token<instr> IL_OP_JMP IL_OP_JMPC IL_OP_JMPCN
%token<ident> IL_IDENTIFIER 

%left OR
%left XOR
%left AND
%left EQ NE
%left LT LE GT GE
%left '+' '-'
%left '*' '/' MOD
%left POW
%left NOT

%type <expr>        expression primary_expression
%type <expr>        signed_lit_or_const
%type <expr>        variable variable_name symbolic_variable
%type <expr>        multi_element_variable array_variable structured_variable
%type <expr>        opt_strlen opt_init initializer init_simple opt_BY
%type <paramlist>   opt_any_param_list any_param_list  /*##AC*/
%type <expr>        any_call /*##AC*/
%type <arrayInit>   array_initial_elements
%type <structInit>  structInit
%type <list>        opt_array_dim_list array_dim_list expression_list
%type <btype>       basic_type_edge basic_type_kw 
%type <dtype>       data_type opt_ret_type opt_ovRetType opt_cast
%type <typeor>      typeList opt_TypeList
%type <decllist>    decl_idlist opt_decl_idlist
%type <tagval>      AttribAssignment
%type <attrlist>    AttribList opt_sysComment
%type <ident>       fun_name opt_task_name  pouname opt_ResType 
%type <ident>       overload_body 
%type <exprtyp>     stop_token
%type <declmod>     decl_modifier decl_modifiers opt_decl_modifiers
%type <optaddr>     opt_addr
%type <poutyp>      poutyp_kw
%type <ident>       opt_NATIVE IDDotList
%type <lit>         LITERAL
%type <taskinit>    task_initialisation_list opt_task_initialisation_list
%type <tasklist>    task_configuration_list opt_task_configuration_list
%type <stmtlist>    statement_list labeled_statement
%type <stmt>        opt_statement_list statement empty_statement 
%type <stmt>        assignment_statement
%type <stmt>        elsif_statement_list opt_else_statement_list
%type <caselist>    case_element_list
%type <caserange>   case_list
%type <stmt>        fb_invocation if_statement case_statement for_statement for_assignmet
%type <stmt>        while_statement repeat_statement exit_statement return_statement
%type <stmt>        native_statement synchronized_statement goto_statement /*##AC*/
%type <declmod>     opt_CONSTANT
/* IL stuff */
%type <instrlist>   opt_instruction_list
%type <instrlist>   instruction_list
%type <instr>       il_instruction
%type <instr>       il_simple_operation il_expression 
%type <instr>       il_jump_operation il_fb_call /* il_formal_funct_call */
%type <instr>       il_simple_operator il_expr_operator
%type <instr>       il_call_operator il_jump_operator il_return_operator
%type <expr>        il_operand

%{
/* to avoid 1000 Warning of the same kind */
#pragma  warning( disable : 4309 4305 4102 4244)
%}


%%
file
 : POU_Decl
 | data_type_declaration
 | configuration_declaration
 | resource_declaration
 | GVLCST_START    gvlcst_file 
 | HDRFILE_START   POUheader   
 {
    pContext->endOfDecls(true);
 }
 | EXPR_START      expression  
 {
    pContext->m_pSingleExpression = $2;
 }
 | STACTION_START  opt_statement_list 
 { 
    assert(!pContext->m_pSTAction); 
    pContext->m_pSTAction = $2;
 } 
 | BODY_START      opt_pou_body2
 | IL_BODY_START   il_opt_pou_body2
 /* only to accept an empty file */
 | opt_sysComment  
 {
    pContext->handleEmptyFile();
 }
 ;

opt_sysComment
 : AttribList opt_comma SYS_COMMENT_END 
 { 
    $$ = $1;
 }
 | SYS_COMMENT_BEGIN SYS_COMMENT_END  { $$ = NULL;}
 | /* NIL */  { $$ = NULL;} 
 ;

opt_comma  /*TODO changed ',' usage syntax document it! now the last 
            *  tag=val pair may be optionally be followd by a comma
            *  this is usefull when deleting the line with the last pair.
            */
  : ','
  | /* NIL */
  ;



AttribList   
 : SYS_COMMENT_BEGIN AttribAssignment
 {
    $$ = new ST_CAttrList(&$1, pContext);
    if($2.pTag)
    {
        assert($2.pVal);
        $$->addAttr($2.pTag, $2.pVal);
    }
 }
 | AttribList ',' AttribAssignment
 {
    if($3.pTag)
    {
        assert($$ && $3.pVal);
        $$->addAttr($3.pTag, $3.pVal);
    }
 }
 ; 



AttribAssignment
 : IDDotList EQ WSTRING_BEG
 {
    $$.pVal = pContext->getAttribVal(&$3);
    if($$.pVal)
    {
        $$.pTag = $1;
    }
    else
    {
        /*error after unterm string */
        delete($1);
        $$.pTag = NULL;
    }
 } 
 | IDDotList EQ IDENTIFIER
 {
    $$.pTag = NULL;
    pContext->m_pdm->msg1(CG_E_ATTRIB_VALUE_NOT_STRING, $3->getEdp(), $1->getName());
 } 
 | IDDotList EQ
 {  
    $$.pTag = $1;
    $$.pVal = new ST_Identifier_C(&$2, pContext, _T(""));
 }
 ;

IDDotList
 : IDENTIFIER                 
 { 
    $$ = $1;
 }
 | IDENTIFIER '.' IDENTIFIER  
 {
    FC_CString jot(MAX_PATH);
    const CG_Edipos* pEdp1 = $1->getEdp();
    const CG_Edipos* pEdp2 = $3->getEdp();

    (jot<<$1->getName()).addc(_T('.'));
    
    if(pEdp1->lc.l!=pEdp2->lc.l || pEdp1->lc.c + jot.len() != pEdp2->lc.c)/*HACK?*/
    {
        /*for the moment don't allow too much!*/
        pContext->m_pdm->msg1(CG_E_SYNTAX_UNEXPECTED, &$2, 
			pContext->m_pdm->res1(CG_RES_STR_ST1));
    }
    jot<<$3->getName();
    $$ = new ST_Identifier_C(pEdp1, pContext, jot);
    delete($1);
    delete($3);
 } 
 ;
/*####################################################################################*/


/*####################################################################################*/
/*                              GVL or CST file content                               */
/*####################################################################################*/
gvlcst_file 
 : opt_sysComment
 { 
    //TODO check std attribs:
    if($1)
    {
        pContext->m_pSymtabPrv->registerAttributes($1->convertToStruct());
        delete($1);
    }
 } 
 opt_decl_sections  
 { 
    pContext->endOfDecls(false);
 }
 ;



/*####################################################################################*/
/*                              Data Type Declarations                                */
/*####################################################################################*/
data_type_declaration
 : opt_sysComment 
 {
    assert(!pContext->m_pSysCommentTMP);
    pContext->m_pSysCommentTMP = $1;
 }
 TYPE 
 {
    if(pContext->checkFileNameToPOUTyp(&$3, CG_POU_SDT)!=CG_POU_SDT)
        YYABORT;
 }
 type_declaration_list END_TYPE
 {
    pContext->endDeclSection(pContext->m_ulhdrEndTMP);
    pContext->endOfDecls(false);

 }
 ;

type_declaration_list
 : type_declaration ';'
 | type_declaration_list type_declaration ';'
 ;  

type_declaration
 : IDENTIFIER ':' STRUCT 
 {
    bool bOk = pContext->newPOU(pContext->m_pSysCommentTMP, CG_POU_SDT, $1, NULL);
    pContext->m_pSysCommentTMP = NULL;
    if(!bOk) 
        YYABORT;
 }
   opt_decl_list
   END_STRUCT
 | IDENTIFIER ':' IDENTIFIER 
   {pContext->m_pdm->msg0(CG_E_ONLY_DUT_IMPL, $1->getEdp());YYABORT;}
 | IDENTIFIER ':' ARRAY      
   {pContext->m_pdm->msg0(CG_E_ONLY_DUT_IMPL, $1->getEdp());YYABORT;}
 | IDENTIFIER ':' '('        
   {pContext->m_pdm->msg0(CG_E_ONLY_DUT_IMPL, $1->getEdp());YYABORT;}
 | IDENTIFIER ':' basic_type_kw '(' signed_lit_or_const DOTDOT signed_lit_or_const ')'
   {pContext->m_pdm->msg0(CG_E_ONLY_DUT_IMPL, $1->getEdp());YYABORT;}
 | error
 ;
/*####################################################################################*/



/*####################################################################################*/
/*                              Configuration Declarations                            */
/*####################################################################################*/
configuration_declaration
 : opt_sysComment CONFIGURATION IDENTIFIER
   { 
     CG_AttribList* pAttr = $1 ? $1->convertToStruct() : NULL;
     delete($1);
     if(!pContext->newCON($3->getEdp(), $3->getName(), pAttr))
         YYABORT; 
      delete($3);
   }
   opt_conf_decl_sections        /* currently an error if exist */
   opt_resource_declaration_list /* currently an error if not import */
   opt_access_declaration        /* currently an error if exist */
   opt_instance_specific_init    /* currently an error if exist */
   END_CONFIGURATION
 ;


opt_conf_decl_sections
 : VAR_GLOBAL
 {
     pContext->m_pdm->msg1(CG_E_NOT_SUPPORTED, &$1,
       pContext->m_pdm->res1(CG_RES_STR_ST2));
 }
 | /* NIL */
 ;

opt_instance_specific_init
 : VAR_CONFIG   
 { 
   pContext->m_pdm->msg1(CG_E_NOT_SUPPORTED, &$1, _T("VAR_CONFIG"));
   YYABORT;
 }
 | /* NIL */
 ;

opt_resource_declaration_list
 : resource_declaration_list
 | /* NIL */
 ;

resource_declaration_list
 : resource_declaration
 | IMPORT_DIRECTIVE       
 {
     TCHAR* psz = pContext->getImportFile(&$1);
     if(psz)
     {
        pContext->RESImport(&$1, psz); 
        delete(psz);
     }
 }
 | resource_declaration_list resource_declaration
 | resource_declaration_list IMPORT_DIRECTIVE
 {
     TCHAR* psz = pContext->getImportFile(&$2);
     if(psz)
     {
        pContext->RESImport(&$2, psz); 
        delete(psz);
     }
 }
 ;

resource_declaration
 : opt_sysComment RESOURCE IDENTIFIER opt_sysComment opt_ResType
   {
        if(pContext->m_srcType == CG_SRC_TYPE_CON)
        { 
            pContext->m_pdm->msg1(CG_E_NOT_SUPPORTED, &$2, 
				pContext->m_pdm->res1(CG_RES_STR_ST3));
            YYABORT;
        }
        if(!pContext->newRES($1, $3, $4, $5))
            YYABORT;
   }
   opt_decl_sections
   opt_task_configuration_list
   opt_program_configuration_list
   END_RESOURCE
   {
        assert(pContext->m_pCurrTaskList == $8);
        pContext->m_pCurrTaskList = NULL;
        pContext->endOfResource($8);
   }
 ;

opt_ResType 
 : ON IDENTIFIER { $$ = $2;}
 | ON WSTRING_BEG{ $$ = pContext->getAttribVal(&$2); }
 | /* NIL */     { $$ = NULL;}
 ; 

opt_task_configuration_list 
 : task_configuration_list   { $$ = $1; }
 | /* NIL */                 { $$ = NULL; }
 ;

task_configuration_list
 : TASK IDENTIFIER opt_sysComment '(' opt_task_initialisation_list ')' ';'
 {
    $$ = new ST_CTaskList(pContext, $2, $3, $5);
    pContext->m_pCurrTaskList = $$;
 }
 | task_configuration_list TASK IDENTIFIER opt_sysComment '(' opt_task_initialisation_list ')' ';'
 {
    $1->addTask(pContext, $3, $4, $6);
 }
 ;

opt_task_initialisation_list
 : task_initialisation_list  { $$ = $1; }
 | /* NIL */                 { $$ = NULL; }
 ;

task_initialisation_list
 : IDENTIFIER BECOMES expression 
 { 
    $$ = new ST_CTaskInits(pContext, &pContext->m_pFront->m_exprMem, $1, $3);
 }
 | task_initialisation_list ',' IDENTIFIER BECOMES expression 
 {
    $1->addPair(pContext, &pContext->m_pFront->m_exprMem, $3, $5);
 }
 ;


opt_program_configuration_list 
 : program_configuration_list
 | /* NIL */
 ;

program_configuration_list
 : program_configuration ';'   
 | program_configuration_list program_configuration ';'
 ;

program_configuration
 : PROGRAM opt_decl_modifiers IDENTIFIER opt_sysComment opt_task_name ':' IDENTIFIER opt_conf_elements
 { 
    pContext->attachPrgToTask($2, $3, $4, $5, $7); 
 }
 ;

opt_task_name 
 : WITH IDENTIFIER { $$ = $2; }
 |                 { $$ = NULL;}
 ;

opt_conf_elements
 : '(' conf_elements ')' 
 { 
    pContext->m_pdm->msg1(CG_E_NOT_SUPPORTED, &$1, 
		pContext->m_pdm->res1(CG_RES_STR_ST4)); 
 }
 | /* NIL */
 ;

conf_elements
 : conf_element
 | conf_elements ',' conf_element
 ;

conf_element
 : IDENTIFIER WITH IDENTIFIER
 | IDENTIFIER BECOMES expression
 | IDENTIFIER RIGHT_ARROW expression
 ;


opt_access_declaration
 : VAR_ACCESS  
 {
   pContext->m_pdm->msg1(CG_E_NOT_SUPPORTED, &$1, _T("VAR_ACCESS"));
   YYABORT;
 }
 | /* NIL */
 ;
/*####################################################################################*/



/*####################################################################################*/
/*   PROGRAM, FB and FUNCTION        */
/*####################################################################################*/
POU_Decl
 : POUheader
   opt_pou_body
 ;


POUheader:
  opt_sysComment opt_NATIVE poutyp_kw pouname opt_ret_type
  { 
    if($3==CG_POUTYP_UNDEF || !$5)
        YYABORT; 
    if($3!=CG_POU_FUN || $5->type==CG_VOID)
    {
        //HACK $4->m_FilePos is null for MOD,AND,OR,XOR but they have decl sections...
        pContext->m_ulhdrEndTMP = $4->m_FilePos; 
    }
    if(!pContext->newPOU($1, $3, $4, $5)) 
        YYABORT;
    assert(pContext->m_ulhdrEndTMP > 0);
    pContext->m_pSymtabPrv->setNewHdrIFEnd(pContext->m_ulhdrEndTMP, true);
    if($2)
    {
        //convert to new syntax:
        FC_CString jot;
        jot<<_T("#movetop")<<$2->getName();
        pContext->m_pSymtabPrv->registerVarNative($2->getEdp(), jot);
        delete($2);
    }
  }
  opt_decl_sections
  ;    


poutyp_kw
  : PROGRAM        { $$ = pContext->checkFileNameToPOUTyp(&$1, CG_POU_PRG);}
  | FUNCTION_BLOCK { $$ = pContext->checkFileNameToPOUTyp(&$1, CG_POU_FB);}
  | FUNCTION       { $$ = pContext->checkFileNameToPOUTyp(&$1, CG_POU_FUN);}
  ; 
   
opt_ret_type
 : ':' data_type { $$ = $2; }
 | /* NIL */     { $$ = pContext->m_pFront->registerDType(CG_VOID, -1); }
 ;

/* some keywords may be fun names too, also NOT but NOT called as operator 
   in ST only FBD may use NOT as a function */
pouname
 : fun_name { $$ = $1; }
 | NOT      { $$ = new ST_Identifier_C(&$1, pContext, _T("NOT")); }
 ;

fun_name
 : IDENTIFIER   { $$ = $1;}
 | MOD          { $$ = new ST_Identifier_C(&$1, pContext, _T("MOD"));}     
 | AND          { $$ = new ST_Identifier_C(&$1, pContext, _T("AND"));}
 | OR           { $$ = new ST_Identifier_C(&$1, pContext, _T("OR"));}
 | XOR          { $$ = new ST_Identifier_C(&$1, pContext, _T("XOR"));}
 ;


/* BODY syntax is common for all opt_pou_body2 used to parse a body 
 * but skipping the header part. opt_pou_body2 does not call the
 * endOfDecls function and has no inline body syntax.
 */
opt_pou_body2
 : pou_body2 END_FUNCTION_BLOCK {pContext->endOfPou(&$2, CG_POU_FB);}
 | pou_body2 END_FUNCTION       {pContext->endOfPou(&$2, CG_POU_FUN);}
 | pou_body2 END_PROGRAM        {pContext->endOfPou(&$2, CG_POU_PRG);}
 | /* NIL */                    {pContext->endOfPou(pContext->getCurrEdp(), CG_POUTYP_UNDEF);} 
 ;
il_opt_pou_body2
 : il_pou_body2 END_FUNCTION_BLOCK {pContext->endOfPou(&$2, CG_POU_FB);}
 | il_pou_body2 END_FUNCTION       {pContext->endOfPou(&$2, CG_POU_FUN);}
 | il_pou_body2 END_PROGRAM        {pContext->endOfPou(&$2, CG_POU_PRG);}
 | /* NIL */                    {pContext->endOfPou(pContext->getCurrEdp(), CG_POUTYP_UNDEF);} 
 ;

pou_body2
 : opt_statement_list           {pContext->endOfSTBody($1);}
 | sfc_element_list 
 ; 

il_pou_body2
 : opt_instruction_list         
 {
    assert(pContext->m_srcType==CG_SRC_TYPE_IL);
    pContext->m_IlContext.m_Instrs = $1.pinstrbegn;
    pContext->endOfSTBody(pContext->m_IlContext.transformToST());
 }
 ; 


opt_pou_body
 : pou_body END_FUNCTION_BLOCK {pContext->endOfPou(&$2, CG_POU_FB);}
 | pou_body END_FUNCTION       {pContext->endOfPou(&$2, CG_POU_FUN);}
 | pou_body END_PROGRAM        {pContext->endOfPou(&$2, CG_POU_PRG);}
 | /* NIL */                   
 {
    if(!pContext->endOfDecls(true))
       YYABORT;
    pContext->endOfPou(pContext->getCurrEdp(), CG_POUTYP_UNDEF);
 } 
 ;

pou_body
 : {
     if(!pContext->endOfDecls(true))
        YYABORT;
   }
   opt_statement_list      { pContext->endOfSTBody($2);}
 | {
     if(!pContext->endOfDecls(true))
        YYABORT;
   }
   sfc_element_list        
 | overloads
 {
    pContext->endOfDecls(false);
 }
 | {
     if(!pContext->endOfDecls(true))
        YYABORT;
   }
   instruction_list
   {
    assert(pContext->m_srcType==CG_SRC_TYPE_IL);
    pContext->m_IlContext.m_Instrs = $2.pinstrbegn;
    pContext->endOfSTBody(pContext->m_IlContext.transformToST());
   }
 ;

overloads
 :
 {  
     assert(!pContext->m_bDontCheckBackendSupportedTypes);
     pContext->m_bDontCheckBackendSupportedTypes = true;
 }
 overloadlist
 {  
     assert(pContext->m_bDontCheckBackendSupportedTypes);
     pContext->m_bDontCheckBackendSupportedTypes = false;
 }
 ;

overloadlist
 :
   overload
 | overloadlist overload
 ;


overload
 : OVERLOAD opt_ovRetType '(' opt_TypeList ')' overload_body {}
 {
    pContext->m_pSymtabPrv->registerMacroBody(
        &$1,
        true, 
        pContext->m_typeorRetTMP,
        $2,
        $4 ? $4->m_iArgCount    : -1,
        $4 ? $4->m_pArgTypeSets : NULL,
        $4 ? $4->m_optCastToType: NULL,
        $4 ? $4->m_bExtended    : false,
        $6->getEdp(),
        $6->getName()
       );//alles klar?
       delete($4);
       delete($6);
 }
 | KW_INLINE '{' /*}*/
 {
    const TCHAR* psz = pContext->getSkippedBraceText(&$2, '{'/*}*/);
    if(!psz)
        YYABORT;

    pContext->m_pSymtabPrv->registerMacroBody(
        &$1,
        false, 
        CG_TBS_EMPTY,
        NULL,
        0,
        NULL,
        NULL,
        false,
        &$2,
        psz
       );//alles klar?
 }
 | KW_INLINE stop_token
 {
    pContext->m_pSymtabPrv->registerOpAlias(pContext->getCurrEdp(),$2);
 }
 ;

overload_body
 : KW_INLINE '{' /*}*/
 {
    const TCHAR* psz = pContext->getSkippedBraceText(&$2, '{'/*}*/);
    if(!psz)
        YYABORT;
    $$ = new ST_Identifier_C(&$2, pContext, psz);
 }
 | 
 {
    pContext->m_pdm->msg0(CG_E_BODY_IN_POLY_FUN, pContext->getCurrEdp());
    YYABORT;
 } statement_list  { $$ = NULL; }
 ;

opt_TypeList
 : typeList              { $$ = $1;}
 | typeList  ',' PERIOD  { $$ = $1; $$->m_bExtended = true;}
 | /* NIL */             { $$ = NULL;}
 ;

typeList
 : data_type_or opt_cast 
 {
    $$ = new ST_CTypeOrList(pContext->getCurrEdp(), pContext);
    $$->addType(pContext->getCurrEdp(), pContext->m_typeorTMP, $2);
 }
 | typeList ',' data_type_or opt_cast
 {
    ($$ = $1)->addType(pContext->getCurrEdp(), pContext->m_typeorTMP, $4);
 }
 ;



opt_ovRetType 
 : data_type_or  
 { 
    $$ = NULL;
    pContext->m_typeorRetTMP = pContext->m_typeorTMP; 
 }
 |  basic_type_kw TO data_type_or
 {
    if(!CG_IsMonomorph($1) || CG_IsUserTyp($1))
    {
        pContext->m_pdm->msg1(CG_E_ANYTYPE_BAD_USE, &$2, CGT_DTYPES_ToString($1));
        $$ = NULL;
    }
    else
    {
        $$ = pContext->m_pFront->registerDType($1, -1); 
    }
    pContext->m_typeorRetTMP = pContext->m_typeorTMP; 
 } 
 | /* NIL */
 {
    $$ = NULL;
    pContext->m_typeorRetTMP = CG_TBS_VOID; 
 }
 ;

opt_cast
 : TO basic_type_kw 
 { 
    if(!CG_IsMonomorph($2) || CG_IsUserTyp($2))
    {
        pContext->m_pdm->msg1(CG_E_ANYTYPE_BAD_USE, 
            pContext->getCurrEdp(), CGT_DTYPES_ToString($2));
        $$ = NULL;
    }
    else
    {
        $$ = pContext->m_pFront->registerDType($2, -1); 
    }
 }
 |  /* NIL */       
 { 
    $$ = NULL;
 }
 ;

data_type_or
 : data_type                  
 { 
    if($1) //may be error! ##AC
        pContext->m_typeorTMP.initFromCG_DType($1);
    else
        assert(pContext->getErrorsInFile()>0);
 }
 | data_type_or '|' data_type 
 {
    if($3) //may be error! ##AC
 { 
    if(pContext->m_typeorTMP.hasExtTypeDescr() || CG_HasExtTypeDescr($3))
    {
        pContext->m_pdm->msg1(CG_E_NOT_SUPPORTED, 
            &$2, pContext->m_pdm->res1(CG_RES_STR_ST6));
    }
    else
    {
        pContext->m_typeorTMP = pContext->m_typeorTMP.GetTypeSet() | $3->tbs;
    }
 }
    else
        assert(pContext->getErrorsInFile()>0);
 }
 ;


stop_token
 : BECOMES  { $$ = CG_MOVE;}
 | OR       { $$ = CG_OR;  }
 | XOR      { $$ = CG_XOR; }
 | AND      { $$ = CG_AND; }
 | EQ       { $$ = CG_EQ;  }
 | NE       { $$ = CG_NE;  }
 | LT       { $$ = CG_LT;  }
 | LE       { $$ = CG_LE;  }
 | GT       { $$ = CG_GT;  }
 | GE       { $$ = CG_GE;  }
 | '+'      { $$ = CG_ADD; }
 | '-'      { $$ = CG_SUB; }
 | '*'      { $$ = CG_MUL; }
 | '/'      { $$ = CG_DIV; }
 | MOD      { $$ = CG_MOD; }
 | POW      { $$ = CG_POW; }
 | NOT      { $$ = CG_NOT; }
 | '(' ')'  { $$ = CG_NOOP; }
 ;
/*####################################################################################*/







/*####################################################################################*/
/*                          General Variable Declaration Syntax                       */
/*####################################################################################*/
opt_decl_sections
 : decl_sections
 | /* NIL */
 ;

decl_sections
 : decl_section 
 | decl_sections decl_section 
 | error
 ;

decl_section
 : 
 { 
    if(pContext->m_srcType==CG_SRC_TYPE_IL) 
        pContext->m_IlContext.m_bParseBody = false;
 }
   decl_scope 
 { pContext->m_bDeclSecHasAddr = false; }
   opt_decl_list
 {  pContext->endDeclSection(pContext->m_ulhdrEndTMP); }
   END_VAR 
 {  
    if(pContext->m_srcType==CG_SRC_TYPE_IL) 
        pContext->m_IlContext.m_bParseBody = true;
 }
 | VAR_NATIVE 
 { 
    const TCHAR* psz;
    pContext->newDeclScope(&$1, CG_VAR_NATIVE,  0); 
    psz  = pContext->getSkippedText(&$1, _T("END_VAR"));
    if(!psz)
        YYABORT;
    pContext->m_ulhdrEndTMP = UTIL_STS_GetFPosOfNextChar(pContext->m_pScanner);
    pContext->m_pSymtabPrv->registerVarNative(&$1, psz);
    pContext->endDeclSection(pContext->m_ulhdrEndTMP);
 }
 | IMPORT_DIRECTIVE 
 {
    TCHAR* psz = pContext->getImportFile(&$1);
    if(psz)
    {
        pContext->GVLImport(&$1, psz);
        pContext->m_ulhdrEndTMP = UTIL_STS_GetFPosOfNextChar(pContext->m_pScanner);
        pContext->endDeclSection(pContext->m_ulhdrEndTMP);
        delete(psz);
    }
 }
 | DECL_CONST {pContext->newDeclScope(&$1, CG_CONST,  0);}
   opt_decl_list 
   DECL_END_CONST { pContext->endDeclSection(pContext->m_ulhdrEndTMP); }
 ;


opt_decl_list
 : decl_list
 | /* NIL */
 ; 

decl_list
 : decl ';'
 | decl_list decl ';'
 ;

decl
 : opt_decl_idlist opt_addr ':' data_type opt_CONSTANT opt_init
 { 
     pContext->appendDeclListToSymTab($1, $2.optdirect, $2.optsysvar ? $2.optsysvar->getName() : NULL, $4, $5, $6);
	 delete($2.optsysvar);
 }
 | error
 ;

opt_CONSTANT  /* IEC Extension: allow CONSTANT modifier at struct member decl */
 : CONSTANT  { $$ = CG_DMOD_CONSTANT; }
 | /* NIL */ { $$ = 0; }
 ;

opt_addr 
 : AT  DIRECT_VARIABLE  
 { 
	$$.optdirect = $2; 
	$$.optsysvar = NULL; 
	pContext->m_bDeclSecHasAddr = true;
 }
 | AT  IDENTIFIER       
 {
	$$.optdirect = pContext->directAddrFromSysVar($2->getEdp(), $2->getName()); 
	$$.optsysvar = $2; 
	pContext->m_bDeclSecHasAddr = true;
  }
 |  /* NIL */           
 { 
    $$.optdirect = NULL; 
    $$.optsysvar = NULL;
 }
 ;

opt_decl_idlist
 :  decl_idlist { $$ = $1;}
 |  /* NIL */   { $$ = NULL;}  
 ;

decl_idlist
 : IDENTIFIER opt_sysComment
 { 
   $$ = new ST_CDeclList($1->getEdp(), pContext);
   $$->addIdAttr($1, $2); 
 }
 | decl_idlist ',' IDENTIFIER opt_sysComment 
 { 
   $1->addIdAttr($3, $4); 
 }
 ; 

opt_NATIVE
 : NATIVE    
 { 
    const TCHAR* psz = pContext->getSkippedText(&$1, _T("END_NATIVE"));
    if(!psz)
        YYABORT;
    $$ = new ST_Identifier_C(&$1, pContext, psz);
 }
 | /* NIL */ { $$ = NULL;}  
 ;

opt_init
 : BECOMES initializer {$$ = $2;}
 |                     {$$ = NULL;}
 ;

initializer
 :  expression                    {$$ = $1;}
 | '[' array_initial_elements ']' {$$ = $2;} 
 | '(' structInit ')'             {$$ = $2;} 
 ;

structInit
 :  IDENTIFIER BECOMES initializer                
 {
    $$ = new ST_StructInit_C($1->getEdp(), pContext, $1,$3);
 }
 |  structInit ',' IDENTIFIER BECOMES initializer 
 {
    $$->append($3,$5);
 }
 ;

array_initial_elements
 : init_simple                                          
 {
    $$ = new ST_ArrayInit_C($1->getEdp(), pContext, NULL,$1);
 }
 | signed_lit_or_const '(' array_initial_elements ')'   
 {
    $$ = new ST_ArrayInit_C($1->getEdp(), pContext, $1,  $3);
 }
 | signed_lit_or_const '('                        ')'   
 {
    $$ = new ST_ArrayInit_C($1->getEdp(), pContext, $1,  NULL);
 }
 | '(' expression ')'  '(' array_initial_elements ')'   
 {
    $$ = new ST_ArrayInit_C(&$1, pContext, $2,  $5);
 }
 | '(' expression ')'  '('                        ')'   
 {
    $$ = new ST_ArrayInit_C(&$1, pContext, $2,  NULL );
 }
 | array_initial_elements ',' init_simple                                        
 {
    $$->append(NULL, $3);
 }
 | array_initial_elements ',' signed_lit_or_const '(' array_initial_elements ')' 
 {
    $$->append($3, $5);
 }
 | array_initial_elements ',' signed_lit_or_const '('                        ')' 
 {
    $$->append($3, NULL);
 }
 | array_initial_elements ',' '(' expression ')'  '(' array_initial_elements ')' 
 {
    $$->append($4, $7);
 }
 | array_initial_elements ',' '(' expression ')'  '('                        ')' 
 {
    $$->append($4, NULL);
 }
 ;

init_simple
 :  signed_lit_or_const  {$$ = $1;}
 | '(' expression ')'    {$$ = $2;}
 | '(' structInit ')'    {$$ = $2;}
 ;

signed_lit_or_const
 :     LITERAL      {$$ = $1;}
 | '+' LITERAL      {$$ = pContext->checkUPlusMinus(&$1, $2, false);}
 | '-' LITERAL      {$$ = pContext->checkUPlusMinus(&$1, $2, true);}
 | IDENTIFIER       {$$ = pContext->createVar($1);}
 | '+' IDENTIFIER   {$$ = pContext->checkUPlusMinus(&$1, pContext->createVar($2), false);}
 | '-' IDENTIFIER   {$$ = pContext->checkUPlusMinus(&$1, pContext->createVar($2), true);}
 ;


  
data_type /* if a fatal error in type return NULL e.g. undefined POU */
 : basic_type_edge opt_strlen
 { 
    long    lStrLen  = -1;
    bool    bStrLenOk= true;
    if($2)
    {
        if($1!=CG_DTYPES_UNDEF)
            lStrLen = pContext->checkTypeStrLen($1, $2);
        bStrLenOk = lStrLen>=0;;
    }
    
    assert($1!=CG_DTYPES_UNDEF || pContext->getErrorsInFile()>0);
    if($1!=CG_DTYPES_UNDEF && bStrLenOk)
        $$ = pContext->m_pFront->registerDType($1, lStrLen);
    else
        $$ = NULL;
 }
 | IDENTIFIER opt_strlen
 {
    const CG_Symtab* pSym;
    $$ = NULL;

    if(!$2)
        pContext->m_ulhdrEndTMP = $1->m_FilePos;

    //##AC beg
    const TCHAR*         pszType = $1->getName();
    if(pContext->pSymtab && pContext->pSymtab->flags & CG_STAB_FORCEACTASK) //d-306: ->pSymtab may be 
    {                                                                       //NULL if parsing function return type
        //todo check
        if(!_tcsicmp(pszType, "ANY_WAITABLE")) // TODO make real keywords 
            $$ = pContext->m_pFront->registerDType(CG_ANY_WAITABLE, -1);
        else if(!_tcsicmp(pszType, "GOTOLABEL"))
            $$ = pContext->m_pFront->registerDType(CG_GOTOLABEL, -1);
        else if(!_tcsicmp(pszType, "VOID"))
            $$ = pContext->m_pFront->registerDType(CG_VOID, -1);
    }
    
    if(!$$)
    {
        pSym = pContext->getSymtabForDType($1->getEdp(), pszType, ST_WANT_FB|ST_WANT_SDT);
        if(pSym)
            $$ = pContext->m_pFront->registerDType(pSym);
    }

    if($2)
    {
        pContext->m_pdm->msg0(CG_E_BAD_STRLENGTH_TYPE, $2->getEdp());
        delete($2);
    }
    //##AC end
    delete($1);
 }
 | ARRAY opt_array_dim_list OF basic_type_edge opt_strlen
 {
    CG_Subrange ranges[CG_MAX_ARRAY_DIM];
    int         nDims  = 0;
    bool        bDimOk = true;
    long        lStrLen= -1;
    bool        bStrLenOk = true;

    $$ = NULL;
    if($2)
    {
       nDims = pContext->checkArrayDims($2, ranges);
       bDimOk= nDims>0;
    }
    if($5)
    {
        if($4!=CG_DTYPES_UNDEF)
            lStrLen = pContext->checkTypeStrLen($4, $5);
        bStrLenOk = lStrLen>=0;
    }

    assert($4!=CG_DTYPES_UNDEF || pContext->getErrorsInFile()>0);
    if(bDimOk && bStrLenOk && $4!=CG_DTYPES_UNDEF)
    {
        $$ = pContext->m_pFront->registerDType(
                      nDims, ranges, $4, lStrLen, NULL);
    }
 } 
 | ARRAY opt_array_dim_list OF IDENTIFIER opt_strlen
 {
    CG_Subrange      ranges[CG_MAX_ARRAY_DIM];
    const CG_Symtab* pSym;
    int              nDims = 0;
    bool             bDimOk = true;

    if(!$5)
        pContext->m_ulhdrEndTMP = $4->m_FilePos;

    $$ = NULL;
    if($2)
    {
       nDims = pContext->checkArrayDims($2, ranges);
       bDimOk= nDims>0;
    }

    pSym = pContext->getSymtabForDType($4->getEdp(), $4->getName(), ST_WANT_SDT);

    if($5)
    {
        pContext->m_pdm->msg0(CG_E_BAD_STRLENGTH_TYPE, $5->getEdp());
        delete($5);
    }
    else if(pSym && bDimOk)
    {
        $$ = pContext->m_pFront->registerDType(
                    nDims, ranges, CG_STRUCT, -1, pSym); 
    }
    delete($4);
 } 
 ;

opt_array_dim_list 
 : '[' array_dim_list ']' { $$ = $2;}  
 |  /* NIL */             { $$ = NULL;}  
 ;

array_dim_list
 : expression DOTDOT expression
 { 
    $$ = new ST_ExpressionList_C(pContext, $1); 
    $3->Detach(); 
    $$->m_list.add($3); //##AC
 }
 | array_dim_list ',' expression DOTDOT expression 
 { 
    pContext->m_pdm->msg0(CG_E_ONLY_ONE_DIM, $3->getEdp()); 
 }
 ;

opt_strlen
 : '[' expression ']' { $$ = $2; }
 | /* NIL */          { $$ = NULL; }
 ;

basic_type_edge
 : basic_type_kw 
 {
    $$ = $1;
 }
 | basic_type_kw R_EDGE 
 {
    if($1==CG_BOOL)
    {
        $$ = CG_BOOL_R_EDGE;
    }
    else
    {
        pContext->m_pdm->msg0(CG_E_ILLUSE_EDGE, &$2);
        $$ = CG_DTYPES_UNDEF;
    }
 }
 | basic_type_kw F_EDGE 
 {
    if($1==CG_BOOL)
    {
        $$ = CG_BOOL_F_EDGE;
    }
    else
    {
        pContext->m_pdm->msg0(CG_E_ILLUSE_EDGE, &$2);
        $$ = CG_DTYPES_UNDEF;
    }
 }
 ;

basic_type_kw
 : BASIC_TYP { $$ = $1; }
 | QVT_TYP   { $$ = $1; }
 ;



 
decl_scope
 : VAR_GLOBAL   opt_decl_modifiers {pContext->newDeclScope(&$1, CG_VAR_GLOBAL,  $2);}
 | VAR          opt_decl_modifiers {pContext->newDeclScope(&$1, CG_VAR,         $2);}
 | VAR_INPUT    opt_decl_modifiers {pContext->newDeclScope(&$1, CG_VAR_INPUT,   $2);}
 | VAR_OUTPUT   opt_decl_modifiers {pContext->newDeclScope(&$1, CG_VAR_OUTPUT,  $2);}
 | VAR_IN_OUT   opt_decl_modifiers {pContext->newDeclScope(&$1, CG_VAR_IN_OUT,  $2);}
 | VAR_EXTERNAL opt_decl_modifiers {pContext->newDeclScope(&$1, CG_VAR_EXTERNAL,$2);}
 | VAR_TEMP     opt_decl_modifiers {pContext->newDeclScope(&$1, CG_VAR_TEMP,    $2);}
 | VAR_IN_EXT   opt_decl_modifiers {pContext->newDeclScope(&$1, CG_VAR_IN_EXT,  $2);}
 ;

opt_decl_modifiers
 : decl_modifiers {$$ = $1;}
 | /* NIL */      {$$ = 0;}
 ;

decl_modifiers
 : decl_modifier   { $$ = $1;}
 | decl_modifiers decl_modifier
 {
    if(($1 & $2)!=0 || (($1 | $2) & CG_DMOD_ANY_RETAIN)==CG_DMOD_ANY_RETAIN)
    {
        pContext->m_pdm->msg0(CG_E_MODIFIER_TWICE, pContext->getCurrEdp());
        $$ = $1;
    }
    else
    {
        $$ = $1 | $2;
    }
 }
 ;

decl_modifier
 : RETAIN      { $$ = CG_DMOD_RETAIN;}
 | NON_RETAIN  { $$ = CG_DMOD_NON_RETAIN;}
 | CONSTANT    { $$ = CG_DMOD_CONSTANT;}
 ; 
/*####################################################################################*/







/*####################################################################################*/
/*                          SFC Declarations                                          */
/*####################################################################################*/
sfc_element_list
 : sfc_element
 | sfc_element_list sfc_element
 
 ;

sfc_element
 : initial_step
 | step
 | transition
 | action
 ;


step
 : 
  STEP IDENTIFIER ':' 
  {
        pContext->addSFC();
        pContext->m_pSFC->sfcAddStep($2);
        pContext->m_pSFC->sfcStartStep();
  }
  opt_action_association_list
  END_STEP
  {
        pContext->m_pSFC->sfcEndStep();
  }
 ;

initial_step
 : 
  INITIAL_STEP IDENTIFIER ':' 
  {
      pContext->addSFC();
      pContext->m_pSFC->sfcAddInitialStep($2);
      pContext->m_pSFC->sfcStartStep();
  }
  opt_action_association_list
  END_STEP
  {
      pContext->m_pSFC->sfcEndStep();
  }

 ;

transition
    /* unnamed transition */ 
 :  TRANSITION 
    {
       pContext->addSFC();
       pContext->m_pSFC->sfcStartTransition(&$1);
    }
    FROM
    {
       pContext->m_pSFC->sfcStartTransitionFrom();
    } 
    steps 
    TO 
    {
       pContext->m_pSFC->sfcStartTransitionTo();
    }
    steps 
    BECOMES expression ';'
    END_TRANSITION
    {
       const CG_STStmt* pTrn;
       pTrn = pContext->newTransAssignStmt($10->getEdp(), $10);
       pContext->m_pSFC->sfcAddTransitionPrio(-1);
       pContext->m_pSFC->sfcEndTransition(&$12, pTrn);
    }
    
    /* named transition definition and reference, 
     * may be only one with the same IDENTIFIER in one SFC 
     */
 |  TRANSITION IDENTIFIER 
    {
       pContext->addSFC();
       pContext->m_pSFC->sfcStartTransitionNamed($2);
    }
    FROM 
    {
       pContext->m_pSFC->sfcStartTransitionFrom();
    } 
    steps 
    TO 
    {
       pContext->m_pSFC->sfcStartTransitionTo();
    }
    steps 
    BECOMES expression ';'
    END_TRANSITION
    {
       const CG_STStmt* pTrn;
       pTrn = pContext->newTransAssignStmt($11->getEdp(), $11);
       pContext->m_pSFC->sfcEndTransition(&$13, pTrn);
    }
   

    /* named transition reference, may be more references
     * of IDENTIFIER in one SFC
     */
/* |  TRANSITION IDENTIFIER 
    {
      //  pContext->ppc->addSFC();
    }
    FROM steps TO steps ';' 
*/
 ;


steps
 : IDENTIFIER
 {
   pContext->m_pSFC->sfcAddTransitionStep($1, true);
 }
 | '(' IDENTIFIER ',' step_name_list ')'
 {
   pContext->m_pSFC->sfcAddTransitionStep($2, false);
 }
 ;

step_name_list
 : IDENTIFIER
 {
   pContext->m_pSFC->sfcAddTransitionStep($1, false);
 }
 | step_name_list ',' IDENTIFIER
 {
   pContext->m_pSFC->sfcAddTransitionStep($3, false);
 }
 ;
 

action
 : ACTION IDENTIFIER ':'
   {
     pContext->addSFC();
     pContext->m_pSFC->sfcAddAction($2);
     pContext->m_pSFC->sfcStartAction();
   }
   opt_statement_list
   END_ACTION
   {
     pContext->m_pSFC->sfcEndAction(&$6, $5, $2->getName(), $2->getEdp());
     delete $2;
   }
 ;  

opt_action_association_list
 : action_association_list
 | /* NIL */
 ;

action_association_list
 : action_association ';'
 | action_association_list action_association ';'
 ;

action_association
 : IDENTIFIER '(' action_qualifier_list ')'
 {
    pContext->m_pSFC->sfcAddActionAssociation($1);
 }
 ;

action_qualifier_list
 : IDENTIFIER /* possible values are: N,R,S,P,P0,P1 or time quali L,D,SD,DS,SL */
  {
    pContext->m_pSFC->sfcAddActionAssociationQualifier($1);
    pContext->m_pSFC->sfcAddActionAssociationTime(0);
  }
  |IDENTIFIER ',' LITERAL
  {
    pContext->m_pSFC->sfcAddActionAssociationQualifier($1);
    pContext->m_pSFC->sfcAddActionAssociationTime($3);
  }
  ; 
/*
 | action_qualifier_list ',' LITERAL
 | action_qualifier_list ',' '+' LITERAL
 | action_qualifier_list ',' '-' LITERAL
 ;
 | action_qualifier_list ',' IDENTIFIER
 ;
 */
/*####################################################################################*/








/*####################################################################################*/
/*                          ST++                                                      */
/*####################################################################################*/
opt_statement_list
 : statement_list 
 {
    $$=$1.plistbegn; 
    pContext->foundStmts(true);
 }
 | /* NIL */      
 {
    $$ = NULL; 
    pContext->foundStmts(false);
 }
 ;


statement_list
 : labeled_statement ';'                 
 { 
    $$.plistbegn = $1.plistbegn; 
    $$.plistlast = $1.plistlast;
    assert(pContext->getErrorsInFile()>0 || !$$.plistlast->pNext); 
 }
 | statement_list labeled_statement ';'  
 { 
    if($1.plistbegn && $1.plistlast && $2.plistbegn && $2.plistlast)
    {
        //optimise  ;;;;;;;;;;;;;;; in the same ST line, is it worth it??
        $$.plistbegn = $1.plistbegn; 
        assert(!$1.plistlast->pNext);
        $1.plistlast->pNext = $2.plistbegn;
        $$.plistlast = $2.plistlast;
        assert(!$$.plistlast->pNext); 
    }
    else
    {
        assert(pContext->getErrorsInFile()>0);
        $$.plistbegn = NULL; 
        $$.plistlast = NULL; 
    }
 }
 ;
 
labeled_statement
 : IDENTIFIER ':' statement 
 { 
    if($3)
    {
        $$.plistbegn = pContext->newLabel($1->getEdp(), $1->getName(), $3);
        $$.plistlast = $3;   
        assert(!$$.plistlast->pNext); 
    }
    else
    {
        assert(pContext->getErrorsInFile()>0);
        //error handling: insert just the label to avoid later undef label msg.
        pContext->newLabel($1->getEdp(), $1->getName(), NULL);
        $$.plistbegn = NULL; 
        $$.plistlast = NULL; 
    }
    delete($1);
 }
 | statement
 { 
    $$.plistbegn = $1; 
    $$.plistlast = $1; 
 }
 ;

statement
 : empty_statement
 | assignment_statement
 | fb_invocation 
 | if_statement
 | case_statement
 | for_statement
 | while_statement
 | repeat_statement
 | exit_statement
 | return_statement
 | native_statement
 | synchronized_statement
 | goto_statement
 | error                    { $$  = NULL; }
 ;

empty_statement
 : /* NIL */   { $$ = pContext->newEmptyStmt(pContext->getCurrEdp()); }
 ;


assignment_statement
 : variable BECOMES expression  { $$ = pContext->newAssignStmt(&$2, $1, $3); }
 ;


exit_statement
 : EXIT { $$ = pContext->newExitStmt(&$1);}
 ;

return_statement
 : RETURN { $$ = pContext->newReturnStmt(&$1);}
 ;

native_statement
 : NATIVE 
 {
    const TCHAR* psz = pContext->getSkippedText(&$1, _T("END_NATIVE"));
    if(!psz)
        YYABORT;
    $$ = pContext->newNativeStmt(&$1, psz);
 }
 ;
 

/*##AC beg*/
fb_invocation : fun_name  '(' opt_any_param_list ')'
{
    const CG_Symtab* pSym;
    ST_Expression_C* pE = pContext->newCallExpr($1->getEdp(), $1->getName(), CG_POUTYP_UNDEF, $3);
    pE->getCalledPOU(&pSym, NULL);
    pE->fixType();
    if((pSym && pSym->pouTyp==CG_POU_FUN && pSym->pFirst->pType->type!=CG_VOID) && !pE->isErrorExpr())
        pContext->m_pdm->msg2(CG_E_NOVOID_FUNCTION, $1->getEdp(), $1->getName(), ST_TypeSetToErrMsgString(pE->GetType()));

    delete($1);
    $$ = pContext->newCallStmt(pE);
}
;
/*##AC end*/



/* TODO parser stack overflow in long if elsif elsif chains > approx. 100
 * (see old gramm V1.3 for solution) TODO for the moment increased the parser stack dramatically
 */
if_statement 
 : IF {pContext->m_pbt->if_stmts++;}
    expression THEN opt_statement_list elsif_statement_list END_IF
 { 
    pContext->m_pbt->if_stmts--;
    $$ = pContext->newIfElseStmt(&$1, $3, $5, $6);
 }
 ;

elsif_statement_list
 : ELSIF expression THEN opt_statement_list elsif_statement_list
 { 
    $$ = pContext->newIfElseStmt(&$1, $2, $4, $5); 
 }
 | ELSE opt_statement_list
 { 
    $$ = $2; 
 }
 | /* NIL */    
 { 
    $$ = NULL; 
 }
 ;


case_statement
 : CASE expression OF case_element_list opt_else_statement_list END_CASE
 { 
    $$ = pContext->newCaseStmt(&$1, $2, $4.pcasebegn, $5);
 }
 ;

case_element_list
 : case_list ':' statement ';'
 {
    $$.pcasebegn = pContext->newCase($1, $3);
    $$.plistlast = $3;
 }
 | case_element_list case_list ':' statement ';'
 {
    if($1.plistlast && $1.pcasebegn)
    {
        $$.pcasebegn = $1.pcasebegn;

        CG_STCaseList* last = $1.pcasebegn;
        while(last->pNextCase) //TODO optimise
            last = (CG_STCaseList*)last->pNextCase;

        last->pNextCase = pContext->newCase($2, $4);
        $$.plistlast = $4;
    }
    else
    {
        assert(pContext->getErrorsInFile()>0);
        $$.pcasebegn = NULL;
        $$.plistlast = NULL;
    }

 }
 | case_element_list statement ';'
 {
    if($1.plistlast && $1.pcasebegn)
    {
        $$.pcasebegn = $1.pcasebegn;
        assert(!$1.plistlast->pNext);
        $1.plistlast->pNext = $2;
        $$.plistlast = $2;
    }
    else
    {
        assert(pContext->getErrorsInFile()>0);
        $$.pcasebegn = NULL;
        $$.plistlast = NULL;
    }
 }
 ;

opt_else_statement_list
 : ELSE opt_statement_list {$$ = $2;  }
 | /* NIL */               {$$ = NULL;}

 ;

case_list
 : signed_lit_or_const
 { 
    $$ = new ST_CCaseExpr(pContext, $1, NULL);
 }
 | signed_lit_or_const DOTDOT signed_lit_or_const 
 { 
    $$ = new ST_CCaseExpr(pContext, $1, $3);
 }
 | case_list ',' signed_lit_or_const
 {
    ($$ = $1)->addRange(pContext, $3, NULL);
 }
 | case_list ',' signed_lit_or_const DOTDOT signed_lit_or_const 
 {
    ($$ = $1)->addRange(pContext, $3, $5);
 }
 ;

for_statement
 : for_assignmet DO opt_statement_list END_FOR
 { 
    pContext->m_pbt->for_stmts--;
    ST_Identifier_C *pId = NULL;
    pContext->m_pbt->FORVars.pop(pId);
    assert(pId);
    delete(pId);
    if($1)
    {
        assert(!$1->fors.pStmts);
        $1->fors.pStmts = $3;
    }
    else
    {
        assert(pContext->getErrorsInFile()>0);
    }
    $$ = $1;
 }
 ;

for_assignmet 
 : FOR IDENTIFIER BECOMES expression TO expression opt_BY
 {
    ST_Identifier_C* pIdCopy;//must create a copy now newForStmt deletes $2!
    pIdCopy = new ST_Identifier_C($2->getEdp(), pContext, $2->getName());
    pIdCopy->Detach();

    pContext->m_pbt->for_stmts++; 
    $$ = pContext->newForStmt(&$1, $2, $4, $6, $7); 
    //remember that id is currently non writable:
    pContext->m_pbt->FORVars.push(pIdCopy);
 }
 ;

opt_BY
 : BY expression  { $$ = $2;}
 | /* NIL */      { $$ = NULL;} 
 ;

while_statement
 : WHILE {pContext->m_pbt->while_stmts++;} expression DO opt_statement_list END_WHILE
 { 
    pContext->m_pbt->while_stmts--;
    $$ = pContext->newWhileStmt(&$1, $3, $5); 
 }
 ;

repeat_statement
 : REPEAT {pContext->m_pbt->repeat_stmts++;} opt_statement_list UNTIL expression END_REPEAT
 { 
    pContext->m_pbt->repeat_stmts--;
    $$ = pContext->newRepeatStmt(&$1, $3, $5); 
 }
 ;


synchronized_statement
 : SYNCHRONIZED 
 { pContext->m_pbt->sync_stmts++;}
   opt_statement_list 
   END_SYNCHRONIZED
 { 
   $$ = pContext->newSynchronizedStmt(&$1, $3); 
   pContext->m_pbt->sync_stmts--;
 }
 ;

goto_statement
 : GOTO IDENTIFIER
 { 
    $$ = pContext->newGotoStmt($2->getEdp(), $2->getName());
    delete($2);
 }
 ;


expression
 : expression OR expression
 {
    $$ = pContext->createOp(&$2, CG_OR, $1, $3);
 }
 | expression XOR expression
 {
    $$ = pContext->createOp(&$2, CG_XOR, $1, $3);
 }
 | expression AND expression
 {
    $$ = pContext->createOp(&$2, CG_AND, $1, $3);
 }
 | expression EQ expression
 {
    $$ = pContext->createOp(&$2, CG_EQ, $1, $3);
 }
 | expression NE expression
 {
    $$ = pContext->createOp(&$2, CG_NE, $1, $3);
 }
 | expression LT expression
 {
    $$ = pContext->createOp(&$2, CG_LT, $1, $3);
 }
 | expression LE expression
 {
    $$ = pContext->createOp(&$2, CG_LE, $1, $3);
 }
 | expression GT expression
 {
    $$ = pContext->createOp(&$2, CG_GT, $1, $3);
 }
 | expression GE expression
 {
    $$ = pContext->createOp(&$2, CG_GE, $1, $3);
 }
 | expression '+' expression
 {
    $$ = pContext->createOp(&$2, CG_ADD, $1, $3);
 }
 | expression '-' expression
 {
    $$ = pContext->createOp(&$2, CG_SUB, $1, $3);
 }
 | expression '*' expression
 {
    $$ = pContext->createOp(&$2, CG_MUL, $1, $3);
 }
 | expression '/' expression
 {
    $$ = pContext->createOp(&$2, CG_DIV, $1, $3);
 }
 | expression MOD expression
 {
    $$ = pContext->createOp(&$2, CG_MOD, $1, $3);
 }
 | expression POW expression
 {
    $$ = pContext->createOp(&$2, CG_POW, $1, $3);
 }
 | '-' expression %prec NOT
 {
    $$ = pContext->createOp(&$1, CG_UMINUS, $2, NULL);
 }
 | '+' expression %prec NOT
 {
    $$ = pContext->checkUPlusMinus(&$1, $2, false);
 }
 | NOT expression
 {
    $$ = pContext->createOp(&$1, CG_NOT, $2, NULL);
 }
 | primary_expression
 ;

primary_expression 
 : LITERAL                {$$ = $1;}  
 | variable               {$$ = $1;} 
 | '(' expression ')'     {$$ = pContext->createOp(&$1, CG_NOOP, $2, NULL);}
/*##AC beg*/
 | any_call               {$$ = $1;}                
 | IDENTIFIER ':'         {$$ = new ST_CLabelRef($1->getEdp(), pContext, $1->getName()); delete($1);}
/*##AC end*/
 ;


LITERAL
 :  WSTRING_BEG     { $$ = pContext->getStringLiteral(&$1, _T('"'));}
 |  STRING_BEG      { $$ = pContext->getStringLiteral(&$1, _T('\''));}
 |  SIMPLE_LITERAL  { $$ = $1;}
 ;

expression_list
 : expression {$$ = new ST_ExpressionList_C(pContext, $1);}
 | expression_list ',' expression {$3->Detach(); ($$ = $1)->m_list.add($3);/*##AC*/}
 ;

/*##AC beg*/
any_call : fun_name  '(' opt_any_param_list ')'
{
    $$ = pContext->newCallExpr($1->getEdp(), $1->getName(), CG_POUTYP_UNDEF, $3);
    delete($1);
}
| QVT_TYP  '(' opt_any_param_list ')'
{
    if($1==CG_DTYPES_UNDEF)
        $$ = new ST_ErrorExpr_C(&$2, pContext, NULL);
    else
        $$ = pContext->newCallExpr(&$2, ST_CG_DTYPES_ToString($1), CG_POU_FUN, $3);
}
;

opt_any_param_list  
 : any_param_list { $$ = $1; }
 | /* NIL */      { $$ = NULL; }
 ;

any_param_list
  : IDENTIFIER BECOMES     expression 
  { 
    ($$ = new ST_CParamList($1->getEdp(), pContext, 16))->addNamedParam($1->getEdp(), $1->getName(), false, false, $3);
    delete($1);
  }
  | IDENTIFIER RIGHT_ARROW expression 
  { 
    ($$ = new ST_CParamList($1->getEdp(), pContext, 16))->addNamedParam($1->getEdp(), $1->getName(), true, false, $3);
    delete($1);
  }
  | expression 
  { 
    ($$ = new ST_CParamList($1->getEdp(), pContext, 16))->addPosParam($1);
  }
  | any_param_list ',' IDENTIFIER BECOMES     expression 
  {
    ($$ = $1)->addNamedParam($3->getEdp(), $3->getName(), false, false, $5);
    delete($3);
  }
  | any_param_list ',' IDENTIFIER RIGHT_ARROW expression 
  {
    ($$ = $1)->addNamedParam($3->getEdp(), $3->getName(), true, false, $5);
    delete($3);
  }
  | any_param_list ','                        expression 
  {
    ($$ = $1)->addPosParam($3);
  }
  ;
/*##AC end*/






variable
 : DIRECT_VARIABLE { $$ = pContext->checkDirectBodyVar($1);} 
 | symbolic_variable
 ;

symbolic_variable
 : variable_name
 | multi_element_variable
 ;

variable_name
 : IDENTIFIER { $$ = pContext->createVar($1);}
 ;

multi_element_variable
 : array_variable
 | structured_variable
 ;

array_variable
 : symbolic_variable '[' expression_list ']'
 { 
    /* avoid second error message if base var is alrwady in error, 
     * ignore subscripts instead  
     */
    $$ = $1->isErrorExpr() ?  $1 : new ST_ArrayAccess_C(pContext, $1, $3);
 }
 ;

structured_variable
 : symbolic_variable '.' IDENTIFIER
 { 
   if ($1->toStepActionClass()!=NULL)
   {
       $1->toStepActionClass()->setFlagTyp($3);   
       $$ = $1;
   }
   else if($1->isErrorExpr())
   {
       $$ = $1;
   } 
   else
   {
       $$ = new ST_StructAccess_C(pContext, $1, $3);
   }

 }
 ;

/*####################################################################################*/
/*                          IL                                                        */
/*####################################################################################*/
opt_instruction_list
 : instruction_list 
 |  
 {
    $$.pinstrbegn = $$.pinstrlast = NULL;
 }
 ;

instruction_list
 : il_instruction
 { 
    $$.pinstrbegn = $1; 
    $$.pinstrlast = $1;
    assert(pContext->getErrorsInFile()>0 || !$$.pinstrlast->pNext); 
 }
 | instruction_list il_instruction 
 { 
    if($1.pinstrbegn && $1.pinstrlast && $2)
    {
        //optimise  ;;;;;;;;;;;;;;; in the same ST line, is it worth it??
        $$.pinstrbegn = $1.pinstrbegn; 
        assert(!$1.pinstrlast->pNext);
        $1.pinstrlast->pNext = $2;
        $$.pinstrlast = $2;
        assert(!$$.pinstrlast->pNext); 
    }
    else
    {
        assert(pContext->getErrorsInFile()>0);
        $$.pinstrbegn = NULL; 
        $$.pinstrlast = NULL; 
    }
 }
 ;

/* the grammar accepts that all instructions can have a label (conform to IEC 61131-3 B.2.1)
   later it will be check that only LD, CAL, RET, JMP or funct_call could have a label
 */
il_instruction
 : il_simple_operation
 | il_expression
 | il_jump_operation
 | il_fb_call
 | il_return_operator
 /* dummy instruction for labels */
 | IL_IDENTIFIER ':'
 { 
    $$ = new IL_Instr($1->getEdp(), IL_LABEL, IL_LBLTYP, $1->getName());
    delete($1);
 }
 ;

il_simple_operation
 : il_simple_operator il_operand
 {
    assert($1->m_ityp == IL_SIMPLEOP || $1->m_ityp == IL_EXPR);
    $1->m_ityp = IL_SIMPLEOP;
    $$ = $1;
    $$->simpleop.expr = $2;
 }
 | IL_OP_NOT                                /* NOT takes no parameters */
 | IL_IDENTIFIER opt_any_param_list         /* non-formal function call */
 {
    $$ = new IL_Instr($1->getEdp(), IL_FUNCALL, IL_CALFUN, $1->getName());
    $$->func.paramlist = $2;
    delete($1); 
 }
 ;


il_expression
 : il_expr_operator '(' 
   opt_instruction_list 
   IL_OP_RPAR
 {
    assert($1->m_ityp == IL_EXPR);
    $$ = $1;
    assert(!$$->expr.subop);
    $$->expr.instrList = $3.pinstrbegn;
 }
 | il_expr_operator '(' il_operand 
   opt_instruction_list 
   IL_OP_RPAR
 {
    assert($1->m_ityp == IL_EXPR);
    $$ = $1;
    $$->expr.subop = $3;
    $$->expr.instrList = $4.pinstrbegn;
 }
 ;

il_jump_operation
 : il_jump_operator IDENTIFIER
 {
    $$ = $1;
    $$->jmp.label = $2;
 }
 ;

il_operand
 : LITERAL                {$$ = $1;}  
 | '+' LITERAL            {$$ = pContext->checkUPlusMinus(&$1, $2, false);}
 | '-' LITERAL            {$$ = pContext->checkUPlusMinus(&$1, $2, true);}
 | variable               {$$ = $1;} 
 | '+' variable           {$$ = pContext->checkUPlusMinus(&$1, $2, false);}
 | '-' variable           {$$ = pContext->checkUPlusMinus(&$1, $2, true);}
 ;

il_fb_call
 : il_call_operator IDENTIFIER 
 {
    assert($1->m_ityp == IL_CALFB);
    $$ = $1;
    assert(!$$->fbc.FBIName);
    $$->fbc.FBIName = $2;
    $$->fbc.paramlist = NULL;
 }
 | il_call_operator IDENTIFIER '('
 {
    pContext->m_IlContext.m_bParseBody = false;
 } 
  opt_any_param_list ')'
 {
    pContext->m_IlContext.m_bParseBody = true;

    assert($1->m_ityp == IL_CALFB);
    $<instr>$ = $1;
    $$->fbc.FBIName = $2;
    $$->fbc.paramlist = $5;
 }
 ;

il_simple_operator
 : IL_OP_LD
 | IL_OP_LDN
 | IL_OP_ST
 | IL_OP_STN
 | IL_OP_S
 | IL_OP_R
 | IL_OP_S1
 | IL_OP_R1
 | IL_OP_CLK
 | IL_OP_CU
 | IL_OP_CD
 | IL_OP_PV
 | IL_OP_IN
 | IL_OP_PT
 | il_expr_operator
 ;

il_expr_operator
 : IL_OP_AND
 | IL_OP_OR
 | IL_OP_XOR
 | IL_OP_ANDN
 | IL_OP_ORN
 | IL_OP_XORN
 | IL_OP_ADD
 | IL_OP_SUB
 | IL_OP_MUL
 | IL_OP_DIV
 | IL_OP_MOD
 | IL_OP_GT
 | IL_OP_GE
 | IL_OP_EQ
 | IL_OP_LT
 | IL_OP_LE
 | IL_OP_NE
 ;

il_call_operator
 : IL_OP_CAL
 | IL_OP_CALC
 | IL_OP_CALCN
 ;

il_return_operator
 : IL_OP_RET 
 | IL_OP_RETC
 | IL_OP_RETCN
 ;

il_jump_operator
 : IL_OP_JMP 
 | IL_OP_JMPC
 | IL_OP_JMPCN
 ;


/*####################################################################################*/
%%




int st_parse::yynexttoken(YYSTYPE* val)
{
    int          tok;
    int          nLine;
    int          nCol;
    const TCHAR* yytext;
    CG_DTYPES    basicType;


    if(pContext->m_startToken)
    {
        //return a special tok to switch grammer:
        tok = pContext->m_startToken;
        pContext->m_startToken = 0;    
        return tok;
    }
    
nextToken: 

    tok = UTIL_STS_NextToken(pContext->m_pScanner, &yytext, &nLine, &nCol);
        //printf("token: '%s' (%d,%d)\n",tok==UTIL_STS_EOF?"<EOF>":yytext, nLine, nCol);

    if(pContext->m_srcType==CG_SRC_TYPE_IL && pContext->m_IlContext.m_bParseBody)
    {
        switch(tok)
        {
        case '\n':
        case ':':
            pContext->m_IlContext.m_tokensafterCR = 0;
            break;
        default:
            pContext->m_IlContext.m_tokensafterCR++;

            if(tok==')' && pContext->m_IlContext.m_tokensafterCR==1)
            {
                val->edp  = *pContext->setCurrEdp(nLine, nCol);
                return IL_OP_RPAR;
            }
        }
    }

    if(tok=='\n')
        goto nextToken;

    val->edp    = *pContext->setCurrEdp(nLine, nCol);

    switch(tok)
    {
    case UTIL_STS_TOKEN_TO_LONG:
        pContext->m_pdm->msg0(CG_E_TOKEN_TO_LONG, pContext->getCurrEdp());
        //CAUTION: you must skip this line otherwise you get an endless loop
        UTIL_STS_SkipLine(pContext->m_pScanner);
    goto nextToken; 

    case UTIL_STS_SYNTAX_ERROR:
    {
        TCHAR buff[256];
        unsigned int ui;
        if(!_istascii(*yytext))
        {
            ui = *yytext & 0xff; 
            wsprintf(buff, "'%c' = 0x%x", *yytext, ui);
            pContext->m_pdm->msg1(CG_E_SYNTAX_UNKNOWNCHAR,
                pContext->getCurrEdp(), buff);
        }
        else    
        {
            pContext->m_pdm->msg0(CG_E_SYNTAX, 
                pContext->getCurrEdp());
        }
    }
    goto nextToken;
    
    case UTIL_STS_SYS_COMMENT_BEGIN:        return SYS_COMMENT_BEGIN;
    case UTIL_STS_SYS_COMMENT_END:          return SYS_COMMENT_END;
    case UTIL_STS_ST_COMMENT_BEGIN:
        switch(UTIL_STS_SkipComment(pContext->m_pScanner, _T("*)")))
        {
        case 0:
            pContext->m_pdm->msg0(CG_E_EOF_IN_COMMENT, pContext->getCurrEdp());
            return UTIL_STS_EOF;
        case 2:
            pContext->m_pdm->msg0(CG_W_NESTED_COMMENT, pContext->getCurrEdp());
            goto nextToken;
        case 1:
            goto nextToken; 
        default:
            assert(!"bad enum");
            goto nextToken;
        }

    case UTIL_STS_DIRECT_VARIABLE:
        val->direct = pContext->getDirectAddr(yytext);
    return DIRECT_VARIABLE; 

    //literals:
    case UTIL_STS_DT_LITERAL:
    case UTIL_STS_TOD_LITERAL:
    case UTIL_STS_INT_LITERAL:
    case UTIL_STS_DATE_LITERAL:
    case UTIL_STS_TRUE_LITERAL:
    case UTIL_STS_TIME_LITERAL: 
    case UTIL_STS_FALSE_LITERAL:
    case UTIL_STS_FLOAT_LITERAL:
        val->lit = ST_LitTokenToLiteral(
                pContext->m_pdm,
                pContext->getCurrEdp(), 
                pContext, 
                &pContext->m_pFront->m_features,
                tok, 
                yytext
              );
    return SIMPLE_LITERAL;
    case UTIL_STS_STRING_LITERAL_BEGIN:
    return *yytext==_T('"') ? WSTRING_BEG : STRING_BEG;

    case UTIL_STS_PERIOD:                return PERIOD;
    case UTIL_STS_POW:                   return POW;
    case UTIL_STS_AND:                   return AND;
    case UTIL_STS_EQ:                    return EQ; 
    case UTIL_STS_NE:                    return NE; 
    case UTIL_STS_LT:                    return LT; 
    case UTIL_STS_LE:                    return LE; 
    case UTIL_STS_GT:                    return GT; 
    case UTIL_STS_GE:                    return GE;
    case UTIL_STS_BECOMES:               return BECOMES;

    case UTIL_STS_DOTDOT:                return DOTDOT;
    case UTIL_STS_RIGHT_ARROW:           return RIGHT_ARROW;

    case UTIL_STS_PRAGMA_DIRECTIVE: 
        do{
            tok = UTIL_STS_NextToken(pContext->m_pScanner, &yytext, &nLine, &nCol);
        }while(tok=='\n');

        val->edp = *pContext->setCurrEdp(nLine, nCol);
        if(tok != UTIL_STS_IDENTIFIER_EX)
        {
            pContext->m_pdm->msg1(CG_E_SYNTAX_EXPECTED, &val->edp, 
				pContext->m_pdm->res1(CG_RES_STR_ST7));
            goto nextToken;
        }
     
        if(!lstrcmpi(yytext, _T("inline")))
        {
            pContext->m_pdm->msg2(CG_W_OLD_SYNTAX, &val->edp, 
                _T("'#pragma inline {...}'"), 
                pContext->m_pdm->jot1()<<_T("\n    'inline{...}'")<<
                pContext->m_pdm->res2(CG_RES_STR_ST8)
               );
            return KW_INLINE;
        }
        else if(!lstrcmpi(yytext, _T("overload")))
        {
            pContext->m_pdm->msg2(CG_W_OLD_SYNTAX, &val->edp, 
                pContext->m_pdm->res1(CG_RES_STR_ST9), 
                pContext->m_pdm->res1(CG_RES_STR_ST10)<<
                pContext->m_pdm->res1(CG_RES_STR_ST11)
              );
            return OVERLOAD;
        }
        //TODO #pragma error|warning,  more old stuff ??
        pContext->m_pdm->msg1(CG_W_UNKNOWN_PRAGMA, &val->edp, yytext); 
        UTIL_STS_SkipLine(pContext->m_pScanner);
        goto nextToken;
   
    case UTIL_STS_IMPORT_DIRECTIVE:      return IMPORT_DIRECTIVE;

    case UTIL_STS_IDENTIFIER_EX:
    {
        UTIL_KW_ENUM kwEnum;
        HRESULT res;
        res = UTIL_CheckIdentifierEx(yytext, &kwEnum);

        if(pContext->m_srcType==CG_SRC_TYPE_IL && pContext->m_IlContext.m_bParseBody)
        {
            int op = -1;
            // in IL mode these are il operators
            switch(kwEnum)
            {
            case UTIL_KW_NO_KEYWORD: op = pContext->m_IlContext.getOperator(yytext); break;
            case UTIL_KW_MOD:        op = IL_OP_MOD;                                 break;
            case UTIL_KW_AND:        op = IL_OP_AND;                                 break;
            case UTIL_KW_OR:         op = IL_OP_OR;                                  break;
            case UTIL_KW_XOR:        op = IL_OP_XOR;                                 break;
            case UTIL_KW_NOT:        op = IL_OP_NOT;                                 break;
            }
            if(op != -1)
            {
                IL_OP il_op = il_ParseContext::getILOp(op);
                val->instr = new IL_Instr(pContext->getCurrEdp(), il_op, il_ParseContext::getInstType(il_op), yytext);
                return op;
            }
        }
                    
        switch(kwEnum)
        {
        case UTIL_KW_NO_KEYWORD:
            //check if special predefined var:
            if(res == E_UTIL_ID_SYNTAX && !_tcsicmp(yytext, _T("__edipos")))
            {
                TCHAR         szEdipos[2*MAX_PATH];
                ST_BINVAL_TYP*pBinVal;

                if((pContext->m_pFront->m_features.m_pCFeat->allTypes & CG_TBS_WSTRING)==0)
                {
                    pContext->m_pdm->errNS2(pContext->getCurrEdp(), pContext->m_pdm->res1(CG_RES_STR_DATATYPE),
                          ST_CG_DTYPES_ToString(CG_WSTRING));
                }
                
                pContext->m_pdm->edpToStrBuffer(pContext->getCurrEdp(), 
                    szEdipos+1, FC_ARRAY_LEN(szEdipos)-1-1, false);

                pBinVal = new ST_BINVAL_TYP;
                CG_BITSET strType = pContext->m_pFront->m_features.m_pCFeat->allTypes & CG_TBS_STRING ? 
                                    CG_TBS_STRING : CG_TBS_WSTRING; 
                strType = CG_TBS_WSTRING; //HCAK todo don't fix type now, wait until Propagate...

                //power HACK we need a function like bin string to IEC string.
                TCHAR delim = strType == CG_TBS_STRING ? _T('\'') : _T('"');
                int iStrLen = _tcslen(szEdipos+1);
                szEdipos[0] = delim;
                szEdipos[1+ iStrLen] = delim;
                szEdipos[2+ iStrLen] = 0;

                assert(!_tcschr(szEdipos, _T('$')));

                ST_InitBinValString(pBinVal, szEdipos);
				assert(pBinVal->iLen>=2); //TODO HACK
                val->lit = new ST_Literal_C(
                    pContext->getCurrEdp(), 
                    pContext,
                    pBinVal, 
                    ST_TypeSet_C(strType, iStrLen), //TODO HACK
                    false,
                    NULL
                    );

                return SIMPLE_LITERAL;

            }//else syntax err or correct id:

            val->ident = new ST_Identifier_C(pContext->getCurrEdp(), pContext, yytext);
            
            //HACK for function return see goto PossibleEndHdr:
            val->ident->m_FilePos = UTIL_STS_GetFPosOfNextChar(pContext->m_pScanner);

            if(res != S_OK)
               ST_CheckIdentifier(pContext->m_pdm, val->ident->getEdp(), yytext);
            if(pContext->m_srcType==CG_SRC_TYPE_IL && pContext->m_IlContext.m_bParseBody && 
                                      pContext->m_IlContext.m_tokensafterCR==1)
                return IL_IDENTIFIER;
            else
                return IDENTIFIER; 

        case UTIL_KW_TRUE:
                val->lit = ST_LitTokenToLiteral(
                    pContext->m_pdm,
                    pContext->getCurrEdp(), 
                    pContext,
                    &pContext->m_pFront->m_features,
                    UTIL_STS_TRUE_LITERAL, 
                    yytext
                );
                return SIMPLE_LITERAL;  
                              
        case UTIL_KW_FALSE:                  
                val->lit = ST_LitTokenToLiteral(
                    pContext->m_pdm,
                    pContext->getCurrEdp(), 
                    pContext,
                    &pContext->m_pFront->m_features,
                    UTIL_STS_FALSE_LITERAL, 
                    yytext
                );
                return SIMPLE_LITERAL;

        case UTIL_KW_MOD:                   return MOD; 
        
        case UTIL_KW_AND:                   return AND;
        case UTIL_KW_OR:                    return OR; 
        case UTIL_KW_XOR:                   return XOR; 
        case UTIL_KW_NOT:                   return NOT;
   
        case UTIL_KW_CONFIGURATION:         return CONFIGURATION; 
        case UTIL_KW_END_CONFIGURATION:     return END_CONFIGURATION; 
        case UTIL_KW_RESOURCE:              return RESOURCE; 
        case UTIL_KW_END_RESOURCE:          return END_RESOURCE; 
        case UTIL_KW_ON:                    return ON;
        case UTIL_KW_WITH:                  return WITH; 
        case UTIL_KW_TASK:                  return TASK; 
        case UTIL_KW_TYPE:                  return TYPE;              
        case UTIL_KW_END_TYPE:              return END_TYPE;
        case UTIL_KW_STRUCT:                return STRUCT;
        case UTIL_KW_END_STRUCT:            return END_STRUCT;
        case UTIL_KW_PROGRAM:               return PROGRAM;              
        case UTIL_KW_END_PROGRAM:           return END_PROGRAM;        
        case UTIL_KW_FUNCTION:              return FUNCTION;
        case UTIL_KW_END_FUNCTION:          return END_FUNCTION;
        case UTIL_KW_FUNCTION_BLOCK:        return FUNCTION_BLOCK;     
        case UTIL_KW_END_FUNCTION_BLOCK:    return END_FUNCTION_BLOCK; 

        //basic types:
#define CHKTYPE(type, tk) basicType=type; tok=tk; goto CheckSupportedType
        case UTIL_KW_SINT:          CHKTYPE(CG_SINT, BASIC_TYP);
        case UTIL_KW_INT:           CHKTYPE(CG_INT, BASIC_TYP);
        case UTIL_KW_DINT:          CHKTYPE(CG_DINT, BASIC_TYP);
        case UTIL_KW_LINT:          CHKTYPE(CG_LINT, BASIC_TYP);
        case UTIL_KW_USINT:         CHKTYPE(CG_USINT, BASIC_TYP);
        case UTIL_KW_UINT:          CHKTYPE(CG_UINT, BASIC_TYP);
        case UTIL_KW_UDINT:         CHKTYPE(CG_UDINT, BASIC_TYP);
        case UTIL_KW_ULINT:         CHKTYPE(CG_ULINT, BASIC_TYP);
        case UTIL_KW_REAL:          CHKTYPE(CG_REAL, BASIC_TYP);
        case UTIL_KW_LREAL:         CHKTYPE(CG_LREAL, BASIC_TYP);
        case UTIL_KW_DATE:          CHKTYPE(CG_DATE, BASIC_TYP);
        case UTIL_KW_TIME_OF_DAY:   CHKTYPE(CG_TOD, BASIC_TYP);
        case UTIL_KW_TOD:           CHKTYPE(CG_TOD, BASIC_TYP);
        case UTIL_KW_DATE_AND_TIME: CHKTYPE(CG_DT, BASIC_TYP);
        case UTIL_KW_DT:            CHKTYPE(CG_DT, BASIC_TYP);
        case UTIL_KW_STRING:        CHKTYPE(CG_STRING, BASIC_TYP);
        case UTIL_KW_WSTRING:       CHKTYPE(CG_WSTRING, BASIC_TYP);
        case UTIL_KW_TIME:          CHKTYPE(CG_TIME, BASIC_TYP);
        case UTIL_KW_BOOL:          CHKTYPE(CG_BOOL, BASIC_TYP);
        case UTIL_KW_BYTE:          CHKTYPE(CG_BYTE, BASIC_TYP);
        case UTIL_KW_WORD:          CHKTYPE(CG_WORD, BASIC_TYP);
        case UTIL_KW_DWORD:         CHKTYPE(CG_DWORD, BASIC_TYP);
        case UTIL_KW_LWORD:         CHKTYPE(CG_LWORD, BASIC_TYP);
        //qvt types  TODO add all even if not supported !
        case UTIL_KW_QVT_BOOL:      CHKTYPE(CG_QVT_BOOL, QVT_TYP);
        case UTIL_KW_QVT_BYTE:      CHKTYPE(CG_QVT_BYTE, QVT_TYP);
        case UTIL_KW_QVT_WORD:      CHKTYPE(CG_QVT_WORD, QVT_TYP);
        case UTIL_KW_QVT_DWORD:     CHKTYPE(CG_QVT_DWORD, QVT_TYP);
        case UTIL_KW_QVT_DINT:      CHKTYPE(CG_QVT_DINT, QVT_TYP);
        case UTIL_KW_QVT_LREAL:     CHKTYPE(CG_QVT_LREAL, QVT_TYP);
        case UTIL_KW_QVT_LINT:      CHKTYPE(CG_QVT_LINT, QVT_TYP);
        case UTIL_KW_QVT_LWORD:     CHKTYPE(CG_QVT_LWORD, QVT_TYP);
        case UTIL_KW_QVT_WSTRING:   CHKTYPE(CG_QVT_WSTRING, QVT_TYP);
        //any types
        case UTIL_KW_ANY:           CHKTYPE(CG_ANY, BASIC_TYP);
        case UTIL_KW_ANY_NUM:       CHKTYPE(CG_ANY_NUM, BASIC_TYP);
        case UTIL_KW_ANY_REAL:      CHKTYPE(CG_ANY_REAL, BASIC_TYP); 
        case UTIL_KW_ANY_INT:       CHKTYPE(CG_ANY_INT, BASIC_TYP);
        case UTIL_KW_ANY_BIT:       CHKTYPE(CG_ANY_BIT, BASIC_TYP);
        case UTIL_KW_ANY_STRING:    CHKTYPE(CG_ANY_STRING, BASIC_TYP);
        case UTIL_KW_ANY_DATE:      CHKTYPE(CG_ANY_DATE, BASIC_TYP);
        case UTIL_KW_ANY_ELEMENTARY:CHKTYPE(CG_ANY_ELEMENTARY, BASIC_TYP);
        case UTIL_KW_ANY_MAGNITUDE: CHKTYPE(CG_ANY_MAGNITUDE, BASIC_TYP);
        case UTIL_KW_ANY_DERIVED:   CHKTYPE(CG_ANY_DERIVED, BASIC_TYP);
#undef CHKTYPE

        //misc type decl:
        case UTIL_KW_AT:                    return AT;
        case UTIL_KW_ARRAY:                 return ARRAY;
        case UTIL_KW_EN:
        case UTIL_KW_ENO:
            //not supported msg now and return normal ids: 
            pContext->m_pdm->msg1(CG_E_NOT_SUPPORTED, pContext->getCurrEdp(), yytext);
            val->ident = new ST_Identifier_C(pContext->getCurrEdp(), pContext, yytext);
           return IDENTIFIER;

        case UTIL_KW_R_EDGE:                return R_EDGE;
        case UTIL_KW_F_EDGE:                return F_EDGE;

        //decl scopes:
        case UTIL_KW_VAR:                   return VAR;  
        case UTIL_KW_VAR_INPUT:             return VAR_INPUT;
        case UTIL_KW_VAR_OUTPUT:            return VAR_OUTPUT;
        case UTIL_KW_VAR_IN_OUT:            return VAR_IN_OUT;
        case UTIL_KW_VAR_EXTERNAL:          return VAR_EXTERNAL;
        case UTIL_KW_VAR_ACCESS:            return VAR_ACCESS;
        case UTIL_KW_VAR_GLOBAL:            return VAR_GLOBAL;    
        case UTIL_KW_VAR_TEMP:              return VAR_TEMP;
        case UTIL_KW_VAR_IN_EXT:            return VAR_IN_EXT;
        case UTIL_KW_END_VAR:    tok = END_VAR; goto PossibleEndHdr;
    
        case UTIL_KW_RETAIN:                return RETAIN;
        case UTIL_KW_NON_RETAIN:            return NON_RETAIN;
        case UTIL_KW_CONSTANT:              return CONSTANT;
        case UTIL_KW_VAR_CONFIG:            return VAR_CONFIG;

        case UTIL_KW_READ_ONLY:             
            //not supported msg now: 
            pContext->m_pdm->msg1(CG_E_NOT_SUPPORTED, pContext->getCurrEdp(), yytext);
            return READ_ONLY;
        case UTIL_KW_READ_WRITE:
            //not supported msg now: 
            pContext->m_pdm->msg1(CG_E_NOT_SUPPORTED, pContext->getCurrEdp(), yytext);
            return READ_WRITE;

        //SFC tokens:
        case UTIL_KW_STEP:                  return STEP;
        case UTIL_KW_END_STEP:              return END_STEP;
        case UTIL_KW_INITIAL_STEP:          return INITIAL_STEP;
        case UTIL_KW_TRANSITION:            return TRANSITION;
        case UTIL_KW_END_TRANSITION:        return END_TRANSITION;
        case UTIL_KW_ACTION:                return ACTION;
        case UTIL_KW_END_ACTION:            return END_ACTION;
        case UTIL_KW_FROM:                  return FROM;

        case UTIL_KW_IF:                    return IF; 
        case UTIL_KW_THEN:                  return THEN; 
        case UTIL_KW_ELSIF:                 return ELSIF; 
        case UTIL_KW_ELSE:                  return ELSE; 
        case UTIL_KW_END_IF:                return END_IF;

        case UTIL_KW_CASE:                  return CASE; 
        case UTIL_KW_OF:                    return OF; 
        case UTIL_KW_END_CASE:              return END_CASE;

        case UTIL_KW_WHILE:                 return WHILE; 
        case UTIL_KW_DO:                    return DO; 
        case UTIL_KW_END_WHILE:             return END_WHILE;

        case UTIL_KW_REPEAT:                return REPEAT; 
        case UTIL_KW_UNTIL:                 return UNTIL; 
        case UTIL_KW_END_REPEAT:            return END_REPEAT;

        case UTIL_KW_FOR:                   return FOR; 
        case UTIL_KW_TO:                    return TO; 
        case UTIL_KW_BY:                    return BY; 
        case UTIL_KW_END_FOR:               return END_FOR;

        case UTIL_KW_EXIT:                  return EXIT; 
        case UTIL_KW_RETURN:                return RETURN;
               
        //ATCM enhancements:
        case UTIL_KW_GOTO:                  return GOTO;
        case UTIL_KW_SYNCHRONIZED:          return SYNCHRONIZED;
        case UTIL_KW_END_SYNCHRONIZED:      return END_SYNCHRONIZED;
        case UTIL_KW_NATIVE:                return NATIVE;
        case UTIL_KW_VAR_NATIVE:            return VAR_NATIVE;
        case UTIL_KW_END_NATIVE:            return END_NATIVE;
        case UTIL_KW_CONST:                 return DECL_CONST;
        case UTIL_KW_END_CONST:       tok = DECL_END_CONST; goto PossibleEndHdr;
        case UTIL_KW_INLINE:                return KW_INLINE;
        case UTIL_KW_OVERLOAD:              return OVERLOAD;
        default: 
           assert(!"bad keyword enum");
        }
    }
    
    case ']':  goto PossibleEndHdr; //function return wstring[len']'

    //default: tokens <256 or token not valid in this grammer:
    default:
        return tok;
    }




CheckSupportedType:
    if(pContext->m_bDontCheckBackendSupportedTypes)
    {
        if((CG_FRONT_END_TYPES & ST_DTypesToTBS(basicType))==0)
        {
            pContext->m_pdm->errNS2(&val->edp, pContext->m_pdm->res1(CG_RES_STR_DATATYPE),
              ST_CG_DTYPES_ToString(basicType));
        }
    }
    else
    {
        if(!pContext->m_pFront->m_features.checkBasicType(ST_DTypesToTBS(basicType)))
        {
            pContext->m_pdm->errNS2(&val->edp, pContext->m_pdm->res1(CG_RES_STR_DATATYPE),
              ST_CG_DTYPES_ToString(basicType));
        }
    }

    val->btype = basicType;
   //fall through to HACK 

PossibleEndHdr: //HACK I'm still to stupid to do it any better...
    pContext->m_ulhdrEndTMP = UTIL_STS_GetFPosOfNextChar(pContext->m_pScanner);

    return tok;
}

void st_parse::yyerrorStackOverflow()
{
    pContext->m_pdm->msg0(CG_E_YACC_STACK, pContext->getCurrEdp());
}
void st_parse::yyerrorSyntax()
{
    if(yychar == SYS_COMMENT_BEGIN)
    {
        pContext->m_pdm->msg0(CG_E_MISPLACED_SYS_COMMENT, pContext->getCurrEdp());

        switch(UTIL_STS_SkipComment(pContext->m_pScanner, _T("*)")))
        {
        case 0:
            pContext->m_pdm->msg0(CG_E_EOF_IN_COMMENT, pContext->getCurrEdp());
        break;
        case 2:
            pContext->m_pdm->msg0(CG_W_NESTED_COMMENT, pContext->getCurrEdp());
        break;
        case 1:
        break;
        default:
            assert(!"bad enum");
        }
        yyclearin();
        return;
    }
    
    if(pContext->pSymtab && pContext->pSymtab->ulHdrBytes!=0)
    {
        switch(yychar)
        {
        case VAR: case VAR_INPUT: case VAR_OUTPUT: case VAR_IN_OUT:
        case VAR_EXTERNAL: case VAR_GLOBAL:
        case VAR_TEMP: case VAR_IN_EXT: case VAR_CONFIG:
        //TODO all header decl KWs
            pContext->m_pdm->msg1(CG_E_SYNTAX_UNEXPECTED,
             pContext->getCurrEdp(), pContext->m_pdm->res1(CG_RES_STR_ST12));
        return;    
        }
    }

    pContext->m_pdm->msg0(CG_E_SYNTAX, pContext->getCurrEdp());
}



void st_ParseContext::initTmpMembers()
{
    m_pFirstHeapElement = NULL;
    m_pLastHeapElement = NULL;

    m_errCountBeginFile = 0;

    m_pFront = NULL;
    m_pyacc   = NULL;
	m_pScanner= NULL;


    CG_InitCMDLEdipos(&m_currEdp);
    m_srcType = CG_SRC_TYPE_UNDEF;
    m_ftLastWrite = 0;
    m_pszFile     = NULL;
    m_pLibCtx     = NULL;
    m_pSysCommentTMP = NULL;
    m_typeorTMP      = CG_TBS_EMPTY;
    m_typeorRetTMP   = CG_TBS_EMPTY;

    m_bDontCheckBackendSupportedTypes = false;
    m_currDeclScope = CG_DECLSCOPE_UNDEF;
    m_currDeclBitSet = 0;
    m_pCurrTaskList = NULL;
    //##AC m_pCurrFBICall = NULL;

    m_pResource = NULL;
    m_pConfig   = NULL;

    m_pSFC = NULL;
    m_pBody = NULL;
    m_pbt   = NULL;
    pSymtab = NULL;
    m_pSymtabPrv = NULL;
    assert(m_Tasks.getUsed()==0);

    m_startToken = 0;
    m_lLineOffset= 0;
    m_lColOffset= 0;
    m_pSingleExpression = NULL;
    m_pSTAction = NULL;


    //assert(m_addrExpansionResult.resetUsed());
    m_bDeclSecHasAddr = false;
}

st_ParseContext::st_ParseContext(CG_CIECFront* pFront)
{
    int result;

    initTmpMembers();

    m_pFront = pFront;
    m_pdm    = pFront->m_pdm;
    m_pyacc  = new st_parse(&result, 300);//TODO must use stack size 300
                                          //due to TODO with elsif solution 
    m_pScanner = UTIL_STS_CreateScanner();
    if(!(m_pyacc && m_pScanner && (result==0 || result==1)))
    {
        CGT_IERR(m_pdm, _T("create parser failed"));
        return;
    }

    //set the context for the parser: 
    m_pyacc->setParseContext(this);
    m_IlContext.m_pSTContext = this;
}

st_ParseContext::~st_ParseContext()
{
    if(m_pScanner)
        UTIL_STS_DeleteScanner(m_pScanner);
    delete(m_pyacc);

    //check error and cleanup handling:
    assert(!m_pszFile && !m_ftLastWrite);
    assert(!m_pResource && !m_pConfig && !m_pSymtabPrv);
    assert(!m_pCurrTaskList /*##AC*/ && 
           !m_pSysCommentTMP);
    assert(!m_pSingleExpression);
    assert(!m_pSTAction);
    assert(!m_pFirstHeapElement);
    assert(!m_pSFC && !m_pBody && !m_pbt);
}

 
ST_CSymtabPrv* st_ParseContext::parseHeader(
    const TCHAR*         pszFileName,//used for initial edipos
    const CG_LIBContext* pLibCtx,    //!=NULL if from lib 
    const TCHAR*         pszText,    //text, either a complete file or 
                                     //a 4gr header part or from a lib a .hdr
    unsigned             ulsize,     //size in TCHARs without 
                                     //the terminating 0
    __int64              ftLastWrite //file time of file to parse
)
{
    ST_CSymtabPrv* pRet;
    
   
    //remember the tot err count at begin of file (only for assertions..)
    m_errCountBeginFile = m_pFront->m_pdm->getTotalErrors();    

    //should be false anyway (we have one instace of the parser per POU)
    //but be safe:
    m_bDontCheckBackendSupportedTypes = false;

    //make pszFileName a global string:
    assert(!m_pszFile);
    m_pszFile = m_pFront->registerString(pszFileName);
    CG_InitSTEdipos(&m_currEdp, m_pszFile);
    m_lLineOffset = m_lColOffset = 0;

    m_pLibCtx   = pLibCtx;
    m_ftLastWrite = ftLastWrite;

    m_srcType  = m_pFront->m_kad.getSourceTypeByExt(FC_StringGetFileExt(pszFileName));

    assert(m_startToken==0);
    
    switch(m_srcType)
    {
    case CG_SRC_TYPE_CST:
        //create now, is ok even if file is empty.
        //empty file yields an empty symtab not an error:
        newSymtab(&m_currEdp, CG_POU_CST, m_pszFile);
        //set start token needed to reduce syntax freedom:
        m_startToken = GVLCST_START;
    break;
    case CG_SRC_TYPE_GVL:
        //create now, is ok even if file is empty.
        //empty file yields an empty symtab not an error:
        newSymtab(&m_currEdp, CG_POU_GVL, m_pszFile);

        //set start token needed to reduce syntax freedom:
        m_startToken = GVLCST_START;
    break;
    case CG_SRC_TYPE_SDT:
    case CG_SRC_TYPE_ST:
    case CG_SRC_TYPE_IL:
    break;
    case CG_SRC_TYPE_4GR:
    case CG_SRC_TYPE_HDR:
        m_startToken = HDRFILE_START;
    break;
    default:
        assert(!"bad file type for parseAnyHeader");
    }
    
    UTIL_STS_SetInputString(m_pScanner, pszText, ulsize);
    
    
    assert(!m_pBody);
    m_pyacc->yyreset();
    m_pyacc->yyparse();

    //cannot happen even if syntax error:
    assert(!m_pResource && !m_pCurrTaskList);
    assert(!m_pSFC && !m_pBody /*##AC*/);


    if(getErrorsInFile()==0)
    {
        assert(!m_pSysCommentTMP);
        assert(pSymtab->flags & CG_STAB_HDR_NOERR);
    }
    else
    {
        //only set to NULL, memory is freed in next for loop !
        m_pCurrTaskList = NULL;
        m_pSysCommentTMP= NULL;
        //##AC m_pCurrFBICall  = NULL;
        
        //clean up lost parse tree elements:
        while(m_pFirstHeapElement)
            delete(m_pFirstHeapElement);
    }

    pRet = m_pSymtabPrv;
    m_pSymtabPrv  = NULL;
    m_ftLastWrite = 0;
    m_pszFile     = NULL;

    return pRet;
}


ST_CSymtabPrv* st_ParseContext::parseWholeSTFile(
    const TCHAR*         pszFileName,//used for initial edipos, 
    const CG_LIBContext* pLibCtx,    //!=NULL if from lib 
    const TCHAR*         pszText,    //text, either a complete file or 
                                     //a 4gr header part or from a lib a .hdr
    unsigned             ulsize,     //size in TCHARs without the 
                                     //terminating 0
    ST_CBody**           ppBody,     //the body to be returned
    __int64              ftLastWrite,//file time of file to parse
    ST_CSymtabPrv*       pSymOld     //!=NULL if load body after only hdr was loaded
)
{
    ST_CSymtabPrv* pRet;
   
    //remember the tot err count at begin of file (only for assertions..)
    m_errCountBeginFile = m_pFront->m_pdm->getTotalErrors();
    
    //should be false anyway (we have one instace of the parser per POU)
    //but be safe:
    m_bDontCheckBackendSupportedTypes = false;

    //make pszFileName a global string:
    assert(!m_pszFile);
    m_pszFile = m_pFront->registerString(pszFileName);
    CG_InitSTEdipos(&m_currEdp, m_pszFile);
    m_lLineOffset = m_lColOffset = 0;

    m_pLibCtx = pLibCtx;
    m_ftLastWrite = ftLastWrite;

    m_srcType  = m_pFront->m_kad.getSourceTypeByExt(FC_StringGetFileExt(pszFileName));

    assert(m_srcType==CG_SRC_TYPE_ST);


    if(pSymOld)
    {   //set symtab to old one and skip the header using a start token:
        m_startToken = BODY_START;
        m_pSymtabPrv = pSymOld;
        pSymtab      = pSymOld->getCGSym();
        
        assert(pSymtab->ulHdrBytes>0 && pSymtab->ulHdrBytes<ulsize);
        UTIL_STS_SetInputStringEx(m_pScanner, pszText, ulsize, pSymtab->ulHdrBytes);
    }
    else
    {
        UTIL_STS_SetInputString(m_pScanner, pszText, ulsize);
    }
    
    assert(!m_pBody && !m_pbt);
    m_pBody = new ST_CBody();
    m_pbt   = new ST_CBodyTMP();

    m_pyacc->yyreset();
    m_pyacc->yyparse();

    //cannot happen even if syntax error:
    assert(!m_pResource && !m_pCurrTaskList);

    if(getErrorsInFile()==0)
    {
        assert(!m_pSysCommentTMP && /*##AC*/ !m_pSFC);
        assert(this->m_pSymtabPrv);//must now exist!
    }
    else
    {
        delete(m_pSFC);
        m_pSFC = NULL;

        //only set to NULL, memory is freed in next for loop !
        m_pCurrTaskList  = NULL;
        m_pSysCommentTMP = NULL;
        //##AC m_pCurrFBICall   = NULL;
        
        //delete body if any error but not the symtab:
        delete(m_pBody);
        m_pBody = NULL;
              
        //clean up lost parse tree elements:
        while(m_pFirstHeapElement)
            delete(m_pFirstHeapElement);
    }

    pRet = m_pSymtabPrv;
    m_pSymtabPrv = NULL;

    *ppBody       = m_pBody;
    m_pBody       = NULL;
    m_ftLastWrite = 0;
    m_pszFile     = NULL;
    delete(m_pbt);
    m_pbt         = NULL;    

    return pRet;
}

ST_CSymtabPrv* st_ParseContext::parseWholeILFile(
    const TCHAR*         pszFileName,//used for initial edipos, 
    const CG_LIBContext* pLibCtx,    //!=NULL if from lib 
    const TCHAR*         pszText,    //text, either a complete file or 
                                     //a 4gr header part or from a lib a .hdr
    unsigned             ulsize,     //size in TCHARs without the 
                                     //terminating 0
    ST_CBody**           ppBody,     //the body to be returned
    __int64              ftLastWrite,//file time of file to parse
    ST_CSymtabPrv*       pSymOld     //!=NULL if load body after only hdr was loaded
)
{
    ST_CSymtabPrv* pRet;
 
    //remember the tot err count at begin of file (only for assertions..)
    m_errCountBeginFile = m_pFront->m_pdm->getTotalErrors();
    
    //should be false anyway (we have one instace of the parser per POU)
    //but be safe:
    m_bDontCheckBackendSupportedTypes = false;

    //make pszFileName a global string:
    assert(!m_pszFile);
    m_pszFile = m_pFront->registerString(pszFileName);
    CG_InitSTEdipos(&m_currEdp, m_pszFile);
    m_lLineOffset = m_lColOffset = 0;

    m_pLibCtx = pLibCtx;
    m_ftLastWrite = ftLastWrite;

    m_srcType  = m_pFront->m_kad.getSourceTypeByExt(FC_StringGetFileExt(pszFileName));

    assert(m_srcType==CG_SRC_TYPE_IL);


    if(pSymOld)
    {   //set symtab to old one and skip the header using a start token:
        m_startToken = IL_BODY_START;
        m_pSymtabPrv = pSymOld;
        pSymtab      = pSymOld->getCGSym();
        
        assert(pSymtab->ulHdrBytes>0 && pSymtab->ulHdrBytes<ulsize);
        UTIL_STS_SetInputStringEx(m_pScanner, pszText, ulsize, pSymtab->ulHdrBytes);
    }
    else
    {
        UTIL_STS_SetInputString(m_pScanner, pszText, ulsize);
    }
    
    assert(!m_pBody && !m_pbt);
    m_pBody = new ST_CBody();
    m_pbt   = new ST_CBodyTMP();

    m_pyacc->yyreset();
    m_pyacc->yyparse();

    //cannot happen even if syntax error:
    assert(!m_pResource && !m_pCurrTaskList);

    if(getErrorsInFile()==0)
    {
        assert(!m_pSysCommentTMP && !m_pSFC);
        assert(this->m_pSymtabPrv);//must now exist!
    }
    else
    {
        delete(m_pSFC);
        m_pSFC = NULL;

        //only set to NULL, memory is freed in next for loop !
        m_pCurrTaskList  = NULL;
        m_pSysCommentTMP = NULL;
        
        //delete body if any error but not the symtab:
        delete(m_pBody);
        m_pBody = NULL;
              
        //clean up lost parse tree elements:
        while(m_pFirstHeapElement)
            delete(m_pFirstHeapElement);
    }

    pRet = m_pSymtabPrv;
    m_pSymtabPrv = NULL;

    *ppBody       = m_pBody;
    m_pBody       = NULL;
    m_ftLastWrite = 0;
    m_pszFile     = NULL;
    delete(m_pbt);
    m_pbt         = NULL;    

    return pRet;
}


ST_CSymtabPrv* st_ParseContext::parse4GRFile(
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
)
{
    ST_CSymtabPrv* pRet;
   
    //remember the tot err count at begin of file (only for assertions..)
    m_errCountBeginFile = m_pFront->m_pdm->getTotalErrors();    

    //should be false anyway (we have one instace of the parser per POU)
    //but be safe:
    m_bDontCheckBackendSupportedTypes = false;

    //make pszFileName a global string:
    assert(!m_pszFile);
    m_pszFile = m_pFront->registerString(pszFileName);
    CG_InitSTEdipos(&m_currEdp, m_pszFile);

    m_pLibCtx = pLibCtx;
    m_ftLastWrite = ftLastWrite;

    m_srcType  = m_pFront->m_kad.getSourceTypeByExt(FC_StringGetFileExt(pszFileName));
    assert(m_srcType==CG_SRC_TYPE_4GR);


    if(pSymOld)
    {
        m_pSymtabPrv = pSymOld;
        pSymtab      = pSymOld->getCGSym();
        assert(pSymtab->flags & CG_STAB_HDR_NOERR);
    }
    else
    {
        //parse the header part:
        m_startToken = HDRFILE_START;
        UTIL_STS_SetInputString(m_pScanner, pszText, ulsize);
        m_pyacc->yyreset();
        m_pyacc->yyparse();
   
        //cannot happen even if syntax error:
        assert(!m_pResource && !m_pCurrTaskList);
        assert(!m_pSFC && !m_pBody);

        if(getErrorsInFile()==0)
        {
            assert(this->m_pSymtabPrv);//must now exist!
            assert(!m_pSysCommentTMP &&  !m_pSFC && !m_pBody);
        }
        else
        {
            //only set to NULL, memory is freed in next for loop !
            m_pCurrTaskList  = NULL;
            m_pSysCommentTMP = NULL;
        
            //clean up lost parse tree elements:
            while(m_pFirstHeapElement)
                delete(m_pFirstHeapElement);
        }
    }


    if(getErrorsInFile()==0)
    {
        //header has 0 errors and XML of body could be 
        //loaded too, now check the graphics:
        assert(!m_pBody && !m_pbt);
        m_pBody = new ST_CBody();
        m_pbt   = new ST_CBodyTMP();

        assert(pSymtab->flags & CG_STAB_HDR_NOERR);
        ST_LDFBD_To_CBody(this, grElements, codeGenOpt);

        //should not happen since parse expr/trans/action
        //must clean up on their onw.
        assert(!m_pFirstHeapElement);

        
        //end of pou edipos for graphic languages:
        CG_Edipos edp;
        memset(&edp, 0, sizeof(edp));
        edp.pszFile = m_pszFile;
        edp.typ = CG_EDP_4GR_END;
        endOfPou(&edp, CG_POUTYP_UNDEF);
    }

    if(getErrorsInFile()!=0)
    {
        delete(m_pBody);
        m_pBody = NULL;    
    }

    pRet = m_pSymtabPrv;
    m_pSymtabPrv = NULL;

    *ppBody       = m_pBody;
    m_pBody       = NULL;
    m_ftLastWrite = 0;
    m_pszFile     = NULL;
    delete(m_pbt);
    m_pbt         = NULL;    

    return pRet;
}

ST_Expression_C* st_ParseContext::parse4GRBox( //use only after call parse4GRFile
  const CG_Edipos* pEdp,     //must not be NULL
  const TCHAR*     pszExpr,  //expression as string, should not have newlines.
  bool             bNoWarns  //if true no warnings, usefull for reparse same expr. 
)
{
    ST_Expression_C* pExpr = NULL;
    int errCountBegin = getErrorsInFile();

    assert(!m_pSingleExpression && !m_pSTAction && m_pSymtabPrv);
    assert(m_pszFile && m_ftLastWrite && m_pBody);

    //should be false anyway (we have one instace of the parser per POU)
    //but be safe:
    m_bDontCheckBackendSupportedTypes = false;

    m_currEdp   = *pEdp;
    m_lLineOffset = m_lColOffset = 0;
    m_startToken = EXPR_START;
    
    UTIL_STS_SetInputString(m_pScanner, pszExpr, _tcslen(pszExpr));

    if(bNoWarns)
        m_pdm->disableWarnings();
    //do the work:
    m_pyacc->yyreset();
    m_pyacc->yyparse();

    if(bNoWarns)
        m_pdm->enableWarnings();

    if(errCountBegin==getErrorsInFile())
    {
        assert(!m_pSysCommentTMP /*##AC*/);
        assert(m_pSingleExpression);//must now exist!
        
        //Detach now, the caller is responsible for pExpr:
        if(m_pSingleExpression)
        {
            m_pSingleExpression->Detach();
            pExpr = m_pSingleExpression;
            m_pSingleExpression = NULL;
        }
    }
    else
    {
        //only set to NULL, memory is freed in next for loop !
        //##AC m_pCurrFBICall   = NULL;
        delete(m_pSingleExpression);
        m_pSingleExpression = NULL;
        //clean up lost parse tree elements:
        while(m_pFirstHeapElement)
            delete(m_pFirstHeapElement);
    }

    //reset the start token:
    m_startToken = 0;
    return pExpr;
}



CG_STStmt* st_ParseContext::parseSTAction( //use only after call parse4GRFile
    const CG_Edipos* pEdp,     //must not be NULL
    const TCHAR*     pszAction,//ST action text as string
    bool             bNoWarns  //if true no warnings, usefull for reparse same expr. 
)
{
    CG_STStmt*  pSTAction;
    int         errCountBegin = getErrorsInFile();

    assert(!m_pSingleExpression && m_pSymtabPrv && !m_pSTAction);
    assert(m_pszFile && m_ftLastWrite && m_pBody);

    //should be false anyway (we have one instace of the parser per POU)
    //but be safe:
    m_bDontCheckBackendSupportedTypes = false;

    //edipos special handling:
    assert(pEdp->typ == CG_EDP_4GR_BOX && pEdp->gr.l==0 && pEdp->gr.c==0);
    m_currEdp = *pEdp;
    m_lLineOffset = m_lColOffset = 0;

    //patch the m_curredp to be a st-box, then st_ParseContext::setCurrEdp
    //will update the m_currEdp.gr.l and m_currEdp.gr.c members: 
    m_currEdp.typ = CG_EDP_4GR_STBOX;


    m_startToken = STACTION_START;
   
    UTIL_STS_SetInputString(m_pScanner, pszAction, _tcslen(pszAction));

    if(bNoWarns)
        m_pdm->disableWarnings();
    //do the work:
    m_pyacc->yyreset();
    m_pyacc->yyparse();

    if(bNoWarns)
        m_pdm->enableWarnings();

    assert(!m_pSingleExpression);
    if(errCountBegin==getErrorsInFile())
    {
        pSTAction = m_pSTAction;
        //can happen if empty action: assert(pSTAction);
    }
    else
    {
        pSTAction = NULL;
        //only set to NULL, memory is freed in next for loop !
        //##AC m_pCurrFBICall   = NULL;
        //clean up lost parse tree elements:
        while(m_pFirstHeapElement)
            delete(m_pFirstHeapElement);
    }

    //reset the start token:
    m_startToken = 0;
    m_pSTAction = NULL;
    
    return pSTAction;
}






ST_CConfig* st_ParseContext::parseConfiguration(
    const TCHAR*     pszFileName,//used for initial edipos, 
    const TCHAR*     pszText,    //a complete .res file
    unsigned         ulsize,     //size in TCHARs without the 
                                 //terminating 0
    __int64          ftLastWrite //file time of file to parse
)
{
    ST_CConfig* pCon;

    //remember the tot err count at begin of file (only for assertions..)
    m_errCountBeginFile = m_pFront->m_pdm->getTotalErrors();    

    //should be false anyway (we have one instace of the parser per POU)
    //but be safe:
    m_bDontCheckBackendSupportedTypes = false;

    //make pszFileName a global string:
    assert(!m_pszFile);
    m_ftLastWrite = ftLastWrite;
    m_pszFile = m_pFront->registerString(pszFileName);
    CG_InitSTEdipos(&m_currEdp, m_pszFile);
    m_lLineOffset = m_lColOffset = 0;

    m_srcType  = m_pFront->m_kad.getSourceTypeByExt(FC_StringGetFileExt(pszFileName));
    assert(m_srcType==CG_SRC_TYPE_CON); 
    
    UTIL_STS_SetInputString(m_pScanner, pszText, ulsize);
    m_pyacc->yyreset();
    m_pyacc->yyparse();

    //cannot happen even if syntax error:
    assert(!m_pSFC && !m_pBody /*##AC*/ && 
           !m_pCurrTaskList && !m_pSymtabPrv);

    
    if(getErrorsInFile()==0)
    {
        assert(!m_pSysCommentTMP);
        assert(m_pConfig);//must now exist!
    }
    else
    {
        //only set to NULL, memory is freed in next for loop !
        m_pSysCommentTMP = NULL;
        //clean up lost parse tree elements:
        while(m_pFirstHeapElement)
            delete(m_pFirstHeapElement);

        delete(m_pConfig);
        m_pConfig = NULL;
    }

    pCon = m_pConfig;
    m_pConfig = NULL;
    m_pszFile = NULL;
    m_ftLastWrite = 0;

    return pCon;
}



CG_Resource* st_ParseContext::parseResource(
    const TCHAR*     pszFileName,//used for initial edipos, 
    const TCHAR*     pszText,    //a complete .res file
    unsigned         ulsize,     //size in TCHARs without the 
                                 //terminating 0
    ST_CSymtabPrv**  ppSymPrvNew, //OUT: if 0 errors the glob var table
    __int64          ftLastWrite
)
{
    CG_Resource* pRet;

    //remember the tot err count at begin of file (only for assertions..)
    m_errCountBeginFile = m_pFront->m_pdm->getTotalErrors();    

    //should be false anyway (we have one instace of the parser per POU)
    //but be safe:
    m_bDontCheckBackendSupportedTypes = false;

    //make pszFileName a global string:
    assert(!m_pszFile);
    m_ftLastWrite = ftLastWrite;
    m_pszFile = m_pFront->registerString(pszFileName);
    CG_InitSTEdipos(&m_currEdp, m_pszFile);
    m_lLineOffset = m_lColOffset = 0;

    m_srcType  = m_pFront->m_kad.getSourceTypeByExt(FC_StringGetFileExt(pszFileName));

    assert(m_srcType==CG_SRC_TYPE_RES); 
    
    UTIL_STS_SetInputString(m_pScanner, pszText, ulsize);
    m_pyacc->yyreset();
    m_pyacc->yyparse();

    //cannot happen even if syntax error:
    assert(!m_pSFC && !m_pBody /*##AC*/);

    
    if(getErrorsInFile()==0)
    {
        assert(!m_pSysCommentTMP);
        assert(!m_pCurrTaskList); //must have been deleted
        assert(m_pSymtabPrv && m_pResource);//must now exist!
        assert(m_pResource->pGVL == pSymtab);

        // set the no error flag:
        assert(!m_pResource->bNoError);
        m_pResource->bNoError = true;
    }
    else
    {
        //only set to NULL, memory is freed in next for loop !
        m_pCurrTaskList  = NULL;
        m_pSysCommentTMP = NULL;
        //##AC m_pCurrFBICall   = NULL;

        //clean up lost parse tree elements:
        while(m_pFirstHeapElement)
            delete(m_pFirstHeapElement);
        
        //return the resource even if error but assert:
        assert(!m_pResource || !m_pResource->bNoError);
    }

    pRet = m_pResource;
    *ppSymPrvNew = m_pSymtabPrv;
    m_pResource = NULL;
    m_pSymtabPrv = NULL;
    m_pszFile = NULL;
    m_ftLastWrite = 0;

    return pRet;
}


ST_Expression_C* st_ParseContext::parseSingleExpression(
  const CG_Edipos* pEdp,     //must not be NULL
  bool             bIncCol,  //pEdp's line,column info is updated for every diag msg
  const TCHAR*     pszExpr,  //expression as string, should not have newlines.
  ST_CSymtabPrv*   pSymPrv,  //may be NULL, if only constant expr. should be evaluated.
  bool             bNoWarns  //if true no warnings, usefull for reparse same expr. 
)
{
    ST_Expression_C* pExpr = NULL;
    assert(pszExpr);

    //should be false anyway (we have one instace of the parser per POU)
    //but be safe:
    m_bDontCheckBackendSupportedTypes = false;
    
    assert(!m_pSingleExpression);
    assert(!m_pszFile && !m_ftLastWrite && !m_pSFC);
    if(pEdp->pszFile)
        m_pszFile = m_pFront->registerString(pEdp->pszFile);

    m_errCountBeginFile = m_pFront->m_pdm->getTotalErrors();
    m_currEdp   = *pEdp;
    if(bIncCol && m_currEdp.typ==CG_EDP_ST)
    {
        m_lLineOffset = m_currEdp.lc.l - 1; //-1 because UTIL_STS_NextToken return 1 based line,col 
        m_lColOffset  = m_currEdp.lc.c - 1;
    }
    else if(bIncCol && m_currEdp.typ==CG_EDP_4GR_STBOX)
    {
        m_lLineOffset = m_currEdp.gr.l - 1;
        m_lColOffset  = m_currEdp.gr.c - 1;
    }
    else
    {
        m_lLineOffset = m_lColOffset = 0;
    }
    m_startToken = EXPR_START;
    m_pSymtabPrv= pSymPrv;
    pSymtab     = pSymPrv ? pSymPrv->getCGSym() : NULL;
    
    UTIL_STS_SetInputString(m_pScanner, pszExpr, _tcslen(pszExpr));

    if(bNoWarns)
        m_pdm->disableWarnings();

    //do the work:
    m_pyacc->yyreset();
    m_pyacc->yyparse();

    if(bNoWarns)
        m_pdm->enableWarnings();

    if(getErrorsInFile()==0)
    {
        assert(!m_pSysCommentTMP /*##AC*/);
        assert(!m_pCurrTaskList); //must have been deleted
        assert(m_pSingleExpression);//must now exist!
        
        //Detach now, the caller is responsible for pExpr:
        if(m_pSingleExpression)
        {
            m_pSingleExpression->Detach();
            pExpr = m_pSingleExpression;
            m_pSingleExpression = NULL;
        }
    }
    else
    {
        //only set to NULL, memory is freed in next for loop !
        //##AC m_pCurrFBICall   = NULL;

        delete(m_pSingleExpression);
        m_pSingleExpression = NULL;

        //clean up lost parse tree elements:
        while(m_pFirstHeapElement)
            delete(m_pFirstHeapElement);
    }

    //reset at least the symtab variables:
    m_pSymtabPrv = NULL;
    pSymtab      = NULL;
    m_startToken = 0;
    m_pszFile  = NULL;

    return pExpr;
}

int il_ParseContext::getOperator(const TCHAR* pszToken)
{
    assert(pszToken && pszToken[0]);

    if(m_tokensafterCR != 1)
        return -1;

    switch(pszToken[0])
    {
        case _T('a'):
        case _T('A'):
            if(!strcmpi(pszToken, _T("ADD")))   return IL_OP_ADD;
            if(!strcmpi(pszToken, _T("AND")))   return IL_OP_AND;
            if(!strcmpi(pszToken, _T("ANDN")))  return IL_OP_ANDN;
            break;
        case _T('c'):
        case _T('C'):
            if(!strcmpi(pszToken, _T("CAL")))   return IL_OP_CAL;
            if(!strcmpi(pszToken, _T("CALC")))  return IL_OP_CALC;
            if(!strcmpi(pszToken, _T("CALCN"))) return IL_OP_CALCN;
            if(!strcmpi(pszToken, _T("CLK")))   return IL_OP_CLK;
            if(!strcmpi(pszToken, _T("CU")))    return IL_OP_CU;
            if(!strcmpi(pszToken, _T("CD")))    return IL_OP_CD;
            break;
        case _T('d'):
        case _T('D'):
            if(!strcmpi(pszToken, _T("DIV")))   return IL_OP_DIV;
            break;
        case _T('e'):
        case _T('E'):
            if(!strcmpi(pszToken, _T("EQ")))    return IL_OP_EQ;
            break;
        case _T('g'):
        case _T('G'):
            if(!strcmpi(pszToken, _T("GE")))    return IL_OP_GE;
            if(!strcmpi(pszToken, _T("GT")))    return IL_OP_GT;
            break;
        case _T('i'):
        case _T('I'):
            if(!strcmpi(pszToken, _T("IN")))    return IL_OP_IN;
            break;
        case _T('j'):
        case _T('J'):
            if(!strcmpi(pszToken, _T("JMP")))   return IL_OP_JMP;
            if(!strcmpi(pszToken, _T("JMPC")))  return IL_OP_JMPC;
            if(!strcmpi(pszToken, _T("JMPCN"))) return IL_OP_JMPCN;
            break;
        case _T('l'):
        case _T('L'):
            if(!strcmpi(pszToken, _T("LD")))    return IL_OP_LD;
            if(!strcmpi(pszToken, _T("LDN")))   return IL_OP_LDN;
            if(!strcmpi(pszToken, _T("LE")))    return IL_OP_LE;
            if(!strcmpi(pszToken, _T("LT")))    return IL_OP_LT;
            break;
        case _T('m'):
        case _T('M'):
            if(!strcmpi(pszToken, _T("MOD")))   return IL_OP_MOD;
            if(!strcmpi(pszToken, _T("MUL")))   return IL_OP_MUL;
            break;
        case _T('n'):
        case _T('N'):
            if(!strcmpi(pszToken, _T("NE")))    return IL_OP_NE;
            if(!strcmpi(pszToken, _T("NOT")))   return IL_OP_NOT;
            break;
        case _T('o'):
        case _T('O'):
            if(!strcmpi(pszToken, _T("OR")))    return IL_OP_OR;
            if(!strcmpi(pszToken, _T("ORN")))   return IL_OP_ORN;
            break;
        case _T('p'):
        case _T('P'):
            if(!strcmpi(pszToken, _T("PT")))    return IL_OP_PT;
            if(!strcmpi(pszToken, _T("PV")))    return IL_OP_PV;
            break;
        case _T('r'):
        case _T('R'):
            if(!strcmpi(pszToken, _T("R")))     return IL_OP_R;
            if(!strcmpi(pszToken, _T("R1")))    return IL_OP_R1;
            if(!strcmpi(pszToken, _T("RET")))   return IL_OP_RET;
            if(!strcmpi(pszToken, _T("RETC")))  return IL_OP_RETC;
            if(!strcmpi(pszToken, _T("RETCN"))) return IL_OP_RETCN;
            break;
        case _T('s'):
        case _T('S'):
            if(!strcmpi(pszToken, _T("S")))     return IL_OP_S;
            if(!strcmpi(pszToken, _T("S1")))    return IL_OP_S1;
            if(!strcmpi(pszToken, _T("ST")))    return IL_OP_ST;
            if(!strcmpi(pszToken, _T("STN")))   return IL_OP_STN;
            if(!strcmpi(pszToken, _T("SUB")))   return IL_OP_SUB;
            break;
        case _T('x'):
        case _T('X'):
            if(!strcmpi(pszToken, _T("XOR")))   return IL_OP_XOR;
            if(!strcmpi(pszToken, _T("XORN")))  return IL_OP_XORN;
            break;
        case _T(')'):
            if(!strcmpi(pszToken, _T(")")))     return IL_OP_RPAR;
            break;
        case _T('&'):
            if(!strcmpi(pszToken, _T("&")))     return IL_OP_AND;
            if(!strcmpi(pszToken, _T("&N")))    return IL_OP_ANDN;
            break;
    }
    return -1;
}



IL_OP il_ParseContext::getILOp(int op)
{
    switch(op)
    {
    case IL_OP_ADD:   return IL_ADD;   
    case IL_OP_AND:   return IL_AND;   
    case IL_OP_ANDN:  return IL_ANDN;  
    case IL_OP_CAL:   return IL_CAL;   
    case IL_OP_CALC:  return IL_CALC;  
    case IL_OP_CALCN: return IL_CALCN; 
    case IL_OP_CLK:   return IL_CLK;   
    case IL_OP_CU:    return IL_CU;    
    case IL_OP_CD:    return IL_CD;    
    case IL_OP_DIV:   return IL_DIV;   
    case IL_OP_EQ:    return IL_EQ;    
    case IL_OP_GE:    return IL_GE;    
    case IL_OP_GT:    return IL_GT;    
    case IL_OP_IN:    return IL_IN;    
    case IL_OP_JMP:   return IL_JMP;   
    case IL_OP_JMPC:  return IL_JMPC;  
    case IL_OP_JMPCN: return IL_JMPCN; 
    case IL_OP_LD:    return IL_LD;    
    case IL_OP_LDN:   return IL_LDN;   
    case IL_OP_LE:    return IL_LE;    
    case IL_OP_LT:    return IL_LT;    
    case IL_OP_MOD:   return IL_MOD;   
    case IL_OP_MUL:   return IL_MUL;   
    case IL_OP_NE:    return IL_NE;    
    case IL_OP_NOT:   return IL_NOT;   
    case IL_OP_OR:    return IL_OR;    
    case IL_OP_ORN:   return IL_ORN;   
    case IL_OP_PT:    return IL_PT;    
    case IL_OP_PV:    return IL_PV;    
    case IL_OP_R:     return IL_R;     
    case IL_OP_R1:    return IL_R1;    
    case IL_OP_RET:   return IL_RET;   
    case IL_OP_RETC:  return IL_RETC;  
    case IL_OP_RETCN: return IL_RETCN; 
    case IL_OP_S:     return IL_S;     
    case IL_OP_S1:    return IL_S1;    
    case IL_OP_ST:    return IL_ST;    
    case IL_OP_STN:   return IL_STN;   
    case IL_OP_SUB:   return IL_SUB;   
    case IL_OP_XOR:   return IL_XOR;   
    case IL_OP_XORN:  return IL_XORN;  
    case IL_OP_RPAR:  return IL_RPAR;
    }
    return IL_OP_UNDEF;
}
