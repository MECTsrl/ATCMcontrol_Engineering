
#ifndef _VERSION_H_
#define _VERSION_H_

#ifdef _DEBUG
#   ifndef DEBUG
#   define DEBUG
#   endif // ! DEBUG
#endif // ! _DEBUG

// short description of component
#define APP_FILEDESCRIPTION		"ATCMControl " PROD_SHORT_VERSION_STR " - Engineering"

// internal name of component (i.e. LIBRARY name of DLLs etc.)
#define APP_INTERNALNAME        "4CEng"

// build file name of component
#define APP_ORIGINALFILENAME    "4CEng.exe"

//we support -RegServer -UnregServer
#define APP_SELFREGISTER

// target is a application
#define APP_FILETYPE            VFT_APP

#endif // _VERSION_H_

