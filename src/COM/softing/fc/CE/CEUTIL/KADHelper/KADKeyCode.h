#ifndef __KADKEYCODE_H_
#define __KADKEYCODE_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

#define     NUM_KEY_CODES   96

// virtual key codes as defined in the MSDN library
// 0-9 and A-Z are defined additionally to set the virtual key flag
// CKADMenuItem::m_wAccelVirt correctly (see CKADInfo::SetAcceleratorData())
const char* con_caKeyCode[] =
{
    "VK_BACK",      "8",	//	BACKSPACE key
    "VK_AB",        "9",	//	AB key
    "VK_CLEAR",     "C",	//	CLEAR key
    "VK_RETURN",    "D",	//	ENTER key
    "VK_PAUSE",	    "13",	//	PAUSE key
    "VK_CAPITAL",   "14",   //	CAPS LOCK key
    "VK_ESCAPE",    "1B",   //	ESC key
    "VK_SPACE",     "20",   //	SPACEBAR
    "VK_PRIOR",     "21",   //	PAGE UP key
    "VK_NEXT",      "22",   //	PAGE DOWN key
    "VK_END",	    "23",   //	END key
    "VK_HOME",	    "24",   //	HOME key
    "VK_LEFT",	    "25",   //	LEFT ARROW key
    "VK_UP",	    "26",   //	UP ARROW key
    "VK_RIGHT",	    "27",   //	RIGHT ARROW key
    "VK_DOWN",	    "28",   //	DOWN ARROW key
    "VK_SELECT",    "29",   //	SELECT key
    "VK_PRINT",	    "2A",   //	PRINT key
    "VK_EXECUTE",	"2B",   //	EXECUTE key
    "VK_SNAPSHOT",	"2C",   //	PRINT SCREEN key
    "VK_INSERT",    "2D",   //	INS key
    "VK_DELETE",    "2E",   //	DEL key
    "VK_HELP",	    "2F",   //	HELP key
    "0",	        "30",   //	0 key
    "1",	        "31",   //	1 key
    "2",	        "32",   //  2 key
    "3",	        "33",   //  3 key
    "4",	        "34",   //	4 key
    "5",	        "35",   //	5 key
    "6",	        "36",   //	6 key
    "7",	        "37",   //	7 key
    "8",	        "38",   //	8 key
    "9",	        "39",   //	9 key
    "A",	        "41",   //	A key
    "B",	        "42",   //	B key
    "C",	        "43",   //	C key
    "D",	        "44",   //	D key
    "E",	        "45",   //	E key
    "F",	        "46",   //	F key
    "G",	        "47",   //	G key
    "H",	        "48",   //	H key
    "I",        	"49",   //	I key
    "J",        	"4A",   //	J key
    "K",        	"4B",   //	K key
    "L",        	"4C",   //	L key
    "M",        	"4D",   //	M key
    "N",        	"4E",   //	N key
    "O",        	"4F",   //	O key
    "P",        	"50",   //	P key
    "Q",        	"51",   //	Q key
    "R",        	"52",   //	R key
    "S",        	"53",   //	S key
    "T",        	"54",   //	T key
    "U",        	"55",   //	U key
    "V",        	"56",   //	V key
    "W",        	"57",   //	W key
    "X",        	"58",   //	X key
    "Y",        	"59",   //	Y key
    "Z",        	"5A",   //	Z key
    "VK_LWIN",  	"5B",   //	Left Windows key (Microsoft® Natural® keyboard)
    "VK_RWIN",  	"5C",   //	Right Windows key (Natural keyboard)
    "VK_APPS",  	"5D",   //	Applications key (Natural keyboard)
    "VK_SLEEP",	    "5F",   //	Computer Sleep key
    "VK_NUMPAD0",	"60",   //	Numeric keypad 0 key
    "VK_NUMPAD1",	"61",   //	Numeric keypad 1 key
    "VK_NUMPAD2",	"62",   //	Numeric keypad 2 key
    "VK_NUMPAD3",	"63",   //	Numeric keypad 3 key
    "VK_NUMPAD4",	"64",   //	Numeric keypad 4 key
    "VK_NUMPAD5",	"65",   //	Numeric keypad 5 key
    "VK_NUMPAD6",	"66",   //	Numeric keypad 6 key
    "VK_NUMPAD7",	"67",   //	Numeric keypad 7 key
    "VK_NUMPAD8",	"68",   //	Numeric keypad 8 key
    "VK_NUMPAD9",	"69",   //	Numeric keypad 9 key
    "VK_MULTIPLY",	"6A",   //	Multiply key
    "VK_ADD",	    "6B",   //	Add key
    "VK_SEPARATOR",	"6C",   //	Separator key
    "VK_SUBTRACT",	"6D",   //	Subract key
    "VK_DECIMAL",	"6E",   //	Decimal key
    "VK_DIVIDE",	"6F",   //	Divide key
    "VK_F1",	    "70",   //	F1 key
    "VK_F2",	    "71",   //	F2 key
    "VK_F3",	    "72",   //	F3 key
    "VK_F4",	    "73",   //	F4 key
    "VK_F5",	    "74",   //	F5 key
    "VK_F6",	    "75",   //	F6 key
    "VK_F7",	    "76",   //	F7 key
    "VK_F8",	    "77",   //	F8 key
    "VK_F9",	    "78",   //	F9 key
    "VK_F10",	    "79",   //	F10 key
    "VK_F11",   	"7A",   //	F11 key
    "VK_F12",   	"7B",   //	F12 key
    "VK_F13",   	"7C",   //	F13 key
    "VK_F14",   	"7D",   //	F14 key
    "VK_F15",   	"7E",   //	F15 key
    "VK_F16",   	"7F",   //	F16 key
    "VK_NUMLOCK",	"90",   //	NUM LOCK key
    "VK_SCROLL",	"91"    //	SCROLL LOCK key
};

#endif // __KADKEYCODE_H_