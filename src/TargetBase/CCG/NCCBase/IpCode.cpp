/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/NCCBase/IpCode.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: IpCode.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/NCCBase/IpCode.cpp $
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
#include "IpCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////
CIpCodeElem::CIpCodeElem()
{
    // Basisklasse hier mit Initialisieren
    // Damit die Tabellen ohne Konstruktor angelegt werden.
    // COpCode
    OpcodeString= NULL;
    OpCode      = 0;    
    live        = 0;
    type        = 0;
    parCount    = 0;
    dataCount   = 0;
    // CIpCodeElem
	isNative	= 0;	
	Praefix		= 0;	
	ParSize		= 1;	
	memset(Par1.uc8, 0, sizeof(Par1));
	memset(Par2.uc8, 0, sizeof(Par2));
	memset(Par3.uc8, 0, sizeof(Par3));
	nNccCode    = 0;	
    memset(NccCode,  0, sizeof(NccCode));
	nNccIpAsm   = 0;	
	pSTRING		= NULL;	// Speicher für Stringkonstante
	// jmp-Label 
	offset      = 0;
	label       = 0;
	jmpto		= 0;	// Sprungziel; 0 = kein Sprung
	// Breaks
	BpIndex		= -1;	// Index in Datei der Haltepunkte; -1 = ohne
    BpNumber    = 0;
    // Segment
    seg_size    = 0;
}

CIpCodeElem::CIpCodeElem(CIpCodeElem& e)
{
    // COpCode
    OpcodeString = e.OpcodeString;  // da const
    OpCode       = e.OpCode;      
    live         = e.live;
    type         = e.type;
    parCount     = e.parCount; 
    dataCount    = e.dataCount;
    // CIpCodeElem
    isNative     = e.isNative;
    Praefix      = e.Praefix;  
    ParSize      = e.ParSize;  
    nNccCode     = e.nNccCode;
    nNccIpAsm    = e.nNccIpAsm;
	// jmp-Label 
    offset       = e.offset;
    label        = e.label;
    jmpto        = e.jmpto;
	// Breaks
    BpIndex      = e.BpIndex;  
    BpNumber     = e.BpNumber;  
    // Segment
    seg_size     = e.seg_size;

	memcpy(Par1.uc8, e.Par1.uc8, sizeof(Par1));
	memcpy(Par2.uc8, e.Par2.uc8, sizeof(Par1));
	memcpy(Par3.uc8, e.Par3.uc8, sizeof(Par1));
	memcpy(NccCode,  e.NccCode,  sizeof(NccCode));

	if( e.pSTRING ) {
		pSTRING = new unsigned char[e.pSTRING[0]+2];
		memcpy(pSTRING, e.pSTRING, e.pSTRING[0]+2);
	} else
		pSTRING = NULL;
}

CIpCodeElem& CIpCodeElem::operator=(const CIpCodeElem& e)
{
    if( this == &e ) return *this;

    // COpCode
    OpcodeString = e.OpcodeString;  // da const
    OpCode       = e.OpCode;      
    live         = e.live;
    type         = e.type;
    parCount     = e.parCount; 
    dataCount    = e.dataCount;
    // CIpCodeElem
    isNative     = e.isNative;
    Praefix      = e.Praefix;  
    ParSize      = e.ParSize;  
    nNccCode     = e.nNccCode;
    nNccIpAsm    = e.nNccIpAsm;
	// jmp-Label 
    offset       = e.offset;
    label        = e.label;
    jmpto        = e.jmpto;
	// Breaks
    BpIndex      = e.BpIndex;  
    BpNumber     = e.BpNumber;  
    // Segment
    seg_size     = e.seg_size;

	memcpy(Par1.uc8, e.Par1.uc8, sizeof(Par1));
	memcpy(Par2.uc8, e.Par2.uc8, sizeof(Par1));
	memcpy(Par3.uc8, e.Par3.uc8, sizeof(Par1));
	memcpy(NccCode,  e.NccCode,  sizeof(NccCode));


	if( e.pSTRING ) {
        if( pSTRING ) delete[] pSTRING;
		pSTRING = new unsigned char[e.pSTRING[0]+2];
		memcpy(pSTRING, e.pSTRING, e.pSTRING[0]+2);
	} else
		pSTRING = NULL;

   return *this;
}


CIpCodeElem::~CIpCodeElem()
{
	if( pSTRING ) delete[] pSTRING;
	pSTRING = NULL;
}

CString	CIpCodeElem::Format(unsigned long ulLoadAddress, char BreakPointType)
{
	CString			str, line, bline;
	unsigned short	i;

	if( BpIndex != -1 ) {
        // Versteckten Breaks ausschreiben
        if( BreakPointType == 'I' ) {
		    str.Format("%06X  BpNum=#%d        BREAK", offset+ulLoadAddress, BpNumber);
        } else {
            // virtueller Haltepunkt
		    str.Format("%06X  BpNum=#%d             ", offset+ulLoadAddress, BpNumber);
        }
		line += str;
        // BREAKs-Zeile ohne \n
        if( nNccCode ) 
            line += CString('\n');
	}

    // BREAK-Zeile beenden
    if( !nNccCode ) 
        return line;

	str.Format("%06X  ", offset+ulLoadAddress);
	line += str;
			
	// Source
   if (OpcodeString)
   {
	   str.Format("%-12s    ", OpcodeString);
   } else {
	   str.Format("%-12s    ", "?");
   }
	line += str;

	// Hexcode
	if( Praefix ) {
		str.Format("%2x", Praefix);
		line += str;
	}
	
	if( (OpCode & 0xFF00) != 0 ) {
		str.Format("%04x  ", OpCode);
		line += str;
	} else {
		str.Format("%02x  ", OpCode & 0x00FF);
		line += str;
	}
	// Parameter1
	if( parCount > 0 )
		for(i = 0; i < ParSize; i++) {
			str.Format("%02x", Par1.uc8[ParSize-i-1]);
			line += str;
		}
	line += CString("  ");
	if( parCount > 1 )
		for(i = 0; i < ParSize; i++) {
			str.Format("%02x", Par2.uc8[ParSize-i-1]);
			line += str;
		}
   return line;
}

/* ---------------------------------------------------------------------------- */
