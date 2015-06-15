#ifndef __CESYSDEF_H_
#define __CESYSDEF_H_



/////////////////////////////////////////////////////////////////////////////
// this files contains only defines used by all components of the 4CE-Project
// the file will be included in *.IDL files too!!!


// for version ATCMControl V20:
// ==> all defines that will probably no longer be needed are commented
// like  '// ***V20:*** //'
// if they are really no longer needed delete these lines, else uncomment them
// should be tested if there are more unnecessary lines



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// global defines
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


/*  format version - denotes the short product version string (PROD_SHORT_VERSION_STR)
    where a change of the file format forces an update/migration tool.
    Proceedings: The version string read from the project file will be
    compared against this definition to determine whether an update/
    migration tool is mandatory.

    Note the changes of the file format here:
    old PROD_SHORT_VERSION_STR  -->     new PROD_SHORT_VERSION_STR
    empty                       -->     V1.2
*/
#define CE_FORMAT_VERSION           _T("V2.0")
#define CE_FORMAT_VERSION_CONVERT   _T("V1.2")
#define CE_CONVERTER                _T("engineering\\bin\\4c13to20.exe")


// name of 4CCG mutex, used for checking if compiler is running
#define _4CCG_MUTEX  _T("4ccg_mutex")



#define CE_NUMUSRTXT     3	//defines the number of user texts in each 4CObject
#define CE_NUMCONFIGS	30	//number of configurations

// ***V20:*** //#define CEPARSE_POS_INVALID -1 //invalid parse object position

// width of a bitmap  used in Navigator and PrjListView
#define BITMAP_WIDTH    20
#define BITMAP_BACKGROUND_COL   RGB(255,0,255)

// defines the maximal number of Targetss
#define CE_MAXTARGET	30

// defines the refresh cycle used by the WatchView [ms]
#define CE_WATCH_CYCLE	500

// Facility codes for CE
#define CE_FACILITY            0x280
#define CE_CEMAIN_FACILITY     (CE_FACILITY + 0x00)

//address separator for subscription strings
#define CE_SEP_SUBSCR   _T('.')

// defines the minimum connection timeout
#define CE_MIN_CONN_TIMEOUT	15
// define the minimum time to enable an abort of a other connection
#define CE_MIN_CONN_ABORTTIME 10

//predefined variables and their states
#define CE_STATE_RESOURCE _T("__system.resourcestate")    //var to subscribe
#define CE_STATE_TASK     _T("__state")    //var to subscribe
#define CE_CLEARED_RESOURCE _T ("__system.dload.cleared")

#define CE_STATE_ERROR (-1)        //error state
#define CE_STATE_CREATE 0          //oncreation
#define CE_STATE_PAUSED 1          //paused
#define CE_STATE_EXEC   2          //execute
#define CE_STATE_BPREACHED 3       //Breakpoint reached only for task
#define CE_STATE_EXEC_CLEARED 4         //executing and cleared
#define CE_STATE_PAUSED_CLEARED 5  //paused and cleared

//var type defines
#define CVAR_TYPE_LOCAL         0x01 
#define CVAR_TYPE_GLOBAL        0x02 
#define CVAR_TYPE_IN            0x04 
#define CVAR_TYPE_OUT           0x08 
#define CVAR_TYPE_INOUT         (CVAR_TYPE_IN | CVAR_TYPE_OUT)
#define CVAR_TYPE_RETAIN        0x10 
#define CVAR_TYPE_NONRETAIN     0x20 
#define CVAR_TYPE_CONST         0x40 
#define CVAR_TYPE_EXTERNAL		0x80
#define CVAR_TYPE_INEXT         0x100
#define CVAR_TYPE_TEMP          0x200

// OPC qualities
#define OPC_QUALITY_MASK            0xC0
#define OPC_STATUS_MASK             0xFC
#define OPC_LIMIT_MASK              0x03
#define OPC_QUALITY_BAD             0x00
#define OPC_QUALITY_UNCERTAIN       0x40
#define OPC_QUALITY_GOOD            0xC0
#define OPC_QUALITY_CONFIG_ERROR    0x04
#define OPC_QUALITY_NOT_CONNECTED   0x08
#define OPC_QUALITY_DEVICE_FAILURE  0x0c
#define OPC_QUALITY_SENSOR_FAILURE  0x10
#define OPC_QUALITY_LAST_KNOWN      0x14
#define OPC_QUALITY_COMM_FAILURE    0x18
#define OPC_QUALITY_OUT_OF_SERVICE  0x1C
#define OPC_QUALITY_LAST_USABLE     0x44
#define OPC_QUALITY_SENSOR_CAL      0x50
#define OPC_QUALITY_EGU_EXCEEDED    0x54
#define OPC_QUALITY_SUB_NORMAL      0x58
#define OPC_QUALITY_LOCAL_OVERRIDE  0xD8
#define OPC_LIMIT_OK                0x00
#define OPC_LIMIT_LOW               0x01
#define OPC_LIMIT_HIGH              0x02
#define OPC_LIMIT_CONST             0x03




/////////////////////////////////////////////////////////////////////////////
// defines the WM_USER messages used by 4CE


// Message used by Navigator to notify about mouse clicks
// LPARAM points to the NAVDBlClkMsg or NAVRClickMsg structure
#define	NAVN_RCLICK		                    (WM_USER + 100)
#define NAVN_DBLCLK		                    (WM_USER + 101)
#define PROJECT_PARSE                       (WM_USER + 102)
#define DEL_KEY_PRESSED                     (WM_USER + 103)
#define INS_KEY_PRESSED                     (WM_USER + 104)
#define MSG_PARSE_NOW                       (WM_USER + 105)
#define WM_NAV_FV_FILE_CHANGED              (WM_USER + 106)
#define WM_NAV_FV_DECR_CHECK_DISABLE_COUNT  (WM_USER + 107)


// Message used by CompilerNotify-Thread to notify about a new Message
// LPARAM points to the MessageText (LPCTSTR)
#define MSGTHRD_NOTIFY	                    (WM_USER + 200)
#define MSGTHRD_READY                       (WM_USER + 201)

// Message used by CPComp to notify about session and variable informations
// LPARAM points to the SessionMsg structure
#define SESSION_NOTIFY                      (WM_USER + 300)
#define VARIABLE_NOTIFY                     (WM_USER + 301)

// Message used by generic editor view to delay activation of an inplace
// standard editor view until generic editor view is ready to go.
#define ACTIVATE_EDITOR                     (WM_USER + 400)

// Message used by generic editor frame to trigger layout recalculation
#define RECALCULATE_LAYOUT                  (WM_USER + 401)

// Message used by generic editor frame to give focus to active child frame
// (Ctrl+Tab did not work without this when pressed while inside an editor)
#define SET_FOCUS_TO_ACTIVE_CHILD_FRAME     (WM_USER + 402)

// Message sent by splitter tab window whenever context menu
// in tab zone is popped up.
#define WM_TAB_CONTEXTMENU                  (WM_USER + 500)

// Message is sent when a fieldbus master configurator is closed
#define WM_CONFIGURE_COMPLETED              (WM_USER + 12345)




/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// defines for console
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// defines the special ATCM URL-Scheme (used by PanelView to interrupt any Navigation)
// use only uppercase in this text!
#define CONSOLE_4C_URL_SCHEME	_T("ATCM")

// Special URL scheme used to navigate to files. Different from "file"
// scheme in that it allows relative path specifications. These rela-
// tive specifications are resolved by prepending the project path.
#define CONSOLE_4CFILE_URL_SCHEME _T("ATCMFILE")

// string for calling the DEFAULT.HTM
#define CONSOLE_MAIN_FACEPLATE	_T("http://%s/4c/CP_Login.asp?password=%s")

// string for calling a special task faceplate
#define CONSOLE_TASK_FACEPLATE	_T("http://%s/4c/CP_Login.asp?password=%s&url=/4C/CP_Loader.asp&AspFile=CP_Task.asp&ObjName=%s&ConfFile=/4C/config/tasks.txt&StandAlone=1")


// string for calling a special task faceplate
#define CONSOLE_TASKLIST_FACEPLATE	_T("http://%s/4c/CP_Login.asp?password=%s&url=/4C/CP_Loader.asp&AspFile=CP_TaskList.asp&ConfFile=/4c/config/tasks.txt&ObjName=TaskList of %s&StandAlone=1")



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// defines some common file names and paths
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define CE_PROJ_FILE_EXT        _T("4cp")

//  Standard or default paths to library and projects directories
//    related to the install directory
//    Do not use this constants directly - use UTIL functions instead.
#define CE_STDPROJECT_PATH      _T("\\Projects")
#define CE_STDLIB_PATH          _T("\\lib")

// relative path to the compiler output gen directory (relative to project dir)
#define CE_GEN_PATH             _T("gen")

// relative path to the compiler output classes directory (relative to project dir)
#define CE_4CCG_CLASS_PATH      _T("gen\\__classes")

// relative path to the directory where the representation of class and target view 
// are stored (relative to project dir)
#define CE_PROJECT_VIEW_PATH    _T("gen\\__views")

// relative path to the image directory
#define CE_IMAGE_PATH           _T("Engineering\\bin\\Images")

// file name for the online manual relative to installation dir.
#define CE_MANUAL_FILENAME      _T("\\Engineering\\Help\\Manual.chm")

// the files to store the class and targetview between two sessions
#define CE_FILE_FILEVIEW        _T("fileview.xml")
#define CE_FILE_CLASSVIEW       _T("classview.xml")
#define CE_FILE_TARGETVIEW      _T("targetview.xml")
#define CE_FILE_INSTANCEVIEW    _T("instanceview.xml")

// file extension of instance view XML-files created by compiler
#define CE_INSTANCE_EXT         _T("inst")

/////////////////////////////////////////////////////////////////////////////
// file name and entry name for master password file
#define WHO_FILE_NAME   _T("\\Engineering\\bin\\who.list")
#define WHO_ENGINEERING _T("Engineering")

// predefined 4cp file sections and key values
#define CE_CP_FILE         _T("file")
#define CE_CP_TARGETS      _T("TARGETS")
#define CE_CP_USERHELP     _T("USER_HELP")
#define CE_CP_ADDFILES     _T("ADD_FILES")
#define CE_CP_ATTRIBUTES   _T("Attributes")
#define CE_CP_FORMAT       _T("FORMAT")
#define CE_CP_VERSION      _T("version")
#define CE_CP_VER_20       _T("V2.0")
#define CE_CP_USERTEXT1    _T("UserText1")
#define CE_CP_USERTEXT2    _T("UserText2")
#define CE_CP_USERTEXT3    _T("UserText3")
#define CE_CP_URL          _T("UserUrl")
#define CE_CP_ALIAS        _T("Alias")

#define CE_CP_LIBS         _T("LIBS")
#define CE_CP_GVLS         _T("Classes.GVLs")
#define CE_CP_CONSTS       _T("Classes.Constants")

#define CE_CP_SETTINGS_TARGET           _T("Target")
#define CE_CP_SETTINGS_CONNECTTIMEOUT   _T("ConnectionTimeout")


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// defines for xml representation of backparser output, tree representation
// of navigator and project manager
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define CE_XMLTAG_NODE              _T("NODE")
#define CE_XMLTAG_LINK              _T("LINK")
#define CE_XMLTAG_TMPNODE           _T("TMPNODE")
                                    
#define CE_XMLATTR_TEXT             _T("TEXT")
#define CE_XMLATTR_ICON             _T("ICON")
#define CE_XMLATTR_ID               _T("ID")
#define CE_XMLATTR_TYPE             _T("TYPE")
#define CE_XMLATTR_SOURCE           _T("SOURCE")
#define CE_XMLATTR_OPENSOURCE       _T("OPENSOURCE")
#define CE_XMLATTR_TOOLTIP          _T("TOOLTIP")
#define CE_XMLATTR_USRTEXT1         _T("USERTEXT1")
#define CE_XMLATTR_USRTEXT2         _T("USERTEXT2")
#define CE_XMLATTR_USRTEXT3         _T("USERTEXT3")
#define CE_XMLATTR_URL              _T("URL")
#define CE_XMLATTR_USER_HELP        _T("USER_HELP")
#define CE_XMLATTR_LOCATION         _T("LOCATION")
#define CE_XMLATTR_DATA_TYPE        _T("DATA_TYPE")
#define CE_XMLATTR_CONSOLE_URL      _T("CONSOLE_URL")
#define CE_XMLATTR_INSTANCE_PATH    _T("INSTANCE_PATH")
#define CE_XMLATTR_VISIBILITY       _T("VISIBILITY")
#define CE_XMLATTR_STATE            _T("STATE")
#define CE_XMLATTR_STATETEXT        _T("STATETEXT")
#define CE_XMLATTR_CREATED          _T("CREATED")
#define CE_XMLATTR_MODIFIED         _T("MODIFIED")
#define CE_XMLATTR_USRCREATED       _T("USRCREATED")
#define CE_XMLATTR_USRMODIFIED      _T("USRMODIFIED")
#define CE_XMLATTR_ALIAS            _T("ALIAS")
// for pous (non iec)
#define CE_XMLATTR_MANGLEDNAME      _T("MANGLED_NAME")
// for vairiables

#define CE_XMLATTR_ADDRESS          _T("ADDRESS")
#define CE_XMLATTR_DATATYPE         _T("DATA_TYPE")
#define CE_XMLATTR_INITVAL          _T("INIT_VALUE")
#define CE_XMLATTR_MODIFIER         _T("MODIFIER")
#define CE_XMLATTR_EDGE             _T("EDGE")
#define CE_XMLATTR_ALIAS            _T("ALIAS")
#define CE_XMLATTR_COMMENT          _T("COMMENT")

// for project manager internal
#define CE_XMLATTR_MODTIME          _T("MOD_TIME")
#define CE_XMLATTR_PARSERSOURCE     _T("PARSER_SOURCE")

#define CE_XMLATTR_SYSTEM           _T("SYSTEM")
#define CE_XMLATTR_EXPANDED         _T("EXPANDED")
#define CE_XMLATTR_FILTER_VIS       _T("FVIS")

#define CE_XMLATTR_TRUE_VAL         _T("1")
#define CE_XMLATTR_FALSE_VAL        _T("0")

#define CE_XMLATTR_ADDR             _T("CONNECT_INFO")
#define CE_XMLATTR_USERADDR         _T("USER_CONNECT_INFO")
#define CE_XMLATTR_CONSOLE_ADDR     _T("CONSOLE_ADDR")
#define CE_XMLATTR_RESOURCE         _T("ASSIGNMENT")
#define CE_XMLATTR_INSTANCE_PATH    _T("INSTANCE_PATH")
#define CE_XMLATTR_STATE_VAR        _T("STATE_VAR")
#define CE_XMLATTR_STATETEXT_VAR    _T("STATETEXT_VAR")
#define CE_XMLATTR_DOWNLOAD         _T("DOWNLOAD")
#define CE_XMLATTR_FIRMWARE         _T("FIRMWARE")

#define CE_XMLATTR_CHANNELNR        _T("CHANNEL_NR")

#define CE_XMLTYPE_ERRORNODE        _T("ERROR_NODE")

#define CE_XML_IDPATH_SEP           _T("/")
#define CE_XML_IDPATH_SEP_CHAR      _T('/')
#define CE_XML_IDPATH_SEP_DOT       _T(".")
#define CE_XML_IDPATH_SEP_DOT_CHAR  _T('.')

// used to build up id-paths for different views
#define CE_XML_IDPATH_CLASSVIEW     _T("CLASSVIEW")
#define CE_XML_IDPATH_TARGETVIEW    _T("TARGETVIEW")
#define CE_XML_IDPATH_INSTANCEVIEW  _T("INSTANCEVIEW")

#define CE_XML_INST_ADD             _T("_INST")
#define CE_XMLATTR_TARGET_TYPE      _T("TARGET_TYPE")

#define CE_XMLATTR_WAIT_TARGET_CONN _T("WAIT_FOR_TARGET_CONNECTION")

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// defines for acessing components via the ComponentManagers 
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#define CE_COMPMAN_KADMAN           _T("KadManager")
#define CE_COMPMAN_PRJMAN           _T("ProjectManager")
#define CE_COMPMAN_MSGVIEW          _T("Message View")
#define CE_COMPMAN_NAV              _T("Navigator")
#define CE_COMPMAN_WATCH            _T("Watch")
#define CE_COMPMAN_POUVARSEL        _T("PouVarSelection")
#define CE_COMPMAN_BREAKPTMAN       _T("BreakpointManager")
#define CE_COMPMAN_BPVIEW           _T("BreakPointView")
#define CE_COMPMAN_XREF             _T("XRefTool")
#define CE_COMPMAN_EXE              _T("ExeDelegator")
#define CE_COMPMAN_FIF              _T("FindInFiles")

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// registry sections and keys
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// engineering
#define REG_SECTION_ENGINEERING _T("ATCMControl Engineering")
#define REG_KEY_LAST_PROJECT    _T("LastProject")

// FRAME:
#define REG_SECTION_FRAME       _T("Frame")
#define REG_KEY_FRAME_SHOW      _T("Show")
#define REG_KEY_FRAME_LEFT      _T("Left")
#define REG_KEY_FRAME_RIGHT     _T("Right")
#define REG_KEY_FRAME_TOP       _T("Top")
#define REG_KEY_FRAME_BOTTOM    _T("Bottom")

// DOCKING (CSizedDialogBar)
#define REG_KEY_DOCKING_HEIGHT  _T("DockingHeight")

// FIND IN FILES
#define FIF_OPTION_WHOLEWORD         0x01
#define FIF_OPTION_CASESENSITIVE     0x02
#define FIF_OPTION_REGEXP            0x04

// PRE DOWNLOAD STEP
#define CE_PREDL_STEP_DISP_ID   _T("PreDLStep")
#define CE_POSTDL_STEP_DISP_ID  _T("PostDLStep")

// file status enumerator
enum EFileStatus
{
    eFSNormal   = 0,    // status normal
    eFSReadOnly = 1,    // status read only
    eFSMissing  = 2,    // status missing
};

#endif // __CESYSDEF_H_