/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc.bac/bacIec.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: bacIec.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc.bac/bacIec.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				RunTime
 *
 * =COMPONENT			ioBac
 *
 * =CURRENT 	 $Date: 28.02.07 19:10 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _BACIEC_H_
#define _BACIEC_H_

#if defined(INC_4C)
#error "Must not be included in 4C!"
#endif

#if !defined(_STDINC_H_)
#error "Include via stdInc.h only!"
#endif

#if ! defined(IP_CFG_INST32) && ! defined(IP_CFG_INST64)
#error 32 or 64 bit instance data alignment is required for BACnet object definitions!
/* No 64 bit data types are used in the shared memory structures, so there should be
 * no difference in the structure definitions between 32 and 64 bit instance data
 * alignment.
 */
#endif

#define RTS_PRAGMA_PACK_1	/* >>>> Align 1 Begin >>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
#include "osAlign.h"
#undef	RTS_PRAGMA_PACK_1


typedef struct
{
	DEC_VAR(IEC_STRLEN, curLen);
	DEC_VAR(IEC_STRLEN, maxLen);
	DEC_VAR(IEC_CHAR,	str[MAX_OBJECT_NAME_LEN]);

} BAC_STRING50;


/* --------------------------------------------------------------------------- */

/* Device
 * ----------------------------------------------------------------------------
 */
typedef struct				/* sizeof(SDV) == 60	*/
{
	DEC_VAR(IEC_UDINT,		Object_Identifier); 			/*	  0 	4		*/
	DEC_VAR(BAC_STRING50,	Object_Name);					/*	  4    52		*/

	/* >>>>> None BACnet attributes >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
	DEC_VAR(IEC_WORD,		Quality);						/*	 56 	2		*/
	DEC_VAR(IEC_UINT,		alignment_dummy_1); 			/*	 58 	2		*/
	/* <<<<< None BACnet attributes <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

} SDV;

/* Analog Input
 * ----------------------------------------------------------------------------
 */
typedef struct				/* sizeof(SAI) == 72	*/
{
	DEC_VAR(IEC_UDINT,		Object_Identifier); 			/*	  0 	4		*/
	DEC_VAR(BAC_STRING50,	Object_Name);					/*	  4    52		*/

	DEC_VAR(IEC_REAL,		Present_Value); 				/*	 56 	4		*/
	DEC_VAR(IEC_WORD,		Status_Flags);					/*	 60 	2		*/

	/* >>>>> None BACnet attributes >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
	DEC_VAR(IEC_WORD,		Quality);						/*	 62 	2		*/
	DEC_VAR(IEC_UDINT,		Device_ID); 					/*	 64 	4		*/
	DEC_VAR(IEC_REAL,		Command_Value); 				/*	 68 	4		*/
	/* <<<<< None BACnet attributes <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

} SAI;

/* Analog Output
 * ----------------------------------------------------------------------------
 */
typedef struct				/* sizeof(SAO) == 72	*/
{
	DEC_VAR(IEC_UDINT,		Object_Identifier); 			/*	  0 	4		*/
	DEC_VAR(BAC_STRING50,	Object_Name);					/*	  4    52		*/

	DEC_VAR(IEC_REAL,		Present_Value); 				/*	 56 	4		*/
	DEC_VAR(IEC_WORD,		Status_Flags);					/*	 60 	2		*/

	/* >>>>> None BACnet attributes >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
	DEC_VAR(IEC_WORD,		Quality);						/*	 62 	2		*/
	DEC_VAR(IEC_UDINT,		Device_ID); 					/*	 64 	4		*/
	DEC_VAR(IEC_REAL,		Command_Value); 				/*	 68 	4		*/
	/* <<<<< None BACnet attributes <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

} SAO;

/* Analog Value
 * ----------------------------------------------------------------------------
 */
typedef struct				/* sizeof(SAV) == 72	*/
{
	DEC_VAR(IEC_UDINT,		Object_Identifier); 			/*	  0 	4		*/
	DEC_VAR(BAC_STRING50,	Object_Name);					/*	  4    52		*/

	DEC_VAR(IEC_REAL,		Present_Value); 				/*	 56 	4		*/
	DEC_VAR(IEC_WORD,		Status_Flags);					/*	 60 	2		*/

	/* >>>>> None BACnet attributes >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
	DEC_VAR(IEC_WORD,		Quality);						/*	 62 	2		*/
	DEC_VAR(IEC_UDINT,		Device_ID); 					/*	 64 	4		*/
	DEC_VAR(IEC_REAL,		Command_Value); 				/*	 68 	4		*/
	/* <<<<< None BACnet attributes <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

} SAV;

/* --------------------------------------------------------------------------- */

/* Binary Input
 * ----------------------------------------------------------------------------
 */
typedef struct				/* sizeof(SBI) == 72	*/
{
	DEC_VAR(IEC_UDINT,		Object_Identifier); 			/*	  0 	4		*/
	DEC_VAR(BAC_STRING50,	Object_Name);					/*	  4    52		*/

	DEC_VAR(IEC_WORD,		Present_Value); 				/*	 56 	2		*/
	DEC_VAR(IEC_WORD,		Status_Flags);					/*	 58 	2		*/

	/* >>>>> None BACnet attributes >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
	DEC_VAR(IEC_WORD,		Quality);						/*	 60 	2		*/
	DEC_VAR(IEC_WORD,		alignment_dummy_1); 			/*	 62 	2		*/
	DEC_VAR(IEC_UDINT,		Device_ID); 					/*	 64 	4		*/
	DEC_VAR(IEC_WORD,		Command_Value); 				/*	 68 	2		*/
	DEC_VAR(IEC_WORD,		alignment_dummy_2); 			/*	 70 	2		*/
	/* <<<<< None BACnet attributes <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

} SBI;

/* Binary Output
 * ----------------------------------------------------------------------------
 */
typedef struct				/* sizeof(SBO) == 72	*/
{
	DEC_VAR(IEC_UDINT,		Object_Identifier); 			/*	  0 	4		*/
	DEC_VAR(BAC_STRING50,	Object_Name);					/*	  4    52		*/

	DEC_VAR(IEC_WORD,		Present_Value); 				/*	 56 	2		*/
	DEC_VAR(IEC_WORD,		Status_Flags);					/*	 58 	2		*/

	/* >>>>> None BACnet attributes >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
	DEC_VAR(IEC_WORD,		Quality);						/*	 60 	2		*/
	DEC_VAR(IEC_WORD,		alignment_dummy_1); 			/*	 62 	2		*/
	DEC_VAR(IEC_UDINT,		Device_ID); 					/*	 64 	4		*/
	DEC_VAR(IEC_WORD,		Command_Value); 				/*	 68 	2		*/
	DEC_VAR(IEC_WORD,		alignment_dummy_2); 			/*	 70 	2		*/
	/* <<<<< None BACnet attributes <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

} SBO;

/* Binary Value
 * ----------------------------------------------------------------------------
 */
typedef struct				/* sizeof(SBV) == 72	*/
{
	DEC_VAR(IEC_UDINT,		Object_Identifier); 			/*	  0 	4		*/
	DEC_VAR(BAC_STRING50,	Object_Name);					/*	  4    52		*/

	DEC_VAR(IEC_WORD,		Present_Value); 				/*	 56 	2		*/
	DEC_VAR(IEC_WORD,		Status_Flags);					/*	 58 	2		*/

	/* >>>>> None BACnet attributes >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
	DEC_VAR(IEC_WORD,		Quality);						/*	 60 	2		*/
	DEC_VAR(IEC_WORD,		alignment_dummy_1); 			/*	 62 	2		*/
	DEC_VAR(IEC_UDINT,		Device_ID); 					/*	 64 	4		*/
	DEC_VAR(IEC_WORD,		Command_Value); 				/*	 68 	2		*/
	DEC_VAR(IEC_WORD,		alignment_dummy_2); 			/*	 70 	2		*/
	/* <<<<< None BACnet attributes <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

} SBV;

/* --------------------------------------------------------------------------- */

/* Multi-state Input
 * ----------------------------------------------------------------------------
 */
typedef struct				/* sizeof(SMI) == 72	*/
{
	DEC_VAR(IEC_UDINT,		Object_Identifier); 			/*	  0 	4		*/
	DEC_VAR(BAC_STRING50,	Object_Name);					/*	  4    52		*/

	DEC_VAR(IEC_UDINT,		Present_Value); 				/*	 56 	4		*/
	DEC_VAR(IEC_WORD,		Status_Flags);					/*	 60 	2		*/

	/* >>>>> None BACnet attributes >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
	DEC_VAR(IEC_WORD,		Quality);						/*	 62 	2		*/
	DEC_VAR(IEC_UDINT,		Device_ID); 					/*	 64 	4		*/
	DEC_VAR(IEC_UDINT,		Command_Value); 				/*	 68 	4		*/
	/* <<<<< None BACnet attributes <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

} SMI;

/* Multi-state Output
 * ----------------------------------------------------------------------------
 */
typedef struct				/* sizeof(SMO) == 72	*/
{
	DEC_VAR(IEC_UDINT,		Object_Identifier); 			/*	  0 	4		*/
	DEC_VAR(BAC_STRING50,	Object_Name);					/*	  4    52		*/

	DEC_VAR(IEC_UDINT,		Present_Value); 				/*	 56 	4		*/
	DEC_VAR(IEC_WORD,		Status_Flags);					/*	 60 	2		*/

	/* >>>>> None BACnet attributes >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
	DEC_VAR(IEC_WORD,		Quality);						/*	 62 	2		*/
	DEC_VAR(IEC_UDINT,		Device_ID); 					/*	 64 	4		*/
	DEC_VAR(IEC_UDINT,		Command_Value); 				/*	 68 	4		*/
	/* <<<<< None BACnet attributes <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

} SMO;

/* Multi-state Value
 * ----------------------------------------------------------------------------
 */
typedef struct				/* sizeof(SMV) == 72	*/
{
	DEC_VAR(IEC_UDINT,		Object_Identifier); 			/*	  0 	4		*/
	DEC_VAR(BAC_STRING50,	Object_Name);					/*	  4    52		*/

	DEC_VAR(IEC_UDINT,		Present_Value); 				/*	 56 	4		*/
	DEC_VAR(IEC_WORD,		Status_Flags);					/*	 60 	2		*/

	/* >>>>> None BACnet attributes >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
	DEC_VAR(IEC_WORD,		Quality);						/*	 62 	2		*/
	DEC_VAR(IEC_UDINT,		Device_ID); 					/*	 64 	4		*/
	DEC_VAR(IEC_UDINT,		Command_Value); 				/*	 68 	4		*/
	/* <<<<< None BACnet attributes <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

} SMV;

/* --------------------------------------------------------------------------- */

/* Notification Class
 * ----------------------------------------------------------------------------
 */
typedef struct				/* sizeof(SNC) == 64	*/
{
	DEC_VAR(IEC_UDINT,		Object_Identifier); 			/*	  0 	4		*/
	DEC_VAR(BAC_STRING50,	Object_Name);					/*	  4    52		*/

	/* >>>>> None BACnet attributes >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
	DEC_VAR(IEC_WORD,		Quality);						/*	 56 	2		*/
	DEC_VAR(IEC_WORD,		dummy_01);						/*	 58 	2		*/
	DEC_VAR(IEC_UDINT,		Device_ID); 					/*	 60 	4		*/
	/* <<<<< None BACnet attributes <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

} SNC;

/* --------------------------------------------------------------------------- */

/* Trend Log
 * ----------------------------------------------------------------------------
 */
typedef struct				/* sizeof(STL) == 64	*/
{
	DEC_VAR(IEC_UDINT,		Object_Identifier); 			/*	  0 	4		*/
	DEC_VAR(BAC_STRING50,	Object_Name);					/*	  4    52		*/

	/* >>>>> None BACnet attributes >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
	DEC_VAR(IEC_WORD,		Quality);						/*	 56 	2		*/
	DEC_VAR(IEC_WORD,		dummy_01);						/*	 58 	2		*/
	DEC_VAR(IEC_UDINT,		Device_ID); 					/*	 60 	4		*/
	/* <<<<< None BACnet attributes <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

} STL;

/* --------------------------------------------------------------------------- */

/* Schedule
 * ----------------------------------------------------------------------------
 */
typedef struct				/* sizeof(SSC) == 64	*/
{
	DEC_VAR(IEC_UDINT,		Object_Identifier); 			/*	  0 	4		*/
	DEC_VAR(BAC_STRING50,	Object_Name);					/*	  4    52		*/

	/* >>>>> None BACnet attributes >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
	DEC_VAR(IEC_WORD,		Quality);						/*	 56 	2		*/
	DEC_VAR(IEC_WORD,		dummy_01);						/*	 58 	2		*/
	DEC_VAR(IEC_UDINT,		Device_ID); 					/*	 60 	4		*/
	/* <<<<< None BACnet attributes <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

} SSC;

/* --------------------------------------------------------------------------- */

#define RTS_PRAGMA_PACK_DEF 	
#include "osAlign.h"
#undef	RTS_PRAGMA_PACK_DEF 	/* <<<< Align 1 end <<<<<<<<<<<<<<<<<<<<<<<<<<< */


#endif /* _BACIEC_H_ */

/* ---------------------------------------------------------------------------- */
