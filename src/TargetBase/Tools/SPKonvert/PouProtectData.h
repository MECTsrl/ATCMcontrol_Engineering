/* $Header: /4CReleased/V2.20.00/TargetBase/TOOLS/SPKonvert/PouProtectData.h 1     28.02.07 19:07 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: PouProtectData.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/TOOLS/SPKonvert/PouProtectData.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / AddOnHandler
 *
 * =COMPONENT			AddOnBase
 *
 * =CURRENT 	 $Date: 28.02.07 19:07 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _POUPROTECTDATA_H_
#define _POUPROTECTDATA_H_

//------------------------------------------------------------------*
/**
 *  class CPouProtectData
 *
 *  @see    CGHFile
*/
class CPouProtectData
{
public:
    CPouProtectData() {};
    virtual ~CPouProtectData() {};

public:
    CString m_strFileName;
    CString m_strFingerprint;
};

#endif

/* ---------------------------------------------------------------------------- */
