/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/asm/asm.y 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: asm.y $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/asm/asm.y $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			ASM
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ------------------------------------------------------------------------------
 * Change Log
 *
 * Ref.		Date	 Author	Comment
 *
 * ##
 * ------------------------------------------------------------------------------
 */
%{

#yy_movetop #include "asm_ParseContext.h"
#yy_movetop #include "globalContext.h"
%}


%union 
{
    ST_LineCol               pos;
    ST_IDENT_TYP             id;
    ASM_DIRECT_TYP           direct;
    ASM_LITERAL_TYP          lit;
    ASM_INITIALISER_TYP      init;
    ASM_DECL_SCOPE           dscope;
    ASM_PRIMARY_TYP          argtyp;
    ASM_INST_PATH_TYP        path;
    const ASM_DATA_TYP*      datatype;
	ASM_MEM_REGION			 region;
	ASM_INIT_REPEAT_VAL_TYP  repeatVal;
    unsigned long            auLong;
    const ASM_OPCDEF*        pOPC;
};

%token<pos>    IMPORT WSTRING_BEG STRING_BEG EOL
%token<pos>    TARGET PRG FB FUN STRUCT RESOURCE TASK CLSSID END_SECTION
%token<pos>    DECL_VAR DECL_IN DECL_OUT DECL_INOUT DECL_INEXT CONFIG COPY DECL_ALIGN8
%token<pos>    GLOBAL GLOBAL_RETAIN BECOMES BP STMTBEG JMP JMPC JMPCN
%token<pos>    CALL READ WRITE CHANNELS CHANNEL
%token<id>     IDENTIFIER CALOP ST_INFO
%token<direct> DIRECT_VARIABLE 
%token<lit>    LITERAL_NOSTR LITERAL_OFFSET LITERAL_OBJID
%token<auLong> ANY_UNSIGNED
%token<pOPC>   VMOPC


%type <lit>       LITERAL opt_LITERAL ARRAY_INIT_VAL
%type <init>      opt_InitVal
%type <path>      instpath
%type <id>        opt_identifier 
%type <dscope>    declScope
%type <argtyp>    expression
%type <direct>    opt_DIRECT_VARIABLE
%type <datatype>  type
%type <region>    memory_region
%type <repeatVal> ARRAY_INIT
 
%{
/* to avoid 1000 Warning of the same kind */
#pragma  warning( disable : 4309 4305 4102 4244)
%}


%%
file
 : opt_target_section opt_asmSections 
 ;

opt_asmSections 
 : asmSections 
 | /* empty */
 ;

asmSections 
 : asmSection
 | asmSections asmSection
 ; 

asmSection
 : fun_section           EOL
 | fb_section            EOL
 | prg_section           EOL
 | global_section        EOL
 | global_retain_section EOL
 | struct_section        EOL
 | resource_section      EOL
 | task_section          EOL
 | config_section        EOL
 | copy_section          EOL
 | channels_section      EOL
 ;


opt_target_section
 : target_section
 {
    const ASM_TARGET_ATTRIBUTES* pAttr = GetGC()->getTrgAttr();
    
    //check for required attributes:
    if(pAttr->trg_type.isEmpty())
        ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_MISSING, _T(".target attribute: targettype"));
   
    if(pAttr->trg_version == 0)
        ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_MISSING, _T(".target attribute: firmwareversion"));

    if(pAttr->trg_instAlignBytes <= 0)
        ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_MISSING, _T(".target attribute: instAlignBytes"));

    if(pAttr->trg_stackAlignBytes <= 0)
        ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_MISSING, _T(".target attribute: stackAlignBytes"));

    if(pAttr->trg_bigEndian < 0)
        ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_MISSING, _T(".target attribute: bigEndian"));

    if(pAttr->trg_minOperandSize < 0)
        ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_MISSING, _T(".target attribute: minOperandSize"));

    if(pAttr->trg_maxObjects == 0)
        ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_MISSING, _T(".target attribute: maxobjects"));
 }
 | /* empty */ 
 {
    ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_EXPECTED, _T("'.target' section"));
 }
 ;

target_section 
 : TARGET  EOL {pContext->m_bEnableInstr = false;}
   opt_attribute_list
   END_SECTION EOL
 ;


opt_attribute_list
 : attribute_list
 | /* NIL */
 ;


attribute_list
 : attribute EOL
 | attribute_list attribute EOL
 ;

attribute
 : IDENTIFIER LITERAL 
 {
    GetGC()->addTargetAttribute(pContext->getEdiPos(), $1, $2);
 }
 ;

fun_section
 : FUN IDENTIFIER opt_LITERAL EOL {pContext->m_bEnableInstr = true;}
   { pContext->addProc(pContext->getEdiPos($1), $2, ASM_FUN, $3);}
   opt_clssid_assign
   opt_stmt_list
   END_SECTION
 ;

fb_section
 : FB IDENTIFIER opt_LITERAL EOL {pContext->m_bEnableInstr = true;}
   { pContext->addProc(pContext->getEdiPos($1), $2, ASM_FB, $3);}
   opt_clssid_assign
   opt_stmt_list
   END_SECTION
 ;

prg_section
 : PRG IDENTIFIER opt_LITERAL EOL {pContext->m_bEnableInstr = true;}
   { pContext->addProc(pContext->getEdiPos($1), $2, ASM_PRG, $3);}
   opt_clssid_assign
   opt_stmt_list
   END_SECTION
 ;

opt_clssid_assign
 : CLSSID LITERAL EOL    {pContext->assignClssid(pContext->getEdiPos($1), $2);}
 | /* NIL */
 ;


global_retain_section
  : GLOBAL_RETAIN EOL {pContext->m_bEnableInstr = false; pContext->m_bIsRetain = true;}
    opt_global_decls
    END_SECTION {pContext->m_bIsRetain = false;}
  ;

global_section
  : GLOBAL EOL {pContext->m_bEnableInstr = false; pContext->m_bIsRetain = false;}
    opt_global_decls
    END_SECTION
  ;

struct_section
  : STRUCT IDENTIFIER EOL
    {
        pContext->m_bEnableInstr = false;
        ASM_LITERAL_TYP mone;
        mone.type = ASM_DINT;
        mone.any64= -1;
        pContext->addProc(pContext->getEdiPos($1), $2, ASM_STRUCT, mone);
    }
    opt_stmt_list
    END_SECTION
  ;

resource_section
 : RESOURCE  EOL {pContext->m_bEnableInstr = false;}
   opt_task_properties
   END_SECTION
 ;
opt_task_properties
 : task_properties
 | /* NIL */
 ;
task_properties
 : task_property EOL
 | task_properties task_property EOL
 ;
task_property
 : IDENTIFIER LITERAL LITERAL LITERAL LITERAL LITERAL LITERAL LITERAL
 {
    GetGC()->addTaskProperties(pContext->getEdiPos(), $1, $2, $3, $4, $5, $6, $7, $8);
 }
 ;


task_section
 : TASK  IDENTIFIER  EOL
   {
       pContext->m_bEnableInstr = true; //<-TODO: is this correct ? should be false ??
       GetGC()->setActualTask(pContext->getEdiPos($1), &($2));
       pContext->setActualTask(pContext->getEdiPos($1), &($2));
   }
   opt_task_proginst_readwrite_list
   END_SECTION
 ;
opt_task_proginst_readwrite_list
 : task_proginst_readwrite_list
 | /* NIL */
 ;

task_proginst_readwrite_list
 : task_proginst_readwrite EOL
 | task_proginst_readwrite_list task_proginst_readwrite EOL
 ;

task_proginst_readwrite
 : CALL IDENTIFIER
 {
    GetGC()->addTaskPrginstAssignment(pContext->getEdiPos(), $2);
 }
 | READ IDENTIFIER ':' memory_region_list_opt 
 {
    pContext->appendRegionList(pContext->getEdiPos($1), $2, ASM_READ_LIST);
 }
 | WRITE IDENTIFIER ':' memory_region_list_opt
 {
    pContext->appendRegionList(pContext->getEdiPos($1), $2, ASM_WRITE_LIST);
 }
 ;

memory_region_list_opt
 : memory_region_list
 | /* NULL */
 {
	pContext->m_tmpRegionList.setUsed(0);
 }
 ;

memory_region_list
 : memory_region   
 {
	pContext->m_tmpRegionList.setUsed(0);
	pContext->m_tmpRegionList.add($1);
 }
 | memory_region_list ',' memory_region 
 {
	pContext->m_tmpRegionList.add($3);
 }
 ;

memory_region
 : '(' ANY_UNSIGNED ',' ANY_UNSIGNED ')'
 {
    static char buf[40];
	$$.ulOffset = $2;
    if ($4 > 65535)
    {
        ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_INT_LITERAL, _ultot($4, buf, 16));
    }
    $$.uSize = $4;
 }
 ;



config_section
  : CONFIG EOL {pContext->m_bEnableInstr = false;}
    opt_config_decls
    END_SECTION
  ;

opt_config_decls
 : config_decls
 | /* NIL */
 ;

config_decls
 : config_decl EOL
 | config_decls config_decl EOL
 ;

config_decl
 : instpath opt_InitVal
 { 
    if ($2.initType != ASM_INIT_UNDEF)
        GetGC()->appendConfigData(pContext->getEdiPos(), &$1, $2); 
 }
 ;  


copy_section
  : COPY IDENTIFIER opt_identifier EOL
    {
      pContext->m_bEnableInstr = false;
      pContext->m_tmpCopyItemList.setUsed(0); 
    }
    opt_copy_item_list
    {
        CASMCopySection* pCPS = new CASMCopySection(
           $2, 
           $3, 
           pContext->m_tmpCopyItemList.getUsed(),
           pContext->m_tmpCopyItemList.getMemCopy()
        );
        pContext->m_tmpCopyItemList.setUsed(0);
        GetGC()->addCopySection(pCPS);
    }
    END_SECTION
  ;

opt_identifier
  : IDENTIFIER  { $$ = $1;}
  | /* NIL */   { $$.psz = NULL;} 
  ;

opt_copy_item_list
 : copy_item_list
 | /* NIL */
 ;

copy_item_list
 : instpath opt_memberList EOL
 {
    pContext->addCopyItem($1, &pContext->m_tmpIdList);
 }
 | copy_item_list instpath opt_memberList EOL
 {
    pContext->addCopyItem($2, &pContext->m_tmpIdList);
 }
 ;

opt_memberList
 : '(' ident_list ')' 
 | /* NIL */  {  pContext->m_tmpIdList.setUsed(0); }

 ;

ident_list
 : IDENTIFIER                  
 {
    pContext->m_tmpIdList.setUsed(0); 
    pContext->m_tmpIdList.add($1);
 }
 | ident_list ',' IDENTIFIER
 {
    pContext->m_tmpIdList.add($3);
 }
 ;
 


channels_section
 : CHANNELS EOL
   {
      pContext->m_bEnableInstr = false;
   }
   opt_channel_list
   {
   }
   END_SECTION
 ;

opt_channel_list
 : channel_list
 | /* empty */
 ;

channel_list
 : channel
 | channel_list channel
 ;

channel
 : CHANNEL LITERAL EOL
   {
        GetGC()->addChannel(ASM_ForceDINT(pContext->getEdiPos(), &$2));
   }
   channel_attr_list
   END_SECTION EOL
 ;    
channel_attr_list
 : channel_attr EOL
 | channel_attr_list channel_attr EOL
 ;

channel_attr
 : IDENTIFIER LITERAL 
 {
    GetGC()->addChannelAttribute(pContext->getEdiPos(), $1.psz, &$2);
 }
 ;










opt_global_decls
 : global_decls
 | /* NIL */
 ;

global_decls
 : global_decl EOL
 | global_decls global_decl EOL
 ;
  
global_decl
 : type IDENTIFIER opt_DIRECT_VARIABLE opt_InitVal
 {
    ASM_SEGM_TYP segm;
    if($1->type==ASM_PRGI)
        segm = pContext->m_bIsRetain ? ASM_SEGM_RETAIN : ASM_SEGM_INST;
    else
        segm = pContext->m_bIsRetain ? ASM_SEGM_RETAIN : ASM_SEGM_GLOBAL;

    if (!GetGC()->addGlobalDecl(pContext->getEdiPos(), $1, $2, segm, $3, $4))
        YYABORT;
 }
 ;

opt_DIRECT_VARIABLE
 : IDENTIFIER DIRECT_VARIABLE  
 {
    if(_tcscmp(_T("at"), $1.psz))
        ST_DiagMsg2(pContext->getEdiPos(), ST_MISSING_X_BEFOR_Y, _T("at"),  _T("%"));
    
    $$ = $2;
 }
 | /* NIL */
 {
    ASM_InitDirectAddr(&$$);
 } 
 ;

opt_InitVal
 : BECOMES LITERAL
 {
    $$.initType = ASM_INIT_LIT;
    $$.lit      = $2;
 }
 | BECOMES '[' ARRAY_INIT_LIST ']'
 {
    $$.initType  = ASM_INIT_LIST;
    $$.pInitList = pContext->getInitList();
 }
 | /* NIL */
 {
    $$.initType = ASM_INIT_UNDEF;
 }
 ;

ARRAY_INIT_LIST
 : ARRAY_INIT
 { 
    pContext->m_InitList.setUsed(0);
    pContext->m_InitList.add($1);
 }
 | ARRAY_INIT_LIST ',' ARRAY_INIT
 { 
    pContext->m_InitList.add($3);
 }
 ;

ARRAY_INIT
 : LITERAL
 {
    $$.nRepeat   = 1;
    $$.nListSize = 1;
    $$.pLitList  = pContext->makeLiteralList($1);
 }
 | LITERAL '(' ARRAY_LIT_LIST ')'
 {
    $$.nRepeat   = (int) $1.any64;
    $$.nListSize = pContext->m_LitList.getUsed();
    $$.pLitList  = pContext->getLiteralList();
 }
 | LITERAL '(' ')'
 {
    $$.nRepeat   = (int) $1.any64;
    $$.nListSize = 0;
    $$.pLitList  = NULL;
 }
 ;

ARRAY_LIT_LIST
 : ARRAY_INIT_VAL
 {
    pContext->m_LitList.setUsed(0);
    pContext->m_LitList.add($1);
 }
 | ARRAY_LIT_LIST ',' ARRAY_INIT_VAL
 {
    pContext->m_LitList.add($3);
 }
 ;

ARRAY_INIT_VAL
 : LITERAL
 {
    $$ = $1;
 }
 | '<' '>'
 {
    /* according to ASM syntax: '[..., n(val,<>,<>,<>), ...]' */
    /* "literals" that are '<>' are indicated by ASM_VOID     */
    $$.type = ASM_VOID;
 }
 ;
opt_LITERAL
 : LITERAL           {$$ = $1;}
 | /* NIL */         {$$.type = ASM_TYPE_UNDEF;}
 ;


LITERAL
 : LITERAL_NOSTR
 | LITERAL_OFFSET
 | LITERAL_OBJID
 | WSTRING_BEG
 {
    ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_TYPE_NOT_IMPL, _T("WSTRING"));
    $$.type = ASM_TYPE_UNDEF;
 }
 | STRING_BEG
 {
    if(!ASM_SkipIECStringLiteral(pContext->m_pScanner, _T('\''), true))
    {
       ST_DiagMsg0(pContext->getEdiPos(), E_4CCG_ERR_UNTERM_STRING);
       YYABORT;
    }
    else
    {
        $$.pszIECStr = GetGC()->registerStringLit(ASM_GetSkippedTextBlock(pContext->m_pScanner));
        $$.type = ASM_STRING;
    }
 }
 ;


opt_stmt_list
 : stmt_list
 | /* NIL */
 ;

stmt_list
 : stmt  EOL     
 | stmt_list stmt EOL
 ;

stmt
 : declScope type IDENTIFIER opt_DIRECT_VARIABLE opt_InitVal 
 { 
    pContext->m_curProc->addDecl(pContext->getEdiPos(), $3, $1, $2, $4, $5);
 }
 |  DECL_ALIGN8
 { 
    pContext->m_curProc->m_alignNextDecl = 8;
 }
 | BP LITERAL
 {
    pContext->makeBpPos($1, &$2, true);
 }
 | STMTBEG LITERAL
 {
    pContext->makeBpPos($1, &$2, false);
 }
 | ST_INFO
 {
    ASM_PRIMARY_TYP     arg;
    arg.argkind = ASM_PRM_LABEL;
    arg.label   = $1;
    ASM_PRIMARY_TYP*     args[]={&arg};
    new ASM_INSTR_TYP(pContext->getEdiPos(), ASM_ST_INFO, pContext->m_curProc, NULL, 1, args);
 }
 | CALOP 
 {
    pContext->makeCalop(pContext->m_currPos, $1);
 }
 | VMOPC 
 {
    new ASM_INSTR_TYP(pContext->getEdiPos(), ASM_VMOPC, pContext->m_curProc, $1, 0, NULL);
 }
 | VMOPC expression 
 {
    ASM_PRIMARY_TYP* args[] = {&$2};
    new ASM_INSTR_TYP(pContext->getEdiPos(), ASM_VMOPC, pContext->m_curProc, $1, 1, args);
 }
 | VMOPC expression expression 
 {
    ASM_PRIMARY_TYP* pArg[] = {&$2, &$3};
    new ASM_INSTR_TYP(pContext->getEdiPos(), ASM_VMOPC, pContext->m_curProc, $1, 2, pArg);
 }
 | VMOPC expression expression expression 
 {
    ASM_PRIMARY_TYP* pArg[] = {&$2, &$3, &$4};
    new ASM_INSTR_TYP(pContext->getEdiPos(), ASM_VMOPC, pContext->m_curProc, $1, 3, pArg);
 }
 | JMP IDENTIFIER
 {
    pContext->makeJmp($1, ASM_JMP, $2);
 }
 | JMPC IDENTIFIER
 { 
    pContext->makeJmp($1, ASM_JMPC, $2);
 }
 | JMPCN IDENTIFIER
 { 
    pContext->makeJmp($1, ASM_JMPCN, $2);
 }
 | ':' IDENTIFIER {pContext->makeLabel($2);}
 ;




declScope
 : DECL_VAR    {$$ = ASM_DECL_VAR;}
 | DECL_IN     {$$ = ASM_DECL_IN;}
 | DECL_INEXT  {$$ = ASM_DECL_INEXT;}
 | DECL_OUT    {$$ = ASM_DECL_OUT;} 
 | DECL_INOUT  {$$ = ASM_DECL_INOUT;} 
 ;

expression
 : LITERAL    
 { 
    $$.argkind = ASM_PRM_LIT; 
    $$.lit = $1;
 }
 | IDENTIFIER '(' type ')'
 {
    if(!_tcscmp($1.psz , _T("aidx")))
    {
        if($3->type!=ASM_INT && $3->type!=ASM_DINT)
        {
            ST_DiagMsg1(pContext->getEdiPos(), E_ASM_BAD_AIDX, $3->name.psz);
            $$.argkind = ASM_PRM_NONE; 
        }
        else
        {
            $$.argkind = ASM_PRM_AIDX; 
            $$.pType = $3;
        }
    }
    else if(!_tcscmp($1.psz , _T("sizeof8")))
    {
        $$.argkind = ASM_PRM_SIZEOF8; 
        $$.pType = $3;
    }
    else if(!_tcscmp($1.psz , _T("sizeof1")))
    {
        $$.argkind = ASM_PRM_SIZEOF1; 
        $$.pType = $3;
    }
    else if(!_tcscmp($1.psz , _T("clsid")))
    {
        ASM_PROC_ENUM pout = $3->pFBIDef ? $3->pFBIDef->getPouTyp() : ASM_PROC_TYP_UNDEF;
        if(!(pout==ASM_FUN || pout==ASM_FB))
        {
            ST_DiagMsg1(pContext->getEdiPos(), E_ASM_BAD_CLSID, $3->name.psz);
            $$.argkind = ASM_PRM_NONE; 
        }
        else
        {
            $$.argkind = ASM_PRM_CLSID; 
            $$.pType = $3;
        }
    }
    else if(!_tcscmp($1.psz , _T("fbid")))
    {
        ASM_PROC_ENUM pout = $3->pFBIDef ? $3->pFBIDef->getPouTyp() : ASM_PROC_TYP_UNDEF;
        if(pout!=ASM_FB || !$3->pFBIDef->isBuildIn())
        {
            ST_DiagMsg1(pContext->getEdiPos(), E_ASM_BAD_FBID, $3->name.psz);
            $$.argkind = ASM_PRM_NONE; 
        }
        else
        {
            $$.argkind = ASM_PRM_FBID; 
            $$.lId   = $3->pFBIDef->getOpCode();
        }
    }
    else if(!_tcscmp($1.psz , _T("funlocbytes")))
    {
        ASM_PROC_ENUM pout = $3->pFBIDef ? $3->pFBIDef->getPouTyp() : ASM_PROC_TYP_UNDEF;
        if(pout!=ASM_FUN)
        {
            ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_UNKNOWN_FUNCT, $3->name.psz);
            $$.argkind = ASM_PRM_NONE; 
        }
        else
        {
            $$.argkind = ASM_PRM_FUNLOCBYTES; 
            $$.pType = $3;
        }
    }
    else if(!_tcscmp($1.psz , _T("leavebytes")))
    {
        ASM_PROC_ENUM pout = $3->pFBIDef ? $3->pFBIDef->getPouTyp() : ASM_PROC_TYP_UNDEF;
        if(pout!=ASM_FUN)
        {
            ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_UNKNOWN_FUNCT, $3->name.psz);
            $$.argkind = ASM_PRM_NONE; 
        }
        else
        {
            $$.argkind = ASM_PRM_LEAVEBYTES; 
            $$.pType = $3;
        }
    }
    else
    {
        pContext->tokenSyntax($1.psz);
        $$.argkind = ASM_PRM_NONE; 
    }
 }

 | instpath   
 { 
    pContext->checkVarReference($1, &$$);
 }
 ;


instpath
 : IDENTIFIER               { $$ = GetGC()->registerInstPathPlusMember(NULL, $1);}
 | instpath '.' IDENTIFIER  { $$ = GetGC()->registerInstPathPlusMember(&$1, $3);}
 | instpath '.' LITERAL     { $$ = GetGC()->registerInstArrayPath($1, &$3);}
 ;


type 
 : IDENTIFIER  
 { 
    $$ = pContext->makeType($1, false, NULL, NULL); 
 }
 | IDENTIFIER '[' LITERAL ']'
 { 
    $$ = pContext->makeType($1, true, &$3, NULL); 
 }
 | IDENTIFIER '['  ']' 
 { 
    $$ = pContext->makeType($1, true, NULL, NULL);
 }
 | IDENTIFIER '(' LITERAL ')' '['  ']' 
 { 
    $$ = pContext->makeType($1, true, NULL, &$3); 
 }
 | IDENTIFIER '(' LITERAL ')' '[' LITERAL ']' 
 {  
    $$ = pContext->makeType($1, true, &$6, &$3); 
 }
 | IDENTIFIER '(' LITERAL ')' 
 { 
    $$ = pContext->makeType($1, false, NULL, &$3); 
 }
 ;

%%




int asm_parse::yynexttoken(YYSTYPE* yylval)
{
    int                         tok;
    const TCHAR*                yytext;
    ST_LineCol                  pos;


nextToken:  
    tok = ASM_NextToken(pContext->m_pScanner, &yytext, 
                        &pos.nLine, &pos.nColumn);

    //printf (">>%s<<\n", yytext);

    yylval->pos = pos;
    pContext->m_currPos = pos;

    switch(tok)
    {
    case ASM_TOK_TOKEN_TO_LONG:
        ST_DiagMsg0(pContext->getEdiPos(), E_4CCG_ERR_TOKEN_TO_LONG);
        //CAUTION: you must skip this line otherwise you get an endless loop
        ASM_SkipLine(pContext->m_pScanner, false);
        goto nextToken; 

    case ASM_TOK_SYNTAX_ERROR:
        pContext->tokenSyntax(yytext);
        goto nextToken;
    case ASM_TOK_IDENTIFIER_EX:
        yylval->id = GetGC()->registerId(yytext);
        return IDENTIFIER;
    case ASM_TOK_CALOP:
        if(pContext->m_bEnableInstr)
        {
            FC_StringSkipWhite(&yytext);
            yylval->pOPC = GetGC()->getOpc(yytext);
            if(yylval->pOPC)
                return VMOPC;

            //TODO should be dumped as upper case by backend:
            TCHAR buffer[1024];
            strcpy(buffer, yytext);
            strupr(buffer);

            if( ! GetGC()->getOpcode(buffer))
            {
                ST_DiagMsg0(pContext->getEdiPos(), E_4CCG_ERR_SYNTAX);
                ASM_SkipLine(pContext->m_pScanner, false);
                goto nextToken; 
            }
            yylval->id = GetGC()->registerId(buffer);
            return CALOP;
        }
        break;
    case ASM_TOK_DIRECT_VARIABLE:
        if(!ASM_ParseDirectAddr(yytext, &yylval->direct))
            ST_DiagMsg2(pContext->getEdiPos(), E_4CCG_ERR_ADDR_SYNTAX, yytext, _T("%[IQM]([BWDL]<byteoff> | {X}<byteoff>.<bitoff>"));
        return DIRECT_VARIABLE;

    //literals:
    case ASM_TOK_LITERAL:
        ASM_LitTokenToLiteral(pContext->getEdiPos(pos), yytext, &yylval->lit);
        return LITERAL_NOSTR;

    case ASM_TOK_ANY_UNSIGNED:
    {
        TCHAR* pszEnd = NULL;
		if(yytext[1] == 'x' || yytext[1] == 'X') //void 009 to be parsed as octal by _tcstoul
			yylval->auLong = _tcstoul(yytext, &pszEnd, 16);
		else
			yylval->auLong = _tcstoul(yytext, &pszEnd, 10);
        if (pszEnd == NULL || *pszEnd != 0)
            ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_INT_LITERAL, yytext);

        return ANY_UNSIGNED;
    }
    case ASM_TOK_LITERAL_OFFSET:
        if(!ASM_LitOffsetToLiteral(yytext, &yylval->lit))
        {
            ST_DiagMsg0(pContext->getEdiPos(), E_4CCG_ERR_SYNTAX);
            ASM_SkipLine(pContext->m_pScanner, false);
            goto nextToken; 
        }
        return LITERAL_OFFSET;

    case ASM_TOK_LITERAL_OBJID:
        if(!ASM_LitObjidToLiteral(yytext, &yylval->lit))
        {
            ST_DiagMsg0(pContext->getEdiPos(), E_4CCG_ERR_SYNTAX);
            ASM_SkipLine(pContext->m_pScanner, false);
            goto nextToken; 
        }
        return LITERAL_OBJID;

    case ASM_TOK_STRING_LITERAL_BEGIN:  
        return *yytext==_T('"') ? WSTRING_BEG : STRING_BEG;

    //keywords & simple tokens:
    case ASM_TOK_TARGET:        return TARGET;
    case ASM_TOK_FUN:           return FUN;
    case ASM_TOK_FB:            return FB; 
    case ASM_TOK_PRG:           return PRG; 
    case ASM_TOK_GLOBAL:        return GLOBAL;
    case ASM_TOK_GLOBAL_RETAIN: return GLOBAL_RETAIN;
    case ASM_TOK_STRUCT:        return STRUCT;
    case ASM_TOK_RESOURCE:      return RESOURCE;
    case ASM_TOK_CLSSID:        return CLSSID;
    case ASM_TOK_CONFIG:        return CONFIG;
    case ASM_TOK_COPY:          return COPY;
    case ASM_TOK_CHANNELS:      return CHANNELS;
    case ASM_TOK_CHANNEL:       return CHANNEL;
    case ASM_TOK_END_SECTION:   return END_SECTION;
 
    case ASM_TOK_TASK:          return TASK;
    case ASM_TOK_VAR:           return DECL_VAR;
    case ASM_TOK_IN:            return DECL_IN;
    case ASM_TOK_INEXT:         return DECL_INEXT;
    case ASM_TOK_OUT:           return DECL_OUT;
    case ASM_TOK_INOUT:         return DECL_INOUT;
    case ASM_TOK_ALIGN8:        return DECL_ALIGN8;
    case ASM_TOK_BECOMES:       return BECOMES;
    
    //instructions: break and handle special case after switch:
    case ASM_TOK_BP:          tok = BP; break;
    case ASM_TOK_STMTBEG:     tok = STMTBEG; break;
    
    case ASM_TOK_ST_INFO:
        ASM_SkipLine(pContext->m_pScanner, true);
        yylval->id = GetGC()->registerId(ASM_GetSkippedTextBlock(pContext->m_pScanner));
        tok = ST_INFO; 
    break;
	case ASM_TOK_CALL:        tok = CALL; break;
    case ASM_TOK_JMP:         tok = JMP;  break;
    case ASM_TOK_JMPC:        tok = JMPC; break;
    case ASM_TOK_JMPCN:       tok = JMPCN; break;
	case ASM_TOK_READ:        tok = READ; break;
	case ASM_TOK_WRITE:       tok = WRITE; break;
    case '\n':                return EOL;
    default:
       //default: tokens <256 like '(' or ')' or ...
        return tok;
    }

    
    if(!pContext->m_bEnableInstr)
    {
        FC_StringSkipWhite(&yytext);
        yylval->id = GetGC()->registerId(yytext);
        tok =  IDENTIFIER;
    }
    return tok;
}

void asm_parse::yyerrorStackOverflow()
{
    ST_DiagMsg0(pContext->getEdiPos(), E_4CCG_ERR_YACC_STACK);
}
void asm_parse::yyerrorSyntax()
{
    ST_DiagMsg0(pContext->getEdiPos(), E_4CCG_ERR_SYNTAX);
}





/* Called from Outside */
bool ParseFile(FileContext_C* pfc)
{
    int              result;
    asm_ParseContext ctx(pfc);
    asm_parse        parser(&result);

    assert(result==0 || result==1);

    //open file:
    if(!ctx.asm_LoadFile())
        return false;

    //set the context for the parser: 
    parser.setParseContext(&ctx);

    //do the work:
    parser.yyparse();

    return true;
}


/* end of yacc file */
