/*####COPYRIGHTBEGIN####
 -------------------------------------------
 Copyright (C) 2003 Chris Gurtler

 -------------------------------------------
####COPYRIGHTEND####*/

 /* =======================================================================
    common_enum.h     BACnet Enumerations

    Chris Gurtler

*/


#ifndef _common_enum_h_
#define _common_enum_h_

#define MAX_NPDU						2056 //Buffer for holding the NPDU

//General DataTypes used throughout
typedef unsigned int UINT;
typedef void * LPVOID;
typedef unsigned short USINT;

/*  --- Enumerations ------------------------------------------------- */

typedef enum BACnetEngineeringUnits
{
	UNITS_meters_per_second_per_second  		= 166,
	UNITS_square_meters  				= 0,
	UNITS_square_centimeters  			= 116,
	UNITS_square_feet  				= 1,
	UNITS_square_inches  				= 115,
	UNITS_currency1  				= 105,
	UNITS_currency2  				= 106,
	UNITS_currency3  				= 107,
	UNITS_currency4  				= 108,
	UNITS_currency5  				= 109,
	UNITS_currency6  				= 110,
	UNITS_currency7  				= 111,
	UNITS_currency8  				= 112,
	UNITS_currency9  				= 113,
	UNITS_currency10 				= 114,
	UNITS_milliamperes  				= 2,
	UNITS_amperes  					= 3,
	UNITS_amperes_per_meter  			= 167,
	UNITS_amperes_per_square_meter  		= 168,
	UNITS_ampere_square_meters  			= 169,
	UNITS_farads  					= 170,
	UNITS_henrys  					= 171,
	UNITS_ohms  					= 4,
	UNITS_ohm_meters  				= 172,
	UNITS_milliohms  				= 145,
	UNITS_kilohms  					= 122,
	UNITS_megohms  					= 123,
	UNITS_siemens  					= 173, // 1 mho equals 1 siemens
	UNITS_siemens_per_meter  			= 174,
	UNITS_teslas  					= 175,
	UNITS_volts  					= 5,
	UNITS_millivolts  				= 124,
	UNITS_kilovolts  				= 6,
	UNITS_megavolts  				= 7,
	UNITS_volt_amperes  				= 8,
	UNITS_kilovolt_amperes  			= 9,
	UNITS_megavolt_amperes  			= 10,
	UNITS_volt_amperes_reactive  			= 11,
	UNITS_kilovolt_amperes_reactive  		= 12,
	UNITS_megavolt_amperes_reactive  		= 13,
	UNITS_volts_per_degree_Kelvin  			= 176,
	UNITS_volts_per_meter  				= 177,
	UNITS_degrees_phase  				= 14,
	UNITS_power_factor  				= 15,
	UNITS_webers  					= 178,
	UNITS_joules  					= 16,
	UNITS_kilojoules  				= 17,
	UNITS_kilojoules_per_kilogram  			= 125,
	UNITS_megajoules  				= 126,
	UNITS_watt_hours  				= 18,
	UNITS_kilowatt_hours  				= 19,
	UNITS_megawatt_hours  				= 146,
	UNITS_btus  					= 20,
	UNITS_kilo_btus  				= 147,
	UNITS_mega_btus  				= 148,
	UNITS_therms  					= 21,
	UNITS_ton_hours  				= 22,
	UNITS_joules_per_kilogram_dry_air  		= 23,
	UNITS_kilojoules_per_kilogram_dry_air  		= 149,
	UNITS_megajoules_per_kilogram_dry_air  		= 150,
	UNITS_btus_per_pound_dry_air  			= 24,
	UNITS_btus_per_pound  				= 117,
	UNITS_joules_per_degree_Kelvin  		= 127,
	UNITS_kilojoules_per_degree_Kelvin  		= 151,
	UNITS_megajoules_per_degree_Kelvin  		= 152,
	UNITS_joules_per_kilogram_degree_Kelvin  	= 128,
	UNITS_newton  					= 153,
	UNITS_cycles_per_hour  				= 25,
	UNITS_cycles_per_minute  			= 26,
	UNITS_hertz  					= 27,
	UNITS_kilohertz  				= 129,
	UNITS_megahertz 				= 130,
	UNITS_per_hour  				= 131,
	UNITS_grams_of_water_per_kilogram_dry_air  	= 28,
	UNITS_percent_relative_humidity  		= 29,
	UNITS_millimeters  				= 30,
	UNITS_centimeters  				= 118,
	UNITS_meters  					= 31,
	UNITS_inches  					= 32,
	UNITS_feet  					= 33,
	UNITS_candelas  				= 179,
	UNITS_candelas_per_square_meter  		= 180,
	UNITS_watts_per_square_foot  			= 34,
	UNITS_watts_per_square_meter  			= 35,
	UNITS_lumens  					= 36,
	UNITS_luxes  					= 37,
	UNITS_foot_candles  				= 38,
	UNITS_kilograms  				= 39,
	UNITS_pounds_mass  				= 40,
	UNITS_tons  					= 41,
	UNITS_grams_per_second  			= 154,
	UNITS_grams_per_minute  			= 155,
	UNITS_kilograms_per_second  			= 42,
	UNITS_kilograms_per_minute  			= 43,
	UNITS_kilograms_per_hour  			= 44,
	UNITS_pounds_mass_per_second 			= 119,
	UNITS_pounds_mass_per_minute  			= 45,
	UNITS_pounds_mass_per_hour  			= 46,
	UNITS_tons_per_hour  				= 156,
	UNITS_milliwatts  				= 132,
	UNITS_watts  					= 47,
	UNITS_kilowatts  				= 48,
	UNITS_megawatts  				= 49,
	UNITS_btus_per_hour  				= 50,
	UNITS_kilo_btus_per_hour  			= 157,
	UNITS_horsepower  				= 51,
	UNITS_tons_refrigeration 			= 52,
	UNITS_pascals  					= 53,
	UNITS_hectopascals  				= 133,
	UNITS_kilopascals  				= 54,
	UNITS_millibars  				= 134,
	UNITS_bars  					= 55,
	UNITS_pounds_force_per_square_inch  		= 56,
	UNITS_centimeters_of_water  			= 57,
	UNITS_inches_of_water  				= 58,
	UNITS_millimeters_of_mercury  			= 59,
	UNITS_centimeters_of_mercury  			= 60,
	UNITS_inches_of_mercury  			= 61,
	UNITS_degrees_Celsius  				= 62,
	UNITS_degrees_Kelvin  				= 63,
	UNITS_degrees_Kelvin_per_hour  			= 181,
	UNITS_degrees_Kelvin_per_minute    		= 182,
	UNITS_degrees_Fahrenheit    			= 64,
	UNITS_degree_days_Celsius    			= 65,
	UNITS_degree_days_Fahrenheit    		= 66,
	UNITS_delta_degrees_Fahrenheit    		= 120,
	UNITS_delta_degrees_Kelvin    			= 121,
	UNITS_years    					= 67,
	UNITS_months    				= 68,
	UNITS_weeks    					= 69,
	UNITS_days    					= 70,
	UNITS_hours    					= 71,
	UNITS_minutes    				= 72,
	UNITS_seconds    				= 73,
	UNITS_hundredths_seconds    			= 158,
	UNITS_milliseconds    				= 159,
	UNITS_newton_meters    				= 160,
	UNITS_millimeters_per_second   			= 161,
	UNITS_millimeters_per_minute   			= 162,
	UNITS_meters_per_second    			= 74,
	UNITS_meters_per_minute    			= 163,
	UNITS_meters_per_hour    			= 164,
	UNITS_kilometers_per_hour    			= 75,
	UNITS_feet_per_second    			= 76,
	UNITS_feet_per_minute    			= 77,
	UNITS_miles_per_hour    			= 78,
	UNITS_cubic_feet    				= 79,
	UNITS_cubic_meters    				= 80,
	UNITS_imperial_gallons    			= 81,
	UNITS_liters    				= 82,
	UNITS_us_gallons    				= 83,
	UNITS_cubic_feet_per_second    			= 142,
	UNITS_cubic_feet_per_minute    			= 84,
	UNITS_cubic_meters_per_second    		= 85,
	UNITS_cubic_meters_per_minute    		= 165,
	UNITS_cubic_meters_per_hour    			= 135,
	UNITS_imperial_gallons_per_minute    		= 86,
	UNITS_liters_per_second    			= 87,
	UNITS_liters_per_minute    			= 88,
	UNITS_liters_per_hour    			= 136,
	UNITS_us_gallons_per_minute    			= 89,
	UNITS_degrees_angular    			= 90,
	UNITS_degrees_Celsius_per_hour    		= 91,
	UNITS_degrees_Celsius_per_minute    		= 92,
	UNITS_degrees_Fahrenheit_per_hour    		= 93,
	UNITS_degrees_Fahrenheit_per_minute   		= 94,
	UNITS_joule_seconds    				= 183,
	UNITS_kilograms_per_cubic_meter    		= 186,
	UNITS_kilowatt_hours_per_square_meter 		= 137,
	UNITS_kilowatt_hours_per_square_foot    	= 138,
	UNITS_megajoules_per_square_meter    		= 139,
	UNITS_megajoules_per_square_foot    		= 140,
	UNITS_no_units    				= 95,
	UNITS_newton_seconds    			= 187,
	UNITS_newtons_per_meter    			= 188,
	UNITS_parts_per_million    			= 96,
	UNITS_parts_per_billion    			= 97,
	UNITS_percent    				= 98,
	UNITS_percent_obscuration_per_foot    		= 143,
	UNITS_percent_obscuration_per_meter    		= 144,
	UNITS_percent_per_second    			= 99,
	UNITS_per_minute    				= 100,
	UNITS_per_second    				= 101,
	UNITS_psi_per_degree_Fahrenheit    		= 102,
	UNITS_radians    				= 103,
	UNITS_radians_per_second    			= 184,
	UNITS_revolutions_per_minute    		= 104,
	UNITS_square_meters_per_Newton    		= 185,
	UNITS_watts_per_meter_per_degree_Kelvin 	= 189,
	UNITS_watts_per_square_meter_degree_kelvin    	= 141,
}BACnetEngineeringUnits;

typedef enum bacnet_tsm_states
{
    IDLE,
    SEGMENTED_REQUEST,
    AWAIT_CONFIRMATION,
    SEGMENTED_CONF,
    AWAIT_RESPONSE,
	SEGMENTED_RESPONSE
}bacnet_tsm_states;

typedef enum BACnetSegmentation
{
    BACnetSegmentationSegmentedBoth,
    BACnetSegmentationSegmentedTransmit,
    BACnetSegmentationSegmentedReceive,
    BACnetSegmentationNoSegmentation
}BACnetSegmentation;

//Modified CG 29 Sep 2005
typedef enum BACnetObjectType
{
	BACnetObjectTypeNone					= 999,
    BACnetObjectTypeAnalogInput				= 0,
    BACnetObjectTypeAnalogOutput			= 1,
    BACnetObjectTypeAnalogValue				= 2,
    BACnetObjectTypeBinaryInput				= 3,
    BACnetObjectTypeBinaryOutput			= 4,
    BACnetObjectTypeBinaryValue				= 5,
    BACnetObjectTypeCalendar				= 6,
    BACnetObjectTypeCommand					= 7,
    BACnetObjectTypeDevice					= 8,
    BACnetObjectTypeEventEnrolment			= 9,
    BACnetObjectTypeFile					= 10,
    BACnetObjectTypeGroup					= 11,
    BACnetObjectTypeLoop					= 12,
    BACnetObjectTypeMultistateInput			= 13,
    BACnetObjectTypeMultistateOutput		= 14,
    BACnetObjectTypeNotificationClass		= 15,
    BACnetObjectTypeProgram					= 16,
    BACnetObjectTypeSchedule				= 17,
	BACnetObjectTypeAveraging				= 18,
	BACnetObjectTypeMultistateValue			= 19,
	BACnetObjectTypeTrendLog				= 20,
	BACnetObjectTypeLifeSafetyPoint			= 21,
	BACnetObjectTypeLifeSafetyZone			= 22,
	BACnetObjectTypeAccumulator				= 23,
	BACnetObjectTypePulseConverter			= 24,
}BACnetObjectType;


typedef enum BACnetNotifyType
{
	NotifyTypeAlarm							= 0,
	NotifyTypeEvent							= 1,
	NotifyTypeAckNotification				= 2,
}BACnetNotifyType;

typedef enum BACnetProgramState
{
	BACnetProgramStateIdle,
	BACnetProgramStateLoading,
	BACnetProgramStateRunning,
	BACnetProgramStateWaiting,
	BACnetProgramStateHalted,
	BACnetProgramStateUnloading
}BACnetProgramState;

typedef enum BACnetProgramRequest
{
	BACnetProgramRequestReady,
	BACnetProgramRequestLoad,
	BACnetProgramRequestRun,
	BACnetProgramRequestHalt,
	BACnetProgramRequestRestart,
	BACnetProgramRequestUnload
}BACnetProgramRequest;

typedef enum BACnetProgramError
{
	BACnetProgramErrorNormal,
	BACnetProgramErrorLoadFailed,
	BACnetProgramErrorInternal,
	BACnetProgramErrorProgram,
	BACnetProgramErrorOther
}BACnetProgramError;

typedef enum BACnetAction
{
	BACnetActionDirect,
	BACnetActionReverse
}BACnetAction;

typedef enum BACnetReliability
{
	BACnetReliabilityNoFaultDetected				= 0,
	BACnetReliabilityNoSensor						= 1,
	BACnetReliabilityOverRange						= 2,
	BACnetReliabilityUnderRange						= 3,
	BACnetReliabilityOpenLoop						= 4,
	BACnetReliabilityShortedLoop					= 5,
	BACnetReliabilityNoOutput						= 6,
	BACnetReliabilityUnreliableOther				= 7,
	BACnetReliabilityProcessError					= 8,
	BACnetReliabilityMultistateFault				= 9,
	BACnetReliabilityConfigurationError				= 10,
}BACnetReliability;

typedef enum BACnetPolarity
{
	BACnetPolarityNormal,
	BACnetPolarityReverse
}BACnetPolarity;

typedef enum BACnetFileAccessMethod
{
	BACnetFileAccessMethodUndefined = 999,
	BACnetFileAccessMethodRecordAccess = 0,
	BACnetFileAccessMethodStreamAccess,
}BACnetFileAccessMethod;

typedef enum BACnetDeviceStatus
{
    BACnetDeviceStatusNone					= -1,
    BACnetDeviceStatusOperational			= 0,
    BACnetDeviceStatusOperationalReadOnly	= 1,
    BACnetDeviceStatusDownloadRequired		= 2,
    BACnetDeviceStatusDownloadInProgress	= 3,
    BACnetDeviceStatusNonOperational		= 4,
    BACnetDeviceStatusBackupInProgress		= 5
}BACnetDeviceStatus;

//Modified CG 30 Sep 2005
typedef enum BACnetEventType
{
	BACnetEventTypeUndefined				= 999,
	BACnetEventTypeChangeOfBitString		= 0,
	BACnetEventTypeChangeOfState			= 1,
	BACnetEventTypeChangeOfValue			= 2,
	BACnetEventTypeCommandFailure			= 3,
	BACnetEventTypeFloatingLimit			= 4,
	BACnetEventTypeOutOfRange				= 5,
	BACnetEventTypeComplexEventType			= 6,
	BACnetEventTypeBufferReady				= 7,
	BACnetEventTypeChangeOfLifeSafety		= 8,
	BACnetEventTypeExtended					= 9,
	BACnetEventTypeBufferReadyNew			= 10,
}BACnetEventType;

typedef enum BACnetExtendedEventChoice
{
	BACnetExtendedEventChoiceNull			= 0,
	BACnetExtendedEventChoiceReal			= 1,
	BACnetExtendedEventChoiceInteger		= 2,
	BACnetExtendedEventChoiceBoolean		= 3,
	BACnetExtendedEventChoiceDouble			= 4,
	BACnetExtendedEventChoiceOctet			= 5,
	BACnetExtendedEventChoiceBitstring		= 6,
	BACnetExtendedEventChoiceEnum			= 7,
	BACnetExtendedEventChoicePropertyValue	= 8,
}BACnetExtendedEventChoice;

typedef enum MaxAPDUlengthAccepted
{
	MaxAPDUlengthAcceptedFiftyOctets,
    MaxAPDUlengthAcceptedOneHundedTwentyEightOctets,
    MaxAPDUlengthAcceptedTwoHundedSixOctets,
    MaxAPDUlengthAcceptedFourHundredEightyOctets,
    MaxAPDUlengthAcceptedOneThousandTwentyFourOctets,
    MaxAPDUlengthAcceptedFouteenHundredSeventySixOctets
}MaxAPDUlengthAccepted;

typedef enum BACnetServiceType
{
    SERVICE_CONF_SERV,
    SERVICE_UNCONF_SERV,
    SERVICE_SEGMENT_ACK,
    SERVICE_ERROR,
    SERVICE_REJECT,
    SERVICE_ABORT
}BACnetServiceType;

typedef enum BACnetServiceDirection
{
    ServiceDirectionRequest,
    ServiceDirectionIndication,
    ServiceDirectionResponse,
    ServiceDirectionConfirm
}BACnetServiceDirection;



//Modified CG 29 Sep 2005

typedef enum BACnetLifeSafetyState
{
	LifeSafetState_Quiet								= 0,
	LifeSafetState_PreAlarm								= 1,
	LifeSafetState_Alarm								= 2,
	LifeSafetState_Fault								= 3,
	LifeSafetState_FaultPreAlarm						= 4,
	LifeSafetState_FaultAlarm							= 5,
	LifeSafetState_NotReady								= 6,
	LifeSafetState_Active								= 7,
	LifeSafetState_Tamper								= 8,
	LifeSafetState_TestAlarm							= 9,
	LifeSafetState_TestActive							= 10,
	LifeSafetState_TestFault							= 11,
	LifeSafetState_TestFaultAlarm						= 12,
	LifeSafetState_Holdup								= 13,
	LifeSafetState_Duress								= 14,
	LifeSafetState_TamperAlarm							= 15,
	LifeSafetState_Abnormal								= 16,
	LifeSafetState_EmergencyPower						= 17,
	LifeSafetState_Delayed								= 18,
	LifeSafetState_Blocked								= 19,
	LifeSafetState_LocalAlarm							= 20,
	LifeSafetState_GeneralAlarm							= 21,
	LifeSafetState_Supervisory							= 22,
	LifeSafetState_TestSupervisory						= 23
}BACnetLifeSafetyState;

typedef enum BACnetMaintenance
{
	Maintenance_None									= 0,
	Maintenance_PeriodicTest							= 1,
	Maintenance_NeedServiceOperational					= 2,
	Maintenance_NeedServiceInoperative					= 3
}BACnetMaintenance;

typedef enum BACnetSilencedState
{
	SilencedState_Unsilenced							= 0,
	SilencedState_AudibleSilenced						= 1,
	SilencedState_VisibleSilenced						= 2,
	SilencedState_AllSilenced							= 3
}BACnetSilencedState;

typedef enum BACnetLifeSafetyMode
{
	LifeSafetyMode_Off									= 0,
	LifeSafetyMode_On									= 1,
	LifeSafetyMode_Test									= 2,
	LifeSafetyMode_Manned								= 3,
	LifeSafetyMode_Unmanned								= 4,
	LifeSafetyMode_Armed								= 5,
	LifeSafetyMode_Disarmed								= 6,
	LifeSafetyMode_Prearmed								= 7,
	LifeSafetyMode_Slow									= 8,
	LifeSafetyMode_Fast									= 9,
	LifeSafetyMode_Disconnected							= 10,
	LifeSafetyMode_Enabled								= 11,
	LifeSafetyMode_Disabled								= 12,
	LifeSafetyMode_AutomaticReleaseDisabled				= 13,
	LifeSafetyMode_Default								= 14
}BACnetLifeSafetyMode;

typedef enum BACnetLifeSafetyOperation
{
	LifeSafetyOperation_None							= 0,
	LifeSafetyOperation_Silence							= 1,
	LifeSafetyOperation_SilenceAudible					= 2,
	LifeSafetyOperation_SilenceVisual					= 3,
	LifeSafetyOperation_Reset							= 4,
	LifeSafetyOperation_ResetAlarm						= 5,
	LifeSafetyOperation_ResetFault						= 6,
	LifeSafetyOperation_Unsilence						= 7,
	LifeSafetyOperation_UnsilenceAudible				= 8,
	LifeSafetyOperation_ResetFaultVisual				= 9
}BACnetLifeSafetyOperation;

//BACnet Property Identifier
typedef enum BACnetPropertyIdentifier
{
    Property_acceptedModes								= 175,
    Property_ackedTransitions							= 0,
    Property_ackRequired								= 1,
    Property_action										= 2,
    Property_actionText									= 3,
    Property_activeText									= 4,
    Property_activeVtSessions							= 5,
	Property_activeCOVSubscriptions						= 152,
	Property_adjustValue								= 176,
    Property_alarmValue									= 6,
    Property_alarmValues								= 7,
    Property_all										= 8,
    Property_allWritesSuccessful						= 9,
    Property_apduSegmentTimeout							= 10,
    Property_apduTimeout								= 11,
    Property_applicationSoftwareVersion					= 12,
    Property_archive									= 13,
    Property_attemptedSamples							= 124,
    Property_averageValue								= 125,
	Property_backupFailureTimeout						= 153,
    Property_bias										= 14,
	Property_bufferSize									= 126,
    Property_changeOfStateCount							= 15,
    Property_changeOfStateTime							= 16,
	Property_notificationClass							= 17,
	// 18 was deleted !!!!
	Property_clientCovIncrement							= 127,
	Property_configurationFiles							= 154,
    Property_controlledVariableReference				= 19,
    Property_controlledVariableUnits					= 20,
	Property_controlledVariableValue					= 21,
    Property_count										= 177,
    Property_countBeforeChange							= 178,
    Property_countChangeTime							= 179,
    Property_covIncrement								= 22,
    Property_covPeriod									= 180,
	Property_covResubscriptionInterval					= 128,
	Property_currentNotifyTime							= 129,
	Property_databaseRevision							= 155,
    Property_dateList									= 23,
    Property_daylightSavingsStatus						= 24,
    Property_deadband									= 25,
    Property_derivativeConstant							= 26,
    Property_derivativeConstantUnits					= 27,
    Property_description								= 28,
    Property_descriptionOfHalt							= 29,
    Property_deviceAddressBinding						= 30,
    Property_deviceType									= 31,
	Property_directReading								= 156,
    Property_effectivePeriod							= 32,
    Property_elapsedActiveTime							= 33,
    Property_errorLimit									= 34,
    Property_eventEnable								= 35,
    Property_eventParameters							= 83,
    Property_eventState									= 36,
	Property_eventTimeStamps							= 130,
    Property_eventType									= 37,
    Property_exceptionSchedule							= 38,
    Property_faultValues								= 39,
    Property_feedbackValue								= 40,
    Property_fileAccessMethod							= 41,
    Property_fileSize									= 42,
    Property_fileType									= 43,
    Property_firmwareRevision							= 44,
    Property_highLimit									= 45,
    Property_inactiveText								= 46,
    Property_inProcess									= 47,
	Property_inputReference								= 181,
    Property_instanceOf									= 48,
    Property_integralConstant							= 49,
    Property_integralConstantUnits						= 50,
    Property_issueConfirmedNotifications				= 51,
	Property_lastNotifyRecord							= 173,
	Property_lastRestoreTime							= 157,
	Property_lifeSafetyAlarmValues						= 166,
    Property_limitEnable								= 52,
	Property_limitMonitoringInterval					= 182,
    Property_listOfGroupMembers							= 53,
    Property_listOfObjectReferences						= 54,
    Property_listOfSessionKeys							= 55,
    Property_localDate									= 56,
    Property_localTime									= 57,
    Property_location									= 58,
	Property_logBuffer									= 131,
	Property_logDeviceObjectProperty					= 132,
	Property_logEnable									= 133,
	Property_logInterval								= 134,
	Property_loggingObject								= 183,
	Property_loggingRecord								= 184,
    Property_lowLimit									= 59,
	Property_maintenanceRequired						= 158,
    Property_manipulatedVariableReference				= 60,
    Property_maximumOutput								= 61,
    Property_maximumValue								= 135,
    Property_maximumValueTimestamp						= 149,
    Property_maxAPDUlengthAccepted						= 62,
    Property_maxInfoFrames								= 63,
    Property_maxMaster									= 64,
    Property_maxPresValue								= 65,
    Property_maxSegmentsAccepted						= 167,
	Property_memberOf									= 159,
    Property_minimumOffTime								= 66,
    Property_minimumOnTime								= 67,
    Property_minimumOutput								= 68,
    Property_minimumValue								= 136,
    Property_minimumValueTimestamp						= 150,
    Property_minPresValue								= 69,
	Property_mode										= 160,
    Property_modelName									= 70,
    Property_modificationDate							= 71,
	Property_notificationThreshold						= 137,
    Property_notifyType									= 72,
    Property_numberOfAPDUretries						= 73,
    Property_numberOfStates								= 74,
    Property_objectIdentifier							= 75,
    Property_objectList									= 76,
    Property_objectName									= 77,
    Property_objectPropertyReference					= 78,
    Property_objectType									= 79,
	Property_operationExpected							= 161,
    Property_optional									= 80,
    Property_outOfService								= 81,
    Property_outputUnits								= 82,
    Property_polarity									= 84,
	Property_prescale									= 185,
    Property_presentValue								= 85,
	Property_previousNotifyTime							= 138,
	Property_protocolRevision							= 139,
    Property_priority									= 86,
    Property_priorityArray								= 87,
    Property_priorityForWriting							= 88,
    Property_processIdentifier							= 89,
    Property_profileName								= 168,
    Property_programChange								= 90,
    Property_programLocation							= 91,
    Property_programState								= 92,
    Property_proportionalConstant						= 93,
    Property_proportionalConstantUnits					= 94,
    Property_protocolConformanceClass					= 95,
    Property_protocolObjectTypesSupported				= 96,
    Property_protocolServicesSupported					= 97,
    Property_protocolVersion							= 98,
	Property_pulseRate									= 186,
    Property_readOnly									= 99,
    Property_reasonForHalt								= 100,
    Property_recipient									= 101,
    Property_recipientList								= 102,
	Property_recordsSinceNotification					= 140,
	Property_recordCount								= 141,
    Property_reliability								= 103,
    Property_relinquishDefault							= 104,
    Property_required									= 105,
    Property_resolution									= 106,
	Property_scale										= 187,
	Property_scaleFactor								= 188,
	Property_scheduleDefault							= 174,
    Property_segmentationSupported						= 107,
    Property_setpoint									= 108,
    Property_setpointReference							= 109,
	Property_setting									= 162,
	Property_silenced									= 163,
	Property_startTime									= 142,
    Property_stateText									= 110,
    Property_statusFlags								= 111,
	Property_stopTime									= 143,
	Property_stopWhenFull								= 144,
    Property_systemStatus								= 112,
    Property_timeDelay									= 113,
    Property_timeOfActiveTimeReset						= 114,
    Property_timeOfStateCountReset						= 115,
    Property_timeSynchronizationRecipients				= 116,
	Property_totalRecordCount							= 145,
	Property_trackingValue								= 164,
    Property_units										= 117,
    Property_updateInterval								= 118,
	Property_updateTime									= 189,
    Property_utcOffset									= 119,
    Property_validSamples								= 146,
	Property_valueBeforeChange							= 190,
	Property_valueSet									= 191,
	Property_valueChangeTime							= 192,
    Property_varianceValue								= 151,
    Property_vendorIdentifier							= 120,
    Property_vendorName									= 121,
    Property_vtClassesSupported							= 122,
    Property_weeklySchedule								= 123,
    Property_windowInterval								= 147,
    Property_windowSamples								= 148,
    Property_zoneMembers								= 165,


	//Proprietary Properties
    Property_lastTransition								= 551,
    Property_activeSubscriptions						= 552,
    Property_previousRecordCount						= 553,
    Property_ackedTransitionsNotification,
    Property_fileName,
	Property_listOfDataLinkLayerConnectStrings,
	Property_VirtualDevice,
	Property_listOfVirtualDevices,
	Property_macAddress,
	Property_saveRequired,
	Property_virtualIOKey,
	Property_deviceScanEnable,
	Property_localDeviceID,
	Property_BACnetIPEnable,
	Property_BACnetIPNetworkNumber,
	Property_BACnetEthernetEnable,
	Property_BACnetEthernetNetworkNumber,
	Property_VirtualDeviceAddress,
	Property_LastEventEnrolmentTime,
	Property_deviceScanStatus,
	Property_remoteDeviceAddress,
	Property_remoteDeviceMacAddress,
	Property_remoteDeviceMaxAPDULength,
	Property_remoteDeviceLinkLayerNetworkNumber,
	Property_listOfBACnetDataLinkLayerConnectStrings
}BACnetPropertyIdentifier;

typedef enum BACnetErrorClass
{
    BACnetErrorClassNone =			-1,
    BACnetErrorClassDevice =		0,
    BACnetErrorClassObject =		1,
    BACnetErrorClassProperty =		2,
    BACnetErrorClassResources =		3,
    BACnetErrorClassSecurity =		4,
    BACnetErrorClassServices =		5,
    BACnetVt =						6,
	BACnetErrorClassNoResponse =	1000
}BACnetErrorClass;

//Modified CG 30 Sep 2005
typedef enum BACnetErrorCode
{
    BACnetErrorCodeNone =								-1,
    BACnetErrorCodeOther =								0,
    BACnetErrorCodeAuthenticationFailed =				1,
    BACnetErrorCodeCharacterSetNotSupported =			41,
    BACnetErrorCodeConfigurationInProgress =			2,
    BACnetErrorCodeDeviceBusy =							3,
    BACnetErrorCodeDynamicCreationNotSupported =		4,
    BACnetErrorCodeFileAccessDenied =					5,
    BACnetErrorCodeIncompatiblesecurityLevels =			6,
    BACnetErrorCodeInconsistentParameters =				7,
    BACnetErrorCodeInconsistentSelectionCriteria =		8,
    BACnetErrorCodeInvalidAarrayIndex =					42,
    BACnetErrorCodeInvalidConfigurationData =			46,
    BACnetErrorCodeInvalidDataType =					9,
    BACnetErrorCodeInvalidFileAccessMethod =			10,
    BACnetErrorCodeInvalidFileStartPosition =			11,
    BACnetErrorCodeInvalidOperatorName =				12,
    BACnetErrorCodeInvalidParameterDataType =			13,
    BACnetErrorCodeInvalidTimeStamp =					14,
    BACnetErrorCodeKeyGenerationError =					15,
    BACnetErrorCodeMissingRequiredParameter =			16,
    BACnetErrorCodeNoObjectsOfSpecifiedType =			17,
    BACnetErrorCodeNoSpaceForObject =					18,
    BACnetErrorCodeNoSpaceToAddListElement =			19,
    BACnetErrorCodeNoSpaceToWriteProperty =				20,
    BACnetErrorCodeNoVTSessionsAvailable =				21,
    BACnetErrorCodePropertyIsNotAList =					22,
    BACnetErrorCodeObjectDeletionNotPermitted =			23,
    BACnetErrorCodeObjectIdentifierAlreadyExists =		24,
    BACnetErrorCodeOperationalProblem =					25,
    BACnetErrorCodePasswordFailure =					26,
    BACnetErrorCodeReadAccessDenied =					27,
    BACnetErrorCodeSecurityNotSupported =				28,
    BACnetErrorCodeServiceRequestDenied =				29,
    BACnetErrorCodeTimeout =							30,
    BACnetErrorCodeUnknownObject =						31,
    BACnetErrorCodeUnknownProperty =					32,
//  BACnetErrorCodeThisErrorCodeWasRemoved =			33,
    BACnetErrorCodeUnknownVTClass =						34,
    BACnetErrorCodeUnknownVTSession =					35,
    BACnetErrorCodeUnsupportedObjectType =				36,
    BACnetErrorCodeValueOutOfRange =					37,
    BACnetErrorCodeVTSessionAlreadyClosed =				38,
    BACnetErrorCodeVTSessionTerminationFailure =		39,
    BACnetErrorCodeWriteAccessDenied =					40,
    BACnetErrorCodeCOVSubscriptionFailed =				43,
    BACnetErrorCodeNotCOVProperty =						44,
    BACnetErrorCodeNotUpdated =							500
}BACnetErrorCode;

typedef enum BACnetAbortReason
{
    BACnetAbortReasonEmpty = -1,
    BACnetAbortReasonOther,
    BACnetAbortReasonBufferOverflow,
    BACnetAbortReasonInvalidAPDUinThisState,
    BACnetAbortReasonPreemptedByHigherPriorityTask,
    BACnetAbortReasonSegmentationNotSupported,
    //Vendor specific
    BACnetAbortReasonInsufficientResources = 101
}BACnetAbortReason;

typedef enum BACnetRejectReason
{
    BACnetRejectReasonEmpty = -1,
    BACnetRejectReasonOther,
    BACnetRejectReasonBufferOverflow,
    BACnetRejectReasonInconsistentParameters,
    BACnetRejectReasonInvalidParameterDataType,
    BACnetRejectReasonInvalidTag,
    BACnetRejectReasonMissingRequiredParameter,
    BACnetRejectReasonParameterOutOfRange,
    BACnetRejectReasonTooManyArguments,
    BACnetRejectReasonUnidentifiedEnumeration,
    BACnetRejectReasonUnrecognisedService,
}BACnetRejectReason;

typedef enum BACnetConfirmedServiceChoice
{
    BACnetConfirmedServiceChoiceAcknowledgeAlarm					= 0,
    BACnetConfirmedServiceChoiceConfirmedCOVnotification			= 1,
    BACnetConfirmedServiceChoiceConfirmedEventNotification			= 2,
    BACnetConfirmedServiceChoiceGetAlarmSummary						= 3,
    BACnetConfirmedServiceChoiceGetEnrolmentSummary					= 4,
    BACnetConfirmedServiceChoiceSubscribeCOV						= 5,
    BACnetConfirmedServiceChoiceAtomicReadFile						= 6,
    BACnetConfirmedServiceChoiceAtomicWriteFile						= 7,
    BACnetConfirmedServiceChoiceAddListElement						= 8,
    BACnetConfirmedServiceChoiceRemoveListElement					= 9,
    BACnetConfirmedServiceChoiceCreateObject						= 10,
    BACnetConfirmedServiceChoiceDeleteObject						= 11,
    BACnetConfirmedServiceChoiceReadProperty						= 12,
    BACnetConfirmedServiceChoiceReadPropertyConditional				= 13,
    BACnetConfirmedServiceChoiceReadPropertyMultiple				= 14,
    BACnetConfirmedServiceChoiceWriteProperty						= 15,
    BACnetConfirmedServiceChoiceWritePropertyMultiple				= 16,
    BACnetConfirmedServiceChoiceDeviceCommunicationControl			= 17,
    BACnetConfirmedServiceChoiceConfirmedPrivateTransfer			= 18,
    BACnetConfirmedServiceChoiceConfirmedTextMessage				= 19,
    BACnetConfirmedServiceChoiceReinitializeDevice					= 20,
    BACnetConfirmedServiceChoiceVtOpen								= 21,
    BACnetConfirmedServiceChoiceVtClose								= 22,
    BACnetConfirmedServiceChoiceVtData								= 23,
    BACnetConfirmedServiceChoiceAuthenticate						= 24,
    BACnetConfirmedServiceChoiceRequestKey							= 25,
	BACnetConfirmedServiceChoiceReadRange							= 26,
	BACnetConfirmedServiceChoiceLifeSafetyOperation					= 27,
	BACnetConfirmedServiceChoiceSubscribeCOVProperty				= 28,
	BACnetConfirmedServiceChoiceGetEventInformation					= 29,
}BACnetConfirmedServiceChoice;

typedef enum BACnetUnConfirmedServiceChoice
{
    BACnetUnConfirmedServiceChoiceIam								= 0,
    BACnetUnConfirmedServiceChoiceIhave								= 1,
    BACnetUnConfirmedServiceChoiceUnconfirmedCOVNotification		= 2,
    BACnetUnConfirmedServiceChoiceUnconfirmedEventNotification		= 3,
    BACnetUnConfirmedServiceChoiceUnconfirmedPrivateTransfer		= 4,
    BACnetUnConfirmedServiceChoiceUnconfirmedTextMessage			= 5,
    BACnetUnConfirmedServiceChoiceTimeSynchronization				= 6,
    BACnetUnConfirmedServiceChoiceWhoHas							= 7,
    BACnetUnConfirmedServiceChoiceWhoIs								= 8,
    BACnetUnConfirmedServiceChoiceUtcTimeSynchronization			= 9
}BACnetUnConfirmedServiceChoice;

typedef enum BACnetErrorChoice
{
    BACnetErrorChoiceEmpty								= -1,
    BACnetErrorChoiceAcknowledgeAlarm					= 0,
    BACnetErrorChoiceConfirmedCOVnotification			= 1,
    BACnetErrorChoiceConfirmedEventNotification			= 2,
    BACnetErrorChoiceGetAlarmSummary					= 3,
    BACnetErrorChoiceGetEnrolmentSummary				= 4,
    BACnetErrorChoiceSubscribeCOV						= 5,
    BACnetErrorChoiceAtomicReadFile						= 6,
    BACnetErrorChoiceAtomicWriteFile					= 7,
    BACnetErrorChoiceAddListElement						= 8,
    BACnetErrorChoiceRemoveListElement					= 9,
    BACnetErrorChoiceCreateObject						= 10,
    BACnetErrorChoiceDeleteObject						= 11,
    BACnetErrorChoiceReadProperty						= 12,
    BACnetErrorChoiceReadPropertyConditional			= 13,
    BACnetErrorChoiceReadPropertyMultiple				= 14,
    BACnetErrorChoiceWriteProperty						= 15,
    BACnetErrorChoiceWritePropertyMultiple				= 16,
    BACnetErrorChoiceDeviceCommunicationControl			= 17,
    BACnetErrorChoiceConfirmedPrivateTransfer			= 18,
    BACnetErrorChoiceConfirmedTextMessage				= 19,
    BACnetErrorChoiceReinitializeDevice					= 20,
    BACnetErrorChoiceVtOpen								= 21,
    BACnetErrorChoiceVtClose							= 22,
    BACnetErrorChoiceVtData								= 23,
    BACnetErrorChoiceAuthenticate						= 24,
    BACnetErrorChoiceRequestKey							= 25,
    BACnetErrorChoiceReadRange							= 26,
	BACnetErrorChoiceLifeSafetyOperation				= 27,
	BACnetErrorChoiceSubscribeCOVProperty				= 28,
	BACnetErrorChoiceGetEventInformation				= 29,
    BACnetErrorChoiceRemoteLogin						= 501,
}BACnetErrorChoice;

//BACnet Application Tag Numbers
typedef enum BACnetApplicationTagNumber
{
    bacnetDT_Null									= 0,	//Primitive data type - Null Value
    bacnetDT_Boolean								= 1,	//Primitive data type - Boolean Value
    bacnetDT_UnsignedInteger						= 2,	//Primitive data type - Unsigned Integer Value
    bacnetDT_SignedInteger							= 3,	//Primitive data type - Signed Integer Value
    bacnetDT_Real									= 4,	//Primitive data type - Real Value
    bacnetDT_Double									= 5,	//Primitive data type - Double Value
    bacnetDT_OctetString							= 6,	//Primitive data type - Octet String
    bacnetDT_CharacterString						= 7,	//Primitive data type - Character String
    bacnetDT_BitString								= 8,	//Primitive data type - Bit String
    bacnetDT_Enumerated								= 9,	//Primitive data type - Enumerated Value
    bacnetDT_Date									= 10,	//Primitive data type - Date Value
    bacnetDT_Time									= 11,	//Primitive data type - Time Value
    bacnetDT_ObjectIdentifier						= 12,	//Primitive data type - Object Identifier
	bacnetDT_SignedLong								= 20,
	bacnetDT_Address								= 21,	//Complex Data Type
    bacnetDT_DateTime								= 22,	//Complex Data Type
    bacnetDT_CalendarEntry							= 23,	//Complex Data Type
	bacnetDT_DateRange								= 24,	//Complex Data Type
	bacnetDT_WeekNDay								= 25,	//Complex Data Type
	bacnetDT_TimeValue								= 26,	//Complex Data Type
    bacnetDT_TimeStamp								= 27,	//Complex Data Type
	bacnetDT_ActionCommand							= 28,	//Complex Data Type
	bacnetDT_SessionKey								= 29,	//Complex Data Type
	bacnetDT_Recipient								= 30,	//Complex Data Type
	bacnetDT_RecipientProcess						= 31,	//Complex Data Type
	bacnetDT_AddressBinding							= 32,	//Complex Data Type
	bacnetDT_ObjectPropertyReference				= 33,	//Complex Data Type
	bacnetDT_FunctionBlockIdentifier				= 34,
	bacnetDT_FunctionBlockInput						= 35,
	bacnetDT_DeviceObjectPropertyReference			= 36,	//Complex Data Type
	bacnetDT_LogRecord								= 37,
	bacnetDT_ReadAccessSpecification				= 38,
	bacnetDT_WriteAccessSpecification				= 39,
	bacnetDT_AlarmSummaryAckElement					= 40,
	bacnetDT_EnrolmentSummaryAckElement				= 41,
	bacnetDT_ReadAccessResult						= 42,
	bacnetDT_ReadAccessPropertyValue				= 43,
	bacnetDT_PropertyReference						= 44,
	bacnetDT_Destination							= 45,
	bacnetDT_DailySchedule							= 46,
	bacnetDT_SpecialEvent							= 47,
	bacnetDT_PriorityValue							= 48,
	bacnetDT_PropertyValue							= 49,
	bacnetDT_PropertyStates							= 50,
	bacnetDT_EventParameter							= 51,
	bacnetDT_NotificationParameter					= 52,
	bacnetDT_SelectionCriteria						= 53,
	bacnetDT_ClientCOV								= 54,
	bacnetDT_ObjectPropertyValue					= 55,
	bacnetDT_Element								= 56,
	bacnetDT_COVSubscription						= 101,
	bacnetDT_Scale									= 102,
	bacnetDT_Prescale								= 103,
	bacnetDT_AccumulatorRecord						= 104,
	bacnetDT_EventSummaryElement					= 105,
	bacnetDT_DeviceObjectPropertyValue				= 110,
	bacnetDT_SetpointReference						= 112,
	//BACnet Arrays
	bacnetDT_Array									= 57,
	bacnetDT_PriorityArray							= 58,
	bacnetDT_ActionList								= 59,
    bacnetDT_ArrayOfBACnetLogRecord					= 60,
    bacnetDT_ArrayOfBACnetCalendarEntry				= 61,
    bacnetDT_ArrayOfBACnetActionList				= 62,
    bacnetDT_ArrayOfBACnetActionCommand				= 63,
    bacnetDT_ArrayOfCharacterString					= 64,
    bacnetDT_ArrayOfBACnetObjectIdentifier			= 65,
    bacnetDT_ArrayOfBACnetFunctionBlockIdentifier	= 66,
    bacnetDT_ArrayOfBACnetVTClass					= 67,
    bacnetDT_ArrayOfBACnetVTSession					= 68,
    bacnetDT_ArrayOfBACnetSessionKey				= 69,
    bacnetDT_ArrayOfBACnetRecipient					= 70,
    bacnetDT_ArrayOfBACnetAddressBinding			= 71,
    bacnetDT_ArrayOfReadAccessSpecification			= 72,
    bacnetDT_ArrayOfReadAccessResult				= 73,
	bacnetDT_ArrayOfReadAccessPropertyValue			= 74,
    bacnetDT_ArrayOfUnsigned						= 75,
    bacnetDT_ArrayOfEnumerated						= 111,
    bacnetDT_ArrayOfBACnetDestination				= 76,
    bacnetDT_ArrayOfBACnetDailySchedule				= 77,
    bacnetDT_ArrayOfSpecialEvent					= 78,
    bacnetDT_ArrayOfBACnetObjectPropertyReference	= 79,
    bacnetDT_ArrayOfBACnetTimeValue					= 80,
	bacnetDT_ArrayOfBitString						= 81,
	bacnetDT_ArrayOfBACnetPropertyStates			= 82,
	bacnetDT_ArrayOfBACnetObjectPropertyValue		= 83,
    bacnetDT_EventTimeStamps						= 84,

	//BACnet Lists
	bacnetDT_List									= 85,
    bacnetDT_SequenceOfBACnetPropertyReference		= 86,
    bacnetDT_SequenceOfBACnetPropertyValue			= 87,
    bacnetDT_SequenceOfWriteAccessSpecification		= 88,
    bacnetDT_SequenceOfReadAccessSpecification		= 89,
    bacnetDT_SequenceOfReadAccessResult				= 90,
//	bacnetDT_SequenceOfReadAccessPropertyValue		= 91, Not Used
	bacnetDT_SequenceOfSelectionCriteria			= 92,
	bacnetDT_SequenceOfAlarmSummaryAckElement		= 93,
	bacnetDT_SequenceOfEnrolmentSummaryAckElement	= 94,
	bacnetDT_SequenceOfEventSummaryElement			= 106,
	bacnetDT_SequenceOfCOVSubscription				= 107,
	bacnetDT_SequenceOfLifeSafetyMode				= 108,
	bacnetDT_SequenceOfLifeSafetyState				= 109,
	bacnetDT_SequenceOfElements						= 95,
	bacnetDT_DataLinkLayer							= 96,

	//Constructed Data Type
	bacnetDT_Constructed							= 97,
	bacnetDT_ArrayOfConstructed						= 98,

}BACnetApplicationTagNumber;

typedef enum BACnetReturnDataType
{
    BACnetReturnDataTypeEmpty = -1,
    BACnetReturnDataTypeBACnetData,
    BACnetReturnDataTypeBACnetArrayData
}BACnetReturnDataType;

typedef enum BACnetPDUType
{
    BACnetPDUTypeConfirmedRequest,
    BACnetPDUTypeUnconfirmedRequest,
    BACnetPDUTypeSimpleAckPDU,
    BACnetPDUTypeComplexAckPDU,
    BACnetPDUTypeSegmentAckPDU,
    BACnetPDUTypeErrorPDU,
    BACnetPDUTypeRejectPDU,
    BACnetPDUTypeAbortPDU
}BACnetPDUType;

typedef enum BACnetNetworkMessageType
{
    BACnetNetworkMessageTypeNetworkMessageEmpty = -1,
    BACnetNetworkMessageTypeWhoIsRouterToNetwork,
    BACnetNetworkMessageTypeIamRouterToNetwork,
    BACnetNetworkMessageTypeIcouldBeRouterToNetwork,
    BACnetNetworkMessageTypeRejectMessageToNetwork,
    BACnetNetworkMessageTypeRouterBusyToNetwork,
    BACnetNetworkMessageTypeRouterAvailableToNetwork,
    BACnetNetworkMessageTypeInitializingRoutingTable,
    BACnetNetworkMessageTypeInitializingRoutingTableAck,
    BACnetNetworkMessageTypeEstablishingConnectionToNetwork,
    BACnetNetworkMessageTypeDisconnectConnectionToNetwork
}BACnetNetworkMessageType;

typedef enum BACnetIPBVLCFunction
{
	BACnetIPBVLCFunctionBVLCFunctionEmpty = -1,
    BACnetIPBVLCFunctionBVLCResult,
    BACnetIPBVLCFunctionWriteBroadcastDistributionTable,
    BACnetIPBVLCFunctionReadBroadcastDistributionTable,
    BACnetIPBVLCFunctionReadBroadcastDistributionTableAck,
    BACnetIPBVLCFunctionForwardedNPDU,
    BACnetIPBVLCFunctionRegisterForeignDevice,
    BACnetIPBVLCFunctionReadForeignDeviceTable,
    BACnetIPBVLCFunctionReadForeignDeviceTableAck,
    BACnetIPBVLCFunctionDeleteForeignDeviceTableEntry,
    BACnetIPBVLCFunctionDistributeBroadcastToNetwork,
    BACnetIPBVLCFunctionOriginalUnicastNPDU,
    BACnetIPBVLCFunctionOriginalBroadcastNPDU,
}BACnetIPBVLCFunction;

typedef enum BACnetDataLinkLayers
{
    BACnetDataLinkLayerNone,
    BACnetDataLinkLayerIP,
    BACnetDataLinkLayerEthernet,
    BACnetDataLinkLayerMSTP,
    BACnetDataLinkLayerLon,
    BACnetDataLinkLayerAPI,
    BACnetDataLinkLayerPTP,
	BACnetDataLinkLayerAnexH,
    BACnetDataLinkLayerOldAPI,
    BACnetDataLinkLayerLibraryModule,
}BACnetDataLinkLayers;

typedef enum BACnetTimeStampTypes
{
	BACnetTimeStampTime,
	BACnetTimeStampSequenceNumber,
	BACnetTimeStampDateTime
}BACnetTimeStampTypes;

typedef enum BACnetBinaryPV
{
    BACnetBinaryPVInactive,
    BACnetBinaryPVActive
}BACnetBinaryPV;

typedef enum BACnetEventState
{
    BACnetEventStateNormal,
    BACnetEventStateFault,
    BACnetEventStateOffNormal,
    BACnetEventStateHighLimit,
    BACnetEventStateLowLimit,
	BACnetEventStateLifeSafetyAlarm //Modified CG 30 Sep 2005
}BACnetEventState;

typedef enum BACnetPriorityValueTypes
{
    BACnetPriorityValueType_Null,
    BACnetPriorityValueType_Real,
    BACnetPriorityValueType_Binary,
    BACnetPriorityValueType_UnsignedInteger,
    BACnetPriorityValueType_AbstractData
}BACnetPriorityValueTypes;

typedef enum BACnetReadPropertyConditionalSelectionLogic
{
	BACnetReadPropertyConditionalSelectionLogicTypeAnd,
	BACnetReadPropertyConditionalSelectionLogicTypeOr,
	BACnetReadPropertyConditionalSelectionLogicTypeAll
}BACnetReadPropertyConditionalSelectionLogic;

typedef enum BACnetRelationalSpecifier
{
	BACnetRelationalSpecifierEqual,
	BACnetRelationalSpecifierNotEqual,
	BACnetRelationalSpecifierLessThan,
	BACnetRelationalSpecifierGreaterThan,
	BACnetRelationalSpecifierGreaterThanOrEqual
}BACnetRelationalSpecifier;

typedef enum BACnetStateOfDevice
{
	BACnetStateOfDeviceColdstart		= 0,
	BACnetStateOfDeviceWarmstart		= 1,
	BACnetStateOfDeviceStartBackup		= 2,
	BACnetStateOfDeviceEndBackup		= 3,
	BACnetStateOfDeviceStartRestore		= 4,
	BACnetStateOfDeviceEndRestore		= 5,
	BACnetStateOfDeviceAbortRestore		= 6,
}BACnetStateOfDevice;


typedef enum BACnetAcknowledgmentFilter
{
	BACnetAcknowledgmentFilterAll,
	BACnetAcknowledgmentFilterAcked,
	BACnetAcknowledgmentFilterNotAcked
}BACnetAcknowledgmentFilter;

typedef enum BACnetEventStateFilter
{
	BACnetEventStateFilterOffNormal,
	BACnetEventStateFilterFault,
	BACnetEventStateFilterNormal,
	BACnetEventStateFilterAll,
	BACnetEventStateFilterActive
}BACnetEventStateFilter;

typedef enum BACnetObjectSpecifierType
{
	BACnetObjectSpecifierTypeUnknown = -1,
	BACnetObjectSpecifierTypeObjectType = 0,
	BACnetObjectSpecifierTypeObjectIdentifier = 1
}BACnetObjectSpecifierType;

typedef enum BACnetCovCriteriaChoice
{
	BACnetCovCriteriaChoiceBitmask,
	BACnetCovCriteriaChoiceReferencedPropertyIncrement
}BACnetCovCriteriaChoice;

typedef enum TRACE_MESSAGE_TYPE
{
	msgDebug,
	msgBACnetRecievePacket,
	msgBACnetSendPacket
}TRACE_MESSAGE_TYPE;

typedef enum BACnetClientCOVType
{
	BACnetClientCOVTypeReal,
	BACnetClientCOVTypeNull
}BACnetClientCOVType;

typedef enum BACnetLogDatumType
{
	BACnetLogDatumTypeLogStatus,
	BACnetLogDatumTypeBooleanValue,
	BACnetLogDatumTypeRealValue,
	BACnetLogDatumTypeEnumValue,
	BACnetLogDatumTypeUnsignedValue,
	BACnetLogDatumTypeSignedValue,
	BACnetLogDatumTypeBitstringValue,
	BACnetLogDatumTypeNullValue,
	BACnetLogDatumTypeFailure,
	BACnetLogDatumTypeTimeChange,
	BACnetLogDatumTypeAnyValue
}BACnetLogDatumType;

typedef enum BACnetReadRangeRequestRangeType
{
	BACnetReadRangeRequestRangeTypeNone,
	BACnetReadRangeRequestRangeTypeByPosition,
	BACnetReadRangeRequestRangeTypeByTime,
	BACnetReadRangeRequestRangeTypeTimeRange,
	BACnetReadRangeRequestRangeTypeSequenceNumber,
	BACnetReadRangeRequestRangeTypeByTimeNew
}BACnetReadRangeRequestRangeType;

typedef enum BACnetMessageClassChoice
{
	BACnetMessageClassUnknown = -1,
	BACnetMessageClassNumeric = 0,
	BACnetMessageClassCharacter = 1
}BACnetMessageClassChoice;

typedef enum BACnetMessagePriority
{
	BACnetMessagePriorityNormal,
	BACnetMessagePriorityUrgent
}BACnetMessagePriority;

typedef enum BACnetRecipientType
{
	BACnetRecipientTypeNone = -1,
	BACnetRecipientTypeDevice,
	BACnetRecipientTypeAddress
}BACnetRecipientType;

typedef enum BACnetReadAccessReadResultType
{
	BACnetReadAccessReadResultTypePropertyValue,
	BACnetReadAccessReadResultTypePropertyAccessError
}BACnetReadAccessReadResultType;

typedef enum BACnetSpecialEventPeriodType
{
	BACnetSpecialEventPeriodTypeCalendarEntry,
	BACnetSpecialEventPeriodTypeCalendarReference
}BACnetSpecialEventPeriodType;

typedef enum BACnetPropertyStatesChoice
{
	PropertyStateBoolean,
	PropertyStateBACnetBinaryPV,
	PropertyStateBACnetEventType,
	PropertyStateBACnetPolarity,
	PropertyStateBACnetProgramChange,
	PropertyStateBACnetProgramState,
	PropertyStateBACnetReasonForHalt,
	PropertyStateBACnetReliability,
	PropertyStateBACnetState,
	PropertyStateBACnetSystemStatus,
	PropertyStateBACnetUnits,
	PropertyStateUnsigned
}BACnetPropertyStatesChoice;

typedef enum BACnetOnDemandStatusType
{
	BACnetOnDemandStatusNone,
	BACnetOnDemandStatusNotUpdated,
	BACnetOnDemandStatusScanned,
	BACnetOnDemandStatusIdle,
	BACnetOnDemandStatusStopScanning
}BACnetOnDemandStatusType;

typedef enum BACnetVirtualControllerEventType
{
	BACnetVirtualControllerEventTypeEnrolInput,
	BACnetVirtualControllerEventTypeUnenrolInput,
	BACnetVirtualControllerEventTypeEnrolOutput,
	BACnetVirtualControllerEventTypeUnenrolOutput,
	BACnetVirtualControllerEventTypeWrite
}BACnetVirtualControllerEventType;

typedef enum BACnetCalendarEntryType
{
	BACnetCalendarEntryTypeDate,
	BACnetCalendarEntryTypeDateRange,
	BACnetCalendarEntryTypeWeekNDay
}BACnetCalendarEntryType;

typedef enum BACnetFunctionBlockInputType
{
	BACnetFunctionBlockInputTypeDeviceObjectIdentifier,
	BACnetFunctionBlockInputTypeAbstractData
}BACnetFunctionBlockInputType;

typedef enum BACnetConfirmedPrivateTransferServiceNumber
{
	BACnetConfirmedPrivateTransferServiceNumberGetRecordset,
	BACnetConfirmedPrivateTransferServiceNumberExecuteQuery
}BACnetConfirmedPrivateTransferServiceNumber;

typedef enum BACnetRemoteServerType
{
	BACnetRemoteServerTypeNone,
	BACnetRemoteServerTypeXML,
	BACnetRemoteServerTypePDU
}BACnetRemoteServerType;

typedef enum BACnetDriverOperationType
{
	BACnetDriverOperationClient,
	BACnetDriverOperationServer
}BACnetDriverOperationType;

typedef enum BACnetSecurityReturnCode
{
	BACNETSECURITY_IS_REGISTERED = 0,
	BACNETSECURITY_DONGLE_ATTACHED = 1,
	BACNETSECURITY_FILE_NOT_FOUND = -10,
	BACNETSECURITY_PATH_NOT_VALID = -11,
	BACNETSECURITY_NO_PATH = -12,
	BACNETSECURITY_NOT_REMOVABLE = -13,
	BACNETSECURITY_CFG_FAILED = -14,
	BACNETSECURITY_COULD_NOT_WRITE_WININI = -15,
	BACNETSECURITY_COULD_NOT_WRITE_PRIVINI = -16,
	BACNETSECURITY_NOT_REGISTERED = -20,
	BACNETSECURITY_NO_REGINFO = -21,
	BACNETSECURITY_NO_SERNUMBER = -22,
	BACNETSECURITY_FEATURES_NOT_AVAILABLE = -23,
	BACNETSECURITY_WRONG_CODE = -24,
	BACNETSECURITY_INI_CONTROL_FAILED = -25,
	BACNETSECURITY_INI_CONTROL_MISSING = -26,
	BACNETSECURITY_COULDNOT_GET_DISKSERNUM = -31,
	BACNETSECURITY_DISKSERNUM_FAULT = -32
}BACnetSecurityReturnCode;

typedef enum BACnetDaysOfWeekType
{
	BACnetDaysOfWeekTypeNone,
	BACnetDaysOfWeekTypeMonday = 1,
	BACnetDaysOfWeekTypeTuesday = 2,
	BACnetDaysOfWeekTypeWednesday = 4,
	BACnetDaysOfWeekTypeThursday = 8,
	BACnetDaysOfWeekTypeFriday = 16,
	BACnetDaysOfWeekTypeSaturday = 32,
	BACnetDaysOfWeekTypeSunday = 64
}BACnetDaysOfWeekType;

typedef enum BACnetTransitionBitsType
{
	BACnetTransitionBitsTypeNone,
	BACnetTransitionBitsTypeOffNormal = 1,
	BACnetTransitionBitsTypeFault = 2,
	BACnetTransitionBitsTypeNormal = 4
}BACnetTransitionBitsType;

typedef enum BACnetObjectChoice
{
	ObjectChoiceObjectIdentifier,
	ObjectChoiceObjectName
}BACnetObjectChoice;

typedef enum BACnetScaleType
{
	ScaleType_Float					= 0,
	ScaleType_Integer				= 1
}BACnetScaleType;

typedef enum BACnetAccumulatorStatus
{
	AccumulatorStatus_Normal				= 0,
	AccumulatorStatus_Starting				= 1,
	AccumulatorStatus_Recovered				= 2,
	AccumulatorStatus_Abnormal				= 3,
	AccumulatorStatus_Failed				= 4
}BACnetAccumulatorStatus;

typedef enum log_priority
{
	L_NETWORK_PACKET,
	L_NETWORK_MESSAGE,
	L_CLIENT_TSM,
	L_SERVER_TSM,
	L_SERVICE_INDICATION,
	L_PTP_PACKET,
	L_ADDRESS_BINDING,
	L_BACNET_ETHERNET,
	L_BACNET_IP,
	L_BACNET_MSTP,
	L_BACNET_PTP,
	L_BACNET_API,
	L_BACNET_ANEXH,
	L_DATALINK,
	L_DATATYPE_UTILS,
	L_DRIVER,
	L_MEM,
	L_OBJECT,
	L_READ_WRITE,
	L_MAC,
	L_SERIALIZE,
	L_APPLICATION_LAYER,
	L_LUA,
	L_DATABASE
}log_priority;

typedef enum callback_type
{
	callback_log,
	callback_service_indication_iam,
	callback_service_indication_Ihave,
	callback_service_indication_unconfirmed_cov_notification,
	callback_service_indication_unconfirmed_event_notification,
	callback_service_indication_unconfirmed_private_transfer,
	callback_service_indication_unconfirmed_text_message,
	callback_service_indication_time_synchronization,
	callback_service_indication_who_has,
	callback_service_indication_who_is,
	callback_service_indication_utc_time_synchronization,
	callback_service_indication_confirmed_cov_notification,
	callback_service_indication_confirmed_event_notification,
	callback_service_indication_confirmed_private_transfer,
	callback_service_indication_confirmed_text_message,
	callback_service_indication_life_safety_operation,
	callback_service_override_read_property,
	callback_service_override_read_property_conditional,
	callback_service_override_read_property_multiple,
	callback_service_override_subscribe_cov,
	callback_service_override_atomic_read_file,
	callback_service_override_atomic_write_file,
	callback_service_override_add_list_element,
	callback_service_override_remove_list_element,
	callback_service_override_create_object,
	callback_service_override_delete_object,
	callback_service_override_write_property,
	callback_service_override_write_property_multiple,
	callback_service_override_confirmed_private_transfer,
	callback_service_override_device_communication_control,
	callback_service_override_reinitialize_device,
	callback_service_override_read_range,
	callback_service_override_life_safety_operation,
	callback_service_override_subscribe_cov_property,
	callback_service_override_get_event_information,
	callback_service_override_confirmed_cov_notification,
	callback_service_override_confirmed_event_notification,
	callback_service_override_confirmed_text_message,
	callback_service_override_acknowledge_alarm,
	callback_service_override_get_alarm_summary,
	callback_service_override_get_enrolment_summary,
	callback_service_override_iam,
	callback_service_override_Ihave,
	callback_service_override_unconfirmed_cov_notification,
	callback_service_override_unconfirmed_event_notification,
	callback_service_override_unconfirmed_private_transfer,
	callback_service_override_unconfirmed_text_message,
	callback_service_override_time_synchronization,
	callback_service_override_who_has,
	callback_service_override_who_is,
	callback_service_override_utc_time_synchronization,
	callback_read_object_property_override,
	callback_write_object_property_override,
	callback_proprietary_property_init,
	callback_proprietary_all_property_add,
	callback_read_object_proprietary_property,
	callback_write_object_proprietary_property,
	callback_object_process_overide,
	callback_cov_append_active_subscription,
	callback_event_append_recipient_list,
	callback_signal_handler,
	callback_cov_notification,
	callback_library_module_init,
	callback_library_module_run,
	callback_library_module_stop,
	callback_library_module_message,
	callback_object_api_present_value_cov_notification,
	callback_cache_notification,
	callback_object_present_value_read_request_notification,
	callback_insert_object_notification,
	callback_remove_object_notification,
}callback_type;

#endif


