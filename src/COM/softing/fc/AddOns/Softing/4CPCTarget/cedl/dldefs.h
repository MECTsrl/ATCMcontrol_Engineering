
#ifndef __DLDEFS_H_
#define __DLDEFS_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Defines:    ------------------------------------------------*

// 07.03.2000	13			split from engineering database
// 29.03.2001   14          migration to ATCMControl V2.0
// 26.07.2013	15			migration to ATCMControl
#define CE_DLDatabaseVersion 14

#define CEDL_BASEDB _T("CEDL_Base.mdb")
#define CEDL_DB	_T("CEDL.mdb")

#define CEDL_CONFIGTABLE _T("Configuration")
#define CEDL_DOMAINTABLE _T("Domain")
#define CEDL_LISTFILE	_T("loader.list")

#define CEDL_UPFILE1 _T("upld1_%d.dl")
#define CEDL_DLFILE1 _T("downld1_%d.dl")
#define CEDL_DELFILE _T("del_%d.dl")

#define CEDL_DOMDIR _T("__cedl")
#define CEDL_DOMFILE _T("dl") //domain file header

#define CEDL_VERSIONDOMAIN _T("__.list")
#define CEDL_VISVERSIONDOMAIN _T("__vis.list")

/**
 * @type CEDL_TARGET_TYPE - determines type of target.
 * 4CC or 4CCONSOLE
*/
typedef enum {
	/**Target type 4CC resp. 4CPC*/
	CC,
	/**Target type 4CCONSOLE*/
	CONSOLE
}CEDL_TARGET_TYPE;

typedef enum {
	CEDL_STAT_STRT,
	CEDL_STAT_IDLE,			//after init
	CEDL_STAT_ADAPT_2,
	CEDL_STAT_WAIT_CONNECT,	//after connect
	CEDL_STAT_GETDV,		//connection established - domain dir retrieved
	CEDL_STAT_ADAPT_1,		//domain dir there - adaption cycle 1
    CEDL_STAT_DLCC,         //pending download to control
    CEDL_STAT_UPCC,         //upload control domains
    CEDL_STAT_DELCC,        //delete control domains
	CEDL_STAT_ABORT,		//Abort
    CEDL_STAT_DIR           //Get domain directory
}CEDL_STAT;

typedef enum {
	DOM_UNCHANGED	= 0x0,
	DOM_CHANGED		= 0x01,
	DOM_DELETED		= 0x02,
	DOM_NEW			= 0x04,
	DOM_SELECTED	= 0x08,
    DOM_UPNEW       = 0x10
}CEDL_DOM_FLAG;

typedef enum {
    BUILD_DL_LIST = 0x1,    //build download list request
    DOWN_LD = 0x2,          //download request
    DOWN_LD_ALL = 0x04,     //download all request (with DOWN_LD)
    CHANGED = 0x8,         //changed | selected control domains
    PRJ_NEW = 0x10,         //detected completely different project
    PRJ_CHANGED = 0x20,     //project changed
    CC_ONL = 0x40,         //online change of target
    DEL_STATION = 0x80,     //station is marked for delete
	ATTACHED = 0x1000		//IBT/DL session is attached
}CEDL_SIGNAL;

/**
 * @type CEDL_DFILE_FIELD - determines fields in a domain line of
 * loader.list
*/
typedef enum { //field indexes of CCG's domain file list (loader.list)
	/** invalid field in domain line*/
	DFILE_FIELD_INVALID,
	/** name of domain */
	DFILE_FIELD_DOMNAME,
	/** path of domain relative to gen directory*/
	DFILE_FIELD_DOMPATH
}CEDL_DFILE_FIELD;

typedef enum { //var idents
    ProjectName,
    DloadDate,
    DloadHost,
    DloadGUID,
    Cleared,
    ProjectGUID,
}CEDL_VAR;

typedef struct{
    CEDL_VAR iVarId;
    LPCTSTR pszAddr;
    LPCTSTR pszVarName;
} CEDL_VARDESR;

// Message used by the Downloader to notify about DownloadList ready
#define	CEDL_MSG_DLLIST_READY		(WM_USER + 200)
// Message used by the Downloader to notify about Download ready
#define	CEDL_MSG_DOWNLOAD_READY		(WM_USER + 201)
// Message used by the Downloader to notify about Download error
#define	CEDL_MSG_DOWNLOAD_ERROR		(WM_USER + 202)
// Message used by the variable notification about changes in download state vars
#define	CEDL_MSG_VAR_CHANGED		(WM_USER + 203)

#endif // __DLDEFS_H_
