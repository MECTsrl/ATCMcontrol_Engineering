// BACnetSimple2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BACnetSimple2.h"

#include "..\..\Control\inc.bac\bacnapi.h"
#include "..\..\Control\inc.bac\bacnet_template_library.h"

#include "..\..\inc\vmSharedDef.h"
#include "..\..\inc\bacShared.h"

#include "functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* ENUMERATED - IEC_WORD
 * ----------------------------------------------------------------------------
 */
void Ini_Reliability(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_WORD Value	= BACnetReliabilityNoFaultDetected;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_reliability;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Units(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_WORD Value	= UNITS_newton;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_units;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Notify_Type(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_WORD Value	= NotifyTypeEvent;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_notifyType;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Polarity(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_WORD Value	= BACnetPolarityNormal;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_polarity;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}



/* Boolean / IEC_BOOL
 * ----------------------------------------------------------------------------
 */
void Ini_Out_Of_Service(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_BOOL Value	= FALSE;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_outOfService;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Log_Enable(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_BOOL Value	= TRUE;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_logEnable;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Stop_When_Full(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_BOOL Value	= TRUE;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_stopWhenFull;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}


/* REAL - IEC_REAL
 * ----------------------------------------------------------------------------
 */
void Ini_Present_Value_A(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_REAL Value	= (IEC_REAL)3.14;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_presentValue;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Min_Pres_Value(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_REAL Value	= (IEC_REAL)0.0;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_minPresValue;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Max_Pres_Value(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_REAL Value	= (IEC_REAL)1000.0;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_maxPresValue;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Resolution(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_REAL Value	= (IEC_REAL)0.02;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_resolution;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_COV_Increment(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_REAL Value	= (IEC_REAL)0.5;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_covIncrement;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_High_Limit(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_REAL Value	= (IEC_REAL)500.0;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_highLimit;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Low_Limit(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_REAL Value	= (IEC_REAL)1.00;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_lowLimit;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Deadband(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_REAL Value	= (IEC_REAL)1.11;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_deadband;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}




/* Unsigned / IEC_UDINT
 * ----------------------------------------------------------------------------
 */
void Ini_Time_Delay(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_UDINT Value	= 0;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_timeDelay;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Notification_Class(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_UDINT Value	= 1;	// must be an existing NC ID!

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_notificationClass;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Update_Interval(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_UDINT Value	= 2;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_updateInterval;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Minimum_Off_Time(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_UDINT Value	= 2;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_minimumOffTime;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Minimum_On_Time(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_UDINT Value	= 2;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_minimumOnTime;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Present_Value_M(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_UDINT Value	= 2;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_presentValue;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Number_Of_States(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_UDINT Value	= 5;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_numberOfStates;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Feedback_Value_M(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_UDINT Value	= 3;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_feedbackValue;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Buffer_Size(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_UDINT Value	= 60;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_bufferSize;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Log_Interval(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_UDINT Value	= 100;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_logInterval;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_COV_Resubscription_Interval(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_UDINT Value	= 100;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_covResubscriptionInterval;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Priority_For_Writing(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_UDINT Value	= 8;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_priorityForWriting;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Notification_Treshold(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_UDINT Value	= 20;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_notificationThreshold;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}



/* BACnetBinaryPV / IEC_WORD
 * ----------------------------------------------------------------------------
 */
void Ini_Present_Value_B(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_WORD Value	= BACnetBinaryPVActive;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_presentValue;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Feedback_Value_B(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_WORD Value	= BACnetBinaryPVActive;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_feedbackValue;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Alarm_Value(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_WORD Value	= BACnetBinaryPVInactive;

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_alarmValue;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}



/* CharacterString - IEC_CHAR *
 * ----------------------------------------------------------------------------
 */
void Ini_Description(UINT ulObjID, CFile *F, IEC_CHAR *szValue)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_description;

	IV.uDataSize	= (unsigned short)(strlen(szValue) + 1);	// + ZB!
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(szValue, IV.uDataSize);		// including ZB!
}

void Ini_Device_Type(UINT ulObjID, CFile *F, IEC_CHAR *szValue)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_deviceType;

	IV.uDataSize	= (unsigned short)(strlen(szValue) + 1);	// + ZB!
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(szValue, IV.uDataSize);		// including ZB!
}

void Ini_Inactive_Text(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_CHAR *szValue = "Hugo";

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_inactiveText;

	IV.uDataSize	= (unsigned short)(strlen(szValue) + 1);	// + ZB!
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(szValue, IV.uDataSize);		// including ZB!
}

void Ini_Active_Text(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_CHAR *szValue = "Otto";

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_activeText;

	IV.uDataSize	= (unsigned short)(strlen(szValue) + 1);	// + ZB!
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(szValue, IV.uDataSize);		// including ZB!
}



/* Sequence/List/Array of Unsigned - IEC_UDINT[x]
 * ----------------------------------------------------------------------------
 */
void Ini_Priority(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_UDINT pValue[3] = {1,2,3};

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_priority;

	IV.uDataSize	= sizeof(pValue);
	IV.uArrayElem	= 3;

	F->Write(&IV, sizeof(IV));
	F->Write(pValue, IV.uDataSize);
}

void Ini_Alarm_Values(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_UDINT pValue[1] = {4};

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_alarmValues;

	IV.uDataSize	= sizeof(pValue);
	IV.uArrayElem	= 1;

	F->Write(&IV, sizeof(IV));
	F->Write(pValue, IV.uDataSize);
}

void Ini_Fault_Values(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_UDINT pValue[1] = {5};

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_faultValues;

	IV.uDataSize	= sizeof(pValue);
	IV.uArrayElem	= 1;

	F->Write(&IV, sizeof(IV));
	F->Write(pValue, IV.uDataSize);
}



/* Sequence/List/Array of CharacterString - IEC_CHAR * [x]
 * ----------------------------------------------------------------------------
 */
void Ini_State_Text(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_CHAR szValue[5][7] = {"State1","State2","State3","State4","State5"};

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_stateText;

	IV.uDataSize	= sizeof(szValue);	// + ZB!
	IV.uArrayElem	= 5;

	F->Write(&IV, sizeof(IV));
	F->Write(szValue, IV.uDataSize);		// including ZB!
}



/* Bitstring (2) / IEC_BYTE
 * ----------------------------------------------------------------------------
 */
void Ini_Limit_Enable(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_BYTE Value	= 0;		// 2 Bits:

	Value			|= 0x01;	// HIGH-LIMIT
	Value			|= 0x02;	// LOW-LIMIT

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_limitEnable;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}



/* Bitstring (3) - IEC_BYTE
 * ----------------------------------------------------------------------------
 */
void Ini_Ack_Required(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_BYTE Value	= 0;		// 3 Bits:

	Value			|= 0x01;	// TO-OFFNORMAL
	Value			|= 0x02;	// TO-FAULT
	Value			|= 0x04;	// TO-NORMAL

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_ackRequired;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Event_Enable(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	IEC_BYTE Value	= 0;		// 3 Bits:

	Value			|= 0x01;	// TO-OFFNORMAL
	Value			|= 0x02;	// TO-FAULT
	Value			|= 0x04;	// TO-NORMAL

	IV.iObjectID	= ulObjID;
	IV.eType		= Property_eventEnable;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}



/* BACnetDateTime - XBACnetDateTime
 * ----------------------------------------------------------------------------
 */
void Ini_Start_Time(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	XBACnetDateTime Value;

	Value.Date.day			= 2;
	Value.Date.month		= 10;
	Value.Date.year			= 106;
	Value.Date.dayOfWeek	= 1;

	Value.Time.hour				= 17;
	Value.Time.minute			= 30;
	Value.Time.second			= 11;
	Value.Time.hundrethsOfASec	= 0;
		
	IV.iObjectID	= ulObjID;
	IV.eType		= Property_startTime;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

void Ini_Stop_Time(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	XBACnetDateTime Value;

	Value.Date.day			= 3;
	Value.Date.month		= 10;
	Value.Date.year			= 106;
	Value.Date.dayOfWeek	= 2;

	Value.Time.hour				= 11;
	Value.Time.minute			= 11;
	Value.Time.second			= 11;
	Value.Time.hundrethsOfASec	= 0;
		
	IV.iObjectID	= ulObjID;
	IV.eType		= Property_stopTime;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}



/* BACnetDeviceObjectPropertyReference
 * ----------------------------------------------------------------------------
 */
void Ini_Log_DevObjPropRev(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	XBACnetDevObjPropRef Value;

	Value.eObjType		= BACnetObjectTypeAnalogOutput;
	Value.ePropType		= Property_presentValue;
	Value.iArrayIndex	= -1;
	Value.iDeviceID		= DEVICE_LOCAL;
	Value.iObjectID		= ulObjID;
		
	IV.iObjectID	= ulObjID;
	IV.eType		= Property_logDeviceObjectProperty;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}



/* Array of BACnetDeviceObjectPropertyReference
 * ----------------------------------------------------------------------------
 */
void Ini_List_Of_DevObjPropRef(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	XBACnetDevObjPropRef Value;
		
	IV.iObjectID	= ulObjID;
	IV.eType		= Property_listOfObjectReferences;

	IV.uArrayElem	= 1;
	IV.uDataSize	= IV.uArrayElem * sizeof(Value);

	F->Write(&IV, sizeof(IV));

//	for (int i = 0; i < IV.uArrayElem; i++)
	{
		Value.eObjType		= BACnetObjectTypeAnalogOutput;
		Value.ePropType		= Property_presentValue;
		Value.iArrayIndex	= -1;
		Value.iDeviceID		= DEVICE_LOCAL;
		Value.iObjectID		= 1;
		F->Write(&Value, sizeof(Value));
#if 0
		Value.eObjType		= BACnetObjectTypeAnalogValue;
		Value.ePropType		= Property_presentValue;
		Value.iArrayIndex	= -1;
		Value.iDeviceID		= DEVICE_LOCAL;
		Value.iObjectID		= 1;
		F->Write(&Value, sizeof(Value));
#endif
	}

}



/* BACnetClientCOV
 * ----------------------------------------------------------------------------
 */
void Ini_Client_COV_Increment(UINT ulObjID, CFile *F)
{
	
}



/* BACnetDateRange - XBACnetDateRange
 * ----------------------------------------------------------------------------
 */
void Ini_Effective_Period(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	XBACnetDateRange Value;

	Value.Start.day			= 1;
	Value.Start.month		= 8;
	Value.Start.year		= 106;
	Value.Start.dayOfWeek	= 0xFF;

	Value.End.day			= 31;
	Value.End.month			= 8;
	Value.End.year			= 107;
	Value.End.dayOfWeek		= 0xFF;
		
	IV.iObjectID	= ulObjID;
	IV.eType		= Property_effectivePeriod;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}



/* BACnetArray[7] of BACnetDailySchedule
 * ----------------------------------------------------------------------------
 */
void Ini_Weekly_Schedule(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	XBACnetDailySchedule Value;
	XBACnetTimeValue TV;

	memset(&TV, 0x00, sizeof(TV));
		
	IV.iObjectID	= ulObjID;
	IV.eType		= Property_weeklySchedule;

	IV.uDataSize	= 7 * (sizeof(Value) + 2 * sizeof(XBACnetTimeValue));
	IV.uArrayElem	= 7;

	F->Write(&IV, sizeof(IV));

	for (int i = 0; i < 7; i++)
	{
		Value.ulElements = 2;	
		F->Write(&Value, sizeof(Value));

		TV.Value.wType			= ANY_REAL;
		TV.Value.real_value		= (float)11;
		TV.Time.hour			= 13;
		TV.Time.minute			= 30;
		TV.Time.second			= 0;
		TV.Time.hundrethsOfASec	= 0;
		F->Write(&TV, sizeof(TV));

		TV.Value.wType			= ANY_REAL;
		TV.Value.real_value		= (float)14;
		TV.Time.hour			= 14;
		TV.Time.minute			= 31;
		TV.Time.second			= 0;
		TV.Time.hundrethsOfASec	= 0;
		F->Write(&TV, sizeof(TV));
	}
}



/* BACnetArray[N] of BACnetSpecialEvent
 * ----------------------------------------------------------------------------
 */
void Ini_Exception_Schedule(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	XBACnetSpecialEvent Value;
	XBACnetTimeValue TV;

	memset(&Value, 0x00, sizeof(Value));
	memset(&TV, 0x00, sizeof(TV));
		
	IV.iObjectID	= ulObjID;
	IV.eType		= Property_exceptionSchedule;

	IV.uDataSize	= 1 * (sizeof(Value) + 2 * sizeof(XBACnetTimeValue));
	IV.uArrayElem	= 1;

	F->Write(&IV, sizeof(IV));

	for (int i = 0; i < 1; i++)
	{
		Value.wType			= BACnetSpecialEventPeriodTypeCalendarEntry;
		Value.EventPriority = i;

		Value.CalEntry.wType	= BACnetCalendarEntryTypeDateRange;
		Value.CalEntry.DateRange.Start.day			= 1;
		Value.CalEntry.DateRange.Start.month		= 1;
		Value.CalEntry.DateRange.Start.year			= 106;
		Value.CalEntry.DateRange.Start.dayOfWeek	= 0xff;

		Value.CalEntry.DateRange.End.day			= 1;
		Value.CalEntry.DateRange.End.month			= 1;
		Value.CalEntry.DateRange.End.year			= 107;
		Value.CalEntry.DateRange.End.dayOfWeek		= 0xff;

		Value.ulElements	= 2;	
		F->Write(&Value, sizeof(Value));

		TV.Value.wType			= ANY_REAL;
		TV.Value.real_value		= (float)77;
		TV.Time.hour			= 15;
		TV.Time.minute			= 30;
		TV.Time.second			= 0;
		TV.Time.hundrethsOfASec	= 0;
		F->Write(&TV, sizeof(TV));

		TV.Value.wType			= ANY_REAL;
		TV.Value.real_value		= (float)88;
		TV.Time.hour			= 16;
		TV.Time.minute			= 31;
		TV.Time.second			= 0;
		TV.Time.hundrethsOfASec	= 0;
		F->Write(&TV, sizeof(TV));
	}
}



/* Any - XBACnetAny
 * ----------------------------------------------------------------------------
 */
void Ini_Present_Value_Any(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	XBACnetAny Value;

	memset(&Value, 0x00, sizeof(Value));

	Value.wType		 = ANY_REAL;
	Value.real_value = (float)44.44;
		
	IV.iObjectID	= ulObjID;
	IV.eType		= Property_presentValue;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}


void Ini_Schedule_Default(UINT ulObjID, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	XBACnetAny Value;

	memset(&Value, 0x00, sizeof(Value));

	Value.wType		 = ANY_REAL;
	Value.real_value = (float)55.55;
		
	IV.iObjectID	= ulObjID;
	IV.eType		= Property_scheduleDefault;

	IV.uDataSize	= sizeof(Value);
	IV.uArrayElem	= 0;

	F->Write(&IV, sizeof(IV));
	F->Write(&Value, IV.uDataSize);
}

