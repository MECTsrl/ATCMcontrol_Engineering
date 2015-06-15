
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "Stdafx.h"
#include "SizedDialogBar.h"
#include "CESysDef.h"
#include "MainFrm.h"


//----  Static Initializations:   ----------------------------------*

IMPLEMENT_DYNCREATE(CSizedDialogBar, SECDialogBar)

CSizedDialogBar::CSizedDialogBar()
{   
    m_nHeight = GetStandardDockingHeight();
}

CSizedDialogBar::~CSizedDialogBar()
{
}



//------------------------------------------------------------------*
/**
 * save profile
 *
 *  save docking height in registry
 *  registry path must be provieded by derived class
 *
 * @see             LoadSizeData()
*/
void CSizedDialogBar::SaveSizeData()
{
    // TODO: get current height
    if(m_dwMRUDockingState & CBRS_ALIGN_BOTTOM || m_dwMRUDockingState & CBRS_ALIGN_TOP)
    {
        m_nHeight = m_szDockHorz.cx;
    }
    else
    {
        m_nHeight = m_szDockVert.cx;
    }
    AfxGetApp()->WriteProfileInt(m_strRegKey, REG_KEY_DOCKING_HEIGHT, m_nHeight);
}


//------------------------------------------------------------------*
/**
 * load profile
 *
 *  get docking height from registry
 *  registry path must be provieded by derived class
 *
 * @see             SaveSizeData()
*/
void CSizedDialogBar::LoadSizeData()
{
    m_nHeight = AfxGetApp()->GetProfileInt(m_strRegKey, REG_KEY_DOCKING_HEIGHT, GetStandardDockingHeight());
}


//------------------------------------------------------------------*
/**
 * get standard docking height in pixels
 *
 *  base class returns 150
 *
*/
int CSizedDialogBar::GetStandardDockingHeight()
{
    return 150;
}


//------------------------------------------------------------------*
/**
 * return docking height
 *
 * @return          docking height in pixels
 * @see             SECMDIFrameWnd::DockControlBarEx()
 *
*/
int CSizedDialogBar::GetDockingHeight()
{
    return m_nHeight;
}


void CSizedDialogBar::OnBarEndDock()
{
   /* if ((GetStyle() & WS_VISIBLE) != 0)
    {
        GetBarSizePos(m_settings.m_iRow,
                      m_settings.m_iColumn,
                      m_settings.m_uiMRUDockId,
                      m_settings.m_fPctWidth,
                      m_settings.m_iHeight);
    }*/

    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    ASSERT(pMainFrame);
    if (pMainFrame != NULL)
    {
        pMainFrame->StoreDockingTools();
    }
}

void CSizedDialogBar::OnBarEndFloat()
{
    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    ASSERT(pMainFrame);
    if (pMainFrame != NULL)
    {
        pMainFrame->StoreDockingTools();
    }
}

void CSizedDialogBar::OnBarEndMDIFloat()
{
    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    ASSERT(pMainFrame);
    if (pMainFrame != NULL)
    {
        pMainFrame->StoreDockingTools();
    }
}

