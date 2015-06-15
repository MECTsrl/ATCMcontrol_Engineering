
#ifndef __PRINTSETTINGS_H_
#define __PRINTSETTINGS_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "PrintLayout.h"
#include "XMLHelper\XMLNode.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#define CE_PSET_ELEM_PRINT          _T("PRINT")
#define CE_PSET_ELEM_COVERPAGE      _T("COVER")
#define CE_PSET_ELEM_HEADER         _T("HEADER")
#define CE_PSET_ELEM_FOOTER         _T("FOOTER")

// coverpage attributes
#define CE_PSET_ATTR_BITMAP         _T("BITMAP")
#define CE_PSET_ATTR_PRJNAMCAP      _T("PRJNAMCAP")
#define CE_PSET_ATTR_PRJNAM         _T("PRJNAM")
#define CE_PSET_ATTR_PRJNOCAP       _T("PRJNOCAP")
#define CE_PSET_ATTR_PRJNO          _T("PRJNO")
#define CE_PSET_ATTR_PRJMANCAP      _T("PRJMANCAP")
#define CE_PSET_ATTR_PRJMAN         _T("PRJMAN")
#define CE_PSET_ATTR_COMMENTCAP     _T("COMMENTCAP")
#define CE_PSET_ATTR_COMMENT        _T("COMMENT")

// print header attributes
#define CE_PSET_ATTR_LBITMAP        _T("LBITMAP")
#define CE_PSET_ATTR_RBITMAP        _T("RBITMAP")
#define CE_PSET_ATTR_TITLE          _T("TITLE")

// print footer attributes
#define CE_PSET_ATTR_REVNOCAP       _T("REVNOCAP")
#define CE_PSET_ATTR_REVNO1         _T("REVNO1")
#define CE_PSET_ATTR_REVNO2         _T("REVNO2")
#define CE_PSET_ATTR_REVNO3         _T("REVNO3")
#define CE_PSET_ATTR_CHDESCCAP      _T("CHDESCCAP")
#define CE_PSET_ATTR_CHDESC1        _T("CHDESC1")
#define CE_PSET_ATTR_CHDESC2        _T("CHDESC2")
#define CE_PSET_ATTR_CHDESC3        _T("CHDESC3")
#define CE_PSET_ATTR_CHDATCAP       _T("CHDATCAP")
#define CE_PSET_ATTR_CHDAT1         _T("CHDAT1")
#define CE_PSET_ATTR_CHDAT2         _T("CHDAT2")
#define CE_PSET_ATTR_CHDAT3         _T("CHDAT3")
#define CE_PSET_ATTR_CHNAMCAP       _T("CHNAMCAP")
#define CE_PSET_ATTR_CHNAM1         _T("CHNAM1")
#define CE_PSET_ATTR_CHNAM2         _T("CHNAM2")
#define CE_PSET_ATTR_CHNAM3         _T("CHNAM3")
#define CE_PSET_ATTR_REVDATCAP      _T("REVDATCAP")
#define CE_PSET_ATTR_REVDAT         _T("REVDAT")
#define CE_PSET_ATTR_REVDRWCAP      _T("REVDRWCAP")
#define CE_PSET_ATTR_REVDRW         _T("REVDRW")
#define CE_PSET_ATTR_REVAPPRCAP     _T("REVAPPRCAP")
#define CE_PSET_ATTR_REVAPPR        _T("REVAPPR")
#define CE_PSET_ATTR_REVRELCAP      _T("REVRELCAP")
#define CE_PSET_ATTR_REVREL         _T("REVREL")
#define CE_PSET_ATTR_CBITMAP        _T("CBITMAP")
#define CE_PSET_ATTR_DATTIME        _T("DATTIME")
#define CE_PSET_ATTR_PRJPATH        _T("PRJPATH")
#define CE_PSET_ATTR_OBJNAME        _T("OBJNAME")
#define CE_PSET_ATTR_PAGECNT        _T("PAGECNT")
#define CE_PSET_ATTR_INFO1          _T("INFO1")
#define CE_PSET_ATTR_INFO2          _T("INFO2")
#define CE_PSET_ATTR_INFO3          _T("INFO3")
#define CE_PSET_ATTR_INFO4          _T("INFO4")

// other attributes
#define CE_PSET_ATTR_PRINTFLAGS     _T("PRINTFLAGS")


//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
enum EPrintFlags
{
    ePF_coverpage   = 0x001,    // cover page
    ePF_contents    = 0x002,    // contents
    ePF_fileview    = 0x004,    // file view
    ePF_classview   = 0x008,    // class view
    ePF_instview    = 0x010,    // instance view
    ePF_hwview      = 0x020,    // hardware view
    ePF_grheader    = 0x040,    // ST-Header of grafic editor files
	ePF_crossref	= 0x080,	// cross reference
};

//------------------------------------------------------------------*
/**
 *  class CPrintSettings
 *
 *  @see    ....
*/
class CPrintSettings
{
public:
    CPrintSettings();
    ~CPrintSettings();

    void Save(CXMLNode& node, CXMLDocument& document);
    void Load(CXMLNode& node);

    int GetHeaderHeight();
    int GetFooterHeight();

protected:
    void SaveCoverPage(CXMLNode& rtNodeCoverPage, CXMLDocument& rtDocument);
    void SavePrintHeader(CXMLNode& rtNodePrintHeader, CXMLDocument& rtDocument);
    void SavePrintFooter(CXMLNode& rtNodePrintFooter, CXMLDocument& rtDocument);

    void LoadCoverPage(CXMLNode& rtNodeCoverPage);
    void LoadPrintHeader(CXMLNode& rtNodePrintHeader);
    void LoadPrintFooter(CXMLNode& rtNodePrintFooter);

public:
    CCoverPage      m_tCoverPage;
    CPrintHeader    m_tPrintHeader;
    CPrintFooter    m_tPrintFooter;

    UINT            m_uiPrintFlags;

};

#endif // __PRINTSETTINGS_H_

