/*!!!!! this file has been generated using yyreplace.pl !!!!!*/
/* input files for yyreplace.pl: st.y, st.cpp~, st.h~*/

/*START yyreplace.pl: include files and #yy_movetop lines from st.y */
#include "stdafx.h"
#include "st_scan.h"
#include "watchexpr.h"
#include "watchexprman.h"
#include <errno.h>
/*END yyreplace.pl: include files from st.y */


/*START yyreplace.pl: file st.h~ */
typedef union {
    struct
    {
        CString *text;
        LONG    offset;
    } ident;
    CExpressionNode                             *expr;
    BL_IEC_TYP                                  type;
    long                                        number;
    TCHAR                                       letter;
    struct
    {
        BL_IEC_TYP source;
        BL_IEC_TYP target;
    }                                           conversion;
    CList<CExpressionNode *, CExpressionNode *> *exprlist;
} YYSTYPE;
const IDENTIFIER = 257;
const MACHINEADDRESS = 258;
const DIRECT_VARIABLE = 259;
const LITERAL = 260;
const NUMBER = 261;
const LETTER = 262;
const DOTDOT = 263;
const CONVERSION = 264;
const POW = 265;
const NOT = 266;
const OR = 267;
const XOR = 268;
const AND = 269;
const ADD = 270;
const SUB = 271;
const MUL = 272;
const DIV = 273;
const MOD = 274;
const EQ = 275;
const NE = 276;
const LT = 277;
const LE = 278;
const GT = 279;
const GE = 280;
const EQ_SHORT = 281;
const NE_SHORT = 282;
const LT_SHORT = 283;
const LE_SHORT = 284;
const GT_SHORT = 285;
const GE_SHORT = 286;


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
// st_parse => class defining a parsing object
//	st_parse needs a class yy_, which defines the ner.
// %prefix or option -p xx determines name of this class; if not used,
// defaults to 'yy_'
//
// constructor fills in the tables for this grammar; give it a size
//    to determine size of state and value stacks. Default is 150 entries.
// destructor discards those state and value stacks
//
// int st_parse::yyparse(yy_ *) invokes parse; if this returns,
//	it can be recalled to continue parsing at last point.
// void st_parse::yyreset() can be called to reset the parse;
//	call yyreset() before st_parse::yyparse(yy_ *)
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


class st_ParseContext;
class st_parse {
	st_ParseContext* pContext;
public:
	st_ParseContext* getParseContext(){return pContext;}
	void          setParseContext(st_ParseContext* pc){pContext=pc;}

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
	st_parse(int* pError, int = 150); 

    // another constructor
    //pError=0 == no error
    //pError=1 == out of memory
    //pError=2 == bad state given
    st_parse(int* pError, int, short *, YYSTYPE *);	

	~st_parse();		// destructor

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
/*END yyreplace.pl: file st.h~ */
/*START yyreplace.pl: file st.cpp~ */
/* yacc -lmv -LC -P C:\ATCMControl_SDK\GenWork\GenDir.tmp\4Control_R\gentools\yyparse.cpp -o st.cpp~ -D st.h~ st.y */


#include "stdafx.h"
#include "st_scan.h"
#include "watchexpr.h"
#include "watchexprman.h"
#include <errno.h>

//global parse context

struct PrefixedIdentifier
{
    PrefixedIdentifier () { }

    PrefixedIdentifier (LONG lOffset, const CString &strPrefix)
    : m_lOffset (lOffset),
      m_strPrefix (strPrefix)
    {
    }

    LONG    m_lOffset;
    CString m_strPrefix;
};


class st_ParseContext
{
public:    
    sts_scanner                                                        *m_pScanner;
    bool                                                               m_bRangeError;
    CExpressionHolder                                                  *m_pHolder;
    CExpressionNode                                                    *m_pRoot;
    CEExpandMode                                                       m_ExpandMode;
    CString                                                            m_strScope;
    CList<PrefixedIdentifier, const PrefixedIdentifier &>              m_PrefixedIdentifiers;
    CArray<CExpressionHolder::Range, const CExpressionHolder::Range &> *m_pRanges;
};

static HRESULT ParseExpression (CString &strText, CEExpandMode expandMode, const CString &strScope, CExpressionHolder *pHolder, CEWatchItemState &state);
static void PrependPrefixes (CString &strText, CList<PrefixedIdentifier, const PrefixedIdentifier &> &prefixedIdentifiers);
HRESULT ParseFormat (const CString &strText, CExpressionHolder *pHolder, CEWatchItemState &state);
static HRESULT ParseRanges (const CString &strText, CExpressionHolder *pHolder, CEWatchItemState &state);
static HRESULT ParseItemPart (const CString &strText, CExpressionHolder *pHolder, STS_ScannerMode mode, st_ParseContext *pCtx);
static CExpressionNode *BuildIntegerLiteral (st_ParseContext *pContext, LPCTSTR pszText);
static CExpressionNode *BuildFloatLiteral (st_ParseContext *pContext, LPCTSTR pszText);
static CExpressionNode *BuildTimeLiteral (st_ParseContext *pContext, LPCTSTR pszText);
static CExpressionNode *BuildDateLiteral (st_ParseContext *pContext, LPCTSTR pszText);
static CExpressionNode *BuildTimeOfDayLiteral (st_ParseContext *pContext, LPCTSTR pszText);
static CExpressionNode *BuildDateTimeLiteral (st_ParseContext *pContext, LPCTSTR pszText);

struct ConversionTypes
{
    BL_IEC_TYP m_SourceType;
    BL_IEC_TYP m_TargetType;
};

static ConversionTypes conversionTypes[] =
{
    { BL_BYTE , BL_INT   },     // BYTE_TO_INT
    { BL_BYTE , BL_DINT  },     // BYTE_TO_DINT
    { BL_BYTE , BL_DWORD },     // BYTE_TO_DWORD
    { BL_BYTE , BL_REAL  },     // BYTE_TO_REAL
    { BL_BYTE , BL_LREAL },     // BYTE_TO_LREAL
    { BL_BYTE , BL_WORD  },     // BYTE_TO_WORD
    { BL_DINT , BL_BYTE  },     // DINT_TO_BYTE
    { BL_DINT , BL_INT   },     // DINT_TO_INT
    { BL_DINT , BL_DWORD },     // DINT_TO_DWORD
    { BL_DINT , BL_REAL  },     // DINT_TO_REAL
    { BL_DINT , BL_LREAL },     // DINT_TO_LREAL
    { BL_DINT , BL_TIME  },     // DINT_TO_TIME
    { BL_DINT , BL_WORD  },     // DINT_TO_WORD
    { BL_INT  , BL_BYTE  },     // INT_TO_BYTE
    { BL_INT  , BL_DINT  },     // INT_TO_DINT
    { BL_INT  , BL_DWORD },     // INT_TO_DWORD
    { BL_INT  , BL_REAL  },     // INT_TO_REAL
    { BL_INT  , BL_LREAL },     // INT_TO_LREAL
    { BL_INT  , BL_TIME  },     // INT_TO_TIME
    { BL_INT  , BL_WORD  },     // INT_TO_WORD
    { BL_DWORD, BL_BYTE  },     // DWORD_TO_BYTE
    { BL_DWORD, BL_INT   },     // DWORD_TO_INT
    { BL_DWORD, BL_DINT  },     // DWORD_TO_DINT
    { BL_DWORD, BL_REAL  },     // DWORD_TO_REAL
    { BL_DWORD, BL_LREAL },     // DWORD_TO_LREAL
    { BL_DWORD, BL_WORD  },     // DWORD_TO_WORD
    { BL_LREAL, BL_BYTE  },     // LREAL_TO_BYTE
    { BL_LREAL, BL_INT   },     // LREAL_TO_INT
    { BL_LREAL, BL_DINT  },     // LREAL_TO_DINT
    { BL_LREAL, BL_DWORD },     // LREAL_TO_DWORD
    { BL_LREAL, BL_REAL  },     // LREAL_TO_REAL
    { BL_LREAL, BL_WORD  },     // LREAL_TO_WORD
    { BL_REAL , BL_BYTE  },     // REAL_TO_BYTE
    { BL_REAL , BL_INT   },     // REAL_TO_INT
    { BL_REAL , BL_DINT  },     // REAL_TO_DINT
    { BL_REAL , BL_DWORD },     // REAL_TO_DWORD
    { BL_REAL , BL_LREAL },     // REAL_TO_LREAL
    { BL_REAL , BL_WORD  },     // REAL_TO_WORD
    { BL_TIME , BL_DINT  },     // TIME_TO_DINT
    { BL_TIME , BL_INT   },     // TIME_TO_INT
    { BL_WORD , BL_BYTE  },     // WORD_TO_BYTE
    { BL_WORD , BL_INT   },     // WORD_TO_INT
    { BL_WORD , BL_DINT  },     // WORD_TO_DINT
    { BL_WORD , BL_DWORD },     // WORD_TO_DWORD
    { BL_WORD , BL_REAL  },     // WORD_TO_REAL
    { BL_WORD , BL_LREAL },     // WORD_TO_LREAL
};


/* to avoid 1000 Warning of the same kind */
#pragma warning (disable: 4309 4305 4102 4244 4551)

#if YYDEBUG
enum YY_Types { YY_t_NoneDefined, YY_t_ident, YY_t_expr, YY_t_number, YY_t_letter, YY_t_conversion, YY_t_exprlist
};
#endif
#if YYDEBUG
yyTypedRules yyRules[] = {
	{ "&00: %06 &00",  0},
	{ "%06: %01",  0},
	{ "%06: %07",  0},
	{ "%01: %02",  2},
	{ "%01: &33 %01",  2},
	{ "%01: &34 %01",  2},
	{ "%01: &11 %01",  2},
	{ "%01: %01 &33 %01",  2},
	{ "%01: %01 &34 %01",  2},
	{ "%01: %01 &35 %01",  2},
	{ "%01: %01 &36 %01",  2},
	{ "%01: %01 &19 %01",  2},
	{ "%01: %01 &10 %01",  2},
	{ "%01: %01 &12 %01",  2},
	{ "%01: %01 &13 %01",  2},
	{ "%01: %01 &14 %01",  2},
	{ "%01: %01 &32 %01",  2},
	{ "%01: %01 &26 %01",  2},
	{ "%01: %01 &27 %01",  2},
	{ "%01: %01 &28 %01",  2},
	{ "%01: %01 &29 %01",  2},
	{ "%01: %01 &30 %01",  2},
	{ "%01: %01 &31 %01",  2},
	{ "%02: &05",  2},
	{ "%02: %03",  2},
	{ "%02: &37 %01 &38",  2},
	{ "%02: &09 &37 %01 &38",  2},
	{ "%02: &15 %04",  2},
	{ "%02: &16 &37 %01 &39 %01 &38",  2},
	{ "%02: &17 %04",  2},
	{ "%02: &18 &37 %01 &39 %01 &38",  2},
	{ "%02: &19 &37 %01 &39 %01 &38",  2},
	{ "%02: &14 %04",  2},
	{ "%02: &12 %04",  2},
	{ "%02: &13 %04",  2},
	{ "%02: &20 &37 %01 &39 %01 &38",  2},
	{ "%02: &21 &37 %01 &39 %01 &38",  2},
	{ "%02: &22 &37 %01 &39 %01 &38",  2},
	{ "%02: &23 &37 %01 &39 %01 &38",  2},
	{ "%02: &24 &37 %01 &39 %01 &38",  2},
	{ "%02: &25 &37 %01 &39 %01 &38",  2},
	{ "%03: &02",  2},
	{ "%03: &03",  2},
	{ "%03: %03 &40 &02",  2},
	{ "%03: %03 &41 %01 &42",  2},
	{ "%04: &37 %01 &39 %05 &38",  6},
	{ "%05: %01",  6},
	{ "%05: %01 &39 %05",  6},
	{ "%07: &41 %08 &42",  0},
	{ "%08: %09",  0},
	{ "%08: %09 &39 %08",  0},
	{ "%09: &06",  0},
	{ "%09: &06 &08 &06",  0},
{ "$accept",  0},{ "error",  0}
};
yyNamedType yyTokenTypes[] = {
	{ "$end",  0,  0},
	{ "error",  256,  0},
	{ "IDENTIFIER",  257,  1},
	{ "MACHINEADDRESS",  258,  1},
	{ "DIRECT_VARIABLE",  259,  0},
	{ "LITERAL",  260,  2},
	{ "NUMBER",  261,  3},
	{ "LETTER",  262,  4},
	{ "DOTDOT",  263,  0},
	{ "CONVERSION",  264,  5},
	{ "POW",  265,  2},
	{ "NOT",  266,  2},
	{ "OR",  267,  2},
	{ "XOR",  268,  2},
	{ "AND",  269,  2},
	{ "ADD",  270,  2},
	{ "SUB",  271,  2},
	{ "MUL",  272,  2},
	{ "DIV",  273,  2},
	{ "MOD",  274,  2},
	{ "EQ",  275,  2},
	{ "NE",  276,  2},
	{ "LT",  277,  2},
	{ "LE",  278,  2},
	{ "GT",  279,  2},
	{ "GE",  280,  2},
	{ "EQ_SHORT",  281,  2},
	{ "NE_SHORT",  282,  2},
	{ "LT_SHORT",  283,  2},
	{ "LE_SHORT",  284,  2},
	{ "GT_SHORT",  285,  2},
	{ "GE_SHORT",  286,  2},
	{ "'&'",  38,  0},
	{ "'+'",  43,  0},
	{ "'-'",  45,  0},
	{ "'*'",  42,  0},
	{ "'/'",  47,  0},
	{ "'('",  40,  0},
	{ "')'",  41,  0},
	{ "','",  44,  0},
	{ "'.'",  46,  0},
	{ "'['",  91,  0},
	{ "']'",  93,  0}

};
#endif
static short yydef[] = {

	  54,    3, 65535,   20,   53,   18,   17,   16,   15,   14, 
	  13,   12,   11,   10,    9,    8,    7,    6,    5,    4,   19
};
static short yyex[] = {

	   0,    0, 65535,    1
};
static short yyact[] = {

	65445, 65448, 65447, 65429, 65421, 65422, 65398, 65444, 65446, 65437, 
	65436, 65438, 65443, 65442, 65441, 65440, 65439, 65435, 65434, 65433, 
	65432, 65431, 65430,  280,  279,  278,  277,  276,  275,  274, 
	 273,  272,  271,  270,  269,  268,  267,  266,  264,  260, 
	 258,  257,   91,   45,   43,   40, 65532,  261, 65450,   40, 
	65451,   40, 65452,   40, 65453,   40, 65454,   40, 65455,   40, 
	65456,   40, 65457,   40, 65458,   40, 65459,   40, 65460,   40, 
	65445, 65448, 65447, 65421, 65422, 65398, 65444, 65446, 65437, 65436, 
	65438, 65443, 65442, 65441, 65440, 65439, 65435, 65434, 65433, 65432, 
	65431, 65430,  280,  279,  278,  277,  276,  275,  274,  273, 
	 272,  271,  270,  269,  268,  267,  266,  264,  260,  258, 
	 257,   45,   43,   40, 65463, 65462,   91,   46, 65470, 65477, 
	65479, 65478, 65476, 65474, 65473, 65472, 65471, 65475, 65469, 65468, 
	65467, 65466, 65465, 65464,  286,  285,  284,  283,  282,  281, 
	 274,  269,  268,  267,  265,   47,   45,   43,   42,   38, 
	65480,  263, 65481,   44, 65397,   93, 65470, 65405, 65477, 65479, 
	65478, 65476, 65474, 65473, 65472, 65471, 65475, 65469, 65468, 65467, 
	65466, 65465, 65464,  286,  285,  284,  283,  282,  281,  274, 
	 269,  268,  267,  265,   47,   45,   43,   42,   41,   38, 
	65423,  257, 65427,  261, 65470, 65477, 65479, 65494, 65478, 65476, 
	65474, 65473, 65472, 65471, 65475, 65469, 65468, 65467, 65466, 65465, 
	65464,  286,  285,  284,  283,  282,  281,  274,  269,  268, 
	 267,  265,   47,   45,   44,   43,   42,   38, 65470, 65477, 
	65479, 65495, 65478, 65476, 65474, 65473, 65472, 65471, 65475, 65469, 
	65468, 65467, 65466, 65465, 65464,  286,  285,  284,  283,  282, 
	 281,  274,  269,  268,  267,  265,   47,   45,   44,   43, 
	  42,   38, 65470, 65477, 65479, 65496, 65478, 65476, 65474, 65473, 
	65472, 65471, 65475, 65469, 65468, 65467, 65466, 65465, 65464,  286, 
	 285,  284,  283,  282,  281,  274,  269,  268,  267,  265, 
	  47,   45,   44,   43,   42,   38, 65470, 65477, 65479, 65497, 
	65478, 65476, 65474, 65473, 65472, 65471, 65475, 65469, 65468, 65467, 
	65466, 65465, 65464,  286,  285,  284,  283,  282,  281,  274, 
	 269,  268,  267,  265,   47,   45,   44,   43,   42,   38, 
	65470, 65477, 65479, 65498, 65478, 65476, 65474, 65473, 65472, 65471, 
	65475, 65469, 65468, 65467, 65466, 65465, 65464,  286,  285,  284, 
	 283,  282,  281,  274,  269,  268,  267,  265,   47,   45, 
	  44,   43,   42,   38, 65470, 65477, 65479, 65499, 65478, 65476, 
	65474, 65473, 65472, 65471, 65475, 65469, 65468, 65467, 65466, 65465, 
	65464,  286,  285,  284,  283,  282,  281,  274,  269,  268, 
	 267,  265,   47,   45,   44,   43,   42,   38, 65470, 65477, 
	65479, 65500, 65478, 65476, 65474, 65473, 65472, 65471, 65475, 65469, 
	65468, 65467, 65466, 65465, 65464,  286,  285,  284,  283,  282, 
	 281,  274,  269,  268,  267,  265,   47,   45,   44,   43, 
	  42,   38, 65470, 65477, 65479, 65501, 65478, 65476, 65474, 65473, 
	65472, 65471, 65475, 65469, 65468, 65467, 65466, 65465, 65464,  286, 
	 285,  284,  283,  282,  281,  274,  269,  268,  267,  265, 
	  47,   45,   44,   43,   42,   38, 65470, 65477, 65479, 65502, 
	65478, 65476, 65474, 65473, 65472, 65471, 65475, 65469, 65468, 65467, 
	65466, 65465, 65464,  286,  285,  284,  283,  282,  281,  274, 
	 269,  268,  267,  265,   47,   45,   44,   43,   42,   38, 
	65470, 65477, 65479, 65503, 65478, 65476, 65474, 65473, 65472, 65471, 
	65475, 65469, 65468, 65467, 65466, 65465, 65464,  286,  285,  284, 
	 283,  282,  281,  274,  269,  268,  267,  265,   47,   45, 
	  44,   43,   42,   38, 65470, 65406, 65477, 65479, 65478, 65476, 
	65474, 65473, 65472, 65471, 65475, 65469, 65468, 65467, 65466, 65465, 
	65464,  286,  285,  284,  283,  282,  281,  274,  269,  268, 
	 267,  265,   47,   45,   43,   42,   41,   38, 65470, 65477, 
	65479, 65478, 65476, 65424, 65474, 65473, 65472, 65471, 65475, 65469, 
	65468, 65467, 65466, 65465, 65464,  286,  285,  284,  283,  282, 
	 281,  274,  269,  268,  267,  265,   93,   47,   45,   43, 
	  42,   38, 65477, 65479, 65478, 65476, 65474, 65475,  274,  265, 
	  47,   45,   43,   42, 65477, 65479, 65478, 65476, 65474, 65475, 
	65467, 65466, 65465, 65464,  286,  285,  284,  283,  274,  265, 
	  47,   45,   43,   42, 65477, 65479, 65478, 65476, 65474, 65475, 
	65469, 65468, 65467, 65466, 65465, 65464,  286,  285,  284,  283, 
	 282,  281,  274,  265,   47,   45,   43,   42, 65470, 65477, 
	65479, 65478, 65476, 65474, 65471, 65475, 65469, 65468, 65467, 65466, 
	65465, 65464,  286,  285,  284,  283,  282,  281,  274,  269, 
	 265,   47,   45,   43,   42,   38, 65470, 65477, 65479, 65478, 
	65476, 65474, 65472, 65471, 65475, 65469, 65468, 65467, 65466, 65465, 
	65464,  286,  285,  284,  283,  282,  281,  274,  269,  268, 
	 265,   47,   45,   43,   42,   38, 65474,  265, 65477, 65476, 
	65474, 65475,  274,  265,   47,   42, 65470, 65420, 65477, 65479, 
	65478, 65476, 65474, 65473, 65472, 65471, 65475, 65469, 65468, 65467, 
	65466, 65465, 65464,  286,  285,  284,  283,  282,  281,  274, 
	 269,  268,  267,  265,   47,   45,   43,   42,   41,   38, 
	65470, 65419, 65477, 65479, 65478, 65476, 65474, 65473, 65472, 65471, 
	65475, 65469, 65468, 65467, 65466, 65465, 65464,  286,  285,  284, 
	 283,  282,  281,  274,  269,  268,  267,  265,   47,   45, 
	  43,   42,   41,   38, 65470, 65418, 65477, 65479, 65478, 65476, 
	65474, 65473, 65472, 65471, 65475, 65469, 65468, 65467, 65466, 65465, 
	65464,  286,  285,  284,  283,  282,  281,  274,  269,  268, 
	 267,  265,   47,   45,   43,   42,   41,   38, 65470, 65417, 
	65477, 65479, 65478, 65476, 65474, 65473, 65472, 65471, 65475, 65469, 
	65468, 65467, 65466, 65465, 65464,  286,  285,  284,  283,  282, 
	 281,  274,  269,  268,  267,  265,   47,   45,   43,   42, 
	  41,   38, 65470, 65416, 65477, 65479, 65478, 65476, 65474, 65473, 
	65472, 65471, 65475, 65469, 65468, 65467, 65466, 65465, 65464,  286, 
	 285,  284,  283,  282,  281,  274,  269,  268,  267,  265, 
	  47,   45,   43,   42,   41,   38, 65470, 65415, 65477, 65479, 
	65478, 65476, 65474, 65473, 65472, 65471, 65475, 65469, 65468, 65467, 
	65466, 65465, 65464,  286,  285,  284,  283,  282,  281,  274, 
	 269,  268,  267,  265,   47,   45,   43,   42,   41,   38, 
	65425,   41, 65470, 65477, 65479, 65514, 65478, 65476, 65474, 65473, 
	65472, 65471, 65475, 65469, 65468, 65467, 65466, 65465, 65464,  286, 
	 285,  284,  283,  282,  281,  274,  269,  268,  267,  265, 
	  47,   45,   44,   43,   42,   38, 65470, 65411, 65477, 65479, 
	65478, 65476, 65474, 65473, 65472, 65471, 65475, 65469, 65468, 65467, 
	65466, 65465, 65464,  286,  285,  284,  283,  282,  281,  274, 
	 269,  268,  267,  265,   47,   45,   43,   42,   41,   38, 
	65470, 65410, 65477, 65479, 65478, 65476, 65474, 65473, 65472, 65471, 
	65475, 65469, 65468, 65467, 65466, 65465, 65464,  286,  285,  284, 
	 283,  282,  281,  274,  269,  268,  267,  265,   47,   45, 
	  43,   42,   41,   38, 65470, 65408, 65477, 65479, 65478, 65476, 
	65474, 65473, 65472, 65471, 65475, 65469, 65468, 65467, 65466, 65465, 
	65464,  286,  285,  284,  283,  282,  281,  274,  269,  268, 
	 267,  265,   47,   45,   43,   42,   41,   38,   -1
};
static short yypact[] = {

	 116,  134,  150,  151,  153,  608,  608,  608,  608,  624, 
	 624,  646,  646,  672,  701,  717,  717,  717,  722,  722, 
	 949,   92, 1051, 1017,  983,  931,  913,  879,  845,  811, 
	 777,  743,   92,   92,   92,   92,   92,   92,   92,   92, 
	  92,   92,  585,  551,  517,  483,  449,  415,  381,  347, 
	 313,  279,  245,  211,   47,  193,   92,   92,   92,   92, 
	  92,   92,   92,   92,   92,   92,   92,   92,   92,   92, 
	  92,   92,  191,   92,  173,   92,   92,   92,   92,   92, 
	  92,   92,   92,   92,   92,   92,  155,   92,   92,   92, 
	  92,   69,   61,   67,   61,   65,   63,   61,   61,   61, 
	  59,   57,   55,   53,   51,   49,   47,   23
};
static short yygo[] = {

	65513, 65512, 65511, 65509, 65508, 65507, 65506, 65505, 65504, 65516, 
	65517, 65518, 65519, 65520, 65404, 65521, 65522, 65523, 65524, 65525, 
	65526, 65527, 65528, 65529, 65530, 65493, 65492, 65491, 65490, 65489, 
	65488, 65487, 65486, 65485, 65484, 65483, 65482, 65401, 65402, 65403, 
	65461, 65534, 65515,  107,   90,   89,   88,   87,   85,   84, 
	  83,   82,   81,   80,   79,   78,   77,   76,   75,   73, 
	  71,   70,   69,   68,   67,   66,   65,   64,   63,   62, 
	  61,   60,   59,   58,   57,   56,   41,   40,   39,   38, 
	  37,   36,   34,   33,   32, 65399, 65535, 65407, 65409, 65412, 
	65413, 65414,   98,   97,   94,   92, 65426, 65510,   21, 65533, 
	65400, 65396, 65449,   54, 65531,   -1
};
static short yypgo[] = {

	   0,    0,    0,   99,   42,   42,   42,   42,   42,   42, 
	  42,   42,   42,   42,   42,   42,   42,   42,   42,   97, 
	 104,  104,   97,   91,   86,   86,   86,   86,   85,   85, 
	  85,   85,   85,   85,   85,   85,   85,   85,   85,   85, 
	  85,   85,   85,   85,   42,   42,   42,   42,   99,   42, 
	  85,  100,  102,  102,   85,    0
};
static short yyrlen[] = {

	   0,    0,    0,    1,    3,    3,    3,    3,    3,    3, 
	   3,    3,    3,    3,    3,    3,    3,    3,    3,    1, 
	   1,    3,    3,    5,    4,    3,    1,    1,    6,    6, 
	   6,    6,    6,    6,    2,    2,    2,    6,    6,    2, 
	   6,    2,    4,    3,    3,    2,    2,    2,    1,    1, 
	   1,    3,    3,    1,    1,    2
};
#define YYS0	107
#define YYDELTA	87
#define YYNPACT	108
#define YYNDEF	21

#define YYr53	0
#define YYr54	1
#define YYr55	2
#define YYr1	3
#define YYr7	4
#define YYr8	5
#define YYr9	6
#define YYr10	7
#define YYr11	8
#define YYr13	9
#define YYr14	10
#define YYr15	11
#define YYr16	12
#define YYr17	13
#define YYr18	14
#define YYr19	15
#define YYr20	16
#define YYr21	17
#define YYr22	18
#define YYr46	19
#define YYr51	20
#define YYr52	21
#define YYr47	22
#define YYr45	23
#define YYr44	24
#define YYr43	25
#define YYr42	26
#define YYr41	27
#define YYr40	28
#define YYr39	29
#define YYr38	30
#define YYr37	31
#define YYr36	32
#define YYr35	33
#define YYr34	34
#define YYr33	35
#define YYr32	36
#define YYr31	37
#define YYr30	38
#define YYr29	39
#define YYr28	40
#define YYr27	41
#define YYr26	42
#define YYr25	43
#define YYr12	44
#define YYr6	45
#define YYr5	46
#define YYr4	47
#define YYrACCEPT	YYr53
#define YYrERROR	YYr54
#define YYrLR2	YYr55
#if YYDEBUG
char * yysvar[] = {
	"$accept",
	"expression",
	"primary",
	"designator",
	"expressionlist2",
	"expressions",
	"root",
	"rangelist",
	"ranges",
	"range",
	0
};
short yyrmap[] = {

	  53,   54,   55,    1,    7,    8,    9,   10,   11,   13, 
	  14,   15,   16,   17,   18,   19,   20,   21,   22,   46, 
	  51,   52,   47,   45,   44,   43,   42,   41,   40,   39, 
	  38,   37,   36,   35,   34,   33,   32,   31,   30,   29, 
	  28,   27,   26,   25,   12,    6,    5,    4,    2,    3, 
	  23,   48,   50,   49,   24,    0
};
short yysmap[] = {

	  20,   27,   28,   29,   30,   87,   88,   89,   90,   91, 
	  92,   93,   94,   95,   96,   98,   99,  100,  101,  102, 
	 124,  135,  127,  126,  125,  123,  122,  121,  120,  119, 
	 118,  117,  114,  113,  112,  111,  110,  109,  108,  107, 
	 106,  105,   85,   83,   82,   81,   80,   79,   78,   77, 
	  76,   75,   74,   73,   71,   70,   69,   68,   67,   66, 
	  65,   64,   63,   62,   61,   60,   59,   58,   57,   56, 
	  55,   54,   50,   49,   48,   47,   45,   43,   42,   38, 
	  37,   36,   35,   34,   33,   32,   31,   24,   23,   22, 
	  19,   18,   17,   16,   15,   14,   13,   12,   11,   10, 
	   9,    8,    7,    6,    5,    4,    1,    0,  103,  139, 
	 134,  116,   86,    2,    3,  128,  129,  130,  131,  132, 
	 133,   39,   40,   41,  136,  137,   44,  138,   46,  115, 
	  84,   97,   51,   52,   53,   26,   25,   21,   72,  104
};
int st_parse::yyntoken = 43;
int st_parse::yynvar = 10;
int st_parse::yynstate = 140;
int st_parse::yynrule = 56;
#endif



// C++ YACC parser code
// Copyright 1991 by Mortice Kern Systems Inc.  All rights reserved.
//
// If YYDEBUG is defined as 1 and st_parse::yydebug is set to 1,
// yyparse() will print a travelogue of its actions as it reads
// and parses input.
//
// YYSYNC can be defined to cause yyparse() to attempt to always
// hold a lookahead token

const YY_MIN_STATE_NUM = 20;	// not useful to be too small!

#if YYDEBUG
#ifdef YYTRACE
long	* st_parse::States	= yyStates;
#endif
yyTypedRules * st_parse::Rules	= yyRules;
yyNamedType * st_parse::TokenTypes = yyTokenTypes;

#define YY_TRACE(fn) { done = 0; fn(); if (done) YYRETURN(-1); }
#endif

// Constructor for st_parse: user-provided tables
st_parse::st_parse(int *pError, int sz, short * states, YYSTYPE * stack)
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
// Constructor for st_parse: allocate tables with new
st_parse::st_parse(int *pError, int sz)
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
// Destructor for class st_parse
//	Free up space
st_parse::~st_parse()
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
st_parse::yyparse()
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
		
case YYr1: {	/* root :  expression */

        pContext->m_pRoot = yypvt[0].expr;
    
} break;

case YYr4: {	/* expression :  '+' expression */

        yyval.expr = pContext->m_pHolder->NewUnaryArithmetic (CExpressionUnaryArithmetic::Plus, yypvt[0].expr);
    
} break;

case YYr5: {	/* expression :  '-' expression */

        yyval.expr = pContext->m_pHolder->NewUnaryArithmetic (CExpressionUnaryArithmetic::Minus, yypvt[0].expr);
    
} break;

case YYr6: {	/* expression :  NOT expression */

        yyval.expr = pContext->m_pHolder->NewUnaryLogical (CExpressionUnaryLogical::Not, yypvt[0].expr);
    
} break;

case YYr7: {	/* expression :  expression '+' expression */

        yyval.expr = pContext->m_pHolder->NewBinaryArithmetic (CExpressionBinaryArithmetic::Add, yypvt[-2].expr, yypvt[0].expr);
    
} break;

case YYr8: {	/* expression :  expression '-' expression */

        yyval.expr = pContext->m_pHolder->NewBinaryArithmetic (CExpressionBinaryArithmetic::Subtract, yypvt[-2].expr, yypvt[0].expr);
    
} break;

case YYr9: {	/* expression :  expression '*' expression */

        yyval.expr = pContext->m_pHolder->NewBinaryArithmetic (CExpressionBinaryArithmetic::Multiply, yypvt[-2].expr, yypvt[0].expr);
    
} break;

case YYr10: {	/* expression :  expression '/' expression */

        yyval.expr = pContext->m_pHolder->NewBinaryArithmetic (CExpressionBinaryArithmetic::Divide, yypvt[-2].expr, yypvt[0].expr);
    
} break;

case YYr11: {	/* expression :  expression MOD expression */

        yyval.expr = pContext->m_pHolder->NewBinaryArithmetic (CExpressionBinaryArithmetic::Modulo, yypvt[-2].expr, yypvt[0].expr);
    
} break;

case YYr12: {	/* expression :  expression POW expression */

        yyval.expr = pContext->m_pHolder->NewPower (yypvt[-2].expr, yypvt[0].expr);
    
} break;

case YYr13: {	/* expression :  expression OR expression */

        yyval.expr = pContext->m_pHolder->NewBinaryLogical (CExpressionBinaryLogical::Or, yypvt[-2].expr, yypvt[0].expr);
    
} break;

case YYr14: {	/* expression :  expression XOR expression */

        yyval.expr = pContext->m_pHolder->NewBinaryLogical (CExpressionBinaryLogical::Xor, yypvt[-2].expr, yypvt[0].expr);
    
} break;

case YYr15: {	/* expression :  expression AND expression */

        yyval.expr = pContext->m_pHolder->NewBinaryLogical (CExpressionBinaryLogical::And, yypvt[-2].expr, yypvt[0].expr);
    
} break;

case YYr16: {	/* expression :  expression '&' expression */

        yyval.expr = pContext->m_pHolder->NewBinaryLogical (CExpressionBinaryLogical::And, yypvt[-2].expr, yypvt[0].expr);
    
} break;

case YYr17: {	/* expression :  expression EQ_SHORT expression */

        yyval.expr = pContext->m_pHolder->NewRelation (CExpressionRelation::Equal, yypvt[-2].expr, yypvt[0].expr);
    
} break;

case YYr18: {	/* expression :  expression NE_SHORT expression */

        yyval.expr = pContext->m_pHolder->NewRelation (CExpressionRelation::Unequal, yypvt[-2].expr, yypvt[0].expr);
    
} break;

case YYr19: {	/* expression :  expression LT_SHORT expression */

        yyval.expr = pContext->m_pHolder->NewRelation (CExpressionRelation::Less, yypvt[-2].expr, yypvt[0].expr);
    
} break;

case YYr20: {	/* expression :  expression LE_SHORT expression */

        yyval.expr = pContext->m_pHolder->NewRelation (CExpressionRelation::LessEqual, yypvt[-2].expr, yypvt[0].expr);
    
} break;

case YYr21: {	/* expression :  expression GT_SHORT expression */

        yyval.expr = pContext->m_pHolder->NewRelation (CExpressionRelation::Greater, yypvt[-2].expr, yypvt[0].expr);
    
} break;

case YYr22: {	/* expression :  expression GE_SHORT expression */

        yyval.expr = pContext->m_pHolder->NewRelation (CExpressionRelation::GreaterEqual, yypvt[-2].expr, yypvt[0].expr);
    
} break;

case YYr25: {	/* primary :  '(' expression ')' */

        yyval.expr = yypvt[-1].expr;
    
} break;

case YYr26: {	/* primary :  CONVERSION '(' expression ')' */

        yyval.expr = pContext->m_pHolder->NewConversion (yypvt[-1].expr, yypvt[-3].conversion.source, yypvt[-3].conversion.target);
    
} break;

case YYr27: {	/* primary :  ADD expressionlist2 */

        yyval.expr = pContext->m_pHolder->NewFunctionalArithmetic (CExpressionBinaryArithmetic::Add, yypvt[0].exprlist);
        delete yypvt[0].exprlist;
    
} break;

case YYr28: {	/* primary :  SUB '(' expression ',' expression ')' */

        yyval.expr = pContext->m_pHolder->NewBinaryArithmetic (CExpressionBinaryArithmetic::Subtract, yypvt[-3].expr, yypvt[-1].expr);
    
} break;

case YYr29: {	/* primary :  MUL expressionlist2 */

        yyval.expr = pContext->m_pHolder->NewFunctionalArithmetic (CExpressionBinaryArithmetic::Multiply, yypvt[0].exprlist);
        delete yypvt[0].exprlist;
    
} break;

case YYr30: {	/* primary :  DIV '(' expression ',' expression ')' */

        yyval.expr = pContext->m_pHolder->NewBinaryArithmetic (CExpressionBinaryArithmetic::Divide, yypvt[-3].expr, yypvt[-1].expr);
    
} break;

case YYr31: {	/* primary :  MOD '(' expression ',' expression ')' */

        yyval.expr = pContext->m_pHolder->NewBinaryArithmetic (CExpressionBinaryArithmetic::Modulo, yypvt[-3].expr, yypvt[-1].expr);
    
} break;

case YYr32: {	/* primary :  AND expressionlist2 */

        yyval.expr = pContext->m_pHolder->NewFunctionalLogical (CExpressionBinaryLogical::And, yypvt[0].exprlist);
        delete yypvt[0].exprlist;
    
} break;

case YYr33: {	/* primary :  OR expressionlist2 */

        yyval.expr = pContext->m_pHolder->NewFunctionalLogical (CExpressionBinaryLogical::Or, yypvt[0].exprlist);
        delete yypvt[0].exprlist;
    
} break;

case YYr34: {	/* primary :  XOR expressionlist2 */

        yyval.expr = pContext->m_pHolder->NewFunctionalLogical (CExpressionBinaryLogical::Xor, yypvt[0].exprlist);
        delete yypvt[0].exprlist;
    
} break;

case YYr35: {	/* primary :  EQ '(' expression ',' expression ')' */

        yyval.expr = pContext->m_pHolder->NewRelation (CExpressionRelation::Equal, yypvt[-3].expr, yypvt[-1].expr);
    
} break;

case YYr36: {	/* primary :  NE '(' expression ',' expression ')' */

        yyval.expr = pContext->m_pHolder->NewRelation (CExpressionRelation::Unequal, yypvt[-3].expr, yypvt[-1].expr);
    
} break;

case YYr37: {	/* primary :  LT '(' expression ',' expression ')' */

        yyval.expr = pContext->m_pHolder->NewRelation (CExpressionRelation::Less, yypvt[-3].expr, yypvt[-1].expr);
    
} break;

case YYr38: {	/* primary :  LE '(' expression ',' expression ')' */

        yyval.expr = pContext->m_pHolder->NewRelation (CExpressionRelation::LessEqual, yypvt[-3].expr, yypvt[-1].expr);
    
} break;

case YYr39: {	/* primary :  GT '(' expression ',' expression ')' */

        yyval.expr = pContext->m_pHolder->NewRelation (CExpressionRelation::Greater, yypvt[-3].expr, yypvt[-1].expr);
    
} break;

case YYr40: {	/* primary :  GE '(' expression ',' expression ')' */

        yyval.expr = pContext->m_pHolder->NewRelation (CExpressionRelation::GreaterEqual, yypvt[-3].expr, yypvt[-1].expr);
    
} break;

case YYr41: {	/* designator :  IDENTIFIER */

        CString strPrefix;

        yyval.expr = pContext->m_pHolder->NewScopedName (pContext->m_ExpandMode, pContext->m_strScope, *yypvt[0].ident.text, strPrefix);
        delete yypvt[0].ident.text;

        if (!strPrefix.IsEmpty ())
        {
            PrefixedIdentifier prefixedIdentifier (yypvt[0].ident.offset, strPrefix);

            pContext->m_PrefixedIdentifiers.AddTail (prefixedIdentifier);
        };
    
} break;

case YYr42: {	/* designator :  MACHINEADDRESS */

        yyval.expr = pContext->m_pHolder->NewConfiguration (*yypvt[0].ident.text, true);
        delete yypvt[0].ident.text;
    
} break;

case YYr43: {	/* designator :  designator '.' IDENTIFIER */

        yyval.expr = pContext->m_pHolder->NewComponent (yypvt[-2].expr, *yypvt[0].ident.text);
        delete yypvt[0].ident.text;
    
} break;

case YYr44: {	/* designator :  designator '[' expression ']' */

        yyval.expr = pContext->m_pHolder->NewIndexing (yypvt[-3].expr, yypvt[-1].expr);
    
} break;

case YYr45: {	/* expressionlist2 :  '(' expression ',' expressions ')' */

        yyval.exprlist = yypvt[-1].exprlist;
        yyval.exprlist->AddHead (yypvt[-3].expr);
    
} break;

case YYr46: {	/* expressions :  expression */

        yyval.exprlist = new CList<CExpressionNode *, CExpressionNode *>;
        yyval.exprlist->AddHead (yypvt[0].expr);
    
} break;

case YYr47: {	/* expressions :  expression ',' expressions */

        yyval.exprlist = yypvt[0].exprlist;
        yyval.exprlist->AddHead (yypvt[-2].expr);
    
} break;

case YYr51: {	/* range :  NUMBER */

        pContext->m_pRanges->Add (CExpressionHolder::Range (yypvt[0].number, yypvt[0].number));
    
} break;

case YYr52: {	/* range :  NUMBER DOTDOT NUMBER */

        pContext->m_pRanges->Add (CExpressionHolder::Range (yypvt[-2].number, yypvt[0].number));
    
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
st_parse::yyGetType(int tok)
{
	yyNamedType * tp;
	for (tp = &yyTokenTypes[yyntoken-1]; tp > yyTokenTypes; tp--)
		if (tp->token == tok)
			return tp->type;
	return 0;
}

	
// Print a token legibly.
char *
st_parse::yyptok(int tok)
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
st_parse::yygetState(int num)
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
st_parse::yyExpandName(int num, int isrule, char * buf, int len)
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
st_parse::yyShowState()
{
	(void) printf("state %d (%d), char %s (%d)\n%d stateStack entries\n",
		yysmap[yystate],yystate,yyptok(yychar),yychar,
		yypv - valueStack);
}
// show results of reduction: yyi is rule number
void
st_parse::yyShowReduce()
{
	(void) printf("Reduce by rule %d (pop#=%d)\n", yyrmap[rule], npop);
}
// show read token
void
st_parse::yyShowRead()
{
	(void) printf("read %s (%d)\n", yyptok(yychar), yychar);
}
// show Goto
void
st_parse::yyShowGoto()
{
	(void) printf("goto %d (%d)\n", yysmap[yystate], yystate);
}
// show Shift
void
st_parse::yyShowShift()
{
	(void) printf("shift %d (%d)\n", yysmap[yystate], yystate);
}
// show error recovery
void
st_parse::yyShowErrRecovery()
{
	(void) printf("Error recovery pops state %d (%d), uncovers %d (%d)\n",
		yysmap[*(yyps-1)], *(yyps-1), yysmap[yystate], yystate);
}
// show token discards in error processing
void
st_parse::yyShowErrDiscard()
{
	(void) printf("Error recovery discards %s (%d), ",
		yyptok(yychar), yychar);
}
#endif	/* ! YYTRACE */
#endif	/* YYDEBUG */
HRESULT ParseItem (const CString &strText, CEExpandMode expandMode, const CString &strScope, CExpressionHolder *pHolder)
{
    HRESULT hrExpression = S_OK;
    HRESULT hrFormat = S_OK;
    HRESULT hrRanges = S_OK;
    int iPos;
    unsigned int uiLevel=0;
    CString strExpression;
    CString strFormatRanges;
    CString strFormat;
    CString strRanges;
    CEWatchItemState state;
    CEWatchItemState expressionState;
    CEWatchItemState formatState;
    CEWatchItemState rangesState;
    CString strPrefixedText;

    iPos = 0;
    while (iPos < strText.GetLength ())
    {
        TCHAR c;

        c = strText[iPos];

        if (c == '(' || c == '[')
        {
            ++uiLevel;
            ++iPos;
            continue;
        };

        if (c == ')' || c == ']')
        {
            if (uiLevel > 0)
            {
                --uiLevel;
            };

            ++iPos;
            continue;
        };

        if (c == '{' || c == '"' || c == '\'')
        {
            ++iPos;

            if (c == '{')
            {
                c = '}';
            };

            while (iPos < strText.GetLength () && strText[iPos] != c)
            {
                ++iPos;
            };

            if (iPos < strText.GetLength ())
            {
                ++iPos;
            };

            continue;
        };

        if (c == ',' && uiLevel == 0)
        {
            break;
        };

        ++iPos;
    };

    strExpression = strText.Left (iPos);

    if (iPos < strText.GetLength ())
    {
        strFormatRanges = strText.Mid (iPos + 1);

        iPos = 0;
        while (iPos < strFormatRanges.GetLength ())
        {
            if (strFormatRanges[iPos] == '[')
            {
                break;
            };

            ++iPos;
        };

        strFormat = strFormatRanges.Left (iPos);
        strRanges = strFormatRanges.Mid (iPos);
    };

    hrExpression = ParseExpression (strExpression, expandMode, strScope, pHolder, expressionState);
    if (FAILED (hrExpression))
    {
        return (hrExpression);
    };

    if (!strFormat.IsEmpty ())
    {
        hrFormat = ParseFormat (strFormat, pHolder, formatState);
        if (FAILED (hrFormat))
        {
            return (hrFormat);
        };
    };

    if (!strRanges.IsEmpty ())
    {
        hrRanges = ParseRanges (strRanges, pHolder, rangesState);
        if (FAILED (hrRanges))
        {
            return (hrRanges);
        };
    };

    if (hrExpression == S_FALSE)
    {
        state = expressionState;
    }
    else if (hrFormat == S_FALSE)
    {
        state = formatState;
    }
    else if (hrRanges == S_FALSE)
    {
        state = rangesState;
    }
    else
    {
        state = CEGoodWatchItem;
    };

    strPrefixedText = strExpression;
    if (!strFormat.IsEmpty () || !strRanges.IsEmpty ())
    {
        strPrefixedText += _T (",") + strFormat + strRanges;
    };

    pHolder->SetText (strPrefixedText, state);

    return (S_OK);
}


static HRESULT ParseExpression (CString &strText, CEExpandMode expandMode, const CString &strScope, CExpressionHolder *pHolder, CEWatchItemState &state)
{
    st_ParseContext context;
    HRESULT hr;

    context.m_pRoot = NULL;
    context.m_bRangeError = false;
    context.m_ExpandMode = strText.Find('.')>0 ? CEExpandNever : expandMode; //##EF:don't expand if "PC1.res1..":  PC1 may be var name in another target!! 
    context.m_strScope = strScope;

    hr = ParseItemPart (strText, pHolder, STS_EXPRESSION_SYNTAX, &context);
    if (FAILED (hr))
    {
        return (hr);
    };

    if (hr != S_OK)
    {
        state = CESyntaxError;
    }
    else
    {
        PrependPrefixes (strText, context.m_PrefixedIdentifiers);
        
        if (context.m_bRangeError)
        {
            hr = S_FALSE;
            state = CEBadNumber;
        };
    };

    pHolder->SetExpression (strText, hr == S_OK ? context.m_pRoot : NULL);

    return (hr);
}


static void PrependPrefixes (CString &strText, CList<PrefixedIdentifier, const PrefixedIdentifier &> &prefixedIdentifiers)
{
    while (!prefixedIdentifiers.IsEmpty ())
    {
        POSITION pPosition;
        POSITION pBestPosition=NULL;
        PrefixedIdentifier bestPrefixedIdentifier;

        pPosition = prefixedIdentifiers.GetHeadPosition ();
        while (pPosition != NULL)
        {
            POSITION pCurrentPosition;
            PrefixedIdentifier prefixedIdentifier;

            pCurrentPosition = pPosition;

            prefixedIdentifier = prefixedIdentifiers.GetNext (pPosition);

            if (pBestPosition == NULL || prefixedIdentifier.m_lOffset > bestPrefixedIdentifier.m_lOffset)
            {
                pBestPosition = pCurrentPosition;
                bestPrefixedIdentifier = prefixedIdentifier;
            };
        };

        strText = strText.Left (bestPrefixedIdentifier.m_lOffset) +
                  bestPrefixedIdentifier.m_strPrefix + _T (".") +
                  strText.Mid (bestPrefixedIdentifier.m_lOffset);

        prefixedIdentifiers.RemoveAt (pBestPosition);
    };
}


HRESULT ParseFormat (const CString &strText, CExpressionHolder *pHolder, CEWatchItemState &state)
{
    HRESULT hr;
    BL_FormatInfo formatInfo;
    USES_CONVERSION;

    if (pHolder->GetType () == NULL)
    {
        hr = BL_ParseFormat (T2COLE (strText), &formatInfo);
    }
    else
    {
        BL_IEC_TYP iecType;

        if (!pHolder->IsSimpleType (iecType))
        {
            hr = E_BL_ERR_FORMATERROR;
        }
        else
        {
            hr = BL_ParseFormatForType (T2COLE (strText), iecType, &formatInfo);
        };
    };

    if (SUCCEEDED (hr))
    {
        // is S_FALSE if format string is empty
        hr = S_OK;
    }
    else
    {
        if (hr == E_BL_ERR_SYNTAX)
        {
            state = CESyntaxError;
            hr = S_FALSE;
        }
        else if (hr == E_BL_ERR_RANGE)
        {
            state = CEBadNumber;
            hr = S_FALSE;
        }
        else if (hr == E_BL_ERR_FORMATERROR)
        {
            state = CEFormatInvalid;
            hr = S_FALSE;
        };
    };

    pHolder->SetFormat (strText, hr == S_OK ? &formatInfo : NULL);

    return (hr);
}


static HRESULT ParseRanges (const CString &strText, CExpressionHolder *pHolder, CEWatchItemState &state)
{
    st_ParseContext context;
    CArray<CExpressionHolder::Range, const CExpressionHolder::Range &> ranges;
    HRESULT hr;

    context.m_pRanges = &ranges;

    hr = ParseItemPart (strText, pHolder, STS_RANGES_SYNTAX, &context);
    if (FAILED (hr))
    {
        return (hr);
    };

    if (hr != S_OK)
    {
        state = CESyntaxError;
    }
    else if (context.m_bRangeError)
    {
        hr = S_FALSE;
        state = CEBadNumber;
    }
    else if (pHolder->GetSubItemCount () == 0)
    {
        hr = S_FALSE;
        state = CERangeInvalid;
    };

    pHolder->SetRanges (strText, hr == S_OK ? &ranges : NULL);

    return (hr);
}


static HRESULT ParseItemPart (const CString &strText, CExpressionHolder *pHolder, STS_ScannerMode mode, st_ParseContext *pCtx)
{
    sts_scanner *pScanner;
    int result;
    st_parse parser (&result);

    if (result == 1)
    {
        return (E_OUTOFMEMORY);
    };
    ASSERT (result == 0);

    pScanner = STS_CreateScanner ();
    ASSERT (pScanner != NULL);
    if (pScanner == NULL)
    {
        delete pHolder;
        return (E_OUTOFMEMORY);
    };
    STS_SetInputString (pScanner, (LPCTSTR)strText, strText.GetLength ());
    STS_SetScannerMode (pScanner, mode);

    pCtx->m_pScanner = pScanner;
    pCtx->m_pHolder = pHolder;
    parser.setParseContext (pCtx);

    result = parser.yyparse ();

    STS_DeleteScanner (pScanner);

    if (result < 0)
    {
        return (E_OUTOFMEMORY);
    };

    return (result == 0 ? S_OK : S_FALSE);
}


int st_parse::yynexttoken (YYSTYPE *yylval)
{
    int     tok;
    LPCTSTR yytext;
    USES_CONVERSION;
    CString strLiteral;

nextToken:  
    tok = STS_NextToken (pContext->m_pScanner, &yytext);
    
    switch(tok)
    {
    case STS_SYNTAX_ERROR:
        return (-2);
        //pContext->tokenSyntax (yytext);
		goto nextToken;
    
    case STS_IDENTIFIER:
        yylval->ident.text = new CString (yytext);
        yylval->ident.offset = STS_GetStartOffset (pContext->m_pScanner);
	    return (IDENTIFIER);

    case STS_DIRECT_VARIABLE:
        yylval->ident.text = new CString (yytext);
	    return (DIRECT_VARIABLE);

	case STS_INT_LITERAL:
        yylval->expr = BuildIntegerLiteral (pContext, yytext);
		return (LITERAL);

    case STS_FALSE_LITERAL:
		yylval->expr = pContext->m_pHolder->NewLiteral (false);
		return (LITERAL);

    case STS_TRUE_LITERAL:
		yylval->expr = pContext->m_pHolder->NewLiteral (true);
		return (LITERAL);

    case STS_DT_LITERAL:
        yylval->expr = BuildDateTimeLiteral (pContext, yytext);
        return (LITERAL);

    case STS_TOD_LITERAL:
        yylval->expr = BuildTimeOfDayLiteral (pContext, yytext);
        return (LITERAL);

    case STS_DATE_LITERAL:
        yylval->expr = BuildDateLiteral (pContext, yytext);
        return (LITERAL);

    case STS_TIME_LITERAL:
        yylval->expr = BuildTimeLiteral (pContext, yytext);
        return (LITERAL);

    case STS_FLOAT_LITERAL:
		yylval->expr = BuildFloatLiteral (pContext, yytext);
		return (LITERAL);

    case STS_STRING_LITERAL:
    case STS_WSTRING_LITERAL:
        strLiteral = yytext;
        ASSERT ((strLiteral[0] == _T ('"') ||
                 strLiteral[0] == _T ('\'')) &&
                (strLiteral[strLiteral.GetLength () - 1] == _T ('"') ||
                 strLiteral[strLiteral.GetLength () - 1] == _T ('\'')));
        strLiteral = strLiteral.Mid (1, strLiteral.GetLength () - 2);
		yylval->expr = pContext->m_pHolder->NewLiteral (strLiteral);
	    return LITERAL;

    case STS_ADD: return (ADD);
    case STS_SUB: return (SUB);
    case STS_MUL: return (MUL);
    case STS_DIV: return (DIV);
    case STS_MOD: return (MOD);
    case STS_POW: return (POW);
    case STS_AND: return (AND);
    case STS_OR : return (OR);
    case STS_XOR: return (XOR);
    case STS_NOT: return (NOT);

    case STS_EQ: return (EQ);
    case STS_NE: return (NE);
    case STS_LT: return (LT);
    case STS_LE: return (LE);
    case STS_GT: return (GT);
    case STS_GE: return (GE);

    case STS_EQ_SHORT: return (EQ_SHORT);
    case STS_NE_SHORT: return (NE_SHORT);
    case STS_LT_SHORT: return (LT_SHORT);
    case STS_LE_SHORT: return (LE_SHORT);
    case STS_GT_SHORT: return (GT_SHORT);
    case STS_GE_SHORT: return (GE_SHORT);

    case STS_MACHINE_ADDRESS:
        {
            CString strTemp;

            strTemp = yytext;
            ASSERT (strTemp.GetLength () >= 2);
            yylval->ident.text = new CString (strTemp.Mid (1, strTemp.GetLength () - 2));
            return MACHINEADDRESS;
        };

    case STS_BYTE_TO_DINT:
    case STS_BYTE_TO_DWORD:
    case STS_BYTE_TO_LREAL:
    case STS_BYTE_TO_WORD:
    case STS_DINT_TO_BYTE:
    case STS_DINT_TO_DWORD:
    case STS_DINT_TO_LREAL:
    case STS_DINT_TO_TIME:
    case STS_DINT_TO_WORD:
    case STS_DWORD_TO_BYTE:
    case STS_DWORD_TO_DINT:
    case STS_DWORD_TO_LREAL:
    case STS_DWORD_TO_WORD:
    case STS_LREAL_TO_BYTE:
    case STS_LREAL_TO_DINT:
    case STS_LREAL_TO_DWORD:
    case STS_LREAL_TO_WORD:
    case STS_TIME_TO_DINT:
    case STS_WORD_TO_BYTE:
    case STS_WORD_TO_DINT:
    case STS_WORD_TO_DWORD:
    case STS_WORD_TO_LREAL:
        yylval->conversion.source = conversionTypes[tok - STS_FIRST_CONVERSION].m_SourceType;
        yylval->conversion.target = conversionTypes[tok - STS_FIRST_CONVERSION].m_TargetType;
        return (CONVERSION);

    case STS_DOTDOT: return (DOTDOT);

    //default: tokens <256 or token not valid in this grammer:
    default:
        return (tok);
    };
}


void st_parse::yyerrorStackOverflow ()
{
}


void st_parse::yyerrorSyntax ()
{
}


static CExpressionNode *BuildIntegerLiteral (st_ParseContext *pContext, LPCTSTR pszText)
{
    LPCTSTR psz;
    bool bHaveType=false;
    BL_IEC_TYP iecType;
    DWORD dwModifiers;
    bool bNegative=false;
    unsigned int uiBase=10;
    HRESULT hr;
    __int64 llValue;
    CExpressionNode *pExpression;
    USES_CONVERSION;

    psz = pszText;
    while (_istalpha (*psz))
    {
        ++psz;
    };
    if (psz != pszText)
    {
        TCHAR szBuffer[20];

        // Lex enforces '#'. Anything else is a programming error.
        ASSERT (*psz == _T ('#'));
        // Lex enforces limited set of type names which are all
        // shorter than buffe size. Anything else is a programming
        // error.
        ASSERT (psz - pszText < sizeof (szBuffer) / sizeof (TCHAR));

        if (*psz == _T ('#') &&
            psz - pszText < sizeof (szBuffer) / sizeof (TCHAR))
        {
            _tcsncpy (szBuffer, pszText, psz - pszText);
            szBuffer[psz - pszText] = _T ('\0');

            hr = BL_TypenameToDatatype (T2W (szBuffer), &iecType, &dwModifiers);
            // Lex enforces type name. Anything else is a programming error.
            ASSERT (SUCCEEDED (hr));
            if (SUCCEEDED (hr))
            {
                bHaveType = true;
            };

            ++psz;
        };
    };

    if (*psz == _T ('+') || *psz == _T ('-'))
    {
        if (*psz == _T ('-'))
        {
            bNegative = true;
        };

        ++psz;
    };

    if (psz[0] == _T ('2') && psz[1] == _T ('#'))
    {
        uiBase = 2;
        psz += 2;
    }
    else if (psz[0] == _T ('8') && psz[1] == _T ('#'))
    {
        uiBase = 8;
        psz += 2;
    }
    else if (psz[0] == _T ('1') && psz[1] == _T ('6') && psz[2] == _T ('#'))
    {
        uiBase = 16;
        psz += 3;
    };

    hr = BL_ParseInt64 (T2W (psz), &llValue, false, uiBase, NULL);
    // Lex enforces a number. Anything else is a programming error.
    ASSERT (SUCCEEDED (hr));

    if (bNegative)
    {
        llValue = -llValue;
    };

    if (bHaveType)
    {
        if (iecType == BL_BOOL)
        {
            pExpression = pContext->m_pHolder->NewLiteral ((bool)(llValue != 0));
        }
        else
        {
            pExpression = pContext->m_pHolder->NewLiteral (llValue, iecType);
        };
    }
    else
    {
        pExpression = pContext->m_pHolder->NewLiteral (llValue);
    };

    return (pExpression);
}


static CExpressionNode *BuildFloatLiteral (st_ParseContext *pContext, LPCTSTR pszText)
{
    LPTSTR pszBuffer;
    LPTSTR psz;
    TCHAR c;
    bool bHaveType=false;
    HRESULT hr;
    BL_IEC_TYP iecType;
    DWORD dwModifiers;
    double dblValue;
    LPTSTR pszEnd;
    CExpressionNode *pExpression;
    USES_CONVERSION;

    pszBuffer = new TCHAR[_tcslen (pszText) + 1];
    if (pszBuffer == NULL)
    {
        return (NULL);
    };

    psz = pszBuffer;
    while ((c = *pszText++) != _T ('\0'))
    {
        if (c != _T ('_'))
        {
            *psz++ = c;
        };
    };
    *psz = _T ('\0');

    psz = pszBuffer;
    while (_istalpha (*psz))
    {
        ++psz;
    };
    if (psz != pszBuffer)
    {
        ASSERT (*psz == _T ('#'));
        if (*psz == _T ('#'))
        {
            *psz = _T ('\0');

            hr = BL_TypenameToDatatype (T2W (pszBuffer), &iecType, &dwModifiers);
            // Lex enforces type name. Anything else is a programming error.
            ASSERT (SUCCEEDED (hr));
            ASSERT (iecType == BL_LREAL || iecType == BL_REAL);
            if (SUCCEEDED (hr) && (iecType == BL_LREAL || iecType == BL_REAL))
            {
                bHaveType = true;
            };

            ++psz;        
        };
    };

    errno = 0;
    dblValue = _tcstod (psz, &pszEnd);
    ASSERT (*pszEnd == _T ('\0'));
    if (*pszEnd != _T ('\0'))
    {
        dblValue = 0.0;
    };
    if (errno == ERANGE)
    {
        pContext->m_bRangeError = true;
    };

    delete[] pszBuffer;

    if (bHaveType)
    {
        pExpression = pContext->m_pHolder->NewLiteral (dblValue, iecType);
    }
    else
    {
        pExpression = pContext->m_pHolder->NewLiteral (dblValue);
    };

    return (pExpression);
}


static CExpressionNode *BuildTimeLiteral (st_ParseContext *pContext, LPCTSTR pszText)
{
    HRESULT hr;
    __int64 llValue=0;
    CExpressionNode *pExpression;
    USES_CONVERSION;

    hr = BL_IECTimeStrToMillis (T2W (pszText), &llValue, NULL);
    if (hr == E_BL_ERR_RANGE)
    {
        pContext->m_bRangeError = true;
    }
    else
    {
        // Lex enforces TIME literal. Anything else is a programming error.
        ASSERT (SUCCEEDED (hr));
    };

    pExpression = pContext->m_pHolder->NewLiteral (llValue, BL_TIME);
    return (pExpression);
}


static CExpressionNode *BuildDateLiteral (st_ParseContext *pContext, LPCTSTR pszText)
{
    HRESULT hr;
    long lValue=0;
    SYSTEMTIME systemTime;
    CExpressionNode *pExpression;
    USES_CONVERSION;

    hr = BL_IECDateStrToDaysSince1970 (T2W (pszText), &systemTime, &lValue, NULL);
    if (hr == E_BL_ERR_RANGE)
    {
        pContext->m_bRangeError = true;
    }
    else
    {
        // Lex enforces DATE literal. Anything else is a programming error.
        ASSERT (SUCCEEDED (hr));
    };

    pExpression = pContext->m_pHolder->NewLiteral ((__int64)lValue, BL_DATE);
    return (pExpression);
}


static CExpressionNode *BuildTimeOfDayLiteral (st_ParseContext *pContext, LPCTSTR pszText)
{
    HRESULT hr;
    long lValue=0;
    SYSTEMTIME systemTime;
    CExpressionNode *pExpression;
    USES_CONVERSION;

    hr = BL_IECTimeOfDayStrToMillis (T2W (pszText), &systemTime, &lValue, NULL);
    if (hr == E_BL_ERR_RANGE)
    {
        pContext->m_bRangeError = true;
    }
    else
    {
        // Lex enforces TOD literal. Anything else is a programming error.
        ASSERT (SUCCEEDED (hr));
    };

    pExpression = pContext->m_pHolder->NewLiteral ((__int64)lValue, BL_TOD);
    return (pExpression);
}


static CExpressionNode *BuildDateTimeLiteral (st_ParseContext *pContext, LPCTSTR pszText)
{
    HRESULT hr;
    __int64 llValue=0;
    SYSTEMTIME systemTime;
    CExpressionNode *pExpression;
    USES_CONVERSION;

    hr = BL_IECDateAndTimeStrToUTCJava (T2W (pszText), &systemTime, &llValue, NULL);
    if (hr == E_BL_ERR_RANGE)
    {
        pContext->m_bRangeError = true;
    }
    else
    {
        // Lex enforces DT literal. Anything else is a programming error.
        ASSERT (SUCCEEDED (hr));
    };

    pExpression = pContext->m_pHolder->NewLiteral (llValue, BL_DT);
    return (pExpression);
}

/*END yyreplace.pl: file st.cpp~ */
