/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/CG_IECFront_i.c 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CG_IECFront_i.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/CG_IECFront_i.c $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CCG
 *
 * =COMPONENT           CG_IECFront
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
 *==Hand made define GUID file for interfaces and coclass from CG_IECFront.h
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  19.02.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#include <unknwn.h>

#ifdef __cplusplus
extern "C"{
#endif 

//clsid and the iid of the interfaces of this class:
EXTERN_C const GUID CLSID_CG_IECFront  = {0x877aaed0, 0xe550, 0x11d5, {0xa5, 0x35, 0x00, 0x08, 0xc7, 0x79, 0xc6, 0x00}};
EXTERN_C const GUID IID_ICG_IECFront   = {0xa92225e0, 0x4883, 0x11d6, {0xa5, 0x74, 0x0, 0x8, 0xc7, 0x79, 0xc6, 0x0}};
//old EXTERN_C const GUID IID_ICG_IECFront   = {0x877aaed0, 0xe550, 0x11d4, {0xa5, 0x35, 0x00, 0x08, 0xc7, 0x79, 0xc6, 0x00}};







//interfaces retunred from IID_ICG_IECFront:
EXTERN_C const GUID IID_ICG_Body       = {0x5653c340, 0x46ef, 0x11d6, {0xa5, 0x74, 0x0,  0x8,  0xc7, 0x79, 0xc6, 0x00}};
//  old      IID_ICG_Body                {0xc47d7410, 0xe55e, 0x11d4, {0xa5, 0x35, 0x00, 0x08, 0xc7, 0x79, 0xc6, 0x00}};
EXTERN_C const GUID IID_ICG_Expr       = {0x33c89b00, 0x033e, 0x11d5, {0xa5, 0x38, 0x00, 0x08, 0xc7, 0x79, 0xc6, 0x00}}; 
EXTERN_C const GUID IID_ICG_Config     = {0xd7bb2ab8, 0x94e4, 0x4d71, {0x9e, 0x83, 0xa7, 0xd7, 0x3e, 0x64, 0x37, 0xde}};
EXTERN_C const GUID IID_ICG_ExtBodyInfo= {0x8b175050,0x1252,0x11d5, {0xa5, 0x3a, 0x00, 0x08, 0xc7, 0x79, 0xc6, 0x00}};




#ifdef __cplusplus
}
#endif



/*
 *----------------------------------------------------------------------------*
 *  $History: CG_IECFront_i.c $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 8  *****************
 * User: Ef           Date: 5.04.02    Time: 15:07
 * Updated in $/4Control/COM/softing/fc/inc
 * changed clsid (SFC Exception handling)
 * 
 * *****************  Version 7  *****************
 * User: Ef           Date: 3.04.02    Time: 12:43
 * Updated in $/4Control/COM/softing/fc/inc
 * new guid for sfc exception handling
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 15.03.01   Time: 18:18
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 12.03.01   Time: 18:34
 * Updated in $/4Control/COM/softing/fc/inc
 * implemented config
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 6.03.01    Time: 22:01
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 1.03.01    Time: 20:53
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 19.02.01   Time: 21:13
 * Updated in $/4Control/COM/softing/fc/inc
 * use _i.c file
 *==
 *----------------------------------------------------------------------------*
*/
