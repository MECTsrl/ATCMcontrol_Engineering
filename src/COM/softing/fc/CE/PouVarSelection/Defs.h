#ifndef __DEFS_H_
#define __DEFS_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define PVS_MAX_LRU        5  // max elements in lru list


// constants for the two dialogs that can be selected 
// via the tab control
#define PVS_DLG_POU_SEL     0   // pou selection dialog
#define PVS_DLG_VAR_SEL     1   // var selection dialog


// constant strings to display in selection lists ==> moved to resources
//#define PVS_SEL_ALL             _T("<All>")
//#define PVS_SEL_PROJECT         _T("<Project>")
//#define PVS_SEL_HEADER          _T("<Header>")
//#define PVS_SEL_GLOBAL          _T("<Global>")
//#define PVS_SEL_CONSTS          _T("<Constants>")
//#define PVS_SEL_COMPLEX         _T("<Complex>")
//#define PVS_SEL_SIMPLE          _T("<Simple>")


//#define PVS_SEL_NONE            _T("None")


#define PVS_USER_HELP_MARK_FILE  _T("__HELPFILES")


#define PVS_HEADER_DIR          _T("Header")
#define PVS_HEADER_EXT          _T(".hdr")









#endif // __DEFS_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: Defs.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:58
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CE/PouVarSelection
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CE/PouVarSelection
 * 
 * *****************  Version 13  *****************
 * User: Jd           Date: 22.10.01   Time: 9:39
 * Updated in $/4Control/COM/softing/fc/CE/PouVarSelection
 * Moved some hard coded strings to resources.
 * 
 * *****************  Version 12  *****************
 * User: Jd           Date: 1.08.01    Time: 14:45
 * Updated in $/4Control/COM/softing/fc/CE/PouVarSelection
 * Get LruList and PouDef functions for Editor.
 * 
 * *****************  Version 11  *****************
 * User: Jd           Date: 31.07.01   Time: 10:19
 * Updated in $/4Control/COM/softing/fc/CE/PouVarSelection
 * Implementation of types filter in var decl.
 * 
 * *****************  Version 10  *****************
 * User: Jd           Date: 19.07.01   Time: 16:58
 * Updated in $/4Control/COM/softing/fc/CE/PouVarSelection
 * Rearranged Variable selection dialog.
 * 
 * *****************  Version 9  *****************
 * User: Jd           Date: 18.07.01   Time: 15:27
 * Updated in $/4Control/COM/softing/fc/CE/PouVarSelection
 * Detailed data type handling for variable objects.
 * 
 * *****************  Version 8  *****************
 * User: Jd           Date: 12.07.01   Time: 14:47
 * Updated in $/4Control/COM/softing/fc/CE/PouVarSelection
 * Show header from pou selection.
 * 
 * *****************  Version 7  *****************
 * User: Jd           Date: 10.07.01   Time: 10:33
 * Updated in $/4Control/COM/softing/fc/CE/PouVarSelection
 * Integrated Show User Help.
 * 
 * *****************  Version 6  *****************
 * User: Jd           Date: 4.07.01    Time: 17:02
 * Updated in $/4Control/COM/softing/fc/CE/PouVarSelection
 * Moved PouVar Helper for drag and drop to static lib.
 * 
 * *****************  Version 5  *****************
 * User: Jd           Date: 4.07.01    Time: 15:41
 * Updated in $/4Control/COM/softing/fc/CE/PouVarSelection
 * Implemented drag and drop from pou selection. Changed list boxes into
 * list controls.
 * 
 * *****************  Version 4  *****************
 * User: Jd           Date: 9.04.01    Time: 15:33
 * Updated in $/4Control/COM/softing/fc/CE/PouVarSelection
 * 
 * *****************  Version 3  *****************
 * User: Jd           Date: 9.04.01    Time: 11:48
 * Updated in $/4Control/COM/softing/fc/CE/PouVarSelection
 * Adapted to new mangled name syntax.
 * 
 * *****************  Version 2  *****************
 * User: Jd           Date: 9.04.01    Time: 11:16
 * Updated in $/4Control/COM/softing/fc/CE/PouVarSelection
 * Reworked data holding objects.Filled var selection.
 * 
 * *****************  Version 1  *****************
 * User: Jd           Date: 2.04.01    Time: 9:52
 * Created in $/4Control/COM/softing/fc/CE/PouVarSelection
 *==
 *----------------------------------------------------------------------------*
*/
