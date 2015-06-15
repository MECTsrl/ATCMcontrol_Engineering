
//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "ConfigGSD.h"
#include "ConfigGSDBasic.h"
#include "SheetConfigGSD.H"
#include "defines.h"
//----  Local Defines:   -------------------------------------------*


/////////////////////////////////////////////////////////////////////////////
// CConfigGSDBasic

//---------------------------- Constructor -----------------------------------*
CConfigGSDBasic::CConfigGSDBasic()
: m_szFileName()
{
}

//----------------------------- Destructor -----------------------------------*
CConfigGSDBasic::~CConfigGSDBasic()
{

}

HRESULT CConfigGSDBasic::FinalConstruct()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    
    CWnd* pWnd = CWnd::GetForegroundWindow();
	Sheet.Construct(IDS_SHEET_CAPTION, pWnd, 0);
	Sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
    return S_OK;
}

//----------------------------- FUNCTION -------------------------------------*
STDMETHODIMP CConfigGSDBasic::SetGSDFileName(BSTR sFileGSD)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	if (sFileGSD == NULL)	{
		m_szFileName.Empty();
		return E_FAIL;
	}
	else	{
/*		CString sFile(sFileGSD);
		OFSTRUCT of;

		HFILE hf = ::OpenFile(sFile, &of, OF_READ);
		if (hf == HFILE_ERROR)	{
			m_szFileName.Empty();
			return E_FAIL;
		}*/
		m_szFileName = sFileGSD;
	}
	return S_OK;
}

//----------------------------- FUNCTION -------------------------------------*
STDMETHODIMP CConfigGSDBasic::StartConfigGSD(BSTR sFileGSD)
{
    return StartConfigGSDEx(sFileGSD, NULL);
}

//----------------------------- FUNCTION -------------------------------------*
STDMETHODIMP CConfigGSDBasic::StartConfigGSDEx(BSTR sFileGSD, BSTR sDialogState)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	HRESULT hr = S_OK;
	if (sFileGSD == NULL && m_szFileName.IsEmpty())	{
		hr = GetGSDFile(m_szFileName);
		if (FAILED(hr))	{
			return hr;
		}
	}
	else if (sFileGSD != NULL)	{
		m_szFileName = sFileGSD;
	}

	Sheet.SetGSDFileName(m_szFileName);
	hr = Sheet.ParseAndInitPages();
	while(FAILED(hr))	{
		if (AfxMessageBox(_T("The given Filename wasn't valid!!\nDo you want to search for another file?"), MB_YESNO)
			== IDNO)	{
			return STG_E_FILENOTFOUND;
		}
		hr = GetGSDFile(m_szFileName);
		if (FAILED(hr))
			return STG_E_FILENOTFOUND;
		Sheet.SetGSDFileName(m_szFileName);
		hr = Sheet.ParseAndInitPages();
	}

    if (sDialogState != NULL && *sDialogState != _T('\0')) {
        CString state = sDialogState;
        hr = Sheet.SetState(state);
        if (hr != S_OK)
            AfxMessageBox(_T("Not all parameter values have been restored!!\nPlease make sure that the gsd file is of the correct version."), MB_OK);
    }

	if (Sheet.DoModal() == IDOK)
		return S_OK;
	else
		return E_ABORT;
}

//----------------------------- FUNCTION -------------------------------------*
STDMETHODIMP CConfigGSDBasic::get_Ident_Number(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	
	*pVal = Sheet.GetIdentNumber();
	
	return S_OK;
}

//----------------------------- FUNCTION -------------------------------------*
STDMETHODIMP CConfigGSDBasic::get_FileName(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	*pVal = m_szFileName.AllocSysString();

	return S_OK;
}

//----------------------------- FUNCTION -------------------------------------*
STDMETHODIMP CConfigGSDBasic::get_ConfigBytes(VARIANT * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	if (pVal == NULL)
		return E_POINTER;
	CByteArray	CfgBytes;
	Sheet.GetCfgByteArray(&CfgBytes);

	pVal->vt = VT_UI1 | VT_ARRAY;	// bytearray
	SAFEARRAYBOUND boundData;
	boundData.lLbound = 0;
	boundData.cElements = CfgBytes.GetSize();

	SAFEARRAY*	psaCfgData = ::SafeArrayCreate(VT_UI1, 1, &boundData);
	if (psaCfgData == NULL)
		return E_OUTOFMEMORY;

	// copy CfgBytes to SAFEARRAY
	HRESULT hr;
	const BYTE* pConfigBytes = CfgBytes.GetData();
//	_ASSERTE(pConfigBytes != NULL);
	BYTE* pRawData;
	hr = ::SafeArrayAccessData(psaCfgData, (void HUGEP* FAR*)&pRawData);
	::memcpy(pRawData, pConfigBytes, CfgBytes.GetSize());
	::SafeArrayUnaccessData(psaCfgData);
	pVal->parray = psaCfgData;

	return S_OK;
}

//----------------------------- FUNCTION -------------------------------------*
STDMETHODIMP CConfigGSDBasic::get_UserPrmData(VARIANT * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	if (pVal == NULL)
		return E_POINTER;
	CByteArray	UserBytes;
	Sheet.GetUserPrmData(&UserBytes);

	pVal->vt = VT_UI1 | VT_ARRAY;	// bytearray
	SAFEARRAYBOUND boundData;
	boundData.lLbound = 0;
	boundData.cElements = UserBytes.GetSize();

	SAFEARRAY*	psaUserData = ::SafeArrayCreate(VT_UI1, 1, &boundData);
	if (psaUserData == NULL)
		return E_OUTOFMEMORY;

	// copy CfgBytes to SAFEARRAY
	HRESULT hr;
	const BYTE* pUserBytes = UserBytes.GetData();
//	_ASSERTE(pConfigBytes != NULL);
	BYTE* pRawData;
	hr = ::SafeArrayAccessData(psaUserData, (void HUGEP* FAR*)&pRawData);
	::memcpy(pRawData, pUserBytes, UserBytes.GetSize());
	::SafeArrayUnaccessData(psaUserData);
	pVal->parray = psaUserData;

	return S_OK;
}

//----------------------------- FUNCTION -------------------------------------*
HRESULT CConfigGSDBasic::GetGSDFile(CString& szFileName)
{
    TCHAR szCurrentDir[MAX_PATH];

	CFileDialog	FileDlg(TRUE, NULL, NULL, 0, _T("GSD Files (*.gs*)|*.gs*||"), NULL);

    // set current working directory as initial dir
    _tgetcwd(szCurrentDir, sizeof(szCurrentDir) / sizeof(*szCurrentDir));
    FileDlg.m_ofn.lpstrInitialDir = szCurrentDir;

	if (FileDlg.DoModal() == IDOK)	{
		szFileName  = FileDlg.GetPathName();
	}
	else	{
		szFileName = _T("No spezific File loaded");
		return E_ABORT;
	}
	return S_OK;
}

//-------------------------------  Function  -----------------------------------
STDMETHODIMP CConfigGSDBasic::SetHelpConnection(IConfigGSDHelp* pHelp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	// TODO: Add your implementation code here
	Sheet.SetHelpConnection(pHelp);

	return S_OK;
}

//-------------------------------  Function  -----------------------------------
STDMETHODIMP CConfigGSDBasic::get_GSDObject(LPUNKNOWN* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	// TODO: Add your implementation code here
	
	return Sheet.GetGSDObject(pVal);
}

//-------------------------------  Function  -----------------------------------
STDMETHODIMP CConfigGSDBasic::get_DialogState(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    if (pVal == NULL)
        return E_POINTER;

    *pVal = Sheet.GetState().AllocSysString();

    return S_OK;
}


