////////////////////////////////////////////////////////////////////////////////
// RegExp.cpp
////////////////////////////////////////////////////////////////////////////////

#include "RegEx.h"


// definition  number   opnd? meaning 
#define  REGEX_END      0     // no End of program. 
#define  REGEX_BOL      1     // no Match beginning of line. 
#define  REGEX_EOL      2     // no Match end of line. 
#define  REGEX_ANY      3     // no Match any character. 
#define  REGEX_ANYOF 4     // str   Match any of these. 
#define  REGEX_ANYBUT   5     // str   Match any but one of these. 
#define  REGEX_BRANCH   6     // node  Match this, or the next..\&. 
#define  REGEX_BACK  7     // no "next" ptr points backward. 
#define  REGEX_EXACTLY  8     // str   Match this string. 
#define  REGEX_NOTHING  9     // no Match empty string. 
#define  REGEX_STAR  10    // node  Match this 0 or more times. 
#define  REGEX_PLUS  11    // node  Match this 1 or more times. 
#define  REGEX_OPEN  20    // no Sub-RE starts here. 
                  //    REGEX_OPEN+1 is number 1, etc. 
#define  REGEX_CLOSE 30    // no Analogous to REGEX_OPEN. 

// Utility definitions.
 
#define  REGEX_FAIL(m)     { regerror(m); return(NULL); }
#define  REGEX_ISREPN(c)   ((c) == ('*') || (c) == ('+') || (c) == ('?'))
#define  REGEX_META     "^$.[()|?+*\\"

// Flags to be passed up and down.
 
#define  REGEX_HASWIDTH 01 // Known never to match null string. 
#define  REGEX_SIMPLE      02 // Simple enough to be REGEX_STAR/REGEX_PLUS operand. 
#define  REGEX_SPSTART     04 // Starts with * or +. 
#define  REGEX_WORST    0  // Worst case. 


CRegExp::CRegExp()
{
   bCompiled = FALSE;
   program = NULL;
   sFoundText = NULL;

   for( int i = 0; i < NSUBEXP; i++ )
   {
      startp[i] = NULL;
      endp[i] = NULL;
   }
}

CRegExp::~CRegExp()
{
   delete program;
   delete sFoundText;
}


CRegExp* CRegExp::RegComp(const char *exp)
{
   char *scan;
   int flags;

   if (exp == NULL)
      return NULL;
   
   bCompiled = TRUE;

   // First pass: determine size, legality. 
   bEmitCode = FALSE;
   regparse = (char *)exp;
   regnpar = 1;
   regsize = 0L;
   regdummy[0] = REGEX_NOTHING;
   regdummy[1] = regdummy[2] = 0;
   regcode = regdummy;
   if (reg(0, &flags) == NULL)
      return(NULL);

   // Allocate space. 
   delete program;
   program = new char[regsize];
   memset( program, 0, regsize * sizeof(char) );

   if (program == NULL)
      return NULL;

   // Second pass: emit code. 
   bEmitCode = TRUE;
   regparse = (char *)exp;
   regnpar = 1;
   regcode = program;
   if (reg(0, &flags) == NULL)
      return NULL;

   // Dig out information for optimizations. 
   regstart = ('\0');    // Worst-case defaults. 
   reganch = 0;
   regmust = NULL;
   regmlen = 0;
   scan = program;      // First REGEX_BRANCH. 
   if (OP(regnext(scan)) == REGEX_END) 
   {  
      // Only one top-level choice. 
      scan = OPERAND(scan);

      // Starting-point info. 
      if (OP(scan) == REGEX_EXACTLY)
         regstart = *OPERAND(scan);
      else if (OP(scan) == REGEX_BOL)
         reganch = 1;

      // If there's something expensive in the r.e., find the
      // longest literal string that must appear and make it the
      // regmust.  Resolve ties in favor of later strings, since
      // the regstart check works with the beginning of the r.e.
      // and avoiding duplication strengthens checking.  Not a
      // strong reason, but sufficient in the absence of others.
       
      if (flags&REGEX_SPSTART) 
      {
         char *longest = NULL;
         size_t len = 0;

         for (; scan != NULL; scan = regnext(scan))
            if (OP(scan) == REGEX_EXACTLY && strlen(OPERAND(scan)) >= len) 
            {
               longest = OPERAND(scan);
               len = strlen(OPERAND(scan));
            }
         regmust = longest;
         regmlen = (int)len;
      }
   }

   return this;
}

// reg - regular expression, i.e. main body or parenthesized thing
//
// Caller must absorb opening parenthesis.
//
// Combining parenthesis handling with the base level of regular expression
// is a trifle forced, but the need to tie the tails of the branches to what
// follows makes it hard to avoid.
 


char *CRegExp::reg(int paren, int *flagp)
{
   char *ret;
   char *br;
   char *ender;
   int parno;
   int flags;

   *flagp = REGEX_HASWIDTH;   // Tentatively. 

   if (paren) 
   {
      // Make an REGEX_OPEN node. 
      if (regnpar >= NSUBEXP)
      {
         TRACE1("Too many (). NSUBEXP is set to %d\n", NSUBEXP );
         return NULL;
      }
      parno = regnpar;
      regnpar++;
      ret = regnode((char)(REGEX_OPEN+parno));
   }

   // Pick up the branches, linking them together. 
   br = regbranch(&flags);
   if (br == NULL)
      return(NULL);
   if (paren)
      regtail(ret, br); // REGEX_OPEN -> first. 
   else
      ret = br;
   *flagp &= ~(~flags&REGEX_HASWIDTH); // Clear bit if bit 0. 
   *flagp |= flags&REGEX_SPSTART;
   while (*regparse == ('|')) {
      regparse++;
      br = regbranch(&flags);
      if (br == NULL)
         return(NULL);
      regtail(ret, br); // REGEX_BRANCH -> REGEX_BRANCH. 
      *flagp &= ~(~flags&REGEX_HASWIDTH);
      *flagp |= flags&REGEX_SPSTART;
   }

   // Make a closing node, and hook it on the end. 
   ender = regnode((char)((paren) ? REGEX_CLOSE+parno : REGEX_END));
   regtail(ret, ender);

   // Hook the tails of the branches to the closing node. 
   for (br = ret; br != NULL; br = regnext(br))
      regoptail(br, ender);

   // Check for proper termination. 
   if (paren && *regparse++ != (')')) 
   {
      TRACE0("unterminated ()\n");
      return NULL;
   } 
   else if (!paren && *regparse != ('\0')) 
   {
      if (*regparse == (')')) 
      {
         TRACE0("unmatched ()\n");
         return NULL;
      } 
      else
      {
         TRACE0("internal error: junk on end\n");
         return NULL;
      }
      // NOTREACHED 
   }

   return(ret);
}




//
// regbranch - one alternative of an | operator
//
// Implements the concatenation operator.
 
char *CRegExp::regbranch(int *flagp)
{
   char *ret;
   char *chain;
   char *latest;
   int flags;
   int c;

   *flagp = REGEX_WORST;            // Tentatively. 

   ret = regnode(REGEX_BRANCH);
   chain = NULL;
   while ((c = *regparse) != ('\0') && c != ('|') && c != (')')) {
      latest = regpiece(&flags);
      if (latest == NULL)
         return(NULL);
      *flagp |= flags&REGEX_HASWIDTH;
      if (chain == NULL)      // First piece. 
         *flagp |= flags&REGEX_SPSTART;
      else
         regtail(chain, latest);
      chain = latest;
   }
   if (chain == NULL)         // Loop ran zero times. 
      (void) regnode(REGEX_NOTHING);

   return(ret);
}

//
// regpiece - something followed by possible [*+?]
//
// Note that the branching code sequences used for ? and the general cases
// of * and + are somewhat optimized:  they use the same REGEX_NOTHING node as
// both the endmarker for their branch list and the body of the last branch.
// It might seem that this node could be dispensed with entirely, but the
// endmarker role is not redundant.
 
char *CRegExp::regpiece(int *flagp)
{
   char *ret;
   char op;
   char *next;
   int flags;

   ret = regatom(&flags);
   if (ret == NULL)
      return(NULL);

   op = *regparse;
   if (!REGEX_ISREPN(op)) {
      *flagp = flags;
      return(ret);
   }

   if (!(flags&REGEX_HASWIDTH) && op != ('?'))
   {
      TRACE0("*+ operand could be empty\n");
      return NULL;
   }

   switch (op) {
   case ('*'):  *flagp = REGEX_WORST|REGEX_SPSTART;       break;
   case ('+'):  *flagp = REGEX_WORST|REGEX_SPSTART|REGEX_HASWIDTH; break;
   case ('?'):  *flagp = REGEX_WORST;            break;
   }

   if (op == ('*') && (flags&REGEX_SIMPLE))
      reginsert(REGEX_STAR, ret);
   else if (op == ('*')) {
      // Emit x* as (x&|), where & means "self". 
      reginsert(REGEX_BRANCH, ret);    // Either x 
      regoptail(ret, regnode(REGEX_BACK));   // and loop 
      regoptail(ret, ret);    // back 
      regtail(ret, regnode(REGEX_BRANCH));   // or 
      regtail(ret, regnode(REGEX_NOTHING));  // null. 
   } else if (op == ('+') && (flags&REGEX_SIMPLE))
      reginsert(REGEX_PLUS, ret);
   else if (op == ('+')) {
      // Emit x+ as x(&|), where & means "self". 
      next = regnode(REGEX_BRANCH);    // Either 
      regtail(ret, next);
      regtail(regnode(REGEX_BACK), ret);  // loop back 
      regtail(next, regnode(REGEX_BRANCH));  // or 
      regtail(ret, regnode(REGEX_NOTHING));  // null. 
   } else if (op == ('?')) {
      // Emit x? as (x|) 
      reginsert(REGEX_BRANCH, ret);    // Either x 
      regtail(ret, regnode(REGEX_BRANCH));   // or 
      next = regnode(REGEX_NOTHING);      // null. 
      regtail(ret, next);
      regoptail(ret, next);
   }
   regparse++;
   if (REGEX_ISREPN(*regparse))
   {
      TRACE0("nested *?+\n");
      return NULL;
   }

   return(ret);
}

//
// regatom - the lowest level
//
// Optimization:  gobbles an entire sequence of ordinary characters so that
// it can turn them into a single node, which is smaller to store and
// faster to run.  Backslashed characters are exceptions, each becoming a
// separate node; the code is simpler that way and it's not worth fixing.
 
char *CRegExp::regatom(int *flagp)
{
   char *ret;
   int flags;

   *flagp = REGEX_WORST;      // Tentatively. 

   switch (*regparse++) {
   case ('^'):
      ret = regnode(REGEX_BOL);
      break;
   case ('$'):
      ret = regnode(REGEX_EOL);
      break;
   case ('.'):
      ret = regnode(REGEX_ANY);
      *flagp |= REGEX_HASWIDTH|REGEX_SIMPLE;
      break;
   case ('['): {
      int range;
      int rangeend;
      int c;

      if (*regparse == ('^')) {   // Complement of range. 
         ret = regnode(REGEX_ANYBUT);
         regparse++;
      } else
         ret = regnode(REGEX_ANYOF);
      if ((c = *regparse) == (']') || c == ('-')) {
         regc((char)c);
         regparse++;
      }
      while ((c = *regparse++) != ('\0') && c != (']')) {
         if (c != ('-'))
            regc((char)c);
         else if ((c = *regparse) == (']') || c == ('\0'))
            regc(('-'));
         else 
         {
            range = (unsigned) (char)*(regparse-2);
            rangeend = (unsigned) (char)c;
            if (range > rangeend)
            {
               TRACE0("invalid [] range\n");
               return NULL;
            }
            for (range++; range <= rangeend; range++)
               regc((char)range);
            regparse++;
         }
      }
      regc(('\0'));
      if (c != (']'))
      {
         TRACE0("unmatched []\n");
         return NULL;
      }
      *flagp |= REGEX_HASWIDTH|REGEX_SIMPLE;
      break;
      }
   case ('('):
      ret = reg(1, &flags);
      if (ret == NULL)
         return(NULL);
      *flagp |= flags&(REGEX_HASWIDTH|REGEX_SPSTART);
      break;
   case ('\0'):
   case ('|'):
   case (')'):
      // supposed to be caught earlier 
      TRACE0("internal error: \\0|) unexpected\n");
      return NULL;
      break;
   case ('?'):
   case ('+'):
   case ('*'):
      TRACE0("?+* follows nothing\n");
      return NULL;
      break;
   case ('\\'):
      if (*regparse == ('\0'))
      {
         TRACE0("trailing \\\n");
         return NULL;
      }
      ret = regnode(REGEX_EXACTLY);
      regc(*regparse++);
      regc(('\0'));
      *flagp |= REGEX_HASWIDTH|REGEX_SIMPLE;
      break;
   default: {
      size_t len;
      char ender;

      regparse--;
      len = strcspn(regparse, REGEX_META);
      if (len == 0)
      {
         TRACE0("internal error: strcspn 0\n");
         return NULL;
      }
      ender = *(regparse+len);
      if (len > 1 && REGEX_ISREPN(ender))
         len--;      // Back off clear of ?+* operand. 
      *flagp |= REGEX_HASWIDTH;
      if (len == 1)
         *flagp |= REGEX_SIMPLE;
      ret = regnode(REGEX_EXACTLY);
      for (; len > 0; len--)
         regc(*regparse++);
      regc(('\0'));
      break;
      }
   }

   return(ret);
}



// reginsert - insert an operator in front of already-emitted operand
//
// Means relocating the operand.
 
void CRegExp::reginsert(char op, char *opnd)
{
   char *place;

   if (!bEmitCode) {
      regsize += 3;
      return;
   }

   (void) memmove(opnd+3, opnd, (size_t)((regcode - opnd)*sizeof(char)));
   regcode += 3;

   place = opnd;     // Op node, where operand used to be. 
   *place++ = op;
   *place++ = ('\0');
   *place++ = ('\0');
}

//
// regtail - set the next-pointer at the end of a node chain
 
void CRegExp::regtail(char *p, char *val)
{
   char *scan;
   char *temp;
// int offset;

   if (!bEmitCode)
      return;

   // Find last node. 
   for (scan = p; (temp = regnext(scan)) != NULL; scan = temp)
      continue;

   *((short *)(scan+1)) = (char)((OP(scan) == REGEX_BACK) ? scan - val : val - scan);
}


// regoptail - regtail on operand of first argument; nop if operandless
 
void CRegExp::regoptail(char *p, char *val)
{
   // "Operandless" and "op != REGEX_BRANCH" are synonymous in practice. 
   if (!bEmitCode || OP(p) != REGEX_BRANCH)
      return;
   regtail(OPERAND(p), val);
}


// RegFind  - match a regexp against a string
// Returns  - Returns position of regexp or -1
//         if regular expression not found
// Note     - The regular expression should have been
//         previously compiled using RegComp
int CRegExp::RegFind(const char *str)
{
   char *string = (char *)str; // avert const poisoning 
   char *s;

   // Delete any previously stored found string
   delete sFoundText;
   sFoundText = NULL;

   // Be paranoid. 
   if(string == NULL) 
   {
      TRACE0("NULL argument to regexec\n");
      return(-1);
   }

   // Check validity of regex
   if (!bCompiled) 
   {
      TRACE0("No regular expression provided yet.\n");
      return(-1);
   }

   // If there is a "must appear" string, look for it. 
   if (regmust != NULL && strstr(string, regmust) == NULL)
      return(-1);

   // Mark beginning of line for ^
   regbol = string;

   // Simplest case:  anchored match need be tried only once. 
   if (reganch)
   {
      if( regtry(string) )
      {
         // Save the found substring in case we need it
         sFoundText = new char[GetFindLen()+1];
         sFoundText[GetFindLen()] = ('\0');
         strncpy(sFoundText, string, GetFindLen() );

         return 0;
      }
      //String not found
      return -1;
   }

   // Messy cases:  unanchored match. 
   if (regstart != ('\0')) 
   {
      // We know what char it must start with. 
      for (s = string; s != NULL; s = strchr(s+1, regstart))
         if (regtry(s))
         {
            int nPos = s-str;

            // Save the found substring in case we need it later
            sFoundText = new char[GetFindLen()+1];
            sFoundText[GetFindLen()] = ('\0');
            strncpy(sFoundText, s, GetFindLen() );

            return nPos;
         }
      return -1;
   } 
   else 
   {
      // We don't -- general case
      for (s = string; !regtry(s); s++)
         if (*s == ('\0'))
            return(-1);

      int nPos = s-str;

      // Save the found substring in case we need it later
      sFoundText = new char[GetFindLen()+1];
      sFoundText[GetFindLen()] = ('\0');
      strncpy(sFoundText, s, GetFindLen() );

      return nPos;
   }
   // NOTREACHED 
}


// regtry - try match at specific point
 
int   CRegExp::regtry(char *string)
{
   int i;
   char **stp;
   char **enp;

   reginput = string;

   stp = startp;
   enp = endp;
   for (i = NSUBEXP; i > 0; i--) 
   {
      *stp++ = NULL;
      *enp++ = NULL;
   }
   if (regmatch(program)) 
   {
      startp[0] = string;
      endp[0] = reginput;
      return(1);
   } 
   else
      return(0);
}

// regmatch - main matching routine
//
// Conceptually the strategy is simple:  check to see whether the current
// node matches, call self recursively to see whether the rest matches,
// and then act accordingly.  In practice we make some effort to avoid
// recursion, in particular by going through "ordinary" nodes (that don't
// need to know whether the rest of the match failed) by a loop instead of
// by recursion.
 
int   CRegExp::regmatch(char *prog)
{
   char *scan;   // Current node. 
   char *next;      // Next node. 

   for (scan = prog; scan != NULL; scan = next) {
      next = regnext(scan);

      switch (OP(scan)) {
      case REGEX_BOL:
         if (reginput != regbol)
            return(0);
         break;
      case REGEX_EOL:
         if (*reginput != ('\0'))
            return(0);
         break;
      case REGEX_ANY:
         if (*reginput == ('\0'))
            return(0);
         reginput++;
         break;
      case REGEX_EXACTLY: {
         size_t len;
         char *const opnd = OPERAND(scan);

         // Inline the first character, for speed. 
         if (*opnd != *reginput)
            return(0);
         len = strlen(opnd);
         if (len > 1 && strncmp(opnd, reginput, len) != 0)
            return(0);
         reginput += len;
         break;
         }
      case REGEX_ANYOF:
         if (*reginput == ('\0') ||
               strchr(OPERAND(scan), *reginput) == NULL)
            return(0);
         reginput++;
         break;
      case REGEX_ANYBUT:
         if (*reginput == ('\0') ||
               strchr(OPERAND(scan), *reginput) != NULL)
            return(0);
         reginput++;
         break;
      case REGEX_NOTHING:
         break;
      case REGEX_BACK:
         break;
      case REGEX_OPEN+1: case REGEX_OPEN+2: case REGEX_OPEN+3:
      case REGEX_OPEN+4: case REGEX_OPEN+5: case REGEX_OPEN+6:
      case REGEX_OPEN+7: case REGEX_OPEN+8: case REGEX_OPEN+9: {
         const int no = OP(scan) - REGEX_OPEN;
         char *const input = reginput;

         if (regmatch(next)) {
            // Don't set startp if some later
            // invocation of the same parentheses
            // already has.
             
            if (startp[no] == NULL)
               startp[no] = input;
            return(1);
         } else
            return(0);
         break;
         }
      case REGEX_CLOSE+1: case REGEX_CLOSE+2: case REGEX_CLOSE+3:
      case REGEX_CLOSE+4: case REGEX_CLOSE+5: case REGEX_CLOSE+6:
      case REGEX_CLOSE+7: case REGEX_CLOSE+8: case REGEX_CLOSE+9: {
         const int no = OP(scan) - REGEX_CLOSE;
         char *const input = reginput;

         if (regmatch(next)) {
            // Don't set endp if some later
            // invocation of the same parentheses
            // already has.
             
            if (endp[no] == NULL)
               endp[no] = input;
            return(1);
         } else
            return(0);
         break;
         }
      case REGEX_BRANCH: {
         char *const save = reginput;

         if (OP(next) != REGEX_BRANCH)    // No choice. 
            next = OPERAND(scan);   // Avoid recursion. 
         else {
            while (OP(scan) == REGEX_BRANCH) {
               if (regmatch(OPERAND(scan)))
                  return(1);
               reginput = save;
               scan = regnext(scan);
            }
            return(0);
            // NOTREACHED 
         }
         break;
         }
      case REGEX_STAR: 
      case REGEX_PLUS: {
         const char nextch = 
            (OP(next) == REGEX_EXACTLY) ? *OPERAND(next) : ('\0');
         size_t no;
         char *const save = reginput;
         const size_t min = (OP(scan) == REGEX_STAR) ? 0 : 1;

         for (no = regrepeat(OPERAND(scan)) + 1; no > min; no--) {
            reginput = save + no - 1;
            // If it could work, try it. 
            if (nextch == ('\0') || *reginput == nextch)
               if (regmatch(next))
                  return(1);
         }
         return(0);
         break;
         }
      case REGEX_END:
         return(1);  // Success! 
         break;
      default:
         TRACE0("regexp corruption\n");
         return(0);
         break;
      }
   }

   // We get here only if there's trouble -- normally "case REGEX_END" is
   // the terminating point.
    
   TRACE0("corrupted pointers\n");
   return(0);
}


// regrepeat - report how many times something simple would match
 
size_t CRegExp::regrepeat(char *node)
{
   size_t count;
   char *scan;
   char ch;

   switch (OP(node)) 
   {
   case REGEX_ANY:
      return(strlen(reginput));
      break;
   case REGEX_EXACTLY:
      ch = *OPERAND(node);
      count = 0;
      for (scan = reginput; *scan == ch; scan++)
         count++;
      return(count);
      break;
   case REGEX_ANYOF:
      return(strspn(reginput, OPERAND(node)));
      break;
   case REGEX_ANYBUT:
      return(strcspn(reginput, OPERAND(node)));
      break;
   default:    // Oh dear.  Called inappropriately. 
      TRACE0("internal error: bad call of regrepeat\n");
      return(0);  // Best compromise. 
      break;
   }
   // NOTREACHED 
}

// regnext - dig the "next" pointer out of a node
 
char *CRegExp::regnext(char *p)
{
   const short &offset = *((short*)(p+1)); 

   if (offset == 0)
      return(NULL);

   return((OP(p) == REGEX_BACK) ? p-offset : p+offset);
}

// GetReplaceString  - Converts a replace expression to a string
// Returns        - Pointer to newly allocated string
//               Caller is responsible for deleting it
char* CRegExp::GetReplaceString( const char* sReplaceExp )
{
   char *src = (char *)sReplaceExp;
   char *buf;
   char c;
   int no;
   size_t len;

   if( sReplaceExp == NULL || sFoundText == NULL )
      return NULL;


   // First compute the length of the string
   int replacelen = 0;
   while ((c = *src++) != ('\0')) 
   {
      if (c == ('&'))
         no = 0;
      else if (c == ('\\') && isdigit(*src))
         no = *src++ - ('0');
      else
         no = -1;

      if (no < 0) 
      {  
         // Ordinary character. 
         if (c == ('\\') && (*src == ('\\') || *src == ('&')))
            c = *src++;
         replacelen++;
      } 
      else if (startp[no] != NULL && endp[no] != NULL &&
               endp[no] > startp[no]) 
      {
         // Get tagged expression
         len = endp[no] - startp[no];
         replacelen += len;
      }
   }

   // Now allocate buf
   buf = new char[replacelen+1];
   if( buf == NULL )
      return NULL;

   char* sReplaceStr = buf;

   // Add null termination
   buf[replacelen] = ('\0');
   
   // Now we can create the string
   src = (char *)sReplaceExp;
   while ((c = *src++) != ('\0')) 
   {
      if (c == ('&'))
         no = 0;
      else if (c == ('\\') && isdigit(*src))
         no = *src++ - ('0');
      else
         no = -1;

      if (no < 0) 
      {  
         // Ordinary character. 
         if (c == ('\\') && (*src == ('\\') || *src == ('&')))
            c = *src++;
         *buf++ = c;
      } 
      else if (startp[no] != NULL && endp[no] != NULL &&
               endp[no] > startp[no]) 
      {
         // Get tagged expression
         len = endp[no] - startp[no];
         int tagpos = startp[no] - startp[0];

         strncpy(buf, sFoundText + tagpos, len);
         buf += len;
      }
   }

   return sReplaceStr;
}

