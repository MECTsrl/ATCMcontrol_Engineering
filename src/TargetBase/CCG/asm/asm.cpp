/*!!!!! this file has been generated using yyreplace.pl !!!!!*/
/* input files for yyreplace.pl: asm.y, asm.cpp~, asm.h~*/

/*START yyreplace.pl: include files and #yy_movetop lines from asm.y */
 #include "asm_ParseContext.h"
 #include "globalContext.h"
/*END yyreplace.pl: include files from asm.y */


/*START yyreplace.pl: file asm.h~ */
typedef union {
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
} YYSTYPE;
const IMPORT = 257;
const WSTRING_BEG = 258;
const STRING_BEG = 259;
const EOL = 260;
const TARGET = 261;
const PRG = 262;
const FB = 263;
const FUN = 264;
const STRUCT = 265;
const RESOURCE = 266;
const TASK = 267;
const CLSSID = 268;
const END_SECTION = 269;
const DECL_VAR = 270;
const DECL_IN = 271;
const DECL_OUT = 272;
const DECL_INOUT = 273;
const DECL_INEXT = 274;
const CONFIG = 275;
const COPY = 276;
const DECL_ALIGN8 = 277;
const GLOBAL = 278;
const GLOBAL_RETAIN = 279;
const BECOMES = 280;
const BP = 281;
const STMTBEG = 282;
const JMP = 283;
const JMPC = 284;
const JMPCN = 285;
const CALL = 286;
const READ = 287;
const WRITE = 288;
const CHANNELS = 289;
const CHANNEL = 290;
const IDENTIFIER = 291;
const CALOP = 292;
const ST_INFO = 293;
const DIRECT_VARIABLE = 294;
const LITERAL_NOSTR = 295;
const LITERAL_OFFSET = 296;
const LITERAL_OBJID = 297;
const ANY_UNSIGNED = 298;
const VMOPC = 299;


#ifdef YYTRACE
#define YYDEBUG 1
#else
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#endif
// C++ YACC parser header
// Copyright 1991 by Mortice Kern Systems Inc.  All rights reserved.
//
// asm_parse => class defining a parsing object
//	asm_parse needs a class yy_, which defines the ner.
// %prefix or option -p xx determines name of this class; if not used,
// defaults to 'yy_'
//
// constructor fills in the tables for this grammar; give it a size
//    to determine size of state and value stacks. Default is 150 entries.
// destructor discards those state and value stacks
//
// int asm_parse::yyparse(yy_ *) invokes parse; if this returns,
//	it can be recalled to continue parsing at last point.
// void asm_parse::yyreset() can be called to reset the parse;
//	call yyreset() before asm_parse::yyparse(yy_ *)
#include <stdio.h>		// uses printf(), et cetera

const YYERRCODE = 256;		// YACC 'error' value

// You can use these macros in your action code
#define YYERROR		goto yyerrlabel
#define YYACCEPT	YYRETURN(0)
#define YYABORT		YYRETURN(1)
#define YYRETURN(val)	return(val)

/*
 * Simulate bitwise negation as if it was done ona two's complement machine.
 * This makes the generated code portable to machines with different 
 * representations of integers (ie. signed magnitude).
 */
#define yyneg(s)	(-((s)+1))

#if YYDEBUG
typedef struct yyNamedType_tag {	/* Tokens */
	char	* name;		/* printable name */
	short	token;		/* token # */
	short	type;		/* token type */
} yyNamedType;
typedef struct yyTypedRules_tag {	/* Typed rule table */
	char	* name;		/* compressed rule string */
	short	type;		/* rule result type */
} yyTypedRules;
#endif


class asm_ParseContext;
class asm_parse {
	asm_ParseContext* pContext;
public:
	asm_ParseContext* getParseContext(){return pContext;}
	void          setParseContext(asm_ParseContext* pc){pContext=pc;}

protected:
	int	mustfree;	// set if tables should be deleted
	int	size;		// size of state and value stacks
	int	reset;		// if set, reset state
	short	yyi;		// table index
	short	yystate;	// current state

	short	* stateStack;	// states stack
	YYSTYPE	* valueStack;	// values stack
	short	* yyps;		// top of state stack
	YYSTYPE * yypv;		// top of value stack

	YYSTYPE yylval;		// saved yylval
	YYSTYPE	yyval;		// $
	YYSTYPE * yypvt;	// $n
	int	yychar;		// current token
	int	yyerrflag;	// error flag
	int	yynerrs;	// error count
#if YYDEBUG
	int	done;		// set from trace to stop parse
	int	rule, npop;	// reduction rule # and length
	short	* typeStack;	// type stack to mirror valueStack[]
	short	* yytp;		// top of type stack
	char	* yygetState(int);	// read 'states.out'
#endif
public:
#if YYDEBUG
	// C++ has trouble with initialized arrays inside classes
	static long * States;		// pointer to yyStates[]
	static yyTypedRules * Rules;	// pointer to yyRules[]
	static yyNamedType * TokenTypes; // pointer to yyTokenTypes[]
	static int	yyntoken;	// number of tokens
	static int	yynvar;		// number of variables (nonterminals)
	static int	yynstate;	// number of YACC-generated states
	static int	yynrule;	// number of rules in grammar

	char*	yyptok(int);		// printable token string
	int	yyExpandName(int, int, char *, int);
						// expand encoded string
	virtual int	yyGetType(int);		// return type of token
	virtual void	yyShowRead();		// see newly read token
	virtual void	yyShowState();		// see state, value stacks
	virtual void	yyShowReduce();		// see reduction
	virtual void	yyShowGoto();		// see goto
	virtual void	yyShowShift();		// see shift
	virtual void	yyShowErrRecovery();	// see error recovery
	virtual void	yyShowErrDiscard();	// see token discard in error
#endif
	int	yydebug;	// if set, tracing if compiled with YYDEBUG=1

    // constructor for this grammar
    //pError=0 == no error
    //pError=1 == out of memory
	asm_parse(int* pError, int = 150); 

    // another constructor
    //pError=0 == no error
    //pError=1 == out of memory
    //pError=2 == bad state given
    asm_parse(int* pError, int, short *, YYSTYPE *);	

	~asm_parse();		// destructor

	int	yyparse();	
	int yynexttoken(YYSTYPE* val);
	void yyerrorSyntax();
	void yyerrorStackOverflow();

	void	yyreset() { reset = 1; } // restore state for next yyparse()

	void	setdebug(int y) { yydebug = y; }

// The following are useful in user actions:

	void	yyerrok() { yyerrflag = 0; }	// clear error
	void	yyclearin() { yychar = -1; }	// clear input
	int	YYRECOVERING() { return yyerrflag != 0; }
};
// end of .hpp header
/*END yyreplace.pl: file asm.h~ */
/*START yyreplace.pl: file asm.cpp~ */
/* yacc -lmv -LC -P C:\ATCMControl_SDK\GenWork\GenDir.tmp\4Control_R\gentools\yyparse.cpp -o asm.cpp~ -D asm.h~ asm.y */


/*yyreplace.pl: moved on top: #yy_movetop #include "asm_ParseContext.h" */
/*yyreplace.pl: moved on top: #yy_movetop #include "globalContext.h" */

/* to avoid 1000 Warning of the same kind */
#pragma  warning( disable : 4309 4305 4102 4244)

#if YYDEBUG
enum YY_Types { YY_t_NoneDefined, YY_t_pos, YY_t_id, YY_t_direct, YY_t_lit, YY_t_auLong, YY_t_pOPC, YY_t_init, YY_t_path, YY_t_dscope, YY_t_argtyp, YY_t_datatype, YY_t_region, YY_t_repeatVal
};
#endif
#if YYDEBUG
yyTypedRules yyRules[] = {
	{ "&00: %13 &00",  0},
	{ "%13: %14 %15",  0},
	{ "%15: %16",  0},
	{ "%15:",  0},
	{ "%16: %17",  0},
	{ "%16: %16 %17",  0},
	{ "%17: %18 &05",  0},
	{ "%17: %19 &05",  0},
	{ "%17: %20 &05",  0},
	{ "%17: %21 &05",  0},
	{ "%17: %22 &05",  0},
	{ "%17: %23 &05",  0},
	{ "%17: %24 &05",  0},
	{ "%17: %25 &05",  0},
	{ "%17: %26 &05",  0},
	{ "%17: %27 &05",  0},
	{ "%17: %28 &05",  0},
	{ "%14: %29",  0},
	{ "%14:",  0},
	{ "%30:",  0},
	{ "%29: &06 &05 %30 %31 &14 &05",  0},
	{ "%31: %32",  0},
	{ "%31:",  0},
	{ "%32: %33 &05",  0},
	{ "%32: %32 %33 &05",  0},
	{ "%33: &36 %01",  0},
	{ "%34:",  0},
	{ "%35:",  0},
	{ "%18: &09 &36 %02 &05 %34 %35 %36 %37 &14",  0},
	{ "%38:",  0},
	{ "%39:",  0},
	{ "%19: &08 &36 %02 &05 %38 %39 %36 %37 &14",  0},
	{ "%40:",  0},
	{ "%41:",  0},
	{ "%20: &07 &36 %02 &05 %40 %41 %36 %37 &14",  0},
	{ "%36: &13 %01 &05",  0},
	{ "%36:",  0},
	{ "%42:",  0},
	{ "%22: &24 &05 %42 %43 &14",  0},
	{ "%44:",  0},
	{ "%21: &23 &05 %44 %43 &14",  0},
	{ "%45:",  0},
	{ "%23: &10 &36 &05 %45 %37 &14",  0},
	{ "%46:",  0},
	{ "%24: &11 &05 %46 %47 &14",  0},
	{ "%47: %48",  0},
	{ "%47:",  0},
	{ "%48: %49 &05",  0},
	{ "%48: %48 %49 &05",  0},
	{ "%49: &36 %01 %01 %01 %01 %01 %01 %01",  0},
	{ "%50:",  0},
	{ "%25: &12 &36 &05 %50 %51 &14",  0},
	{ "%51: %52",  0},
	{ "%51:",  0},
	{ "%52: %53 &05",  0},
	{ "%52: %52 %53 &05",  0},
	{ "%53: &31 &36",  0},
	{ "%53: &32 &36 &45 %54",  0},
	{ "%53: &33 &36 &45 %54",  0},
	{ "%54: %55",  0},
	{ "%54:",  0},
	{ "%55: %11",  0},
	{ "%55: %55 &46 %11",  0},
	{ "%11: &47 &43 &46 &43 &48",  12},
	{ "%56:",  0},
	{ "%26: &20 &05 %56 %57 &14",  0},
	{ "%57: %58",  0},
	{ "%57:",  0},
	{ "%58: %59 &05",  0},
	{ "%58: %58 %59 &05",  0},
	{ "%59: %05 %04",  0},
	{ "%60:",  0},
	{ "%62:",  0},
	{ "%27: &21 &36 %06 &05 %60 %61 %62 &14",  0},
	{ "%06: &36",  2},
	{ "%06:",  2},
	{ "%61: %63",  0},
	{ "%61:",  0},
	{ "%63: %05 %64 &05",  0},
	{ "%63: %63 %05 %64 &05",  0},
	{ "%64: &47 %65 &48",  0},
	{ "%64:",  0},
	{ "%65: &36",  0},
	{ "%65: %65 &46 &36",  0},
	{ "%66:",  0},
	{ "%68:",  0},
	{ "%28: &34 &05 %66 %67 %68 &14",  0},
	{ "%67: %69",  0},
	{ "%67:",  0},
	{ "%69: %70",  0},
	{ "%69: %69 %70",  0},
	{ "%71:",  0},
	{ "%70: &35 %01 &05 %71 %72 &14 &05",  0},
	{ "%72: %73 &05",  0},
	{ "%72: %72 %73 &05",  0},
	{ "%73: &36 %01",  0},
	{ "%43: %74",  0},
	{ "%43:",  0},
	{ "%74: %75 &05",  0},
	{ "%74: %74 %75 &05",  0},
	{ "%75: %10 &36 %09 %04",  0},
	{ "%09: &36 &39",  3},
	{ "%09:",  3},
	{ "%04: &25 %01",  7},
	{ "%04: &25 &49 %76 &50",  7},
	{ "%04:",  7},
	{ "%76: %12",  0},
	{ "%76: %76 &46 %12",  0},
	{ "%12: %01",  13},
	{ "%12: %01 &47 %77 &48",  13},
	{ "%12: %01 &47 &48",  13},
	{ "%77: %03",  0},
	{ "%77: %77 &46 %03",  0},
	{ "%03: %01",  4},
	{ "%03: &51 &52",  4},
	{ "%02: %01",  4},
	{ "%02:",  4},
	{ "%01: &40",  4},
	{ "%01: &41",  4},
	{ "%01: &42",  4},
	{ "%01: &03",  4},
	{ "%01: &04",  4},
	{ "%37: %78",  0},
	{ "%37:",  0},
	{ "%78: %79 &05",  0},
	{ "%78: %78 %79 &05",  0},
	{ "%79: %07 %10 &36 %09 %04",  0},
	{ "%79: &22",  0},
	{ "%79: &26 %01",  0},
	{ "%79: &27 %01",  0},
	{ "%79: &38",  0},
	{ "%79: &37",  0},
	{ "%79: &44",  0},
	{ "%79: &44 %08",  0},
	{ "%79: &44 %08 %08",  0},
	{ "%79: &44 %08 %08 %08",  0},
	{ "%79: &28 &36",  0},
	{ "%79: &29 &36",  0},
	{ "%79: &30 &36",  0},
	{ "%79: &45 &36",  0},
	{ "%07: &15",  9},
	{ "%07: &16",  9},
	{ "%07: &19",  9},
	{ "%07: &17",  9},
	{ "%07: &18",  9},
	{ "%08: %01",  10},
	{ "%08: &36 &47 %10 &48",  10},
	{ "%08: %05",  10},
	{ "%05: &36",  8},
	{ "%05: %05 &53 &36",  8},
	{ "%05: %05 &53 %01",  8},
	{ "%10: &36",  11},
	{ "%10: &36 &49 %01 &50",  11},
	{ "%10: &36 &49 &50",  11},
	{ "%10: &36 &47 %01 &48 &49 &50",  11},
	{ "%10: &36 &47 %01 &48 &49 %01 &50",  11},
	{ "%10: &36 &47 %01 &48",  11},
{ "$accept",  0},{ "error",  0}
};
yyNamedType yyTokenTypes[] = {
	{ "$end",  0,  0},
	{ "error",  256,  0},
	{ "IMPORT",  257,  1},
	{ "WSTRING_BEG",  258,  1},
	{ "STRING_BEG",  259,  1},
	{ "EOL",  260,  1},
	{ "TARGET",  261,  1},
	{ "PRG",  262,  1},
	{ "FB",  263,  1},
	{ "FUN",  264,  1},
	{ "STRUCT",  265,  1},
	{ "RESOURCE",  266,  1},
	{ "TASK",  267,  1},
	{ "CLSSID",  268,  1},
	{ "END_SECTION",  269,  1},
	{ "DECL_VAR",  270,  1},
	{ "DECL_IN",  271,  1},
	{ "DECL_OUT",  272,  1},
	{ "DECL_INOUT",  273,  1},
	{ "DECL_INEXT",  274,  1},
	{ "CONFIG",  275,  1},
	{ "COPY",  276,  1},
	{ "DECL_ALIGN8",  277,  1},
	{ "GLOBAL",  278,  1},
	{ "GLOBAL_RETAIN",  279,  1},
	{ "BECOMES",  280,  1},
	{ "BP",  281,  1},
	{ "STMTBEG",  282,  1},
	{ "JMP",  283,  1},
	{ "JMPC",  284,  1},
	{ "JMPCN",  285,  1},
	{ "CALL",  286,  1},
	{ "READ",  287,  1},
	{ "WRITE",  288,  1},
	{ "CHANNELS",  289,  1},
	{ "CHANNEL",  290,  1},
	{ "IDENTIFIER",  291,  2},
	{ "CALOP",  292,  2},
	{ "ST_INFO",  293,  2},
	{ "DIRECT_VARIABLE",  294,  3},
	{ "LITERAL_NOSTR",  295,  4},
	{ "LITERAL_OFFSET",  296,  4},
	{ "LITERAL_OBJID",  297,  4},
	{ "ANY_UNSIGNED",  298,  5},
	{ "VMOPC",  299,  6},
	{ "':'",  58,  0},
	{ "','",  44,  0},
	{ "'('",  40,  0},
	{ "')'",  41,  0},
	{ "'['",  91,  0},
	{ "']'",  93,  0},
	{ "'<'",  60,  0},
	{ "'>'",  62,  0},
	{ "'.'",  46,  0}

};
#endif
static short yydef[] = {

	   3, 65535, 65531,  158, 65527, 65523, 65519, 65515, 65511,  156, 
	65507, 65503, 65499, 65495, 65491,  144, 65487,  148, 65483,  153, 
	65479,   15,  142, 65475,  151,   11,  140, 65471, 65465,  146, 
	  14,   23,   12, 65461,  154,  154,  154, 65457,    9, 65453, 
	65449,   13, 65445,   16, 65439, 65435, 65431,  149, -109
};
static short yyex[] = {

	   0,  157, 65535,    1,    0,    0, 65535,    1,  269,  155, 
	65535,    1,  260,    5, 65535,    1,  260,   10, 65535,    1, 
	 260,   10, 65535,    1,  260,   10, 65535,    1,  269,  143, 
	65535,    1,  269,  147, 65535,    1,  269,  152, 65535,    1, 
	 269,  141, 65535,    1,  269,  141, 65535,    1,  260,    8, 
	65535,    1,  269,  150, 65535,    1,  269,  139, 65535,    1, 
	 269,  145, 65535,    1,  260,    7,  280,    7, 65535,    1, 
	 260,    6, 65535,    1,  260,    8, 65535,    1,  260,    6, 
	65535,    1,  260,    4, 65535,    1,  260,    4, 65535,    1, 
	 260,    7,  280,    7, 65535,    1,  269,  139, 65535,    1, 
	 269,  139, 65535,    1,  269,  139, 65535,    1,  260,    8, 
	65535,    1
};
static short yyact[] = {

	65373,  261, 65298,  260, 65382, 65383, 65384, 65379, 65378, 65377, 
	65376, 65375, 65380, 65381, 65374,  289,  279,  278,  276,  275, 
	 267,  266,  265,  264,  263,  262, 65329,  260, 65530,  291, 
	65320,  260, 65396,  291, 65311,  260, 65397,  291, 65309,  260, 
	65307,  260, 65529,  291, 65528,  291, 65527,  291, 65283,  260, 
	65284,  260, 65285,  260, 65286,  260, 65287,  260, 65288,  260, 
	65289,  260, 65290,  260, 65291,  260, 65292,  260, 65293,  260, 
	65398,  291, 65324,  291, 65313,  260, 65310,  260, 65346, 65347, 
	65255, 65254, 65253,  297,  296,  295,  259,  258, 65281,  260, 
	65406,  269, 65407,  290, 65322,  260, 65366,  291, 65410,  291, 
	65514,  291, 65304,  260, 65302,  260, 65300,  260, 65280,  260, 
	65282,  260, 65419, 65420,  280,   46, 65267,  260, 65268,  269, 
	65424, 65423, 65422,  288,  287,  286, 65273,  260, 65274,  269, 
	65429, 65359, 65360, 65362, 65363, 65361, 65349, 65434, 65433, 65432, 
	65431, 65430, 65353, 65352, 65510,  299,  293,  292,  285,  284, 
	 283,  282,  281,  277,  274,  273,  272,  271,  270,   58, 
	65438, 65439,   91,   40, 65508,  291, 65257,  260, 65276,  269, 
	65308,  269, 65331,  260, 65263,  269, 65346, 65347, 65367, 65255, 
	65254, 65253,  297,  296,  295,  291,  259,  258, 65441, 65346, 
	65347, 65255, 65254, 65253,  297,  296,  295,  259,  258,   91, 
	65266,  260, 65442,  291, 65443,  291, 65314,  291, 65270,  260, 
	65271,  269, 65272,  260, 65358,  291, 65357,  291, 65356,  291, 
	65355,  291, 65346, 65347, 65504, 65255, 65254, 65253,  297,  296, 
	 295,  291,  259,  258, 65252,  260, 65275,  269, 65370, 65346, 
	65347, 65255, 65254, 65253,  297,  296,  295,  259,  258,   93, 
	65450,  291, 65256,  260, 65452, 65419,   46,   40, 65496,   58, 
	65495,   58, 65269,  260, 65419,   46, 65457,   40, 65493,  291, 
	65251,  260, 65492,   41, 65369,   93, 65334,  294, 65420,  280, 
	65458,  268, 65459,  291, 65327,  291, 65325,  260, 65265,  269, 
	65464,   40, 65465, 65336,   93,   44, 65466,   40, 65469,   91, 
	65259,  260, 65475, 65459,  291,  269, 65264, 65476,   44,   41, 
	65326,  260, 65340, 65477, 65346, 65347, 65255, 65254, 65253,  297, 
	 296,  295,  259,  258,   60,   41, 65479,  298, 65480,   44, 
	65365,   41, 65371, 65346, 65347, 65255, 65254, 65253,  297,  296, 
	 295,  259,  258,   93, 65306,  260, 65277,  269, 65278,  269, 
	65279,  269, 65258,  260, 65260,  260, 65328,  291, 65344,   62, 
	65339, 65483,   44,   41, 65484,   44, 65372,   93, 65477, 65346, 
	65347, 65255, 65254, 65253,  297,  296,  295,  259,  258,   60, 
	65486,  298, 65319,   41,   -1
};
static short yypact[] = {

	   1,   15,   26,   15,   71,   73,   83,   83,   83,   71, 
	  93,   97,   99,  101,  101,   93,  114,   97,  123,   99, 
	 145,  162,  101,   97,  123,  228,  145,  251,  256,   97, 
	 265,  267,  228,  279,  281,  281,  281,  256,  291,  297, 
	 297,  228,  251,  299,  145,  145,  145,  329,  279,  383, 
	  83,  381,  374,  367,   83,  297,  365,  362,  359,  357, 
	 355,  353,  351,  349,  347,  345,  338,  331,   83,  327, 
	  83,  319,  311,  308,  304,  301,   83,   83,  101,   83, 
	 294,  289,  287,  285,  283,  277,  275,  273,  271,  269, 
	  83,  263,  261,  259,   83,  253,  244,   83,  237,  235, 
	 101,   83,   83,  221,  219,  217,  215,  213,   83,  211, 
	 209,  207,  205,  203,  201,  194,  182,  175,  173,  171, 
	 169,  167,  165,  129,  127,   83,  119,  117,   83,  111, 
	 109,  107,  105,  103,   95,   91,   89,   83,   77,   75, 
	  69,   67,   65,   63,   61,   59,   57,   55,   53,   51, 
	  49,   47,   45,   43,   41,   39,   37,   35,   33,   31, 
	  29,   27,    3
};
static short yygo[] = {

	65364, 65364, 65364, 65312, 65343, 65485, 65482, 65481, 65497, 65343, 
	65332, 65470, 65467, 65456, 65497, 65449, 65448, 65350, 65351, 65445, 
	65335, 65368, 65427, 65417, 65299, 65345,  137,  128,  125,  116, 
	 115,  108,  102,  101,   97,   96,   94,   90,   79,   77, 
	  76,   71,   70,   68,   66,   54,   52,   50,   41,   32, 
	  25, 65403, 65404, 65402,    8,    7, 65342, 65341,   52, 65333, 
	65348, 65321,   48,   33, 65519, 65519, 65507, 65498, 65505,   29, 
	  23,   17,   11, 65401, 65435, 65494, 65354, 65503,   41,   32, 
	65487, 65502,   42, 65468, 65446, 65413,  100,   78, 65318, 65317, 
	  55, 65338, 65337,   70, 65533, 65534, 65296, 65532, 65294, 65295, 
	   3, 65395, 65394, 65393, 65392, 65391, 65390, 65389, 65388, 65387, 
	65386, 65385, 65297, 65531, 65400, 65526, 65405, 65399,    9, 65301, 
	65499, 65490, 65489, 65491,   36,   35, 65471, 65472, 65473, 65437, 
	  46,   45,   44, 65303, 65500, 65305, 65501, 65521, 65416, 65415, 
	  14, 65522, 65515, 65523, 65412, 65516, 65428, 65411,   19, 65517, 
	65426, 65511, 65444, 65425,   24, 65315, 65316,   40, 65488, 65524, 
	65409, 65518, 65421, 65408,   17, 65512, 65323, 65454, 65506, 65463, 
	65453,   37, 65462, 65525, 65330, 65418, 65520, 65261, 65262,   15, 
	65451, 65461, 65474, 65460,   74, 65513, 65440, 65414,   22, 65455, 
	65478, 65509, 65447, 65436,   26,   -1
};
static short yypgo[] = {

	   0,    0,    0,   95,  156,   73,  170,   81,   61,   92, 
	  53,  193,  193,  193,   77,   85,   85,   85,   85,   85, 
	  85,   68,   68,   68,   77,   77,   74,   74,   74,   74, 
	  74,  193,  193,  193,  193,  193,  193,  193,  193,  193, 
	 193,  193,   25,   25,   53,   57,   57,  190,  190,   92, 
	  92,  189,  189,   61,   61,   81,  187,  183,  180,  175, 
	 173,  172,  172,  168,  168,   73,  167,  165,  163,  159, 
	  89,  158,  158,  153,  153,  153,  149,  147,  143,  142, 
	 141,  105,  137,  123,  136,  135,  134,  133,  120,  119, 
	 117,  113,   95,   94,   97,   97,   99,   99,   99,   99, 
	  99,   99,   99,   99,   99,   99,   99,  112,  115,  115, 
	 101,  102,  103,  104,  106,  107,  145,  145,  108,  151, 
	 151,  109,  161,  161,  110,  170,  111,  176,  176,  178, 
	 181,  181,  185,  185,   25,   25,   25,  191,  191,  129, 
	 129,  139,  139,  174,  174,  166,  166,  160,  160,  156, 
	 150,  150,  144,  144,  123,  114,  114,   96,   96,    0
};
static short yyrlen[] = {

	   0,    0,    0,    0,    0,    0,    0,    0,    0,    1, 
	   0,    1,    2,    3,    1,    1,    4,    7,    6,    3, 
	   4,    3,    3,    1,    4,    1,    1,    1,    1,    1, 
	   1,    2,    2,    2,    2,    4,    1,    1,    2,    2, 
	   1,    5,    1,    1,    1,    2,    1,    3,    1,    3, 
	   4,    3,    1,    4,    2,    2,    4,    2,    0,    0, 
	   0,    3,    1,    4,    3,    1,    0,    0,    2,    0, 
	   5,    3,    1,    4,    4,    2,    0,    8,    0,    0, 
	   0,    5,    0,    3,    0,    0,    0,    0,    0,    0, 
	   2,    0,    1,    2,    1,    2,    2,    2,    2,    2, 
	   2,    2,    2,    2,    2,    2,    2,    6,    2,    3, 
	   9,    9,    9,    5,    6,    5,    2,    3,    6,    2, 
	   3,    5,    2,    3,    8,    3,    6,    1,    2,    7, 
	   2,    3,    2,    3,    1,    1,    1,    2,    3,    0, 
	   1,    0,    1,    0,    1,    0,    1,    0,    1,    1, 
	   0,    1,    0,    1,    0,    0,    1,    0,    1,    2
};
#define YYS0	0
#define YYDELTA	146
#define YYNPACT	163
#define YYNDEF	49

#define YYr157	0
#define YYr158	1
#define YYr159	2
#define YYr18	3
#define YYr60	4
#define YYr75	5
#define YYr81	6
#define YYr102	7
#define YYr105	8
#define YYr108	9
#define YYr116	10
#define YYr132	11
#define YYr133	12
#define YYr134	13
#define YYr147	14
#define YYr151	15
#define YYr156	16
#define YYr155	17
#define YYr154	18
#define YYr153	19
#define YYr152	20
#define YYr150	21
#define YYr149	22
#define YYr148	23
#define YYr146	24
#define YYr145	25
#define YYr144	26
#define YYr143	27
#define YYr142	28
#define YYr141	29
#define YYr140	30
#define YYr139	31
#define YYr138	32
#define YYr137	33
#define YYr136	34
#define YYr135	35
#define YYr131	36
#define YYr130	37
#define YYr129	38
#define YYr128	39
#define YYr127	40
#define YYr126	41
#define YYr121	42
#define YYr120	43
#define YYr115	44
#define YYr114	45
#define YYr113	46
#define YYr112	47
#define YYr111	48
#define YYr110	49
#define YYr109	50
#define YYr107	51
#define YYr106	52
#define YYr104	53
#define YYr103	54
#define YYr101	55
#define YYr100	56
#define YYr95	57
#define YYr91	58
#define YYr85	59
#define YYr84	60
#define YYr83	61
#define YYr82	62
#define YYr79	63
#define YYr78	64
#define YYr74	65
#define YYr72	66
#define YYr71	67
#define YYr70	68
#define YYr64	69
#define YYr63	70
#define YYr62	71
#define YYr61	72
#define YYr58	73
#define YYr57	74
#define YYr56	75
#define YYr50	76
#define YYr49	77
#define YYr43	78
#define YYr41	79
#define YYr39	80
#define YYr38	81
#define YYr37	82
#define YYr35	83
#define YYr33	84
#define YYr32	85
#define YYr30	86
#define YYr29	87
#define YYr27	88
#define YYr26	89
#define YYr25	90
#define YYr19	91
#define YYr17	92
#define YYrACCEPT	YYr157
#define YYrERROR	YYr158
#define YYrLR2	YYr159
#if YYDEBUG
char * yysvar[] = {
	"$accept",
	"LITERAL",
	"opt_LITERAL",
	"ARRAY_INIT_VAL",
	"opt_InitVal",
	"instpath",
	"opt_identifier",
	"declScope",
	"expression",
	"opt_DIRECT_VARIABLE",
	"type",
	"memory_region",
	"ARRAY_INIT",
	"file",
	"opt_target_section",
	"opt_asmSections",
	"asmSections",
	"asmSection",
	"fun_section",
	"fb_section",
	"prg_section",
	"global_section",
	"global_retain_section",
	"struct_section",
	"resource_section",
	"task_section",
	"config_section",
	"copy_section",
	"channels_section",
	"target_section",
	"$19",
	"opt_attribute_list",
	"attribute_list",
	"attribute",
	"$26",
	"$27",
	"opt_clssid_assign",
	"opt_stmt_list",
	"$29",
	"$30",
	"$32",
	"$33",
	"$37",
	"opt_global_decls",
	"$39",
	"$41",
	"$43",
	"opt_task_properties",
	"task_properties",
	"task_property",
	"$50",
	"opt_task_proginst_readwrite_list",
	"task_proginst_readwrite_list",
	"task_proginst_readwrite",
	"memory_region_list_opt",
	"memory_region_list",
	"$64",
	"opt_config_decls",
	"config_decls",
	"config_decl",
	"$71",
	"opt_copy_item_list",
	"$72",
	"copy_item_list",
	"opt_memberList",
	"ident_list",
	"$84",
	"opt_channel_list",
	"$85",
	"channel_list",
	"channel",
	"$91",
	"channel_attr_list",
	"channel_attr",
	"global_decls",
	"global_decl",
	"ARRAY_INIT_LIST",
	"ARRAY_LIT_LIST",
	"stmt_list",
	"stmt",
	0
};
short yyrmap[] = {

	 157,  158,  159,   18,   60,   75,   81,  102,  105,  108, 
	 116,  132,  133,  134,  147,  151,  156,  155,  154,  153, 
	 152,  150,  149,  148,  146,  145,  144,  143,  142,  141, 
	 140,  139,  138,  137,  136,  135,  131,  130,  129,  128, 
	 127,  126,  121,  120,  115,  114,  113,  112,  111,  110, 
	 109,  107,  106,  104,  103,  101,  100,   95,   91,   85, 
	  84,   83,   82,   79,   78,   74,   72,   71,   70,   64, 
	  63,   62,   61,   58,   57,   56,   50,   49,   43,   41, 
	  39,   38,   37,   35,   33,   32,   30,   29,   27,   26, 
	  25,   19,   17,    1,    4,    5,    6,    7,    8,    9, 
	  10,   11,   12,   13,   14,   15,   16,   20,   23,   24, 
	  28,   31,   34,   40,   42,   44,   47,   48,   51,   54, 
	  55,   65,   68,   69,   73,   80,   86,   89,   90,   92, 
	  93,   94,   98,   99,  117,  118,  119,  124,  125,  123, 
	 122,   97,   96,   88,   87,   77,   76,   67,   66,   59, 
	  53,   52,   46,   45,   36,   22,   21,    3,    2,    0
};
short yysmap[] = {

	   0,    3,    4,   29,   31,   33,   40,   41,   42,   57, 
	  59,   62,   64,   66,   67,   83,   87,   89,   91,   94, 
	  96,   97,  100,  111,  122,  137,  145,  149,  159,  160, 
	 179,  180,  182,  193,  195,  196,  197,  201,  203,  206, 
	 207,  211,  212,  214,  219,  220,  221,  235,  241,  282, 
	 279,  277,  275,  265,  262,  261,  260,  258,  254,  251, 
	 250,  249,  246,  245,  244,  243,  242,  239,  238,  233, 
	 231,  230,  228,  226,  224,  223,  222,  218,  210,  209, 
	 205,  202,  200,  199,  198,  192,  191,  189,  187,  185, 
	 173,  171,  168,  167,  164,  151,  148,  147,  146,  144, 
	 143,  141,  140,  136,  135,  134,  133,  126,  124,  123, 
	 121,  120,  119,  118,  116,  113,  112,  110,  108,  102, 
	 101,   99,   98,   95,   93,   92,   90,   88,   81,   80, 
	  79,   76,   75,   74,   61,   58,   56,   55,   37,   35, 
	  27,   26,   25,   24,   23,   22,   21,   20,   19,   18, 
	  17,   16,   15,   14,   13,   12,   11,   10,    9,    8, 
	   7,    6,    1,  280,  266,  190,  215,  162,  163,   86, 
	 263,  181,  128,  129,  130,  131,  132,  175,  176,  177, 
	 178,  240,  138,  139,  183,  184,  142,  264,   68,   69, 
	  73,  274,  255,  281,  256,  257,  276,  259,  204,  232, 
	 165,  216,  217,  247,  157,   84,   32,  273,  225,  253, 
	 227,   60,  161,   85,  114,   34,  284,  278,  234,  236, 
	 237,  169,   63,  283,   36,   65,   38,  153,   39,  267, 
	 154,  103,  155,  104,  156,  105,   77,    5,    2,   30, 
	  28,   54,   53,   52,   51,   50,   49,   48,   47,   46, 
	  45,   44,   43,  107,   78,  106,  270,  269,  268,  152, 
	 188,  127,  125,  174,  172,  170,  208,  117,  115,  166, 
	 229,  252,  158,   82,  109,  272,  248,  271,  150,  194, 
	  72,   71,   70,  186,  213
};
int asm_parse::yyntoken = 54;
int asm_parse::yynvar = 80;
int asm_parse::yynstate = 285;
int asm_parse::yynrule = 160;
#endif



// C++ YACC parser code
// Copyright 1991 by Mortice Kern Systems Inc.  All rights reserved.
//
// If YYDEBUG is defined as 1 and asm_parse::yydebug is set to 1,
// yyparse() will print a travelogue of its actions as it reads
// and parses input.
//
// YYSYNC can be defined to cause yyparse() to attempt to always
// hold a lookahead token

const YY_MIN_STATE_NUM = 20;	// not useful to be too small!

#if YYDEBUG
#ifdef YYTRACE
long	* asm_parse::States	= yyStates;
#endif
yyTypedRules * asm_parse::Rules	= yyRules;
yyNamedType * asm_parse::TokenTypes = yyTokenTypes;

#define YY_TRACE(fn) { done = 0; fn(); if (done) YYRETURN(-1); }
#endif

// Constructor for asm_parse: user-provided tables
asm_parse::asm_parse(int *pError, int sz, short * states, YYSTYPE * stack)
{
	*pError = 0;
    pContext = 0;
	mustfree = 0;
	if ((size = sz) < YY_MIN_STATE_NUM
	 || (stateStack = states) == (short *) 0
	 || (valueStack = stack) == (YYSTYPE *) 0) {
		*pError = 2;
		return;
	}
	reset = 1;		// force reset
#if YYDEBUG
	yydebug = 0;
	typeStack = new short[size+1];
	if (typeStack == (short *) 0) {
		*pError = 1;
		return;
	}
#endif
}
// Constructor for asm_parse: allocate tables with new
asm_parse::asm_parse(int *pError, int sz)
{
	*pError = 0;
    pContext = 0;
	size = sz;
	reset = 1;		// force reset
	mustfree = 1;		// delete space in deconstructor
#if YYDEBUG
	yydebug = 0;
	typeStack = new short[size+1];
#endif
	stateStack = new short[size+1];
	valueStack = new YYSTYPE[size+1];

	if (stateStack == (short *) 0 || valueStack == (YYSTYPE *) 0
#if YYDEBUG
		|| typeStack == (short *) 0
#endif
	    ) {
        delete(stateStack);
        delete(valueStack);
        *pError =1;
		return;
	}
}
// Destructor for class asm_parse
//	Free up space
asm_parse::~asm_parse()
{
	if (mustfree) {
		delete stateStack;
		delete valueStack;
	}
	stateStack = (short *) 0;
#if YYDEBUG
	delete typeStack;
#endif
}

// The parser proper.
//	Note that this code is reentrant; you can return a value
//	and then resume parsing by recalling yyparse().
//	Call yyreset() before yyparse() if you want a fresh start
int
asm_parse::yyparse()
{
	short	* yyp, * yyq;		// table lookup
	int	yyj;
#if YYDEBUG
	int	yyruletype = 0;
#endif


	if (reset) {			// start new parse
		yynerrs = 0;
		yyerrflag = 0;
		yyps = stateStack;
		yypv = valueStack;
#if YYDEBUG
		yytp = typeStack;
#endif
		yystate = YYS0;
		yyclearin();
		reset = 0;
	} else			// continue saved parse
		goto yyNext;			// after action

yyStack:
	if (++yyps > &stateStack[size]) {
		yyerrorStackOverflow();
		YYABORT;
	}
	*yyps = yystate;	/* stack current state */
	*++yypv = yyval;	/* ... and value */
#if YYDEBUG
	if (yydebug) {
		*++yytp = yyruletype;	/* ... and type */
		YY_TRACE(yyShowState)
	}
#endif

	/*
	 * Look up next action in action table.
	 */
yyEncore:
#ifdef YYSYNC
	if (yychar < 0) {
		if ((yychar = yynexttoken(&yylval)) < 0) {
			if (yychar == -2) YYABORT;
			yychar = 0;
		}	/* endif */
#if YYDEBUG
		if (yydebug)
			yyShowRead();	// show new input token
#endif
	}
#endif
	if (yystate >= sizeof yypact/sizeof yypact[0]) 	/* simple state */
		yyi = yystate - YYDELTA;	/* reduce in any case */
	else {
		if(*(yyp = &yyact[yypact[yystate]]) >= 0) {
			/* Look for a shift on yychar */
#ifndef YYSYNC
			if (yychar < 0) {
				if ((yychar = yynexttoken(&yylval)) < 0) {
					if (yychar == -2) YYABORT;
					yychar = 0;
				}	/* endif */
				
#if YYDEBUG
				if (yydebug)
					yyShowRead();	// show new input token
#endif
			}
#endif
			yyq = yyp;
			yyi = yychar;
			while (yyi < *yyp++)
				;
			if (yyi == yyp[-1]) {
				yystate = yyneg(yyq[yyq-yyp]);
#if YYDEBUG
				if (yydebug) {
					yyruletype = yyGetType(yychar);
					YY_TRACE(yyShowShift)
				}
#endif
				yyval = yylval;		/* stack value */
				yyclearin();		/* clear token */
				if (yyerrflag)
					yyerrflag--;	/* successful shift */
				goto yyStack;
			}
		}

		/*
	 	 *	Fell through - take default action
	 	 */

		if (yystate >= sizeof yydef /sizeof yydef[0])
			goto yyError;
		if ((yyi = yydef[yystate]) < 0)	 { /* default == reduce? */

			/* Search exception table */
			yyp = &yyex[yyneg(yyi)];
#ifndef YYSYNC
			if (yychar < 0) {
				if ((yychar = yynexttoken(&yylval)) < 0) {
					if (yychar == -2) YYABORT;
					yychar = 0;
				}	/* endif */
				
#if YYDEBUG
				if (yydebug)
					yyShowRead();	// show new input token
#endif
			}
#endif
			while((yyi = *yyp) >= 0 && yyi != yychar)
				yyp += 2;
			yyi = yyp[1];
		}
	}

	yyj = yyrlen[yyi];

#if YYDEBUG
	if (yydebug) {
		npop = yyj; rule = yyi;
		YY_TRACE(yyShowReduce)
		yytp -= yyj;
	}
#endif
	yyps -= yyj;		/* pop stacks */
	yypvt = yypv;		/* save top */
	yypv -= yyj;
	yyval = yypv[1];	/* default action $ = $1 */
#if YYDEBUG
	if (yydebug)
		yyruletype = yyRules[yyrmap[yyi]].type;
#endif
	switch (yyi) {		/* perform semantic action */
		
case YYr17: {	/* opt_target_section :  target_section */

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
 
} break;

case YYr18: {	/* opt_target_section :  */

    ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_EXPECTED, _T("'.target' section"));
 
} break;

case YYr19: {	/* target_section :  TARGET EOL */
pContext->m_bEnableInstr = false;
} break;

case YYr25: {	/* attribute :  IDENTIFIER LITERAL */

    GetGC()->addTargetAttribute(pContext->getEdiPos(), yypvt[-1].id, yypvt[0].lit);
 
} break;

case YYr26: {	/* fun_section :  FUN IDENTIFIER opt_LITERAL EOL */
pContext->m_bEnableInstr = true;
} break;

case YYr27: {	/* fun_section :  FUN IDENTIFIER opt_LITERAL EOL $26 */
 pContext->addProc(pContext->getEdiPos(yypvt[-4].pos), yypvt[-3].id, ASM_FUN, yypvt[-2].lit);
} break;

case YYr29: {	/* fb_section :  FB IDENTIFIER opt_LITERAL EOL */
pContext->m_bEnableInstr = true;
} break;

case YYr30: {	/* fb_section :  FB IDENTIFIER opt_LITERAL EOL $29 */
 pContext->addProc(pContext->getEdiPos(yypvt[-4].pos), yypvt[-3].id, ASM_FB, yypvt[-2].lit);
} break;

case YYr32: {	/* prg_section :  PRG IDENTIFIER opt_LITERAL EOL */
pContext->m_bEnableInstr = true;
} break;

case YYr33: {	/* prg_section :  PRG IDENTIFIER opt_LITERAL EOL $32 */
 pContext->addProc(pContext->getEdiPos(yypvt[-4].pos), yypvt[-3].id, ASM_PRG, yypvt[-2].lit);
} break;

case YYr35: {	/* opt_clssid_assign :  CLSSID LITERAL EOL */
pContext->assignClssid(pContext->getEdiPos(yypvt[-2].pos), yypvt[-1].lit);
} break;

case YYr37: {	/* global_retain_section :  GLOBAL_RETAIN EOL */
pContext->m_bEnableInstr = false; pContext->m_bIsRetain = true;
} break;

case YYr38: {	/* global_retain_section :  GLOBAL_RETAIN EOL $37 opt_global_decls END_SECTION */
pContext->m_bIsRetain = false;
} break;

case YYr39: {	/* global_section :  GLOBAL EOL */
pContext->m_bEnableInstr = false; pContext->m_bIsRetain = false;
} break;

case YYr41: {	/* struct_section :  STRUCT IDENTIFIER EOL */

        pContext->m_bEnableInstr = false;
        ASM_LITERAL_TYP mone;
        mone.type = ASM_DINT;
        mone.any64= -1;
        pContext->addProc(pContext->getEdiPos(yypvt[-2].pos), yypvt[-1].id, ASM_STRUCT, mone);
    
} break;

case YYr43: {	/* resource_section :  RESOURCE EOL */
pContext->m_bEnableInstr = false;
} break;

case YYr49: {	/* task_property :  IDENTIFIER LITERAL LITERAL LITERAL LITERAL LITERAL LITERAL LITERAL */

    GetGC()->addTaskProperties(pContext->getEdiPos(), yypvt[-7].id, yypvt[-6].lit, yypvt[-5].lit, yypvt[-4].lit, yypvt[-3].lit, yypvt[-2].lit, yypvt[-1].lit, yypvt[0].lit);
 
} break;

case YYr50: {	/* task_section :  TASK IDENTIFIER EOL */

       pContext->m_bEnableInstr = true; //<-TODO: is this correct ? should be false ??
       GetGC()->setActualTask(pContext->getEdiPos(yypvt[-2].pos), &(yypvt[-1].id));
       pContext->setActualTask(pContext->getEdiPos(yypvt[-2].pos), &(yypvt[-1].id));
   
} break;

case YYr56: {	/* task_proginst_readwrite :  CALL IDENTIFIER */

    GetGC()->addTaskPrginstAssignment(pContext->getEdiPos(), yypvt[0].id);
 
} break;

case YYr57: {	/* task_proginst_readwrite :  READ IDENTIFIER ':' memory_region_list_opt */

    pContext->appendRegionList(pContext->getEdiPos(yypvt[-3].pos), yypvt[-2].id, ASM_READ_LIST);
 
} break;

case YYr58: {	/* task_proginst_readwrite :  WRITE IDENTIFIER ':' memory_region_list_opt */

    pContext->appendRegionList(pContext->getEdiPos(yypvt[-3].pos), yypvt[-2].id, ASM_WRITE_LIST);
 
} break;

case YYr60: {	/* memory_region_list_opt :  */

	pContext->m_tmpRegionList.setUsed(0);
 
} break;

case YYr61: {	/* memory_region_list :  memory_region */

	pContext->m_tmpRegionList.setUsed(0);
	pContext->m_tmpRegionList.add(yypvt[0].region);
 
} break;

case YYr62: {	/* memory_region_list :  memory_region_list ',' memory_region */

	pContext->m_tmpRegionList.add(yypvt[0].region);
 
} break;

case YYr63: {	/* memory_region :  '(' ANY_UNSIGNED ',' ANY_UNSIGNED ')' */

    static char buf[40];
	yyval.region.ulOffset = yypvt[-3].auLong;
    if (yypvt[-1].auLong > 65535)
    {
        ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_INT_LITERAL, _ultot(yypvt[-1].auLong, buf, 16));
    }
    yyval.region.uSize = yypvt[-1].auLong;
 
} break;

case YYr64: {	/* config_section :  CONFIG EOL */
pContext->m_bEnableInstr = false;
} break;

case YYr70: {	/* config_decl :  instpath opt_InitVal */
 
    if (yypvt[0].init.initType != ASM_INIT_UNDEF)
        GetGC()->appendConfigData(pContext->getEdiPos(), &yypvt[-1].path, yypvt[0].init); 
 
} break;

case YYr71: {	/* copy_section :  COPY IDENTIFIER opt_identifier EOL */

      pContext->m_bEnableInstr = false;
      pContext->m_tmpCopyItemList.setUsed(0); 
    
} break;

case YYr72: {	/* copy_section :  COPY IDENTIFIER opt_identifier EOL $71 opt_copy_item_list */

        CASMCopySection* pCPS = new CASMCopySection(
           yypvt[-4].id, 
           yypvt[-3].id, 
           pContext->m_tmpCopyItemList.getUsed(),
           pContext->m_tmpCopyItemList.getMemCopy()
        );
        pContext->m_tmpCopyItemList.setUsed(0);
        GetGC()->addCopySection(pCPS);
    
} break;

case YYr74: {	/* opt_identifier :  IDENTIFIER */
 yyval.id = yypvt[0].id;
} break;

case YYr75: {	/* opt_identifier :  */
 yyval.id.psz = NULL;
} break;

case YYr78: {	/* copy_item_list :  instpath opt_memberList EOL */

    pContext->addCopyItem(yypvt[-2].path, &pContext->m_tmpIdList);
 
} break;

case YYr79: {	/* copy_item_list :  copy_item_list instpath opt_memberList EOL */

    pContext->addCopyItem(yypvt[-2].path, &pContext->m_tmpIdList);
 
} break;

case YYr81: {	/* opt_memberList :  */
  pContext->m_tmpIdList.setUsed(0); 
} break;

case YYr82: {	/* ident_list :  IDENTIFIER */

    pContext->m_tmpIdList.setUsed(0); 
    pContext->m_tmpIdList.add(yypvt[0].id);
 
} break;

case YYr83: {	/* ident_list :  ident_list ',' IDENTIFIER */

    pContext->m_tmpIdList.add(yypvt[0].id);
 
} break;

case YYr84: {	/* channels_section :  CHANNELS EOL */

      pContext->m_bEnableInstr = false;
   
} break;

case YYr85: {	/* channels_section :  CHANNELS EOL $84 opt_channel_list */

   
} break;

case YYr91: {	/* channel :  CHANNEL LITERAL EOL */

        GetGC()->addChannel(ASM_ForceDINT(pContext->getEdiPos(), &yypvt[-1].lit));
   
} break;

case YYr95: {	/* channel_attr :  IDENTIFIER LITERAL */

    GetGC()->addChannelAttribute(pContext->getEdiPos(), yypvt[-1].id.psz, &yypvt[0].lit);
 
} break;

case YYr100: {	/* global_decl :  type IDENTIFIER opt_DIRECT_VARIABLE opt_InitVal */

    ASM_SEGM_TYP segm;
    if(yypvt[-3].datatype->type==ASM_PRGI)
        segm = pContext->m_bIsRetain ? ASM_SEGM_RETAIN : ASM_SEGM_INST;
    else
        segm = pContext->m_bIsRetain ? ASM_SEGM_RETAIN : ASM_SEGM_GLOBAL;

    if (!GetGC()->addGlobalDecl(pContext->getEdiPos(), yypvt[-3].datatype, yypvt[-2].id, segm, yypvt[-1].direct, yypvt[0].init))
        YYABORT;
 
} break;

case YYr101: {	/* opt_DIRECT_VARIABLE :  IDENTIFIER DIRECT_VARIABLE */

    if(_tcscmp(_T("at"), yypvt[-1].id.psz))
        ST_DiagMsg2(pContext->getEdiPos(), ST_MISSING_X_BEFOR_Y, _T("at"),  _T("%"));
    
    yyval.direct = yypvt[0].direct;
 
} break;

case YYr102: {	/* opt_DIRECT_VARIABLE :  */

    ASM_InitDirectAddr(&yyval.direct);
 
} break;

case YYr103: {	/* opt_InitVal :  BECOMES LITERAL */

    yyval.init.initType = ASM_INIT_LIT;
    yyval.init.lit      = yypvt[0].lit;
 
} break;

case YYr104: {	/* opt_InitVal :  BECOMES '[' ARRAY_INIT_LIST ']' */

    yyval.init.initType  = ASM_INIT_LIST;
    yyval.init.pInitList = pContext->getInitList();
 
} break;

case YYr105: {	/* opt_InitVal :  */

    yyval.init.initType = ASM_INIT_UNDEF;
 
} break;

case YYr106: {	/* ARRAY_INIT_LIST :  ARRAY_INIT */
 
    pContext->m_InitList.setUsed(0);
    pContext->m_InitList.add(yypvt[0].repeatVal);
 
} break;

case YYr107: {	/* ARRAY_INIT_LIST :  ARRAY_INIT_LIST ',' ARRAY_INIT */
 
    pContext->m_InitList.add(yypvt[0].repeatVal);
 
} break;

case YYr108: {	/* ARRAY_INIT :  LITERAL */

    yyval.repeatVal.nRepeat   = 1;
    yyval.repeatVal.nListSize = 1;
    yyval.repeatVal.pLitList  = pContext->makeLiteralList(yypvt[0].lit);
 
} break;

case YYr109: {	/* ARRAY_INIT :  LITERAL '(' ARRAY_LIT_LIST ')' */

    yyval.repeatVal.nRepeat   = (int) yypvt[-3].lit.any64;
    yyval.repeatVal.nListSize = pContext->m_LitList.getUsed();
    yyval.repeatVal.pLitList  = pContext->getLiteralList();
 
} break;

case YYr110: {	/* ARRAY_INIT :  LITERAL '(' ')' */

    yyval.repeatVal.nRepeat   = (int) yypvt[-2].lit.any64;
    yyval.repeatVal.nListSize = 0;
    yyval.repeatVal.pLitList  = NULL;
 
} break;

case YYr111: {	/* ARRAY_LIT_LIST :  ARRAY_INIT_VAL */

    pContext->m_LitList.setUsed(0);
    pContext->m_LitList.add(yypvt[0].lit);
 
} break;

case YYr112: {	/* ARRAY_LIT_LIST :  ARRAY_LIT_LIST ',' ARRAY_INIT_VAL */

    pContext->m_LitList.add(yypvt[0].lit);
 
} break;

case YYr113: {	/* ARRAY_INIT_VAL :  LITERAL */

    yyval.lit = yypvt[0].lit;
 
} break;

case YYr114: {	/* ARRAY_INIT_VAL :  '<' '>' */

    
    
    yyval.lit.type = ASM_VOID;
 
} break;

case YYr115: {	/* opt_LITERAL :  LITERAL */
yyval.lit = yypvt[0].lit;
} break;

case YYr116: {	/* opt_LITERAL :  */
yyval.lit.type = ASM_TYPE_UNDEF;
} break;

case YYr120: {	/* LITERAL :  WSTRING_BEG */

    ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_TYPE_NOT_IMPL, _T("WSTRING"));
    yyval.lit.type = ASM_TYPE_UNDEF;
 
} break;

case YYr121: {	/* LITERAL :  STRING_BEG */

    if(!ASM_SkipIECStringLiteral(pContext->m_pScanner, _T('\''), true))
    {
       ST_DiagMsg0(pContext->getEdiPos(), E_4CCG_ERR_UNTERM_STRING);
       YYABORT;
    }
    else
    {
        yyval.lit.pszIECStr = GetGC()->registerStringLit(ASM_GetSkippedTextBlock(pContext->m_pScanner));
        yyval.lit.type = ASM_STRING;
    }
 
} break;

case YYr126: {	/* stmt :  declScope type IDENTIFIER opt_DIRECT_VARIABLE opt_InitVal */
 
    pContext->m_curProc->addDecl(pContext->getEdiPos(), yypvt[-2].id, yypvt[-4].dscope, yypvt[-3].datatype, yypvt[-1].direct, yypvt[0].init);
 
} break;

case YYr127: {	/* stmt :  DECL_ALIGN8 */
 
    pContext->m_curProc->m_alignNextDecl = 8;
 
} break;

case YYr128: {	/* stmt :  BP LITERAL */

    pContext->makeBpPos(yypvt[-1].pos, &yypvt[0].lit, true);
 
} break;

case YYr129: {	/* stmt :  STMTBEG LITERAL */

    pContext->makeBpPos(yypvt[-1].pos, &yypvt[0].lit, false);
 
} break;

case YYr130: {	/* stmt :  ST_INFO */

    ASM_PRIMARY_TYP     arg;
    arg.argkind = ASM_PRM_LABEL;
    arg.label   = yypvt[0].id;
    ASM_PRIMARY_TYP*     args[]={&arg};
    new ASM_INSTR_TYP(pContext->getEdiPos(), ASM_ST_INFO, pContext->m_curProc, NULL, 1, args);
 
} break;

case YYr131: {	/* stmt :  CALOP */

    pContext->makeCalop(pContext->m_currPos, yypvt[0].id);
 
} break;

case YYr132: {	/* stmt :  VMOPC */

    new ASM_INSTR_TYP(pContext->getEdiPos(), ASM_VMOPC, pContext->m_curProc, yypvt[0].pOPC, 0, NULL);
 
} break;

case YYr133: {	/* stmt :  VMOPC expression */

    ASM_PRIMARY_TYP* args[] = {&yypvt[0].argtyp};
    new ASM_INSTR_TYP(pContext->getEdiPos(), ASM_VMOPC, pContext->m_curProc, yypvt[-1].pOPC, 1, args);
 
} break;

case YYr134: {	/* stmt :  VMOPC expression expression */

    ASM_PRIMARY_TYP* pArg[] = {&yypvt[-1].argtyp, &yypvt[0].argtyp};
    new ASM_INSTR_TYP(pContext->getEdiPos(), ASM_VMOPC, pContext->m_curProc, yypvt[-2].pOPC, 2, pArg);
 
} break;

case YYr135: {	/* stmt :  VMOPC expression expression expression */

    ASM_PRIMARY_TYP* pArg[] = {&yypvt[-2].argtyp, &yypvt[-1].argtyp, &yypvt[0].argtyp};
    new ASM_INSTR_TYP(pContext->getEdiPos(), ASM_VMOPC, pContext->m_curProc, yypvt[-3].pOPC, 3, pArg);
 
} break;

case YYr136: {	/* stmt :  JMP IDENTIFIER */

    pContext->makeJmp(yypvt[-1].pos, ASM_JMP, yypvt[0].id);
 
} break;

case YYr137: {	/* stmt :  JMPC IDENTIFIER */
 
    pContext->makeJmp(yypvt[-1].pos, ASM_JMPC, yypvt[0].id);
 
} break;

case YYr138: {	/* stmt :  JMPCN IDENTIFIER */
 
    pContext->makeJmp(yypvt[-1].pos, ASM_JMPCN, yypvt[0].id);
 
} break;

case YYr139: {	/* stmt :  ':' IDENTIFIER */
pContext->makeLabel(yypvt[0].id);
} break;

case YYr140: {	/* declScope :  DECL_VAR */
yyval.dscope = ASM_DECL_VAR;
} break;

case YYr141: {	/* declScope :  DECL_IN */
yyval.dscope = ASM_DECL_IN;
} break;

case YYr142: {	/* declScope :  DECL_INEXT */
yyval.dscope = ASM_DECL_INEXT;
} break;

case YYr143: {	/* declScope :  DECL_OUT */
yyval.dscope = ASM_DECL_OUT;
} break;

case YYr144: {	/* declScope :  DECL_INOUT */
yyval.dscope = ASM_DECL_INOUT;
} break;

case YYr145: {	/* expression :  LITERAL */
 
    yyval.argtyp.argkind = ASM_PRM_LIT; 
    yyval.argtyp.lit = yypvt[0].lit;
 
} break;

case YYr146: {	/* expression :  IDENTIFIER '(' type ')' */

    if(!_tcscmp(yypvt[-3].id.psz , _T("aidx")))
    {
        if(yypvt[-1].datatype->type!=ASM_INT && yypvt[-1].datatype->type!=ASM_DINT)
        {
            ST_DiagMsg1(pContext->getEdiPos(), E_ASM_BAD_AIDX, yypvt[-1].datatype->name.psz);
            yyval.argtyp.argkind = ASM_PRM_NONE; 
        }
        else
        {
            yyval.argtyp.argkind = ASM_PRM_AIDX; 
            yyval.argtyp.pType = yypvt[-1].datatype;
        }
    }
    else if(!_tcscmp(yypvt[-3].id.psz , _T("sizeof8")))
    {
        yyval.argtyp.argkind = ASM_PRM_SIZEOF8; 
        yyval.argtyp.pType = yypvt[-1].datatype;
    }
    else if(!_tcscmp(yypvt[-3].id.psz , _T("sizeof1")))
    {
        yyval.argtyp.argkind = ASM_PRM_SIZEOF1; 
        yyval.argtyp.pType = yypvt[-1].datatype;
    }
    else if(!_tcscmp(yypvt[-3].id.psz , _T("clsid")))
    {
        ASM_PROC_ENUM pout = yypvt[-1].datatype->pFBIDef ? yypvt[-1].datatype->pFBIDef->getPouTyp() : ASM_PROC_TYP_UNDEF;
        if(!(pout==ASM_FUN || pout==ASM_FB))
        {
            ST_DiagMsg1(pContext->getEdiPos(), E_ASM_BAD_CLSID, yypvt[-1].datatype->name.psz);
            yyval.argtyp.argkind = ASM_PRM_NONE; 
        }
        else
        {
            yyval.argtyp.argkind = ASM_PRM_CLSID; 
            yyval.argtyp.pType = yypvt[-1].datatype;
        }
    }
    else if(!_tcscmp(yypvt[-3].id.psz , _T("fbid")))
    {
        ASM_PROC_ENUM pout = yypvt[-1].datatype->pFBIDef ? yypvt[-1].datatype->pFBIDef->getPouTyp() : ASM_PROC_TYP_UNDEF;
        if(pout!=ASM_FB || !yypvt[-1].datatype->pFBIDef->isBuildIn())
        {
            ST_DiagMsg1(pContext->getEdiPos(), E_ASM_BAD_FBID, yypvt[-1].datatype->name.psz);
            yyval.argtyp.argkind = ASM_PRM_NONE; 
        }
        else
        {
            yyval.argtyp.argkind = ASM_PRM_FBID; 
            yyval.argtyp.lId   = yypvt[-1].datatype->pFBIDef->getOpCode();
        }
    }
    else if(!_tcscmp(yypvt[-3].id.psz , _T("funlocbytes")))
    {
        ASM_PROC_ENUM pout = yypvt[-1].datatype->pFBIDef ? yypvt[-1].datatype->pFBIDef->getPouTyp() : ASM_PROC_TYP_UNDEF;
        if(pout!=ASM_FUN)
        {
            ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_UNKNOWN_FUNCT, yypvt[-1].datatype->name.psz);
            yyval.argtyp.argkind = ASM_PRM_NONE; 
        }
        else
        {
            yyval.argtyp.argkind = ASM_PRM_FUNLOCBYTES; 
            yyval.argtyp.pType = yypvt[-1].datatype;
        }
    }
    else if(!_tcscmp(yypvt[-3].id.psz , _T("leavebytes")))
    {
        ASM_PROC_ENUM pout = yypvt[-1].datatype->pFBIDef ? yypvt[-1].datatype->pFBIDef->getPouTyp() : ASM_PROC_TYP_UNDEF;
        if(pout!=ASM_FUN)
        {
            ST_DiagMsg1(pContext->getEdiPos(), E_4CCG_ERR_UNKNOWN_FUNCT, yypvt[-1].datatype->name.psz);
            yyval.argtyp.argkind = ASM_PRM_NONE; 
        }
        else
        {
            yyval.argtyp.argkind = ASM_PRM_LEAVEBYTES; 
            yyval.argtyp.pType = yypvt[-1].datatype;
        }
    }
    else
    {
        pContext->tokenSyntax(yypvt[-3].id.psz);
        yyval.argtyp.argkind = ASM_PRM_NONE; 
    }
 
} break;

case YYr147: {	/* expression :  instpath */
 
    pContext->checkVarReference(yypvt[0].path, &yyval.argtyp);
 
} break;

case YYr148: {	/* instpath :  IDENTIFIER */
 yyval.path = GetGC()->registerInstPathPlusMember(NULL, yypvt[0].id);
} break;

case YYr149: {	/* instpath :  instpath '.' IDENTIFIER */
 yyval.path = GetGC()->registerInstPathPlusMember(&yypvt[-2].path, yypvt[0].id);
} break;

case YYr150: {	/* instpath :  instpath '.' LITERAL */
 yyval.path = GetGC()->registerInstArrayPath(yypvt[-2].path, &yypvt[0].lit);
} break;

case YYr151: {	/* type :  IDENTIFIER */
 
    yyval.datatype = pContext->makeType(yypvt[0].id, false, NULL, NULL); 
 
} break;

case YYr152: {	/* type :  IDENTIFIER '[' LITERAL ']' */
 
    yyval.datatype = pContext->makeType(yypvt[-3].id, true, &yypvt[-1].lit, NULL); 
 
} break;

case YYr153: {	/* type :  IDENTIFIER '[' ']' */
 
    yyval.datatype = pContext->makeType(yypvt[-2].id, true, NULL, NULL);
 
} break;

case YYr154: {	/* type :  IDENTIFIER '(' LITERAL ')' '[' ']' */
 
    yyval.datatype = pContext->makeType(yypvt[-5].id, true, NULL, &yypvt[-3].lit); 
 
} break;

case YYr155: {	/* type :  IDENTIFIER '(' LITERAL ')' '[' LITERAL ']' */
  
    yyval.datatype = pContext->makeType(yypvt[-6].id, true, &yypvt[-1].lit, &yypvt[-4].lit); 
 
} break;

case YYr156: {	/* type :  IDENTIFIER '(' LITERAL ')' */
 
    yyval.datatype = pContext->makeType(yypvt[-3].id, false, NULL, &yypvt[-1].lit); 
 
} break;
	case YYrACCEPT:
		YYACCEPT;
	case YYrERROR:
		goto yyError;
	}
yyNext:
	/*
	 *	Look up next state in goto table.
	 */

	yyp = &yygo[yypgo[yyi]];
	yyq = yyp++;
	yyi = *yyps;
	while (yyi < *yyp++)		/* busy little loop */
		;
	yystate = yyneg(yyi == *--yyp? yyq[yyq-yyp]: *yyq);
#if YYDEBUG
	if (yydebug)
		YY_TRACE(yyShowGoto)
#endif
	goto yyStack;

yyerrlabel:	;		/* come here from YYERROR	*/
	yyerrflag = 1;
	if (yyi == YYrERROR) {
		yyps--, yypv--;
#if YYDEBUG
		if (yydebug) yytp--;
#endif
	}
	
yyError:
	switch (yyerrflag) {

	case 0:		/* new error */
		yynerrs++;
		yyi = yychar;
		yyerrorSyntax();
		if (yyi != yychar) {
			/* user has changed the current token */
			/* try again */
			yyerrflag++;	/* avoid loops */
			goto yyEncore;
		}

	case 1:		/* partially recovered */
	case 2:
		yyerrflag = 3;	/* need 3 valid shifts to recover */
			
		/*
		 *	Pop states, looking for a
		 *	shift on `error'.
		 */

		for ( ; yyps > stateStack; yyps--, yypv--
#if YYDEBUG
					, yytp--
#endif
		) {
			if (*yyps >= sizeof yypact/sizeof yypact[0])
				continue;
			yyp = &yyact[yypact[*yyps]];
			yyq = yyp;
			do
				;
			while (YYERRCODE < *yyp++);
			if (YYERRCODE == yyp[-1]) {
				yystate = yyneg(yyq[yyq-yyp]);
				goto yyStack;
			}
				
			/* no shift in this state */
#if YYDEBUG
			if (yydebug && yyps > stateStack+1)
				YY_TRACE(yyShowErrRecovery)
#endif
			/* pop stacks; try again */
		}
		/* no shift on error - abort */
		break;

	case 3:
		/*
		 *	Erroneous token after
		 *	an error - discard it.
		 */

		if (yychar == 0)  /* but not EOF */
			break;
#if YYDEBUG
		if (yydebug)
			YY_TRACE(yyShowErrDiscard)
#endif
		yyclearin();
		goto yyEncore;	/* try again in same state */
	}
	YYABORT;

}
#if YYDEBUG
/*
 * Return type of token
 */
int
asm_parse::yyGetType(int tok)
{
	yyNamedType * tp;
	for (tp = &yyTokenTypes[yyntoken-1]; tp > yyTokenTypes; tp--)
		if (tp->token == tok)
			return tp->type;
	return 0;
}

	
// Print a token legibly.
char *
asm_parse::yyptok(int tok)
{
	yyNamedType * tp;
	for (tp = &yyTokenTypes[yyntoken-1]; tp > yyTokenTypes; tp--)
		if (tp->token == tok)
			return tp->name;
	return "";
}
/*
 * Read state 'num' from YYStatesFile
 */
#ifdef YYTRACE

char *
asm_parse::yygetState(int num)
{
	int	size;
	char	*cp;
	static FILE *yyStatesFile = (FILE *) 0;
	static char yyReadBuf[YYMAX_READ+1];

	if (yyStatesFile == (FILE *) 0
	 && (yyStatesFile = fopen(YYStatesFile, "r")) == (FILE *) 0)
		return "yyExpandName: cannot open states file";

	if (num < yynstate - 1)
		size = (int)(States[num+1] - States[num]);
	else {
		/* length of last item is length of file - ptr(last-1) */
		if (fseek(yyStatesFile, 0L, 2) < 0)
			goto cannot_seek;
		size = (int) (ftell(yyStatesFile) - States[num]);
	}
	if (size < 0 || size > YYMAX_READ)
		return "yyExpandName: bad read size";
	if (fseek(yyStatesFile, States[num], 0) < 0) {
	cannot_seek:
		return "yyExpandName: cannot seek in states file";
	}

	(void) fread(yyReadBuf, 1, size, yyStatesFile);
	yyReadBuf[size] = '\0';
	return yyReadBuf;
}
#endif /* YYTRACE */
/*
 * Expand encoded string into printable representation
 * Used to decode yyStates and yyRules strings.
 * If the expansion of 's' fits in 'buf', return 1; otherwise, 0.
 */
int
asm_parse::yyExpandName(int num, int isrule, char * buf, int len)
{
	int	i, n, cnt, type;
	char	* endp, * cp, * s;

	if (isrule)
		s = yyRules[num].name;
	else
#ifdef YYTRACE
		s = yygetState(num);
#else
		s = "*no states*";
#endif

	for (endp = buf + len - 8; *s; s++) {
		if (buf >= endp) {		/* too large: return 0 */
		full:	(void) strcpy(buf, " ...\n");
			return 0;
		} else if (*s == '%') {		/* nonterminal */
			type = 0;
			cnt = yynvar;
			goto getN;
		} else if (*s == '&') {		/* terminal */
			type = 1;
			cnt = yyntoken;
		getN:
			if (cnt < 100)
				i = 2;
			else if (cnt < 1000)
				i = 3;
			else
				i = 4;
			for (n = 0; i-- > 0; )
				n = (n * 10) + *++s - '0';
			if (type == 0) {
				if (n >= yynvar)
					goto too_big;
				cp = yysvar[n];
			} else if (n >= yyntoken) {
			    too_big:
				cp = "<range err>";
			} else
				cp = yyTokenTypes[n].name;

			if ((i = strlen(cp)) + buf > endp)
				goto full;
			(void) strcpy(buf, cp);
			buf += i;
		} else
			*buf++ = *s;
	}
	*buf = '\0';
	return 1;
}
#ifndef YYTRACE
/*
 * Show current state of yyparse
 */
void
asm_parse::yyShowState()
{
	(void) printf("state %d (%d), char %s (%d)\n%d stateStack entries\n",
		yysmap[yystate],yystate,yyptok(yychar),yychar,
		yypv - valueStack);
}
// show results of reduction: yyi is rule number
void
asm_parse::yyShowReduce()
{
	(void) printf("Reduce by rule %d (pop#=%d)\n", yyrmap[rule], npop);
}
// show read token
void
asm_parse::yyShowRead()
{
	(void) printf("read %s (%d)\n", yyptok(yychar), yychar);
}
// show Goto
void
asm_parse::yyShowGoto()
{
	(void) printf("goto %d (%d)\n", yysmap[yystate], yystate);
}
// show Shift
void
asm_parse::yyShowShift()
{
	(void) printf("shift %d (%d)\n", yysmap[yystate], yystate);
}
// show error recovery
void
asm_parse::yyShowErrRecovery()
{
	(void) printf("Error recovery pops state %d (%d), uncovers %d (%d)\n",
		yysmap[*(yyps-1)], *(yyps-1), yysmap[yystate], yystate);
}
// show token discards in error processing
void
asm_parse::yyShowErrDiscard()
{
	(void) printf("Error recovery discards %s (%d), ",
		yyptok(yychar), yychar);
}
#endif	/* ! YYTRACE */
#endif	/* YYDEBUG */
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

/*END yyreplace.pl: file asm.cpp~ */
