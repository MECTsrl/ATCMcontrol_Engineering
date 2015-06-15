
#include"stdafx.h"

#include "TaskConfiguration.h"

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::TaskConfiguration
 * Description      constructor for class TaskConfiguration
 * @param           const string& Name
 * @param           StringPosRange nameRange
 * @return          -
 * @exception       -
 * @see             -
*/
TaskConfiguration::TaskConfiguration(const string& Name, StringPosRange nameRange)
    : m_name(Name), m_nameRange(nameRange)
{
    m_sys_cmnt = NULL;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::~TaskConfiguration
 * Description      destructor for class TaskConfiguration
 * @return          -
 * @exception       -
 * @see             -
*/
TaskConfiguration::~TaskConfiguration()
{
    if (m_sys_cmnt != NULL)
    {
        delete m_sys_cmnt;
    }
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setStart
 * Description      sets the start position of the task configuration
 * @param           StringPos start
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setStart(StringPos start)
{
    m_range.setFirst(start);
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setEnd
 * Description      sets the end position of the task configuration
 * @param           StringPos end
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setEnd(StringPos end)
{
    m_range.setLast(end);
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getRange
 * Description      returns the range of the task configuration
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange TaskConfiguration::getRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_range;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::invite
 * Description      calls visit of the visitor for itself
 * @param           const IECSRCVisitor& visitor
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::invite(const IECSRCVisitor& visitor) const
{
    visitor.visit(*this);
}

//------------------------------------------------------------------*
/**
 * Function name    FunctionBlock::getName
 * Description      returns the name of the task 
 * @return          string
 * @exception       -
 * @see             -
*/
string TaskConfiguration::getName() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_name;
}

//------------------------------------------------------------------*
/**
 * Function name    FunctionBlock::getNameRange
 * Description      returns the ragen of the name of the task 
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange TaskConfiguration::getNameRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_nameRange;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::SetSysComment
 * Description      sets the system comment of the task configuration
 * @param           SysComment *comment
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::SetSysComment(SysComment *comment)
{
    m_sys_cmnt = comment;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::GetFileSystemComment
 * Description      returns the system comment of the task configuration
 * @return          const SysComment*
 * @exception       -
 * @see             -
*/
const SysComment* TaskConfiguration::GetFileSystemComment() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_sys_cmnt;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setInterval
 * Description      sets the interval of the task configuration
 * @param           const string& value
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setInterval(const string& value)
{
    m_interval = value;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getInterval
 * Description      returns the interval of the task configuration
 * @return          string
 * @exception       -
 * @see             -
*/
string TaskConfiguration::getInterval() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_interval;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setIntervalRange
 * Description      sets the range of the interval of the task configuration
 * @param           StringPosRange spr
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setIntervalRange(StringPosRange spr)
{
    m_intervalRange = spr;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getIntervalRange
 * Description      returns the range of the interval of the task configuration
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange TaskConfiguration::getIntervalRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_intervalRange;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setPriority
 * Description      sets the priority of the task configuration
 * @param           const string& value
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setPriority(const string& value)
{
    m_priority = value;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getPriority
 * Description      returns the priority of the task configuration
 * @return          string
 * @exception       -
 * @see             -
*/
string TaskConfiguration::getPriority() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_priority;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setPriorityRange
 * Description      sets the range of the priority of the task configuration
 * @param           StringPosRange spr
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setPriorityRange(StringPosRange spr)
{
    m_priorityRange = spr;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getPriorityRange
 * Description      reeturns the range of the priority of the task configuration
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange TaskConfiguration::getPriorityRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_priorityRange;
}


//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setAutostart
 * Description      sets the autostart flag of the task configuration
 * @param           const string& value
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setAutostart(const string& value)
{
    m_autostart = value;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getAutostart
 * Description      returns the autostart flag of the task configuration
 * @return          string
 * @exception       -
 * @see             -
*/
string TaskConfiguration::getAutostart() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_autostart;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setAutostartRange
 * Description      sets the range of the autostart flag of the task configuration
 * @param           StringPosRange spr
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setAutostartRange(StringPosRange spr)
{
    m_autostartRange = spr;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getAutostartRange
 * Description      returns the range of the autostart flag of the task configuration
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange TaskConfiguration::getAutostartRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_autostartRange;
}


// NFTASKHANDLING 19.05.05 SIS >>
//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setLoaded
 * Description      sets the autostart flag of the task configuration
 * @param           const string& value
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setLoaded(const string& value)
{
    m_loaded = value;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getLoaded
 * Description      returns the loaded flag of the task configuration
 * @return          string
 * @exception       -
 * @see             -
*/
string TaskConfiguration::getLoaded() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_loaded;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setLoadedRange
 * Description      sets the range of the loaded flag of the task configuration
 * @param           StringPosRange spr
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setLoadedRange(StringPosRange spr)
{
    m_loadedRange = spr;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getLoadedRange
 * Description      returns the range of the loaded flag of the task configuration
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange TaskConfiguration::getLoadedRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_loadedRange;
}
// NFTASKHANDLING 19.05.05 SIS <<

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setIosync
 * Description      sets the iosync value of the task configuration
 * @param           const string& value
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setIosync(const string& value)
{
    m_iosync = value;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getIosync
 * Description      returns the iosync value of the task configuration
 * @return          string
 * @exception       -
 * @see             -
*/
string TaskConfiguration::getIosync() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_iosync;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setIosyncRange
 * Description      sets the range of the iosync value of the task configuration
 * @param           StringPosRange spr
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setIosyncRange(StringPosRange spr)
{
    m_iosyncRange = spr;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getIosyncRange
 * Description      returns the range of the iosync value of the task configuration
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange TaskConfiguration::getIosyncRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_iosyncRange;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setFftask
 * Description      sets the Fftask value of the task configuration
 * @param           const string& value
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setFftask(const string& value)
{
    m_fftask = value;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getFftask
 * Description      returns the Fftask value of the task configuration
 * @return          string
 * @exception       -
 * @see             -
*/
string TaskConfiguration::getFftask() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_fftask;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setFftaskRange
 * Description      sets the range of the Fftask value of the task configuration
 * @param           const string& value
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setFftaskRange(StringPosRange spr)
{
    m_fftaskRange = spr;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getFftaskRange
 * Description      returns the range of the Fftask value of the task configuration
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange TaskConfiguration::getFftaskRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_fftaskRange;
}


// SIS: event driven tasks >>
//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setEvent
 * Description      sets the Event value of the task configuration
 * @param           const string& value
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setEvent(const string& value)
{
    m_event = value;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getEvent
 * Description      returns the Event value of the task configuration
 * @return          string
 * @exception       -
 * @see             -
*/
string TaskConfiguration::getEvent() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_event;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setEventRange
 * Description      sets the range of the Event value of the task configuration
 * @param           const string& value
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setEventRange(StringPosRange spr)
{
    m_eventRange = spr;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getEventRange
 * Description      returns the range of the Event value of the task configuration
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange TaskConfiguration::getEventRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_eventRange;
}
// SIS: event driven tasks <<


//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setWatchdog
 * Description      sets the Watchdog value of the task configuration
 * @param           const string& value
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setWatchdog(const string& value)
{
    m_watchdog = value;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getWatchdog
 * Description      returns the Watchdog value of the task configuration
 * @return          string
 * @exception       -
 * @see             -
*/
string TaskConfiguration::getWatchdog() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_watchdog;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setWatchdogRange
 * Description      sets the range of the Watchdog value of the task configuration
 * @param           const string& value
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setWatchdogRange(StringPosRange spr)
{
    m_watchdogRange = spr;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getWatchdogRange
 * Description      returns the range of the Watchdog value of the task configuration
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange TaskConfiguration::getWatchdogRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_watchdogRange;
}



//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::setInitialisationListRange
 * Description      sets the range of initialisation list of the task configuration
 * @param           const string& value
 * @return          -
 * @exception       -
 * @see             -
*/
void TaskConfiguration::setInitialisationListRange(StringPosRange range)
{
    m_initialisationListRange = range;
}

//------------------------------------------------------------------*
/**
 * Function name    TaskConfiguration::getInitialisationListRange
 * Description      returns the range of initialisation list of the task configuration
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange TaskConfiguration::getInitialisationListRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_initialisationListRange;
}


void TaskConfiguration::setTaskAttribute(const string& name, const string& value)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    
    ASSERT(m_arrAttributeNames.GetSize() == m_arrAttributeValues.GetSize());

    m_arrAttributeNames.Add(name);
    m_arrAttributeValues.Add(value);
}


int TaskConfiguration::getNumTaskAttributes() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    ASSERT(m_arrAttributeNames.GetSize() == m_arrAttributeValues.GetSize());

    return m_arrAttributeNames.GetSize();
}


void TaskConfiguration::getTaskAttibute(int num, string& strName, string& strValue) const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    if (num<getNumTaskAttributes())
    {
        strName  = m_arrAttributeNames.GetAt(num);
        strValue = m_arrAttributeValues.GetAt(num);
    }
}



