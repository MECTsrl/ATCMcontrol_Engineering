#include "MsgTabCtr.h"

// MsgTabWnd.h: interface for the CMsgTabWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTABWND_H__D9E1A787_0FDD_49E3_AC23_8529C8E70447__INCLUDED_)
#define AFX_MSGTABWND_H__D9E1A787_0FDD_49E3_AC23_8529C8E70447__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMsgTabWnd : public SECTabWnd   
{
public:
	CMsgTabWnd();
	virtual ~CMsgTabWnd();
//implimentation
public:
    void SetTabIcon(int nIndex, HICON hIcon);
    void SetTabColor(int nIndex, COLORREF Color);
    BOOL CreateTabCtrl(DWORD dwStyle, UINT nID);
protected:
    //CMsgTabCtr* m_pTabCtrl;

};

#endif // !defined(AFX_MSGTABWND_H__D9E1A787_0FDD_49E3_AC23_8529C8E70447__INCLUDED_)


