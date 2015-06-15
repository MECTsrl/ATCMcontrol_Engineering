#ifndef __KSO_H_
#define __KSO_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// defines for kernel standard operations (KSO)
#define KSO_MAX_ITEM_COUNT          50       // count of reserved numbers

#define KSO_OPEN                     0       // connected to attribute SOURCE
#define KSO_OPEN_USER_HELP           1       // connected to attribute USER_HELP
#define KSO_OPEN_URL                 2       // connected to attribute URL
#define KSO_GOTO                     3       // connected to attribute LOCATION
#define KSO_GOTO_DEFINITION          4       // connected to attribute TYPE_REF
#define KSO_DELETE                   5       // connected to attribute SOURCE
#define KSO_OPEN_CONSOLE             6       // connected to attribute CONSOLE_URL
#define KSO_OPEN_INSTANCE            7       // connected to attribute INSTANCE_PATH

#define KSO_CONNECT                  8       // connected to attribute CONNECT_INFO
#define KSO_DISCONNECT               9       // connected to attribute CONNECT_INFO
#define KSO_CONNECT_EX              10       // connected to attribute CONNECT_INFO
#define KSO_DISCONNECT_EX           11       // connected to attribute CONNECT_INFO

#define KSO_DOWNLOAD_FULL           12       // connected to attribute CONNECT_INFO
#define KSO_DOWNLOAD_CHANGES        13       // connected to attribute CONNECT_INFO
#define KSO_DOWNLOAD_SELECTION      14       // connected to attribute CONNECT_INFO
#define KSO_DOWNLOAD_FULL_ALL       15      
#define KSO_DOWNLOAD_CHANGES_ALL    16
#define KSO_DOWNLOAD_FULL_SELECT    17
#define KSO_DOWNLOAD_CHANGES_SELECT 18

#define KSO_BUILD_ALL               19       // not connected to attribute
#define KSO_BUILD_SELECT            20
#define KSO_BUILD_TARGET            21       // not connected to attribute
#define KSO_COMPILE_FILE            22       // not connected to attribute
#define KSO_BUILD_LIBRARY           23

#define KSO_START_CONTROL           24       // not connected to attribute
#define KSO_STOP_CONTROL            25       // not connected to attribute
#define KSO_START_ALL_TASKS         26       // not connected to attribute
#define KSO_STOP_ALL_TASKS          27       // not connected to attribute
#define KSO_COLDSTART_RESET         28       // not connected to attribute
#define KSO_WARMSTART_RESET         29       // not connected to attribute
#define KSO_INITIALIZE_CLEAR        30       // not connected to attribute
#define KSO_START_TASK              31       // not connected to attribute
#define KSO_STOP_TASK               32       // not connected to attribute

#define KSO_SETTINGS_RESET_DOCKING  33
#define KSO_SETTINGS_LIBRARY        34
#define KSO_SETTINGS_PASSWORD       35
#define KSO_SETTINGS_ENGINEERING    36
#define KSO_SETTINGS_PRINT          37

#define KSO_DOWNLOAD_FLASH          38
#define KSO_UPDATE_FLASH            39

#define KSO_CLEAR_FLASH             40

#define KSO_ITEM_COUNT              41       // current count of KSOs (except for KSO_EXPAND_ALL / COLLAPSE_ALL)


// these KSOs are used by the navigator
#define KSO_EXPAND_ALL         KSO_MAX_ITEM_COUNT - 2       // not connected
#define KSO_COLLAPSE_ALL       KSO_MAX_ITEM_COUNT - 1       // not connected

// defines for key / mouse events
#define NUM_KEY_MOUSE_EVENTS    7       // number of key / mouse events
// these are used as indices for events in key_mouse_array
#define KEY_MOUSE_OPEN          0       // open action (return key)
#define KEY_MOUSE_DELETE        1       // delete action (del key)
#define KEY_MOUSE_INSERT        2       // insert action (ins key)
#define KEY_MOUSE_RENAME        3       // rename action (left mouse click an sel item)
#define KEY_MOUSE_CUT           4       // cut operation (same as drag begin move)
#define KEY_MOUSE_COPY          5       // copy operation (same as drag begin copy)
#define KEY_MOUSE_PASTE         6       // paste operation (same as drop)

#endif // __KSO_H_