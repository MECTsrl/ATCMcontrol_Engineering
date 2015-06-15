


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "cemain.h"
#include "Panel.h"
//#include "prjdoc.h"
#include <WinInet.h>
//----  Static Initializations:   ----------------------------------*


// Panel.cpp : implementation file
//



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanelDoc

IMPLEMENT_DYNCREATE(CPanelDoc, CDocument)

CPanelDoc::CPanelDoc() : m_StrURL(_T(""))
{
}

BOOL CPanelDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CPanelDoc::~CPanelDoc()
{
}


BEGIN_MESSAGE_MAP(CPanelDoc, CDocument)
	//{{AFX_MSG_MAP(CPanelDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, OnUpdateMruList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelDoc diagnostics

#ifdef _DEBUG
void CPanelDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPanelDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPanelDoc serialization

void CPanelDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
/////////////////////////////////////////////////////////////////////////////
// CPanelDoc methods

void CPanelDoc::SetURL(const CString & StrURL)
{
    m_StrURL = StrURL;
}

const CString & CPanelDoc::GetURL(void) const
{
    return m_StrURL;
}

/////////////////////////////////////////////////////////////////////////////
// CPanelDoc commands

BOOL CPanelDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized creation code here
	m_StrURL = lpszPathName;
	return TRUE;
}


void CPanelDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU)
{
    URL_COMPONENTS urlComponents;
    CString strTitle;

    urlComponents.dwStructSize = sizeof (urlComponents);

    urlComponents.dwSchemeLength = 1;
    urlComponents.lpszScheme = NULL;

    urlComponents.dwHostNameLength = 1;
    urlComponents.lpszHostName = NULL;

    urlComponents.dwUserNameLength = 0;    
    urlComponents.lpszUserName = NULL;

    urlComponents.dwPasswordLength = 0;
    urlComponents.lpszPassword = NULL;

    urlComponents.dwUrlPathLength = 1;    
    urlComponents.lpszUrlPath = NULL;

    urlComponents.dwExtraInfoLength = 1;
    urlComponents.lpszExtraInfo = NULL;


    // Do not call base classes SetPathName. There is
    // a restriction that path name must not be longer
    // than MAX_PATH characters.

    m_strPathName = lpszPathName;

    strTitle = m_strPathName;

    
    if (InternetCrackUrl (lpszPathName, strlen (lpszPathName), 0, &urlComponents))
    {
        if (urlComponents.lpszExtraInfo != NULL)
        {
            CString strExtraInfo;

            if (urlComponents.dwExtraInfoLength == 0)
            {
                strExtraInfo = urlComponents.lpszExtraInfo;
            }
            else
            {
                strExtraInfo = CString (urlComponents.lpszExtraInfo).Left (urlComponents.dwExtraInfoLength);
            };

            strTitle = strTitle.Left (strTitle.GetLength () - strExtraInfo.GetLength ());
        };
    };

    SetTitle (strTitle);
}


void CPanelDoc::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    //OH: there is no CPrjDoc
	/*CPrjDoc * pProject = ((CCEApp *)AfxGetApp())->GetProjectDoc();

	if(!pProject) {
		pCmdUI->Enable(TRUE);
        return;
	}

	if(pProject->m_pActDB) {
		pCmdUI->Enable(FALSE);
	}
    else {
        pCmdUI->Enable(TRUE);
	}*/
	
}

void CPanelDoc::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    //OH: there is no CPrjDoc
	/*CPrjDoc * pProject = ((CCEApp *)AfxGetApp())->GetProjectDoc();

	if(!pProject) {
		pCmdUI->Enable(TRUE);
        return;
	}

	if(pProject->m_pActDB) {
		pCmdUI->Enable(FALSE);
	}
    else {
        pCmdUI->Enable(TRUE);
	}*/
	
}

void CPanelDoc::OnUpdateMruList(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    //OH: there is no CPrjDoc
	/*CPrjDoc * pProject = ((CCEApp *)AfxGetApp())->GetProjectDoc();

	if(!pProject) {
		pCmdUI->Enable(TRUE);
        return;
	}

	if(pProject->m_pActDB) {
		pCmdUI->Enable(FALSE);
	}
    else {
        pCmdUI->Enable(TRUE);
	}*/
	
}

