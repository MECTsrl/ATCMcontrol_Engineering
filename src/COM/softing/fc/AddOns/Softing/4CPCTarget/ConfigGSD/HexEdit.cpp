//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "HexEdit.hpp"
#include "resource.h"
#include "SheetConfigGSD.h"

//----  Local Defines:   -----------------------------------------------------*

//----  Static Initializations:   --------------------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int    s_iColPerRow = 8;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CHexEdit
///////////////////////////////////////////////////////////////////////////////

//----------------------------- MESSAGE MAP ----------------------------------*
BEGIN_MESSAGE_MAP(CHexEdit, CEdit)
	//{{AFX_MSG_MAP(CHexEdit)
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	ON_CONTROL_REFLECT(EN_VSCROLL, OnVscroll)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//----------------------------- FUNCTION -------------------------------------*
CHexEdit::CHexEdit()
/*>>>> 
ctor

Result
  void
<<<<*/
  : m_iMaxLen(0),
	m_dwColPerRow(8),
	m_dwVisibleLines(3),
	m_bHasChanged(FALSE)
{
}

//----------------------------- FUNCTION -------------------------------------*
CHexEdit::~CHexEdit()
/*>>>> 
dtor

Result
  void
<<<<*/
{
}


/////////////////////////////////////////////////////////////////////////////
// CHexEdit message handlers

//----------------------------- FUNCTION -------------------------------------*
BOOL
    CHexEdit::PreCreateWindow(CREATESTRUCT& cs) 
/*>>>> 

Result
  void
<<<<*/
{
    if ((cs.dwExStyle & ES_MULTILINE) != 0) {
        cs.dwExStyle |= ES_WANTRETURN | WS_VSCROLL;
    }

    return CEdit::PreCreateWindow(cs);
}

//----------------------------- FUNCTION -------------------------------------*
int
    CHexEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
/*>>>> 

Result
  0 if ok; -1 if error
<<<<*/
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

//----------------------------- FUNCTION -------------------------------------*
void
    CHexEdit::OnUpdate() 
/*>>>> 

Result
  void
<<<<*/
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
}

//----------------------------- FUNCTION -------------------------------------*
void
    CHexEdit::OnVscroll() 
/*>>>> 

Result
  void
<<<<*/
{
	// TODO: Add your control notification handler code here
}

//----------------------------- FUNCTION -------------------------------------*
void
    CHexEdit::OnChange() 
/*>>>> 

Result
  void
<<<<*/
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_CHANGE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	m_bHasChanged = TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
void
    CHexEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
/*>>>> 

Result
  void
<<<<*/
{
	// TODO: Add your message handler code here and/or call default
    nChar = toupper(nChar);
    if (   (nChar >= '0' && (nChar <= '9'))
        || (nChar >= 'A' && (nChar <= 'F'))
        || (nChar == VK_ESCAPE)
        || (nChar == VK_RETURN)
        || (nChar == VK_SPACE)
        || (nChar == VK_TAB)
        || (nChar == 0x03)      // Ctrl + C: Copy
        || (nChar == 0x18)      // Ctrl + X: Cut
        || (nChar == 0x16)      // Ctrl + V: Paste
        || (nChar == VK_BACK)) 
    {
        CEdit::OnChar(nChar, nRepCnt, nFlags);
    }
    else 
    {
        // not a valid hex code
        ::MessageBeep(MB_ICONEXCLAMATION);
        return;
    }
}

//----------------------------- FUNCTION -------------------------------------*
void CHexEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
    int iMaxLen = GetWindowTextLength();
    BYTE*   pData = new BYTE[iMaxLen];
    ASSERT(pData != NULL);
    int cnt = GetHexContent(pData, iMaxLen);
    SetHexContent(pData, cnt, m_iMaxLen);
    delete [] pData;
}
//----------------------------- FUNCTION -------------------------------------*
void
    CHexEdit::SetHexContent(BYTE const* pBuffer, int iBufLen, int iMaxLen /*= 0*/)
/*>>>> 

Result
  void
<<<<*/
{
//    ASSERT(pBuffer != NULL);
    ASSERT(iBufLen >= 0);

    m_iMaxLen = iMaxLen;
    if (m_iMaxLen > 0) {
        LimitText(m_iMaxLen * 3);
    }
    else {
        LimitText(m_iMaxLen);
    }

    // calculate tabstop width
    CDC *pDC = GetDC();
    ASSERT(pDC != NULL);
    static const TCHAR  szSampleField[] = _T("AA   ");
    int iWidth = (pDC->GetTextExtent(szSampleField, _tcslen(szSampleField))).cx;
    iWidth = (iWidth * 4) / LOWORD(::GetDialogBaseUnits());
    SetTabStops(iWidth);

    CString sEdit;
    // get ptr to work string buffer
    LPTSTR pszEdit = sEdit.GetBufferSetLength((iBufLen + 1) * 4 * sizeof(TCHAR));

    // preformat text stream with TABs between columns and newline between lines/rows
    int iOffset = 0;
    while (iOffset < iBufLen)
    {
        int iCol = iOffset % s_iColPerRow;
        int iRow = iOffset / s_iColPerRow;
        if (iCol == 0) {
            // first column in line
            wsprintf(pszEdit, _T("\r\n%02X"), (int) pBuffer[iOffset]);
            pszEdit += 4;
        }
        else {
            // any other column in line
            wsprintf(pszEdit, _T("\t%02X"), (int) pBuffer[iOffset]);
            pszEdit += 3;
        }
        iOffset++;
    }
    *pszEdit = '\0';
    sEdit.ReleaseBuffer();
    pszEdit = (LPTSTR) (LPCTSTR) sEdit;
    if (iOffset > 0) {
        pszEdit += 2;   // compensate leading '\r\n' which is not necessary in first line
    }
    SetWindowText(pszEdit);
}

//----------------------------- FUNCTION -------------------------------------*
int
    CHexEdit::GetHexContent(BYTE* pBuffer, int iMaxBufLen)
/*>>>> 

Result
  void
<<<<*/
{
    CString sEdit;
    GetWindowText(sEdit);

    int iOffset = CreateHexString(sEdit, pBuffer, iMaxBufLen);

    return iOffset;
}

//----------------------------- FUNCTION -------------------------------------*
void CHexEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (nSBCode != SB_ENDSCROLL && nSBCode != SB_THUMBPOSITION)	{
		DWORD dwStartline = GetFirstVisibleLine( );
		DWORD dwLinecount = GetLineCount();

		switch (nSBCode)	{
		case SB_LINEDOWN:
			if ((dwStartline + m_dwVisibleLines) < dwLinecount)
				dwStartline = (dwStartline + 1) * m_dwColPerRow;
			else
				dwStartline *= m_dwColPerRow;
			break;
		case SB_LINEUP:
			if (dwStartline > 0)
				dwStartline = (dwStartline - 1) * m_dwColPerRow;
			break;
		case SB_PAGEDOWN:
			dwStartline = (dwStartline + m_dwVisibleLines) * m_dwColPerRow;
			if (dwStartline > ((dwLinecount - m_dwVisibleLines + 1) * m_dwColPerRow))
				dwStartline = (dwLinecount - m_dwVisibleLines + 1)*m_dwColPerRow;
			break;
		case SB_PAGEUP:
			dwStartline = (dwStartline - m_dwVisibleLines) * m_dwColPerRow;
			if (dwStartline < 0)
				dwStartline = 0;
			break;
		case SB_THUMBTRACK:
			dwStartline = nPos * m_dwColPerRow;
			break;
		default:
			break;
		}

		CString szLineNmb(_T(""));

		for (DWORD i = dwStartline; i <= dwLinecount * m_dwColPerRow; i += m_dwColPerRow)	{
			CString szNmb(_T(""));
			szNmb.Format(_T("%d."), i);

			szLineNmb += szNmb;
			szLineNmb += _T("\r\n");
		}
		CWnd* Wnd = GetParent( );
		Wnd->GetDlgItem(IDC_USER_PRM_LINECOUNT)->SetWindowText(szLineNmb);
	}
	
	CEdit::OnVScroll(nSBCode, nPos, pScrollBar);
}

//----------------------------- FUNCTION -------------------------------------*
int CHexEdit::CreateHexString(CString &sText, BYTE* pBuffer, int iMaxBufLen)
{
/*convert a hex string to binary hex bytes

I   pCfgData:   ptr to special format byte
I   idsInOutString: resource ID of input/output string
O   sCfgTxt:    string to receive decoded text
O   lenIO:      length of inputs/outputs in special identifier byte

Result
  number of hex bytes
<<<<*/
	LPTSTR  pszEdit = (LPTSTR)(LPCTSTR) sText;

	int iOffset = 0;
	BOOL bEnd = sText.IsEmpty();
	for (iOffset = 0; !bEnd && (iOffset < iMaxBufLen); iOffset++)
	{
		LPCTSTR  pszVal = pszEdit;
		// overread non-whitespace until end of hex field
		while (*pszEdit && !_istspace(*pszEdit))
			pszEdit++;
    
		bEnd = (*pszEdit == '\0');

		// overread whitespace to start of next hex field
		if (!bEnd) {
			*pszEdit = '\0';
			pszEdit++;
			while (*pszEdit && _istspace(*pszEdit))
				pszEdit++;
			bEnd = (*pszEdit == '\0');  // did we now reach end of string?
		}

		ULONG   ulVal = _tcstoul(pszVal, NULL, 16);
		ASSERT(ulVal <= 0xff);
		ulVal &= 0xFF;      // mask to byte
		pBuffer[iOffset] = (BYTE) ulVal;
	}
	return iOffset;
}

//----------------------------- FUNCTION -------------------------------------*
void CHexEdit::SetLinePropperties(DWORD dwColPerRow, DWORD dwVisibleLine)
{
	m_dwColPerRow = dwColPerRow;
	m_dwVisibleLines = dwVisibleLine;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL CHexEdit::HasChanged()
{
	return m_bHasChanged;
}

//----------------------------- FUNCTION -------------------------------------*
void CHexEdit::ResetChangedFlag()
{
	m_bHasChanged = FALSE;
}
