/* $Header: /4CReleased/V2.20.00/COM/softing/fc/res/Product.h 2     31.01.08 18:58 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Product.h $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/res/Product.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				
 *
 * =COMPONENT			
 *
 * =CURRENT 	 $Date: 31.01.08 18:58 $
 *			 $Revision: 2 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _PRODUCT_H_
#define _PRODUCT_H_

//---------------------------------------------------------------------------
// Update these fields for each build.
//---------------------------------------------------------------------------
// Layout of ATCMControl version number:
//      V<H>.<U>.<S>.<B>
// Explanation:
//  <H>     main version            1 digit
//  <U>     upgrade version         2 digits
//  <S>     servicepack version     2 digits
//  <B>     build index             4 digits

#define PROD_VER_MAJOR          2
#define PROD_VER_MINOR          20
#define PROD_VER_SERVP          02
#define PROD_VER_BUILD          PRODUCT_BUILD

#define __MK_STR(x)             #x
#define _MK_STR(x)              __MK_STR(x)

#define PROD_SHORT_VERSION_STR  "V2.2"
#define PROD_PRODUCTNAME        "ATCMControl"
#define PROD_VERSION_STR        _MK_STR(PROD_VER_MAJOR.PROD_VER_MINOR.PROD_VER_SERVP.PROD_VER_BUILD)
#define PROD_FULL_VERSION_STR   _MK_STR(V2.2 (PROD_VER_MAJOR.PROD_VER_MINOR.PROD_VER_SERVP.PROD_VER_BUILD))
// NOTE: Please do not show PROD_VERSION_STR in About-Dialogs, use PROD_FULL_VERSION_STR instead!
// it is only used by setup-tools/IE4 CAB-Downloader to find out which version
// is newer. It is important that Service-Patches are handled like minor versions.

#define PROD_COMPANYNAME        "ATCM"
#define PROD_COPYRIGHT          "ATCM. All rights reserved."
#define PROD_COPYRIGHT_SHORT    "ATCM. All rights reserved."


/////////////////////////////////////////////////////////////////////////////
#endif // _PRODUCT_H_

