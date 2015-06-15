
#include "stdafx.h"
#include <mmsystem.h>
#include "trailer.h"

//----  Local Defines:   -----------------------------------------------------*
typedef
  struct tagT_INSTANCE
    {
      HFONT     hFont;
      int       nWidth, nHeight;
      int       nTopLine;
      int       nLineHeight;
      int       nLineOffset;
      CFont     *pTitleFont;
      CFont     *pStdFont;
      CFont     *pItalicFont;
    } T_INSTANCE, FAR *LPT_INSTANCE;


static BOOL bRegisteredClass = FALSE;


typedef enum _TRAILER_FATTR
{
    normal,
    italic,
    title
} TRAILER_FATTR;

typedef struct _TRAILER_ENTRY
{
    COLORREF        clrText;    // -1 means: use system default color
    TRAILER_FATTR   fontAttr;
    TCHAR*          pszTextLine;
} TRAILER_ENTRY;


#define HEADING1        RGB(128, 0, 0)
#define HEADING2        RGB(0, 0, 128)
#define DEFAULT         (COLORREF) -1

//----  Static Initializations:   --------------------------------------------*
static TRAILER_ENTRY  Trailer[] =
{
   HEADING1, title,  _T("WWW.4CONTROL.DE"),
   HEADING1, title,  _T("proudly presents"),
   DEFAULT , normal,  _T(""),
   DEFAULT , title,  _T("The Version V2.0 Development Team"),
   DEFAULT , normal,  _T("(in alphabetical order)"),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T("Bernd Edlinger"),
   DEFAULT , normal,  _T("Frank Edlinger"),
   DEFAULT , normal,  _T("Eduard Himmelsdorfer"),
   DEFAULT , normal,  _T("Dirk Jung"),
   DEFAULT , normal,  _T("Thomas Kappen"),
   DEFAULT , normal,  _T("Uwe Kellmann"),
   DEFAULT , normal,  _T("Katrin Kunitz"),
   DEFAULT , normal,  _T("Achim Liebl"),
   DEFAULT , normal,  _T("Wolfram Lüders"),
   DEFAULT , normal,  _T("Hassen Ouni"),
   DEFAULT , normal,  _T("Andre Oksas"),
   DEFAULT , normal,  _T("Thomas Reich"),
   DEFAULT , normal,  _T("Norbert Schmechel"),
   DEFAULT , normal,  _T("Dirk Schmidt"),
   DEFAULT , normal,  _T("Stefan Sixt"),
   DEFAULT , normal,  _T("Michael von le Suire"),
   DEFAULT , normal,  _T("Thomas Will"),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T("in Munich and Ratingen."),
   DEFAULT , normal,  _T(""),
   DEFAULT , italic,  _T("Their Creativity, Competence, Cooperation are the basis "),
   DEFAULT , italic,  _T("for the success of ATCMControl"),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T(""),

   HEADING2, title,  _T("Special thanks ..."),
   DEFAULT , normal,  _T(""),
   DEFAULT , title,  _T("... to our friends in Dresden"),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T("Heiko Auerswald"),
   DEFAULT , normal,  _T("Lutz Pietschmann"),
   DEFAULT , normal,  _T("Stefan Rößner"),
   DEFAULT , normal,  _T("René Weber"),
   DEFAULT , normal,  _T(""),
   DEFAULT , italic,  _T("for enlightening our product with graphic editors."),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T(""),
   DEFAULT , title,  _T("... to our graphic consultants"),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T("Matthias Mielchen"),
   DEFAULT , normal,  _T("Norbert Podlesny"),
   DEFAULT , normal,  _T(""),
   DEFAULT , italic,  _T("for their tool support."),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T(""),
   DEFAULT , title,  _T("... to our application partners"),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T("Götz Hofmann"),
   DEFAULT , normal,  _T("Thorsten Sandow"),
   DEFAULT , normal,  _T("Reinhard Seif"),
   DEFAULT , normal,  _T("Michael Ruppelt"),
   DEFAULT , normal,  _T(""),
   DEFAULT , italic,  _T("for torturing our software."),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T(""),
   DEFAULT , title,  _T("... to our managers"),
   DEFAULT , normal,  _T(""),
   DEFAULT , italic,  _T("This project wouldn't have ever happened "),
   DEFAULT , italic,  _T("without the initiative and staying power of"),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T("Eduard Himmelsdorfer"),
   DEFAULT , normal,  _T("Dr. Rainer Mittmann"),
   DEFAULT , normal,  _T("Dr. Manfred Patz"),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T(""),
   DEFAULT , title,  _T("At last some thanks to our near and far competitors..."),
   DEFAULT , normal,  _T(""),
   DEFAULT , italic,  _T("... You show us, that we are on the right way."),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T(""),

   DEFAULT , title,  _T("Some words of wisdom which guided"),
   DEFAULT , title,  _T("our development team through many mishaps..."),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T("Anything that happens, happens."),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T("Anything that, in happening, causes something else to"),
   DEFAULT , normal,  _T("happen, causes something else to happen."),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T("Anything that, in happening, causes itself to happen again,"),
   DEFAULT , normal,  _T("happens again."),
   DEFAULT , normal,  _T(""),
   DEFAULT , normal,  _T("It doesn't necessarily do it in chronological order, though."),
   DEFAULT , normal,  _T(""),
   DEFAULT , italic,  _T("From: \"Mostly Harmless\""),
   DEFAULT , italic,  _T("in"),
   DEFAULT , italic,  _T("\"The Hitch Hiker's Guide to the Galaxy\""),
   DEFAULT , italic,  _T("by Douglas Adams"),
   DEFAULT , normal,  _T("")
};

#define nMaxLine        (sizeof(Trailer)/sizeof(Trailer[0]))

#define TRAILER_CLASS	_T("TrailerClass")

static BOOL RegisterTrailerClass(HINSTANCE hInst);



//------------------------------------------------------------------*
/**
 * Function name			StartTrailer
 * Description		start the trailer (shows the scrolling texts, play the music	
 * @param			CWnd *pParent
 * @return			void 
 * @exception			
 * @see			
*/
void StartTrailer(CWnd *pParent) {
    if(!bRegisteredClass) {
        RegisterTrailerClass(GetModuleHandle(NULL));
        bRegisteredClass = TRUE;
    }

    CRect   rc;
    pParent->GetClientRect(rc);
    rc.DeflateRect(5, 15, 5, 5);

    HWND hTrailer = ::CreateWindow(TRAILER_CLASS, NULL,
        WS_CHILD|WS_VISIBLE,
        rc.left, rc.top,
        rc.Width(), rc.Height(),
        pParent->m_hWnd, 0, GetModuleHandle(NULL), NULL);

    TCHAR szPath[_MAX_PATH];
    ::GetModuleFileName(AfxGetInstanceHandle(), szPath, _MAX_PATH);
    LPTSTR  pszBase = _tcsrchr(szPath, _T('\\'));
    if(pszBase != NULL) {
        *pszBase = 0;
    }

    TCHAR szMciOpen[2*_MAX_PATH];
    int  nFile = 1+(GetTickCount()/50 % 9);

    wsprintf(szMciOpen, _T("open \"%s\\cred%d.4cw\" type sequencer alias midi"), szPath, nFile);
    mciSendString(szMciOpen, NULL, 0, NULL);
    mciSendString(_T("play midi notify"), NULL, 0, hTrailer);
}


//------------------------------------------------------------------*
/**
 * Function name			TrailerWndProc
 * Description		Window Procedure for the trailer window	
 * @param			HWND hWnd
 * @param			UINT message
 * @param			WPARAM wParam
 * @param			LPARAM lParam
 * @return			LRESULT WINAPI 
 * @exception			
 * @see			
*/
LRESULT WINAPI TrailerWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    LPT_INSTANCE lpInst = (LPT_INSTANCE)GetWindowLong(hWnd, 0);
    switch(message) {
        case WM_CREATE: 
            {
                lpInst = new T_INSTANCE;
                if(lpInst == NULL) {
                    return -1;
                }
                SetWindowLong(hWnd, 0, (LONG)lpInst);
                lpInst->hFont    = NULL;
                lpInst->nWidth   = 0;
                lpInst->nHeight  = 0;
                lpInst->nTopLine = 0;
                lpInst->nLineHeight = HIWORD(GetDialogBaseUnits());
                lpInst->nLineOffset = 0;
                lpInst->pTitleFont = NULL;
                lpInst->pStdFont = NULL;
                lpInst->pItalicFont = NULL;

                // standard font..
                LOGFONT logfont;
                memset(&logfont, 0, sizeof(logfont));
                logfont.lfHeight = 16;
                logfont.lfWeight = FW_NORMAL;
                logfont.lfItalic = FALSE;
                logfont.lfPitchAndFamily = FF_SWISS;
	            lstrcpy(logfont.lfFaceName, _T("Tahoma"));
        
                CFont   *pFont = new CFont;
                ASSERT(pFont != NULL);
                BOOL bOK = pFont->CreateFontIndirect(&logfont);
                if(bOK) {
                    lpInst->pStdFont = pFont;
                }

                // italic variant..
                memset(&logfont, 0, sizeof(logfont));
                logfont.lfHeight = 16;
                logfont.lfWeight = FW_NORMAL;
                logfont.lfItalic = TRUE;
                logfont.lfPitchAndFamily = FF_SWISS;
	            lstrcpy(logfont.lfFaceName, _T("Tahoma"));
        
                pFont = new CFont;
                ASSERT(pFont != NULL);
                bOK = pFont->CreateFontIndirect(&logfont);
                if(bOK) {
                    lpInst->pItalicFont = pFont;
                }

	            memset(&logfont, 0, sizeof(logfont));
	            logfont.lfHeight = 18;
	            logfont.lfWeight = FW_BOLD;
                logfont.lfPitchAndFamily = FF_SWISS;
	            lstrcpy(logfont.lfFaceName, _T("Tahoma"));

                pFont = new CFont;
                ASSERT(pFont != NULL);
                bOK = pFont->CreateFontIndirect(&logfont);
                if(bOK) {
                    lpInst->pTitleFont = pFont;
                }

                HDC hDC = GetDC(hWnd);
                TEXTMETRIC tm;
                HFONT hOld = (HFONT) SelectObject(hDC, (HFONT) *lpInst->pTitleFont);
                if(hOld != NULL) {
                    if(GetTextMetrics(hDC, &tm)) {
                        lpInst->nLineHeight = tm.tmHeight;
                    }
                    SelectObject(hDC, hOld);
                }
        
                SetTimer(hWnd,0,40,NULL);
            }
            break;
        case WM_SIZE:
            lpInst->nWidth  = LOWORD(lParam);
            lpInst->nHeight = HIWORD(lParam);
            lpInst->nTopLine = 0;
            lpInst->nLineOffset = lpInst->nHeight;
            break;
        case WM_TIMER:
            if(--lpInst->nLineOffset <= -lpInst->nLineHeight) {
                lpInst->nLineOffset += lpInst->nLineHeight;
                if(++lpInst->nTopLine == nMaxLine) {
                    lpInst->nTopLine = 0;
                    lpInst->nLineOffset = lpInst->nHeight;
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;
                }
            }
            ScrollWindow(hWnd, 0, -1, NULL, NULL);
            UpdateWindow(hWnd);
            break;
        case WM_PAINT: 
            {
                PAINTSTRUCT ps;
                HDC hDC = BeginPaint(hWnd, &ps);
                int i;
                if(lpInst->hFont != NULL) {
                    SelectObject(hDC, lpInst->hFont);
                }
                SetTextAlign(hDC, TA_CENTER|TA_TOP);
                SetTextColor(hDC, GetSysColor(COLOR_BTNTEXT));
                SetBkColor(hDC, GetSysColor(COLOR_BTNFACE));
                for(i=lpInst->nTopLine; i<nMaxLine; i++) {
                    int y = (i-lpInst->nTopLine) * lpInst->nLineHeight + lpInst->nLineOffset;
                    if (y >= ps.rcPaint.bottom)
                        break;
                    if (ps.rcPaint.top >= y + lpInst->nLineHeight)
                        continue;
                    if (Trailer[i].pszTextLine != NULL) {
                        // prepare for displaying...
                        HFONT   hOld = NULL;
                        if(Trailer[i].fontAttr != normal) {
                            if(Trailer[i].fontAttr == title) {
                                hOld = (HFONT) SelectObject(hDC, (HFONT) *lpInst->pTitleFont);
                            }
                            if(Trailer[i].fontAttr == italic) {
                                hOld = (HFONT) SelectObject(hDC, (HFONT) *lpInst->pItalicFont);
                            }
                        } else {
                            hOld = (HFONT) SelectObject(hDC, (HFONT) *lpInst->pStdFont);
                        }
                        COLORREF clrOld = NULL;
                        if(Trailer[i].clrText != DEFAULT) {
                            clrOld = SetTextColor(hDC, Trailer[i].clrText);
                        }
/*                    
                        // some colored background
                        if((i % 2) == 0) {
                            HFONT hSave = (HFONT) SelectObject(hDC, (HFONT) *lpInst->pTitleFont);
                            COLORREF clrSave = SetTextColor(hDC, RGB(8, 222, 165));
                            TextOut(hDC, lpInst->nWidth/20, y, _T("4CONTROL"), lstrlen(_T("4CONTROL")));
                            
                            UINT  alignSave = SetTextAlign(hDC, TA_CENTER|TA_TOP|TA_UPDATECP);
                            SetTextColor(hDC, RGB(231, 16, 132));
                            TextOut(hDC, lpInst->nWidth/20, y, _T("c"), lstrlen(_T("c")));
                            
                            SetTextColor(hDC, clrSave);
                            SelectObject(hDC, hSave);
                            SetTextAlign(hDC, alignSave);
                        }
*/
                        // actually draw text
                        TextOut(hDC, lpInst->nWidth/2, y, Trailer[i].pszTextLine, lstrlen(Trailer[i].pszTextLine));

                        // restore settings
                        if(hOld != NULL) {
                            SelectObject(hDC, hOld);
                        }
                        if(clrOld != NULL) {
                            SetTextColor(hDC, clrOld);
                        }

                    }
                }
                EndPaint(hWnd, &ps);
            }
            break;
        case WM_SETFONT:
            lpInst->hFont = (HFONT)wParam;
            if(lpInst->hFont!=NULL) {
                HDC hDC = GetDC(hWnd);
                TEXTMETRIC tm;
                SelectObject(hDC, lpInst->hFont);
                if(GetTextMetrics(hDC, &tm)) {
                    lpInst->nLineHeight = tm.tmHeight;
                }
            
                ReleaseDC(hWnd, hDC);
            } else {
                lpInst->nLineHeight = HIWORD(GetDialogBaseUnits());
            }
            if(LOWORD(lParam)) {
                InvalidateRect(hWnd, NULL, TRUE);
                UpdateWindow(hWnd);
            }
            break;
        case WM_GETFONT:
            return (LRESULT)(UINT)lpInst->hFont;
        case MM_MCINOTIFY:
            if(wParam == MCI_NOTIFY_SUCCESSFUL) {
                mciSendString(_T("play midi from 0 notify"), NULL, 0, hWnd);
            }
            break;
        case WM_DESTROY:
            KillTimer(hWnd,0);
            mciSendString(_T("close midi"), NULL, 0, NULL);
            if(lpInst->pStdFont != NULL) {
                delete lpInst->pStdFont;
            }
            if(lpInst->pItalicFont != NULL) {
                delete lpInst->pItalicFont;
            }
            if(lpInst->pTitleFont != NULL) {
                delete lpInst->pTitleFont;
            }
            if(lpInst != NULL) {
                delete lpInst;
                SetWindowLong(hWnd, 0, NULL);
            }
            break;
        default:
            return(DefWindowProc(hWnd, message, wParam, lParam));
    }
    return 0L;
}


//------------------------------------------------------------------*
/**
 * Function name			RegisterTrailerClass
 * Description		register the trailer window class	
 * @param			HINSTANCE hInst
 * @return			BOOL 
 * @exception			
 * @see			
*/
BOOL RegisterTrailerClass(HINSTANCE hInst) {
  WNDCLASS wc;
  wc.style       = CS_HREDRAW|CS_VREDRAW;
  wc.lpfnWndProc = TrailerWndProc;
  wc.cbClsExtra  = 0;
  wc.cbWndExtra  = sizeof(LPT_INSTANCE);
  wc.hInstance   = hInst;
  wc.hIcon       = NULL;
  wc.hCursor     = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
  wc.lpszMenuName  = NULL;
  wc.lpszClassName = TRAILER_CLASS;
  return(RegisterClass(&wc));
}

