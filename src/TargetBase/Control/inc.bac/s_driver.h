/*####COPYRIGHTBEGIN####
 -------------------------------------------
 Copyright (C) 2003 Chris Gurtler

 -------------------------------------------
####COPYRIGHTEND####*/

#if !defined INCLUDE_BACN_DRIVER
#define INCLUDE_BACN_DRIVER

#include "common_struct.h"
#include "datatype.h"
#include "s_service_indication.h"

typedef struct object_init
{
	/*!This is the name of the database file used to store BACnet Local Objects.
	Local Objects are loaded from the database at startup and the BACnet Server updates the
	database when a BACnet Service writes to an object. If the file name is set to NULL then
	local objects will NOT be stored offline in the database.*/
	char *database_file_name;

	/*If this flag is set then the driver will load the objects from the data base at startup*/
	BYTE load_objects;

	/*If this flag is set then the driver will load the virtual networks from the data base at startup*/
	BYTE load_virtual_networks;

	/*!This is the name of the LUA script file used by the BACnet program object to process local objects.
	If the file name is set to NULL then no program object will be created, and no script file used.*/
	char *script_file_name;

	/*!This Callback Function is used to overide the Read Property handler of the BACnet API.
	A return value of 0 indicates that the handler is overridden, a return value of -1 tells the API to use the default handler.
	*/
	read_object_property_override_fn read_object_property_override;
	/*!This Callback Function is used to overide the .
	A return value of 0 indicates that the handler is overridden, a return value of -1 tells the API to use the default handler.
	*/
	write_object_property_override_fn write_object_property_override;
	/*!This Callback Function is used to overide the .
	A return value of 0 indicates that the handler is overridden, a return value of -1 tells the API to use the default handler.
	*/
	read_object_proprietary_property_fn read_object_proprietary_property;
	/*!This Callback Function is used to overide the .
	A return value of 0 indicates that the handler is overridden, a return value of -1 tells the API to use the default handler.
	*/
	write_object_proprietary_property_fn write_object_proprietary_property;
	/*!This Callback Function is used to overide the .
	A return value of 0 indicates that the handler is overridden, a return value of -1 tells the API to use the default handler.
	*/
	proprietary_property_init_fn proprietary_property_init;
	/*!This Callback Function is used to overide the .
	A return value of 0 indicates that the handler is overridden, a return value of -1 tells the API to use the default handler.
	*/
	proprietary_all_property_add_fn proprietary_all_property_add;
	/*!This Callback Function is used to overide the .
	A return value of 0 indicates that the handler is overridden, a return value of -1 tells the API to use the default handler.
	*/
	object_process_overide_fn object_process_overide;

	/*!This Callback Function is used to append destinations to the list of active subscriptions .
	A return value of 0 indicates that the handler is overridden, a return value of -1 tells the API to use the default handler.
	*/
	cov_append_active_subscription_fn cov_append_active_subscription;

	/*!This Callback Function is used to append recipients to the recipient list .
	A return value of 0 indicates that the handler is overridden, a return value of -1 tells the API to use the default handler.
	*/
	event_append_recipient_list_fn event_append_recipient_list;

	/*!This callback is fired to indicate that the application should start or stop updating
	the present value of the internal object.
	*/
	read_present_value_notification_fn read_present_value_notification;

	/*!This callback is fired to indicate that an Internal object has been inserted into the BACnet Server.
	This can occur when the createObject Service is called or during startup when objects are loaded from the database.
	*/
	object_insert_notification_fn object_insert_notification;

	/*!This callback is fired to indicate that an Internal object has been removed from the BACnet Server.
	This can occur when the deleteObject Service is called.
	*/
	object_remove_notification_fn object_remove_notification;

    char *applicationSoftwareVersion;
    char *firmwareRevision;
    char *modelName;
    char *objectName;
    char *vendorName;
    char *description;
    char *location;

	/*!Specifies the Object Profile Name to be used with all Objects. This will 
	default to "123-20051025".
	*/
	char *objects_profile_name;

    UINT vendorIdentifier;

	UINT apduTimeout;
	UINT apduSegmentTimeout;
	UINT numberOfAPDUretries;

	int  intrinsic;

	/*!Specifies the interval between Database Commits in seconds, if changes are made to the database
	then the data will be written to disk when a commit takes place. The default commit
	interval is 1 second.*/
	int database_commit_interval; 
}object_init;

/*! \struct object_init
 * BACnet Device Object Initialisation Structure.
 *
 * This is part of the BACnetDriver initialisation structure is used to hold information for creating the BACnet Server.
 *
 */

typedef struct BACnetDriver
{
	/*!Device ID of the BACnet Server.*/
	int device_id;

	/*!System Status which will be loaded into the device object. 
	If the system_status is Operational or OperationalReadOnly, then an Iam message will be sent when the driver
	starts. For all other states, no Iam message will be sent at start time. When
	an application updates the system_status to Operational or OperationalReadOnly then
	an Iam message will be sent.*/
	BACnetDeviceStatus system_status;

	/*!Logging Information.*/
	bacnet_logging logging;

	/*!Device Object Information. Used to populate the Device Object of the local device. eg ObjectName, Description etc.*/
	object_init device;

	/*!Data link Layer Information used for creating the Data Link Layers or Ports.
	There can be up to 128 ports assigned to the driver, and these can be any of the following types.
	BACnet/IP, BACnet Ethernet, MSTP, PTP or Anex H.*/
	datalink_init datalink;

	/*!Callback Functions for service indication. Each service has a corresponding callback
	function that must be passed into the Initialisation Structure. The callback
	function will be executed when the BACnet Server receives the corresponding service.
	There are service indications for whoIs, Iam, COV notifications, Event notifactions, etc.*/
	service_indication_init service_indication;

	/*!Callback Functions for service overrides. Each Service has a corresponding callback
	function that must be passed into the Initialisation Structure. The callback
	function will be executed when the BACnet Server receives the corresponding service.
	The default behaviour of the service can be overrided using these callback functions.*/
	service_overrides_init service_overrides;

	/*!The COV notification callback function is executed when the value of
	an internal object changes state for any reason. 
	It is not the same as the standard BACnet COV services.
	*/
	object_access_cov_notification_fn cov_notification;

	/*!Callback Function to indicate that the present value property
	of an internal object has been read from another device using the standard BACnet Services.
	This callback function forms part of the Object API that should be used when the BACnet
	Server is acting as a gateway.*/
	object_present_value_read_request_notification_fn internal_object_read_request_notification;
	
	db_overrides_init db_overrides;		// ##DB##
	
	sof_enable_datalink_layer_fn sof_enable_datalink_layer;		// ##SOF##

}BACnetDriver;

/*! \struct BACnetDriver
 * BACnet Server Initialisation Structure.
 *
 * The initialisation structure is used to hold information for creating the BACnet Server.
 */

#endif

