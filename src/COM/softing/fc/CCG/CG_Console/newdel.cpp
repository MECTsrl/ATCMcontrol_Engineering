

//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include <stdio.h>

//----  Static Initializations:   ----------------------------------*

//memory allocation:
#ifdef DEBUG
 //To use the fc_tools.lib we must supply the memory allocation 
 //functions that are called from fc_tools.lib, use the fc_todebug.dll:
 #define FC_LIB_USE_TODEBUG
#else
 #define FC_LIB_USE_CRT
#endif

#include "fc_tools\fc_libmem.h"


#ifdef DEBUG
//Note: overload new/delete if debug is needed so that
//we can fc_tools.lib allocated memory can be deleted 
//safely. 
void* operator new(size_t s)
{
    //if(s==4)
    //    assert(0);
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


