/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/inc/4cvmAdapt.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: 4cvmAdapt.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/inc/4cvmAdapt.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			4CVM
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _4CVMADAPT_H_
#define _4CVMADAPT_H_

#include "CG_IECFront.h"


/*
    Adapt4CVM_GetBuildNumber:
    return :  the build number of the addon's build
 */
long Adapt4CVM_GetBuildNumber();


/*
    Need Adapt4CVM_NeedLicensing:
    @param szTrgType  : output: the target type, only targets with this name 
                                are allowed, if "" is returned do not check the
                                target type name. Reason for this param: we don't
                                want that the 4cvm.exe for the win or deme target 
                                can be simply copied to 4cosai !
    return true:  licensing needed, Adpat4CVM_GetLicensingInfo will be called 
                                    to get additional product info or do the licensing
           false: no license checks at all
 */
bool Adapt4CVM_NeedLicensing2(TCHAR szTrgType[MAX_PATH]);

/*
    Adpat4CVM_GetLicensingInfo
    

 */
void Adapt4CVM_GetLicensingInfo(long& lProdNum, const TCHAR*& pszProd, long& lMedia);

/*
    Adapt4CVM_NeedSourceProtectionCheck
    return true: if source protection should be checked
 */
bool Adapt4CVM_NeedSourceProtectionCheck();


/*
    Adapt4CVM_SetFeatures
    adaptable filling of the fature set for the IEC front end
    The feature set will be set by standard values for the vmm compiler and the
    values read from trhe kad for the target before this call
    In this adaptation function aditionla values, not changeble via the kad, can be set and reset for a target
 */
void Adapt4CVM_SetFeatures(CG_Features* pFeat);


#endif // __4CVMADAPT_H_

/* ---------------------------------------------------------------------------- */
