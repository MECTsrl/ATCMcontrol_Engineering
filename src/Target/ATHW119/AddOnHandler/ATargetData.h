/* $Header: /4CReleased/V2.20.00/Target/TEMPLATES/AddOnHandler/ATargetData.h 1     28.02.07 18:54 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ATargetData.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/TEMPLATES/AddOnHandler/ATargetData.h $
 *
 * =PROJECT 			4CONTROL V2.x
 *
 * =SWKE				Target
 *
 * =COMPONENT			AddOnHandler
 *
 * =CURRENT 	 $Date: 28.02.07 18:54 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _ATARGETDATA_H_
#define _ATARGETDATA_H_

#include "BaseTargetData.h"

/* ---------------------------------------------------------------------------- */
/**
 * class CATargetData
 *
 */
class CATargetData : public CBaseTargetData
{
public:
    CATargetData(CBaseTargetObject* pTargetObject,
                    ICEProjInfo* pICEProjInfo,
                    const CString& crstrKADFileName);

    virtual CString GetCommChannelDescription(const CString& crstrCommChannel);

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
