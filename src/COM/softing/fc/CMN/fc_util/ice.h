/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/ice.h 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: ice.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util/ice.h $
 *
 * =PROJECT             CAK1020    ATCMControl V2.0
 *
 * =SWKE                CLIC
 *
 * =COMPONENT           LICLIB
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  02.02.01  Wi      File created (copied from V1.3)
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

/*
 * Header file for the ICE encryption library.
 *
 * Written by Matthew Kwan - July 1996
 */

#ifndef _ICE_H
#define _ICE_H

typedef struct ice_key_struct   ICE_KEY;

extern ICE_KEY*         ice_key_create(int n);
extern void             ice_key_destroy(ICE_KEY* ik);
extern void             ice_key_set(ICE_KEY* ik, const unsigned char* k);
extern unsigned char*   ice_key_encrypt(const ICE_KEY* ik, const unsigned char* ptxt, unsigned char* ctxt);
extern unsigned char*   ice_key_decrypt(const ICE_KEY* ik, const unsigned char* ctxt, unsigned char* ptxt);
extern int              ice_key_key_size(const ICE_KEY* ik);
extern int              ice_key_block_size(const ICE_KEY* ik);

#undef P_

#endif

/*
 *----------------------------------------------------------------------------*
 *  $History: ice.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 1  *****************
 * User: Sis          Date: 7.08.02    Time: 13:51
 * Created in $/4Control/COM/softing/fc/CMN/fc_util
 * 
 * *****************  Version 1  *****************
 * User: Wi           Date: 2.02.01    Time: 16:27
 * Created in $/4Control/COM/softing/fc/CLIC/LicLib/License
 * moved from V1.3
 * 
 *==
 *----------------------------------------------------------------------------*
*/