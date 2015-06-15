#ifndef CEDB_H
#define CEDB_H

/*  Database version of the engineering database: 
	(changes on engineering database cause no loss of any data)
	Increment database version each time you've made
    database changes, but comment the old entry and add a comment + date
    to the new entry.
*/
//  Date        Version     Description
// 14.07.1998   1           Initial database version
// 22.07.1998   2           Stored Procedure as test
// 29.07.1998   3           FilesTypes added
// 06.10.1998   4           DeleteFlag in FilesTable added
// 27.10.1998   5           DepFilesTable added
// 04.02.1999   6           next DBVersion for V1.1 (no real changes!)
// 18.03.1999   7           AssignTable added
// 25.03.1999	8           Task attribute "Autostart"
// 25.03.1999   10          Put/GetObjAttribute added
// 30.04.1999   11          FBus master attribute "Channel"
// 08.06.1999   12          Task attribute "IOSync"
// 17.08.1999   13          Language attribute of POU "CEDB_*_Language"
// 22.02.2000	14			new Fieldbusmaster type added
#define CE_DatabaseVersion	 14


/*  Database version of the engineerings downloader database: 
	(changes on donwloader database cause a full download for all controls with older versions!!!)
	Increment database version each time you've made
    database changes, but comment the old entry and add a comment + date
    to the new entry.
*/
// 07.03.2000	13			split from engineering database
#define CE_DLDatabaseVersion 13

// ATCM File Classes
typedef enum {
    CEDB_SrcFile            = 0,
    CEDB_AddFile            = 1,
    CEDB_LibFile            = 2
} CEFILECLASS;

// ATCM Object Classes
typedef enum  {
    CEDB_InvalidObjClass    = -1,
	CEDB_ProjectObj			= 0,
	CEDB_ConfigurationObj	= 1,
	CEDB_ResourceObj		= 2,
	CEDB_TaskObj			= 3,
	CEDB_ProgramObj			= 4,
	CEDB_DUTObj				= 5,
	CEDB_FBObj				= 6,
	CEDB_FncObj				= 7,
	CEDB_VarObj				= 8,
	CEDB_GlobVarObj			= 9,
	CEDB_ConstObj			= 10,
	CEDB_ProgramInstObj		= 11,
	CEDB_FBInstObj			= 12,
    CEDB_GlobVarPool        = 13,
    CEDB_ConstPool          = 14,
    CEDB_LibObj             = 15,
	CEDB_TagSystemObj		= 20,
    CEDB_GroupObj           = 30,
    CEDB_PCPoolObj          = 99,
	CEDB_PCObj				= 100,
	CEDB_CPUObj				= 101,
	CEDB_MemObj				= 102,
	CEDB_HDObj				= 103,
	CEDB_PrinterObj			= 104,
	CEDB_LANObj				= 105,
	CEDB_KeyBrdObj			= 106,
	CEDB_DisplayObj			= 107,
	CEDB_USVObj				= 108,
	CEDB_FieldbusMasterObj	= 109,
	CEDB_FieldbusSlaveObj	= 110,
	CEDB_FieldbusModulObj	= 111,
	CEDB_MultimediaObj		= 112,
    CEDB_OPCMasterObj       = 113,
	CEDB_OtherMasterObj		= 114
} CEDBOBJCLASS;

typedef enum {
    CEDB_TaskInterval       = 3000,
    CEDB_TaskPriority       = 3001,
    CEDB_PCAdress           = 3002,
	CEDB_TaskAutoStart		= 3003,
    CEDB_FBUS_Channel       = 3004,
	CEDB_TaskIOSync  		= 3005,
    CEDB_Language			= 3006
} CEOBJATTRIBUTE;
#endif //CEDB_H
