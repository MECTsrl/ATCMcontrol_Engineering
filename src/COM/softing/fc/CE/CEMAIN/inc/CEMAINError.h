
#ifndef __CEMAINERROR_H_
#define __CEMAINERROR_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


//standard setting for severity:
//predefined system wide facility codes (do not change!). You may
//insert additional facility codes between the main areas:
//currently supported languages:
//////////ERROR MESSAGES/////////////////////////////////////////////
//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define E_FACILITY_ENGINEERING           0x280


//
// Define the severity codes
//


//
// MessageId: E_CECOMPMAN_COMPONENT_ALREADY_ADDED
//
// MessageText:
//
//  Engineering: error S1900: Component already added
//
#define E_CECOMPMAN_COMPONENT_ALREADY_ADDED ((HRESULT)0xC2800384L)

//---------------
//
// MessageId: E_CECOMPMAN_UNKNOWN_COMPONENT
//
// MessageText:
//
//  Engineering: error S1901: Unknown component
//
#define E_CECOMPMAN_UNKNOWN_COMPONENT    ((HRESULT)0xC2800385L)

//---------------
//
// MessageId: E_CECOMPMAN_COMPONENT_NOT_ALIVE
//
// MessageText:
//
//  Engineering: error S1902: Component not alive
//
#define E_CECOMPMAN_COMPONENT_NOT_ALIVE  ((HRESULT)0xC2800386L)

//---------------
//
// MessageId: E_CECOMPMAN_NOT_COM_COMPONENT
//
// MessageText:
//
//  Engineering: error S1903: Component is not a COM component
//
#define E_CECOMPMAN_NOT_COM_COMPONENT    ((HRESULT)0xC2800387L)

//---------------
//
// MessageId: E_CECOMPMAN_NOT_DOCKINGTOOL_COMPONENT
//
// MessageText:
//
//  Engineering: error S1904: Component is not a docking tool
//
#define E_CECOMPMAN_NOT_DOCKINGTOOL_COMPONENT ((HRESULT)0xC2800388L)

//---------------
//
// MessageId: E_CECOMPMAN_NOT_PCSET_COMPONENT
//
// MessageText:
//
//  Engineering: error S1905: Component is not a PC set
//
#define E_CECOMPMAN_NOT_PCSET_COMPONENT  ((HRESULT)0xC2800389L)

//---------------
//
// MessageId: E_CECOMPMAN_INVALID_DOCKINGTOOL_CLASS
//
// MessageText:
//
//  Engineering: error S1906: Invalid docking tool class
//
#define E_CECOMPMAN_INVALID_DOCKINGTOOL_CLASS ((HRESULT)0xC280038AL)

//---------------
//
// MessageId: E_CECOMPMAN_FAILED_TO_CREATE_DOCKINGTOOL_WINDOW
//
// MessageText:
//
//  Engineering: error S1907: Failed to create docking tool window
//
#define E_CECOMPMAN_FAILED_TO_CREATE_DOCKINGTOOL_WINDOW ((HRESULT)0xC280038BL)

//---------------
//
// MessageId: E_CESTATEMAN_CALLBACK_ALREADY_REGISTERED
//
// MessageText:
//
//  Engineering: error S1950: State change callback already registered
//
#define E_CESTATEMAN_CALLBACK_ALREADY_REGISTERED ((HRESULT)0xC28003B6L)

//---------------
//
// MessageId: E_CESTATEMAN_CALLBACK_NOT_REGISTERED
//
// MessageText:
//
//  Engineering: error S1950: State change callback not registered
//
#define E_CESTATEMAN_CALLBACK_NOT_REGISTERED ((HRESULT)0xC28003B7L)

//---------------

#endif // __CEMAINERROR_H_
