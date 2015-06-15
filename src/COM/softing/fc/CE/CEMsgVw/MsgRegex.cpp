


// MsgRegex.cpp: implementation of the CMsgRegex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fc_todebug\fc_mem.h"
#include "fc_todebug\fc_assert.h"
#include "regex.h"
#include "MsgRegex.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgRegex::CMsgRegex()
{
    m_pRegexAll = NULL;      
    m_pRegmatchAll = NULL;
    m_iGroupsAll = 0;
    m_pRegexSrc = NULL;      
    m_pRegmatchSrc = NULL;
    m_iGroupsSrc = 0;
    m_strSrcName = _T("");
    m_strSrcPos = _T("");
    m_strIdText =_T("");
    m_strDesc = _T("");
    m_iMsgType = CEMSG_TYPE_DONTKNOW;

}

CMsgRegex::~CMsgRegex()
{
    // Free all allocated memory
    if (m_pRegexAll)
    {
       regfree(m_pRegexAll);
       FC_FreeMemory(m_pRegexAll);
    }

    if (m_pRegmatchAll)
    {
        FC_FreeMemory(m_pRegmatchAll);
    }
 
    if (m_pRegexSrc)
    {
       regfree(m_pRegexSrc);
       FC_FreeMemory(m_pRegexSrc);
    }

    if (m_pRegmatchSrc)
    {
        FC_FreeMemory(m_pRegmatchSrc);
    }
}

int CMsgRegex::EvaluateMessage(LPCTSTR pszText)
{
    int iRegError;
    int iStart;
    int iEnd;
    CString strTypeText;
    CString strPosText;

    if (!m_pRegexAll)
    {
        // Compile general regular expression (matches "<source> : <error type>: <description>"
        if (!FC_NewMemory((void **)&m_pRegexAll, sizeof(*m_pRegexAll)))
        {
            FC_DEBUGPRINT(_T("CEMSG> out of memory\n"));
            return -1;
        }

        iRegError = regcomp(m_pRegexAll, CEMSG_PATTERNALL, REG_EXTENDED);
        if (iRegError)
        {
            FC_DEBUGPRINT1(_T("CEMSG> error %d compiling regular expression\n"), iRegError);
            regfree(m_pRegexAll);
            m_pRegexAll = NULL;
            return -1;
        }

        m_iGroupsAll = m_pRegexAll->re_nsub + 1;
        ASSERT (m_iGroupsAll == 5);
        if (!FC_NewMemory((void **)&m_pRegmatchAll, sizeof(*m_pRegmatchAll) * m_iGroupsAll))
        {
            FC_DEBUGPRINT(_T("CEMSG> out of memory\n"));
            regfree(m_pRegexAll);
            m_pRegexAll = NULL;
            m_iGroupsAll = 0;
            return -1;
        }
    }

    if (!m_pRegexSrc)
    {
        // Compile source regular expression (matches "<filename>(<position>)"
        if (!FC_NewMemory((void **)&m_pRegexSrc, sizeof(*m_pRegexSrc)))
        {
            FC_DEBUGPRINT(_T("CEMSG> out of memory\n"));
            return -1;
        }

        iRegError = regcomp(m_pRegexSrc, CEMSG_PATTERNSRC, REG_EXTENDED);
        if (iRegError)
        {
            FC_DEBUGPRINT1(_T("CEMSG> error %d compiling regular expression\n"), iRegError);
            regfree(m_pRegexSrc);
            m_pRegexSrc = NULL;
            return -1;
        }

        m_iGroupsSrc = m_pRegexSrc->re_nsub + 1;
        ASSERT (m_iGroupsSrc == 3);
        if (!FC_NewMemory((void **)&m_pRegmatchSrc, sizeof(*m_pRegmatchSrc) * m_iGroupsSrc))
        {
            FC_DEBUGPRINT(_T("CEMSG> out of memory\n"));
            regfree(m_pRegexSrc);
            m_pRegexSrc = NULL;
            m_iGroupsSrc = 0;
            return -1;
        }
    }

    m_strSrcName.Empty();
    m_strIdText.Empty();
    m_strDesc.Empty();
    m_strSrcPos.Empty();
    m_iMsgType = CEMSG_TYPE_DONTKNOW;
    iRegError = regexec(m_pRegexAll, pszText, m_iGroupsAll, m_pRegmatchAll, 0);
    if (!iRegError)
    {
        // extract message type
        iStart = m_pRegmatchAll[2].rm_so;
        iEnd = m_pRegmatchAll[2].rm_eo;
        if (iStart >= 0 && iStart < iEnd)
        {
            strTypeText = &pszText[iStart];
            strTypeText = strTypeText.Left(iEnd - iStart);
            if(strTypeText.CompareNoCase(CEMSG_FATAL_ERROR_TEXT) == 0)
            {
                m_iMsgType = CEMSG_TYPE_FATALERROR;
            }
            else if(strTypeText.CompareNoCase(CEMSG_ERROR_TEXT) == 0)
            {
                m_iMsgType = CEMSG_TYPE_ERROR;
            }
            else if (strTypeText.CompareNoCase(CEMSG_WARNING_TEXT) == 0)
            {
                m_iMsgType = CEMSG_TYPE_WARNING;
            }
            else
            {
                m_iMsgType = CEMSG_TYPE_DONTKNOW;
            }
        }
        
        // extract message id
        iStart = m_pRegmatchAll[3].rm_so;
        iEnd = m_pRegmatchAll[3].rm_eo;
        if (iStart >= 0 && iStart < iEnd)
        {
            m_strIdText = &pszText[iStart];
            m_strIdText = m_strIdText.Left(iEnd - iStart);
        }

        // extract description
        iStart = m_pRegmatchAll[4].rm_so;
        iEnd = m_pRegmatchAll[4].rm_eo;
        if (iStart >= 0 && iStart < iEnd)
        {
            m_strDesc = &pszText[iStart];
            m_strDesc = m_strDesc.Left(iEnd - iStart);
        }

        // extract source
        iStart = m_pRegmatchAll[1].rm_so;
        iEnd = m_pRegmatchAll[1].rm_eo;
        if (iStart >= 0 && iStart < iEnd)
        {
            CString strSrc = &pszText[iStart];
            strSrc = strSrc.Left(iEnd - iStart);
            LPCTSTR pszSrcText = strSrc;
            iRegError = regexec(m_pRegexSrc, strSrc, m_iGroupsSrc, m_pRegmatchSrc, 0);
            if (!iRegError)
            {
                // extract filename
                iStart = m_pRegmatchSrc[1].rm_so;
                iEnd = m_pRegmatchSrc[1].rm_eo;
                if (iStart >= 0 && iStart < iEnd)
                {
                    m_strSrcName = &pszSrcText[iStart];
                    m_strSrcName = m_strSrcName.Left(iEnd - iStart);
                }

                // extract position
                iStart = m_pRegmatchSrc[2].rm_so;
                iEnd = m_pRegmatchSrc[2].rm_eo;
                if (iStart >= 0 && iStart < iEnd)
                {
                    strPosText = &pszSrcText[iStart];
                    m_strSrcPos = strPosText.Left(iEnd - iStart);
                }
            }
            else
            {
                m_strSrcName = &pszText[iStart];
            }
        }
    }
    else if (REG_NOMATCH == iRegError)
    {
        iRegError = regexec(m_pRegexSrc, pszText, m_iGroupsSrc, m_pRegmatchSrc, 0);
        if (!iRegError)
        {
            // extract filename
            iStart = m_pRegmatchSrc[1].rm_so;
            iEnd = m_pRegmatchSrc[1].rm_eo;
            if (iStart >= 0 && iStart < iEnd)
            {
                m_strSrcName = &pszText[iStart];
                m_strSrcName = m_strSrcName.Left(iEnd - iStart);
            }

            // extract position
            iStart = m_pRegmatchSrc[2].rm_so;
            iEnd = m_pRegmatchSrc[2].rm_eo;
            if (iStart >= 0 && iStart < iEnd)
            {
                strPosText = &pszText[iStart];
                m_strSrcPos = strPosText.Left(iEnd - iStart);
            }

            m_iMsgType = CEMSG_TYPE_SOURCE;
        }
        else
        {
            m_iMsgType = CEMSG_TYPE_STANDARD;
        }
    }

    return 0;
}


void CMsgRegex::GetSourceName(CString & strSrcName)
{
    strSrcName = m_strSrcName;
}

void CMsgRegex::GetSourcePos(CString & strSrcPos)
{
    strSrcPos = m_strSrcPos;
}

void CMsgRegex::GetIdText(CString & strIdText)
{
    strIdText = m_strIdText;
}

void CMsgRegex::GetDescription(CString & strDesc)
{
    strDesc = m_strDesc;
}

int CMsgRegex::GetMessageType(void)
{
    return m_iMsgType;
}

