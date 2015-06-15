/*####COPYRIGHTBEGIN####
 -------------------------------------------
 Copyright (C) 2003 Chris Gurtler

 -------------------------------------------
####COPYRIGHTEND####*/

 /* =======================================================================
    bacnet_template_library.h     BACnet Template Library

    The BACnet Template Library is a set of C++ wrapper classes for the BACnet Server API

    The purpose of wrapper classes is to simplify the interface into the BACnet Server API
	by Automating many of function such as memory management.


    Chris Gurtler

*/

#ifndef BACNETTEMPLATELIBRARY_H
#define BACNETTEMPLATELIBRARY_H

#include "bacnapi.h"

#include <string.h>
#include <stdio.h>

class CBACnetEventState
{
public:
	CBACnetEventState(BACnetEventState data) {m_EventState = data;}
	operator const char*()
	{
		switch (m_EventState)
		{
		case BACnetEventStateNormal: return "Normal";
		case BACnetEventStateFault: return "Fault";
		case BACnetEventStateOffNormal: return "Off Normal";
		case BACnetEventStateHighLimit: return "High Limit";
		case BACnetEventStateLowLimit: return "Low Limit";
		default: return "Unknown";
		}
	}

private:
	BACnetEventState m_EventState;
};

class CBACnetObjectType
{
public:
	CBACnetObjectType(BACnetObjectType data)
	{
		m_ObjectType = data;
	}
	operator const char*()
	{
		switch (m_ObjectType)
		{
			case BACnetObjectTypeAnalogInput: return "AnalogInput";
			case BACnetObjectTypeAnalogOutput: return "AnalogOutput";
			case BACnetObjectTypeAnalogValue: return "AnalogValue";
			case BACnetObjectTypeBinaryInput: return "BinaryInput";
			case BACnetObjectTypeBinaryOutput: return "BinaryOutput";
			case BACnetObjectTypeBinaryValue: return "BinaryValue";
			case BACnetObjectTypeCalendar: return "Calendar";
			case BACnetObjectTypeCommand: return "Command";
			case BACnetObjectTypeDevice: return "Device";
			case BACnetObjectTypeEventEnrolment: return "EventEnrolment";
			case BACnetObjectTypeFile: return "File";
			case BACnetObjectTypeGroup: return "Group";
			case BACnetObjectTypeLoop: return "Loop";
			case BACnetObjectTypeMultistateInput: return "MultistateInput";
			case BACnetObjectTypeMultistateOutput: return "MultistateOutput";
			case BACnetObjectTypeNotificationClass: return "NotificationClass";
			case BACnetObjectTypeProgram: return "Program";
			case BACnetObjectTypeSchedule: return "Schedule";
			case BACnetObjectTypeAveraging: return "Averaging";
			case BACnetObjectTypeMultistateValue: return "MultistateValue";
			case BACnetObjectTypeTrendLog: return "TrendLog";
			case BACnetObjectTypeLifeSafetyPoint: return "LifeSafetyPoint";
			case BACnetObjectTypeLifeSafetyZone: return "LifeSafetyZone";
			case BACnetObjectTypeAccumulator: return "Accumulator";
			case BACnetObjectTypePulseConverter: return "PulseConverter";
			default:
				if (m_ObjectType < 128)
					return "Unknown";
				else
					return "Proprietary";
		}
	}
private:
	BACnetObjectType m_ObjectType;
};

class CBACnetPropertyIdentifier
{
public:
	CBACnetPropertyIdentifier(BACnetPropertyIdentifier data)
	{
		m_Property = data;
	}
	operator const char*()
	{
		switch (m_Property)
		{
			case Property_acceptedModes: return "acceptedModes";
			case Property_ackedTransitions: return "ackedTransitions";
			case Property_ackRequired: return "ackRequired";
			case Property_action: return "action";
			case Property_actionText: return "actionText";
			case Property_activeText: return "activeText";
			case Property_activeVtSessions: return "activeVtSessions";
			case Property_activeCOVSubscriptions: return "activeCOVSubscriptions";
			case Property_adjustValue: return "adjustValue";
			case Property_alarmValue: return "alarmValue";
			case Property_alarmValues: return "alarmValues";
			case Property_all: return "all";
			case Property_allWritesSuccessful: return "allWritesSuccessful";
			case Property_apduSegmentTimeout: return "apduSegmentTimeout";
			case Property_apduTimeout: return "apduTimeout";
			case Property_applicationSoftwareVersion: return "applicationSoftwareVersion";
			case Property_archive: return "archive";
			case Property_attemptedSamples: return "attemptedSamples";
			case Property_averageValue: return "averageValue";
			case Property_backupFailureTimeout: return "backupFailureTimeout";
			case Property_bias: return "bias";
			case Property_bufferSize: return "bufferSize";
			case Property_changeOfStateCount: return "changeOfStateCount";
			case Property_changeOfStateTime: return "changeOfStateTime";
			case Property_notificationClass: return "notificationClass";
			case Property_clientCovIncrement: return "clientCovIncrement";
			case Property_configurationFiles: return "configurationFiles";
			case Property_controlledVariableReference: return "controlledVariableReference";
			case Property_controlledVariableUnits: return "controlledVariableUnits";
			case Property_controlledVariableValue: return "controlledVariableValue";
			case Property_count: return "count";
			case Property_countBeforeChange: return "countBeforeChange";
			case Property_countChangeTime: return "countChangeTime";
			case Property_covIncrement: return "covIncrement";
			case Property_covPeriod: return "covPeriod";
			case Property_covResubscriptionInterval: return "covResubscriptionInterval";
			case Property_currentNotifyTime: return "currentNotifyTime";
			case Property_databaseRevision: return "databaseRevision";
			case Property_dateList: return "dateList";
			case Property_daylightSavingsStatus: return "daylightSavingsStatus";
			case Property_deadband: return "deadband";
			case Property_derivativeConstant: return "derivativeConstant";
			case Property_derivativeConstantUnits: return "derivativeConstantUnits";
			case Property_description: return "description";
			case Property_descriptionOfHalt: return "descriptionOfHalt";
			case Property_deviceAddressBinding: return "deviceAddressBinding";
			case Property_deviceType: return "deviceType";
			case Property_directReading: return "directReading";
			case Property_effectivePeriod: return "effectivePeriod";
			case Property_elapsedActiveTime: return "elapsedActiveTime";
			case Property_errorLimit: return "errorLimit";
			case Property_eventEnable: return "eventEnable";
			case Property_eventParameters: return "eventParameters";
			case Property_eventState: return "eventState";
			case Property_eventTimeStamps: return "eventTimeStamps";
			case Property_eventType: return "eventType";
			case Property_exceptionSchedule: return "exceptionSchedule";
			case Property_faultValues: return "faultValues";
			case Property_feedbackValue: return "feedbackValue";
			case Property_fileAccessMethod: return "fileAccessMethod";
			case Property_fileSize: return "fileSize";
			case Property_fileType: return "fileType";
			case Property_firmwareRevision: return "firmwareRevision";
			case Property_highLimit: return "highLimit";
			case Property_inactiveText: return "inactiveText";
			case Property_inProcess: return "inProcess";
			case Property_inputReference: return "inputReference";
			case Property_instanceOf: return "instanceOf";
			case Property_integralConstant: return "integralConstant";
			case Property_integralConstantUnits: return "integralConstantUnits";
			case Property_issueConfirmedNotifications: return "issueConfirmedNotifications";
			case Property_lastNotifyRecord: return "lastNotifyRecord";
			case Property_lastRestoreTime: return "lastRestoreTime";
			case Property_lifeSafetyAlarmValues: return "lifeSafetyAlarmValues";
			case Property_limitEnable: return "limitEnable";
			case Property_limitMonitoringInterval: return "limitMonitoringInterval";
			case Property_listOfGroupMembers: return "listOfGroupMembers";
			case Property_listOfObjectReferences: return "listOfObjectReferences";
			case Property_listOfSessionKeys: return "listOfSessionKeys";
			case Property_localDate: return "localDate";
			case Property_localTime: return "localTime";
			case Property_location: return "location";
			case Property_logBuffer: return "logBuffer";
			case Property_logDeviceObjectProperty: return "logDeviceObjectProperty";
			case Property_logEnable: return "logEnable";
			case Property_logInterval: return "logInterval";
			case Property_loggingObject: return "loggingObject";
			case Property_loggingRecord: return "loggingRecord";
			case Property_lowLimit: return "lowLimit";
			case Property_maintenanceRequired: return "maintenanceRequired";
			case Property_manipulatedVariableReference: return "manipulatedVariableReference";
			case Property_maximumOutput: return "maximumOutput";
			case Property_maximumValue: return "maximumValue";
			case Property_maximumValueTimestamp: return "maximumValueTimestamp";
			case Property_maxAPDUlengthAccepted: return "maxAPDUlengthAccepted";
			case Property_maxInfoFrames: return "maxInfoFrames";
			case Property_maxMaster: return "maxMaster";
			case Property_maxPresValue: return "maxPresValue";
			case Property_maxSegmentsAccepted: return "maxSegmentsAccepted";
			case Property_memberOf: return "memberOf";
			case Property_minimumOffTime: return "minimumOffTime";
			case Property_minimumOnTime: return "minimumOnTime";
			case Property_minimumOutput: return "minimumOutput";
			case Property_minimumValue: return "minimumValue";
			case Property_minimumValueTimestamp: return "minimumValueTimestamp";
			case Property_minPresValue: return "minPressValue";
			case Property_mode: return "mode";
			case Property_modelName: return "modelName";
			case Property_modificationDate: return "modificationDate";
			case Property_notificationThreshold: return "notificationThreshold";
			case Property_notifyType: return "notifyType";
			case Property_numberOfAPDUretries: return "numberOfAPDUretries";
			case Property_numberOfStates: return "numberOfStates";
			case Property_objectIdentifier: return "objectIdentifier";
			case Property_objectList: return "objectList";
			case Property_objectName: return "objectName";
			case Property_objectPropertyReference: return "objectPropertyReference";
			case Property_objectType: return "objectType";
			case Property_operationExpected: return "operationExpected";
			case Property_optional: return "optional";
			case Property_outOfService: return "outOfService";
			case Property_outputUnits: return "outputUnits";
			case Property_polarity: return "polarity";
			case Property_prescale: return "prescale";
			case Property_presentValue: return "presentValue";
			case Property_previousNotifyTime: return "previousNotifyTime";
			case Property_protocolRevision: return "protocolRevision";
			case Property_priority: return "priority";
			case Property_priorityArray: return "priorityArray";
			case Property_priorityForWriting: return "priorityForWriting";
			case Property_processIdentifier: return "processIdentifier";
			case Property_profileName: return "profileName";
			case Property_programChange: return "programChange";
			case Property_programLocation: return "programLocation";
			case Property_programState: return "programState";
			case Property_proportionalConstant: return "proportionalConstant";
			case Property_proportionalConstantUnits: return "proportionalConstantUnits";
			case Property_protocolConformanceClass: return "protocolConformanceClass";
			case Property_protocolObjectTypesSupported: return "protocolObjectTypesSupported";
			case Property_protocolServicesSupported: return "protocolServicesSupported";
			case Property_protocolVersion: return "protocolVersion";
			case Property_pulseRate: return "pulseRate";
			case Property_readOnly: return "readOnly";
			case Property_reasonForHalt: return "reasonForHalt";
			case Property_recipient: return "recipient";
			case Property_recipientList: return "recipientList";
			case Property_recordsSinceNotification: return "recordsSinceNotification";
			case Property_recordCount: return "recordCount";
			case Property_reliability: return "reliability";
			case Property_relinquishDefault: return "relinquishDefault";
			case Property_required: return "required";
			case Property_resolution: return "resolution";
			case Property_scale: return "scale";
			case Property_scaleFactor: return "scaleFactor";
			case Property_scheduleDefault: return "scheduleDefault";
			case Property_segmentationSupported: return "segmentationSupported";
			case Property_setpoint: return "setpoint";
			case Property_setpointReference: return "setpointReference";
			case Property_setting: return "setting";
			case Property_silenced: return "silenced";
			case Property_startTime: return "startTime";
			case Property_stateText: return "stateText";
			case Property_statusFlags: return "statusFlags";
			case Property_stopTime: return "stopTime";
			case Property_stopWhenFull: return "stopWhenFull";
			case Property_systemStatus: return "systemStatus";
			case Property_timeDelay: return "timeDelay";
			case Property_timeOfActiveTimeReset: return "timeOfActiveTimeReset";
			case Property_timeOfStateCountReset: return "timeOfStateCountReset";
			case Property_timeSynchronizationRecipients: return "timeSynchronizationRecipients";
			case Property_totalRecordCount: return "totalRecordCount";
			case Property_trackingValue: return "trackingValue";
			case Property_units: return "units";
			case Property_updateInterval: return "updateInterval";
			case Property_updateTime: return "updateTime";
			case Property_utcOffset: return "utcOffset";
			case Property_validSamples: return "validSamples";
			case Property_valueBeforeChange: return "valueBeforeChange";
			case Property_valueSet: return "valueSet";
			case Property_valueChangeTime: return "valueChangeTime";
			case Property_varianceValue: return "varianceValue";
			case Property_vendorIdentifier: return "vendorIdentifier";
			case Property_vendorName: return "vendorName";
			case Property_vtClassesSupported: return "vtClassesSupported";
			case Property_weeklySchedule: return "weeklySchedule";
			case Property_windowInterval: return "windowInterval";
			case Property_windowSamples: return "windowSamples";
			case Property_zoneMembers: return "zoneMembers";
			default: return "Unknown";
		}
	}

private:
	BACnetPropertyIdentifier m_Property;
};

class CBACnetError
{
public:
	CBACnetError(BACnetErrorCode Code, BACnetErrorClass Class)
	{
		m_ErrorCode = Code;
		m_ErrorClass = Class;
	}
	const char* GetErrorCode()
	{
		switch (m_ErrorCode)
		{
		case BACnetErrorCodeNone: return "None";
		case BACnetErrorCodeOther: return "Other";
		case BACnetErrorCodeAuthenticationFailed: return "AuthenticationFailed";
		case BACnetErrorCodeConfigurationInProgress: return "ConfigurationInProgress";
		case BACnetErrorCodeDeviceBusy: return "DeviceBusy";
		case BACnetErrorCodeDynamicCreationNotSupported: return "DynamicCreationNotSupported";
		case BACnetErrorCodeFileAccessDenied: return "FileAccessDenied";
		case BACnetErrorCodeIncompatiblesecurityLevels: return "IncompatiblesecurityLevels";
		case BACnetErrorCodeInconsistentParameters: return "InconsistentParameters";
		case BACnetErrorCodeInconsistentSelectionCriteria: return "InconsistentSelectionCriteria";
		case BACnetErrorCodeInvalidDataType: return "InvalidDataType";
		case BACnetErrorCodeInvalidFileAccessMethod: return "InvalidFileAccessMethod";
		case BACnetErrorCodeInvalidFileStartPosition: return "InvalidFileStartPosition";
		case BACnetErrorCodeInvalidOperatorName: return "InvalidOperatorName";
		case BACnetErrorCodeInvalidParameterDataType: return "InvalidParameterDataType";
		case BACnetErrorCodeInvalidTimeStamp: return "InvalidTimeStamp";
		case BACnetErrorCodeKeyGenerationError: return "KeyGenerationError";
		case BACnetErrorCodeMissingRequiredParameter: return "MissingRequiredParameter";
		case BACnetErrorCodeNoObjectsOfSpecifiedType: return "NoObjectsOfSpecifiedType";
		case BACnetErrorCodeNoSpaceForObject: return "NoSpaceForObject";
		case BACnetErrorCodeNoSpaceToAddListElement: return "NoSpaceToAddListElement";
		case BACnetErrorCodeNoSpaceToWriteProperty: return "NoSpaceToWriteProperty";
		case BACnetErrorCodeNoVTSessionsAvailable: return "NoVTSessionsAvailable";
		case BACnetErrorCodePropertyIsNotAList: return "PropertyIsNotAList";
		case BACnetErrorCodeObjectDeletionNotPermitted: return "ObjectDeletionNotPermitted";
		case BACnetErrorCodeObjectIdentifierAlreadyExists: return "ObjectIdentifierAlreadyExists";
		case BACnetErrorCodeOperationalProblem: return "OperationalProblem";
		case BACnetErrorCodePasswordFailure: return "PasswordFailure";
		case BACnetErrorCodeReadAccessDenied: return "ReadAccessDenied";
		case BACnetErrorCodeSecurityNotSupported: return "SecurityNotSupported";
		case BACnetErrorCodeServiceRequestDenied: return "ServiceRequestDenied";
		case BACnetErrorCodeTimeout: return "Timeout";
		case BACnetErrorCodeUnknownObject: return "UnknownObject";
		case BACnetErrorCodeUnknownProperty: return "UnknownProperty";
		case BACnetErrorCodeUnknownVTClass: return "UnknownVTClass";
		case BACnetErrorCodeUnknownVTSession: return "UnknownVTSession";
		case BACnetErrorCodeUnsupportedObjectType: return "UnsupportedObjectType";
		case BACnetErrorCodeValueOutOfRange: return "ValueOutOfRange";
		case BACnetErrorCodeVTSessionAlreadyClosed: return "VTSessionAlreadyClosed";
		case BACnetErrorCodeVTSessionTerminationFailure: return "VTSessionTerminationFailure";
		case BACnetErrorCodeWriteAccessDenied: return "WriteAccessDenied";
		case BACnetErrorCodeCharacterSetNotSupported: return "CharacterSetNotSupported";
		case BACnetErrorCodeInvalidAarrayIndex: return "InvalidAarrayIndex";
		case BACnetErrorCodeCOVSubscriptionFailed: return "COVSubscriptionFailed";
		case BACnetErrorCodeNotCOVProperty: return "NotCOVProperty";
		default: return "Unknown";
		}
	}
	const char* GetErrorClass()
	{
		switch (m_ErrorClass)
		{
		case BACnetErrorClassNone: return "None";
		case BACnetErrorClassDevice: return "Device";
		case BACnetErrorClassObject: return "Object";
		case BACnetErrorClassProperty: return "Property";
		case BACnetErrorClassResources: return "Resources";
		case BACnetErrorClassSecurity: return "Security";
		case BACnetErrorClassServices: return "Services";
		case BACnetVt: return "BACnetVt";
		case BACnetErrorClassNoResponse: return "NoResponse";
		default: return "Unknown";
		}
	}
private:
	BACnetErrorCode m_ErrorCode;
	BACnetErrorClass m_ErrorClass;
};

class CBACnetAnyBase
{
public:
	CBACnetAnyBase()
	{
		m_Attached = 0;
		m_Wrapper = NULL;
		temp_buf = NULL;
	}
	virtual ~CBACnetAnyBase()
	{
		if (temp_buf)
			bacnapi_string_destroy(temp_buf);
		temp_buf = NULL;
		if (!m_Attached && m_Wrapper)
		{
			bacnapi_ClearBACnetDataType(m_Wrapper);
			delete m_Wrapper;
			m_Wrapper = NULL;
		}
	}

	//Operators
#ifdef WTL
	operator CString()
	{
		CString result;
		char *buf = NULL;

		//Use the bacnapi_string_get function to convert the BACnetDataType structure stored in value into a string
		bacnapi_string_get(&buf, m_Wrapper);
		result = buf;
		//Clear the memory in buf using the bacnapi_string_destroy function
		bacnapi_string_destroy(buf);

		return result;
	}
#endif
	operator const char*()
	{
		//Use the bacnapi_string_get function to convert the BACnetDataType structure stored in value into a string
		if (temp_buf)
			bacnapi_string_destroy(temp_buf);
		temp_buf = NULL;
		bacnapi_string_get(&temp_buf, m_Wrapper);
		return temp_buf;
	}
	operator BACnetDataType*(){return m_Wrapper;}
	operator BACnetAddressBinding*()
	{
		ChangeType(bacnetDT_AddressBinding);
		return (BACnetAddressBinding*)m_Wrapper->_type.addressBinding;
	}

	int Clear()
	{
		init();
		bacnapi_ClearBACnetDataType(m_Wrapper);
		return 0;
	}
	int ChangeType(BACnetApplicationTagNumber type, const BACnetDataType *pSrc = NULL)
	{
		init();
		if (m_Wrapper->type != type)
		{
			Clear();
			if (bacnapi_initialize_data(m_Wrapper, type))
				return -1;
		}
		return 0;
	}
	int Copy(BACnetDataType *pSrc)
	{
		init();
		if (bacnapi_access_BACnetData(m_Wrapper, pSrc, -1, 1))
			return -1;
		return 0;
	}
	int Attach(BACnetDataType *pSrc)
	{
		m_Attached = 1;
		m_Wrapper = pSrc;
		return 0;
	}
	int Detach(BACnetDataType *pSrc)
	{
		m_Attached = 0;
		pSrc = m_Wrapper;
		m_Wrapper = NULL;
		return 0;
	}

	BACnetApplicationTagNumber GetType(){init(); return m_Wrapper->type;}

protected:
	char* temp_buf;
	BACnetDataType* m_Wrapper;
	unsigned char m_Attached;
	void init()
	{
		if (m_Wrapper)
			return;
		m_Wrapper = new BACnetDataType;
		memset(m_Wrapper, 0, sizeof(m_Wrapper));
	}
};

class CBACnetArray
{
public:
	CBACnetArray()
	{
		m_Array = NULL;
	}
	virtual ~CBACnetArray(){}

protected:
	int Initialize(BACnetApplicationTagNumber type)
	{
		if (m_Array == NULL)
			return -1;

		if (m_Array->type == bacnetDT_Null)
		{
			if (bacnapi_initialize_data(m_Array, type))
				return -1;

			if (m_Array->type != type)
				return -1;
		}
		return 0;
	}
	BACnetDataType* AddElement(BACnetApplicationTagNumber list_type, BACnetApplicationTagNumber element_type)
	{
		BACnetDataType empty_element;
		BACnetDataType* new_element;

		Initialize(list_type);

		memset(&empty_element, 0, sizeof(empty_element));
		new_element = bacnapi_set_element_in_bacnet_array((BACnetArray*)m_Array->_type.array, &empty_element);
		if (new_element == NULL)
			return NULL;

		bacnapi_initialize_data(new_element, element_type);
		if (new_element->type != element_type)
			return NULL;

		return new_element;
	}
	BACnetDataType* GetElement(BACnetApplicationTagNumber list_type, int index)
	{
		BACnetDataType* element;
		Initialize(list_type);

		element = bacnapi_get_element_from_bacnet_array((BACnetArray*)m_Array->_type.array, index);
		if (element == NULL)
			return NULL;

		return element;
	}
	int GetArraySize(BACnetApplicationTagNumber list_type)
	{
		Initialize(list_type);

		return ((BACnetArray*)m_Array->_type.array)->ubound;
	}

	BACnetDataType *m_Array;
};

typedef enum BACnetPVNull
{
    BACnetNull
}BACnetPVNull;

class CBACnetBitString
{
public:
	CBACnetBitString()
	{
		m_Array = NULL;
	}
	CBACnetBitString(BACnetBitString * pSrc)
	{
		m_Array = pSrc;
	}
	virtual ~CBACnetBitString(){}

	BYTE SetElement(BYTE data, int index)
	{
		return bacnapi_set_bit(m_Array, data, index) == 0 ? 0xFF : 0x0;
	}
	BYTE* GetElement(int index)
	{
		if (m_Array == NULL || m_Array->contents == NULL || GetArraySize() <= index)
			return NULL;

		return m_Array->contents + index;
	}
	int GetArraySize()
	{
		return m_Array->length;
	}
	BYTE GetStatus(int index)
	{
		BYTE* tmp = GetElement(index);
		if (tmp && *tmp)
			return 0xFF;
		else
			return 0;
	}

	BYTE Init(int size)
	{
		if (GetArraySize() >= size)
			return 0xFF;

		for (int i=m_Array->length; i<size; i++)
		{
			if (bacnapi_set_bit(m_Array, 0, i))
				return 0x0;
		}
		return 0xFF;
	}
protected:
	BACnetBitString *m_Array;
};

class CBACnetStatusFlags : CBACnetBitString
{
    //In Alarm
    //Fault
    //Overridden
    //Out Of Service
public:
	CBACnetStatusFlags()
	{
		m_Array = NULL;
	}
	CBACnetStatusFlags(BACnetBitString * pSrc)
	{
		m_Array = pSrc;
	}
	BYTE GetInAlarm()
	{
		BYTE *element;
		if (m_Array == NULL || m_Array->contents == NULL || GetArraySize() != 4)
			return 0;
		element = GetElement(0);
		return *element;
	}
	BYTE GetFault()
	{
		BYTE *element;
		if (m_Array == NULL || m_Array->contents == NULL || GetArraySize() != 4)
			return 0;
		element = GetElement(1);
		return *element;
	}
	BYTE GetOverridden()
	{
		BYTE *element;
		if (m_Array == NULL || m_Array->contents == NULL || GetArraySize() != 4)
			return 0;
		element = GetElement(2);
		return *element;
	}
	BYTE GetOutOfService()
	{
		BYTE *element;
		if (m_Array == NULL || m_Array->contents == NULL || GetArraySize() != 4)
			return 0;
		element = GetElement(3);
		return *element;
	}
	BYTE SetInAlarm(BYTE data)
	{
		if (m_Array == NULL)
			return 0;
		if (!Init(4))
			return 0x0;
		return SetElement(data, 0);
	}
	BYTE SetFault(BYTE data)
	{
		if (m_Array == NULL)
			return 0;
		if (!Init(4))
			return 0x0;
		return SetElement(data, 1);
	}
	BYTE SetOverridden(BYTE data)
	{
		if (m_Array == NULL)
			return 0;
		if (!Init(4))
			return 0x0;
		return SetElement(data, 2);
	}
	BYTE SetOutOfService(BYTE data)
	{
		if (m_Array == NULL)
			return 0;
		if (!Init(4))
			return 0x0;
		return SetElement(data, 3);
	}
};

class CBACnetLimitEnable : CBACnetBitString
{
    //Low Limit Enable
    //High Limit Enable
public:
	CBACnetLimitEnable()
	{
		m_Array = NULL;
	}
	CBACnetLimitEnable(BACnetBitString * pSrc)
	{
		m_Array = pSrc;
	}
	BYTE GetLowLimit()
	{
		BYTE *element;
		if (m_Array == NULL || m_Array->contents == NULL || GetArraySize() != 2)
			return 0;
		element = GetElement(0);
		return *element;
	}
	BYTE GetHighLimit()
	{
		BYTE *element;
		if (m_Array == NULL || m_Array->contents == NULL || GetArraySize() != 2)
			return 0;
		element = GetElement(1);
		return *element;
	}

	BYTE SetLowLimit(BYTE data)
	{
		if (m_Array == NULL)
			return 0;
		if (!Init(2))
			return 0x0;
		return SetElement(data, 0);
	}
	BYTE SetHighLimit(BYTE data)
	{
		if (m_Array == NULL)
			return 0;
		if (!Init(2))
			return 0x0;
		return SetElement(data, 1);
	}
};

class CBACnetTransitionBits : CBACnetBitString
{
    //to offnormal
    //to BACnetEventStateFault
	//to BACnetEventStateNormal
public:
	CBACnetTransitionBits()
	{
		m_Array = NULL;
	}
	CBACnetTransitionBits(BACnetBitString * pSrc)
	{
		m_Array = pSrc;
	}
	operator BACnetBitString*(){return m_Array;}
	BYTE GetToOffNormal()
	{
		BYTE *element;
		if (m_Array == NULL || m_Array->contents == NULL || GetArraySize() != 3)
			return 0;
		element = GetElement(0);
		return *element;
	}
	BYTE GetToFault()
	{
		BYTE *element;
		if (m_Array == NULL || m_Array->contents == NULL || GetArraySize() != 3)
			return 0;
		element = GetElement(1);
		return *element;
	}
	BYTE GetToNormal()
	{
		BYTE *element;
		if (m_Array == NULL || m_Array->contents == NULL || GetArraySize() != 3)
			return 0;
		element = GetElement(2);
		return *element;
	}
	BYTE SetToOffNormal(BYTE data)
	{
		if (m_Array == NULL)
			return 0;
		if (!Init(3))
			return 0x0;
		return SetElement(data, 0);
	}
	BYTE SetToFault(BYTE data)
	{
		if (m_Array == NULL)
			return 0;
		if (!Init(3))
			return 0x0;
		return SetElement(data, 1);
	}
	BYTE SetToNormal(BYTE data)
	{
		if (m_Array == NULL)
			return 0;
		if (!Init(3))
			return 0x0;
		return SetElement(data, 2);
	}
private:

};

class CBACnetDaysOfWeek : CBACnetBitString
{
public:
	CBACnetDaysOfWeek()
	{
		m_Array = NULL;
	}
	CBACnetDaysOfWeek(BACnetBitString * pSrc)
	{
		m_Array = pSrc;
	}
	BYTE GetMonday()
	{
		BYTE *element;
		if (m_Array == NULL || m_Array->contents == NULL || GetArraySize() != 7)
			return 0;
		element = GetElement(0);
		return *element;
	}
	BYTE GetTuesday()
	{
		BYTE *element;
		if (m_Array == NULL || m_Array->contents == NULL || GetArraySize() != 7)
			return 0;
		element = GetElement(1);
		return *element;
	}
	BYTE GetWednesday()
	{
		BYTE *element;
		if (m_Array == NULL || m_Array->contents == NULL || GetArraySize() != 7)
			return 0;
		element = GetElement(2);
		return *element;
	}
	BYTE GetThursday()
	{
		BYTE *element;
		if (m_Array == NULL || m_Array->contents == NULL || GetArraySize() != 7)
			return 0;
		element = GetElement(3);
		return *element;
	}
	BYTE GetFriday()
	{
		BYTE *element;
		if (m_Array == NULL || m_Array->contents == NULL || GetArraySize() != 7)
			return 0;
		element = GetElement(4);
		return *element;
	}
	BYTE GetSaturday()
	{
		BYTE *element;
		if (m_Array == NULL || m_Array->contents == NULL || GetArraySize() != 7)
			return 0;
		element = GetElement(5);
		return *element;
	}
	BYTE GetSunday()
	{
		BYTE *element;
		if (m_Array == NULL || m_Array->contents == NULL || GetArraySize() != 7)
			return 0;
		element = GetElement(6);
		return *element;
	}

	BYTE SetMonday(BYTE data)
	{
		if (m_Array == NULL)
			return 0;
		if (!Init(7))
			return 0x0;
		return SetElement(data, 0);
	}
	BYTE SetTuesday(BYTE data)
	{
		if (m_Array == NULL)
			return 0;
		if (!Init(7))
			return 0x0;
		return SetElement(data, 1);
	}
	BYTE SetWednesday(BYTE data)
	{
		if (m_Array == NULL)
			return 0;
		if (!Init(7))
			return 0x0;
		return SetElement(data, 2);
	}
	BYTE SetThursday(BYTE data)
	{
		if (m_Array == NULL)
			return 0;
		if (!Init(7))
			return 0x0;
		return SetElement(data, 3);
	}
	BYTE SetFriday(BYTE data)
	{
		if (m_Array == NULL)
			return 0;
		if (!Init(7))
			return 0x0;
		return SetElement(data, 4);
	}
	BYTE SetSaturday(BYTE data)
	{
		if (m_Array == NULL)
			return 0;
		if (!Init(7))
			return 0x0;
		return SetElement(data, 5);
	}
	BYTE SetSunday(BYTE data)
	{
		if (m_Array == NULL)
			return 0;
		if (!Init(7))
			return 0x0;
		return SetElement(data, 6);
	}

private:

};

class CBACnetPropertyStates
{
public:
	CBACnetPropertyStates(){m_Value = NULL;}
	CBACnetPropertyStates(BACnetPropertyStates *pSrc)
	{
		m_Value = pSrc;
	}
	//Assignment Operators
	CBACnetPropertyStates& operator =(BYTE src){m_Value->type = PropertyStateBoolean; m_Value->_type.booleanValue = src; return *this;}
	CBACnetPropertyStates& operator =(BACnetBinaryPV src){m_Value->type = PropertyStateBACnetBinaryPV; m_Value->_type.binaryValue = src; return *this;}
	CBACnetPropertyStates& operator =(BACnetEventType src){m_Value->type = PropertyStateBACnetEventType; m_Value->_type.eventType = src; return *this;}
	CBACnetPropertyStates& operator =(BACnetPolarity src){m_Value->type = PropertyStateBACnetPolarity; m_Value->_type.polarity = src; return *this;}
	CBACnetPropertyStates& operator =(BACnetProgramRequest src){m_Value->type = PropertyStateBACnetProgramChange; m_Value->_type.programChange = src; return *this;}
	CBACnetPropertyStates& operator =(BACnetProgramState src){m_Value->type = PropertyStateBACnetProgramState; m_Value->_type.programState = src; return *this;}
	CBACnetPropertyStates& operator =(BACnetProgramError src){m_Value->type = PropertyStateBACnetReasonForHalt; m_Value->_type.reasonForHalt = src;	return *this;}
	CBACnetPropertyStates& operator =(BACnetReliability src){m_Value->type = PropertyStateBACnetReliability; m_Value->_type.reliability = src; return *this;}
	CBACnetPropertyStates& operator =(BACnetDeviceStatus src){m_Value->type = PropertyStateBACnetSystemStatus; m_Value->_type.systemStatus = src; return *this;}
//	CBACnetPropertyStates& operator =(BACnetEngineeringUnits src){m_Value->type = PropertyStateBACnetUnits;	m_Value->_type.units = src;	return *this;}
	CBACnetPropertyStates& operator =(UINT src){m_Value->type = PropertyStateUnsigned; m_Value->_type.unsignedValue = src; return *this;}

	//Extraction Operators
	operator BYTE(){m_Value->type = PropertyStateBoolean; return m_Value->_type.booleanValue;}
	operator BACnetBinaryPV(){m_Value->type = PropertyStateBACnetBinaryPV; return m_Value->_type.binaryValue;}
	operator BACnetEventType(){m_Value->type = PropertyStateBACnetEventType; return m_Value->_type.eventType;}
	operator BACnetPolarity(){m_Value->type = PropertyStateBACnetPolarity; return m_Value->_type.polarity;}
	operator BACnetProgramRequest(){m_Value->type = PropertyStateBACnetProgramChange; return m_Value->_type.programChange;}
	operator BACnetProgramState(){m_Value->type = PropertyStateBACnetProgramState; return m_Value->_type.programState;}
	operator BACnetProgramError(){m_Value->type = PropertyStateBACnetReasonForHalt;	return m_Value->_type.reasonForHalt;}
	operator BACnetReliability(){m_Value->type = PropertyStateBACnetReliability; return m_Value->_type.reliability;}
	operator BACnetEventState(){m_Value->type = PropertyStateBACnetState; return m_Value->_type.state;}
	operator BACnetDeviceStatus(){m_Value->type = PropertyStateBACnetSystemStatus; return m_Value->_type.systemStatus;}
//	operator BACnetEngineeringUnits(){m_Value->type = PropertyStateBACnetUnits; return m_Value->_type.units;}
	operator UINT(){m_Value->type = PropertyStateUnsigned; return m_Value->_type.unsignedValue;}

	BACnetPropertyStatesChoice GetType() {return m_Value->type;}
private:
	BACnetPropertyStates* m_Value;
};

class CSequenceOfBACnetPropertyStates : CBACnetArray
{
public:
	CSequenceOfBACnetPropertyStates(){}
	CSequenceOfBACnetPropertyStates(BACnetDataType *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfBACnetPropertyStates(){}
	//List access
	CBACnetPropertyStates Add(BACnetBinaryPV state)
	{
		BACnetPropertyStates *states = NULL;
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfBACnetPropertyStates, bacnetDT_PropertyStates);
		if (new_element == NULL)
			return CBACnetPropertyStates();

		states = (BACnetPropertyStates*)new_element->_type.propertyStates;
		if (states == NULL)
			return CBACnetPropertyStates();

		states->type = PropertyStateBACnetBinaryPV;
		states->_type.binaryValue = state;

		return CBACnetPropertyStates(states);
	}
	CBACnetPropertyStates GetItem(int index)
	{
		BACnetDataType* element;
		BACnetPropertyStates *states = NULL;

		element = GetElement(bacnetDT_ArrayOfBACnetPropertyStates, index);
		if (element)
			states = (BACnetPropertyStates*)element->_type.propertyStates;

		return CBACnetPropertyStates(states);
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_ArrayOfBACnetPropertyStates);
	}
};

class CBACnetObjectPropertyReference
{
public:
	CBACnetObjectPropertyReference(){}
	CBACnetObjectPropertyReference(BACnetObjectPropertyReference *pSrc)
	{
		m_Value = pSrc;
	}
	void Copy(BACnetObjectPropertyReference *pSrc)
	{
		if (m_Value)
			*m_Value = *pSrc;
	}

	//Set Properties
	void Set(BACnetObjectType ObjectType, int Instance, BACnetPropertyIdentifier Property, int Index = -1)
	{
		if (m_Value)
		{
			m_Value->objectIdentifier.objectType = ObjectType;
			m_Value->objectIdentifier.instance = Instance;
			m_Value->property = Property;
			m_Value->propertyArrayIndex = Index;
		}
	}
	void SetPropertyIdentifier(BACnetPropertyIdentifier data)
	{
		if (m_Value)
			m_Value->property = data;
	}
	void SetPropertyArrayIndex(int data)
	{
		if (m_Value)
			m_Value->propertyArrayIndex = data;
	}
	void SetObjectIdentifierObjectType(BACnetObjectType data)
	{
		if (m_Value)
			m_Value->objectIdentifier.objectType = data;
	}
	void SetObjectIdentifierInstance(int data)
	{
		if (m_Value)
			m_Value->objectIdentifier.instance = data;
	}

	//Get Properties
	BACnetPropertyIdentifier GetPropertyIdentifier()
	{
		if (m_Value == NULL)
			return (BACnetPropertyIdentifier)0;
		return m_Value->property;
	}
	int GetPropertyArrayIndex()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->propertyArrayIndex;
	}
	BACnetObjectType GetObjectIdentifierObjectType()
	{
		if (m_Value == NULL)
			return BACnetObjectTypeNone;
		return m_Value->objectIdentifier.objectType;
	}
	int GetObjectIdentifierObjectInstance()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->objectIdentifier.instance;
	}

protected:
	BACnetObjectPropertyReference* m_Value;
};

class CBACnetSetpointReference : CBACnetObjectPropertyReference
{
public:
	CBACnetSetpointReference(){}
	CBACnetSetpointReference(BACnetSetpointReference *pSrc)
	{
		m_Ref = pSrc;
		m_Value = &pSrc->objectPropertyReference;
	}
	void Copy(BACnetSetpointReference *pSrc)
	{
		if (m_Ref)
			*m_Ref = *pSrc;
	}

private:
	BACnetSetpointReference* m_Ref;
};


class CBACnetDeviceObjectPropertyReference
{
public:
	CBACnetDeviceObjectPropertyReference(){}
	CBACnetDeviceObjectPropertyReference(BACnetDeviceObjectPropertyReference *pSrc)
	{
		m_Value = pSrc;
	}
	//Set Properties
	void Set(int DeviceID, BACnetObjectType ObjectType, int Instance, BACnetPropertyIdentifier Property, int Index = -1)
	{
		if (m_Value)
		{
			m_Value->deviceIdentifier.objectType = BACnetObjectTypeDevice;
			m_Value->deviceIdentifier.instance = DeviceID;
			m_Value->objectIdentifier.objectType = ObjectType;
			m_Value->objectIdentifier.instance = Instance;
			m_Value->property = Property;
			m_Value->propertyArrayIndex = Index;
		}
	}
	void SetPropertyIdentifier(BACnetPropertyIdentifier data)
	{
		if (m_Value)
			m_Value->property = data;
	}
	void SetPropertyArrayIndex(int data)
	{
		if (m_Value)
			m_Value->propertyArrayIndex = data;
	}
	void SetObjectIdentifierObjectType(BACnetObjectType data)
	{
		if (m_Value)
			m_Value->objectIdentifier.objectType = data;
	}
	void SetObjectIdentifierInstance(int data)
	{
		if (m_Value)
			m_Value->objectIdentifier.instance = data;
	}
	void SetDeviceIdentifierObjectType(BACnetObjectType data)
	{
		if (m_Value)
			m_Value->objectIdentifier.objectType = data;
	}
	void SetDeviceIdentifierInstance(int data)
	{
		if (m_Value)
			m_Value->objectIdentifier.instance = data;
	}

	//Get Properties
	BACnetPropertyIdentifier GetPropertyIdentifier()
	{
		if (m_Value == NULL)
			return (BACnetPropertyIdentifier)0;
		return m_Value->property;
	}
	int GetPropertyArrayIndex()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->propertyArrayIndex;
	}
	BACnetObjectType GetObjectIdentifierObjectType()
	{
		if (m_Value == NULL)
			return BACnetObjectTypeNone;
		return m_Value->objectIdentifier.objectType;
	}
	int GetObjectIdentifierObjectInstance()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->objectIdentifier.instance;
	}
	BACnetObjectType GetDeviceIdentifierObjectType()
	{
		if (m_Value == NULL)
			return BACnetObjectTypeNone;
		return m_Value->deviceIdentifier.objectType;
	}
	int GetDeviceIdentifierObjectInstance()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->deviceIdentifier.instance;
	}

private:
	BACnetDeviceObjectPropertyReference* m_Value;
};

class CBACnetActionCommand
{
public:
	CBACnetActionCommand(){}
	CBACnetActionCommand(BACnetActionCommand *pSrc)
	{
		m_Value = pSrc;
	}
	//Set Properties
	void Set(int DeviceID, BACnetObjectType ObjectType, int Instance, BACnetPropertyIdentifier Property, int Index = -1)
	{
		if (m_Value)
		{
			m_Value->deviceIdentifier.objectType = BACnetObjectTypeDevice;
			m_Value->deviceIdentifier.instance = DeviceID;
			m_Value->objectIdentifier.objectType = ObjectType;
			m_Value->objectIdentifier.instance = Instance;
			m_Value->propertyIdentifier = Property;
			m_Value->propertyArrayIndex = Index;
		}
	}
	void SetPropertyIdentifier(BACnetPropertyIdentifier data)
	{
		if (m_Value)
			m_Value->propertyIdentifier = data;
	}
	void SetPropertyArrayIndex(int data)
	{
		if (m_Value)
			m_Value->propertyArrayIndex = data;
	}
	void SetObjectIdentifierObjectType(BACnetObjectType data)
	{
		if (m_Value)
			m_Value->objectIdentifier.objectType = data;
	}
	void SetObjectIdentifierInstance(int data)
	{
		if (m_Value)
			m_Value->objectIdentifier.instance = data;
	}
	void SetDeviceIdentifierObjectType(BACnetObjectType data)
	{
		if (m_Value)
			m_Value->objectIdentifier.objectType = data;
	}
	void SetDeviceIdentifierInstance(int data)
	{
		if (m_Value)
			m_Value->objectIdentifier.instance = data;
	}
	void SetPriority(UINT data)
	{
		if (m_Value)
			m_Value->priority = data;
	}
	void SetPostDelay(UINT data)
	{
		if (m_Value)
			m_Value->postDelay = data;
	}
	void SetQuitOnFailure(BYTE data)
	{
		if (m_Value)
			m_Value->quitOnFailure = data;
	}
	void SetWriteSuccessful(BYTE data)
	{
		if (m_Value)
			m_Value->writeSuccessful = data;
	}

	//Get Properties
	BACnetPropertyIdentifier GetPropertyIdentifier()
	{
		if (m_Value == NULL)
			return (BACnetPropertyIdentifier)0;
		return m_Value->propertyIdentifier;
	}
	int GetPropertyArrayIndex()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->propertyArrayIndex;
	}
	BACnetObjectType GetObjectIdentifierObjectType()
	{
		if (m_Value == NULL)
			return BACnetObjectTypeNone;
		return m_Value->objectIdentifier.objectType;
	}
	int GetObjectIdentifierObjectInstance()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->objectIdentifier.instance;
	}
	BACnetObjectType GetDeviceIdentifierObjectType()
	{
		if (m_Value == NULL)
			return BACnetObjectTypeNone;
		return m_Value->deviceIdentifier.objectType;
	}
	int GetDeviceIdentifierObjectInstance()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->deviceIdentifier.instance;
	}
	UINT GetPriority()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->priority;
	}
	UINT GetPostDelay()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->postDelay;
	}
	BYTE GetQuitOnFailure()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->quitOnFailure;
	}
	BYTE GetWriteSuccessful()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->writeSuccessful;
	}
	BACnetDataType* GetValue()
	{
		if (m_Value == NULL)
			return NULL;
		return &m_Value->propertyValue;
	}

private:
	BACnetActionCommand* m_Value;
};

class CBACnetObjectIdentifier
{
public:
	CBACnetObjectIdentifier(){}
	CBACnetObjectIdentifier(BACnetObjectIdentifier *pSrc)
	{
		m_Value = pSrc;
	}
	void Copy(BACnetObjectIdentifier *pSrc)
	{
		if (m_Value)
			*m_Value = *pSrc;
	}
	//Set Properties
	void Set(BACnetObjectType ObjectType, int Instance)
	{
		if (m_Value)
		{
			m_Value->objectType = BACnetObjectTypeDevice;
		}
	}
	void SetObjectType(BACnetObjectType data)
	{
		if (m_Value)
			m_Value->objectType = data;
	}
	void SetInstance(int data)
	{
		if (m_Value)
			m_Value->instance = data;
	}

	//Get Properties
	BACnetObjectType GetObjectType()
	{
		if (m_Value == NULL)
			return BACnetObjectTypeNone;
		return m_Value->objectType;
	}
	int GetInstance()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->instance;
	}

private:
	BACnetObjectIdentifier* m_Value;
};

class CBACnetTimeValue
{
public:
	CBACnetTimeValue(){}
	CBACnetTimeValue(BACnetTimeValue *pSrc)
	{
		m_Value = pSrc;
	}

	//Get Properties
	int GetTime(timeb *value)
	{
		BACnetDateTime dateTime;

		memset(&dateTime, 0, sizeof(dateTime));
		if (m_Value == NULL)
			return -1;
		dateTime.time = m_Value->time;
		bacnetapi_get_timeb(value, &dateTime);
		return 0;
	}
	operator BACnetTimeValue*(){return m_Value;}
	BACnetDataType* GetValue()
	{
		if (m_Value == NULL)
			return NULL;
		return &m_Value->value;
	}
	float GetReal()
	{
		ChangeType(bacnetDT_Real);
		return m_Value->value._type.Real;
	}
	BYTE GetHour(){return m_Value->time.hour;}
	BYTE GetMinute(){return m_Value->time.minute;}
	int GetSignedInteger()
	{
		ChangeType(bacnetDT_SignedInteger);
		return m_Value->value._type.SignedInteger;
	}
	UINT GetUnsignedInteger()
	{
		ChangeType(bacnetDT_UnsignedInteger);
		return m_Value->value._type.UnsignedInteger;
	}
	UINT GetEnumerated()
	{
		ChangeType(bacnetDT_Enumerated);
		return m_Value->value._type.Enumerated;
	}
	BACnetApplicationTagNumber GetType(){return m_Value->value.type;}

	CBACnetTimeValue& operator =(UINT src)
	{
		bacnapi_access_unsignedInteger(&src, &m_Value->value, 0);
		return *this;
	}
	CBACnetTimeValue& operator =(BACnetPVNull src)
	{
		bacnapi_initialize_data(&m_Value->value, bacnetDT_Null);
		return *this;
	}
	CBACnetTimeValue& operator =(BACnetEventType src)
	{
		bacnapi_access_enumerated((UINT*)&src, &m_Value->value, 0);
		return *this;
	}
	CBACnetTimeValue& operator =(BACnetNotifyType src)
	{
		bacnapi_access_enumerated((UINT*)&src, &m_Value->value, 0);
		return *this;
	}
	CBACnetTimeValue& operator =(int src)
	{
		bacnapi_access_signedInteger(&src, &m_Value->value, 0);
		return *this;
	}
	CBACnetTimeValue& operator =(float src)
	{
		bacnapi_access_real(&src, &m_Value->value, 0);
		return *this;
	}
	CBACnetTimeValue& operator =(char* src)
	{
		bacnapi_access_string(&src, &m_Value->value, 0);
		return *this;
	}

	//Set Properties
	int Set(BACnetDataType *Value, BYTE Hour, BYTE Minute, BYTE Second = 0, BYTE Hundreths = 0)
	{
		m_Value->time.hour = Hour;
		m_Value->time.minute = Minute;
		m_Value->time.second = Second;
		m_Value->time.hundrethsOfASec = Hundreths;
		if (bacnapi_access_BACnetData(&m_Value->value, Value, -1, 1))
			return -1;

		return 0;
	}

private:
	BACnetTimeValue* m_Value;
	int ChangeType(BACnetApplicationTagNumber type, const BACnetDataType *pSrc = NULL)
	{
		if (m_Value->value.type != type)
		{
			Clear();
			if (bacnapi_initialize_data(&m_Value->value, type))
				return -1;
		}
		return 0;
	}
	int Clear()
	{
		bacnapi_ClearBACnetDataType(&m_Value->value);
		return 0;
	}
};

class CSequenceOfBACnetTimeValue : CBACnetArray
{
public:
	CSequenceOfBACnetTimeValue(){}
	CSequenceOfBACnetTimeValue(SequenceOfBACnetTimeValue *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfBACnetTimeValue(){}
	//List access
	CBACnetTimeValue Add(BACnetTimeValue *timeValue)
	{
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfBACnetTimeValue, bacnetDT_TimeValue);

		bacnapi_access_timeValue(timeValue, new_element, 0);
		return CBACnetTimeValue(timeValue);
	}
	CBACnetTimeValue Add(BACnetDataType *Value, BYTE Hour, BYTE Minute, BYTE Second = 0, BYTE Hundreths = 0)
	{
		BACnetTimeValue *timeValue = NULL;
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfBACnetTimeValue, bacnetDT_TimeValue);

		timeValue = (BACnetTimeValue*)new_element->_type.timeValue;
		if (timeValue)
		{
			timeValue->time.hour = Hour;
			timeValue->time.minute = Minute;
			timeValue->time.second = Second;
			timeValue->time.hundrethsOfASec = Hundreths;
			if (bacnapi_access_BACnetData(&timeValue->value, Value, -1, 1))
				return CBACnetTimeValue();
		}

		return CBACnetTimeValue(timeValue);
	}
	CBACnetTimeValue GetItem(int index)
	{
		BACnetDataType* element;
		BACnetTimeValue *timeValue = NULL;

		element = GetElement(bacnetDT_ArrayOfBACnetTimeValue, index);
		if (element)
			timeValue = (BACnetTimeValue*)element->_type.timeValue;

		return CBACnetTimeValue(timeValue);
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_ArrayOfBACnetTimeValue);
	}
};

class CSequenceOfCharacterString : CBACnetArray
{
public:
	CSequenceOfCharacterString(){}
	CSequenceOfCharacterString(SequenceOfCharacterString *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfCharacterString(){}
	//List access
	char* Add(char *data)
	{
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfCharacterString, bacnetDT_CharacterString);

		bacnapi_access_string(&data, new_element, 0);
		return data;
	}
	char** GetItem(int index)
	{
		BACnetDataType* element;

		element = GetElement(bacnetDT_ArrayOfCharacterString, index);
		if (element)
			return &element->_type.String;

		return NULL;
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_ArrayOfCharacterString);
	}
};

class CSequenceOfUnsigned : CBACnetArray
{
public:
	CSequenceOfUnsigned(){}
	CSequenceOfUnsigned(SequenceOfCharacterString *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfUnsigned(){}
	//List access
	UINT Add(UINT data)
	{
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfUnsigned, bacnetDT_UnsignedInteger);

		bacnapi_access_unsignedInteger(&data, new_element, 0);
		return data;
	}
	UINT* GetItem(int index)
	{
		BACnetDataType* element;

		element = GetElement(bacnetDT_ArrayOfUnsigned, index);
		if (element)
			return &element->_type.UnsignedInteger;

		return NULL;
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_ArrayOfUnsigned);
	}
};

class CBACnetCalendarEntry
{
public:
	CBACnetCalendarEntry(){}
	CBACnetCalendarEntry(BACnetCalendarEntry *pSrc)
	{
		m_Value = pSrc;
	}
	int SetChoice(BACnetCalendarEntryType data)
	{
		if (!m_Value)
			return -1;
		m_Value->choice = data;
		return 0;
	}
	int SetDate(int Year, int Month, int Day)
	{
		if (m_Value == NULL)
			return -1;
		m_Value->date.year = (BYTE)Year-1900;
		m_Value->date.month = (BYTE)Month;
		m_Value->date.day = (BYTE)Day;
		m_Value->date.dayOfWeek=0;
		m_Value->choice = BACnetCalendarEntryTypeDate;
		return 0;
	}
	int SetDate(const time_t data)
	{
		if (m_Value == NULL)
			return -1;
		BACnetDateTime temp;
		bacnetapi_set_time(&temp, data);
		m_Value->date = temp.date;
		m_Value->choice = BACnetCalendarEntryTypeDate;
		return 0;
	}
	int SetDateRange(const time_t StartDate, const time_t EndDate)
	{
		if (m_Value == NULL)
			return -1;
		BACnetDateTime temp;
		bacnetapi_set_time(&temp, StartDate);
		m_Value->dateRange.startDate = temp.date;
		bacnetapi_set_time(&temp, EndDate);
		m_Value->dateRange.endDate = temp.date;
		m_Value->choice = BACnetCalendarEntryTypeDateRange;
		return 0;
	}
	int SetWeekNDay(BYTE Month, BYTE WeekOfMonth, BYTE DayOfWeek)
	{
		if (m_Value == NULL)
			return -1;
		m_Value->weekNDay.month = Month;
		m_Value->weekNDay.weekOfMonth = WeekOfMonth;
		m_Value->weekNDay.dayOfWeek = DayOfWeek;
		m_Value->choice = BACnetCalendarEntryTypeWeekNDay;
		return 0;
	}
	//Get Properties
	operator BACnetCalendarEntry*(){return m_Value;}
	BACnetCalendarEntryType GetChoice(){return m_Value->choice;}
	BYTE GetWeekNDayMonth(){return m_Value->weekNDay.month;}
	BYTE GetWeekNDayWeekOfMonth(){return m_Value->weekNDay.weekOfMonth;}
	BYTE GetWeekNDayDayOfWeek(){return m_Value->weekNDay.dayOfWeek;}

	BYTE GetDateYear(){return m_Value->date.year;}
	BYTE GetDateMonth(){return m_Value->date.month;}
	BYTE GetDateDay(){return m_Value->date.day;}

	time_t GetFromTime()
	{
		BACnetDateTime temp;
		time_t val = 0;

		memset(&temp, 0, sizeof(temp));

		if (m_Value == NULL && m_Value->choice != BACnetCalendarEntryTypeDate)
			return 0;

		temp.date = m_Value->date;
		bacnetapi_get_time(&val, &temp);
		return val;
	}
	int GetDateRange(time_t* StartDate, time_t* EndDate)
	{
		BACnetDateTime temp;

		memset(&temp, 0, sizeof(temp));

		if (m_Value == NULL && m_Value->choice != BACnetCalendarEntryTypeDateRange)
			return -1;

		temp.date = m_Value->dateRange.startDate;
		bacnetapi_get_time(StartDate, &temp);
		temp.date = m_Value->dateRange.endDate;
		bacnetapi_get_time(EndDate, &temp);
		return 0;
	}


private:
	BACnetCalendarEntry* m_Value;
};

class CBACnetSpecialEvent
{
public:
	CBACnetSpecialEvent(){}
	CBACnetSpecialEvent(BACnetSpecialEvent *pSrc)
	{
		m_Value = pSrc;
	}

	//Set Properties
	int SetPeriodType(BACnetSpecialEventPeriodType data)
	{
		if (!m_Value)
			return -1;
		m_Value->periodType = data;
		return 0;
	}
	int SetEventPriority(UINT data)
	{
		if (!m_Value)
			return -1;
		m_Value->eventPriority = (UINT)data;
		return 0;
	}
	int SetCalendarReferenceObjectType(BACnetObjectType data)
	{
		if (m_Value)
		{
			m_Value->period.calendarReference.objectType = data;
			m_Value->periodType = BACnetSpecialEventPeriodTypeCalendarReference;
		}
		return 0;
	}
	int SetCalendarReferenceInstance(int data)
	{
		if (m_Value)
		{
			m_Value->period.calendarReference.instance = data;
			m_Value->periodType = BACnetSpecialEventPeriodTypeCalendarReference;
		}
		return 0;
	}

	//Get Properties
	operator BACnetSpecialEvent*(){return m_Value;}
	CSequenceOfBACnetTimeValue GetListOfTimeValues()
	{
		if (!m_Value)
			return CSequenceOfBACnetTimeValue();

		return CSequenceOfBACnetTimeValue(&m_Value->listOfTimeValues);
	}
	UINT GetEventPriority()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->eventPriority;
	}
	BACnetObjectType GetCalendarReferenceObjectType()
	{
		if (m_Value == NULL && m_Value->periodType != BACnetSpecialEventPeriodTypeCalendarReference)
			return BACnetObjectTypeNone;
		return m_Value->period.calendarReference.objectType;
	}
	int GetCalendarReferenceObjectInstance()
	{
		if (m_Value == NULL && m_Value->periodType != BACnetSpecialEventPeriodTypeCalendarReference)
			return -1;
		return m_Value->period.calendarReference.instance;
	}
	CBACnetCalendarEntry GetCalendarEntry()
	{
		if (m_Value == NULL && m_Value->periodType != BACnetSpecialEventPeriodTypeCalendarEntry)
			return CBACnetCalendarEntry();
		return CBACnetCalendarEntry(&m_Value->period.calendarEntry);
	}

private:
	BACnetSpecialEvent* m_Value;
};

class CSequenceOfBACnetSpecialEvent : CBACnetArray
{
public:
	CSequenceOfBACnetSpecialEvent(){}
	CSequenceOfBACnetSpecialEvent(SequenceOfSpecialEvent *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfBACnetSpecialEvent(){}
	//List access
	CBACnetSpecialEvent Add(BACnetSpecialEvent *specialEvent)
	{
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfSpecialEvent, bacnetDT_SpecialEvent);

		bacnapi_access_specialEvent(specialEvent, new_element, 0);
		return CBACnetSpecialEvent(specialEvent);
	}
	CBACnetSpecialEvent Add()
	{
		BACnetSpecialEvent *specialEvent = NULL;
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfSpecialEvent, bacnetDT_SpecialEvent);

		specialEvent = (BACnetSpecialEvent*)new_element->_type.specialEvent;
		return CBACnetSpecialEvent(specialEvent);
	}
	CBACnetSpecialEvent GetItem(int index)
	{
		BACnetDataType* element;
		BACnetSpecialEvent *specialEvent = NULL;

		element = GetElement(bacnetDT_ArrayOfSpecialEvent, index);
		if (element)
			specialEvent = (BACnetSpecialEvent*)element->_type.specialEvent;

		return CBACnetSpecialEvent(specialEvent);
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_ArrayOfSpecialEvent);
	}
};

class CBACnetDateTime
{
public:
	CBACnetDateTime(){}
	CBACnetDateTime(BACnetDateTime *pSrc)
	{
		m_Value = pSrc;
	}

	//Get Properties

	int GetDateTime(timeb *value)
	{
		if (m_Value == NULL)
			return -1;
		return bacnetapi_get_timeb(value, m_Value);
	}

	int SetDateTime(timeb *value)
	{
		if (m_Value == NULL)
			return -1;
		return bacnetapi_set_timeb(m_Value, value);
	}


private:
	BACnetDateTime* m_Value;
};

class CBACnetTimeStamp
{
public:
	CBACnetTimeStamp(){}
	CBACnetTimeStamp(BACnetTimeStamp *pSrc)
	{
		m_Value = pSrc;
	}

	//Get Properties
	BACnetTimeStampTypes GetType()
	{
		if (m_Value == NULL)
			return (BACnetTimeStampTypes)0;
		return m_Value->type;
	}

	int GetDateTime(timeb *value)
	{
		if (m_Value == NULL && m_Value->type != BACnetTimeStampDateTime)
			return -1;
		return bacnetapi_get_timeb(value, &m_Value->_type.dateTime);
	}

	int GetTime(timeb *value)
	{
		BACnetDateTime dateTime;

		memset(&dateTime, 0, sizeof(dateTime));

		if (m_Value == NULL && m_Value->type != BACnetTimeStampTime)
			return -1;
		dateTime.time = m_Value->_type.time;
		return bacnetapi_get_timeb(value, &dateTime);
	}

	UINT GetSequenceNumber()
	{
		if (m_Value == NULL && m_Value->type != BACnetTimeStampSequenceNumber)
			return 0;
		return m_Value->_type.sequenceNumber;
	}

private:
	BACnetTimeStamp* m_Value;
};

class CBACnetScale
{
public:
	CBACnetScale(){}
	CBACnetScale(BACnetScale *pSrc)
	{
		m_Value = pSrc;
	}

	int Copy(BACnetScale *pSrc)
	{
		if (m_Value == NULL)
			return -1;
		*m_Value = *pSrc;
		return 0;
	}

	//Get Properties
	BACnetScaleType GetChoice()
	{
		if (m_Value == NULL)
			return (BACnetScaleType)0;
		return m_Value->choice;
	}

	float GetFloatScale()
	{
		if (m_Value == NULL && m_Value->choice != ScaleType_Float)
			return 0;
		return m_Value->_choice.floatScale;
	}

	UINT GetIntegerScale()
	{
		if (m_Value == NULL && m_Value->choice != ScaleType_Integer)
			return 0;
		return m_Value->_choice.integerScale;
	}

	int SetFloatScale(float data)
	{
		if (!m_Value)
			return -1;
		m_Value->choice = ScaleType_Float;
		m_Value->_choice.floatScale = data;
		return 0;
	}

	int SetIntegerScale(UINT data)
	{
		if (!m_Value)
			return -1;
		m_Value->choice = ScaleType_Integer;
		m_Value->_choice.integerScale = data;
		return 0;
	}

private:
	BACnetScale* m_Value;
};

class CBACnetPrescale
{
public:
	CBACnetPrescale(){}
	CBACnetPrescale(BACnetPrescale *pSrc)
	{
		m_Value = pSrc;
	}

	int Copy(BACnetPrescale *pSrc)
	{
		if (m_Value == NULL)
			return -1;
		*m_Value = *pSrc;
		return 0;
	}

	//Get Properties
	UINT GetMultiplier()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->multiplier;
	}

	UINT GetModuloDivide()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->moduloDivide;
	}

	int SetMultiplier(UINT data)
	{
		if (!m_Value)
			return -1;
		m_Value->multiplier = data;
		return 0;
	}

	int SetModuloDivide(UINT data)
	{
		if (!m_Value)
			return -1;
		m_Value->moduloDivide = data;
		return 0;
	}

private:
	BACnetPrescale* m_Value;
};

class CBACnetAddress
{
public:
	CBACnetAddress(){m_Value = NULL;}
	CBACnetAddress(BACnetAddress *pSrc)
	{
		m_Value = pSrc;
	}
	//Set Properties
	int SetNetworkNumber(int data)
	{
		if (!m_Value)
			return -1;
		m_Value->network = (UINT)data;
		return 0;
	}
	int SetMACAddress(BYTE *data, int length)
	{
		if (length < 1 || length > 6)
			return -1;
		memset(&m_Value->mac, 0, sizeof(m_Value->mac));
		m_Value->mac.length = length;
		memcpy(m_Value->mac.contents, data, length);
		return 0;
	}

	//Get Properties
	int GetNetworkNumber()
	{
		if (m_Value == NULL)
			return -1;
		return (int)m_Value->network;
	}
	int GetMACAddressLength()
	{
		return m_Value->mac.length;
	}
	int GetMACAddress(BYTE* data, int* length)
	{
		*length = m_Value->mac.length;
		memcpy(data, m_Value->mac.contents, m_Value->mac.length);
		return 0;
	}

private:
	BACnetAddress* m_Value;
};

class CBACnetRecipient
{
public:
	CBACnetRecipient(){m_Value = NULL;}
	CBACnetRecipient(BACnetRecipient *pSrc)
	{
		m_Value = pSrc;
		if (m_Value && m_Value->type == BACnetRecipientTypeAddress)
			address = CBACnetAddress(&m_Value->_type.address);
	}
	//Set Properties
	int SetRecipientType(BACnetRecipientType data)
	{
		if (!m_Value)
			return -1;
		if (data != BACnetRecipientTypeAddress && m_Value->type == BACnetRecipientTypeAddress)
			bacnapi_ClearBACnetAddress(&m_Value->_type.address);
		m_Value->type = data;
		return 0;
	}
	int SetObjectIdentifierObjectType(BACnetObjectType data)
	{
		if (!m_Value)
			return -1;

		SetRecipientType(BACnetRecipientTypeDevice);
		m_Value->_type.device.objectType = data;
		return 0;
	}
	int SetObjectIdentifierInstance(int data)
	{
		if (!m_Value)
			return -1;

		SetRecipientType(BACnetRecipientTypeDevice);
		m_Value->_type.device.instance = data;
		return 0;
	}

	//Get Properties
	operator BACnetRecipient*(){return m_Value;}
	BACnetObjectType GetObjectIdentifierObjectType()
	{
		if (!m_Value)
			return BACnetObjectTypeNone;

		SetRecipientType(BACnetRecipientTypeDevice);
		return m_Value->_type.device.objectType;
	}
	int GetObjectIdentifierInstance()
	{
		if (!m_Value)
			return -1;

		SetRecipientType(BACnetRecipientTypeDevice);
		return m_Value->_type.device.instance;
	}
	BACnetRecipientType GetRecipientType()
	{
		if (m_Value == NULL)
			return (BACnetRecipientType)0;
		return m_Value->type;
	}
	CBACnetAddress& GetAddress()
	{
		if (!m_Value)
			return address;

		SetRecipientType(BACnetRecipientTypeAddress);
		return address;
	}

private:
	CBACnetAddress address;
	BACnetRecipient* m_Value;
};

class CBACnetPriorityValue
{
public:
	CBACnetPriorityValue(){}
	CBACnetPriorityValue(BACnetPriorityValue *pSrc){m_Value = pSrc;}
	//Set Properties
	void SetNull()
	{
		if (m_Value)
		{
			if (m_Value->type == BACnetPriorityValueType_AbstractData)
				bacnapi_ClearBACnetDataType(&m_Value->_type.constructedValue);
			m_Value->type = BACnetPriorityValueType_Null;
		}
	}
	void SetReal(float data)
	{
		if (m_Value)
		{
			if (m_Value->type == BACnetPriorityValueType_AbstractData)
				bacnapi_ClearBACnetDataType(&m_Value->_type.constructedValue);
			m_Value->type = BACnetPriorityValueType_Real;
			m_Value->_type.Real = data;
		}
	}
	void SetBinary(BACnetBinaryPV data)
	{
		if (m_Value)
		{
			if (m_Value->type == BACnetPriorityValueType_AbstractData)
				bacnapi_ClearBACnetDataType(&m_Value->_type.constructedValue);
			m_Value->type = BACnetPriorityValueType_Binary;
			m_Value->_type.Binary = data;
		}
	}
	void SetUnsigned(UINT data)
	{
		if (m_Value)
		{
			if (m_Value->type == BACnetPriorityValueType_AbstractData)
				bacnapi_ClearBACnetDataType(&m_Value->_type.constructedValue);
			m_Value->type = BACnetPriorityValueType_UnsignedInteger;
			m_Value->_type.UnsignedInteger = data;
		}
	}
	int SetConstructed(BACnetDataType *data)
	{
		if (m_Value)
		{
			if (m_Value->type == BACnetPriorityValueType_AbstractData)
				bacnapi_ClearBACnetDataType(&m_Value->_type.constructedValue);
			m_Value->type = BACnetPriorityValueType_AbstractData;
			return bacnapi_access_BACnetData(&m_Value->_type.constructedValue, data, -1, 1);
		}
	}

	//Get Properties
	BACnetPriorityValueTypes GetType()
	{
		if (m_Value == NULL)
			return (BACnetPriorityValueTypes)0;
		return m_Value->type;
	}
	float GetReal()
	{
		if (m_Value == NULL || m_Value->type != BACnetPriorityValueType_Real)
			return 0;
		return m_Value->_type.Real;
	}
	BACnetBinaryPV GetBinary()
	{
		if (m_Value == NULL || m_Value->type != BACnetPriorityValueType_Binary)
			return (BACnetBinaryPV)0;
		return m_Value->_type.Binary;
	}
	UINT GetUnsigned()
	{
		if (m_Value == NULL || m_Value->type != BACnetPriorityValueType_UnsignedInteger)
			return 0;
		return m_Value->_type.UnsignedInteger;
	}
	BACnetDataType* GetConstructed()
	{
		if (m_Value == NULL || m_Value->type != BACnetPriorityValueType_AbstractData)
			return NULL;
		return &m_Value->_type.constructedValue;
	}

private:
	BACnetPriorityValue* m_Value;
};

class CBACnetPriorityArray : CBACnetArray
{
public:
	CBACnetPriorityArray(){}
	CBACnetPriorityArray(BACnetPriorityArray *pSrc){m_Array = pSrc; Init();}
	virtual ~CBACnetPriorityArray(){}
	//List access
	CBACnetPriorityValue AddReal(float Value, int Index = -1)
	{
		if (Index > 16)
			return CBACnetPriorityValue();

		if (Index == -1)
		{
			BACnetPriorityValue *priorityValue = NULL;
			BACnetDataType* new_element = AddElement(bacnetDT_PriorityArray, bacnetDT_PriorityValue);

			priorityValue = (BACnetPriorityValue*)new_element->_type.priorityValue;
			CBACnetPriorityValue Val(priorityValue);
			Val.SetReal(Value);
			return Val;
		}
		if (m_Array->type == bacnetDT_PriorityArray)
		{
			BACnetDataType* element;
			BACnetPriorityValue *priorityValue = NULL;

			element = GetElement(bacnetDT_PriorityArray, Index);
			if (element)
				priorityValue = (BACnetPriorityValue*)element->_type.priorityValue;

			CBACnetPriorityValue Val(priorityValue);
			Val.SetReal(Value);
			return Val;
		}
		return CBACnetPriorityValue();
	}
	CBACnetPriorityValue AddNull(int Index = -1)
	{
		if (Index > 16)
			return CBACnetPriorityValue();

		if (Index == -1)
		{
			BACnetPriorityValue *priorityValue = NULL;
			BACnetDataType* new_element = AddElement(bacnetDT_PriorityArray, bacnetDT_PriorityValue);

			priorityValue = (BACnetPriorityValue*)new_element->_type.priorityValue;
			CBACnetPriorityValue Val(priorityValue);
			Val.SetNull();
			return Val;
		}
		if (m_Array->type == bacnetDT_PriorityArray)
		{
			BACnetDataType* element;
			BACnetPriorityValue *priorityValue = NULL;

			element = GetElement(bacnetDT_PriorityArray, Index);
			if (element)
				priorityValue = (BACnetPriorityValue*)element->_type.priorityValue;

			CBACnetPriorityValue Val(priorityValue);
			Val.SetNull();
			return Val;
		}
		return CBACnetPriorityValue();
	}
	CBACnetPriorityValue AddBinary(BACnetBinaryPV Value, int Index = -1)
	{
		if (Index > 16)
			return CBACnetPriorityValue();

		if (Index == -1)
		{
			BACnetPriorityValue *priorityValue = NULL;
			BACnetDataType* new_element = AddElement(bacnetDT_PriorityArray, bacnetDT_PriorityValue);

			priorityValue = (BACnetPriorityValue*)new_element->_type.priorityValue;
			CBACnetPriorityValue Val(priorityValue);
			Val.SetBinary(Value);
			return Val;
		}
		if (m_Array->type == bacnetDT_PriorityArray)
		{
			BACnetDataType* element;
			BACnetPriorityValue *priorityValue = NULL;

			element = GetElement(bacnetDT_PriorityArray, Index);
			if (element)
				priorityValue = (BACnetPriorityValue*)element->_type.priorityValue;

			CBACnetPriorityValue Val(priorityValue);
			Val.SetBinary(Value);
			return Val;
		}
		return CBACnetPriorityValue();
	}
	CBACnetPriorityValue AddUnsigned(UINT Value, int Index = -1)
	{
		if (Index > 16)
			return CBACnetPriorityValue();

		if (Index == -1)
		{
			BACnetPriorityValue *priorityValue = NULL;
			BACnetDataType* new_element = AddElement(bacnetDT_PriorityArray, bacnetDT_PriorityValue);

			priorityValue = (BACnetPriorityValue*)new_element->_type.priorityValue;
			CBACnetPriorityValue Val(priorityValue);
			Val.SetUnsigned(Value);
			return Val;
		}
		if (m_Array->type == bacnetDT_PriorityArray)
		{
			BACnetDataType* element;
			BACnetPriorityValue *priorityValue = NULL;

			element = GetElement(bacnetDT_PriorityArray, Index);
			if (element)
				priorityValue = (BACnetPriorityValue*)element->_type.priorityValue;

			CBACnetPriorityValue Val(priorityValue);
			Val.SetUnsigned(Value);
			return Val;
		}
		return CBACnetPriorityValue();
	}

	CBACnetPriorityValue GetItem(int index)
	{
		BACnetDataType* element;
		BACnetPriorityValue *priorityValue = NULL;

		if (m_Array->type == bacnetDT_PriorityArray)
		{
			element = GetElement(bacnetDT_PriorityArray, index);
			if (element)
				priorityValue = (BACnetPriorityValue*)element->_type.priorityValue;

		}
		return CBACnetPriorityValue(priorityValue);
	}
	int GetSize()
	{
		if (m_Array->type == bacnetDT_Null)
			return 0;
		else
			return GetArraySize(bacnetDT_SequenceOfBACnetPropertyValue);
	}
private:
	void Init()
	{
		if (m_Array->type == bacnetDT_Null ||
			m_Array->type == bacnetDT_PriorityArray && GetSize() == 0)
		{
			//Populate the Array
			for (int i = 0; i < 16; i++)
			{
				AddNull();
			}
		}
	}
};

class CBACnetAddressBinding
{
public:
	CBACnetAddressBinding(){}
	CBACnetAddressBinding(BACnetAddressBinding *pSrc)
	{
		m_Value = pSrc;
	}

	//Set Properties
	int SetDeviceId(int data)
	{
		if (!m_Value)
			return -1;
		m_Value->deviceObjectIdentifier.objectType = BACnetObjectTypeDevice;
		m_Value->deviceObjectIdentifier.instance = data;
		return 0;
	}
	int SetNetworkNumber(int data)
	{
		if (!m_Value)
			return -1;
		m_Value->deviceAddress.network = (UINT)data;
		return 0;
	}
	int SetMACAddress(BYTE *data, int length)
	{
		if (length < 1 || length > 6)
			return -1;
		memset(&m_Value->deviceAddress.mac, 0, sizeof(m_Value->deviceAddress.mac));
		m_Value->deviceAddress.mac.length = length;
		memcpy(m_Value->deviceAddress.mac.contents, data, length);
		return 0;
	}

	//Get Properties
	operator BACnetAddressBinding*(){return m_Value;}
	int GetDeviceId(){return m_Value->deviceObjectIdentifier.instance;}
	int GetNetworkNumber()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->deviceAddress.network;
	}
	int GetMACAddressLength()
	{
		return m_Value->deviceAddress.mac.length;
	}
	int GetMACAddress(BYTE* data, int* length)
	{
		*length = m_Value->deviceAddress.mac.length;
		memcpy(data, m_Value->deviceAddress.mac.contents, m_Value->deviceAddress.mac.length);
		return 0;
	}
	CBACnetAddress GetAddress(){return CBACnetAddress(&m_Value->deviceAddress);}

private:
	BACnetAddressBinding* m_Value;
};

class CBACnetDestination
{
public:
	CBACnetDestination(){}
	CBACnetDestination(BACnetDestination *pSrc)
	{
		m_Value = pSrc;
		if (m_Value)
		{
			validDays = CBACnetDaysOfWeek(&m_Value->validDays);
			recipient = CBACnetRecipient(&m_Value->recipient);
			transitions = CBACnetTransitionBits(&m_Value->transitions);
		}
	}

	//Set Properties
	int Set(int Instance, BYTE IssueConfirmedNotifications = 0xFF, UINT Lifetime = 0, UINT ProcessIdentifier = 0xFF)
	{
		int i;
		m_Value->recipient.type = BACnetRecipientTypeDevice;
		m_Value->recipient._type.device.instance = Instance;
		m_Value->recipient._type.device.objectType = BACnetObjectTypeDevice;
		m_Value->issueConfirmedNotifications = IssueConfirmedNotifications;
		m_Value->processIdentifier = ProcessIdentifier;
		for (i=0; i<3; i++)
		{
			if (bacnapi_set_bit(&m_Value->transitions, 0xFF, i))
				return -1;
		}
		for (i=0; i<7; i++)
		{
			if (bacnapi_set_bit(&m_Value->validDays, 0xFF, i))
				return -1;
		}
		memset(&m_Value->fromTime,0xFF, sizeof(m_Value->fromTime));
		memset(&m_Value->toTime,0xFF, sizeof(m_Value->fromTime));

		return 0;
	}

	int SetFromTime(const time_t data)
	{
		if (m_Value == NULL)
			return -1;
		BACnetDateTime temp;
		bacnetapi_set_time(&temp, data);
		m_Value->fromTime = temp.time;
		return 0;
	}
	int SetToTime(const time_t data)
	{
		if (m_Value == NULL)
			return -1;
		BACnetDateTime temp;
		bacnetapi_set_time(&temp, data);
		m_Value->toTime = temp.time;
		return 0;
	}
	int SetProcessIdentifier(UINT data)
	{
		if (m_Value == NULL)
			return -1;
		m_Value->processIdentifier = data;
		return 0;
	}
	int SetIssueConfirmedNotifications(BYTE data)
	{
		if (m_Value == NULL)
			return -1;
		m_Value->issueConfirmedNotifications = data;
		return 0;
	}

	//Get Properties
	operator BACnetDestination*(){return m_Value;}
	UINT GetProcessIdentifier(){return m_Value->processIdentifier;}
	BYTE GetIssueConfirmedNotifications(){return m_Value->issueConfirmedNotifications;}
	time_t GetFromTime()
	{
		BACnetDateTime temp;
		time_t val = 0;

		memset(&temp, 0, sizeof(temp));

		temp.time = m_Value->fromTime;
		bacnetapi_get_time(&val, &temp);
		return val;
	}
	time_t GetToTime()
	{
		BACnetDateTime temp;
		time_t val = 0;

		memset(&temp, 0, sizeof(temp));

		temp.time = m_Value->toTime;
		bacnetapi_get_time(&val, &temp);
		return val;
	}

	CBACnetDaysOfWeek &GetValidDays(){return validDays;}
	CBACnetRecipient &GetRecipient(){return recipient;}
	CBACnetTransitionBits &GetTransitions(){return transitions;}
private:
	CBACnetDaysOfWeek validDays;
	CBACnetRecipient recipient;
	CBACnetTransitionBits transitions;
	BACnetDestination* m_Value;
};

class CSequenceOfBACnetObjectIdentifier : CBACnetArray
{
public:
	CSequenceOfBACnetObjectIdentifier(){}
	CSequenceOfBACnetObjectIdentifier(SequenceOfBACnetObjectIdentifier *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfBACnetObjectIdentifier(){}
	//List access
	CBACnetObjectIdentifier Add(BACnetObjectType ObjectType, int Instance)
	{
		BACnetDataType empty_element;
		BACnetDataType* new_element;
		BACnetObjectIdentifier *objectIdentifier = NULL;

		Initialize(bacnetDT_ArrayOfBACnetObjectIdentifier);

		memset(&empty_element, 0, sizeof(empty_element));
		new_element = bacnapi_set_element_in_bacnet_array((BACnetArray*)m_Array->_type.array, &empty_element);
		if (new_element == NULL)
			return CBACnetObjectIdentifier();

		bacnapi_initialize_data(new_element, bacnetDT_ObjectIdentifier);
		if (new_element->type != bacnetDT_ObjectIdentifier)
			return CBACnetObjectIdentifier();

		objectIdentifier = &new_element->_type.ObjectIdentifier;
		if (objectIdentifier)
		{
			objectIdentifier->instance = Instance;
			objectIdentifier->objectType = ObjectType;
		}
		return CBACnetObjectIdentifier(objectIdentifier);
	}
	CBACnetObjectIdentifier GetItem(int index)
	{
		BACnetDataType* element;
		BACnetObjectIdentifier *objectIdentifier = NULL;

		element = GetElement(bacnetDT_ArrayOfBACnetObjectIdentifier, index);
		if (element)
			return CBACnetObjectIdentifier(&element->_type.ObjectIdentifier);

		return CBACnetObjectIdentifier(objectIdentifier);
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_ArrayOfBACnetObjectIdentifier);
	}
};

class CSequenceOfPriority : CBACnetArray
{
    //to offnormal
    //to BACnetEventStateFault
	//to BACnetEventStateNormal
public:
	CSequenceOfPriority()
	{
		m_Array = NULL;
	}
	CSequenceOfPriority(SequenceOfUnsigned * pSrc)
	{
		m_Array = pSrc;
	}
	operator SequenceOfUnsigned*(){return m_Array;}
	UINT GetToOffNormal()
	{
		BACnetDataType* element;

		if (m_Array == NULL || m_Array->type != bacnetDT_ArrayOfUnsigned || GetArraySize(bacnetDT_ArrayOfUnsigned) != 3)
			return 0;

		element = GetElement(bacnetDT_ArrayOfUnsigned, 0);
		if (element)
			return element->_type.UnsignedInteger;

		return 0;
	}
	UINT GetToFault()
	{
		BACnetDataType* element;

		if (m_Array == NULL || m_Array->type != bacnetDT_ArrayOfUnsigned || GetArraySize(bacnetDT_ArrayOfUnsigned) != 3)
			return 0;

		element = GetElement(bacnetDT_ArrayOfUnsigned, 1);
		if (element)
			return element->_type.UnsignedInteger;

		return 0;
	}
	UINT GetToNormal()
	{
		BACnetDataType* element;

		if (m_Array == NULL || m_Array->type != bacnetDT_ArrayOfUnsigned || GetArraySize(bacnetDT_ArrayOfUnsigned) != 3)
			return 0;

		element = GetElement(bacnetDT_ArrayOfUnsigned, 2);
		if (element)
			return element->_type.UnsignedInteger;

		return 0;
	}
	UINT SetToOffNormal(BYTE data)
	{
		BACnetDataType* element;
		Init(3);

		element = GetElement(bacnetDT_ArrayOfUnsigned, 0);
		if (element)
		{
			element->_type.UnsignedInteger = data;
			return data;
		}
		return 0;
	}
	UINT SetToFault(BYTE data)
	{
		BACnetDataType* element;
		Init(3);

		element = GetElement(bacnetDT_ArrayOfUnsigned, 1);
		if (element)
		{
			element->_type.UnsignedInteger = data;
			return data;
		}
		return 0;
	}
	UINT SetToNormal(BYTE data)
	{
		BACnetDataType* element;
		Init(3);

		element = GetElement(bacnetDT_ArrayOfUnsigned, 2);
		if (element)
		{
			element->_type.UnsignedInteger = data;
			return data;
		}
		return 0;
	}
private:
	BYTE Init(int size)
	{
		BACnetArray* array;

		if (m_Array == NULL || m_Array->type != bacnetDT_ArrayOfUnsigned)
			return 0;

		array = (BACnetArray*)m_Array->_type.sequenceOfUnsigned;
		if (GetArraySize(bacnetDT_ArrayOfUnsigned) >= size)
			return 0xFF;

		for (int i=array->ubound; i<size; i++)
		{
			if (AddElement(bacnetDT_ArrayOfUnsigned, bacnetDT_Enumerated) == NULL)
				return 0x0;
		}
		return 0xFF;
	}
};

class CBACnetEventTimeStamps : CBACnetArray
{
    //to offnormal
    //to BACnetEventStateFault
	//to BACnetEventStateNormal
public:
	CBACnetEventTimeStamps()
	{
		m_Array = NULL;
	}
	CBACnetEventTimeStamps(BACnetEventTimeStamps * pSrc)
	{
		m_Array = pSrc;
	}
	operator BACnetEventTimeStamps*(){return m_Array;}
	CBACnetTimeStamp GetToOffNormal()
	{
		BACnetDataType* element;
		BACnetTimeStamp *timeStamp = NULL;

		if (m_Array != NULL && m_Array->type == bacnetDT_EventTimeStamps && GetArraySize(bacnetDT_EventTimeStamps) == 3)
		{
			element = GetElement(bacnetDT_EventTimeStamps, 0);
			if (element)
				timeStamp = (BACnetTimeStamp*)element->_type.timeStamp;
		}
		return CBACnetTimeStamp(timeStamp);
	}
	CBACnetTimeStamp GetToFault()
	{
		BACnetDataType* element;
		BACnetTimeStamp *timeStamp = NULL;

		if (m_Array != NULL && m_Array->type == bacnetDT_EventTimeStamps && GetArraySize(bacnetDT_EventTimeStamps) == 3)
		{
			element = GetElement(bacnetDT_EventTimeStamps, 1);
			if (element)
				timeStamp = (BACnetTimeStamp*)element->_type.timeStamp;
		}
		return CBACnetTimeStamp(timeStamp);
	}
	CBACnetTimeStamp GetToNormal()
	{
		BACnetDataType* element;
		BACnetTimeStamp *timeStamp = NULL;

		if (m_Array != NULL && m_Array->type == bacnetDT_EventTimeStamps && GetArraySize(bacnetDT_EventTimeStamps) == 3)
		{
			element = GetElement(bacnetDT_EventTimeStamps, 2);
			if (element)
				timeStamp = (BACnetTimeStamp*)element->_type.timeStamp;
		}
		return CBACnetTimeStamp(timeStamp);
	}
	BACnetTimeStamp SetToOffNormal(BACnetTimeStamp data)
	{
		BACnetDataType* element;
		Init(3);

		element = GetElement(bacnetDT_EventTimeStamps, 0);
		if (element)
		{
			*(BACnetTimeStamp*)element->_type.timeStamp = data;
		}
		return data;
	}
	BACnetTimeStamp SetToFault(BACnetTimeStamp data)
	{
		BACnetDataType* element;
		Init(3);

		element = GetElement(bacnetDT_EventTimeStamps, 1);
		if (element)
		{
			*(BACnetTimeStamp*)element->_type.timeStamp = data;
		}
		return data;
	}
	BACnetTimeStamp SetToNormal(BACnetTimeStamp data)
	{
		BACnetDataType* element;
		Init(3);

		element = GetElement(bacnetDT_EventTimeStamps, 2);
		if (element)
		{
			*(BACnetTimeStamp*)element->_type.timeStamp = data;
		}
		return data;
	}
private:
	BYTE Init(int size)
	{
		BACnetArray* array;

		if (m_Array == NULL || m_Array->type != bacnetDT_EventTimeStamps)
			return 0;

		array = (BACnetArray*)m_Array->_type.eventTimeStamps;
		if (GetArraySize(bacnetDT_EventTimeStamps) >= size)
			return 0xFF;

		for (int i=array->ubound; i<size; i++)
		{
			if (AddElement(bacnetDT_EventTimeStamps, bacnetDT_TimeStamp) == NULL)
				return 0x0;
		}
		return 0xFF;
	}
};

class CSequenceOfBACnetRecipient : CBACnetArray
{
public:
	CSequenceOfBACnetRecipient(){}
	CSequenceOfBACnetRecipient(SequenceOfBACnetRecipient *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfBACnetRecipient(){}
	//List access
	CBACnetRecipient Add(CBACnetRecipient &data)
	{
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfBACnetRecipient, bacnetDT_Recipient);

		bacnapi_access_recipient(data, new_element, 0);

		return CBACnetRecipient((BACnetRecipient*)new_element->_type.recipient);
	}
	CBACnetRecipient Add(int Instance, BYTE IssueConfirmedNotifications = 0xFF, UINT Lifetime = 0, UINT ProcessIdentifier = 0xFF)
	{
		BACnetRecipient *recipient = NULL;
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfBACnetRecipient, bacnetDT_Recipient);

		recipient = (BACnetRecipient*)new_element->_type.recipient;
		if (recipient)
		{
			recipient->type = BACnetRecipientTypeDevice;
			recipient->_type.device.instance = Instance;
			recipient->_type.device.objectType = BACnetObjectTypeDevice;
		}

		return CBACnetRecipient(recipient);
	}
	CBACnetRecipient GetItem(int index)
	{
		BACnetDataType* element;
		BACnetRecipient *recipient = NULL;

		element = GetElement(bacnetDT_ArrayOfBACnetRecipient, index);
		if (element)
			recipient = (BACnetRecipient*)element->_type.recipient;

		return CBACnetRecipient(recipient);
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_ArrayOfBACnetRecipient);
	}
};

class CSequenceOfBACnetAddressBinding : CBACnetArray
{
public:
	CSequenceOfBACnetAddressBinding(){}
	CSequenceOfBACnetAddressBinding(SequenceOfBACnetAddressBinding *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfBACnetAddressBinding(){}
	//List access
	CBACnetAddressBinding Add(CBACnetAddressBinding &data)
	{
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfBACnetAddressBinding, bacnetDT_AddressBinding);

		bacnapi_access_addressBinding(data, new_element, 0);

		return CBACnetAddressBinding((BACnetAddressBinding*)new_element->_type.addressBinding);
	}
	CBACnetAddressBinding GetItem(int index)
	{
		BACnetDataType* element;
		BACnetAddressBinding *addressBinding = NULL;

		element = GetElement(bacnetDT_ArrayOfBACnetAddressBinding, index);
		if (element)
			addressBinding = (BACnetAddressBinding*)element->_type.addressBinding;

		return CBACnetAddressBinding(addressBinding);
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_ArrayOfBACnetAddressBinding);
	}
};

class CSequenceOfBACnetDestination : CBACnetArray
{
public:
	CSequenceOfBACnetDestination(){}
	CSequenceOfBACnetDestination(SequenceOfBACnetDestination *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfBACnetDestination(){}
	//List access
	CBACnetDestination Add(CBACnetDestination &data)
	{
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfBACnetDestination, bacnetDT_Destination);

		bacnapi_access_destination(data, new_element, 0);

		return CBACnetDestination((BACnetDestination*)new_element->_type.destination);
	}
	CBACnetDestination Add(int Instance, BYTE IssueConfirmedNotifications = 0xFF, UINT Lifetime = 0, UINT ProcessIdentifier = 0xFF)
	{
		BACnetDestination *destination = NULL;
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfBACnetDestination, bacnetDT_Destination);

		destination = (BACnetDestination*)new_element->_type.destination;
		if (destination)
		{
			int i;
			destination->recipient.type = BACnetRecipientTypeDevice;
			destination->recipient._type.device.instance = Instance;
			destination->recipient._type.device.objectType = BACnetObjectTypeDevice;
			destination->issueConfirmedNotifications = IssueConfirmedNotifications;
			destination->processIdentifier = ProcessIdentifier;
			for (i=0; i<3; i++)
			{
				if (bacnapi_set_bit(&destination->transitions, 0xFF, i))
					return 0x0;
			}
			for (i=0; i<7; i++)
			{
				if (bacnapi_set_bit(&destination->validDays, 0xFF, i))
					return 0x0;
			}
			memset(&destination->fromTime,0xFF, sizeof(destination->fromTime));
			memset(&destination->toTime,0xFF, sizeof(destination->fromTime));
		}

		return CBACnetDestination(destination);
	}
	CBACnetDestination GetItem(int index)
	{
		BACnetDataType* element;
		BACnetDestination *destination = NULL;

		element = GetElement(bacnetDT_ArrayOfBACnetDestination, index);
		if (element)
			destination = (BACnetDestination*)element->_type.destination;

		return CBACnetDestination(destination);
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_ArrayOfBACnetDestination);
	}
};

class CSequenceOfBACnetActionCommand : CBACnetArray
{
public:
	CSequenceOfBACnetActionCommand(){}
	CSequenceOfBACnetActionCommand(SequenceOfBACnetActionCommand *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfBACnetActionCommand(){}
	//List access
	CBACnetActionCommand Add(int DeviceID, BACnetObjectType ObjectType, int Instance, BACnetPropertyIdentifier Property, int Index = -1)
	{
		BACnetActionCommand *actionCommand = NULL;
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfBACnetActionCommand, bacnetDT_ActionCommand);

		actionCommand = (BACnetActionCommand*)new_element->_type.actionCommand;
		if (actionCommand)
		{
			actionCommand->deviceIdentifier.objectType = BACnetObjectTypeDevice;
			actionCommand->deviceIdentifier.instance = DeviceID;
			actionCommand->objectIdentifier.objectType = ObjectType;
			actionCommand->objectIdentifier.instance = Instance;
			actionCommand->propertyIdentifier = Property;
			actionCommand->propertyArrayIndex = Index;
		}

		return CBACnetActionCommand(actionCommand);
	}
	CBACnetActionCommand GetItem(int index)
	{
		BACnetDataType* element;
		BACnetActionCommand *actionCommand = NULL;

		element = GetElement(bacnetDT_ArrayOfBACnetActionCommand, index);
		if (element)
			actionCommand = (BACnetActionCommand*)element->_type.actionCommand;

		return CBACnetActionCommand(actionCommand);
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_ArrayOfBACnetActionCommand);
	}
};

class CBACnetActionList
{
public:
	CBACnetActionList(){}
	CBACnetActionList(BACnetActionList *pSrc){m_Value = pSrc;}

	CSequenceOfBACnetActionCommand GetAction()
	{
		return CSequenceOfBACnetActionCommand(&m_Value->action);
	}
private:
	BACnetActionList* m_Value;
};

class CBACnetDailySchedule
{
public:
	CBACnetDailySchedule(){}
	CBACnetDailySchedule(BACnetDailySchedule *pSrc){m_Value = pSrc;}

	CSequenceOfBACnetTimeValue GetDaySchedule()
	{
		return CSequenceOfBACnetTimeValue(&m_Value->daySchedule);
	}

	operator BACnetDailySchedule*(){return m_Value;}
private:
	BACnetDailySchedule* m_Value;
};

class CSequenceOfBACnetDailySchedule : CBACnetArray
{
public:
	CSequenceOfBACnetDailySchedule(){}
	CSequenceOfBACnetDailySchedule(SequenceOfBACnetDailySchedule *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfBACnetDailySchedule(){}
	//List access
	CBACnetDailySchedule Add()
	{
		BACnetDailySchedule *dailySchedule = NULL;
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfBACnetDailySchedule, bacnetDT_DailySchedule);

		dailySchedule = (BACnetDailySchedule*)new_element->_type.dailySchedule;

		return CBACnetDailySchedule(dailySchedule);
	}
	CBACnetDailySchedule GetItem(int index)
	{
		BACnetDataType* element;
		BACnetDailySchedule *dailySchedule = NULL;

		element = GetElement(bacnetDT_ArrayOfBACnetDailySchedule, index);
		if (element)
			dailySchedule = (BACnetDailySchedule*)element->_type.destination;

		return CBACnetDailySchedule(dailySchedule);
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_ArrayOfBACnetDailySchedule);
	}
};

class CSequenceOfBACnetCalendarEntry : CBACnetArray
{
public:
	CSequenceOfBACnetCalendarEntry(){}
	CSequenceOfBACnetCalendarEntry(SequenceOfBACnetCalendarEntry *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfBACnetCalendarEntry(){}
	//List access
	CBACnetCalendarEntry Add(BACnetCalendarEntry *calendarEntry)
	{
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfBACnetCalendarEntry, bacnetDT_CalendarEntry);

		bacnapi_access_calendarEntry(calendarEntry, new_element, 0);
		return CBACnetCalendarEntry(calendarEntry);
	}
	CBACnetCalendarEntry Add()
	{
		BACnetCalendarEntry *calendarEntry = NULL;
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfBACnetCalendarEntry, bacnetDT_CalendarEntry);

		calendarEntry = (BACnetCalendarEntry*)new_element->_type.calendarEntry;

		return CBACnetCalendarEntry(calendarEntry);
	}
	CBACnetCalendarEntry AddDate(int Year, int Month, int Day)
	{
		BACnetCalendarEntry *calendarEntry = NULL;
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfBACnetCalendarEntry, bacnetDT_CalendarEntry);

		calendarEntry = (BACnetCalendarEntry*)new_element->_type.calendarEntry;

		CBACnetCalendarEntry newCalendarEntry(calendarEntry);
		newCalendarEntry.SetDate(Year, Month, Day);
		return newCalendarEntry;
	}
	CBACnetCalendarEntry GetItem(int index)
	{
		BACnetDataType* element;
		BACnetCalendarEntry *calendarEntry = NULL;

		element = GetElement(bacnetDT_ArrayOfBACnetCalendarEntry, index);
		if (element)
			calendarEntry = (BACnetCalendarEntry*)element->_type.destination;

		return CBACnetCalendarEntry(calendarEntry);
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_ArrayOfBACnetCalendarEntry);
	}
};

class CSequenceOfBACnetActionList : CBACnetArray
{
public:
	CSequenceOfBACnetActionList(){}
	CSequenceOfBACnetActionList(SequenceOfBACnetActionList *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfBACnetActionList(){}
	//List access
	CBACnetActionList Add()
	{
		BACnetActionList *actionList = NULL;
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfBACnetActionList, bacnetDT_ActionList);

		actionList = (BACnetActionList*)new_element->_type.actionList;

		return CBACnetActionList(actionList);
	}
	CBACnetActionList GetItem(int index)
	{
		BACnetDataType* element;
		BACnetActionList *actionList = NULL;

		element = GetElement(bacnetDT_ArrayOfBACnetActionList, index);
		if (element)
			actionList = (BACnetActionList*)element->_type.destination;

		return CBACnetActionList(actionList);
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_ArrayOfBACnetActionList);
	}
};

class CSequenceOfBACnetDeviceObjectPropertyReference : CBACnetArray
{
public:
	CSequenceOfBACnetDeviceObjectPropertyReference(){}
	CSequenceOfBACnetDeviceObjectPropertyReference(SequenceOfBACnetDeviceObjectPropertyReference *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfBACnetDeviceObjectPropertyReference(){}
	//List access
	CBACnetDeviceObjectPropertyReference Add(int DeviceID, BACnetObjectType ObjectType, int Instance, BACnetPropertyIdentifier Property, int Index = -1)
	{
		BACnetDeviceObjectPropertyReference *deviceObjectPropertyReference = NULL;
		BACnetDataType* new_element = AddElement(bacnetDT_ArrayOfBACnetObjectPropertyReference, bacnetDT_DeviceObjectPropertyReference);

		deviceObjectPropertyReference = (BACnetDeviceObjectPropertyReference*)new_element->_type.deviceObjectPropertyReference;
		if (deviceObjectPropertyReference)
		{
			deviceObjectPropertyReference->deviceIdentifier.objectType = BACnetObjectTypeDevice;
			deviceObjectPropertyReference->deviceIdentifier.instance = DeviceID;
			deviceObjectPropertyReference->objectIdentifier.objectType = ObjectType;
			deviceObjectPropertyReference->objectIdentifier.instance = Instance;
			deviceObjectPropertyReference->property = Property;
			deviceObjectPropertyReference->propertyArrayIndex = Index;
		}

		return CBACnetDeviceObjectPropertyReference(deviceObjectPropertyReference);
	}
	CBACnetDeviceObjectPropertyReference GetItem(int index)
	{
		BACnetDataType* element;
		BACnetDeviceObjectPropertyReference *deviceObjectPropertyReference = NULL;

		element = GetElement(bacnetDT_ArrayOfBACnetObjectPropertyReference, index);
		if (element)
			deviceObjectPropertyReference = (BACnetDeviceObjectPropertyReference*)element->_type.destination;

		return CBACnetDeviceObjectPropertyReference(deviceObjectPropertyReference);
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_ArrayOfBACnetObjectPropertyReference);
	}
};

class CBACnetEventParameterChangeOfBitString
{
public:
	CBACnetEventParameterChangeOfBitString(){}
	CBACnetEventParameterChangeOfBitString(_BACnetEventParameterChangeOfBitString *pSrc)
	{
		m_Value = pSrc;
	}
private:
	_BACnetEventParameterChangeOfBitString* m_Value;
};

class CBACnetEventParameterChangeOfState
{
public:
	CBACnetEventParameterChangeOfState(){}
	CBACnetEventParameterChangeOfState(_BACnetEventParameterChangeOfState *pSrc)
	{
		m_Value = pSrc;
	}
	CSequenceOfBACnetPropertyStates GetPropertyStates()
	{
		return CSequenceOfBACnetPropertyStates(&m_Value->listOfValues);
	}
	void SetTimeDelay(UINT Data)
	{
		if (m_Value)
			m_Value->timeDelay = Data;
	}
	void SetChangeOfState(UINT TimeDelay, BACnetBinaryPV State = BACnetBinaryPVActive)
	{
		if (m_Value)
		{
			m_Value->timeDelay = TimeDelay;
			CSequenceOfBACnetPropertyStates sequenceOfStates(&m_Value->listOfValues);
			sequenceOfStates.Add(State);
		}
	}
	UINT GetTimeDelay()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->timeDelay;
	}
private:
	_BACnetEventParameterChangeOfState* m_Value;
};

class CBACnetEventParameterChangeOfValue
{
public:
	CBACnetEventParameterChangeOfValue(){}
	CBACnetEventParameterChangeOfValue(_BACnetEventParameterChangeOfValue *pSrc)
	{
		m_Value = pSrc;
	}
	//Set
	int SetTimeDelay(UINT data)
	{
		if (!m_Value)
			return -1;
		m_Value->timeDelay = data;
		return 0;
	}
	int SetReferencedPropertyIncrement(float data)
	{
		ChangeType(BACnetCovCriteriaChoiceReferencedPropertyIncrement);
		if (!m_Value)
			return -1;
		m_Value->_covCriteria.referencedPropertyIncrement = data;
		return 0;
	}
	int Set(UINT TimeDelay, float Increment)
	{
		ChangeType(BACnetCovCriteriaChoiceReferencedPropertyIncrement);
		if (!m_Value)
			return -1;
		m_Value->_covCriteria.referencedPropertyIncrement = Increment;
		m_Value->timeDelay = TimeDelay;
		return 0;
	}

	//Get
	UINT GetTimeDelay()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->timeDelay;
	}
	CBACnetBitString GetBitMask()
	{
		ChangeType(BACnetCovCriteriaChoiceBitmask);
		bitMask = CBACnetBitString(&m_Value->_covCriteria.bitmask);
		return bitMask;
	}
	float GetReferencedPropertyIncrement()
	{
		ChangeType(BACnetCovCriteriaChoiceReferencedPropertyIncrement);
		if (m_Value == NULL)
			return 0;
		return m_Value->_covCriteria.referencedPropertyIncrement;
	}

private:
	void ChangeType(BACnetCovCriteriaChoice type)
	{
		if (m_Value->covCriteria != type && m_Value->covCriteria == BACnetCovCriteriaChoiceBitmask)
			bacnapi_ClearBitString(&m_Value->_covCriteria.bitmask);
		m_Value->covCriteria = type;
	}
	CBACnetBitString bitMask;
	_BACnetEventParameterChangeOfValue* m_Value;
};

class CBACnetEventParameterCommandFailure
{
public:
	CBACnetEventParameterCommandFailure(){}
	CBACnetEventParameterCommandFailure(_BACnetEventParameterCommandFailure *pSrc)
	{
		m_Value = pSrc;
		if (m_Value)
			feedbackPropertyReference = CBACnetDeviceObjectPropertyReference(&m_Value->feedbackPropertyReference);
	}
	//Set
	int SetTimeDelay(UINT data)
	{
		if (!m_Value)
			return -1;
		m_Value->timeDelay = data;
		return 0;
	}
	int Set(UINT TimeDelay, int DeviceID, BACnetObjectType ObjectType, int Instance, BACnetPropertyIdentifier Property, int Index = -1)
	{
		if (!m_Value)
			return -1;
		m_Value->timeDelay = TimeDelay;
		feedbackPropertyReference.Set(DeviceID, ObjectType, Instance, Property, Index);
		return 0;
	}

	//Get
	UINT GetTimeDelay()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->timeDelay;
	}
	CBACnetDeviceObjectPropertyReference& GetFeedbackPropertyReference(){return feedbackPropertyReference;}
private:
	CBACnetDeviceObjectPropertyReference feedbackPropertyReference;
	_BACnetEventParameterCommandFailure* m_Value;
};

class CBACnetEventParameterFloatingLimit
{
public:
	CBACnetEventParameterFloatingLimit(){}
	CBACnetEventParameterFloatingLimit(_BACnetEventParameterFloatingLimit *pSrc)
	{
		m_Value = pSrc;
		if (m_Value)
			setpointReference = CBACnetDeviceObjectPropertyReference(&m_Value->setpointReference);
	}
	//Set
	int SetTimeDelay(UINT data)
	{
		if (!m_Value)
			return -1;
		m_Value->timeDelay = data;
		return 0;
	}
	int SetLowDiffLimit(float data)
	{
		if (!m_Value)
			return -1;
		m_Value->lowDiffLimit = data;
		return 0;
	}
	int SetHighDiffLimit(float data)
	{
		if (!m_Value)
			return -1;
		m_Value->highDiffLimit = data;
		return 0;
	}
	int SetDeadband(float data)
	{
		if (!m_Value)
			return -1;
		m_Value->deadband = data;
		return 0;
	}
	int Set(UINT TimeDelay, float LowDiffLimit, float HighDiffLimit, float Deadband, int DeviceID, BACnetObjectType ObjectType, int Instance, BACnetPropertyIdentifier Property, int Index = -1)
	{
		if (!m_Value)
			return -1;
		m_Value->timeDelay = TimeDelay;
		m_Value->lowDiffLimit = LowDiffLimit;
		m_Value->highDiffLimit = HighDiffLimit;
		m_Value->deadband = Deadband;
		setpointReference.Set(DeviceID, ObjectType, Instance, Property, Index);
		return 0;
	}

	//Get
	UINT GetTimeDelay()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->timeDelay;
	}
	float GetLowDiffLimit()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->lowDiffLimit;
	}
	float GetHighDiffLimit()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->highDiffLimit;
	}
	float GetDeadband()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->deadband;
	}
	CBACnetDeviceObjectPropertyReference& GetFeedbackPropertyReference(){return setpointReference;}
private:
	CBACnetDeviceObjectPropertyReference setpointReference;
	_BACnetEventParameterFloatingLimit* m_Value;
};

class CBACnetEventParameterOutOfRange
{
public:
	CBACnetEventParameterOutOfRange(){}
	CBACnetEventParameterOutOfRange(_BACnetEventParameterOutOfRange *pSrc)
	{
		m_Value = pSrc;
	}
	//Set
	int SetTimeDelay(UINT data)
	{
		if (!m_Value)
			return -1;
		m_Value->timeDelay = data;
		return 0;
	}
	int SetLowDiffLimit(float data)
	{
		if (!m_Value)
			return -1;
		m_Value->lowDiffLimit = data;
		return 0;
	}
	int SetHighDiffLimit(float data)
	{
		if (!m_Value)
			return -1;
		m_Value->highDiffLimit = data;
			return 0;
	}
	int SetDeadband(float data)
	{
		if (!m_Value)
			return -1;
		m_Value->deadband = data;
		return 0;
	}
	int Set(UINT TimeDelay, float LowDiffLimit, float HighDiffLimit, float Deadband)
	{
		if (!m_Value)
			return -1;
		m_Value->timeDelay = TimeDelay;
		m_Value->lowDiffLimit = LowDiffLimit;
		m_Value->highDiffLimit = HighDiffLimit;
		m_Value->deadband = Deadband;
		return 0;
	}

	//Get
	UINT GetTimeDelay()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->timeDelay;
	}
	float GetLowDiffLimit()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->lowDiffLimit;
	}
	float GetHighDiffLimit()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->highDiffLimit;
	}
	float GetDeadband()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->deadband;
	}

private:
	_BACnetEventParameterOutOfRange* m_Value;
};

class CBACnetEventParameterBufferReady
{
public:
	CBACnetEventParameterBufferReady(){}
	CBACnetEventParameterBufferReady(_BACnetEventParameterBufferReady *pSrc)
	{
		m_Value = pSrc;
	}
	//Set
	int SetNotificationThreshold(UINT data)
	{
		if (!m_Value)
			return -1;
		m_Value->notificationThreshold = data;
			return 0;
	}
	int SetPreviousNotificationCount(UINT data)
	{
		if (!m_Value)
			return -1;
		m_Value->previousNotificationCount = data;
		return 0;
	}
	int Set(UINT TimeDelay, UINT NotificationThreshold, UINT PreviousNotificationCount)
	{
		if (!m_Value)
			return -1;
		m_Value->notificationThreshold = NotificationThreshold;
		m_Value->previousNotificationCount = PreviousNotificationCount;
		return 0;
	}

	//Get
	UINT GetNotificationThreshold()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->notificationThreshold;
	}
	UINT GetPreviousNotificationCount()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->previousNotificationCount;
	}

private:
	_BACnetEventParameterBufferReady* m_Value;
};

class CBACnetEventParameter
{
public:
	CBACnetEventParameter(){}
	CBACnetEventParameter(BACnetEventParameter *pSrc)
	{
		m_Value = pSrc;
	}
	void SetChangeOfState(UINT TimeDelay, BACnetBinaryPV State = BACnetBinaryPVActive)
	{
		if (m_Value)
		{
			ChangeType(BACnetEventTypeChangeOfState);
			CBACnetEventParameterChangeOfState changeOfState(&m_Value->_type.changeOfState);
			changeOfState.SetChangeOfState(TimeDelay, State);
		}
	}
	void SetChangeOfValue(UINT TimeDelay, float Increment)
	{
		if (m_Value)
		{
			ChangeType(BACnetEventTypeChangeOfValue);
			CBACnetEventParameterChangeOfValue changeOfValue(&m_Value->_type.changeOfValue);
			changeOfValue.Set(TimeDelay, Increment);
		}
	}
	void SetCommandFailure(UINT TimeDelay, int DeviceID, BACnetObjectType ObjectType, int Instance, BACnetPropertyIdentifier Property, int Index = -1)
	{
		if (m_Value)
		{
			ChangeType(BACnetEventTypeCommandFailure);
			CBACnetEventParameterCommandFailure commandFailure(&m_Value->_type.commandFailure);
			commandFailure.Set(TimeDelay, DeviceID, ObjectType, Instance, Property, Index);
		}
	}
	void SetFloatingLimit(UINT TimeDelay, float LowDiffLimit, float HighDiffLimit, float Deadband, int DeviceID, BACnetObjectType ObjectType, int Instance, BACnetPropertyIdentifier Property, int Index = -1)
	{
		if (m_Value)
		{
			ChangeType(BACnetEventTypeFloatingLimit);
			CBACnetEventParameterFloatingLimit floatingLimit(&m_Value->_type.floatingLimit);
			floatingLimit.Set(TimeDelay, LowDiffLimit, HighDiffLimit, Deadband, DeviceID, ObjectType, Instance, Property, Index);
		}
	}
	void SetOutOfRange(UINT TimeDelay, float LowDiffLimit, float HighDiffLimit, float Deadband)
	{
		if (m_Value)
		{
			ChangeType(BACnetEventTypeOutOfRange);
			CBACnetEventParameterOutOfRange outOfRange(&m_Value->_type.outOfRange);
			outOfRange.Set(TimeDelay, LowDiffLimit, HighDiffLimit, Deadband);
		}
	}
	void SetBufferReady(UINT TimeDelay, UINT NotificationThreshold, UINT PreviousNotificationCount)
	{
		if (m_Value)
		{
			ChangeType(BACnetEventTypeBufferReady);
			CBACnetEventParameterBufferReady bufferReady(&m_Value->_type.bufferReady);
			bufferReady.Set(TimeDelay, NotificationThreshold, PreviousNotificationCount);
		}
	}

	BACnetEventType GetEventType()
	{
		if (m_Value == NULL)
			return (BACnetEventType)0;
		return m_Value->type;
	}
	CBACnetEventParameterChangeOfBitString GetChangeOfBitString()
	{
		if (m_Value == NULL)
			return CBACnetEventParameterChangeOfBitString();

		if (m_Value->type != BACnetEventTypeChangeOfBitString)
			ChangeType(BACnetEventTypeChangeOfBitString);

		return CBACnetEventParameterChangeOfBitString(&m_Value->_type.changeOfBitString);
	}
	CBACnetEventParameterChangeOfState GetChangeOfState()
	{
		if (m_Value == NULL)
			return CBACnetEventParameterChangeOfState();

		if (m_Value->type != BACnetEventTypeChangeOfState)
			ChangeType(BACnetEventTypeChangeOfState);

		return CBACnetEventParameterChangeOfState(&m_Value->_type.changeOfState);
	}
	CBACnetEventParameterChangeOfValue GetChangeOfValue()
	{
		if (m_Value == NULL)
			return CBACnetEventParameterChangeOfValue();

		if (m_Value->type != BACnetEventTypeChangeOfValue)
			ChangeType(BACnetEventTypeChangeOfValue);

		return CBACnetEventParameterChangeOfValue(&m_Value->_type.changeOfValue);
	}
	CBACnetEventParameterCommandFailure GetCommandFailure()
	{
		if (m_Value == NULL)
			return CBACnetEventParameterCommandFailure();

		if (m_Value->type != BACnetEventTypeCommandFailure)
			ChangeType(BACnetEventTypeCommandFailure);

		return CBACnetEventParameterCommandFailure(&m_Value->_type.commandFailure);
	}
	CBACnetEventParameterFloatingLimit GetFloatingLimit()
	{
		if (m_Value == NULL)
			return CBACnetEventParameterFloatingLimit();

		if (m_Value->type != BACnetEventTypeFloatingLimit)
			ChangeType(BACnetEventTypeFloatingLimit);

		return CBACnetEventParameterFloatingLimit(&m_Value->_type.floatingLimit);
	}
	CBACnetEventParameterOutOfRange GetOutOfRange()
	{
		if (m_Value == NULL)
			return CBACnetEventParameterOutOfRange();

		if (m_Value->type != BACnetEventTypeOutOfRange)
			ChangeType(BACnetEventTypeOutOfRange);

		return CBACnetEventParameterOutOfRange(&m_Value->_type.outOfRange);
	}
	CBACnetEventParameterBufferReady GetBufferReady()
	{
		if (m_Value == NULL)
			return CBACnetEventParameterBufferReady();

		if (m_Value->type != BACnetEventTypeBufferReady)
			ChangeType(BACnetEventTypeBufferReady);

		return CBACnetEventParameterBufferReady(&m_Value->_type.bufferReady);
	}
private:
	void ChangeType(BACnetEventType type)
	{
		if (m_Value->type != type)
		{
			bacnapi_ClearBACnetEventParameter(m_Value);
		}
		m_Value->type = type;
	}
	BACnetEventParameter* m_Value;
};

typedef enum BACnetBoolean
{
	BACnetBooleanFalse = 0,
	BACnetBooleanTrue = 0xFF
};

class CBACnetAny : public CBACnetAnyBase
{
public:
	CBACnetAny()
	{
	}
	~CBACnetAny(){}
	CBACnetAny(BACnetDataType *pSrc)
	{
		m_Attached = 1;
		m_Wrapper = pSrc;
	}
	CBACnetAny(BACnetPVNull src)
	{
		init();
	}
	CBACnetAny(UINT src)
	{
		init();
		bacnapi_access_unsignedInteger(&src, m_Wrapper, 0);
	}
	CBACnetAny(float src)
	{
		init();
		bacnapi_access_real(&src, m_Wrapper, 0);
	}
	CBACnetAny(char* src)
	{
		init();
		bacnapi_access_string(&src, m_Wrapper, 0);
	}
	CBACnetAny(BACnetTimeValue *src)
	{
		init();
		bacnapi_access_timeValue(src, m_Wrapper, 0);
	}
	CBACnetAny(BACnetSpecialEvent *src)
	{
		init();
		bacnapi_access_specialEvent(src, m_Wrapper, 0);
	}
	CBACnetAny(BACnetCalendarEntry *src)
	{
		init();
		bacnapi_access_calendarEntry(src, m_Wrapper, 0);
	}
	CBACnetAny(BACnetNotifyType src)
	{
		init();
		bacnapi_access_enumerated((unsigned int*)&src, m_Wrapper, 0);
	}
	CBACnetAny(BACnetEventType src)
	{
		init();
		bacnapi_access_enumerated((unsigned int*)&src, m_Wrapper, 0);
	}
	CBACnetAny(BACnetBinaryPV src)
	{
		init();
		bacnapi_access_enumerated((unsigned int*)&src, m_Wrapper, 0);
	}
	CBACnetAny(BACnetBoolean src)
	{
		init();
		bacnapi_access_boolean((BYTE*)&src, m_Wrapper, 0);
	}
	CBACnetAny(BACnetBitString *src)
	{
		init();
		bacnapi_access_bitString(src, m_Wrapper, 0);
	}
	CBACnetAny(BACnetOctet *src)
	{
		init();
		bacnapi_access_octets(src, m_Wrapper, 0);
	}
	CBACnetAny(BACnetDataType* pSrc, int fCopy)
	{
		if (fCopy)
			Copy(pSrc);
		else
		{
			m_Attached = 1;
			m_Wrapper = pSrc;
		}
	}
	CBACnetAnyBase& operator =(UINT src)
	{
		bacnapi_access_unsignedInteger(&src, m_Wrapper, 0);
		return *this;
	}
	CBACnetAnyBase& operator =(BACnetPVNull src)
	{
		bacnapi_initialize_data(m_Wrapper, bacnetDT_Null);
		return *this;
	}
	CBACnetAnyBase& operator =(BACnetEventType src)
	{
		bacnapi_access_enumerated((UINT*)&src, m_Wrapper, 0);
		return *this;
	}
	CBACnetAnyBase& operator =(BACnetBoolean src)
	{
		bacnapi_access_boolean((BYTE*)&src, m_Wrapper, 0);
		return *this;
	}
	CBACnetAnyBase& operator =(BACnetNotifyType src)
	{
		bacnapi_access_enumerated((UINT*)&src, m_Wrapper, 0);
		return *this;
	}
	CBACnetAnyBase& operator =(int src)
	{
		bacnapi_access_signedInteger(&src, m_Wrapper, 0);
		return *this;
	}
	CBACnetAnyBase& operator =(float src)
	{
		bacnapi_access_real(&src, m_Wrapper, 0);
		return *this;
	}
	CBACnetAnyBase& operator =(char* src)
	{
		bacnapi_access_string(&src, m_Wrapper, 0);
		return *this;
	}

	//Extractors
	operator float(){return m_Wrapper->_type.Real;}
	operator int(){return m_Wrapper->_type.SignedInteger;}
	operator UINT(){return m_Wrapper->_type.UnsignedInteger;}
	operator const UINT*(){return &m_Wrapper->_type.UnsignedInteger;}
	operator const float*(){return &m_Wrapper->_type.Real;}
	operator const BACnetBitString*(){return &m_Wrapper->_type.BitString;}

	BACnetBinaryPV GetBinary()
	{
		ChangeType(bacnetDT_Enumerated);
		return (BACnetBinaryPV)m_Wrapper->_type.Enumerated;
	}
	BYTE GetBoolean()
	{
		ChangeType(bacnetDT_Boolean);
		return m_Wrapper->_type.Boolean;
	}
	float GetReal()
	{
		ChangeType(bacnetDT_Real);
		return m_Wrapper->_type.Real;
	}
	int GetSignedInteger()
	{
		ChangeType(bacnetDT_SignedInteger);
		return m_Wrapper->_type.SignedInteger;
	}
	UINT GetUnsignedInteger()
	{
		ChangeType(bacnetDT_UnsignedInteger);
		return m_Wrapper->_type.UnsignedInteger;
	}
	UINT GetEnumerated()
	{
		ChangeType(bacnetDT_Enumerated);
		return m_Wrapper->_type.Enumerated;
	}
	CBACnetDateTime GetDateTime()
	{
		ChangeType(bacnetDT_DateTime);
		return CBACnetDateTime((BACnetDateTime*)m_Wrapper->_type.dateTime);
	}
	CBACnetTransitionBits GetEventTransitionBits()
	{
		ChangeType(bacnetDT_BitString);
		return CBACnetTransitionBits(&m_Wrapper->_type.BitString);
	}
	CBACnetScale GetScale()
	{
		ChangeType(bacnetDT_Scale);
		return CBACnetScale((BACnetScale*)m_Wrapper->_type.scale);
	}
	CBACnetPrescale GetPrescale()
	{
		ChangeType(bacnetDT_Prescale);
		return CBACnetPrescale((BACnetPrescale*)m_Wrapper->_type.prescale);
	}
	CBACnetTimeValue GetTimeValue()
	{
		ChangeType(bacnetDT_TimeValue);
		return CBACnetTimeValue((BACnetTimeValue*)m_Wrapper->_type.timeValue);
	}
	CBACnetDailySchedule GetDailySchedule()
	{
		ChangeType(bacnetDT_DailySchedule);
		return CBACnetDailySchedule((BACnetDailySchedule*)m_Wrapper->_type.dailySchedule);
	}
	CSequenceOfPriority GetSequenceOfPriority()
	{
		ChangeType(bacnetDT_ArrayOfUnsigned);
		return CSequenceOfPriority((SequenceOfUnsigned*)m_Wrapper);
	}
	CBACnetLimitEnable GetLimitEnable()
	{
		ChangeType(bacnetDT_BitString);
		return CBACnetLimitEnable(&m_Wrapper->_type.BitString);
	}
	CBACnetActionList GetActionList()
	{
		ChangeType(bacnetDT_ActionList);
		return CBACnetActionList((BACnetActionList*)m_Wrapper->_type.actionList);
	}
	CSequenceOfBACnetActionList GetSequenceOfActionList()
	{
		ChangeType(bacnetDT_ArrayOfBACnetActionList);
		return CSequenceOfBACnetActionList((SequenceOfBACnetActionList*)m_Wrapper);
	}
	CBACnetAddressBinding GetAddressBinding()
	{
		ChangeType(bacnetDT_AddressBinding);
		return CBACnetAddressBinding((BACnetAddressBinding*)m_Wrapper->_type.addressBinding);
	}
	CSequenceOfBACnetAddressBinding GetSequenceOfAddressBinding()
	{
		ChangeType(bacnetDT_ArrayOfBACnetAddressBinding);
		return CSequenceOfBACnetAddressBinding((SequenceOfBACnetAddressBinding*)m_Wrapper);
	}
	CBACnetActionCommand GetActionCommand()
	{
		ChangeType(bacnetDT_ActionCommand);
		return CBACnetActionCommand((BACnetActionCommand*)m_Wrapper->_type.actionCommand);
	}
	CSequenceOfBACnetActionCommand GetSequenceOfActionCommand()
	{
		ChangeType(bacnetDT_ArrayOfBACnetActionCommand);
		return CSequenceOfBACnetActionCommand((SequenceOfBACnetActionCommand*)m_Wrapper);
	}
	CBACnetRecipient GetRecipient()
	{
		ChangeType(bacnetDT_Recipient);
		return CBACnetRecipient((BACnetRecipient*)m_Wrapper->_type.recipient);
	}
	CSequenceOfBACnetRecipient GetSequenceOfRecipient()
	{
		ChangeType(bacnetDT_ArrayOfBACnetRecipient);
		return CSequenceOfBACnetRecipient((SequenceOfBACnetRecipient*)m_Wrapper);
	}
	CBACnetDestination GetDestination()
	{
		ChangeType(bacnetDT_Destination);
		return CBACnetDestination((BACnetDestination*)m_Wrapper->_type.destination);
	}
	CSequenceOfBACnetDestination GetSequenceOfDestination()
	{
		ChangeType(bacnetDT_ArrayOfBACnetDestination);
		return CSequenceOfBACnetDestination((SequenceOfBACnetDestination*)m_Wrapper);
	}
	CSequenceOfBACnetCalendarEntry GetSequenceOfCalendarEntry()
	{
		ChangeType(bacnetDT_ArrayOfBACnetCalendarEntry);
		return CSequenceOfBACnetCalendarEntry((SequenceOfBACnetCalendarEntry*)m_Wrapper);
	}
	CSequenceOfBACnetSpecialEvent GetSequenceOfSpecialEvent()
	{
		ChangeType(bacnetDT_ArrayOfSpecialEvent);
		return CSequenceOfBACnetSpecialEvent((SequenceOfSpecialEvent*)m_Wrapper);
	}
	CBACnetPriorityArray GetPriorityArray()
	{
		ChangeType(bacnetDT_PriorityArray);
		return CBACnetPriorityArray((BACnetPriorityArray*)m_Wrapper);
	}
	CBACnetPriorityValue GetPriorityValue()
	{
		ChangeType(bacnetDT_PriorityValue);
		return CBACnetPriorityValue((BACnetPriorityValue*)m_Wrapper->_type.priorityValue);
	}
	CSequenceOfBACnetObjectIdentifier GetSequenceOfBACnetObjectIdentifier()
	{
		ChangeType(bacnetDT_ArrayOfBACnetObjectIdentifier);
		return CSequenceOfBACnetObjectIdentifier((SequenceOfBACnetObjectIdentifier*)m_Wrapper);
	}
	CBACnetSetpointReference GetSetpointReference()
	{
//		ChangeType(bacnetDT_SetpointReference);
		return CBACnetSetpointReference((BACnetSetpointReference*)m_Wrapper->_type.dailySchedule);
	}
	CSequenceOfBACnetTimeValue GetDailyScheduleSequenceOfTimeValue()
	{
		ChangeType(bacnetDT_DailySchedule);
		BACnetDailySchedule *dailySchedule = (BACnetDailySchedule*)m_Wrapper->_type.dailySchedule;
		return CSequenceOfBACnetTimeValue((SequenceOfBACnetTimeValue*)&dailySchedule->daySchedule);
	}
	SequenceOfReadAccessSpecification* GetSequenceOfReadAccessSpecification()
	{
		ChangeType(bacnetDT_SequenceOfReadAccessSpecification);
		return (SequenceOfReadAccessSpecification*)m_Wrapper->_type.sequenceOfReadAccessSpecification;
	}
	CSequenceOfBACnetDeviceObjectPropertyReference GetSequenceOfDeviceObjectPropertyReference()
	{
		ChangeType(bacnetDT_ArrayOfBACnetObjectPropertyReference);
		return CSequenceOfBACnetDeviceObjectPropertyReference((SequenceOfBACnetTimeValue*)m_Wrapper);
	}
	CBACnetObjectPropertyReference GetObjectPropertyReference()
	{
		ChangeType(bacnetDT_ObjectPropertyReference);
		return CBACnetObjectPropertyReference((BACnetObjectPropertyReference*)m_Wrapper->_type.objectPropertyReference);
	}
	CBACnetDeviceObjectPropertyReference GetDeviceObjectPropertyReference()
	{
		ChangeType(bacnetDT_DeviceObjectPropertyReference);
		return CBACnetDeviceObjectPropertyReference((BACnetDeviceObjectPropertyReference*)m_Wrapper->_type.objectPropertyReference);
	}
	CBACnetEventParameter GetEventParameters()
	{
		ChangeType(bacnetDT_EventParameter);
		return CBACnetEventParameter((BACnetEventParameter*)m_Wrapper->_type.eventParameter);
	}
	CBACnetBitString GetBACnetBitString()
	{
		ChangeType(bacnetDT_BitString);
		return CBACnetBitString(&m_Wrapper->_type.BitString);
	}
	CBACnetObjectIdentifier GetBACnetObjectIdentifier()
	{
		ChangeType(bacnetDT_ObjectIdentifier);
		return CBACnetObjectIdentifier(&m_Wrapper->_type.ObjectIdentifier);
	}
	CBACnetCalendarEntry GetBACnetCalendarEntry()
	{
		ChangeType(bacnetDT_CalendarEntry);
		return CBACnetCalendarEntry((BACnetCalendarEntry*)m_Wrapper->_type.calendarEntry);
	}
	CBACnetStatusFlags GetStatusFlags()
	{
		ChangeType(bacnetDT_BitString);
		return CBACnetStatusFlags(&m_Wrapper->_type.BitString);
	}
	void SetEnumerated(UINT data)
	{
		ChangeType(bacnetDT_Enumerated);
		bacnapi_access_enumerated((UINT*)&data, m_Wrapper, 0);
	}
	void SetBoolean(BYTE data)
	{
		ChangeType(bacnetDT_Boolean);
		bacnapi_access_boolean((BYTE*)&data, m_Wrapper, 0);
	}
	void SetOctets(BYTE *data, int size)
	{
		BACnetOctet octets;

		octets.contents = data;
		octets.len = size;

		ChangeType(bacnetDT_OctetString);
		bacnapi_access_octets(&octets, m_Wrapper, 0);
	}
};

class CBACnetServiceReadProperty
{
public:
	CBACnetServiceReadProperty()
	{
		bacnapi_read_property_initialize(service, request, ack, error);
		request.propertyArrayIndex = -1;
	}
	~CBACnetServiceReadProperty()
	{
		bacnapi_read_property_destroy(service, request, ack, error);
	}

	//Execution routines
	BYTE Execute(){bacnapi_ClearReadPropertyAck(&ack); return read_property(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, BACnetObjectType ObjectType, int Instance, BACnetPropertyIdentifier PropertyIdentifier, int ArrayIndex = -1)
	{
		bacnapi_ClearReadPropertyAck(&ack);
		service.device.device_id = DeviceID;
		request.objectIdentifier.objectType = ObjectType;
		request.objectIdentifier.instance = Instance;
		request.propertyIdentifier = PropertyIdentifier;
		request.propertyArrayIndex = ArrayIndex;
		return read_property(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetObjectIdentifierObjectType(BACnetObjectType data){request.objectIdentifier.objectType = data;}
	void SetObjectIdentifierInstance(int data){request.objectIdentifier.instance = data;}
	void SetPropertyIdentifier(BACnetPropertyIdentifier data){request.propertyIdentifier = data;}
	void SetPropertyArrayIndex(int data){request.propertyArrayIndex = data;}
	void SetDeviceID(int data){service.device.device_id = data;}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType(){return request.objectIdentifier.objectType;}
	int GetObjectIdentifierInstance(int data){return request.objectIdentifier.instance;}
	BACnetPropertyIdentifier GetPropertyIdentifier(){return request.propertyIdentifier;}
	int GetPropertyArrayIndex(){return request.propertyArrayIndex;}
	int GetDeviceID(){return service.device.device_id;}

	CBACnetAny GetPropertyValue(){return CBACnetAny(&ack.propertyValue, 0x0);}
	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.readProperty.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.readProperty.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}

private:
	BACnetServiceReadPropertyRequest request;
	BACnetServiceReadPropertyAck ack;
	BACnetError error;
	BACnetServiceReadProperty service;
};

class CBACnetReadAccessPropertyValue
{
public:
	CBACnetReadAccessPropertyValue(){}
	CBACnetReadAccessPropertyValue(BACnetReadAccessPropertyValue *pSrc)
	{
		m_Value = pSrc;
	}
	//Set Properties
	void SetPropertyIdentifier(BACnetPropertyIdentifier data)
	{
		if (m_Value)
			m_Value->propertyIdentifier = data;
	}
	void SetPropertyArrayIndex(int data)
	{
		if (m_Value)
			m_Value->propertyArrayIndex = data;
	}
	void SetErrorClass(BACnetErrorClass data)
	{
		if (m_Value)
		{
			if (m_Value->readResultType == BACnetReadAccessReadResultTypePropertyValue)
				bacnapi_ClearBACnetDataType(&m_Value->readResult.propertyValue);
			m_Value->readResultType = BACnetReadAccessReadResultTypePropertyAccessError;
			m_Value->readResult.propertyAccessError.errorClass = data;
		}
	}
	void SetErrorCode(BACnetErrorCode data)
	{
		if (m_Value)
		{
			if (m_Value->readResultType == BACnetReadAccessReadResultTypePropertyValue)
				bacnapi_ClearBACnetDataType(&m_Value->readResult.propertyValue);
			m_Value->readResultType = BACnetReadAccessReadResultTypePropertyAccessError;
			m_Value->readResult.propertyAccessError.errorCode = data;
		}
	}

	//Get Properties
	BACnetPropertyIdentifier GetPropertyIdentifier()
	{
		if (m_Value == NULL)
			return (BACnetPropertyIdentifier)0;
		return m_Value->propertyIdentifier;
	}
	int GetPropertyArrayIndex()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->propertyArrayIndex;
	}
	BACnetReadAccessReadResultType GetReadResultType()
	{
		if (m_Value == NULL)
			return BACnetReadAccessReadResultType(0);
		return m_Value->readResultType;
	}
	BACnetDataType* GetPropertyValueHandle()
	{
		m_Value->readResultType = BACnetReadAccessReadResultTypePropertyValue;
		return &m_Value->readResult.propertyValue;
	}
	CBACnetAny GetPropertyValue(){return CBACnetAny(&m_Value->readResult.propertyValue, 0x0);}

	//Error Codes
	BACnetErrorCode GetErrorCode()
	{
		if (GetReadResultType() == BACnetReadAccessReadResultTypePropertyValue)
			return BACnetErrorCodeNone;
		return m_Value->readResult.propertyAccessError.errorCode;
	}
	BACnetErrorClass GetErrorClass()
	{
		if (GetReadResultType() == BACnetReadAccessReadResultTypePropertyValue)
			return BACnetErrorClassNone;
		return m_Value->readResult.propertyAccessError.errorClass;
	}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}

private:
	BACnetReadAccessPropertyValue* m_Value;
};

class CBACnetPropertyValue
{
public:
	CBACnetPropertyValue(){}
	CBACnetPropertyValue(BACnetPropertyValue *pSrc)
	{
		m_Value = pSrc;
	}
	//Set Properties
	void SetPropertyIdentifier(BACnetPropertyIdentifier data)
	{
		if (m_Value)
			m_Value->propertyIdentifier = data;
	}
	void SetPropertyArrayIndex(int data)
	{
		if (m_Value)
			m_Value->propertyArrayIndex = data;
	}
	void SetPriority(UINT data)
	{
		if (m_Value)
			m_Value->priority = data;
	}

	//Get Properties
	BACnetPropertyIdentifier GetPropertyIdentifier()
	{
		if (m_Value == NULL)
			return (BACnetPropertyIdentifier)0;
		return m_Value->propertyIdentifier;
	}
	int GetPropertyArrayIndex()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->propertyArrayIndex;
	}
	UINT GetPriority()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->priority;
	}
	CBACnetAny GetPropertyValue(){return CBACnetAny(&m_Value->value);}

private:
	BACnetPropertyValue* m_Value;
};

class CBACnetRecipientProcess
{
public:
	CBACnetRecipientProcess(){m_Value = NULL;}
	CBACnetRecipientProcess(BACnetRecipientProcess *pSrc)
	{
		m_Value = pSrc;
		if (m_Value)
			recipient = CBACnetRecipient(&m_Value->recipient);
	}
	//Set Properties
	int SetProcessIdentifier(UINT data)
	{
		if (!m_Value)
			return -1;
		m_Value->processIdentifier = data;
		return 0;
	}

	//Get Properties
	UINT GetProcessIdentifier()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->processIdentifier;
	}
	CBACnetRecipient& GetRecipient()
	{
		return recipient;
	}

private:
	CBACnetRecipient recipient;
	BACnetRecipientProcess* m_Value;
};

class CBACnetServiceGetAlarmSummaryAckElement
{
public:
	CBACnetServiceGetAlarmSummaryAckElement(){}
	CBACnetServiceGetAlarmSummaryAckElement(BACnetServiceGetAlarmSummaryAckElement *pSrc)
	{
		m_Value = pSrc;
		if (m_Value)
			acknowledgedTransitions = CBACnetTransitionBits(&m_Value->acknowledgedTransitions);
	}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType()
	{
		if (m_Value == NULL)
			return BACnetObjectTypeNone;
		return m_Value->objectIdentifier.objectType;
	}
	int GetObjectIdentifierInstance()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->objectIdentifier.instance;
	}
	BACnetEventState GetAlarmState()
	{
		if (m_Value == NULL)
			return (BACnetEventState)0;
		return m_Value->alarmState;
	}

	CBACnetTransitionBits& GetAcknowledgedTransitions(){return acknowledgedTransitions;}
private:
	CBACnetTransitionBits acknowledgedTransitions;
	BACnetServiceGetAlarmSummaryAckElement* m_Value;
};

class CBACnetEventSummaryElement
{
public:
	CBACnetEventSummaryElement(){}
	CBACnetEventSummaryElement(BACnetEventSummaryElement *pSrc)
	{
		m_Value = pSrc;
		if (m_Value)
		{
			acknowledgedTransitions = CBACnetTransitionBits(&m_Value->acknowledgedTransitions);
			eventEnable = CBACnetTransitionBits(&m_Value->eventEnable);
			eventTimeStamps = CBACnetEventTimeStamps(&m_Value->eventTimeStamps);
			eventPriorities = CSequenceOfPriority(&m_Value->eventPriorities);
		}
	}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType()
	{
		if (m_Value == NULL)
			return BACnetObjectTypeNone;
		return m_Value->objectIdentifier.objectType;
	}
	int GetObjectIdentifierInstance()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->objectIdentifier.instance;
	}
	BACnetEventState GetEventState()
	{
		if (m_Value == NULL)
			return (BACnetEventState)0;
		return m_Value->eventState;
	}
	BACnetNotifyType GetNotifyType()
	{
		if (m_Value == NULL)
			return (BACnetNotifyType)0;
		return m_Value->notifyType;
	}

	CBACnetTransitionBits& GetAcknowledgedTransitions(){return acknowledgedTransitions;}
	CBACnetTransitionBits& GetEventEnable(){return eventEnable;}
	CBACnetEventTimeStamps& GetEventTimeStamps(){return eventTimeStamps;}
	CSequenceOfPriority& GetEventPriorities(){return eventPriorities;}
private:
	CBACnetTransitionBits acknowledgedTransitions;
	CBACnetTransitionBits eventEnable;
	CBACnetEventTimeStamps eventTimeStamps;
	CSequenceOfPriority eventPriorities;
	BACnetEventSummaryElement* m_Value;
};

class CBACnetServiceGetEnrolmentSummaryAckElement
{
public:
	CBACnetServiceGetEnrolmentSummaryAckElement(){}
	CBACnetServiceGetEnrolmentSummaryAckElement(BACnetServiceGetEnrolmentSummaryAckElement *pSrc)
	{
		m_Value = pSrc;
	}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType()
	{
		if (m_Value == NULL)
			return BACnetObjectTypeNone;
		return m_Value->objectIdentifier.objectType;
	}
	int GetObjectIdentifierInstance()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->objectIdentifier.instance;
	}
	BACnetEventState GetEventState()
	{
		if (m_Value == NULL)
			return (BACnetEventState)0;
		return m_Value->eventState;
	}
	BACnetEventType GetBACnetEventType()
	{
		if (m_Value == NULL)
			return (BACnetEventType)0;
		return m_Value->eventType;
	}
	UINT GetPriority()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->priority;
	}
	UINT GetNotificationClass()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->notificationClass;
	}

	CBACnetTransitionBits& GetAcknowledgedTransitions(){return acknowledgedTransitions;}
private:
	CBACnetTransitionBits acknowledgedTransitions;
	BACnetServiceGetEnrolmentSummaryAckElement* m_Value;
};

class CBACnetLogRecord
{
public:
	CBACnetLogRecord(){}
	CBACnetLogRecord(BACnetLogRecord *pSrc)
	{
		m_Value = pSrc;
	}

	//Get Properties
	BYTE GetBooleanValue()
	{
		if (m_Value == NULL && m_Value->choice != BACnetLogDatumTypeBooleanValue)
			return 0;
		return m_Value->_choice.booleanValue;
	}
	BACnetLogStatus GetLogStatus()
	{
		return m_Value->_choice.logStatus;
	}
	float GetRealValue()
	{
		if (m_Value == NULL && m_Value->choice != BACnetLogDatumTypeRealValue)
			return 0;
		return m_Value->_choice.realValue;
	}
	UINT GetEnumValue()
	{
		if (m_Value == NULL && m_Value->choice != BACnetLogDatumTypeEnumValue)
			return 0;
		return m_Value->_choice.enumValue;
	}
	UINT GetUnsignedValue()
	{
		if (m_Value == NULL && m_Value->choice != BACnetLogDatumTypeUnsignedValue)
			return 0;
		return m_Value->_choice.unsignedValue;
	}
	int GetSignedValue()
	{
		if (m_Value == NULL && m_Value->choice != BACnetLogDatumTypeSignedValue)
			return 0;
		return m_Value->_choice.signedValue;
	}
	UINT GetNullValue()
	{
		if (m_Value == NULL && m_Value->choice != BACnetLogDatumTypeNullValue)
			return 0;
		return m_Value->_choice.nullValue;
	}
	float GetTimeChange()
	{
		if (m_Value == NULL && m_Value->choice != BACnetLogDatumTypeTimeChange)
			return 0;
		return m_Value->_choice.timeChange;
	}

	BACnetErrorCode GetFailureErrorCode()
	{
		return m_Value->_choice.failure.errorCode;
	}
	BACnetErrorClass GetFailureErrorClass()
	{
		return m_Value->_choice.failure.errorClass;
	}
	BACnetLogDatumType GetChoice(){return m_Value->choice;}
	int GetTimeStamp(timeb *value)
	{
		return bacnetapi_get_timeb(value, &m_Value->timestamp);
	}
	CBACnetAny GetAnyValue()
	{
		if (m_Value == NULL && m_Value->choice != BACnetLogDatumTypeAnyValue)
			CBACnetAny();
		return CBACnetAny(&m_Value->_choice.anyValue, 0x0);
	}
	CBACnetBitString GetBitStringValue()
	{
		if (m_Value == NULL && m_Value->choice != BACnetLogDatumTypeBitstringValue)
			CBACnetBitString();
		return CBACnetBitString(&m_Value->_choice.bitstringValue);
	}
	CBACnetStatusFlags GetStatusFlags(){return CBACnetStatusFlags(&m_Value->statusFlags);}
private:
	BACnetLogRecord* m_Value;
};

class CPropertyReference
{
public:
	CPropertyReference(){}
	CPropertyReference(BACnetPropertyReference *pSrc){m_Value = pSrc;}
	//Set Properties
	void SetPropertyIdentifier(BACnetPropertyIdentifier data)
	{
		if (m_Value)
			m_Value->propertyIdentifier = data;
	}
	void SetPropertyArrayIndex(int data)
	{
		if (m_Value)
			m_Value->propertyArrayIndex = data;
	}

	//Get Properties
	BACnetPropertyIdentifier GetPropertyIdentifier()
	{
		if (m_Value == NULL)
			return (BACnetPropertyIdentifier)0;
		return m_Value->propertyIdentifier;
	}
	int GetPropertyArrayIndex()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->propertyArrayIndex;
	}

private:
	BACnetPropertyReference* m_Value;
};

class CSequenceOfBACnetPropertyValue : CBACnetArray
{
public:
	CSequenceOfBACnetPropertyValue(){}
	CSequenceOfBACnetPropertyValue(SequenceOfBACnetPropertyValue *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfBACnetPropertyValue(){}
	//List access
	CBACnetPropertyValue Add(BACnetPropertyIdentifier propertyIdentifier, CBACnetAny value = BACnetNull, UINT Priority = 0, int propertyArrayIndex = -1)
	{
		BACnetPropertyValue *propertyValue = NULL;
		BACnetDataType* new_element = AddElement(bacnetDT_SequenceOfBACnetPropertyValue, bacnetDT_PropertyValue);

		propertyValue = (BACnetPropertyValue*)new_element->_type.propertyValue;
		propertyValue->propertyIdentifier = propertyIdentifier;
		propertyValue->priority = Priority;
		if (bacnapi_access_BACnetData(&propertyValue->value, value.operator BACnetDataType *(), -1, 1))
			return CBACnetPropertyValue();

		return CBACnetPropertyValue(propertyValue);
	}
	CBACnetPropertyValue GetItem(int index)
	{
		BACnetDataType* element;
		BACnetPropertyValue *propertyValue = NULL;

		if (m_Array->type == bacnetDT_PropertyValue)
		{
			return CBACnetPropertyValue((BACnetPropertyValue*)m_Array->_type.propertyValue);
		}
		else
		{
			element = GetElement(bacnetDT_SequenceOfBACnetPropertyValue, index);
			if (element)
				propertyValue = (BACnetPropertyValue*)element->_type.propertyValue;

			return CBACnetPropertyValue(propertyValue);
		}
	}
	int GetSize()
	{
		if (m_Array->type == bacnetDT_PropertyValue)
			return 1;
		else if (m_Array->type == bacnetDT_Null)
			return 0;
		else
			return GetArraySize(bacnetDT_SequenceOfBACnetPropertyValue);
	}
};

class CSequenceOfAlarmSummaryAck : CBACnetArray
{
public:
	CSequenceOfAlarmSummaryAck(){}
	CSequenceOfAlarmSummaryAck(SequenceOfAlarmSummaryAck *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfAlarmSummaryAck(){}
	//List access
	CBACnetServiceGetAlarmSummaryAckElement GetItem(int index)
	{
		BACnetDataType* element;
		BACnetServiceGetAlarmSummaryAckElement *alarmSummaryAckElement = NULL;

		element = GetElement(bacnetDT_SequenceOfAlarmSummaryAckElement, index);
		if (element)
			alarmSummaryAckElement = (BACnetServiceGetAlarmSummaryAckElement*)element->_type.alarmSummaryAckElement;

		return CBACnetServiceGetAlarmSummaryAckElement(alarmSummaryAckElement);
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_SequenceOfAlarmSummaryAckElement);
	}
};

class CSequenceOfEnrolmentSummaryAck : CBACnetArray
{
public:
	CSequenceOfEnrolmentSummaryAck(){}
	CSequenceOfEnrolmentSummaryAck(SequenceOfEnrolmentSummaryAck *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfEnrolmentSummaryAck(){}
	//List access
	CBACnetServiceGetEnrolmentSummaryAckElement GetItem(int index)
	{
		BACnetDataType* element;
		BACnetServiceGetEnrolmentSummaryAckElement *enrolmentSummaryAckElement = NULL;


		if (m_Array->type == bacnetDT_EnrolmentSummaryAckElement)
		{
			return CBACnetServiceGetEnrolmentSummaryAckElement((BACnetServiceGetEnrolmentSummaryAckElement*)m_Array->_type.enrolmentSummaryAckElement);
		}
		else
		{
			element = GetElement(bacnetDT_SequenceOfEnrolmentSummaryAckElement, index);
			if (element)
				enrolmentSummaryAckElement = (BACnetServiceGetEnrolmentSummaryAckElement*)element->_type.enrolmentSummaryAckElement;

			return CBACnetServiceGetEnrolmentSummaryAckElement(enrolmentSummaryAckElement);
		}
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_SequenceOfEnrolmentSummaryAckElement);
	}
};

class CSequenceOfEventSummaryAck : CBACnetArray
{
public:
	CSequenceOfEventSummaryAck(){}
	CSequenceOfEventSummaryAck(SequenceOfEventSummaries *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfEventSummaryAck(){}
	//List access
	CBACnetEventSummaryElement GetItem(int index)
	{
		BACnetDataType* element;
		BACnetEventSummaryElement *eventSummaryAckElement = NULL;


		if (m_Array->type == bacnetDT_EnrolmentSummaryAckElement)
		{
			return CBACnetEventSummaryElement((BACnetEventSummaryElement*)m_Array->_type.eventSummaryElement);
		}
		else
		{
			element = GetElement(bacnetDT_SequenceOfEventSummaryElement, index);
			if (element)
				eventSummaryAckElement = (BACnetEventSummaryElement*)element->_type.eventSummaryElement;

			return CBACnetEventSummaryElement(eventSummaryAckElement);
		}
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_SequenceOfEventSummaryElement);
	}
};

class CArrayOfBACnetLogRecord : CBACnetArray
{
public:
	CArrayOfBACnetLogRecord(){}
	CArrayOfBACnetLogRecord(BACnetDataType *pSrc){m_Array = pSrc;}
	virtual ~CArrayOfBACnetLogRecord(){}
	//Array Access
	CBACnetLogRecord GetItem(int index)
	{
		BACnetDataType* element;
		BACnetLogRecord *logRecord = NULL;

		element = GetElement(bacnetDT_ArrayOfBACnetLogRecord, index);
		if (element)
			logRecord = (BACnetLogRecord*)element->_type.logRecord;

		return CBACnetLogRecord(logRecord);
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_ArrayOfBACnetLogRecord);
	}
};

class CSequenceOfBACnetAny : CBACnetArray
{
public:
	CSequenceOfBACnetAny(){}
	CSequenceOfBACnetAny(BACnetDataType *pSrc){m_Array = pSrc;}
	virtual ~CSequenceOfBACnetAny(){}
	//List access
	CBACnetAny Add(CBACnetAny value = BACnetNull)
	{
		BACnetDataType* new_element = AddElement(bacnetDT_SequenceOfElements, bacnetDT_Null);

		if (bacnapi_access_BACnetData(new_element, value.operator BACnetDataType *(), -1, 1))
			return CBACnetAny();

		return CBACnetAny(new_element);
	}
	CBACnetAny GetItem(int index)
	{
		return CBACnetAny(GetElement(bacnetDT_SequenceOfElements, index));
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_SequenceOfElements);
	}
};

class CBACnetNotificationParameterChangeOfBitString
{
public:
	CBACnetNotificationParameterChangeOfBitString(){}
	CBACnetNotificationParameterChangeOfBitString(_BACnetNotificationParameterChangeOfBitString *pSrc)
	{
		m_Value = pSrc;
		if (m_Value)
		{
			statusFlags = CBACnetStatusFlags(&m_Value->statusFlags);
			referencedBitstring = CBACnetBitString(&m_Value->referencedBitstring);
		}
	}
	CBACnetStatusFlags& GetStatusFlags()
	{
		return statusFlags;
	}
	CBACnetBitString& GetReferencedBitstring()
	{
		return referencedBitstring;
	}

private:
	CBACnetStatusFlags statusFlags;
	CBACnetBitString referencedBitstring;
	_BACnetNotificationParameterChangeOfBitString* m_Value;
};

class CBACnetNotificationParameterChangeOfState
{
public:
	CBACnetNotificationParameterChangeOfState(){}
	CBACnetNotificationParameterChangeOfState(_BACnetNotificationParameterChangeOfState *pSrc)
	{
		m_Value = pSrc;
		if (m_Value)
		{
			statusFlags = CBACnetStatusFlags(&m_Value->statusFlags);
			newState = CBACnetPropertyStates(&m_Value->newState);
		}
	}
	CBACnetStatusFlags& GetStatusFlags()
	{
		return statusFlags;
	}
	CBACnetPropertyStates& GetNewState()
	{
		return newState;
	}

private:
	CBACnetStatusFlags statusFlags;
	CBACnetPropertyStates newState;
	_BACnetNotificationParameterChangeOfState* m_Value;
};

class CBACnetNotificationParameterChangeOfValue
{
public:
	CBACnetNotificationParameterChangeOfValue(){}
	CBACnetNotificationParameterChangeOfValue(_BACnetNotificationParameterChangeOfValue *pSrc)
	{
		m_Value = pSrc;
		if (m_Value)
			statusFlags = CBACnetStatusFlags(&m_Value->statusFlags);
	}
	CBACnetStatusFlags& GetStatusFlags()
	{
		return statusFlags;
	}
	BACnetCovCriteriaChoice GetNewValue()
	{
		return m_Value->newValue;
	}
	CBACnetBitString& GetChangedBits()
	{
		changedBits = CBACnetBitString(&m_Value->_newValue.changedBits);
		return changedBits;
	}
	float GetChangedValue()
	{
		return m_Value->_newValue.changedValue;
	}

private:
	CBACnetStatusFlags statusFlags;
	CBACnetBitString changedBits;
	_BACnetNotificationParameterChangeOfValue* m_Value;
};

class CBACnetNotificationParameterCommandFailure
{
public:
	CBACnetNotificationParameterCommandFailure(){}
	CBACnetNotificationParameterCommandFailure(_BACnetNotificationParameterCommandFailure *pSrc)
	{
		m_Value = pSrc;
	}
	CBACnetStatusFlags GetStatusFlags()
	{
		return CBACnetStatusFlags(&m_Value->statusFlags);
	}
	CBACnetAny GetCommandValue()
	{
		return CBACnetAny(&m_Value->commandValue);
	}
	CBACnetAny GetFeedbackValue()
	{
		return CBACnetAny(&m_Value->feedbackValue);
	}

private:
	_BACnetNotificationParameterCommandFailure* m_Value;
};

class CBACnetNotificationParameterFloatingLimit
{
public:
	CBACnetNotificationParameterFloatingLimit(){}
	CBACnetNotificationParameterFloatingLimit(_BACnetNotificationParameterFloatingLimit *pSrc)
	{
		m_Value = pSrc;
		if (m_Value)
			statusFlags = CBACnetStatusFlags(&m_Value->statusFlags);
	}
	CBACnetStatusFlags& GetStatusFlags()
	{
		return statusFlags;
	}
	float GetReferenceValue()
	{
		return m_Value->referenceValue;
	}
	float GetSetpointValue()
	{
		return m_Value->setpointValue;
	}
	float GetErrorLimit()
	{
		return m_Value->errorLimit;
	}

private:
	CBACnetStatusFlags statusFlags;
	_BACnetNotificationParameterFloatingLimit* m_Value;
};

class CBACnetNotificationParameterOutOfRange
{
public:
	CBACnetNotificationParameterOutOfRange(){}
	CBACnetNotificationParameterOutOfRange(_BACnetNotificationParameterOutOfRange *pSrc)
	{
		m_Value = pSrc;
		if (m_Value)
			statusFlags = CBACnetStatusFlags(&m_Value->statusFlags);
	}
	CBACnetStatusFlags& GetStatusFlags()
	{
		return statusFlags;
	}
	float GetExceedingValue()
	{
		return m_Value->exceedingValue;
	}
	float GetDeadband()
	{
		return m_Value->deadband;
	}
	float GetExceedingLimit()
	{
		return m_Value->exceedingLimit;
	}

private:
	CBACnetStatusFlags statusFlags;
	_BACnetNotificationParameterOutOfRange* m_Value;
};

class CBACnetNotificationParameterBufferReady
{
public:
	CBACnetNotificationParameterBufferReady(){}
	CBACnetNotificationParameterBufferReady(_BACnetNotificationParameterBufferReady *pSrc)
	{
		m_Value = pSrc;
	}

	//Get Properties
	BACnetObjectType GetBufferObjectType()
	{
		if (m_Value == NULL)
			return BACnetObjectTypeNone;
		return m_Value->bufferObject.objectType;
	}
	int GetBufferObjectInstance()
	{
		return m_Value->bufferObject.instance;
	}
	BACnetObjectType GetDeviceObjectType()
	{
		return m_Value->bufferDevice.objectType;
	}
	int GetDeviceInstance()
	{
		return m_Value->bufferDevice.instance;
	}
	void GetPreviousNotification(timeb *value)
	{
		bacnetapi_get_timeb(value, &m_Value->previousNotification);
	}
	void GetCurrentNotification(timeb *value)
	{
		bacnetapi_get_timeb(value, &m_Value->currentNotification);
	}

private:
	_BACnetNotificationParameterBufferReady* m_Value;
};

class CBACnetNotificationParameterBufferReadyNew
{
public:
	CBACnetNotificationParameterBufferReadyNew(){}
	CBACnetNotificationParameterBufferReadyNew(_BACnetNotificationParameterBufferReadyNew *pSrc)
	{
		m_Value = pSrc;
	}

	//Get Properties
	BACnetObjectType GetBufferObjectType()
	{
		if (m_Value == NULL)
			return BACnetObjectTypeNone;
		return m_Value->bufferProperty.objectIdentifier.objectType;
	}
	int GetBufferObjectInstance()
	{
		return m_Value->bufferProperty.objectIdentifier.instance;
	}
	BACnetObjectType GetDeviceObjectType()
	{
		return m_Value->bufferProperty.deviceIdentifier.objectType;
	}
	int GetDeviceInstance()
	{
		return m_Value->bufferProperty.deviceIdentifier.instance;
	}
	BACnetPropertyIdentifier GetPropertyIdentifier()
	{
		return m_Value->bufferProperty.property;
	}
	int GetArrayIndex()
	{
		return m_Value->bufferProperty.propertyArrayIndex;
	}
	UINT GetPreviousNotification()
	{
		return m_Value->previousNotification;
	}
	UINT GetCurrentNotification()
	{
		return m_Value->currentNotification;
	}

private:
	_BACnetNotificationParameterBufferReadyNew* m_Value;
};

class CBACnetNotificationParameters
{
public:
	CBACnetNotificationParameters(){}
	CBACnetNotificationParameters(BACnetNotificationParameters *pSrc)
	{
		m_Value = pSrc;
	}
	CBACnetNotificationParameterChangeOfBitString& GetChangeOfBitString()
	{
		changeOfBitString = CBACnetNotificationParameterChangeOfBitString(&m_Value->_type.changeOfBitString);
		return changeOfBitString;
	}
	CBACnetNotificationParameterChangeOfState& GetChangeOfState()
	{
		changeOfState = CBACnetNotificationParameterChangeOfState(&m_Value->_type.changeOfState);
		return changeOfState;
	}
	CBACnetNotificationParameterChangeOfValue& GetChangeOfValue()
	{
		changeOfValue = CBACnetNotificationParameterChangeOfValue(&m_Value->_type.changeOfValue);
		return changeOfValue;
	}
	CBACnetNotificationParameterCommandFailure& GetCommandFailure()
	{
		commandFailure = CBACnetNotificationParameterCommandFailure(&m_Value->_type.commandFailure);
		return commandFailure;
	}
	CBACnetNotificationParameterFloatingLimit& GetFloatingLimit()
	{
		floatingLimit = CBACnetNotificationParameterFloatingLimit(&m_Value->_type.floatingLimit);
		return floatingLimit;
	}
	CBACnetNotificationParameterOutOfRange& GetOutOfRange()
	{
		outOfRange = CBACnetNotificationParameterOutOfRange(&m_Value->_type.outOfRange);
		return outOfRange;
	}
	CSequenceOfBACnetPropertyValue& GetComplexEventType()
	{
		complexEventType = CSequenceOfBACnetPropertyValue(&m_Value->_type.complexEventType);
		return complexEventType;
	}
	CBACnetNotificationParameterBufferReadyNew& GetBufferReadyNew()
	{
		bufferReadyNew = CBACnetNotificationParameterBufferReadyNew(&m_Value->_type.bufferReadyNew);
		return bufferReadyNew;
	}
	BACnetEventType GetEventType()
	{
		return m_Value->type;
	}

private:
	CBACnetNotificationParameterChangeOfBitString changeOfBitString;
	CBACnetNotificationParameterChangeOfState changeOfState;
	CBACnetNotificationParameterChangeOfValue changeOfValue;
	CBACnetNotificationParameterCommandFailure commandFailure;
	CBACnetNotificationParameterFloatingLimit floatingLimit;
	CBACnetNotificationParameterOutOfRange outOfRange;
	CSequenceOfBACnetPropertyValue complexEventType;
	CBACnetNotificationParameterBufferReady bufferReady;
	CBACnetNotificationParameterBufferReadyNew bufferReadyNew;
	BACnetNotificationParameters* m_Value;
};

class CSequenceOfReadAccessPropertyValue
{
public:
	CSequenceOfReadAccessPropertyValue(){}
	CSequenceOfReadAccessPropertyValue(SequenceOfBACnetReadAccessPropertyValue *pSrc){m_Value = pSrc;}
	virtual ~CSequenceOfReadAccessPropertyValue(){}
	//List access
	CBACnetReadAccessPropertyValue Add(BACnetPropertyIdentifier propertyIdentifier, int propertyArrayIndex = -1)
	{
		BACnetReadAccessPropertyValue *propertyValue = NULL;
		if (m_Value)
		{
			propertyValue = bacnapi_set_element_read_access_property_value(m_Value, NULL);
			propertyValue->propertyIdentifier = propertyIdentifier;
			propertyValue->propertyArrayIndex = propertyArrayIndex;
		}
		return CBACnetReadAccessPropertyValue(propertyValue);
	}
	CBACnetReadAccessPropertyValue GetItem(int index)
	{
		BACnetReadAccessPropertyValue *propertyValue = NULL;
		if (m_Value)
			propertyValue = bacnapi_get_element_read_access_property_value(m_Value, index);
		return CBACnetReadAccessPropertyValue(propertyValue);
	}
	int GetSize()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->qty;
	}
private:
	SequenceOfBACnetReadAccessPropertyValue *m_Value;
};

class CSequenceOfBACnetPropertyReference
{
public:
	CSequenceOfBACnetPropertyReference(){}
	CSequenceOfBACnetPropertyReference(SequenceOfBACnetPropertyReference *pSrc){m_Value = pSrc;}
	virtual ~CSequenceOfBACnetPropertyReference(){}
	//List access
	CPropertyReference Add(BACnetPropertyIdentifier propertyIdentifier, int propertyArrayIndex = -1)
	{
		BACnetPropertyReference *propertyReference = NULL;
		if (m_Value)
		{
			propertyReference = bacnapi_set_element_property_reference(m_Value, NULL);
			propertyReference->propertyIdentifier = propertyIdentifier;
			propertyReference->propertyArrayIndex = propertyArrayIndex;
		}
		return CPropertyReference(propertyReference);
	}
	CPropertyReference GetItem(int index)
	{
		BACnetPropertyReference *propertyReference = NULL;
		if (m_Value)
			propertyReference = bacnapi_get_element_property_reference(m_Value, index);
		return CPropertyReference(propertyReference);
	}
	int GetSize()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->qty;
	}
private:
	SequenceOfBACnetPropertyReference *m_Value;
};

class CReadAccessResult
{
public:
	CReadAccessResult(){}
	CReadAccessResult(_ReadAccessResult *pSrc)
	{
		m_Value = pSrc;
		listOfResults = CSequenceOfReadAccessPropertyValue(&m_Value->listOfResults);
	}
	//Set Properties
	void SetObjectIdentifierObjectType(BACnetObjectType data)
	{
		if (m_Value != NULL)
			m_Value->objectIdentifier.objectType = data;
	}
	void SetObjectIdentifierInstance(int data)
	{
		if (m_Value != NULL)
			m_Value->objectIdentifier.instance = data;
	}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType()
	{
		if (m_Value == NULL)
			return BACnetObjectTypeNone;
		return m_Value->objectIdentifier.objectType;
	}
	int GetObjectIdentifierInstance()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->objectIdentifier.instance;
	}

	CSequenceOfReadAccessPropertyValue &GetListOfResults(){return listOfResults;}
private:
	CSequenceOfReadAccessPropertyValue listOfResults;
	_ReadAccessResult *m_Value;
};

class CReadAccessSpecification
{
public:
	CReadAccessSpecification(){}
	CReadAccessSpecification(_ReadAccessSpecification *pSrc)
	{
		m_Value = pSrc;
		listOfPropertyReferences = CSequenceOfBACnetPropertyReference(&m_Value->listOfPropertyReferences);
	}
	//Set Properties
	void SetObjectIdentifierObjectType(BACnetObjectType data)
	{
		if (m_Value != NULL)
			m_Value->objectIdentifier.objectType = data;
	}
	void SetObjectIdentifierInstance(int data)
	{
		if (m_Value != NULL)
			m_Value->objectIdentifier.instance = data;
	}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType()
	{
		if (m_Value == NULL)
			return BACnetObjectTypeNone;
		return m_Value->objectIdentifier.objectType;
	}
	int GetObjectIdentifierInstance()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->objectIdentifier.instance;
	}

	CSequenceOfBACnetPropertyReference& GetListOfPropertyReferences(){return listOfPropertyReferences;}
private:
	CSequenceOfBACnetPropertyReference listOfPropertyReferences;
	_ReadAccessSpecification *m_Value;
};

class CWriteAccessSpecification
{
public:
	CWriteAccessSpecification(){}
	CWriteAccessSpecification(_WriteAccessSpecification *pSrc)
	{
		m_Value = pSrc;
		listOfProperties = CSequenceOfBACnetPropertyValue(&m_Value->listOfProperties);
	}
	//Set Properties
	void SetObjectIdentifierObjectType(BACnetObjectType data)
	{
		if (m_Value != NULL)
			m_Value->objectIdentifier.objectType = data;
	}
	void SetObjectIdentifierInstance(int data)
	{
		if (m_Value != NULL)
			m_Value->objectIdentifier.instance = data;
	}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType()
	{
		if (m_Value == NULL)
			return BACnetObjectTypeNone;
		return m_Value->objectIdentifier.objectType;
	}
	int GetObjectIdentifierInstance()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->objectIdentifier.instance;
	}

	CSequenceOfBACnetPropertyValue& GetListOfProperties(){return listOfProperties;}
private:
	CSequenceOfBACnetPropertyValue listOfProperties;
	_WriteAccessSpecification *m_Value;
};

class CSequenceOfReadAccessResult
{
public:
	CSequenceOfReadAccessResult(){}
	CSequenceOfReadAccessResult(SequenceOfReadAccessResult *pSrc){m_Value = pSrc;}
	virtual ~CSequenceOfReadAccessResult(){}

	//List access
	CReadAccessResult Add(BACnetObjectType objectType, int instance)
	{
		_ReadAccessResult *readAccessResult = NULL;
		if (m_Value)
		{
			readAccessResult = bacnapi_set_element_read_access_result(m_Value, NULL);
			readAccessResult->objectIdentifier.objectType = objectType;
			readAccessResult->objectIdentifier.instance = instance;
		}
		return CReadAccessResult(readAccessResult);
	}
	CReadAccessResult GetItem(int index)
	{
		_ReadAccessResult *readAccessResult = NULL;
		if (m_Value)
			readAccessResult = bacnapi_get_element_read_access_result(m_Value, index);
		return CReadAccessResult(readAccessResult);
	}
	int GetSize()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->qty;
	}
private:
	SequenceOfReadAccessResult *m_Value;
};

class CSequenceOfReadAccessSpecification
{
public:
	CSequenceOfReadAccessSpecification(){}
	CSequenceOfReadAccessSpecification(SequenceOfReadAccessSpecification *pSrc){m_Value = pSrc;}
	virtual ~CSequenceOfReadAccessSpecification(){}

	//List access
	CReadAccessSpecification Add(BACnetObjectType objectType, int instance)
	{
		_ReadAccessSpecification *readAccessSpecification = NULL;
		if (m_Value)
		{
			readAccessSpecification = bacnapi_set_element_read_access_specification(m_Value, NULL);
			readAccessSpecification->objectIdentifier.objectType = objectType;
			readAccessSpecification->objectIdentifier.instance = instance;
		}
		return CReadAccessSpecification(readAccessSpecification);
	}
	CReadAccessSpecification GetItem(int index)
	{
		_ReadAccessSpecification *readAccessSpecification = NULL;
		if (m_Value)
			readAccessSpecification = bacnapi_get_element_read_access_specification(m_Value, index);
		return CReadAccessSpecification(readAccessSpecification);
	}
	int GetSize()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->qty;
	}
private:
	SequenceOfReadAccessSpecification *m_Value;
};

class CSequenceOfWriteAccessSpecification : CBACnetArray
{
public:
	CSequenceOfWriteAccessSpecification(){}
	CSequenceOfWriteAccessSpecification(SequenceOfWriteAccessSpecification *pSrc){m_Array = (SequenceOfWriteAccessSpecification*)pSrc;}
	virtual ~CSequenceOfWriteAccessSpecification(){}

	//List access
	CWriteAccessSpecification Add(BACnetObjectType objectType, int instance)
	{
		_WriteAccessSpecification *writeAccessSpecification = NULL;
		BACnetDataType* new_element = AddElement(bacnetDT_SequenceOfWriteAccessSpecification, bacnetDT_WriteAccessSpecification);

		writeAccessSpecification = (_WriteAccessSpecification*)new_element->_type.writeAccessSpecification;
		writeAccessSpecification->objectIdentifier.objectType = objectType;
		writeAccessSpecification->objectIdentifier.instance = instance;

		return CWriteAccessSpecification(writeAccessSpecification);
	}
	CWriteAccessSpecification GetItem(int index)
	{
		BACnetDataType* element;
		_WriteAccessSpecification *writeAccessSpecification = NULL;

		if (m_Array->type == bacnetDT_WriteAccessSpecification)
		{
			return CWriteAccessSpecification((_WriteAccessSpecification*)m_Array->_type.writeAccessSpecification);
		}
		else
		{
			element = GetElement(bacnetDT_SequenceOfWriteAccessSpecification, index);
			if (element)
				writeAccessSpecification = (_WriteAccessSpecification*)element->_type.writeAccessSpecification;

			return CWriteAccessSpecification(writeAccessSpecification);
		}
	}
	int GetSize()
	{
		return GetArraySize(bacnetDT_SequenceOfWriteAccessSpecification);
	}
};

class CBACnetServiceReadPropertyMultiple
{
public:
	CBACnetServiceReadPropertyMultiple()
	{
		bacnapi_read_property_multiple_initialize(service, request, ack, error);
		listOfReadAccessResult = CSequenceOfReadAccessResult(&ack.listOfReadAccessResult);
		listOfReadAccessSpecs = CSequenceOfReadAccessSpecification(&request.listOfReadAccessSpecs);
	}
	~CBACnetServiceReadPropertyMultiple()
	{
		bacnapi_read_property_multiple_destroy(service, request, ack, error);
	}

	//Execution routines
	BYTE Execute(){return read_property_multiple(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID)
	{
		service.device.device_id = DeviceID;
		return read_property_multiple(&service) ? 0x0 : 0xFF;
	}
	void Reset()
	{
		bacnapi_read_property_multiple_destroy(service, request, ack, error);
		bacnapi_read_property_multiple_initialize(service, request, ack, error);
	}

	//Set Properties
	void SetDeviceID(int data){service.device.device_id = data;}

	//Get Properties
	int GetDeviceID(){return service.device.device_id;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.readPropertyMultiple.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.readPropertyMultiple.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}
	CSequenceOfReadAccessSpecification& GetListOfReadAccessSpecs(){return listOfReadAccessSpecs;}
	CSequenceOfReadAccessResult& GetListOfReadAccessResult(){return listOfReadAccessResult;}
private:
	CSequenceOfReadAccessSpecification listOfReadAccessSpecs;
	CSequenceOfReadAccessResult listOfReadAccessResult;
	BACnetServiceReadPropertyMultipleRequest request;
	BACnetServiceReadPropertyMultipleAck ack;
	BACnetError error;
	BACnetServiceReadPropertyMultiple service;
};

class CBACnetServiceWriteProperty
{
public:
	CBACnetServiceWriteProperty()
	{
		bacnapi_write_property_initialize(service, request, error);
		request.propertyArrayIndex = -1;
	}
	~CBACnetServiceWriteProperty()
	{
		bacnapi_write_property_destroy(service, request, error);
	}

	//Execution routines
	BYTE Execute(){return write_property(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, BACnetObjectType ObjectType, int Instance, BACnetPropertyIdentifier PropertyIdentifier, CBACnetAny Value = BACnetNull, UINT priority = 0, int ArrayIndex = -1)
	{
		service.device.device_id = DeviceID;
		bacnapi_ClearBACnetDataType(&request.propertyValue);
		if (bacnapi_access_BACnetData(&request.propertyValue, Value, -1, 1))
			return 0;
		request.priority = priority;
		request.objectIdentifier.objectType = ObjectType;
		request.objectIdentifier.instance = Instance;
		request.propertyIdentifier = PropertyIdentifier;
		request.propertyArrayIndex = ArrayIndex;
		return write_property(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetObjectIdentifierObjectType(BACnetObjectType data){request.objectIdentifier.objectType = data;}
	void SetObjectIdentifierInstance(int data){request.objectIdentifier.instance = data;}
	void SetPropertyIdentifier(BACnetPropertyIdentifier data){request.propertyIdentifier = data;}
	void SetPropertyArrayIndex(int data){request.propertyArrayIndex = data;}
	void SetPriority(UINT data){request.priority = data;}
	void SetDeviceID(int data){service.device.device_id = data;}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType(){return request.objectIdentifier.objectType;}
	int GetObjectIdentifierInstance(int data){return request.objectIdentifier.instance;}
	BACnetPropertyIdentifier GetPropertyIdentifier(){return request.propertyIdentifier;}
	int GetPropertyArrayIndex(){return request.propertyArrayIndex;}
	UINT GetPriority(){return request.priority;}
	int GetDeviceID(){return service.device.device_id;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.writeProperty.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.writeProperty.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}

	CBACnetAny GetPropertyValue(){return CBACnetAny(&request.propertyValue, 0x0);}
private:
	BACnetServiceWritePropertyRequest request;
	BACnetError error;
	BACnetServiceWriteProperty service;
};

class CBACnetServiceWritePropertyMultiple
{
public:
	CBACnetServiceWritePropertyMultiple()
	{
		bacnapi_write_property_multiple_initialize(service, request, error);
		listOfWriteAccessSpecifications = CSequenceOfWriteAccessSpecification(&request.listOfWriteAccessSpecifications);
	}
	~CBACnetServiceWritePropertyMultiple()
	{
		bacnapi_write_property_multiple_destroy(service, request, error);
	}

	//Execution routines
	BYTE Execute(){return write_property_multiple(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID)
	{
		service.device.device_id = DeviceID;
		return write_property_multiple(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetDeviceID(int data){service.device.device_id = data;}

	//Get Properties
	int GetDeviceID(){return service.device.device_id;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.writePropertyMultiple.errorType.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.writePropertyMultiple.errorType.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}

	CSequenceOfWriteAccessSpecification& GetListOfWriteAccessSpecifications(){return listOfWriteAccessSpecifications;}
private:
	CSequenceOfWriteAccessSpecification listOfWriteAccessSpecifications;
	BACnetServiceWritePropertyMultipleRequest request;
	BACnetError error;
	BACnetServiceWritePropertyMultiple service;
};

class CBACnetServiceGetAlarmSummary
{
public:
	CBACnetServiceGetAlarmSummary()
	{
		bacnapi_get_alarm_summary_initialize(service, ack, error);
		sequenceOfAlarmSummaryAck = CSequenceOfAlarmSummaryAck(&ack.sequenceOfAlarmSummaryAck);
	}
	~CBACnetServiceGetAlarmSummary()
	{
		bacnapi_get_alarm_summary_destroy(service, ack, error);
	}

	//Execution routines
	BYTE Execute(){return get_alarm_summary(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID)
	{
		service.device.device_id = DeviceID;
		return get_alarm_summary(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetDeviceID(int data){service.device.device_id = data;}

	//Get Properties
	int GetDeviceID(){return service.device.device_id;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.getAlarmSummary.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.getAlarmSummary.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}
	CSequenceOfAlarmSummaryAck& GetSequenceOfAlarmSummaryAck(){return sequenceOfAlarmSummaryAck;}
private:
	CSequenceOfAlarmSummaryAck sequenceOfAlarmSummaryAck;
	BACnetServiceGetAlarmSummaryAck ack;
	BACnetError error;
	BACnetServiceGetAlarmSummary service;
};

class CBACnetServiceGetEnrolmentSummary
{
public:
	CBACnetServiceGetEnrolmentSummary()
	{
		bacnapi_get_enrolment_summary_initialize(service, request, ack, error);
		enrolmentFilter = CBACnetRecipientProcess(&request.enrolmentFilter);
		sequenceOfEnrolmentSummaryAck = CSequenceOfEnrolmentSummaryAck(&ack.sequenceOfEnrolmentSummaryAck);
		request.eventStateFilter = BACnetEventStateFilterAll;
		request.eventTypeFilter = BACnetEventTypeUndefined;
		request.enrolmentFilter.notEmpty = 0;
		request.priorityFilter.minPriority = -1;
		request.priorityFilter.maxPriority = -1;
		request.notificationClassFilter = -1;
	}
	~CBACnetServiceGetEnrolmentSummary()
	{
		bacnapi_get_enrolment_summary_destroy(service, request, ack, error);
	}

	//Execution routines
	BYTE Execute(){return get_enrolment_summary(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID)
	{
		service.device.device_id = DeviceID;
		return get_enrolment_summary(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetAcknowledgmentFilter(BACnetAcknowledgmentFilter data){request.acknowledgementFilter = data;}
	void SetEventStateFilter(BACnetEventStateFilter data){request.eventStateFilter = data;}
	void SetEventTypeFilter(BACnetEventType data){request.eventTypeFilter = data;}
	void SetMinPriority(UINT data){request.priorityFilter.minPriority = data;}
	void SetMaxPriority(UINT data){request.priorityFilter.maxPriority = data;}
	void SetNotificationClassFilter(UINT data){request.notificationClassFilter = data;}
	void SetDeviceID(int data){service.device.device_id = data;}

	//Get Properties
	BACnetAcknowledgmentFilter GetAcknowledgmentFilter(){return request.acknowledgementFilter;}
	BACnetEventStateFilter GetEventStateFilter(){return request.eventStateFilter;}
	BACnetEventType GetEventTypeFilter(){return request.eventTypeFilter;}
	UINT GetMinPriority(){return request.priorityFilter.minPriority;}
	UINT GetMaxPriority(){return request.priorityFilter.maxPriority;}
	UINT GetNotificationClassFilter(){return request.notificationClassFilter;}
	int GetDeviceID(){return service.device.device_id;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.getEnrolmentSummary.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.getEnrolmentSummary.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}
	CBACnetRecipientProcess& GetEnrolmentFilter(){return enrolmentFilter;}

	CSequenceOfEnrolmentSummaryAck& GetSequenceOfEnrolmentSummaryAck(){return sequenceOfEnrolmentSummaryAck;}
private:
	CBACnetRecipientProcess enrolmentFilter;
	BACnetServiceGetEnrolmentSummaryRequest request;
	BACnetServiceGetEnrolmentSummaryAck ack;
	BACnetError error;
	BACnetServiceGetEnrolmentSummary service;
	CSequenceOfEnrolmentSummaryAck sequenceOfEnrolmentSummaryAck;
};

class CBACnetServiceSubscribeCOV
{
public:
	CBACnetServiceSubscribeCOV()
	{
		bacnapi_subscribe_cov_initialize(service, request, error);
		request.issueConfirmedNotifications = 0xFF;
		request.lifetime = 300;
	}
	~CBACnetServiceSubscribeCOV()
	{
		bacnapi_subscribe_cov_destroy(service, request, error);
	}

	//Execution routines
	BYTE Execute(){return subscribe_cov(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, BACnetObjectType ObjectType, int Instance, UINT subscriberProcessIdentifier = 0, BYTE issueConfirmedNotifications = 0xFF, int lifetime = 600)
	{
		service.device.device_id = DeviceID;
		request.subscriberProcessIdentifier = subscriberProcessIdentifier;
		request.monitoredObjectIdentifier.objectType = ObjectType;
		request.monitoredObjectIdentifier.instance = Instance;
		request.issueConfirmedNotifications = issueConfirmedNotifications;
		request.lifetime = lifetime;
		return subscribe_cov(&service) ? 0x0 : 0xFF;
	}
	//Set Properties
	void SetObjectIdentifierObjectType(BACnetObjectType data){request.monitoredObjectIdentifier.objectType = data;}
	void SetObjectIdentifierInstance(int data){request.monitoredObjectIdentifier.instance = data;}
	void SetSubscriberProcessIdentifier(UINT data){request.subscriberProcessIdentifier = data;}
	void SetIssueConfirmedNotifications(int data){request.issueConfirmedNotifications = data;}
	void SetLifetime(int data){request.lifetime = data;}
	void SetDeviceID(int data){service.device.device_id = data;}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType(){return request.monitoredObjectIdentifier.objectType;}
	int GetObjectIdentifierInstance(int data){return request.monitoredObjectIdentifier.instance;}
	UINT GetSubscriberProcessIdentifier(){return request.subscriberProcessIdentifier;}
	BYTE GetIssueConfirmedNotifications(){return request.issueConfirmedNotifications;}
	int GetLifetime(){return request.lifetime;}
	int GetDeviceID(){return service.device.device_id;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.subscribeCOV.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.subscribeCOV.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}

private:
	BACnetServiceSubscribeCOVRequest request;
	BACnetError error;
	BACnetServiceSubscribeCOV service;
};

class CBACnetFileStreamAccessRequest
{
public:
	CBACnetFileStreamAccessRequest(BACnetFileStreamAccessRequest* src){m_Value = src;}

	//Set Properties
	void SetFileStartPosition(int data){m_Value->fileStartPosition = data;}
	void SetRequestedOctetCount(UINT data){m_Value->requestedOctetCount = data;}

	//Get Properties
	int GetFileStartPosition(){return m_Value->fileStartPosition;}
	int GetRequestedOctetCount(){return m_Value->requestedOctetCount;}
private:
	BACnetFileStreamAccessRequest* m_Value;
};

class CBACnetFileRecordAccessRequest
{
public:
	CBACnetFileRecordAccessRequest(BACnetFileRecordAccessRequest* src){m_Value = src;}

	//Set Properties
	void SetFileStartRecord(int data){m_Value->fileStartRecord = data;}
	void SetRequestedRecordCount(UINT data){m_Value->requestedRecordCount = data;}

	//Get Properties
	int GetFileStartRecord(){return m_Value->fileStartRecord;}
	UINT GetRequestedRecordCount(){return m_Value->requestedRecordCount;}
private:
	BACnetFileRecordAccessRequest* m_Value;
};

class CBACnetFileStreamAccessAck
{
public:
	CBACnetFileStreamAccessAck(BACnetFileStreamAccessAck* src){m_Value = src;}

	//Set Properties
	void SetFileStartPosition(int data){m_Value->fileStartPosition = data;}
	BYTE SetFileData(BYTE *data, int length)
	{
		BACnetDataType wrapper;

		wrapper.type = bacnetDT_OctetString;
		wrapper._type.Octet.contents = data;
		wrapper._type.Octet.len = length;

		if (!m_Value)
			return 0;

		if (bacnapi_access_octets(&m_Value->fileData, &wrapper, 1) == 0)
			return 0xFF;

		return 0x0;
	}

	//Get Properties
	int GetFileStartPosition(){return m_Value->fileStartPosition;}
	int GetFileDataLength(){return m_Value->fileData.len;}
	const BYTE* GetFileData(){return m_Value->fileData.contents;}

private:
	BACnetFileStreamAccessAck* m_Value;
};

class CBACnetFileRecordAccessAck
{
public:
	CBACnetFileRecordAccessAck(BACnetFileRecordAccessAck* src){m_Value = src;}

	//Set Properties
	void SetFileStartRecord(int data){m_Value->fileStartRecord = data;}
	void SetReturnedRecordCount(UINT data){m_Value->returnedRecordCount = data;}
	BYTE SetFileRecordData(BYTE *data, int length)
	{
		BACnetDataType wrapper;

		wrapper.type = bacnetDT_OctetString;
		wrapper._type.Octet.contents = data;
		wrapper._type.Octet.len = length;

		if (!m_Value)
			return 0;

		if (bacnapi_access_octets(&m_Value->fileRecordData, &wrapper, 1) == 0)
			return 0xFF;
	}

	//Get Properties
	int GetFileStartRecord(){return m_Value->fileStartRecord;}
	int GetReturnedRecordCount(){return m_Value->returnedRecordCount;}
	int GetFileRecordDataLength(){return m_Value->fileRecordData.len;}
	const BYTE* GetFileRecordData(){return m_Value->fileRecordData.contents;}

private:
	BACnetFileRecordAccessAck* m_Value;
};

class CBACnetServiceAtomicReadFile
{
public:
	CBACnetServiceAtomicReadFile()
	{
		bacnapi_atomic_read_file_initialize(service, request, ack, error);
	}
	~CBACnetServiceAtomicReadFile()
	{
		bacnapi_atomic_read_file_destroy(service, request, ack, error);
	}

	//Execution routines
	BYTE Execute(){return atomic_read_file(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, BACnetObjectType ObjectType, int Instance)
	{
		service.device.device_id = DeviceID;
		request.objectIdentifier.objectType = ObjectType;
		request.objectIdentifier.instance = Instance;
		return atomic_read_file(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetObjectIdentifierObjectType(BACnetObjectType data){request.objectIdentifier.objectType = data;}
	void SetObjectIdentifierInstance(int data){request.objectIdentifier.instance = data;}
	void SetStreamAccessFileStartPosition(int data)
	{
		request.accessMethod = BACnetFileAccessMethodStreamAccess;
		request._accessMethod.streamAccess.fileStartPosition = data;
	}
	void SetStreamAccessRequestedOctetCount(UINT data)
	{
		request.accessMethod = BACnetFileAccessMethodStreamAccess;
		request._accessMethod.streamAccess.requestedOctetCount = data;
	}
	void SetRecordAccessFileStartRecord(int data)
	{
		request.accessMethod = BACnetFileAccessMethodRecordAccess;
		request._accessMethod.recordAccess.fileStartRecord = data;
	}
	void SetRecordAccessRequestedRecordCount(UINT data)
	{
		request.accessMethod = BACnetFileAccessMethodRecordAccess;
		request._accessMethod.recordAccess.requestedRecordCount = data;
	}

	void SetDeviceID(int data){service.device.device_id = data;}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType(){return request.objectIdentifier.objectType;}
	int GetObjectIdentifierInstance(){return request.objectIdentifier.instance;}
	int GetStreamAccessFileStartPosition(){return request._accessMethod.streamAccess.fileStartPosition;}
	UINT GetStreamAccessRequestedOctetCount(){return request._accessMethod.streamAccess.requestedOctetCount;}
	int GetRecordAccessFileStartRecord(){return request._accessMethod.recordAccess.fileStartRecord;}
	UINT GetRecordAccessRequestedRecordCount(){return request._accessMethod.recordAccess.requestedRecordCount;}
	BACnetFileAccessMethod GetRequestFileAccessMethod(){return request.accessMethod;}
	BACnetFileAccessMethod GetAckFileAccessMethod(){return ack.accessMethod;}
	BACnetOctet* GetAckStreamAccessOctets(){return &ack._accessMethod.streamAccess.fileData;}

	int GetAckStreamAccessFileStartPosition()
	{
		if (ack.accessMethod != BACnetFileAccessMethodStreamAccess)
			return -1;
		return ack._accessMethod.streamAccess.fileStartPosition;
	}
	int GetAckStreamAccessFileDataLength()
	{
		if (ack.accessMethod != BACnetFileAccessMethodStreamAccess)
			return -1;
		return ack._accessMethod.streamAccess.fileData.len;
	}
	const BYTE* GetAckStreamAccessFileData()
	{
		if (ack.accessMethod != BACnetFileAccessMethodStreamAccess)
			return NULL;
		return ack._accessMethod.streamAccess.fileData.contents;
	}

	int GetAckRecordAccessFileStartRecord()
	{
		if (ack.accessMethod != BACnetFileAccessMethodRecordAccess)
			return -1;
		return ack._accessMethod.recordAccess.fileStartRecord;
	}
	int GetAckRecordAccessReturnedRecordCount()
	{
		if (ack.accessMethod != BACnetFileAccessMethodRecordAccess)
			return -1;
		return ack._accessMethod.recordAccess.returnedRecordCount;
	}
	int GetAckRecordAccessFileDataRecordLength()
	{
		if (ack.accessMethod != BACnetFileAccessMethodRecordAccess)
			return -1;
		return ack._accessMethod.recordAccess.fileRecordData.len;
	}
	const BYTE* GetAckRecordAccessFileRecordData()
	{
		if (ack.accessMethod != BACnetFileAccessMethodRecordAccess)
			return NULL;
		return ack._accessMethod.recordAccess.fileRecordData.contents;
	}
	int GetAckEndOfFile(){return ack.endOfFile;}

	int GetDeviceID(){return service.device.device_id;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.atomicReadFile.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.atomicReadFile.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}
private:
	BACnetServiceAtomicReadFileRequest request;
	BACnetServiceAtomicReadFileAck ack;
	BACnetError error;
	BACnetServiceAtomicReadFile service;
};

class CBACnetServiceAtomicWriteFile
{
public:
	CBACnetServiceAtomicWriteFile()
	{
		bacnapi_atomic_write_file_initialize(service, request, ack, error);
	}
	~CBACnetServiceAtomicWriteFile()
	{
		bacnapi_atomic_write_file_destroy(service, request, ack, error);
	}

	//Execution routines
	BYTE Execute(){return atomic_write_file(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, BACnetObjectType ObjectType, int Instance, BYTE* data, int len, int FileStartPosition = 0)
	{
		service.device.device_id = DeviceID;
		request.objectIdentifier.objectType = ObjectType;
		request.objectIdentifier.instance = Instance;
		request.accessMethod = BACnetFileAccessMethodStreamAccess;
		SetStreamAccessFileStartPosition(FileStartPosition);
		SetStreamAccessFileData(data, len);
		return atomic_write_file(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetObjectIdentifierObjectType(BACnetObjectType data){request.objectIdentifier.objectType = data;}
	void SetObjectIdentifierInstance(int data){request.objectIdentifier.instance = data;}
	void SetDeviceID(int data){service.device.device_id = data;}
	void SetStreamAccessFileStartPosition(int data)
	{
		Init(BACnetFileAccessMethodStreamAccess);
		request._accessMethod.streamAccess.fileStartPosition = data;
	}
	void SetStreamAccessFileData(BYTE* data, int len)
	{
		Init(BACnetFileAccessMethodStreamAccess);
		bacnapi_set_octets(&request._accessMethod.streamAccess.fileData, data, len);
		request.accessMethod = BACnetFileAccessMethodStreamAccess;
	}
	void SetRecordAccessFileStartRecord(int data)
	{
		Init(BACnetFileAccessMethodRecordAccess);
		request._accessMethod.recordAccess.fileStartRecord = data;
	}
	void SetRecordAccessReturnedRecordCount(UINT data)
	{
		Init(BACnetFileAccessMethodRecordAccess);
		request._accessMethod.recordAccess.returnedRecordCount = data;
	}
	void SetRecordAccessFileRecordData(BYTE* data, int len)
	{
		Init(BACnetFileAccessMethodRecordAccess);
		bacnapi_set_octets(&request._accessMethod.recordAccess.fileRecordData, data, len);
		request.accessMethod = BACnetFileAccessMethodStreamAccess;
	}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType(){return request.objectIdentifier.objectType;}
	int GetObjectIdentifierInstance(int data){return request.objectIdentifier.instance;}
	int GetDeviceID(){return service.device.device_id;}
	BACnetFileAccessMethod GetRequestFileAccessMethod(){return request.accessMethod;}
	BACnetFileAccessMethod GetAckFileAccessMethod(){return ack.accessMethod;}
	int GetAckFileStartPosition(){return ack.fileStartPosition;}
	int GetAckFileStartRecord(){return ack.fileStartRecord;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.atomicWriteFile.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.atomicWriteFile.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}
private:
	BACnetServiceAtomicWriteFileRequest request;
	BACnetServiceAtomicWriteFileAck ack;
	BACnetError error;
	BACnetServiceAtomicWriteFile service;
	void Init(BACnetFileAccessMethod accessMethod)
	{
		if (request.accessMethod != accessMethod)
		{
			if (request.accessMethod == BACnetFileAccessMethodStreamAccess)
				bacnapi_ClearOctet(&request._accessMethod.streamAccess.fileData);
			else if (request.accessMethod == BACnetFileAccessMethodRecordAccess)
				bacnapi_ClearOctet(&request._accessMethod.recordAccess.fileRecordData);
		}
		request.accessMethod = accessMethod;
	}
};

class CBACnetServiceAcknowledgeAlarm
{
public:
	CBACnetServiceAcknowledgeAlarm()
	{
		bacnapi_acknowledge_alarm_initialize(service, request, error);
	}
	~CBACnetServiceAcknowledgeAlarm()
	{
		bacnapi_acknowledge_alarm_destroy(service, request, error);
	}
	//Execution routines
	BYTE Execute(){return acknowledge_alarm(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, BACnetObjectType ObjectType, int Instance, BACnetEventState eventStateAcknowledged, const timeb *timeStamp, UINT acknowledgingProcessIdentifier = 0, char* acknowledgmentSource = "")
	{
		service.device.device_id = DeviceID;
		request.acknowledgingProcessIdentifier = acknowledgingProcessIdentifier;
		request.eventObjectIdentifier.objectType = ObjectType;
		request.eventObjectIdentifier.instance = Instance;
		request.eventStateAcknowledged = eventStateAcknowledged;
		request.timeStamp.type = BACnetTimeStampDateTime;
		bacnetapi_set_timeb(&request.timeStamp._type.dateTime, timeStamp);
		bacnapi_access_string(&acknowledgmentSource, &request.acknowledgmentSource, 0);

		return acknowledge_alarm(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetEventStateAcknowledged(BACnetEventState data){request.eventStateAcknowledged = data;}
	void SetAcknowledgingProcessIdentifier(UINT data){request.acknowledgingProcessIdentifier = data;}
	void SetEventObjectIdentifierObjectType(BACnetObjectType data){request.eventObjectIdentifier.objectType = data;}
	void SetEventObjectIdentifierInstance(int data){request.eventObjectIdentifier.instance = data;}
	void SetDeviceID(int data){service.device.device_id = data;}
	void SetTimeStamp(BACnetTimeStamp data){request.timeStamp = data;}

	//Get Properties
	BACnetEventState GetEventStateAcknowledged(){return request.eventStateAcknowledged;}
	UINT GetAcknowledgingProcessIdentifier(){return request.acknowledgingProcessIdentifier;}
	BACnetObjectType GetEventObjectIdentifierObjectType(){return request.eventObjectIdentifier.objectType;}
	int GetEventObjectIdentifierInstance(int data){return request.eventObjectIdentifier.instance;}
	int GetDeviceID(){return service.device.device_id;}
	CBACnetTimeStamp GetTimeStamp(){return CBACnetTimeStamp(&request.timeStamp);}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.acknowledgeAlarm.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.acknowledgeAlarm.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}

private:

	BACnetError error;
	BACnetServiceAcknowledgeAlarmRequest request;
	BACnetServiceAcknowledgeAlarm service;
};

class CBACnetServiceAddListElement
{
public:
	CBACnetServiceAddListElement()
	{
		bacnapi_add_list_element_initialize(service, request, error);
		listOfElements = CSequenceOfBACnetAny(&request.listOfElements);
	}
	~CBACnetServiceAddListElement()
	{
		bacnapi_add_list_element_destroy(service, request, error);
	}

	//Execution routines
	BYTE Execute(){return add_list_element(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, BACnetObjectType ObjectType, int Instance, BACnetPropertyIdentifier PropertyIdentifier, int ArrayIndex = -1)
	{
		service.device.device_id = DeviceID;
		request.objectIdentifier.objectType = ObjectType;
		request.objectIdentifier.instance = Instance;
		request.propertyIdentifier = PropertyIdentifier;
		request.propertyArrayIndex = ArrayIndex;
		return add_list_element(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetObjectIdentifierObjectType(BACnetObjectType data){request.objectIdentifier.objectType = data;}
	void SetObjectIdentifierInstance(int data){request.objectIdentifier.instance = data;}
	void SetPropertyIdentifier(BACnetPropertyIdentifier data){request.propertyIdentifier = data;}
	void SetPropertyArrayIndex(int data){request.propertyArrayIndex = data;}
	void SetDeviceID(int data){service.device.device_id = data;}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType(){return request.objectIdentifier.objectType;}
	int GetObjectIdentifierInstance(int data){return request.objectIdentifier.instance;}
	BACnetPropertyIdentifier GetPropertyIdentifier(){return request.propertyIdentifier;}
	int GetPropertyArrayIndex(){return request.propertyArrayIndex;}
	int GetDeviceID(){return service.device.device_id;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.addListElement.errorType.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.addListElement.errorType.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}

	CSequenceOfBACnetAny& GetListOfElements(){return listOfElements;}
private:
	CSequenceOfBACnetAny listOfElements;
	BACnetServiceAddListElementRequest request;
	BACnetError error;
	BACnetServiceAddListElement service;
};

class CBACnetServiceRemoveListElement
{
public:
	CBACnetServiceRemoveListElement()
	{
		bacnapi_remove_list_element_initialize(service, request, error);
		listOfElements = CSequenceOfBACnetAny(&request.listOfElements);
	}
	~CBACnetServiceRemoveListElement()
	{
		bacnapi_remove_list_element_destroy(service, request, error);
	}

	//Execution routines
	BYTE Execute(){return remove_list_element(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, BACnetObjectType ObjectType, int Instance, BACnetPropertyIdentifier PropertyIdentifier, int ArrayIndex = -1)
	{
		service.device.device_id = DeviceID;
		request.objectIdentifier.objectType = ObjectType;
		request.objectIdentifier.instance = Instance;
		request.propertyIdentifier = PropertyIdentifier;
		request.propertyArrayIndex = ArrayIndex;
		return remove_list_element(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetObjectIdentifierObjectType(BACnetObjectType data){request.objectIdentifier.objectType = data;}
	void SetObjectIdentifierInstance(int data){request.objectIdentifier.instance = data;}
	void SetPropertyIdentifier(BACnetPropertyIdentifier data){request.propertyIdentifier = data;}
	void SetPropertyArrayIndex(int data){request.propertyArrayIndex = data;}
	void SetDeviceID(int data){service.device.device_id = data;}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType(){return request.objectIdentifier.objectType;}
	int GetObjectIdentifierInstance(int data){return request.objectIdentifier.instance;}
	BACnetPropertyIdentifier GetPropertyIdentifier(){return request.propertyIdentifier;}
	int GetPropertyArrayIndex(){return request.propertyArrayIndex;}
	int GetDeviceID(){return service.device.device_id;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.removeListElement.errorType.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.removeListElement.errorType.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}

	CSequenceOfBACnetAny& GetListOfElements(){return listOfElements;}
private:
	CSequenceOfBACnetAny listOfElements;
	BACnetServiceRemoveListElementRequest request;
	BACnetError error;
	BACnetServiceRemoveListElement service;
};

class CBACnetServiceCreateObject
{
public:
	CBACnetServiceCreateObject()
	{
		bacnapi_create_object_initialize(service, request, ack, error);
		listOfInitialValues = CSequenceOfBACnetPropertyValue(&request.listOfInitialValues);
	}
	~CBACnetServiceCreateObject()
	{
		bacnapi_create_object_destroy(service, request, ack, error);
	}

	//Execution routines
	BYTE Execute(){return create_object(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, BACnetObjectType ObjectType, int Instance = -1)
	{
		service.device.device_id = DeviceID;
		if (Instance == -1)
		{
			request.type = BACnetObjectSpecifierTypeObjectType;
			request._type.objectType = ObjectType;
		}
		else
		{
			request.type = BACnetObjectSpecifierTypeObjectIdentifier;
			request._type.objectIdentifier.objectType = ObjectType;
			request._type.objectIdentifier.instance = Instance;
		}
		return create_object(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetObjectIdentifierObjectType(BACnetObjectType data)
	{
		request.type = BACnetObjectSpecifierTypeObjectIdentifier;
		request._type.objectIdentifier.objectType = data;
	}
	void SetObjectIdentifierInstance(int data)
	{
		request.type = BACnetObjectSpecifierTypeObjectIdentifier;
		request._type.objectIdentifier.instance = data;
	}
	void SetObjectType(BACnetObjectType data)
	{
		request.type = BACnetObjectSpecifierTypeObjectType;
		request._type.objectType = data;
	}
	void SetDeviceID(int data){service.device.device_id = data;}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType(){return request._type.objectIdentifier.objectType;}
	int GetObjectIdentifierInstance(int data){return request._type.objectIdentifier.instance;}
	BACnetObjectType GetObjectType(int data){return request._type.objectType;}
	BACnetObjectType GetAckObjectType(){return ack.objectIdentifier.objectType;}
	int GetAckInstance(int data){return ack.objectIdentifier.instance;}
	int GetDeviceID(){return service.device.device_id;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.createObject.errorType.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.createObject.errorType.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}

	CSequenceOfBACnetPropertyValue& GetListOfInitialValues(){return listOfInitialValues;}
private:
	CSequenceOfBACnetPropertyValue listOfInitialValues;
	BACnetServiceCreateObjectRequest request;
	BACnetServiceCreateObjectAck ack;
	BACnetError error;
	BACnetServiceCreateObject service;
};

class CBACnetServiceDeleteObject
{
public:
	CBACnetServiceDeleteObject()
	{
		bacnapi_delete_object_initialize(service, request, error);
	}
	~CBACnetServiceDeleteObject()
	{
		bacnapi_delete_object_destroy(service, request, error);
	}

	//Execution routines
	BYTE Execute(){return delete_object(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, BACnetObjectType ObjectType, int Instance)
	{
		service.device.device_id = DeviceID;
		request.objectIdentifier.objectType = ObjectType;
		request.objectIdentifier.instance = Instance;
		return delete_object(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetObjectIdentifierObjectType(BACnetObjectType data){request.objectIdentifier.objectType = data;}
	void SetObjectIdentifierInstance(int data){request.objectIdentifier.instance = data;}
	void SetDeviceID(int data){service.device.device_id = data;}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType(){return request.objectIdentifier.objectType;}
	int GetObjectIdentifierInstance(int data){return request.objectIdentifier.instance;}
	int GetDeviceID(){return service.device.device_id;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.deleteObject.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.deleteObject.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}

private:
	BACnetServiceDeleteObjectRequest request;
	BACnetError error;
	BACnetServiceDeleteObject service;
};

class CBACnetServiceDeviceCommunicationControl
{
public:
	CBACnetServiceDeviceCommunicationControl()
	{
		bacnapi_device_communication_control_initialize(service, request, error);
	}
	~CBACnetServiceDeviceCommunicationControl()
	{
		bacnapi_device_communication_control_destroy(service, request, error);
	}

	//Execution routines
	BYTE Execute(){return device_communication_control(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, BACnetEnableDisable EnableDisable = DeviceCommunicationControlEnable, const char *Password = NULL, UINT TimeDuration = 0)
	{
		service.device.device_id = DeviceID;
		request.timeDuration = TimeDuration;
		request.enableDisable = EnableDisable;
		if (Password)
			bacnapi_string_set(&request.password, Password);
		return device_communication_control(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetTimeDuration(UINT data){request.timeDuration = data;}
	void SetEnableDisable(BACnetEnableDisable data){request.enableDisable = data;}
	void SetPassword(const char *data)
	{
		if (data)
			bacnapi_string_set(&request.password, data);
	}
#ifdef WTL
	void SetPassword(const CString data)
	{
		bacnapi_string_set(&request.password, data.operator LPCTSTR());
	}
#endif
	void SetDeviceID(int data){service.device.device_id = data;}

	//Get Properties
	UINT GetTimeDuration(){return request.timeDuration;}
	BACnetEnableDisable GetEnableDisable(){return request.enableDisable;}
#ifdef WTL
	CString GetPassword()
	{
		CString result;
		char *buf = NULL;

		//Use the bacnapi_string_get function to convert the BACnetDataType structure stored in value into a string
		bacnapi_string_get(&buf, &request.password);
		result = buf;
		//Clear the memory in buf using the bacnapi_string_destroy function
		bacnapi_string_destroy(buf);

		return result;
	}
#endif
	int GetDeviceID(){return service.device.device_id;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.deviceCommunicationControl.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.deviceCommunicationControl.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}

private:
	BACnetServiceDeviceCommunicationControlRequest request;
	BACnetError error;
	BACnetServiceDeviceCommunicationControl service;
};

class CBACnetServiceConfirmedPrivateTransfer
{
public:
	CBACnetServiceConfirmedPrivateTransfer()
	{
		bacnapi_confirmed_private_transfer_initialize(service, request, ack, error);
	}
	~CBACnetServiceConfirmedPrivateTransfer()
	{
		bacnapi_confirmed_private_transfer_destroy(service, request, ack, error);
	}

	//Execution routines
	BYTE Execute(){return confirmed_private_transfer(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, CBACnetAny ServiceParameters = BACnetNull, UINT VendorID = 123, UINT ServiceNumber = 0)
	{
		service.device.device_id = DeviceID;
		request.vendorID = VendorID;
		request.serviceNumber = ServiceNumber;
		bacnapi_ClearBACnetDataType(&request.serviceParameters);
		if (bacnapi_access_BACnetData(&request.serviceParameters, ServiceParameters, -1, 1))
			return 0;
		return confirmed_private_transfer(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetRequestVendorID(UINT data){request.vendorID = data;}
	void SetRequestServiceNumber(UINT data){request.serviceNumber = data;}
	void SetDeviceID(int data){service.device.device_id = data;}

	//Get Properties
	UINT SetRequestVendorID(){return request.vendorID;}
	UINT SetRequestServiceNumber(){return request.serviceNumber;}
	UINT SetAckVendorID(){return ack.vendorID;}
	UINT SetAckServiceNumber(){return ack.serviceNumber;}
	int GetDeviceID(){return service.device.device_id;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.confirmedPrivateTransfer.errorType.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.confirmedPrivateTransfer.errorType.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}

	CBACnetAny GetServiceParameters(){return CBACnetAny(&request.serviceParameters);}
	CBACnetAny GetResultBlock(){return CBACnetAny(&ack.resultBlock);}
private:
	BACnetServicePrivateTransferRequest request;
	BACnetServiceConfirmedPrivateTransferAck ack;
	BACnetError error;
	BACnetServiceConfirmedPrivateTransfer service;
};

class CBACnetServiceConfirmedTextMessage
{
public:
	CBACnetServiceConfirmedTextMessage()
	{
		bacnapi_confirmed_text_message_initialize(service, request, error);
		request.messageClass = BACnetMessageClassUnknown;
	}
	~CBACnetServiceConfirmedTextMessage()
	{
		bacnapi_confirmed_text_message_destroy(service, request, error);
	}

	//Execution routines
	BYTE Execute(){return confirmed_text_message(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, const char *Message, BACnetMessagePriority MessagePriority = BACnetMessagePriorityNormal)
	{
		service.device.device_id = DeviceID;
		SetMessage(Message);
		request.messagePriority = MessagePriority;
		return confirmed_text_message(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetMessagePriority(BACnetMessagePriority data){request.messagePriority = data;}
	void SetMessageClassNumeric(UINT data)
	{
		if (request.messageClass == BACnetMessageClassCharacter)
			bacnapi_ClearBACnetDataType(&request._messageClass.character);
		request.messageClass = BACnetMessageClassCharacter;
		request._messageClass.numeric = data;
	}
	void SetMessageClassCharacter(const char *data)
	{
		request.messageClass = BACnetMessageClassCharacter;
		if (data)
			bacnapi_string_set(&request._messageClass.character, data);
	}
#ifdef WTL
	void SetMessageClassCharacter(const CString data)
	{
		request.messageClass = BACnetMessageClassCharacter;
		bacnapi_string_set(&request._messageClass.character, data.operator LPCTSTR());
	}
#endif
	void SetMessage(const char *data)
	{
		if (data)
			bacnapi_string_set(&request.message, data);
	}
#ifdef WTL
	void SetMessage(const CString data)
	{
		bacnapi_string_set(&request.message, data.operator LPCTSTR());
	}
#endif
	void SetDeviceID(int data){service.device.device_id = data;}

	//Get Properties
	BACnetMessagePriority GetMessagePriority(){return request.messagePriority;}
#ifdef WTL
	CString GetMessageClassCharacter()
	{
		CString result;
		char *buf = NULL;

		if (request.messageClass == BACnetMessageClassCharacter)
		{
			//Use the bacnapi_string_get function to convert the BACnetDataType structure stored in value into a string
			bacnapi_string_get(&buf, &request._messageClass.character);
			result = buf;
			//Clear the memory in buf using the bacnapi_string_destroy function
			bacnapi_string_destroy(buf);
		}

		return result;
	}
	CString GetMessage()
	{
		CString result;
		char *buf = NULL;

		//Use the bacnapi_string_get function to convert the BACnetDataType structure stored in value into a string
		bacnapi_string_get(&buf, &request.message);
		result = buf;
		//Clear the memory in buf using the bacnapi_string_destroy function
		bacnapi_string_destroy(buf);

		return result;
	}
#endif
	int GetDeviceID(){return service.device.device_id;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.confirmedTextMessage.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.confirmedTextMessage.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}
private:
	BACnetServiceTextMessageRequest request;
	BACnetError error;
	BACnetServiceConfirmedTextMessage service;
};

class CBACnetServiceReinitializeDevice
{
public:
	CBACnetServiceReinitializeDevice()
	{
		bacnapi_reinitialize_device_message_initialize(service, request, error);
	}
	~CBACnetServiceReinitializeDevice()
	{
		bacnapi_reinitialize_device_destroy(service, request, error);
	}

	//Execution routines
	BYTE Execute(){return reinitialize_device(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, BACnetStateOfDevice ReinitialisedStateOfDevice = BACnetStateOfDeviceColdstart, const char* Password = NULL)
	{
		service.device.device_id = DeviceID;
		SetPassword(Password);
		SetReinitialisedStateOfDevice(ReinitialisedStateOfDevice);
		return reinitialize_device(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetReinitialisedStateOfDevice(BACnetStateOfDevice data){request.reinitialisedStateOfDevice = data;}
	void SetPassword(const char *data)
	{
		if (data)
			bacnapi_string_set(&request.password, data);
	}
#ifdef WTL
	void SetPassword(const CString data)
	{
		bacnapi_string_set(&request.password, data.operator LPCTSTR());
	}
#endif
	void SetDeviceID(int data){service.device.device_id = data;}

	//Get Properties
#ifdef WTL
	CString GetPassword()
	{
		CString result;
		char *buf = NULL;

		//Use the bacnapi_string_get function to convert the BACnetDataType structure stored in value into a string
		bacnapi_string_get(&buf, &request.password);
		result = buf;
		//Clear the memory in buf using the bacnapi_string_destroy function
		bacnapi_string_destroy(buf);

		return result;
	}
#endif
	BACnetStateOfDevice GetReinitialisedStateOfDevice(){return request.reinitialisedStateOfDevice;}
	int GetDeviceID(){return service.device.device_id;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.reinitializeDevice.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.reinitializeDevice.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}
private:
	BACnetServiceReinitializeDeviceRequest request;
	BACnetError error;
	BACnetServiceReinitializeDevice service;
};

class CBACnetServiceReadRange
{
public:
	CBACnetServiceReadRange()
	{
		bacnapi_read_range_initialize(service, request, ack, error);
		resultFlags = CBACnetBitString(&ack.resultFlags);
		itemData = CArrayOfBACnetLogRecord(&ack.itemData);
	}
	~CBACnetServiceReadRange()
	{
		bacnapi_read_range_destroy(service, request, ack, error);
	}

	//Execution routines
	BYTE Execute(){return read_range(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, int Instance, UINT ReferencedIndex = 1, UINT Count = 50)
	{
		SetObjectIdentifierObjectType(BACnetObjectTypeTrendLog);
		SetObjectIdentifierInstance(Instance);
		SetPropertyIdentifier(Property_logBuffer);
		SetPropertyArrayIndex(-1);
		SetByPositionReferencedIndex(ReferencedIndex);
		SetByPositionCount(Count);
		service.device.device_id = DeviceID;
		return read_range(&service) ? 0x0 : 0xFF;
	}

	BYTE ExecuteBySequenceNumber(int DeviceID, int Instance, UINT referenceSequenceNumber = 1, UINT Count = 50)
	{
		SetObjectIdentifierObjectType(BACnetObjectTypeTrendLog);
		SetObjectIdentifierInstance(Instance);
		SetPropertyIdentifier(Property_logBuffer);
		SetPropertyArrayIndex(-1);
		SetSequenceNumber(referenceSequenceNumber);
		SetBySequenceNumberCount(Count);
		service.device.device_id = DeviceID;
		return read_range(&service) ? 0x0 : 0xFF;
	}

	BYTE ExecuteByTime(int DeviceID, int Instance, time_t ReferencedTime = 0, UINT Count = 50)
	{
		SetObjectIdentifierObjectType(BACnetObjectTypeTrendLog);
		SetObjectIdentifierInstance(Instance);
		SetPropertyIdentifier(Property_logBuffer);
		SetPropertyArrayIndex(-1);
		SetByTimeReferencedTime(ReferencedTime);
		SetByTimeCount(Count);
		service.device.device_id = DeviceID;
		return read_range(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetObjectIdentifierObjectType(BACnetObjectType data){request.objectIdentifier.objectType = data;}
	void SetObjectIdentifierInstance(int data){request.objectIdentifier.instance = data;}
	void SetPropertyIdentifier(BACnetPropertyIdentifier data){request.propertyIdentifier = data;}
	void SetPropertyArrayIndex(int data){request.propertyArrayIndex = data;}
	void SetSequenceNumber(UINT data)
	{
		request.range = BACnetReadRangeRequestRangeTypeSequenceNumber;
		request._range.bySequenceNumber.referenceSequenceNumber = data;
	}
	void SetBySequenceNumberCount(int data)
	{
		request.range = BACnetReadRangeRequestRangeTypeSequenceNumber;
		request._range.bySequenceNumber.count = data;
	}
	void SetByPositionReferencedIndex(UINT data)
	{
		request.range = BACnetReadRangeRequestRangeTypeByPosition;
		request._range.byPosition.referencedIndex = data;
	}
	void SetByPositionCount(int data)
	{
		request.range = BACnetReadRangeRequestRangeTypeByPosition;
		request._range.byPosition.count = data;
	}
	void SetByTimeReferencedTime(const time_t data)
	{
		request.range = BACnetReadRangeRequestRangeTypeByTimeNew;
		bacnetapi_set_time(&request._range.byTime.referencedTime, data);
	}
	void SetByTimeCount(int data)
	{
		request.range = BACnetReadRangeRequestRangeTypeByTimeNew;
		request._range.byTime.count = data;
	}
	void SetByTimeRangeBeginningTime(const time_t data)
	{
		request.range = BACnetReadRangeRequestRangeTypeTimeRange;
		bacnetapi_set_time(&request._range.timeRange.beginningTime, data);
	}
	void SetByTimeRangeEndingTime(const time_t data)
	{
		request.range = BACnetReadRangeRequestRangeTypeTimeRange;
		bacnetapi_set_time(&request._range.timeRange.endingTime, data);
	}
	void SetDeviceID(int data){service.device.device_id = data;}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType(){return request.objectIdentifier.objectType;}
	int GetObjectIdentifierInstance(int data){return request.objectIdentifier.instance;}
	BACnetPropertyIdentifier GetPropertyIdentifier(){return request.propertyIdentifier;}
	int GetPropertyArrayIndex(){return request.propertyArrayIndex;}
	UINT GetByPositionReferencedIndex()
	{
		if (request.range == BACnetReadRangeRequestRangeTypeByPosition)
			return request._range.byPosition.referencedIndex;
		return 0;
	}
	UINT GetByPositionCount()
	{
		if (request.range == BACnetReadRangeRequestRangeTypeByPosition)
			return request._range.byPosition.count;
		return 0;
	}

	time_t GetByTimeReferencedTime()
	{
		time_t val = 0;

		if (request.range == BACnetReadRangeRequestRangeTypeByTimeNew)
			bacnetapi_get_time(&val, &request._range.byTime.referencedTime);
		return val;
	}
	int GetByTimeCount()
	{
		if (request.range == BACnetReadRangeRequestRangeTypeByTimeNew)
			return request._range.byTime.count;
		return 0;
	}

	time_t GetByTimeRangeBeginningTime()
	{
		time_t val = 0;

		if (request.range == BACnetReadRangeRequestRangeTypeTimeRange)
			bacnetapi_get_time(&val, &request._range.timeRange.beginningTime);
		return val;
	}
	time_t GetByTimeRangeEndingTime()
	{
		time_t val = 0;

		if (request.range == BACnetReadRangeRequestRangeTypeTimeRange)
			bacnetapi_get_time(&val, &request._range.timeRange.endingTime);
		return val;
	}
	BACnetObjectType GetAckObjectIdentifierObjectType(){return ack.objectIdentifier.objectType;}
	int GetAckObjectIdentifierInstance(int data){return ack.objectIdentifier.instance;}
	BACnetPropertyIdentifier GetAckPropertyIdentifier(){return ack.propertyIdentifier;}
	int GetAckPropertyArrayIndex(){return ack.propertyArrayIndex;}
	UINT GetAckItemCount(){return ack.itemCount;}
	BYTE GetAckFirstSequencePresent(){return ack.firstSequenceNumberFlag;}
	UINT GetAckFirstSequence(){return ack.firstSequenceNumber;}
	BYTE GetAckFirstItem(){BYTE* a = resultFlags.GetElement(0); return a ? *a : 0;}
	BYTE GetAckLastItem(){BYTE* a = resultFlags.GetElement(1); return a ? *a : 0;}
	BYTE GetAckMoreItems(){BYTE* a = resultFlags.GetElement(2); return a ? *a : 0;}

	int GetDeviceID(){return service.device.device_id;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.readRange.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.readRange.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}
	CBACnetBitString& GetAckResultFlags(){return resultFlags;}
	CArrayOfBACnetLogRecord GetAckItemData(){return itemData;}
private:
	CBACnetBitString resultFlags;
	CArrayOfBACnetLogRecord itemData;
	BACnetServiceReadRangeRequest request;
	BACnetServiceReadRangeAck ack;
	BACnetError error;
	BACnetServiceReadRange service;
};

class CBACnetServiceUnconfirmedPrivateTransfer
{
public:
	CBACnetServiceUnconfirmedPrivateTransfer()
	{
		bacnapi_unconfirmed_private_transfer_initialize(service, request);
	}
	~CBACnetServiceUnconfirmedPrivateTransfer()
	{
		bacnapi_unconfirmed_private_transfer_destroy(service, request);
	}

	//Execution routines
	BYTE Execute(){return bacnapi_execute_unconfirmed_private_transfer(&service) ? 0x0 : 0xFF;}
	BYTE Execute(CBACnetAny ServiceParameters = BACnetNull, UINT VendorID = 123, UINT ServiceNumber = 0)
	{
		bacnapi_ClearBACnetDataType(&request.serviceParameters);
		if (bacnapi_access_BACnetData(&request.serviceParameters, ServiceParameters, -1, 1))
			return 0;
		return bacnapi_execute_unconfirmed_private_transfer(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetRequestVendorID(UINT data){request.vendorID = data;}
	void SetRequestServiceNumber(UINT data){request.serviceNumber = data;}

	//Get Properties
	UINT GetRequestVendorID(){return request.vendorID;}
	UINT GetRequestServiceNumber(){return request.serviceNumber;}

	CBACnetAny GetServiceParameters(){return CBACnetAny(&request.serviceParameters);}
private:
	BACnetServicePrivateTransferRequest request;
	BACnetServiceUnconfirmedPrivateTransfer service;
};

class CBACnetServiceUnconfirmedTextMessage
{
public:
	CBACnetServiceUnconfirmedTextMessage()
	{
		bacnapi_unconfirmed_text_message_initialize(service, request);
		request.messageClass = BACnetMessageClassUnknown;
	}
	~CBACnetServiceUnconfirmedTextMessage()
	{
		bacnapi_unconfirmed_text_message_destroy(service, request);
	}

	//Execution routines
	BYTE Execute(){return bacnapi_execute_unconfirmed_text_message(&service) ? 0x0 : 0xFF;}
	BYTE Execute(const char *Message, BACnetMessagePriority MessagePriority = BACnetMessagePriorityNormal)
	{
		SetMessage(Message);
		request.messagePriority = MessagePriority;
		return bacnapi_execute_unconfirmed_text_message(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetMessagePriority(BACnetMessagePriority data){request.messagePriority = data;}
	void SetMessageClassNumeric(UINT data)
	{
		if (request.messageClass == BACnetMessageClassCharacter)
			bacnapi_ClearBACnetDataType(&request._messageClass.character);
		request.messageClass = BACnetMessageClassCharacter;
		request._messageClass.numeric = data;
	}
	void SetMessageClassCharacter(const char *data)
	{
		request.messageClass = BACnetMessageClassCharacter;
		if (data)
			bacnapi_string_set(&request._messageClass.character, data);
	}
#ifdef WTL
	void SetMessageClassCharacter(const CString data)
	{
		request.messageClass = BACnetMessageClassCharacter;
		bacnapi_string_set(&request._messageClass.character, data.operator LPCTSTR());
	}
#endif
	void SetMessage(const char *data)
	{
		if (data)
			bacnapi_string_set(&request.message, data);
	}
#ifdef WTL
	void SetMessage(const CString data)
	{
		bacnapi_string_set(&request.message, data.operator LPCTSTR());
	}
#endif
	//Get Properties
	BACnetMessagePriority GetMessagePriority(){return request.messagePriority;}
#ifdef WTL
	CString GetMessageClassCharacter()
	{
		CString result;
		char *buf = NULL;

		if (request.messageClass == BACnetMessageClassCharacter)
		{
			//Use the bacnapi_string_get function to convert the BACnetDataType structure stored in value into a string
			bacnapi_string_get(&buf, &request._messageClass.character);
			result = buf;
			//Clear the memory in buf using the bacnapi_string_destroy function
			bacnapi_string_destroy(buf);
		}

		return result;
	}
	CString GetMessage()
	{
		CString result;
		char *buf = NULL;

		//Use the bacnapi_string_get function to convert the BACnetDataType structure stored in value into a string
		bacnapi_string_get(&buf, &request.message);
		result = buf;
		//Clear the memory in buf using the bacnapi_string_destroy function
		bacnapi_string_destroy(buf);

		return result;
	}
#endif

private:
	BACnetServiceTextMessageRequest request;
	BACnetServiceUnconfirmedTextMessage service;
};

class CBACnetServiceTimeSynchronization
{
public:
	CBACnetServiceTimeSynchronization()
	{
		memset(&service, 0, sizeof(service));
		m_Value = &service;
	}
	CBACnetServiceTimeSynchronization(BACnetServiceTimeSynchronization *pSrc)
	{
		m_Value = pSrc;
	}
	~CBACnetServiceTimeSynchronization(){}

	//Execution routines
	BYTE Execute(const timeb *time)
	{
		bacnetapi_set_timeb(&service.time, time);
		return bacnapi_execute_time_synchronization(&service) ? 0x0 : 0xFF;
	}

	void GetTime(timeb *value)
	{
		bacnetapi_get_timeb(value, &m_Value->time);
	}

private:
	BACnetServiceTimeSynchronization service;
	BACnetServiceTimeSynchronization *m_Value;
};

class CBACnetServiceUtcTimeSynchronization
{
public:
	CBACnetServiceUtcTimeSynchronization()
	{
		memset(&service, 0, sizeof(service));
		m_Value = &service;
	}
	CBACnetServiceUtcTimeSynchronization(BACnetServiceUtcTimeSynchronization *pSrc)
	{
		m_Value = pSrc;
	}
	~CBACnetServiceUtcTimeSynchronization()
	{
		bacnapi_ClearUtcTimeSynchronization(&service);
	}

	//Execution routines
	BYTE Execute(const timeb *time)
	{
		bacnetapi_set_timeb(&service.time, time);
		return bacnapi_execute_utc_time_synchronization(&service) ? 0x0 : 0xFF;
	}

private:
	BACnetServiceUtcTimeSynchronization service;
	BACnetServiceUtcTimeSynchronization *m_Value;
};

class CBACnetServiceWhoIs
{
public:
	CBACnetServiceWhoIs()
	{
		memset(&service, 0, sizeof(service));
		m_Value = &service;
	}
	CBACnetServiceWhoIs(BACnetServiceWhoIs *pSrc)
	{
		m_Value = pSrc;
	}
	~CBACnetServiceWhoIs(){}

	//Execution routines
	BYTE Execute(int lowLimit = -1, int highLimit = -1)
	{
		service.lowLimit = lowLimit;
		service.highLimit = highLimit;
		return bacnapi_execute_who_is(&service) ? 0x0 : 0xFF;
	}

	//Get Functions
	int GetLowLimit(){return m_Value->lowLimit;}
	int GetHighLimit(){return m_Value->highLimit;}

private:
	BACnetServiceWhoIs service;
	BACnetServiceWhoIs *m_Value;
};

class CBACnetServiceIam
{
public:
	CBACnetServiceIam()
	{
		memset(&service, 0, sizeof(service));
		m_Value = &service;
	}
	CBACnetServiceIam(BACnetServiceIam *pSrc)
	{
		m_Value = pSrc;
	}
	~CBACnetServiceIam(){}

	BYTE Execute(int DeviceID, int network, BYTE* MacAddress, int MacAddressLen, UINT VendorID = 123, BACnetSegmentation SegmentationSupported = BACnetSegmentationSegmentedBoth, UINT MaxAPDULengthAccepted = 480)
	{
		bacnet_address address;

		address.network = network;
		address.mac.length = MacAddressLen;
		memcpy(address.mac.contents, MacAddress, MacAddressLen);

		service.iAmDeviceIdentifier.objectType = BACnetObjectTypeDevice;
		service.iAmDeviceIdentifier.instance = DeviceID;
		service.maxAPDULengthAccepted = MaxAPDULengthAccepted;
		service.segmentationSupported = SegmentationSupported;
		service.vendorID = VendorID;
		return bacnapi_execute_iam_ex(&address, &service) ? 0x0 : 0xFF;
	}

	//Get Properties
	BACnetObjectType GetDeviceIdentifierObjectType(){return m_Value->iAmDeviceIdentifier.objectType;}
	int GetDeviceIdentifierInstance(){return m_Value->iAmDeviceIdentifier.instance;}
	BACnetSegmentation GetSegmentationSupported(){return m_Value->segmentationSupported;}
	UINT GetVendorID(){return m_Value->vendorID;}

private:
	BACnetServiceIam service;
	BACnetServiceIam *m_Value;
};

class CBACnetServiceTextMessageRequest
{
public:
	CBACnetServiceTextMessageRequest(BACnetServiceTextMessageRequest *pSrc){service = pSrc;}
	~CBACnetServiceTextMessageRequest(){}

	//Get Properties
	BACnetMessagePriority GetMessagePriority(){return service->messagePriority;}
	const char* GetMessage()
	{
		if (service->message.type == bacnetDT_CharacterString)
			return service->message._type.String;
		else
			return "";
	}
#ifdef WTL
	CString GetMessageClassCharacter()
	{
		CString result;
		char *buf = NULL;

		if (service->messageClass == BACnetMessageClassCharacter)
		{
			//Use the bacnapi_string_get function to convert the BACnetDataType structure stored in value into a string
			bacnapi_string_get(&buf, &service->_messageClass.character);
			result = buf;
			//Clear the memory in buf using the bacnapi_string_destroy function
			bacnapi_string_destroy(buf);
		}

		return result;
	}
	CString GetMessage()
	{
		CString result;
		char *buf = NULL;

		//Use the bacnapi_string_get function to convert the BACnetDataType structure stored in value into a string
		bacnapi_string_get(&buf, &service->message);
		result = buf;
		//Clear the memory in buf using the bacnapi_string_destroy function
		bacnapi_string_destroy(buf);

		return result;
	}
#endif
private:
	BACnetServiceTextMessageRequest *service;
};

class CBACnetServiceIHave
{
public:
	CBACnetServiceIHave(BACnetServiceIHave *pSrc)
	{
		temp_buf = NULL;
		service = pSrc;
	}
	~CBACnetServiceIHave()
	{
		if (temp_buf)
			bacnapi_string_destroy(temp_buf);
		temp_buf = NULL;
	}

	//Get Properties
	BACnetObjectType GetDeviceIdentifierObjectType(){return service->deviceIdentifier.objectType;}
	int GetDeviceIdentifierInstance(){return service->deviceIdentifier.instance;}
	BACnetObjectType GetObjectIdentifierObjectType(){return service->objectIdentifier.objectType;}
	int GetObjectIdentifierInstance(){return service->objectIdentifier.instance;}

	const char* GetObjectName()
	{
		//Use the bacnapi_string_get function to convert the BACnetDataType structure stored in value into a string
		if (temp_buf == NULL)
			bacnapi_string_get(&temp_buf, &service->objectName);
		return temp_buf;
	}
#ifdef WTL
	CString GetObjectName()
	{
		CString result;
		char *buf = NULL;

		//Use the bacnapi_string_get function to convert the BACnetDataType structure stored in value into a string
		bacnapi_string_get(&buf, &service->objectName);
		result = buf;
		//Clear the memory in buf using the bacnapi_string_destroy function
		bacnapi_string_destroy(buf);

		return result;
	}
#endif

private:
	char *temp_buf;
	BACnetServiceIHave *service;
};

class CBACnetServicePrivateTransferRequest
{
public:
	CBACnetServicePrivateTransferRequest(BACnetServicePrivateTransferRequest *pSrc)
	{
		m_Value = pSrc;
	}

	//Get Properties
	UINT GetVendorID(){return m_Value->vendorID;}
	UINT GetServiceNumber(){return m_Value->serviceNumber;}

	//Set Properties
	UINT SetRequestVendorID(){return m_Value->vendorID;}
	UINT SetRequestServiceNumber(){return m_Value->serviceNumber;}
	UINT SetAckVendorID(){return m_Value->vendorID;}
	UINT SetAckServiceNumber(){return m_Value->serviceNumber;}

	CBACnetAny GetServiceParameters(){return CBACnetAny(&m_Value->serviceParameters);}
private:
	BACnetServicePrivateTransferRequest *m_Value;
};

class CBACnetServiceWhoHas
{
public:
	CBACnetServiceWhoHas()
	{
		memset(&service, 0, sizeof(service));
		m_Value = &service;
	}
	CBACnetServiceWhoHas(BACnetServiceWhoHas *pSrc)
	{
		m_Value = pSrc;
	}
	~CBACnetServiceWhoHas()
	{
		bacnapi_ClearBACnetDataType(&service.object.objectName);
	}

	//Execution routines
	BYTE Execute(int deviceRangeLowLimit = -1, int deviceRangeHighLimit = -1)
	{
		service.limits.deviceRangeLowLimit = deviceRangeLowLimit;
		service.limits.deviceRangeHighLimit = deviceRangeHighLimit;
		return bacnapi_execute_who_has(&service) ? 0x0 : 0xFF;
	}

	void SetObjectIdentifierObjectType(BACnetObjectType data)
	{
		bacnapi_ClearBACnetDataType(&service.object.objectName);
		service.objectChoice = ObjectChoiceObjectIdentifier;
		service.object.objectIdentifier.objectType = data;
	}
	void SetObjectIdentifierInstance(int data)
	{
		bacnapi_ClearBACnetDataType(&service.object.objectName);
		service.objectChoice = ObjectChoiceObjectIdentifier;
		service.object.objectIdentifier.instance = data;
	}
	void SetObjectName(const char *data)
	{
		service.objectChoice = ObjectChoiceObjectName;
		if (data)
			bacnapi_string_set(&service.object.objectName, data);
	}
#ifdef WTL
	void SetObjectName(const CString data)
	{
		service.objectChoice = ObjectChoiceObjectName;
		bacnapi_string_set(&service.object.objectName, data.operator LPCTSTR());
	}
#endif
	//Get Functions
	int GetLowLimit(){return m_Value->limits.deviceRangeLowLimit;}
	int GetHighLimit(){return m_Value->limits.deviceRangeLowLimit;}
	BACnetObjectChoice GetObjectChoice(){return m_Value->objectChoice;}
	BACnetObjectType GetObjectIdentifierObjectType()
	{
		if (m_Value->objectChoice != ObjectChoiceObjectIdentifier)
			return BACnetObjectTypeNone;

		return m_Value->object.objectIdentifier.objectType;
	}
	int GetObjectIdentifierInstance(int data)
	{
		if (m_Value->objectChoice != ObjectChoiceObjectIdentifier)
			return -1;

		return m_Value->object.objectIdentifier.instance;
	}
#ifdef WTL
	CString GetObjectName()
	{
		CString result;
		char *buf = NULL;

		if (m_Value->objectChoice != ObjectChoiceObjectName)
			throw exception("Invalid Type");

		//Use the bacnapi_string_get function to convert the BACnetDataType structure stored in value into a string
		bacnapi_string_get(&buf, &m_Value->object.objectName);
		result = buf;
		//Clear the memory in buf using the bacnapi_string_destroy function
		bacnapi_string_destroy(buf);

		return result;
	}
#endif

private:
	BACnetServiceWhoHas service;
	BACnetServiceWhoHas *m_Value;
};

class CBACnetServiceCOVNotificationRequest
{
public:
	CBACnetServiceCOVNotificationRequest(){}
	CBACnetServiceCOVNotificationRequest(BACnetServiceCOVNotificationRequest* pSrc)
	{
		m_Value = pSrc;
		if (m_Value)
			listOfValues = CSequenceOfBACnetPropertyValue(&m_Value->listOfValues);
	}

	//Get Properties
	UINT GetSubscriberProcessIdentifier()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->subscriberProcessIdentifier;
	}
	BACnetObjectType GetInitiatingDeviceObjectType()
	{
		if (m_Value == NULL)
			return BACnetObjectTypeNone;
		return m_Value->initiatingDeviceIdentifier.objectType;
	}
	int GetInitiatingDeviceObjectInstance()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->initiatingDeviceIdentifier.instance;
	}
	BACnetObjectType GetMonitoredObjectIdentifierObjectType()
	{
		if (m_Value == NULL)
			return (BACnetObjectType)0;
		return m_Value->monitoredObjectIdentifier.objectType;
	}
	int GetMonitoredObjectIdentifierObjectInstance()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->monitoredObjectIdentifier.instance;
	}
	UINT GetTimeRemaining()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->timeRemaining;
	}

	CSequenceOfBACnetPropertyValue& GetListOfValues(){return listOfValues;}
private:
	CSequenceOfBACnetPropertyValue listOfValues;
	BACnetServiceCOVNotificationRequest* m_Value;
};

class CBACnetServiceEventNotificationRequest
{
public:
	CBACnetServiceEventNotificationRequest()
	{
		temp_buf = NULL;
	}
	CBACnetServiceEventNotificationRequest(BACnetServiceEventNotificationRequest* pSrc)
	{
		temp_buf = NULL;
		m_Value = pSrc;
		if (m_Value)
			eventValues = CBACnetNotificationParameters(&m_Value->eventValues);
	}
	~CBACnetServiceEventNotificationRequest()
	{
		if (temp_buf)
			bacnapi_string_destroy(temp_buf);
		temp_buf = NULL;
	}

	//Get Properties
	BACnetObjectType GetInitiatingDeviceIdentifierObjectType()
	{
		if (m_Value == NULL)
			return BACnetObjectTypeNone;
		return m_Value->initiatingDeviceIdentifier.objectType;
	}
	int GetInitiatingDeviceIdentifierObjectInstance()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->initiatingDeviceIdentifier.instance;
	}
	BACnetObjectType GetEventObjectIdentifierObjectType()
	{
		if (m_Value == NULL)
			return BACnetObjectTypeNone;
		return m_Value->eventObjectIdentifier.objectType;
	}
	int GetEventObjectIdentifierObjectInstance()
	{
		if (m_Value == NULL)
			return -1;
		return m_Value->eventObjectIdentifier.instance;
	}
	CBACnetTimeStamp GetTimeStamp()
	{
		if (m_Value == NULL)
			return CBACnetTimeStamp();
		return CBACnetTimeStamp(&m_Value->timeStamp);
	}
	UINT GetNotificationClass()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->notificationClass;
	}
	UINT GetPriority()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->priority;
	}
	BACnetEventType GetEventType()
	{
		if (m_Value == NULL)
			return (BACnetEventType)0;
		return m_Value->eventType;
	}
	const char* GetMessageText()
	{
		//Use the bacnapi_string_get function to convert the BACnetDataType structure stored in value into a string
		if (temp_buf == NULL)
			bacnapi_string_get(&temp_buf, &m_Value->messageText);
		return temp_buf;
	}
#ifdef WTL
	CString GetMessageText()
	{
		CString result;
		char *buf = NULL;

		if (m_Value == NULL)
			throw exception("Structure is not initialized");

		//Use the bacnapi_string_get function to convert the BACnetDataType structure stored in value into a string
		bacnapi_string_get(&buf, &m_Value->messageText);
		result = buf;
		//Clear the memory in buf using the bacnapi_string_destroy function
		bacnapi_string_destroy(buf);

		return result;
	}
#endif
	BACnetNotifyType GetNotifyType()
	{
		if (m_Value == NULL)
			return (BACnetNotifyType)0;
		return m_Value->notifyType;
	}
	BYTE GetAckRequired()
	{
		if (m_Value == NULL)
			return 0;
		return m_Value->ackRequired;
	}
	BACnetEventState GetFromState()
	{
		if (m_Value == NULL)
			return (BACnetEventState)0;
		return m_Value->fromState;
	}
	BACnetEventState GetToState()
	{
		if (m_Value == NULL)
			return (BACnetEventState)0;
		return m_Value->toState;
	}

	CBACnetNotificationParameters& GetEventValues(){return eventValues;}
private:
	char *temp_buf;
	CBACnetNotificationParameters eventValues;
	BACnetServiceEventNotificationRequest* m_Value;
};

class CBACnetServiceLifeSafetyOperation
{
public:
	CBACnetServiceLifeSafetyOperation()
	{
		bacnapi_life_safety_operation_initialize(service, request, error);
	}
	~CBACnetServiceLifeSafetyOperation()
	{
		bacnapi_life_safety_operation_destroy(service, request, error);
	}

	//Execution routines
	BYTE Execute(){return life_safety_operation(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, UINT RequestingProcessIdentifier, const char* RequestingSource, BACnetLifeSafetyOperation Request, BYTE ObjectIdentifierFlag = 0, BACnetObjectType ObjectType = BACnetObjectTypeLifeSafetyPoint, int Instance = 0)
	{
		service.device.device_id = DeviceID;
		request.requestingProcessIdentifier = RequestingProcessIdentifier;
		SetRequestingSource(RequestingSource);
		request.objectIdentifier.objectType = ObjectType;
		request.objectIdentifier.instance = Instance;
		request.objectIdentifierFlag = ObjectIdentifierFlag;
		request.request = Request;
		return life_safety_operation(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetObjectIdentifierObjectType(BACnetObjectType data){request.objectIdentifierFlag = 0xFF; request.objectIdentifier.objectType = data;}
	void SetObjectIdentifierInstance(int data){request.objectIdentifierFlag = 0xFF; request.objectIdentifier.instance = data;}
	void SetObjectIdentifierFlag(BYTE data){request.objectIdentifierFlag = data;}
	void SetRequest(BACnetLifeSafetyOperation data){request.request = data;}
	void SetRequestingProcessIdentifier(UINT data){request.requestingProcessIdentifier = data;}
	void SetRequestingSource(const char *data)
	{
		if (data)
			bacnapi_string_set(&request.requestingSource, data);
	}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType(){return request.objectIdentifier.objectType;}
	int GetObjectIdentifierInstance(int data){return request.objectIdentifier.instance;}
	BYTE GetObjectIdentifierFlag(){return request.objectIdentifierFlag;}
	BACnetLifeSafetyOperation GetRequest(){return request.request;}
	UINT GetRequestingProcessIdentifier(){return request.requestingProcessIdentifier;}
	int GetDeviceID(){return service.device.device_id;}
	const char* GetRequestingSource()
	{
		if (request.requestingSource.type == bacnetDT_CharacterString)
			return request.requestingSource._type.String;
		else
			return "";
	}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.lifeSafetyOperation.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.lifeSafetyOperation.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}

private:
	BACnetServiceLifeSafetyOperationRequest request;
	BACnetError error;
	BACnetServiceLifeSafetyOperation service;
};

class CBACnetServiceSubscribeCOVProperty
{
public:
	CBACnetServiceSubscribeCOVProperty()
	{
		bacnapi_subscribe_cov_property_initialize(service, request, error);
		request.issueConfirmedNotifications = 0xFF;
		request.lifetime = 300;
	}
	~CBACnetServiceSubscribeCOVProperty()
	{
		bacnapi_subscribe_cov_property_destroy(service, request, error);
	}

	//Execution routines
	BYTE Execute(){return subscribe_cov_property(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, BACnetObjectType ObjectType, int Instance, BACnetPropertyIdentifier PropertyIdentifier, int PropertyArrayIndex = -1, BYTE CovIncrementPresentFlag = 0, float CovIncrement = 0, UINT subscriberProcessIdentifier = 1, BYTE issueConfirmedNotifications = 0xFF, int lifetime = 600)
	{
		service.device.device_id = DeviceID;
		request.subscriberProcessIdentifier = subscriberProcessIdentifier;
		request.monitoredObjectIdentifier.objectType = ObjectType;
		request.monitoredObjectIdentifier.instance = Instance;
		request.issueConfirmedNotifications = issueConfirmedNotifications;
		request.lifetime = lifetime;
		request.monitoredPropertyIdentifier.propertyIdentifier = PropertyIdentifier;
		request.monitoredPropertyIdentifier.propertyArrayIndex = PropertyArrayIndex;
		request.covIncrement = CovIncrement;
		request.covIncrementPresentFlag = CovIncrementPresentFlag;
		return subscribe_cov_property(&service) ? 0x0 : 0xFF;
	}
	//Set Properties
	void SetObjectIdentifierObjectType(BACnetObjectType data){request.monitoredObjectIdentifier.objectType = data;}
	void SetObjectIdentifierInstance(int data){request.monitoredObjectIdentifier.instance = data;}
	void SetSubscriberProcessIdentifier(UINT data){request.subscriberProcessIdentifier = data;}
	void SetIssueConfirmedNotifications(int data){request.issueConfirmedNotifications = data;}
	void SetLifetime(int data){request.lifetime = data;}
	void SetDeviceID(int data){service.device.device_id = data;}
	void SetPropertyIdentifier(BACnetPropertyIdentifier data){request.monitoredPropertyIdentifier.propertyIdentifier = data;}
	void SetPropertyArrayIndex(int data){request.monitoredPropertyIdentifier.propertyArrayIndex = data;}
	void SetCovIncrement(float data){request.covIncrement = data;}
	void SetCovIncrementPresentFlag(BYTE data){request.covIncrementPresentFlag = data;}

	//Get Properties
	BACnetObjectType GetObjectIdentifierObjectType(){return request.monitoredObjectIdentifier.objectType;}
	int GetObjectIdentifierInstance(int data){return request.monitoredObjectIdentifier.instance;}
	UINT GetSubscriberProcessIdentifier(){return request.subscriberProcessIdentifier;}
	BYTE GetIssueConfirmedNotifications(){return request.issueConfirmedNotifications;}
	int GetLifetime(){return request.lifetime;}
	int GetDeviceID(){return service.device.device_id;}
	BACnetPropertyIdentifier GetPropertyIdentifier(){return request.monitoredPropertyIdentifier.propertyIdentifier;}
	int GetPropertyArrayIndex(){return request.monitoredPropertyIdentifier.propertyArrayIndex;}
	float GetCovIncrement(){return request.covIncrement;}
	BYTE GetCovIncrementPresentFlag(){return request.covIncrementPresentFlag;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.subscribeCOVProperty.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.subscribeCOVProperty.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}

private:
	BACnetServiceSubscribeCOVPropertyRequest request;
	BACnetError error;
	BACnetServiceSubscribeCOVProperty service;
};

class CBACnetServiceGetEventInformation
{
public:
	CBACnetServiceGetEventInformation()
	{
		bacnapi_get_event_information_initialize(service, request, ack, error);
		listOfEventSummaries = CSequenceOfEventSummaryAck(&ack.listOfEventSummaries);
	}
	~CBACnetServiceGetEventInformation()
	{
		bacnapi_get_event_information_destroy(service, request, ack, error);
	}

	//Execution routines
	BYTE Execute(){return get_event_information(&service) ? 0x0 : 0xFF;}
	BYTE Execute(int DeviceID, BACnetObjectType ObjectType = BACnetObjectTypeDevice, int Instance = -1)
	{
		if (Instance > -1)
		{
			request.lastReceivedObjectIdentifierPresentFlag = 0xFF;
			request.lastReceivedObjectIdentifier.objectType = ObjectType;
			request.lastReceivedObjectIdentifier.instance = Instance;
		}
		service.device.device_id = DeviceID;
		return get_event_information(&service) ? 0x0 : 0xFF;
	}

	//Set Properties
	void SetDeviceID(int data){service.device.device_id = data;}
	void SetObjectIdentifierObjectType(BACnetObjectType data){request.lastReceivedObjectIdentifier.objectType = data;}
	void SetObjectIdentifierInstance(int data){request.lastReceivedObjectIdentifier.instance = data;}
	void SetlastReceivedObjectIdentifierPresentFlag(BYTE data){request.lastReceivedObjectIdentifierPresentFlag = data;}

	//Get Properties
	int GetDeviceID(){return service.device.device_id;}
	BACnetObjectType GetObjectIdentifierObjectType(){return request.lastReceivedObjectIdentifier.objectType;}
	int GetObjectIdentifierInstance(){return request.lastReceivedObjectIdentifier.instance;}
	BYTE GetLastReceivedObjectIdentifierPresentFlag(){return request.lastReceivedObjectIdentifierPresentFlag;}

	//Error Codes
	BACnetErrorCode GetErrorCode(){return error._choice.getAlarmSummary.errorCode;}
	BACnetErrorClass GetErrorClass(){return error._choice.getAlarmSummary.errorClass;}
	const char* GetErrorCodeString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorCode();}
	const char* GetErrorClassString(){return CBACnetError(GetErrorCode(), GetErrorClass()).GetErrorClass();}
	CSequenceOfEventSummaryAck& GetSequenceOfEventSummariesAck(){return listOfEventSummaries;}
private:
	CSequenceOfEventSummaryAck listOfEventSummaries;
	BACnetServiceGetEventInformationRequest request;
	BACnetServiceGetEventInformationAck ack;
	BACnetError error;
	BACnetServiceGetEventInformation service;
};

class CBACnetDatalinkLayer
{
public:
	CBACnetDatalinkLayer(){}
	CBACnetDatalinkLayer(datalink_init* pSrc)
	{
		m_Value = pSrc;
	}
	virtual ~CBACnetDatalinkLayer(){}

	void Init()
	{
		memset(m_Value, 0, sizeof(*m_Value));
	}
	BYTE Add(char* connect_string)
	{
		char *con = NULL;

		con = GetNextConnectString();
		if (con == NULL)
			return false;
		sprintf(con, connect_string);
		return 0xFF;
	}
	BYTE AddIP(int Network, UINT UDPPort = 47808, char * IPAddress = NULL, char * SubnetMask = NULL, char * BBMDAddress = NULL, UINT BBMDUDPPort = 47808, UINT BBMDMaxForeignDevices = 0, UINT ForegnDeviceExpiryTime = 120, UINT Eth = 0, const char* description = NULL)
	{
		char *connect_string = NULL;

		connect_string = GetNextConnectString();
		if (connect_string == NULL)
			return false;

		sprintf(connect_string, "TYPE=BACNETIP;NETWORK=%d;UDPPORT=%d;ETH=%d;BBMDMAXFOREIGNDEVICES=%d;FOREIGNDEVICEEXPIRYTIME=%d;", Network, UDPPort, Eth, BBMDMaxForeignDevices, ForegnDeviceExpiryTime);
		if (IPAddress)
			sprintf(connect_string, "%sIPADDRESS=%s;", connect_string, IPAddress);
		if (SubnetMask)
			sprintf(connect_string, "%sSUBNETMASK=%s;", connect_string, SubnetMask);
		if (BBMDAddress)
			sprintf(connect_string, "%sBBMDIPADDRESS=%s;BBMDPORTNO=%d;", connect_string, BBMDAddress, BBMDUDPPort);
		if (description)
			sprintf(connect_string, "%sDESCRIPTION=%s;", connect_string, description);
		return 0xFF;
	}
	BYTE AddAPI(int Network = 99999, UINT Port = 5003, const char* description = NULL)
	{
		char *connect_string = NULL;

		connect_string = GetNextConnectString();
		if (connect_string == NULL)
			return false;

		sprintf(connect_string, "TYPE=BACNETLEGACYAPI;NETWORK=%d;PORT=%d;", Network, Port);
		if (description)
			sprintf(connect_string, "%sDESCRIPTION=%s;", connect_string, description);
		return 0xFF;
	}
	BYTE AddEthernet(int Network, UINT NIC = 0, const char* description = NULL)
	{
		char *connect_string = NULL;

		connect_string = GetNextConnectString();
		if (connect_string == NULL)
			return false;

		sprintf(connect_string, "TYPE=BACNETETHERNET;NETWORK=%d;NIC=%d;", Network, NIC);
		if (description)
			sprintf(connect_string, "%sDESCRIPTION=%s;", connect_string, description);
		return 0xFF;
	}

	BYTE AddMSTP(int Network, UINT Port, UINT Baudrate = 9600, UINT Databits = 8, UINT Stopbits = 0, UINT Handshaking = 0, UINT TS = 0, UINT Slot = 10, UINT MaxInfoFrames = 50, UINT MaxMaster = 100, UINT MinOctets = 4, UINT NoToken = 500, UINT Poll = 50, UINT ReplyTimeout = 300, UINT RetryToken = 1, UINT UsageDelay = 15, UINT UseageTimeout = 100, const char* description = NULL)
	{
		char *connect_string = NULL;

		connect_string = GetNextConnectString();
		if (connect_string == NULL)
			return false;

		sprintf(connect_string, "TYPE=BACNETMSTP;NETWORK=%d;PORT=%d;BAUDRATE=%d;DATABITS=%d;STOPBITS=%d;HANDSHAKING=%d;TS=%d;SLOT=%d;MAX_INFO_FRAMES=%d;MAX_MASTER=%d;MIN_OCTETS=%d;NO_TOKEN=%d;POLL=%d;REPLY_TIMEOUT=%d;RETRY_TOKEN=%d;USEAGE_DELAY=%d;USEAGE_TIMEOUT=%d;", Network, Port, Baudrate, Databits, Stopbits, Handshaking, TS, Slot, MaxInfoFrames, MaxMaster, MinOctets, NoToken, Poll, ReplyTimeout, RetryToken, UsageDelay, UseageTimeout);
		if (description)
			sprintf(connect_string, "%sDESCRIPTION=%s;", connect_string, description);
		return 0xFF;
	}

	BYTE AddPTP(int Network, UINT Port, char* DialString = NULL, char* Password = NULL, UINT Baudrate = 9600, UINT Databits = 8, UINT Stopbits = 0, UINT Handshaking = 0, const char* description = NULL)
	{
		char *connect_string = NULL;

		connect_string = GetNextConnectString();
		if (connect_string == NULL)
			return false;

		sprintf(connect_string, "TYPE=BACNETPTP;NETWORK=%d;PORT=%d;BAUDRATE=%d;DATABITS=%d;STOPBITS=%d;HANDSHAKING=%d;", Network, Port, Baudrate, Databits, Stopbits, Handshaking);
		if (DialString)
			sprintf(connect_string, "%sDIALSTRING=%s;", connect_string, DialString);
		if (Password)
			sprintf(connect_string, "%sPASSWORD=%s;", connect_string, Password);
		if (description)
			sprintf(connect_string, "%sDESCRIPTION=%s;", connect_string, description);
		return 0xFF;
	}

	BYTE AddOutOfProcess(char *Host = NULL, const char* description = NULL)
	{
		char *connect_string = NULL;

		connect_string = GetNextConnectString();
		if (connect_string == NULL)
			return false;

		if (Host)
			sprintf(connect_string, "TYPE=BACNETOUTOFPROCESS;HOST=%s;", Host);
		else
			sprintf(connect_string, "TYPE=BACNETOUTOFPROCESS;");
		return 0xFF;
	}

	BYTE AddAnexH(int Network, const char* description = NULL)
	{
		char *connect_string = NULL;

		connect_string = GetNextConnectString();
		if (connect_string == NULL)
			return false;

		sprintf(connect_string, "TYPE=BACNETANEXH;NETWORK=%d;", Network);
		if (description)
			sprintf(connect_string, "%s;DESCRIPTION=%s;", connect_string, description);
		return 0xFF;
	}

	BYTE GetLoadFromDatabase(){return m_Value->load_from_database;}
	void SetLoadFromDatabase(BYTE data){m_Value->load_from_database = data;}

	const char* GetLastError()
	{
		for (int i=0; i < 128; i++)
		{
			if (m_Value->connect_strings[i].error_message[0] == '\0')
				return m_Value->connect_strings[i].error_message;
		}
		return "";
	}
#ifdef WTL
	CString GetLastError()
	{
		for (int i=0; i < 128; i++)
		{
			if (m_Value->connect_strings[i].error_message[0] == '\0')
				return CString(m_Value->connect_strings[i].error_message);
		}
		return CString("");
	}
#endif

private:
	datalink_init* m_Value;
	char *GetNextConnectString()
	{
		for (int i=0; i < 128; i++)
		{
			if (m_Value->connect_strings[i].connect_string == NULL || m_Value->connect_strings[i].connect_string[0] == '\0')
			{
				return (char*)bacnapi_allocate_connect_string(m_Value, i, 2056);
			}
		}
		return NULL;
	}
};

class CBACnetLogging
{
public:
	CBACnetLogging(){}
	CBACnetLogging(bacnet_logging* pSrc){logging = pSrc;}

	void SetLogFunction(log_fn a){logging->log =a;}
	void SetLogFileName(char *FileName){copy_string(&logging->file_name, FileName);}
	void SetApplication_layer(BYTE data){logging->application_layer = data;}
	void SetNetwork(BYTE data){logging->network = data;}
	void SetClientTSM(BYTE data){logging->client_tsm = data;}
	void SetServerTSM(BYTE data){logging->server_tsm = data;}
	void SetServiceIndication(BYTE data){logging->service_indication = data;}
	void SetAddressBinding(BYTE data){logging->address_binding = data;}
	void SetEthernet(BYTE data){logging->ethernet = data;}
	void SetAnex_h(BYTE data){logging->anex_h = data;}
	void SetIP(BYTE data){logging->ip = data;}
	void SetMSTP(BYTE data){logging->mstp = data;}
	void SetPTP(BYTE data){logging->ptp = data;}
	void SetAPI(BYTE data){logging->api = data;}
	void SetDataLink(BYTE data){logging->datalink = data;}
	void SetDatatypeUtils(BYTE data){logging->datatype_utils = data;}
	void SetDriver(BYTE data){logging->driver = data;}
	void SetMem(BYTE data){logging->mem = data;}
	void SetObject(BYTE data){logging->object = data;}
	void SetCache(BYTE data){logging->cache = data;}
	void SetMAC(BYTE data){logging->mac = data;}
	void SetObjectAPI(BYTE data){logging->object_api = data;}
	void SetSerialize(BYTE data){logging->serialize = data;}
	void SetPDU(BYTE data){logging->pdu = data;}
	void SetDatabase(BYTE data){logging->database = data;}

	const char* GetLogFileName(){return logging->file_name;}
	BYTE GetApplication_layer(){return logging->application_layer;}
	BYTE GetNetwork(){return logging->network;}
	BYTE GetClientTSM(){return logging->client_tsm;}
	BYTE GetServerTSM(){return logging->server_tsm;}
	BYTE GetServiceIndication(){return logging->service_indication;}
	BYTE GetAddressBinding(){return logging->address_binding;}
	BYTE GetEthernet(){return logging->ethernet;}
	BYTE GetAnex_h(){return logging->anex_h;}
	BYTE GetIP(){return logging->ip;}
	BYTE GetMSTP(){return logging->mstp;}
	BYTE GetPTP(){return logging->ptp;}
	BYTE GetAPI(){return logging->api;}
	BYTE GetDataLink(){return logging->datalink;}
	BYTE GetDatatypeUtils(){return logging->datatype_utils;}
	BYTE GetDriver(){return logging->driver;}
	BYTE GetMem(){return logging->mem;}
	BYTE GetObject(){return logging->object;}
	BYTE GetCache(){return logging->cache;}
	BYTE GetMAC(){return logging->mac;}
	BYTE GetObjectAPI(){return logging->object_api;}
	BYTE GetSerialize(){return logging->serialize;}
	BYTE GetPDU(){return logging->pdu;}
	BYTE GetDatabase(){return logging->database;}

private:
	bacnet_logging *logging;
	int copy_string(char** destination, char* source)
	{
		BACnetDataType tmp;
		tmp.type = bacnetDT_CharacterString;
		tmp._type.String = source;
		return bacnapi_access_string(destination, &tmp, 1);
	}
};

class CBACnetDriver
{
public:
	CBACnetDriver()
	{
		memset(&driver, 0, sizeof(driver));
		driver.device.numberOfAPDUretries = 3;
		driver.device.apduTimeout = 6000;
		driver.device.apduSegmentTimeout = 6000;

		initialized = 0x0;
		dataLinkLayers = CBACnetDatalinkLayer(&driver.datalink);
		logging = CBACnetLogging(&driver.logging);
	}
	virtual ~CBACnetDriver()
	{
		Destroy();
	}

	BYTE Save(char *file_name = NULL){return bacnapi_save_config(&driver, file_name) == 0 ? 0xFF : 0x0;}
	BYTE Load(char *file_name = NULL)
	{
		int trace_flag = 0;
		int daemon_flag = 0;
		int simulator_enable_flag = 0;

		if (bacnapi_config_file_exists())
		{
			bacnapi_load_config(&driver, file_name, &daemon_flag, &simulator_enable_flag, &trace_flag);
			bacnapi_edit_device(file_name);
			bacnapi_edit_ports(file_name);
		}
		return bacnapi_load_config(&driver, file_name, &daemon_flag, &simulator_enable_flag, &trace_flag) == 0 ? 0xFF : 0x0;
	}
	int EditPorts(char *file_name = NULL){return bacnapi_edit_ports(file_name);}
	int EditDevice(char *file_name = NULL){return bacnapi_edit_device(file_name);}
	//Set Properties
	void SetDeviceID(int data){driver.device_id = data;}

	void SetSystemStatus(BACnetDeviceStatus data){driver.system_status = data;}
	BACnetDeviceStatus GetSystemStatus(){return driver.system_status;}

	void SetDatabaseFileName(char* data)
	{
		copy_string(&driver.device.database_file_name, data);
		driver.device.load_virtual_networks = 1;
		driver.device.load_objects = 1;
	}
	const char* GetScriptFileName(){return driver.device.script_file_name;}
	void SetScriptFileName(char* data){copy_string(&driver.device.script_file_name, data);}

	const char* GetDatabaseFileName(){return driver.device.database_file_name;}

	void SetVendorId(UINT data)
	{
		driver.device.vendorIdentifier = data;
	}

	void SetObjectAccessCOVNotification(object_access_cov_notification_fn data)
	{
		driver.cov_notification = data;
	}
	void SetObjectInsertNotification(object_insert_notification_fn data)
	{
		driver.device.object_insert_notification = data;
	}
	void SetObjectRemoveNotification(object_remove_notification_fn data)
	{
		driver.device.object_remove_notification = data;
	}
	void SetReadObjectPropertyOverride(read_object_property_override_fn data)
	{
		driver.device.read_object_property_override = data;
	}
	void SetWriteObjectPropertyOverride(write_object_property_override_fn data)
	{
		driver.device.write_object_property_override = data;
	}
	void SetReadPropertyOverride(service_override_read_property_fn data)
	{
		driver.service_overrides.read_property = data;
	}
	void SetReadPropertyConditionalOverride(service_override_read_property_conditional_fn data)
	{
		driver.service_overrides.read_property_conditional = data;
	}
	void SetReadPropertyMultipleOverride(service_override_read_property_multiple_fn data)
	{
		driver.service_overrides.read_property_multiple = data;
	}
	void SetSubscribeCOVOverride(service_override_subscribe_cov_fn data)
	{
		driver.service_overrides.subscribe_cov = data;
	}
	void SetAtomicReadFileOverride(service_override_atomic_read_file_fn data)
	{
		driver.service_overrides.atomic_read_file = data;
	}
	void SetAtomicWriteFileOverride(service_override_atomic_write_file_fn data)
	{
		driver.service_overrides.atomic_write_file = data;
	}
	void SetAddListElementOverride(service_override_add_list_element_fn data)
	{
		driver.service_overrides.add_list_element = data;
	}
	void SetRemoveListElementListElementOverride(service_override_remove_list_element_fn data)
	{
		driver.service_overrides.remove_list_element = data;
	}
	void SetCreateObjectOverride(service_override_create_object_fn data)
	{
		driver.service_overrides.create_object = data;
	}
	void SetDeleteObjectOverride(service_override_delete_object_fn data)
	{
		driver.service_overrides.delete_object = data;
	}
	void SetWritePropertyOverride(service_override_write_property_fn data)
	{
		driver.service_overrides.write_property = data;
	}
	void SetWritePropertyMultipleOverride(service_override_write_property_multiple_fn data)
	{
		driver.service_overrides.write_property_multiple = data;
	}
	void SetConfirmedPrivateTranserOverride(service_override_confirmed_private_transfer_fn data)
	{
		driver.service_overrides.confirmed_private_transfer = data;
	}
	void SetDeviceCommunicationControlOverride(service_override_device_communication_control_fn data)
	{
		driver.service_overrides.device_communication_control = data;
	}
	void SetReinitialiseDeviceOverride(service_override_reinitialize_device_fn data)
	{
		driver.service_overrides.reinitialize_device = data;
	}
	void SetReadRangeOverride(service_override_read_range_fn data)
	{
		driver.service_overrides.read_range = data;
	}
	void SetLifeSafetyOperationOverride(service_override_life_safety_operation_fn data)
	{
		driver.service_overrides.life_safety_operation = data;
	}
	void SetSubscribeCOVPropertyOverride(service_override_subscribe_cov_property_fn data)
	{
		driver.service_overrides.subscribe_cov_property = data;
	}
	void SetGetEventInformationOverride(service_override_get_event_information_fn data)
	{
		driver.service_overrides.get_event_information = data;
	}

	//Set Unconfirmed Service Notification Callback Functions
	void SetIamNotification(service_indication_iam_fn data)
	{
		driver.service_indication.iam = data;
	}
	void SetIhaveNotification(service_indication_Ihave_fn data)
	{
		driver.service_indication.Ihave = data;
	}
	void SetUnconfirmedCOVNotification(service_indication_unconfirmed_cov_notification_fn data)
	{
		driver.service_indication.unconfirmed_cov_notification = data;
	}
	void SetUnconfirmedEventNotification(service_indication_unconfirmed_event_notification_fn data)
	{
		driver.service_indication.unconfirmed_event_notification = data;
	}
	void SetUnconfirmedPrivateTransferNotification(service_indication_unconfirmed_private_transfer_fn data)
	{
		driver.service_indication.unconfirmed_private_transfer = data;
	}
	void SetUnconfirmedTextMessageNotification(service_indication_unconfirmed_text_message_fn data)
	{
		driver.service_indication.unconfirmed_text_message = data;
	}
	void SetTimeSynchronizationNotification(service_indication_time_synchronization_fn data)
	{
		driver.service_indication.time_synchronization = data;
	}
	void SetWhoHasNotification(service_indication_who_has_fn data)
	{
		driver.service_indication.who_has = data;
	}
	void SetWhoIsNotification(service_indication_who_is_fn data)
	{
		driver.service_indication.who_is = data;
	}
	void SetUTCTimeSynchronistationNotification(service_indication_utc_time_synchronization_fn data)
	{
		driver.service_indication.utc_time_synchronization = data;
	}

	//Set Confirmed Service Notification Callback Functions
	void SetConfirmedCOVNotification(service_indication_confirmed_cov_notification_fn data)
	{
		driver.service_indication.confirmed_cov_notification = data;
	}
	void SetConfirmedEventNotification(service_indication_confirmed_event_notification_fn data)
	{
		driver.service_indication.confirmed_event_notification = data;
	}
	void SetConfirmedPrivateTransferNotification(service_indication_confirmed_private_transfer_fn data)
	{
		driver.service_indication.confirmed_private_transfer = data;
	}
	void SetConfirmedTextMessageNotification(service_indication_confirmed_text_message_fn data)
	{
		driver.service_indication.confirmed_text_message = data;
	}
	void SetLifeSafetyOperationNotification(service_indication_life_safety_operation_fn data)
	{
		driver.service_indication.life_safety_operation = data;
	}

	void SetReadObjectProprietaryProperty(read_object_proprietary_property_fn data)
	{
		driver.device.read_object_proprietary_property = data;
	}

	void SetWriteObjectProprietaryProperty(write_object_proprietary_property_fn data)
	{
		driver.device.write_object_proprietary_property = data;
	}

	void SetProprietaryPropertyInit(proprietary_property_init_fn data)
	{
		driver.device.proprietary_property_init = data;
	}

	void SetProprietaryAllPropertyAdd(proprietary_all_property_add_fn data)
	{
		driver.device.proprietary_all_property_add = data;
	}

	void SetObjectProcessOveride(object_process_overide_fn data)
	{
		driver.device.object_process_overide = data;
	}

	void SetCovAppendActiveSubscription(cov_append_active_subscription_fn data)
	{
		driver.device.cov_append_active_subscription = data;
	}

	void SetEventAppendRecipientList(event_append_recipient_list_fn data)
	{
		driver.device.event_append_recipient_list = data;
	}
	
	void SetReadPresentValueNotification(read_present_value_notification_fn data)
	{
		driver.device.read_present_value_notification = data;
	}
	
	BYTE CreateClient(char* Host = NULL)
	{
		stop_driver();
		if (!dataLinkLayers.AddOutOfProcess(Host))
			return 0x0;
		return start_driver();
	}

	BYTE Create()
	{
		stop_driver();
		return start_driver();
	}

	void Destroy()
	{
		stop_driver();
		bacnapi_destroy_driver_initilization_struct(&driver);
	}

	BYTE CreateAnexH(int DeviceID, int Network = 10)
	{
		stop_driver();
		driver.device_id = DeviceID;
		if (!dataLinkLayers.AddAnexH(Network))
			return 0x0;
		return start_driver();
	}

	BYTE CreateBACnetIP(int DeviceID, int Network = 1, UINT UDPPort = 47808, char * BBMDAddress = NULL, UINT BBMDMaxForeignDevices = 0, UINT ForegnDeviceExpiryTime = 120, UINT Eth = 0)
	{
		stop_driver();
		driver.device_id = DeviceID;
		if (!dataLinkLayers.AddIP(Network, UDPPort, NULL, NULL, BBMDAddress, BBMDMaxForeignDevices, ForegnDeviceExpiryTime, Eth))
			return 0x0;
		return start_driver();
	}

	//Get Properties
	int GetDeviceID()
	{
		if (initialized)
			return bacnapi_device_id();
		return -1;
	}
	int GetNetwork()
	{
		if (initialized)
			return bacnapi_network();
		return -1;
	}
	CBACnetLogging &GetLogging()
	{
		return logging;
	}

	int InternalObjectSetRealTimeData(BACnetObjectType ObjectType, int Instance, CBACnetAny Value, int ObjectOffline = 0)
	{
		BACnetDeviceObjectPropertyReference reference;
		BYTE Priority = 0;
		BACnetDataType feedbackPresentValue;
		BYTE feedbackStatusFlags = 0;
		int error_code = 0;

		ZERO_STRUCT(feedbackPresentValue);
		reference.deviceIdentifier.objectType = BACnetObjectTypeDevice;
		reference.deviceIdentifier.instance = GetDeviceID();
		reference.objectIdentifier.objectType = ObjectType;
		reference.objectIdentifier.instance = Instance;
		reference.property = Property_presentValue;
		reference.propertyArrayIndex = -1;

		if (bacnapi_object_write_properties(&reference, &ObjectOffline, Value, &Priority, &error_code, &feedbackPresentValue, &feedbackStatusFlags, 1, 0))
			return error_code;
		bacnapi_ClearBACnetDataType(&feedbackPresentValue);
		return 0;
	}

	int CreateInternalObject(BACnetObjectType ObjectType, int Instance, char* ObjectName = NULL)
	{
		char* connect_strings[1];
		char connect_string[2056];
		int error_code;

		if (ObjectName)
			sprintf(connect_string, "%d;%d;%d;%s;", GetDeviceID(), ObjectType, Instance, ObjectName);
		else
			sprintf(connect_string, "%d;%d;%d;", GetDeviceID(), ObjectType, Instance);

		connect_strings[0] = connect_string;
		return bacnapi_insert_object(connect_strings, &error_code, 1);
	}

	CBACnetDatalinkLayer &GetDatalinkLayers(){return dataLinkLayers;}
	BACnetDriver driver;
private:
	CBACnetDatalinkLayer dataLinkLayers;
	CBACnetLogging logging;
	BYTE initialized;

	int copy_string(char** destination, char* source)
	{
		BACnetDataType tmp;
		tmp.type = bacnetDT_CharacterString;
		tmp._type.String = source;
		return bacnapi_access_string(destination, &tmp, 1);
	}

	BYTE start_driver()
	{
		if (initialized)
			return 0x0;
		BYTE retCode = bacnapi_init(&driver) == 0 ? 0xFF : 0x0;

		if (retCode)
			initialized = 0xFF;
		else
			initialized = 0x0;

		return retCode;
	}
	void stop_driver()
	{
		if (initialized)
		{
			initialized = 0x0;
			bacnapi_destroy();
		}
	}
};

class CInternalObjectInsert
{
public:
	CInternalObjectInsert(int qty)
	{
		m_quantity = qty;
		if (m_quantity > 0)
		{
			m_connect_strings = new char*[qty];
			m_error_code = new int[qty];
			memset(m_connect_strings, 0, qty * sizeof(char*));
		}
	}
	~CInternalObjectInsert()
	{
		if (m_quantity > 0)
		{
			int i;
			for (i = 0; i < m_quantity; i++)
			{
				if (m_connect_strings[i])
					delete m_connect_strings[i];
			}
			delete m_connect_strings;
			delete m_error_code;
		}
	}
	void Insert(int index, char* connect_string)
	{
		char* buf = new char[strlen(connect_string) + 1];
		if (buf)
		{
			strcpy(buf, connect_string);
			m_connect_strings[index] = buf;
		}
	}
	void Update()
	{
		if (m_quantity > 0)
			bacnapi_insert_object(m_connect_strings, m_error_code, m_quantity);
	}

private:
	int m_quantity;
	char** m_connect_strings;
	int* m_error_code;
};

class CInternalObjectUpdate
{
public:
	CInternalObjectUpdate(int qty)
	{
		m_quantity = qty;
		if (m_quantity > 0)
		{
			m_reference = new BACnetDeviceObjectPropertyReference[qty];
			m_priority = new BYTE[qty];
			m_object_offline = new int[qty];
			m_error_code = new int[qty];
			m_value = new BACnetDataType[qty];
			m_feedback_value = new BACnetDataType[qty];
			m_feedback_status_flags = new BYTE[qty];

			memset(m_reference, 0, qty * sizeof(BACnetDeviceObjectPropertyReference));
			memset(m_priority, 0, qty * sizeof(BYTE));
			memset(m_object_offline, 0, qty * sizeof(int));
			memset(m_error_code, 0, qty * sizeof(int));
			memset(m_value, 0, qty * sizeof(BACnetDataType));
			memset(m_feedback_value, 0, qty * sizeof(BACnetDataType));
			memset(m_feedback_status_flags, 0, qty * sizeof(BYTE));

		}
	}
	~CInternalObjectUpdate()
	{
		if (m_quantity > 0)
		{
			delete m_priority;
			delete m_reference;
			delete m_object_offline;
			delete m_error_code;
			delete m_value;
			delete m_feedback_value;
			delete m_feedback_status_flags;
		}
	}
	void Write(int index, int deviceID, int objectType, int instance, int objectOffline, BACnetDataType *value, int priority = 0, int property = 85, int arrayIndex = -1)
	{
		m_reference[index].deviceIdentifier.objectType = BACnetObjectTypeDevice;
		m_reference[index].deviceIdentifier.instance = deviceID;
		m_reference[index].objectIdentifier.objectType = (BACnetObjectType)objectType;
		m_reference[index].objectIdentifier.instance = instance;
		m_reference[index].property = (BACnetPropertyIdentifier)property;
		m_reference[index].propertyArrayIndex = arrayIndex;
		m_priority[index] = priority;
		m_object_offline[index] = objectOffline;
		m_error_code[index] = 0;
		m_value[index] = *value;
	}
	void Update()
	{
		if (m_quantity > 0)
			bacnapi_object_write_properties(m_reference, m_object_offline, m_value, m_priority, m_error_code, m_feedback_value, m_feedback_status_flags, m_quantity, 0);
	}


private:
	int m_quantity;
	BACnetDeviceObjectPropertyReference* m_reference;
	BYTE* m_priority;
	int* m_object_offline;
	int* m_error_code;
	BACnetDataType *m_value;
	BACnetDataType *m_feedback_value;
	BYTE *m_feedback_status_flags;
};

class CCache1
{
public:
	CCache1()
	{
		memset(&m_Value, 0, sizeof(m_Value));
		memset(&m_FeedbackValue, 0, sizeof(m_FeedbackValue));
		status_flags = 0;
	}
	~CCache1()
	{
		bacnapi_ClearBACnetDataType(&m_Value);
		bacnapi_ClearBACnetDataType(&m_FeedbackValue);
	}
	int Read(int DeviceID, BACnetObjectType ObjectType, int Instance, BACnetPropertyIdentifier Property = Property_presentValue, int ArrayIndex = -1)
	{
		BACnetDeviceObjectPropertyReference reference;

		reference.deviceIdentifier.objectType = BACnetObjectTypeDevice;
		reference.deviceIdentifier.instance = DeviceID;
		reference.objectIdentifier.objectType = ObjectType;
		reference.objectIdentifier.instance = Instance;
		reference.property = Property;
		reference.propertyArrayIndex = ArrayIndex;
		return bacnapi_object_read(&reference, &m_Value, &status_flags, 0) ? 0x0 : 0xFF;;
	}
	int Write(int DeviceID, BACnetObjectType ObjectType, int Instance, BACnetPropertyIdentifier Property = Property_presentValue, BYTE priority = 10, int ArrayIndex = -1)
	{
		BACnetDeviceObjectPropertyReference reference;

		reference.deviceIdentifier.objectType = BACnetObjectTypeDevice;
		reference.deviceIdentifier.instance = DeviceID;
		reference.objectIdentifier.objectType = ObjectType;
		reference.objectIdentifier.instance = Instance;
		reference.property = Property;
		reference.propertyArrayIndex = ArrayIndex;
		bacnapi_ClearBACnetDataType(&m_FeedbackValue);
		return bacnapi_object_write(&reference, 0, &m_Value, priority, &m_FeedbackValue, &status_flags, 0) ? 0x0 : 0xFF;;
	}
	int Write(int DeviceID, BACnetObjectType ObjectType, int Instance, BACnetPropertyIdentifier Property = Property_presentValue, CBACnetAny Value = BACnetNull, BYTE priority = 10, int ArrayIndex = -1)
	{
		BACnetDeviceObjectPropertyReference reference;

		reference.deviceIdentifier.objectType = BACnetObjectTypeDevice;
		reference.deviceIdentifier.instance = DeviceID;
		reference.objectIdentifier.objectType = ObjectType;
		reference.objectIdentifier.instance = Instance;
		reference.property = Property;
		reference.propertyArrayIndex = ArrayIndex;

		bacnapi_ClearBACnetDataType(&m_Value);
		bacnapi_ClearBACnetDataType(&m_FeedbackValue);
		if (bacnapi_access_BACnetData(&m_Value, Value, -1, 1))
			return 0;
		return bacnapi_object_write(&reference, 0, &m_Value, priority, &m_FeedbackValue, &status_flags, 0) ? 0x0 : 0xFF;;
	}

	CBACnetAny GetValue(){return CBACnetAny(&m_Value, 0x0);}
private:
	BACnetDataType m_Value;
	BACnetDataType m_FeedbackValue;
	BYTE status_flags;
};


#endif
