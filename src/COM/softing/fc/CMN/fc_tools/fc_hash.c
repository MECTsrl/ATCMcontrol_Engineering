/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_hash.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_hash.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_hash.c $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT             $Date: 28.02.07 19:00 $
 *                      $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==Hash table implementation  
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  15.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/


#include <tchar.h>
#include <stddef.h>  //offsetof
#include <windows.h> //lstrcmp
#include <stdio.h>   //fopen
#include "fc_tools\fc_hash.h"
#include "fc_tools\fc_libmem.h"

static unsigned long FC_GetStringHash(const TCHAR* pszStr, int lStrLen)
{
    register unsigned long nHash = 0;
    
    for(;*pszStr;pszStr++)
        nHash = (nHash<<5) + nHash + *pszStr;

    return nHash;
}
static unsigned long FC_GetStringIHash(const TCHAR* pszStr, int lStrLen)
{
    register unsigned long nHash = 0;
    TCHAR                  buffer[256];
    TCHAR*                 pszLower = buffer; 
    TCHAR*                 psz; 
    int                    nBytes = sizeof(TCHAR)*(lStrLen+1);
    

    if(lStrLen>=FC_ARRAY_LEN(buffer))
    {
        if(!FC_LibNewMemory((void**)&pszLower, nBytes))
            return 0;
    }
        
    memcpy(pszLower, pszStr, nBytes); 
    _tcslwr(pszLower);

    for(psz=pszLower;*psz;psz++)
        nHash = (nHash<<5) + nHash + *psz;

    if(pszLower != buffer)
        FC_LibFreeMemory(pszLower);
    
    return nHash;
}



typedef struct _TableEntry TableEntry;


//CAUTION: do not change the order of members 
//see fc_hash.h FC_HashEnumPairs struct type and the C++ wrappers
struct _TableEntry 
{
    //next five members are exposed via type cast to FC_HashEnumPair
    TableEntry*   pNext;  //doubly linked list to enumerate key-val
    TableEntry*   pPrev;  //pairs in natural order as inserted/deleted
    const TCHAR*  pKey;   //pointer to key data 0-term const TCHAR* 
                          //or (DWORD)pKey. if bAllocKey
                          //is used points to memory allocated 
                          //in keyData[] zero sized array.
    void*         pVal;   //only one value for a key always a 32-bit data
   
    //internal data:
    unsigned long hash;   //remember the hash value for debug & performance
    TableEntry*   pNHash; //simply linked list used for the hash table itself
    TCHAR         keyData[0];//dirty thing
};
struct _FC_HashTable
{
    TableEntry** pTable;
    long         length;     //allocated size pTable
    FC_HASH_KEY  keyTyp;     //interpretation of pKey
    int          bAllocKey;  //memory allocation for key
    long         nCurrElems; //current count of all entries
    float        fthreshold; //user defined threshold set on creation
    long         threshold;  //if nCurrElems>=threshold do rehash
    TableEntry*  pFirst;     //doubly linked list to enumerate key-val
    TableEntry*  pLast;      //first and last pointer
};



//returns NULL if not found pszKey or the tyable entry:
static TableEntry* FC_HashFindEx(
    const FC_HashTable* pHT, 
    const TCHAR*        pKey,
    size_t*             pKeySize, 
    unsigned long*      pHash
)
{
    TableEntry*    pe;
    unsigned long  hash;
    size_t         keySize;

    keySize = 0;

    FC_LIBASSERT(pHT);

    if(pHT->keyTyp==FC_HASH_KEY_IS_DWORD)
    {
        *pHash = hash = (unsigned long)pKey;
        *pKeySize = 0;
        if(pHT->length>0)
        {
            for(pe = pHT->pTable[hash % pHT->length];pe; pe=pe->pNHash) 
            {
                if(pe->pKey==pKey)
                {
                    FC_LIBASSERT(pe->hash==hash);
                    return pe;
                }
            }
        }

        return NULL;
    }

    //_PSZ or _IPSZ
    FC_LIBASSERT(pKey);
    *pKeySize = keySize = sizeof(TCHAR)*(_tcslen(pKey)+1);
 
    if(pHT->keyTyp==FC_HASH_KEY_IS_IPSZ)
    {
        *pHash = hash = FC_GetStringIHash(pKey, keySize-1);
        if(pHT->length>0)
        {
            for(pe = pHT->pTable[hash % pHT->length];pe; pe=pe->pNHash) 
            {
                if(pe->hash==hash && !lstrcmpi(pe->pKey, pKey))
                    return pe;
            }
        }
    }
    else
    {
        *pHash = hash = FC_GetStringHash(pKey, keySize-1);
        if(pHT->length>0)
        {
            for(pe = pHT->pTable[hash % pHT->length];pe; pe=pe->pNHash) 
            {
                if(pe->hash==hash && !lstrcmp(pe->pKey, pKey))
                    return pe;
            }
        }
    }
    
    return NULL;
}

static int FC_RehashTable(FC_HashTable* pHT) 
{
    long         i;
    long         index;
    TableEntry*  pe;
    long         oldCapacity = pHT->length;
    TableEntry** pOldTable   = pHT->pTable;
    long         newCapacity = oldCapacity * 2 + 1;
    TableEntry** pNewTable;
    TableEntry*  pOld;  


    if(!FC_LibNewMemory((void**)&pNewTable, sizeof(TableEntry*)*newCapacity))
        return 0;

    memset(pNewTable, 0, sizeof(TableEntry*)*newCapacity);

    FC_LIBASSERT(pHT->fthreshold>=0.10f && pHT->fthreshold<=0.90f);
    pHT->threshold = (long)(newCapacity * pHT->fthreshold);
    pHT->pTable    = pNewTable;
    pHT->length    = newCapacity;

    for(i = oldCapacity ; i-- > 0 ;) 
    {
        for(pOld = pOldTable[i] ; pOld ; ) 
        {
            pe = pOld;
            pOld = pOld->pNHash;

            index = pe->hash % newCapacity;
            pe->pNHash = pNewTable[index];
            pNewTable[index] = pe;
        }
    }

    if(pOldTable)
        FC_LibFreeMemory(pOldTable);
    return 1;
}















FC_HashTable* FC_HashCreate(
    FC_HASH_KEY    keyTyp,      //pKey interpretation
    int            bAllocKey,   //if pKey is const TCHAR*
                                //bAllocKey causes internal allocation
                                //If false: 
                                //pointers to keys must be valid 
                                //as long as the hash table is 
                                //not deleted.
    long           initialSize, //The initial size of the hash table >=0.
                                //If 0 table is initially empty and grows
                                //on rehash as: newsize = 2 * oldsize + 1.
    float          threshold    //Percentage when to rehash, between 0.10..0.90.
                                //If outside of range 0.75 is assumed.
)
{
    FC_HashTable*   pHT;

    //check the flags:
    if(keyTyp==FC_HASH_KEY_IS_DWORD)
        bAllocKey = 0;//ok..
    
    //check for range, no assertion:
    if(threshold<0.10f || threshold>0.90f)
        threshold = 0.75f;

    if(initialSize<0)
       initialSize=0;

    if(!FC_LibNewMemory((void**)&pHT, sizeof(FC_HashTable)))
        return NULL;

    pHT->keyTyp     = keyTyp;
    pHT->bAllocKey  = bAllocKey;
    pHT->nCurrElems = 0;
    pHT->length     = initialSize<0 ? 0 : initialSize;
    pHT->fthreshold = threshold;
    pHT->threshold  = (long)(pHT->length * threshold);
    pHT->pFirst     = NULL;
    pHT->pLast      = NULL;

    if(pHT->length>0)
    {
        if(!FC_LibNewMemory((void**)&pHT->pTable, sizeof(TableEntry*) * pHT->length))
        {
            FC_LibFreeMemory(pHT);
            return NULL;
        }
        memset(pHT->pTable, 0, sizeof(TableEntry*) * pHT->length);
    }
    else
    {
        pHT->pTable = NULL;
    }
        
    return pHT;
}

void FC_HashDelete(
    FC_HashTable* pHT
)
{
    if(pHT==NULL)
        return;
    
    FC_HashClear(pHT);
    if(pHT->pTable)
        FC_LibFreeMemory(pHT->pTable);

    FC_LibFreeMemory(pHT);
}

void FC_HashClear(
    FC_HashTable* pHT
)
{
    FC_LIBASSERT(pHT);
    if(pHT==NULL)
        return;

#if 0 //  changed: 20.03.03: defined(DEBUG) || 1 //   ||1   do always ...
	  //if bAllocKey is false and C++ deletePtrs was used the pszKey and pVal are not valid at this point
	  //for performance reasons the simple solution is to accept this short time of having invalid pointers
	  //the #else code does not need pszKey nor pVal, so it will work as expected.
    {
    int bOk = 1;
    while(pHT->pFirst && bOk)
    {
        bOk= FC_HashRemove(pHT, pHT->pFirst->pKey);
        FC_LIBASSERT(bOk || !"FC_HashClear: data corrupted");
    }
    FC_LIBASSERT(pHT->pLast==NULL && pHT->nCurrElems==0);
    }

#else

    if(pHT->nCurrElems!=0)
	{
		long          index;
		TableEntry*   pe;
		TableEntry*   pe2;

		for(index = 0; index<pHT->length; index++)
		{
			pe = pHT->pTable[index];
			pHT->pTable[index] = NULL;
			while(pe)
			{
				pe2= pe;
				pe = pe->pNHash;
				FC_LibFreeMemory(pe2);
				if(--pHT->nCurrElems == 0)
				{
					pHT->pFirst= NULL;
					pHT->pLast = NULL;
					return;
				}
			}
		}
	    FC_LIBASSERT(!"FC_HashClear: memory corrupted");
    }
#endif
}




//returns false if pKey not found otherwise true
const FC_HashEnumPairs* FC_HashFind(
    const FC_HashTable* pHT,    //table where to search 
    const TCHAR*        pKey    //const TCHAR* or a DWORD
)
{
    const TableEntry* pe;
    unsigned long     hash;
    size_t            keySize;

    pe = FC_HashFindEx(pHT, pKey, &keySize, &hash);
    return (const FC_HashEnumPairs*)pe;
}



//returns false if key already exits, true if new key created:
//if key already exists it's value is replaced
//If out of memory returns -1
int FC_HashInsert(
    FC_HashTable* pHT,     //pointer to hash table 
    const TCHAR*  pKey,    //pointer to key
    void*         pVal     //the 32 bit value
)
{
    return FC_HashInsertEx(pHT, pKey, pVal, 1, NULL, NULL);
}


int FC_HashRemove(
    FC_HashTable*  pHT,     //pointer to hash table 
    const TCHAR*   pKey     //pointer to key or DWORD
)
{
    TableEntry*    pe;
    TableEntry*    peFirst;
    unsigned long  hash;
    unsigned long  index;
    size_t         keySize;

    pe = FC_HashFindEx(pHT, pKey, &keySize, &hash);
    if(!pe)
        return 0;

    //remove from hash struct:
    index = hash % pHT->length;
    peFirst = pHT->pTable[index];
    if(pe==peFirst)
    {
        pHT->pTable[index] = pe->pNHash;
    }
    else
    {
        for( ;peFirst; peFirst = peFirst->pNHash)
        {
            if(pe==peFirst->pNHash)
            {
                peFirst->pNHash = pe->pNHash;
                break;
            }
        }
        if(!peFirst)
        {
            FC_LIBASSERT(!"FC_HashRemove: memory corrupted");
            return 0;
        }
    }

    //update the entry counter: 
    FC_LIBASSERT(pHT->nCurrElems>0 || !"FC_HashRemove: memory corrupted");
    pHT->nCurrElems--;


    //update the linked list of key-val pairs:
    if(pe->pPrev)
    {
        pe->pPrev->pNext = pe->pNext;
    }
    else
    {
        FC_LIBASSERT(pHT->pFirst == pe);
        pHT->pFirst = pe->pNext;
    }

    if(pe->pNext)
    {
        pe->pNext->pPrev = pe->pPrev;
    }
    else
    {
        FC_LIBASSERT(pHT->pLast == pe);
        pHT->pLast = pe->pPrev;
    }

    //free mem of the entry:
    FC_LibFreeMemory(pe); 

    return 1;
}




//returns 0 if key already exits, 1 if new key created:
//if key already exists it's value is replaced
//If out of memory returns -1
int FC_HashInsertEx(
    FC_HashTable* pHT,     //pointer to hash table 
    const TCHAR*  pKey,    //pointer to key 
    void*         pVal,    //the 32 bit value
    int           bReplace,//true: replace a possible previous val
                           //false: don't insert pVal
    void**        ppVal,   //Output: receives a previous value if any
                           //if no previous value *ppVal is unchanged.
                           //ppVal may be NULL
    const TCHAR** ppKey    //Output: receives always a pointer to key-data.
                           //If no previous value: 
                           //    *ppKey is set to the newly inserted 
                           //    internal pKey pointer which might be
                           //    allocated if bAllocKey
                           //    was used on creation.
                           //If a previous value: 
                           //    *ppKey is set to the previously inserted
                           //    internal pKey pointer which might be
                           //    allocated if bAllocKey
                           //    was used on creation.
                           //ppKey may be NULL.
                           //Note: this param is normally only usefull if used
                           //together with bAllocKey, then the hash table 
                           //it can be used as e.g. a string bag.
)
{
    unsigned long hash;
    unsigned long index;
    TableEntry*   pe;
    size_t        keySize;
    size_t        nBytesAlloc;

    // Makes sure the key is not already in the hashtable.
    pe = FC_HashFindEx(pHT, pKey, &keySize, &hash);
    if(pe)
    {
        if(ppVal)
            *ppVal = pe->pVal;
        if(ppKey)
            *ppKey = pe->pKey;
        if(bReplace)
            pe->pVal = pVal;
        return 0;
    }


    if(pHT->nCurrElems >= pHT->threshold) 
    {
        // Rehash the table if the threshold is exceeded
        if(!FC_RehashTable(pHT))
            return -1;
    } 

    //Creates the new entry with some dirty void* arithmetics:
    nBytesAlloc = offsetof(TableEntry, keyData);
    if(pHT->bAllocKey)
        nBytesAlloc += keySize;

    if(!FC_LibNewMemory((void**)&pe, nBytesAlloc))
        return -1;

    pe->hash   = hash;
    pe->pVal   = pVal;

    if(pHT->bAllocKey)
    {   
        pe->pKey = pe->keyData;
        memcpy(pe->keyData, pKey, keySize);
    }
    else
    {
        pe->pKey   = pKey;
    }
    

    //update the linked list of key-val pairs:
    pe->pNext = NULL;
    if(!pHT->pFirst)
    {
        FC_LIBASSERT(!pHT->pLast);
        pHT->pLast = pHT->pFirst = pe;
        pe->pPrev = NULL;
    }
    else
    {
        pe->pPrev = pHT->pLast;
        FC_LIBASSERT(!pHT->pLast->pNext);
        pHT->pLast->pNext = pe;
        pHT->pLast = pe;
    }

    //update the hash table structure:
    index = hash % pHT->length;
    pe->pNHash  = pHT->pTable[index];
    pHT->pTable[index] = pe;
    pHT->nCurrElems++;


    if(ppKey)
        *ppKey = pe->pKey;

    return 1;
}


long FC_HashCountPairs(const FC_HashTable* pHT)
{
    return pHT->nCurrElems;
}
const FC_HashEnumPairs* FC_HashFirstPair(const FC_HashTable* pHT)
{
    return (const FC_HashEnumPairs*)pHT->pFirst;
}
const FC_HashEnumPairs* FC_HashLastPair(const FC_HashTable* pHT)
{
    return (const FC_HashEnumPairs*)pHT->pLast;
}

const TCHAR** FC_HashGetNewPZPSZKeys(
    const FC_HashTable* pHT, 
    int*                pLen,
    int                 bReverseOrder
)
{
    int               i;
    const TCHAR**     pzpsz;
    const TableEntry* pe;

    FC_LIBASSERT(pHT->keyTyp!=FC_HASH_KEY_IS_DWORD);

    if(!FC_LibNewMemory((void**)&pzpsz, 
        (pHT->nCurrElems +1)*sizeof(const TCHAR*)))
    {
        if(pLen)
            *pLen = 0;
        return NULL;
    }

    if(bReverseOrder)
    {
        for(i=0, pe=pHT->pLast; pe; pe = pe->pPrev)
            pzpsz[i++] = pe->pKey;
    }
    else
    {
        for(i=0, pe=pHT->pFirst; pe; pe = pe->pNext)
            pzpsz[i++] = pe->pKey;
    }

    pzpsz[i] = NULL;
    FC_LIBASSERT(i==pHT->nCurrElems);

    if(pLen)
        *pLen = pHT->nCurrElems;

    return pzpsz;
}
const void** FC_HashGetNewPZPValues(
    const FC_HashTable* pHT, 
    int*                pLen,
    int                 bReverseOrder
)
{
    int               i;
    const void**      pzp;
    const TableEntry* pe;

    if(!FC_LibNewMemory((void**)&pzp, 
        (pHT->nCurrElems +1)*sizeof(const TCHAR*)))
    {
        if(pLen)
            *pLen = 0;
        return NULL;
    }

    if(bReverseOrder)
    {
        for(i=0, pe=pHT->pLast; pe; pe = pe->pPrev)
            pzp[i++] = pe->pVal;
    }
    else
    {
        for(i=0, pe=pHT->pFirst; pe; pe = pe->pNext)
            pzp[i++] = pe->pVal;
    }

    pzp[i] = NULL;
    FC_LIBASSERT(i==pHT->nCurrElems);
   
    if(pLen)
        *pLen = pHT->nCurrElems;

    return pzp;
}

const void** FC_HashGetNewPZPKeys(
    const FC_HashTable* pHT, 
    int*                pLen,
    int                 bReverseOrder
)
{
    int               i;
    const void**      pzp;
    const TableEntry* pe;

    FC_LIBASSERT(pHT->keyTyp==FC_HASH_KEY_IS_DWORD);

    if(!FC_LibNewMemory((void**)&pzp, 
        (pHT->nCurrElems +1)*sizeof(const TCHAR*)))
    {
        if(pLen)
            *pLen = 0;
        return NULL;
    }

    if(bReverseOrder)
    {
        for(i=0, pe=pHT->pLast; pe; pe = pe->pPrev)
            pzp[i++] = pe->pKey;
    }
    else
    {
        for(i=0, pe=pHT->pFirst; pe; pe = pe->pNext)
            pzp[i++] = pe->pKey;
    }


    pzp[i] = NULL;
    FC_LIBASSERT(i==pHT->nCurrElems);
   
    if(pLen)
        *pLen = pHT->nCurrElems;

    return pzp;
}


/*
 *----------------------------------------------------------------------------*
 *  $History: fc_hash.c $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 2  *****************
 * User: Wi           Date: 26.09.03   Time: 13:11
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * changes for acyclic tasks
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 7  *****************
 * User: Ef           Date: 11.03.02   Time: 15:08
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * removed dead code
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 9.04.01    Time: 10:26
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * new allocKey/ValReverse order
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 1.03.01    Time: 20:53
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 12.02.01   Time: 14:23
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * removed the debug dump 
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 6.02.01    Time: 16:18
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * new final version of hash stuff
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 22.01.01   Time: 11:03
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * new fc_system.c
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 15.01.01   Time: 16:21
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 *==
 *----------------------------------------------------------------------------*
*/
