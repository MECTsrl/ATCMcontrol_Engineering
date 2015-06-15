// 
// 
// Stingray Software Extension Classes
// Copyright (C) 1995 Stingray Software Inc.
// All Rights Reserved
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//
//  Author:		Scot Wingo
//  Description:	sectod.h - Definition of SECTipOfDay
//  Created:		July 25 1995
//
//  CHANGELOG:
//
//    MSW    9/30/95    Code Review
//
//
//

#ifndef __SECTOD_H__
#define __SECTOD_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

//
// SEC Extension DLL
// Initialize declaration context
//

#ifdef _SECDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL

#define _MAX_TIP 512

/////////////////////////////
// AutoDuck tag block block for SECTipOfDay 

//@doc SECTipOfDay
//@mfunc int   | SECTipOfDay |    GetCurrentTip | Call this method to get the tip index value which you need to save in your
// profile/registry so that the user does not see repeats of the tips.


//@doc SECTipOfDay
//@mfunc LPCTSTR   | SECTipOfDay | GetCurrentFileName | Returns the name of the current tip file as specified in the constructor.


//@doc SECTipOfDay
//@mfunc BOOL   | SECTipOfDay |   GetShowAtStartup | Returns the current state of the internal variable that controls if the show
// at startup check box will be checked or not. Call this routine to see if you should show the tip of the day dialog or not.
// This value is considered part of the state of the class and should be stored in the profile/registry.


//@doc SECTipOfDay
//@mfunc void   | SECTipOfDay |   SetCurrentTip | Use this to reset the tips or to initialize a new tip file.
//@parm int  | nNewTip | Specifies a new tip number for the next tip to be read from the .tip file.


//@doc SECTipOfDay
//@mfunc void   | SECTipOfDay |   SetTipIcon | Specifies a new icon for the tip of the day window.  The default is a light bulb 
// similar to the one found in MS Word.
//@parm UINT  | nNewIconID | The resource ID of the new tip icon.


//@doc SECTipOfDay
//@mfunc void   | SECTipOfDay |   SetLeadInFont |Lets you change the font of the "Did you know..." lead-in text.
//@parm CFont *  | pNewLeadInFont | A CFont pointer that specifies the new lead-in font.

//@doc SECTipOfDay
//@mfunc void   | SECTipOfDay |   SetTipFont | Lets you change the font of the tip text.  Tips are
// automatically clipped to fit in the tip window.
//@parm CFont *  | pNewTipFont | A CFont pointer that specifies the new tip font.

//@doc SECTipOfDay
//@mfunc virtual void  | SECTipOfDay | OnHelp | Override this member function to respond to the tip of the day help button.




// adinsert AutoDuck insertion point for SECTipOfDay 
//@doc SECTipOfDay 
//@class The SECTipOfDay class provides a customizable tip of the day dialog similar to those
// found in Microsoft Word and Windows 95.  Tip (.tip) files are plain ASCII files that can be easily
// created and modified by the end user.
// Customizations include: changing the icon, tip font, dialog title, lead-in text, and total button manipulation.
// SECTipOfDay has a ‘show at startup’ check box and other state information such as the current tip which
// the user must store in the profile/registry.
//@comm The TODTEST sample in the \SAMPLES\TOOLKIT\STANDARD\UIEXT\TODTEST directory demonstrates how to create both modal and
// modeless tip of the day dialogs, and how to store/restore state information from the profile.
// Also, full customization is demonstrated.
//@BASE public|CDialog
class SECTipOfDay : public CDialog
{
    DECLARE_DYNAMIC(SECTipOfDay)
    
// Construction
//@access Public Members
public:
   
	//@cmember
	/* The SECTipOfDay constructor creates the SECTipOfDay object.*/
	SECTipOfDay(const TCHAR * lpszTipFileName,
		int nTip = 1,
		BOOL bShowAtStart = 1,
		CWnd* pParent = NULL);

	//@cmember
	/* This function creates a modeless tip of the day dialog.*/
    virtual BOOL Create();
    
//Operations


	//@cmember
	/* Returns the tip index for the current tip.*/
    int     GetCurrentTip() {return m_nCurrentTip;}
	//@cmember
	/* Returns the name of the current tip file.*/
    LPCTSTR  GetCurrentFileName() {return m_lpTipFileName;}
    
	//@cmember
	/* Gets the setting of the internal state which controls if the show at startup check box */
	// will be shown or not.
    BOOL    GetShowAtStartup() {return m_bShowState;}    
	//@cmember
	/* Sets the current tip number.*/
    void    SetCurrentTip(int nNewTip) {m_nCurrentTip = nNewTip;}
	//@cmember
	/* Can be called to customize the tip icon. Default is Word-like light bulb.*/
    void    SetTipIcon(UINT nNewIconID) {m_nIconID = nNewIconID;}    
	//@cmember
	/* Called to customize the lead-in text.*/
    void    SetLeadInText(const TCHAR * lpNewLeadInText);
	//@cmember
	/* Called to customize the lead-in text font.*/
    void    SetLeadInFont(CFont * pNewLeadInFont) {m_pLeadInFont = pNewLeadInFont;}
	//@cmember
	/* Sets the font for the tip text.*/
    void    SetTipFont(CFont * pNewTipFont) {m_pTipFont = pNewTipFont;}
    
// Overrides
    //Place holder  
	//@cmember
	/* Override to hook in your on-line help.*/
    virtual void OnHelp() {return;} 
	//@cmember
	/* Override to change or add initialization.*/
    virtual int  OnInitDialog();
    
// Implementation
protected:
    BOOL    m_bShowState;
    int     m_nCurrentTip;
    int     m_nTipsInFile;
    const TCHAR *  m_lpTipFileName;
    TCHAR   m_szCurrentTip[_MAX_TIP];
    CRect   m_rectTip;
    
    //Customizeable values
    TCHAR  * m_lpLeadInText;
    CFont * m_pTipFont;
    CFont * m_pLeadInFont;
    UINT    m_nIconID;

    //Internal routines
    void LoadTip(int nTipToLoad);
    void RedrawTip();
    void CountTips();
public:
    virtual ~SECTipOfDay();
#ifdef _DEBUG
    void Dump(CDumpContext & dc) const;
    void AssertValid() const;
#endif
    // Message map functions
    afx_msg void OnNextTip();
    afx_msg void OnPreviousTips();
    afx_msg void OnPaint();
    afx_msg void OnShowButtonChanged();
    
    DECLARE_MESSAGE_MAP()
};

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif //__SECTOD_H__

