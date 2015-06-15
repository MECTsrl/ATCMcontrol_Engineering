
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

#include "stdafx.h"
#include "resource.h"
#include "fc_todebug\fc_mem.h"
#include "findfileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecentFindFile

void CRecentFindFileList::Add(LPCTSTR lpszPattern)
{
	ASSERT(m_pRecFileList->m_arrNames != NULL);
	ASSERT(lpszPattern != NULL);

	// update the MRU list, if an existing MRU string matches file name
	for (int iMRU = 0; iMRU < m_pRecFileList->m_nSize-1; iMRU++)
	{
		if (!lstrcmp(m_pRecFileList->m_arrNames[iMRU], lpszPattern))
			break;      // iMRU will point to matching entry
	}
	// move MRU strings before this one down
	for (; iMRU > 0; iMRU--)
	{
		ASSERT(iMRU > 0);
		ASSERT(iMRU < m_pRecFileList->m_nSize);
		m_pRecFileList->m_arrNames[iMRU] = m_pRecFileList->m_arrNames[iMRU-1];
	}
	// place this one at the beginning
	m_pRecFileList->m_arrNames[0] = lpszPattern;
}


/////////////////////////////////////////////////////////////////////////////
// CMRUComboBox constructor & destructor


//////////////////////////////////////////////////////////////////////////
//
// Function:    CMRUComboBox()
//
// Description:
//  Class constructor.
//
// Notes:
//  Calls base class constructor and initializes member variables.
//
//////////////////////////////////////////////////////////////////////////

CMRUComboBox::CMRUComboBox() : CComboBox(),
    m_bRefreshAfterAdd   ( FALSE ),
    m_bSaveAfterAdd      ( FALSE ),
    m_bSaveOnDestroy     ( TRUE ),
    m_nMaxMRUSize        ( MRUC_DEFAULT_MRU_SIZE ),
    m_pMRU               ( NULL ),
    m_bParamsChanged     ( FALSE )
{
}


//////////////////////////////////////////////////////////////////////////
//
// Function:    CMRUComboBox()
//
// Description:
//  Class destructor.
//
//////////////////////////////////////////////////////////////////////////

CMRUComboBox::~CMRUComboBox()
{
                                        // Save the MRU if we need to.
    if ( m_bSaveOnDestroy )
        {
        if ( !SaveMRU() )
            {
            TRACE0("CMRUComboBox -- Warning - SaveMRU() in destructor failed. MRU was not saved.\n");
            }
        }

                                        // Free up the CRecentFileList object.
    if ( NULL != m_pMRU )
        {
        delete m_pMRU;
        }
}


BEGIN_MESSAGE_MAP(CMRUComboBox, CComboBox)
	//{{AFX_MSG_MAP(CMRUComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMRUComboBox MRU operations


//////////////////////////////////////////////////////////////////////////
//
// Function:    AddToMRU()
//
// Description:
//  Adds a string to the MRU list.
//
// Input:
//  szNewItem: [in] The string to add.
//
// Returns:
//  TRUE if successful, FALSE if not.
//
//////////////////////////////////////////////////////////////////////////

BOOL CMRUComboBox::AddToMRU ( LPCTSTR szNewItem )
{
                                        // String can't be a null pointer!
    ASSERT ( NULL != szNewItem );

                                        // Allocate a new CRecentFileList 
                                        // if necessary.
    if ( NULL == m_pMRU )
        {
        if ( !AllocNewMRU() )
            {
            TRACE0("CMRUComboBox -- AllocNewMRU() failed in AddToMRU().\n");
            return FALSE;
            }
        }

                                        // Add it to the MRU list.
    m_pMRU->Add ( szNewItem );

                                        // Automagically refresh the combobox?
    if ( m_bRefreshAfterAdd )
        {
        RefreshCtrl();
        }

                                        // Automagically save the MRU?
    if ( m_bSaveAfterAdd )
        {
        SaveMRU();
        }


    return TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
// Function:    EmptyMRU()
//
// Description:
//  Removes all strings from the MRU list.
//
// Input:
//  Nothing.
//
// Returns:
//  Nothing.
//
//////////////////////////////////////////////////////////////////////////

void CMRUComboBox::EmptyMRU()
{
int i;

    if ( NULL != m_pMRU )
        {
                                        // Remove all strings from the MRU list.
                                        // Go in reverse order to keep the
                                        // strings from changing positions 
                                        // while we're doing our dirty work.
        for ( i = m_pMRU->GetSize() - 1; i >= 0; i-- )
            {
            m_pMRU->Remove(i);
            }
        }
}


//////////////////////////////////////////////////////////////////////////
//
// Function:    LoadMRU()
//
// Description:
//  Loads an MRU from the registry or the app's INI file.
//
// Input:
//  Nothing.
//
// Returns:
//  TRUE if successful, FALSE if not.
//
//////////////////////////////////////////////////////////////////////////

BOOL CMRUComboBox::LoadMRU()
{
                                        // We always allocate a new
                                        // CRecentFileList object when loading.
    if ( !AllocNewMRU() )
        {
        TRACE0("CMRUComboBox -- AllocNewMRU() failed in LoadMRU().\n");
        return FALSE;
        }

    m_pMRU->ReadList();

    return TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
// Function:    SaveMRU()
//
// Description:
//  Writes the MRU to the registry or app's INI file.
//
// Input:
//  Nothing.
//
// Returns:
//  TRUE if successful, FALSE if not.
//
//////////////////////////////////////////////////////////////////////////

BOOL CMRUComboBox::SaveMRU()
{
                                        // If we haven't created a 
                                        // CRecentFileList yet, then there's
                                        // nothing to save.
    if ( NULL == m_pMRU )
        {
        TRACE0("CMRUComboBox -- SaveMRU failed - no CRecentFileList created.\n");
        return FALSE;
        }

                                        // Check that the CRecentFileList 
                                        // parameters are kosher.
    if ( !VerifyMRUParams() )
        {
        TRACE0("CMRUComboBox -- SaveMRU() failed - params not set.\n");
        return FALSE;
        }

                                        // If the registry key/value strings
                                        // have been changed, we need to make
                                        // a new CRecentFileList.
    if ( m_bParamsChanged )
        {
        if ( !AllocNewMRU() )
            {
            TRACE0("CMRUComboBox -- SaveMRU failed - couldn't reallocate CRecentFileList with new MRU params.\n");
            return FALSE;
            }
        }

    m_pMRU->WriteList();

    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CMRUComboBox combobox control operations


//////////////////////////////////////////////////////////////////////////
//
// Function:    RefreshCtrl()
//
// Description:
//  Sets the contents of the combobox according to the MRU list.
//
// Input:
//  Nothing.
//
// Returns:
//  Nothing.
//
//////////////////////////////////////////////////////////////////////////

void CMRUComboBox::RefreshCtrl()
{
CString cstrComboText;

                                        // Save the contents of the edit
                                        // portion of the combobox.
    GetWindowText ( cstrComboText );

    ResetContent();

    for ( int i = 0; i < m_pMRU->GetSize(); i++ )
        {
                                        // Don't add empty strings to the combobox.
        if ( (*m_pMRU)[i].GetLength() > 0 )
            {
            if ( AddString ( (*m_pMRU)[i] ) < 0 )
                {
                TRACE1("CMRUComboBox -- Warning - RefreshCtrl() couldn't add MRU item %d to combobox.\n",
                       i );
                }
            }
        }

                                        // Restore the editbox text.
    SetWindowText ( cstrComboText );
}


/////////////////////////////////////////////////////////////////////////////
// CMRUComboBox data accessor functions


//////////////////////////////////////////////////////////////////////////
//
// Function:    SetMRURegKey()
//
// Description:
//  Sets the registry key (or INI file section) in which the MRU will be
//  saved.
//
// Input:
//  szRegKey: [in] The key/section name.
//
// Returns:
//  Nothing.
//
//////////////////////////////////////////////////////////////////////////

void CMRUComboBox::SetMRURegKey ( LPCTSTR szRegKey )
{
                                        // The key name can't be a null string.
    ASSERT ( NULL != szRegKey );

    try
        {
                                        // Store the reg key name & set the
                                        // changed flag.
        m_cstrRegKey = szRegKey;

        m_bParamsChanged = TRUE;
        }
    catch ( CMemoryException )
        {
        TRACE0("CMRUComboBox -- Memory exception in CMRUComboBox::SetMRURegKey()!\n");
        throw;
        }
}


//////////////////////////////////////////////////////////////////////////
//
// Function:    GetMRURegKey
//
// Description:
//  Returns the current registry key or INI file section in which the MRU
//  will be saved.
//
// Input:
//  Nothing.
//
// Returns:
//  The key name.
//
//////////////////////////////////////////////////////////////////////////

const CString& CMRUComboBox::GetMRURegKey() const
{
    return m_cstrRegKey;
}


//////////////////////////////////////////////////////////////////////////
//
// Function:    SetMRUValueFormat()
//
// Description:
//  Sets the format to be used for writing MRU items to the registry or
//  the app's INI file.
//
// Input:
//  szValueFormat: [in] The format to use.
//
// Returns:
//  TRUE if the format is acceptable (i.e., contains "%d"), FALSE if not.
//
//////////////////////////////////////////////////////////////////////////

BOOL CMRUComboBox::SetMRUValueFormat ( LPCTSTR szValueFormat )
{
BOOL bRetVal = FALSE;

                                        // The key name can't be a null string.
    ASSERT ( NULL != szValueFormat );


                                        // Check that the format strings 
                                        // contains "%d"
    if ( NULL == _tcsstr ( szValueFormat, _T("%d") ) )
        {
        TRACE0("CMRUComboBox -- SetMRUValueFormat() returning FALSE - argument didn't contain \"%d\"\n");
        return FALSE;
        }
    else
        {
        try
            {
                                        // Save the format string and set the
                                        // changed flag.
            m_cstrRegValueFormat = szValueFormat;
            m_bParamsChanged = TRUE;
            bRetVal = TRUE;
            }
        catch ( CMemoryException )
            {
            TRACE0("CMRUComboBox -- Memory exception in CMRUComboBox::SetMRUValueFormat()!\n");
            throw;
            }
        }

    return bRetVal;
}


//////////////////////////////////////////////////////////////////////////
//
// Function:    GetMRUValueFormat
//
// Description:
//  Returns the current registry value format string.
//
// Input:
//  Nothing.
//
// Returns:
//  The format string.
//
//////////////////////////////////////////////////////////////////////////

const CString& CMRUComboBox::GetMRUValueFormat() const
{
    return m_cstrRegValueFormat;
}


//////////////////////////////////////////////////////////////////////////
//
// Function:    SetMaxMRUSize()
//
// Description:
//  Sets the max number of entries that the MRU can hold.
//
// Input:
//  nMaxSize: [in] The new MRU size.
//
// Returns:
//  The previous MRU size, or 0 if there was no previous MRU allocated, or
//  -1 on error.
//
// Notes:
//  This function always reallocates a CRecentFileList so that the size
//  change (and registry string changes, if any) takes place immediately.
//
//////////////////////////////////////////////////////////////////////////

int CMRUComboBox::SetMaxMRUSize ( int nMaxSize )
{
int nRetVal = m_nMaxMRUSize;

                                        // New size needs to be a positive
                                        // number.
    ASSERT ( nMaxSize >= 1 );

    if ( nMaxSize <= 0 )
        return -1;


    m_nMaxMRUSize = nMaxSize;

    if ( NULL == m_pMRU )
        {
        nRetVal = 0;                    // no previous size
        }

    if ( !AllocNewMRU() )
        {
        nRetVal = -1;                   // error!!
        TRACE0("CMRUComboBox -- SetMaxMRUSize() failed - couldn't allocate new CRecentFileList.\n");
        }

    return nRetVal;
}


//////////////////////////////////////////////////////////////////////////
//
// Function:    GetMaxMRUSize
//
// Description:
//  Returns the current max MRU size.
//
// Input:
//  Nothing.
//
// Returns:
//  The current max size.
//
//////////////////////////////////////////////////////////////////////////

int CMRUComboBox::GetMaxMRUSize() const
{
    return m_nMaxMRUSize;
}


//////////////////////////////////////////////////////////////////////////
//
// Function:    SetAutoSaveOnDestroy()
//
// Description:
//  Sets whether the CMRUComboBox will automatically save the MRU when
//  the object is destroyed.
//
// Input:
//  bAutoSave: [in] Flag: enable auto-saving?
//
// Returns:
//  The previous value of this setting.
//
//////////////////////////////////////////////////////////////////////////

BOOL CMRUComboBox::SetAutoSaveOnDestroy ( BOOL bAutoSave )
{
BOOL bRetVal = m_bSaveOnDestroy;

    m_bSaveOnDestroy = bAutoSave;

    return bRetVal;
}


//////////////////////////////////////////////////////////////////////////
//
// Function:    SetAutoSaveAfterAdd()
//
// Description:
//  Sets whether the CMRUComboBox will automatically save the MRU after
//  an item is added successfully.
//
// Input:
//  bAutoSave: [in] Flag: enable auto-saving?
//
// Returns:
//  The previous value of this setting.
//
//////////////////////////////////////////////////////////////////////////

BOOL CMRUComboBox::SetAutoSaveAfterAdd ( BOOL bAutoSave )
{
BOOL bRetVal = m_bSaveAfterAdd;

    m_bSaveAfterAdd = bAutoSave;

    return bRetVal;
}


//////////////////////////////////////////////////////////////////////////
//
// Function:    SetAutoRefreshAfterAdd()
//
// Description:
//  Sets whether the CMRUComboBox will automatically refresh the combobox
//  control after an item is added successfully.
//
// Input:
//  bAutoSave: [in] Flag: enable auto-refresh?
//
// Returns:
//  The previous value of this setting.
//
//////////////////////////////////////////////////////////////////////////

BOOL CMRUComboBox::SetAutoRefreshAfterAdd ( BOOL bAutoSave )
{
BOOL bRetVal = m_bRefreshAfterAdd;

    m_bRefreshAfterAdd = bAutoSave;

    return bRetVal;
}


/////////////////////////////////////////////////////////////////////////////
// CMRUComboBox misc. functions


//////////////////////////////////////////////////////////////////////////
//
// Function:    VerifyMRUParams()
//
// Description:
//  Checks the registry and size parameters and makes sure they're valid
//  for CRecentFileList.
//
// Input:
//  Nothing. (uses member variables)
//
// Returns:
//  TRUE if the params are OK, FALSE if not.
//
//////////////////////////////////////////////////////////////////////////

BOOL CMRUComboBox::VerifyMRUParams() const
{
BOOL bRetVal = TRUE;

                                        // 1. The registry key string must be
                                        //    non-empty.
    if ( m_cstrRegKey.IsEmpty() || 0 == m_cstrRegKey.GetLength() )
        {
        TRACE0("CMRUComboBox -- VerifyMRUParams() - registry key name not set.\n");
        bRetVal = FALSE;
        }

                                        // 2. The reg value must be non-empty
                                        //    and contain "%d"
    if ( m_cstrRegValueFormat.IsEmpty() || 
         0 == m_cstrRegValueFormat.GetLength() )
        {
        TRACE0("CMRUComboBox -- VerifyMRUParams() - registry value format not set.\n");
        bRetVal = FALSE;
        }
    else if ( -1 == m_cstrRegValueFormat.Find ( _T("%d") ) )
        {
        TRACE0("CMRUComboBox -- VerifyMRUParams() - registry value format doesn't contain \"%d\"\n");
        bRetVal = FALSE;
        }

                                        // 3. The Max MRU size must be > 0.
    if ( m_nMaxMRUSize <= 0 )
        {
        TRACE0("CMRUComboBox -- VerifyMRUParams() - max MRU size is set to <= 0\n");
        bRetVal = FALSE;
        }


    return bRetVal;
}


//////////////////////////////////////////////////////////////////////////
//
// Function:    AllocNewMRU()
//
// Description:
//  Allocates a new CRecentFileList, and copies the contents of the previous
//  MRU (if any) to the new one.
//
// Input:
//  Nothing.
//
// Returns:
//  TRUE if successful, FALSE if not.
//
//////////////////////////////////////////////////////////////////////////

BOOL CMRUComboBox::AllocNewMRU()
{
CString* acstrOldList = NULL;
int      nItemsToCopy;
int      i;

                                        // Make sure the MRU params are OK.
    if ( !VerifyMRUParams() )
        {
        TRACE0("CMRUComboBox -- AllocNewMRU() returning FALSE - MRU list params invalid or not set.\n");
        return FALSE;
        }


    try
        {
                                        // Figuring out how many strings to
                                        // copy: The lesser of the new MRU 
                                        // size and the previous MRU's size.
                                        // Of course, if there was no previous
                                        // MRU, then nothing will be copied.
        nItemsToCopy = m_nMaxMRUSize;

        if ( NULL != m_pMRU )
            {
            nItemsToCopy = __min ( m_nMaxMRUSize, m_pMRU->GetSize() );

                                        // Save the contents of the old MRU list.
            acstrOldList = new CString [ nItemsToCopy ];

            for ( i = 0; i < nItemsToCopy; i++ )
                {
                acstrOldList[i] = (*m_pMRU)[i];
                }

                                        // Nuke the old CRecentFileList object...
            delete m_pMRU;
            }

                                        // and make a new one!
        m_pMRU = new CRecentFindFileList ( 1, m_cstrRegKey,
                                           m_cstrRegValueFormat, m_nMaxMRUSize);


        // Copy the MRU strings if there was a previous MRU.  We add
        // the strings in reverse numerical order so they end up in the same
        // order as they were in the old MRU.

        if ( NULL != acstrOldList )
            {
            for ( i = nItemsToCopy - 1; i >= 0; i-- )
                {
                m_pMRU->Add ( acstrOldList[i] );
                }

            delete [] acstrOldList;
            }
        }
    catch ( CMemoryException )
        {
        TRACE0("CMRUComboBox -- Memory exception in AllocNewMRU()!\n");
        
        if ( NULL != m_pMRU )
            {
            delete m_pMRU;
            }

        throw;
        }

                                        // Reset the changed flag.
    m_bParamsChanged = FALSE;

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMenuButton

CMenuButton::CMenuButton()
{
}

CMenuButton::~CMenuButton()
{
}


BEGIN_MESSAGE_MAP(CMenuButton, CButton)
	//{{AFX_MSG_MAP(CMenuButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuButton message handlers

DWORD CMenuButton::DoSingleClick(UINT uiIDMenuResource)
{
    // -[Feral]-----------------------------------------------------------
    // Vars and Assignment
    CMenu menu;
    int iMenuID = 0;
    DWORD dwSelectionMade;
    VERIFY(menu.LoadMenu(uiIDMenuResource) );

    CMenu *pmenuPopup = menu.GetSubMenu(iMenuID);
    ASSERT(pmenuPopup != NULL);
    POINT point;

    // -[Feral]-----------------------------------------------------------
    // get the point where the menu is to be displayed.
    // this is hte lower left corner of the control (button)
    CRect rect;
    GetWindowRect(rect);
    point.x = rect.left;
    point.y = rect.bottom;

    // -[Feral]-----------------------------------------------------------
    // show and track the menu
    dwSelectionMade = pmenuPopup->TrackPopupMenu( (TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD),
                                                  point.x, point.y, this
                                                );
    // -[Feral]-----------------------------------------------------------
    // Clean Up and Return
    pmenuPopup->DestroyMenu();
    // The value of dwSelectionMade is the id of the command selected or 0 if no selection was made
    return dwSelectionMade;
}

void CMenuButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // -[Feral]-----------------------------------------------------------
    // VALIDATION:
    ASSERT(lpDrawItemStruct != NULL);

    // -[Feral]-----------------------------------------------------------
    // Vars and Assignment
    CRect rect      = lpDrawItemStruct->rcItem;
    CDC *pDC        = CDC::FromHandle(lpDrawItemStruct->hDC);
    UINT uiState    = lpDrawItemStruct->itemState;
    CPen pen;
    CPen *ppenOld   = NULL;

    // -[Feral]-----------------------------------------------------------
    // set the pen color based on if we are disabled or not
    if( (uiState&ODS_DISABLED) )
    {
        pen.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_GRAYTEXT) );
    }
    else
    {
        pen.CreatePen(PS_SOLID, 0, ::GetSysColor(COLOR_BTNTEXT) );
    }


    // -[Feral]-----------------------------------------------------------
    // select the pen into the DC.
    ppenOld = pDC->SelectObject(&pen);


    // -[Feral]-----------------------------------------------------------
    // draw the border
    if( (uiState&ODS_SELECTED) )
    {
        pDC->DrawFrameControl(rect, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED);
    }
    else
    {
        pDC->DrawFrameControl(rect, DFC_BUTTON, DFCS_BUTTONPUSH);
    }


    // -[Feral]-----------------------------------------------------------
    // Draw the Arrow...
    // Something like:
    // X
    // XX
    // XXX
    // XXXX
    // XXX
    // XX
    // X
    // In the Center of the button.
    CSize sizeArrow(4,7);
    CSize sizeOffsetTop(1,+1);          // size of the top stairsteps
    CSize sizeOffsetBottom(1,-1);       // size of the bottom stairsteps
    CPoint pointCenter( (rect.Width()/2), (rect.Height()/2) );
    CPoint pointStart( (pointCenter.x-(sizeArrow.cx/2) ), (pointCenter.y-(sizeArrow.cy/2) ) );
    CPoint pointStop ( (pointStart.x), (pointStart.y+sizeArrow.cy) );
    // -[Feral]-----------------------------------------------------------
    // start at the left and work to the right...
    for(int iInd=0; iInd<sizeArrow.cx; iInd++)
    {
        // -[Feral]-------------------------------------------------------
        // draw the lines
        pDC->MoveTo(pointStart);
        pDC->LineTo(pointStop);

        // -[Feral]-------------------------------------------------------
        // offset our points. (going right!)
        pointStart.Offset(sizeOffsetTop);
        pointStop .Offset(sizeOffsetBottom);
    }

    // -[Feral]-----------------------------------------------------------
    // Draw the focus rectangle if necessary.
    if( (uiState&ODS_FOCUS) )
    {
        CRect rectFocus(rect);
        rectFocus.DeflateRect(3,3);     // This looked prety close
        pDC->DrawFocusRect(rectFocus);
    }

    // -[Feral]-----------------------------------------------------------
    // Clean Up and Return
    pDC->SelectObject(ppenOld);         // Restore the pen
}

/////////////////////////////////////////////////////////////////////////////
// CFindTextCombo

CFindTextCombo::CFindTextCombo()
{
}

CFindTextCombo::~CFindTextCombo()
{
}


BEGIN_MESSAGE_MAP(CFindTextCombo, CMRUComboBox)
	//{{AFX_MSG_MAP(CFindTextCombo)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CFindTextCombo 


void CFindTextCombo::OnLButtonUp(UINT nFlags, CPoint point) 
{
    // a combobox control receives this message when it loses the focus
    // in opposition to CBN_KILLFOCUS it returns correct edit selection values
	
	CMRUComboBox::OnLButtonUp(nFlags, point);

    CFindfileDlg * pDlg = (CFindfileDlg *)GetParent();
    pDlg->SetFindTextSel(GetEditSel());
}


#define REGEXPLEN  40
typedef struct _tagInsertRegExpType
{
    DWORD dwMenuId;
    TCHAR szRegExp[REGEXPLEN];
    int   iIndent;
}
FIF_INSERT_REGEXP_TYP;

FIF_INSERT_REGEXP_TYP InsertRegExp[] =
{
{FIF_IDM_ANY_CHAR                , _T("."),        1},
{FIF_IDM_CHAR_IN_RANGE           , _T("[]"),       1},
{FIF_IDM_CHAR_NOT_IN_RANGE       , _T("[^]"),      2},
{FIF_IDM_BEGIN_OF_LINE           , _T("^"),        1},
{FIF_IDM_END_OF_LINE             , _T("$"),        1},
{FIF_IDM_OR                      , _T("|"),        1},
{FIF_IDM_0_OR_MORE               , _T("*"),        1},
{FIF_IDM_1_OR_MORE               , _T("+"),        1},
{FIF_IDM_GROUP                   , _T("()"),       1},
{FIF_IDM_WHITESPACE              , _T("[[:space:]]"),              11},
{FIF_IDM_ALPHANUM_CHAR           , _T("[A-Za-z0-9]"),              11},
{FIF_IDM_ALPHABET_CHAR           , _T("[A-Za-z]"),                  8},
{FIF_IDM_DECIMAL_DIGIT           , _T("[0-9]"),                     5},
{FIF_IDM_HEX_NUMBER              , _T("16#([A-Fa-f0-9]+)"),        17},
{FIF_IDM_NUMBER                  , _T("([0-9]\\.[0-9]*)|(\\.[0-9]+)|([0-9]+)"), 35},
{FIF_IDM_INTEGER                 , _T("([0-9]{1})"),               10},
{FIF_IDM_IDENTIFIER              , _T("([A-Za-z_][A-Za-z0-9_]*)"), 24},
{FIF_IDM_ALPHABET_STRING         , _T("[A-Za-z]+"),                 9},
{FIF_IDM_QUOTED_STRING           , _T("(\".*\"|'.*')"),            11}
};

/////////////////////////////////////////////////////////////////////////////
// CFindfileDlg Dialogfeld

CFindfileDlg::CFindfileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindfileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindfileDlg)
	m_bCaseSensitive = FALSE;
	m_strFindText = _T("");
	m_bRegExp = FALSE;
	m_bWholeWords = FALSE;
	//}}AFX_DATA_INIT

    m_dwFindTextSel = 0;
    m_uiOption = 0;
}

void CFindfileDlg::SetFindTextSel(DWORD dwFindTextSel)
{
    m_dwFindTextSel = dwFindTextSel;
}

void CFindfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

    CWinApp * pApp = AfxGetApp();

    if (!pDX->m_bSaveAndValidate)
    {
        if (pApp)
        {
            m_uiOption = pApp->GetProfileInt(FIF_OPTION_SECTION, FIF_OPTION_ENTRY, 0);
        }
        m_bCaseSensitive = !!(m_uiOption & FIF_OPTION_CASESENSITIVE);
        m_bRegExp        = !!(m_uiOption & FIF_OPTION_REGEXP);
        m_bWholeWords    = !!(m_uiOption & FIF_OPTION_WHOLEWORD);
    }

	//{{AFX_DATA_MAP(CFindfileDlg)
   	DDX_Control(pDX, FIF_IDC_INSERT_RE, m_mbInsertRegEx);
	DDX_CBString(pDX, FIF_IDC_FINDTEXT, m_strFindText);
	DDX_Check(pDX, FIF_IDC_CASE_SENSITIVE, m_bCaseSensitive);
	DDX_Check(pDX, FIF_IDC_REGULAR_EXP, m_bRegExp);
	DDX_Check(pDX, FIF_IDC_WHOLE_WORDS, m_bWholeWords);
	//}}AFX_DATA_MAP

    if (pDX->m_bSaveAndValidate)
    {
        m_uiOption = 0;

        if (m_bCaseSensitive)
        {
            m_uiOption |= FIF_OPTION_CASESENSITIVE;
        }

        if (m_bRegExp)
        {
            m_uiOption |= FIF_OPTION_REGEXP;
        }

        if (m_bWholeWords)
        {
            m_uiOption |= FIF_OPTION_WHOLEWORD;
        }

        if (pApp)
        {
            pApp->WriteProfileInt(FIF_OPTION_SECTION, FIF_OPTION_ENTRY, m_uiOption);
        }

        CMRUComboBox * pComboBox = (CMRUComboBox *)GetDlgItem(FIF_IDC_FINDTEXT);
        if (pComboBox)
        {
            pComboBox->AddToMRU(m_strFindText);
        }
    }
}

BEGIN_MESSAGE_MAP(CFindfileDlg, CDialog)
	//{{AFX_MSG_MAP(CFindfileDlg)
	ON_BN_CLICKED(FIF_IDC_INSERT_RE, OnInsertRegExp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindfileDlg Nachrichten-Handler

BOOL CFindfileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    m_FindTextCombo.SubclassDlgItem(FIF_IDC_FINDTEXT, this);
    CMRUComboBox * pComboBox = (CMRUComboBox *)GetDlgItem(FIF_IDC_FINDTEXT);
    if (pComboBox)
    {
        pComboBox->SetMRURegKey(FIF_SECTION);
        pComboBox->SetMRUValueFormat ( _T("#%d") );
        pComboBox->SetAutoRefreshAfterAdd ( TRUE );
        pComboBox->SetAutoSaveAfterAdd ( TRUE );
        pComboBox->LoadMRU();
        if(!m_strFindText.IsEmpty())
        {
            pComboBox->AddToMRU(m_strFindText);
        }
        pComboBox->RefreshCtrl();
        if (pComboBox->GetCount() > 0)
        {
            pComboBox->SetCurSel(0);
        }
    }

	return TRUE;  
}

void CFindfileDlg::OnInsertRegExp() 
{

    DWORD dwId = m_mbInsertRegEx.DoSingleClick(FIF_POPUP_MENU);
    if (dwId >= FIF_IDM_ANY_CHAR && dwId <= FIF_IDM_QUOTED_STRING)
    {
        CheckDlgButton(FIF_IDC_REGULAR_EXP, 1);  // check regular expression checkbox
        CComboBox * pComboBox = (CComboBox *)GetDlgItem(FIF_IDC_FINDTEXT);
        if (pComboBox)
        {
            int i;
            for (i = 0; i < sizeof(InsertRegExp) / sizeof(*InsertRegExp) ; i++)
            {
                if (dwId == InsertRegExp[i].dwMenuId)
                {
                    CString strText;
                    CString strInsText = InsertRegExp[i].szRegExp;
                    int iLen = strInsText.GetLength();
                    pComboBox->GetWindowText(strText);
                    strText = strText.Left(LOWORD(m_dwFindTextSel)) + strInsText + 
                              strText.Mid(HIWORD(m_dwFindTextSel));
                    pComboBox->SetWindowText(strText);
                    pComboBox->SetFocus();
                    pComboBox->SetEditSel(LOWORD(m_dwFindTextSel) + InsertRegExp[i].iIndent, 
                                          LOWORD(m_dwFindTextSel) + InsertRegExp[i].iIndent);
                    break;
                }
            }
        }
    }
}

