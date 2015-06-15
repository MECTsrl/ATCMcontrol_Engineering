/* $Header: /4CReleased/V2.20.00/Target/TEMPLATES/AddOnHandler/AAddOnParameter.cpp 1     28.02.07 18:54 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: AAddOnParameter.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/Target/TEMPLATES/AddOnHandler/AAddOnParameter.cpp $
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

#include "StdAfx.h"
#include "AAddOnParameter.h"

/* ----  Target Specific Includes:    ----------------------------------------- */

/* ----  Local Defines:   ----------------------------------------------------- */

/* ----  Global Variables:   -------------------------------------------------- */

/* ----  Local Functions:   --------------------------------------------------- */

/* ----  Implementations:   --------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/**
 * CAAddOnParameter
 *
 */
CAAddOnParameter::CAAddOnParameter()
{
}

/* ---------------------------------------------------------------------------- */
/**
 * GetCommChannelDescription
 *
 */
CString CAAddOnParameter::GetCommChannelDescription(const CString& crstrCommChannel)
{
    return CBaseAddonParameter::GetCommChannelDescription(crstrCommChannel);
}

/* ---------------------------------------------------------------------------- */
/**
 * GetCommChannelFromDescription
 *
 */
CString CAAddOnParameter::GetCommChannelFromDescription(const CString& crstrCommChannelDescr)
{
    return CBaseAddonParameter::GetCommChannelFromDescription(crstrCommChannelDescr);
}

/* ---------------------------------------------------------------------------- */
/**
 * GetStandardProtocol
 *
 */
CString CAAddOnParameter::GetStandardProtocol(const CString& crstrCommChannel)
{
    return CBaseAddonParameter::GetStandardProtocol(crstrCommChannel);
}

/* ---------------------------------------------------------------------------- */
/**
 * GetStandardProtocolParameter
 *
 */
CString CAAddOnParameter::GetStandardProtocolParameter(const CString& crstrCommChannel)
{
    return CBaseAddonParameter::GetStandardProtocolParameter(crstrCommChannel);
}

/* ---------------------------------------------------------------------------- */
/**
 * GetStandardConnectParameter
 *
 */
CString CAAddOnParameter::GetStandardConnectParameter(const CString& crstrCommChannel)
{
    return CBaseAddonParameter::GetStandardConnectParameter(crstrCommChannel);
}


/* ============================================================================
 * T A R G E T   S P E C I F I C
 * ============================================================================
 *
 */


/* ---------------------------------------------------------------------------- */
