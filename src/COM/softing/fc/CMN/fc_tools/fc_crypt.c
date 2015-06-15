/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_crypt.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $RCSFile$
 *
 * =PROJECT             CAK1020
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  File created: 08.11.2004
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

//----  Includes:   -------------------------------------------*
#include <stddef.h>  //NULL
#include <memory.h>  //memset
#include <string.h>  //strncpy
#include "fc_tools\fc_crypt.h"
#include "fc_tools\fc_libmem.h"

//----  Local Defines:   -------------------------------------------*
// Encyption defines
#define ENCRYPT_SIGNATURE               "fc_tools"//<- CAUTION MUST(!) BE 8 CHARS
#define ENCRYPT_MAX_KEYLEN				(8)


//----  Static Initializations:   ----------------------------------*


// DES algorithm functions
static int  desinit(int mode);
static void desdone();
static int  setkey(char (*kn)[8], const char* key);
static int  endes(char (*kn)[8], char* block);
static int  dedes(char (*kn)[8], char* block);






FC_TOAPI void FC_TODECL FC_DESEncodeBlock(
    long            nMode,
    const char*     pszPw, 
    const char*     pData, 
    unsigned long   nData,
    char*           pOut,
    unsigned long*  pnOut
)
{
    // calculate encryption key by encrypt character key with fix help key
	char	        szKey[ENCRYPT_MAX_KEYLEN+1];
	char            ks[16][8];
    int             nDESMode;
    char*           p;
    unsigned long   nData_align8;
    unsigned long   ul;
        
    nDESMode = (nMode & FC_DES_FAST) ? 1 : 0;


    //key must be at least 8 byte long, if pszPw is shorter fill with 0chars:
    memset(szKey, 0 , ENCRYPT_MAX_KEYLEN+1);
    strncpy(szKey, pszPw, ENCRYPT_MAX_KEYLEN);
    
    desinit(nDESMode);
	setkey(ks,szKey);


    //first 4 byte is original length:
    *((long*)pOut) = nData;
    p = pOut + sizeof(nData);

    memcpy(p, ENCRYPT_SIGNATURE, 8);
    endes(ks,p);
    p += 8;

    //align to the next 8 byte boundary:
    nData_align8 = ((nData+7)/8)*8;

    memcpy(p, pData, nData);
    memset(p+nData, 0, nData_align8 - nData);


    for(ul=0; ul<nData_align8; ul += 8)
        endes(ks,p+ul);

	desdone();
    
    //set the final result size:
    *pnOut = 4 + 8 + nData_align8;
}



FC_TOAPI int FC_TODECL FC_DESGetDecodedSize(
    long            nMode,
    const char*     pszPw, 
    const char*     pData, 
    unsigned long   nData,
    unsigned long*  pSize
)
{
    //all modes currently supported store 
    //the decoded size in the first 4 bytes of pData
    //and then an 8 byte magic key follows:
    if(nData<4 || !pData) 
        return 3;

    //the first 4 byte as ulong:
    *pSize = *((long*)pData);

    //TODO: consider: check the password an return 1 if invalid...

    //verify the size:
    if(!FC_DESCheckDecodedSize(nMode, *pSize, nData))
        return 3;

    //looks good, all ok:
    return 0;
}

FC_TOAPI int FC_TODECL FC_DESCheckDecodedSize(
    long            nMode,
    unsigned long   ulDecodedSize, 
    unsigned long   ulEncodedSize
)
{
    //all modes currently supported store 
    //the decoded size in the first 4 bytes of pData
    //and then an 8 byte magic key follows:
    if(ulEncodedSize<12 || ((ulEncodedSize-12) % 8)!=0) 
        return 0;

    //verify the size:
    if(ulDecodedSize>ulEncodedSize-12 || ulDecodedSize+8<ulEncodedSize-12)
        return 0;
    
    //looks good, all ok:
    return 1;
}




FC_TOAPI int FC_TODECL FC_DESDecodeBlock(
    long            nMode,
    const char*     pszPw, 
    const char*     pData, 
    unsigned long   nData,
    char*           pOut,
    unsigned long   nOut
)
{
	char	        szBuffer[ENCRYPT_MAX_KEYLEN+1];
	char            ks[16][8];
    unsigned long   nDecodedSize;
    int             nDESMode;
    unsigned long   nAlign8Size;
    unsigned long   ul;
    unsigned long   ulLastBlock;

    //all modes currently supported store 
    //the decoded size in the first 4 bytes of pData
    //and then an 8 byte magic key follows:
    if(nData<12 || !pData || ((nData-12) % 8)!=0) 
        return 3;

    //the first 4 byte as ulong:
    nDecodedSize = *((long*)pData);

    if(nDecodedSize>nData-12 || nDecodedSize+8<nData-12)
        return 3;

    //check that output buffer is large enough:
    if(nOut<nDecodedSize)
        return 2;

    nDESMode = (nMode & FC_DES_FAST) ? 1 : 0;


    //key must be at least 8 byte long, if pszPw is shorter fill with 0chars:
    memset(szBuffer, 0 , ENCRYPT_MAX_KEYLEN+1);
    strncpy(szBuffer, pszPw, ENCRYPT_MAX_KEYLEN);
    
    desinit(nDESMode);
	setkey(ks,szBuffer);

    //copy the magic key in buffer and decode:
    memcpy(szBuffer, pData+4, 8);
    dedes(ks,szBuffer);

    //check the password, the decoded magic key must match ENCRYPT_SIGNATURE:
    if(strncmp(szBuffer, ENCRYPT_SIGNATURE, 8))
    {
    	desdone();
        return 1;
    }

    //copy to pOut and decode in place the "multiple of 8 bytes" 
    //amount of pData:
    nAlign8Size = (nDecodedSize/8)*8;
    memcpy(pOut, pData+12, nAlign8Size);

    for(ul=0; ul<nAlign8Size; ul+=8)
        dedes(ks, pOut+ul);

    
    //now the remaining bytes:
    ulLastBlock = nData-12-nAlign8Size;
    if(ulLastBlock > 0)
    {
        FC_LIBASSERT(ulLastBlock == 8);
        memcpy(szBuffer, pData+12+nAlign8Size, ulLastBlock); 
        dedes(ks, szBuffer);
        FC_LIBASSERT(nDecodedSize-nAlign8Size < 8);
        memcpy(pOut+nAlign8Size, szBuffer, nDecodedSize-nAlign8Size);
    }

	desdone();
    return 0;
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
static int desinit(int mode)
{
	if(sp != NULL){
		/* Already initialized */
		return 0;
	}
	desmode = mode;
	
	//sp = (long (*)[64]) new long[8][64];
    sp = NULL;
    if(!FC_LibNewMemory((void**)&sp, sizeof(long)*8*64)){
		return 0;
	}
	spinit();
	if(mode == 1)	/* No permutations */
		return 0;

	//iperm = (char (*)[16][8])new char[16][16][8];
    iperm = NULL;
    if(!FC_LibNewMemory((void**)&iperm, sizeof(char)*16*16*8)){
        FC_LibFreeMemory(sp);
		return -1;
	}
	perminit(iperm,ip);

	//fperm = (char (*)[16][8])new char[16][16][8];
    fperm = NULL;
    if(!FC_LibNewMemory((void**)&fperm, sizeof(char)*16*16*8)){
        FC_LibFreeMemory(sp);
        FC_LibFreeMemory(iperm);
		return -1;
	}
	perminit(fperm,fp);
	
	return 0;
}

/* Free up storage used by DES */
static void desdone()
{
	if(!sp)
		return;	/* Already done */

    FC_LibFreeMemory(sp);
	if(iperm)
        FC_LibFreeMemory(iperm);
	if(fperm)
        FC_LibFreeMemory(fperm);

	sp = NULL;
	iperm = NULL;
	fperm = NULL;
}

/* Set key (initialize key schedule array) */
static int setkey(char (*kn)[8], const char* key)		
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
static int endes(char (*kn)[8], char *block)
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
static int dedes(char (*kn)[8], char *block)
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




/*
 *----------------------------------------------------------------------------*
 *  $Log: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_crypt.c $
 * 
 * 1     28.02.07 19:00 Ln
 * 
 * 2     8.02.05 13:52 Ef
 * fixed d-169
 * 
 * 1     12.11.04 10:49 Ef
 * added DES crypto helper functions
 *==
 *----------------------------------------------------------------------------*
*/
