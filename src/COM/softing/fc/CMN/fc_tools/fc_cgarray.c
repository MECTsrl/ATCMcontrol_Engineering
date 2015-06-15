/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_cgarray.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_cgarray.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_cgarray.c $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT             $Date: 28.02.07 19:00 $
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
 *==Implementation of GArray memory allocation
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  08.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/



#include <windows.h> //WORD, DWORD
#include <stddef.h> //offsetof

#include "fc_tools\fc_libmem.h"
#define FC_GARRAY_IMPL_C_ //define to get the C-declarations
#include "fc_tools\fc_cgarray.h"


static FC_GArrayTYP g_GArray_Empty = {0,0,0,"\0 !empty array"};

FC_TOAPI FC_GArrayTYP* FC_TODECL FC_GArrayNew(
    size_t   elemSize,     //sizeof(type) size of one element
    unsigned uiPreAllocLen
)
{
    FC_GArrayTYP* pa;
    size_t        size;

    FC_LIBASSERT(elemSize>0);
    if(uiPreAllocLen>0)
    {
        size = offsetof(FC_GArrayTYP, data) + uiPreAllocLen*elemSize;

        if(FC_LibNewMemory((void**)&pa, size))
        {
            pa->umax = uiPreAllocLen;
            pa->used = 0;
        }
        else
        {
            pa = &g_GArray_Empty;
        }
    }
    else
    {
        pa = &g_GArray_Empty;
    }

    return pa;
}
FC_TOAPI FC_GArrayTYP* FC_TODECL FC_GArrayFree(FC_GArrayTYP* pga)
{
    if(pga!=&g_GArray_Empty)
        FC_LibFreeMemory(pga);
    return &g_GArray_Empty;
}
FC_TOAPI int FC_TODECL FC_GArrayResize(
    size_t         elemSize, //sizeof one element
    FC_GArrayTYP** ppga, 
    unsigned       umax
)
{
    unsigned    nDataBytes;

    FC_LIBASSERT(elemSize>0 && (*ppga)->used<=(*ppga)->umax);//assert basics:

    //don't want get smaller than used portion:
    if(umax<(*ppga)->used)
    {
        FC_LIBASSERT("FC_GArrayResize: attempt to resize smaller than used portion");
        umax = (*ppga)->used;
    }

    nDataBytes = umax*elemSize;
    if(nDataBytes>0)
    {
        nDataBytes += offsetof(FC_GArrayTYP, data);

        if((*ppga) != &g_GArray_Empty)
        {
            if(!FC_LibResizeMemory(ppga, nDataBytes))
                return 0;
        }
        else
        {
            if(!FC_LibNewMemory(ppga, nDataBytes))
                return 0;
            (*ppga)->used = 0;
        }

        (*ppga)->umax = umax;
    }
    else if((*ppga) != &g_GArray_Empty)
    {
        FC_LibFreeMemory(*ppga);
        (*ppga) = &g_GArray_Empty;
    }
    else
    {
        FC_LIBASSERT((*ppga)->umax==0 && (*ppga)->used==0);
    }

    return 1;
}

__inline unsigned GrowStrategy(
    size_t   umax //have allocated elems
)
{
    unsigned umaxNew = umax;

    if(umax<16)
    {
        umaxNew++;
    }
    else
    {
        umaxNew +=  (unsigned)(umax * 0.3);
    }
    return umaxNew;
}


FC_TOAPI int  FC_TODECL FC_GArrayAddElem(
    size_t         elemSize, //sizeof one element
    FC_GArrayTYP** ppga,
    const void*    pElem
)
{
    void*    pa;
    unsigned umaxNew;

    if((*ppga)->used==(*ppga)->umax)
    {
        umaxNew = GrowStrategy((*ppga)->umax);
        if(!FC_GArrayResize(elemSize, ppga, umaxNew))
             return -1;
    }
    else
    {
        FC_LIBASSERT((*ppga)->used<(*ppga)->umax);
    }

    pa = (*ppga)->data + (*ppga)->used * elemSize; 

    if(pElem)
    {
        switch(elemSize)
        {
        case 1: *((char*)pa) =  *((char*)pElem);
        break;
        case 2: *((WORD*)pa) =  *((WORD*)pElem);
        break;
        case 4: *((DWORD*)pa) =  *((DWORD*)pElem);
        break;
        case 8: ((DWORD*)pa)[0] = ((DWORD*)pElem)[0];
                ((DWORD*)pa)[1] = ((DWORD*)pElem)[1];
        break;
        default:
             memcpy(pa, pElem, elemSize);
        }
    }
    else
    {
         memset(pa, 0, elemSize);
    }

    return (*ppga)->used++;
}

FC_TOAPI void* FC_TODECL FC_GArrayGetMemCopy(
    size_t         elemSize,
    FC_GArrayTYP** ppga
)
{
    void*     pc;
    unsigned  nDataBytes;

    nDataBytes = (*ppga)->used * elemSize;

    if(nDataBytes==0)
        return NULL;

    FC_LIBASSERT(*ppga!=&g_GArray_Empty);

    if(!FC_LibNewMemory(&pc, nDataBytes))
        return NULL;

    return memcpy(pc, (*ppga)->data, nDataBytes);
}

FC_TOAPI void* FC_TODECL FC_GArrayGetMem(
    size_t         elemSize,
    FC_GArrayTYP** ppga
)
{
    void*     pc;
    unsigned  nDataBytes;

    nDataBytes = (*ppga)->used * elemSize;

    if(nDataBytes==0)
        return NULL;

    FC_LIBASSERT(*ppga!=&g_GArray_Empty);

    
    memmove(*ppga, (*ppga)->data, nDataBytes);

    if(!FC_LibResizeMemory(ppga, nDataBytes))
        return NULL;

    pc = *ppga;
    *ppga = &g_GArray_Empty;
    return pc;
}

















FC_TOAPI void  FC_TODECL FC_StrListFree(
    TCHAR*const  ppsz[], 
    unsigned     uisize 
)
{
    unsigned     ui;

    for(ui=0; ui<uisize; ui++)
        FC_LibFreeMemory(ppsz[ui]);
}

FC_TOAPI int FC_TODECL FC_StrListAddStrDup(
    FC_GArrayTYP** ppga,
    const TCHAR*   psz
)
{
    size_t   size;
    void**   pa;
    unsigned umaxNew;

    if((*ppga)->used==(*ppga)->umax)
    {
        umaxNew = GrowStrategy((*ppga)->umax);
        if(!FC_GArrayResize(sizeof(TCHAR*), ppga, umaxNew))
             return -1;
    }
    else
    {
        FC_LIBASSERT((*ppga)->used<(*ppga)->umax);
    }

    //get pointer to last TCHAR* elem:
    pa = (void**)((*ppga)->data + (*ppga)->used * sizeof(TCHAR*));

    //create copy of psz:
    size = (_tcslen(psz)+1)*sizeof(TCHAR);
    if(!FC_LibNewMemory(pa,  size))
        return -1;
    memcpy(*pa, psz, size);
    return (*ppga)->used++;
}


FC_TOAPI int  FC_TODECL  FC_StrListAppendList(
    FC_GArrayTYP**     ppga, //<type> must be TCHAR*
    const TCHAR*const  ppszadd[/*uiadd*/], 
    unsigned           uiadd
)
{
    unsigned     ui;
    size_t       size;
    void**       pa;

    if(!FC_GArrayResize(sizeof(TCHAR*), ppga, (*ppga)->used + uiadd))
        return 0;

    for(ui=0; ui<uiadd; ui++)
    {
        //get pointer to current last TCHAR* elem:
        pa = (void**)((*ppga)->data + (*ppga)->used * sizeof(TCHAR*));
        
        //create copy of psz:
        size = (_tcslen(ppszadd[ui])+1)*sizeof(TCHAR);
        if(!FC_LibNewMemory(pa, size))
            return 0;
        memcpy(*pa, ppszadd[ui], size);
        (*ppga)->used++;
    }
    return 1;
}


FC_TOAPI int  FC_TODECL FC_StrListFind(
    const TCHAR*const  ppsz[], 
    unsigned           uisize, 
    const TCHAR*       pszSearch 
)
{
    unsigned     ui;
    for(ui=0; ui<uisize; ui++)
    {
        if(!_tcscmp(ppsz[ui], pszSearch))
            return ui;
    }
    return -1;
}
FC_TOAPI int  FC_TODECL FC_StrListFindI(
    const TCHAR*const  ppsz[], 
    unsigned           uisize, 
    const TCHAR*       pszSearch 
)
{
    unsigned     ui;
    for(ui=0; ui<uisize; ui++)
    {
        if(!_tcsicmp(ppsz[ui], pszSearch))
            return ui;
    }
    return -1;
}


static int ST_Comp(const void* x, const void* y)
{
    const TCHAR** px = (const TCHAR**)x;
    const TCHAR** py = (const TCHAR**)y;
    return _tcscmp(*px, *py);
}
FC_TOAPI void FC_TODECL FC_StrListQsort(
    TCHAR*       ppsz[], 
    unsigned     uisize 
)
{
    if(uisize>1)
    {
        qsort(ppsz, uisize, sizeof(TCHAR*), ST_Comp);
    }
}


static int ST_CompI(const void* x, const void* y)
{
    const TCHAR** px = (const TCHAR**)x;
    const TCHAR** py = (const TCHAR**)y;
    return _tcsicmp(*px, *py);
}
FC_TOAPI void  FC_TODECL FC_StrListQsortI(
    TCHAR*       ppsz[], 
    unsigned     uisize 
)
{
    if(uisize>1)
    {
        qsort(ppsz, uisize, sizeof(TCHAR*), ST_CompI);
    }
}



/*
 *----------------------------------------------------------------------------*
 *  $History: fc_cgarray.c $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 9  *****************
 * User: Ef           Date: 24.03.02   Time: 15:47
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * removed wrong assertion
 * 
 * *****************  Version 8  *****************
 * User: Ef           Date: 11.03.02   Time: 15:08
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * removed dead code
 * 
 * *****************  Version 7  *****************
 * User: Die          Date: 30.11.01   Time: 18:25
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * fixed bug in getMem
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 26.02.01   Time: 10:48
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * corrected SWKE name
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 12.02.01   Time: 14:24
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * method getMem
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 29.01.01   Time: 21:12
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * new function addZero
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 22.01.01   Time: 11:08
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * renamed file 
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 8.01.01    Time: 10:40
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * added some file headers
 *==
 *----------------------------------------------------------------------------*
*/
