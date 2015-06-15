/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/ToolBox.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: ToolBox.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/ToolBox.cpp $
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



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "toolBox.h"
//----  Static Initializations:   ----------------------------------*


//----  Local Defines:   -----------------------------------------------------*
// Colors
#define rgbWhite RGB(255,255,255)
// Raster op codes
#define DSa     0x008800C6L
#define DSx     0x00660046L



//----  Static Initializations:   --------------------------------------------*
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


///////////////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CRunningOS
///////////////////////////////////////////////////////////////////////////////

//----------------------------- FUNCTION -------------------------------------*
CRunningOS::CRunningOS()
/*>>>> 
ctor

Result
  void
<<<<*/
{
    SetActualEnvironment();
}

//----------------------------- FUNCTION -------------------------------------*
CRunningOS::~CRunningOS()
/*>>>> 
dtor

Result
  void
<<<<*/
{
}

//----------------------------- FUNCTION -------------------------------------*
void
    CRunningOS::SetActualEnvironment()
/*>>>> 
query version and set enumerator accordingly

Result
  void
<<<<*/
{
    DWORD   dwVersion = ::GetVersion();
            // GetVersion returns different values when 
            // compiling as Win16 or Win32 app.
    DWORD   dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
    DWORD   dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));
    DWORD   dwBuild = 0;

#ifdef _WIN32
    // compiling under Win32 ----------------------------------
    if (dwVersion < 0x80000000) {
        // must be Windows NT, decide which version:
        if (dwWindowsMajorVersion >= 0x04) {
            m_OsEnv = WindowsNT40;
        }
        else {
            if (dwWindowsMinorVersion >= 0x51) {
                m_OsEnv = WindowsNT351;
            }
            else {
                // older than NT 3.51 (=> has no common controls etc !)
                m_OsEnv = WindowsNT;
            }
        }
        dwBuild = (DWORD)(HIWORD(dwVersion));
    }
    else if (dwWindowsMajorVersion < 4) {
        // Win32s
        m_OsEnv = Win32s;
        dwBuild = (DWORD)(HIWORD(dwVersion) & ~0x8000);
    }
    else {
        // Windows 95:
        m_OsEnv = Windows95;
        dwBuild =  0;   // Windows 95 -- No build numbers provided
    }

#else   // ! _WIN32
    // compiling under Win16 ----------------------------------
    if (dwWindowsMajorVersion < 3) {
        // must be groofy old!!!!!
        m_OsEnv = Windows2;
    }
    else {
        // which Windows 3 ??
        switch (dwWindowsMinorVersion) {
        case 0:
            m_OsEnv = Windows30;
            break;
        case 10:
            // looks like Win3.1,
            //  but is it real Win16 or is it running as WoW (under NT)
            if (::GetWinFlags() & 0x4000)
                // WIN_NT (0x4000) identifies Windows NT (Windows SDK KBase, Q131371)
                m_OsEnv = WindowsNT;
            else
                m_OsEnv = Windows31;
            break;
        case 11:
            // commonly not set, since WfW uses Win3.1 signature!
            m_OsEnv = WfW311;
            break;
        case 95:
            // running as Win16 under Win95:
            m_OsEnv = Windows95;
            break;
        default:
            m_OsEnv = Windows30;
            break;
        }
    }
#endif  // ! _WIN32
}

//----------------------------- FUNCTION -------------------------------------*
BOOL
    CRunningOS::IsUnicodeAvailable(void) const
/*>>>> 

Result
  TRUE if running OS environment has UNICODE support
<<<<*/
{
    BOOL bOK = FALSE;

#ifdef _WIN32

    switch(m_OsEnv) {
    case Windows2:
    case Windows30:
    case WfW311:
    case Windows31:
    case Win32s:
    case Windows95:
        bOK = FALSE;
        break;
    case WindowsNT:
    case WindowsNT351:
    case WindowsNT40:
        bOK = TRUE;
        break;
    default:
        bOK = FALSE;
        break;
    }

    return bOK;

#else   // ! _WIN32

    return FALSE;       // Win16 apps have no UNICODE support at all!!

#endif  // ! _WIN32
}

///////////////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CHourglass
///////////////////////////////////////////////////////////////////////////////

//----------------------------- FUNCTION -------------------------------------*
CHourglass::CHourglass(HWND hwndCap /* = NULL*/)
/*>>>> 
Hourglass constructor:
Change cursor to hourglass, and capture the mouse

I   hwndCap     window wanting to capture focus

Result
  void
<<<<*/
{
   m_hSaveCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
   m_hwndCapture = hwndCap;
   if (m_hwndCapture)
      ::SetCapture(m_hwndCapture);
}

//----------------------------- FUNCTION -------------------------------------*
CHourglass::~CHourglass()
/*>>>> 
Hourglass destructur: restore original cursor.

Result
  void
<<<<*/
{
   ::SetCursor(m_hSaveCursor);
   if (m_hwndCapture)
      ::ReleaseCapture();
}




///////////////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CTransBmp
///////////////////////////////////////////////////////////////////////////////


//----------------------------- FUNCTION -------------------------------------*
CTransBmp::CTransBmp()
/*>>>> 
ctor

Result
  void
<<<<*/
  : m_hbmMask(NULL),
    m_crTranspMask(0),
    m_iWidth(0),
    m_iHeight(0)
{
}


//----------------------------- FUNCTION -------------------------------------*
CTransBmp::~CTransBmp()
/*>>>> 
dtor

Result
  void
<<<<*/
{
    if (m_hbmMask) {
        ::DeleteObject(m_hbmMask);
    }
}
  
  
//----------------------------- FUNCTION -------------------------------------*
void
    CTransBmp::GetMetrics()
/*>>>> 
update cache with bitmap width/height

Result
  void
<<<<*/
{
    // Get the width and height into cache
    ASSERT(m_hObject);
    BITMAP      bm;
    GetObject(sizeof(bm), &bm);
    m_iWidth  = bm.bmWidth;
    m_iHeight = bm.bmHeight;
}
  
  
//----------------------------- FUNCTION -------------------------------------*
int
    CTransBmp::GetWidth()
/*>>>> 

Result
  width of bitmap
<<<<*/
{
    if ((m_iWidth == 0) || (m_iHeight == 0)){
        GetMetrics();
    }
    return m_iWidth;
}

//----------------------------- FUNCTION -------------------------------------*
int
    CTransBmp::GetHeight()
/*>>>> 

Result
  height of bitmap
<<<<*/
{
    if ((m_iWidth == 0) || (m_iHeight == 0)){
        GetMetrics();
    }
    return m_iHeight;
}
  

//----------------------------- FUNCTION -------------------------------------*
void
    CTransBmp::CreateMask(HDC hDC)
/*>>>> 
create the mask for the transparent color
the upper left pixel determines the transparent color

Result
  void
<<<<*/
{
    ASSERT(m_hObject);
    ASSERT(hDC);

    // Nuke any existing mask.
    if (m_hbmMask) {
        ::DeleteObject(m_hbmMask);
    }

    // Create memory DCs to work with.
    CDC *pCDC = CDC::FromHandle(hDC);
    ASSERT(pCDC);
    CDC cdcMask;
    CDC cdcImage;
    cdcMask.CreateCompatibleDC(pCDC);
    cdcImage.CreateCompatibleDC(pCDC);

    // Create a monochrome bitmap for the mask.
    m_hbmMask = ::CreateBitmap(GetWidth(), GetHeight(), 1, 1, NULL);

    // Select the mono bitmap into its DC.
    HBITMAP hbmOldMask = (HBITMAP) cdcMask.SelectObject(m_hbmMask);

    // Select the image bitmap into its DC.
    HBITMAP hbmOldImage = (HBITMAP) cdcImage.SelectObject(m_hObject);

    // Set the transparency color to be the top-left pixel.
    m_crTranspMask = cdcImage.GetPixel(0, 0);
    cdcImage.SetBkColor(m_crTranspMask);

    // Make the mask.
    cdcMask.BitBlt(0, 0, GetWidth(), GetHeight(),
        &cdcImage, 0, 0,
        SRCCOPY);

    cdcMask.SelectObject(hbmOldMask);
    cdcImage.SelectObject(hbmOldImage);
}
  
  
//----------------------------- FUNCTION -------------------------------------*
void
    CTransBmp::Draw(HDC hDC, int x, int y)
/*>>>> 
draw bitmap on given DC

I   hDC:    device context to draw on
I   x:      x/y device coordinates to draw bitmap to
I   y:

Result
  void
<<<<*/
{
    ASSERT(hDC);
    ASSERT(m_hObject);
    // Create a memory DC.
    CDC *pCDC = CDC::FromHandle(hDC);
    ASSERT(pCDC);
    CDC cdcMem;
    cdcMem.CreateCompatibleDC(pCDC);
    // Select the bitmap into the mem DC.
    HBITMAP hbmold = 
        (HBITMAP) cdcMem.SelectObject((HBITMAP)(m_hObject));
    // Blt the bits.
    pCDC->BitBlt(x, y, GetWidth(), GetHeight(),
        &cdcMem, 0, 0,
        SRCCOPY);
    cdcMem.SelectObject(hbmold);
}
  
//----------------------------- FUNCTION -------------------------------------*
void
    CTransBmp::DrawTrans(HDC hDC, int x, int y)
/*>>>> 
draw bitmap transparently on given DC

I   hDC:    device context to draw on
I   x:      x/y device coordinates to draw bitmap to
I   y:

Result
  void
<<<<*/
{
    ASSERT(hDC);
    if (!m_hbmMask) {
        CreateMask(hDC);
    }
    ASSERT(m_hObject);
    ASSERT(m_hbmMask);

    int dx = GetWidth();
    int dy = GetHeight();

    // Create a memory DC to which to draw.
    HDC hdcOffScr = ::CreateCompatibleDC(hDC);
    // Create a bitmap for the off-screen DC that is really
    // color-compatible with the destination DC.
    HBITMAP hbmOffScr = ::CreateBitmap(dx, dy, 
                             (BYTE)GetDeviceCaps(hDC, PLANES),
                             (BYTE)GetDeviceCaps(hDC, BITSPIXEL),
                             NULL);
    // Select the buffer bitmap into the off-screen DC.
    HBITMAP hbmOldOffScr = (HBITMAP)::SelectObject(hdcOffScr, hbmOffScr);

    // Copy the image of the destination rectangle to the
    // off-screen buffer DC, so we can play with it.
    ::BitBlt(hdcOffScr, 0, 0, dx, dy,
             hDC,       x, y,
             SRCCOPY);

    // Create a memory DC for the source image.
    HDC hdcImage = ::CreateCompatibleDC(hDC); 
    HBITMAP hbmOldImage = (HBITMAP)::SelectObject(hdcImage, m_hObject);

    // Create a memory DC for the mask.
    HDC hdcMask = ::CreateCompatibleDC(hDC);
    HBITMAP hbmOldMask = (HBITMAP)::SelectObject(hdcMask, m_hbmMask);

    // XOR the image with the destination.
    ::SetBkColor(hdcOffScr, rgbWhite);
    ::BitBlt(hdcOffScr, 0, 0, dx, dy,
             hdcImage,  0, 0,
             DSx);
    // AND the destination with the mask.
    ::BitBlt(hdcOffScr, 0, 0, dx, dy,
             hdcMask,   0, 0,
             DSa);
    // XOR the destination with the image again.
    ::BitBlt(hdcOffScr, 0, 0, dx, dy,
             hdcImage,  0, 0,
             DSx);

    // Copy the resultant image back to the screen DC.
    ::BitBlt(hDC,       x, y, dx, dy,
             hdcOffScr, 0, 0,
             SRCCOPY);

    // Tidy up.
    ::SelectObject(hdcOffScr, hbmOldOffScr);
    ::SelectObject(hdcImage, hbmOldImage);
    ::SelectObject(hdcMask, hbmOldMask);
    ::DeleteObject(hbmOffScr);
    ::DeleteDC(hdcOffScr);
    ::DeleteDC(hdcImage);
    ::DeleteDC(hdcMask);
}
  

///////////////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CTransBitmapButton
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CTransBitmapButton, CButton)

//----------------------------- FUNCTION -------------------------------------*
CTransBitmapButton::CTransBitmapButton()
/*>>>> 
ctor

Result
  void
<<<<*/
{
}


//----------------------------- FUNCTION -------------------------------------*
BOOL
    CTransBitmapButton::LoadBitmaps(LPCTSTR lpszBitmapResource)
/*>>>> 
load the bitmap from resource

Result
  TRUE if bitmap found
<<<<*/
{
    // delete old bitmaps (if present)
    m_bitmap.DeleteObject();


    if (!m_bitmap.LoadBitmap(lpszBitmapResource))
    {
//        TRACE0("Failed to load bitmap.\n");
		if (HIWORD(lpszBitmapResource) != 0)
			TRACE1("ERROR: Cannot find bitmap resource named '%s'.\n",
				lpszBitmapResource);
		else
			TRACE1("ERROR: Cannot find bitmap resource with IDB 0x%04X.\n",
				LOWORD((DWORD)lpszBitmapResource));
        return FALSE;   // need this one image
    }
    BOOL bAllLoaded = TRUE;


    return bAllLoaded;
}

//----------------------------- FUNCTION -------------------------------------*
void
    CTransBitmapButton::SizeToContent()
/*>>>> 
resize the button to the size of the bitmap

Result
  void
<<<<*/
{
    ASSERT(m_bitmap.m_hObject != NULL);

    VERIFY(SetWindowPos(NULL, -1, -1,
                        m_bitmap.GetWidth(), m_bitmap.GetHeight(),
                        SWP_NOMOVE|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOACTIVATE));
}



//----------------------------- FUNCTION -------------------------------------*
void
    CTransBitmapButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
/*>>>> 
Draw the appropriate bitmap

Result
  void
<<<<*/
{
    ASSERT(lpDIS != NULL);
    // must have at least the first bitmap loaded before calling DrawItem
    if (m_bitmap.m_hObject == NULL)
    {
        ASSERT(FALSE);     // bitmap is required
        return;     // bail out w/o drawing bitmap
    }
    // Special hack for Win3.1 and WfW3.11:
    // Ctl3d.dll uses own DC and BKColor, which is always gray
    // so this DC still has the old dialog BkColor (white)
    CRunningOS          os;
    CRunningOS::OsEnv   osEnv = os.GetCurrOsEnv();
    if (   osEnv == CRunningOS::Windows30
        || osEnv == CRunningOS::WfW311
        || osEnv == CRunningOS::Windows31)
    {
        CRect   rect;
        GetWindowRect(&rect);
        ScreenToClient(&rect);
        // overpaint the white background with button color:
        HBRUSH hBk = ::CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
        ::FillRect(lpDIS->hDC, &rect, hBk);
        ::DeleteObject(hBk);

        m_bitmap.DrawTrans(lpDIS->hDC, 0, 0);
    }
    else {
        // running under Win95 or WinNT (don't care if Win16/WoW or Win32)
        m_bitmap.DrawTrans(lpDIS->hDC, 0, 0);
    }
}


///////////////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CTransBitmapButton
///////////////////////////////////////////////////////////////////////////////

//----------------------------- FUNCTION -------------------------------------*
CWindowPlacement::CWindowPlacement()
/*>>>> 
ctor

Result
  void
<<<<*/
{
    // Note: "length" is inherited from WINDOWPLACEMENT
    length = sizeof(WINDOWPLACEMENT);
    showCmd = SW_SHOWNORMAL;
}

//----------------------------- FUNCTION -------------------------------------*
CWindowPlacement::~CWindowPlacement()
/*>>>> 
dtor

Result
  void
<<<<*/
{
}

//----------------------------- FUNCTION -------------------------------------*
BOOL
    CWindowPlacement::Restore(LPCTSTR pszKeyName, CWnd* pWnd)
/*>>>> 
Restore window placement from profile key

Result
  TRUE if restore was ok
<<<<*/
{
    GetProfileWP(pszKeyName);

    // Only restore if window intersects the screen.
    //
    CRect rcTemp, rcScreen(0, 0, GetSystemMetrics(SM_CXSCREEN),
                                 GetSystemMetrics(SM_CYSCREEN));
    if (!::IntersectRect(&rcTemp, &rcNormalPosition, &rcScreen)) {
        return FALSE;
    }

    showCmd = SW_HIDE;
    pWnd->SetWindowPlacement(this);  // set placement
    return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
void
    CWindowPlacement::Save(LPCTSTR pszKeyName, CWnd* pWnd)
/*>>>> 
Save window placement in app profile

Result
  void
<<<<*/
{
    pWnd->GetWindowPlacement(this);
    WriteProfileWP(pszKeyName);
}


#define BASE_WP_KEY     _T("WinPos.")
#define WP_SHOW_CMD     BASE_WP_KEY _T("showCmd")
#define WP_FLAGS        BASE_WP_KEY _T("flags")
#define WP_MIN_POS_X    BASE_WP_KEY _T("ptMinPosition.x")
#define WP_MIN_POS_Y    BASE_WP_KEY _T("ptMinPosition.y")
#define WP_MAX_POS_X    BASE_WP_KEY _T("ptMaxPosition.x")
#define WP_MAX_POS_Y    BASE_WP_KEY _T("ptMaxPosition.y")
#define WP_LEFT         BASE_WP_KEY _T("left")
#define WP_RIGHT        BASE_WP_KEY _T("right")
#define WP_TOP          BASE_WP_KEY _T("top")
#define WP_BOTTOM       BASE_WP_KEY _T("bottom")

//----------------------------- FUNCTION -------------------------------------*
void
    CWindowPlacement::GetProfileWP(LPCTSTR pszKeyName)
/*>>>> 
Get window placement from profile.

Result
  void
<<<<*/
{
    CWinApp *pApp = AfxGetApp();
    ASSERT_VALID(pApp);

    showCmd = pApp->GetProfileInt(pszKeyName, WP_SHOW_CMD, showCmd);
    flags   = pApp->GetProfileInt(pszKeyName, WP_FLAGS, flags);

    ptMinPosition.x = pApp->GetProfileInt(pszKeyName, WP_MIN_POS_X, 
                                          ptMinPosition.x);
    ptMinPosition.y = pApp->GetProfileInt(pszKeyName, WP_MIN_POS_Y,
                                          ptMinPosition.y);
    ptMaxPosition.x = pApp->GetProfileInt(pszKeyName, WP_MAX_POS_X, 
                                          ptMaxPosition.x);
    ptMaxPosition.y = pApp->GetProfileInt(pszKeyName, WP_MAX_POS_Y,
                                          ptMaxPosition.y);

    RECT& rc  = rcNormalPosition;
    rc.left   = pApp->GetProfileInt(pszKeyName, WP_LEFT,   rc.left);
    rc.right  = pApp->GetProfileInt(pszKeyName, WP_RIGHT,  rc.right);
    rc.top    = pApp->GetProfileInt(pszKeyName, WP_TOP,    rc.top);
    rc.bottom = pApp->GetProfileInt(pszKeyName, WP_BOTTOM, rc.bottom);
}


//----------------------------- FUNCTION -------------------------------------*
void
    CWindowPlacement::WriteProfileWP(LPCTSTR pszKeyName)
/*>>>> 
Write window placement to app profile

Result
  void
<<<<*/
{
    CWinApp *pApp = AfxGetApp();
    ASSERT_VALID(pApp);
    pApp->WriteProfileInt(pszKeyName, WP_SHOW_CMD,  showCmd);
    pApp->WriteProfileInt(pszKeyName, WP_FLAGS,     flags);
    pApp->WriteProfileInt(pszKeyName, WP_MIN_POS_X, ptMinPosition.x);
    pApp->WriteProfileInt(pszKeyName, WP_MIN_POS_Y, ptMinPosition.y);
    pApp->WriteProfileInt(pszKeyName, WP_MAX_POS_X, ptMaxPosition.x);
    pApp->WriteProfileInt(pszKeyName, WP_MAX_POS_Y, ptMaxPosition.y);
    pApp->WriteProfileInt(pszKeyName, WP_LEFT,      rcNormalPosition.left);
    pApp->WriteProfileInt(pszKeyName, WP_RIGHT,     rcNormalPosition.right);
    pApp->WriteProfileInt(pszKeyName, WP_TOP,       rcNormalPosition.top);
    pApp->WriteProfileInt(pszKeyName, WP_BOTTOM,    rcNormalPosition.bottom);
}

// The ugly casts are required to help the VC++ 3.0 compiler decide which
// operator<< or operator>> to use. If you're using VC++ 4.0 or later, you 
// can delete this stuff.
//
#if (_MSC_VER < 1000)      // 1000 = VC++ 4.0
#define UINT_CAST (LONG)
#define UINT_CASTREF (LONG&)
#else
#define UINT_CAST
#define UINT_CASTREF
#endif


//----------------------------- FUNCTION -------------------------------------*
CArchive&
    operator<<(CArchive& ar, const CWindowPlacement& wp)
/*>>>> 
Write window placement to archive

I   ar: ref to archive to insert into
I   wp: window placement

Result
  ref to archive
<<<<*/
{
    ar << UINT_CAST wp.length;
    ar << UINT_CAST wp.flags;
    ar << UINT_CAST wp.showCmd;
    ar << wp.ptMinPosition;
    ar << wp.ptMaxPosition;
    ar << wp.rcNormalPosition;
    return ar;
}

//----------------------------- FUNCTION -------------------------------------*
CArchive&
    operator>>(CArchive& ar, CWindowPlacement& wp)
/*>>>> 
Read window placement from archive

I   ar: ref to archive to extract from
I   wp: window placement

Result
  ref to archive
<<<<*/
{
    ar >> UINT_CASTREF wp.length;
    ar >> UINT_CASTREF wp.flags;
    ar >> UINT_CASTREF wp.showCmd;
    ar >> wp.ptMinPosition;
    ar >> wp.ptMaxPosition;
    ar >> wp.rcNormalPosition;
    return ar;
}


#ifdef _WIN32
#pragma comment(lib, "version.lib")
#endif
//----------------------------- FUNCTION -------------------------------------*
CString
    GetAppVersionInfo(WORD & wMajorVersion,
                      WORD & wMinorVersion, WORD & wBuild)
/*>>>> 
retrieve version number and version string from version-info  resource

O   wMajorVersion:      major version number
O   wMinorVersion:      minor version number
O   wBuild:             build number

Result
  CString with version string
<<<<*/
{
    // get full path to EXE !!
    CString szAppName;
    LPTSTR psz = szAppName.GetBuffer((_MAX_PATH + 1) * sizeof(TCHAR));

    ::GetModuleFileName(AfxGetInstanceHandle(), psz, _MAX_PATH);

    LPCTSTR pszExe = szAppName;
    CString sVer(_T(""));

    wMajorVersion   = 0;
    wMinorVersion   = 0;
    wBuild          = 0;
    DWORD dwDummy = 0;
    DWORD dwVerInfoSize =
        ::GetFileVersionInfoSize((LPTSTR)pszExe, &dwDummy);

    if (dwVerInfoSize)
    {
        void *  pVerBlock = new BYTE[dwVerInfoSize];
        ASSERT(pVerBlock);

        // Get the info and fill in the pertinent dialog components
        if(::GetFileVersionInfo((LPTSTR) pszExe, 0L, dwVerInfoSize, pVerBlock)) {
            VS_FIXEDFILEINFO    *pVer = NULL;
            LPVOID  lpEntry = 0;
            UINT    iBytes = 0;

            ::VerQueryValue(pVerBlock,  _T("\\"), 
                            &lpEntry, &iBytes); 
            pVer = (VS_FIXEDFILEINFO*) lpEntry;
            wMajorVersion   = HIWORD(pVer->dwFileVersionMS);
            wMinorVersion   = LOWORD(pVer->dwFileVersionMS);
            wBuild          = LOWORD(pVer->dwFileVersionLS);

            ::VerQueryValue(pVerBlock,  _T("\\VarFileInfo\\Translation"), 
                            &lpEntry, &iBytes); 
            

            ::VerQueryValue(pVerBlock,  _T("\\StringFileInfo\\040904E4\\FileVersion"), 
                            &lpEntry, &iBytes); 

            if (iBytes > 0) {
                sVer = (LPCTSTR) lpEntry;
            }
        }
        delete [] pVerBlock;
    }
    if (sVer.GetLength() <= 0) {
        sVer = _T("No version info available!");
    }

    return sVer;
}

///////////////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CVersionInfo
///////////////////////////////////////////////////////////////////////////////

//----------------------------- FUNCTION -------------------------------------*
CVersionInfo::CVersionInfo(HMODULE hModule /*= NULL*/)
/*>>>> 
ctor

Result
  void
<<<<*/
  : m_pVerBlock(NULL),
    m_pszBaseInfoName(NULL)
{
    if (hModule == NULL) {
        m_hModule = ::AfxGetInstanceHandle();
    }
    if (m_hModule != NULL) {
        TCHAR   szName[_MAX_PATH + 4*sizeof(TCHAR)];
        BOOL    bRet = FALSE;
        
        DWORD dwRet = ::GetModuleFileName(hModule, szName, sizeof(szName)/sizeof(TCHAR));
        if (dwRet == 0) {
            return;
        }
        
        DWORD dwDummy = 0;    
        DWORD dwVerInfoSize =
            ::GetFileVersionInfoSize((LPTSTR) szName, &dwDummy);
        if (dwVerInfoSize)
        {
            m_pVerBlock = new BYTE[dwVerInfoSize];
//            ASSERT(m_pVerBlock != NULL);
            
            // Get the version info area
            if(::GetFileVersionInfo((LPTSTR) szName, 0L, dwVerInfoSize, m_pVerBlock))
            {
                // read translations:
                LPVOID  lpEntry = 0;
                UINT    iBytes = 0;
                ::VerQueryValue(m_pVerBlock,  _T("\\VarFileInfo\\Translation"),
                            &lpEntry, &iBytes);
                struct LANG_N_CP {
                    WORD    lcid;
                    WORD    cp;
                };
            
                // get first translation:
                if (iBytes > 0) {
                    int nTranslations = iBytes / sizeof(struct LANG_N_CP);
                    struct LANG_N_CP *pTable = (struct LANG_N_CP*) lpEntry;
                    // build string for translated file info table:
                    m_pszBaseInfoName = new TCHAR[_MAX_PATH];
                    wsprintf(m_pszBaseInfoName, _T("\\StringFileInfo\\%04X%04X\\"),
                        pTable[0].lcid, pTable[0].cp);
                }
            }
            else
            {
                // oops, no version info !
                delete [] m_pVerBlock;
                m_pVerBlock = NULL;
                return;
            }
        }
    }
}


//----------------------------- FUNCTION -------------------------------------*
BOOL CVersionInfo::ReadInfoString(LPCTSTR pszInfoKey, CString& sInfoText)
/*>>>> 
dtor

Result
  void
<<<<*/
{
    if (   (m_pszBaseInfoName == NULL)
        || (m_pVerBlock == NULL))
    {
        sInfoText = _T("");
        return FALSE;
    }

    CString sInfoName;
    sInfoName  = m_pszBaseInfoName;
    sInfoName += pszInfoKey;
    LPVOID  lpEntry = 0;
    UINT    iBytes = 0;

    BOOL bFound = ::VerQueryValue(m_pVerBlock, (LPTSTR)(LPCTSTR) sInfoName, &lpEntry, &iBytes);
    if (bFound) {
        if (iBytes > 0) {
            // show string
            sInfoText = (LPCTSTR) lpEntry;
        }
        else {
            sInfoText = _T("[+]");
        }
    }
    return bFound;
}

//----------------------------- FUNCTION -------------------------------------*
CVersionInfo::~CVersionInfo()
/*>>>> 
dtor

Result
  void
<<<<*/
{
    if (m_pVerBlock != NULL) {
        delete [] m_pVerBlock;
        m_pVerBlock = NULL;
    }
    if (m_pszBaseInfoName != NULL) {
        delete [] m_pszBaseInfoName;
        m_pszBaseInfoName = NULL;
    }
}

/*
 *----------------------------------------------------------------------------*
 *  $History: ToolBox.cpp $
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
 * User: Oh           Date: 6/25/01    Time: 3:32p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/
