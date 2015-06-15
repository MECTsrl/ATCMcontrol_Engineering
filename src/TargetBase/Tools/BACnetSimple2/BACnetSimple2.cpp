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

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

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
		// TODO: code your application's behavior here.

		DoIt();
	}

	return nRetCode;
}

/* Special Conditions: 
 * ----------------------------------------------------------------------------
 * * Notification class objects must be placed before all data objects in 
 *   configuration file.
 * * Notification class objects can only be local.
 * * Notification class objects are not mapped into the IEC shared memory.
 * * Names of associated local and remote objects must be the same. (Names
 *   differ in this application only for testing reasons!)
 */

UINT g_ulLocIECObj = 0;
UINT g_ulRemIECObj = 0;
UINT g_ulLocIntObj = 0;
UINT g_ulRemIntObj = 0;

#define DL_DIR			"D:\\4C\\Engineering\\bin\\4CWin\\vmm\\_cust\\"

void DoIt(void)
{
	UINT i;

	XBACnetObj XO;
	CFile F;
	char szName[100];

	/* Clear configuration files
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_STACK, CFile::modeCreate | CFile::modeWrite, NULL);
	F.Close();
	F.Open(DL_DIR BAC_CFG_OBJ_LOC, CFile::modeCreate | CFile::modeWrite, NULL);
	F.Close();
	F.Open(DL_DIR BAC_CFG_OBJ_REM, CFile::modeCreate | CFile::modeWrite, NULL);
	F.Close();
	F.Open(DL_DIR BAC_CFG_LOC_INIVAL, CFile::modeCreate | CFile::modeWrite, NULL);
	F.Close();
	F.Open(DL_DIR BAC_CFG_LOC_INITEXT, CFile::modeCreate | CFile::modeWrite, NULL);
	F.Close();

	/* Notification Class Objects - Local
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_LOC, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_LOC_NC; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		NC_Initialize(i, XO.ulNoOfIniVal, XO.ulOffIniVal);

		XO.eType		= BACnetObjectTypeNotificationClass;

		XO.iDeviceID	= DEVICE_LOCAL;

		XO.iObjectID	= i;
		XO.ulOffset		= -1;

		XO.ulOffIniTxt	= -1;

		sprintf(szName, "NC%02d L (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulLocIntObj++;
	}
	F.Close();


	/* Analog Output Objects - Local
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_LOC, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_LOC_AO; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		AO_Initialize(i, XO.ulNoOfIniVal, XO.ulOffIniVal);

		XO.eType		= BACnetObjectTypeAnalogOutput;

		XO.iDeviceID	= DEVICE_LOCAL;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulOffIniTxt	= -1;

		sprintf(szName, "AO%02d L (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulLocIECObj++;
	}
	F.Close();


	/* Analog Input Objects - Local
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_LOC, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_LOC_AI; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		AI_Initialize(i, XO.ulNoOfIniVal, XO.ulOffIniVal);

		XO.eType		= BACnetObjectTypeAnalogInput;

		XO.iDeviceID	= DEVICE_LOCAL;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulOffIniTxt	= -1;

		sprintf(szName, "AI%02d L (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulLocIECObj++;
	}
	F.Close();


	/* Analog Value Objects - Local
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_LOC, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_LOC_AV; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		AV_Initialize(i, XO.ulNoOfIniVal, XO.ulOffIniVal);

		XO.eType		= BACnetObjectTypeAnalogValue;

		XO.iDeviceID	= DEVICE_LOCAL;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulOffIniTxt	= -1;

		sprintf(szName, "AV%02d L (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulLocIECObj++;
	}
	F.Close();


	/* Binary Output Objects - Local
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_LOC, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_LOC_BO; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		BO_Initialize(i, XO.ulNoOfIniVal, XO.ulOffIniVal);

		XO.eType		= BACnetObjectTypeBinaryOutput;

		XO.iDeviceID	= DEVICE_LOCAL;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulOffIniTxt	= -1;

		sprintf(szName, "BO%02d L (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulLocIECObj++;
	}
	F.Close();


	/* Binary Input Objects - Local
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_LOC, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_LOC_BI; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		BI_Initialize(i, XO.ulNoOfIniVal, XO.ulOffIniVal);

		XO.eType		= BACnetObjectTypeBinaryInput;

		XO.iDeviceID	= DEVICE_LOCAL;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulOffIniTxt	= -1;

		sprintf(szName, "BI%02d L (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulLocIECObj++;
	}
	F.Close();


	/* Binary Value Objects - Local
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_LOC, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_LOC_BV; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		BV_Initialize(i, XO.ulNoOfIniVal, XO.ulOffIniVal);

		XO.eType		= BACnetObjectTypeBinaryValue;

		XO.iDeviceID	= DEVICE_LOCAL;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulOffIniTxt	= -1;

		sprintf(szName, "BV%02d L (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulLocIECObj++;
	}
	F.Close();


	/* Multi-state Output Objects - Local
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_LOC, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_LOC_MO; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		MO_Initialize(i, XO.ulNoOfIniVal, XO.ulOffIniVal);

		XO.eType		= BACnetObjectTypeMultistateOutput;

		XO.iDeviceID	= DEVICE_LOCAL;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulOffIniTxt	= -1;

		sprintf(szName, "MO%02d L (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulLocIECObj++;
	}
	F.Close();


	/* Multi-state Input Objects - Local
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_LOC, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_LOC_MI; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		MI_Initialize(i, XO.ulNoOfIniVal, XO.ulOffIniVal);

		XO.eType		= BACnetObjectTypeMultistateInput;

		XO.iDeviceID	= DEVICE_LOCAL;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulOffIniTxt	= -1;

		sprintf(szName, "MI%02d L (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulLocIECObj++;
	}
	F.Close();


	/* Multi-state Value Objects - Local
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_LOC, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_LOC_MV; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		MV_Initialize(i, XO.ulNoOfIniVal, XO.ulOffIniVal);

		XO.eType		= BACnetObjectTypeMultistateValue;

		XO.iDeviceID	= DEVICE_LOCAL;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulOffIniTxt	= -1;

		sprintf(szName, "MV%02d L (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulLocIECObj++;
	}
	F.Close();

	
	/* Trend Log Objects - Local
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_LOC, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_LOC_TL; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		TL_Initialize(i, XO.ulNoOfIniVal, XO.ulOffIniVal);

		XO.eType		= BACnetObjectTypeTrendLog;

		XO.iDeviceID	= DEVICE_LOCAL;

		XO.iObjectID	= i;
		XO.ulOffset		= -1;

		XO.ulOffIniTxt	= -1;

		sprintf(szName, "TL%02d L (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulLocIntObj++;
	}
	F.Close();


	/* Schedule Objects - Local
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_LOC, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_LOC_SC; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		SC_Initialize(i, XO.ulNoOfIniVal, XO.ulOffIniVal);

		XO.eType		= BACnetObjectTypeSchedule;

		XO.iDeviceID	= DEVICE_LOCAL;

		XO.iObjectID	= i;
		XO.ulOffset		= -1;

		XO.ulOffIniTxt	= -1;

		sprintf(szName, "SC%02d L (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulLocIntObj++;
	}
	F.Close();
	
	/* ------------------------------------------------------------------------ */
	/* ------------------------------------------------------------------------ */

	/* Notification Class - Remote
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_REM, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_REM_NC; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		XO.eType		= BACnetObjectTypeNotificationClass;

		XO.iDeviceID	= DEVICE_REMOTE;

		XO.iObjectID	= i;
		XO.ulOffset		= -1;

		XO.ulNoOfIniVal	= 0;
		XO.ulOffIniTxt	= -1;
		XO.ulOffIniTxt	= -1;

		sprintf(szName, "NC%02d R (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulRemIntObj++;
	}
	F.Close();


	/* Analog Output Objects - Remote
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_REM, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_REM_AO; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		XO.eType		= BACnetObjectTypeAnalogOutput;

		XO.iDeviceID	= DEVICE_REMOTE;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulNoOfIniVal	= 0;
		XO.ulOffIniTxt	= -1;
		XO.ulOffIniTxt	= -1;

		sprintf(szName, "AO%02d R (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulRemIECObj++;
	}
	F.Close();


	/* Analog Input Objects - Remote
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_REM, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_REM_AI; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		XO.eType		= BACnetObjectTypeAnalogInput;

		XO.iDeviceID	= DEVICE_REMOTE;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulNoOfIniVal	= 0;
		XO.ulOffIniTxt	= -1;
		XO.ulOffIniTxt	= -1;

		sprintf(szName, "AI%02d R (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulRemIECObj++;
	}
	F.Close();


	/* Analog Value Objects - Remote
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_REM, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_REM_AV; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		XO.eType		= BACnetObjectTypeAnalogValue;

		XO.iDeviceID	= DEVICE_REMOTE;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulNoOfIniVal	= 0;
		XO.ulOffIniTxt	= -1;
		XO.ulOffIniTxt	= -1;

		sprintf(szName, "AV%02d R (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulRemIECObj++;
	}
	F.Close();


	/* Binary Output Objects - Remote
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_REM, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_REM_BO; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		XO.eType		= BACnetObjectTypeBinaryOutput;

		XO.iDeviceID	= DEVICE_REMOTE;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulNoOfIniVal	= 0;
		XO.ulOffIniTxt	= -1;
		XO.ulOffIniTxt	= -1;

		sprintf(szName, "BO%02d R (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulRemIECObj++;
	}
	F.Close();


	/* Binary Input Objects - Remote
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_REM, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_REM_BI; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		XO.eType		= BACnetObjectTypeBinaryInput;

		XO.iDeviceID	= DEVICE_REMOTE;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulNoOfIniVal	= 0;
		XO.ulOffIniTxt	= -1;
		XO.ulOffIniTxt	= -1;

		sprintf(szName, "BI%02d R (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulRemIECObj++;
	}
	F.Close();


	/* Binary Value Objects - Remote
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_REM, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_REM_BV; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		XO.eType		= BACnetObjectTypeBinaryValue;

		XO.iDeviceID	= DEVICE_REMOTE;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulNoOfIniVal	= 0;
		XO.ulOffIniTxt	= -1;
		XO.ulOffIniTxt	= -1;

		sprintf(szName, "BV%02d R (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulRemIECObj++;
	}
	F.Close();


	/* Multi-state Output Objects - Remote
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_REM, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_REM_MO; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		XO.eType		= BACnetObjectTypeMultistateOutput;

		XO.iDeviceID	= DEVICE_REMOTE;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulNoOfIniVal	= 0;
		XO.ulOffIniTxt	= -1;
		XO.ulOffIniTxt	= -1;

		sprintf(szName, "MO%02d R (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulRemIECObj++;
	}
	F.Close();


	/* Multi-state Input Objects - Remote
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_REM, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_REM_MI; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		XO.eType		= BACnetObjectTypeMultistateInput;

		XO.iDeviceID	= DEVICE_REMOTE;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulNoOfIniVal	= 0;
		XO.ulOffIniTxt	= -1;
		XO.ulOffIniTxt	= -1;

		sprintf(szName, "MI%02d R (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulRemIECObj++;
	}
	F.Close();


	/* Multi-state Value Objects - Remote
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_REM, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_REM_MV; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		XO.eType		= BACnetObjectTypeMultistateValue;

		XO.iDeviceID	= DEVICE_REMOTE;

		XO.iObjectID	= i;
		XO.ulOffset		= (g_ulRemIECObj + g_ulLocIECObj) * 100;

		XO.ulNoOfIniVal	= 0;
		XO.ulOffIniTxt	= -1;
		XO.ulOffIniTxt	= -1;

		sprintf(szName, "MV%02d R (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulRemIECObj++;
	}
	F.Close();


	/* Trend Log Objects - Remote
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_REM, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_REM_TL; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		TL_Initialize(i, XO.ulNoOfIniVal, XO.ulOffIniVal);

		XO.eType		= BACnetObjectTypeTrendLog;

		XO.iDeviceID	= DEVICE_REMOTE;

		XO.iObjectID	= i;
		XO.ulOffset		= -1;

		XO.ulOffIniTxt	= -1;

		sprintf(szName, "TL%02d R (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulRemIntObj++;
	}
	F.Close();


	/* Schedule Objects - Remote
	 * ------------------------------------------------------------------------
	 */
	F.Open(DL_DIR BAC_CFG_OBJ_REM, CFile::modeWrite, NULL);
	F.SeekToEnd();

	for (i = 1; i <= NO_REM_SC; i++)
	{
		memset(&XO, 0x00, sizeof(XO));

		SC_Initialize(i, XO.ulNoOfIniVal, XO.ulOffIniVal);

		XO.eType		= BACnetObjectTypeSchedule;

		XO.iDeviceID	= DEVICE_REMOTE;

		XO.iObjectID	= i;
		XO.ulOffset		= -1;

		XO.ulOffIniTxt	= -1;

		sprintf(szName, "SC%02d R (Offs:%d)", XO.iObjectID, XO.ulOffset);

		XO.ulNameLen = strlen(szName);

		F.Write(&XO, sizeof(XO));
		F.Write(szName, XO.ulNameLen + 1);
		
		g_ulRemIntObj++;
	}
	F.Close();


	/* Device / Stack
	 * ------------------------------------------------------------------------
	 */
	XBACnetStack BS;

	memset(&BS, 0x00, sizeof(XBACnetStack));

	BS.iDeviceID	= DEVICE_LOCAL;
	
	BS.ulNoLocObj	= g_ulLocIECObj + g_ulLocIntObj;
	BS.ulNoRemObj	= g_ulRemIECObj + g_ulRemIntObj;

	BS.iDefPrioLoc	= 0;
	BS.iDefPrioRem	= 4;

	BS.iCOVLifeTime		= 3600;
	BS.byConfirmedCOV	= 1;

	F.Open(DL_DIR BAC_CFG_STACK, CFile::modeWrite, NULL);
	F.Write(&BS, sizeof(XBACnetStack));
	F.Close();

}


/* NC_Initialize
 * ----------------------------------------------------------------------------
 */
void NC_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal)
{
	CFile F;

	F.Open(DL_DIR BAC_CFG_LOC_INIVAL, CFile::modeWrite, NULL);
	
	ulNoOfIniVal	= 0;
	ulOffIniVal		= F.SeekToEnd();

	Ini_Description(ulObjID, &F, (IEC_CHAR *)"NC");		ulNoOfIniVal++;
	Ini_Priority(ulObjID, &F);							ulNoOfIniVal++;
	Ini_Ack_Required(ulObjID, &F);						ulNoOfIniVal++;

	F.Close();
}

/* AO_Initialize
 * ----------------------------------------------------------------------------
 */
void AO_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal)
{
	CFile F;

	F.Open(DL_DIR BAC_CFG_LOC_INIVAL, CFile::modeWrite, NULL);
	
	ulNoOfIniVal	= 0;
	ulOffIniVal		= F.SeekToEnd();

	Ini_Present_Value_A(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Description(ulObjID, &F, (IEC_CHAR *)"AO");		ulNoOfIniVal++;
	Ini_Device_Type(ulObjID, &F, (IEC_CHAR *)"AO");		ulNoOfIniVal++;
	Ini_Reliability(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Out_Of_Service(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Units(ulObjID, &F);								ulNoOfIniVal++;
	Ini_Min_Pres_Value(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Max_Pres_Value(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Resolution(ulObjID, &F);						ulNoOfIniVal++;
	Ini_COV_Increment(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Time_Delay(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notification_Class(ulObjID, &F);				ulNoOfIniVal++;
	Ini_High_Limit(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Low_Limit(ulObjID, &F);							ulNoOfIniVal++;
	Ini_Deadband(ulObjID, &F);							ulNoOfIniVal++;
	Ini_Limit_Enable(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Event_Enable(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notify_Type(ulObjID, &F);						ulNoOfIniVal++;

	F.Close();
}

/* AI_Initialize
 * ----------------------------------------------------------------------------
 */
void AI_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal)
{
	CFile F;

	F.Open(DL_DIR BAC_CFG_LOC_INIVAL, CFile::modeWrite, NULL);
	
	ulNoOfIniVal	= 0;
	ulOffIniVal		= F.SeekToEnd();

	Ini_Present_Value_A(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Description(ulObjID, &F, (IEC_CHAR *)"AI");		ulNoOfIniVal++;
	Ini_Device_Type(ulObjID, &F, (IEC_CHAR *)"AI");		ulNoOfIniVal++;
	Ini_Reliability(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Out_Of_Service(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Update_Interval(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Units(ulObjID, &F);								ulNoOfIniVal++;
	Ini_Min_Pres_Value(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Max_Pres_Value(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Resolution(ulObjID, &F);						ulNoOfIniVal++;
	Ini_COV_Increment(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Time_Delay(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notification_Class(ulObjID, &F);				ulNoOfIniVal++;
	Ini_High_Limit(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Low_Limit(ulObjID, &F);							ulNoOfIniVal++;
	Ini_Deadband(ulObjID, &F);							ulNoOfIniVal++;
	Ini_Limit_Enable(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Event_Enable(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notify_Type(ulObjID, &F);						ulNoOfIniVal++;

	F.Close();
}

/* AV_Initialize
 * ----------------------------------------------------------------------------
 */
void AV_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal)
{
	CFile F;

	F.Open(DL_DIR BAC_CFG_LOC_INIVAL, CFile::modeWrite, NULL);
	
	ulNoOfIniVal	= 0;
	ulOffIniVal		= F.SeekToEnd();

	Ini_Present_Value_A(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Description(ulObjID, &F, (IEC_CHAR *)"AV");		ulNoOfIniVal++;
	Ini_Reliability(ulObjID, &F);						ulNoOfIniVal++;		
	Ini_Out_Of_Service(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Units(ulObjID, &F);								ulNoOfIniVal++;
	Ini_COV_Increment(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Time_Delay(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notification_Class(ulObjID, &F);				ulNoOfIniVal++;
	Ini_High_Limit(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Low_Limit(ulObjID, &F);							ulNoOfIniVal++;
	Ini_Deadband(ulObjID, &F);							ulNoOfIniVal++;
	Ini_Limit_Enable(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Event_Enable(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notify_Type(ulObjID, &F);						ulNoOfIniVal++;

	F.Close();
}

/* BO_Initialize
 * ----------------------------------------------------------------------------
 */
void BO_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal)
{
	CFile F;

	F.Open(DL_DIR BAC_CFG_LOC_INIVAL, CFile::modeWrite, NULL);
	
	ulNoOfIniVal	= 0;
	ulOffIniVal		= F.SeekToEnd();

	Ini_Present_Value_B(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Description(ulObjID, &F, (IEC_CHAR *)"BO");		ulNoOfIniVal++;
	Ini_Device_Type(ulObjID, &F, (IEC_CHAR *)"BO");		ulNoOfIniVal++;
	Ini_Reliability(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Out_Of_Service(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Polarity(ulObjID, &F);							ulNoOfIniVal++;
	Ini_Inactive_Text(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Active_Text(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Minimum_Off_Time(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Minimum_On_Time(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Time_Delay(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notification_Class(ulObjID, &F);				ulNoOfIniVal++;
	Ini_Feedback_Value_B(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Event_Enable(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notify_Type(ulObjID, &F);						ulNoOfIniVal++;

	F.Close();
}

/* BI_Initialize
 * ----------------------------------------------------------------------------
 */
void BI_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal)
{
	CFile F;

	F.Open(DL_DIR BAC_CFG_LOC_INIVAL, CFile::modeWrite, NULL);
	
	ulNoOfIniVal	= 0;
	ulOffIniVal		= F.SeekToEnd();

	Ini_Present_Value_B(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Description(ulObjID, &F, (IEC_CHAR *)"BI");		ulNoOfIniVal++;
	Ini_Device_Type(ulObjID, &F, (IEC_CHAR *)"BI");		ulNoOfIniVal++;
	Ini_Reliability(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Out_Of_Service(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Polarity(ulObjID, &F);							ulNoOfIniVal++;
	Ini_Inactive_Text(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Active_Text(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Time_Delay(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notification_Class(ulObjID, &F);				ulNoOfIniVal++;
	Ini_Alarm_Value(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Event_Enable(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notify_Type(ulObjID, &F);						ulNoOfIniVal++;

	F.Close();
}

/* BV_Initialize
 * ----------------------------------------------------------------------------
 */
void BV_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal)
{
	CFile F;

	F.Open(DL_DIR BAC_CFG_LOC_INIVAL, CFile::modeWrite, NULL);
	
	ulNoOfIniVal	= 0;
	ulOffIniVal		= F.SeekToEnd();

	Ini_Present_Value_B(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Description(ulObjID, &F, (IEC_CHAR *)"BV");		ulNoOfIniVal++;
	Ini_Reliability(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Out_Of_Service(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Inactive_Text(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Active_Text(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Minimum_Off_Time(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Minimum_On_Time(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Time_Delay(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notification_Class(ulObjID, &F);				ulNoOfIniVal++;
	Ini_Alarm_Value(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Event_Enable(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notify_Type(ulObjID, &F);						ulNoOfIniVal++;

	F.Close();
}

/* MO_Initialize
 * ----------------------------------------------------------------------------
 */
void MO_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal)
{
	CFile F;

	F.Open(DL_DIR BAC_CFG_LOC_INIVAL, CFile::modeWrite, NULL);
	
	ulNoOfIniVal	= 0;
	ulOffIniVal		= F.SeekToEnd();

	Ini_Number_Of_States(ulObjID, &F);					ulNoOfIniVal++;
	
	Ini_Present_Value_M(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Description(ulObjID, &F, (IEC_CHAR *)"MO");		ulNoOfIniVal++;
	Ini_Device_Type(ulObjID, &F, (IEC_CHAR *)"MO");		ulNoOfIniVal++;
	Ini_Reliability(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Out_Of_Service(ulObjID, &F);					ulNoOfIniVal++;
	Ini_State_Text(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Time_Delay(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notification_Class(ulObjID, &F);				ulNoOfIniVal++;
	Ini_Feedback_Value_M(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Event_Enable(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notify_Type(ulObjID, &F);						ulNoOfIniVal++;

	F.Close();
}

/* MI_Initialize
 * ----------------------------------------------------------------------------
 */
void MI_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal)
{
	CFile F;

	F.Open(DL_DIR BAC_CFG_LOC_INIVAL, CFile::modeWrite, NULL);
	
	ulNoOfIniVal	= 0;
	ulOffIniVal		= F.SeekToEnd();

	Ini_Number_Of_States(ulObjID, &F);					ulNoOfIniVal++;

	Ini_Present_Value_M(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Description(ulObjID, &F, (IEC_CHAR *)"MI");		ulNoOfIniVal++;
	Ini_Device_Type(ulObjID, &F, (IEC_CHAR *)"MI");		ulNoOfIniVal++;
	Ini_Reliability(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Out_Of_Service(ulObjID, &F);					ulNoOfIniVal++;
	Ini_State_Text(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Time_Delay(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notification_Class(ulObjID, &F);				ulNoOfIniVal++;
	Ini_Alarm_Values(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Fault_Values(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Event_Enable(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notify_Type(ulObjID, &F);						ulNoOfIniVal++;

	F.Close();
}

/* MV_Initialize
 * ----------------------------------------------------------------------------
 */
void MV_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal)
{
	CFile F;

	F.Open(DL_DIR BAC_CFG_LOC_INIVAL, CFile::modeWrite, NULL);
	
	ulNoOfIniVal	= 0;
	ulOffIniVal		= F.SeekToEnd();

	Ini_Number_Of_States(ulObjID, &F);					ulNoOfIniVal++;

	Ini_Present_Value_M(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Description(ulObjID, &F, (IEC_CHAR *)"MV");		ulNoOfIniVal++;
	Ini_Reliability(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Out_Of_Service(ulObjID, &F);					ulNoOfIniVal++;
	Ini_State_Text(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Time_Delay(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notification_Class(ulObjID, &F);				ulNoOfIniVal++;
	Ini_Alarm_Values(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Fault_Values(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Event_Enable(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notify_Type(ulObjID, &F);						ulNoOfIniVal++;

	F.Close();
}

/* TL_Initialize
 * ----------------------------------------------------------------------------
 */
void TL_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal)
{
	CFile F;

	F.Open(DL_DIR BAC_CFG_LOC_INIVAL, CFile::modeWrite, NULL);
	
	ulNoOfIniVal	= 0;
	ulOffIniVal		= F.SeekToEnd();

	Ini_Buffer_Size(ulObjID, &F);						ulNoOfIniVal++;

	Ini_Description(ulObjID, &F, (IEC_CHAR *)"TL");		ulNoOfIniVal++;
	Ini_Log_Enable(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Start_Time(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Stop_Time(ulObjID, &F);							ulNoOfIniVal++;
	Ini_Log_DevObjPropRev(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Log_Interval(ulObjID, &F);						ulNoOfIniVal++;
//	Ini_COV_Resubscription_Interval(ulObjID, &F);		ulNoOfIniVal++;
//	Ini_Client_COV_Increment(ulObjID, &F);				ulNoOfIniVal++;
	Ini_Stop_When_Full(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Notification_Treshold(ulObjID, &F);				ulNoOfIniVal++;
	Ini_Notification_Class(ulObjID, &F);				ulNoOfIniVal++;
	Ini_Event_Enable(ulObjID, &F);						ulNoOfIniVal++;
	Ini_Notify_Type(ulObjID, &F);						ulNoOfIniVal++;

	F.Close();
}

/* SC_Initialize
 * ----------------------------------------------------------------------------
 */
void SC_Initialize(UINT ulObjID, unsigned long &ulNoOfIniVal, unsigned long &ulOffIniVal)
{
	CFile F;

	F.Open(DL_DIR BAC_CFG_LOC_INIVAL, CFile::modeWrite, NULL);
	
	ulNoOfIniVal	= 0;
	ulOffIniVal		= F.SeekToEnd();

	Ini_Description(ulObjID, &F, (IEC_CHAR *)"SC");		ulNoOfIniVal++;
	Ini_Effective_Period(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Weekly_Schedule(ulObjID, &F);					ulNoOfIniVal++;
	Ini_Exception_Schedule(ulObjID, &F);				ulNoOfIniVal++;
	Ini_Schedule_Default(ulObjID, &F);					ulNoOfIniVal++;

//  Enable in order to create an reference!
//	Ini_List_Of_DevObjPropRef(ulObjID, &F);				ulNoOfIniVal++;

	Ini_Priority_For_Writing(ulObjID, &F);				ulNoOfIniVal++;
//	Ini_Reliability(ulObjID, &F);						ulNoOfIniVal++;
//	Ini_Out_Of_Service(ulObjID, &F);					ulNoOfIniVal++;

	F.Close();
}

