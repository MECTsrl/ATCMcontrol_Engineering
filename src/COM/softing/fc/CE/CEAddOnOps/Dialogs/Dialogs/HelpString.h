// HelpString.h: interface for the CHelpString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELPSTRING_H__A378F7DE_7B6E_4FD4_8FED_0C027C4AFDFB__INCLUDED_)
#define AFX_HELPSTRING_H__A378F7DE_7B6E_4FD4_8FED_0C027C4AFDFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHelpString : public CString  
{
public:
	CHelpString(UINT nID);
	virtual ~CHelpString();

};

#endif // !defined(AFX_HELPSTRING_H__A378F7DE_7B6E_4FD4_8FED_0C027C4AFDFB__INCLUDED_)
