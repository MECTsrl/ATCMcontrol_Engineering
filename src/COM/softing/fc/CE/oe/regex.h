
////////////////////////////////////////////////////////////////////////
// RegExp.h
//
// This code has been derived from work by Henry Spencer. 
// The main changes are
// 1. All char variables and functions have been changed to char
//    counterparts
// 2. Added GetFindLen() & GetReplaceString() to enable search
//    and replace operations.
// 3. And of course, added the C++ Wrapper
//
// The original copyright notice follows:
//
// Copyright (c) 1986, 1993, 1995 by University of Toronto.
// Written by Henry Spencer.  Not derived from licensed software.
//
// Permission is granted to anyone to use this software for any
// purpose on any computer system, and to redistribute it in any way,
// subject to the following restrictions:
//
// 1. The author is not responsible for the consequences of use of
// this software, no matter how awful, even if they arise
// from defects in it.
//
// 2. The origin of this software must not be misrepresented, either
// by explicit claim or by omission.
//
// 3. Altered versions must be plainly marked as such, and must not
// be misrepresented (by explicit claim or omission) as being
// the original software.
//
// 4. This notice must not be removed or altered.
/////////////////////////////////////////////////////////////////////////////

// Inclusion Guards
#ifndef __REGEX_H__
#define __REGEX_H__

// OE Extension DLL
// Initialize declaration context
#ifdef _OEDLL
	#undef AFX_DATA
	#define AFX_DATA OE_DATAEXT
#endif // _OEDLL


#define NSUBEXP  10


class CRegExp
{
public:
   CRegExp();
   ~CRegExp();

   CRegExp *RegComp( const char *re );
   int RegFind(const char *str);
   char* GetReplaceString( const char* sReplaceExp );
   int GetFindLen()
   {
      if( startp[0] == NULL || endp[0] == NULL )
         return 0;

      return endp[0] - startp[0];
   }


private:
   char *regnext(char *node);
   void reginsert(char op, char *opnd);

   int regtry(char *string);
   int regmatch(char *prog);
   size_t regrepeat(char *node);
   char *reg(int paren, int *flagp);
   char *regbranch(int *flagp);
   void regtail(char *p, char *val);
   void regoptail(char *p, char *val);
   char *regpiece(int *flagp);
   char *regatom(int *flagp);

   // Inline functions
private:
   char OP(char *p) {return *p;};

   char *OPERAND( char *p) {return (char*)((short *)(p+1)+1); };

   // regc - emit (if appropriate) a byte of code
   void regc(char b)
   {
      if (bEmitCode)
         *regcode++ = b;
      else
         regsize++;
   };

   // regnode - emit a node
   char *  regnode(char op)
   {
      if (!bEmitCode) {
         regsize += 3;
         return regcode;
      }

      *regcode++ = op;
      *regcode++ = ('\0');     /* Null next pointer. */
      *regcode++ = ('\0');

      return regcode-3;
   };


private:
   BOOL bEmitCode;
   BOOL bCompiled;
   char *sFoundText;

   char *startp[NSUBEXP];
   char *endp[NSUBEXP];
   char regstart;      // Internal use only. 
   char reganch;    // Internal use only. 
   char *regmust;      // Internal use only. 
   int regmlen;      // Internal use only. 
   char *program;      // Unwarranted chumminess with compiler. 

   char *regparse;  // Input-scan pointer. 
   int regnpar;      // () count. 
   char *regcode;      // Code-emit pointer; ®dummy = don't. 
   char regdummy[3];   // NOTHING, 0 next ptr 
   long regsize;     // Code size. 

   char *reginput;  // String-input pointer. 
   char *regbol;    // Beginning of input, for ^ check. 
};

// OE Extension DLL
// Reset declaration context
#undef AFX_DATA
#define AFX_DATA

#endif __REGEX_H__ // #ifndef __REGEX_H__

