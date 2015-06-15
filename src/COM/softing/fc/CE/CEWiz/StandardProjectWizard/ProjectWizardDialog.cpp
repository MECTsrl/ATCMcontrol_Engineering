#include "stdafx.h"
#include "ProjectWizardDialog.h"


//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CProjectWizardDialog, CPropertySheet)
END_MESSAGE_MAP()


CProjectWizardDialog::CProjectWizardDialog (CProjectWizardData *pData)
: CPropertySheet (_T (""), AfxGetMainWnd (), 0),
  m_pageResource (this, pData),
  m_pageLibraries (this, pData, IDS_LIBSPAGE_CAPTION),
  m_pageNames (this, pData),
  m_pageProperties (this, pData, IDS_PROPSPAGE_CAPTION)
{
    AddPage (&m_pageResource);
    AddPage (&m_pageLibraries);
    AddPage (&m_pageNames);
    AddPage (&m_pageProperties);

    SetWizardMode ();
}