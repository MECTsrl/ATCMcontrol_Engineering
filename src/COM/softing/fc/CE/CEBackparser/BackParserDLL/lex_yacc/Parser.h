typedef union {
    LexVal     *lexval;
    SysComment *syscmnt;
} YYSTYPE;
const SYSTEM_COMMENT_BEGIN = 257;
const SYSTEM_COMMENT_END = 258;
const VARIABLE_COMMENT = 259;
const VAR_GLOBAL = 260;
const END_VAR = 261;
const TOKEN_CONST = 262;
const TOKEN_END_CONST = 263;
const TYPE = 264;
const END_TYPE = 265;
const STRUCT = 266;
const END_STRUCT = 267;
const CONFIGURATION = 268;
const END_CONFIGURATION = 269;
const RESOURCE = 270;
const ON = 271;
const END_RESOURCE = 272;
const WITH = 273;
const TASK = 274;
const PROGRAM = 275;
const END_PROGRAM = 276;
const FUNCTION_BLOCK = 277;
const END_FUNCTION_BLOCK = 278;
const FUNCTION = 279;
const END_FUNCTION = 280;
const SINT = 281;
const TOKEN_INT = 282;
const DINT = 283;
const LINT = 284;
const USINT = 285;
const TOKEN_UINT = 286;
const UDINT = 287;
const ULINT = 288;
const REAL = 289;
const LREAL = 290;
const TOKEN_DATE = 291;
const TIME_OF_DAY = 292;
const TOD = 293;
const DATE_AND_TIME = 294;
const DT = 295;
const STRING = 296;
const WSTRING = 297;
const TIME = 298;
const TOKEN_BOOL = 299;
const TOKEN_BYTE = 300;
const TOKEN_WORD = 301;
const TOKEN_DWORD = 302;
const LWORD = 303;
const AT = 304;
const EN = 305;
const ENO = 306;
const R_EDGE = 307;
const F_EDGE = 308;
const VAR = 309;
const VAR_INPUT = 310;
const VAR_OUTPUT = 311;
const VAR_IN_OUT = 312;
const VAR_EXTERNAL = 313;
const VAR_ACCESS = 314;
const VAR_TEMP = 315;
const VAR_IN_EXT = 316;
const VAR_CONFIG = 317;
const READ_ONLY = 318;
const READ_WRITE = 319;
const STEP = 320;
const END_STEP = 321;
const INITIAL_STEP = 322;
const TRANSITION = 323;
const END_TRANSITION = 324;
const ACTION = 325;
const END_ACTION = 326;
const FROM = 327;
const IF = 328;
const THEN = 329;
const ELSE = 330;
const ELSIF = 331;
const END_IF = 332;
const CASE = 333;
const END_CASE = 334;
const WHILE = 335;
const DO = 336;
const END_WHILE = 337;
const REPEAT = 338;
const UNTIL = 339;
const END_REPEAT = 340;
const FOR = 341;
const TO = 342;
const BY = 343;
const END_FOR = 344;
const EXIT = 345;
const RETURN = 346;
const GOTO = 347;
const SYNCHRONIZED = 348;
const END_SYNCHRONIZED = 349;
const VAR_NATIVE = 350;
const END_NATIVE = 351;
const QVT_BOOL = 352;
const QVT_BYTE = 353;
const QVT_WORD = 354;
const QVT_DWORD = 355;
const QVT_DINT = 356;
const QVT_LREAL = 357;
const CURL = 358;
const SEMICOLON = 359;
const COLON = 360;
const COMMA = 361;
const DOT = 362;
const DOTDOT = 363;
const EQ = 364;
const NE = 365;
const LT = 366;
const LE = 367;
const GT = 368;
const GE = 369;
const MOD = 370;
const AND = 371;
const OR = 372;
const XOR = 373;
const NOT = 374;
const POW = 375;
const PLUS = 376;
const MINUS = 377;
const TIMES = 378;
const DIV = 379;
const BECOMES = 380;
const RIGHT_ARROW = 381;
const PERIOD = 382;
const PIPE = 383;
const FILENAME = 384;
const IDENTIFIER = 385;
const DT_LITERAL = 386;
const TOD_LITERAL = 387;
const INT_LITERAL = 388;
const DATE_LITERAL = 389;
const TOKEN_TRUE = 390;
const TOKEN_FALSE = 391;
const PAREN_OPEN = 392;
const PAREN_CLOSE = 393;
const SQUARED_BRACKET_OPEN = 394;
const SQUARED_BRACKET_CLOSE = 395;
const CURLY_BRACES_OPEN = 396;
const CURLY_BRACES_CLOSE = 397;
const TIME_LITERAL = 398;
const BOOL_LITERAL = 399;
const FLOAT_LITERAL = 400;
const STRING_LITERAL = 401;
const ARRAY = 402;
const OF = 403;
const ANY = 404;
const ANY_NUM = 405;
const ANY_REAL = 406;
const ANY_INT = 407;
const ANY_BIT = 408;
const ANY_STRING = 409;
const ANY_DATE = 410;
const ANY_ELEMENTARY = 411;
const ANY_MAGNITUDE = 412;
const ANY_DERIVED = 413;
const DIRECT_VARIABLE = 414;
const RETAIN = 415;
const NON_RETAIN = 416;
const CONSTANT = 417;
const IMPORT_DIRECTIVE = 418;
const PRAGMA_DIRECTIVE = 419;
extern YYSTYPE yylval;

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
// yy_parse => class defining a parsing object
//	yy_parse needs a class yy_scan, which defines the scanner.
// %prefix or option -p xx determines name of this class; if not used,
// defaults to 'yy_scan'
//
// constructor fills in the tables for this grammar; give it a size
//    to determine size of state and value stacks. Default is 150 entries.
// destructor discards those state and value stacks
//
// int yy_parse::yyparse(yy_scan *) invokes parse; if this returns,
//	it can be recalled to continue parsing at last point.
// void yy_parse::yyreset() can be called to reset the parse;
//	call yyreset() before yy_parse::yyparse(yy_scan *)
#include <stdio.h>		// uses printf(), et cetera
#include <stdlib.h>		// uses exit()

#include "Global.h"

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

#ifdef YACC_WINDOWS

// include all windows prototypes, macros, constants, etc.

#include <windows.h>

// the following is the handle to the current
// instance of a windows program. The user
// program calling yyparse must supply this!

#ifdef STRICT
extern HINSTANCE hInst;	
#else
extern HANDLE hInst;	
#endif

#endif	/* YACC_WINDOWS */

//=== INSERTED
#include "ParserContext.h"
//=== END OF INSERTED

class yy_parse
//=== INSERTED
 : public ParserContext
//=== END OF INSERTED

{
protected:

#ifdef YACC_WINDOWS

	// protected member function for actual scanning 

	int	win_yyparse(yy_scan * ps);	// parse with given scanner

#endif	/* YACC_WINDOWS */

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
	yy_scan* scan;			// pointer to scanner
	int	yydebug;	// if set, tracing if compiled with YYDEBUG=1

	BACKPARSERDLLEXPORT yy_parse(int = 150);	// constructor for this grammar
	yy_parse(int, short *, YYSTYPE *);	// another constructor

	BACKPARSERDLLEXPORT ~yy_parse();		// destructor

	BACKPARSERDLLEXPORT int	yyparse(yy_scan * ps);	// parse with given scanner

	void	yyreset() { reset = 1; } // restore state for next yyparse()

	BACKPARSERDLLEXPORT void	setdebug(int y) 
    { 
        //=== INSERTED
        AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
        //=== END OF INSERTED

        yydebug = y; 
    }

// The following are useful in user actions:

	void	yyerrok() { yyerrflag = 0; }	// clear error
	void	yyclearin() { yychar = -1; }	// clear input
	int	YYRECOVERING() { return yyerrflag != 0; }
};
// end of .hpp header
