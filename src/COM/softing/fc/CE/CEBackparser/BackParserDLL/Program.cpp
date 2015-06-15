
#include "stdafx.h"

#include "Program.h"

//------------------------------------------------------------------*
/**
 * Function name    Program::Program
 * Description      constructor for class Program
 * @param           const string& name
 * @param           const StringPosRange nameRange
 * @param           const StringPos lineNo
 * @param           const StringPos nextVarSectInsertPos
 * @return          -
 * @exception       -
 * @see             -
*/
Program::Program(const string& name,
                 const StringPosRange& nameRange,
                 StringPos lineNo,
                 StringPos nextVarSectInsertPos)
    : POU(name, nameRange, lineNo, nextVarSectInsertPos)
{
}

//------------------------------------------------------------------*
/**
 * Function name    Program::getPOUType
 * Description      returns the type of the pou
 * @return          string
 * @exception       -
 * @see             -
*/
string Program::getPOUType() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    
    return _PROGRAM; 
}

//------------------------------------------------------------------*
/**
 * Function name    Program::invite
 * Description      calls visit of the visitor for itself
 * @param           const IECSRCVisitor& visitor
 * @return          -
 * @exception       -
 * @see             -
*/
void Program::invite(const IECSRCVisitor& visitor) const
{
    POU::invite(visitor);
    visitor.visit(*this);
}

