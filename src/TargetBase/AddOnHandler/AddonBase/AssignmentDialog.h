#ifndef _ASSIGNMENTDIALOG_H_
#define _ASSIGNMENTDIALOG_H_

#include "AddonBaseResource.h"
#include <atlhost.h>

class CBaseTargetData;

//------------------------------------------------------------------*
/**
 * CAssignmentDialog shows a dialog to assign a resource to the pc target.
 *
 * @see             
*/
class CAssignmentDialog : 
	public CAxDialogImpl<CAssignmentDialog>
{
public:
	CAssignmentDialog(CStringArray& rastrResources, const CString& crstrResource);
	~CAssignmentDialog();

	enum { IDD = IDD_ASSIGNMENTDIALOG };

BEGIN_MSG_MAP(CAssignmentDialog)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(ID_OK, OnOK)
	COMMAND_ID_HANDLER(ID_CANCEL, OnCancel)
    COMMAND_ID_HANDLER(2, OnCancel)  // cancel handler -> ESC key
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
//    MESSAGE_HANDLER(WM_HELP, OnShowHelp)
	COMMAND_HANDLER(IDC_LIST_RESOURCES, LBN_DBLCLK, OnDblclkListResources)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDblclkListResources(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT OnShowHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    void DoOk();

public:
    CString         m_strResource;      // currently selected resource

protected:
    CStringArray    m_astrResources;    // all available resources

};

#endif

/* ---------------------------------------------------------------------------- */
