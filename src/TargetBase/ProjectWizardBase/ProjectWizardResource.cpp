/* $Header: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardResource.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ProjectWizardResource.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardResource.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase
 *
 * =COMPONENT			ProjectWizardBase
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "ProjectWizardRes.h"
#include "ProjectWizardResource.h"
#include "utilif.h"

///////////////////////////////////////////////////////////////////////////

CProjectWizardResource::CProjectWizardResource()
{
}

CString CProjectWizardResource::Trim(CString strString)
// removes spaces before and after the string
{
    CString strResult(strString);
    
    strResult.TrimLeft();
    strResult.TrimRight();
    return strResult;
}

void CProjectWizardResource::SetResourceName(CString strResource)
// sets the name of the resource
{
    m_strResource = Trim(strResource);
}

CString CProjectWizardResource::GetResourceName()
// returns the name of the resource
{
    return m_strResource;
}


void CProjectWizardResource::SetTarget(CString strTarget)
// sets the name of the target 
{
    m_strTarget = Trim(strTarget);
}

CString CProjectWizardResource::GetTarget()
// returns the name of the target
{
    return m_strTarget;
}

void CProjectWizardResource::SetOPCServer(CString strOPCServer)
{
    m_strOPCServer = strOPCServer;
}

CString CProjectWizardResource::GetOPCServer()
{
    return m_strOPCServer;
}

void CProjectWizardResource::SetCommChannel(CString strCommChannel)
{
    m_strCommChannel = strCommChannel;
}

CString CProjectWizardResource::GetCommChannel()
{
    return m_strCommChannel;
}


//------------------------------------------------------------------*
/**
 * set connect parameter.
 *
 * @param            crstrConnectPar: connect parameter to set.
 * @param            crstrCommChannel: corresponding communication channel 
 *                                     (default current comm channel)
 * @return           -
 * @exception        -
 * @see              -
*/
void CProjectWizardResource::SetConnectPar(const CString& crstrConnectPar, const CString& crstrCommChannel /*= _T("")*/)
{
    CString strCommChannel(crstrCommChannel);
    if(crstrCommChannel.IsEmpty())
    {
        strCommChannel = m_strCommChannel;
    }
    m_tConnectParMap.SetAt(strCommChannel, crstrConnectPar);
}


//------------------------------------------------------------------*
/**
 * get connect parameter.
 *
 * @param            crstrCommChannel: corresponding communication channel 
 *                                     (default current comm channel)
 * @return           connect parameter string.
 * @exception        -
 * @see              -
*/
CString CProjectWizardResource::GetConnectPar(const CString& crstrCommChannel /*_T("")*/)
{
    CString strConnectPar;
    CString strCommChannel(crstrCommChannel);
    if(crstrCommChannel.IsEmpty())
    {
        strCommChannel = m_strCommChannel;
    }
    m_tConnectParMap.Lookup(strCommChannel, strConnectPar);
    return strConnectPar;
}

POSITION CProjectWizardResource::GetConnectParStartPosition()
{
    return m_tConnectParMap.GetStartPosition();
}

void CProjectWizardResource::GetNextConnectPar(POSITION& rNextPosition, CString& rstrCommChannel, CString& rstrConnectPar)
{
    m_tConnectParMap.GetNextAssoc(rNextPosition, rstrCommChannel, rstrConnectPar);
}


void CProjectWizardResource::SetFirmwareVersionIndex(UINT uiFirmwareVersionIndex)
{
    m_uiFirmwareVersionIndex = uiFirmwareVersionIndex;
}

UINT CProjectWizardResource::GetFirmwareVersionIndex()
{
    return m_uiFirmwareVersionIndex;
}

void CProjectWizardResource::SetControlString(CString strControlString)
{
    m_strControlString = Trim(strControlString);
}

CString CProjectWizardResource::GetControlString()
{
    return m_strControlString;
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
int CProjectWizardResource::GetTaskCount()
{
    return(m_Tasks.GetSize());
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
void CProjectWizardResource::SetTaskCount(int iCount)
{
    m_Tasks.SetSize(iCount);
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
CProjectWizardTask *CProjectWizardResource::GetTask(int i)
{
    return(&m_Tasks[i]);
}

/* ---------------------------------------------------------------------------- */
