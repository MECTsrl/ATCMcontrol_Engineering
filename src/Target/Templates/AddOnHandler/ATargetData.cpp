/* $Header: /4CReleased/V2.20.00/Target/TEMPLATES/AddOnHandler/ATargetData.cpp 1     28.02.07 18:54 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ATargetData.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/Target/TEMPLATES/AddOnHandler/ATargetData.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Target
 *
 * =COMPONENT			AddOnHandler
 *
 * =CURRENT 	 $Date: 28.02.07 18:54 $
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
#include "ATargetData.h"
#include "CommDefinitions.h"
#include "utilif.h"

/* ----  Target Specific Includes:    ----------------------------------------- */

/* ----  Local Defines:   ----------------------------------------------------- */

/* ----  Global Variables:   -------------------------------------------------- */

/* ----  Local Functions:   --------------------------------------------------- */

/* ----  Implementations:   --------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/**
 * CATargetData
 *
 */
CATargetData::CATargetData
(
    CBaseTargetObject* pTargetObject,
    ICEProjInfo* pICEProjInfo,
    const CString& crstrKADFileName
) :  CBaseTargetData(pTargetObject, pICEProjInfo, crstrKADFileName)
{

}

/* ---------------------------------------------------------------------------- */
/**
 * GetCommChannelDescription
 *
 */
CString CATargetData::GetCommChannelDescription(const CString& crstrCommChannel)
{
    return _T("");
}

/* ---------------------------------------------------------------------------- */
/**
 * GetStandardProtocol
 *
 */
CString CATargetData::GetStandardProtocol(const CString& crstrCommChannel)
{
    return STD_PROTOCOL;
}

/* ---------------------------------------------------------------------------- */
/**
 * GetStandardProtocolParameter
 *
 */
CString CATargetData::GetStandardProtocolParameter(const CString& crstrCommChannel)
{
    return STD_PROTOCOL_PARAMETER;
}

/* ---------------------------------------------------------------------------- */
/**
 * GetStandardConnectParameter
 *
 */
CString CATargetData::GetStandardConnectParameter(const CString& crstrCommChannel)
{
    CString strStdConnPar;
    if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_TCP) == 0)
    {
        // get local machine
        CString strStdConnPar;
        UTIL_GetLocalMachineName(strStdConnPar.GetBuffer(100), 100);
        strStdConnPar.ReleaseBuffer();
    }
    else if(crstrCommChannel.CompareNoCase(COMM_CHANNEL_RS232) == 0)
    {
        strStdConnPar = STD_RS232_PAR;
    }
    return strStdConnPar;
}


/* ============================================================================
 * T A R G E T   S P E C I F I C
 * ============================================================================
 *
 */


/* ---------------------------------------------------------------------------- */
