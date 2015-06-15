/* lex -alTv -LC -o patchver.cpp -D patchver.h patchver.l */
#define INITIAL 0
const yy_endst = 77;
const yy_nxtmax = 329;
#define YY_LA_SIZE 10

static unsigned int yy_la_act[] = {
 6, 6, 5, 4, 3, 2, 1, 0, 0
};

static unsigned char yy_look[] = {
 0
};

static int yy_final[] = {
 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4,
 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5,
 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6,
 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 8
};
#ifndef yy_state_t
#define yy_state_t unsigned char
#endif

static yy_state_t yy_begin[] = {
 0, 0, 0
};

static yy_state_t yy_next[] = {
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 77, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 3, 8, 9, 10, 5, 12, 11, 13, 14, 15, 16, 17, 18, 19, 7, 20,
 21, 6, 22, 23, 4, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66,
 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 0
};

static yy_state_t yy_check[] = {
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ~0U, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 1, 3, 8, 9, 3, 11, 9, 12, 13, 14, 15, 16, 17, 18, 3, 10,
 20, 3, 21, 22, 3, 23, 24, 25, 26, 27, 28, 7, 30, 31, 32, 33,
 34, 35, 36, 37, 38, 39, 40, 6, 42, 43, 44, 45, 46, 47, 48, 49,
 50, 51, 52, 53, 54, 5, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
 66, 67, 4, 69, 70, 71, 72, 73, 74, 75, 0
};

static yy_state_t yy_default[] = {
 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 0
};

static int yy_base[] = {
 0, 221, 330, 190, 253, 224, 223, 201, 179, 182, 191, 172, 181, 191, 194, 194,
 183, 233, 234, 330, 207, 196, 186, 199, 213, 202, 211, 246, 247, 330, 205, 217,
 201, 220, 204, 211, 225, 214, 223, 258, 259, 330, 217, 215, 214, 204, 214, 232,
 220, 220, 231, 226, 228, 272, 273, 330, 234, 235, 217, 227, 245, 233, 233, 244,
 239, 241, 285, 286, 330, 241, 241, 252, 247, 249, 293, 294, 330, 330
};



// MKS LEX prototype scanner code
// Copyright 1991 by Mortice Kern Systems Inc.
// All rights reserved.

// You can redefine YY_INTERACTIVE to be 0 to get a very slightly
// faster scanner:
#ifndef YY_INTERACTIVE
#define	YY_INTERACTIVE	1
#endif

// You can compile with -DYY_DEBUG to get a print trace of the scanner
#ifdef YY_DEBUG
#undef YY_DEBUG
#define YY_DEBUG(fmt,a1,a2)	fprintf(stderr,fmt,a1,a2)
#else
#define YY_DEBUG(fmt,a1,a2)
#endif

const MIN_NUM_STATES = 20;

// Do *NOT* redefine the following:
#define	BEGIN		yy_start =
#define	REJECT		goto yy_reject
#define	yymore()	goto yy_more


#pragma warning(disable: 4068 4102)
#include "patchver.h"
#include "BuildNr.h"
#include "Product.h"


// Constructor for yy_scan. Set up tables
#pragma argsused
yy_scan::yy_scan(int sz, char* buf, char* sv, yy_state_t* states)
{
	mustfree = 0;
	if ((size = sz) < MIN_NUM_STATES
	  || (yytext = buf) == 0
	  || (state = states) == 0) {
		yyerror("Bad space for scanner!");
		exit(1);
	}
#ifdef YY_PRESERVE
	save = sv;
#endif
}
// Constructor for yy_scan. Set up tables
yy_scan::yy_scan(int sz)
{
	size = sz;
	yytext = new char[sz+1];	// text buffer
	state = new yy_state_t[sz+1];	// state buffer
#ifdef YY_PRESERVE
	save = new char[sz];	// saved yytext[]
	push = save + sz;
#endif
	if (yytext == NULL
#ifdef YY_PRESERVE
	  || save == NULL
#endif
	  || state == NULL) {
		yyerror("No space for scanner!");
		exit(1);
	}
	mustfree = 1;
	yy_end = 0;
	yy_start = 0;
	yy_lastc = YYNEWLINE;
	yyin = stdin;
	yyout = stdout;
	yylineno = 1;
	yyleng = 0;
}

// Descructor for yy_scan
yy_scan::~yy_scan()
{
	if (mustfree) {
		mustfree = 0;
		delete(yytext);
		delete(state);
#ifdef YY_PRESERVE
		delete(save);
#endif
	}
}

// Print error message, showing current line number
void
yy_scan::yyerror(char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
#ifdef LEX_WINDOWS
	// Windows has no concept of a standard error output!
	// send output to yyout as a simple solution
	if (yylineno)
		fprintf(yyout, "%d: ", yylineno);
	(void) vfprintf(yyout, fmt, va);
	fputc('\n', yyout);
#else /* LEX_WINDOWS */
	if (yylineno)
		fprintf(stderr, "%d: ", yylineno);
	(void) vfprintf(stderr, fmt, va);
	fputc('\n', stderr);
#endif /* LEX_WINDOWS */
	va_end(va);
}


#ifdef LEX_WINDOWS

// The initial portion of the lex scanner
// In an windows environment, it will load the desired
// resources, obtain pointers to them, and then call
// the protected member win_yylex() to acutally begin the
// scanning. When complete, win_yylex() will return a
// value back to our new yylex() function, which will 
// record that value temporarily, release the resources
// from global memory, and finally return the value
// back to the caller of yylex().

int
yy_scan::yylex()
{
	int wReturnValue;
	HANDLE hRes_table;
	unsigned short far *old_yy_la_act;	// remember old pointer values
	short far *old_yy_final;
	yy_state_t far *old_yy_begin;
	yy_state_t far *old_yy_next;
	yy_state_t far *old_yy_check;
	yy_state_t far *old_yy_default;
	short far *old_yy_base;

	// the following code will load the required
	// resources for a Windows based parser. 

	hRes_table = LoadResource (hInst,
		FindResource (hInst, "UD_RES_yyLEX", "yyLEXTBL"));
	
	// return an error code if any
	// of the resources did not load 

	if (hRes_table == (HANDLE)NULL) 
		return (0);
	
	// the following code will lock the resources
	// into fixed memory locations for the scanner
	// (remember previous pointer locations)

	old_yy_la_act = yy_la_act;
	old_yy_final = yy_final;
	old_yy_begin = yy_begin;
	old_yy_next = yy_next;
	old_yy_check = yy_check;
	old_yy_default = yy_default;
	old_yy_base = yy_base;

	yy_la_act = (unsigned short far *)LockResource (hRes_table);
	yy_final = (short far *)(yy_la_act + Sizeof_yy_la_act);
	yy_begin = (yy_state_t far *)(yy_final + Sizeof_yy_final);
	yy_next = (yy_state_t far *)(yy_begin + Sizeof_yy_begin);
	yy_check = (yy_state_t far *)(yy_next + Sizeof_yy_next);
	yy_default = (yy_state_t far *)(yy_check + Sizeof_yy_check);
	yy_base = (short far *)(yy_default + Sizeof_yy_default);


	// call the standard yylex() code

	wReturnValue = win_yylex();

	// unlock the resources

	UnlockResource (hRes_table);

	// and now free the resource

	FreeResource (hRes_table);

	//
	// restore previously saved pointers
	//

	yy_la_act = old_yy_la_act;
	yy_final = old_yy_final;
	yy_begin = old_yy_begin;
	yy_next = old_yy_next;
	yy_check = old_yy_check;
	yy_default = old_yy_default;
	yy_base = old_yy_base;

	return (wReturnValue);
}	// end yylex()

// The actual lex scanner
// yy_sbuf[0:yyleng-1] contains the states corresponding to yytext.
// yytext[0:yyleng-1] contains the current token.
// yytext[yyleng:yy_end-1] contains pushed-back characters.
// When the user action routine is active,
// save contains yytext[yyleng], which is set to '\0'.
// Things are different when YY_PRESERVE is defined. 

int 
yy_scan::win_yylex()

#else /* LEX_WINDOWS */

// The actual lex scanner
// yy_sbuf[0:yyleng-1] contains the states corresponding to yytext.
// yytext[0:yyleng-1] contains the current token.
// yytext[yyleng:yy_end-1] contains pushed-back characters.
// When the user action routine is active,
// save contains yytext[yyleng], which is set to '\0'.
// Things are different when YY_PRESERVE is defined. 
int
yy_scan::yylex()
#endif /* LEX_WINDOWS */

{
	int c, i, yybase;
	unsigned  yyst;		/* state */
	int yyfmin, yyfmax;	/* yy_la_act indices of final states */
	int yyoldi, yyoleng;	/* base i, yyleng before look-ahead */
	int yyeof;		/* 1 if eof has already been read */



#ifdef YYEXIT
	yyLexFatal = 0;
#endif
	yyeof = 0;
	i = yyleng;
	YY_SCANNER();

  yy_again:
	if ((yyleng = i) > 0) {
		yy_lastc = yytext[i-1];	// determine previous char
		while (i > 0)	//	// scan previously token
			if (yytext[--i] == YYNEWLINE)	// fix yylineno
				yylineno++;
	}
	yy_end -= yyleng;		// adjust pushback
	if (yy_end > 0)
		memmove(yytext, yytext+yyleng, (size_t) yy_end);
	i = 0;

  yy_contin:
	yyoldi = i;

	/* run the state machine until it jams */
	yyst = yy_begin[yy_start + ((yy_lastc == YYNEWLINE) ? 1 : 0)];
	state[i] = (yy_state_t) yyst;
	do {
		YY_DEBUG("<state %d, i = %d>\n", yyst, i);
		if (i >= size) {
			YY_FATAL("Token buffer overflow");
#ifdef YYEXIT
			if (yyLexFatal)
				return -2;
#endif
		}	/* endif */

		/* get input char */
		if (i < yy_end)
			c = yytext[i];		/* get pushback char */
		else if (!yyeof && (c = yygetc()) != EOF) {
			yy_end = i+1;
			yytext[i] = c;
		} else /* c == EOF */ {
			c = EOF;		/* just to make sure... */
			if (i == yyoldi) {	/* no token */
				yyeof = 0;
				if (yywrap())
					return 0;
				else
					goto yy_again;
			} else {
				yyeof = 1;	/* don't re-read EOF */
				break;
			}
		}
		YY_DEBUG("<input %d = 0x%02x>\n", c, c);

		/* look up next state */
		while ((yybase = yy_base[yyst]+(unsigned char)c) > yy_nxtmax
		    || yy_check[yybase] != (yy_state_t) yyst) {
			if (yyst == yy_endst)
				goto yy_jammed;
			yyst = yy_default[yyst];
		}
		yyst = yy_next[yybase];
	  yy_jammed: ;
	     state[++i] = (yy_state_t) yyst;
	} while (!(yyst == yy_endst || YY_INTERACTIVE &&
		yy_base[yyst] > yy_nxtmax && yy_default[yyst] == yy_endst));

	YY_DEBUG("<stopped %d, i = %d>\n", yyst, i);
	if (yyst != yy_endst)
		++i;

  yy_search:
	/* search backward for a final state */
	while (--i > yyoldi) {
		yyst = state[i];
		if ((yyfmin = yy_final[yyst]) < (yyfmax = yy_final[yyst+1]))
			goto yy_found;	/* found final state(s) */
	}
	/* no match, default action */
	i = yyoldi + 1;
	output(yytext[yyoldi]);
	goto yy_again;

  yy_found:
	YY_DEBUG("<final state %d, i = %d>\n", yyst, i);
	yyoleng = i;		/* save length for REJECT */
	
	// pushback look-ahead RHS, handling trailing context
	if ((c = (int)(yy_la_act[yyfmin]>>9) - 1) >= 0) {
		unsigned char *bv = yy_look + c*YY_LA_SIZE;
		static unsigned char bits [8] = {
			1<<0, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1<<7
		};
		while (1) {
			if (--i < yyoldi) {	/* no / */
				i = yyoleng;
				break;
			}
			yyst = state[i];
			if (bv[(unsigned)yyst/8] & bits[(unsigned)yyst%8])
				break;
		}
	}

	/* perform action */
	yyleng = i;
	YY_USER();
	switch (yy_la_act[yyfmin] & 0777) {
	case 0:
	{ fputs(_MK_STR(PROD_VER_MAJOR)","_MK_STR(PROD_VER_MINOR)","_MK_STR(PROD_VER_SERVP)","_MK_STR(PROD_VER_BUILD), yyout); }
	break;
	case 1:
	{ fputs(PROD_FULL_VERSION_STR, yyout); }
	break;
	case 2:
	{ fputs(PROD_SHORT_VERSION_STR, yyout); }
	break;
	case 3:
	{ fputs(PROD_PRODUCTNAME, yyout); }
	break;
	case 4:
	{ fputs(PROD_COMPANYNAME, yyout); }
	break;
	case 5:
	{ fputs(PROD_COPYRIGHT_SHORT, yyout); }
	break;
	case 6:
	{ fputs(yytext, yyout); }
	break;


	}
	YY_SCANNER();
	i = yyleng;
	goto yy_again;			/* action fell though */

  yy_reject:
	YY_SCANNER();
	i = yyoleng;			/* restore original yytext */
	if (++yyfmin < yyfmax)
		goto yy_found;		/* another final state, same length */
	else
		goto yy_search;		/* try shorter yytext */

  yy_more:
	YY_SCANNER();
	i = yyleng;
	if (i > 0)
		yy_lastc = yytext[i-1];
	goto yy_contin;
}

/*
 * user callable input/unput functions.
 */
void
yy_scan::yy_reset()
{
	YY_INIT();
	yylineno = 1;
}
/* get input char with pushback */
int
yy_scan::input()
{
	int c;
#ifndef YY_PRESERVE
	if (yy_end > yyleng) {
		yy_end--;
		memmove(yytext+yyleng, yytext+yyleng+1,
			(size_t) (yy_end-yyleng));
		c = save;
		YY_USER();
#else
	if (push < save+size) {
		c = *push++;
#endif
	} else
		c = yygetc();
	yy_lastc = c;
	if (c == YYNEWLINE)
		yylineno++;
	return c;
}

/* pushback char */
int
yy_scan::unput(int c)
{
#ifndef YY_PRESERVE
	if (yy_end >= size) {
		YY_FATAL("Push-back buffer overflow");
	} else {
		if (yy_end > yyleng) {
			yytext[yyleng] = save;
			memmove(yytext+yyleng+1, yytext+yyleng,
				(size_t) (yy_end-yyleng));
			yytext[yyleng] = 0;
		}
		yy_end++;
		save = c;
#else
	if (push <= save) {
		YY_FATAL("Push-back buffer overflow");
	} else {
		*--push = c;
#endif
		if (c == YYNEWLINE)
			yylineno--;
	}	/* endif */
	return c;
}


int main(int argc, char **argv)
{
  if (argc!=3)
    {
      fprintf(stderr,"error: invalid arguments\n");
      fprintf(stderr,"usage: patchver [input] [output]\n");
      return 1;
    }
  FILE *infile = fopen(argv[1], "r");
  if (infile == NULL)
    {
      fprintf(stderr,"error: cannot open '%s'\n", argv[1]);
      return 1;
    }
  FILE *outfile = fopen(argv[2], "w");
  if (outfile == NULL)
    {
      fprintf(stderr,"error: cannot create '%s'\n", argv[2]);
      return 1;
    }

  printf("patchver: version=%s\n",PROD_FULL_VERSION_STR);

  yy_scan scanner;
  scanner.setinput(infile);
  scanner.setoutput(outfile);
  scanner.yylex();
  scanner.setinput(NULL);
  scanner.setoutput(NULL);
  fclose(infile);
  fclose(outfile);
  return 0;
}
