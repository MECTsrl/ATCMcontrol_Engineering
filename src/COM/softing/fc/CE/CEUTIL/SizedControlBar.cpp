


//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "SizedControlBar.h"
#include "CESysDef.h"
#include "MainFrm.h"


IMPLEMENT_DYNCREATE(CSizedControlBar, SECControlBar)


CSizedControlBar::CSizedControlBar()
{

}

CSizedControlBar::~CSizedControlBar()
{

}


void CSizedControlBar::OnBarEndDock()
{
    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    ASSERT(pMainFrame);
    if (pMainFrame != NULL)
    {
        pMainFrame->StoreDockingTools();
    }
}

void CSizedControlBar::OnBarEndFloat()
{
    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    ASSERT(pMainFrame);
    if (pMainFrame != NULL)
    {
        pMainFrame->StoreDockingTools();
    }
}

void CSizedControlBar::OnBarEndMDIFloat()
{
    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    ASSERT(pMainFrame);
    if (pMainFrame != NULL)
    {
        pMainFrame->StoreDockingTools();
    }
}

