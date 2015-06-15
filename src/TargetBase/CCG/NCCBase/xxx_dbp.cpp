/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/NCCBase/xxx_dbp.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: xxx_dbp.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/NCCBase/xxx_dbp.cpp $
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
#include "xxx_dbp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

Cxxx_dbp::Cxxx_dbp(CString& FileName, int tb)
{
	strFileName      = FileName;
	trashBreakpoints = tb;
}

Cxxx_dbp::~Cxxx_dbp()
{
	BreakList.RemoveAll();
    BreakMap.RemoveAll();
}

void Cxxx_dbp::read(void)
{
	CString		strLine;
	CBreakElem	BreakElem;
	CStdioFile	Break_File(strFileName, CFile::modeRead);
	int		    LastipLineNumber = -1;

	BreakElem.isNative = 0;
	while( Break_File.ReadString(strLine) ) 
    {
        if( sscanf(strLine, "%d;%d;%c", &BreakElem.stLineNumber, &BreakElem.ipLineNumber, &BreakElem.cTyp) != 3 ) {
            ASSERT(_ASSERT_);
			throw CFormatException("xxx_dbp");
        }
        if( BreakElem.cTyp != 'I' && BreakElem.cTyp != 'X' ) {
            ASSERT(_ASSERT_);
			throw CFormatException("xxx_dbp");
        }
		BreakList.Add(BreakElem);
        if (LastipLineNumber == BreakElem.ipLineNumber)
        {
            CString str;
            str.Format("- can't process duplicate breakpoint offsets");
            ASSERT(_ASSERT_);
            throw CFormatException(str);
        } else {
            LastipLineNumber = BreakElem.ipLineNumber;
            // MapIpOffset mit Key = ipLineNumber und Value = Index in BreakList
            BreakMap.SetAt((void*)(BreakElem.ipLineNumber), (void*)(BreakList.GetUpperBound()));
        }
	}
	Break_File.Close();
}

void Cxxx_dbp::write(int iDebugNames)
{
	CString		name;

	name = strFileName;
	if( iDebugNames ) name += CString('_');
	CStdioFile	Break_File(name, iDebugNames?CFile::modeWrite|CFile::modeCreate:CFile::modeWrite|CFile::modeCreate);
	int			index, size;
	CString		str;

	for(size = BreakList.GetSize(), index = 0; index < size; index++) {
        const CBreakElem& bpe = BreakList[index];
        char  cgNewTyp  = bpe.cTyp;
        //set the type info to N(=Native) but only if a real User Breakpoint position
        if(cgNewTyp == 'I' && bpe.isNative)
            cgNewTyp = 'N';
		if(trashBreakpoints && cgNewTyp == 'I')
			cgNewTyp = 'X';
		str.Format("%d;%d;%C\n", bpe.stLineNumber, bpe.ipLineNumber, cgNewTyp);
		Break_File.WriteString( str );
	}
	Break_File.Close();
}

int Cxxx_dbp::isBreakpoint(int CodeOffset)
{
    void*   pvalue;

    if (BreakMap.Lookup((void*)CodeOffset, pvalue)) {
        return (int)pvalue;
    }

	return -1;
}

void Cxxx_dbp::update(int index, int LineOffset)
{
    BreakMap.RemoveKey((void*)(BreakList[index].ipLineNumber));
    BreakList[index].ipLineNumber += LineOffset;
    // MapIpOffset mit Key = ipLineNumber und Value = Index in BreakList
    BreakMap.SetAt((void*)(BreakList[index].ipLineNumber), (void*)index);
}

/* ---------------------------------------------------------------------------- */
