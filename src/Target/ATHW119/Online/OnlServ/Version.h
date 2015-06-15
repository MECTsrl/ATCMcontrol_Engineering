/* $Header: /4CReleased/V2.20.00/Target/TEMPLATES/Online/OnlServ/Version.h 1     28.02.07 18:54 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Version.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/TEMPLATES/Online/OnlServ/Version.h $
 *
 * =PROJECT 			4CONTROL V2.x
 *
 * =SWKE				Target / Online
 *
 * =COMPONENT			OnlServ
 *
 * =CURRENT 	 $Date: 28.02.07 18:54 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _VERSION_H_
#define _VERSION_H_

// short description of component
#define APP_FILEDESCRIPTION "4C Embedded " PROD_SHORT_VERSION_STR " - Online Server"

// internal name of component (i.e. LIBRARY name of DLLs etc.)
#define APP_INTERNALNAME        "OnlServATHW119"

// build file name of component
#define APP_ORIGINALFILENAME    APP_INTERNALNAME ".exe"

//we support -RegServer -UnregServer
#define APP_SELFREGISTER

// target is a application
#define APP_FILETYPE            VFT_APP

#endif

/* ---------------------------------------------------------------------------- */
