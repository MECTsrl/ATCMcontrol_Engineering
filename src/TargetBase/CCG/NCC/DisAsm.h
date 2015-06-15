/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/NCC/DisAsm.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: DisAsm.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/NCC/DisAsm.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / CCG
 *
 * =COMPONENT			NCC
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

#ifndef _DISASM_H_
#define _DISASM_H_

class CDisAsm  
{
static const char Mnemonic[256][7];
static const unsigned char Mode[256];

static unsigned short	SWAP8(unsigned short x) {return ((x & 0x00F0) >> 4) | ((x & 0x000F) << 4);}
static unsigned short	SWAP16(unsigned short x) {return ((x & 0xFF00) >> 8) | ((x & 0x00FF) << 8);}
static unsigned char	HN(unsigned char x) {return x >> 4;}
static unsigned char	LN(unsigned char x) {return x&0x0F;}

static CString			Reg(int nr, int rword);
static CString			RReg(int nr);
static const char*		CC(int nr);
static int				getSize(unsigned short Operator, unsigned short Operand);
static CString			Decode (unsigned short Operator, unsigned short Operand);
public:
		CDisAsm();
		virtual ~CDisAsm();

static CString			Format( unsigned char* code, unsigned short int len, const char* blank, unsigned long adr);
static void             test(CFile& f, CStdioFile& l, int) {}
};

#endif

/* ---------------------------------------------------------------------------- */
