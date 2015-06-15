/*####COPYRIGHTBEGIN####
 -------------------------------------------
 Copyright (C) 2003 Chris Gurtler

 -------------------------------------------
####COPYRIGHTEND####*/

/*

  datatype.h
  Writen By Chris Gurtler
  Date 27/3/2001

  Contains Data Types used by BACnet

  Version 1.0

  Revision History
  30 Jan 2006 - Proprietary Objects Added
  22 May 2006 - Modified Active Subscription Data Type to SequenceOfBACnetCOVSubscription

*/

#ifndef INCLUDE_DATA_TYPES
#define INCLUDE_DATA_TYPES

#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>

#include "common_enum.h"
#include "common_struct.h"

//BACnet Octet String Structure
typedef struct BACnetOctet
{
    UINT len;
    BYTE *contents;
}BACnetOctet;

//BACNet Bit String Structure
typedef struct BACnetBitString
{
    UINT length;
    UINT position;
    BYTE *contents;
}BACnetBitString;

//BACnet Date Structure
typedef struct BACnetDate
{
    BYTE year;
    BYTE month;
    BYTE day;
    BYTE dayOfWeek;
}BACnetDate;

//BACnet Time Structure
typedef struct BACnetTime
{
    BYTE hour;
    BYTE minute;
    BYTE second;
    BYTE hundrethsOfASec;
}BACnetTime;

//BACnet DateTime Structure
typedef struct BACnetDateTime
{
    BACnetDate date;
    BACnetTime time;
}BACnetDateTime;

//BACnetDateRange Structure
typedef struct BACnetDateRange
{
    BACnetDate startDate;
    BACnetDate endDate;
}BACnetDateRange;

//BACnetWeekNDay BACnetWeekNDay
typedef struct BACnetWeekNDay
{
    BYTE month;
    BYTE weekOfMonth;
    BYTE dayOfWeek;
}BACnetWeekNDay;

//BACnetCalendarEntry Structure
typedef struct BACnetCalendarEntry
{
	BACnetCalendarEntryType choice;
    BACnetDate date;
    BACnetDateRange dateRange;
    BACnetWeekNDay weekNDay;
}BACnetCalendarEntry;

//BACnet Object Identifier Structure
typedef struct BACnetObjectIdentifier
{
    BACnetObjectType objectType;
    int instance;
}BACnetObjectIdentifier;

//BACnet Device Object Property Reference
typedef struct BACnetDeviceObjectPropertyReference
{
    BACnetObjectIdentifier objectIdentifier;
    BACnetPropertyIdentifier property;
    int propertyArrayIndex;
    BACnetObjectIdentifier deviceIdentifier;
}BACnetDeviceObjectPropertyReference;

//BACnet Object Property Reference
typedef struct BACnetObjectPropertyReference
{
    BACnetObjectIdentifier objectIdentifier;
    BACnetPropertyIdentifier property;
    int propertyArrayIndex;
}BACnetObjectPropertyReference;

//BACnet Function Block Identifier
typedef struct BACnetFunctionBlockIdentifier
{
    BACnetObjectIdentifier objectIdentifier;
    UINT sequenceNumber;
}BACnetFunctionBlockIdentifier;

//BACnet Application Datatype
typedef struct BACnetDataType
{
    BACnetApplicationTagNumber type;
    union _BACnetDataType
    {
		//Base data Types
        UINT Null;
        BYTE Boolean;
        UINT UnsignedInteger;
        UINT UnsignedLong;
        int SignedInteger;
        float Real;
        double Double;
        BACnetOctet Octet;
        char *String;
        BACnetBitString BitString;
        UINT Enumerated;
        BACnetDate Date;
        BACnetTime Time;
        BACnetObjectIdentifier ObjectIdentifier;
        UINT Reserved1;
        UINT Reserved2;
        UINT Reserved3;
		long SignedLong;
		//Constructed Data Types
		void* address;
		void* constructed;
		void* dateTime;
		void* calendarEntry;
		void* dateRange;
		void* weekNDay;
		void* clientCOV;
		void* timeValue;
		void* timeStamp;
		void* actionCommand;
		void* sessionKey;
		void* recipient;
		void* recipientProcess;
		void* addressBinding;
		void* objectPropertyReference;
		void* deviceObjectPropertyReference;
		void* logRecord;
		void* readAccessSpecification;
		void* writeAccessSpecification;
		void* readAccessResult;
		void* readAccessPropertyValue;
		void* propertyReference;
		void* destination;
		void* dailySchedule;
		void* specialEvent;
		void* eventTimeStamps;
		void* priorityValue;
		void* propertyValue;
		void* propertyStates;
		void* eventParameter;
		void* notificationParameters;
		void* objectPropertyValue;
		void* selectionCriteria;
		void* alarmSummaryAckElement;
		void* enrolmentSummaryAckElement;
		void* element;			//BACnet Abstract Data Type
		void* array;			//BACnet Array
		void* listOfelements;	//BACnet List
		void* functionBlockInput;
		void* COVSubscription;
		void* scale;
		void* prescale;
		void* accumulatorRecord;
		void* eventSummaryElement;
		void* deviceObjectPropertyValue;
		int list;				//List Number
		//BACnet Arrays
		void* priorityArray;
		void* actionList;
        void* sequenceOfBACnetCOVSubscription;			//Used by Property_activeCOVSubscriptions
        void* sequenceOfBACnetLogRecord;				//Used by Property_logBuffer
        void* sequenceOfBACnetCalendarEntry;			//Used by Property_dateList
        void* sequenceOfBACnetActionList;				//Used by sequenceOfBACnetActionCommand
        void* sequenceOfBACnetActionCommand;			//Used in Property_action
        void* sequenceOfCharacterString;				//Used in Property_actionText, Property_stateText
        void* sequenceOfBACnetObjectIdentifier;			//Used in Property_objectList
		void* sequenceOfBACnetFunctionBlockIdentifier;
        void* sequenceOfBACnetVTClass;					//Uesd in Property_vtClassesSupported
        void* sequenceOfBACnetVTSession;				//Used in Property_activeVtSessions
        void* sequenceOfBACnetSessionKey;				//Used in Property_listOfSessionKeys
        void* sequenceOfBACnetRecipient;				//Used in Property_timeSynchronizationRecipients
        void* sequenceOfBACnetAddressBinding;			//Used in Property_deviceAddressBinding
        void* sequenceOfReadAccessSpecification;		//Used in Property_listOfGroupMembers
		void* sequenceOfWriteAccessSpecification;
		void* sequenceOfAlarmSummaryAckElement;
		void* sequenceOfEnrolmentSummaryAckElement;
        void* sequenceOfReadAccessResult;				//To do - check this Used in Group Object - Property_presentValue
        void* sequenceOfReadAccessPropertyValue;		//To do - check this Used in Group Object
        void* sequenceOfUnsigned;						//Used in Property
        void* sequenceOfBACnetDestination;				//Used in Property_recipientList
        void* sequenceOfBACnetDailySchedule;			//Used in Property_weeklySchedule
        void* sequenceOfSpecialEvent;					//Used in Property_exceptionSchedule
        void* sequenceOfBACnetObjectPropertyReference;  //Used in Property_listOfObjectReferences
		void* sequenceOfBACnetTimeStamp;				//Used in Property_eventTimeStamps
		void* sequenceOfBACnetTimeValue;				//Used in Daily Schedule Object
		void* sequenceOfBACnetBitString;				//Used in BACnetEventParametr Object
		void* sequenceOfBACnetPropertyStates;			//Used in BACnetEventParametr Object
		void* sequenceOfBACnetObjectPropertyValue;
		void* sequenceOfConstructed;
		void* setpointReference;
		void* dataLinkLayer;
    }_type;
}BACnetDataType;

//BACnet Constructed Data Type
typedef struct BACnetConstructedDataType
{
    int applicationTag;
    BACnetDataType data;
}BACnetConstructedDataType;

typedef BACnetBitString BACnetEventTransitionBits;
typedef BACnetBitString BACnetStatusFlags;
typedef BACnetBitString BACnetDaysOfWeek;
typedef BACnetBitString BACnetTransitionBits;
typedef BACnetBitString BACnetLogStatus;
typedef BACnetBitString BACnetResultFlags;

typedef BACnetDataType BACnetCharacterString;

//Constructed Data Types
typedef BACnetDataType BACnetPriorityArray;
typedef BACnetDataType BACnetEventTimeStamps;

typedef BACnetDataType SequenceOfBACnetCalendarEntry;
typedef BACnetDataType SequenceOfBACnetActionList;
typedef BACnetDataType SequenceOfCharacterString;
typedef BACnetDataType SequenceOfBACnetObjectIdentifier;
typedef BACnetDataType SequenceOfBACnetFunctionBlockIdentifier;
typedef BACnetDataType SequenceOfBACnetVTClass;
typedef BACnetDataType SequenceOfBACnetVTSession;
typedef BACnetDataType SequenceOfBACnetSessionKey;
typedef BACnetDataType SequenceOfBACnetRecipient;
typedef BACnetDataType SequenceOfBACnetAddressBinding;
typedef BACnetDataType SequenceOfUnsigned;
typedef BACnetDataType SequenceOfBACnetDestination;
typedef BACnetDataType SequenceOfBACnetDailySchedule;
typedef BACnetDataType SequenceOfSpecialEvent;
typedef BACnetDataType SequenceOfBACnetDeviceObjectPropertyReference;
typedef BACnetDataType SequenceOfObjectPropertyReference;
typedef BACnetDataType SequenceOfBACnetActionCommand;
typedef BACnetDataType SequenceOfBACnetTimeValue;
typedef BACnetDataType SequenceOfBACnetPropertyValue;
typedef BACnetDataType SequenceOfSelectionCriteria;
typedef BACnetDataType SequenceOfWriteAccessSpecification;
typedef BACnetDataType SequenceOfAlarmSummaryAck;
typedef BACnetDataType SequenceOfEnrolmentSummaryAck;
typedef BACnetDataType ListOfBACnetLogRecord;
typedef BACnetDataType BACnetListOfLifeSafetyState; //Modified CG 20 Sep 2005
typedef BACnetDataType BACnetListOfLifeSafetyMode;
typedef BACnetDataType SequenceOfEventSummaries;
typedef BACnetDataType SequenceOfBitString;
typedef BACnetDataType SequenceOfBACnetCOVSubscription; //Modified CG 22 May 2006

//BACnetTimeValue Structure
typedef struct BACnetTimeValue
{
    BACnetTime time;
	BACnetDataType value;
}BACnetTimeValue;

//BACnetTimeStamp Structure
typedef struct BACnetTimeStamp
{
	BACnetTimeStampTypes type;
	union _BACnetTimeStampTypes
	{
		BACnetTime time;
		UINT sequenceNumber;
		BACnetDateTime dateTime;
	}_type;
}BACnetTimeStamp;

//BACnetActionCommand Structure
typedef struct BACnetActionCommand
{
    BACnetObjectIdentifier deviceIdentifier;
    BACnetObjectIdentifier objectIdentifier;
	BACnetPropertyIdentifier propertyIdentifier;
	int propertyArrayIndex;
	BACnetDataType propertyValue;
	UINT priority;
	UINT postDelay;
	BYTE quitOnFailure;
	BYTE writeSuccessful;
	time_t last_send_time;
}BACnetActionCommand;

//BACnet Function Block Identifier
typedef struct BACnetFunctionBlockInput
{
	BACnetFunctionBlockInputType type;
    union _BACnetFunctionBlockInputType
    {
		BACnetDeviceObjectPropertyReference referencedProperty;
		BACnetDataType abstractData;
	}_type;
}BACnetFunctionBlockInput;

//BACnetActionList Structure
typedef struct BACnetActionList
{
	SequenceOfBACnetActionCommand action;
}BACnetActionList;

//_ReadAccessSpecification Structure
typedef struct _WriteAccessSpecification
{
    BACnetObjectIdentifier objectIdentifier;
	SequenceOfBACnetPropertyValue listOfProperties;
}_WriteAccessSpecification;

//BACnetDailySchedule Structure
typedef struct BACnetDailySchedule
{
	SequenceOfBACnetTimeValue daySchedule;
}BACnetDailySchedule;

typedef enum BACnetReadAccessResultElementChoice
{
	BACnetReadAccessResultElementChoicePropertyValue,
	BACnetReadAccessResultElementChoiceError
}BACnetReadAccessResultElementChoice;

//BACnetPropertyReference Structure
typedef struct BACnetPropertyReference
{
    BACnetPropertyIdentifier propertyIdentifier;
	int propertyArrayIndex;
}BACnetPropertyReference;

typedef struct SequenceOfBACnetPropertyReference
{
	BACnetPropertyReference entry;
	BACnetPropertyReference* entries;
	int qty;
}SequenceOfBACnetPropertyReference;

//_ReadAccessSpecification Structure
typedef struct _ReadAccessSpecification
{
    BACnetObjectIdentifier objectIdentifier;
	SequenceOfBACnetPropertyReference listOfPropertyReferences;
}_ReadAccessSpecification;

typedef struct SequenceOfReadAccessSpecification
{
	_ReadAccessSpecification entry;
	_ReadAccessSpecification* entries;
	int qty;
}SequenceOfReadAccessSpecification;

//BACnetSpecialEvent Structure
typedef struct BACnetSpecialEvent
{
	BACnetSpecialEventPeriodType periodType;
    union _period
    {
        BACnetCalendarEntry calendarEntry;
        BACnetObjectIdentifier calendarReference;
    }period;
	SequenceOfBACnetTimeValue listOfTimeValues;
	BACnetDataType currentValue;
	UINT eventPriority;
}BACnetSpecialEvent;

//BACNet Array
typedef struct BACnetArray
{
    UINT length;
    UINT position;
    UINT ubound;
    BACnetDataType* contents;
	BYTE sorted_flag; //Used to force a qsort. (Resort if sorted_flag = 0)
}BACnetArray;

//BACnet Priority Value
typedef struct BACnetPriorityValue
{
    BACnetPriorityValueTypes type;
    union _BACnetPriorityValue
    {
        UINT Null;
        float Real;
        BACnetBinaryPV Binary;
        UINT UnsignedInteger;
        BACnetDataType constructedValue;
    }_type;
}BACnetPriorityValue;

//BACnet Property Value
typedef struct BACnetPropertyValue
{
    BACnetPropertyIdentifier propertyIdentifier;
    int propertyArrayIndex;
    BACnetDataType value;
    UINT priority;
}BACnetPropertyValue;

typedef struct _Error
{
    BACnetErrorClass errorClass;
    BACnetErrorCode errorCode;
}_Error;

typedef struct _ChangeListError
{
    _Error errorType;
    UINT firstFailedElementNumber;
}_ChangeListError;

typedef struct _CreateObjectError
{
    _Error errorType;
    UINT firstFailedElementNumber;
}_CreateObjectError;

typedef struct _WritePropertyMultipleError
{
    _Error errorType;
    BACnetObjectPropertyReference firstFailedElementNumber;
}_WritePropertyMultipleError;

typedef struct _VTCloseError
{
    _Error errorType;
    SequenceOfUnsigned listOfVTSessionIdentifiers;
}_VTCloseError;

typedef struct _ConfirmedPrivateTransferError
{
    _Error errorType;
    UINT vendorID;
    UINT serviceNumber;
    BACnetDataType errorParameters;
}_ConfirmedPrivateTransferError;

typedef struct BACnetError
{
    BACnetErrorChoice choice;
    union errorChoices
    {
        _Error confirmedCOVnotification;
        _Error acknowledgeAlarm;
        _Error confirmedEventNotification;
        _Error getAlarmSummary;
        _Error getEnrolmentSummary;
        _Error subscribeCOV;
        _Error atomicReadFile;
        _Error atomicWriteFile;
        _ChangeListError addListElement;
        _ChangeListError removeListElement;
        _CreateObjectError createObject;
        _Error deleteObject;
        _Error readProperty;
        _Error readPropertyConditional;
        _Error readPropertyMultiple;
        _Error writeProperty;
        _WritePropertyMultipleError writePropertyMultiple;
        _Error deviceCommunicationControl;
        _ConfirmedPrivateTransferError confirmedPrivateTransfer;
        _Error confirmedTextMessage;
        _Error reinitializeDevice;
        _Error vtOpen;
        _VTCloseError vtClose;
        _Error vtData;
        _Error authenticate;
        _Error requestKey;
        _Error readRange;
        _Error lifeSafetyOperation;
        _Error subscribeCOVProperty;
        _Error getEventInformation;
    }_choice;
}BACnetError;

//BACnet Object Property Value
typedef struct BACnetObjectPropertyValue
{
    BACnetObjectIdentifier objectIdentifier;
    BACnetPropertyIdentifier propertyIdentifier;
    int propertyArrayIndex;
    BACnetDataType value;
    UINT priority;
	_Error propertyAccessError;
}BACnetObjectPropertyValue;

//BACnet Property Value
typedef struct BACnetReadAccessPropertyValue
{
    BACnetPropertyIdentifier propertyIdentifier;
    int propertyArrayIndex;
	BACnetReadAccessReadResultType readResultType;
	union _BACnetReadResult
	{
		BACnetDataType propertyValue;
		_Error propertyAccessError;
	} readResult;
}BACnetReadAccessPropertyValue;

typedef struct SequenceOfBACnetReadAccessPropertyValue
{
	BACnetReadAccessPropertyValue entry;
	BACnetReadAccessPropertyValue* entries;
	int qty;
}SequenceOfBACnetReadAccessPropertyValue;

//_ReadAccessResult Structure
typedef struct _ReadAccessResult
{
    BACnetObjectIdentifier objectIdentifier;
	SequenceOfBACnetReadAccessPropertyValue listOfResults;
}_ReadAccessResult;

typedef struct SequenceOfReadAccessResult
{
	_ReadAccessResult entry;
	_ReadAccessResult* entries;
	int qty;
}SequenceOfReadAccessResult;

//BACnet List Of Object Property Value
typedef struct BACnetListOfObjectPropertyValue
{
    UINT length;
    UINT position;
    BACnetObjectPropertyValue *contents;
}BACnetListOfObjectPropertyValue;

typedef bacnet_address BACnetAddress;

//BACnetAddressBinding Structure
typedef struct BACnetAddressBinding
{
    BACnetObjectIdentifier deviceObjectIdentifier;
	BACnetAddress deviceAddress;
}BACnetAddressBinding;

//BACnetSessionKey Structure
typedef struct BACnetSessionKey
{
    BACnetOctet sessionKey;
	BACnetAddress peerAddress;
}BACnetSessionKey;

typedef struct BACnetDeviceAddress
{
    UINT network;
    BYTE address[6];
    BYTE length;
}BACnetDeviceAddress;

typedef struct BACnetRecipient
{
	BACnetRecipientType type;
    union _BACnetRecipientType
    {
		BACnetObjectIdentifier device;
		BACnetAddress address;
	}_type;
}BACnetRecipient;

typedef struct BACnetRecipientProcess
{
	BYTE notEmpty;
	BACnetRecipient recipient;
	UINT processIdentifier;
}BACnetRecipientProcess;

//BACnetDestination Structure
typedef struct BACnetDestination
{
    BACnetDaysOfWeek validDays;
	BACnetTime fromTime;
	BACnetTime toTime;
	BACnetRecipient recipient;
	UINT processIdentifier;
	BYTE issueConfirmedNotifications;
	BACnetTransitionBits transitions;
}BACnetDestination;

//BACnetEventParameter Structure
typedef struct _BACnetEventParameterChangeOfBitString
{
	UINT timeDelay;
	BACnetBitString bitmask;
	SequenceOfBitString listOfBitStringValues;
}_BACnetEventParameterChangeOfBitString;

typedef struct _BACnetEventParameterChangeOfState
{
	UINT timeDelay;
	BACnetDataType listOfValues;
}_BACnetEventParameterChangeOfState;

typedef struct _BACnetEventParameterChangeOfValue
{
	UINT timeDelay;
	BACnetCovCriteriaChoice covCriteria;
	union _BACnetEventParameterCOVcriteria
	{
		BACnetBitString bitmask;
		float referencedPropertyIncrement;
	}_covCriteria;
	union _BACnetEventParameterCOVcriteriaOldValues
	{
		BACnetBitString oldBitString;
		float oldValue;
	}_covOldValueCriteria;
}_BACnetEventParameterChangeOfValue;

typedef struct _BACnetEventParameterCommandFailure
{
	UINT timeDelay;
	BACnetDeviceObjectPropertyReference feedbackPropertyReference;
}_BACnetEventParameterCommandFailure;

typedef struct _BACnetEventParameterFloatingLimit
{
	UINT timeDelay;
	BACnetDeviceObjectPropertyReference setpointReference;
	float lowDiffLimit;
	float highDiffLimit;
	float deadband;
}_BACnetEventParameterFloatingLimit;

typedef struct _BACnetEventParameterOutOfRange
{
	UINT timeDelay;
	float lowDiffLimit;
	float highDiffLimit;
	float deadband;
}_BACnetEventParameterOutOfRange;

typedef struct _BACnetEventParameterBufferReady
{
	UINT notificationThreshold;
	UINT previousNotificationCount;
}_BACnetEventParameterBufferReady;

typedef struct _BACnetEventParameterBufferReadyNew
{
	UINT notificationThreshold;
	UINT previousNotificationCount;
}_BACnetEventParameterBufferReadyNew;

//Modified 30 Sep 2005
typedef struct _BACnetEventParameterLifeSafety
{
	UINT timeDelay;
	BACnetListOfLifeSafetyState listOfLifeSafetyAlarmValues;
	BACnetListOfLifeSafetyState listOfAlarmValues;
	BACnetDeviceObjectPropertyReference modePropertyReference;
	BACnetLifeSafetyMode oldMode;
	BACnetLifeSafetyState oldState;
}_BACnetEventParameterLifeSafety;

typedef struct _BACnetEventParameterExtended
{
	UINT vendorID;
	UINT extendedEventType;
	BACnetExtendedEventChoice parameters;
	union _BACnetExtendedEventChoice
	{
		float Real;
		UINT Integer;
		BYTE Boolean;
		double Double;
		BACnetOctet Octet;
		BACnetBitString Bitstring;
		UINT Enum;
		BACnetDeviceObjectPropertyReference Reference;
	}_parameters;
}_BACnetEventParameterExtended;

typedef struct BACnetEventParameter
{
	BACnetEventType type;
    union _BACnetEventParameterType
    {
		_BACnetEventParameterChangeOfBitString changeOfBitString;
		_BACnetEventParameterChangeOfState changeOfState;
		_BACnetEventParameterChangeOfValue changeOfValue;
		_BACnetEventParameterCommandFailure commandFailure;
		_BACnetEventParameterFloatingLimit floatingLimit;
		_BACnetEventParameterOutOfRange outOfRange;
		_BACnetEventParameterBufferReady bufferReady;
		_BACnetEventParameterLifeSafety lifeSafety;
		_BACnetEventParameterBufferReadyNew bufferReadyNew;
		_BACnetEventParameterExtended extended;
	} _type;
}BACnetEventParameter;

//BACnetPropertyStates Structure
typedef struct BACnetPropertyStates
{
	BACnetPropertyStatesChoice type;
	union _BACnetPropertyStatesChoice
	{
		BYTE booleanValue;
		BACnetBinaryPV binaryValue;
		BACnetEventType eventType;
		BACnetPolarity polarity;
		BACnetProgramRequest programChange;
		BACnetProgramState programState;
		BACnetProgramError reasonForHalt;
		BACnetReliability reliability;
		BACnetEventState state;
		BACnetDeviceStatus systemStatus;
		BACnetEngineeringUnits units;
		UINT unsignedValue;
		BACnetLifeSafetyMode lifeSafetyMode; //Modified CG 30 Sep 2005
		BACnetLifeSafetyState lifeSafetyState; //Modified CG 30 Sep 2005
	}_type;
}BACnetPropertyStates;

typedef struct _BACnetNotificationParameterChangeOfBitString
{
	BACnetStatusFlags statusFlags;
	BACnetBitString referencedBitstring;
}_BACnetNotificationParameterChangeOfBitString;

typedef struct _BACnetNotificationParameterChangeOfState
{
	BACnetStatusFlags statusFlags;
	BACnetPropertyStates newState;
}_BACnetNotificationParameterChangeOfState;

typedef struct _BACnetNotificationParameterChangeOfValue
{
	BACnetStatusFlags statusFlags;
	BACnetCovCriteriaChoice newValue;
	union _BACnetNotificationParameterCOVcriteria
	{
		BACnetBitString changedBits;
		float changedValue;
	}_newValue;
}_BACnetNotificationParameterChangeOfValue;

typedef struct _BACnetNotificationParameterCommandFailure
{
	BACnetStatusFlags statusFlags;
	BACnetDataType commandValue;
	BACnetDataType feedbackValue;
}_BACnetNotificationParameterCommandFailure;

typedef struct _BACnetNotificationParameterFloatingLimit
{
	BACnetStatusFlags statusFlags;
	float referenceValue;
	float setpointValue;
	float errorLimit;
}_BACnetNotificationParameterFloatingLimit;

typedef struct _BACnetNotificationParameterOutOfRange
{
	BACnetStatusFlags statusFlags;
	float exceedingValue;
	float deadband;
	float exceedingLimit;
}_BACnetNotificationParameterOutOfRange;

typedef struct _BACnetNotificationParameterBufferReady
{
	BACnetObjectIdentifier bufferDevice;
	BACnetObjectIdentifier bufferObject;
	BACnetDateTime previousNotification;
	BACnetDateTime currentNotification;
}_BACnetNotificationParameterBufferReady;

typedef struct _BACnetNotificationParameterBufferReadyNew
{
	BACnetDeviceObjectPropertyReference bufferProperty;
	UINT previousNotification;
	UINT currentNotification;
}_BACnetNotificationParameterBufferReadyNew;

//BACnet Object Property Value
typedef struct BACnetDeviceObjectPropertyValue
{
    BACnetObjectIdentifier deviceIdentifier;
    BACnetObjectIdentifier objectIdentifier;
    BACnetPropertyIdentifier propertyIdentifier;
    int propertyArrayIndex;
    BACnetDataType value;
}BACnetDeviceObjectPropertyValue;

typedef struct _BACnetNotificationParameterExtended
{
	UINT vendorID;
	UINT extendedEventType;
	BACnetExtendedEventChoice parameters;
	union _BACnetExtendedNotificationParameterChoice
	{
		float Real;
		UINT Integer;
		BYTE Boolean;
		double Double;
		BACnetOctet Octet;
		BACnetBitString Bitstring;
		UINT Enum;
		BACnetDeviceObjectPropertyValue PropertyValue;
	}_parameters;
}_BACnetNotificationParameterExtended;

//Modified 30 Sep 2005
typedef struct _BACnetNotificationParameterLifeSafety
{
	BACnetLifeSafetyState newState;
	BACnetLifeSafetyMode newMode;
	BACnetStatusFlags statusFlags;
	BACnetLifeSafetyOperation operationExpected;
}_BACnetNotificationParameterLifeSafety;

//BACnetNotificationParameters Structure
typedef struct BACnetNotificationParameters
{
	BACnetEventType type;
    union _BACnetNotificationParameterType
    {
		_BACnetNotificationParameterChangeOfBitString changeOfBitString;
		_BACnetNotificationParameterChangeOfState changeOfState;
		_BACnetNotificationParameterChangeOfValue changeOfValue;
		_BACnetNotificationParameterCommandFailure commandFailure;
		_BACnetNotificationParameterFloatingLimit floatingLimit;
		_BACnetNotificationParameterOutOfRange outOfRange;
		SequenceOfBACnetPropertyValue complexEventType;
		_BACnetNotificationParameterBufferReady bufferReady;
		_BACnetNotificationParameterLifeSafety lifeSafety;
		_BACnetNotificationParameterBufferReadyNew bufferReadyNew;
		_BACnetNotificationParameterExtended extended;
	} _type;
}BACnetNotificationParameters;

typedef struct BACnetClientCOV
{
	BACnetClientCOVType choice;
    union _BACnetClientCOVType
	{
		float realIncrement;
		UINT defaultIncrement;
	}_choice;
}BACnetClientCOV;

typedef struct BACnetSetpointReference
{
	BYTE notEmpty;
	BACnetObjectPropertyReference objectPropertyReference;
}BACnetSetpointReference;

typedef struct BACnetLogRecord
{
	BACnetDateTime timestamp;
	BACnetLogDatumType choice;
	union _BACnetLogDatumType
	{
		BACnetLogStatus logStatus;
		BYTE booleanValue;
		float realValue;
		UINT enumValue;
		UINT unsignedValue;
		int signedValue;
		BACnetBitString bitstringValue;
		UINT nullValue;
		_Error failure;
		float timeChange;
		BACnetDataType anyValue;
	}_choice;
	BACnetStatusFlags statusFlags;
}BACnetLogRecord;

typedef struct BACnetAccumulatorRecord
{
	BACnetDateTime timeStamp;
	UINT presentValue;
	UINT accumulatedValue;
	BACnetAccumulatorStatus accumulatorStatus;
}BACnetAccumulatorRecord;

typedef struct BACnetPrescale
{
	UINT multiplier;
	UINT moduloDivide;
}BACnetPrescale;

typedef struct BACnetScale
{
	BACnetScaleType choice;
	union _BACnetScaleType
	{
		float floatScale;
		UINT integerScale;
	} _choice;
}BACnetScale;

typedef struct BACnetCOVSubscription
{
	BACnetRecipientProcess recipient;
	BACnetObjectPropertyReference monitoredPropertyReference;
	BYTE issueConfirmedNotifications;
	UINT timeRemaining;
	float covIncrement;
	//Extra Data
	BYTE covIncrement_flag;
    float previousValue;
	int lifetime;
	time_t lastSubscription;
}BACnetCOVSubscription;

/***********Confirmed Alarm and Event Services****************/
typedef struct BACnetServiceAcknowledgeAlarmRequest
{
	UINT acknowledgingProcessIdentifier;
	BACnetObjectIdentifier eventObjectIdentifier;
	BACnetEventState eventStateAcknowledged;
	BACnetTimeStamp timeStamp;
	BACnetCharacterString acknowledgmentSource;
	BACnetTimeStamp timeOfAcknowledgment;
}BACnetServiceAcknowledgeAlarmRequest;

typedef struct BACnetServiceAcknowledgeAlarm
{
	address_binding device;
	BACnetServiceAcknowledgeAlarmRequest *request;
	BYTE ack;
	BACnetError *error;
}BACnetServiceAcknowledgeAlarm;

typedef struct BACnetServiceCOVNotificationRequest
{
	UINT subscriberProcessIdentifier;
	BACnetObjectIdentifier initiatingDeviceIdentifier;
	BACnetObjectIdentifier monitoredObjectIdentifier;
	UINT timeRemaining;
	BACnetDataType listOfValues; //Sequence of BACnetPropertyValue
}BACnetServiceCOVNotificationRequest;

typedef struct BACnetServiceConfirmedCOVNotification
{
	address_binding device;
	BACnetAddress address;
	BACnetServiceCOVNotificationRequest *request;
	BYTE ack;
	BACnetError *error;
}BACnetServiceConfirmedCOVNotification;

typedef struct BACnetServiceEventNotificationRequest
{
	UINT processIdentifer;
	BACnetObjectIdentifier initiatingDeviceIdentifier;
	BACnetObjectIdentifier eventObjectIdentifier;
	BACnetTimeStamp timeStamp;
	UINT notificationClass;
	UINT priority;
	BACnetEventType eventType;
	BACnetCharacterString messageText;
	BACnetNotifyType notifyType;
	BYTE ackRequired;
	BACnetEventState fromState;
	BACnetEventState toState;
	BACnetNotificationParameters eventValues;
}BACnetServiceEventNotificationRequest;

typedef struct BACnetServiceConfirmedEventNotification
{
	address_binding device;
	BACnetServiceEventNotificationRequest *request;
	BYTE ack;
	BACnetError *error;
}BACnetServiceConfirmedEventNotification;

typedef struct BACnetServiceGetAlarmSummaryAckElement
{
	BACnetObjectIdentifier objectIdentifier;
	BACnetEventState alarmState;
	BACnetEventTransitionBits acknowledgedTransitions;
}BACnetServiceGetAlarmSummaryAckElement;

typedef struct BACnetServiceGetAlarmSummaryAck
{
	SequenceOfAlarmSummaryAck sequenceOfAlarmSummaryAck;
}BACnetServiceGetAlarmSummaryAck;

typedef struct BACnetServiceGetAlarmSummary
{
	address_binding device;
	BACnetServiceGetAlarmSummaryAck *ack;
	BACnetError *error;
}BACnetServiceGetAlarmSummary;

typedef struct BACnetServiceGetEnrolmentSummaryRequest
{
	BACnetAcknowledgmentFilter acknowledgementFilter;
	BACnetRecipientProcess enrolmentFilter;
	BACnetEventStateFilter eventStateFilter;
	BACnetEventType eventTypeFilter;
	struct BACnetPriorityFilter
	{
		int minPriority;
		int maxPriority;
	}priorityFilter;
	int notificationClassFilter;
}BACnetServiceGetEnrolmentSummaryRequest;

typedef struct BACnetServiceGetEnrolmentSummaryAckElement
{
	BACnetObjectIdentifier objectIdentifier;
	BACnetEventType eventType;
	BACnetEventState eventState;
	UINT priority;
	UINT notificationClass;
}BACnetServiceGetEnrolmentSummaryAckElement;

typedef struct BACnetServiceGetEnrolmentSummaryAck
{
	SequenceOfEnrolmentSummaryAck sequenceOfEnrolmentSummaryAck;
}BACnetServiceGetEnrolmentSummaryAck;

typedef struct BACnetServiceGetEnrolmentSummary
{
	address_binding device;
	BACnetServiceGetEnrolmentSummaryRequest *request;
	BACnetServiceGetEnrolmentSummaryAck *ack;
	BACnetError *error;
}BACnetServiceGetEnrolmentSummary;

typedef struct BACnetServiceSubscribeCOVRequest
{
	UINT subscriberProcessIdentifier;
	BACnetObjectIdentifier monitoredObjectIdentifier;
	BYTE issueConfirmedNotifications;
	int lifetime;
}BACnetServiceSubscribeCOVRequest;

typedef struct BACnetServiceSubscribeCOV
{
	address_binding device;
	BACnetServiceSubscribeCOVRequest *request;
	BYTE ack;
	BACnetError *error;
}BACnetServiceSubscribeCOV;

/***********Confirmed File Access Services****************/
typedef struct BACnetFileStreamAccessRequest
{
	int fileStartPosition;
	UINT requestedOctetCount;
}BACnetFileStreamAccessRequest;

typedef struct BACnetFileRecordAccessRequest
{
	int fileStartRecord;
	UINT requestedRecordCount;
}BACnetFileRecordAccessRequest;

typedef struct BACnetFileStreamAccessAck
{
	int fileStartPosition;
	BACnetOctet fileData;
}BACnetFileStreamAccessAck;

typedef struct BACnetFileRecordAccessAck
{
	int fileStartRecord;
	UINT returnedRecordCount;
	BACnetOctet fileRecordData;
}BACnetFileRecordAccessAck;

typedef struct BACnetServiceAtomicReadFileRequest
{
	BACnetObjectIdentifier objectIdentifier;
	BACnetFileAccessMethod accessMethod;
	union BACnetServiceAtomicReadFileRequestAccessMethod
	{
		BACnetFileStreamAccessRequest streamAccess;
		BACnetFileRecordAccessRequest recordAccess;
	}_accessMethod;
}BACnetServiceAtomicReadFileRequest;

typedef struct BACnetServiceAtomicReadFileAck
{
	BYTE endOfFile;
	BACnetFileAccessMethod accessMethod;
	union BACnetServiceAtomicReadFileAckAccessMethod
	{
		BACnetFileStreamAccessAck streamAccess;
		BACnetFileRecordAccessAck recordAccess;
	}_accessMethod;
}BACnetServiceAtomicReadFileAck;

typedef struct BACnetServiceAtomicReadFile
{
	address_binding device;
	BACnetServiceAtomicReadFileRequest *request;
	BACnetServiceAtomicReadFileAck *ack;
	BACnetError *error;
}BACnetServiceAtomicReadFile;

typedef struct BACnetServiceAtomicWriteFileRequest
{
	BACnetObjectIdentifier objectIdentifier;
	BACnetFileAccessMethod accessMethod;
	union BACnetServiceAtomicWriteFileAccessMethod
	{
		BACnetFileStreamAccessAck streamAccess;
		BACnetFileRecordAccessAck recordAccess;
	}_accessMethod;
}BACnetServiceAtomicWriteFileRequest;

typedef struct BACnetServiceAtomicWriteFileAck
{
	BACnetFileAccessMethod accessMethod;
	int fileStartPosition;
	int fileStartRecord;
}BACnetServiceAtomicWriteFileAck;

typedef struct BACnetServiceAtomicWriteFile
{
	address_binding device;
	BACnetServiceAtomicWriteFileRequest *request;
	BACnetServiceAtomicWriteFileAck *ack;
	BACnetError *error;
}BACnetServiceAtomicWriteFile;

/***********Confirmed Remote Device Management Services****************/
typedef enum BACnetEnableDisable
{
	DeviceCommunicationControlEnable,
	DeviceCommunicationControlDisable
}BACnetEnableDisable;

typedef struct BACnetServiceDeviceCommunicationControlRequest
{
	UINT timeDuration;
	BACnetEnableDisable enableDisable;
	BACnetCharacterString password;
}BACnetServiceDeviceCommunicationControlRequest;

typedef struct BACnetServiceDeviceCommunicationControl
{
	address_binding device;
	BACnetServiceDeviceCommunicationControlRequest *request;
	BYTE ack;
	BACnetError *error;
}BACnetServiceDeviceCommunicationControl;

typedef struct BACnetServicePrivateTransferRequest
{
	UINT vendorID;
	UINT serviceNumber;
	BACnetDataType serviceParameters;
}BACnetServicePrivateTransferRequest;

typedef struct BACnetServiceConfirmedPrivateTransferAck
{
	UINT vendorID;
	UINT serviceNumber;
	BACnetDataType resultBlock;
}BACnetServiceConfirmedPrivateTransferAck;

typedef struct BACnetServiceConfirmedPrivateTransfer
{
	address_binding device;
	BACnetServicePrivateTransferRequest *request;
	BACnetServiceConfirmedPrivateTransferAck *ack;
	BACnetError *error;
}BACnetServiceConfirmedPrivateTransfer;

typedef struct BACnetServiceTextMessageRequest
{
	BACnetObjectIdentifier objectIdentifier;
	BACnetMessageClassChoice messageClass;
	struct BACnetMessageClass
	{
		UINT numeric;
		BACnetCharacterString character;
	}_messageClass;
	BACnetMessagePriority messagePriority;
	BACnetDataType message;
}BACnetServiceTextMessageRequest;

typedef struct BACnetServiceConfirmedTextMessage
{
	address_binding device;
	BACnetServiceTextMessageRequest *request;
	BYTE ack;
	BACnetError *error;
}BACnetServiceConfirmedTextMessage;

typedef struct BACnetServiceReinitializeDeviceRequest
{
	BACnetStateOfDevice reinitialisedStateOfDevice;
	BACnetCharacterString password;
}BACnetServiceReinitializeDeviceRequest;

typedef struct BACnetServiceReinitializeDevice
{
	address_binding device;
	BACnetServiceReinitializeDeviceRequest *request;
	BYTE ack;
	BACnetError *error;
}BACnetServiceReinitializeDevice;

/***********Confirmed Security Services****************/

typedef struct BACnetServiceAuthenticateRequest
{
	UINT pseudoRandomNumber;
	UINT expectedInvokeID;
	BACnetCharacterString operatorName;
	BACnetCharacterString operatorPassword;
	BYTE startEncipheredSession;
}BACnetServiceAuthenticateRequest;

typedef struct BACnetServiceAuthenticateAck
{
	UINT modifiedRandomNumber;
}BACnetServiceAuthenticateAck;

typedef struct BACnetServiceAuthenticate
{
	address_binding device;
	BACnetServiceAuthenticateRequest *request;
	BACnetServiceAuthenticateAck *ack;
	BACnetError *error;
}BACnetServiceAuthenticate;

typedef struct BACnetServiceRequestKeyRequest
{
	BACnetObjectIdentifier requestingDeviceIdentifier;
	BACnetAddress requestingDeviceAddress;
	BACnetObjectIdentifier remoteDeviceIdentifier;
	BACnetAddress remoteDeviceAddress;
}BACnetServiceRequestKeyRequest;

typedef struct BACnetServiceRequestKey
{
	address_binding device;
	BACnetServiceRequestKeyRequest *request;
	BYTE ack;
	BACnetError *error;
}BACnetServiceRequestKey;

/***********Confirmed Object Access Services****************/

typedef struct BACnetServiceAddListElementRequest
{
	BACnetObjectIdentifier objectIdentifier;
	BACnetPropertyIdentifier propertyIdentifier;
	int propertyArrayIndex;
	BACnetDataType listOfElements;
}BACnetServiceAddListElementRequest;

typedef struct BACnetServiceAddListElement
{
	address_binding device;
	BACnetServiceAddListElementRequest *request;
	BYTE ack;
	BACnetError *error;
}BACnetServiceAddListElement;

typedef struct BACnetServiceCreateObjectRequest
{
	BACnetObjectSpecifierType type;
	union _BACnetObjectSpecifierType
	{
		BACnetObjectType objectType;
		BACnetObjectIdentifier objectIdentifier;
	}_type;
	SequenceOfBACnetPropertyValue listOfInitialValues;
}BACnetServiceCreateObjectRequest;

typedef struct BACnetServiceCreateObjectAck
{
	BACnetObjectIdentifier objectIdentifier;
}BACnetServiceCreateObjectAck;

typedef struct BACnetServiceCreateObject
{
	address_binding device;
	BACnetServiceCreateObjectRequest *request;
	BACnetServiceCreateObjectAck *ack;
	BACnetError *error;
}BACnetServiceCreateObject;

typedef struct BACnetServiceDeleteObjectRequest
{
	BACnetObjectIdentifier objectIdentifier;
}BACnetServiceDeleteObjectRequest;

typedef struct BACnetServiceDeleteObject
{
	address_binding device;
	BACnetServiceDeleteObjectRequest *request;
	BYTE ack;
	BACnetError *error;
}BACnetServiceDeleteObject;

typedef struct BACnetServiceReadPropertyRequest
{
	BACnetObjectIdentifier objectIdentifier;
	BACnetPropertyIdentifier propertyIdentifier;
	int propertyArrayIndex;
}BACnetServiceReadPropertyRequest;

typedef struct BACnetServiceReadPropertyAck
{
	BACnetObjectIdentifier objectIdentifier;
	BACnetPropertyIdentifier propertyIdentifier;
	int propertyArrayIndex;
	BACnetDataType propertyValue;
}BACnetServiceReadPropertyAck;

typedef struct BACnetServiceReadProperty
{
	address_binding device;
	BACnetServiceReadPropertyRequest *request;
	BACnetServiceReadPropertyAck *ack;
	BACnetError *error;
}BACnetServiceReadProperty;

union _BACnetServiceReadRangeRange
{
	struct _BACnetServiceReadRangeByPosition
	{
		UINT referencedIndex;
		int count;
	}byPosition;
	struct _BACnetServiceReadRangeByTime
	{
		BACnetDateTime referencedTime;
		int count;
	}byTime;
	struct _BACnetServiceReadRangeTimeRange
	{
		BACnetDateTime beginningTime;
		BACnetDateTime endingTime;
	}timeRange;
	struct _BACnetServiceReadRangeSequenceNumber
	{
		UINT referenceSequenceNumber;
		int count;
	}bySequenceNumber;
};

typedef struct BACnetRange
{
	struct _request
	{
		BACnetReadRangeRequestRangeType range;
		union _BACnetServiceReadRangeRange _range;
		UINT totalRecordCount;
	} request;
	struct _result
	{
		struct _resultFlags
		{
			BYTE firstItem;
			BYTE lastItem;
			BYTE moreItems;
		} resultFlags;
		UINT itemCount;
		UINT firstSequenceNumber;
	} result;
}BACnetRange;

typedef struct BACnetServiceReadRangeRequest
{
	BACnetObjectIdentifier objectIdentifier;
	BACnetPropertyIdentifier propertyIdentifier;
	int propertyArrayIndex;
	BACnetReadRangeRequestRangeType range;
	union _BACnetServiceReadRangeRange _range;
}BACnetServiceReadRangeRequest;

typedef struct BACnetServiceReadRangeAck
{
	BACnetObjectIdentifier objectIdentifier;
	BACnetPropertyIdentifier propertyIdentifier;
	int propertyArrayIndex;
	BACnetResultFlags resultFlags;
	UINT itemCount;
	BACnetDataType itemData;
	BYTE firstSequenceNumberFlag;
	UINT firstSequenceNumber;
}BACnetServiceReadRangeAck;

typedef struct BACnetServiceReadRange
{
	address_binding device;
	BACnetServiceReadRangeRequest *request;
	BACnetServiceReadRangeAck *ack;
	BACnetError *error;
}BACnetServiceReadRange;



typedef struct BACnetSelectionCriteria
{
	BACnetPropertyIdentifier propertyIdentifier;
	int propertyArrayIndex;
	BACnetRelationalSpecifier relationalSpecifier;
	BACnetDataType comparisonValue;
}BACnetSelectionCriteria;

typedef struct BACnetServiceReadPropertyConditionalRequest
{
	BACnetReadPropertyConditionalSelectionLogic selectionLogic;
	SequenceOfSelectionCriteria listOfSelectionCriteria;
	SequenceOfBACnetPropertyReference listOfPropertyReferences;
}BACnetServiceReadPropertyConditionalRequest;

typedef struct BACnetServiceReadPropertyConditionalAck
{
	SequenceOfReadAccessResult listOfReadAccessResults;
}BACnetServiceReadPropertyConditionalAck;

typedef struct BACnetServiceReadPropertyConditional
{
	address_binding device;
	BACnetServiceReadPropertyConditionalRequest *request;
	BACnetServiceReadPropertyConditionalAck *ack;
	BACnetError *error;
}BACnetServiceReadPropertyConditional;

typedef struct BACnetServiceReadPropertyMultipleRequest
{
	SequenceOfReadAccessSpecification listOfReadAccessSpecs;
}BACnetServiceReadPropertyMultipleRequest;

typedef struct BACnetServiceReadPropertyMultipleAck
{
	SequenceOfReadAccessResult listOfReadAccessResult;
}BACnetServiceReadPropertyMultipleAck;

typedef struct BACnetServiceReadPropertyMultiple
{
	address_binding device;
	BACnetServiceReadPropertyMultipleRequest *request;
	BACnetServiceReadPropertyMultipleAck *ack;
	BACnetError *error;
}BACnetServiceReadPropertyMultiple;

typedef struct BACnetServiceRemoveListElementRequest
{
	BACnetObjectIdentifier objectIdentifier;
	BACnetPropertyIdentifier propertyIdentifier;
	int propertyArrayIndex;
	BACnetDataType listOfElements;
}BACnetServiceRemoveListElementRequest;

typedef struct BACnetServiceRemoveListElement
{
	address_binding device;
	BACnetServiceRemoveListElementRequest *request;
	BYTE ack;
	BACnetError *error;
}BACnetServiceRemoveListElement;

typedef struct BACnetServiceWritePropertyRequest
{
	BACnetObjectIdentifier objectIdentifier;
	BACnetPropertyIdentifier propertyIdentifier;
	int propertyArrayIndex;
	BACnetDataType propertyValue;
	UINT priority;
}BACnetServiceWritePropertyRequest;

typedef struct BACnetServiceWriteProperty
{
	address_binding device;
	BYTE ack;
	BACnetServiceWritePropertyRequest *request;
	BACnetError *error;
}BACnetServiceWriteProperty;

typedef struct BACnetServiceWritePropertyMultipleRequest
{
	SequenceOfWriteAccessSpecification listOfWriteAccessSpecifications;
}BACnetServiceWritePropertyMultipleRequest;

typedef struct BACnetServiceWritePropertyMultiple
{
	address_binding device;
	BYTE ack;
	BACnetServiceWritePropertyMultipleRequest *request;
	BACnetError *error;
}BACnetServiceWritePropertyMultiple;

//Modified CG 30 Sep 2005
/***********New Services****************/

typedef struct BACnetServiceLifeSafetyOperationRequest
{
	UINT requestingProcessIdentifier;
	BACnetCharacterString requestingSource;
	BACnetLifeSafetyOperation request;
	BACnetObjectIdentifier objectIdentifier;
	BYTE objectIdentifierFlag;
}BACnetServiceLifeSafetyOperationRequest;

typedef struct BACnetServiceLifeSafetyOperation
{
	address_binding device;
	BYTE ack;
	BACnetServiceLifeSafetyOperationRequest *request;
	BACnetError *error;
}BACnetServiceLifeSafetyOperation;

typedef struct BACnetServiceSubscribeCOVPropertyRequest
{
	UINT subscriberProcessIdentifier;
	BACnetObjectIdentifier monitoredObjectIdentifier;
	BYTE issueConfirmedNotifications;
	int lifetime;
	BACnetPropertyReference monitoredPropertyIdentifier;
	float covIncrement;
	BYTE covIncrementPresentFlag;
}BACnetServiceSubscribeCOVPropertyRequest;

typedef struct BACnetServiceSubscribeCOVProperty
{
	address_binding device;
	BYTE ack;
	BACnetServiceSubscribeCOVPropertyRequest *request;
	BACnetError *error;
}BACnetServiceSubscribeCOVProperty;

typedef struct BACnetServiceGetEventInformationRequest
{
	BYTE lastReceivedObjectIdentifierPresentFlag;
	BACnetObjectIdentifier lastReceivedObjectIdentifier;
}BACnetServiceGetEventInformationRequest;

typedef struct BACnetEventSummaryElement
{
	BACnetObjectIdentifier objectIdentifier;
	BACnetEventState eventState;
	BACnetEventTransitionBits acknowledgedTransitions;
	BACnetEventTimeStamps eventTimeStamps;
	BACnetNotifyType notifyType;
	BACnetEventTransitionBits eventEnable;
	SequenceOfUnsigned eventPriorities;
}BACnetEventSummaryElement;

typedef struct BACnetServiceGetEventInformationAck
{
	SequenceOfEventSummaries listOfEventSummaries;
	BYTE moreEvents;
}BACnetServiceGetEventInformationAck;

typedef struct BACnetServiceGetEventInformation
{
	address_binding device;
	BACnetServiceGetEventInformationRequest *request;
	BACnetServiceGetEventInformationAck *ack;
	BACnetError *error;
}BACnetServiceGetEventInformation;



/***********UnConfirmed Alarm and Event Services****************/

typedef struct BACnetServiceUnconfirmedCOVNotification
{
	int deviceID;
	BACnetAddress address;
	BACnetServiceCOVNotificationRequest *request;
}BACnetServiceUnconfirmedCOVNotification;

typedef struct BACnetServiceUnconfirmedEventNotification
{
	int deviceID;
	BACnetAddress address;
	BACnetServiceEventNotificationRequest *request;
}BACnetServiceUnconfirmedEventNotification;

/***********UnConfirmed Remote Device Management Services****************/

typedef struct BACnetServiceIam
{
    BACnetObjectIdentifier iAmDeviceIdentifier;
    UINT maxAPDULengthAccepted;
    BACnetSegmentation segmentationSupported;
    UINT vendorID;
}BACnetServiceIam;

typedef struct BACnetServiceIHave
{
    BACnetObjectIdentifier deviceIdentifier;
    BACnetObjectIdentifier objectIdentifier;
	BACnetCharacterString objectName;
}BACnetServiceIHave;

typedef struct BACnetServiceUnconfirmedPrivateTransfer
{
	int deviceID;
	BACnetServicePrivateTransferRequest *request;
}BACnetServiceUnconfirmedPrivateTransfer;

typedef struct BACnetServiceUnconfirmedTextMessage
{
	int deviceID;
	BACnetServiceTextMessageRequest *request;
}BACnetServiceUnconfirmedTextMessage;

typedef struct BACnetServiceTimeSynchronization
{
	BACnetDateTime time;
}BACnetServiceTimeSynchronization;

typedef struct BACnetServiceWhoHas
{
	struct BACnetWhoHasLimits
	{
		int deviceRangeLowLimit;
		int deviceRangeHighLimit;
	}limits;
	BACnetObjectChoice objectChoice;
	union _BACnetObjectChoice
	{
		BACnetObjectIdentifier objectIdentifier;
		BACnetDataType objectName;
	}object;
}BACnetServiceWhoHas;

typedef struct BACnetServiceWhoIs
{
    int lowLimit;
    int highLimit;
}BACnetServiceWhoIs;

typedef struct BACnetServiceUtcTimeSynchronization
{
	BACnetDateTime time;
}BACnetServiceUtcTimeSynchronization;


typedef struct BACnetMessageBuffer
{
    BYTE *buffer; //Buffer for holding PDU message
    UINT position; //Current position inside Buffer
    UINT length; //Size of the Buffer
}BACnetMessageBuffer;

typedef struct BACnetPriorityArrayAna
{
	float priorityArray[16];
	unsigned int bits;
	float relinquishDefault;
	BACnetReliability reliability;
	BYTE outOfService;
}BACnetPriorityArrayAna;

typedef struct BACnetPriorityArrayBin
{
	BACnetBinaryPV priorityArray[16];
	unsigned int bits;
	BACnetBinaryPV relinquishDefault;
	BACnetReliability reliability;
	BYTE outOfService;
}BACnetPriorityArrayBin;

typedef struct BACnetPriorityArrayUint
{
	UINT priorityArray[16];
	unsigned int bits;
	UINT relinquishDefault;
	BACnetReliability reliability;
	BYTE outOfService;
}BACnetPriorityArrayUint;

typedef struct BACnetObjectAveraging
{
	//Required Properties
	UINT attemptedSamples;
	BACnetDeviceObjectPropertyReference objectPropertyReference;
	UINT windowInterval;
	UINT windowSamples;
	//Required for holding samples
	ListOfBACnetLogRecord buffer;
	time_t lastSample;
}BACnetObjectAveraging;

typedef struct BACnetObjectAnalogInput
{
    float presentValue;
	char* deviceType;
	BACnetEventState eventState;
    BACnetReliability reliability;
    BYTE outOfService;
	UINT updateInterval;
	BACnetEngineeringUnits engineeringUnits;
	float minPresValue;
	float maxPresValue;
	float resolution;
	float covIncrement;
	UINT timeDelay;
	UINT notificationClass;
	float highLimit;
	float lowLimit;
	float deadband;
	BYTE limitEnable;
	BYTE eventEnable;
	BYTE ackedTransitions;
	BACnetNotifyType notifyType;
	struct timeb eventTimeStamps[3];
	time_t lastTransition;
}BACnetObjectAnalogInput;

typedef struct BACnetObjectAnalogOutput
{
	char* deviceType;
	BACnetEventState eventState;
	BACnetEngineeringUnits engineeringUnits;
	float minPresValue;
	float maxPresValue;
	float resolution;
	BACnetPriorityArrayAna presentValue;
	float covIncrement;
	UINT timeDelay;
	UINT notificationClass;
	float highLimit;
	float lowLimit;
	float deadband;
	BYTE limitEnable;
	BYTE eventEnable;
	BYTE ackedTransitions;
	BACnetNotifyType notifyType;
	struct timeb eventTimeStamps[3];
	time_t lastTransition;
}BACnetObjectAnalogOutput;

typedef struct BACnetObjectAnalogValue
{
	BACnetEventState eventState;
	BACnetEngineeringUnits engineeringUnits;
	BACnetPriorityArrayAna presentValue;
	float covIncrement;
	UINT timeDelay;
	UINT notificationClass;
	float highLimit;
	float lowLimit;
	float deadband;
	BYTE limitEnable;
	BYTE eventEnable;
	BYTE ackedTransitions;
	BACnetNotifyType notifyType;
	struct timeb eventTimeStamps[3];
	time_t lastTransition;
}BACnetObjectAnalogValue;

typedef struct BACnetObjectBinaryInput
{
    BACnetBinaryPV presentValue;
	char* deviceType;
	BACnetEventState eventState;
    BACnetReliability reliability;
    BYTE outOfService;
	BACnetPolarity polarity;
	char* inactiveText;
	char* activeText;
	struct timeb changeOfStateTime;
	UINT changeOfStateCount;
	struct timeb timeOfStateCountReset;
	UINT elapsedActiveTime;
	struct timeb timeOfActiveTimeReset;
	UINT timeDelay;
	UINT notificationClass;
	BACnetBinaryPV alarmValue;
	BYTE eventEnable;
	BYTE ackedTransitions;
	BACnetNotifyType notifyType;
	struct timeb eventTimeStamps[3];
	time_t lastTransition;
}BACnetObjectBinaryInput;

typedef struct BACnetObjectBinaryOutput
{
	BACnetPriorityArrayBin presentValue;
	char* deviceType;
	BACnetEventState eventState;
	BACnetPolarity polarity;
	char* inactiveText;
	char* activeText;
	struct timeb changeOfStateTime;
	UINT changeOfStateCount;
	struct timeb timeOfStateCountReset;
	UINT elapsedActiveTime;
	struct timeb timeOfActiveTimeReset;
	UINT minimumOffTime;
	UINT minimumOnTime;
	UINT timeDelay;
	UINT notificationClass;
	BACnetBinaryPV feedbackValue;
	BYTE eventEnable;
	BYTE ackedTransitions;
	BACnetNotifyType notifyType;
	struct timeb eventTimeStamps[3];
	time_t lastTransition;
}BACnetObjectBinaryOutput;

typedef struct BACnetObjectBinaryValue
{
	BACnetPriorityArrayBin presentValue;
	BACnetEventState eventState;
	char* inactiveText;
	char* activeText;
	struct timeb changeOfStateTime;
	UINT changeOfStateCount;
	struct timeb timeOfStateCountReset;
	UINT elapsedActiveTime;
	struct timeb timeOfActiveTimeReset;
	UINT minimumOffTime;
	UINT minimumOnTime;
	UINT timeDelay;
	UINT notificationClass;
	BACnetBinaryPV alarmValue;
	BYTE eventEnable;
	BYTE ackedTransitions;
	BACnetNotifyType notifyType;
	struct timeb eventTimeStamps[3];
	time_t lastTransition;
}BACnetObjectBinaryValue;

typedef struct BACnetObjectCalendar
{
    BYTE presentValue;
	SequenceOfBACnetCalendarEntry dateList;
}BACnetObjectCalendar;

typedef struct BACnetObjectCommand
{
    UINT presentValue;
    UINT oldPresentValue;
	BYTE inProcess;
	BYTE allWritesSuccessful;
	SequenceOfBACnetActionList action;
	SequenceOfCharacterString actionText;
}BACnetObjectCommand;

typedef struct BACnetObjectDevice
{
    char* vendorName;
    UINT vendorIdentifier;
    char* modelName;
    char* firmwareRevision;
    char* applicationSoftwareVersion;
    char* location;
    UINT protocolVersion;
    UINT protocolRevision;
    UINT protocolConformanceClass;
	SequenceOfBACnetVTClass vtClassesSupported;
	SequenceOfBACnetVTSession activeVtSessions;
	BYTE daylightSavingsStatus;
	UINT apduSegmentTimeout;
    UINT apduTimeout;
    UINT numberOfAPDUretries;
	SequenceOfBACnetSessionKey listOfSessionKeys;
	SequenceOfBACnetRecipient timeSynchronizationRecipients;
	BACnetDataType configurationFiles;
	UINT backupFailureTimeout;
	UINT databaseRevision;
	struct timeb lastRestoreTime;
}BACnetObjectDevice;

typedef struct BACnetObjectEventEnrolment
{
	BACnetEventType eventType;
	BACnetNotifyType notifyType;
	BACnetEventParameter eventParameters;
	BACnetDeviceObjectPropertyReference objectPropertyReference;
	BACnetEventState eventState;
	BYTE eventEnable;
	BYTE ackedTransitions;
	UINT notificationClass;
	struct timeb eventTimeStamps[3];
	time_t lastTransition;
}BACnetObjectEventEnrolment;

typedef struct BACnetObjectFile
{
	char* fileType;
	BYTE archive;
	BYTE readOnly;
	BYTE restore;
	BACnetFileAccessMethod fileAccessMethod;
    char* fileName;
}BACnetObjectFile;

typedef struct BACnetObjectGroup
{
	BACnetDataType listOfGroupMembers;
}BACnetObjectGroup;

typedef struct BACnetObjectNotificationClass
{
	UINT priority[3];
	BYTE ackRequired;
	SequenceOfBACnetDestination recipientList;
}BACnetObjectNotificationClass;

typedef struct BACnetObjectLoop
{
	float presentValue;
	BACnetEventState eventState;
	BACnetReliability reliability;
	BYTE outOfService;
	UINT updateInterval;
	BACnetEngineeringUnits outputUnits;
	BACnetObjectPropertyReference manipulatedVariableReference;
	BACnetObjectPropertyReference controlledVariableReference;
	float controlledVariableValue;
	BACnetEngineeringUnits controlledVariableUnits;
	BACnetSetpointReference setpointReference;
	float setpoint;
	BACnetAction action;
	float proportionalConstant;
	BACnetEngineeringUnits proportionalConstantUnits;
	float integralConstant;
	BACnetEngineeringUnits integralConstantUnits;
	float derivativeConstant;
	BACnetEngineeringUnits derivativeConstantUnits;
	float bias;
	float maximumOutput;
	float minimumOutput;
	UINT priorityForWriting;
	float covIncrement;
	UINT timeDelay;
	UINT notificationClass;
	float errorLimit;
	BYTE eventEnable;
	BYTE ackedTransitions;
	BACnetNotifyType notifyType;
	struct timeb eventTimeStamps[3];
	float this_target;
	float next_target;
	float rms_error;
	float integral;
	float last_error;
	float last_output;
	time_t lastTransition;
}BACnetObjectLoop;

typedef struct BACnetObjectMultistateInput
{
	UINT presentValue;
	char* deviceType;
	BACnetEventState eventState;
    BACnetReliability reliability;
    BYTE outOfService;
	UINT numberOfStates;
	SequenceOfCharacterString stateText;
	UINT timeDelay;
	UINT notificationClass;
	SequenceOfUnsigned alarmValues;
	SequenceOfUnsigned faultValues;
	BYTE eventEnable;
	BYTE ackedTransitions;
	BACnetNotifyType notifyType;
	struct timeb eventTimeStamps[3];
	time_t lastTransition;
}BACnetObjectMultistateInput;

typedef struct BACnetObjectMultistateOutput
{
	BACnetPriorityArrayUint presentValue;
	char* deviceType;
	BACnetEventState eventState;
	UINT numberOfStates;
	SequenceOfCharacterString stateText;
	UINT timeDelay;
	UINT notificationClass;
	UINT feedbackValue;
	BYTE eventEnable;
	BYTE ackedTransitions;
	BACnetNotifyType notifyType;
	struct timeb eventTimeStamps[3];
	time_t lastTransition;
}BACnetObjectMultistateOutput;

typedef struct BACnetObjectMultistateValue
{
	BACnetPriorityArrayUint presentValue;
	BACnetEventState eventState;
	UINT numberOfStates;
	SequenceOfCharacterString stateText;
	UINT timeDelay;
	UINT notificationClass;
	SequenceOfUnsigned alarmValues;
	SequenceOfUnsigned faultValues;
	BYTE eventEnable;
	BYTE ackedTransitions;
	BACnetNotifyType notifyType;
	struct timeb eventTimeStamps[3];
	time_t lastTransition;
}BACnetObjectMultistateValue;

typedef struct BACnetObjectProgram
{
	BACnetProgramState programState;
	BACnetProgramRequest programChange;
	BACnetProgramError reasonForHalt;
	char* descriptionOfHalt;
	char* programLocation;
	char* instanceOf;
	BACnetReliability reliability;
	BYTE outOfService;
	struct timeb lastScan;
	void* L;
}BACnetObjectProgram;

typedef struct BACnetObjectSchedule
{
	BACnetDataType scheduleDefault; //Added in Addendum 2001 -135a
	BACnetReliability reliability; //Added in Addendum 2001 -135a
	BYTE outOfService; //Added in Addendum 2001 -135a
	BACnetDataType presentValue;
	BACnetDataType oldPresentValue;
	BACnetDateRange effectivePeriod;
	SequenceOfBACnetDailySchedule weeklySchedule;
	SequenceOfSpecialEvent exceptionSchedule;
	SequenceOfBACnetDeviceObjectPropertyReference listOfObjectPropertyReference;
	UINT priorityForWriting;
	time_t last_process_time;
}BACnetObjectSchedule;

typedef struct BACnetObjectTrendLog
{
	BYTE logEnable;
	BACnetDateTime startTime;
	BACnetDateTime stopTime;
	BACnetDeviceObjectPropertyReference logDeviceObjectProperty;
	UINT logInterval;
	UINT CovResubscriptionInterval;
	BACnetClientCOV clientCovIncrement;
	BYTE stopWhenFull;
	UINT bufferSize;
	ListOfBACnetLogRecord logBuffer;
	UINT previousRecordCount;
	UINT previousNotificationCount;
	UINT totalRecordCount;
	UINT notificationThreshold;
	UINT recordsSinceNotification;
	UINT lastNotifyRecord;
	BACnetEventState eventState;
	UINT notificationClass;
	BYTE eventEnable;
	BYTE ackedTransitions;
	BACnetNotifyType notifyType;
	struct timeb eventTimeStamps[3];
	struct timeb lastSample;
	BYTE logDisabled;
	BYTE logPurged;
}BACnetObjectTrendLog;

typedef struct BACnetObjectLifeSafetyPoint
{
    BACnetLifeSafetyState presentValue;
	BACnetLifeSafetyState trackingValue;
	char* deviceType;
	BACnetEventState eventState;
    BACnetReliability reliability;
    BYTE outOfService;
	BACnetLifeSafetyMode mode;
	BACnetListOfLifeSafetyMode acceptedModes;
	UINT timeDelay;
	UINT notificationClass;
	BACnetListOfLifeSafetyState lifeSafetyAlarmValues;
	BACnetListOfLifeSafetyState alarmValues;
	BACnetListOfLifeSafetyState faultValues;
	BYTE eventEnable;
	BYTE ackedTransitions;
	BACnetNotifyType notifyType;
	struct timeb eventTimeStamps[3];
	BACnetSilencedState silenced;
	BACnetLifeSafetyOperation operationExpected;
	BACnetMaintenance maintenanceRequired;
	BYTE setting;
	float directReading;
	BACnetEngineeringUnits engineeringUnits;
	SequenceOfBACnetObjectIdentifier memberOf;
	BACnetLifeSafetyMode oldMode;
	BACnetLifeSafetyState oldState;
	time_t lastTransition;
}BACnetObjectLifeSafetyPoint;

typedef struct BACnetObjectLifeSafetyZone
{
    BACnetLifeSafetyState presentValue;
	BACnetLifeSafetyState trackingValue;
	char* deviceType;
	BACnetEventState eventState;
    BACnetReliability reliability;
    BYTE outOfService;
	BACnetLifeSafetyMode mode;
	BACnetListOfLifeSafetyMode acceptedModes;
	UINT timeDelay;
	UINT notificationClass;
	BACnetListOfLifeSafetyState lifeSafetyAlarmValues;
	BACnetListOfLifeSafetyState alarmValues;
	BACnetListOfLifeSafetyState faultValues;
	BYTE eventEnable;
	BYTE ackedTransitions;
	BACnetNotifyType notifyType;
	struct timeb eventTimeStamps[3];
	BACnetSilencedState silenced;
	BACnetLifeSafetyOperation operationExpected;
	BACnetMaintenance maintenanceRequired;
	SequenceOfBACnetObjectIdentifier memberOf;
	BACnetLifeSafetyMode oldMode;
	BACnetLifeSafetyState oldState;
	time_t lastTransition;
}BACnetObjectLifeSafetyZone;

typedef struct BACnetObjectAccumulator
{
    UINT presentValue;
	char* deviceType;
	BACnetEventState eventState;
    BACnetReliability reliability;
    BYTE outOfService;
	BACnetScale scale;
	BACnetEngineeringUnits engineeringUnits;
	BACnetPrescale prescale;
	UINT maxPresValue;
	struct timeb valueChangeTime;
	UINT valueBeforeChange;
	UINT valueSet;
	BACnetAccumulatorRecord loggingRecord;
	BACnetObjectIdentifier loggingObject;
	UINT pulseRate;
	UINT highLimit;
	UINT lowLimit;
	UINT limitMonitoringInterval;
	UINT notificationClass;
	UINT timeDelay;
	BYTE limitEnable;
	BYTE eventEnable;
	BYTE ackedTransitions;
	BACnetNotifyType notifyType;
	struct timeb eventTimeStamps[3];
	//Not visible to BACnet Clients
	UINT accumulator;
	//Required for holding samples to determine the pulse rate
	ListOfBACnetLogRecord buffer;
	struct timeb first_sample;
	struct timeb last_sample;
	time_t lastTransition;
}BACnetObjectAccumulator;

typedef struct BACnetObjectPulseConverter
{
    float presentValue;
	BACnetObjectPropertyReference inputReference;
	BACnetEventState eventState;
    BACnetReliability reliability;
    BYTE outOfService;
	BACnetEngineeringUnits engineeringUnits;
	float scaleFactor;
	float adjustValue;
	BYTE count_initialized;
	UINT previous_count;
	UINT count;
	struct timeb updateTime;
	struct timeb countChangeTime;
	UINT countBeforeChange;
	float covIncrement;
	float covPeriod;
	UINT notificationClass;
	UINT timeDelay;
	float highLimit;
	float lowLimit;
	float deadband;
	BYTE limitEnable;
	BYTE eventEnable;
	BYTE ackedTransitions;
	BACnetNotifyType notifyType;
	struct timeb eventTimeStamps[3];
	time_t lastTransition;
}BACnetObjectPulseConverter;

typedef struct BACnetObjectContainer
{
	//Device ID of Stored Object
	int device_id;
	//Required Properties common to all objects
    BACnetObjectIdentifier objectIdentifier;
    char* objectName;
	char* description;
	//Storage location for proprietary properties
	void* proprietary_data;
	int proprietary_data_len;
	//Internal Properties common to all objects
	SequenceOfBACnetCOVSubscription activeSubscriptions;
	//Used to flag the entire object as unaccessible
	int error; 
	//Used to fire the callback function read_present_value_notification
	time_t lastReadRequest;

	//Object storage location for standard objects
	union BACnetObjectTypeDetail
	{
		BACnetObjectAnalogInput* analogInput;
		BACnetObjectAnalogOutput* analogOutput;
		BACnetObjectAnalogValue* analogValue;
		BACnetObjectBinaryInput* binaryInput;
		BACnetObjectBinaryOutput* binaryOutput;
		BACnetObjectBinaryValue* binaryValue;
		BACnetObjectCalendar* calendar;
		BACnetObjectCommand* command;
		BACnetObjectDevice* device;
		BACnetObjectEventEnrolment* eventEnrolment;
		BACnetObjectFile* file;
		BACnetObjectGroup* group;
		BACnetObjectLoop* loop;
		BACnetObjectMultistateInput* multistateInput;
		BACnetObjectMultistateOutput* multistateOutput;
		BACnetObjectNotificationClass* notificationClass;
		BACnetObjectProgram* program;
		BACnetObjectSchedule* schedule;
		BACnetObjectAveraging* averaging;
		BACnetObjectMultistateValue* multistateValue;
		BACnetObjectTrendLog* trendLog;
		BACnetObjectLifeSafetyPoint* lifeSafetyPoint;
		BACnetObjectLifeSafetyZone* lifeSafetyZone;
		BACnetObjectAccumulator* accumulator;
		BACnetObjectPulseConverter* pulseConverter;
	} object;
}BACnetObjectContainer;

#endif

