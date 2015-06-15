/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Util.cpp 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: BAR_Util.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB/BAR_Util.cpp $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CBar
 *
 * =COMPONENT           bar
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *           $Revision: 1 $
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
 *  02.07.01  KT      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/



//=== c++ standard includes

//=== windows includes

//==== specific includes
#include "BAR_Util.hpp"

#include <atlbase.h>

#include <fcntl.h>
#include <io.h>
#include <ctype.h>

#include <time.h>

// Encyption defines
#define ENCRYPT_BLOCK_LEN               (8)
#define ENCRYPT_BUFFER_BLOCKS           (256)
#define ENCRYPT_BUFFER_SIZE             (ENCRYPT_BUFFER_BLOCKS*ENCRYPT_BLOCK_LEN)
#define ENCRYPT_HEADER_SIZE             (24)
#define ENCRYPT_SIGNATURE               _T("4CBRTool")
#define ENCRYPT_SIGNATURE_SIZE          (8)
#define ENCRYPT_MAX_KEYLEN				(8)
#define ENCRYPT_HELP_PASSWORD           "x´j}5k#q"

//=============================================================================*
bool ExistsFile(const string& sFileName)
//=============================================================================*
{   
    bool bFileExists = true;
    int fh = _open(sFileName.c_str(), _O_RDONLY);
    if( ( fh == -1 ) && ( errno == ENOENT ) )
    {
        bFileExists = false;
    }
    _close(fh);
    return bFileExists;
}

//=============================================================================*
bool ExistsDir(const string& sDirName)
//=============================================================================*
{   
    DWORD dwFileAttr = ::GetFileAttributes(sDirName.c_str());
    if ( dwFileAttr == -1 )
        return false;
    if ( dwFileAttr & FILE_ATTRIBUTE_DIRECTORY )
        return true;

    return false;
}

//=============================================================================*
string HRToString(HRESULT hr)
//=============================================================================*
{  
    ostringstream ossHResult;
    ossHResult << "hr=0x" << hex << hr;
    return ossHResult.str();
}

//=============================================================================*
wstring HRToWString(HRESULT hr)
//=============================================================================*
{  
    wostringstream ossHResult;
    ossHResult << L"hr=0x" << hex << hr;
    return ossHResult.str();
}

//=============================================================================*
string TrgToString(CSC_TRANSFER_TARGET trg)
//=============================================================================*
{  
    if ( trg == ProgramDomains )
        return "program";
    else if ( trg == VisualDomains )
        return "visual";
    else
        return "";

}

//=============================================================================*
string GetUpper(const string& s)
//=============================================================================*
{
    string sUpper = "";
    for(string::const_iterator p = s.begin();
        p != s.end();
        ++p)
    {
        sUpper += toupper(*p);
    }
    return sUpper;
}

//=============================================================================*
wstring ToWS(const string& s)
//=============================================================================*
{
    return wstring(CComBSTR(s.c_str()).m_str);
}


//=============================================================================*
string GUIDToString(const GUID& guid)
//=============================================================================*
{
   unsigned char* pszGuid = NULL;
   string sGUID = "";
   if ( ( RPC_S_OK == ::UuidToString((GUID*)&guid, &pszGuid ) ) &&
        ( pszGuid != NULL ) )
   {
       sGUID = (char*)pszGuid;
   }

   ::RpcStringFree(&pszGuid);
   return sGUID;
 }

////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------*/
/* PACK / UNPACK                                                        */
/*----------------------------------------------------------------------*/

/* Sofware DES functions
 * written 12 Dec 1986 by Phil Karn, KA9Q; large sections adapted from
 * the 1977 public-domain program by Jim Gillogly
 * Modified for additional speed - 6 December 1988 Phil Karn
 * Modified for parameterized key schedules - Jan 1991 Phil Karn
 * Callers now allocate a key schedule as follows:
 *	kn = (char (*)[8])malloc(sizeof(char) * 8 * 16);
 *	or
 *	char kn[16][8];
 */

//#define	NULL	0

#ifdef	LITTLE_ENDIAN
static long byteswap();
#endif
static void perminit(char perm[16][16][8], char p[64]);
static void permute(char *inblock, char perm[16][16][8], char *outblock);
static long f(register long r, register char *subkey);
static void spinit();

/* Tables defined in the Data Encryption Standard documents */

/* initial permutation IP */
static char ip[] = {
	58, 50, 42, 34, 26, 18, 10,  2,
	60, 52, 44, 36, 28, 20, 12,  4,
	62, 54, 46, 38, 30, 22, 14,  6,
	64, 56, 48, 40, 32, 24, 16,  8,
	57, 49, 41, 33, 25, 17,  9,  1,
	59, 51, 43, 35, 27, 19, 11,  3,
	61, 53, 45, 37, 29, 21, 13,  5,
	63, 55, 47, 39, 31, 23, 15,  7
};

/* final permutation IP^-1 */
static char fp[] = {
	40,  8, 48, 16, 56, 24, 64, 32,
	39,  7, 47, 15, 55, 23, 63, 31,
	38,  6, 46, 14, 54, 22, 62, 30,
	37,  5, 45, 13, 53, 21, 61, 29,
	36,  4, 44, 12, 52, 20, 60, 28,
	35,  3, 43, 11, 51, 19, 59, 27,
	34,  2, 42, 10, 50, 18, 58, 26,
	33,  1, 41,  9, 49, 17, 57, 25
};

/* expansion operation matrix
 * This is for reference only; it is unused in the code
 * as the f() function performs it implicitly for speed
 */
#ifdef notdef
static char ei[] = {
	32,  1,  2,  3,  4,  5,
	 4,  5,  6,  7,  8,  9,
	 8,  9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32,  1 
};
#endif

/* permuted choice table (key) */
static char pc1[] = {
	57, 49, 41, 33, 25, 17,  9,
	 1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36,

	63, 55, 47, 39, 31, 23, 15,
	 7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29,
	21, 13,  5, 28, 20, 12,  4
};

/* number left rotations of pc1 */
static char totrot[] = {
	1,2,4,6,8,10,12,14,15,17,19,21,23,25,27,28
};

/* permuted choice key (table) */
static char pc2[] = {
	14, 17, 11, 24,  1,  5,
	 3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

/* The (in)famous S-boxes */
static char si[8][64] = {
	/* S1 */
	14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
	 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
	 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
	15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,

	/* S2 */
	15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
	 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
	 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
	13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,

	/* S3 */
	10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
	13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
	13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
	 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,

	/* S4 */
	 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
	13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
	10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
	 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,

	/* S5 */
	 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
	14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
	 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
	11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,

	/* S6 */
	12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
	10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
	 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
	 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,

	/* S7 */
	 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
	13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
	 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
	 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,

	/* S8 */
	13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
	 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
	 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
	 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};

/* 32-bit permutation function P used on the output of the S-boxes */
static char p32i[] = {	
	16,  7, 20, 21,
	29, 12, 28, 17,
	 1, 15, 23, 26,
	 5, 18, 31, 10,
	 2,  8, 24, 14,
	32, 27,  3,  9,
	19, 13, 30,  6,
	22, 11,  4, 25
};
/* End of DES-defined tables */

/* Lookup tables initialized once only at startup by desinit() */
static long (*sp)[64];		/* Combined S and P boxes */

static char (*iperm)[16][8];	/* Initial and final permutations */
static char (*fperm)[16][8];


/* bit 0 is left-most in byte */
static int bytebit[] = {
	0200,0100,040,020,010,04,02,01
};

static int nibblebit[] = {
	 010,04,02,01
};
static int desmode;

/* Allocate space and initialize DES lookup arrays
 * mode == 0: standard Data Encryption Algorithm
 * mode == 1: DEA without initial and final permutations for speed
 */
int desinit(int mode)
{
	if(sp != NULL){
		/* Already initialized */
		return 0;
	}
	desmode = mode;
	
	sp = (long (*)[64])new long[8][64];
	if(!sp){
		return NULL;
	}
	spinit();
	if(mode == 1)	/* No permutations */
		return 0;

	iperm = (char (*)[16][8])new char[16][16][8];
	if(!iperm){
		delete [] sp;
		return -1;
	}
	perminit(iperm,ip);

	fperm = (char (*)[16][8])new char[16][16][8];
	if(!fperm){
		delete [] sp;
		delete [] iperm;
		return -1;
	}
	perminit(fperm,fp);
	
	return 0;
}

/* Free up storage used by DES */
void desdone()
{
	if(!sp)
		return;	/* Already done */

	delete [] sp;
	if(iperm)
		delete [] iperm;
	if(fperm)
		delete [] fperm;

	sp = NULL;
	iperm = NULL;
	fperm = NULL;
}

/* Set key (initialize key schedule array) */
int setkey(char (*kn)[8], const char* key)		
/* Key schedule, 64 bits (will use only 56) */
{
	char pc1m[56];		/* place to modify pc1 into */
	char pcr[56];		/* place to rotate pc1 into */
	register int i,j,l;
	int m;

	if(kn == NULL){
		return -1;
	}

	/* Clear key schedule */
	memset((char *)kn,0,16*8);

	for (j=0; j<56; j++) {		/* convert pc1 to bits of key */
		l=pc1[j]-1;				/* integer bit location	 */
		m = l & 07;				/* find bit		 */
		pc1m[j]= (char) ((key[l>>3] &	/* find which key byte l is in */
			bytebit[m])					/* and which bit of that byte */
			? 1 : 0);					/* and store 1-bit result */
	}
	for (i=0; i<16; i++) {		/* key chunk for each iteration */
		for (j=0; j<56; j++)	/* rotate pc1 the right amount */
			pcr[j] = pc1m[(l=j+totrot[i])<(j<28? 28 : 56) ? l: l-28];
			/* rotate left and right halves independently */
		for (j=0; j<48; j++){	/* select bits individually */
			/* check bit that goes to kn[j] */
			if (pcr[pc2[j]-1]){
				/* mask it in if it's there */
				l=j % 6;
				kn[i][j/6] |= bytebit[l] >> 2;
			}
		}
	}
	return 0;
}

/* In-place encryption of 64-bit block */
int endes(char (*kn)[8], char *block)
/* Key schedule */
{
	register long left,right;
	register char *knp;
	long work[2]; 		/* Working data storage */

	if(kn == NULL || block == NULL)
		return -1;
	permute(block,iperm,(char*)work);	/* Initial Permutation */
#ifdef	LITTLE_ENDIAN
	left = byteswap(work[0]);
	right = byteswap(work[1]);
#else
	left = work[0];
	right = work[1];
#endif

	/* Do the 16 rounds.
	 * The rounds are numbered from 0 to 15. On even rounds
	 * the right half is fed to f() and the result exclusive-ORs
	 * the left half; on odd rounds the reverse is done.
	 */
	knp = &kn[0][0];
	left ^= f(right,knp);
	knp += 8;
	right ^= f(left,knp);
	knp += 8;
	left ^= f(right,knp);
	knp += 8;
	right ^= f(left,knp);
	knp += 8;
	left ^= f(right,knp);
	knp += 8;
	right ^= f(left,knp);
	knp += 8;
	left ^= f(right,knp);
	knp += 8;
	right ^= f(left,knp);
	knp += 8;
	left ^= f(right,knp);
	knp += 8;
	right ^= f(left,knp);
	knp += 8;
	left ^= f(right,knp);
	knp += 8;
	right ^= f(left,knp);
	knp += 8;
	left ^= f(right,knp);
	knp += 8;
	right ^= f(left,knp);
	knp += 8;
	left ^= f(right,knp);
	knp += 8;
	right ^= f(left,knp);

	/* Left/right half swap, plus byte swap if little-endian */
#ifdef	LITTLE_ENDIAN
	work[1] = byteswap(left);
	work[0] = byteswap(right);
#else
	work[0] = right;
	work[1] = left;
#endif
	permute((char *)work,fperm,(char *)block);	/* Inverse initial permutation */
	return 0;
}

/* In-place decryption of 64-bit block. This function is the mirror
 * image of encryption; exactly the same steps are taken, but in
 * reverse order
 */
int dedes(char (*kn)[8], char *block)
/* Key schedule */
{
	register long left,right;
	register char *knp;
	long work[2];	/* Working data storage */

	if(kn == NULL || block == NULL)
		return -1;
	permute(block,iperm,(char *)work);	/* Initial permutation */

	/* Left/right half swap, plus byte swap if little-endian */
#ifdef	LITTLE_ENDIAN
	right = byteswap(work[0]);
	left = byteswap(work[1]);
#else
	right = work[0];
	left = work[1];
#endif
	/* Do the 16 rounds in reverse order.
	 * The rounds are numbered from 15 to 0. On even rounds
	 * the right half is fed to f() and the result exclusive-ORs
	 * the left half; on odd rounds the reverse is done.
	 */
	knp = &kn[15][0];
	right ^= f(left,knp);
	knp -= 8;
	left ^= f(right,knp);
	knp -= 8;
	right ^= f(left,knp);
	knp -= 8;
	left ^= f(right,knp);
	knp -= 8;
	right ^= f(left,knp);
	knp -= 8;
	left ^= f(right,knp);
	knp -= 8;
	right ^= f(left,knp);
	knp -= 8;
	left ^= f(right,knp);
	knp -= 8;
	right ^= f(left,knp);
	knp -= 8;
	left ^= f(right,knp);
	knp -= 8;
	right ^= f(left,knp);
	knp -= 8;
	left ^= f(right,knp);
	knp -= 8;
	right ^= f(left,knp);
	knp -= 8;
	left ^= f(right,knp);
	knp -= 8;
	right ^= f(left,knp);
	knp -= 8;
	left ^= f(right,knp);

#ifdef	LITTLE_ENDIAN
	work[0] = byteswap(left);
	work[1] = byteswap(right);
#else
	work[0] = left;
	work[1] = right;
#endif
	permute((char *)work,fperm,block);	/* Inverse initial permutation */
	return 0;
}

/* Permute inblock with perm */
static void permute(char *inblock, char perm[16][16][8], char *outblock)			
/* result into outblock,64 bits */
/* 2K bytes defining perm. */
{
	register char *ib, *ob;		/* ptr to input or output block */
	register char *p, *q;
	register int j;

	if(perm == NULL){
		/* No permutation, just copy */
		memcpy(outblock,inblock,8);
		return;
	}
	/* Clear output block */
	memset(outblock,0x00,8);

	ib = inblock;
	for (j = 0; j < 16; j += 2, ib++) { /* for each input nibble */
		ob = outblock;
		p = perm[j][(*ib >> 4) & 0xf];
		q = perm[j + 1][*ib & 0xf];
		/* and each output byte, OR the masks together */
		*ob++ |= *p++ | *q++;
		*ob++ |= *p++ | *q++;
		*ob++ |= *p++ | *q++;
		*ob++ |= *p++ | *q++;
		*ob++ |= *p++ | *q++;
		*ob++ |= *p++ | *q++;
		*ob++ |= *p++ | *q++;
		*ob++ |= *p++ | *q++;
	}
}

/* The nonlinear function f(r,k), the heart of DES */
static long f(register long r, register char *subkey)	
/* 32 bits */
/* 48-bit key for this round */
{
	register long *spp;
	register long rval,rt;
	register int er;

#ifdef	TRACE
	_tprintf(_T("f(%08lx, %02x %02x %02x %02x %02x %02x %02x %02x) = "),
		r,
		subkey[0], subkey[1], subkey[2],
		subkey[3], subkey[4], subkey[5],
		subkey[6], subkey[7]);
#endif
	/* Run E(R) ^ K through the combined S & P boxes.
	 * This code takes advantage of a convenient regularity in
	 * E, namely that each group of 6 bits in E(R) feeding
	 * a single S-box is a contiguous segment of R.
	 */
	subkey += 7;

	/* Compute E(R) for each block of 6 bits, and run thru boxes */
	er = ((int)r << 1) | ((r & 0x80000000) ? 1 : 0);
	spp = &sp[7][0];
	rval = spp[(er ^ *subkey--) & 0x3f];
	spp -= 64;
	rt = (unsigned long)r >> 3;
	rval |= spp[((int)rt ^ *subkey--) & 0x3f];
	spp -= 64;
	rt >>= 4;
	rval |= spp[((int)rt ^ *subkey--) & 0x3f];
	spp -= 64;
	rt >>= 4;
	rval |= spp[((int)rt ^ *subkey--) & 0x3f];
	spp -= 64;
	rt >>= 4;
	rval |= spp[((int)rt ^ *subkey--) & 0x3f];
	spp -= 64;
	rt >>= 4;
	rval |= spp[((int)rt ^ *subkey--) & 0x3f];
	spp -= 64;
	rt >>= 4;
	rval |= spp[((int)rt ^ *subkey--) & 0x3f];
	spp -= 64;
	rt >>= 4;
	rt |= (r & 1) << 5;
	rval |= spp[((int)rt ^ *subkey) & 0x3f];
#ifdef	TRACE
	_tprintf(_T(" %08lx\n"),rval);
#endif
	return rval;
}
/* initialize a perm array */
static void perminit(char perm[16][16][8], char p[64])
/* 64-bit, either init or final */
{
	register int l, j, k;
	int i,m;

	/* Clear the permutation array */
	memset((char *)perm,0,16*16*8);

	for (i=0; i<16; i++)		/* each input nibble position */
		for (j = 0; j < 16; j++)/* each possible input nibble */
		for (k = 0; k < 64; k++)/* each output bit position */
		{   l = p[k] - 1;	/* where does this bit come from*/
			if ((l >> 2) != i)  /* does it come from input posn?*/
			continue;	/* if not, bit k is 0	 */
			if (!(j & nibblebit[l & 3]))
			continue;	/* any such bit in input? */
			m = k & 07;	/* which bit is this in the byte*/
			perm[i][j][k>>3] |= bytebit[m];
		}
}

/* Initialize the lookup table for the combined S and P boxes */
static void spinit()
{
	char pbox[32];
	int p,i,s,j,rowcol;
	long val;

	/* Compute pbox, the inverse of p32i.
	 * This is easier to work with
	 */
	for(p=0;p<32;p++){
		for(i=0;i<32;i++){
			if(p32i[i]-1 == p){
				pbox[p] = (char) i;
				break;
			}
		}
	}
	for(s = 0; s < 8; s++){			/* For each S-box */
		for(i=0; i<64; i++){		/* For each possible input */
			val = 0;
			/* The row number is formed from the first and last
			 * bits; the column number is from the middle 4
			 */
			rowcol = (i & 32) | ((i & 1) ? 16 : 0) | ((i >> 1) & 0xf);
			for(j=0;j<4;j++){	/* For each output bit */
				if(si[s][rowcol] & (8 >> j)){
				 val |= 1L << (31 - pbox[4*s + j]);
				}
			}
			sp[s][i] = val;

//#ifdef	DEBUG
//			_tprintf(_T("sp[%d][%2d] = %08lx\n"),s,i,sp[s][i]);
//#endif
		}
	}
}
#ifdef	LITTLE_ENDIAN
/* Byte swap a long */
static long
byteswap(x)
unsigned long x;
{
	register char *cp,tmp;

	cp = (char *)&x;
	tmp = cp[3];
	cp[3] = cp[0];
	cp[0] = tmp;

	tmp = cp[2];
	cp[2] = cp[1];
	cp[1] = tmp;

	return x;
}
#endif

/*----------------------------------------------------------------------*/

BOOL EncryptBlock(char* lpszBuffer, const char* lpszKey)
{
	char ks[16][8];

	desdone();
	desinit(0);
	setkey(ks,lpszKey);
	
	endes(ks,lpszBuffer);

	desdone();		// memleak
	return TRUE;
}

/*----------------------------------------------------------------------*/

BOOL DecryptBlock(char* lpszBuffer, const char* lpszKey)
{
	char ks[16][8];

	desdone();
	desinit(0);
	setkey(ks,lpszKey);
	
	dedes(ks,lpszBuffer);

	desdone();		// memleak
	return TRUE;
}


/*----------------------------------------------------------------------*/

E_EncryptErr Pack(const char* lpszUnpackedFile, const char* lpszPackedFile, const char* lpszKey)
{
    // open files
    HANDLE   hFileIn = NULL;
    HANDLE   hFileOut = NULL;

    hFileIn = ::CreateFile(lpszUnpackedFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if(hFileIn == INVALID_HANDLE_VALUE)
    {
        return eEE_OpenFile;
    }
    hFileOut = ::CreateFile(lpszPackedFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFileOut == INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(hFileIn);
        return eEE_CreateFile;
    }

    // initialisation
	DWORD dwWrite=0;
	DWORD dwRead=0;

    // calculate encryption key by encrypt character key with fix help key
	char	 szKey[ENCRYPT_MAX_KEYLEN];
	char     ks[16][8];

    ::memset(szKey, 0 , ENCRYPT_MAX_KEYLEN);
    strncpy(szKey, lpszKey, ENCRYPT_MAX_KEYLEN);

	desdone();
	desinit(0);
	setkey(ks, ENCRYPT_HELP_PASSWORD);
    endes(ks,szKey);
	desdone();
	desinit(0);
	setkey(ks,szKey);

	// get size of unpacked file (without keycode ...)
    BY_HANDLE_FILE_INFORMATION  tFileInfo;
    if(!GetFileInformationByHandle(hFileIn, &tFileInfo))
    {
		desdone();
        ::CloseHandle(hFileIn);
        ::CloseHandle(hFileOut);
        return eEE_ReadFile;
    }
    
    __int64 i64_TextSize = (tFileInfo.nFileSizeHigh<<32) + tFileInfo.nFileSizeLow;

	if(i64_TextSize == 0)
	{
		desdone();
        ::CloseHandle(hFileIn);
        ::CloseHandle(hFileOut);
        return eEE_ReadFile;
    }

    // construct file header in memory
    char    szHeader[ENCRYPT_HEADER_SIZE];
	char	szBuf[ENCRYPT_BLOCK_LEN];
	char*   lpszTmp    = szHeader;

    DWORD   dwWritten;
    int     iRand;
    int     iSize;

    // initialize random generator with time
    srand((unsigned int)time(NULL));
    iRand = rand()*rand();

    // add 4 bytes random number
    iSize = sizeof(iRand);
    ::memcpy(lpszTmp, &iRand, iSize);
    lpszTmp += iSize;

    // add 8 bytes signature
    iSize = ENCRYPT_SIGNATURE_SIZE;
    ::memcpy(lpszTmp, ENCRYPT_SIGNATURE, ENCRYPT_SIGNATURE_SIZE);
    lpszTmp += iSize;

    // add 8 bytes text size
    iSize = sizeof(i64_TextSize);
    ::memcpy(lpszTmp, &i64_TextSize, iSize);
    lpszTmp += iSize;
    
    // add 4 bytes random number
    iRand = rand()*rand();
    iSize = sizeof(iRand);
    ::memcpy(lpszTmp, &iRand, iSize);
    lpszTmp += iSize;

    // encrypt header
    lpszTmp = szHeader;
    for(int iCount = 0; iCount < 3; ++iCount)
    {
		::memcpy(szBuf, lpszTmp, ENCRYPT_BLOCK_LEN);
		endes(ks,szBuf);
        ::memcpy(lpszTmp, szBuf, ENCRYPT_BLOCK_LEN);

        lpszTmp += ENCRYPT_BLOCK_LEN;
    }

    // write header to output file
    iSize = ENCRYPT_HEADER_SIZE;
    if(!WriteFile(hFileOut, (void*) szHeader, iSize, &dwWritten, NULL) || dwWritten != iSize)
    {
		desdone();
        ::CloseHandle(hFileIn);
        ::CloseHandle(hFileOut);
        return eEE_WriteFile;
    }

    // encrypt input file and write to output file
    E_EncryptErr    eErr = eEE_Ok;
	int             iBufPos = 0;
	char*           lpszCharBuf = new char [ENCRYPT_BUFFER_SIZE];
    __int64         i64_PosText;

    if(!lpszCharBuf)
    {
		desdone();
        ::CloseHandle(hFileIn);
        ::CloseHandle(hFileOut);
        return eEE_NoMemory;
    }

	lpszCharBuf[0]=0;

    for (i64_PosText = 0; i64_PosText < i64_TextSize; i64_PosText += ENCRYPT_BUFFER_SIZE)
	{
		if(!ReadFile(hFileIn, (void*)lpszCharBuf, ENCRYPT_BUFFER_SIZE, &dwRead, NULL))
        {
            eErr = eEE_ReadFile;
            break;
        }
		
		for (dwWrite = 0; dwWrite < dwRead; dwWrite += ENCRYPT_BLOCK_LEN)
		{
			iBufPos = 0;

			if ((dwRead - dwWrite) < ENCRYPT_BLOCK_LEN)
            {
				iBufPos = (int) dwRead % ENCRYPT_BLOCK_LEN;
            }

			lpszTmp = lpszCharBuf+dwWrite;
			
			::memcpy(szBuf, lpszTmp, ENCRYPT_BLOCK_LEN);
			if (iBufPos)
            {
				::memset(szBuf+iBufPos, 0, ENCRYPT_BLOCK_LEN-iBufPos);
            }
			endes(ks,szBuf);
			::memcpy(lpszTmp, szBuf, ENCRYPT_BLOCK_LEN);
		}

		if (dwWrite>0)	// entered for-loop
		{
			// write Block to dest file
            if(!::WriteFile(hFileOut, (void*)lpszCharBuf, dwWrite, &dwWritten, NULL) || dwWritten != dwWrite)
            {
                eErr = eEE_WriteFile;
                break;
            }
		}
	}

	delete lpszCharBuf;

	desdone();

    ::CloseHandle(hFileIn);
    ::CloseHandle(hFileOut);

	return eErr;
}

/*----------------------------------------------------------------------*/

E_EncryptErr Unpack(const char* lpszPackedFile, const char* lpszUnpackedFile, const char* lpszKey)
{
    // open files
    HANDLE   hFileIn = NULL;
    HANDLE   hFileOut = NULL;

    hFileIn = ::CreateFile(lpszPackedFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if(hFileIn == INVALID_HANDLE_VALUE)
    {
        return eEE_OpenFile;
    }
    hFileOut = ::CreateFile(lpszUnpackedFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFileOut == INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(hFileIn);
        return eEE_CreateFile;
    }

    // initialisation
	DWORD dwWrite=0;
	DWORD dwRead=0;

    // calculate encryption key by encrypt character key with fix help key
	char	 szKey[ENCRYPT_MAX_KEYLEN];
	char     ks[16][8];

    ::memset(szKey, 0 , ENCRYPT_MAX_KEYLEN);
    strncpy(szKey, lpszKey, ENCRYPT_MAX_KEYLEN);

	desdone();
	desinit(0);
	setkey(ks, ENCRYPT_HELP_PASSWORD);
    endes(ks,szKey);
	desdone();
	desinit(0);
	setkey(ks,szKey);

    // read file header into memory
    char    szHeader[ENCRYPT_HEADER_SIZE];
	char	szBuf[ENCRYPT_BLOCK_LEN];
	char*   lpszTmp    = szHeader;
    __int64 i64_TextSize;

    DWORD   dwWritten;
    int     iSize;

    iSize = ENCRYPT_HEADER_SIZE;
    if(!ReadFile(hFileIn, (void*)szHeader, iSize, &dwRead, NULL) || dwRead != iSize)
    {
		desdone();
        ::CloseHandle(hFileIn);
        ::CloseHandle(hFileOut);
        return eEE_ReadFile;
    }

    // decrypt header
    for(int iCount = 0; iCount < 3; ++iCount)
    {
		::memcpy(szBuf, lpszTmp, ENCRYPT_BLOCK_LEN);
		dedes(ks,szBuf);
        ::memcpy(lpszTmp, szBuf, ENCRYPT_BLOCK_LEN);

        lpszTmp += ENCRYPT_BLOCK_LEN;
    }

    // check decryption and extract header info
    // skip random number (4 bytes)
    iSize = 4;
    lpszTmp = szHeader + iSize;

    // check correct signature
    if(strncmp(lpszTmp, ENCRYPT_SIGNATURE, ENCRYPT_SIGNATURE_SIZE) != 0)
    {
		desdone();
        ::CloseHandle(hFileIn);
        ::CloseHandle(hFileOut);
        return eEE_WrongPassword;
    }

    // read 8 bytes text size
    iSize = sizeof(i64_TextSize);
    lpszTmp += iSize;
    ::memcpy(&i64_TextSize, lpszTmp, iSize);
    lpszTmp += iSize;


    // decrypt input file and write to output file
    E_EncryptErr    eErr = eEE_Ok;
	int             iBufPos = 0;
	char*           lpszCharBuf = new char [ENCRYPT_BUFFER_SIZE];
    __int64         i64_PosText;
    __int64         i64_TotalWritten = 0;

	if(!lpszCharBuf)
    {
		desdone();
        ::CloseHandle(hFileIn);
        ::CloseHandle(hFileOut);
        return eEE_NoMemory;
    }

	lpszCharBuf[0] = 0;

    for (i64_PosText = 0; i64_PosText < i64_TextSize; i64_PosText += ENCRYPT_BUFFER_SIZE)
	{
		if(!ReadFile(hFileIn, (void*)lpszCharBuf, ENCRYPT_BUFFER_SIZE, &dwRead, NULL))
        {
            eErr = eEE_ReadFile;
            break;
        }
			
		for (dwWrite = 0; dwWrite < dwRead; dwWrite += ENCRYPT_BLOCK_LEN)
		{
			lpszTmp = lpszCharBuf+dwWrite;
			
			::memcpy(szBuf, lpszTmp, ENCRYPT_BLOCK_LEN);
			dedes(ks,szBuf);
			::memcpy(lpszTmp, szBuf, ENCRYPT_BLOCK_LEN);
		}

        // do not write more than original text size
        if(i64_TotalWritten + dwWrite > i64_TextSize)
        {
            dwWrite = (DWORD)(i64_TextSize - i64_TotalWritten);
        }

		if (dwWrite>0)	// entered for-loop
		{
			// write Block to dest file
            if(!::WriteFile(hFileOut, (void*)lpszCharBuf, dwWrite, &dwWritten, NULL) || dwWritten != dwWrite)
            {
                eErr = eEE_WriteFile;
                break;
            }
            i64_TotalWritten += dwWritten;
		}
	}

    delete lpszCharBuf;

	desdone();

    ::CloseHandle(hFileIn);
    ::CloseHandle(hFileOut);

	return eErr;
}


/*
 *----------------------------------------------------------------------------*
 *  $History: BAR_Util.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 7  *****************
 * User: Sis          Date: 8.11.04    Time: 12:14
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * fixed unpack bug
 * 
 * *****************  Version 6  *****************
 * User: Sis          Date: 1.06.04    Time: 16:12
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * mem leak
 * 
 * *****************  Version 5  *****************
 * User: Sis          Date: 1.06.04    Time: 12:12
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 4  *****************
 * User: Sis          Date: 1.06.04    Time: 12:05
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * encryption bug
 * 
 * *****************  Version 3  *****************
 * User: Sis          Date: 1.06.04    Time: 11:37
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 2  *****************
 * User: Sis          Date: 21.04.04   Time: 13:19
 * Updated in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * added encrypt
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/CTOOL/CBAR/BAR_LIB
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 10.07.01   Time: 16:39
 * Created in $/4Control/COM/softing/fc/CBAR/BAR_LIB
 * first draft
 *==
 *----------------------------------------------------------------------------*
*/
