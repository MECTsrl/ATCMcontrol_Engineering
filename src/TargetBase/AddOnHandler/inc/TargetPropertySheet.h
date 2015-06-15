#ifndef _TARGETPROPERTYSHEET_H_
#define _TARGETPROPERTYSHEET_H_

//----  Aggregate Includes:   --------------------------------------*
#include "PropPageInfo.h"
#include "PropPageTargetGeneral.h"

//----  Forward Class Definitions:   -------------------------------*
class CBaseTargetData;
class CBasePropertyPage;
class CBaseCommPropertyPage;
class CPropPageCustomDownload;
class CPropPageSocketComm;
class CPropPageSerialComm;


//------------------------------------------------------------------*
/**
 * CTargetPropertySheet
 *
 *  - target properties dialog / target new dialog
 *  - contains:
 *    - general page
 *    - communication pages
 *    - info page
 *  - for each communication channel in firmware data a communication 
 *    page is created (if possible)
 *  - init: new: target data is set in InitNewTarget()
 *          properties: target data of all pages is set in ReadTargetData()
 *  - exit: update is made in UpdatePageData(), validation in Validate()
 *          saving of data in WriteTargetData()
 *  - all pages must be derived from CBasePropertyPage to ensure dialog
 *    consistency
 *  - communication pages must be derived from CBaseCommPropertyPage
 *
 *  caution: to be able to quit the dialog after call of 
 *           EnsureTargetDisconnected() all saving must be done
 *           here and not in the property pages.
 *           Do not forget to call AddPages() and InitPages() before
 *           calling DoModal()
 *
 * @see             
*/
class CTargetPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CTargetPropertySheet)

public:
    // construction
	CTargetPropertySheet(CBaseTargetData* pBaseTargetData, BOOL bNewDlg, UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CTargetPropertySheet(CBaseTargetData* pBaseTargetData, BOOL bNewDlg, LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CTargetPropertySheet();

    virtual void AddPages();    
    virtual void InitPages();   

// Operations
public:
    // is called by general page when firmware version is changed
    void CheckCommChannels(UINT uiFirmwareVersionIndex);

protected:
    // Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTargetPropertySheet)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CTargetPropertySheet)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
    afx_msg BOOL OnOK(UINT nID);
	DECLARE_MESSAGE_MAP()

protected:
    // adding pages
    virtual void AddCommunicationPages();
    virtual BOOL AddCommPageForChannel(const CString& crstrCommChannel);
    virtual void AddCommPropertyPage(CBaseCommPropertyPage* pPage);

    // initializing pages
    virtual void InitPagesStatic();
    virtual void InitNewTarget();

    // read/write target data
    virtual void ReadTargetData();
    virtual void WriteTargetData();

    // dialog functions
    virtual void UpdatePageData();
    virtual BOOL Validate();

    // helper functions
    CBaseCommPropertyPage* GetPageForCommChannel(const CString& crstrCommChannel);

// Attributes
public:
    // property pages
    CBasePropertyPage*      m_pPageGeneral;             // general page
    CList<CBaseCommPropertyPage*, CBaseCommPropertyPage*> m_tPageList;  
                                                        // list of communication property pages
    CBasePropertyPage*      m_pPageInfo;                // info page
    CPropPageCustomDownload* m_pPageCustDownload;

protected:
    CBaseTargetData*        m_pTargetData;              // base target data
    BOOL                    m_bNewDlg;                  // new dialog?
    CString                 m_strDLDir;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ 


#endif