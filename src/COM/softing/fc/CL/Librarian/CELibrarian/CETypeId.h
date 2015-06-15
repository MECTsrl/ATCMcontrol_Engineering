/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CETypeId.h 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CETypeId.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CETypeId.h $
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

#ifndef __CETYPEID_H_
#define __CETYPEID_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include "CEDB.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*




#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



//ObjClass - IID definition
typedef struct {
	long			lIId;
	CEDBOBJCLASS	ObjClass;	// ObjClass 
} ObjClassId ;
;
/*
class ObjClassId
{
public:
	long lIId;
	CEDBOBJCLASS ObjClass;
};
*/
class CETypeId  
{
public:

	typedef	enum ImageIDs {			// IDs of the levels of the tree 
					            // also the code for Bitmap access!
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
    IID_DUMMY1,
    IID_DUMMY2,
    IID_DUMMY3,
    IID_DUMMY4,
    IID_DUMMY5,
	IID_OVL_COMPILED,
	IID_OVL_LOADED,
	IID_OVL_ONL,
	IID_OVL_ONLDOWN,
	IID_OVL_ONLDIST
};

protected:
	static ObjClassId ObjIIdList[];

public:
	CETypeId();
	virtual ~CETypeId();

public:
	static long ObjClassToIId(CEDBOBJCLASS ObjClass);
};


#endif // __CETYPEID_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: CETypeId.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:32p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/
