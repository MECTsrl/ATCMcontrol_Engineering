#if !defined(AFX_CHILDDIALOG_H__619331B3_7DE7_4DB1_A039_2103E87E8E71__INCLUDED_)
#define AFX_CHILDDIALOG_H__619331B3_7DE7_4DB1_A039_2103E87E8E71__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CChildDialog declaration

class CChildDialog : public CDialog
{
public:
	CChildDialog ();

protected:
	virtual BOOL OnCommand (WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify (WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnOK ();
	virtual void OnCancel ();
	virtual BOOL OnCmdMsg (UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
};

#endif // !defined(AFX_CHILDDIALOG_H__619331B3_7DE7_4DB1_A039_2103E87E8E71__INCLUDED_)
