/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_todebug/fc_mem.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_mem.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_todebug/fc_mem.c $
 *
 * =PROJECT	            CAK1020  ATCMControl V2.0
 *
 * =SWKE                fc
 *
 * =COMPONENT           fc_todebug
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
 *== The implementation of checked malloc free and realloc.
 *   Also the IMalloc can be enabled via a #define INSTALL_MALLOC_SPY
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  02.01.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/


#include <tchar.h>
#include <windows.h>

#include <stdlib.h>
#include <memory.h>

#define FC_DBAPI __declspec(dllexport)

#include "fc_todebug\fc_assert.h"
#include "fc_todebug\fc_mem.h"
#include "fc_mem_l.h"


#ifdef REALLOC_STATISTICS
//static for realloc statistics, also for release
static ULONG ReallocsTotal;
static ULONG ReallocMoved;
#endif


#ifdef DEBUG
//statics  only for debug code:
static FC_BlockInfo_TYP  * lpbiHead = NULL;
static FC_BlockInfo_TYP  * lpbiTail = NULL;
static UINT cnt=0, sum=0;
static unsigned long  g_ulnAlloc = 0;
static unsigned long  g_ulnAllocDebugBreak = 0;
static UINT ToMemGap1 = FC_MEM_GAP1;
static UINT ToMemGap2 = FC_MEM_GAP2;
static UINT CheckGapsImmediate = 0;
static BOOL MoveOnResize = FALSE;
static BYTE ToMemGarbage      = FC_MEM_GARBAGE;
static BYTE ToMemGarbageFree  = FC_MEM_GARBAGE_FREE;
static BYTE ToMemGarbageStart = FC_MEM_GARBAGE_START;
static BYTE ToMemGarbageEnd   = FC_MEM_GARBAGE_END;
static UINT ToWarningInterval = FC_WARNING_INTERVAL;
static UINT nLastCount = 0;
static UINT nCheckCount = 0;

static BOOL   bCriticalSectionInit = FALSE;
static CRITICAL_SECTION csMemoryAllocation;
/* FC_MemoryAPIs may be called before PROCESS_ATTACH 
 * if this happens then we are not yet multi-threaded.
 * so just dont use csMemoryAllocation then!
 */

#ifdef INSTALL_MALLOC_SPY
extern IMallocSpy myMallocSpy;
static IMalloc *pMalloc;
#endif

/*---------------------------------------------------------------------------
 functions
---------------------------------------------------------------------------*/


/***************************************************************************/
VOID FC_InitMemorySystem(VOID)
/***************************************************************************/
{
    assert(!bCriticalSectionInit);
    InitializeCriticalSection(&csMemoryAllocation);
    bCriticalSectionInit = TRUE;
    if (cnt == 0) {
      ToMemGap1 = GetPrivateProfileInt(SECTION_GAP, KEY_GAP1, FC_MEM_GAP1, FC_MEM_INIFILE);
      ToMemGap1 &= ~3;
      ToMemGap2 = GetPrivateProfileInt(SECTION_GAP, KEY_GAP2, FC_MEM_GAP2, FC_MEM_INIFILE);
      ToMemGap2 &= ~3;
      ToMemGarbageStart = (BYTE)GetPrivateProfileInt(SECTION_GAP, KEY_GAPBYTE1, FC_MEM_GARBAGE_START, FC_MEM_INIFILE);
      ToMemGarbageEnd = (BYTE)GetPrivateProfileInt(SECTION_GAP, KEY_GAPBYTE2, FC_MEM_GARBAGE_END, FC_MEM_INIFILE);
    }
    ToMemGarbage = (BYTE)GetPrivateProfileInt(SECTION_GAP, KEY_NEW_MEM, FC_MEM_GARBAGE, FC_MEM_INIFILE);
    ToMemGarbageFree = (BYTE)GetPrivateProfileInt(SECTION_GAP, KEY_FREE_MEM, FC_MEM_GARBAGE_FREE, FC_MEM_INIFILE);

    CheckGapsImmediate = (UINT)GetPrivateProfileInt(SECTION_GAP, KEY_CHECK_IMM, 0,
                                              FC_MEM_INIFILE);
    MoveOnResize = (GetPrivateProfileInt(SECTION_GAP, KEY_MOVE_ON_RESIZE, 0,
                                              FC_MEM_INIFILE) > 0);
    ToWarningInterval = (UINT)GetPrivateProfileInt(SECTION_GAP, KEY_WARN_MEM, FC_WARNING_INTERVAL, FC_MEM_INIFILE);

    FC_DEBUGPRINT4(_T("FC_MEM> CheckGaps: Immediate=%d, MoveOnResize=%d, Gap1=%d Gap2=%d\n"),
      CheckGapsImmediate, MoveOnResize, ToMemGap1, ToMemGap2);
    FC_DEBUGPRINT1(_T("FC_MEM> DLL_PROCESS_ATTACH (cnt=%d)\n"), cnt);

#ifdef INSTALL_MALLOC_SPY
    {
      HRESULT hr;
      hr = CoGetMalloc(1, &pMalloc);
      assert(hr == S_OK && pMalloc!=NULL);
      hr = CoRegisterMallocSpy(&myMallocSpy);
      assert(hr == S_OK);
    }
#endif
}

/***************************************************************************/
VOID FC_ExitMemorySystem(VOID)
/***************************************************************************/
{
    FC_BlockInfo_TYP  * lpbiTmp;

    assert(bCriticalSectionInit);
    DeleteCriticalSection(&csMemoryAllocation);
    bCriticalSectionInit = FALSE;
    FC_DEBUGPRINT3(
        _T("FC_MEM> DLL_PROCESS_DETACH ")
        _T("cnt=%d, sum=%d  first ulnAlloc=%u)\n"),
            cnt, sum, lpbiHead ? lpbiHead->ulnAlloc: 0
    );
#ifdef REALLOC_STATISTICS
    FC_DEBUGPRINT2(_T("FC_MEM> REALLOC_STATISTICS (cnt=%d, mov=%d)\n"), 
        ReallocsTotal, ReallocMoved);
#endif
    lpbiTmp = lpbiTail;
    while (lpbiTmp!=NULL)
    {
        FC_DEBUGPRINT2(_T("FC_MEM> ulnAlloc=%u  size=%i\n"), lpbiTmp->ulnAlloc, lpbiTmp->Size);
        lpbiTmp = lpbiTmp->lpbiPrev;
    }

}

/***************************************************************************/
static void FC_CheckAllGaps(void)
/***************************************************************************/
{
UINT i,n;
FC_BlockInfo_TYP  * lpbi;

if (bCriticalSectionInit)
  EnterCriticalSection(&csMemoryAllocation);

if (++nCheckCount >= CheckGapsImmediate)
{
  nCheckCount = 0;
  for (lpbi = lpbiHead, n=0; lpbi != NULL && n<=cnt; lpbi = lpbi->lpbiNext, n++)
    {
    LPBYTE lpbStart = lpbi->lpb;

    assert(lpbi == (lpbi->lpbiNext == NULL ? lpbiTail : lpbi->lpbiNext->lpbiPrev));
    assert(lpbi == (lpbi->lpbiPrev == NULL ? lpbiHead : lpbi->lpbiPrev->lpbiNext));

    /* check gap */
    for (i=0; i<ToMemGap1; i++)
      assert((lpbStart-ToMemGap1)[i] == ToMemGarbageStart);

    for (i=0; i<ToMemGap2; i++)
      assert((lpbStart+lpbi->Size)[i] == ToMemGarbageEnd);
    }

  assert(n == cnt);
}

if (bCriticalSectionInit)
  LeaveCriticalSection(&csMemoryAllocation);

}
/***************************************************************************/
static FC_BlockInfo_TYP  * FC_GetBlockInfo(LPBYTE lpb)
/***************************************************************************/
{
UINT n;
FC_BlockInfo_TYP  * lpbi;

if (bCriticalSectionInit)
  EnterCriticalSection(&csMemoryAllocation);

for (lpbi = lpbiHead, n=0; lpbi != NULL && n<=cnt; lpbi = lpbi->lpbiNext, n++)
  {
  LPBYTE lpbStart = lpbi->lpb;
  LPBYTE lpbEnd   = lpbi->lpb + lpbi->Size - 1;

  if (PtrGrtrEq(lpb, lpbStart) && PtrLessEq(lpb, lpbEnd))
    break;
  }

assert(n <= cnt);

if (bCriticalSectionInit)
  LeaveCriticalSection(&csMemoryAllocation);

/* Blockinfo not found? -> - garbage 
                           - freed pointer 
                           - moved block during resize */
assert(lpbi != NULL);

return lpbi;
}

/***************************************************************************/
static FC_BlockInfo_TYP  * FC_GetBlockInfoEx(LPBYTE lpb)
/***************************************************************************/
{
/* fast version of FC_GetBlockInfo;
 * but it works only if lpb points to the beginning of a memory block
 */
FC_BlockInfo_TYP  * lpbi;

lpbi = (FC_BlockInfo_TYP  *)(lpb - ToMemGap1 - sizeof(FC_BlockInfo_TYP));

assert(lpbi != NULL);

assert(PtrEqual(lpbi->lpb, lpb));
//assert(lpbi == (lpbi->lpbiNext == NULL ? lpbiTail : lpbi->lpbiNext->lpbiPrev));
//assert(lpbi == (lpbi->lpbiPrev == NULL ? lpbiHead : lpbi->lpbiPrev->lpbiNext));

return lpbi;
}

static VOID FC_FillCallArray(FARPROC lpCaller[FC_MAX_CALL_LEVEL])
{
  lpCaller[0] = NULL;
  lpCaller[1] = NULL;
  lpCaller[2] = NULL;
  lpCaller[3] = NULL;
  lpCaller[4] = NULL;
  lpCaller[5] = NULL;
  lpCaller[6] = NULL;
  lpCaller[7] = NULL;
#ifdef _X86_
  __asm{
    mov ebx,fs:[4]          ; ebx = top of stack
    mov eax,lpCaller
    mov edx,dword ptr[ebp]  ; activation frame of FC_CreateBlockInfo
    mov edx,dword ptr[edx]  ; activation frame of FC_NewMemory
    mov ecx,dword ptr[edx+4]
    mov [eax+0*4],ecx       ; lpCaller[0]

    mov edx,dword ptr[edx]
    cmp  edx,ebp
    jb short done
    cmp  edx,ebx
    jae short done
    mov ecx,dword ptr[edx+4]
    mov [eax+1*4],ecx       ; lpCaller[1]

    mov edx,dword ptr[edx]
    cmp  edx,ebp
    jb short done
    cmp  edx,ebx
    jae short done
    mov ecx,dword ptr[edx+4]
    mov [eax+2*4],ecx       ; lpCaller[2]

    mov edx,dword ptr[edx]
    cmp  edx,ebp
    jb short done
    cmp  edx,ebx
    jae short done
    mov ecx,dword ptr[edx+4]
    mov [eax+3*4],ecx       ; lpCaller[3]

    mov edx,dword ptr[edx]
    cmp  edx,ebp
    jb short done
    cmp  edx,ebx
    jae short done
    mov ecx,dword ptr[edx+4]
    mov [eax+4*4],ecx       ; lpCaller[4]

    mov edx,dword ptr[edx]
    cmp  edx,ebp
    jb short done
    cmp  edx,ebx
    jae short done
    mov ecx,dword ptr[edx+4]
    mov [eax+5*4],ecx       ; lpCaller[5]

    mov edx,dword ptr[edx]
    cmp  edx,ebp
    jb short done
    cmp  edx,ebx
    jae short done
    mov ecx,dword ptr[edx+4]
    mov [eax+6*4],ecx       ; lpCaller[6]

    mov edx,dword ptr[edx]
    cmp  edx,ebp
    jb short done
    cmp  edx,ebx
    jae short done
    mov ecx,dword ptr[edx+4]
    mov [eax+7*4],ecx       ; lpCaller[7]
done:
  }
#else
/* not yet implemented! */
#endif

  return;
}

/***************************************************************************/
static BOOL FC_CreateBlockInfo(LPBYTE lpbNew, size_t SizeNew)
/***************************************************************************/
{
FC_BlockInfo_TYP  * lpbi;

assert((lpbNew != NULL) && (SizeNew != 0));

lpbi = (FC_BlockInfo_TYP  *)(lpbNew - ToMemGap1 - sizeof(FC_BlockInfo_TYP));
if (lpbi != NULL)
  {
  if (bCriticalSectionInit)
    EnterCriticalSection(&csMemoryAllocation);

  lpbi->lpb      = lpbNew;
  lpbi->Size     = SizeNew;
  lpbi->ulnAlloc = ++g_ulnAlloc;
  lpbi->lpbiNext = lpbiHead;
  lpbi->lpbiPrev = NULL;
  if (lpbiHead != NULL)
    lpbiHead->lpbiPrev = lpbi;
  else
    lpbiTail = lpbi;
  lpbiHead = lpbi;

  __try{
    FC_FillCallArray(lpbi->lpCaller);
  }
  __except( GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION
            ? EXCEPTION_EXECUTE_HANDLER
            : EXCEPTION_CONTINUE_SEARCH )
  {
  }

  cnt++;
  sum+=SizeNew;

  if (ToWarningInterval != 0 && !(cnt % ToWarningInterval) && cnt != nLastCount)
     {
       nLastCount = cnt;
       FC_DEBUGPRINT2(_T("FC_MEM> more than %d memory blocks, memory leak? (sum=%d)\n"), cnt, sum);
     }

  if (bCriticalSectionInit)
    LeaveCriticalSection(&csMemoryAllocation);
  }
return (lpbi != NULL);
}

/***************************************************************************/
static void FC_FreeBlockInfo(LPBYTE lpbToFree)
/***************************************************************************/
{
FC_BlockInfo_TYP  * lpbi;

if (bCriticalSectionInit)
  EnterCriticalSection(&csMemoryAllocation);

lpbi = FC_GetBlockInfoEx(lpbToFree);

assert(lpbi == (lpbi->lpbiNext == NULL ? lpbiTail : lpbi->lpbiNext->lpbiPrev));
assert(lpbi == (lpbi->lpbiPrev == NULL ? lpbiHead : lpbi->lpbiPrev->lpbiNext));

if (lpbi->lpbiNext != NULL)
  lpbi->lpbiNext->lpbiPrev = lpbi->lpbiPrev;
else
  lpbiTail = lpbi->lpbiPrev;

if (lpbi->lpbiPrev != NULL)
  lpbi->lpbiPrev->lpbiNext = lpbi->lpbiNext;
else
  lpbiHead = lpbi->lpbiNext;

cnt--;
sum-=lpbi->Size;

if (bCriticalSectionInit)
  LeaveCriticalSection(&csMemoryAllocation);

/* lpbToFree invalid ? */
assert(lpbi != NULL);

/* no text buffer !! */
memset(lpbi, ToMemGarbage, sizeof(FC_BlockInfo_TYP));
}

/***************************************************************************/
static void FC_UpdateBlockInfo(LPBYTE lpbOld, 
                               LPBYTE lpbNew, 
                               size_t SizeNew)
/***************************************************************************/
{
/* assert(this is called in a critical section); */
FC_BlockInfo_TYP  * lpbi;
FC_BlockInfo_TYP  * lpbiPrev;

assert((lpbNew != 0) && (SizeNew != 0));

lpbi = (FC_BlockInfo_TYP  *)(lpbNew - ToMemGap1 - sizeof(FC_BlockInfo_TYP));
lpbiPrev = (FC_BlockInfo_TYP  *)(lpbOld - ToMemGap1 - sizeof(FC_BlockInfo_TYP));

/* lpbOld points to begining of block ? */
assert(lpbOld == lpbi->lpb);
assert(lpbiPrev == (lpbi->lpbiNext == NULL ? lpbiTail : lpbi->lpbiNext->lpbiPrev));
assert(lpbiPrev == (lpbi->lpbiPrev == NULL ? lpbiHead : lpbi->lpbiPrev->lpbiNext));

sum+=SizeNew - lpbi->Size;

lpbi->lpb = lpbNew;
lpbi->Size = SizeNew;
lpbi->ulnAlloc = ++g_ulnAlloc;
if (lpbi->lpbiNext != NULL)
  lpbi->lpbiNext->lpbiPrev = lpbi;
else
  lpbiTail = lpbi;

if (lpbi->lpbiPrev != NULL)
  lpbi->lpbiPrev->lpbiNext = lpbi;
else
  lpbiHead = lpbi;

}

/***************************************************************************/
static size_t FC_SizeOfBlock(LPBYTE lpb)
/***************************************************************************/
{
FC_BlockInfo_TYP  * lpbi;

lpbi = FC_GetBlockInfoEx(lpb);

/* lpbOld points to begining of block ? */
assert(lpb == lpbi->lpb);

return (lpbi->Size);
}

/* endif DEBUG */
#endif



/***************************************************************************/
FC_DBAPI UINT FC_DBDECL FC_GetAllocCount(UINT* psize)
/***************************************************************************/
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
{
#ifdef DEBUG
    if(psize)
        *psize = sum;
    return cnt;
#else
    if(psize)
        *psize = 0;
    return 0;
#endif
}

FC_DBAPI void FC_DBDECL FC_SetAllocBreak(
    unsigned long ulnAlloc
)
{
#ifdef DEBUG
    g_ulnAllocDebugBreak = ulnAlloc;
#endif
}

FC_DBAPI unsigned long FC_DBDECL FC_GetFirstAllocBreak(void)
{
#ifdef DEBUG
    unsigned long ulnAlloc;
    if (bCriticalSectionInit)
       EnterCriticalSection(&csMemoryAllocation);

    if(lpbiHead)
        ulnAlloc = lpbiHead->ulnAlloc;
    else
        ulnAlloc = 0;

   if (bCriticalSectionInit)
      LeaveCriticalSection(&csMemoryAllocation);

   return ulnAlloc;

#else
   return 0;
#endif
}



/***************************************************************************/
FC_DBAPI int FC_DBDECL FC_IsValidBlock(void  * lpv, size_t Size)
/***************************************************************************/
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
{
#ifdef DEBUG
/* function has to exist for beeing exported */
FC_BlockInfo_TYP  * lpbi;
LPBYTE lpb = (LPBYTE)lpv;

assert((lpv != NULL) && (Size != 0));

lpbi = FC_GetBlockInfo(lpb);

assert(PtrLessEq(lpb + Size, lpbi->lpb + lpbi->Size)); 

#endif
return TRUE;
}



/***************************************************************************/
FC_DBAPI int FC_DBDECL FC_NewMemory(
/***************************************************************************/
     void ** lplpNew,
     size_t SizeNew)
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
{
LPBYTE * lplpTmp = (LPBYTE *)lplpNew;

assert((lplpNew != NULL) && (SizeNew != 0));

#ifdef DEBUG
if (CheckGapsImmediate)
    FC_CheckAllGaps();

*lplpTmp = (LPBYTE)HeapAlloc(GetProcessHeap(),0,SizeNew + ToMemGap1 + ToMemGap2 + sizeof(FC_BlockInfo_TYP));
#else
*lplpTmp = (LPBYTE)HeapAlloc(GetProcessHeap(),0,SizeNew);
#endif

#ifdef DEBUG
    /* fill allocated memory */
    if (*lplpTmp != NULL)
      {
      *lplpTmp += ToMemGap1 + sizeof(FC_BlockInfo_TYP);
/* no text buffer !! */
      memset(*lplpTmp - ToMemGap1, ToMemGarbageStart, ToMemGap1);
      memset(*lplpTmp, ToMemGarbage, SizeNew);
      memset(*lplpTmp + SizeNew, ToMemGarbageEnd, ToMemGap2);

      /* if unable to create blockinfo fake a total memory failure */
      if (!FC_CreateBlockInfo(*lplpTmp, SizeNew))
        {
        HeapFree(GetProcessHeap(),0,*lplpTmp - ToMemGap1 - sizeof(FC_BlockInfo_TYP));
        *lplpTmp = NULL;
        }
      }

      if(g_ulnAllocDebugBreak==g_ulnAlloc)
        assert(!"g_ulnAllocDebugBreak==g_ulnAlloc");
#endif

return (*lplpTmp != NULL);
}

/***************************************************************************/
FC_DBAPI int FC_DBDECL FC_ResizeMemory(
/***************************************************************************/
     void ** lplpNew, 
     size_t SizeNew)
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
{
LPBYTE * lplpTmp = (LPBYTE *)lplpNew;
LPBYTE lpTmp;
#ifdef DEBUG
    size_t SizeOld;
    unsigned int i;
#endif

assert((lplpNew != NULL) && (SizeNew != 0));

#ifdef DEBUG
    if (bCriticalSectionInit)
      EnterCriticalSection(&csMemoryAllocation);
#endif

#ifdef DEBUG
    if (CheckGapsImmediate)
       FC_CheckAllGaps();

    SizeOld = FC_SizeOfBlock(*lplpTmp);

    /* check gap */
    for (i=0; i<ToMemGap1; i++)
      assert((*lplpTmp-ToMemGap1)[i] == ToMemGarbageStart);

    for (i=0; i<ToMemGap2; i++)
      assert((*lplpTmp+SizeOld)[i] == ToMemGarbageEnd);
#endif

#ifdef DEBUG
if (MoveOnResize)
{
    // allocate new memory to get a new pointer
    lpTmp = (LPBYTE)HeapAlloc(GetProcessHeap(),0,SizeNew + ToMemGap1 + ToMemGap2 + sizeof(FC_BlockInfo_TYP));
    if (lpTmp != NULL)
    {
        // copy memory to new location
        memcpy(lpTmp, *lplpTmp - ToMemGap1 - sizeof(FC_BlockInfo_TYP),
               min(SizeOld, SizeNew) + ToMemGap1 + ToMemGap2 + sizeof(FC_BlockInfo_TYP));
        // free the old memory block
        memset(*lplpTmp - ToMemGap1 - sizeof(FC_BlockInfo_TYP),
               ToMemGarbageFree, SizeOld + ToMemGap1 + ToMemGap2 + sizeof(FC_BlockInfo_TYP));
        HeapFree(GetProcessHeap(),0,*lplpTmp - ToMemGap1 - sizeof(FC_BlockInfo_TYP));
    }
}
else
{
    /* if shrinking, erase tail contents */
    if (SizeOld > SizeNew)
      {
/* no text buffer !! */
      memset(*lplpTmp + SizeNew, ToMemGarbageFree, SizeOld - SizeNew + ToMemGap2);
      }

    lpTmp = (LPBYTE)HeapReAlloc(GetProcessHeap(),0,*lplpTmp - ToMemGap1 - sizeof(FC_BlockInfo_TYP),
                                SizeNew + ToMemGap1 + ToMemGap2 + sizeof(FC_BlockInfo_TYP));
}

#else
lpTmp = (LPBYTE)HeapReAlloc(GetProcessHeap(),0,*lplpTmp, SizeNew);
#endif

if (lpTmp != NULL)
  {
#ifdef DEBUG
    lpTmp += ToMemGap1 + sizeof(FC_BlockInfo_TYP);

    FC_UpdateBlockInfo(*lplpTmp, lpTmp, SizeNew);
    if(g_ulnAllocDebugBreak==g_ulnAlloc)
        assert(!"g_ulnAllocDebugBreak==g_ulnAlloc");

    /* if expanding, erase the new tail */
    if (SizeNew > SizeOld)
      {
/* no text buffer !! */
      memset(lpTmp + SizeOld, ToMemGarbage, SizeNew - SizeOld);
      }

    /* fill the gap in every case */
    memset(lpTmp + SizeNew, ToMemGarbageEnd, ToMemGap2);
#endif

#ifdef REALLOC_STATISTICS
    InterlockedIncrement(&ReallocsTotal);
    if(*lplpNew != lpTmp)
        InterlockedIncrement(&ReallocMoved);
#endif
  *lplpNew = lpTmp;
  }

#ifdef DEBUG
    if (bCriticalSectionInit)
      LeaveCriticalSection(&csMemoryAllocation);
#endif

return (lpTmp != NULL);
}

/***************************************************************************/
FC_DBAPI void FC_DBDECL FC_FreeMemory(
/***************************************************************************/
     void * lpMem)
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
{
LPBYTE lpTmp = (LPBYTE)lpMem;
#ifdef DEBUG
    size_t SizeOld;
    unsigned int i;
#endif

assert(lpMem != NULL);

#ifdef DEBUG
    if (CheckGapsImmediate)
       FC_CheckAllGaps();

    SizeOld = FC_SizeOfBlock(lpTmp);

    /* check gap */
    for (i=0; i<ToMemGap1; i++)
      assert((lpTmp-ToMemGap1)[i] == ToMemGarbageStart);

    for (i=0; i<ToMemGap2; i++)
      assert((lpTmp+SizeOld)[i] == ToMemGarbageEnd);

    FC_FreeBlockInfo(lpTmp);

    lpTmp -= ToMemGap1 + sizeof(FC_BlockInfo_TYP);
/* no text buffer !! */
    memset(lpTmp, ToMemGarbageFree, SizeOld + ToMemGap1 + ToMemGap2 + sizeof(FC_BlockInfo_TYP));
#endif

HeapFree(GetProcessHeap(),0,lpTmp);
}


#if defined(DEBUG) && defined(INSTALL_MALLOC_SPY)
HRESULT STDMETHODCALLTYPE IMallocSpy_QueryInterface( 
            IMallocSpy __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
{
    if (riid == NULL || ppvObject == NULL || This == NULL)
        return E_POINTER;
    if (IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid, &IID_IMallocSpy))
    {
        This->lpVtbl->AddRef(This);
        *ppvObject = This;
        return S_OK;
    }
    *ppvObject = NULL;
    return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE IMallocSpy_AddRef( 
            IMallocSpy __RPC_FAR * This)
{
    return 1;
}

ULONG STDMETHODCALLTYPE IMallocSpy_Release( 
            IMallocSpy __RPC_FAR * This)
{
    return 1;
}

static void __RPC_FAR *pOrgRequest;
static ULONG cbOrgRequest;

ULONG STDMETHODCALLTYPE IMallocSpy_PreAlloc( 
            IMallocSpy __RPC_FAR * This,
            /* [in] */ ULONG cbRequest)
{
    cbOrgRequest = cbRequest;
    return 1;
}

void __RPC_FAR *STDMETHODCALLTYPE IMallocSpy_PostAlloc( 
            IMallocSpy __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *pActual)
{
    pMalloc->lpVtbl->Free(pMalloc, pActual);
    if (!cbOrgRequest)
         cbOrgRequest++;
    FC_NewMemory(&pActual, cbOrgRequest);
    return pActual;
}

void __RPC_FAR *STDMETHODCALLTYPE IMallocSpy_PreFree( 
            IMallocSpy __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *pRequest,
            /* [in] */ BOOL fSpyed)
{
    if (fSpyed && pRequest!=NULL) {
        FC_FreeMemory(pRequest);
        return NULL;
    }
    assert(pRequest == NULL || pMalloc->lpVtbl->DidAlloc(pMalloc, pRequest) == 1);
    return pRequest;
}

void STDMETHODCALLTYPE IMallocSpy_PostFree( 
            IMallocSpy __RPC_FAR * This,
            /* [in] */ BOOL fSpyed)
{
    return ;
}

ULONG STDMETHODCALLTYPE IMallocSpy_PreRealloc( 
            IMallocSpy __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *pRequest,
            /* [in] */ ULONG cbRequest,
            /* [out] */ void __RPC_FAR *__RPC_FAR *ppNewRequest,
            /* [in] */ BOOL fSpyed)
{
    cbOrgRequest = cbRequest;
    pOrgRequest = pRequest;

    *ppNewRequest = NULL;
    return 1;
}

void __RPC_FAR *STDMETHODCALLTYPE IMallocSpy_PostRealloc( 
            IMallocSpy __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *pActual,
            /* [in] */ BOOL fSpyed)
{
    pMalloc->lpVtbl->Free(pMalloc, pActual);
    if (pOrgRequest == NULL) {
        if (!cbOrgRequest)
           cbOrgRequest++;
        FC_NewMemory(&pActual, cbOrgRequest);
    }
    else if (cbOrgRequest == 0) {
        if (fSpyed)
            FC_FreeMemory(pOrgRequest);
        else
            pMalloc->lpVtbl->Free(pMalloc, pOrgRequest);
        pActual = NULL;
    }
    if (fSpyed) {
        pActual = pOrgRequest;
        if (!FC_ResizeMemory(&pActual, cbOrgRequest))
          pActual = NULL;
    }
    else //if (!fSpyed)
    {
        if (FC_NewMemory(&pActual, cbOrgRequest))
        {
          ULONG orgSize = pMalloc->lpVtbl->GetSize(pMalloc, pOrgRequest);

          if (orgSize == -1)
            orgSize = 0;
          if (cbOrgRequest < orgSize)
            orgSize = cbOrgRequest;
          memcpy(pActual, pOrgRequest, orgSize);
          pMalloc->lpVtbl->Free(pMalloc, pOrgRequest);
        }
    }
    return pActual;
}

void __RPC_FAR *STDMETHODCALLTYPE IMallocSpy_PreGetSize( 
            IMallocSpy __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *pRequest,
            /* [in] */ BOOL fSpyed)
{
    if (fSpyed) {
        pOrgRequest = pRequest;
        return NULL;
    }
    return pRequest;
}

ULONG STDMETHODCALLTYPE IMallocSpy_PostGetSize( 
            IMallocSpy __RPC_FAR * This,
            /* [in] */ ULONG cbActual,
            /* [in] */ BOOL fSpyed)
{
    if (fSpyed) {
        return FC_SizeOfBlock(pOrgRequest);
    }
    return cbActual;
}

void __RPC_FAR *STDMETHODCALLTYPE IMallocSpy_PreDidAlloc( 
            IMallocSpy __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *pRequest,
            /* [in] */ BOOL fSpyed)
{
    if (fSpyed) {
        assert(FC_IsValidBlock(pRequest, FC_SizeOfBlock(pRequest)));
        return NULL;
    }
    return pRequest;
}

int STDMETHODCALLTYPE IMallocSpy_PostDidAlloc( 
            IMallocSpy __RPC_FAR * This,
            /* [in] */ void __RPC_FAR *pRequest,
            /* [in] */ BOOL fSpyed,
            /* [in] */ int fActual)
{
    if (fSpyed) {
        return 1;
    }
    return fActual;
}

void STDMETHODCALLTYPE IMallocSpy_PreHeapMinimize( 
            IMallocSpy __RPC_FAR * This)
{
    return ;
}

void STDMETHODCALLTYPE IMallocSpy_PostHeapMinimize( 
            IMallocSpy __RPC_FAR * This)
{
    return ;
}

static IMallocSpyVtbl myMallocSpyVtbl = {
    IMallocSpy_QueryInterface,
    IMallocSpy_AddRef,
    IMallocSpy_Release,
    IMallocSpy_PreAlloc,
    IMallocSpy_PostAlloc,
    IMallocSpy_PreFree,
    IMallocSpy_PostFree,
    IMallocSpy_PreRealloc,
    IMallocSpy_PostRealloc,
    IMallocSpy_PreGetSize,
    IMallocSpy_PostGetSize,
    IMallocSpy_PreDidAlloc,
    IMallocSpy_PostDidAlloc,
    IMallocSpy_PreHeapMinimize,
    IMallocSpy_PostHeapMinimize
};
static IMallocSpy myMallocSpy = {&myMallocSpyVtbl};
#endif // INSTALL_MALLOC_SPY
