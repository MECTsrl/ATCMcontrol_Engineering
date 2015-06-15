/* $Header: /4CReleased/V2.20.00/Target/4CWin/Control/osShared/osUtil.c 2     3.08.07 15:01 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: osUtil.c $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/Control/osShared/osUtil.c $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				RunTime
 *
 * =COMPONENT			osShared
 *
 * =CURRENT 	 $Date: 3.08.07 15:01 $
 *			 $Revision: 2 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Local Defines:   ----------------------------------------------------- */

#define __4CFILE__	"osUtil.c"

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdInc.h"

/* ----  Target Specific Includes:	 ------------------------------------------ */

#include <windows.h>

/* ----  Local Defines:   ----------------------------------------------------- */

#define VM_MAGIC	0xA5A5u

/* ----  Global Variables:	 -------------------------------------------------- */

#if defined (RTS_CFG_MEM_TRACE)
  static IEC_DINT g_lMemAlloc  = 0;
  static IEC_DINT g_lMemObject = 0;
#endif

#if defined(_SOF_4CFC_SRC_)
  extern unsigned long *g_jiffies_ptr;
#endif

/* ----  Local Functions:	--------------------------------------------------- */

/* ----  Implementations:	--------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/**
 * osTrace
 *
 * Prints the given debug message to an appropriate device.
 *
 * @return			OK if successful else error number.
 */
#if defined(RTS_CFG_DEBUG_OUTPUT)

IEC_UINT osTrace(IEC_CHAR *szFormat, ...)
{
	va_list 	va;
	va_start(va, szFormat);
	vprintf(szFormat, va); 
	va_end(va);

  #if defined(RTS_CFG_DEBUG_FILE)
	{
		IEC_UDINT	hF;
		IEC_UINT	uRes;
		IEC_CHAR	szFile[VMM_MAX_PATH + 1];

		static	IEC_UINT uLocked = 0;

		if (uLocked != 0)
		{
			return OK;
		}

		uLocked++;

		uRes = utilOpenFile(&hF, (IEC_CHAR *)szFile, VMM_MAX_PATH, osGetTraceDir, VMM_DIR_TRACE, VMM_FILE_TRACE, FIO_MODE_APPEND);
		if (uRes == OK)
		{
			va_start(va, szFormat);
			vfprintf((FILE *)hF, szFormat, va); 
			va_end(va);
  
			xxxClose(hF,FALSE);
		}

		uLocked--;
	}
  #endif

	return OK;
}
#endif /* RTS_CFG_DEBUG_OUTPUT */


/* ---------------------------------------------------------------------------- */
/**
 * osSleep			
 *
 * Suspend the task for the given time (in ms).
 *
 * @param			ulTime		Suspend time in ms.
 * @return			OK if successful else error number.
 */
IEC_UINT osSleep(IEC_UDINT ulTime)
{
	IEC_UINT uRes = OK;

	SleepEx(ulTime, TRUE);
	
	RETURN(uRes);
}

/* ---------------------------------------------------------------------------- */
/**
 * osGetTime32
 *
 * @return			Current time as 32 bit value in milliseconds.
 */
IEC_UDINT osGetTime32(void)
{

	return (IEC_UDINT)osGetTime64();
}

/* ---------------------------------------------------------------------------- */
/**
 * osGetTime64
 *
 * @return			Current time as 64 bit value in milliseconds.
 */
IEC_ULINT osGetTime64(void)
{
	LARGE_INTEGER pc;

	if (QueryPerformanceCounter (&pc))
	{
		static BOOL 		 pf_valid = FALSE;
		static LARGE_INTEGER pf;

		if (! pf_valid)
		{
			pf_valid = QueryPerformanceFrequency (&pf);
		}

		if (pf_valid && (pf.LowPart | pf.HighPart) != 0)
		{
			return (*(__int64 *)&pc * (__int64)1000) / *(__int64 *)&pf;
		}
	}

	return GetTickCount();
}

/* ---------------------------------------------------------------------------- */
/**
 * osGetTimeUS
 *
 * @return			Current time as 64 bit value in microseconds.
 */
IEC_ULINT osGetTimeUS(void)
{
	LARGE_INTEGER pc;

	if (QueryPerformanceCounter (&pc))
	{
		static BOOL 		 pf_valid = FALSE;
		static LARGE_INTEGER pf;

		if (! pf_valid)
		{
			pf_valid = QueryPerformanceFrequency (&pf);
		}

		if (pf_valid && (pf.LowPart | pf.HighPart) != 0)
		{
			return (*(__int64 *)&pc * (__int64)1000 * (__int64)1000) / *(__int64 *)&pf;
		}
	}

	return (__int64)GetTickCount() * (__int64)1000;
}

/* ---------------------------------------------------------------------------- */
/**
 * osGetTime32Ex
 *
 * @return			Current time as 32 bit value in milliseconds.
 */
IEC_UDINT osGetTime32Ex(void)
{

	return osGetTime32();
}

/* ---------------------------------------------------------------------------- */
/**
 * osGetTime64
 *
 * @return			Current time as 64 bit value in milliseconds.
 */
IEC_ULINT osGetTime64Ex(void)
{

	return osGetTime64();
}

/* ---------------------------------------------------------------------------- */
/**
 * osGetTimeUS
 *
 * @return			Current time as 64 bit value in microseconds.
 */
IEC_ULINT osGetTimeUSEx(void)
{
	
	return osGetTimeUS();
}

/* ---------------------------------------------------------------------------- */
/**
 * osMalloc
 *
 * Allocates an memory object within the given Segment.
 *
 * @return			OK if successful else error number.
 */
IEC_DATA OS_LPTR *osMalloc(IEC_UDINT ulSize)
{
	IEC_DATA OS_LPTR *pRet	= NULL;

  #if defined(RTS_CFG_MEMORY_CHECK)

	IEC_UINT uMagic = VM_MAGIC;

	#if defined(IP_CFG_INST32) || defined(IP_CFG_INST64)
	  IEC_UINT uAli = 2;
	#else
	  IEC_UINT uAli = 0;
	#endif

	IEC_DATA OS_LPTR *pData = OS_MALLOC(sizeof(uMagic) + uAli + sizeof(ulSize) + ulSize + sizeof(uMagic));

	if (pData == NULL)
	{
	  #if defined(RTS_CFG_DEBUG_OUTPUT)
		osTrace("\r\n*** [M E M O R Y   E R R O R]: Memory allocation (%ld bytes) failed!\r\n", ulSize);
	  #endif
		return NULL;
	}

	pRet = pData + sizeof(uMagic) + uAli + sizeof(ulSize);
	pRet = OS_NORMALIZEPTR(pRet);

	*(IEC_UINT *)pData = (IEC_UINT)(uMagic ^ (IEC_UINT)ulSize);
	pData += sizeof(uMagic) + uAli;

	*(IEC_UDINT *)pData = ulSize;
	pData += sizeof(ulSize) + ulSize;

	*(IEC_UINT *)pData = (IEC_UINT)(uMagic ^ ~(IEC_UINT)ulSize);

  #if defined (RTS_CFG_MEM_TRACE)
	g_lMemObject++;
	g_lMemAlloc += sizeof(uMagic) + uAli + sizeof(ulSize) + ulSize + sizeof(uMagic);
  #if defined(RTS_CFG_DEBUG_OUTPUT)
	osTrace("--- Alloc    %7ld   Bytes at 0x%08lx (%02ld/%05ld)\r\n", ulSize, pRet, g_lMemObject, g_lMemAlloc / 1024);
  #endif
  #endif

	return pRet;

  #else

	pRet = (IEC_DATA OS_LPTR *)OS_MALLOC(ulSize);
	pRet = OS_NORMALIZEPTR(pRet);

	return pRet;

  #endif
}

/* ---------------------------------------------------------------------------- */
/**
 * osFree
 *
 * Frees a memory object.
 *
 * @return			OK if successful else error number.
 */
IEC_UINT osFree(IEC_DATA OS_LPTR **ppData)
{
	IEC_UINT uRes = OK;

  #if defined(RTS_CFG_MEMORY_CHECK)

	#if defined(IP_CFG_INST32) || defined(IP_CFG_INST64)
	  IEC_UINT uAli = 2;
	#else
	  IEC_UINT uAli = 0;
	#endif

	IEC_UINT  uMagic = 0;
	IEC_UDINT ulSize = 0;
	IEC_DATA OS_LPTR *pData = NULL;
	IEC_DATA OS_LPTR *pEnd	= NULL;

	if (ppData == NULL || *ppData == NULL)
	{
		RETURN(OK);
	}

	pData	= *ppData;

	pData  -= sizeof(ulSize);
	ulSize	= *(IEC_UDINT *)pData;

	pData  -= uAli + sizeof(uMagic);
	uMagic	= *(IEC_UINT *)pData;

	if (uMagic != (IEC_UINT)(VM_MAGIC ^ (IEC_UINT)ulSize))
	{
	  #if defined(RTS_CFG_DEBUG_OUTPUT)
		osTrace("\r\n*** [M E M O R Y   E R R O R]: Memory overwriting at 0x%08lx (begin) detected!\r\n", *ppData);
	  #endif
		*ppData = NULL;
		RETURN(ERR_ERROR);
	}

	pEnd = pData + sizeof(uMagic) + uAli + sizeof(ulSize) + ulSize;
	uMagic = *(IEC_UINT *)pEnd;

	if (uMagic != (IEC_UINT)(VM_MAGIC ^ ~(IEC_UINT)ulSize))
	{
	  #if defined(RTS_CFG_DEBUG_OUTPUT)
		osTrace("\r\n*** [M E M O R Y   E R R O R]: Memory overwriting at 0x%08lx (end) detected!\r\n", *ppData);
	  #endif
		*ppData = NULL;
		RETURN(ERR_ERROR);
	}

  #if defined (RTS_CFG_MEM_TRACE)
	g_lMemObject--;
	g_lMemAlloc -= sizeof(uMagic) + uAli + sizeof(ulSize) + ulSize + sizeof(uMagic);
  #if defined(RTS_CFG_DEBUG_OUTPUT)
	osTrace("--- Free     %7ld   Bytes at 0x%08lx (%02ld/%05ld)\r\n", ulSize, *ppData, g_lMemObject, g_lMemAlloc / 1024);
  #endif
  #endif

	OS_FREE(pData);
	*ppData = NULL;

	RETURN(uRes);

  #else

	OS_FREE(*ppData);
	*ppData = NULL;
	
	RETURN(uRes);

  #endif
}

/* ---------------------------------------------------------------------------- */
