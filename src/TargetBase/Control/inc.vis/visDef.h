/* $Header: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc.vis/visDef.h 1     28.02.07 19:10 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: visDef.h $
 *						 $Logfile: /4CReleased/V2.20.00/Target/4CWin/CONTROL/inc.vis/visDef.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				RunTime
 *
 * =COMPONENT			visLib
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

#ifndef _VISDEF_H_
#define _VISDEF_H_

#if defined(INC_4C)
#error "Must not be included in 4C!"
#endif

#if !defined(_STDINC_H_)
#error "Include via stdInc.h only!"
#endif


/* Communication time out for the response from the remote ATCMControl
 * Run Time System.
 */
#define VIS_TCP_TIMEOUT 		6000


/* State variables of the ATCMControl VisuComm library.
 */
typedef struct 
{
	IEC_BOOL  bInitialized; 	/* Connected to remote RTS and		*/
								/* configuration received.			*/
	IEC_BOOL  bLogin;			/* Logged in to remote RTS and		*/
								/* IEC project ID received. 		*/

	/* The following values are retrieved from the remote RTS and	*/
	/* are only valid if the library is connected to an 4C RTS. 	*/

	IEC_BOOL  bBE_Control;		/* TRUE, if target is Big Endian	*/
	IEC_BOOL  bBE_This; 		/* TRUE, if I'm Big Endian			*/

	IEC_UDINT ulFirmware;		/* Firmware version 				*/
	IEC_UDINT ulMaxData;		/* Comm. Buffer size (MAX_DATA) 	*/
	IEC_UDINT ulPort;			/* TCP/IP Portnummer				*/

	IEC_DATA  pProjectID[VMM_GUID]; /* Current project GUID 		*/	
	
	IEC_DATA  *pCmdBuff;		/* Communication buffer 			*/

} SVisInfo; 					/* -------------------------------- */


#endif /* _VISDEF_H_ */

/* ---------------------------------------------------------------------------- */
