typedef union {
	LONG            ulVal;
    BOOL            bVal;
    int             iToken;
    TCHAR          *pszText;
    CString        *pString;
    CByteArray     *pOctets;
    CPrmTextItem   *pTextItem;
    CPrmTextDef    *pTextDef;
    CExtParamDefinition *pParamDef;
    GSDDATATYPE		dType;
    listEnumValues *pEnumValues;
} YYSTYPE;
const _KEYWORD = 257;
const _PROFIBUS_DP = 258;
const _INTEGER_LITERAL = 259;
const CRLF = 260;
const _COMMENT = 261;
const _VISIBLE_STRING = 262;
const _NAME_CHAR = 263;
const _OTHER_CHAR = 264;
const _SUB_FAMILY = 265;
const _GSD_REVISION = 266;
const _VENDOR_NAME = 267;
const _MODEL_NAME = 268;
const _REVISION = 269;
const _REVISION_NUMBER = 270;
const _IDENT_NUMBER = 271;
const _PROTOCOL_IDENT = 272;
const _STATION_TYPE = 273;
const _FMS_SUPP = 274;
const _HW_RELEASE = 275;
const _SW_RELEASE = 276;
const _PHYSICAL_LAYER = 277;
const _9_6_SUPP = 278;
const _19_2_SUPP = 279;
const _93_75_SUPP = 280;
const _187_5_SUPP = 281;
const _500_SUPP = 282;
const _1_5M_SUPP = 283;
const _3M_SUPP = 284;
const _6M_SUPP = 285;
const _12M_SUPP = 286;
const _31_25_SUPP = 287;
const _45_45_SUPP = 288;
const _MAXTSDR_9_6 = 289;
const _MAXTSDR_19_2 = 290;
const _MAXTSDR_93_75 = 291;
const _MAXTSDR_187_5 = 292;
const _MAXTSDR_500 = 293;
const _MAXTSDR_1_5M = 294;
const _MAXTSDR_3M = 295;
const _MAXTSDR_6M = 296;
const _MAXTSDR_12M = 297;
const _MAXTSDR_31_25 = 298;
const _MAXTSDR_45_45 = 299;
const _REDUNDANCY = 300;
const _REPEATER_CTRL_SIG = 301;
const _24V_PINS = 302;
const _IMPL_TYPE = 303;
const _BMP_DEVICE = 304;
const _BMP_DIAG = 305;
const _BMP_SF = 306;
const _DOWNLOAD_SUPP = 307;
const _UPLOAD_SUPP = 308;
const _ACT_PARA_BRCT_SUPP = 309;
const _ACT_PARAM_SUPP = 310;
const _MAX_MPS_LENGTH = 311;
const _MAX_LSDU_MS = 312;
const _MAX_LSDU_MM = 313;
const _MIN_POLL_TIMEOUT = 314;
const _DPV1_MASTER = 315;
const _TRDY_9_6 = 316;
const _TRDY_19_2 = 317;
const _TRDY_93_75 = 318;
const _TRDY_187_5 = 319;
const _TRDY_500 = 320;
const _TRDY_1_5M = 321;
const _TRDY_3M = 322;
const _TRDY_6M = 323;
const _TRDY_12M = 324;
const _TRDY_31_25 = 325;
const _TRDY_45_45 = 326;
const _TQUI_9_6 = 327;
const _TQUI_19_2 = 328;
const _TQUI_93_75 = 329;
const _TQUI_187_5 = 330;
const _TQUI_500 = 331;
const _TQUI_1_5M = 332;
const _TQUI_3M = 333;
const _TQUI_6M = 334;
const _TQUI_12M = 335;
const _TQUI_31_25 = 336;
const _TQUI_45_45 = 337;
const _TSET_9_6 = 338;
const _TSET_19_2 = 339;
const _TSET_93_75 = 340;
const _TSET_187_5 = 341;
const _TSET_500 = 342;
const _TSET_1_5M = 343;
const _TSET_3M = 344;
const _TSET_6M = 345;
const _TSET_12M = 346;
const _TSET_31_25 = 347;
const _TSET_45_45 = 348;
const _LAS_LEN = 349;
const _TSDI_9_6 = 350;
const _TSDI_19_2 = 351;
const _TSDI_93_75 = 352;
const _TSDI_187_5 = 353;
const _TSDI_500 = 354;
const _TSDI_1_5M = 355;
const _TSDI_3M = 356;
const _TSDI_6M = 357;
const _TSDI_12M = 358;
const _TSDI_31_25 = 359;
const _TSDI_45_45 = 360;
const _MAX_SLAVES_SUPP = 361;
const _MAX_MASTER_INP_LEN = 362;
const _MAX_MASTER_OUT_LEN = 363;
const _MAX_MASTER_DATA_LEN = 364;
const _FREEZE_MODE_SUPP = 365;
const _SYNC_MODE_SUPP = 366;
const _AUTO_BAUD_SUPP = 367;
const _SET_SLAVE_ADD_SUPP = 368;
const _USER_PRM_DATA_LEN = 369;
const _USER_PRM_DATA = 370;
const _USER_PRM_DATA_DEF = 371;
const _MIN_SLAVE_INTERVALL = 372;
const _MODULAR_STATION = 373;
const _MAX_MODULE = 374;
const _MAX_INPUT_LEN = 375;
const _MAX_OUTPUT_LEN = 376;
const _MAX_DATA_LEN = 377;
const _UNIT_DIAG_BIT = 378;
const _UNIT_DIAG_AREA = 379;
const _UNIT_DIAG_AREA_END = 380;
const _CHANNEL_DIAG = 381;
const _VALUE = 382;
const _MODULE = 383;
const _END_MODULE = 384;
const _MODUL_OFFSET = 385;
const _FAIL_SAFE = 386;
const _MAX_DIAG_DATA_LEN = 387;
const _SLAVE_FAMILY = 388;
const _MAX_USER_PRM_DATA_LEN = 389;
const _EXT_USR_PRM_DATA_REF = 390;
const _EXT_USR_PRM_DATA_CONST = 391;
const _EXT_MOD_PRM_DATA_LEN = 392;
const _EXT_USR_PRM_DATA = 393;
const _END_EXT_USR_PRM_DATA = 394;
const _PRM_TEXT_REF = 395;
const _PRM_TEXT = 396;
const _END_PRM_TEXT = 397;
const _TEXT = 398;
const _TYPE_UNSIGNED8 = 399;
const _TYPE_UNSIGNED16 = 400;
const _TYPE_UNSIGNED32 = 401;
const _TYPE_SIGNED8 = 402;
const _TYPE_SIGNED16 = 403;
const _TYPE_SIGNED32 = 404;
const _TYPE_BIT = 405;
const _TYPE_BIT_AREA = 406;
const _DPV1_SLAVE = 407;
const _C1_RW_SUPP = 408;
const _C2_RW_SUPP = 409;
const _C1_MAX_DATA_LEN = 410;
const _C2_MAX_DATA_LEN = 411;
const _C1_RESPONSE_TO = 412;
const _C2_RESPONSE_TO = 413;
const _C1_RW_REQ = 414;
const _C2_RW_REQ = 415;
const _C2_MAX_CNT_CHANNELS = 416;
const _MAX_INIT_PDU_LEN = 417;
const _DPV1_DATA_TYPES = 418;
const _WD_BASE_1MS_SUPP = 419;
const _CHECK_CFG_MODE = 420;
const _ALARM_SUPP = 421;
const _DIAGNOSTIC_ALARM_SUPP = 422;
const _PROCESS_ALARM_SUPP = 423;
const _PULL_PLUG_ALARM_SUPP = 424;
const _STATUS_ALARM_SUPP = 425;
const _UPDATE_ALARM_SUPP = 426;
const _MAN_SPEC_ALARM_SUPP = 427;
const _EXTRA_ALARM_SAP_SUPP = 428;
const _ALARM_SEQ_MODE_CNT = 429;
const _ALARM_TYPE_MODE_SUPP = 430;
const _FAIL_SAVE_REQUIRED = 431;
const _DIAGNOSTIC_ALARM_RQ = 432;
const _PROCESS_ALARM_RQ = 433;
const _PULL_PLUG_ALARM_RQ = 434;
const _STATUS_ALARM_RQ = 435;
const _UPDATE_ALARM_RQ = 436;
const _MAN_SPEC_ALARM_RQ = 437;
const _MASTER_SYNC_MODE_SUPP = 438;
const _MASTER_FAIL_SAFE_SUPP = 439;
const _DPV1_CONFORMANCE_CLASS = 440;
const _C1_MASTER_RW_SUPP = 441;
const _M_DPV1_ALARM_SUPP = 442;
const _MASTER_FREEZE_MODE_SUPP = 443;
const _M_DIAGNOSTIC_ALARM_SUPP = 444;
const _M_PROCESS_ALARM_SUPP = 445;
const _M_PULL_PLUG_ALARM_SUPP = 446;
const _M_STATUS_ALARM_SUPP = 447;
const _M_UPDATE_ALARM_SUPP = 448;
const _M_MAN_SPEC_ALARM_SUPP = 449;
const _M_EXTRA_ALARM_SAP_SUPP = 450;
const _M_ALARM_SEQ_MODE = 451;
const _M_ALARM_TYPE_MODE_SUPP = 452;
const _PHYSICAL_IF = 453;
const _END_PHYSICAL_IF = 454;
const _TX_DEL_9_6 = 455;
const _TX_DEL_19_2 = 456;
const _TX_DEL_31_25 = 457;
const _TX_DEL_45_45 = 458;
const _TX_DEL_93_75 = 459;
const _TX_DEL_187_5 = 460;
const _TX_DEL_500 = 461;
const _TX_DEL_1_5M = 462;
const _TX_DEL_3M = 463;
const _TX_DEL_6M = 464;
const _TX_DEL_12M = 465;
const _RC_DEL_9_6 = 466;
const _RC_DEL_19_2 = 467;
const _RC_DEL_31_25 = 468;
const _RC_DEL_45_45 = 469;
const _RC_DEL_93_75 = 470;
const _RC_DEL_187_5 = 471;
const _RC_DEL_500 = 472;
const _RC_DEL_1_5M = 473;
const _RC_DEL_3M = 474;
const _RC_DEL_6M = 475;
const _RC_DEL_12M = 476;
const _DIAG_UPDATE_DELAY = 477;
const _INFO_TEXT = 478;
const _SLOT_DEF = 479;
const _END_SLOT_DEF = 480;
const _SLOT = 481;
const _USER_KEYWORD = 482;
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


class yy_parse {
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

	yy_parse(int = 150);	// constructor for this grammar
	yy_parse(int, short *, YYSTYPE *);	// another constructor

	~yy_parse();		// destructor

	int	yyparse(yy_scan * ps);	// parse with given scanner

	void	yyreset() { reset = 1; } // restore state for next yyparse()

	void	setdebug(int y) { yydebug = y; }

// The following are useful in user actions:

	void	yyerrok() { yyerrflag = 0; }	// clear error
	void	yyclearin() { yychar = -1; }	// clear input
	int	YYRECOVERING() { return yyerrflag != 0; }
};
// end of .hpp header
