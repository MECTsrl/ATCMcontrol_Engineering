/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/opcDynGroup.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: opcDynGroup.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/opcDynGroup.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
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
#include "Security.h"
#include "opcDynGroup.h"
#include "opcDynServer.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CDynamicOpcGroup
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CDynamicOpcGroup::CDynamicOpcGroup()
:GenericGroup(poll, GENLIST_CREATE_MAP, 10, 101)
{
    dwOrigUpdateRate = 0;
    dwMaxRequestTime = 0;
    dwRequestCount   = 0;

    trustUniqueChilds(TRUE);
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CDynamicOpcGroup::~CDynamicOpcGroup()
{
}

//------------------------------------------------------------------*
/**
 * createDynamicTag
 *
 * @param           void
 * @return          void
 * @exception       -
*/
HRESULT
    CDynamicOpcGroup::validateItem(IN LPCTSTR id, IN LPCTSTR accessPath, 
			                       IN VARTYPE reqDataType, OUT VARTYPE *nativeDataType, 
			                       OUT DWORD *accessRights, OUT OPTIONAL GenericItemTag **tag /*= NULL*/)
{
    CString strID = id;
    strID.MakeLower();
    return GenericGroup::validateItem(strID, accessPath, reqDataType, nativeDataType, accessRights, tag);
}



BOOL 
    CDynamicOpcGroup::changeUpdateRate(IN DWORD updateRate)
{
    BOOL bRet = TRUE;
    if (dwOrigUpdateRate == 0)
    {
        dwOrigUpdateRate = getUpdateRate();
        dwLastCheckTime = ::GetTickCount();
    }
    dwRequestCount = 0;
    dwMaxRequestTime = 0;
    dwLastCheckTime = ::GetTickCount();

    if (updateRate>getUpdateRate())
    {
        TRACE(_T("Adapt Group Update Rate (++++increase++++): Group %s, update rate: %u ms\n" ), getName(), updateRate);    
        bRet = setUpdateRate(updateRate);
    }

    return bRet;
}

#define GROUP_REQUEST_TIME_REFRESH 10

BOOL 
    CDynamicOpcGroup::checkUpdateRate(IN DWORD requestTime)
{
    BOOL ret = FALSE;

    if (dwOrigUpdateRate == 0)
    {
        dwOrigUpdateRate = getUpdateRate();
        dwLastCheckTime = ::GetTickCount();
    }  
    
    if (requestTime>dwMaxRequestTime)
    {
        dwMaxRequestTime = requestTime;
    }
    
    dwRequestCount++;

    DWORD dwTimeNow = ::GetTickCount();

    if (dwTimeNow - dwLastCheckTime > 10000)
    
    //if (dwRequestCount> (DWORD)(/*m_leafList.getCount() * */GROUP_REQUEST_TIME_REFRESH))
    {
        dwMaxRequestTime += 100;
        if (dwMaxRequestTime<dwOrigUpdateRate)
        {
            dwMaxRequestTime = dwOrigUpdateRate;
        }
        if (dwMaxRequestTime<getUpdateRate())// && dwMaxRequestTime>dwOrigUpdateRate)
        {
            setUpdateRate(dwMaxRequestTime);
            TRACE(_T("Adapt Group Update Rate (----decrease----): Group %s, update rate: %u ms, after %u tries\n" ), 
                              getName(), dwMaxRequestTime, dwRequestCount);    
            ret = TRUE;
        }
        dwMaxRequestTime = 0;
        dwRequestCount = 0;
        dwLastCheckTime = dwTimeNow;
    }
    return ret;
}

/* ---------------------------------------------------------------------------- */
