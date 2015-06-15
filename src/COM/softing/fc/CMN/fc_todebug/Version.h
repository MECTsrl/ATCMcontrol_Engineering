
#ifndef _VERSION_H_
#define _VERSION_H_

//---------------------------------------------------------------------------
// Change these defines for each component
//---------------------------------------------------------------------------
// short description of component
#ifdef DEBUG
 #define FC_DEB_  "debug"
#else
 #define FC_DEB_  "release"
#endif
#ifdef INSTALL_MALLOC_SPY
 #define FC_MALSPY_ "|MS"
#else
 #define FC_MALSPY_ ""
#endif
#ifdef REALLOC_STATISTICS
 #define FC_RELSTAT_ "|RS"
#else
 #define FC_RELSTAT_ ""
#endif

#define APP_FILEDESCRIPTION		"ATCMControl " PROD_SHORT_VERSION_STR " - Foundation Code: " FC_DEB_ FC_MALSPY_ FC_RELSTAT_

// internal name of component (i.e. LIBRARY name of DLLs etc.)
#define APP_INTERNALNAME        "fc_todebug"

// build file name of component
#define APP_ORIGINALFILENAME    "fc_todebug.dll"

#define APP_FILETYPE            VFT_DLL

#endif // _VERSION_H_

/*
 *----------------------------------------------------------------------------*
 *  $History: Version.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_todebug
 * 
 * *****************  Version 2  *****************
 * User: Ln           Date: 21.10.03   Time: 20:56
 * Updated in $/4Control/COM/softing/fc/CMN/fc_todebug
 * 
 *==
 *----------------------------------------------------------------------------*
 */
