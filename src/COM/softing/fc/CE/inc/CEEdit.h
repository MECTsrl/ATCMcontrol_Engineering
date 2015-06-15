#ifndef __CEEDIT_H_
#define __CEEDIT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "AfxWin.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CCEEdit : public CEdit  
{
public:
	CCEEdit();
	virtual ~CCEEdit();

    
protected:
    
    DECLARE_DYNCREATE(CCEEdit)

    // Generated message map functions
    //{{AFX_MSG(CCEEdit)
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	//}}AFX_MSG

    //{{AFX_VIRTUAL(CCEEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

    DECLARE_MESSAGE_MAP()

protected:
    HACCEL  m_hAccelerators;

};

#endif // __CEEDIT_H_

