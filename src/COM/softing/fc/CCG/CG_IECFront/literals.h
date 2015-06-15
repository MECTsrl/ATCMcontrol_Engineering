
#ifndef __LITERALS_H_
#define __LITERALS_H_

//Literal values:
struct ST_BINVAL_TYP
{
    long    iLen; //>=0 if pText is used, -1 if any64 is used
    
    union
    {
        __int64 any64; //ANY_INT, ANY_BIT, ANY_DATE, TIME
        
        TCHAR*  pText; //ANY_REAL, ANY_STRING
                       //ANY_REAL: pText must be signed '-' or '+'
                       //Float format must be compatible with 
                       //C-runtime function _tcstod.
                       
                       //pText is always null terminated, 
                       //even if binary string.
    };
};

//init: ANY_INT, ANY_BIT, ANY_DATE:
inline void ST_InitBinValI64(ST_BINVAL_TYP* pInit, __int64 any64) 
{
    pInit->iLen = -1;
    pInit->any64 = any64;
}

//ANY_REAL, ANY_STRING:
void ST_InitBinValString(ST_BINVAL_TYP* pInit, const TCHAR* pText); 
//init pInit with content of pFrom:
void ST_InitBinValFrom(ST_BINVAL_TYP* pInit, const ST_BINVAL_TYP* pFrom);
//free pFree:
void ST_FreeInitBinVal(ST_BINVAL_TYP* pFree);
//init pInit from CG_Literal:
void ST_InitBinValFrom(ST_BINVAL_TYP* pInit, const CG_Literal* pFrom);

class ST_Literal_C;
ST_BINVAL_TYP* ST_LitBinValEval
(
    CGT_CDiagMsg*            pdm,
    const CG_Edipos*         pEdp,
    CG_EXPRTYP               op,
    const ST_Literal_C*      pLit1,
    const ST_Literal_C*      pLit2,
    CG_BITSET*               pResultType
);

FC_CStr ST_BinValToIECString(const ST_BINVAL_TYP* pBinVal, CG_BITSET lTypSet);

class st_ParseContext;
class CFeatureSet;
ST_Literal_C* ST_LitTokenToLiteral(
    CGT_CDiagMsg*      pdm,
    const CG_Edipos*   pEdp, 
    st_ParseContext*   pfc,
    const CFeatureSet* pFeat,
    int                tok, 
    const TCHAR*       pText
);





#endif // __LITERALS_H_
