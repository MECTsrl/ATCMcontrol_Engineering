
/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "AddonBaseResource.h"
#include "PouProtectionDlg.h"
#include "GHFile.h"
#include "utilif.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPouProtectionDlg dialog

CPouProtectionDlg::CPouProtectionDlg(CGHFile* pGHFile, ICEProjInfo* pICEProjInfo, CWnd* pParent /*=NULL*/)
	: CDialog(CPouProtectionDlg::IDD, pParent)
{

    m_pGHFile = pGHFile;
    m_pICEProjInfo = pICEProjInfo;

    ASSERT(m_pGHFile);
    ASSERT(m_pICEProjInfo);

    // determine project path
    CComBSTR    sProjPath;
    HRESULT hr = m_pICEProjInfo->getProjectPath(&sProjPath);
    ASSERT(hr == S_OK);

    m_strProjPath = sProjPath;

	//{{AFX_DATA_INIT(CPouProtectionDlg)
	//}}AFX_DATA_INIT
}


void CPouProtectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPouProtectionDlg)
	DDX_Control(pDX, IDC_PROTECTED_POUS, m_tProtPouListCtrl);
	DDX_Control(pDX, IDC_UNPROTECTED_POUS, m_tUnprotPouListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPouProtectionDlg, CDialog)
	//{{AFX_MSG_MAP(CPouProtectionDlg)
	ON_BN_CLICKED(IDC_ADD_ALL_POUS, OnAddAllPous)
	ON_BN_CLICKED(IDC_ADD_POU, OnAddPou)
	ON_BN_CLICKED(IDC_REMOVE_ALL_POUS, OnRemoveAllPous)
	ON_BN_CLICKED(IDC_REMOVE_POU, OnRemovePou)
	ON_LBN_DBLCLK(IDC_PROTECTED_POUS, OnDblclkProtectedPous)
	ON_LBN_DBLCLK(IDC_UNPROTECTED_POUS, OnDblclkUnprotectedPous)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPouProtectionDlg message handlers


//------------------------------------------------------------------*
/**
 * add new pou to protection list.
 *
 * @param           crstrFileName: file name of POU
 * @return          -
 * @exception       -
 * @see             -
*/
void CPouProtectionDlg::AddNewPou(const CString& crstrFileName)
{
    m_tUnprotAssoc.deleteData(crstrFileName);
    m_tProtAssoc.insertData(crstrFileName, (bool*)2);
}

//------------------------------------------------------------------*
/**
 * remove new pou from protection list.
 *
 * @param           crstrFileName: file name of POU
 * @return          -
 * @exception       -
 * @see             -
*/
void CPouProtectionDlg::RemoveNewPou(const CString& crstrFileName)
{
    m_tProtAssoc.deleteData(crstrFileName);
    m_tUnprotAssoc.insertData(crstrFileName, (bool*)2);
}


//------------------------------------------------------------------*
/**
 * on add all pous.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPouProtectionDlg::OnAddAllPous() 
{
    // run over all unprotected pous and protect them
    CStringArray    astrPous;
    int             iNumPous;
    int             iPou;
    CString         strFileName;

    m_tUnprotAssoc.getAllStringsFromSubstring("", astrPous);
    iNumPous = astrPous.GetSize();
    if(iNumPous == 0)
    {
        return;
    }

    for(iPou = 0; iPou < iNumPous; ++iPou)
    {
        strFileName = astrPous[iPou];
        AddNewPou(strFileName);
    }

    FillListControls();
}

//------------------------------------------------------------------*
/**
 * on add pou to protection list.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPouProtectionDlg::OnAddPou() 
{
    CString     strFileName;
    int iCount = m_tUnprotPouListCtrl.GetSelCount();
    CArray<int,int> aiSelectedItems;

    aiSelectedItems.SetSize(iCount);
    m_tUnprotPouListCtrl.GetSelItems(iCount, aiSelectedItems.GetData());
    
    for(int iItem = 0; iItem < iCount; ++iItem)
    {
        m_tUnprotPouListCtrl.GetText(aiSelectedItems[iItem], strFileName);
        AddNewPou(strFileName);
    }
    FillListControls();
}


//------------------------------------------------------------------*
/**
 * on remove all pous.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPouProtectionDlg::OnRemoveAllPous() 
{
    // run over all protected pous and unprotect them
    CStringArray    astrPous;
    int             iNumPous;
    int             iPou;
    CString         strFileName;

    m_tProtAssoc.getAllStringsFromSubstring("", astrPous);
    iNumPous = astrPous.GetSize();
    if(iNumPous == 0)
    {
        return;
    }

    for(iPou = 0; iPou < iNumPous; ++iPou)
    {
        strFileName = astrPous[iPou];
        RemoveNewPou(strFileName);
    }

    FillListControls();
}


//------------------------------------------------------------------*
/**
 * on remove pou.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPouProtectionDlg::OnRemovePou() 
{
    CString     strFileName;
    int iCount = m_tProtPouListCtrl.GetSelCount();
    CArray<int,int> aiSelectedItems;

    aiSelectedItems.SetSize(iCount);
    m_tProtPouListCtrl.GetSelItems(iCount, aiSelectedItems.GetData());
    
    for(int iItem = 0; iItem < iCount; ++iItem)
    {
        m_tProtPouListCtrl.GetText(aiSelectedItems[iItem], strFileName);
        RemoveNewPou(strFileName);
    }
    FillListControls();
}


//------------------------------------------------------------------*
/**
 * on double click unprotected pous.
 *
 *  add selected pous.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPouProtectionDlg::OnDblclkUnprotectedPous() 
{
    OnAddPou();
}

//------------------------------------------------------------------*
/**
 * on double click protected pous.
 *
 *  remove selected pous.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPouProtectionDlg::OnDblclkProtectedPous() 
{
    OnRemovePou();
}

//------------------------------------------------------------------*
/**
 * on ok.
 *
 *  save data to gh file
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPouProtectionDlg::OnOK() 
{
    if(SaveToGhFile())
    {
        SetReadOnlyFlags();
    }
	CDialog::OnOK();
}


//------------------------------------------------------------------*
/**
 * on init dialog.
 *
 * @param           -
 * @return          see CDialog::OnInitDialog()
 * @exception       -
 * @see             -
*/
BOOL CPouProtectionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    LoadFromProject();
    LoadFromGhFile();
    SaveOldAssoc();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------*
/**
 * save to gh file.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
BOOL CPouProtectionDlg::SaveToGhFile()
{
    ASSERT(m_pGHFile);
    if(!m_pGHFile)
    {
        return FALSE;
    }
    m_pGHFile->ResetPouProtectData();
    CStringArray    astrPous;
    CString         strFileName;
    CString         strFingerprint;

    m_tProtAssoc.getAllStringsFromSubstring("", astrPous);
    int iNumPous = astrPous.GetSize();
    for(int iPou = 0; iPou < iNumPous; ++iPou)
    {
        strFileName = astrPous[iPou];
        VERIFY(DetermineFingerprint(strFileName, strFingerprint));
        m_pGHFile->AddPouProtectData(strFileName, strFingerprint);
    }

    // now write to file system
    return m_pGHFile->Write();
}


//------------------------------------------------------------------*
/**
 * load from project.
 *
 * @param           -
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CPouProtectionDlg::LoadFromProject()
{
    ASSERT(m_pICEProjInfo);
    if(!m_pICEProjInfo)
    {
        return FALSE;
    }

    // remove all unprotected files
    m_tUnprotAssoc.reset();

    BOOL bReturn = TRUE;
    if(!LoadPousOfTypeFromProject(_T("PROGRAM")))
    {
        bReturn = FALSE;
    }
    if(!LoadPousOfTypeFromProject(_T("FUNCTION_BLOCK")))
    {
        bReturn = FALSE;
    }
    if(!LoadPousOfTypeFromProject(_T("FUNCTION")))
    {
        bReturn = FALSE;
    }
    return bReturn;
}


//------------------------------------------------------------------*
/**
 * load pous of type from project.
 *
 * @param           crstrType: KAD type
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CPouProtectionDlg::LoadPousOfTypeFromProject(const CString& crstrType)
{
    SAFEARRAY* parrClasses;
    CComBSTR    sType(crstrType);
    HRESULT hr = m_pICEProjInfo->getClassesOfType(sType, &parrClasses);
    if (hr!=S_OK)
    {
        return FALSE;
    }

    BSTR HUGEP *pbstr;
    BSTR bstr;
    unsigned long i;

    // Get a pointer to the elements of the array.
    hr = ::SafeArrayAccessData(parrClasses, (void HUGEP**)&pbstr);
    if (FAILED(hr))
    {
        return FALSE;
    }

    CString strFileName;
    for (i = 0; i < ((parrClasses)->rgsabound[0]).cElements; i++)
    {
        bstr = pbstr[i];
        m_tUnprotAssoc.insertData((CString)bstr, (bool*)1);
    }

    ::SafeArrayUnaccessData(parrClasses);		
    ::SafeArrayDestroy(parrClasses);

    return TRUE;
}


//------------------------------------------------------------------*
/**
 * load from gh file.
 *
 *  read data from m_pGHFile class in dialog
 *  gh file data is already read and stored in m_pGHFile class.
 *
 * @param           -
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CPouProtectionDlg::LoadFromGhFile()
{
    ASSERT(m_pGHFile);
    if(!m_pGHFile)
    {
        return FALSE;
    }
    POSITION    pos = m_pGHFile->GetFirstPouProtectDataPosition();
    CPouProtectData tData;

    while(pos)
    {
        m_pGHFile->GetNextPouProtectData(pos, tData);
        // remove file from unprotected and add to protected
        VERIFY(m_tUnprotAssoc.deleteData(tData.m_strFileName));
        VERIFY(m_tProtAssoc.insertData(tData.m_strFileName, (bool*)1));
    }

    // now fill list controls
    FillListControls();

    return TRUE;
}

//------------------------------------------------------------------*
/**
 * save old pou protection data in association memory.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPouProtectionDlg::SaveOldAssoc()
{
    CStringArray    astrPous;
    int             iNumPous;
    int             iPou;

    m_tUnprotOldAssoc.reset();
    m_tProtOldAssoc.reset();

    m_tUnprotAssoc.getAllStringsFromSubstring("", astrPous);
    iNumPous = astrPous.GetSize();
    for(iPou = 0; iPou < iNumPous; ++iPou)
    {
        m_tUnprotOldAssoc.insertData(astrPous[iPou], (bool*)1);
    }

    astrPous.RemoveAll();
    m_tProtAssoc.getAllStringsFromSubstring("", astrPous);
    iNumPous = astrPous.GetSize();
    for(iPou = 0; iPou < iNumPous; ++iPou)
    {
        m_tProtOldAssoc.insertData(astrPous[iPou], (bool*)1);
    }
}


//------------------------------------------------------------------*
/**
 * fill list controls.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CPouProtectionDlg::FillListControls()
{
    ASSERT(m_tUnprotPouListCtrl.GetSafeHwnd());
    ASSERT(m_tProtPouListCtrl.GetSafeHwnd());

    m_tUnprotPouListCtrl.ResetContent();
    m_tProtPouListCtrl.ResetContent();

    CStringArray    astrPous;
    CString         strFileName;
    int             iNumPous;
    int             iPou;
    int             iTmp;

    m_tUnprotAssoc.getAllStringsFromSubstring("", astrPous);
    iNumPous = astrPous.GetSize();
    for(iPou = 0; iPou < iNumPous; ++iPou)
    {
        strFileName = astrPous[iPou];
        m_tUnprotPouListCtrl.AddString(strFileName);
        iTmp = (int)m_tUnprotAssoc.getData(strFileName);
        if(iTmp == 2)
        {
            m_tUnprotPouListCtrl.SetSel(iPou);
            m_tUnprotAssoc.insertData(strFileName, (bool*)1, true);
        }
    }

    astrPous.RemoveAll();
    m_tProtAssoc.getAllStringsFromSubstring("", astrPous);
    iNumPous = astrPous.GetSize();
    for(iPou = 0; iPou < iNumPous; ++iPou)
    {
        strFileName = astrPous[iPou];
        m_tProtPouListCtrl.AddString(strFileName);
        iTmp = (int)m_tProtAssoc.getData(strFileName);
        if(iTmp == 2)
        {
            m_tProtPouListCtrl.SetSel(iPou);
            m_tProtAssoc.insertData(strFileName, (bool*)1, true);
        }
    }
}


//------------------------------------------------------------------*
/**
 * determine fingerprint.
 *
 * @param           crstrFileName: file name from project manager
 * @param           rstrFingerprint: fingerprint string
 * @return          -
 * @exception       -
 * @see             -
*/
BOOL CPouProtectionDlg::DetermineFingerprint(const CString& crstrFileName, CString& rstrFingerprint)
{

    // read file and calculate fingerprint
    CString strTmp;
    CFileStatus tStatus;
    CString strFilePathName = GetFilePathName(crstrFileName);

    if(!CFile::GetStatus(strFilePathName, tStatus))
    {
        return FALSE;
    }
    long    lSize = tStatus.m_size;

    CFile   tFile;
    try
    {
        tFile.Open(strFilePathName, CFile::modeRead);
        tFile.Read(strTmp.GetBuffer(lSize+1), lSize);
        strTmp.ReleaseBuffer();
        strTmp.SetAt(lSize,0);
        tFile.Close();
    }
    catch(CFileException* pE)
    {
        pE->Delete();
        return FALSE;
    }

    HRESULT hr;
    hr = UTIL_GetStringFingerprint(strTmp, rstrFingerprint.GetBuffer(33));
    rstrFingerprint.ReleaseBuffer();
    if(hr != S_OK)
    {
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * get file path name.
 *
 * @param           crstrFileName: file name
 * @return          file name incl path
 * @exception       -
 * @see             -
*/
CString CPouProtectionDlg::GetFilePathName(const CString& crstrFileName)
{
    // check if file name is absolute
    CString strFilePathName;
    HRESULT hr;
    hr = UTIL_IsAbsolutePath(crstrFileName);
    if(FAILED(hr))
    {
        return strFilePathName;
    }
    
    if(hr == S_OK)
    {
        strFilePathName = crstrFileName;
    }
    else
    {
        // project path must be != empty
        ASSERT(!m_strProjPath.IsEmpty());
        strFilePathName.Format("%s\\%s", m_strProjPath, crstrFileName);
    }
    return strFilePathName;
}


//------------------------------------------------------------------*
/**
 * set read only flags.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPouProtectionDlg::SetReadOnlyFlags()
{
    // run over all newly protected files and set read only flag
    CFileStatus     tStatus;
    CStringArray    astrPous;
    int             iNumPous;
    int             iPou;
    CString         strFilePathName;

    m_tProtAssoc.getAllStringsFromSubstring("", astrPous);
    iNumPous = astrPous.GetSize();
    for(iPou = 0; iPou < iNumPous; ++iPou)
    {
//        if(!m_tProtOldAssoc.getData(astrPous[iPou]))
//        {
            strFilePathName = GetFilePathName(astrPous[iPou]);
            if(CFile::GetStatus(strFilePathName, tStatus))
            {
                tStatus.m_attribute |= CFile::readOnly;
                try
                {
                    CFile::SetStatus(strFilePathName, tStatus);
                }
                catch(CFileException* pE)
                {
                    pE->Delete();
                }
            }
//        }
    }

    // run over all newly unprotected files and remove read only flag
    astrPous.RemoveAll();
    m_tUnprotAssoc.getAllStringsFromSubstring("", astrPous);
    iNumPous = astrPous.GetSize();
    for(iPou = 0; iPou < iNumPous; ++iPou)
    {
//        if(!m_tUnprotOldAssoc.getData(astrPous[iPou]))
//        {
            strFilePathName = GetFilePathName(astrPous[iPou]);
            if(CFile::GetStatus(strFilePathName, tStatus))
            {
                tStatus.m_attribute &= ~CFile::readOnly;
                try
                {
                    CFile::SetStatus(strFilePathName, tStatus);
                }
                catch(CFileException* pE)
                {
                    pE->Delete();
                }
            }
//        }
    }
}

/* ---------------------------------------------------------------------------- */
