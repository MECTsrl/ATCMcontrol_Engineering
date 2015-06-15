/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/inc/NccCode.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: NccCode.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/inc/NccCode.h $
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

#if !defined(AFX_NCCCODE_H__5D3E8E41_2CD3_11D5_A05C_0050BF490551__INCLUDED_)
#define AFX_NCCCODE_H__5D3E8E41_2CD3_11D5_A05C_0050BF490551__INCLUDED_

#include "IpCode.h"


/*
	Ncc				Ip
	LOAD		<-	PUSH
	STORE		<-	POP

	Operation mit 1 Operanden: OperatorCode enthält Operation; Opcode = 0;
	Operation mit 2 Operanden: OperatorCode enthält Operation; Opcode = Ladebefehl;
	sonst                    : OperatorCode = 0;               Opcode = Befehl;

*/
class CNccCodeElem : public CIpCodeElem  
{
public:
    unsigned short  swapFlag;
	unsigned short	OperatorCode;
	Parameter		OperatorParam;
	int				url;
	int				h;	    // Hilfsspeicherindex; -1 = keiner
    int             ftype;  // 0 = integer, cREAL, cLREAL
    int             swapftype;  // 0 = integer, cREAL, cLREAL; nur für swap-Operand
    int             exchange;   // 1 = I-Akku -> F-Akku

	CNccCodeElem();
    CNccCodeElem& operator=(const CIpCodeElem&);
	CNccCodeElem(CNccCodeElem&);
    CNccCodeElem& operator=(const CNccCodeElem&);
	virtual ~CNccCodeElem();

	virtual	CString	Format(bool disasm, bool startnative, unsigned long ulLoadAddress=0);	// Textzeile(n) aus Code generieren
	int				AddOpcode(CBlob Blob);
};

typedef CArray<CNccCodeElem, CNccCodeElem> CNccCodeList;

#endif

/* ---------------------------------------------------------------------------- */
