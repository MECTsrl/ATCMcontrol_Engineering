%{

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

%}

%union 
{
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
};

%token <ident> IDENTIFIER
%token <ident> MACHINEADDRESS
%token DIRECT_VARIABLE
%token <expr> LITERAL
%token <number> NUMBER
%token <letter> LETTER
%token DOTDOT
%token <conversion> CONVERSION

%token <expr> POW
%token <expr> NOT OR XOR AND
%token <expr> ADD SUB MUL DIV MOD
%token <expr> EQ NE LT LE GT GE EQ_SHORT NE_SHORT LT_SHORT LE_SHORT GT_SHORT GE_SHORT

%left OR
%left XOR
%left AND '&'
%left EQ_SHORT NE_SHORT
%left LT_SHORT LE_SHORT GT_SHORT GE_SHORT
%left '+' '-'
%left '*' '/' MOD
%left POW
%left NOT

%type <expr> expression primary designator
%type <exprlist> expressionlist2 expressions

%{
/* to avoid 1000 Warning of the same kind */
#pragma warning (disable: 4309 4305 4102 4244 4551)
%}

%%

root
 : expression
    {
        pContext->m_pRoot = $1;
    }
 | rangelist
 ;

expression
 : primary
 | '+' expression %prec NOT
    {
        $$ = pContext->m_pHolder->NewUnaryArithmetic (CExpressionUnaryArithmetic::Plus, $2);
    }
 | '-' expression %prec NOT
    {
        $$ = pContext->m_pHolder->NewUnaryArithmetic (CExpressionUnaryArithmetic::Minus, $2);
    }
 | NOT expression
    {
        $$ = pContext->m_pHolder->NewUnaryLogical (CExpressionUnaryLogical::Not, $2);
    }
 | expression '+' expression
    {
        $$ = pContext->m_pHolder->NewBinaryArithmetic (CExpressionBinaryArithmetic::Add, $1, $3);
    }
 | expression '-' expression
    {
        $$ = pContext->m_pHolder->NewBinaryArithmetic (CExpressionBinaryArithmetic::Subtract, $1, $3);
    }
 | expression '*' expression
    {
        $$ = pContext->m_pHolder->NewBinaryArithmetic (CExpressionBinaryArithmetic::Multiply, $1, $3);
    }
 | expression '/' expression
    {
        $$ = pContext->m_pHolder->NewBinaryArithmetic (CExpressionBinaryArithmetic::Divide, $1, $3);
    }
 | expression MOD expression
    {
        $$ = pContext->m_pHolder->NewBinaryArithmetic (CExpressionBinaryArithmetic::Modulo, $1, $3);
    }
 | expression POW expression
    {
        $$ = pContext->m_pHolder->NewPower ($1, $3);
    }
 | expression OR expression
    {
        $$ = pContext->m_pHolder->NewBinaryLogical (CExpressionBinaryLogical::Or, $1, $3);
    }
 | expression XOR expression
    {
        $$ = pContext->m_pHolder->NewBinaryLogical (CExpressionBinaryLogical::Xor, $1, $3);
    }
 | expression AND expression
    {
        $$ = pContext->m_pHolder->NewBinaryLogical (CExpressionBinaryLogical::And, $1, $3);
    }
 | expression '&' expression
    {
        $$ = pContext->m_pHolder->NewBinaryLogical (CExpressionBinaryLogical::And, $1, $3);
    }
 | expression EQ_SHORT expression
    {
        $$ = pContext->m_pHolder->NewRelation (CExpressionRelation::Equal, $1, $3);
    }
 | expression NE_SHORT expression
    {
        $$ = pContext->m_pHolder->NewRelation (CExpressionRelation::Unequal, $1, $3);
    }
 | expression LT_SHORT expression
    {
        $$ = pContext->m_pHolder->NewRelation (CExpressionRelation::Less, $1, $3);
    }
 | expression LE_SHORT expression
    {
        $$ = pContext->m_pHolder->NewRelation (CExpressionRelation::LessEqual, $1, $3);
    }
 | expression GT_SHORT expression
    {
        $$ = pContext->m_pHolder->NewRelation (CExpressionRelation::Greater, $1, $3);
    }
 | expression GE_SHORT expression
    {
        $$ = pContext->m_pHolder->NewRelation (CExpressionRelation::GreaterEqual, $1, $3);
    }
 ;

primary
 : LITERAL
 | designator
 | '(' expression ')'
    {
        $$ = $2;
    }
 | CONVERSION '(' expression ')'
    {
        $$ = pContext->m_pHolder->NewConversion ($3, $1.source, $1.target);
    }
 | ADD expressionlist2
    {
        $$ = pContext->m_pHolder->NewFunctionalArithmetic (CExpressionBinaryArithmetic::Add, $2);
        delete $2;
    }
 | SUB '(' expression ',' expression ')'
    {
        $$ = pContext->m_pHolder->NewBinaryArithmetic (CExpressionBinaryArithmetic::Subtract, $3, $5);
    }
 | MUL expressionlist2
    {
        $$ = pContext->m_pHolder->NewFunctionalArithmetic (CExpressionBinaryArithmetic::Multiply, $2);
        delete $2;
    }
 | DIV '(' expression ',' expression ')'
    {
        $$ = pContext->m_pHolder->NewBinaryArithmetic (CExpressionBinaryArithmetic::Divide, $3, $5);
    }
 | MOD '(' expression ',' expression ')'
    {
        $$ = pContext->m_pHolder->NewBinaryArithmetic (CExpressionBinaryArithmetic::Modulo, $3, $5);
    }
 | AND expressionlist2
    {
        $$ = pContext->m_pHolder->NewFunctionalLogical (CExpressionBinaryLogical::And, $2);
        delete $2;
    }
 | OR expressionlist2
    {
        $$ = pContext->m_pHolder->NewFunctionalLogical (CExpressionBinaryLogical::Or, $2);
        delete $2;
    }
 | XOR expressionlist2
    {
        $$ = pContext->m_pHolder->NewFunctionalLogical (CExpressionBinaryLogical::Xor, $2);
        delete $2;
    }
 | EQ '(' expression ',' expression ')'
    {
        $$ = pContext->m_pHolder->NewRelation (CExpressionRelation::Equal, $3, $5);
    }
 | NE '(' expression ',' expression ')'
    {
        $$ = pContext->m_pHolder->NewRelation (CExpressionRelation::Unequal, $3, $5);
    }
 | LT '(' expression ',' expression ')'
    {
        $$ = pContext->m_pHolder->NewRelation (CExpressionRelation::Less, $3, $5);
    }
 | LE '(' expression ',' expression ')'
    {
        $$ = pContext->m_pHolder->NewRelation (CExpressionRelation::LessEqual, $3, $5);
    }
 | GT '(' expression ',' expression ')'
    {
        $$ = pContext->m_pHolder->NewRelation (CExpressionRelation::Greater, $3, $5);
    }
 | GE '(' expression ',' expression ')'
    {
        $$ = pContext->m_pHolder->NewRelation (CExpressionRelation::GreaterEqual, $3, $5);
    }
 ;

designator
 : IDENTIFIER
    {
        CString strPrefix;

        $$ = pContext->m_pHolder->NewScopedName (pContext->m_ExpandMode, pContext->m_strScope, *$1.text, strPrefix);
        delete $1.text;

        if (!strPrefix.IsEmpty ())
        {
            PrefixedIdentifier prefixedIdentifier ($1.offset, strPrefix);

            pContext->m_PrefixedIdentifiers.AddTail (prefixedIdentifier);
        };
    }
 | MACHINEADDRESS
    {
        $$ = pContext->m_pHolder->NewConfiguration (*$1.text, true);
        delete $1.text;
    }
 | designator '.' IDENTIFIER
    {
        $$ = pContext->m_pHolder->NewComponent ($1, *$3.text);
        delete $3.text;
    }
 | designator '[' expression ']'
    {
        $$ = pContext->m_pHolder->NewIndexing ($1, $3);
    }
 ;

expressionlist2
 : '(' expression ',' expressions ')'
    {
        $$ = $4;
        $$->AddHead ($2);
    }
 ;

expressions
 : expression
    {
        $$ = new CList<CExpressionNode *, CExpressionNode *>;
        $$->AddHead ($1);
    }
 | expression ',' expressions
    {
        $$ = $3;
        $$->AddHead ($1);
    }
 ;

rangelist
 : '[' ranges ']'
 ;

ranges
 : range
 | range ',' ranges
 ;

range
 : NUMBER
    {
        pContext->m_pRanges->Add (CExpressionHolder::Range ($1, $1));
    }
 | NUMBER DOTDOT NUMBER
    {
        pContext->m_pRanges->Add (CExpressionHolder::Range ($1, $3));
    }
 ;

%%

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
