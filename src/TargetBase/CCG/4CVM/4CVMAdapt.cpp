/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVM/4CVMAdapt.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: 4CVMAdapt.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVM/4CVMAdapt.cpp $
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

/* ----  Includes:	 ---------------------------------------------------------- */

#include <stdio.h>
#include <assert.h>
#include <tchar.h>

#include "lic\fc_slbase.h"    //lic acces IF
#include "4CVMAdapt.h"
#include "BuildNr.h"

/* ----  Local Defines:   ----------------------------------------------------- */

/* ----  Global Variables:   -------------------------------------------------- */

/* ----  Local Functions:   --------------------------------------------------- */

/* ----  Implementations:   --------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/**
 * Adapt4CVM_GetBuildNumber
 *
 */
long Adapt4CVM_GetBuildNumber()
{
    return PRODUCT_BUILD;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt4CVM_NeedLicensing2
 *
 */
bool Adapt4CVM_NeedLicensing2(TCHAR szTrgType[MAX_PATH])
{
    // standard implementation:
    _tcscpy(szTrgType, _T("4cwin"));
    return false;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt4CVM_GetLicensingInfo
 *
 */
void Adapt4CVM_GetLicensingInfo(long& lProdNum, const TCHAR*& pszProd, long& lMedia)
{
    // standard implementation, no licensing, no product number
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt4CVM_NeedSourceProtectionCheck
 *
 */
bool Adapt4CVM_NeedSourceProtectionCheck()
{
    // standard implementation, no source protection needed
    return false;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt4CVM_SetFeatures
 *
 */
void Adapt4CVM_SetFeatures(CG_Features* pFeat)
{
    // standard implementation, no additional features changed
}

/* ---------------------------------------------------------------------------- */
