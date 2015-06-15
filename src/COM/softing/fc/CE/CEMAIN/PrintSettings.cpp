
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "PrintSettings.h"
#include "XMLHelper\XMLDocument.h"

//----  Static Initializations:   ----------------------------------*




CPrintSettings::CPrintSettings() :
  m_uiPrintFlags(-1)
{
    m_uiPrintFlags &= ~ePF_crossref;    // do not activate cross ref by default
}

CPrintSettings::~CPrintSettings()
{
}

//------------------------------------------------------------------*
/**
 * get header height.
 *
 * @param           -
 * @return          header height in 0.1 mm units
 * @exception       -
 * @see             -
*/
int CPrintSettings::GetHeaderHeight()
{
    return m_tPrintHeader.m_iHeight;
}

//------------------------------------------------------------------*
/**
 * get footer height.
 *
 * @param           -
 * @return          header height in 0.1 mm units
 * @exception       -
 * @see             -
*/
int CPrintSettings::GetFooterHeight()
{
    return m_tPrintFooter.m_iHeight;
}


//------------------------------------------------------------------*
/**
 * save.
 *
 *  save print settings date to xml node
 *
 * @param           node: xml settings root node
 * @param           document: xml settings document
 * @return          -
 * @exception       -
 * @see             SaveCoverPage, SavePrintHeader, SavePrintFooter
*/
void CPrintSettings::Save(CXMLNode& node, CXMLDocument& document)
{
    CXMLNode    tNodePrint;
    CXMLNode    tNodeCoverPage;
    CXMLNode    tNodePrintHeader;
    CXMLNode    tNodePrintFooter;

    if(!document.CreateNode(tNodePrint, CE_PSET_ELEM_PRINT))
    {
        return;
    }
    if(!node.AppendChild(tNodePrint))
    {
        return;
    }

    // print node attributes
    CString strTmp;
    strTmp.Format("%d", m_uiPrintFlags);
    tNodePrint.SetAttribute(CE_PSET_ATTR_PRINTFLAGS, strTmp);

    if(document.CreateNode(tNodeCoverPage, CE_PSET_ELEM_COVERPAGE))
    {
        if(tNodePrint.AppendChild(tNodeCoverPage))
        {
            SaveCoverPage(tNodeCoverPage, document);
        }
    }

    if(document.CreateNode(tNodePrintHeader, CE_PSET_ELEM_HEADER))
    {
        if(tNodePrint.AppendChild(tNodePrintHeader))
        {
            SavePrintHeader(tNodePrintHeader, document);
        }
    }

    if(document.CreateNode(tNodePrintFooter, CE_PSET_ELEM_FOOTER))
    {
        if(tNodePrint.AppendChild(tNodePrintFooter))
        {
            SavePrintFooter(tNodePrintFooter, document);
        }
    }
}

//------------------------------------------------------------------*
/**
 * save cover page.
 *
 *  save cover page date to xml node
 *
 * @param           node: xml cover page settings node
 * @param           document: xml settings document
 * @return          -
 * @exception       -
 * @see             -
*/
void CPrintSettings::SaveCoverPage(CXMLNode& rtNodeCoverPage, CXMLDocument& rtDocument)
{
    rtNodeCoverPage.SetAttribute(CE_PSET_ATTR_BITMAP, m_tCoverPage.m_strBitmapFile);
    rtNodeCoverPage.SetAttribute(CE_PSET_ATTR_PRJNAMCAP, m_tCoverPage.m_strPrjNameCaption);
    rtNodeCoverPage.SetAttribute(CE_PSET_ATTR_PRJNAM, m_tCoverPage.m_strPrjName);
    rtNodeCoverPage.SetAttribute(CE_PSET_ATTR_PRJNOCAP, m_tCoverPage.m_strPrjNoCaption);
    rtNodeCoverPage.SetAttribute(CE_PSET_ATTR_PRJNO, m_tCoverPage.m_strPrjNo);
    rtNodeCoverPage.SetAttribute(CE_PSET_ATTR_PRJMANCAP, m_tCoverPage.m_strPrjManagerCaption);
    rtNodeCoverPage.SetAttribute(CE_PSET_ATTR_PRJMAN, m_tCoverPage.m_strPrjManager);
    rtNodeCoverPage.SetAttribute(CE_PSET_ATTR_COMMENTCAP, m_tCoverPage.m_strCommentCaption);
    rtNodeCoverPage.SetAttribute(CE_PSET_ATTR_COMMENT, m_tCoverPage.m_strComment);
}

//------------------------------------------------------------------*
/**
 * save print header.
 *
 *  save print header date to xml node
 *
 * @param           node: xml print header settings node
 * @param           document: xml settings document
 * @return          -
 * @exception       -
 * @see             -
*/
void CPrintSettings::SavePrintHeader(CXMLNode& rtNodePrintHeader, CXMLDocument& rtDocument)
{
    rtNodePrintHeader.SetAttribute(CE_PSET_ATTR_LBITMAP, m_tPrintHeader.m_strLeftBitmapFile);
    rtNodePrintHeader.SetAttribute(CE_PSET_ATTR_RBITMAP, m_tPrintHeader.m_strRightBitmapFile);
    rtNodePrintHeader.SetAttribute(CE_PSET_ATTR_TITLE, m_tPrintHeader.m_strTitle);
}

//------------------------------------------------------------------*
/**
 * save print footer.
 *
 *  save print footer date to xml node
 *
 * @param           node: xml print footer settings node
 * @param           document: xml settings document
 * @return          -
 * @exception       -
 * @see             -
*/
void CPrintSettings::SavePrintFooter(CXMLNode& rtNodePrintFooter, CXMLDocument& rtDocument)
{
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_REVNOCAP, m_tPrintFooter.m_strRevNoCaption);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_REVNO1, m_tPrintFooter.m_strRevNo[0]);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_REVNO2, m_tPrintFooter.m_strRevNo[1]);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_REVNO3, m_tPrintFooter.m_strRevNo[2]);

    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_CHDESCCAP, m_tPrintFooter.m_strChangeDescrCaption);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_CHDESC1, m_tPrintFooter.m_strChangeDescr[0]);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_CHDESC2, m_tPrintFooter.m_strChangeDescr[1]);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_CHDESC3, m_tPrintFooter.m_strChangeDescr[2]);

    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_CHDATCAP, m_tPrintFooter.m_strChangeDateCaption);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_CHDAT1, m_tPrintFooter.m_strChangeDate[0]);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_CHDAT2, m_tPrintFooter.m_strChangeDate[1]);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_CHDAT3, m_tPrintFooter.m_strChangeDate[2]);

    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_CHNAMCAP, m_tPrintFooter.m_strChangeNameCaption);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_CHNAM1, m_tPrintFooter.m_strChangeName[0]);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_CHNAM2, m_tPrintFooter.m_strChangeName[1]);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_CHNAM3, m_tPrintFooter.m_strChangeName[2]);

    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_REVDATCAP, m_tPrintFooter.m_strRevDateCaption);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_REVDAT, m_tPrintFooter.m_strRevDate);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_REVDRWCAP, m_tPrintFooter.m_strRevDrawnCaption);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_REVDRW, m_tPrintFooter.m_strRevDrawn);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_REVAPPRCAP, m_tPrintFooter.m_strRevApprovedCaption);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_REVAPPR, m_tPrintFooter.m_strRevApproved);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_REVRELCAP, m_tPrintFooter.m_strRevReleasedCaption);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_REVREL, m_tPrintFooter.m_strRevReleased);

    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_CBITMAP, m_tPrintFooter.m_strCenterBitmap);

    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_DATTIME, m_tPrintFooter.m_strDateAndTime);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_PRJPATH, m_tPrintFooter.m_strPrjPath);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_OBJNAME, m_tPrintFooter.m_strObjName);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_PAGECNT, m_tPrintFooter.m_strPageCount);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_INFO1, m_tPrintFooter.m_strInfo[0]);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_INFO2, m_tPrintFooter.m_strInfo[1]);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_INFO3, m_tPrintFooter.m_strInfo[2]);
    rtNodePrintFooter.SetAttribute(CE_PSET_ATTR_INFO4, m_tPrintFooter.m_strInfo[3]);
}


//------------------------------------------------------------------*
/**
 * load.
 *
 *  load settings date from xml node.
 *
 * @param           node: xml settings root node
 * @return          -
 * @exception       -
 * @see             LoadCoverPage, LoadPrintHeader, LoadPrintFooter
*/
void CPrintSettings::Load(CXMLNode& node)
{
    CXMLNode    tNodePrint;
    CXMLNode    tNodeCoverPage;
    CXMLNode    tNodePrintHeader;
    CXMLNode    tNodePrintFooter;

    if(node.GetFirstChildWithTagName(CE_PSET_ELEM_PRINT, tNodePrint))
    {
        // print node attributes
        CString strTmp;
        tNodePrint.GetAttribute(CE_PSET_ATTR_PRINTFLAGS, strTmp);
        if(!strTmp.IsEmpty())
        {
            m_uiPrintFlags = atoi(strTmp);
        }

        if(tNodePrint.GetFirstChildWithTagName(CE_PSET_ELEM_COVERPAGE, tNodeCoverPage))
        {
            LoadCoverPage(tNodeCoverPage);
        }
        if(tNodePrint.GetFirstChildWithTagName(CE_PSET_ELEM_HEADER, tNodePrintHeader))
        {
            LoadPrintHeader(tNodePrintHeader);
        }
        if(tNodePrint.GetFirstChildWithTagName(CE_PSET_ELEM_FOOTER, tNodePrintFooter))
        {
            LoadPrintFooter(tNodePrintFooter);
        }
    }
}


//------------------------------------------------------------------*
/**
 * load cover page.
 *
 *  load cover page date from xml node.
 *
 * @param           node: xml cover page settings node
 * @return          -
 * @exception       -
 * @see             -
*/
void CPrintSettings::LoadCoverPage(CXMLNode& rtNodeCoverPage)
{
    rtNodeCoverPage.GetAttribute(CE_PSET_ATTR_BITMAP, m_tCoverPage.m_strBitmapFile);
    rtNodeCoverPage.GetAttribute(CE_PSET_ATTR_PRJNAMCAP, m_tCoverPage.m_strPrjNameCaption);
    rtNodeCoverPage.GetAttribute(CE_PSET_ATTR_PRJNAM, m_tCoverPage.m_strPrjName);
    rtNodeCoverPage.GetAttribute(CE_PSET_ATTR_PRJNOCAP, m_tCoverPage.m_strPrjNoCaption);
    rtNodeCoverPage.GetAttribute(CE_PSET_ATTR_PRJNO, m_tCoverPage.m_strPrjNo);
    rtNodeCoverPage.GetAttribute(CE_PSET_ATTR_PRJMANCAP, m_tCoverPage.m_strPrjManagerCaption);
    rtNodeCoverPage.GetAttribute(CE_PSET_ATTR_PRJMAN, m_tCoverPage.m_strPrjManager);
    rtNodeCoverPage.GetAttribute(CE_PSET_ATTR_COMMENTCAP, m_tCoverPage.m_strCommentCaption);
    rtNodeCoverPage.GetAttribute(CE_PSET_ATTR_COMMENT, m_tCoverPage.m_strComment);
}

//------------------------------------------------------------------*
/**
 * load print header.
 *
 *  load print header date from xml node.
 *
 * @param           node: xml print header settings node
 * @return          -
 * @exception       -
 * @see             -
*/
void CPrintSettings::LoadPrintHeader(CXMLNode& rtNodePrintHeader)
{
    rtNodePrintHeader.GetAttribute(CE_PSET_ATTR_LBITMAP, m_tPrintHeader.m_strLeftBitmapFile);
    rtNodePrintHeader.GetAttribute(CE_PSET_ATTR_RBITMAP, m_tPrintHeader.m_strRightBitmapFile);
    rtNodePrintHeader.GetAttribute(CE_PSET_ATTR_TITLE, m_tPrintHeader.m_strTitle);
}

//------------------------------------------------------------------*
/**
 * load print footer.
 *
 *  load print footer date from xml node.
 *
 * @param           node: xml print footer settings node
 * @return          -
 * @exception       -
 * @see             -
*/
void CPrintSettings::LoadPrintFooter(CXMLNode& rtNodePrintFooter)
{
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_REVNOCAP, m_tPrintFooter.m_strRevNoCaption);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_REVNO1, m_tPrintFooter.m_strRevNo[0]);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_REVNO2, m_tPrintFooter.m_strRevNo[1]);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_REVNO3, m_tPrintFooter.m_strRevNo[2]);

    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_CHDESCCAP, m_tPrintFooter.m_strChangeDescrCaption);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_CHDESC1, m_tPrintFooter.m_strChangeDescr[0]);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_CHDESC2, m_tPrintFooter.m_strChangeDescr[1]);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_CHDESC3, m_tPrintFooter.m_strChangeDescr[2]);

    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_CHDATCAP, m_tPrintFooter.m_strChangeDateCaption);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_CHDAT1, m_tPrintFooter.m_strChangeDate[0]);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_CHDAT2, m_tPrintFooter.m_strChangeDate[1]);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_CHDAT3, m_tPrintFooter.m_strChangeDate[2]);

    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_CHNAMCAP, m_tPrintFooter.m_strChangeNameCaption);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_CHNAM1, m_tPrintFooter.m_strChangeName[0]);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_CHNAM2, m_tPrintFooter.m_strChangeName[1]);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_CHNAM3, m_tPrintFooter.m_strChangeName[2]);

    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_REVDATCAP, m_tPrintFooter.m_strRevDateCaption);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_REVDAT, m_tPrintFooter.m_strRevDate);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_REVDRWCAP, m_tPrintFooter.m_strRevDrawnCaption);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_REVDRW, m_tPrintFooter.m_strRevDrawn);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_REVAPPRCAP, m_tPrintFooter.m_strRevApprovedCaption);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_REVAPPR, m_tPrintFooter.m_strRevApproved);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_REVRELCAP, m_tPrintFooter.m_strRevReleasedCaption);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_REVREL, m_tPrintFooter.m_strRevReleased);

    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_CBITMAP, m_tPrintFooter.m_strCenterBitmap);

    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_DATTIME, m_tPrintFooter.m_strDateAndTime);

    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_PRJPATH, m_tPrintFooter.m_strPrjPath);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_OBJNAME, m_tPrintFooter.m_strObjName);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_PAGECNT, m_tPrintFooter.m_strPageCount);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_INFO1, m_tPrintFooter.m_strInfo[0]);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_INFO2, m_tPrintFooter.m_strInfo[1]);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_INFO3, m_tPrintFooter.m_strInfo[2]);
    rtNodePrintFooter.GetAttribute(CE_PSET_ATTR_INFO4, m_tPrintFooter.m_strInfo[3]);

}
