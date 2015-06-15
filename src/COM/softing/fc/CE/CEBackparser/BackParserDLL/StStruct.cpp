#include "stdafx.h"

#include "StStruct.h"

//------------------------------------------------------------------*
/**
 * Function name    StStruct::StStruct
 * Description      constructor for class StStruct
 * @param           const string& name
 * @param           const StringPosRange nameRange
 * @param           const StringPos lineNo
 * @param           const StringPos nextVarSectInsertPos
 * @return          -
 * @exception       -
 * @see             -
*/
StStruct::StStruct(const string& name,
                   const StringPosRange& nameRange,
                   StringPos lineNo,
                   StringPos nextVarSectInsertPos)
    : POU(name, nameRange, lineNo, nextVarSectInsertPos)
{
}

//------------------------------------------------------------------*
/**
 * Function name    StStruct::getPOUType
 * Description      returns the type of the pou
 * @return          string
 * @exception       -
 * @see             -
*/
string StStruct::getPOUType() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    
    return _STRUCT; 
}

//------------------------------------------------------------------*
/**
 * Function name    StStruct::invite
 * Description      calls visit of the visitor for itself
 * @param           const IECSRCVisitor& visitor
 * @return          -
 * @exception       -
 * @see             -
*/
void StStruct::invite(const IECSRCVisitor& visitor) const
{
    POU::invite(visitor);
    visitor.visit(*this);
}

