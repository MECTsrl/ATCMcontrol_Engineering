/* ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Install.h $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/Install.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				CSET
 *
 * =COMPONENT			Tools\Setup
 *
 * =CURRENT 	 $Date: 28.02.07 19:01 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _INSTALL_H_
#define _INSTALL_H_


/////////////////////////////////////////////////////////////////////////////
// CState

class CState : public CObject
{
	public:
		CState(LPCTSTR szKey, int uState, LPCTSTR szPara = NULL)
		{
			m_sKey		= szKey;
			m_uState	= uState;
			if (szPara != NULL)
			{
				m_sPara	= szPara;
			}
		}
	private:
		CState()
		{
		};

	public:
		CString m_sKey;
		UINT	m_uState;
		CString m_sPara;

};


class CPropBase;

/////////////////////////////////////////////////////////////////////////////
// CInstall

class CInstall : public CObject
{
	DECLARE_DYNAMIC(CInstall);
	public:
		CInstall();
		void InitializeInstaller();

	public:
		virtual ~CInstall();


	/* Common Informations
	 */
	protected:
		BOOL	m_bInstallAll;
		BOOL	m_bUnInstallAll;
		BOOL	m_bUnInstallOnly;
		BOOL	m_bAutoClose;

		BOOL	m_bDebugLog;

		CString	m_sType;

		CString m_sTitle;
		CString m_sModule;
		CString	m_sProduct;
		
		CString m_sInstallPath;
		CString m_sInstallPathCmdLine;
		DWORD	m_dwLanguage;
		
	public:
		void	SetInstallPath(LPCTSTR szInstallPath);
		LPCTSTR GetInstallPath();
		void	SetInstallPathCmdLine(LPCTSTR szInstallPath);
		void	SetInstallLang(DWORD dwInstallLang);
		LPCTSTR GetTitle();
		void	SetDebugLog(BOOL bOn);
		void	SetInstallAll(BOOL bInstallAll);
		void	SetUnInstallAll(BOOL bUnInstallAll);
		void	SetUnInstallOnly(BOOL bUnInstallOnly);
		BOOL	GetInstallAll();
		BOOL	GetUnInstallAll();
		BOOL	GetUnInstallOnly();
		void	SetAutoClose(BOOL bAutoClose);
		BOOL	GetAutoClose();
		void	SetType(LPCSTR szType);
		LPCTSTR	GetType();
		

	/* Common helper functions
	 */
	public:
		BOOL IsWindowsNTBased(void);
		BOOL IsWindowsVista(void);
		BOOL IsWindows2K3(void);
		BOOL IsWindowsXP(void);
		BOOL IsWindows2K(void);
		BOOL IsWindowsNT4(void);
		BOOL IsAdmin(void);
		BOOL IsDirectory(LPCTSTR lpszPath);
		BOOL GetFileVersion(LPCTSTR szPath, CString &sVersion);
		BOOL GetInstallPath(CString &sPath);
		UINT CheckVersionNumber(LPCTSTR szVersion, LPCTSTR szMin);
		UINT GetLanguage();
		BOOL GetLanguageSuffix(CString &sSuffix);
		BOOL TranslateKey(CString &sKey);
		
		LPCTSTR GetEmbeddedKey();


	/* Ini File handling
	 */
	protected:
		CString m_sIniCommon;
		CString m_sIniSpecial;
		CString m_sIniLanguage;

	public:
		void SetIniFile(LPCTSTR szIniCommon, LPCTSTR szIniSpecial, LPCTSTR szIniLanguage);
		BOOL GetExeDir(CString &sDir);
		BOOL GetIniFileCommon(CString &sIniCommon);
		BOOL GetIniFileSpecial(CString &sIniSpecial);
		BOOL GetIniFileLanguage(CString &sIniLanguage);
		BOOL GetKeyFromIni(LPCTSTR szSection, LPCTSTR szKey, CString &sValue);
		BOOL CheckValueInIni(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue);


	/* Registry access
	 */
	public:
		BOOL CreateRegKey  (HKEY hKey, LPCSTR szSub, LPCSTR szVal, DWORD dwType, LPBYTE pValue, DWORD dwSize);
		BOOL DeleteRegKey  (HKEY hKey, LPCSTR szKey, LPCSTR szSub);
		BOOL DeleteRegValue(HKEY hKey, LPCSTR szSub, LPCSTR szVal);
		BOOL QueryRegValue (HKEY hKey, LPCSTR szSub, LPCSTR szVal, LPBYTE pValue, DWORD *pdwSize);
		BOOL SetRegValue   (HKEY hKey, LPCSTR szSub, LPCSTR szVal, DWORD dwType, LPBYTE pValue, DWORD dwSize);
		BOOL TestRegKey	   (HKEY hKey, LPCSTR szSub);
		BOOL CreateRunOnce (CString &sValue);


	/* Tracing
	 */
	protected:
		HANDLE	m_hTrace;
		BOOL	m_bTrace;

	protected:
		void InitTrace();
		void FinalizeTrace();

	public:
		void SetTrace(BOOL bTrace);
		BOOL IsTrace();
		void Trc(LPCTSTR szFun, LPCTSTR szAction, LPCTSTR szText, ...);

		
	/* System Checking
	 */
	public:
		UINT IsOSValid(CString &sOsCur, CString &sOsMin, BOOL *pSP);
		BOOL CheckSP(CString &sOsCur, CString &sOsMin, BOOL *pValidOS);
		BOOL CheckDEP(void);
		BOOL CheckIE(CString &sVersion);
		BOOL CheckMSI(CString &sVersion);
		BOOL CheckJVM(CString &sVersion);
		BOOL CheckJDK(CString &sVersion);
		BOOL CheckIIS(CString &sVersion);
		UINT CheckLM(CString &sVersion);
		UINT Check4C(LPCSTR szKey, CString &sVersion);


	/* Installation
	 */
	public:
		BOOL InstallIE(CPropBase *pProp);
		BOOL InstallMSI(CPropBase *pProp);
		BOOL InstallSP(CPropBase *pProp);
		BOOL InstallJDK(CPropBase *pProp);
		BOOL InstallJVM(CPropBase *pProp);
		BOOL InstallIIS(CPropBase *pProp);
		BOOL Install4C(CPropBase *pProp, LPCTSTR szKey);

		BOOL UnInstall4C(CPropBase *pProp, LPCTSTR szKey);

	protected:
		BOOL InstallMSProduct(CPropBase *pProp, LPCTSTR szKey, LPCTSTR szPath, BOOL bReboot);
		BOOL Execute(CPropBase *pMain, LPCTSTR szKey, LPCTSTR szPath);
		BOOL Install4CComponent(CPropBase *pMain, LPCTSTR szKey);
		BOOL UnInstall4CComponent(CPropBase *pMain, LPCTSTR szKey);
		UINT UnInstallMsiPackage(LPCTSTR szCode, HWND hWnd, HINSTANCE hMSI, INSTALLUILEVEL dwUILevel);
		UINT InstallMsiPackage(LPCTSTR szPath, HWND hWnd, HINSTANCE hMSI, INSTALLUILEVEL dwUILevel, LPCTSTR szFeatures);

		
	/* Installation State
	 */
	protected:
		CMapWordToOb	m_mAddOn;
		DWORD			m_dwToDo;
		WORD			m_wMaxKey;

	public:
		void  ActivateList(CPropBase *pProp);
		DWORD GetDefaultToDo();
		void  SetTodo(DWORD dwToDo);

	public:
		BOOL IsOKMS();
		BOOL IsOKKern();

		BOOL IsNotInstKern();
		BOOL IsNotInstAddOn();

		BOOL IsInstSomething();
		BOOL IsInstAnything();
		BOOL IsInst4C(LPCTSTR szKey);

	protected:
		BOOL DeleteWordMap(CMapWordToOb *pMap);
		BOOL DeleteStringMap(CMapStringToOb *pMap);

	protected:
		BOOL ToBeInstalled(WORD wKey, LPCTSTR szKey);


	/* List Control
	 */
	protected:
		CListCtrl	*m_pListCtrl;
		CImageList	m_ImageList;

		int			m_iLastRegular;
		BOOL		m_bDisable;

	public:
		void SetListCtrl(CListCtrl *pListCtrl);
		CListCtrl *GetListCtrl();

		BOOL UpdateListbox(BOOL bRefill);

	protected:
		void DestroyLBContents();
		void AddLBItem(INT iIndex, UINT uItem, LPCTSTR szVersion, WORD wKey, LPCTSTR szKey, UINT uImage);
		void AddLBItem(INT iIndex, LPCTSTR szItem, LPCTSTR szVersion, WORD wKey, LPCTSTR szKey, UINT uImage);
		void UpdateLBItem(INT iIndex, WORD wKey, LPCTSTR szVersion, UINT uState);
		void DeleteLBItem(INT iIndex);
		void SelectLBItem(WORD wKey);
	
	public:
		void SelectNextLBItem(BOOL bNext, CPropBase *pProp);

	protected:
		void CreateImageList();
		void CreateLBColumns();

	public:
		void DisableAllControls(CPropBase *pProp);
		void EnableAllControls(CPropBase *pProp);
		void EnableControls(CPropBase *pProp);
		
		BOOL GetInstallAllState(CPropBase *pProp);
		BOOL GetUnInstallAllState(CPropBase *pProp);

	public:
		void SetWizardButtons(CPropBase *pProp);
		

	/* Message Handler
	 */
	public:
		void OnItemChangedListCtrl(CPropBase *pProp, NM_LISTVIEW* pNMListView);

		void OnInstall(CPropBase *pProp);
		void OnInstallAll(CPropBase *pProp);
		void OnUninstall(CPropBase *pProp);
		void OnUninstallAll(CPropBase *pProp);

	protected:
		void OnBeginInstall();
		void OnFinishInstall();
		void OnBeginUnInstall();
		void OnFinishUnInstall();
};

#endif	// _INSTALL_H_

/* ---------------------------------------------------------------------------- */
