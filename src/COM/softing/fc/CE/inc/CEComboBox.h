#ifndef __CECOMBOBOX_H_
#define __CECOMBOBOX_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "AfxWin.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CCEComboBox : public CComboBox  
{
public:
	CCEComboBox();
	virtual ~CCEComboBox();

    
protected:
    
    DECLARE_DYNCREATE(CCEComboBox)

    // Generated message map functions
    //{{AFX_MSG(CCEComboBox)
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	//}}AFX_MSG

    //{{AFX_VIRTUAL(CCEComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

    DECLARE_MESSAGE_MAP()

protected:
    HACCEL  m_hAccelerators;

};

#endif // __CECOMBOBOX_H_