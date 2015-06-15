/* ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Version.h $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/Version.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				CSET
 *
 * =COMPONENT			Tools\Setup
 *
 * =CURRENT 	 $Date: 28.02.07 19:01 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _VERSION_H_
#define _VERSION_H_

#ifdef _DEBUG
#   ifndef DEBUG
#   define DEBUG
#   endif // ! DEBUG
#endif // ! _DEBUG

// short description of component
#define APP_FILEDESCRIPTION		"ATCMControl " PROD_SHORT_VERSION_STR " - Setup Tool"

// internal name of component (i.e. LIBRARY name of DLLs etc.)
#define APP_INTERNALNAME        "Setup"

// build file name of component
#define APP_ORIGINALFILENAME    "Setup.exe"

//we support -RegServer -UnregServer
#define APP_SELFREGISTER

// target is a DLL
#define APP_FILETYPE            VFT_DLL

#endif // _VERSION_H_

/* ---------------------------------------------------------------------------- */
