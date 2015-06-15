#ifndef _COMMPROPERTYSHEET_H_
#define _COMMPROPERTYSHEET_H_

class CPropPageSocketComm;
class CPropPageSerialComm;
class CBaseCommPropertyPage;

//------------------------------------------------------------------*
/**
 *  class CCommPropertySheet
 *
 *  - shows communication pages in project wizard dialog
 *  - to be used from "outside":
 *    (1) create object
 *    (2) add page using AddPageForChannel() and SetConnectParameter()
 *        each channel
 *    (3) call DoModal()
 *    (4) if OK: call GetConnectParameter() for each channel and 
 *        save it to your target.
 *
 *  @see    ....
 */
class CCommPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CCommPropertySheet)

// Construction
public:
	CCommPropertySheet();
	virtual ~CCommPropertySheet();

    virtual BOOL AddPageForChannel(const CString& crstrCommChannel);
    virtual void SetActiveCommChannel(const CString& crstrCommChannel);

    virtual void SetConnectParameter(const CString& crstrCommChannel, const CString& crstrConnectPar);
    virtual CString GetConnectParameter(const CString& crstrCommChannel);
    
    virtual CBaseCommPropertyPage* GetPageForChannel(const CString& crstrCommChannel);
    virtual void AddPropertyPage(CBaseCommPropertyPage* pPage);

protected:
    // Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommPropertySheet)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CCommPropertySheet)
	//}}AFX_MSG
    afx_msg BOOL OnOK(UINT nID);
	DECLARE_MESSAGE_MAP()

protected:
    CList<CBaseCommPropertyPage*, CBaseCommPropertyPage*> m_tPageList;  // list of property pages
};

#endif