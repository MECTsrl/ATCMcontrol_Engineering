/* $Header: /4CReleased/V2.20.00/Target/TEMPLATES/CCG/4CVM/4CVMAdapt.cpp 1     28.02.07 18:54 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: 4CVMAdapt.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/Target/TEMPLATES/CCG/4CVM/4CVMAdapt.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Target / CCG
 *
 * =COMPONENT			4CVM
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

#include <stdio.h>
#include <assert.h>
#include <tchar.h>

#include "lic\fc_slbase.h"    //lic acces IF
#include "lic\SoftingProd.h"  //lic product codes
#include "lic\LicName.h"
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
    _tcscpy(szTrgType, _T("[*PR]"));
    return false;
}

/* ---------------------------------------------------------------------------- */
/**
 * Adapt4CVM_GetLicensingInfo
 *
 */
void Adapt4CVM_GetLicensingInfo(long& lProdNum, const TCHAR*& pszProd, long& lMedia)
{
/*
    lProdNum = SPROD_4C_[*TNUC];
    pszProd  = SPROD_4C_[*TNUC]_NAME;
    lMedia   = SLIC_MEDIA_ANY;
*/
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
