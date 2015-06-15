
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void Ini_PresentValue_Analog(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	float UI			= (float)(0 + 1);

	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_presentValue;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_PresentValue_Multi(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	unsigned UI			= (unsigned)(i + 1);

	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_presentValue;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}


void Ini_Description(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	char szText[100];
	sprintf(szText, "%02d", i);

	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_description;

	IV.uSize			= (unsigned short)(strlen(szText) + 1);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(szText, IV.uSize);
}

void Ini_DeviceType(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	char szText[100];
	sprintf(szText, "DevType LN%d", i);

	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_deviceType;

	IV.uSize			= (unsigned short)(strlen(szText) + 1);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(szText, IV.uSize);
}

void Ini_Reliability(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	unsigned short UI	= BACnetReliabilityNoFaultDetected;

	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_reliability;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_Out_Of_Service(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	unsigned char UI	= FALSE;

	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_outOfService;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_Update_Interval(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	unsigned UI			= 111 + i;
	
	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_updateInterval;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_Units(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	unsigned short UI	= UNITS_volts_per_degree_Kelvin;
	
	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_units;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_Min_Present_Value(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	float UI			= (float)0.12345;
	
	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_minPresValue;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_Max_Present_Value(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	float UI			= (float)99.12345;
	
	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_maxPresValue;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_Resolution(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	float UI			= (float)1.12345;
	
	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_resolution;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_COV_Increment(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	float UI			= (float)0.22;
	
	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_covIncrement;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_Time_Delay(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	unsigned UI			= 3;
	
	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_timeDelay;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_Notification_Class(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	unsigned UI			= 99;
	
	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_notificationClass;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_High_Limit(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	float UI			= 127;
	
	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_highLimit;

	IV.uSize			= sizeof(float);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_Low_Limit(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	float UI			= 1;
	
	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_lowLimit;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_Deadband(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	float UI			= (float)3.33;
	
	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_deadband;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_Limit_Enable(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	unsigned char UI	= 0x03;
	
	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_limitEnable;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_Event_Enable(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	unsigned char UI	= 0x05;
	
	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_eventEnable;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_Notify_Type(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	unsigned short UI	= NotifyTypeEvent;
	
	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_notifyType;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_Profile_Name(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	char szText[100];
	sprintf(szText, "Profile LN%d", i);

	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_profileName;

	IV.uSize			= (unsigned short)(strlen(szText) + 1);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(szText, IV.uSize);
}

void Ini_Number_Of_States(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	unsigned UI			= 10;
	
	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_numberOfStates;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_State_Text(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));

	char szText[100];
	int  iLen = 0;

	for (int s = 0; s < 10; s++)
	{
		sprintf(szText, "State%d", s + 1);
		iLen += strlen(szText) + 1;
	}

	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_stateText;

	IV.uSize			= (unsigned short)(iLen);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));

	for (s = 0; s < 10; s++)
	{
		sprintf(szText, "State%d", s + 1);
		F->Write(szText, strlen(szText) + 1);
	}
}

void Ini_Alarm_Values(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	unsigned UI[4]		= {5,6,7,8};
	
	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_alarmValues;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

void Ini_Fault_Values(int i, CFile *F)
{
	XBACnetIniVal IV;
	memset(&IV, 0x00, sizeof(IV));
	
	unsigned UI[2]		= {4,9};
	
	IV.iObjectID		= ID_OFFS_AO_SRV + i;
	IV.eType			= Property_faultValues;

	IV.uSize			= sizeof(UI);
	IV.iArrayIndex		= -1;

	F->Write(&IV, sizeof(IV));
	F->Write(&UI, IV.uSize);
}

