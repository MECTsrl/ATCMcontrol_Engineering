/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/res/VersionNr.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: VersionNr.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/res/VersionNr.h $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                GENTOOLS
 *
 * =COMPONENT           GENTOOLS/resource
 *
 * =CURRENT             $Date: 28.02.07 19:01 $
 *                      $Revision: 1 $
 *
 * =ENVIRONMENT         MS Windows NT 4.0 / MS Windows 2000
 *                      MSVC++ V6.0
 *                      MS Java 3.2
 *                      MS Visual SourceSafe 6.0
 *                      Perl5
 *
 * =REFERENCES
 *==
 *----------------------------------------------------------------------------*
 *

 

 

 *==
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    central defines for component file versioning
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATIONS
 * $History: VersionNr.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/res
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/res
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 25.07.03   Time: 10:26
 * Updated in $/4Control/COM/softing/fc/res
 * changed version format
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 11.12.00   Time: 19:06
 * Created in $/4Control/COM/softing/fc/res
 * Taken from V1.3, new files headers. Incremented version number to 2.0
 *==
 ******************************************************************************
H<<*/


#ifndef _VERSION_NR_H_
#define _VERSION_NR_H_

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


#define APP_VER_MAJOR            PROD_VER_MAJOR
#define APP_VER_MINOR            PROD_VER_MINOR
#define APP_VER_SERVP            PROD_VER_SERVP
#define APP_VER_BUILD            PROD_VER_BUILD

#define APP_VERSION_STR          PROD_VERSION_STR

/////////////////////////////////////////////////////////////////////////////
#endif // _VERSION_NR_H_

