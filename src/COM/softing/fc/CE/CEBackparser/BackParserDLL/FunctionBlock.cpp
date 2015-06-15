
#include "stdafx.h"

#include "FunctionBlock.h"

//------------------------------------------------------------------*
/**
 * Function name    FunctionBlock::FunctionBlock
 * Description      constructor for class FunctionBlock
 * @param           const string& name
 * @param           const StringPosRange nameRange
 * @param           const StringPos lineNo
 * @param           const StringPos nextVarSectInsertPos
 * @return          -
 * @exception       -
 * @see             -
*/
FunctionBlock::FunctionBlock(const string& name,
                             const StringPosRange& nameRange,
                             StringPos lineNo,
                             StringPos nextVarSectInsertPos)
    : POU(name, nameRange, lineNo, nextVarSectInsertPos)
{
}

//------------------------------------------------------------------*
/**
 * Function name    FunctionBlock::getPOUType
 * Description      returns the type of the pou
 * @return          string
 * @exception       -
 * @see             -
*/
string FunctionBlock::getPOUType() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    
    return _FUNCTION_BLOCK; 
}

//------------------------------------------------------------------*
/**
 * Function name    FunctionBlock::invite
 * Description      calls visit of the visitor for itself
 * @param           const IECSRCVisitor& visitor
 * @return          -
 * @exception       -
 * @see             -
*/
void FunctionBlock::invite(const IECSRCVisitor& visitor) const
{
    POU::invite(visitor);
    visitor.visit(*this);
}
