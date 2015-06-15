
#ifndef _VERSION_H_
#define _VERSION_H_

#ifdef _DEBUG
#   ifndef DEBUG
#   define DEBUG
#   endif // ! DEBUG
#endif // ! _DEBUG

// short description of component
#define APP_FILEDESCRIPTION		"ATCMControl " PROD_SHORT_VERSION_STR " - Dialogs"

// internal name of component (i.e. LIBRARY name of DLLs etc.)
#define APP_INTERNALNAME        "Dialogs"

// build file name of component
#define APP_ORIGINALFILENAME    "Dialogs.DLL"

// target is a DLL
#define APP_FILETYPE            VFT_DLL

#endif // _VERSION_H_

