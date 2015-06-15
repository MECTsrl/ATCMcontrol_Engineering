/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/ToolBox.h 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: ToolBox.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/ToolBox.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CL
 *
 * =COMPONENT           CELibrarian
 *
 * =CURRENT      $Date: 28.02.07 18:59 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  6/25/2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __TOOLBOX_H_
#define __TOOLBOX_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//----  Aggregate Includes:   ------------------------------------------------*
#ifndef _TCHAR_DEFINED
#error This file is ANSI/UNICODE aware, include tchar.h first!!!
#endif

//----  Forward Class Definitions:   -----------------------------------------*

//----  Defines:    ----------------------------------------------------------*

//----  Globals:    ----------------------------------------------------------*

//----  Defines:    ----------------------------------------------------------*

//----  Globals:    ----------------------------------------------------------*

//----  Prototypes:    -------------------------------------------------------*
CString                     GetAppVersionInfo(WORD & wMajorVersion,
                                              WORD & wMinorVersion,
                                              WORD & wBuild);
                                // retrieve version number and version string
                                // from version-info  resource



class CRunningOS
///////////////////////////////////////////////////////////////////////////////
//  class CRunningOS
//      tell which Win32 environment we're running in
///////////////////////////////////////////////////////////////////////////////
{
// enums:
public:
    enum OsEnv {
        // Win16 only !!!
        Windows2,
        Windows30,
        WfW311,
        Windows31,
        // Win32
        Win32s,                     // WfW 3.11 with Win32s
        Windows95,                  // Win32; Win16 under Win95
        WindowsNT,                  // NT w/o common controls or Win16 under NT!
        WindowsNT351,
        WindowsNT40
    };

public:
// =Construction:
                            CRunningOS();
                           ~CRunningOS();
// =Operations:
public:

private:
    void                    SetActualEnvironment(void);

// =Access
public:
    inline enum OsEnv       GetCurrOsEnv(void) const;
    BOOL                    IsUnicodeAvailable(void) const;

// =Attributes:
protected:
    enum OsEnv              m_OsEnv;
};

/////////////////////// INLINE FUNCTION ///////////////////////////////////////
inline enum CRunningOS::OsEnv
    CRunningOS::GetCurrOsEnv(void) const
/*>>>> 

Result
  length of raw variable buffer
<<<<*/
{
    return m_OsEnv;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


class CHourglass
///////////////////////////////////////////////////////////////////////////////
//  class CHourglass
//      display hourglass while this object is alive
//      opt.: get capture to <hwndCap>
///////////////////////////////////////////////////////////////////////////////
{
public:
// =Operations:
                            CHourglass(HWND hwndCap = NULL);
                            ~CHourglass();

// =Attributes:
private:
    HWND                    m_hwndCapture;    // window to capture mouse, if any
    HCURSOR                 m_hSaveCursor;    // save cursor handle
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


class CTransBmp : public CBitmap
///////////////////////////////////////////////////////////////////////////////
//  class CTransBitmapButton
//      display transparent bitmap as ownerdrawn button in dialog
//      the transparent color is determined by the upper left pixel!
///////////////////////////////////////////////////////////////////////////////
{
public:
// =Construction
                            CTransBmp();
                            ~CTransBmp();

// =Operations
    void                    Draw(HDC hDC, int x, int y);
    void                    Draw(CDC* pDC, int x, int y);
                                // draw bitmap on DC / CDC
    void                    DrawTrans(HDC hDC, int x, int y);
    void                    DrawTrans(CDC* pDC, int x, int y);
                                // draw bitmap transparently on DC / CDC
                                // mask color is determined by upper left pixel
    int                     GetWidth();
                                // width of bitmap in pixels
    int                     GetHeight();
                                // height of bitmap in pixels

//=Helpers
private:
    void                    GetMetrics();
    void                    CreateMask(HDC hDC);

private:
    int                     m_iWidth;
    int                     m_iHeight;
                                // cache of bitmap height and width
    HBITMAP                 m_hbmMask;
                                // Handle to mask bitmap
    COLORREF                m_crTranspMask;
                                // color of transparent pixels
};


/////////////////////// INLINE FUNCTION ///////////////////////////////////////
inline void
    CTransBmp::Draw(CDC* pDC, int x, int y)
/*>>>> 
draw bitmap on given DC

I   hDC:    device context to draw on
I   x:      x/y device coordinates to draw bitmap to
I   y:

Result
  void
<<<<*/
{
    ASSERT(pDC != NULL);
    Draw(pDC->GetSafeHdc(), x, y);
}

/////////////////////// INLINE FUNCTION ///////////////////////////////////////
inline void
    CTransBmp::DrawTrans(CDC *pDC, int x, int y)
/*>>>> 
draw bitmap transparently on given DC

I   hDC:    device context to draw on
I   x:      x/y device coordinates to draw bitmap to
I   y:

Result
  void
<<<<*/
{
    ASSERT(pDC != NULL);
    DrawTrans(pDC->GetSafeHdc(), x, y);
}

  
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


class CTransBitmapButton : public CButton
///////////////////////////////////////////////////////////////////////////////
//  class CTransBitmapButton
//      display transparent bitmap as ownerdrawn button in dialog
//      the transparent color is determined by the upper left pixel!
//      this class implements a subset of MFC's CBitmapButton class
//      i.e. it works with only a single bitmap
///////////////////////////////////////////////////////////////////////////////
{
	DECLARE_DYNAMIC(CTransBitmapButton)
public:
// =Construction
	                        CTransBitmapButton();

// =Operations
	BOOL                    LoadBitmaps(LPCTSTR lpszBitmapResource);
                                // load bitmap resource
	BOOL                    LoadBitmaps(UINT nIDBitmapResource);
                                // load bitmap resource
//	BOOL                    AutoLoad(UINT nID, CWnd* pParent);
	void                    SizeToContent();
                                // size button to size of bitmap

// =Implementation:
public:

// =Overridables
	virtual void            DrawItem(LPDRAWITEMSTRUCT lpDIS);

// =Attributes
protected:
    CTransBmp               m_bitmap;
};


/////////////////////// INLINE FUNCTION ///////////////////////////////////////
inline BOOL
    CTransBitmapButton::LoadBitmaps(UINT nIDBitmapResource)
/*>>>> 
load the bitmap from resource

Result
  TRUE if bitmap found
<<<<*/
{
    return LoadBitmaps(MAKEINTRESOURCE(nIDBitmapResource));
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


#pragma warning (disable: 4097)
    // typedef-name 'NN' used as synonym for class-name 'tagNN'

class CWindowPlacement : public WINDOWPLACEMENT
///////////////////////////////////////////////////////////////////////////////
//  class CWindowPlacement
//      save and load window size and placement to/from registry or archive
///////////////////////////////////////////////////////////////////////////////
{
//=Construction
public:
                            CWindowPlacement();
                            ~CWindowPlacement();

//=Operations
public:
    void                    Save(LPCTSTR pszKeyName, CWnd* pWnd);
                                // Save window placement (to app profile)
                                // reads given window's placement
    BOOL                    Restore(LPCTSTR pszKeyName, CWnd* pWnd);
                                // Restore window placement (from app profile)
                                // actually restore placement of window

    void                    GetProfileWP(LPCTSTR pszKeyName);
                                // Read placement from app profile/registry
    void                    WriteProfileWP(LPCTSTR pszKeyName);
                                // Write placement to app profile/registry

    friend CArchive&        operator<<(CArchive& ar, const CWindowPlacement& wp);
                                // Write placement to archive
    friend CArchive&        operator>>(CArchive& ar, CWindowPlacement& wp);
                                // Read placement from archive

// =Implementation:
protected:

// =Overridables
protected:

// =Attributes
protected:
};
#pragma warning (default: 4097)
  

class CVersionInfo
///////////////////////////////////////////////////////////////////////////////
//  class CVersionInfo
//      read current app version info
///////////////////////////////////////////////////////////////////////////////
{
//=Construction
public:
                            CVersionInfo(HMODULE hModule = NULL);
                            ~CVersionInfo();

//=Operations
public:
    BOOL                    ReadInfoString(LPCTSTR pszInfoKey, CString& sInfoText);

// =Implementation:
protected:

// =Overridables
protected:

// =Attributes
protected:
    HMODULE                 m_hModule;
                                // applications module handle
    void                   *m_pVerBlock;
                                // version info area
    LPTSTR                  m_pszBaseInfoName;
};
  


/////////////////////////////////////////////////////////////////////////////


#endif // __TOOLBOX_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: ToolBox.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:33p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/
