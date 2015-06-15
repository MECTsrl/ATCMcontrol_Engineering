#ifndef FC_MEM_L_H
#define FC_MEM_L_H

/***************************************************************************/
/*    defines                                                              */
/***************************************************************************/
#ifdef DEBUG

#define FC_MEM_INIFILE   _T("debug.ini")
#define SECTION_GAP      _T("Memory")
#define KEY_GAP1         _T("GapSize1")
#define KEY_GAP2         _T("GapSize2")
#define KEY_CHECK_IMM    _T("CheckImmediate")
#define KEY_MOVE_ON_RESIZE  _T("MoveOnResize")
#define KEY_NEW_MEM      _T("NewMem")
#define KEY_FREE_MEM     _T("FreeMem")
#define KEY_GAPBYTE1     _T("FillByte1")
#define KEY_GAPBYTE2     _T("FillByte2")
#define KEY_WARN_MEM     _T("WarnMem")

#define FC_MEM_GAP1          16
#define FC_MEM_GAP2          16

#define FC_MEM_GARBAGE       0xCC
#define FC_MEM_GARBAGE_FREE  0xC7
#define FC_MEM_GARBAGE_START 0xC5
#define FC_MEM_GARBAGE_END   0xC3

#define FC_WARNING_INTERVAL  256	/* issue a warning if too many memory leaks */
#define FC_MAX_CALL_LEVEL    8

/* cast to long is necessary for seg:off */
#define PtrLess(pLeft, pRight)      ((long)(pLeft) <  (long)(pRight))
#define PtrGrtr(pLeft, pRight)      ((long)(pLeft) >  (long)(pRight))
#define PtrEqual(pLeft, pRight)     ((long)(pLeft) == (long)(pRight))
#define PtrLessEq(pLeft, pRight)    ((long)(pLeft) <= (long)(pRight))
#define PtrGrtrEq(pLeft, pRight)    ((long)(pLeft) >= (long)(pRight))

/***************************************************************************/
/*    typedefs                                                             */
/***************************************************************************/
typedef struct FC_BLOCKINFO
  {
  struct FC_BLOCKINFO FAR *lpbiNext;
  struct FC_BLOCKINFO FAR *lpbiPrev;
  LPBYTE        lpb;
  size_t        Size;
  unsigned long ulnAlloc; 
  FARPROC lpCaller[FC_MAX_CALL_LEVEL];	/* for debugging, use a debugger to find the caller */
  }
FC_BlockInfo_TYP;

/***************************************************************************/
/*    prototypes                                                           */
/***************************************************************************/
VOID FC_InitMemorySystem(VOID);
VOID FC_ExitMemorySystem(VOID);
static FC_BlockInfo_TYP FAR * FC_GetBlockInfo(LPBYTE lpb);
static FC_BlockInfo_TYP FAR * FC_GetBlockInfoEx(LPBYTE lpb);
static VOID FC_FillCallArray(FARPROC lpCaller[FC_MAX_CALL_LEVEL]);
static BOOL FC_CreateBlockInfo(LPBYTE lpbNew, size_t SizeNew);
static void FC_FreeBlockInfo(LPBYTE lpbToFree);
static void FC_UpdateBlockInfo(LPBYTE lpbOld, LPBYTE lpbNew, size_t SizeNew);
static size_t FC_SizeOfBlock(LPBYTE lpb);

#endif	// DEBUG

#endif //FC_MEM_L_H

