#ifndef __NAVDEF_H_
#define __NAVDEF_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "KADHelper/kso.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#define ID_NAV_CONTEXT_BASE     2000    // context menu ids start here
#define ID_NAV_CONTEXT_END      2100    // context menu ids stop here

// defines for file view context menu
#define ID_NAV_FV_BEGIN         ID_NAV_CONTEXT_BASE
#define ID_NAV_FV_OPEN          ID_NAV_CONTEXT_BASE + KSO_OPEN
#define ID_NAV_FV_DELETE        ID_NAV_CONTEXT_BASE + KSO_DELETE
#define ID_NAV_FV_CUT           ID_NAV_CONTEXT_BASE + KSO_MAX_ITEM_COUNT + 1
#define ID_NAV_FV_COPY          ID_NAV_CONTEXT_BASE + KSO_MAX_ITEM_COUNT + 2
#define ID_NAV_FV_PASTE         ID_NAV_CONTEXT_BASE + KSO_MAX_ITEM_COUNT + 3
#define ID_NAV_FV_RENAME        ID_NAV_CONTEXT_BASE + KSO_MAX_ITEM_COUNT + 4
#define ID_NAV_FV_PROPERTIES    ID_NAV_CONTEXT_BASE + KSO_MAX_ITEM_COUNT + 5
#define ID_NAV_FV_ADD_FILE      ID_NAV_CONTEXT_BASE + KSO_MAX_ITEM_COUNT + 6
#define ID_NAV_FV_CREATE_FOLDER ID_NAV_CONTEXT_BASE + KSO_MAX_ITEM_COUNT + 7 
#define ID_NAV_FV_OPEN_TEXT     ID_NAV_CONTEXT_BASE + KSO_MAX_ITEM_COUNT + 8
#define ID_NAV_FV_END           ID_NAV_CONTEXT_END

// offset for the CE_STATE definitions (same value range as connection states!
#define CE_STATE_OFFSET 10

typedef enum NAV_TreeMode {
    NAV_NOVIEW      = -1,
	NAV_FILEVIEW    = 0,
	NAV_CLASSVIEW   = 1,
	NAV_INSTVIEW	= 2,
	NAV_HWVIEW      = 3,
    NAV_PRJLISTVIEW = 10
};


typedef 
enum tagCEOBJATTRIBS
    {	CEOBJ_ATTR_ONL	= 1,
	CEOBJ_ATTR_ONLDOWN	= 2,
	CEOBJ_ATTR_ONLDIST	= 3,
	CEOBJ_ATTR_WAIT_FOR_ONL	= 4,
	CEOBJ_ATTR_COMPILED	= 5,
	CEOBJ_ATTR_LOADED	= 6
    }	CEOBJATTRIBS;

// preliminary: use old image resource
// enum for image ids
enum ImageIDs
{
    IID_PROJECT = 0,
    IID_FILES,
	IID_FILES_OPEN,
    IID_FILE,
    IID_PC,
    IID_SYSTEMHWS,
    IID_LIBRARY,
	IID_BOOKS,					// all manuals
	IID_BOOK,					// closed manual
	IID_BOOK_OPEN,				// opened manual
	IID_PAGE,					// manual page
	IID_CONSTANTS,
	IID_CONSTANT,
    IID_DATATYPES,
    IID_DATATYPE,
    IID_PROGRAMS,
    IID_PROGRAM,
    IID_FBS,
    IID_FB,
    IID_FNCS,
    IID_FNC,
    IID_CONFIG,
    IID_RESOURCE,
    IID_TASK,
    IID_PRGINST,
    IID_FBINST,
    IID_GLBVARS,
    IID_VAR,
    IID_FIELDBUSSES,
    IID_MASTER,
    IID_SLAVE,
    IID_MODUL,
    IID_OPCMASTER,
    IID_ADDFILES,
    IID_LIBFILES,
    IID_MSTMASTER,
    IID_DUMMY5,
	IID_OVL_COMPILED,
	IID_OVL_LOADED,
	IID_OVL_ONL,
	IID_OVL_ONLDOWN,
	IID_OVL_ONLDIST,
	IID_OVL_ONLWAIT,
    IID_OVL_STATE_EXEC,
    IID_OVL_STATE_PAUSED,
    IID_OVL_STATE_CREATE,
    IID_OVL_STATE_BPREACHED,
    IID_OVL_STATE_ERROR,
	IID_OVL_STATE_EXEC_CLEARED,
	IID_OVL_STATE_PAUSED_CLEARED,
	IID_PRJVIEW_MULSEL
};

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


#endif // __NAVDEF_H_
