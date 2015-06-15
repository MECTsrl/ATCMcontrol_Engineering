#include "stdafx.h"

#include "Configuration.h"
#include "ImportDirective.h"
//------------------------------------------------------------------*
/**
 * Function name    Configuration::Configuration
 * Description      constructor for class Configuration
 * @param           const string& name
 * @param           const StringPosRange nameRange
 * @param           const StringPosRange lineNo
 * @param           const StringPosRange nextVarSectInsertPos
 * @return          -
 * @exception       -
 * @see             -
*/
Configuration::Configuration(const string& name,
                             const StringPosRange& nameRange,
                             StringPos lineNo,
                             StringPos nextVarSectInsertPos)
    : POU(name, nameRange, lineNo, nextVarSectInsertPos) , m_strProjectName()
{
    m_nextImpDirInsertPos = 0;
    m_nextTaskConfInsertPos = 0;
    m_nextProgConfInsertPos = 0;
}

//------------------------------------------------------------------*
/**
 * Function name    Configuration::*Configuration
 * Description      destructor for class Configuration
 * @return          -
 * @exception       -
 * @see             -
*/
Configuration::~Configuration()
{    
}

//------------------------------------------------------------------*
/**
 * Function name    Configuration::getPOUType
 * Description      Return the type of the pou
 * @return          string
 * @exception       -
 * @see             -
*/
string Configuration::getPOUType() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    
    return _CONFIGURATION;
}

//------------------------------------------------------------------*
/**
 * Function name    Configuration::invite
 * Description      calls visit of the visitor for itself
 * @param           const IECSRCVisitor& visitor
 * @return          -
 * @exception       HRESULT exception.
 * @see             -
*/
void Configuration::invite(const IECSRCVisitor& visitor) const
{
    //create an implicit import node of type LC - lateral communication
    string str = m_strProjectName + _T(".4clc");
    string strFullFilePath = m_strProjectPath + _T("\\") + str;

    CFileStatus fileStatus;
    if (CFile::GetStatus(strFullFilePath.c_str(), fileStatus))
    {
        ImportDirective* pImport = new ImportDirective(str,0,_T("LC"));
        if(!pImport) {
            throw E_OUTOFMEMORY;
        }
    
        Configuration* pThis = const_cast<Configuration*>(this);
        pThis->addImportDirective(pImport);
    }
    POU::invite(visitor);
    visitor.visit(*this);
}

//------------------------------------------------------------------*
/**
 * Function name    Configuration::setTarget
 * Description      sets the target and target rang memember
 * @param           const string& target 
 * @param           StringPosRange targetRange
 * @return          -
 * @exception       -
 * @see             -
*/
void Configuration::setTarget(const string& target, const StringPosRange& targetRange)
{
    m_target = target;
    m_target_range = targetRange;
}

//------------------------------------------------------------------*
/**
 * Function name    Configuration::getTarget
 * Description      Gets the target
 * @return          const string&
 * @exception       -
 * @see             -
*/
const string& Configuration::getTarget() const
{
    return m_target;
}

//------------------------------------------------------------------*
/**
 * Function name    Configuration::getTargetRange
 * Description      Gets the targetRange
 * @return          const StringPosRange&
 * @exception       -
 * @see             -
*/
const StringPosRange& Configuration::getTargetRange() const
{
    return m_target_range;
}


void Configuration::setProjectInfo(BackparserProjectInfo & info)
{
    m_strProjectName = info.m_strProjectName;
    m_strProjectPath = info.m_strProjectPath;
}