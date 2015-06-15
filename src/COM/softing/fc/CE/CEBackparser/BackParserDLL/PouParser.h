
#if !defined __POUPARSER__
#define __POUPARSER__

#include "lex_yacc\Grammar.h"

#include "POU.h"
#include "FunctionBlock.h"
#include "Function.h"
#include "Resource.h"
#include "Configuration.h"
#include "StStruct.h"

class PouParser : public yy_parse
{
public:
    int yyparse(yy_scan* scanner, POU** pou);
    int yyparse(yy_scan* scanner, FunctionBlock** fb);
    int yyparse(yy_scan* scanner, Function** f);
    int yyparse(yy_scan* scanner, Resource** res);
    int yyparse(yy_scan* scanner, Configuration** con);
    int yyparse(yy_scan* scanner, GlobVarConstList** gvcl);
    int yyparse(yy_scan* scanner, StStruct** st);

    virtual stream_scan* getStreamScanner();
};

#endif
