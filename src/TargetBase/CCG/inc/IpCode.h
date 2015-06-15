/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/inc/IpCode.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: IpCode.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/inc/IpCode.h $
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

#if !defined(AFX_IPCODE_H__F3D51FE3_28FF_11D5_A05C_0050BF490551__INCLUDED_)
#define AFX_IPCODE_H__F3D51FE3_28FF_11D5_A05C_0050BF490551__INCLUDED_

union Parameter {
	unsigned char   ui1;
	         char	i1;
	unsigned short  ui2;
	         short  i2;
	unsigned long   ui4;
	         long   i4;
    unsigned short  us8[4];
	unsigned char	uc8[8];
};

class CIpCodeElem : public COpcode {
    /* Elemente der Basisklasse; nur Hint */
    /*
    const    char* OpcodeString;
    unsigned short OpCode;      // Opcode: 1 Byte Codes im low-Teil; 2 Byte: 1.Byte im high-Teil 
    unsigned char  live;        // free - ip - ncc 
    unsigned char  type;        // 
    unsigned char  parCount;    // number of parameter (in ip-stream)
    unsigned char  dataCount;   // size of operands in bits
    */
public:
	// Code
	unsigned short	isNative;	// 0 = ip, 1 = nc
	unsigned short	Praefix;	// Präfix: 0 = Ohne
	unsigned short	ParSize;	// Länge eines Parameters in Byte, gilt auch für Stackpräfix
	Parameter		Par1;		// Parameter 1
	Parameter		Par2;		// Parameter 2
	Parameter		Par3;		// Parameter 3
	unsigned short	nNccCode;	// Befehlslänge
    unsigned char	NccCode[CBlob::BlobSize+32];	// IpCode
	unsigned short	nNccIpAsm;	// Länge der Umschaltung zu NC
	unsigned char*	pSTRING;	// Speicher für Stringkonstante
	// jmp-Label 
	unsigned long	offset;		// Code-Offset im Modul 
	long			label;		// Label-Nummer ab 1    : Label-Nummer ab 1
	long			jmpto;		// rel. Sprungweite		: Label-Nummer; 0 = kein Sprung
	// Breaks
	unsigned long	BpIndex;	// Index in Datei der Haltepunkte; -1 = ohne
    unsigned long   BpNumber;   // zugehörige Breakpointnummer
    // Segment
    unsigned long   seg_size;   // Länge der Segmentkorrektur


	CIpCodeElem();
	CIpCodeElem(CIpCodeElem&);
	virtual		~CIpCodeElem();
    CIpCodeElem& operator=(const CIpCodeElem&);
    // Textzeile aus Code generieren
	virtual		CString	Format(unsigned long ulLoadAddress = 0, char BreakPointType = ' ');	
};

typedef CArray<CIpCodeElem, CIpCodeElem> CIpCodeList;

#endif

/* ---------------------------------------------------------------------------- */
