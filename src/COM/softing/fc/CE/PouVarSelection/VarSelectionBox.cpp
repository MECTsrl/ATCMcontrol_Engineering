// VarSelectionBox.cpp : implementation file
//

#include "stdafx.h"
#include "pouvarselection.h"
#include "VarSelectionBox.h"

#include "PouVarSelCtrl.h"
#include "VarSelDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVarSelectionBox dialog


CVarSelectionBox::CVarSelectionBox(CWnd* pParent /*=NULL*/)
	: CDialog(CVarSelectionBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVarSelectionBox)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_strSelected = _T("");
    m_strEditorText = _T("");
    m_xPos = 0;
    m_yPos = 0;
    m_pParent = pParent;
    m_bNewVar = FALSE;
    m_chLastChar = 0;
    m_pPouVarSelCtrl = NULL;
    m_bEnding  = FALSE;
    m_bMouseOutOfBox = FALSE;
}



void CVarSelectionBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVarSelectionBox)
	DDX_Control(pDX, IDC_VARSEL_COMBO, m_ComboVarSel);	// 13.12.06 SIS: replaced edit + list by combo box
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CVarSelectionBox, CDialog)
	//{{AFX_MSG_MAP(CVarSelectionBox)
	ON_WM_ACTIVATE()
	ON_CBN_DBLCLK(IDC_VARSEL_COMBO, OnDblclkVarList)			// 13.12.06 SIS
	ON_CBN_EDITCHANGE(IDC_VARSEL_COMBO, OnChangeVarSelEdit)		// 13.12.06 SIS
	ON_WM_HELPINFO()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVarSelectionBox message handlers



void CVarSelectionBox::FillVarList(SAFEARRAY* pSArr)
{
    HRESULT hr;
    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    hr = ::SafeArrayAccessData(pSArr, (void HUGEP**)&pbstr);
    if (hr==S_OK)
    {
        for (i = 0; i < ((pSArr)->rgsabound[0]).cElements; i++)
        {
            CString str;
            bstr = pbstr[i];
            str = bstr;
            m_listVars.AddTail(str);
        }

        ::SafeArrayUnaccessData(pSArr);
    }

    ::SafeArrayDestroy(pSArr);
}



BOOL CVarSelectionBox::FindText(const CString& strLine, const LONG iColumn)
{
    ASSERT(m_pPouVarSelCtrl);
    if (!m_pPouVarSelCtrl)
    {
        return FALSE;
    }

    HRESULT hr;
    LONG startCol = iColumn;
    LONG endCol = iColumn;

    m_strComplexVarPart.Empty();;

    // find string including all alphanumerical characters, _ and .
    CString strSelection;
    TCHAR ch;
    
    if (endCol < strLine.GetLength())
    {
        ch = strLine.GetAt(endCol);
        while (ch==_T('_') || ch==_T('.') || _istalnum(ch))
        {
            endCol++;
            if (endCol>=strLine.GetLength())
            {
                break;
            }
            ch = strLine.GetAt(endCol);
        }
    }

    if (startCol > 0 && startCol < strLine.GetLength())		// 13.12.06 SIS: security code
    {
        ch = strLine.GetAt(startCol-1);
        while ((ch==_T('_') || ch==_T('.') || _istalnum(ch))
               && (startCol > 0))
        {
            startCol--;
            if (startCol>0)
            {
                ch = strLine.GetAt(startCol-1);
            }
        }
    }

	if(startCol < strLine.GetLength())						// 13.12.06 SIS: security code
	{
		strSelection = strLine.Mid(startCol, endCol-startCol);
	}

    // investigate string: does it contain . -> complex variable
    if (!strSelection.IsEmpty() && strSelection.Find(_T("."), 0)>=0)
    {
        m_bComplexSelection = TRUE;
        // var selection for complex variable
        int lastPos = strSelection.ReverseFind(_T('.'));
        CString strComplexPrefix = strSelection.Left(lastPos);
        m_strComplexVarPart = strComplexPrefix + _T(".");
        CString strComplexSelection;
        if (strSelection.GetLength()>lastPos+1)
        {
            strComplexSelection = strSelection.Mid(lastPos+1);
        }
        //startCol = startCol + lastPos + 1;
        m_strEditorText = strComplexSelection;
        m_iStartCol = startCol;
        m_iEndCol = endCol;
        CComBSTR sComplexPrefix = strComplexPrefix;
        SAFEARRAY* pSArr;
        hr = m_pPouVarSelCtrl->GetPouMember(sComplexPrefix, &pSArr);
        if(hr == S_OK)
        {
            FillVarList(pSArr);
        }
        if (m_listVars.IsEmpty())
        {
            hr = E_FAIL;
        }
    }
    else
    {
        m_bComplexSelection = FALSE;
        m_strEditorText = strSelection;
        m_iStartCol = startCol;
        m_iEndCol = endCol;
        hr = S_OK;
        CVarSelDlg* pVarSelDlg =  NULL;
        pVarSelDlg = m_pPouVarSelCtrl->GetVarSelDlg();
        if (pVarSelDlg)
        {
            pVarSelDlg->GetVarList(m_listVars, m_strTypeHint);
        }
        else
        {
            hr = E_FAIL;
        }
    }

    if (SUCCEEDED(hr))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



BOOL CVarSelectionBox::Init(CPouVarSelCtrl* pPouVarSelCtrl,
                            LONG xPos, LONG yPos,
                            const CString& strLine,
                            const LONG iColumn,
                            const CString& strTypeHint,
                            const CString& strPouName)
{
    m_pPouVarSelCtrl = pPouVarSelCtrl;
    m_xPos = xPos;
    m_yPos = yPos;
    m_strOrigText = strLine;
    m_strTypeHint = strTypeHint;
    m_strPouName = strPouName;
    
    return FindText(m_strOrigText, iColumn);
}



CString CVarSelectionBox::GetSelection(LONG& iColStart, LONG& iColEnd)
{
    iColStart = m_iStartCol;
    iColEnd = m_iEndCol;
    return m_strComplexVarPart + m_strSelected + m_chLastChar;
}



BOOL CVarSelectionBox::OnInitDialog() 
{
	CDialog::OnInitDialog();

    RECT rectWin;
    GetWindowRect(&rectWin);

    LONG width = rectWin.right - rectWin.left;
    LONG height = rectWin.bottom - rectWin.top;

    // test if it doesn't fit on the screen and show the box above 
    // the given position
    RECT rectWorkArea;
    if (SystemParametersInfo(SPI_GETWORKAREA, 0, &rectWorkArea, 0))
    {
        if ((m_yPos+height) > rectWorkArea.bottom)
        {
            m_yPos -= height;
        }
    }
 

    MoveWindow(m_xPos, m_yPos, width, height);
  
	// 13.12.06 SIS >>
    CSize   sz;
    int     dx=0;
    CDC*    pDC = m_ComboVarSel.GetDC();

    POSITION pos;
    pos = m_listVars.GetHeadPosition();
    while(pos)
    {
        CString strEntry = m_listVars.GetNext(pos);
        m_ComboVarSel.AddString(strEntry);
        sz = pDC->GetTextExtent(strEntry);
        if (sz.cx > dx)
        {
            dx = sz.cx;
        }
    }	

    m_ComboVarSel.ReleaseDC(pDC);

    // Set the horizontal extent only if the current extent is not large enough.
    if (m_ComboVarSel.GetHorizontalExtent() < dx)
    {
       m_ComboVarSel.SetHorizontalExtent(dx);
       ASSERT(m_ComboVarSel.GetHorizontalExtent() == dx);
    }

	if(m_ComboVarSel.GetCount())
	{
		m_ComboVarSel.SetCurSel(0);
	}
	// 13.12.06 SIS <<

    m_timer =  SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CVarSelectionBox::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// 13.12.06 SIS >>
	if(m_ComboVarSel.GetCount())
	{
		m_ComboVarSel.SetCurSel(0);
	}
	// 13.12.06 SIS <<
}



void CVarSelectionBox::OnCancel() 
{
    m_bEnding = TRUE;
	CDialog::OnCancel();
}



void CVarSelectionBox::OnOK()
{
    m_bEnding = TRUE;
    m_bNewVar = FALSE;
    m_strSelected = _T("");

	// 13.12.06 SIS >>
	m_ComboVarSel.GetWindowText(m_strSelected);
	// 13.12.06 SIS <<

    ASSERT(m_pPouVarSelCtrl);
    if (m_pPouVarSelCtrl)
    {
        if (!m_bComplexSelection && m_bNewVar)
        {
            CComBSTR sPouName = m_strPouName;
            CComBSTR sSelected = m_strSelected;
            CComBSTR sDataType = m_strTypeHint;
            m_pPouVarSelCtrl->Fire_ShowPouVarSelection();
            m_pPouVarSelCtrl->ShowVarSelection(sPouName);
            m_pPouVarSelCtrl->SetVarSelectionHint(sPouName, 
                                                  sSelected,
                                                  sDataType);
        }
    }

	CDialog::OnOK();
}



void CVarSelectionBox::OnDblclkVarList() 
{
	OnOK();
}



void CVarSelectionBox::SelectListEntry()
{
	// 13.12.06 SIS >>
    int sel = m_ComboVarSel.FindString(-1, m_strEditorText);
    if (sel>=0)
    {
        m_ComboVarSel.SetCurSel(sel);
    }
    else
    {
        m_ComboVarSel.SetCurSel(-1);
    }
	// 13.12.06 SIS <<
}



void CVarSelectionBox::OnChangeVarSelEdit() 
{
	m_ComboVarSel.GetWindowText(m_strEditorText);	// 13.12.06 SIS
    SelectListEntry();

    // check if the last character in the string is a char
    // that terminates the dialog, that means a char 
    // that does not belong to an IEC identifier (a-zA-Z0-9_)
    if (m_strEditorText.GetLength()>0)
    {
        TCHAR ch = m_strEditorText.GetAt(m_strEditorText.GetLength()-1);
        if ( !_istalnum(ch) && (ch!=_T('_')))
        {
            m_chLastChar = ch;
            m_strEditorText = m_strEditorText.Left(m_strEditorText.GetLength()-1);
            SelectListEntry();
            OnOK();
        }

    }
}


/*
BOOL CVarSelectionBox::OnNcActivate(BOOL bActive) 
{
    if (!bActive && !m_bEnding)
    {
        PostMessage(WM_COMMAND, MAKEWPARAM(IDCANCEL, 0), NULL);
        return TRUE;
    }
	return CDialog::OnNcActivate(bActive);
}
*/

BOOL CVarSelectionBox::OnHelpInfo(HELPINFO* pHelpInfo) 
{
    ASSERT(m_pPouVarSelCtrl!=NULL);
    if (m_pPouVarSelCtrl==NULL)
    {
        return 0;
    }

    CPouVarSelData* pData = NULL;
    pData = m_pPouVarSelCtrl->GetPouVarSelData();
    ASSERT(pData);
    if (!pData)
    {
        return 0;
    }


    CComQIPtr<ICEProjInfo> pProjMan;
    pProjMan = pData->GetProjManInterface();
    ASSERT(pProjMan!=NULL);
    if (pProjMan==NULL)
    {
        return 0;
    }

    pProjMan->ShowContextHelp(IDD_VAR_SELECT);
    return 0;
}

void CVarSelectionBox::OnLButtonDown(UINT nFlags, CPoint point) 
{   
    PostMessage(WM_COMMAND, MAKEWPARAM(IDCANCEL, 0), NULL);
	CDialog::OnLButtonDown(nFlags, point);
}

void CVarSelectionBox::OnMouseMove(UINT nFlags, CPoint point) 
{
    CRect rect;
	this->GetClientRect(&rect);

    if (rect.PtInRect(point) && m_bMouseOutOfBox)
    {
        m_bMouseOutOfBox = FALSE;
        ReleaseCapture();
    }

	CDialog::OnMouseMove(nFlags, point);
}




void CVarSelectionBox::OnTimer(UINT nIDEvent) 
{
	CPoint point;
    ::GetCursorPos(&point);
    CRect rect;
    this->GetClientRect(&rect);
    ClientToScreen(rect);
    if (!rect.PtInRect(point) && !m_bMouseOutOfBox)
    {
        m_bMouseOutOfBox = TRUE;
        SetCapture();
    }
	CDialog::OnTimer(nIDEvent);
}
