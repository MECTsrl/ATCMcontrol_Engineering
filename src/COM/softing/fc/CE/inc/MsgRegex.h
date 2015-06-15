// MsgRegex.h: interface for the CMsgRegex class.
//
//////////////////////////////////////////////////////////////////////
#include "regex.h"

#if !defined(AFX_MSGREGEX_H__B6C0FAE3_C9F3_11D1_AE4F_00A0245BBCC2__INCLUDED_)
#define AFX_MSGREGEX_H__B6C0FAE3_C9F3_11D1_AE4F_00A0245BBCC2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// message patterns (\-[0-9]+)
#define CEMSG_PATTERNALL _T("(.+[ ]*\\:) ([a-zA-Z ]+) ([A-Z][0-9][0-9][0-9][0-9])\\: (.+)")
#define CEMSG_PATTERNSRC _T("([^()]+)\\(([-a-z0-9,]+)\\)[ ]*\\:")

// message type defines
#define CEMSG_TYPE_DONTKNOW      0
#define CEMSG_TYPE_STANDARD      1
#define CEMSG_TYPE_ERROR         2
#define CEMSG_TYPE_FATALERROR    3
#define CEMSG_TYPE_WARNING       4
#define CEMSG_TYPE_SOURCE        5

#define CEMSG_FATAL_ERROR_TEXT  _T("fatal error")
#define CEMSG_ERROR_TEXT        _T("error")
#define CEMSG_WARNING_TEXT      _T("warning")


class CMsgRegex  
{
public:
	CMsgRegex();
	virtual ~CMsgRegex();
    int EvaluateMessage(LPCTSTR pszText); 
    void GetSourceName(CString & strSrcName);
    void GetSourcePos(CString & strSrcName);
    void GetIdText(CString & strIdText);
    void GetDescription(CString & strDesc);
    int GetMessageType(void);


protected:
    regex_t    * m_pRegexAll;      
    regmatch_t * m_pRegmatchAll;
    int          m_iGroupsAll;
    regex_t    * m_pRegexSrc;      
    regmatch_t * m_pRegmatchSrc;
    int          m_iGroupsSrc;
    CString      m_strSrcName;
    CString      m_strSrcPos;
    CString      m_strIdText;
    CString      m_strDesc;
    int          m_iMsgType;
};

#endif // !defined(AFX_MSGREGEX_H__B6C0FAE3_C9F3_11D1_AE4F_00A0245BBCC2__INCLUDED_)


