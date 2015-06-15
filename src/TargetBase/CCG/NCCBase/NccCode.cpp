/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/NCCBase/NccCode.cpp 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: NccCode.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/NCCBase/NccCode.cpp $
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
#include "NccCode.h"
//#include "DisAsm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CNccCodeElem::CNccCodeElem()
{
    swapFlag        =  0;   // kein Swap
    swapftype       =  0;   // Typ des Swap-Operand
    OperatorCode    =  0;   // kein Operator
	memset(OperatorParam.uc8, 0, sizeof(OperatorParam));
	url             = -1;   // Name für dieses Ergebnis
    h               = -1;   // Hilfsspeicherindex; -1 = keiner
    ftype           =  0;   // integer
    exchange        =  0;   // no exchange
}


CNccCodeElem& CNccCodeElem::operator=(const CIpCodeElem& e)
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
        if( pSTRING ) delete[] pSTRING;
		pSTRING = new unsigned char[e.pSTRING[0]+2];
		memcpy(pSTRING, e.pSTRING, e.pSTRING[0]+2);
	} else
		pSTRING = NULL;

    return *this;
}

CNccCodeElem::~CNccCodeElem()
{
    ;
}

CNccCodeElem::CNccCodeElem(CNccCodeElem& e)
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

    // CNccCodeElem
    swapFlag        = e.swapFlag;
    swapftype       = e.swapftype;
    OperatorCode    = e.OperatorCode;
	memcpy(OperatorParam.uc8, e.OperatorParam.uc8, sizeof(OperatorParam));
    url             = e.url;
    h               = e.h;
    ftype           = e.ftype;
    exchange        = e.exchange;
}

CNccCodeElem& CNccCodeElem::operator=(const CNccCodeElem& e)
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

    // CNccCodeElem
    swapFlag    = e.swapFlag;
    swapftype   = e.swapftype;
    OperatorCode= e.OperatorCode;
	memcpy(OperatorParam.uc8, e.OperatorParam.uc8, sizeof(OperatorParam));
    url         = e.url;
    h           = e.h;
    ftype           = e.ftype;
    exchange    = e.exchange;

    return *this;
}

CString CNccCodeElem::Format(bool disasm, bool startnative, unsigned long ulLoadAddress)
{
    CString         str, line, blank;
    unsigned short  i, n, pos, offnative;

    if( !isNative ) {
        line = CIpCodeElem::Format(ulLoadAddress);
    } else {

        blank = CString("                        "); // 24 Leerzeichen
        if( BpIndex != -1 ) {
           str.Format("%06X  BpNum=#%d", offset+ulLoadAddress, BpNumber);
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
       

        pos = line.Find('\n');
        n   = line.GetLength();
        if( pos + 1 < n ) n -= pos+1;
        n = 8 - n;
        ASSERT( n < 8 );
        for(i = 0; i < n; i++)
           line += CString(' ');

        str.Format("Op=%04x ", OperatorCode);
        line += str;

        str.Format("h=%2d ", h);
        line += str;

        if( swapFlag ) 
         line += CString("s");
        else
         line += CString(" ");

        switch(ftype) {
        case 0      : line += CString("  "); break;
        case cREAL  : line += CString("f "); break;
        case cLREAL : line += CString("d "); break;
        case cVECT32: 
        case cVECT64: line += CString("v "); break;
        }

        if( pSTRING ) n = 2; else n = nNccCode;
        if(n > CBlob::BlobSize ) n = 0;  // für testzeit
        if( !disasm ) {
           for(i = 0; i < n; i++) {
               str.Format("%02X ", NccCode[i]);
               line += str;
               if( (i+1) % 16 == 0 ) {
                   // Neue Zeile; Code fortsetzen
                   line += CString("\n")+ blank;
               }
           }
       } else {
            offnative = 0;
            if( startnative ) {
                while( NccCode[offnative] == 0 ) {  //ASM_NOP
                   offnative++;
                   line += CString("NOP;");
                }
                offnative++;
                line += CString("NATIVE\n");
                str.Format("%06X                  ", offset+ulLoadAddress+offnative);
                line += str;
           }
           line += CDisAsm_Format( NccCode+offnative, nNccCode-offnative, blank, offset+ulLoadAddress+offnative);   // $LN$
       }
   }

   if( seg_size ) {
      str.Format("\n-------- Segment limit (+%03d Bytes) --------", seg_size);
      line += str;
   }
    
   return line;
}

int CNccCodeElem::AddOpcode(CBlob Blob)
{
    ASSERT(nNccCode + Blob.size < CBlob::BlobSize);
    memcpy(&NccCode[nNccCode], Blob.buffer, Blob.size);
    nNccCode += Blob.size;
    return 1;
}

/* ---------------------------------------------------------------------------- */
