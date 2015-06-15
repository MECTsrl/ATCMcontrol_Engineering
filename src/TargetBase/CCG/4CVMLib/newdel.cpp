/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/newdel.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: newdel.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/newdel.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			4CVMLib
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include <stdio.h>
#include <tchar.h>


//memory allocation:
#ifdef DEBUG
 //To use the fc_tools.lib we must supply the memory allocation 
 //functions that are called from fc_tools.lib, use the fc_todebug.dll:
 #define FC_LIB_USE_TODEBUG
#else
 #define FC_LIB_USE_CRT
#endif

#include "fc_tools\fc_libmem.h"
void InitModuleCheckMemory(const TCHAR* pszModule)
{
}

void ExitModuleCheckMemory(const TCHAR* pszModule)
{
#if defined(DEBUG) && defined(FC_LIB_USE_TODEBUG)
    unsigned size;
    unsigned cnt;
    
    cnt = FC_GetAllocCount(&size);
    if(cnt || size)
    {
        _tprintf(_T("%s: Memory Leak!! cnt=%d sum=%d first ulnAlloc=%u\n"),
         pszModule, cnt, size, FC_GetFirstAllocBreak());
    }
#endif
}

#ifdef DEBUG
//Note: overload new/delete if debug is needed so that
//we can fc_tools.lib allocated memory can be deleted 
//safely. 
void* operator new(size_t s)
{
    void* p=NULL;
    FC_NewMemory(&p,s);
    return p;
}
void operator delete(void* p)
{
  if(p) 
    FC_FreeMemory(p);
}
#endif

/* ---------------------------------------------------------------------------- */
