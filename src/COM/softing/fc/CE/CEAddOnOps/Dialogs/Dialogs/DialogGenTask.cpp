
#include "stdafx.h"
#include "resource.h"
#include "DialogGenTask.h"
#include "ObjPropGenTask.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenTask::CDialogGenTask
 * Description      constructor for class CDialogGenTask
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenTask::CDialogGenTask(DIALOGTYPE DialogType, 
                               CPropertySheetBase *PropertySheet) :
    m_ObjPropGenTask(DialogType, PropertySheet),
    CDialogGenBase(m_ObjPropGenTask)
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenTask::~CDialogGenTask
 * Description      destructor for class CDialogGenTask
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenTask::~CDialogGenTask()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenTask::GetAutoStart
 * Description      returns the auto start flag
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CDialogGenTask::GetAutoStart()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	return m_ObjPropGenTask.m_AutoStart;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenTask::SetAutoStart
 * Description      sets the auto start flag
 * @param           BOOL AutoStart
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenTask::SetAutoStart(BOOL AutoStart)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	m_ObjPropGenTask.m_AutoStart = AutoStart;
}

// NFTASKHANDLING 19.05.05 SIS >>
//------------------------------------------------------------------*
/**
 * Function name    CDialogGenTask::GetLoaded
 * Description      returns the auto start flag
 * @return          BOOL
 * @exception       -
 * @see             -
*/
BOOL CDialogGenTask::GetLoaded()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	return m_ObjPropGenTask.m_bLoaded;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenTask::SetLoaded
 * Description      sets the auto start flag
 * @param           BOOL Loaded
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenTask::SetLoaded(BOOL bLoaded)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	m_ObjPropGenTask.m_bLoaded = bLoaded;
}
// NFTASKHANDLING 19.05.05 SIS <<


//------------------------------------------------------------------*
/**
 * Function name    CDialogGenTask::GetInterval
 * Description      returns the interval
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogGenTask::GetInterval()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	return m_ObjPropGenTask.m_Interval;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenTask::SetInterval
 * Description      sets the interval
 * @param           CString Interval
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenTask::SetInterval(CString Interval)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	m_ObjPropGenTask.m_Interval = Interval;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenTask::GetIoSync
 * Description      returns the io sync information
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogGenTask::GetIoSync()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	return m_ObjPropGenTask.m_IoSync;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenTask::SetIoSync
 * Description      sets the io sync information
 * @param           CString IoSync
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenTask::SetIoSync(CString IoSync)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	m_ObjPropGenTask.m_IoSync = IoSync;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenTask::GetPriority
 * Description      returns the priority
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogGenTask::GetPriority()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	return m_ObjPropGenTask.m_Priority;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenTask::SetPriority
 * Description      sets the priority
 * @param           CString Priority
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenTask::SetPriority(CString Priority)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	m_ObjPropGenTask.m_Priority = Priority;
}


// SIS: event driven tasks >>
//------------------------------------------------------------------*
/**
 * Function name    CDialogGenTask::GetEventString
 * Description      returns the event string
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogGenTask::GetEventString()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	return m_ObjPropGenTask.m_EventString;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenTask::SetEventString
 * Description      sets the event string
 * @param           CString
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenTask::SetEventString(CString EventString)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	m_ObjPropGenTask.m_EventString = EventString;
}
// SIS: event driven tasks <<

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenTask::SetWatchdog
 * Description      sets the watchdog string
 * @param           CString
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenTask::SetWatchdog(CString Watchdog)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	m_ObjPropGenTask.m_Watchdog = Watchdog;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenTask::GetWatchdog
 * Description      returns the watchdog string
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogGenTask::GetWatchdog()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	return m_ObjPropGenTask.m_Watchdog;
}


