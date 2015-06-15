

//----  Includes:   -------------------------------------------*
#include <stdio.h>
#include <malloc.h>

#include "CG_Messages.h"

#include "CGT_CDiagMsg.h"
#include "fc_tools\fc_cstring.h"


CGT_CDiagMsg::CGT_CDiagMsg(
    const TCHAR*  pszModule,
    HMODULE       hModule
) : m_jot1(MAX_PATH), 
    m_jot2(MAX_PATH), 
    m_jot3(MAX_PATH) 
{
    m_pIFront            = NULL;
    m_displayedErrCount  = 0;
    m_silentErrCount     = 0;
    m_displayedWarnCount = 0;

    m_maxErrors = 100;
    m_iDisableWarnings = 0;
    m_bToManyErrors = 0;
    m_moduleName.load(pszModule);
    //where to get string format resources:
    m_HmoduleRes = hModule;
}



void CGT_CDiagMsg::setFront(ICG_IECFront* pIFront)
{
    m_pIFront = pIFront;
}
void CGT_CDiagMsg::setModuleName(const TCHAR* pszModule)
{
    m_moduleName.load(pszModule);
}
void CGT_CDiagMsg::setNotSupportedName(const TCHAR* pszTrgType)
{
    m_NSName.load(pszTrgType ? pszTrgType : _T(""));
}





long CGT_CDiagMsg::getTotalErrors()const
{
    int nerr = m_displayedErrCount + m_silentErrCount;
    if(m_pIFront)
        nerr += m_pIFront->getTotalErrors();

    return nerr;
}
long CGT_CDiagMsg::getDisplayedErrors()const
{
    int nerr = m_displayedErrCount;
    if(m_pIFront)
        nerr += m_pIFront->getDisplayedErrors();

    return nerr;
}
long CGT_CDiagMsg::getDisplayedWarnings()const
{
    int nwarn = m_displayedWarnCount;
    if(m_pIFront)
        nwarn += m_pIFront->getDisplayedWarnings();

    return nwarn;
}




void CGT_CDiagMsg::userMsg(const TCHAR* psz)
{
    //TODO ьядц must be handeled in respect to context called
    //we don't know, so make best output for embedded in 4CE
    bool bEmbeeded = true;
    if(bEmbeeded)
    {
        _fputts(psz, stdout);
    }
    else
    {
        int    iLen   = lstrlen(psz);
        TCHAR* pszOem = (TCHAR*)alloca((iLen+1)*sizeof(TCHAR));

        if(CharToOem(psz, pszOem))
            _fputts(pszOem, stdout);
        else
            _fputts(psz, stdout);
    }

    fflush(stdout);
}


void CGT_CDiagMsg::addIndirectErr()
{ 
    assert(m_displayedErrCount + (m_pIFront?m_pIFront->getDisplayedErrors():0) >0);
    m_silentErrCount++;
}
void CGT_CDiagMsg::incErrors(long nErr)
{
    assert(nErr>=0);
    m_displayedErrCount += nErr;
}
void CGT_CDiagMsg::incWarnings(long nWarn)
{
    assert(nWarn>=0);
    m_displayedWarnCount += nWarn;
}


void CGT_CDiagMsg::disableWarnings()
{
    assert(m_iDisableWarnings>=0);
    m_iDisableWarnings++;
    //TODO m_pIFront->enableWarnings();
}
void CGT_CDiagMsg::enableWarnings()
{
    m_iDisableWarnings--;
    assert(m_iDisableWarnings>=0);
    //TODO m_pIFront->enableWarnings();
}
void CGT_CDiagMsg::setMaxErrors(int nMax)
{
    assert(nMax>1);
    m_maxErrors = nMax;
}




//diag msg for 0..4 arguments:
void CGT_CDiagMsg::msg0(HRESULT id, const CG_Edipos* pEdp)
{
    msg(id, pEdp, NULL);
}
void CGT_CDiagMsg::msg1(HRESULT id, const CG_Edipos* pEdp, const TCHAR* psz)
{
    const TCHAR* ppsz[] = {psz};
    msg(id, pEdp, ppsz);
}
void CGT_CDiagMsg::msg2(HRESULT id, const CG_Edipos* pEdp, const TCHAR* psz, const TCHAR* psz2)
{
    const TCHAR* ppsz[] = {psz, psz2};
    msg(id, pEdp, ppsz);
}
void CGT_CDiagMsg::msg3(HRESULT id, const CG_Edipos* pEdp, 
    const TCHAR* psz, const TCHAR* psz2,  const TCHAR* psz3)
{
    const TCHAR* ppsz[] = {psz, psz2, psz3};
    msg(id, pEdp, ppsz);
}
void CGT_CDiagMsg::msg4(HRESULT id, const CG_Edipos* pEdp, 
    const TCHAR* psz, const TCHAR* psz2,  const TCHAR* psz3, const TCHAR* psz4)
{
    const TCHAR* ppsz[] = {psz, psz2, psz3, psz4};
    msg(id, pEdp, ppsz);
}
#ifndef UNICODE 
void CGT_CDiagMsg::msg1(HRESULT id, const CG_Edipos* pEdp, 
    const wchar_t* psz)
{
    FC_CString jot;
    jot.load(psz);

    const TCHAR* ppsz[] = {jot};
    msg(id, pEdp, ppsz);
}
void CGT_CDiagMsg::msg2(HRESULT id, const CG_Edipos* pEdp, 
    const wchar_t* psz, const wchar_t* psz2)
{
    FC_CString jot,jot2;
    jot.load(psz);
    jot2.load(psz2);

    const TCHAR* ppsz[] = {jot, jot2};
    msg(id, pEdp, ppsz);
}
void CGT_CDiagMsg::msg3(HRESULT id, const CG_Edipos* pEdp, 
    const wchar_t* psz, const wchar_t* psz2, const wchar_t* psz3)
{
    FC_CString jot,jot2, jot3;
    jot.load(psz);
    jot2.load(psz2);
    jot3.load(psz3);

    const TCHAR* ppsz[] = {jot, jot2, jot3};
    msg(id, pEdp, ppsz);
}
void CGT_CDiagMsg::msg4(HRESULT id, const CG_Edipos* pEdp, 
    const wchar_t* psz, const wchar_t* psz2, const wchar_t* psz3, const wchar_t* psz4)
{
    FC_CString jot,jot2, jot3, jot4;
    jot.load(psz);
    jot2.load(psz2);
    jot3.load(psz3);
    jot4.load(psz4);

    const TCHAR* ppsz[] = {jot, jot2, jot3, jot4};
    msg(id, pEdp, ppsz);
}
#endif



void CGT_CDiagMsg::errNS1(
    const CG_Edipos* pEdp, 
    const TCHAR*     psz1
)
{
    errNS3(pEdp, psz1, NULL, NULL);
}
void CGT_CDiagMsg::errNS2(
    const CG_Edipos* pEdp, 
    const TCHAR*     psz1,
    const TCHAR*     psz2
)
{
    errNS3(pEdp, psz1, psz2, NULL);
}
void CGT_CDiagMsg::errNS3(
    const CG_Edipos* pEdp, 
    const TCHAR*     psz1,
    const TCHAR*     psz2,
    const TCHAR*     psz3
)
{ 
    FC_CString jot(MAX_PATH);

    if(psz1)
        jot<<psz1;
    if(psz2)
        jot<<psz2;
    if(psz3)
        jot<<psz3;

    HRESULT errId =  CG_E_NOT_SUPP_TRG;
    if(m_NSName.isEmpty())
        msg1(CG_E_NOT_SUPPORTED, pEdp, jot);
    else
        msg2(CG_E_NOT_SUPP_TRG, pEdp, m_NSName, jot);
}





void CGT_CDiagMsg::sysErr(
    HRESULT          hr, 
    const CG_Edipos* pEdp, 
    const TCHAR*     pszHint,
    const TCHAR*      pszHint2
)
{
    FC_CString jot(MAX_PATH);
    FC_CString jot2;

    if(pszHint)
    {
        jot<<_T("'")<<pszHint;

        if(pszHint2)
            jot<<pszHint2;
            
        jot<<_T("' ");
    }

    jot.appendWsprintf(_T("returned 0x%x: "), hr);
    jot2.fmtSysErr(hr);
    jot<<jot2;
    msg1(CG_E_COM_ERROR, pEdp, jot);
}

void CGT_CDiagMsg::intErr(
    const CG_Edipos* pEdp,
    const TCHAR*     pszCompilerFile, 
    int              nLine, 
    const TCHAR*     pszReason
)
{
    TCHAR     buffer[64];
  
    const TCHAR* ppsz[3];
    ppsz[0] = pszCompilerFile ? pszCompilerFile : _T("??");
    ppsz[1] = ltoa(nLine, buffer, 10);
    ppsz[2] = pszReason ? pszReason : _T("??");
    
    msg(CG_E_INTERNAL, pEdp, ppsz);
}
void CGT_CDiagMsg::intErr(
    const TCHAR* pszCompilerFile, 
    int          nLine, 
    const TCHAR* pszReason
)
{
    intErr(NULL, pszCompilerFile, nLine, pszReason);
}



void CGT_CDiagMsg::msg( 
    HRESULT           id,    //predefined diag message (see CGMessages.h)
    const CG_Edipos*  pEdp,     //error location (used to find POU's symtab)
    const TCHAR*      ppsz[]    //NULL or the textual inserts for all %[0-9]
)
{
    FC_CString jot(2*MAX_PATH);
    int        iSev = 0;
    int        nErrDisplayed;
    bool       bInfo;
    bool       bWarn;
    bool       bError;
    bool       bFmtIsStr;

    assert(id);
    bInfo  = CGT_IsInfoSeverity(id);
    bError = CGT_IsErrorSeverity(id);
    bWarn  = CGT_IsWarningSeverity(id);
    nErrDisplayed = getDisplayedErrors();
    assert(bInfo || bError || bWarn);

    //no error or warning msgs if too many errors but informational msgs 
    //must still be displayed:
    if(!bInfo && m_bToManyErrors)
    {
        //but update the internal counters! 
        if(bError)
            m_silentErrCount++;

        assert(nErrDisplayed>0);
        return;
    }
      
    if(bError)
    {
        //no error or warning msgs if too many errors but informational msgs 
        //must still be displayed:
        if(nErrDisplayed>=m_maxErrors)
        {
            assert(m_maxErrors>1);
            assert(!m_bToManyErrors);
            m_bToManyErrors = true;
            //if too many errors display a message that we soon will stop:
            id = CG_I_TO_MANY_ERRORS;
            pEdp = NULL;
        }
    }
    else if(bWarn && m_iDisableWarnings!=0)
    {
        return;
    }


    userMsg(edpToStr(pEdp, jot));
    userMsg(_T(" : "));


    //try get string resource from this module first:
    bFmtIsStr = false;
    jot.fmtMsgAr(id, m_HmoduleRes, ppsz);
    
    //try to get a predefined format from the front end:
    if(jot.isEmpty() && m_pIFront)
    {
        BSTR bstrFmt = m_pIFront->getPredefMsgFmt(id);
        if(bstrFmt)
        {
            bFmtIsStr = true;
            jot.load(bstrFmt);
            bFmtIsStr = true;
            SysFreeString(bstrFmt);
        }
    }

    if(bFmtIsStr)
    {
        //message format from supplied by front end:
        TCHAR*  pBuf = NULL;     
        DWORD   dwRet;
        dwRet = FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER|
                FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ARGUMENT_ARRAY,
                jot.psz(), 
                id, 
                MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), //0, force english msgs... TODO
                (LPTSTR)&pBuf, 
                0, 
                (TCHAR**)ppsz);

        assert(pBuf && dwRet);
        if(pBuf && dwRet)
        {
            jot.load(pBuf);
            LocalFree(pBuf);
        }
    }

    //if still not found, error!
    //if not found, error!
    if(jot.isEmpty())
    {
        jot.appendWsprintf(
         _T("%s : error: internal error: no resource for error id=0x%x\n"),
           m_moduleName.psz(), id);
    }

    //assert that:
    assert(bInfo || !_istspace(jot[0]) || !"diagnostic messages must not start with white space");
    userMsg(jot);

    //increment error counter:
    if(bError)
    {
        m_displayedErrCount++;

        //HACK, but works fine...
        if(FC_StringBeginI(jot, _T("fatal error")))
        {
            jot.load(_T("\n"))<<m_moduleName<<_T(" : terminate process ...\n");
            userMsg(jot);
#ifndef DEBUG
            exit(2);
#endif
        }
    }
    else if(bWarn)
    {
        m_displayedWarnCount++;
    }

    if(m_bToManyErrors)
        exit(3);
}


const bool CGT_CDiagMsg::edpToStrBuffer(
    const CG_Edipos* pEdp, 
    TCHAR            szedp[/*nBuf*/], 
    int              nBuf,
    bool             bNoFile
)
{
    TCHAR        buffer[MAX_PATH*3];
    int          bOk;
    const TCHAR* pszFile;


    if(pEdp==NULL)
    {
        bOk = FC_StringCopyToBuffer(szedp, nBuf, bNoFile ? _T("") : m_moduleName);
        assert(bOk);
        return bOk!=0;
    }

    if(!pEdp->pszFile)
    {
        assert(pEdp->typ==CG_EDP_FILE && pEdp->lc.l==0 && pEdp->lc.c==0);
        bOk = FC_StringCopyToBuffer(szedp, nBuf, bNoFile ? _T("") : _T("command line"));
        return bOk!=0;
    }

    pszFile = bNoFile ? _T("") : pEdp->pszFile;

    switch(pEdp->typ)
    {
    case CG_EDP_FILE:   
        wsprintf(buffer, _T("%s"), pszFile);
    break;
    
    case CG_EDP_ST:    
        if(pEdp->lc.l==0)
            wsprintf(buffer, _T("%s"), pszFile);
        else if(pEdp->lc.c==0)
            wsprintf(buffer, _T("%s(%d)"), pszFile, pEdp->lc.l);
        else
            wsprintf(buffer, _T("%s(%d,%d)"), pszFile, pEdp->lc.l, pEdp->lc.c);
    break;    
    
    case CG_EDP_4GR_BOX:
        wsprintf(buffer, _T("%s(b,%d,%d)"), pszFile, pEdp->gr.x, pEdp->gr.y);
    break;    
    
    case CG_EDP_4GR_STBOX:
        if(pEdp->gr.l==0)
            wsprintf(buffer, _T("%s(b,%d,%d)"), pszFile, pEdp->gr.x, pEdp->gr.y);
        else if(pEdp->gr.c==0)
            wsprintf(buffer, _T("%s(b,%d,%d,%d)"), pszFile, pEdp->gr.x, pEdp->gr.y, pEdp->gr.l);
        else
            wsprintf(buffer, _T("%s(b,%d,%d,%d,%d)"), pszFile, pEdp->gr.x, pEdp->gr.y, pEdp->gr.l, pEdp->gr.c);
    break;    
    
    case CG_EDP_4GR_SIGNAL:
        wsprintf(buffer, _T("%s(s,%d,%d)"), pszFile, pEdp->gr.x, pEdp->gr.y);
    break;    
    
    case CG_EDP_4GR_LINE:
        wsprintf(buffer, _T("%s(l,%d,%d,%d,%d)"), pszFile, pEdp->gr.x, pEdp->gr.y, pEdp->gr.l, pEdp->gr.c);
    break;    
    
    case CG_EDP_4GR_REGION:
        wsprintf(buffer, _T("%s(r,%d,%d,%d,%d)"), pszFile, pEdp->gr.x, pEdp->gr.y, pEdp->gr.l, pEdp->gr.c);
    break;
    
    case CG_EDP_4GR_BEG:
        wsprintf(buffer, _T("%s(begin)"), pszFile);
    break;
    
    case CG_EDP_4GR_END:
        wsprintf(buffer, _T("%s(end)"), pszFile);
    break;
    
    case CG_EDP_TEXT:
        assert(pEdp->psz);
        wsprintf(buffer, _T("%s%s"), pszFile, pEdp->psz ? pEdp->psz : _T("??"));
    break;

    default:
        buffer[0] = 0;
        assert(!"CG_Edipos::typ bad enum");
    }

    bOk = FC_StringCopyToBuffer(szedp, nBuf, buffer);
    assert(bOk);

    return bOk!=0;
}

const TCHAR* CGT_CDiagMsg::edpToStr(const CG_Edipos* pEdp, FC_CString& szedp)
{
    TCHAR   sz[2*MAX_PATH];
    boolean bOk;
    
    bOk = edpToStrBuffer(pEdp, sz, FC_ARRAY_LEN(sz), false);
    assert(bOk);
    szedp.load(sz);

    return szedp;
}

const TCHAR* CGT_CDiagMsg::edpToStrNoFile(
    const CG_Edipos* pEdp, 
    FC_CString&      szedp 
)
{
    TCHAR   sz[2*MAX_PATH];
    boolean bOk;
    
    bOk = edpToStrBuffer(pEdp, sz, FC_ARRAY_LEN(sz), true);
    assert(bOk);
    szedp.load(sz);

    return szedp;

}

