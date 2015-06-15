
#define XXX_SERVERxx



#define DEVICE_1	4711
#define DEVICE_2	2709

#if defined(XXX_SERVER)
  #define SRV_COUNT 1
  #define CLT_COUNT 0
#else
  #define SRV_COUNT 0
  #define CLT_COUNT 1
#endif

#define ID_OFFS_AO_SRV	0
#define ID_OFFS_AO_CLT	0

#define OBJ_OFFS		80

#define DL_DIR			"D:\\4C\\Engineering\\bin\\4CWin\\vmm\\_cust\\"


void Ini_PresentValue_Analog(int i, CFile *F);	
void Ini_Description(int i, CFile *F);
void Ini_DeviceType(int i, CFile *F);
void Ini_Reliability(int i, CFile *F);
void Ini_Out_Of_Service(int i, CFile *F);
void Ini_Update_Interval(int i, CFile *F);
void Ini_Units(int i, CFile *F);
void Ini_Min_Present_Value(int i, CFile *F);
void Ini_Max_Present_Value(int i, CFile *F);
void Ini_Resolution(int i, CFile *F);
void Ini_COV_Increment(int i, CFile *F);
void Ini_Time_Delay(int i, CFile *F);
void Ini_Notification_Class(int i, CFile *F);
void Ini_High_Limit(int i, CFile *F);
void Ini_Low_Limit(int i, CFile *F);
void Ini_Deadband(int i, CFile *F);
void Ini_Limit_Enable(int i, CFile *F);
void Ini_Event_Enable(int i, CFile *F);
void Ini_Notify_Type(int i, CFile *F);
void Ini_Profile_Name(int i, CFile *F);
void Ini_PresentValue_Multi(int i, CFile *F);	
void Ini_Number_Of_States(int i, CFile *F);
void Ini_State_Text(int i, CFile *F);
void Ini_Alarm_Values(int i, CFile *F);
void Ini_Fault_Values(int i, CFile *F);
