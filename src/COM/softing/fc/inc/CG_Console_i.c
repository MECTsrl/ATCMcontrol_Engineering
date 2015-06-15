/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/CG_Console_i.c 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CG_Console_i.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/CG_Console_i.c $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CCG
 *
 * =COMPONENT           CG_Console
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *
 


 

 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  02.08.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/

#include <unknwn.h>

#ifdef __cplusplus
extern "C"{
#endif 

//clsid and the iid of the interfaces of this class:
// {10776500-8717-11d5-8793-00010209D2BC}
EXTERN_C const GUID CLSID_CG_Console = {0x10776500, 0x8717, 0x11d5, {0x87, 0x93, 0x00, 0x01, 0x02, 0x09, 0xd2, 0xbc}};

// {10776501-8717-11d5-8793-00010209D2BC}
EXTERN_C const GUID IID_ICG_Console  = {0x10776501, 0x8717, 0x11d5, {0x87, 0x93, 0x00, 0x01, 0x02, 0x09, 0xd2, 0xbc}};


#ifdef __cplusplus
}
#endif



/*
 *----------------------------------------------------------------------------*
 *  $History: CG_Console_i.c $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 6.08.01    Time: 11:06
 * Created in $/4Control/COM/softing/fc/inc
 *==
 *----------------------------------------------------------------------------*
*/
