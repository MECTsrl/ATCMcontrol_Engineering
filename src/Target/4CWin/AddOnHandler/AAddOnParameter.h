/* $Header: /4CReleased/V2.20.00/Target/4CWin/ADDONHANDLER/AAddOnParameter.h 1     28.02.07 19:09 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: AAddOnParameter.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/ADDONHANDLER/AAddOnParameter.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Target
 *
 * =COMPONENT			AddOnHandler
 *
 * =CURRENT 	 $Date: 28.02.07 19:09 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _AADDONPARAMETER_H_
#define _AADDONPARAMETER_H_

#include "BaseAddOnParameter.h"

/* ---------------------------------------------------------------------------- */
/**
 * class CAAddOnParameter
 *
 */
class CAAddOnParameter : public CBaseAddonParameter
{
public:
	CAAddOnParameter();

    virtual CString GetCommChannelDescription(const CString& crstrCommChannel);
    virtual CString GetCommChannelFromDescription(const CString& crstrCommChannelDescr);

    virtual CString GetStandardProtocol(const CString& crstrCommChannel);
    virtual CString GetStandardProtocolParameter(const CString& crstrCommChannel);
    virtual CString GetStandardConnectParameter(const CString& crstrCommChannel);


/* ============================================================================
 * T A R G E T   S P E C I F I C
 * ============================================================================
 *
 */


};

#endif

/* ---------------------------------------------------------------------------- */
