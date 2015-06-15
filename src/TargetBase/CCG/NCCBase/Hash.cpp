/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/NCCBase/Hash.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Hash.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/NCCBase/Hash.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			NCCBase
 *
 * =CURRENT 	 $Date: 28.02.07 19:04 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"

#include "Hash.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

C__objects_hash::C__objects_hash()
{

}

C__objects_hash::~C__objects_hash()
{

}

void C__objects_hash::collect(CString& strFileName)
{
	CString				strLine;
	CString				strName;
	int					pos, index;

	CStdioFile Hash_File(strFileName, CFile::modeRead);

	CodeBlockIndexList.RemoveAll();
	CodeBlockNameList.RemoveAll();

	while( Hash_File.ReadString(strLine) ) {
		pos = strLine.Find(';');
		if( pos == -1 ) continue;
		strName = strLine.Left(pos);
		index = atoi(strLine.Mid(pos+1));
		if( index != -1 ) {
			CodeBlockIndexList.Add(index);
			CodeBlockNameList.Add(strName);
		}
	}
	Hash_File.Close();
}



CString	C__objects_hash::isCodeBlock(unsigned short Index)
{
	int			size, i;

	size = CodeBlockIndexList.GetSize();
	for( i = 0; i < size; i++ )
		if( Index == CodeBlockIndexList[i] ) 
			return CodeBlockNameList[i] + CString(".dbp");
			
	return CString("");
}

/* ---------------------------------------------------------------------------- */
