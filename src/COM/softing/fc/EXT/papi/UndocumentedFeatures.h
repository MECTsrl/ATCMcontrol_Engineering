/*****************************************************************************/
/*                               PROFIBUS DRIVERS                            */
/*                                                                           */
/*  Filename    : UNDOCUMENTEDFEATURES.H                                     */
/*                                                                           */
/*  Description : trace defines and structures the driver                    */
/*                                                                           */
/*  CHANGE_NOTES                                                             */
/*                                                                           */
/*  date      name      change                                               */
/*  -----------------------------------------------------------------------  */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/


#ifndef _UNDOCUMENTEDFEATURES_H_
#define _UNDOCUMENTEDFEATURES_H_

#ifndef FILE_DEVICE_PROFIBOARD
#define FILE_DEVICE_PROFIBOARD  0x8000
#endif

#define IOCTL_PROFI_SET_BOARD_FEATURES           (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x896, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROFI_GET_BOARD_FEATURES           (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x897, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_PROFI_SET_PORT                     (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x898, METHOD_BUFFERED,FILE_WRITE_DATA)
#define IOCTL_PROFI_GET_PORT                     (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x899, METHOD_BUFFERED,FILE_READ_DATA)
#define IOCTL_PROFI_SET_DPRAM                    (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x89A, METHOD_BUFFERED,FILE_WRITE_DATA)
#define IOCTL_PROFI_GET_DPRAM                    (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x89B, METHOD_OUT_DIRECT,FILE_ANY_ACCESS)
#define IOCTL_PROFI_GET_STATISTICS               (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x89C, METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PROFI_TOGGLE_FW_TRACE              (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x89D, METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PROFI_GET_FW_TRACE                 (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x89E, METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PROFI_SET_TRACE                    (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x89F, METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PROFI_GET_TRACE                    (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x8A0, METHOD_OUT_DIRECT,FILE_ANY_ACCESS)
#define IOCTL_PROFI_RESET_STATISTICS             (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x8A4, METHOD_BUFFERED,FILE_ANY_ACCESS)

#define IOCTL_PROFI_GET_TRACEBUF                 (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x8A1, METHOD_BUFFERED,FILE_ANY_ACCESS)

#define IOCTL_PROFI_CREATE_BOARD                 (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x8A2, METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PROFI_DELETE_BOARD                 (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x8A3, METHOD_BUFFERED,FILE_ANY_ACCESS)

#define IOCTL_PROFI_GET_CONFIG                   (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x8A5, METHOD_BUFFERED,FILE_ANY_ACCESS)

#define IOCTL_PROFI_GET_SYMBOLIC_NODE_NAME       (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x8A6, METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PROFI_GET_OPERATION_MODE           (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x8A7, METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PROFI_GET_RTCE_DEVICE              (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x8A8, METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PROFI_CAN_DELETE_BOARD             (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x8A9, METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PROFI_GET_DEVICE_CONFIGURATION     (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x8AF, METHOD_BUFFERED,FILE_ANY_ACCESS)

#define IOCTL_PROFIPNP_GET_DEVICE_CONFIGURATION  (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x8AA, METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PROFIPNP_ENABLE_DEVICE_IRQ         (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x8AB, METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PROFIPNP_DISABLE_DEVICE_IRQ        (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x8AC, METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PROFIPNP_USE_DEVICE                (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x8AD, METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_PROFIPNP_UNUSE_DEVICE              (ULONG)CTL_CODE(FILE_DEVICE_PROFIBOARD, 0x8AE, METHOD_BUFFERED,FILE_ANY_ACCESS)


typedef struct _PROFI_SET_PORT
{
  char PortAddrOffset;
  char Value;
} PROFI_SET_PORT;

typedef struct _PROFI_SET_DPRAM
{
  ULONG       Offset;
  ULONG       Data[1];
} PROFI_SET_DPRAM;

typedef struct _PROFI_STATISTICS
{
  ULONG       ReadServiceRequests;
  ULONG       WriteServiceRequests;
  ULONG       ReadDataRequests;
  ULONG       WriteDataRequests;
  ULONG       DpCycles;
  ULONG       ReadTimeouts;
  ULONG       WriteTimeouts;
  ULONG       ReadErrors;
  ULONG       WriteErrors;
  ULONG       ReadQueueLength;
  ULONG       MaxReadQ;
  ULONG       WriteQueueLength;
  ULONG       MaxWriteQ;
  ULONG       TimeBase;
  ULONG       OpenCnt;
  ULONG       PacketsSent;
  ULONG       BytesSent;
  ULONG       PacketsReceived;
  ULONG       BytesReceived;
} PROFI_STATISTICS;


/*****************************************************************************/
/* Structure for board feature access                                        */
/*****************************************************************************/


typedef struct _PROFI_BOARD_FEATURES
{
  BOOLEAN   bFeature;
  USHORT    Length;
  UCHAR     Features[128];
} PROFI_BOARD_FEATURES;


#define DBGRECNUM  (ULONG)400
#define DBGRECSIZE (ULONG)128



///////////////////////////////////////////////////////////////////////////////
// config resource structure using IOCTL IOCTL_PROFIPNP_GET_DEVICE_CONFIGURATION
//
typedef struct  _PROFIPNP_CONFIGURATION
{
  USHORT                 PnpNumber;

  // type of board and  number of board channels
  ULONG                  BoardType;
  ULONG                  BoardChannels;

  // PnP Configuration information,
  ULONG                  Bus;
  ULONG                  SlotSocket;

  ULONG                  MemoryStart[3];
  ULONG                  MemoryLength[3];

  UCHAR                  InterruptLevel;
  ULONG                  InterruptVector;
  ULONG                  InterruptAffinity;
  ULONG                  InterruptMode;
  BOOLEAN                bInterruptIsShared;

} PROFIPNP_CONFIGURATION,*PPROFIPNP_CONFIGURATION;




///////////////////////////////////////////////////////////////////////////////
// config resource structure using IOCTL IOCTL_PROFI_GET_DEVICE_CONFIGURATION
//
typedef struct _BOARD_CONFIGURATION
{
  ULONG     BoardNumber;

  ULONG     BoardType;
  ULONG     BoardChannel;
  ULONG     RtxSupported;
  ULONG     OperationMode;

  ULONG     IsaIoPort;
  ULONG     IsaPhysicalDpramAddr;
  ULONG     IsaInterrupt;

  ULONG     PciBusNumber;
  ULONG     PciDeviceNumber;
} PROFI_BOARD_CONFIGURATION, *PPROFI_BOARD_CONFIGURATION;


///////////////////////////////////////////////////////////////////////////////
// commands for PROFIPNP driver using IOCTL IOCTL_PROFIPNP_DEVICE_COMMAND
//
typedef enum _PROFIPNP_COMMANDS
{
  PROFIPNP_ENABLE_IRQ   = 0x00000000,
  PROFIPNP_DISABLE_IRQ,
} PROFIPNP_COMMANDS;



///////////////////////////////////////////////////////////////////////////////
// Board configuration status codes used by ControlPanel
//
typedef enum _ERR_CFG
{
  CFG_OK                        = 0x00000000,      // Board is configurated successfully
  ERR_CFG_READ_BOARD_PARAMETERS,                   // Error reading board parameters from registry
  ERR_CFG_READ_FW_PARAMETERS,                      // Error reading firmware parameters from registry
  ERR_CFG_NO_BOARD,                                // Board not found

  ERR_CFG_NO_FREE_IRQ_FOUND,                       // No free (valid) IRQ found
  ERR_CFG_INVALID_IRQ,                             // Configurated IRQ can not be used
  ERR_CFG_MAP_IRQ,                                 // IRQ mapping error

  ERR_CFG_NO_FREE_DPR_FOUND,                       // No free (valid) IRQ found
  ERR_CFG_INVALID_DPR,                             // Configurated IRQ can not be used
  ERR_CFG_MAP_DPR,                                 // IRQ mapping error

  ERR_CFG_DPR_IRQ_IN_USE,                          // Configurated IRQ or DPR already in use

  ERR_CFG_INVALID_PORT,                            // No Board found at Configurated IO-Port
  ERR_CFG_MAP_PORT,                                // IO-Port mapping error
  ERR_CFG_PORT_IN_USE,                             // Configurated IO-Port already in use

  ERR_CFG_START_FIRMWARE,                          // PROFIBUS firmware not running
  ERR_CFG_DOWNLOAD_FIRMWARE,                       // Download PROFIBUS firmware error
  ERR_CFG_READ_FIRMWARE_FILES,                     // PROFIBUS firmware for download not found


  ERR_CFG_FMS_DPV1_MASTER_MODE_NOT_SUPPORTED,      // Board does not support FMS/DPV1 Master operation mode
  ERR_CFG_DPS_MODE_NOT_SUPPORTED,                  // Board does not support DP Slave operation mode
  ERR_CFG_ANALYZER_MODE_NOT_SUPPORTED,             // Board does not support Analyzer operation mode
  ERR_CFG_PROFIBUS_MODE_NOT_SUPPORTED,             // Board does not support PROFIBUS operation mode

  ERR_CFG_INIT_CARD_SERVICES,                      // Initializing of PC Card Services failed
  ERR_CFG_BAD_PCCARDNT_RESOURCES,                  // Bad resources enabled for PROFIcard NT
  ERR_CFG_PCCARDNT_ALREADY_EXIST,                  // More than one PROFIcard NT configured


  ERR_CFG_NO_DRIVER_MEMORY,                        // Not enough system memory available
  ERR_CFG_CREATE_DEVICE,                           // Can not create logical device(s)
  ERR_CFG_SYMBOLIC_LINK_NAME,                      // Can not create symblic link name

  ERR_CFG_MAX_NO_BOARDS_OVERFLOW,                  // Max Number of Boards overflow

  ERR_CFG_INIT_TCPIP,                              // cannot initialize TCP/IP
  ERR_CFG_PROFIGATE_CONFIG_STRING,                 // cannot read config string from PROFIgate
  ERR_CFG_PROFIGATE_DEVICE_SERVER,                 // cannot connect to PROFIgate device server
  ERR_CFG_PROFIGATE_OPEN_DEVICES,                  // cannot open remote PROFIgate devices
  ERR_CFG_PROFIGATE_COMMUNICATION,                 // no communication with PROFIgate
  ERR_CFG_PROFIGATE_CONNECT,                       // cannot connect to PROFIgate

} T_ERR_CFG;

#endif


