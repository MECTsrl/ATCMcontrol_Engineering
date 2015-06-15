//secstr.h

//This file contains the #define directive ID's and the text strings
//used in the Objective Toolkit and Objective Toolkit Pro source files.

#ifndef __SECSTR_H__
#define __SECSTR_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#define SEC_BEDIT1	_T("(nofiles)|NOFILES.$$$||")
#define SEC_BEDIT2	"All files (*.*)|*.*||"

#define SEC_CALEN1	_T("September, 2000")
#define SEC_CALEN2  _T("Sep, 2000")

#define SEC_DTPAR1	_T('h')
#define SEC_DTPAR2	_T('H')
#define SEC_DTPAR3	_T('m')
#define SEC_DTPAR4	_T('s')
#define SEC_DTPAR5	_T('t')
#define SEC_DTPAR6	_T('d')
#define SEC_DTPAR7	_T('M')
#define SEC_DTPAR8	_T('y')
#define SEC_DTPAR9	_T('g')


#define SEC_STOD1	"WARNING: Could not find: \"%s\"..."

#define MAIN_FRAME_LABEL  _T("MainFrame")
#define SEC_WSMGR1	_T("Window %d")

#define SEC_TBPG1	_T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")
#define SEC_TBPG2	_T('&')

#define SEC_SCUT1	_T('\t')
#define SEC_SCUT2	_T('\0')
#define SEC_SCUT3	_T("\t")

#endif // __SECSTR_H__