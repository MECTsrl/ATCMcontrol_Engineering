

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "cgt_literal.h"
#include "baselib.h"

//----  Static Initializations:   ----------------------------------*


struct CGT_CtxInfo
{
	CGT_CDiagMsg*    pdm;
	const CG_Edipos* pEdp;
	const TCHAR*	 pStr;
};

void fct_BL_CB(void* pCtx, int offset, int bBadEscape, int nBadUnicode, int bBadNewLine)
{
	CGT_CtxInfo* pContext = static_cast<CGT_CtxInfo*>(pCtx);
	CG_Edipos    edp      = *pContext->pEdp;
	CG_IncColEdipos(&edp, offset);

	if (bBadEscape)
	{
		TCHAR ebuff[2] = { pContext->pStr[offset], 0};
		pContext->pdm->msg1(CG_E_INVALID_ESCAPE, &edp, ebuff);
	}
	else if (nBadUnicode)
	{
		TCHAR tmp3[6];
		wsprintf(tmp3, "$%04x", nBadUnicode);
		pContext->pdm->msg1(CG_W_BAD_UNICODE_CHAR, &edp, tmp3);
	}
	else if (bBadNewLine)
	    pContext->pdm->msg0(CG_E_NEWLINE_IN_STRING, &edp);
	else
		assert(!"no error");
}

extern "C" bool CGT_IECStringToBinary(
    CGT_CDiagMsg*    pdm,
    const CG_Edipos* pEdp,
    const TCHAR*     pszIECStr, 
	char*            pszBinStr,
    long*            pIChars) 
{
    const TCHAR* pszStr     = pszIECStr;
    int          nInput     = 0;
	CGT_CtxInfo  ctx        = { pdm, pEdp, pszIECStr };
	HRESULT      hr;

    while(isascii(*pszStr) && isspace(*pszStr))
    {
        nInput++;
        pszStr++;
    }
	assert(pszStr[nInput] == _T('\'') || pszStr[nInput] == _T('"'));
    if(pdm && pszStr[nInput] != _T('\'') && pszStr[nInput] != _T('"'))
    {
		assert(pEdp);
		CG_Edipos edp = *pEdp;
        CG_IncColEdipos(&edp, nInput);
        pdm->msg1(CG_E_SYNTAX_EXPECTED, &edp, _T("\" or '"));                                   
        return NULL;
    }

	if(pszStr[nInput] == _T('\''))
		hr = BL_IECStringToBinary(pszIECStr, pszBinStr, pIChars, pdm?fct_BL_CB:NULL, &ctx);
	else
		hr = BL_IECWStringToBinary(pszIECStr, (TCHAR*)pszBinStr, pIChars, pdm?fct_BL_CB:NULL, &ctx);

	if (SUCCEEDED(hr) || (!pszBinStr && hr == E_BL_ERR_BUFFER_TO_SMALL))
		return true;

	if (hr == E_BL_ERR_INVALID_IEC_STR)
	{   
		if (pdm) pdm->msg1(CG_E_SYNTAX_UNEXPECTED, pEdp, pszIECStr);
	}
	else 
		assert(hr != E_BL_ERR_BUFFER_TO_SMALL);
	return false;
}

