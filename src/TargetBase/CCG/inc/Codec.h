/* $Header: /4CReleased/V2.20.00/TargetBase/CCG/inc/Codec.h 1     28.02.07 19:04 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Codec.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/CCG/inc/Codec.h $
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

#if !defined(AFX_CODEC_H__D6EFE7E1_2DA8_11D5_A05C_0050BF490551__INCLUDED_)
#define AFX_CODEC_H__D6EFE7E1_2DA8_11D5_A05C_0050BF490551__INCLUDED_

/* LN010813: Hack: Interpreter configuration manually copied from intDef.h ($TODO$)
 */
#define IP_CFG_WIDE16       /* IP can process wide16 prefix */
#define IP_CFG_REG32        /* 32 bit working register      */
#define IP_CFG_IDX16        /* 16 bit index in object map   */
#define IP_CFG_PTR32        /* 32 bit index in object map   */
#define IP_CFG_AIS16        /* 16 bit index in object map   */

// Ausrichtung eintragen
// Byte:  1
// Word:  2
// DWord: 4
extern int code_allignment;

// is target big-endian?
extern const int big_endian;

#if   defined(IP_CFG_IDX8)
 const int	sizeID			=  1;	//  sizeof(Vector_Index)
#elif defined(IP_CFG_IDX16)
 const int	sizeID			=  2;	
#elif defined(IP_CFG_IDX32)
 const int	sizeID			=  4;	
#elif defined(IP_CFG_IDX64)
 const int	sizeID			=  8;	
#endif

#if   defined(IP_CFG_AIS8)
 const int	sizeRANGE		=  1;	//  sizeof(Vector_Range)
#elif defined(IP_CFG_AIS16)
 const int	sizeRANGE		=  2;
#elif defined(IP_CFG_AIS32)
 const int	sizeRANGE		=  4;
#elif defined(IP_CFG_AIS64)
 const int	sizeRANGE		=  8;
#endif


// maximale im NCC verarbeite Akku-Wortbreite in Bits
extern const int	bitsizeVM;


const int   CMP_GT         =  1,
            CMP_GE         =  2,
            CMP_EQ         =  3,
            CMP_LE         =  4,
            CMP_LT         =  5,
            CMP_NE         =  6;

const int   JMP_UC         =  0,
            JMP_C          =  1,
            JMP_CN         =  2;

const int   fbm            =  0,
            fbmid          =  1,
            fbsys          =  2,
            fbsysid        =  3,
            fb             =  4,
            fbid           =  5,
            fn             =  4,
            fnm            =  5,
            fnsys          =  6,
            fret           = 15;

class CCodec  
{
friend class CParser;

static int  jmp_relative,     // generate relative jmps        // $LN$
            sizeJmpCode,      // size of jmp sequence in byte
            sizeJmpCCode,     // size of cond jmp sequence in byte
            JmpOffset,        // length of jmp code (opcode+par+trap) in byte
            StartJmpCode,     // ?
            StartJmpCCode,    // ?
            sizeAsmIp,        // size of ncc -> ip switch back
            sizeIpAsm,        // size of ip  -> ncc switch to
            maxRelJmp,        // maximale relative positive Sprungweite
            stackAlignment,
            instAlignment;
static unsigned long 
            firmwareVersion;  // firmware version
static int  maxCalls,         // max function/function_block calls
            maxIndexEntries,  // max index entries
            maxObjectEntries; // max object entries

static int	iList;		      // Flag Listoption
public:
    
    CCodec();
	virtual ~CCodec();

	void	Init(void);
	static  int   checkSize(unsigned long ulSize, unsigned long m_LoadAddress);
	static	CBlob	ASM_IP(int rc = 0);
    static  CBlob   IP_ASM(int cc = 0);
	static	CBlob	ASM_NOP(void);

	// Referenz/Load/Store
    CBlob   LoadSelector(unsigned short seg, unsigned short off);
	CBlob	SelectInstance(unsigned short idx);
	CBlob	LoadReference(		  unsigned short seg);
	CBlob	LoadAdrRegReference(unsigned short Par1);
	CBlob	LoadInstanceReference(unsigned short seg, unsigned long Par1);
	CBlob	LoadVectorReference(  unsigned short seg, unsigned long Par1, int Size, unsigned char elemSize, bool akku);

    CBlob	LoadAkkuIm(		Parameter Par      , int Size, int ftype);
	CBlob	LoadAkkuInd(	unsigned char dtype, unsigned long Par1, int Size, int ftype);
	CBlob	LoadAkkuAdrReg(	unsigned char dtype, unsigned long Par1, int Size, int ftype);
	CBlob	LoadAkkuAdrRegV(unsigned char dtype, int ftype);
	CBlob   LoadAkkuAdr(	unsigned short seg,  unsigned long Par1, bool isBoolAdr);

	CBlob	LoadRegIm(		Parameter Par      , int Size, int ftype);
	CBlob	LoadRegInd(		unsigned char dtype, unsigned long Par1, int Size, int ftype);
	CBlob	LoadRegAdrReg(	unsigned char dtype, unsigned long Par1, int Size, int ftype);

	CBlob	LoadBitAdr(		unsigned long Par1, bool akku);
	CBlob	StoreBitAdr(	unsigned long Par1, bool akku);

	CBlob	StoreAkkuInd(	unsigned char dtype, unsigned long Par1, int Size, int ftype);
	CBlob	StoreAkkuAdrReg(unsigned char dtype, unsigned long Par1, int Size, int ftype);
	CBlob	StoreAkkuAdrRegV(unsigned char dtype, int ftype);
	
	CBlob	StoreRegAdrReg( unsigned char dtype, unsigned long Par1, int Size, int ftype);
	CBlob	StoreAdr(		unsigned long Par1, bool akku);

	// Load/Store-temporary
	CBlob	Store(		    unsigned char dataCount, unsigned long Par1, int ftype);
	CBlob	Restore(		unsigned char dataCount, unsigned long Par1, bool op2, int ftype);
    CBlob   Swap(		    unsigned char dataCount, int ftype);
    CBlob   Move(		    unsigned char dataCount, int ftype);
    CBlob   ReMove(         unsigned char dataCount, int ftype);
    CBlob   i2f(            int ftype, bool akku);

	
    // Konvertierungen
	CBlob	ConvINTS2BOOL(  unsigned char stype);
	CBlob	ConvBOOL2INTS(  unsigned char stype);
	CBlob	ConvINTS2INTS(  unsigned char stype, unsigned char dtype);
	CBlob	ConvALL2REAL (  unsigned char stype, unsigned char dtype);
	CBlob	ConvREAL2ALL (  unsigned char stype, unsigned char dtype);
	CBlob	ConvREAL2REAL(  unsigned char stype, unsigned char dtype);

	// Operationen mit einem Operand
	CBlob	Abs(			unsigned char dtype);
	CBlob	Neg(			unsigned char dtype);
	CBlob	Not(			unsigned char dtype);
	
	// Operationen mit zwei Operanden
	CBlob	Add(			unsigned char dtype);
	CBlob	And(			unsigned char dtype);
	CBlob	Cmp(			unsigned char dtype, int CMPtype);
	CBlob	Div(			unsigned char dtype);
	CBlob	Mod(			unsigned char dtype);
	CBlob	Mul(			unsigned char dtype);
	CBlob	Or (			unsigned char dtype);
	CBlob	Pow(			unsigned char dtype, unsigned char etype);
	CBlob	Rol(			unsigned char dtype);
	CBlob	Ror(			unsigned char dtype);
	CBlob	Shl(			unsigned char dtype);
	CBlob	Shr(			unsigned char dtype);
	CBlob	Sub(			unsigned char dtype);
	CBlob	Xor(			unsigned char dtype);

	// Ablaufsteuerung
	CBlob	Jmp(			unsigned int  cond, unsigned long Adr = 0);
    CBlob   calFSM (        unsigned int  smid, unsigned long opC, unsigned long off);
    CBlob   calFBSM(        unsigned int  smid, unsigned long opC, unsigned long off);
    CBlob   calFB  (        unsigned int  smid, unsigned long opC, unsigned long off);
    CBlob   calF   (        unsigned int  smid, unsigned long opC, unsigned long off);
    CBlob   Return (        void);
    CBlob   Leave  (        unsigned long count);

    // function called befor a new statement is generated
    CBlob   OnStatement(bool bCheckExec);

   static void  setJmpAdr(unsigned char* pCode, unsigned long abs_adr, bool bBack);
};

// from old intOpcds.h
#define cBOOL           0x00
#define cUSINT          0x01
#define cSINT           0x02
#define cBYTE           0x03
#define cUINT           0x04
#define cINT            0x05
#define cWORD           0x06
#define cUDINT          0x07
#define cDINT           0x08
#define cDWORD          0x09
#define cREAL           0x0A
#define cSTR            0x0B
#define cULINT          0x0C
#define cLINT           0x0D
#define cLWORD          0x0E
#define cLREAL          0x0F

#define cVECT32         0xFE
#define cVECT64         0xFF

#define cDT             0x00
#define cDATE           0x01
#define cTOD            0x02
#define cTIME           0x03

// Option
const int   option_Names        =   0x0001,
            option_IpList       =   0x0002,
            option_NccList      =   0x0004,
            option_ReAsm        =   0x0008,
            option_Praef64      =   0x0010,
            option_BaseAdr      =   0x0020,
            option_WriteIOFlags =   0x0040,
            option_CheckFreeze  =   0x0080,
            option_Intern       =   0x2000, 
            option_nologo       =   0x4000,
            option_debug        =   0x8000;


#define SEG_INSTANCE		0xFFFFu			/* instance segment */
#define SEG_GLOBALDATA   	0xFFFEu			/* instance segment */

CString	CDisAsm_Format( unsigned char* code, unsigned short int len, const char* blank, unsigned long adr);	// $LN$
// Verbindung ncc.cpp mit nccbase.cpp
void help();
void logo();
extern char* validParameter;

#endif

/* ---------------------------------------------------------------------------- */
