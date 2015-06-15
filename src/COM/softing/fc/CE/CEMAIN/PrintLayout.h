#ifndef __PRINTLAYOUT_H_
#define __PRINTLAYOUT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
// defines for layout
// all sizes are in units of 0.1 mm

// Coverpage
#define PLY_COVPAG_OFFSET_TOP           100     // offset from header
#define PLY_COVPAG_BITMAP_WIDTH         450     // width of center bitmap
#define PLY_COVPAG_BITMAP_HEIGHT        300     // height of center bitmap
#define PLY_COVPAG_OFFSET_PRJCAP         80     // offset from bitmap to project caption
#define PLY_COVPAG_OFFSET_CAPFLD         80     // offset from caption to field
#define PLY_COVPAG_OFFSET_INFO          200     // offset from project field to info block
#define PLY_COVPAG_OFFSET_INFO_HEIGHT   640     // total height of info block
#define PLY_COVPAG_OFFSET_INFO_ROWHEIGHT 80     // height of info row
#define PLY_COVPAG_OFFSET_LEFT          200     // left offset of info block
#define PLY_COVPAG_OFFSET_LEFT2         400     // left offset of info text from captions
// Header
#define PLY_HEADER_BITMAP_WIDTH         530    // width of bitmaps
#define PLY_HEADER_BITMAP_HEIGHT        200    // height of bitmaps

// Footer
#define PLY_FOOTER_FIELD_HEIGHT          40    // height of simple field
#define PLY_FOOTER_REVNO_WIDTH           50    // width of revision number field
#define PLY_FOOTER_REVCHDESCR_WIDTH     200    // width of revision change description field
#define PLY_FOOTER_REVCHDATE_WIDTH      100    // width of revision change date field
#define PLY_FOOTER_REVCHNAME_WIDTH       75    // width of revision change name field
#define PLY_FOOTER_REVCAP_WIDTH          75    // width of revision caption fields
#define PLY_FOOTER_REVFLD_WIDTH         200    // width of revision fields
#define PLY_FOOTER_PRINTDATE_WIDTH      300    // width of print date and time field
#define PLY_FOOTER_PRJPATH_WIDTH        500    // width of project path field
#define PLY_FOOTER_INFOFLD_WIDTH        300    // width of info fields
#define PLY_FOOTER_PAGECOUNT_WIDTH      200    // width of page count field


//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//------------------------------------------------------------------*
/**
 *  class CCoverPage
 *
 *  encapsulation of date for the cover page.
 *
 *  @see    ....
*/
class CCoverPage
{
public:
    CCoverPage();
    ~CCoverPage();

    void Init();

public:

    CString m_strBitmapFile;            // file name of bitmap incl path
    CString m_strPrjNameCaption;        // project name caption, default is "Project"
    CString m_strPrjName;               // project name, default is $(PrjName)
    CString m_strPrjNoCaption;          // project number caption, default is "Project Number"
    CString m_strPrjNo;                 // project number
    CString m_strPrjManagerCaption;     // project manager caption, default is "Project Manager"
    CString m_strPrjManager;            // name of project manager
    CString m_strCommentCaption;        // comment caption, default is "Comment"
    CString m_strComment;               // comment
};



//------------------------------------------------------------------*
/**
 *  class CPrintHeader
 *
 *  encapsulation of date for the print header.
 *
 *  @see    ....
*/
class CPrintHeader
{
public:
    CPrintHeader();
    ~CPrintHeader();

    void Init();

public:

    CString m_strLeftBitmapFile;        // file name incl path of left bitmap
    CString m_strRightBitmapFile;       // file name incl path of right bitmap
    CString m_strTitle;                 // title
    int     m_iHeight;                  // height of header on paper in 0.1 mm units
};



//------------------------------------------------------------------*
/**
 *  class CPrintFooter
 *
 *  encapsulation of date for the print footer.
 *
 *  @see    ....
*/
class CPrintFooter
{
public:
    CPrintFooter();
    ~CPrintFooter();

    void Init();


public:
    CString m_strRevNoCaption;          // revision number caption, default is "Rev"
    CString m_strRevNo[3];              // revision numbers 0 .. 2

    CString m_strChangeDescrCaption;    // change description caption, default is "Change"
    CString m_strChangeDescr[3];        // change descriptions 0 .. 2

    CString m_strChangeDateCaption;     // change date caption, default is "Date"
    CString m_strChangeDate[3];         // change dates 0 .. 2

    CString m_strChangeNameCaption;     // change name caption, default is "Name"
    CString m_strChangeName[3];         // change names 0 .. 2

    CString m_strRevDateCaption;        // revision date caption, default is "Date"
    CString m_strRevDate;               // revision date
    CString m_strRevDrawnCaption;       // revision drawn caption, default is "Drawn"
    CString m_strRevDrawn;              // revision drawn
    CString m_strRevApprovedCaption;    // revision approved caption, default is "Appr."
    CString m_strRevApproved;           // revision approved
    CString m_strRevReleasedCaption;    // revision released caption, default is "Rel."
    CString m_strRevReleased;           // revision released

    CString m_strCenterBitmap;          // file name incl path for center bitmap

    CString m_strDateAndTime;           // date and time, default is $(PrintDate#eng)
    CString m_strPrjPath;               // project path, default is $(PrjPath)
    CString m_strObjName;               // object name, default is $(ObjId)
    CString m_strPageCount;             // default is $(PageCount)
    CString m_strInfo[4];               // free info 0 .. 3

    int     m_iHeight;                  // height of footer on paper in 0.1 mm units
};

#endif // __PRINTLAYOUT_H_

