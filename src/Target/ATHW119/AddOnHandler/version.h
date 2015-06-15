/* $Header: /4CReleased/V2.20.00/Target/TEMPLATES/AddOnHandler/version.h 1     28.02.07 18:54 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: version.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/TEMPLATES/AddOnHandler/version.h $
 *
 * =PROJECT 			4CONTROL V2.x
 *
 * =SWKE				Target
 *
 * =COMPONENT			AddOnHandler
 *
 * =CURRENT 	 $Date: 28.02.07 18:54 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _VERSION_H_
#define _VERSION_H_

// short description of component
#define APP_FILEDESCRIPTION "4C Embedded " PROD_SHORT_VERSION_STR " - AddOn Handler"

// internal name of component (i.e. LIBRARY name of DLLs etc.)
#define APP_INTERNALNAME        "AddOnHandler"

// build file name of component
#define APP_ORIGINALFILENAME    APP_INTERNALNAME ".dll"

//we support -RegServer -UnregServer
#define APP_SELFREGISTER

// target is a DLL
#define APP_FILETYPE            VFT_DLL

#endif

/* ---------------------------------------------------------------------------- */
