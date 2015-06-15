/* $Header: /4CReleased/V2.20.00/Target/4CWin/ONLINE/OnlServ/Version.h 1     28.02.07 19:11 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Version.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/ONLINE/OnlServ/Version.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Target / Online
 *
 * =COMPONENT			OnlServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:11 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _VERSION_H_
#define _VERSION_H_

// short description of component
#define APP_FILEDESCRIPTION "ATCM Embedded " PROD_SHORT_VERSION_STR " - Online Server"

// internal name of component (i.e. LIBRARY name of DLLs etc.)
#define APP_INTERNALNAME        "WinSCS"

// build file name of component
#define APP_ORIGINALFILENAME    APP_INTERNALNAME ".exe"

//we support -RegServer -UnregServer
#define APP_SELFREGISTER

// target is a application
#define APP_FILETYPE            VFT_APP

#endif

/* ---------------------------------------------------------------------------- */
