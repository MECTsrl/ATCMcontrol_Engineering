/*####COPYRIGHTBEGIN####
 -------------------------------------------
 Copyright (C) 2003 Chris Gurtler

 -------------------------------------------
####COPYRIGHTEND####*/

/* =======================================================================
    common_struct.h     BACnet Structures

    Chris Gurtler

*/

#ifndef _common_struct_h_
#define _common_struct_h_

#include "common_enum.h"
#include <memory.h>

//Callback routine for logging messages
typedef void (*log_fn) (int, char*);

//Callback routines for locking/unlocking
typedef void (*lock_fn)(char*, int);
typedef void (*unlock_fn)(char*, int);

#define ZERO_STRUCT(s) memset((void*) &s, 0, sizeof(s))

typedef unsigned char BYTE;

#ifndef FALSE
  #define FALSE 0
#endif

#ifndef TRUE
  #define TRUE  1
#endif

/*  --- Structures ------------------------------------------------- */

typedef struct bacnet_packet
{
	BYTE *message;
	int length;
}bacnet_packet;

typedef struct bacnet_mac
{
	BYTE contents[6];
	BYTE length;
}bacnet_mac;

typedef struct bacnet_address
{
	int network;
	bacnet_mac mac;
}bacnet_address;

typedef struct address_binding
{
	int device_id;
	bacnet_address address;
}address_binding;

typedef struct bacnet_confirmed_request_pdu
{
    BYTE serviceChoice;
    BYTE segmentedMessage; //0 if not segmented
    BYTE moreFollows; //0 if no more follows
    BYTE segmentedResponseAccepted; //0 if segmented response not accepted
    BYTE maxAPDUresponseLengthAccepted; //Accepted values by index are :- 50,128,206,480,1024,1476
    BYTE invokeID;
    BYTE sequenceNumber;
    BYTE proposedWindowSize;
}bacnet_confirmed_request_pdu;

typedef struct bacnet_unconfirmed_request_pdu
{
    BYTE serviceChoice;
}bacnet_unconfirmed_request_pdu;

typedef struct bacnet_simple_ack_pdu
{
    BYTE serviceChoice;
    BYTE invokeID;
}bacnet_simple_ack_pdu;

typedef struct bacnet_complex_ack_pdu
{
    BYTE segmentedMessage;
    BYTE moreFollows;
    BYTE invokeID;
    BYTE sequenceNumber;
    BYTE proposedWindowSize;
    BYTE serviceChoice;
}bacnet_complex_ack_pdu;

typedef struct bacnet_segment_ack_pdu
{
    BYTE negativeACK;
    BYTE server;
    BYTE originalInvokeID;
    BYTE sequenceNumber;
    BYTE actualWindowSize;
}bacnet_segment_ack_pdu;

typedef struct bacnet_error_pdu
{
    BYTE originalInvokeID;
    BYTE serviceChoice;
}bacnet_error_pdu;

typedef struct bacnet_reject_pdu
{
    BYTE originalInvokeID;
    BYTE rejectReason;
}bacnet_reject_pdu;

typedef struct bacnet_abort_pdu
{
    BYTE server;
    BYTE originalInvokeID;
    BYTE abortReason;
}bacnet_abort_pdu;

//The BACnet APDU structure is used by both the client and server tsm's
//Messages from the network layer are decoded into the APDU structure and then put
//onto the appropriate client or server tsm stack
typedef struct bacnet_pdu
{
    BACnetPDUType type;
    union pduTypes
    {
        bacnet_confirmed_request_pdu confirmedRequest;
        bacnet_unconfirmed_request_pdu unConfirmedRequest;
        bacnet_simple_ack_pdu simpleAck;
        bacnet_complex_ack_pdu complexAck;
        bacnet_segment_ack_pdu segmentAck;
        bacnet_error_pdu errorPDU;
        bacnet_reject_pdu rejectPDU;
        bacnet_abort_pdu abortPDU;
    }_type;
	bacnet_packet apdu; //Buffer to the APDU portion of the message
}bacnet_pdu;

typedef struct application_layer_request
{
	bacnet_address source;	// Source Address, used by the subscribe COV request
	bacnet_address address; //address of the requested device
	bacnet_pdu pdu;  //Structure for holding the pdu
}application_layer_request;

typedef struct bacnet_logging
{
	/*!Pointer to a callback function for logging. The callback function will be fired when a message from the BACnet Server is received.*/
	log_fn log;
	/*!Name of the logging file to use by the BACnet server for logging messages to a file.*/
	char *file_name;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE application_layer;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE network;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE client_tsm;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE server_tsm;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE service_indication;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE address_binding;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE ethernet;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE anex_h;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE ip;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE mstp;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE ptp;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE api;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE datalink;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE datatype_utils;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE driver;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE mem;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE object;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE cache;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE mac;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE object_api;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE serialize;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE pdu;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE lua;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE database;
	/*!A Flag for activating messages from this module, zero if logging is inactive.*/
	BYTE lib_module;
}bacnet_logging;
/*! \struct bacnet_logging
 * BACnet Logging Structure.
 *
 * The initialisation structure is used to hold logging information when
   the BACnet Server is created.
 */


typedef struct link_layer_init
{
	/*!
	The Connect String is a string of value name pairs seperated by a semi colon that is used by the BACnet
	Server to initialize the Data Link Layer. The string must contain a type
	parameter to indicate which link layer will be created as follow:<br>
	<table>
	<tr><td>TYPE=BACNETIP</td><td>BACnet/IP</td></tr>
	<tr><td>TYPE=BACNETANEXH</td><td>Anex H</td></tr>
	<tr><td>TYPE=BACNETETHERNET</td><td>BACnet Ethernet</td></tr>
	<tr><td>TYPE=BACNETMSTP</td><td>BACnet MSTP</td></tr>
	<tr><td>TYPE=BACNETPTP</td><td>BACnet PTP</td></tr>
	</table><br>
	<b>Network Number</b><br>
	The connect string must contact a network name value pair to indicate
	the network number that the port is connected to, eg. NETWORK=1.<br>
	<b>BACnet/IP</b><br>
	The following parameters can be set in the BACnet/IP connect string.<br>
	<table>
	<tr><td>UDPPORT</td><td>An integer value that represents the BACnet UDP Port. The default is 47808.</td></tr>
	<tr><td>ETH</td><td>The ethernet number used by Linux only. the default is 0 which indicates eth0</td></tr>
	<tr><td>BBMDIPADDRESS</td><td>The address of the BBMD server if this device is acting as a foreign device. It must be in dotted format eg. 192.168.0.1</td></tr>
	<tr><td>BBMDPORTNO</td><td>The UDP Port of the BBMD Server. The default is 47808</td></tr>
	<tr><td>BBMDMAXFOREIGNDEVICES</td><td>If this device is to act as a BBMD server, then this value must be non zero. The default is zero. Note :- There can be only 1 BBMD per IP subnet, do not use this option unless required.</td></tr>
	</table><br>
	<b>BACnet Ethernet</b><br>
	The following parameters can be set in the BACnet Ethernet connect string.<br>
	<table>
	<tr><td>NIC</td><td>On Linx, this represent the Ethernet Port where 0 = eth0. For Windows, this represents the ethernet adapter number, normally there is only 1 adapter in a Windows PC.</td></tr>
	</table><br>
	<b>Anex H</b><br>
	There are no additional parameters available.<br>
	<b>MSTP</b><br>
	The following parameters can be set in the MSTP connect string.<br>
	<table>
	<tr><td>PORT</td><td>The com port number. eg 1 = com1</td></tr>
	<tr><td>BAUDRATE</td>The baudrate. eg 9600<td></td></tr>
	<tr><td>TS</td><td>This Slot or MAC address. Default = 0.</td></tr>
	<tr><td>SLOT</td><td>Default = 10, do not change.</td></tr>
	<tr><td>MAX_INFO_FRAMES</td><td>Default = 50, do not change.</td></tr>
	<tr><td>MAX_MASTER</td><td>Default = 100, do not change.</td></tr>
	<tr><td>MIN_OCTETS</td><td>Default = 4, do not change.</td></tr>
	<tr><td>POLL</td><td>Default = 50, do not change.</td></tr>
	<tr><td>RETRY_TOKEN</td><td>Default = 1, do not change.</td></tr>
	<tr><td>REPLY_TIMEOUT</td>Default = 300, do not change.<td></td></tr>
	<tr><td>USEAGE_DELAY</td><td>Default = 50, do not change.</td></tr>
	<tr><td>USEAGE_TIMEOUT</td><td>Default = 100, do not change.</td></tr>
	</table><br>
	<b>PTP</b><br>
	The following parameters can be set in the PTP connect string.<br>
	<table>
	<tr><td>PORT</td><td>The com port number. eg 1 = com1</td></tr>
	<tr><td>BAUDRATE</td>The baudrate. eg 9600.<td></td></tr>
	<tr><td>DIALSTRING</td><td>A dialstring to send to the modem. eg ATDT5556781.</td></tr>
	<tr><td>PASSWORD</td><td>Password of the PTP router.</td></tr>
	<tr><td>CONNECT</td><td>1=Connect to PTP router.</td></tr>
	</table><br>
	*/
	char *connect_string;
	/*!If the Data Link Layer was unable to initialize an error message with a description of the connection error will be placed here by the BACnet Server.*/
	char error_message[128];
	/*!A Flag indicating that an error has occured with this data link layer.*/
	BYTE error_flag;
}link_layer_init;
/*! \struct link_layer_init
 * BACnet Logging Structure.
 *
 * The initialisation structure for holding the Data Link Layer initialisation
   information. Each Datalink Layer or port is a connection to the BACnet Network.
   This structure is part of the BACnetDriver structure which is passed
   to the BACnet Server when it is initialized.
 */

typedef struct datalink_init
{
	/*!An array of 128 Connect String Structures for initializing the Data Link Layers.*/
	link_layer_init connect_strings[128];

	/*If this flag is set then the driver will load the data link layers from the data base at startup.*/
	BYTE load_from_database;

	/*!Accepted values by index are :- 50,128,206,480,1024,1476.*/
    BYTE maxAPDUresponseLengthAccepted;

}datalink_init;
/*! \struct datalink_init
 * Datalink Layer Initialization Structure.
 *
 * The initialisation structure for holding the up to 128
   Data Link Layer Connect Strings.
   Each Datalink Layer or port is a connection to the BACnet Network.
   This structure is part of the BACnetDriver structure which is passed
   to the BACnet Server when it is initialized.
 */

#endif


