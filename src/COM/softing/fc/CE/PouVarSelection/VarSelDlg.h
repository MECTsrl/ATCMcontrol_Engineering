
#if !defined(AFX_VARSELDLG_H__5C11FF93_7174_4B80_8AB6_F97488A87154__INCLUDED_)
#define AFX_VARSELDLG_H__5C11FF93_7174_4B80_8AB6_F97488A87154__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CEVarDecl.h"

class CPouVarSelCtrl;
class CPouVarSelData;
class CVarObject;


class CVarSelDlg  :
    public CDialogImpl<CVarSelDlg>
{
public:
	CVarSelDlg();
	~CVarSelDlg();

    enum { IDD = IDD_DLG_VARSEL };

    BEGIN_MSG_MAP(CVarSelDlg)
        MESSAGE_HANDLER(WM_SIZE, OnSize)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_HANDLER(IDC_SCOPE_FILTER_LIST, CBN_SELCHANGE, OnSelchangeScopeFilterList) //m_ScopeFilterList
        COMMAND_HANDLER(IDC_VAR_VAR_EDIT, EN_CHANGE, OnChangeVarVarEdit)
        NOTIFY_HANDLER(IDC_VAR_VAR_LISTCTRL, LVN_ITEMCHANGED, OnItemchangedVarVarListCtrl)
        COMMAND_HANDLER(IDC_VAR_DELETE_BTN, BN_CLICKED, OnClickedVarDeleteBtn)
        COMMAND_HANDLER(IDC_VAR_EXTERNAL_BTN, BN_CLICKED, OnClickedVarExternalBtn)
        COMMAND_HANDLER(IDC_VAR_APPLY_BTN, BN_CLICKED, OnClickedVarApplyBtn)
        COMMAND_HANDLER(IDC_VAR_DEFINEORUPDATE_BTN, BN_CLICKED, OnClickedVarDefineOrUpdateBtn)
        NOTIFY_HANDLER(IDC_VAR_VAR_LISTCTRL, NM_DBLCLK, OnDblclkVarVarListCtrl)
        COMMAND_HANDLER(IDC_VAR_DATA_TYPES_LIST, LBN_SELCHANGE, OnSelchangeVarDataTypesList)
        COMMAND_HANDLER(IDC_VAR_VN_CLASS_CB, CBN_EDITUPDATE, OnEditupdateVarVNClassCB)
        COMMAND_HANDLER(IDC_VAR_VN_CLASS_CB, CBN_SELCHANGE, OnSelchangeVarVNClassCB)
        COMMAND_HANDLER(IDC_VAR_VN_NAME_EDIT, EN_CHANGE, OnChangeVarVNNameEdit)
        COMMAND_HANDLER(IDC_VAR_VN_ADDR_EDIT, EN_CHANGE, OnChangeVarVNAddrEdit)
        COMMAND_HANDLER(IDC_VAR_VN_COMMENT_EDIT, EN_CHANGE, OnChangeVarVNCommentEdit)
        COMMAND_HANDLER(IDC_VAR_VN_INITVAL_EDIT, EN_CHANGE, OnChangeVarVNInitvalEdit)
        COMMAND_HANDLER(IDC_VAR_VN_TYPE_CB, CBN_EDITCHANGE, OnEditchangeVarVNTypeCB)
        COMMAND_HANDLER(IDC_VAR_VN_TYPE_CB, CBN_SELCHANGE, OnSelchangeVarVNTypeCB)
        COMMAND_HANDLER(IDC_SOURCEFILE_STATIC, BN_CLICKED, OnClickedSourcefile_static)
                                               //BN_DOUBLECLICKED  geht nicht für statis text?
        MESSAGE_HANDLER(WM_HELP, OnShowHelp)
    END_MSG_MAP()


    void InitDlg(CPouVarSelCtrl* pPouVarSelCtrl);
    void FinishInit();
    void FillScopeFilterList();
    void FillTypesList();
    void FillDataTypeFilter();
    void FillDataTypeSelection();
    void FillVarClassSelection();


    void ChangeVarSelectionHint(const CString& strPouName, 
                                const CString& strVarNamePrefix, 
                                const CString& strDataType);
    void InvalidateDlgData();

    void GetVarList(CList<CString, CString&>& listVars, const CString& strTypeHint);

    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSelchangeScopeFilterList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedVarFilterHeaderCheck(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnSelchangeVarDataTypesList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeVarVarEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnItemchangedVarVarListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnClickedVarDeleteBtn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedVarExternalBtn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedVarApplyBtn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedVarDefineOrUpdateBtn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
 	LRESULT OnDblclkVarVarListCtrl(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnEditupdateVarVNClassCB(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
   	LRESULT OnSelchangeVarVNClassCB(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeVarVNNameEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnChangeVarVNAddrEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeVarVNCommentEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeVarVNInitvalEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditchangeVarVNTypeCB(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelchangeVarVNTypeCB(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnClickedSourcefile_static(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnShowHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    ICEVarDecl* GetVarDeclHandler(const CString& strFileName);

    BOOL CheckInitValue(const CString& strValue, const CString& strDataType);
    BOOL CheckIECIdentifier(const CString& strName);
    void UpdateIECText(const TCHAR* pszDataType, const TCHAR* pszVarClass);
    void UpdateSourceFileInfo(const TCHAR* pszClass);

    void UpdateInfoFieldsFromVarObj(CVarObject* pVarObj);
private:
    CPouVarSelCtrl* m_pPouVarSelCtrl;
    CPouVarSelData* m_pPouVarSelData;

    CList<CString, CString&> m_listVarTypes;
    CList<CString, CString&> m_listVarSelectedTypes;


    CString                  m_strVarSelectedVar;
    CMap<CString, LPCTSTR, CVarObject*, CVarObject*> m_mapVarVars;//contains same as var list box,
                                                                  //but WITHOUT the type filter, 
                                                                  //therefore here may be more items
                                                                  //than in the var list box if a type 
                                                                  //filter is active.

    BOOL m_bChangeEditLock;
    BOOL m_bDoNotChangeInfo;//##EF TODO what is this good for ?


    CString     m_CurrentScopeFilter; //currently selected filter <All>, <Header>, <Global> or gvl file name,
                                      //initially is <All>
    CString     m_strSelectedVarClass;//used to prevent that the user edits the class field.

    // GUI objects
    CStatic*    m_pStaticFilter;
    CListBox    m_ScopeFilterList;
    CStatic*    m_pStaticTypes;
    CListBox*   m_pLBTypes;
    
    CStatic*    m_pStaticVar;
    CEdit*      m_pEditVar;   //edit box above var list control
    CListCtrl*  m_pLBCtrlVar; //the var list control 

    CButton     m_btnApply;
    CButton     m_btnDelete;
    CButton     m_btnDefineOrUpdate;
    CButton     m_btnExternal;
    
    //Class:
    CStatic     m_staticVNClass;
    CComboBox   m_CBVNClass;

    //Identifier:
    CStatic     m_staticVNName;
    CEdit       m_editVNName;
    
    //Address:
    CStatic     m_staticVNAddr;
    CEdit       m_editVNAddr;

    //Type:
    CStatic     m_staticVNType;
    CComboBox   m_CBVNType;
    
    //Initial:
    CStatic     m_staticVNInitVal;
    CEdit       m_editVNInitVal;
    
    //Comment:
    CStatic     m_staticVNComment;
    CEdit       m_editVNComment;

    CStatic     m_staticSourceFile;
    CEdit       m_editIECText;

    NONCLIENTMETRICS m_sysMetrics;
	

    CString m_strPVSSelAll;
    CString m_strPVSSelHeader;
    CString m_strPVSSelGlobal;
    CString m_strPVSSelComplex;
    CString m_strPVSSelSimple;
    CString m_strPVSDefine;
    CString m_strPVSUpdate;
    CString m_strPVSSource;
    CString m_strPVSInsert;

};

#endif // !defined(AFX_VARSELDLG_H__5C11FF93_7174_4B80_8AB6_F97488A87154__INCLUDED_)