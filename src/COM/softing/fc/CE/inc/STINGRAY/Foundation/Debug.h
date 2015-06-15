//////////////////////////////////////////////////////////////
//
//
// Debug.h is a replacement for the debugging macros which 
// have become so well known in MFC. They will replace the
// ASSERT, VERIFY and TRACE macros in name but not neccesarily 
// in function. Particularly an ASSERT is very difficult to use
// to find the code that caused the error so a less drastic 
// error system will be provided here.
//
// Debug.h was written by Bob Powell (bobpowell@roguewave.com)
//
//////////////////////////////////////////////////////////////

// Exclude if __AFX_H__ is defined
#ifndef __AFX_H__

#ifndef __DEBUGMACROS__
#define __DEBUGMACROS__

#ifndef NONSTANDARD_GDI

#include <crtdbg.h>

#endif //NONSTANDARD_GDI

#include <stdlib.h>
#include <windows.h>
#include <winuser.h>

#ifdef _DEBUG

#ifndef _INC_TCHAR
	#include "tchar.h"

#endif

static void DoAssert(char *pFile, int nLine)
{
	static TCHAR buffer[256];
	wsprintf(buffer,_T("Debug ASSERT failed in file %s at line %d"),pFile,nLine);
	switch(::MessageBox(NULL,(LPCTSTR)buffer,_T("DEBUG"),MB_ABORTRETRYIGNORE | MB_ICONSTOP))
	{
	case IDABORT:
		_exit(1);
		break;
	case IDRETRY:
		DebugBreak(); // Step out of this routine to go back to the file that produced the error.
		break;
	case IDIGNORE:
		break;
	}
}

static void DoTrace(TCHAR *pFormat,...)
{
	va_list args;
	va_start(args, pFormat);
	TCHAR buffer[512];
	int charcount = wsprintf(buffer, pFormat, args);
	if(charcount>0)
#ifndef NONSTANDARD_GDI
		_RPT1(_CRT_WARN,"%s\n",buffer);
#else // if NONSTANDARD_GDI
#endif //NONSTANDARD_GDI
	va_end(args);
}

#define ASSERT(x)\
	if(!(x))\
		DoAssert(__FILE__,__LINE__);\


#define TRACE(x) \
	DoTrace(_T("%s"),x);\

#define TRACE0(x) TRACE(x)

#define TRACE1(x,a)\
	DoTrace(x,a);\

#define TRACE2(x,a,b)\
	DoTrace(x,a,b);\

#define TRACE3(x,a,b,c)\
	DoTrace(x,a,b,c);\

#define TRACE4(x,a,b,c,d)\
	DoTrace(x,a,b,c,d);\

#define VERIFY(x) ASSERT(x)

#else // _DEBUG

#define ASSERT(x) ((void)(0))
#define VERIFY(x) ((void)(x))

#endif // _DEBUG

#endif // defined __DEBUGMACROS__

#endif // !defined __AFX_H__
