
#include "stdafx.h"

#include "GlobVarConstList.h"

//------------------------------------------------------------------*
/**
 * Function name    GlobVarConstList::GlobVarConstList
 * Description      constructor for class GlobVarConstList
 * @return          -
 * @exception       -
 * @see             -
*/
GlobVarConstList::GlobVarConstList()
    : POU("NONAME", NULL, 0, 0)
{
    m_POUType = "";
}

//------------------------------------------------------------------*
/**
 * Function name    GlobVarConstList::getPOUType
 * Description      Return the type
 * @return          string
 * @exception       -
 * @see             -
*/
string GlobVarConstList::getPOUType() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    
    return m_POUType; 
}

//------------------------------------------------------------------*
/**
 * Function name    GlobVarConstList::invite
 * Description      calls visit of the visitor for itself
 * @param           const IECSRCVisitor& visitor
 * @return          -
 * @exception       -
 * @see             -
*/
void GlobVarConstList::invite(const IECSRCVisitor& visitor) const
{
    POU::invite(visitor);
    visitor.visit(*this);
}

//------------------------------------------------------------------*
/**
 * Function name    GlobVarConstList::setPOUType
 * Description      sets the type
 * @param           const string& POUType
 * @return          -
 * @exception       -
 * @see             -
*/
void GlobVarConstList::setPOUType(const string& POUType)
{
    m_POUType = POUType;       
}

//------------------------------------------------------------------*
/**
 * Function name    GlobVarConstList::setFileSysComment
 * Description      sets the members m_filesys_cmnt and m_nextVarSectInsertPos
 * @param           SysComment* fileSysComment
 * @return          -
 * @exception       -
 * @see             -
*/
void GlobVarConstList::setFileSysComment(SysComment* fileSysComment)
{
    POU::setFileSysComment(fileSysComment);
    if ((fileSysComment != 0) && 
        (m_nextVarSectInsertPos <= fileSysComment->GetRange().getLast()))
    {
        m_nextVarSectInsertPos = fileSysComment->GetRange().getLast() + 1;
    }
}

