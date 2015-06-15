#include"stdafx.h"

#include "ProgramConfiguration.h"

//------------------------------------------------------------------*
/**
 * Function name    ProgramConfiguration::ProgramConfiguration
 * Description      constructor for class ProgramConfiguration
 * @param           const string& name
 * @param           const StringPosRange nameRange
 * @return          -
 * @exception       -
 * @see             -
*/
ProgramConfiguration::ProgramConfiguration(const string& name, StringPosRange nameRange) :
    m_name(name), 
    m_nameRange(nameRange),
    m_taskName(_T("")),     
    m_taskNameRange(-1, -2),
    m_programClass(_T("")),     
    m_programClassRange(-1, -2)
{
}

//------------------------------------------------------------------*
/**
 * Function name    ProgramConfiguration::~ProgramConfiguration
 * Description      denstructor for class ProgramConfiguration
 * @return          -
 * @exception       -
 * @see             -
*/
ProgramConfiguration::~ProgramConfiguration()
{
    if (m_sys_cmnt != NULL)
    {
        delete m_sys_cmnt;
    }
}

//------------------------------------------------------------------*
/**
 * Function name    ProgramConfiguration::setStart
 * Description      sets the start position of the program configuration
 * @param           StringPos start
 * @return          -
 * @exception       -
 * @see             -
*/
void ProgramConfiguration::setStart(StringPos start)
{
    m_range.setFirst(start);
}

//------------------------------------------------------------------*
/**
 * Function name    ProgramConfiguration::setEnd
 * Description      sets the end position of the program configuration
 * @param           StringPos end
 * @return          -
 * @exception       -
 * @see             -
*/
void ProgramConfiguration::setEnd(StringPos end)
{
    m_range.setLast(end);
}

//------------------------------------------------------------------*
/**
 * Function name    ProgramConfiguration::getRange
 * Description      returns the range of the program configuration
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange ProgramConfiguration::getRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_range;
}


//------------------------------------------------------------------*
/**
 * Function name    ProgramConfiguration::setEndPos
 * Description      sets the end position of the name
 * @param           const StringPos end
 * @return          -
 * @exception       -
 * @see             -
*/
void ProgramConfiguration::setEndPos(StringPos end) 
{ 
    m_nameRange.setLast(end); 
}

//------------------------------------------------------------------*
/**
 * Function name    ProgramConfiguration::invite
 * Description      calls visit of the visitor for itself
 * @param           const IECSRCVisitor& visitor
 * @return          -
 * @exception       -
 * @see             -
*/
void ProgramConfiguration::invite(const IECSRCVisitor& visitor,
                                  int withTask) const
{
    visitor.visit(*this, withTask);
}

//------------------------------------------------------------------*
/**
 * Function name    ProgramConfiguration::getName
 * Description      returns the name
 * @return          string
 * @exception       -
 * @see             -
*/
string ProgramConfiguration::getName() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_name;
}

//------------------------------------------------------------------*
/**
 * Function name    ProgramConfiguration::getNameRange
 * Description      returns the range of the name
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange ProgramConfiguration::getNameRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_nameRange;
}

//------------------------------------------------------------------*
/**
 * Function name    ProgramConfiguration::getTaskName
 * Description      returns the task name
 * @return          string
 * @exception       -
 * @see             -
*/
string ProgramConfiguration::getTaskName() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_taskName;
}

//------------------------------------------------------------------*
/**
 * Function name    ProgramConfiguration::getTaskNameRange
 * Description      returns the range of the task name
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange ProgramConfiguration::getTaskNameRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_taskNameRange;
}

//------------------------------------------------------------------*
/**
 * Function name    ProgramConfiguration::getProgramClass
 * Description      returns the ProgranClass name
 * @return          string
 * @exception       -
 * @see             -
*/
string ProgramConfiguration::getProgramClass() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_programClass;
}

//------------------------------------------------------------------*
/**
 * Function name    ProgramConfiguration::getProgramClassRange
 * Description      returns the range of the ProgramClass
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
StringPosRange ProgramConfiguration::getProgramClassRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_programClassRange;
}

//------------------------------------------------------------------*
/**
 * Function name    ProgramConfiguration::setTask
 * Description      sets the task and the task range
 * @param           const string& taskName
 * @param           StringPosRange taskNameRange
 * @return          -
 * @exception       -
 * @see             -
*/
void ProgramConfiguration::setTask(const string& taskName, StringPosRange taskNameRange)
{
    m_taskName = taskName;
    m_taskNameRange = taskNameRange;
}

//------------------------------------------------------------------*
/**
 * Function name    ProgramConfiguration::setProgramClass
 * Description      sets the program class and the program class range
 * @param           const string& programClass
 * @param           StringPosRange programClassRange
 * @return          -
 * @exception       -
 * @see             -
*/
void ProgramConfiguration::setProgramClass(const string& programClass, StringPosRange programClassRange)
{
    m_programClass = programClass;
    m_programClassRange = programClassRange;
}

//------------------------------------------------------------------*
/**
 * Function name    ProgramConfiguration::SetSysComment
 * Description      sets the system comment
 * @param           SysComment *comment
 * @return          -
 * @exception       -
 * @see             -
*/
void ProgramConfiguration::SetSysComment(SysComment *comment)
{
    m_sys_cmnt = comment;
}

//------------------------------------------------------------------*
/**
 * Function name    ProgramConfiguration::GetFileSystemComment
 * Description      returns the system comment
 * @return          const SysComment*
 * @exception       -
 * @see             -
*/
const SysComment* ProgramConfiguration::GetFileSystemComment() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_sys_cmnt;
}
