#ifndef NEW_PROJECT_DLG_H
#define NEW_PROJECT_DLG_H

#include "WizardList.h"

class CNewProjectDlg : public CDialog
// this is the dialog displaying a list of project wizards
// from which the user can choose
{
    private:
        CString m_sProjectName;
        CString m_sProjectPath;
    	CListCtrl m_wizardListCtrl;
        CImageList m_imageList;
        CWizardList m_wizardList;
        int m_iSelectedWizardItem;
	    BOOL m_bWizardSelected;
        BOOL IsProjectNameValid();
        void ExecuteWizard(CLSID classId);
        BOOL DirectoryExists();
        void UpdateOkButtonState();
        void EnsureDirectoryExists();
		virtual void DoDataExchange(CDataExchange* pDX);
        void OnBrowseDirectory();
	    void OnChangedProjectName();
	    void OnChangedWizardList(NMHDR* pNMHDR, LRESULT* pResult);
    	virtual BOOL OnInitDialog();
    	void OnOk();
    	enum { IDD = IDD_NEW_PROJECT };
    	DECLARE_MESSAGE_MAP()
    public:
    	CNewProjectDlg(CWnd* pParent = NULL);
        CString m_strFileName;
        HRESULT m_hrDialogState;
};

#endif
