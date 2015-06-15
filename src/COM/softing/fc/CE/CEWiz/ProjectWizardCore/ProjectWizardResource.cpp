#include "stdafx.h"
#include "ProjectWizardRes.h"
#include "ProjectWizardResource.h"
#include "utilif.h"


//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



///////////////////////////////////////////////////////////////////////////

CProjectWizardResource::CProjectWizardResource()
{
}

CString CProjectWizardResource::Trim(CString sString)
// removes spaces before and after the string
{
    CString sResult(sString);
    
    sResult.TrimLeft();
    sResult.TrimRight();
    return sResult;
}

void CProjectWizardResource::SetResource(CString sResource)
// sets the name of the resource
{
    m_sResource = Trim(sResource);
}

CString CProjectWizardResource::GetResource()
// returns the name of the resource
{
    return m_sResource;
}


void CProjectWizardResource::SetTarget(CString sTarget)
// sets the name of the target 
{
    m_sTarget = Trim(sTarget);
}

CString CProjectWizardResource::GetTarget()
// returns the name of the target
{
    return m_sTarget;
}

void CProjectWizardResource::SetAddress(CString sAddress)
// sets the address
{
    m_sAddress = Trim(sAddress);
}

CString CProjectWizardResource::GetAddress()
// returns the address
{
    return m_sAddress;
}


//------------------------------------------------------------------*
/**
 * Returns number of tasks in resource
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
int CProjectWizardResource::GetTaskCount ()
{
    return (m_Tasks.GetSize ());
}


//------------------------------------------------------------------*
/**
 * Sets number of tasks in resource
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardResource::SetTaskCount (int iCount)
{
    m_Tasks.SetSize (iCount);
}


//------------------------------------------------------------------*
/**
 * Returns task at given index
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
CProjectWizardTask *CProjectWizardResource::GetTask (int i)
{
    return (&m_Tasks[i]);
}
