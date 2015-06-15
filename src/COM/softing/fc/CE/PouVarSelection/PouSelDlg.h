

#if !defined(AFX_POUSELDLG_H__D7E37E1F_18B0_4E6A_85AE_B363C8A223C3__INCLUDED_)
#define AFX_POUSELDLG_H__D7E37E1F_18B0_4E6A_85AE_B363C8A223C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPouVarSelCtrl;
class CPouVarSelData;
class CPouVarObject;

class CPouSelDlg  :
    public CDialogImpl<CPouSelDlg>
{
public:
	CPouSelDlg();
	~CPouSelDlg();

    enum { IDD = IDD_DLG_POUSEL };
    enum EnumLastSel 
    {  
        LAST_SEL_NOTHING,
        LAST_SEL_POU,
        LAST_SEL_LRU 
    };

    BEGIN_MSG_MAP(CPouSelDlg)
    NOTIFY_HANDLER(IDC_POU_LIB_LIST, LVN_ITEMCHANGED, OnSelchangePouLibList)
    NOTIFY_HANDLER(IDC_POU_TYPES_LIST, LVN_ITEMCHANGED, OnSelchangePouTypesList)
    COMMAND_HANDLER(IDC_POU_APPLY_BTN, BN_CLICKED, OnClickedPouApplyBtn)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    COMMAND_HANDLER(IDC_POU_POU_EDIT, EN_CHANGE, OnChangePouPouEdit)
    NOTIFY_HANDLER(IDC_POU_POU_LISTCTRL, LVN_BEGINDRAG, OnBegindragPouPouListCtrl)
    NOTIFY_HANDLER(IDC_POU_POU_LISTCTRL, LVN_ITEMCHANGED, OnChangeItemPouPouListCtrl)
    NOTIFY_HANDLER(IDC_POU_POU_LISTCTRL, NM_DBLCLK, OnDblclkPouPouListCtrl)
    NOTIFY_HANDLER(IDC_POU_LRU_LISTCTRL, LVN_BEGINDRAG, OnBegindragPouLruListCtrl)
    NOTIFY_HANDLER(IDC_POU_LRU_LISTCTRL, LVN_ITEMCHANGED, OnChangeItemPouLruListCtrl)
    NOTIFY_HANDLER(IDC_POU_LRU_LISTCTRL, NM_DBLCLK, OnDblclkPouLruListCtrl)
    NOTIFY_HANDLER(IDC_POU_POU_LISTCTRL, NM_RCLICK, OnRclickPouPouListCtrl)
    COMMAND_HANDLER(IDM_SHOW_USER_HELP, BN_CLICKED, OnShowUserHelp)
    COMMAND_HANDLER(IDM_SHOW_HEADER, BN_CLICKED, OnShowHeader)
    MESSAGE_HANDLER(WM_HELP, OnShowHelp)
    END_MSG_MAP()

    void InitDlg(CPouVarSelCtrl* pPouVarSelCtrl);
    void InvalidateDlgData();
    void InvalidateSelectionData();
    void InvalidateLRU();

    CList<CString, CString&>* GetLruList();
    void GetPouList(CList<CString, CString&>& listPous);

private:
    CPouVarSelCtrl* m_pPouVarSelCtrl;
    CPouVarSelData* m_pPouVarSelData;

	LRESULT OnSelchangePouLibList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSelchangePouTypesList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnClickedPouApplyBtn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnChangePouPouEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

    LRESULT OnChangeItemPouPouListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
    LRESULT OnDblclkPouPouListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnBegindragPouPouListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT DoSelchangePouPouListCtrl();
   	LRESULT OnRclickPouPouListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);


    LRESULT OnChangeItemPouLruListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
    LRESULT OnDblclkPouLruListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnBegindragPouLruListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT DoSelchangePouLruListCtrl();


    BOOL StartDrag(CPouVarObject* pPouVarObj);

    BOOL AddToLRU(CString& strPouName);

    LRESULT OnShowUserHelp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnShowHeader(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

    LRESULT OnShowHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);



    // dialog elements
    CStatic*    m_pStaticLib;
    CListCtrl*   m_pLBLib;
    CStatic*    m_pStaticTypes;
    CListCtrl*   m_pLBTypes;
    CStatic*    m_pStaticLru;
    CListCtrl*  m_pLBCLru;
    CStatic*    m_pStaticPou;
    CEdit*      m_pEditPou;
    //CListBox*   m_pLBPou;
    CListCtrl*  m_pLBCPou;
    CStatic*    m_pStaticHeader;
    CEdit*      m_pEditHeader;
    CButton*    m_pBtnApply;

    enum EnumLastSel m_iLastSelected;

    NONCLIENTMETRICS m_sysMetrics;


    // dlg data:
    CList<CString, CString&> m_listPouLibraries;
    CList<CString, CString&> m_listPouSelectedLibraries;
    CList<CString, CString&> m_listPouTypes;
    CList<CString, CString&> m_listPouSelectedTypes;
    CMap<CString, LPCTSTR, CPouVarObject*, CPouVarObject*> m_mapPouPous;
    CString m_strPouSelectedPou;
    CMap<CString, LPCTSTR, CPouVarObject*, CPouVarObject*> m_mapPouLru;
    CList<CString, CString&> m_listPouLru;
    CString m_strPouSelectedLru;


    UINT m_uiClipFormatXML;

    CString m_strCMPouName;   // the name of the pou for which 
                              // the context menu has been opened

	BOOL m_bEditChanging;     // true if edit is changing, so do not update list sel item
                              // this will all be done in edit change handler

    CString m_strPVSSelAll;
    CString m_strPVSSelProject;
    CString m_strPVSSelHeader;
    CString m_strPVSSelGlobal;
    CString m_strPVSSelConsts;
    CString m_strPVSSelComplex;
    CString m_strPVSSelSimple;
    CString m_strPVSSelNone;
};

#endif // !defined(AFX_POUSELDLG_H__D7E37E1F_18B0_4E6A_85AE_B363C8A223C3__INCLUDED_)
