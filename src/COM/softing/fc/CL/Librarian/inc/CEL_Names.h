/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/inc/CEL_Names.h 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CEL_Names.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/inc/CEL_Names.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CL
 *
 * =COMPONENT           CELibrarian
 *
 * =CURRENT      $Date: 28.02.07 18:59 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  6/25/2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __CEL_NAMES_H_
#define __CEL_NAMES_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*



#define CEL_LIBLIST						_T("lib.list")
#define CEL_VAR_EXTERNAL				_T("VAR_EXTERNAL")
//
//Types
//
#define CEL_TYPE_FUNCTION				_T("FUNCTION")
#define CEL_TYPE_FUNCTION_BLOCK			_T("FUNCTION_BLOCK")
#define CEL_TYPE_CONST					_T("CONST")
#define CEL_TYPE_STRUCT					_T("STRUCT")
#define CEL_TYPE_COMMON_FILES			_T("COMMON_FILES")

#define CEL_SEC_NAME_ADDFILES           _T("__ADDFILES")
#define CEL_SEC_NAME_COMMON             _T("__COMMON")

//
// attributes
//
#define CEL_ATTR_FILE					_T("file")
#define CEL_ATTR_TYPE					_T("type")
#define CEL_ATTR_PASSWORD				_T("password")


#define CEL_ATTR_HEADER_FILNAME			_T("headerFileName")
#define CEL_ATTR_HEADER_OFFSET_START	_T("headerByteOffsetStart")
#define CEL_ATTR_HEADER_OFFSET_END		_T("headerByteOffsetEnd")

#define CEL_ATTR_DISPLAY_FILNAME		_T("displayFileName")
#define CEL_ATTR_DISPLAY_OFFSET_START	_T("displayByteOffsetStart")
#define CEL_ATTR_DISPLAY_OFFSET_END		_T("displayByteOffsetEnd")

//
// link file
//
#define CEL_LINK_DEST					_T("dest")
#define CEL_LINK_HEADER					_T("header")
#define CEL_LINK_USES					_T("uses")
#define CEL_LINK_SOURCE					_T("source")
#define CEL_LINK_PUT					_T("put")
#define CEL_LINK_DISPLAY				_T("display")

#endif // __CEL_NAMES_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: CEL_Names.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/inc
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:33p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/inc
 *==
 *----------------------------------------------------------------------------*
*/
