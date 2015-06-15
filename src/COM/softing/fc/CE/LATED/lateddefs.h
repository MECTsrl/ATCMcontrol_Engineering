#ifndef __LATEDDEFS_H_
#define __LATEDDEFS_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#define IMPLEMENTED_CAT    _T("Implemented Categories")
#define BACKSLASH          _T("\\") 
#define LATED_SERIAL_VERSION  (1)         // for serialization

// maximum number of chars of the fix part of the identifiers
#define MAX_GEN_TEXT        13

//Element names:
#define LATED_ELEM_LATCOMM         _T("LATCOMM_4C")
#define LATED_ELEM_LC_DATATYPE     _T("LC_DATATYPE")
#define LATED_ELEM_LC_MEMBER       _T("LC_MEMBER")
#define LATED_ELEM_LC_DATA         _T("LC_DATA")
#define LATED_ELEM_LC_LINK         _T("LC_LINK")
#define LATED_ELEM_LC_LINK_DEST    _T("LC_LINK_DEST")
#define LATED_ELEM_LC_GEN          _T("LC_GEN")

//attribute names:
#define LATED_ATTR_NAME            _T("NAME")
#define LATED_ATTR_DATAID          _T("DATAID")
#define LATED_ATTR_TYPE            _T("TYPE")
#define LATED_ATTR_IP              _T("IP")
#define LATED_ATTR_TARGET          _T("TARGET")
#define LATED_ATTR_CONNTYPE        _T("CONNTYPE")
#define LATED_ATTR_ALIAS           _T("ALIAS")
#define LATED_ATTR_FILE            _T("FILE")
#define LATED_ATTR_GENTYPE         _T("GENTYPE")
#define LATED_ATTR_GENPOS          _T("GENPOS")
#define LATED_ATTR_TASK            _T("TASK")
#define LATED_ATTR_CLASS           _T("CLASS")
#define LATED_ATTR_INTERVAL        _T("INTERVAL")
#define LATED_ATTR_PRIORITY        _T("PRIORITY")

//attribute value
#define LATED_CONNTYPE_P2P         _T("P2P")
#define LATED_CONNTYPE_PUBLISH     _T("PUBLISH")
#define LATED_CONNTYPE_SUBSCRIBE   _T("SUBSCRIBE")
#define LATED_GENTYPE_FILE         _T("FILE")
#define LATED_GENTYPE_LINK_IMP     _T("LINK_IMP")
#define LATED_GENTYPE_LINK_TASK    _T("LINK_TASK")
#define LATED_GENTYPE_LINK_PROG    _T("LINK_PROG")
#define LATED_GENPOS_FIRST         _T("POS_FIRST")
#define LATED_GENPOS_LAST          _T("POS_LAST")


//defines for the backparse interface
#define LATED_TYPE _T("LC")
#define LATED_BACKPARSE_NODE _T("<NODE TEXT=")
#define LATED_QUOTE _T("\"")
#define LATED_BACKPARSE_ID  _T(" ID=")
#define LATED_BACKPARSE_TYPE _T(" TYPE=")
#define LATED_BACKPARSE_SOURCE _T(" SOURCE=")
#define LATED_BACKPARSE_ENDTAG _T("/>")

#define LATED_BRACKET_S _T('[')
#define LATED_BRACKET_E _T(']')

#define LATED_IMAGEID_GROUP      (0)
#define LATED_IMAGEID_DATATYPE   (1)
#define LATED_IMAGEID_MEMBER     (2)
#define LATED_IMAGEID_DATAGROUP  (3)
#define LATED_IMAGEID_LINKSRCPC  (4)
#define LATED_IMAGEID_LINKSRCEC  (5)
#define LATED_IMAGEID_LINKDSTPC  (6)
#define LATED_IMAGEID_LINKDSTEC  (7)
#define LATED_IMAGEID_LINKSRCCMZ (8)
#define LATED_IMAGEID_LINKDSTCMZ (9)

#define LATED_IPSTR_LEN         (80)
#define LATED_TARGET_TYPE_PC    _T("PC")
#define LATED_TARGET_TYPE_EC    _T("WAGO")
#define LATED_TARGET_TYPE_CMZ   _T("CMZ")

#define LC_PORT_4C              (16000)
#define LC_FIRST_DATA_ID        (LC_PORT_4C + 1)
#define LC_LAST_DATA_ID         (LC_FIRST_DATA_ID + 254)

/**
 * type LATED_CONN_TYPE - Connection type
 * 
*/
typedef enum
{
  UNKNOWN=0, 
  P2P = 1, 
  PUBLISH = 2, 
  SUBSCRIBE = 3
} LATED_CONN_TYPE;

/**
 * type LATED_HINT_TYPE - Hint types for views used 
 * with UpdateAllViews()
 * 
*/
typedef enum
{
    /**Invalid hint */
    INVALID = 0,
    /** Update all views after load of document config */
    UPDATE_ALL = 1,
    /** Update all views after new document created */
    UPDATE_NEW = 2,
    /** Update all views upon reload of document or close document */
    UPDATE_CLEAR = 3
} LATED_HINT_TYPE;

enum ListenerId
{
    Invalid = 0,
    TraverseDatatypes,
    TraverseLinks,
    TraverseMembers,
    RemoveMembers,
    AddDataType,
    DelDataType,
    GenFromData,
    GenFromDataReset,
    GenFromLink,
    TraverseIECTypes,
    GetTargetList,
    Add_Link,
    Del_Link,
    AssignDatatype
};

typedef enum
{
    GENTYPE_UNDEF     = -1,
    GENTYPE_FILE      = 0,
    GENTYPE_LINK_IMP,
    GENTYPE_LINK_TASK,
    GENTYPE_LINK_PROG 
} LATED_GENTYPE_TYPE;

typedef enum
{
    GENPOS_UNDEF = -1,
    GENPOS_FIRST = 0,
    GENPOS_LAST  = 1
} LATED_GENPOS_TYPE;

// target type
// caution: for each target type a prefix must be defined
// in ccaTargetPrefixes (lconfiggen.cpp)
typedef enum  ETargetType
{
	eTargetPC,		// 0
	eTargetEC,		// 1
	eTargetCMZ,		// 2
};

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


#endif // __LATEDDEFS_H_


