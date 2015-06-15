
// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the BACNAPI_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// BACNAPI_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifndef BACNAPI_H
#define BACNAPI_H

#if WIN32|WINNT
#ifdef NO_EXPORTS_OR_IMPORTS
#define BACNAPI_API
#else
#ifdef BACNAPI_EXPORTS
#define BACNAPI_API __declspec(dllexport)
#else
#define BACNAPI_API __declspec(dllimport)
#endif
#endif
#else
#define BACNAPI_API
#endif


typedef struct nic_adapter
{
	int nic;
	char name[256];
	char description[256];
}nic_adapter;

#ifdef __cplusplus
extern "C"
{
#endif

#include "s_driver.h"

BACNAPI_API void bacnapi_set_signal_handler(signal_handler_fn fn);

/*
typedef void (*bacnapi_analog_object_cov_notification_fn)(unsigned int, unsigned int, int, int, int, float, int);
typedef void (*bacnapi_binary_object_cov_notification_fn)(unsigned int, unsigned int, int, int, int, unsigned int, int);
*/


/** @defgroup FunctionReference Reference
 *  The API Reference is broken up into sections. The Initialisation routines must be used
 	to start/stop the BACnet Driver, once Started the driver can be used to act as a client
 	or to act as a server. The BACnet Service Requests can be used to issue standard BACnet
 	services to any device on the network, the services are grouped into Confirmed and Unconfirmed Requests.
 	The Cache Routines are not part of the BACnet Specification, they can be used to read/write to BACnet Devices.
 	The Object Access Routines not part of the BACnet Specification and are intended for Gateway Applications.
 * @{
*/

/** @defgroup InitRoutines Initialization Routines
 * The Initialisation routines must be used
   to start/stop the BACnet Driver, once started the driver can be used to act as a client
   or to act as a server.
 * @{
*/
BACNAPI_API int bacnapi_load_from_command_line(int argc, char* argv[], BACnetDriver *driver, int *daemon_flag, int *simulator_enable_flag, int *trace_flag);
BACNAPI_API int bacnapi_load_pid_from_file(int *pid);
BACNAPI_API int bacnapi_create_pid_file(int pid);
BACNAPI_API int bacnapi_remove_pid_file();

/**  Initialization of the BACnet Server.
	 This function must be called before the BACnet Server can be used.
	 The driver parameter holds the BACnetServerInit structure for
	 Initializing the BACnet Server.
*/
BACNAPI_API int bacnapi_init(BACnetDriver *driver);
/**
	Destroy the BACnet Server. This function must be called when the application
	exits to free all memory used by the application.
*/
BACNAPI_API int bacnapi_destroy();

/**
	Inserts a new Data Link Layer, the
	Connect String is a string of value name pairs seperated by a semi colon that is used by the BACnet
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
BACNAPI_API int bacnapi_insert_datalink_layer(char* connect_string);

/**
	Removes a datalink layer from the driver.
*/
BACNAPI_API int bacnapi_remove_datalink_layer(int network);


/**  Deactivates the 30 minute .
	 If this function is not called, the driver will act in evaluation mode.
	 Evaluation mode will restrict the driver to operate for 30 minutes.
*/
BACNAPI_API void bacnapi_set_evaluation_fn(void *e);

/**  Displays the registration dialog box and an evaluation dialog box.
	 This can be used to create a password protected application and give it a custom evaluation
	 message. If NULL strings are passed through to this function, then the evaluation dialog will display a default message.
	 The Registration Dialog Box will allow the user to enter a Registration Code, if the code is correct, then the user will not see
	 the Registration/Evaluation Dialogs again. The registration code works only with a matching Serial Number and must be
	 obtained from SCADA Engine, the serial number will be different on each PC.
*/
BACNAPI_API void bacnapi_display_registration_dialog(char *eval_heading, char *eval_body, char *eval_footer);
/**  Returns the Serial Number for the BACnet Server dll.
*/
BACNAPI_API int bacnapi_get_serial_number(char *serial_number);
/**  Registers the PC to use the BACnet Server dll.
*/
BACNAPI_API int bacnet_api_register(char * regcode);
/**
	 This function returns the device id of the BACnet Server. The Device ID is set in the initialisation
	 structure of the bacnapi_init function call which must be called before this function can be called.
*/
BACNAPI_API int bacnapi_device_id();
/**
	 This function returns the local network number of the BACnet Server. The Network Number is set in the initialisation
	 structure of the bacnapi_init function call which must be called before this function can be called.
	 Note :- If more than 1 port was configured, the local network number returns the number of the first port found.
*/
BACNAPI_API int bacnapi_network();

/**
	 This function is used to disable message boxes and dialog boxes from appearing.
*/
BACNAPI_API void bacnapi_set_quiet_mode(int quiet);

BACNAPI_API int set_project_path(char* id, char* path, char* password);
BACNAPI_API int bacnapi_execute_query(char *query_string);
BACNAPI_API int bacnapi_get_recordset(char **xml, char *query_string);
BACNAPI_API int bacnapi_load_config(BACnetDriver *driver, char* file_name, int *daemon_flag, int *simulator_enable_flag, int *trace_flag);
BACNAPI_API int bacnapi_save_config(BACnetDriver *driver, char* file_name);
BACNAPI_API int bacnapi_edit_ports(char* file_name);
BACNAPI_API int bacnapi_edit_device(char* file_name);
BACNAPI_API void bacnapi_destroy_connect_strings(datalink_init *datalink);
BACNAPI_API void bacnapi_destroy_connect_string(datalink_init *datalink, int index);
BACNAPI_API void* bacnapi_allocate_connect_string(datalink_init *datalink, int index, int size);
BACNAPI_API void bacnapi_destroy_driver_initilization_struct(BACnetDriver *driver);
BACNAPI_API void bacnapi_log_message(int priority, char* message);
BACNAPI_API int bacnapi_config_file_exists();
BACNAPI_API int bacnapi_api_connection_qty();

/*@}*/

/** @defgroup BACnetServicesRequests BACnet Service Requests
 *
 The BACnet Specification allows devices on the Network to communicate using BACnet Services.
 The Services are broken up into Confirmed and Unconfirmed Requests. Confirmed Requests
 require a response back, while Unconfirmed Requests do not expect a reply. The BACnet Server API
 can act as a client and communicate to any device on the BACnet Network by sending BACnet Services.
 BACnet Confirmed Services are executed Synchronously, the function will return
 when the request completes, or a timeout takes place. Unconfirmed Requests return
 immediately, there is no response associated with an Unconfirmed request and no
 guarantee that the request will complete.
 * @{
*/

/** @defgroup SyncServices Confirmed Requests
 *
 Synchronous requests return when the service has completed, which will be after
 a response has been returned or a timeout occurs. Each Service is passed
 a structure that contains the service information. The service structure
 must be populated with a valid request otherwise the service will return an error
 when it is executed. The service structure requires a device id as a paramater, if
 the device id is not known, the BACnet Server will attempt to locate the device using
 a whoIs request.
 * @{
*/

/**
	 This function executes a BACnet acknowledgeAlarm service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int acknowledge_alarm(BACnetServiceAcknowledgeAlarm *service);
/**
	 This function executes a BACnet confirmedCOVNotification service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int confirmed_cov_notification(BACnetServiceConfirmedCOVNotification *service);
/**
	 This function executes a BACnet confirmedEventNotification service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int confirmed_event_notification(BACnetServiceConfirmedEventNotification *service);
/**
	 This function executes a BACnet getAlarmSummary service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int get_alarm_summary(BACnetServiceGetAlarmSummary *service);
/**
	 This function executes a BACnet getEnrolmentSummary service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int get_enrolment_summary(BACnetServiceGetEnrolmentSummary *service);
/**
	 This function executes a BACnet subscribeCOV service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int subscribe_cov(BACnetServiceSubscribeCOV *service);
/**
	 This function executes a BACnet atomicReadFile service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int atomic_read_file(BACnetServiceAtomicReadFile *service);
/**
	 This function executes a BACnet atomicWriteFile service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int atomic_write_file(BACnetServiceAtomicWriteFile *service);
/**
	 This function executes a BACnet addListElement service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int add_list_element(BACnetServiceAddListElement *service);
/**
	 This function executes a BACnet removeListElement service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int remove_list_element(BACnetServiceRemoveListElement *service);
/**
	 This function executes a BACnet createObject service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int create_object(BACnetServiceCreateObject *service);
/**
	 This function executes a BACnet deleteObject service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int delete_object(BACnetServiceDeleteObject *service);
/**
	 This function executes a BACnet readProperty service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int read_property(BACnetServiceReadProperty *service);
/**
	 This function executes a BACnet readPropertyConditional service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int read_property_conditional(BACnetServiceReadPropertyConditional *service);
/**
	 This function executes a BACnet readPropertyultiple service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int read_property_multiple(BACnetServiceReadPropertyMultiple *service);
/**
	 This function executes a BACnet writeProperty service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int write_property(BACnetServiceWriteProperty *service);
/**
	 This function executes a BACnet writePropertyMultiple service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int write_property_multiple(BACnetServiceWritePropertyMultiple *service);
/**
	 This function executes a BACnet deviceCommunicationControl service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int device_communication_control(BACnetServiceDeviceCommunicationControl *service);
/**
	 This function executes a BACnet confirmedPrivateTransfer service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int confirmed_private_transfer(BACnetServiceConfirmedPrivateTransfer *service);
/**
	 This function executes a BACnet confirmedTextMessage service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int confirmed_text_message(BACnetServiceConfirmedTextMessage *service);
/**
	 This function executes a BACnet reinitializeDevice service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int reinitialize_device(BACnetServiceReinitializeDevice *service);
/**
	 This function executes a BACnet authenticate service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int authenticate(BACnetServiceAuthenticate *service);
/**
	 This function executes a BACnet requestKey service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int request_key(BACnetServiceRequestKey *service);
/**
	 This function executes a BACnet readRange service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int read_range(BACnetServiceReadRange *service);
/**
	 This function executes a BACnet lifeSafetyOperation service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int life_safety_operation(BACnetServiceLifeSafetyOperation *service);
/**
	 This function executes a BACnet subscribeCOVProperty service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int subscribe_cov_property(BACnetServiceSubscribeCOVProperty *service);
/**
	 This function executes a BACnet getEventInformation service from the BACnet Server.
	 It will return when the request has completed.
	 The device id must be set in the service structure,
	 the BACnet Server will attempt to 	 locate the device if it is unknown
	 using a whoIs request.
*/
BACNAPI_API int get_event_information(BACnetServiceGetEventInformation *service);

/*@}*/


/** @defgroup UnconfirmedRequest Unconfirmed Requests
 Unconfirmed requests return immediately and are executed by the BACnet Server.
 Each service requires a service structure to be passed to the service.
 The Service structure should be populated with the parameters required and
 cleared after the service has been executed.
 *@{
*/

/**
	 This function executes a BACnet Iam service from the BACnet Server. It will return
	 immediately. The service paramaters are stored in a structure that must
	 be populated before the service is called. This service will be called
	 automatically by the BACnet Server in response to an Iam request.
*/
BACNAPI_API int bacnapi_execute_iam(BACnetServiceIam *service);
BACNAPI_API int bacnapi_execute_iam_ex(bacnet_address *source_address, BACnetServiceIam *service);
/**
	 This function executes a BACnet Ihave service from the BACnet Server. It will return
	 immediately. The service paramaters are stored in a structure that must
	 be populated before the service is called. This service will be called
	 automatically by the BACnet Server in response to an whoHas request.
*/
BACNAPI_API int bacnapi_execute_Ihave(BACnetServiceIHave *service);
/**
	 This function executes a BACnet unconfirmedCOVNotification service from the BACnet Server.
	 It will return immediately. The service paramaters are stored in a structure that must
	 be populated before the service is called. This service will be called
	 automatically by the BACnet Server in response when an Object has changed state.
*/
BACNAPI_API int bacnapi_execute_unconfirmed_cov_notification(BACnetServiceCOVNotificationRequest *service);
/**
	 This function executes a BACnet unconfirmedEventNotification service from the BACnet Server. It will return
	 immediately. The service paramaters are stored in a structure that must
	 be populated before the service is called. This service will be called
	 automatically by the BACnet Server when an alarm is generated.
*/
BACNAPI_API int bacnapi_execute_unconfirmed_event_notification(BACnetServiceEventNotificationRequest *service);
/**
	 This function executes a BACnet unconfirmedPrivateTransfer service from the BACnet Server. It will return
	 immediately. The service paramaters are stored in a structure that must
	 be populated before the service is called.
*/
BACNAPI_API int bacnapi_execute_unconfirmed_private_transfer(BACnetServiceUnconfirmedPrivateTransfer *service);
/**
	 This function executes a BACnet unconfirmedTextMessage service from the BACnet Server. It will return
	 immediately. The service paramaters are stored in a structure that must
	 be populated before the service is called.
*/
BACNAPI_API int bacnapi_execute_unconfirmed_text_message(BACnetServiceUnconfirmedTextMessage *service);
/**
	 This function executes a BACnet timeSynchronistation service from the BACnet Server. It will return
	 immediately. The service paramaters are stored in a structure that must
	 be populated before the service is called.
*/
BACNAPI_API int bacnapi_execute_time_synchronization(BACnetServiceTimeSynchronization *service);
/**
	 This function executes a BACnet whoHas service from the BACnet Server. It will return
	 immediately. The service paramaters are stored in a structure that must
	 be populated before the service is called. If the high and low limit
	 parameters are not required, they should be set to -1;
*/
BACNAPI_API int bacnapi_execute_who_has(BACnetServiceWhoHas *service);
/**
	 This function executes a BACnet whoIs service from the BACnet Server. It will return
	 immediately. The service paramaters are stored in a structure that must
	 be populated before the service is called. If the high and low limit
	 parameters are not required they should be set to -1.
*/
BACNAPI_API int bacnapi_execute_who_is(BACnetServiceWhoIs *service);
/**
	 This function executes a BACnet utcTimeSynchronistation service from the BACnet Server. It will return
	 immediately. The service paramaters are stored in a structure that must
	 be populated before the service is called.
*/
BACNAPI_API int bacnapi_execute_utc_time_synchronization(BACnetServiceUtcTimeSynchronization *service);
/*@}*/
/*@}*/

/** @defgroup ObjectRoutines Object Access Routines
 *
	<p>
 	The Object Access Routines provide an optimised way of accessing both "Internal" and
	"External" BACnet Objects. Internal Objects are stored inside the BACnet Server API
	and are visible to any BACnet Device on the network. External Objects reside inside
	other BACnet devices on the network.
	</p><p>
	When accessing an Internal BACnet Object, the BACnet Server API accesses the Object
	directly without sending a BACnet Service. When Accessing an External Object the
	BACnet Service depends upon the capabilities of the device and the service
    used could be readProperty, readPropertyMultiple, subscribeCOV or subscribeCOVProperty.
	</p><p>
	The Object Access Routines will attempt to read the Status Flags from the object
	if they exist which provides additional information about the object.
	</p><p>
	In the case of Internal Objects, some properties such as the relinquishDefault and
	reliability are read only. These read only properties can only be updated by the BACnet
	Server API.
	</p>
 * @{
*/

/** @defgroup BACnetServerObjectAccess BACnet Object Reference
 * 
 BACnet Object Reference 
 *@{
 */
/*@}*/

/**
Used to construct a failure code from the BACnetErrorClass and BACnetErrorCode.
*/
#define BACNAPI_FAILURE(a,b) (0x80000000 | ((0xFFFF & a) << 16) | (0xFFFF & b))
/**
Used to extract the BACnetErrorClass from the failure code used in the object access routines.
*/
#define BACNAPI_FAILURE_ERROR_CLASS(a) ((0x7FFF0000 & a) >> 16)
/**
Used to extract the BACnetErrorCode from the failure code used in the object access routines.
*/
#define BACNAPI_FAILURE_ERROR_CODE(a) (0xFFFF & a)

/**
	Creates a Virtual Network. This is used for creating Virtual Devices.
*/
BACNAPI_API int bacnapi_insert_virtual_network(int network);

/**
	Deletes a Virtual Network. This function will remove all Virtual Devices
	and associated local objects from the driver.
*/
BACNAPI_API int bacnapi_delete_virtual_network(int network);

/**
	Creates one or more BACnet Virtual Device.

	Creates a quantity of Virtual Devices inside the BACnet Server using the array of connect strings
	If the device already exists then no action is taken.
	If the device does not exist then the default values are loaded into the object.
	A new Program Object and File Object is created for each virtual device.
	Returns 0 on success.<br>
	The format for each connect string is "[Network]; [Device ID]; [Name];" where each parameter
	is seperated by a semi colon. The Network Number must correspond to an 
	existing Virtual Network. The Device ID must be unique to the network.

*/
BACNAPI_API int bacnapi_insert_virtual_device(char** connect_strings, int* error_code, int qty);

/**
	Deletes one or more BACnet Virtual Device. This function will remove
	the Virtual device and all associated local objects.<br>

	Deletes a quantity of Virtual Devices inside the BACnet Server using the array of connect strings
	If the device does not exist then an error will be returned. Returns 0 on success.<br>
	The format for each connect string is "[Device ID];" where each parameter
	is seperated by a semi colon.
*/
BACNAPI_API int bacnapi_delete_virtual_device(char** connect_strings, int* error_code, int qty);

/**
	Creates one or more BACnet Internal Objects

	Creates a quantity of internal objects inside the BACnet Server using the array of connect strings
	If the object already exists then no action is taken.
	If the object does not exist then the default values are loaded into the object
	Returns 0 on success.

	The Format for the connect string depends upon the object type required, as a minumum the
	string is :- "[Device ID]; [ObjectType]; [Instance]; [Name];" where each parameter is seperated
	by a colon. The following table displays the connect string for each object type. All
	parameters following the Instance are optional.<br>
<table>
<tr><td><b>Object Type</b></td><td><b>Connect String</b></td></tr>
<tr><td>Analog Input</td><td>[Device ID]; [ObjectType]; [Instance]; [Name]; [NotificationClass]; [TimeDelay]; [LowLimit]; [HighLimit]; [Deadband]; [updateInterval]; [minPresValue]; [maxPresValue]; [covIncrement]; [resolution];</td></tr>
<tr><td>Analog Output</td><td>[Device ID]; [ObjectType]; [Instance]; [Name]; [NotificationClass]; [TimeDelay]; [LowLimit]; [HighLimit]; [Deadband]; [minPresValue]; [maxPresValue]; [covIncrement]; [resolution];</td></tr>
<tr><td>Analog Value</td><td>[Device ID]; [ObjectType]; [Instance]; [Name]; [NotificationClass]; [TimeDelay]; [LowLimit]; [HighLimit]; [Deadband]; [covIncrement];</td></tr>
<tr><td>Binary Input</td><td>[Device ID]; [ObjectType]; [Instance]; [Name]; [InactiveText]; [ActiveText]; [NotificationClass]; [TimeDelay]; [AlarmValue]; [Polarity];</td></tr>
<tr><td>Binary Output</td><td>[Device ID]; [ObjectType]; [Instance]; [Name]; [InactiveText]; [ActiveText]; [NotificationClass]; [TimeDelay]; [Polarity]; [minimumOffTime]; [minimumOnTime];</td></tr>
<tr><td>Binary Value</td><td>[Device ID]; [ObjectType]; [Instance]; [Name]; [InactiveText]; [ActiveText]; [NotificationClass]; [TimeDelay]; [AlarmValue]; [minimumOffTime]; [minimumOnTime];</td></tr>
<tr><td>Calendar</td><td>[Device ID]; [ObjectType]; [Instance]; [Name];</td></tr>
<tr><td>Command</td><td>[Device ID]; [ObjectType]; [Instance]; [Name];</td></tr>
<tr><td>EventEnrolment</td><td>[Device ID]; [ObjectType]; [Instance]; [Name];</td></tr>
<tr><td>Group</td><td>[Device ID]; [ObjectType]; [Instance]; [Name];</td></tr>
<tr><td>Loop</td><td>[Device ID]; [ObjectType]; [Instance]; [Name]; [controlledObjectType]; [controlledObjectInstance]; [manipulatedObjectType]; [manipulatedObjectInstance]; [setpointObjectType]; [setpointObjectInstance]; [proportionalConstant]; [integralConstant]; [derivativeConstant]; [minimumOutput]; [maximumOutput]; [NotificationClass]; [TimeDelay]; [ErrorLimit]; </td></tr>
<tr><td>MultistateInput</td><td>[Device ID]; [ObjectType]; [Instance]; [Name]; [NumberOfStates]; [State 1; .. State n]; [NotificationClass]; [TimeDelay]; [NumberOfAlarms]; [Alarm 1; .. Alarm n]; [NumberOfFaults]; [Fault 1; .. Fault n]; </td></tr>
<tr><td>MultistateOutput</td><td>[Device ID]; [ObjectType]; [Instance]; [Name]; [NumberOfStates]; [State 1; .. State n]; [NotificationClass]; [TimeDelay]; </td></tr>
<tr><td>MultistateValue</td><td>[Device ID]; [ObjectType]; [Instance]; [Name]; [NumberOfStates]; [State 1; .. State n]; [NotificationClass]; [TimeDelay]; [NumberOfAlarms]; [Alarm 1; .. Alarm n]; [NumberOfFaults]; [Fault 1; .. Fault n]; </td></tr>
<tr><td>NotificationClass</td><td>[Device ID]; [ObjectType]; [Instance]; [Name];[NumberOfRecipients]; [Recipient 1; .. Recipient n]</td></tr>
<tr><td>Schedule</td><td>[Device ID]; [ObjectType]; [Instance]; [Name];</td></tr>
<tr><td>Averaging</td><td>[Device ID]; [ObjectType]; [Instance]; [Name]; [ReferenceDeviceID]; [ReferenceObjectType]; [ReferenceObjectInstance]; [ReferenceProperty]; [ReferenceArrayIndex]; [windowInterval]; [windowSamples];</td></tr>
<tr><td>TrendLog</td><td>[Device ID]; [ObjectType]; [Instance]; [Name]; [BufferSize]; [logEnable]; [logInterval]; [stopWhenFull]; [notificationThreshold]; [ReferenceDeviceID]; [ReferenceObjectType]; [ReferenceObjectInstance]; [ReferenceProperty]; [ReferenceArrayIndex]; [notificationClass];</td></tr>
<tr><td>LifeSafetyPoint</td><td>[Device ID]; [ObjectType]; [Instance]; [Name]; [NumberOfAcceptedModes]; [Accepted Mode 1; .. Accepted Mode n]; [NotificationClass]; [TimeDelay]; [NumberOfLifeSafetyAlarms]; [Alarm 1; .. Alarm n]; [NumberOfAlarms]; [Alarm 1; .. Alarm n]; [NumberOfFaults]; [Fault 1; .. Fault n]; </td></tr>
<tr><td>LifeSafetyZone</td><td>[Device ID]; [ObjectType]; [Instance]; [Name]; [NumberOfAcceptedModes]; [Accepted Mode 1; .. Accepted Mode n]; [NotificationClass]; [TimeDelay]; [NumberOfLifeSafetyAlarms]; [Alarm 1; .. Alarm n]; [NumberOfAlarms]; [Alarm 1; .. Alarm n]; [NumberOfFaults]; [Fault 1; .. Fault n]; </td></tr>
<tr><td>Accumulator</td><td>[Device ID]; [ObjectType]; [Instance]; [Name]; [PrescaleMultiplier]; [PrescaleModuloDivide]; [NotificationClass]; [TimeDelay]; [LowLimit]; [HighLimit]; [LimitMonitoringInterval]; </td></tr>
<tr><td>PulseConverter</td><td>[Device ID]; [ObjectType]; [Instance]; [Name]; [inputReferenceInstance]; [scaleFactor]; [NotificationClass]; [TimeDelay]; [LowLimit]; [HighLimit]; [Deadband]; </td></tr>
</table>

*/
BACNAPI_API int bacnapi_insert_object(char** connect_strings, int* error_code, int qty);

/**
	Deletes one or more BACnet Internal Objects

	Deletes a quantity of internal objects inside the BACnet Server using the array of connect strings
	If the object does not exist then an error will occur.
	Returns 0 on success.<br>

	The Format for the connect string is :- <br>
	"[Device ID];[ObjectType];[Instance];"<br>
	where each parameter is seperated by a colon.<br>

*/
BACNAPI_API int bacnapi_delete_object(char** connect_strings, int* error_code, int qty);

/**
Performs a read request for a single property from a BACnet Object to return the value
of the property and status flags. The request can be syncronous in  which case the
function will not return until the service has completed. If asyncronous operation
is selected, then the return result will initially be Not Updated. Both internal
and external objects can be read using this request. If the object is in an external device
then the API will attempt to locate the device using the whoIs service, read it's capabilities
and then execute the appropriate service. Depending on the device capabilities the BACnet service
used could be readProperty, readPropertyMultiple, subscribeCOV or subscribeCOVProperty.
If the object is an internal object, then it will be accessed directly from the API without issuing a BACnet service.
</p><p>
<b>Inputs</b>
<table>
<tr><td>reference</td><td>Reference Identifier to the object being read.</td></tr>
<tr><td>value</td><td>The returned value from the object. Note - bacnapi_ClearBACnetDataType() must be called to remove any memory allocated by this call.</td></tr>
<tr><td>status_flags</td><td>The returned status flags from the object. See below for further details</td></tr>
<tr><td>async</td><td>If this is set to true, then the API will wait for a response or timeout before returning a result.</td></tr>
</table>
<p>
<b>Result</b><br>
The result will be 0 if no failure occurs during the read request, the resulting failure
code is made up from the BACnetErrorCode and BACnetErrorClass returned from the BACnet Service. The
BACNAPI_FAILURE_ERROR_CLASS and BACNAPI_FAILURE_ERROR_CODE macros can be used to extract
the corresponding BACnetErrorClass and BACnetErrorCode.
</p>
<p>
The Status flags are stored in a single BYTE where each Bit corresponds to a Status Flag. If
the corresponding flag is set, then the Status Flag is True. The API may not always
be able to determine the status flags for a particular Property, they do not always exist for all objects, in
this case the Status Flags exist Bit will not be set.
</p><p>
Bit 0 = Alarm.<br>
Bit 1 = Fault.<br>
Bit 2 = Overridden.<br>
Bit 3 = Out Of Service.<br>
Bit 4 = Status Flags Exist.<br>
</p><p>
To extract the appropriate Flag from the status flags BYTE, use the following logic.<br>
In Alarm == status_flags & 0x01<br>
In Fault == status_flags & 0x02<br>
Overridden == status_flags & 0x04<br>
Out Of Service == status_flags & 0x08<br>
Status Flags Exist == status_flags & 0x10<br>
</p>
*/
BACNAPI_API int bacnapi_object_read(BACnetDeviceObjectPropertyReference *reference, BACnetDataType *value, BYTE *status_flags, BYTE async);

/**
Performs a read request for multiple properties from BACnet Objects to return the value
of the property and status flags. It is similar to the bacnapi_object_read() function,
however it can be used to read multiple properties in a single request giving higher
performance when many read requests are required. The request can be syncronous in  which case the
function will not return until the service has completed. If asyncronous operation
is selected, then the return result will initially be Not Updated. Both internal
and external objects can be read using this request. If the object is in an external device
then the API will attempt to locate the device using the whoIs service, read it's capabilities
and then execute the appropriate service. Depending on the device capabilities the BACnet service
used could be readProperty, readPropertyMultiple, subscribeCOV or subscribeCOVProperty.
If the object is an internal object, then it will be accessed directly from the API without issuing a BACnet service.
</p><p>
<b>Inputs</b>
<table>
<tr><td>reference</td><td>An Array of Reference Identifiers to each object being read. The size of the array must correspond to the qty.</td></tr>
<tr><td>error_code</td><td>An Array of returned error codes for each property. The size of the array must correspond to the qty. See below for further details.</td></tr>
<tr><td>value</td><td>An Array of returned values from the object. The size of the array must correspond to the qty. Note - bacnapi_ClearBACnetDataType() must be called for each element to remove any memory allocated by this call.</td></tr>
<tr><td>status_flags</td><td>An Array of returned status flags from the object. The size of the array must correspond to the qty. See below for further details. </td></tr>
<tr><td>qty</td><td>The quantity of objects.</td></tr>
<tr><td>async</td><td>If this is set to true, then the API will wait for a response or timeout before returning a result.</td></tr>
</table>
<p>
<b>Result</b><br>
The result will be 0 if no failure occurs during the read request, the resulting failure
code is made up from the BACnetErrorCode and BACnetErrorClass returned from the BACnet Service. The
BACNAPI_FAILURE_ERROR_CLASS and BACNAPI_FAILURE_ERROR_CODE macros can be used to extract
the corresponding BACnetErrorClass and BACnetErrorCode.
</p>
<p>
The Status flags are stored in a single BYTE where each Bit corresponds to a Status Flag. If
the corresponding flag is set, then the Status Flag is True. The API may not always
be able to determine the status flags for a particular Property, they do not always exist for all objects, in
this case the Status Flags exist Bit will not be set.
</p><p>
Bit 0 = Alarm.<br>
Bit 1 = Fault.<br>
Bit 2 = Overridden.<br>
Bit 3 = Out Of Service.<br>
Bit 4 = Status Flags Exist.<br>
</p><p>
To extract the appropriate Flag from the status flags BYTE, use the following logic.<br>
In Alarm == status_flags & 0x01<br>
In Fault == status_flags & 0x02<br>
Overridden == status_flags & 0x04<br>
Out Of Service == status_flags & 0x08<br>
Status Flags Exist == status_flags & 0x10<br>
</p>
*/
BACNAPI_API int bacnapi_object_read_properties(BACnetDeviceObjectPropertyReference *reference, int* error_code, BACnetDataType *value, BYTE* status_flags, int qty, BYTE async);

/**
Performs a write request for a single property from a BACnet Object returning
the feedback value and status flags for the property.
The request can be syncronous in  which case the
function will not return until the service has completed. If asyncronous operation
is selected, then the return result will initially be Not Updated. Both internal
and external objects can be written to using this request. If the object is in an external device
then the API will attempt to locate the device using the whoIs service, read it's capabilities
and then execute the appropriate service. Depending on the device capabilities the BACnet service
used could be writeProperty or writePropertyMultiple. The 
If the object is an internal object, then it will be accessed directly from the API without issuing a BACnet service.
</p><p>
<b>Inputs</b>
<table>
<tr><td>reference</td><td>Reference Identifier to the object being written.</td></tr>
<tr><td>object_offline</td><td>Used to place the entire object in an offline state. No object access will be possible while the object is in this state. This parameter is ignored for external devices.</td></tr>
<tr><td>value</td><td>The value to be written to the object. Note - bacnapi_ClearBACnetDataType() must be called to remove any memory allocated by this call.</td></tr>
<tr><td>status_flags</td><td>The returned status flags from the object. See below for further details</td></tr>
<tr><td>priority</td><td>Specifies the priority to use for the request.</td></tr>
<tr><td>feedback_value</td><td>The feedback value returned from the object. Note - bacnapi_ClearBACnetDataType() must be called to remove any memory allocated by this call.</td></tr>
<tr><td>feedback_status_flags</td><td>The status flags returned from the object. See below for further details.</td></tr>
<tr><td>async</td><td>If this is set to true, then the API will wait for a response or timeout before returning a result.</td></tr>
</table>
<p>
<b>Result</b><br>
The result will be 0 if no failure occurs during the read request, the resulting failure
code is made up from the BACnetErrorCode and BACnetErrorClass returned from the BACnet Service. The
BACNAPI_FAILURE_ERROR_CLASS and BACNAPI_FAILURE_ERROR_CODE macros can be used to extract
the corresponding BACnetErrorClass and BACnetErrorCode.
</p>
<p>
The Status flags are stored in a single BYTE where each Bit corresponds to a Status Flag. If
the corresponding flag is set, then the Status Flag is True. The API may not always
be able to determine the status flags for a particular Property, they do not always exist for all objects, in
this case the Status Flags exist Bit will not be set.
</p><p>
Bit 0 = Alarm.<br>
Bit 1 = Fault.<br>
Bit 2 = Overridden.<br>
Bit 3 = Out Of Service.<br>
Bit 4 = Status Flags Exist.<br>
</p><p>
To extract the appropriate Flag from the status flags BYTE, use the following logic.<br>
In Alarm == status_flags & 0x01<br>
In Fault == status_flags & 0x02<br>
Overridden == status_flags & 0x04<br>
Out Of Service == status_flags & 0x08<br>
Status Flags Exist == status_flags & 0x10<br>
</p>
*/
BACNAPI_API int bacnapi_object_write(BACnetDeviceObjectPropertyReference *reference, int object_offline, BACnetDataType *value, BYTE priority, BACnetDataType *feedback_value, BYTE* feedback_status_flags, BYTE async);

/**
Performs a write request for multiple properties from BACnet Objects returning
the feedback value and status flags for the property. It is similar to the bacnapi_object_write() function,
however it can be used to write multiple properties in a single request giving higher
performance when many write requests are required. The request can be syncronous in  which case the
function will not return until the service has completed. If asyncronous operation
is selected, then the return result will initially be Not Updated. Both internal
and external objects can be written to using this request. If the object is in an external device
then the API will attempt to locate the device using the whoIs service, read it's capabilities
and then execute the appropriate service. Depending on the device capabilities the BACnet service
used could be writeProperty, readProperty, readPropertyMultiple, subscribeCOV or subscribeCOVProperty.
If the object is an internal object, then it will be accessed directly from the API without issuing a BACnet service.
</p><p>
<b>Inputs</b>
<table>
<tr><td>reference</td><td>An Array of Reference Identifiers to each object being written. The size of the array must correspond to the qty.</td></tr>
<tr><td>object_offline</td><td>An Array of object_offline flags, used to place the entire object in an offline state. No object access will be possible while the object is in this state. This parameter is ignored for external devices. The size of the array must correspond to the qty.</td></tr>
<tr><td>value</td><td>An Array of values to be written to the object. Note - bacnapi_ClearBACnetDataType() must be called to remove any memory allocated by this call. The size of the array must correspond to the qty.</td></tr>
<tr><td>priority</td><td>An Array of priorities to use for the request. The size of the array must correspond to the qty.</td></tr>
<tr><td>error_code</td><td>An Array of returned error codes for each property. The size of the array must correspond to the qty. See below for further details.</td></tr>
<tr><td>feedback_values</td><td>An Array of feedback values from the object. Note - bacnapi_ClearBACnetDataType() must be called to remove any memory allocated by this call. The size of the array must correspond to the qty.</td></tr>
<tr><td>feedback_status_flags</td><td>An Array of returned status flags from the object. See below for further details. The size of the array must correspond to the qty.</td></tr>
<tr><td>qty</td><td>The quantity of objects.</td></tr>
<tr><td>async</td><td>If this is set to true, then the API will wait for a response or timeout before returning a result.</td></tr>
</table>
<p>
<b>Result</b><br>
The result will be 0 if no failure occurs during the read request, the resulting failure
code is made up from the BACnetErrorCode and BACnetErrorClass returned from the BACnet Service. The
BACNAPI_FAILURE_ERROR_CLASS and BACNAPI_FAILURE_ERROR_CODE macros can be used to extract
the corresponding BACnetErrorClass and BACnetErrorCode.
</p>
<p>
The Status flags are stored in a single BYTE where each Bit corresponds to a Status Flag. If
the corresponding flag is set, then the Status Flag is True. The API may not always
be able to determine the status flags for a particular Property, they do not always exist for all objects, in
this case the Status Flags exist Bit will not be set.
</p><p>
Bit 0 = Alarm.<br>
Bit 1 = Fault.<br>
Bit 2 = Overridden.<br>
Bit 3 = Out Of Service.<br>
Bit 4 = Status Flags Exist.<br>
</p><p>
To extract the appropriate Flag from the status flags BYTE, use the following logic.<br>
In Alarm == status_flags & 0x01<br>
In Fault == status_flags & 0x02<br>
Overridden == status_flags & 0x04<br>
Out Of Service == status_flags & 0x08<br>
Status Flags Exist == status_flags & 0x10<br>
</p>
*/
BACNAPI_API int bacnapi_object_write_properties(BACnetDeviceObjectPropertyReference *reference, int* object_offline, BACnetDataType *value, BYTE *priority, int* error_code, BACnetDataType *feedback_value, BYTE* feedback_status_flags, int qty, BYTE async);

/**
Performs a Subscription of a BACnet Object. In the case of internal objects there is
no action taken. All Internal objects will fire the cov_notification callback message
regardless of a COV subscription message. In the case of an extrenal object, the BACnet
API will determine if the device COV's and then subscribe to this object. If the device
does not support COV's then the API will poll the value using readProperty or readPropertyMultiple.
The subscription type can be overriden using the service_type property. If the subscription is
successful then the returned value is zero.

<b>Inputs</b>
<table>
<tr><td>add</td><td>An indicator to identify if a subscription or unscription is required. A value of 1 indicates a subscription, a value of zero indicates an unsubscription.</td></tr>
<tr><td>reference</td><td>An Array of Reference Identifiers to each object being subscribed. The size of the array must correspond to the qty.</td></tr>
<tr><td>error_code</td><td>An Array of returned error codes for each property. The size of the array must correspond to the qty. A value of zero indicates a successful subscription.</td></tr>
<tr><td>update_interval</td><td>An Array of Update Intervals, used only when the service type is readProperty or readPropertyMultiple. The size of the array must correspond to the qty.</td></tr>
<tr><td>service_type</td><td>An Array of Service Types to use. See below for further details. The size of the array must correspond to the qty.</td></tr>
<tr><td>resubscription_interval</td><td>An Array of Resubscription Intervals. Used Only if the Service type is COV. The size of the array must correspond to the qty.</td></tr>
<tr><td>qty</td><td>The quantity of objects.</td></tr>
</table>
<b>Result</b>
The result will be 0 if no error occurs. The possible return values for this function are listed in the table below.
<br>

Valid Service Types Are : <br>
<table>
<tr><td>AUTOMATIC</td><td>0</td></tr>
<tr><td>READPROPERTY</td><td>1</td></tr>
<tr><td>READPROPERTYMULTIPLE</td><td>2</td></tr>
<tr><td>CONFIRMED_COV</td><td>3</td></tr>
<tr><td>UNCONFIRMED_COV</td><td>4</td></tr>
</table>
*/
BACNAPI_API int bacnapi_object_subscribe(int add, BACnetDeviceObjectPropertyReference *reference, int* error_code, int* update_interval, int* service_type, int* resubscription_interval, int qty);

/**
Sets the Update Interval of a Cache Object. If the object is not already subscribed to in
the cache then this will return a value of 1, otherwise the return value is 0.
The update interval is used when the cache is required to poll for values using readProperty or readPropertyMultiple.
*/
BACNAPI_API int bacnapi_set_update_interval(BACnetDeviceObjectPropertyReference *reference, unsigned int update_interval);

/**
	This function can be used inside callback routines to get the data from a property inside an object.
	It can be used to read from internal or external objects.

	Because this function is <b>not threadsafe</b> it cannot be called from outside of the callback functions
*/
BACNAPI_API int bacnapi_get_object_property(int deviceID, UINT objectType, UINT instance, UINT property, int arrayIndex, BACnetDataType *value);
/**
	This function can be used inside callback routines to set the data from a property inside an object.

	Because this function is <b>not threadsafe</b> it cannot be called from outside of the callback functions
*/
BACNAPI_API int bacnapi_set_object_property(int deviceID, UINT objectType, UINT instance, UINT property, int arrayIndex, BACnetDataType *value, UINT priority);

/**
	This function can be used inside callback routines to send an event notification.
	It has been provided to enable an application to send a propritary alarm and
	would be called from within the object_process_override callback function.
	The deviceID identifies which device has initiated the event, the
	notificationClass is used to determine where the event will be sent to.
	The currentEventState, eventTimeStamps and ackedTransitions will be
	updated by the function.

	Because this function is <b>not threadsafe</b> it cannot be called from outside of the callback functions
*/
BACNAPI_API int bacnapi_send_event_notification
	(
		//These items are to be passed through from the object initiating the event
		int deviceID,
		int notificationClass,
		BYTE toNormalEnable,
		BYTE toOffnormalEnable,
		BACnetObjectIdentifier *objectIdentifier,
		char* message,
		//These items are from the object and will be updated as required
		BACnetEventState *currentEventState,
		struct timeb *eventTimeStamps,
		BYTE *ackedTransitions,
		//These items are calculated from the type of alarm
		BACnetEventState fromState,
		BACnetEventState toState,
		BACnetNotificationParameters *notificationParameters,
		BACnetNotifyType notifyType
	);

/**
	This function can be used inside callback routines to send a cov notification
	It has been provided to enable an application to send a cov notification and
	would be called from within the object_process_override callback function.
	The deviceID identifies which device has initiated the cov notification, the
	activeSubscriptions identify which destinations will receive the event.

	Because this function is <b>not threadsafe</b> it cannot be called from outside of the callback functions
*/
BACNAPI_API int bacnapi_send_cov_notification(int device_id, BACnetObjectIdentifier *objectIdentifier, BACnetDataType *listOfValues, BACnetPropertyValue *propertyValue, BACnetDataType *activeSubscriptions, int error_code);

/*@}*/


/** @defgroup LibraryModule Library Module Routines
 *
    Using the BACnet Server API it is possible to write a Module and load it into
	the BACnet Server. Modules are written as a dll in the case of windows and
	as a shared library in the case of Linux. The advantage of a module is that it
	can run "In Process" and the performance of such a module is better than a module
	running Out Of Process. There are several callback functions that must be implemented
	in order to implement a library module.
 * @{
*/

/**
	Creates a new Module by starting a new thread inside the BACnet Server API.
	Two callback functions must be implemented and passed to the function
	when called. The run function runs the module, when the run function exits the module will stop.
	The stop function is called when the BACnet Server is stopping.
*/
BACNAPI_API int bacnapi_create_module(char* name, library_module_init_fn init, library_module_run_fn run, library_module_stop_fn stop, library_module_message_fn message);

/**
	Used to send a message to a Module.
*/
BACNAPI_API int bacnapi_module_message(char* name, int id, void* message);

BACNAPI_API int bacnapi_set_callback(callback_type type, void* callback);

/*@}*/

/** @defgroup HelperFunctions Helper Functions
 *
 There are many helper functions available which are used in combination
 with the BACnet Services.
 * @{
*/

/** @defgroup DataTypeAccess BACnet Data Access
 *
 The BACnet Data Access routines listed here are intended to be used
 to the data stored in the BACnet Structures. Each of the access
 routines takes a BACnet Data Type Structure and a write flag as an input.
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
 *@{
*/

BACNAPI_API void *bacnapi_malloc (unsigned size);
BACNAPI_API void *bacnapi_calloc (unsigned size);
BACNAPI_API void *bacnapi_realloc (void * a,unsigned b);
BACNAPI_API void bacnapi_free (void * p);

/**
 The bacnapi_string_destroy routine clears the memory that
 was assigned in the bacnapi_string_get call.
*/
BACNAPI_API int bacnapi_string_destroy(char *message);
/**
 The bacnapi_string_get routine converts the value stored in
 the BACnetDataType structure into a string. The destination
 must be a pointer to a string. The bacnapi_string_destroy function
 must be called to clear the memory after it is used.
*/
BACNAPI_API int bacnapi_string_get(char **destination, const BACnetDataType *value);
/**
 The bacnapi_string_set routine converts the value stored in source to
 the BACnetDataType structure. The destination
 must be a pointer to a BACnetDataType structure.
*/
BACNAPI_API int bacnapi_string_set(BACnetDataType *destination, const char *source);
/**
 The bacnapi_access_real routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_real(float *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_enumerated routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_enumerated(UINT *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_BACnetData routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_BACnetData(BACnetDataType* value, BACnetDataType *dataType, int arrayIndex, BYTE writeFlag);
/**
 The bacnapi_access_bitString routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_bitString(BACnetBitString* value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_set_bit routine allows for a single bit to be set inside the BACnetBitString structure.
*/
BACNAPI_API int bacnapi_set_bit(BACnetBitString* value, BYTE data, int index);
/**
 The bacnapi_access_boolean routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_boolean(BYTE *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_string routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_string(char **value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_octets routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_octets(BACnetOctet *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_objectIdentifier routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_objectIdentifier(BACnetObjectIdentifier *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_unsignedInteger routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_unsignedInteger(UINT *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_signedInteger routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_signedInteger(int *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_double routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_double(double *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_date routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_date(BACnetDate *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_time routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_time(BACnetTime *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_time_t routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_time_t(time_t *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_timeb routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_timeb(struct timeb *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_dateTime routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_dateTime(BACnetDateTime *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_calendarEntry routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_calendarEntry(BACnetCalendarEntry *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_dateRange routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_dateRange(BACnetDateRange *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_weekNDay routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_weekNDay(BACnetWeekNDay *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_timeValue routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_timeValue(BACnetTimeValue *value, BACnetDataType *dataType, BYTE writeFlag);
/**
The bacnetapi_set_time is used to copy the time from the value store in time_t
into the BACnetDateTime structure
*/
BACNAPI_API int bacnetapi_set_time(BACnetDateTime *destination, const time_t source);
/**
The bacnetapi_get_time is used to copy the time from the value store in the BACnetDateTime structure
into the time_t value.
*/
BACNAPI_API int bacnetapi_get_time(time_t *destination, const BACnetDateTime *source);
/**
The bacnetapi_set_timeb is used to copy the time from the value store in the timeb structure
into the BACnetDateTime structure
*/
BACNAPI_API int bacnetapi_set_timeb(BACnetDateTime *destination, const struct timeb* source);
/**
The bacnetapi_get_timeb is used to copy the time from the value store in the BACnetDateTime structure
into the timeb structure.
*/
BACNAPI_API int bacnetapi_get_timeb(struct timeb *destination, const BACnetDateTime *source);
/**
 The bacnapi_access_actionCommand routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_actionCommand(BACnetActionCommand *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_sessionKey routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_sessionKey(BACnetSessionKey *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_recipient routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_recipient(BACnetRecipient *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_addressBinding routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_addressBinding(BACnetAddressBinding *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_objectPropertyReference routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_objectPropertyReference(BACnetObjectPropertyReference *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_readAccessSpecification routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_readAccessSpecification(_ReadAccessSpecification *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_readAccessResult routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_readAccessResult(_ReadAccessResult *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_propertyReference routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_propertyReference(BACnetPropertyReference *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_destination routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_destination(BACnetDestination *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_dailySchedule routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_dailySchedule(BACnetDailySchedule *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_specialEvent routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_specialEvent(BACnetSpecialEvent *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_priorityValue routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_priorityValue(BACnetPriorityValue *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_deviceObjectPropertyReference routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_deviceObjectPropertyReference(BACnetDeviceObjectPropertyReference *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_propertyStates routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_propertyStates(BACnetPropertyStates *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_eventParameter routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_eventParameter(BACnetEventParameter *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_signedLong routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_signedLong(long *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_clientCOV routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_clientCOV(BACnetClientCOV *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_COVSubscription routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_COVSubscription(BACnetCOVSubscription *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_scale routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_scale(BACnetScale *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_setpointReference routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_setpointReference(BACnetSetpointReference *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_prescale routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_prescale(BACnetPrescale *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_accumulatorRecord routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_accumulatorRecord(BACnetAccumulatorRecord *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_eventSummaryElement routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_eventSummaryElement(BACnetEventSummaryElement *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_objectPropertyValue routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_objectPropertyValue(BACnetObjectPropertyValue *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_actionList routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_actionList(BACnetActionList *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_sequenceOfReadAccessSpecification routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_sequenceOfReadAccessSpecification(SequenceOfReadAccessSpecification *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_sequenceOfReadAccessResult routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_sequenceOfReadAccessResult(SequenceOfReadAccessResult *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_deviceObjectPropertyValue routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_deviceObjectPropertyValue(BACnetDeviceObjectPropertyValue *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_address routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_address(BACnetAddress *value, BACnetDataType *dataType, BYTE writeFlag);
/**
 The bacnapi_access_constructed routine provides access to
 to the data stored in the BACnet Structure
 If the write flag is set, then the value will be written to the dataType structure.
 If the write flag is not set then the value stored in the dataType structure will be written to the value.
*/
BACNAPI_API int bacnapi_access_constructed(BACnetConstructedDataType *value, BACnetDataType *dataType, BYTE writeFlag);

/**
 The bacnapi_lock is used to lock the mutex of the Main Thread in the BACnet Server API
*/
BACNAPI_API void bacnapi_lock();
/**
 The bacnapi_unlock is used to unlock the mutex of the Main Thread in the BACnet Server API
*/
BACNAPI_API void bacnapi_unlock();

/**
 The bacnapi_copy_event_notification_request function is used to copy a request.
*/
BACNAPI_API int bacnapi_copy_event_notification_request(BACnetServiceEventNotificationRequest *destination, BACnetServiceEventNotificationRequest *source);

/*@}*/

/** @defgroup serviceHelper Service Helper Functions
 *
	The Service Helper Functions are to be used in conjunction with
	the readPropertyMuliple and writePropertyMultiple Services
	to access the list elements
 *@{
*/

/**
 The bacnapi_get_element_property_reference routine gets a pointer to a
 Property Reference in the list or NULL if it is not found.
*/
BACNAPI_API BACnetPropertyReference *bacnapi_get_element_property_reference(SequenceOfBACnetPropertyReference *_list, int index);
/**
 The bacnapi_set_element_property_reference routine sets an element in the list.
*/
BACNAPI_API BACnetPropertyReference *bacnapi_set_element_property_reference(SequenceOfBACnetPropertyReference *_list, BACnetPropertyReference *data);
/**
 The bacnapi_get_element_read_access_specification routine gets a pointer to a
 Read Access Specification in the list or NULL if it is not found.
*/
BACNAPI_API _ReadAccessSpecification *bacnapi_get_element_read_access_specification(SequenceOfReadAccessSpecification *_list, int index);
/**
 The bacnapi_set_element_read_access_specification routine sets an element in the list.
*/
BACNAPI_API _ReadAccessSpecification *bacnapi_set_element_read_access_specification(SequenceOfReadAccessSpecification *_list, _ReadAccessSpecification *data);
/**
 The bacnapi_get_element_read_access_property_value routine gets a pointer to a
 Read Access Property Value in the list or NULL if it is not found.
*/
BACNAPI_API BACnetReadAccessPropertyValue *bacnapi_get_element_read_access_property_value(SequenceOfBACnetReadAccessPropertyValue *_list, int index);
/**
 The bacnapi_set_element_read_access_property_value routine sets an element in the list.
*/
BACNAPI_API BACnetReadAccessPropertyValue *bacnapi_set_element_read_access_property_value(SequenceOfBACnetReadAccessPropertyValue *_list, BACnetReadAccessPropertyValue *data);
/**
 The bacnapi_get_element_read_access_result routine gets a pointer to a
 Read Access Result in the list or NULL if it is not found.
*/
BACNAPI_API _ReadAccessResult *bacnapi_get_element_read_access_result(SequenceOfReadAccessResult *_list, int index);
/**
 The bacnapi_set_element_read_access_result routine sets an element in the list.
*/
BACNAPI_API _ReadAccessResult *bacnapi_set_element_read_access_result(SequenceOfReadAccessResult *_list, _ReadAccessResult *data);

/**
 Gets a list of NIC adapters on the PC, used for the BACnet Ethernet driver
*/
BACNAPI_API int bacnapi_get_adapters(nic_adapter *adapters, int len, char *error, int error_len);

/**
 Allocates the BACnet Octet Structure
*/
BACNAPI_API int bacnapi_allocate_octets(BACnetOctet *octet, UINT length);
/**
 Sets the BACnet Octet Structure
*/
BACNAPI_API int bacnapi_set_octets(BACnetOctet *octet, BYTE *value, UINT length);

/**
 The bacnapi_initialize_data routine changes the data type to the requested type
*/
BACNAPI_API int bacnapi_initialize_data(BACnetDataType *data, BACnetApplicationTagNumber type);
/**
 The bacnapi_get_element_from_bacnet_array routine gets a pointer to a
 BACnetDataType in the array or NULL if it is not found.
*/
BACNAPI_API BACnetDataType *bacnapi_get_element_from_bacnet_array(BACnetArray *array, int index);
/**
 The bacnapi_set_element_in_bacnet_array routine sets an element in a BACnetArray.
*/
BACNAPI_API BACnetDataType *bacnapi_set_element_in_bacnet_array(BACnetArray *array, BACnetDataType *data);

/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetDataType(BACnetDataType *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetEventParameter(BACnetEventParameter *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetNotificationParameters(BACnetNotificationParameters *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetObjectPropertyValue(BACnetObjectPropertyValue *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetTimeValue(BACnetTimeValue *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetAddress(BACnetAddress *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetConstructed(BACnetConstructedDataType *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearOctet(BACnetOctet *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBuffer(BACnetMessageBuffer *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBitString(BACnetBitString *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearArray(BACnetArray *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearDateTime(BACnetDateTime *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetActionCommand(BACnetActionCommand *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetSessionKey(BACnetSessionKey *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetRecipient(BACnetRecipient *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetLogRecord(BACnetLogRecord *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetRecipientProcess(BACnetRecipientProcess *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetAddressBinding(BACnetAddressBinding *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearReadAccessSpecification(_ReadAccessSpecification *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearReadAccessPropertyValue(BACnetReadAccessPropertyValue *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearReadAccessResult(_ReadAccessResult *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearWriteAccessSpecification(_WriteAccessSpecification *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearAlarmSummaryAckElement(BACnetServiceGetAlarmSummaryAckElement *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearEnrolmentSummaryAckElement(BACnetServiceGetEnrolmentSummaryAckElement *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetDestination(BACnetDestination *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetCOVSubscription(BACnetCOVSubscription *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetScale(BACnetScale *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetPrescale(BACnetPrescale *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetAccumulatorRecord(BACnetAccumulatorRecord *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetEventSummaryElement(BACnetEventSummaryElement *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetActionList(BACnetActionList *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearSequenceOfReadAccessResult(SequenceOfReadAccessResult *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearSequenceOfReadAccessSpecification(SequenceOfReadAccessSpecification *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetDeviceObjectPropertyValue(BACnetDeviceObjectPropertyValue *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetDailySchedule(BACnetDailySchedule *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetSpecialEvent(BACnetSpecialEvent *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetPriorityValue(BACnetPriorityValue *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetPriorityArray(BACnetPriorityArray *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetEventTimeStamps(BACnetEventTimeStamps *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetPropertyValue(BACnetPropertyValue *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetSelectionCriteria(BACnetSelectionCriteria *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetPropertyStates(BACnetPropertyStates *data);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetServiceConfirmedEventNotificationRequest(BACnetServiceEventNotificationRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetServiceUnconfirmedEventNotificationRequest(BACnetServiceEventNotificationRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearBACnetError(BACnetError *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearUnconfirmedEventNotificationRequest(BACnetServiceEventNotificationRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearUnconfirmedCOVNotificationRequest(BACnetServiceCOVNotificationRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearUnconfirmedPrivateTransferRequest(BACnetServicePrivateTransferRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearUnconfirmedTextMessageRequest(BACnetServiceTextMessageRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearTimeSynchronization(BACnetServiceTimeSynchronization *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearUtcTimeSynchronization(BACnetServiceUtcTimeSynchronization *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearWhoHas(BACnetServiceWhoHas *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearConfirmedCOVnotificationRequest(BACnetServiceCOVNotificationRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearConfirmedEventNotificationRequest(BACnetServiceEventNotificationRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearGetAlarmSummaryAck(BACnetServiceGetAlarmSummaryAck *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearGetEnrolmentSummaryAck(BACnetServiceGetEnrolmentSummaryAck *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearGetEnrolmentSummaryRequest(BACnetServiceGetEnrolmentSummaryRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearSubscribeCOVRequest(BACnetServiceSubscribeCOVRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearAtomicReadFileAck(BACnetServiceAtomicReadFileAck *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearAtomicReadFileRequest(BACnetServiceAtomicReadFileRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearAtomicWriteFileAck(BACnetServiceAtomicWriteFileAck *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearAtomicWriteFileRequest(BACnetServiceAtomicWriteFileRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearAddListElementRequest(BACnetServiceAddListElementRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearRemoveListElementRequest(BACnetServiceRemoveListElementRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearReadPropertyConditionalAck(BACnetServiceReadPropertyConditionalAck *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearReadPropertyConditionalRequest(BACnetServiceReadPropertyConditionalRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearDeviceCommunicationControlRequest(BACnetServiceDeviceCommunicationControlRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearConfirmedPrivateTransferAck(BACnetServiceConfirmedPrivateTransferAck *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearConfirmedPrivateTransferRequest(BACnetServicePrivateTransferRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearConfirmedTextMessageRequest(BACnetServiceTextMessageRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearReinitializeDeviceRequest(BACnetServiceReinitializeDeviceRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearAuthenticateRequest(BACnetServiceAuthenticateRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearRequestKeyRequest(BACnetServiceRequestKeyRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearReadRangeAck(BACnetServiceReadRangeAck *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearReadRangeRequest(BACnetServiceReadRangeRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearReadPropertyRequest(BACnetServiceReadPropertyRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearReadPropertyAck(BACnetServiceReadPropertyAck *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearReadPropertyMultipleRequest(BACnetServiceReadPropertyMultipleRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearReadPropertyMultipleAck(BACnetServiceReadPropertyMultipleAck *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearWritePropertyRequest(BACnetServiceWritePropertyRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearWritePropertyMultipleRequest(BACnetServiceWritePropertyMultipleRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearIHaveRequest(BACnetServiceIHave *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearIamRequest(BACnetServiceIam *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearWhoIsRequest(BACnetServiceWhoIs *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearCreateObjectRequest(BACnetServiceCreateObjectRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearCreateObjectAck(BACnetServiceCreateObjectAck *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearAcknowledgeAlarmRequest(BACnetServiceAcknowledgeAlarmRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearLifeSafetyOperationRequest(BACnetServiceLifeSafetyOperationRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearSubscribeCOVPropertyRequest(BACnetServiceSubscribeCOVPropertyRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearGetEventInformationRequest(BACnetServiceGetEventInformationRequest *service);
/**
Used to clear resources
*/
BACNAPI_API void bacnapi_ClearGetEventInformationAck(BACnetServiceGetEventInformationAck *service);


/**
Used to initialize the service before it is used.
*/
#define bacnapi_acknowledge_alarm_initialize(a,b,c) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); a.request = &b; a.error = &c;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_acknowledge_alarm_destroy(a,b,c) bacnapi_ClearAcknowledgeAlarmRequest(&b); bacnapi_ClearBACnetError(&c); bacnapi_acknowledge_alarm_initialize(a,b,c);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_get_alarm_summary_initialize(a,b,c) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); a.ack = &b; a.error = &c;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_get_alarm_summary_destroy(a,b,c) bacnapi_ClearGetAlarmSummaryAck(&b); bacnapi_ClearBACnetError(&c); bacnapi_get_alarm_summary_initialize(a,b,c);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_get_enrolment_summary_initialize(a,b,c,d) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); memset(&d, 0, sizeof(d)); a.request = &b; a.ack = &c; a.error = &d;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_get_enrolment_summary_destroy(a,b,c,d) bacnapi_ClearGetEnrolmentSummaryRequest(&b); bacnapi_ClearGetEnrolmentSummaryAck(&c); bacnapi_ClearBACnetError(&d); bacnapi_get_enrolment_summary_initialize(a,b,c,d);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_subscribe_cov_initialize(a,b,c) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); a.request = &b; a.error = &c;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_subscribe_cov_destroy(a,b,c) bacnapi_ClearSubscribeCOVRequest(&b); bacnapi_ClearBACnetError(&c); bacnapi_subscribe_cov_initialize(a,b,c);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_atomic_read_file_initialize(a,b,c,d) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); memset(&d, 0, sizeof(d)); a.request = &b; a.ack = &c; a.error = &d;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_atomic_read_file_destroy(a,b,c,d) bacnapi_ClearAtomicReadFileRequest(&b); bacnapi_ClearAtomicReadFileAck(&c); bacnapi_ClearBACnetError(&d); bacnapi_atomic_read_file_initialize(a,b,c,d);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_atomic_write_file_initialize(a,b,c,d) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); memset(&d, 0, sizeof(d)); a.request = &b; a.ack = &c; a.error = &d;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_atomic_write_file_destroy(a,b,c,d) bacnapi_ClearAtomicWriteFileRequest(&b); bacnapi_ClearAtomicWriteFileAck(&c); bacnapi_ClearBACnetError(&d); bacnapi_atomic_write_file_initialize(a,b,c,d);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_add_list_element_initialize(a,b,c) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); a.request = &b; a.error = &c;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_add_list_element_destroy(a,b,c) bacnapi_ClearAddListElementRequest(&b); bacnapi_ClearBACnetError(&c); bacnapi_add_list_element_initialize(a,b,c);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_remove_list_element_initialize(a,b,c) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); a.request = &b; a.error = &c;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_remove_list_element_destroy(a,b,c) bacnapi_ClearRemoveListElementRequest(&b); bacnapi_ClearBACnetError(&c); bacnapi_remove_list_element_initialize(a,b,c);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_create_object_initialize(a,b,c,d) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); memset(&d, 0, sizeof(d)); a.request = &b; a.ack = &c; a.error = &d;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_create_object_destroy(a,b,c,d) bacnapi_ClearCreateObjectRequest(&b); bacnapi_ClearCreateObjectAck(&c); bacnapi_ClearBACnetError(&d); bacnapi_create_object_initialize(a,b,c,d);


/**
Used to initialize the service before it is used.
*/
#define bacnapi_delete_object_initialize(a,b,c) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); a.request = &b; a.error = &c;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_delete_object_destroy(a,b,c) bacnapi_ClearBACnetError(&c); bacnapi_delete_object_initialize(a,b,c);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_read_property_initialize(a,b,c,d) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); memset(&d, 0, sizeof(d)); a.request = &b; a.ack = &c; a.error = &d;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_read_property_destroy(a,b,c,d) bacnapi_ClearReadPropertyRequest(&b); bacnapi_ClearReadPropertyAck(&c); bacnapi_ClearBACnetError(&d); bacnapi_read_property_initialize(a,b,c,d);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_read_property_conditional_initialize(a,b,c,d) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); memset(&d, 0, sizeof(d)); a.request = &b; a.ack = &c; a.error = &d;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_read_property_conditional_destroy(a,b,c,d) bacnapi_ClearReadPropertyConditionalRequest(&b); bacnapi_ClearReadPropertyConditionalAck(&c); bacnapi_ClearBACnetError(&d); bacnapi_read_property_conditional_initialize(a,b,c,d);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_read_property_multiple_initialize(a,b,c,d) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); memset(&d, 0, sizeof(d)); a.request = &b; a.ack = &c; a.error = &d;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_read_property_multiple_destroy(a,b,c,d) bacnapi_ClearReadPropertyMultipleRequest(&b); bacnapi_ClearReadPropertyMultipleAck(&c); bacnapi_ClearBACnetError(&d); bacnapi_read_property_initialize(a,b,c,d);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_write_property_initialize(a,b,c) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); a.request = &b; a.error = &c;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_write_property_destroy(a,b,c) bacnapi_ClearWritePropertyRequest(&b); bacnapi_ClearBACnetError(&c); bacnapi_write_property_initialize(a,b,c);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_write_property_multiple_initialize(a,b,c) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); a.request = &b; a.error = &c;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_write_property_multiple_destroy(a,b,c) bacnapi_ClearWritePropertyMultipleRequest(&b); bacnapi_ClearBACnetError(&c); bacnapi_write_property_multiple_initialize(a,b,c);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_device_communication_control_initialize(a,b,c) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); a.request = &b; a.error = &c;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_device_communication_control_destroy(a,b,c) bacnapi_ClearDeviceCommunicationControlRequest(&b); bacnapi_ClearBACnetError(&c); bacnapi_device_communication_control_initialize(a,b,c);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_confirmed_private_transfer_initialize(a,b,c,d) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); memset(&d, 0, sizeof(d)); a.request = &b; a.ack = &c; a.error = &d;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_confirmed_private_transfer_destroy(a,b,c,d) bacnapi_ClearConfirmedPrivateTransferRequest(&b); bacnapi_ClearConfirmedPrivateTransferAck(&c); bacnapi_ClearBACnetError(&d); bacnapi_confirmed_private_transfer_initialize(a,b,c,d);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_confirmed_text_message_initialize(a,b,c) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); a.request = &b; a.error = &c;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_confirmed_text_message_destroy(a,b,c) bacnapi_ClearConfirmedTextMessageRequest(&b); bacnapi_ClearBACnetError(&c); bacnapi_confirmed_text_message_initialize(a,b,c);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_reinitialize_device_message_initialize(a,b,c) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); a.request = &b; a.error = &c;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_reinitialize_device_destroy(a,b,c) bacnapi_ClearReinitializeDeviceRequest(&b); bacnapi_ClearBACnetError(&c); bacnapi_reinitialize_device_message_initialize(a,b,c);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_authenticate_initialize(a,b,c,d) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); memset(&d, 0, sizeof(d)); a.request = &b; a.ack = &c; a.error = &d;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_authenticate_destroy(a,b,c,d) bacnapi_ClearAuthenticateRequest(&b); bacnapi_ClearBACnetError(&d); bacnapi_authenticate_initialize(a,b,c,d);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_request_key_initialize(a,b,c,d) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); memset(&d, 0, sizeof(d)); a.request = &b; a.ack = &c; a.error = &d;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_request_key_destroy(a,b,c,d) bacnapi_ClearRequestKeyRequest(&b); bacnapi_ClearBACnetError(&d); bacnapi_request_key_initialize(a,b,c,d);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_read_range_initialize(a,b,c,d) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); memset(&d, 0, sizeof(d)); a.request = &b; a.ack = &c; a.error = &d;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_read_range_destroy(a,b,c,d) bacnapi_ClearReadRangeRequest(&b); bacnapi_ClearReadRangeAck(&c); bacnapi_ClearBACnetError(&d); bacnapi_read_range_initialize(a,b,c,d);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_unconfirmed_private_transfer_initialize(a,b) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); a.request = &b;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_unconfirmed_private_transfer_destroy(a,b) bacnapi_ClearUnconfirmedPrivateTransferRequest(&b); bacnapi_unconfirmed_private_transfer_initialize(a,b);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_unconfirmed_text_message_initialize(a,b) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); a.request = &b;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_unconfirmed_text_message_destroy(a,b) bacnapi_ClearUnconfirmedTextMessageRequest(&b); bacnapi_unconfirmed_text_message_initialize(a,b);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_life_safety_operation_initialize(a,b,c) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); a.request = &b; a.error = &c;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_life_safety_operation_destroy(a,b,c) bacnapi_ClearLifeSafetyOperationRequest(&b); bacnapi_ClearBACnetError(&c); bacnapi_life_safety_operation_initialize(a,b,c);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_subscribe_cov_property_initialize(a,b,c) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); a.request = &b; a.error = &c;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_subscribe_cov_property_destroy(a,b,c) bacnapi_ClearSubscribeCOVPropertyRequest(&b); bacnapi_ClearBACnetError(&c); bacnapi_subscribe_cov_property_initialize(a,b,c);

/**
Used to initialize the service before it is used.
*/
#define bacnapi_get_event_information_initialize(a,b,c,d) memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c)); memset(&d, 0, sizeof(d)); a.request = &b; a.ack = &c; a.error = &d;
/**
Used to clear the resources of a service after it has been executed.
*/
#define bacnapi_get_event_information_destroy(a,b,c,d) bacnapi_ClearGetEventInformationRequest(&b); bacnapi_ClearGetEventInformationAck(&c); bacnapi_ClearBACnetError(&d); bacnapi_get_event_information_initialize(a,b,c,d);

/*@}*/
/*@}*/

#ifdef __cplusplus
}

#endif

#endif
