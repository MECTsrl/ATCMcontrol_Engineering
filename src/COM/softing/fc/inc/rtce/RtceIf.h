/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/rtce/RtceIf.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: RtceIf.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/rtce/RtceIf.h $
 *
 * =PROJECT             7VJ2061  ATCMControl ControlStack
 *
 * =SWKE                RTCE
 *
 * =COMPONENT           RTCE
 *
 * =CURRENT             $Date: 28.02.07 19:01 $
 *                      $Revision: 1 $
 *
 * =ENVIRONMENT         NT Kernel:
 *                      MS Windows NT 4.0
 *                      NT DDK
 *
 * =REFERENCES
 *==
 *----------------------------------------------------------------------------*
 *

 

 

 *==
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATIONS
 * $History: RtceIf.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc/rtce
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc/rtce
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 26.07.01   Time: 11:23
 * Created in $/4Control/COM/softing/fc/inc/rtce
 * 
 * *****************  Version 4  *****************
 * User: Ed           Date: 17.06.99   Time: 18:43
 * Updated in $/RTCE
 * Renamed almost all ProfiX to RtceX
 * 
 * *****************  Version 3  *****************
 * User: Ed           Date: 17.06.99   Time: 15:32
 * Updated in $/RTCE
 * 
 * *****************  Version 2  *****************
 * User: Ed           Date: 16.06.99   Time: 17:54
 * Updated in $/RTCE
 * 
 * *****************  Version 1  *****************
 * User: Ed           Date: 15.06.99   Time: 15:31
 * Created in $/RTCE
 *==
 ******************************************************************************
H<<*/

// definitions for Interface 
// user-mode <-> RTCE device

#define RTCE_USER_INTERFACE_VERSION   19990617

#define FILE_DEVICE_RTCE 0x8001
#define IOCTL_RTCE_GET_VERSION  (ULONG)CTL_CODE(FILE_DEVICE_RTCE, 0x810, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_RTCE_SET_CODE     (ULONG)CTL_CODE(FILE_DEVICE_RTCE, 0x811, METHOD_NEITHER, FILE_WRITE_ACCESS)
#define IOCTL_RTCE_CLEAR_CODE   (ULONG)CTL_CODE(FILE_DEVICE_RTCE, 0x812, METHOD_NEITHER, FILE_WRITE_ACCESS)
#define IOCTL_RTCE_CHANGE_BITS  (ULONG)CTL_CODE(FILE_DEVICE_RTCE, 0x813, METHOD_BUFFERED, FILE_READ_ACCESS|FILE_WRITE_ACCESS)
#define IOCTL_RTCE_CHANGE_BIT   (ULONG)CTL_CODE(FILE_DEVICE_RTCE, 0x814, METHOD_BUFFERED, FILE_READ_ACCESS|FILE_WRITE_ACCESS)
#define IOCTL_RTCE_GET_STATS    (ULONG)CTL_CODE(FILE_DEVICE_RTCE, 0x815, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_RTCE_RESET_STATS  (ULONG)CTL_CODE(FILE_DEVICE_RTCE, 0x816, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct tagRTCE_VERSION_DATA
{
  ULONG User_Interface_Version;     //RTCE_USER_INTERFACE_VERSION
  ULONG Rtce_Interpreter_Version;   //RTCE_INTERPRETER_VERSION
  ULONG Rtce_Library_Version;       //RTCE_INTERPRETER_VERSION (for now)
  ULONG Hardware_Interface_Version; //PROFI_USR_IF_VERSION
} RTCE_VERSION_DATA;

typedef struct tagRTCE_CHANGE_BITS_PARAM
{
  ULONG Offset;
  ULONG Length;
  PVOID Value;
  PVOID Mask;
}RTCE_CHANGE_BITS_PARAM;

typedef struct tagRTCE_CHANGE_BIT_PARAM
{
  ULONG Offset;
  UCHAR Value;
  UCHAR Mask;
  USHORT reserved;
}RTCE_CHANGE_BIT_PARAM;

typedef struct tagRTCE_STATISTIC_DATA
{ //statistics, all values in mycro seconds
  LARGE_INTEGER nCycles;
  ULONG dTmin;
  ULONG dTmax;
  LARGE_INTEGER dTsum;
  LARGE_INTEGER dTsum2;
  ULONG eTmin;
  ULONG eTmax;
  LARGE_INTEGER eTsum;
  LARGE_INTEGER eTsum2;
} RTCE_STATISTIC_DATA;
