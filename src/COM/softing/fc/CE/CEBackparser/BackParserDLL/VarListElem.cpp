
#include "stdafx.h"

#include "VarListElem.h"

//------------------------------------------------------------------*
/**
 * Function name    VarListElem::VarListElem
 * Description      constructor for class VarListElem
 * @param           const string& n
 * @param           const StringPos start
 * @return          -
 * @exception       -
 * @see             -
*/
VarListElem::VarListElem(const string& n, const StringPos start)
    : name(n), range(start)
{
    syscomment = NULL;
}

//------------------------------------------------------------------*
/**
 * Function name    VarListElem::~VarListElem
 * Description      destructor for class VarListElem
 * @return          -
 * @exception       -
 * @see             -
*/
VarListElem::~VarListElem()
{
    if (syscomment != NULL)
    {
        delete syscomment;
        syscomment = NULL;
    }
}

//------------------------------------------------------------------*
/**
 * Function name    VarListElem::getName
 * Description      returns the name of the st variable
 * @return          const string&
 * @exception       -
 * @see             -
*/
const string& VarListElem::getName() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return name;       
}

//------------------------------------------------------------------*
/**
 * Function name    VarListElem::getNameRange
 * Description      returns the range of the name of the st variable
 * @return          const StringPosRange&
 * @exception       -
 * @see             -
*/
const StringPosRange& VarListElem::getNameRange() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return range; 
}

//------------------------------------------------------------------*
/**
 * Function name    VarListElem::getAddress
 * Description      returns the address of the st variable
 * @return          const string&
 * @exception       -
 * @see             -
*/
const string& VarListElem::getAddress() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return address;       
}

//------------------------------------------------------------------*
/**
 * Function name    VarListElem::getAddressRange
 * Description      returns the range of the address of the st variable
 * @return          const StringPosRange&
 * @exception       -
 * @see             -
*/
const StringPosRange& VarListElem::getAddressRange() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return addressRange; 
}

//------------------------------------------------------------------*
/**
 * Function name    VarListElem::getSystemComment
 * Description      returns the system comment of the st variable
 * @return          const SysComment*
 * @exception       -
 * @see             -
*/
const SysComment* VarListElem::getSystemComment() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return syscomment; 
}

