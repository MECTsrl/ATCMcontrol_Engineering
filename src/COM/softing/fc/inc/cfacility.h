/*H>> $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: cfacility.h $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/cfacility.h $
 *
 * =PROJECT             CAK1020    ATCMControl V2.0
 *
 * =SWKE                FC
 *
 * =COMPONENT           all
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
 *    Header to define the Severity and Facility Codes for ATCMControl
 *    Definition of the Facility codes for the kernel componentes
 *    
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.02.01  Wi      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef _CFACILITY_H
#define _CFACILITY_H

//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//

//
// Define the severity codes
//
#define E_SEVERITY_SUCCESS               0x0
#define E_SEVERITY_INFORMATIONAL         0x1
#define E_SEVERITY_WARNING               0x2
#define E_SEVERITY_ERROR                 0x3

//
// Define the facility codes
//

// base of  4C facility codes:
#define E_FACILITY_4C_BASE               0x200
#define E_FACILITY_4C_LAST               0x2FF

#define E_FACILITY_4C_ADDON_BASE         0x300
#define E_FACILITY_4C_ADDON_LAST         0xFFF   

#define IS_4C_FACILITY(fac) (((fac) >= E_FACILITY_4C_BASE) && ((fac) <= E_FACILITY_4C_LAST))
#define IS_4C_ADDON_FACILITY(fac) (((fac) >= E_FACILITY_4C_ADDON_BASE) && ((fac) <= E_FACILITY_4C_ADDON_LAST))

#define E_FACILITY_CONTROL               0x200
#define E_FACILITY_IO                    0x210
#define E_FACILITY_SYSCOM                0x220
#define E_FACILITY_CODEGEN               0x230
#define E_FACILITY_LIBRARIAN             0x260
#define E_FACILITY_SETUP                 0x270

#define E_FACILITY_ENGINEERING           0x280
#define E_FACILITY_KADMAN                0x281
#define E_FACILITY_PRJMAN                0x282
#define E_FACILITY_CEDOWNLOAD            0x283
#define E_FACILITY_CEFINDINFILES         0x284
#define E_FACILITY_CESYSTEM              0x285
#define E_FACILITY_CEBPMAN               0x286
#define E_FACILITY_CEGR                  0x287
#define E_FACILITY_CEST                  0x288
#define E_FACILITY_CEEXE				 0x289
#define E_FACILITY_CEXREF                0x28A

#define E_FACILITY_WIZARD                0x2A0
#define E_FACILITY_BASELIB               0x2F0
#define E_FACILITY_UTIL                  0x2F2
#define E_FACILITY_XML                   0x2F3

#define E_FACILITY_PNCOMP                0xA00

#endif


/*
 *----------------------------------------------------------------------------*
 *  $History: cfacility.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 9  *****************
 * User: He           Date: 7.08.01    Time: 15:21
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 8  *****************
 * User: He           Date: 7.08.01    Time: 15:15
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 7  *****************
 * User: Sis          Date: 2.08.01    Time: 18:25
 * Updated in $/4Control/COM/softing/fc/inc
 * find in files
 * 
 * *****************  Version 6  *****************
 * User: Sis          Date: 17.07.01   Time: 13:34
 * Updated in $/4Control/COM/softing/fc/inc
 * added E_FACILITY_CEEXE
 * 
 * *****************  Version 5  *****************
 * User: Scd          Date: 13.03.01   Time: 9:17
 * Updated in $/4Control/COM/softing/fc/inc
 * added new facility code for pipe network compiler frontend
 * 
 * *****************  Version 4  *****************
 * User: Scd          Date: 15.02.01   Time: 11:48
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 3  *****************
 * User: Oh           Date: 2/14/01    Time: 6:00p
 * Updated in $/4Control/COM/softing/fc/inc
 * E_FACILITY_CEDOWNLOAD inserted
 * 
 * *****************  Version 2  *****************
 * User: Wi           Date: 7.02.01    Time: 11:08
 * Updated in $/4Control/COM/softing/fc/inc
 * first version, moved from V1.3, modified for kernel
 * 
 * 
 *==
 *----------------------------------------------------------------------------*
*/

