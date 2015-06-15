
#include "stdafx.h"

#include "StResource.h"

//------------------------------------------------------------------*
/**
 * Function name    Resource::Resource
 * Description      constructor for class Resource
 * @param           const string& name
 * @param           const StringPosRange nameRange
 * @param           const StringPos lineNo
 * @param           const StringPos nextVarSectInsertPos
 * @return          -
 * @exception       -
 * @see             -
*/
Resource::Resource(const string& name,
                   const StringPosRange& nameRange,
                   StringPos lineNo,
                   StringPos nextVarSectInsertPos)
    : POU(name, nameRange, lineNo, nextVarSectInsertPos)
{
    m_nextImpDirInsertPos = 0;
    m_nextTaskConfInsertPos = 0;
    m_nextProgConfInsertPos = 0;
}

//------------------------------------------------------------------*
/**
 * Function name    Resource::~Resource
 * Description      destructor for class Resource
 * @return          -
 * @exception       -
 * @see             -
*/
Resource::~Resource()
{    
}

//------------------------------------------------------------------*
/**
 * Function name    Resource::getPOUType
 * Description      returns the type of the pou
 * @return          string
 * @exception       -
 * @see             -
*/
string Resource::getPOUType() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    
    return _RESOURCE;
}

//------------------------------------------------------------------*
/**
 * Function name    Resource::invite
 * Description      calls visit of the visitor for itself
 * @param           const IECSRCVisitor& visitor
 * @return          -
 * @exception       -
 * @see             -
*/
void Resource::invite(const IECSRCVisitor& visitor) const
{
    POU::invite(visitor);
    visitor.visit(*this);
}

//------------------------------------------------------------------*
/**
 * Function name    Resource::setTarget
 * Description      sets the target and the range of the target for the resource
 * @param           const string& target
 * @param           const StringPosRange& targetRange
 * @return          -
 * @exception       -
 * @see             -
*/
void Resource::setTarget(const string& target, const StringPosRange& targetRange)
{
    m_target = target;
    m_target_range = targetRange;
}

//------------------------------------------------------------------*
/**
 * Function name    Resource::getTarget
 * Description      returns the target for the resource
 * @return          const string&
 * @exception       -
 * @see             -
*/
const string& Resource::getTarget() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_target;
}

//------------------------------------------------------------------*
/**
 * Function name    Resource::getTargetRange
 * Description      returns the range of the target for the resource
 * @return          const StringPosRange&
 * @exception       -
 * @see             -
*/
const StringPosRange& Resource::getTargetRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_target_range;
}

