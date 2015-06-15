/*H>> $Header: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/resource.h 1     28.02.07 19:04 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: resource.h $
 *                      $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/4CVMLib/resource.h $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CCG
 *
 * =COMPONENT           4CPC
 *
 * =CURRENT             $Date: 28.02.07 19:04 $
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
 *==String table defines for resource file
 *  All strings that need to be international should be a resource string
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  23.02.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#ifndef __RESOURCE_H_
#define __RESOURCE_H_

#define CG_RES_STR_CMD1                 1   //"command line with multiple targets"
#define CG_RES_STR_CMD2                 2   //"build target of type "
#define CG_RES_STR_CMD3                 3   //"build library of type "
#define CG_RES_STR_CMD_MSG              4   //"   - generating debug code -\r\n"

#define CG_RES_STR_NOIOCOMP             5   //"no IO component defined"

#define CG_RES_STR_EDGE_IN_FB           6   //"R/F_EDGE in user function block"

#define CG_RES_STR_AT_IN_PRG            8   //"AT %<addr> inside program"
#define CG_RES_STR_BOOL_WITH_INOUT      9   //"boolean variables with VAR_IN_OUT"
#define CG_RES_STR_INIT_INST            10  //"initialisation of instances"
#define CG_RES_STR_INIT_VAR_DADDR       11  //"initialisation of variables with direct address"
#define CG_RES_STR_LOC_CPLX_TYPE        12  //"local variables of complex type in functions"
#define CG_RES_STR_LOC_QVT_IN_FCT       13  //"local variables of QVT type in functions"

#define CG_RES_STR_MEMCPY_64K           14  //"memcopy of more than 64 kByte"
#define CG_RES_STR_CPLX_DEEPCOPY        15  //"deep copy of instance with complex member"
#define CG_RES_STR_INIT_VAR_AT          16  //"initialisation of variable with direct address"
#define CG_RES_STR_WRONG_INLINE_FB      17  //"function block inline syntax other than [STDLIB|EXTLIB](identifier)"
#define VM_NEXT_FREE                    18  //use me
#define CG_RES_STR_CALL_INOUT_FB        19  //"calling function blocks passed as VAR_IN_OUT"
#define CG_RES_STR_CPLX_TMP_LD_FBD      21  //"complex Tmp Var. LD / FBD network layout forcing Tmp Var"
#define CG_RES_STR_OP_ON_DTYPE          22  //" operation on data type "

#define CG_RES_STR_LIC_DATA             23  //"<license data>"
#define CG_RES_STR_MULTI_TRG            24  //"multiple targets"
#define CG_RES_STR_MORE_THAN            25  //"more than "
#define CG_RES_STR_TASKS                26  //" task(s)"
#define CG_RES_STR_ADR_NO_TRG           27  //"cannot use direct address if POU has no target scope (building a library?)"

#define VM_RES_STR_NS_ARR_IDX           28  //"data type '%1' as array index"
#define VM_RES_STR_NS_RETAIN_DIRADDR    29  //"retain variables with direct address"
#define VM_RES_STR_NS_STRUCTAT_NOSIMPLE 30 //"STRUCT/FB with direct address containing nonsimple members"
#define VM_RES_STR_NS_RETAIN_VARS       31  //"retain variables"
#define VM_RES_STR_COPY_ASSUMED_SIZE    32  //"copy of assumed size array/variable"
#define VM_RES_STR_CPLX_ARRAY_DEEPCOPY  33  //"copy of array of complex data type"
#define VM_RES_STR_NS_ARRAY_AT_ADDR     34  //"array at %-address"
#define VM_RES_STR_NS_CMPLX_ARRAY_AT    35  //"array at %-address with complex type
#define VM_RES_STR_WRONG_ARRAY_ADDR     36  //"wrong address size specifier '%1' for type '%2'"
#define VM_RES_STR_NS_LOCAL_RETAIN      37  //"local RETAIN variables (only VAR_GLOBAL RETAIN possible)"
#define VM_RES_STR_NS_CMPLX_RETAIN      38  //"VAR_GLOBAL RETAIN variables of complex type"
#define CG_RES_STR_NS_NON_RETAIN        39  //NON_RETAIN in user function blocks
#define CG_RES_STR_NS_FBI_RETAIN        40  //RETAIN of function block/program instances
#define CG_RES_STR_NS_MULTI_RETSEC      41  //more than one RETAIN declaration section
#define CG_RES_STR_NS_MULTI_CRETSEC     42  //more than one CONSTANT RETAIN declaration section
#define VM_RES_STR_BOOL_FUN_VARIO       44  //boolean variables as function VAR_OUTPUT/VAR_IN_OUT
#define VM_RES_STR_INV_ADDR_FMT         45  //invalid address format
#define VM_RES_STR_ADDR_CHANNEL_SIZE    46  //exceeds channel size
#define VM_RES_STR_INVALID_CHANNEL      47  //channel %1%2 has no %%3 segment"
#define VM_RES_STR_ARRAY_AT_BITPOS      48  //array at bit position (bit offset must be a multiple of 8)
#define VM_RES_STR_NS_TYPE_AT_ADDR      49  //direct address with data type %1
#define VM_RES_STR_NS_FEATURE_VER_MIN   50  //%1 in this version %2, need >=%3 
#define VM_RES_STR_OVERLAPSWITH         51  //overlaps with   
#define VM_RES_STR_NS_FEATURE_VER_MAX   52  //%1 in this version %2, need <=%3 
#endif // __RESOURCE_H_      


/*
 *----------------------------------------------------------------------------*
 *  $History: resource.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:04
 * Created in $/4CReleased/V2.20.00/TargetBase/CCG/4CVMLib
 * 
 * *****************  Version 11  *****************
 * User: Ef           Date: 11.05.06   Time: 18:03
 * Updated in $/4Control/TargetBase/CCG/4CVMlib
 * funFBId.txt has now min and max control version info
 * 
 * *****************  Version 10  *****************
 * User: Ef           Date: 23.08.05   Time: 10:19
 * Updated in $/4Control/TargetBase/CCG/4CVMlib
 * implemented warning if overlaping %address
 * 
 * *****************  Version 9  *****************
 * User: Ef           Date: 24.05.05   Time: 15:56
 * Updated in $/4Control/TargetBase/CCG/4CVMlib
 * implemented channel info (for fcbac IO-Layer)
 * 
 * *****************  Version 8  *****************
 * User: Ef           Date: 15.04.04   Time: 17:00
 * Updated in $/4Control/TargetBase/CCG/4CVMlib
 * implemented task attr. WATCHDOG
 * 
 * *****************  Version 7  *****************
 * User: Ef           Date: 13.04.04   Time: 18:56
 * Updated in $/4Control/TargetBase/CCG/4CVMlib
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 13.04.04   Time: 12:59
 * Updated in $/4Control/TargetBase/CCG/4CVMlib
 * fixed d-2186
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 19.03.04   Time: 15:06
 * Updated in $/4Control/TargetBase/CCG/4CVMlib
 * fixed d-2159,d-2145,d-1870
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 27.02.04   Time: 15:54
 * Updated in $/4Control/TargetBase/CCG/4CVMlib
 * array at %addr only for 4cosai;4cwin, use __int64 for channel size
 * check
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 27.02.04   Time: 13:17
 * Updated in $/4Control/TargetBase/CCG/4CVMlib
 * fixed LEA of global simple var, fixed array alignment 
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 25.02.04   Time: 14:13
 * Updated in $/4Control/TargetBase/CCG/4CVMlib
 * AT <identifier> syntax for Osai and check array sizes
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:35
 * Created in $/4Control/TARGETBASE/CCG/4CVMlib
 * 
 * *****************  Version 8  *****************
 * User: Ef           Date: 4.11.02    Time: 17:11
 * Updated in $/4Control/TargetBase/CCG/Compiler
 * started with fun 4 bytes aligenment, removed all unnecessary offset
 * calculations
 * 
 * *****************  Version 7  *****************
 * User: Ef           Date: 1.07.02    Time: 16:59
 * Updated in $/4Control/TargetBase/CCG/compiler
 * d-1766
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 28.06.02   Time: 13:21
 * Updated in $/4Control/TargetBase/CCG/compiler
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 7.06.02    Time: 14:56
 * Updated in $/4Control/TargetBase/CCG/compiler
 * retain finished
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 6.06.02    Time: 21:30
 * Updated in $/4Control/TargetBase/CCG/compiler
 * more retain restrictions
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 5.06.02    Time: 17:28
 * Updated in $/4Control/TargetBase/CCG/compiler
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 4.06.02    Time: 17:34
 * Updated in $/4Control/TargetBase/CCG/compiler
 * local retain
 * 
 * *****************  Version 1  *****************
 * User: Jd           Date: 28.02.02   Time: 10:53
 * Created in $/4Control/TargetBase/CCG/compiler
 * KM changes. Moved to new location.
 * 
 * *****************  Version 10  *****************
 * User: Die          Date: 29.01.02   Time: 14:53
 * Updated in $/4Control/COM/softing/fc/AddOns/Softing/4CWinTarget/CCG/compiler
 * 
 * *****************  Version 9  *****************
 * User: Die          Date: 24.01.02   Time: 16:51
 * Updated in $/4Control/COM/softing/fc/AddOns/Softing/4CWinTarget/CCG/compiler
 * 
 * *****************  Version 8  *****************
 * User: Die          Date: 15.01.02   Time: 17:50
 * Updated in $/4Control/COM/softing/fc/CCG/4cvm/compiler
 * 
 * *****************  Version 7  *****************
 * User: Die          Date: 4.01.02    Time: 17:50
 * Updated in $/4Control/COM/softing/fc/CCG/4cvm/compiler
 * 
 * *****************  Version 6  *****************
 * User: Die          Date: 11.12.01   Time: 21:51
 * Updated in $/4Control/COM/softing/fc/CCG/4cvm/compiler
 * 
 * *****************  Version 5  *****************
 * User: Die          Date: 29.11.01   Time: 18:49
 * Updated in $/4Control/COM/softing/fc/CCG/4cvm/compiler
 * 
 * *****************  Version 4  *****************
 * User: Die          Date: 20.11.01   Time: 18:51
 * Updated in $/4Control/COM/softing/fc/CCG/4cvm/compiler
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 16.11.01   Time: 14:33
 * Updated in $/4Control/COM/softing/fc/CCG/4cvm/compiler
 * build libs without target, warnings if no resource assigned  or no
 * control address specified. Also CONTROL,CONSOLE,RESOURCE attrips in 4ch
 * are now optional
 * 
 * *****************  Version 2  *****************
 * User: Ca           Date: 11/15/01   Time: 11:48a
 * Updated in $/4Control/COM/softing/fc/CCG/4cvm/compiler
 * 
 * *****************  Version 1  *****************
 * User: Die          Date: 4.04.01    Time: 16:50
 * Created in $/4Control/COM/softing/fc/CCG/4vm/compiler
 * initial check in
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 1.03.01    Time: 20:59
 * Updated in $/4Control/COM/softing/fc/CCG/4cpc
 *==
 *----------------------------------------------------------------------------*
*/
