/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_libmem.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_libmem.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_libmem.h $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT             $Date: 28.02.07 19:01 $
 *                      $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==FC_<libs> Memory allocation and assert interface definition and 
 *  two implementations. Include this header in one of the exe/dll's
 *  source file and define one of these before the #inlude
 *  #ifdef FC_LIB_IMPLEM_TODEBUG   ---> the fc_todebug.dll is used
 *  #ifdef FC_LIB_USE_CRT       ---> the standard C-runtime is used
 *  #else                       ---> no implementation is supplied.
 *  
 *  Warning: if you use todebug you should also overlaod new/delete operators
 *  accordingly. To do so define FC_LIB_IMPLEM_NEWDEL 
 *
 *  Example how to use fc_todebug.dll:
 *    #define FC_LIB_IMPLEM_TODEBUG
 *    #define FC_LIB_IMPLEM_NEWDEL
 *    #include "fc_libmem.h"
 * 
 *  Example how to use c-runtime:
 *    #define FC_LIB_USE_CRT
 *    #include "fc_libmem.h"
 * 
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  03.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/
#ifndef FC_NEWDEL_H_
#define FC_NEWDEL_H_

#include "fc_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/* FUNCTIONAL_DESCRIPTION
 *
 * allocate memory
 * 
 * INPUT:
 *     sizeNew : size of memory
 * OUTPUT:
 *     *ppNew : pointer to new memory
 * RETURN:
 *     TRUE for success => *ppNew!=NULL
 *     FALSE for no memory => *ppNew == NULL
 * USAGE:
 *     called from fc_tools lib to allocate memory
 */
FC_TOAPI int FC_TODECL FC_LibNewMemory(void** ppNew, size_t sizeNew);


/* FUNCTIONAL_DESCRIPTION
 *
 * reallocate memory
 * 
 * INPUT:
 *     sizeNew : new size of memory
 * INOUT:
 *     *ppNew : pointer to memory
 * RETURN:
 *     TRUE for success => *ppNew resized
 *     FALSE for no memory => *ppNew not resized (still valid!)
 *
 * USAGE:
 *     called from fc_tools lib to reallocate memory
 */
FC_TOAPI int FC_TODECL FC_LibResizeMemory(void** ppNew, size_t sizeNew);


/* FUNCTIONAL_DESCRIPTION
 *
 * free memory
 * 
 * INPUT:
 *     pMem : memory allocated with FC_NewMemory
 */
FC_TOAPI void FC_TODECL FC_LibFreeMemory(void* pMem);


/* FUNCTIONAL_DESCRIPTION
 *
 * assertions inside the fc_tools lib are made with this function
 * INPUT:
 *     pFile, Line and text
 */
#define FC_LIBASSERT(exp) if(exp); else FC_libassert(__FILE__, __LINE__, #exp)
FC_TOAPI void FC_TODECL FC_libassert(const char* pFile, int iLine, const char* pString);


//old define names:
#ifdef FC_LIB_USE_TODEBUG
#define  FC_LIB_IMPLEM_TODEBUG
#endif
#ifdef FC_LIB_USE_CRT
#define  FC_LIB_IMPLEM_CRT
#endif



//standard implementations:
#ifdef FC_LIB_IMPLEM_TODEBUG    //the fc_todebug.dll is used
#include "fc_todebug\fc_mem.h"
#include "fc_todebug\fc_assert.h"
FC_TOAPI int FC_TODECL FC_LibNewMemory(void** ppNew, size_t sizeNew)
{
    return FC_NewMemory(ppNew, sizeNew);
}
FC_TOAPI int FC_TODECL FC_LibResizeMemory(void** ppNew, size_t sizeNew)
{
    return FC_ResizeMemory(ppNew, sizeNew);
}
FC_TOAPI void FC_TODECL FC_LibFreeMemory(void* pMem)
{
    FC_FreeMemory(pMem);
}
FC_TOAPI void FC_TODECL FC_libassert(const char* pFile, int iLine, const char* pString)
{
    FC_Assert(pFile, iLine, pString);
}
#endif

#endif

#ifdef FC_LIB_IMPLEM_CRT       //the standard C-runtime is used
#include <assert.h>
#include <malloc.h>
FC_TOAPI int FC_TODECL FC_LibNewMemory(void** ppNew, size_t sizeNew)
{
    return (*ppNew = malloc(sizeNew))!=NULL;
}
FC_TOAPI int FC_TODECL FC_LibResizeMemory(void** ppNew, size_t sizeNew)
{
    void* p = realloc(*ppNew, sizeNew);
    if(p)
        *ppNew = p;
    return p!=NULL;
}
FC_TOAPI void FC_TODECL FC_LibFreeMemory(void* pMem)
{
    free(pMem);
}
FC_TOAPI void FC_TODECL FC_libassert(const char* pFile, int iLine, const char* pString)
{
#ifndef NDEBUG
//call the CRT function : _CRTIMP void __cdecl _assert(void *, void *, unsigned);
//directly as defined in <assert.h> but only if DEBUG is on, otherwise it's not declared
//in <assert.h>
    _assert((void*)pString, (void*)pFile, iLine);
#endif
}
#endif


#ifdef __cplusplus
}
#endif




#ifdef __cplusplus
#ifdef FC_LIB_IMPLEM_NEWDEL
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


//static libs that already link to fc_tools.lib 
//should also use FC_libassert, you may use fclibassert macro:
#ifdef DEBUG
  #define  fclibassert(x) if(!(x)) FC_libassert(__FILE__, __LINE__, #x)
#else
  #define  fclibassert(x)
#endif


#endif //FC_NEWDEL_H_




/*
 *----------------------------------------------------------------------------*
 *  $History: fc_libmem.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 28.03.02   Time: 16:46
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * fclibassert 
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 14.03.02   Time: 13:08
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * add new/delete overloads
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 26.02.01   Time: 10:48
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * corrected SWKE name
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 19.02.01   Time: 9:57
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * fixed problem with RELEASE sysgen
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 5.01.01    Time: 17:08
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 * from V1.3 tools.dll, and some new features
 *==
 *----------------------------------------------------------------------------*
*/
