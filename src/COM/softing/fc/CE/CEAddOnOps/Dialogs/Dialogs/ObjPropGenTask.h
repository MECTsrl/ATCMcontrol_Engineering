
#if !defined __OBJPROPGENTASK__
#define __OBJPROPGENTASK__

// SIS: event driven tasks >>
#define MIN_EVENT_NUM   1
#define MAX_EVENT_NUM   64

typedef enum ETaskType
{
    eTaskTypeCyclic         = 0,
    eTaskTypeEventDriven    = 1,
    eTaskTypeIoSync         = 2,
};
// SIS: event driven tasks <<

#include "ObjPropGenBase.h"

class CObjPropGenTask : public CObjPropGenBase
{
public:
	CObjPropGenTask(DIALOGTYPE DialogType,
                    CPropertySheetBase *PropertySheet);

	~CObjPropGenTask();

    BOOL	m_AutoStart;
	BOOL	m_bLoaded;	// NFTASKHANDLING 19.05.05 SIS
	CString	m_Interval;
	CString	m_IoSync;
	CString	m_Priority;
    // SIS: event driven tasks >>
    CString     m_EventString;      
    ETaskType   m_eType;
    // SIS: event driven tasks <<
    CString m_Watchdog;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual BOOL OnKillActive();

    // SIS: event driven tasks >>
    BOOL CheckEventString();
    void EnableControls();
    void InitializeMissingData();
    // SIS: event driven tasks <<

    //{{AFX_MSG(CObjPropGenTask)
    //}}AFX_MSG
    // SIS: event driven tasks >>
    afx_msg void OnTypeChanged();
    afx_msg void OnEventsSelect();
    // SIS: event driven tasks <<

    DECLARE_MESSAGE_MAP()
};

#endif

