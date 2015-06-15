/* $Header: /4CReleased/V2.20.00/Target/TEMPLATES/res/VersionNr.h 1     28.02.07 18:54 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: VersionNr.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/TEMPLATES/res/VersionNr.h $
 *
 * =PROJECT 			4CONTROL V2.x
 *
 * =SWKE				Target
 *
 * =COMPONENT			Target
 *
 * =CURRENT 	 $Date: 28.02.07 18:54 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _VERSION_NR_H_
#define _VERSION_NR_H_

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


#define APP_VER_MAJOR            PROD_VER_MAJOR
#define APP_VER_MINOR            PROD_VER_MINOR
#define APP_VER_SERVP            PROD_VER_SERVP
#define APP_VER_BUILD            PROD_VER_BUILD


#define APP_VERSION_STR          PROD_VERSION_STR

#endif

/* ---------------------------------------------------------------------------- */
