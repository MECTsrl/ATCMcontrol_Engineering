/* $Header: /4CReleased/V2.20.00/TargetBase/inc/vmSharedDef.h 1     1.03.07 12:35 Gen4c $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: vmSharedDef.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/inc/vmSharedDef.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			NCC
 *
 * =CURRENT 	 $Date: 1.03.07 12:35 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _VMSHAREDDEF_H_
#define _VMSHAREDDEF_H_

/* vmSharedDef.h
 * ----------------------------------------------------------------------------
 * This file is just a wrapper file to include the vmShared.h on the Engineering
 * side. This file is NOT target dependent and must not therefore not be changed
 * for an adaptation.
 */


/* Common Heap Allocation
 * ----------------------------------------------------------------------------
 */

#define HEAP_FREE(p,l)		{											\
								if (p != NULL)							\
								{										\
									HeapFree(GetProcessHeap(), 0, p);	\
								}										\
								p = NULL;								\
								l = 0;									\
							}

#define HEAP_ALLOC(p,l)		{													\
								p =	(BYTE *)HeapAlloc(GetProcessHeap(), 0, l);	\
								ASSERT(p);										\
							}

#define HEAP_RE_ALLOC(p,l)	{														\
								p =	(BYTE *)HeapReAlloc(GetProcessHeap(), 0, p, l);	\
								ASSERT(p);											\
							}


#pragma warning(disable : 4200) /* nonstandard extension used : nameless struct/union */


/* The following definitions are used to set the correct little / big endian
 * behavior for the entire AddOn.
 * Do not change the names of these definitions! The values are set by MakeTarget.
 */
#define BIG                 1
#define LITTLE              0

/* The following definitions are just dummy values for the shared definitions
 * to be able to compile. Don't use these values in an Engineering component.
 * Target specific stuff must be defined in the target-VM KAD file.
 */
#define MAX_DATA				   1u	/* Dummy						*/
#define MAX_TASKNAME			  33u	/* Max. task name length		*/
#define MAX_INFO_SHORT			  32u
#define MAX_INFO_LONG			 100u
#define VMM_MAX_DBI_FILE		  49u	/* obsolete for V2.1			*/


#define DEC_VAR(type, var)      type   var
#define DEC_PTR(type, var)      type * var

#define IEC_BYTE     unsigned char     /*  8 bit unsigned integer       */
#define IEC_INT      short             /* 16 bit signed integer         */
#define IEC_UINT     unsigned short    /* 16 bit unsigned integer       */
#define IEC_DINT     long              /* 32 bit signed integer         */
#define IEC_UDINT    unsigned long     /* 32 bit unsigned integer       */
#define IEC_DATA     unsigned char     /* Binary data                   */
#define IEC_CHAR     char	           /* Binary data                   */
#define IEC_USINT	 unsigned char	   /* 8 bit unsigned				*/
#define IEC_LREAL	 double			   /* 8 bit unsigned				*/
#define IEC_WORD	 unsigned short	   /* 16 bit signed					*/
#define IEC_BOOL	 unsigned char	   /* 8 bit unsigned				*/
#define IEC_REAL	float

#include "vmShared.h"

#endif

/* ---------------------------------------------------------------------------- */
