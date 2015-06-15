#include "stdafx.h"

#include "Function.h"

//------------------------------------------------------------------*
/**
 * Function name    Function::Function
 * Description      constructor for class Function
 * @param           const string& name
 * @param           const StringPosRange nameRange
 * @param           const StringPos lineNo
 * @param           const StringPos nextVarSectInsertPos
 * @return          -
 * @exception       -
 * @see             -
*/
Function::Function(const string& name,
                   const StringPosRange& nameRange,
                   StringPos lineNo,
                   StringPos nextVarSectInsertPos)
    : POU(name, nameRange, lineNo, nextVarSectInsertPos)
{
}

//------------------------------------------------------------------*
/**
 * Function name    Function::getPOUType
 * Description      returns the type of the pou
 * @return          string
 * @exception       -
 * @see             -
*/
string Function::getPOUType() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    
    return _FUNCTION; 
}

//------------------------------------------------------------------*
/**
 * Function name    Function::invite
 * Description      calls visit of the visitor for itself
 * @param           const IECSRCVisitor& visitor
 * @return          -
 * @exception       -
 * @see             -
*/
void Function::invite(const IECSRCVisitor& visitor) const
{
    POU::invite(visitor);
    visitor.visit(*this);
}

//------------------------------------------------------------------*
/**
 * Function name    Function::setFunctionType
 * Description      sets the return type of the st function
 * @param           const string& functionType 
 * @return          -
 * @exception       -
 * @see             -
*/
void Function::setFunctionType(const string& functionType)
{
    m_function_type = functionType;
}

//------------------------------------------------------------------*
/**
 * Function name    Function::getFunctionType
 * Description      returns the return type of the st function
 * @return          string&
 * @exception       -
 * @see             -
*/
const string& Function::getFunctionType() const
{
    return m_function_type;
}

//------------------------------------------------------------------*
/**
 * Function name    Function::setFunctionTypeRange
 * Description      sets the range of the return type of the st function
 * @param           const string& functionType 
 * @return          -
 * @exception       -
 * @see             -
*/
void Function::setFunctionTypeRange(const StringPosRange functionTypeRange)
{
    m_function_type_range = functionTypeRange;
}

//------------------------------------------------------------------*
/**
 * Function name    Function::getFunctionTypeRange
 * Description      returns the range of the return type of the st function
 * @return          StringPosRange
 * @exception       -
 * @see             -
*/
const StringPosRange& Function::getFunctionTypeRange() const
{
    return m_function_type_range;
}
