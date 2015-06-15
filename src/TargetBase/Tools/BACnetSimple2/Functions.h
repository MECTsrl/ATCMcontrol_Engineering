
/* Functions
 */
void DoIt(void);

void NC_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal);
void AO_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal);
void AI_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal);
void AV_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal);
void BO_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal);
void BI_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal);
void BV_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal);
void MO_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal);
void MI_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal);
void MV_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal);
void TL_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal);
void SC_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal);


/* ENUMERATED - IEC_WORD
 * ----------------------------------------------------------------------------
 */
void Ini_Reliability(UINT ulObjID, CFile *F);
void Ini_Units(UINT ulObjID, CFile *F);
void Ini_Notify_Type(UINT ulObjID, CFile *F);
void Ini_Polarity(UINT ulObjID, CFile *F);


/* Boolean / IEC_BOOL
 * ----------------------------------------------------------------------------
 */
void Ini_Out_Of_Service(UINT ulObjID, CFile *F);
void Ini_Log_Enable(UINT ulObjID, CFile *F);
void Ini_Stop_When_Full(UINT ulObjID, CFile *F);


/* REAL - IEC_REAL
 * ----------------------------------------------------------------------------
 */
void Ini_Present_Value_A(UINT ulObjID, CFile *F);
void Ini_Min_Pres_Value(UINT ulObjID, CFile *F);
void Ini_Max_Pres_Value(UINT ulObjID, CFile *F);
void Ini_Resolution(UINT ulObjID, CFile *F);
void Ini_COV_Increment(UINT ulObjID, CFile *F);
void Ini_High_Limit(UINT ulObjID, CFile *F);
void Ini_Low_Limit(UINT ulObjID, CFile *F);
void Ini_Deadband(UINT ulObjID, CFile *F);


/* Unsigned / IEC_UDINT
 * ----------------------------------------------------------------------------
 */
void Ini_Time_Delay(UINT ulObjID, CFile *F);
void Ini_Notification_Class(UINT ulObjID, CFile *F);
void Ini_Update_Interval(UINT ulObjID, CFile *F);
void Ini_Minimum_Off_Time(UINT ulObjID, CFile *F);
void Ini_Minimum_On_Time(UINT ulObjID, CFile *F);
void Ini_Present_Value_M(UINT ulObjID, CFile *F);
void Ini_Number_Of_States(UINT ulObjID, CFile *F);
void Ini_Feedback_Value_M(UINT ulObjID, CFile *F);
void Ini_Buffer_Size(UINT ulObjID, CFile *F);
void Ini_Log_Interval(UINT ulObjID, CFile *F);
void Ini_COV_Resubscription_Interval(UINT ulObjID, CFile *F);
void Ini_Priority_For_Writing(UINT ulObjID, CFile *F);
void Ini_Notification_Treshold(UINT ulObjID, CFile *F);


/* BACnetBinaryPV / IEC_WORD
 * ----------------------------------------------------------------------------
 */
void Ini_Present_Value_B(UINT ulObjID, CFile *F);
void Ini_Feedback_Value_B(UINT ulObjID, CFile *F);
void Ini_Alarm_Value(UINT ulObjID, CFile *F);


/* CharacterString - IEC_CHAR *
 * ----------------------------------------------------------------------------
 */
void Ini_Description(UINT ulObjID, CFile *F, IEC_CHAR *szValue);
void Ini_Device_Type(UINT ulObjID, CFile *F, IEC_CHAR *szValue);
void Ini_Inactive_Text(UINT ulObjID, CFile *F);
void Ini_Active_Text(UINT ulObjID, CFile *F);


/* Sequence/List/Array of Unsigned - IEC_UDINT[x]
 * ----------------------------------------------------------------------------
 */
void Ini_Priority(UINT ulObjID, CFile *F);
void Ini_Alarm_Values(UINT ulObjID, CFile *F);
void Ini_Fault_Values(UINT ulObjID, CFile *F);


/* Sequence/List/Array of CharacterString - IEC_CHAR * [x]
 * ----------------------------------------------------------------------------
 */
void Ini_State_Text(UINT ulObjID, CFile *F);


/* Bitstring (2) / IEC_BYTE
 * ----------------------------------------------------------------------------
 */
void Ini_Limit_Enable(UINT ulObjID, CFile *F);


/* Bitstring (3) - IEC_BYTE
 * ----------------------------------------------------------------------------
 */
void Ini_Ack_Required(UINT ulObjID, CFile *F);
void Ini_Event_Enable(UINT ulObjID, CFile *F);


/* BACnetDataTime
 * ----------------------------------------------------------------------------
 */
void Ini_Start_Time(UINT ulObjID, CFile *F);
void Ini_Stop_Time(UINT ulObjID, CFile *F);


/* BACnetDeviceObjectPropertyReference
 * ----------------------------------------------------------------------------
 */
void Ini_Log_DevObjPropRev(UINT ulObjID, CFile *F);


/* Array of BACnetDeviceObjectPropertyReference
 * ----------------------------------------------------------------------------
 */
void Ini_List_Of_DevObjPropRef(UINT ulObjID, CFile *F);


/* BACnetClientCOV
 * ----------------------------------------------------------------------------
 */
void Ini_Client_COV_Increment(UINT ulObjID, CFile *F);


/* BACnetDateRange
 * ----------------------------------------------------------------------------
 */
void Ini_Effective_Period(UINT ulObjID, CFile *F);


/* BACnetArray[7] of BACnetDailySchedule
 * ----------------------------------------------------------------------------
 */
void Ini_Weekly_Schedule(UINT ulObjID, CFile *F);


/* BACnetArray[N] of BACnetSpecialEvent
 * ----------------------------------------------------------------------------
 */
void Ini_Exception_Schedule(UINT ulObjID, CFile *F);


/* Any - XBACnetAny
 * ----------------------------------------------------------------------------
 */
void Ini_Present_Value_Any(UINT ulObjID, CFile *F);
void Ini_Schedule_Default(UINT ulObjID, CFile *F);
