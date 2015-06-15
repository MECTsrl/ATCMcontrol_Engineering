/*####COPYRIGHTBEGIN####
 -------------------------------------------
  Copyright (C) 2003 Chris Gurtler

  Revision History
  30 January 2006 - Added Service Overrides for Unconfirmed Services

 -------------------------------------------
####COPYRIGHTEND####*/

#ifndef _s_service_indication_h_
#define _s_service_indication_h_

#include "datatype.h"
#include "common_typedef.h"

typedef struct service_indication_init
{
	/*!This is a pointer to a callback function for Iam Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_indication_iam_fn iam;
	/*!This is a pointer to a callback function for IHave Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_indication_Ihave_fn Ihave;
	/*!This is a pointer to a callback function for unconfirmedCOV Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_indication_unconfirmed_cov_notification_fn unconfirmed_cov_notification;
	/*!This is a pointer to a callback function for unconfirmedEvent Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_indication_unconfirmed_event_notification_fn unconfirmed_event_notification;
	/*!This is a pointer to a callback function for unconfirmedPrivateTranser Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_indication_unconfirmed_private_transfer_fn unconfirmed_private_transfer;
	/*!This is a pointer to a callback function for unconfirmedText Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_indication_unconfirmed_text_message_fn unconfirmed_text_message;
	/*!This is a pointer to a callback function for timeSynchronization Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_indication_time_synchronization_fn time_synchronization;
	/*!This is a pointer to a callback function for whoHas Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_indication_who_has_fn who_has;
	/*!This is a pointer to a callback function for whoIs Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_indication_who_is_fn who_is;
	/*!This is a pointer to a callback function for utcTimeSynchronization Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_indication_utc_time_synchronization_fn utc_time_synchronization;

	/*!This is a pointer to a callback function for confirmedCOV Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_indication_confirmed_cov_notification_fn confirmed_cov_notification;
	/*!This is a pointer to a callback function for confirmedEvent Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_indication_confirmed_event_notification_fn confirmed_event_notification;
	/*!This is a pointer to a callback function for confirmedPrivateTransfer Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_indication_confirmed_private_transfer_fn confirmed_private_transfer;
	/*!This is a pointer to a callback function for confirmedTextMessage Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_indication_confirmed_text_message_fn confirmed_text_message;
	/*!This is a pointer to a callback function for lifeSafetyOperation Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_indication_life_safety_operation_fn life_safety_operation;
}service_indication_init;
/*! \struct service_indication_init
 * BACnet Server Service Indication Initialisation Structure.
 *
 * The initialisation structure is used to hold information for creating the BACnet Server.
   This structure is part of the BACnetDriver structure which is passed
   to the BACnet Server when it is initialized. It contains a pointer to callback functions
   which are used by the BACnet Server to notify an application that a service has been received.
 */

typedef struct service_overrides_init
{
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_read_property_fn read_property;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_read_property_conditional_fn read_property_conditional;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_read_property_multiple_fn read_property_multiple;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_subscribe_cov_fn subscribe_cov;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_atomic_read_file_fn atomic_read_file;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_atomic_write_file_fn atomic_write_file;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_add_list_element_fn add_list_element;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_remove_list_element_fn remove_list_element;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_create_object_fn create_object;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_delete_object_fn delete_object;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_write_property_fn write_property;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_write_property_multiple_fn write_property_multiple;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_confirmed_private_transfer_fn confirmed_private_transfer;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_device_communication_control_fn device_communication_control;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_reinitialize_device_fn reinitialize_device;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_read_range_fn read_range;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_life_safety_operation_fn life_safety_operation;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_subscribe_cov_property_fn subscribe_cov_property;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_get_event_information_fn get_event_information;

	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_confirmed_cov_notification_fn confirmed_cov_notification;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_confirmed_event_notification_fn confirmed_event_notification;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_confirmed_text_message_fn confirmed_text_message;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_acknowledge_alarm_fn acknowledge_alarm;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_get_alarm_summary_fn get_alarm_summary;
	/*!This is a pointer to a callback function for overriding Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_get_enrolment_summary_fn get_enrolment_summary;


	/*!This is a pointer to a callback function for Iam Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_iam_fn iam;
	/*!This is a pointer to a callback function for IHave Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_Ihave_fn Ihave;
	/*!This is a pointer to a callback function for unconfirmedCOV Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_unconfirmed_cov_notification_fn unconfirmed_cov_notification;
	/*!This is a pointer to a callback function for unconfirmedEvent Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_unconfirmed_event_notification_fn unconfirmed_event_notification;
	/*!This is a pointer to a callback function for unconfirmedPrivateTranser Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_unconfirmed_private_transfer_fn unconfirmed_private_transfer;
	/*!This is a pointer to a callback function for unconfirmedText Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_unconfirmed_text_message_fn unconfirmed_text_message;
	/*!This is a pointer to a callback function for timeSynchronization Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_time_synchronization_fn time_synchronization;
	/*!This is a pointer to a callback function for whoHas Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_who_has_fn who_has;
	/*!This is a pointer to a callback function for whoIs Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_who_is_fn who_is;
	/*!This is a pointer to a callback function for utcTimeSynchronization Notifications. If this is set to NULL, then the application will not receive callback messages of this type.*/
	service_override_utc_time_synchronization_fn utc_time_synchronization;


}service_overrides_init;
/*! \struct service_overrides
 * BACnet Server Service Overrides Initialisation Structure.
 *
 * The initialisation structure is used to hold information for creating the BACnet Server.
   This structure is part of the BACnetDriver structure which is passed
   to the BACnet Server when it is initialized. It contains a pointer to callback functions
   which are used by the BACnet Server to notify an application that a service has been received.
   The Callback Functions can be used to override the default behaviour of the service.
 */

// ##DB## >>>

typedef struct db_overrides_init
{
	db_override_update_ana_priority_value_fn update_ana_priority_value;
	db_override_update_binary_pv_priority_value_fn update_binary_pv_priority_value;
	db_override_update_uint_priority_value_fn update_uint_priority_value;

} db_overrides_init;

// ##DB## <<<

#endif

