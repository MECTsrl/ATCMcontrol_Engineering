
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "PrintLayout.h"
#include "Resource.h"

//----  Static Initializations:   ----------------------------------*


///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////  CCoverPage  ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
CCoverPage::CCoverPage()
{
    Init();
}

CCoverPage::~CCoverPage()
{
}

void CCoverPage::Init()
{
    m_strPrjNameCaption.LoadString(IDS_COVPAG_PRJNAMCAP);
    m_strPrjName = _T("$(PrjName)");
    m_strPrjNoCaption.LoadString(IDS_COVPAG_PRJNOCAP);
    m_strPrjManagerCaption.LoadString(IDS_COVPAG_PRJMANCAP);
    m_strCommentCaption.LoadString(IDS_COVPAG_COMMENTCAP);
}

///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////  CPrintHeader  /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

CPrintHeader::CPrintHeader()
{
    Init();
}

CPrintHeader::~CPrintHeader()
{
}

void CPrintHeader::Init()
{
    m_iHeight = 200;
}


///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////  CPrintFooter  /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


CPrintFooter::CPrintFooter()
{
    Init();
}

CPrintFooter::~CPrintFooter()
{
}

void CPrintFooter::Init()
{
    m_strRevNoCaption.LoadString(IDS_FOOTER_REVNOCAP);
    m_strChangeDescrCaption.LoadString(IDS_FOOTER_REVCHDESCCAP);
    m_strChangeDateCaption.LoadString(IDS_FOOTER_REVDATECAP);
    m_strChangeNameCaption.LoadString(IDS_FOOTER_REVNAMECAP);
    m_strRevDateCaption.LoadString(IDS_FOOTER_REVDATECAP);
    m_strRevDrawnCaption.LoadString(IDS_FOOTER_REVDRAWNCAP);
    m_strRevApprovedCaption.LoadString(IDS_FOOTER_REVAPPRCAP);
    m_strRevReleasedCaption.LoadString(IDS_FOOTER_REVRELCAP);
    m_strDateAndTime = _T("$(PrintDate#ger)");
    m_strPrjPath = _T("$(PrjDir)");
    m_strObjName = _T("$(ObjName)");
    m_strPageCount = _T("$(Page)/$(PageCount)");
    m_iHeight = 160;
}

