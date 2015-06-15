/* $Header: /4CReleased/V2.20.00/Target/Templates/res/Product.h 3     31.01.08 18:59 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Product.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/Templates/res/Product.h $
 *
 * =PROJECT 			4CONTROL V2.x
 *
 * =SWKE				Target
 *
 * =COMPONENT			Target
 *
 * =CURRENT 	 $Date: 31.01.08 18:59 $
 *			 $Revision: 3 $
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _PRODUCT_H_
#define _PRODUCT_H_

//---------------------------------------------------------------------------
// Update these fields for each build.
//---------------------------------------------------------------------------
// Layout of 4Control version number:
//      V<H>.<U>.<S>.<B>
// Explanation:
//  <H>     main version            1 digit
//  <U>     upgrade version         2 digits
//  <S>     servicepack version     2 digits
//  <B>     build index             4 digits

#define PROD_VER_MAJOR          2
#define PROD_VER_MINOR          20
#define PROD_VER_SERVP          01
#define PROD_VER_BUILD          PRODUCT_BUILD

#define __MK_STR(x)             #x
#define _MK_STR(x)              __MK_STR(x)

#define PROD_SHORT_VERSION_STR  "V2.2"
#define PROD_PRODUCTNAME        "4C Embedded " PROD_SHORT_VERSION_STR " - ATHW119 AddOn"
#define PROD_VERSION_STR        _MK_STR(PROD_VER_MAJOR.PROD_VER_MINOR.PROD_VER_SERVP.PROD_VER_BUILD)
#define PROD_FULL_VERSION_STR   PROD_SHORT_VERSION_STR " " _MK_STR((PROD_VER_MAJOR.PROD_VER_MINOR.PROD_VER_SERVP.PROD_VER_BUILD))
// NOTE: Please do not show PROD_VERSION_STR in About-Dialogs, use PROD_FULL_VERSION_STR instead!
// it is only used by setup-tools/IE4 CAB-Downloader to find out which version
// is newer. It is important that Service-Patches are handled like minor versions.

#define PROD_COMPANYNAME        "ATCM"
#define PROD_COPYRIGHT          "ATCM. All rights reserved."
#define PROD_COPYRIGHT_SHORT    "\251 ATCM. All rights reserved."

#define PROD_BASE_BUILD_NR   	_MK_STR(BASE_BUILD_NR)

#endif

/* ---------------------------------------------------------------------------- */
