/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CETypeId.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CETypeId.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CETypeId.cpp $
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



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "CELibrarian.h"
#include "CETypeId.h"
//----  Static Initializations:   ----------------------------------*



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

ObjClassId CETypeId::ObjIIdList[] = {
	{IID_PROJECT, CEDB_ProjectObj},
	{IID_CONSTANT, CEDB_ConstObj},
	{IID_DATATYPE, CEDB_DUTObj},
	{IID_FB, CEDB_FBObj},
	{IID_FNC, CEDB_FncObj},
	{IID_CONFIG, CEDB_ConfigurationObj},
	{IID_RESOURCE, CEDB_ResourceObj},
	{IID_TASK, CEDB_TaskObj},
	{IID_PROGRAM, CEDB_ProgramObj},
    {IID_PRGINST, CEDB_ProgramInstObj},
	{IID_VAR, CEDB_VarObj},
	{IID_GLBVARS, CEDB_GlobVarPool},
	{IID_CONSTANTS, CEDB_ConstPool},
    {IID_PC, CEDB_PCObj},
    {IID_MASTER, CEDB_FieldbusMasterObj},
	{IID_SLAVE, CEDB_FieldbusSlaveObj},
	{IID_MODUL, CEDB_FieldbusModulObj}
};

CETypeId::CETypeId()
{

}

CETypeId::~CETypeId()
{

}
/////////////////////////////////////////////////////////////////////
/**
 * get image id from class of library item
 *
 * @param ObjClass	object class id
 * @return		image id of object class
 *				-1 if unknown object class
 * @exception   
 * @see         
 */
long CETypeId::ObjClassToIId(CEDBOBJCLASS ObjClass) {
	int iListCnt = sizeof(ObjIIdList) / sizeof(ObjClassId);
	for(int i = 0; i < iListCnt; i++) {
		if(ObjIIdList[i].ObjClass == ObjClass) {
			return(ObjIIdList[i].lIId);
		}
	}
	return(-1);
};

/*
 *----------------------------------------------------------------------------*
 *  $History: CETypeId.cpp $
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
