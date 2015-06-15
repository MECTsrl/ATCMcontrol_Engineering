/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/inc/xxx_dbp.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: xxx_dbp.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/inc/xxx_dbp.h $
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

#if !defined(AFX_XXX_DBP_H__CEBEBAC2_2A64_11D5_A05C_0050BF490551__INCLUDED_)
#define AFX_XXX_DBP_H__CEBEBAC2_2A64_11D5_A05C_0050BF490551__INCLUDED_

struct CBreakElem {
	int		stLineNumber;
	int		ipLineNumber;
	int		isNative;
    char    cTyp;  // I: BP VM opcode, X: begin of statement but no BP VM opcode
};

typedef CArray<CBreakElem, CBreakElem> CBreakList;

class Cxxx_dbp  
{
	CString		    strFileName;
	CBreakList	    BreakList;
    CMapPtrToPtr    BreakMap;
	int             trashBreakpoints;

public:
	Cxxx_dbp(CString& FileName, int trashBreakpoints);
	virtual ~Cxxx_dbp();

	void	read(void);
	void	write(int iDebugNames);
	int	    isBreakpoint(int CodeOffset);
    void    update(int index, int LineOffset = 1);
	CBreakElem& operator[](int index) {return BreakList.ElementAt(index);}
    CString& getFileName() { return strFileName; }
};

#endif

/* ---------------------------------------------------------------------------- */
