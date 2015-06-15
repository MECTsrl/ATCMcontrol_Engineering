/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/CommPara.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: CommPara.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/CommPara.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibComm
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _COMMPARA_H_
#define _COMMPARA_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/* Progress callback function
 * ----------------------------------------------------------------------------
 */

typedef bool (* PROGRESS_CALLBACK) (int iCurrent, int iMax);


/* Common communication parameters
 * ----------------------------------------------------------------------------
 */
typedef struct
{
	BOOL	bBigEndian;					/* Target is a big endian target	*/
	UINT	uBlockLen;					/* Max. communication data			*/
	
	DWORD	dwFirmware;					/* Current firmware version			*/
	
	DWORD	dwRecvTimeOut;				/* Receive time out					*/
	DWORD	dwSendTimeOut;				/* Send time out					*/

	/* Special time out values
	 */
	DWORD	dwDownEndRecvTO;			/* Downl. end - Apply configuration	*/
	DWORD	dwDownBegRecvTO;			/* Downl. begin - clear resource	*/
	DWORD	dwOCCommitRecvTO;			/* Commit Online Change				*/
	DWORD	dwClearResRecvTO;			/* Clear Resource					*/
	DWORD	dwColdWarmTO;				/* Colstart / Warmstart time out	*/

} SCommPara;


/* Communication error values
 * ----------------------------------------------------------------------------
 */
enum ECommError
{
	ceOK,
	ceError,
	ceFatalInternalError,
	ceTimeOut,

	ceNotOpen,
	ceNotConnected,
	ceAlreadyOpen,
	
	ceInvalidParameters,
	ceConfigure,

	ceErrorSendingData,
	ceErrorReadingData,

	ceAck,
	ceNAck,

	ceCRC
};


/* Message format strings
 * ----------------------------------------------------------------------------
 */
#define FERR_DEF		_T("[CMD]: ERROR V%04X: %s\n")
#define FERR_CMD		_T("[CMD]: ERROR V%04X: %-16s {%s.}\n")
#define FOK_CMD			_T("[CMD]: <done>       %s\n")
#define FCT_CMD			_T("[CMD]: <continued>  %s\n")

#define OPC_ERR_DEF		_T("[OPC]: ERROR V%04X: %s\n")
#define OPC_MSG			_T("[OPC]: %s\n")


/* OPC server error messages
 * ----------------------------------------------------------------------------
 */
#define	OPC_ERR_LOAD_ONLCOMM				100
#define	OPC_ERR_INIT_ONLCOMM				101
#define	OPC_ERR_CONS_ONLCOMM				102
#define OPC_ERR_OPENCOMM					103
#define OPC_ERR_CONNECT						104


/* Message strings
 * ----------------------------------------------------------------------------
 */
#define CMD_TEXT {				  				\
						_T("State"),			\
	/* max. 8 chars */	_T("NYI"),				\
						_T("Login"),			\
						_T("Logout"),			\
						_T("WrmStart"), 		\
						_T("CldStart"), 		\
						_T("StartAll"), 		\
						_T("StopAll"),			\
						_T("StartTsk"), 		\
						_T("StopTask"), 		\
						_T("OpenDbg"),			\
						_T("CloseDbg"), 		\
						_T("SetBP"),			\
						_T("ClearBP"),			\
						_T("ClearAll"), 		\
						_T("SinglStp"), 		\
						_T("Continue"), 		\
						_T("GetValue"), 		\
						_T("SetValue"), 		\
						_T("DwnConf"),			\
						_T("DwnInit"),			\
						_T("DwnCode"),			\
						_T("DwnCust"),			\
						_T("DwnFin"),			\
						_T("StartRes"), 		\
						_T("StopRes"),			\
						_T("DwnEnd"),			\
						_T("DwnBegin"), 		\
						_T("InitClr"),			\
						_T("ClrFlash"), 		\
						_T("DBIGChil"), 		\
						_T("DBIGAddr"), 		\
						_T("DwnDebug"), 		\
						_T("GetTskNr"), 		\
						_T("GetPrjVr"), 		\
						_T("GetPrjIn"), 		\
						_T("LoadPrj"),			\
						_T("SavePrj"),			\
						_T("Custom"),			\
						_T("LoadFile"), 		\
						_T("SaveFile"), 		\
						_T("DirCont"),			\
						_T("OcBegin"),			\
						_T("OcCode"),			\
						_T("OcDebug"),			\
						_T("OcCustom"), 		\
						_T("OcInit"),			\
						_T("OcCommit"), 		\
						_T("OcEnd"),			\
						_T("OcConfig"), 		\
						_T("OcFinish"), 		\
						_T("Flash"),			\
						_T("DelFile"),			\
						_T("GetConf"),			\
						_T("RetWrite"), 		\
						_T("RetCycle"), 		\
						_T("FWDown"),			\
						_T("FWExec"),			\
						_T("FWResult"), 		\
						_T("RetUpld"),			\
						_T("RetDown"),			\
						_T("IECUpld"),			\
						_T("IECDown"),			\
						_T("GetIKey"),			\
						_T("SetLKey"),			\
						_T("GetSNo"),			\
						_T("GetFeat"),			\
						_T("GetType"),			\
						_T("GetVers"),			\
/* max. 8 chars */		_T("SetLicEx"),			\
						_T("ConfigIO"),			\
						_T("ClearCtl")			\
					}

#define CMD_TEXT_LARGE {							\
					_T("Get State"),				\
					_T("NYI"),						\
					_T("Login"),					\
					_T("Logout"),					\
					_T("Warmstart"),				\
					_T("Coldstart"),				\
					_T("Start All Tasks"),			\
					_T("Stop All Tasks"),			\
					_T("Start Task"),				\
					_T("Stop Task"),				\
					_T("Debug Mode"),			    \
					_T("Normal Mode"),				\
					_T("Set Breakpoint"),			\
					_T("Clear Breakpoint"),			\
					_T("Clear All Breakpoints"),	\
					_T("Single Step"),				\
					_T("Continue"),					\
					_T("Get Value"),				\
					_T("Set Value"),				\
					_T("Download Configuration"),	\
					_T("Download Initial Values"),	\
					_T("Download Code"),			\
					_T("Download IO Configuration"),\
					_T("Download Version Info"),	\
					_T("Start Resource"),			\
					_T("Stop Resource"),			\
					_T("Start Tasks"),				\
					_T("Initialize Download"),		\
					_T("Initialize/Clear"),	 		\
					_T("Clear Flash"),				\
					_T("Get Children"),				\
					_T("Get Address"),				\
					_T("Download Debug Information"),\
					_T("Get Task Number"),			\
					_T("Get Project Version"),	    \
					_T("Get Project Information"),	\
					_T("Load Project"),				\
					_T("Save Project"),				\
					_T("Custom"),					\
                    _T("Load File"),	    		\
					_T("Save File"),		    	\
					_T("Directory Contents"),		\
					_T("Online Change Initialize"),	\
					_T("Download Code"),			\
					_T("Download Debug Information"),	\
					_T("Download IO Config"),		\
					_T("Download Initial Values"),	\
					_T("Online Change Commit"),		\
					_T("Online Change End"),		\
					_T("Download Configuration"),	\
					_T("Online Change Ready"),		\
					_T("Flash"),					\
					_T("Delete File"),				\
					_T("Get Configuration"),		\
					_T("RetWrite"), 				\
					_T("RetCycle"), 				\
					_T("FWDown"),					\
					_T("FWExec"),					\
					_T("FWResult"), 				\
					_T("RetUpld"),					\
					_T("RetDown"),					\
					_T("IECUpld"),					\
					_T("IECDown"),					\
					_T("GetIKey"),					\
					_T("SetLKey"),					\
					_T("GetSNo"),					\
					_T("GetFeat"),					\
					_T("GetType"),					\
					_T("GetVers"),					\
					_T("SetLicEx"),					\
					_T("Configure IO Layer"),		\
					_T("Clear Control")				\
				}

#define CMD_TEXT_SYMBOL {							\
					_T("CMD_GET_STATE"),			\
					_T("CMD_NOT_IMPLEMENTED"),		\
					_T("CMD_LOGIN"),				\
					_T("CMD_LOGOUT"),				\
					_T("CMD_WARM_START"),			\
					_T("CMD_COLD_START"),			\
					_T("CMD_START_ALL_TASKS"),		\
					_T("CMD_STOP_ALL_TASKS"),		\
					_T("CMD_START_TASK"),			\
					_T("CMD_STOP_TASK"),			\
					_T("CMD_OPEN_DEBUG_SESSION"),	\
					_T("CMD_CLOSE_DEBUG_SESSION"),	\
					_T("CMD_SET_BREAKPOINT"),		\
					_T("CMD_CLEAR_BREAKPOINT"),		\
					_T("CMD_CLEAR_ALL_BREAKPOINTS"),\
					_T("CMD_SINGLE_STEP"),			\
					_T("CMD_CONTINUE"),				\
					_T("CMD_GET_VALUE"),			\
					_T("CMD_SET_VALUE"),			\
					_T("CMD_DOWNLOAD_CONFIG"),		\
					_T("CMD_DOWNLOAD_INITIAL"),		\
					_T("CMD_DOWNLOAD_CODE"),		\
					_T("CMD_DOWNLOAD_CUSTOM"),		\
					_T("CMD_DOWNLOAD_FINISH"),		\
					_T("CMD_START_RESOURCE"),		\
					_T("CMD_STOP_RESOURCE"),		\
					_T("CMD_DOWNLOAD_END"),			\
					_T("CMD_DOWNLOAD_BEGIN"),		\
					_T("CMD_INITIALIZE_CLEAR"),		\
					_T("CMD_CLEAR_FLASH"),			\
					_T("CMD_DBI_GET_CHILDREN"),		\
					_T("CMD_DBI_GET_ADDRESS"),		\
					_T("CMD_DOWNLOAD_DEBUG"),		\
					_T("CMD_DBI_GET_TASKNR"),		\
					_T("CMD_GET_PROJ_VERSION"),		\
					_T("CMD_GET_PROJ_INFO"),		\
					_T("CMD_LOAD_PROJECT"),			\
					_T("CMD_SAVE_PROJECT"),			\
					_T("CMD_CUSTOM"),				\
                    _T("CMD_LOAD_FILE"),			\
					_T("CMD_SAVE_FILE"),			\
					_T("CMD_DIR_CONTENT"),			\
					_T("CMD_OC_BEGIN"),				\
					_T("CMD_OC_CODE"),				\
					_T("CMD_OC_DEBUG"),				\
					_T("CMD_OC_CUSTOM"),			\
					_T("CMD_OC_INITIAL"),			\
					_T("CMD_OC_COMMIT"),			\
					_T("CMD_OC_END"),				\
					_T("CMD_OC_CONFIG"),			\
					_T("CMD_OC_FINISH"),			\
					_T("CMD_FLASH"),				\
					_T("CMD_DEL_FILE"),				\
					_T("CMD_GET_CONFIG"),			\
					_T("CMD_RET_WRITE"),			\
					_T("CMD_RET_CYCLE"),			\
					_T("CMD_FW_DOWNLOAD"),			\
					_T("CMD_FW_EXECUTE"),			\
					_T("CMD_FW_RESULT"),			\
					_T("CMD_RET_UPLOAD"),			\
					_T("CMD_RET_DOWNLOAD"),			\
					_T("CMD_IEC_UPLOAD"),			\
					_T("CMD_IEC_DOWNLOAD"),			\
					_T("CMD_GET_INSTKEY"),			\
					_T("CMD_SET_LICKEY"),			\
					_T("CMD_GET_SERIALNO"),			\
					_T("CMD_GET_FEATURE"),			\
					_T("CMD_GET_TYPE"),				\
					_T("CMD_GET_VERSION"),			\
					_T("CMD_SET_LICEX"),			\
					_T("CMD_DOWNLOAD_IOL"),			\
					_T("CMD_DOWNLOAD_CLEAR")		\
					}

#endif

/* ---------------------------------------------------------------------------- */
