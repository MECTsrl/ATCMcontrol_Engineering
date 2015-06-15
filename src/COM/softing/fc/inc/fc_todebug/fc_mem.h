/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_todebug/fc_mem.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_mem.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_todebug/fc_mem.h $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                fc
 *
 * =COMPONENT           fc_todebug
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
 *== Checked memory allocation interface 
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  02.01.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/
#ifndef FC_MEM_H_
#define FC_MEM_H_

#include <stddef.h> //size_t

#ifndef FC_DBAPI
#define FC_DBAPI __declspec(dllimport)
#endif
#define FC_DBDECL _stdcall


#ifdef __cplusplus
extern "C" {
#endif

/* FUNCTIONAL_DESCRIPTION
 *
 * allocate memory
 * 
 * INPUT:
 *     SizeNew : size of memory
 *
 * OUTPUT:
 *     *lplpNew : pointer to new memory
 *
 * RETURN:
 *     TRUE for success => *lplpNew!=NULL
 *     FALSE for no memory => *lplpNew == NULL
 *
 * USAGE:
 *   anytype *pMem;
 *   if (FC_NewMemory(&pMem, sizeof(anytype)))
 *     {
 *       do something with pMem;
 *       FC_FreeMemory(pMem);
 *     }
 */
FC_DBAPI int FC_DBDECL FC_NewMemory(void ** lplpNew, size_t SizeNew);


/* FUNCTIONAL_DESCRIPTION
 *
 * reallocate memory
 * 
 * INPUT:
 *     SizeNew : new size of memory
 *
 * INOUT:
 *     *lplpNew : pointer to memory
 *
 * RETURN:
 *     TRUE for success => *lplpNew resized
 *     FALSE for no memory => *lplpNew not resized (still valid!)
 */
FC_DBAPI int FC_DBDECL FC_ResizeMemory(void ** lplpNew, size_t SizeNew);


/* FUNCTIONAL_DESCRIPTION
 *
 * free memory
 * 
 * INPUT:
 *     lpMem : memory allocated with FC_NewMemory
 *
 * OUTPUT:
 *
 * RETURN:
 *
 */
FC_DBAPI void FC_DBDECL FC_FreeMemory(void * lpMem);


/* FUNCTIONAL_DESCRIPTION
 *
 * assert valid  memory
 * 
 * INPUT:
 *     lpv, Size : memory allocated with FC_NewMemory
 *
 * OUTPUT:
 *
 * RETURN:
 *     always TRUE, this function issues an 
 *     assertion only in the DEBUG-Version
 * 
 */
FC_DBAPI int FC_DBDECL FC_IsValidBlock(void * lpv, size_t Size);



/* FUNCTIONAL_DESCRIPTION
 *
 * returns the global object count and the total bytes.
 * 
 * INPUT:
 *     psize : UINT pointer to receive the currently allocatey bytes
 *             may be NULL, if relesae return 0.
 *
 * RETURN:
 *     the number of currently allocated objects, if relesae: return 0
 */
FC_DBAPI unsigned FC_DBDECL FC_GetAllocCount(unsigned* psize);


/* FUNCTIONAL_DESCRIPTION
 *
 * if the allocation with ordinal number ulnAlloc happens 
 * force an  assert(!"g_ulnAllocDebugBreak==g_ulnAlloc")
 * useful to find memory leaks.
 * @param    ulnAlloc:  the allocation number >0 seen 
 *           when exit memory system in the debug view
 * @return   void
 */
FC_DBAPI void FC_DBDECL FC_SetAllocBreak(
    unsigned long ulnAlloc
);

/* FC_GetFirstAllocBreak: returns the allocation count of
 * the currently first element in the global heap lists. 
 * Typically called before process end to detect memory 
 * leaks. 
 * If returns >0 there is a leak the returned number 
 * can be passed to FC_SetAllocBreak on next process
 * start. If the process memory cunsume is reproducable
 * this may help, if not then not.
 *
 * @return   current first heap list element's alloc count.
 */

FC_DBAPI unsigned long FC_DBDECL FC_GetFirstAllocBreak(void);


#ifdef DEBUG
#define FC_IsValidPointer(lpv, Size) FC_IsValidBlock(lpv, Size)
#else
#define FC_IsValidPointer(lpv, Size) TRUE
#endif

#ifdef __cplusplus
}
#endif

#endif //FC_MEM_H_

/*
 *----------------------------------------------------------------------------*
 *  $History: fc_mem.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc/fc_todebug
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc/fc_todebug
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 26.02.01   Time: 10:28
 * Updated in $/4Control/COM/softing/fc/inc/fc_todebug
 * implemented "alloc break"
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 12.01.01   Time: 17:12
 * Updated in $/4Control/COM/softing/fc/inc/fc_todebug
 * new function get cnt and sum
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 5.01.01    Time: 10:09
 * Updated in $/4Control/COM/softing/fc/inc/fc_todebug
 * renamed global defines FC_API -> FC_DBAPI
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 3.01.01    Time: 11:04
 * Updated in $/4Control/COM/softing/fc/inc/fc_todebug
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 2.01.01    Time: 18:15
 * Created in $/4Control/COM/softing/fc/inc/fc_todebug
 * taken and modified from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
