
#include "stdafx.h"

#include "EmptyFile.h"

//------------------------------------------------------------------*
/**
 * Function name    EmptyFile::EmptyFile
 * Description      constructor for class EmptyFile
 * @return          -
 * @exception       -
 * @see             -
*/
EmptyFile::EmptyFile()
    : POU("NONAME", NULL, 0, 0)
{
    m_POUType = _EMPTYFILE;
}

//------------------------------------------------------------------*
/**
 * Function name    EmptyFile::getPOUType
 * Description      Return the type of the pou
 * @return          string
 * @exception       -
 * @see             -
*/
string EmptyFile::getPOUType() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    
    return m_POUType; 
}

//------------------------------------------------------------------*
/**
 * Function name    EmptyFile::invite
 * Description      calls visit of the visitor for itself
 * @param           const IECSRCVisitor& visitor
 * @return          -
 * @exception       -
 * @see             -
*/
void EmptyFile::invite(const IECSRCVisitor& visitor) const
{
    POU::invite(visitor);
    visitor.visit(*this);
}

//------------------------------------------------------------------*
/**
 * Function name    EmptyFile::setPOUType
 * Description      sets the type
 * @param           const string& POUType
 * @return          -
 * @exception       -
 * @see             -
*/
void EmptyFile::setPOUType(const string& POUType)
{
    m_POUType = POUType;       
}

//------------------------------------------------------------------*
/**
 * Function name    EmptyFile::setFileSysComment
 * Description      sets the members m_filesys_cmnt and m_nextVarSectInsertPos
 * @param           SysComment* fileSysComment
 * @return          -
 * @exception       -
 * @see             -
*/
void EmptyFile::setFileSysComment(SysComment* fileSysComment)
{
    POU::setFileSysComment(fileSysComment);
    if ((fileSysComment != 0) && 
        (m_nextVarSectInsertPos <= fileSysComment->GetRange().getLast()))
    {
        m_nextVarSectInsertPos = fileSysComment->GetRange().getLast() + 1;
    }
}

