
#ifndef _VERSION_H_
#define _VERSION_H_

#ifdef _DEBUG
#   ifndef DEBUG
#   define DEBUG
#   endif // ! DEBUG
#endif // ! _DEBUG

// short description of component
#define APP_FILEDESCRIPTION		"ATCMControl " PROD_SHORT_VERSION_STR " - Utility DLL"

// internal name of component (i.e. LIBRARY name of DLLs etc.)
#define APP_INTERNALNAME        "fc_util"

// build file name of component
#define APP_ORIGINALFILENAME    "fc_util.dll"

// target is a DLL
#define APP_FILETYPE            VFT_DLL

#endif // _VERSION_H_

/*
 *----------------------------------------------------------------------------*
 *  $History: version.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 2  *****************
 * User: Ln           Date: 21.10.03   Time: 20:56
 * Updated in $/4Control/COM/softing/fc/CMN/fc_util
 * 
 *==
 *----------------------------------------------------------------------------*
 */
