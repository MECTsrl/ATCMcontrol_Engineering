
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int DoIt(void);

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		return DoIt();
	}

	return nRetCode;
}

void WriteStack(XBACnetStack *BS);
void WriteLocal(void);
void WriteRemote(void);

void IniAI(int i);
void IniMV(int i);
void IniNC(int i);


void WriteStack(XBACnetStack *BS)
{
	CFile F;

	memset(BS, 0x00, sizeof(XBACnetStack));

#if defined(XXX_SERVER)
	BS->iDeviceID	= DEVICE_1;
#else
	BS->iDeviceID	= DEVICE_2;
#endif
	
	BS->ulNoLocObj	= SRV_COUNT;
	BS->ulNoRemObj	= CLT_COUNT;

	BS->iDefPrioLoc	= 0;
	BS->iDefPrioRem	= 4;

	BS->iCOVLifeTime	= 1800;
	BS->byConfirmedCOV = 1;

	F.Open(DL_DIR BAC_CFG_STACK, CFile::modeCreate | CFile::modeWrite, NULL);

	F.Write(BS, sizeof(XBACnetStack));

	F.Close();
}

void WriteLocal(void)
{

}

void WriteRemote(void)
{

}

int DoIt(void)
{
	XBACnetStack BS;

	WriteStack(&BS);


	CFile F;

	/* ------------------------------------------------------------------------ */

	F.Open(DL_DIR BAC_CFG_OBJ_LOC, CFile::modeCreate | CFile::modeWrite, NULL);
#if 0
	for (int i = 0; i < SRV_COUNT; i++)
	{
		XBACnetObj XO;
		memset(&XO, 0x00, sizeof(XO));

		XO.eType		= BACnetObjectTypeNotificationClass;

		XO.iDeviceID	= BS.iDeviceID;

		XO.iObjectID	= 99;
		XO.ulOffset		= 2000;

		XO.ulOffIniVal	= i * (sizeof(XBACnetIniVal) + sizeof(float));
		XO.ulNoOfIniVal	= 1;

		XO.ulOffIniTxt	= -1;

		XO.byLowLimitEna	= 0;
		XO.byHighLimitEna	= 0;

		XO.byEvToFault		= 0;
		XO.byEvToNormal		= 0;
		XO.byEvToOffNormal	= 0;

		char szName[100];
		sprintf(szName, "NC%d (%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
	}
#endif

	for (int i = 0; i < SRV_COUNT; i++)
	{
		XBACnetObj XO;
		memset(&XO, 0x00, sizeof(XO));

		XO.eType		= BACnetObjectTypeAnalogOutput;

		XO.iDeviceID	= DEVICE_1;

		XO.iObjectID	= 100 + i;
		XO.ulOffset		= i * 100;

		XO.ulOffIniVal	= 0;
		XO.ulNoOfIniVal	= 17;

		XO.ulOffIniTxt	= -1;

		char szName[100];
		sprintf(szName, "AO%d (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
	}

	F.Close();

	/* ------------------------------------------------------------------------ */

	F.Open(DL_DIR BAC_CFG_OBJ_REM, CFile::modeCreate | CFile::modeWrite, NULL);

	for (i = 0; i < CLT_COUNT; i++)
	{
		XBACnetObj XO;
		memset(&XO, 0x00, sizeof(XO));

		XO.eType		= BACnetObjectTypeAnalogOutput;

		XO.iDeviceID	= BS.iDeviceID == DEVICE_1 ? DEVICE_2 : DEVICE_1;

		XO.iObjectID	= 100 + i;
		XO.ulOffset		= 1000 + i * 100;

		XO.ulOffIniVal	= -1;
		XO.ulNoOfIniVal	= 0;

		XO.ulOffIniTxt	= -1;

		char szName[100];
		sprintf(szName, "AO%d (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
	}

	F.Close();

	/* ------------------------------------------------------------------------ */

	F.Open(DL_DIR BAC_CFG_LOC_INIVAL, CFile::modeCreate | CFile::modeWrite, NULL);
	F.Close();

	for (i = 0; i < SRV_COUNT; i++)
	{
;
	}

#if defined(XXX_SERVER)
	IniAI(100);
#endif
//	IniNC(99);
//	IniMV(0);

	return 0;
}




void IniAI(int i)
{
	CFile F;
	F.Open(DL_DIR BAC_CFG_LOC_INIVAL, CFile::modeWrite, NULL);
	F.SeekToEnd();

	Ini_PresentValue_Analog(i, &F);	
	Ini_Description(i, &F);
	Ini_DeviceType(i, &F);
	Ini_Reliability(i, &F);
	Ini_Out_Of_Service(i, &F);
//	Ini_Update_Interval(i, &F);
	Ini_Units(i, &F);
	Ini_Min_Present_Value(i, &F);
	Ini_Max_Present_Value(i, &F);
	Ini_Resolution(i, &F);
	Ini_COV_Increment(i, &F);
	Ini_Time_Delay(i, &F);
//	Ini_Notification_Class(i, &F);
	Ini_High_Limit(i, &F);
	Ini_Low_Limit(i, &F);
	Ini_Deadband(i, &F);
	Ini_Limit_Enable(i, &F);
	Ini_Event_Enable(i, &F);
	Ini_Notify_Type(i, &F);

	F.Close();
}

void IniMV(int i)
{
	CFile F;
	F.Open(DL_DIR BAC_CFG_LOC_INIVAL, CFile::modeWrite, NULL);
	F.SeekToEnd();

	Ini_PresentValue_Multi(i, &F);	
	Ini_Description(i, &F);
	Ini_Reliability(i, &F);
	Ini_Out_Of_Service(i, &F);
	Ini_Number_Of_States(i, &F);
	Ini_State_Text(i, &F);
	Ini_Time_Delay(i, &F);
	Ini_Notification_Class(i, &F);
	Ini_Alarm_Values(i, &F);
	Ini_Fault_Values(i, &F);
	Ini_Event_Enable(i, &F);
	Ini_Notify_Type(i, &F);
//	Ini_Profile_Name(i, &F);

	F.Close();
}

void IniNC(int i)
{
	CFile F;
	F.Open(DL_DIR BAC_CFG_LOC_INIVAL, CFile::modeWrite, NULL);
	F.SeekToEnd();

	Ini_Description(i, &F);
//	Ini_Notification_Class(i, &F);


	F.Close();
}
