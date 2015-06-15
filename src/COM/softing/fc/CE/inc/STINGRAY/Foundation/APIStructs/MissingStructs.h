////////////////////////////////////////////////////////////////////////////////////////////
//
// MissingStructs. redefines some of the structures used in Win32 applications that may not
// be available in Windows CE or on other platforms. They are there only for the sake of
// being able to compile the files. The use of these structures may be nonsensical in the
// context of certain platforms
//
////////////////////////////////////////////////////////////////////////////////////////////

struct __declspec(uuid("00000000-0000-0000-c000-000000000046")) IUnknown;

typedef struct tagEXTLOGPEN 
{     
	UINT     elpPenStyle;     
	UINT     elpWidth; 
    UINT     elpBrushStyle;     
	COLORREF elpColor;     
	LONG     elpHatch; 
    DWORD    elpNumEntries;     
	DWORD    elpStyleEntry[1]; 
} EXTLOGPEN; 

typedef struct _ABC 
{ /* abc */
    int     abcA;
    UINT    abcB;
    int     abcC;
} ABC;

#define LPABC ABC* 

typedef struct _ABCFLOAT 
{ /* abcf */
    FLOAT   abcfA;
    FLOAT   abcfB;
    FLOAT   abcfC;
} ABCFLOAT;

#define  LPABCFLOAT ABCFLOAT* 

typedef struct tagKERNINGPAIR 
{ // kp     
	WORD  wFirst;     
	WORD  wSecond; 
    int  iKernAmount; 
} KERNINGPAIR; 

#define LPKERNINGPAIR KERNINGPAIR* 

typedef struct  tagXFORM 
{  /* xfrm */    
	FLOAT eM11;    
	FLOAT eM12;
    FLOAT eM21;    
	FLOAT eM22;    
	FLOAT eDx;    
	FLOAT eDy;
}XFORM;

typedef struct tagPANOSE { 
	// pnse     
	BYTE bFamilyType;     
	BYTE bSerifStyle; 
    BYTE bWeight;     
	BYTE bProportion;     
	BYTE bContrast; 
    BYTE bStrokeVariation;     
	BYTE bArmStyle;     
	BYTE bLetterform; 
    BYTE bMidline;     
	BYTE bXHeight; 
} PANOSE;

typedef struct _OUTLINETEXTMETRIC { // otm     
	UINT   otmSize; 
    TEXTMETRIC otmTextMetrics;     
	BYTE   otmFiller; 
    PANOSE otmPanoseNumber;     
	UINT   otmfsSelection;     
	UINT   otmfsType; 
    int    otmsCharSlopeRise;     
	int    otmsCharSlopeRun; 
    int    otmItalicAngle;     
	UINT   otmEMSquare;     
	int    otmAscent; 
    int    otmDescent;     
	UINT   otmLineGap;     
	UINT   otmsCapEmHeight; 
    UINT   otmsXHeight;     
	RECT   otmrcFontBox;     
	int    otmMacAscent; 
    int    otmMacDescent;     
	UINT   otmMacLineGap; 
    UINT   otmusMinimumPPEM;     
	POINT  otmptSubscriptSize; 
    POINT  otmptSubscriptOffset;     
	POINT  otmptSuperscriptSize; 
    POINT  otmptSuperscriptOffset;     
	UINT   otmsStrikeoutSize; 
    int    otmsStrikeoutPosition;     
	int    otmsUnderscoreSize; 
    int    otmsUnderscorePosition;     
	PSTR   otmpFamilyName; 
    PSTR   otmpFaceName;     
	PSTR   otmpStyleName;     
	PSTR   otmpFullName; 
} OUTLINETEXTMETRIC; 

#define LPOUTLINETEXTMETRIC OUTLINETEXTMETRIC*

typedef struct _GLYPHMETRICS { // glmt
	UINT  gmBlackBoxX; 
    UINT  gmBlackBoxY;     
	POINT gmptGlyphOrigin;     
	short gmCellIncX; 
    short gmCellIncY; 
} GLYPHMETRICS; 

#define LPGLYPHMETRICS GLYPHMETRICS*

typedef struct _FIXED { // fix 
    WORD  fract; 
    short value; 
} FIXED; 
 
typedef struct _MAT2 
{ // mt2     
	FIXED eM11;     
	FIXED eM12;     
	FIXED eM21; 
    FIXED eM22; 
} MAT2; 

UINT CALLBACK FRHookProc(  HWND hdlg,      // handle to the dialog box window
  UINT uiMsg,      // message identifier  
  WPARAM wParam,  // message parameter
  LPARAM lParam   // message parameter
  );

typedef  FARPROC LPFRHOOKPROC;

typedef struct {
DWORD lStructSize; 
HWND hwndOwner; 
HINSTANCE hInstance; 
DWORD Flags; 
LPTSTR lpstrFindWhat; 
LPTSTR lpstrReplaceWith; 
WORD wFindWhatLen; 
WORD wReplaceWithLen; 
LPARAM lCustData; 
LPFRHOOKPROC lpfnHook; 
LPCTSTR lpTemplateName; 
} FINDREPLACE;

#define LPFINDREPLACE FINDREPLACE*

//////////////////////////////////////////////////////////
// declaration of the DrawStateProc......

BOOL CALLBACK DrawStateProc(  
	HDC hdc,       // handle to device context
	LPARAM lData,  // image information  
	WPARAM wData,  // more image information
	int cx,        // width of image  
	int cy         // height of image
	);

typedef FARPROC DRAWSTATEPROC;

////////////////////////////////////////////////////////////////

/* Mapping Modes */
#define MM_TEXT             1
#define MM_LOMETRIC         2
#define MM_HIMETRIC         3
#define MM_LOENGLISH        4
#define MM_HIENGLISH        5
#define MM_TWIPS            6
#define MM_ISOTROPIC        7
#define MM_ANISOTROPIC      8

////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
/* winding */

#ifndef AD_COUNTERCLOCKWISE
#define AD_COUNTERCLOCKWISE 0
#endif

#ifndef AD_CLOCKWISE
#define AD_CLOCKWISE 1
#endif

////////////////////////////////////////////////////////////
/* surface filling for ExtFloodFill */

#ifndef FLOODFILLBORDER
#define FLOODFILLBORDER 0
#endif

#ifndef FLOODFILLSURFACE
#define FLOODFILLSURFACE 1
#endif

////////////////////////////////////////////////////////////////