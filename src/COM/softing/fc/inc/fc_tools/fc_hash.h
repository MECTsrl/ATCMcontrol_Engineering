/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_hash.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_hash.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_hash.h $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT             $Date: 28.02.07 19:01 $
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
 *==A simple hashtable as C-core routines and easy to use C++ inline wrapper 
 * classes.
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  15.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#ifndef __FC_HASH_H_
#define __FC_HASH_H_

#include <tchar.h>
#include "fc_defines.h"




#ifdef __cplusplus
extern "C"{
#endif
//foreward declare a typesafe handle to the hash table:
typedef struct _FC_HashTable FC_HashTable;
#ifdef __cplusplus
}
#endif

   

#ifdef __cplusplus
//Wrap FC_Hashtable functions (declared below):
//FC_CHashStr: hash a V by a string 
//V must be a 32-bit type that can be casted to a void*
//and vice versa without loss of data e.g. any pointer 
//or a long, DWORD,...
template <class V> class FC_CHashStr
{
protected:
    FC_HashTable* m_pHash;
    
    //to get an early compile time error if V is a C++ class
    //if V cannot be casted to void* the error message comes
    //when first calling a method.
    typedef union{V ta;} noclass;
public:
    //enumerate the key<->val pairs, the order is 
    //always the natural order of inserts: 
    struct Pair
    {
        const Pair*  pNext;
        const Pair*  pPrev;
        const TCHAR* pszKey;
        V            pVal;
    };

    inline FC_CHashStr(
        bool  bIgnoreCase,   //if true the hash work case insensitive 
        bool  bAllocKey=true,//if false the const TCHAR* key is not duplicated
        int   initalSize=0,  //the init size of the hash entry array.
        float threshold=0.75 //percentage when to rehash: 0.1 .. 0.75
    );
    inline ~FC_CHashStr();

    //Get a Pair by key name:
    inline const Pair* getPair(const TCHAR* pszKey)const;
   
    //get returns true if found and *ppVal is set to the key's value
    //if pszKey was not found *ppVal is unchanged.
    inline bool get(const TCHAR* pszKey, V* ppVal)const;

    //get returns V if found or NULL, use can use this simple form 
    //of get if V is e.g. a pointer and never a NULL value was 
    //inserted:
    inline V get(const TCHAR* pszKey)const;

    //returns true if a new key created, otherwise false.
    //if key already exists it's value is replaced by pVal:
    inline bool insert(const TCHAR* pszKey, V pVal);


    //returns true if a new key created, otherwise false.
    //if bReplace==true a possible previous V value 
    //is replaced by pVal otherwise the previous value is 
    //unchanged and neither pszKey nor pVal is stored.
    //if ppPrev!=NULL: *ppPerv is unchanged if no pervious value.
    //otherwise *ppPrev receives the previous value.
    inline bool insertEx(
        const TCHAR* pszKey, 
        V            pVal, 
        bool         bReplace, 
        V*           ppPrev
    );

    //remove a key-val pair:
    //return true if pszKey was found and removed,false otherwise:
    inline bool remove(const TCHAR* pszKey);

    
    //enum the key-val pairs in the natural order of key creation:
    const Pair* firstPair()const;
    const Pair* lastPair()const;
    long      countPairs()const;
    //usage example: 
    //const FC_CHashStr<const TCHAR*>::Pair* pcp;
    //
    //for(pcp = sbys.firstPair(); pcp;pcp = pcp->pNext)
    //    _tprintf(_T("%s=%s\n"),pcp->pszKey, pcp->pVal); 

    
    //alloc[Key|Val]Array[Reverse]: returns zero terminated arrays
    //of pointers to all key names (or value pointers) 
    //in natural (or natual reverse) order. The memory for the array is allocated 
    //with FC_LibNewMemory. 
    //The caller must free the memory with FC_LibFreeMemory. 
    //The arrays returned are only valid as long no 
    //call to remove/insert follows. Be carefull..
    //If pLen!=NULL: *pLen receives the length of the 
    //returned arraywithout the NULL terminator, same as 
    //countPairs() would return.
    //
    //Note: allocKeyArray: the pointers to key names point 
    //either into the hash table if bAllocKey was set 
    //on creation or it's the psz-pointer to key name 
    //passed on insert().
    const TCHAR** allocKeyArray(int* pLen)const;
    V*            allocValArray(int* pLen)const;
    const TCHAR** allocKeyArrayReverse(int* pLen)const;
    V*            allocValArrayReverse(int* pLen)const;

    
    //remove all Pairs from the hash:
    inline void clear();

    //if V is a non const pointer deletePtrs() can be used
    //to free memory of V for all values in the hash.
    //usefull for destructor bodies (saves a for-loop)
    //deletePtrs calls also clear:
    inline void deletePtrs();
};


//FC_CHashPtr hash a V-type value by a K-type key.
//The K-type must be 32-bit value is itself used as the hash value.
//V and K types must be a 32-bit type that can be casted to a void*
//and vice versa without loss of data e.g. any pointer, long, DWORD,...
template <class K, class V> class FC_CHashPtr
{
protected:
    FC_HashTable* m_pHash;
    
    //to get an early compile time error if V is a C++ class
    //if V cannot be casted to void* the error message comes
    //when first calling a method.
    typedef union{K tk;V tv;} noclass;
public:
    //enumerate the key<->val pairs, the order is 
    //always the natural order of inserts: 
    struct Pair
    {
        const Pair*   pNext;
        const Pair*   pPrev;
        K             pKey;
        V             pVal;
    };

    inline FC_CHashPtr(int initalSize=0, float threshold=0.75);
    inline ~FC_CHashPtr();

    //Get a Pair by key name:
    inline const Pair* getPair(K pKey)const;
   
    //get returns true if found and *ppVal is set to the key's value
    //if pszKey was not found *ppVal is unchanged.
    inline bool get(K pKey, V* ppVal)const;

    //get returns V if found or NULL, use this simple form 
    //of 'get' if V is e.g. a pointer and never a NULL value was 
    //inserted:
    inline V get(K pKey)const;

    //returns true if a new key created, otherwise false.
    //if key already exists it's value is replaced by pVal:
    inline bool insert(K pKey, V pVal);

    //returns true if a new key created, otherwise false.
    //if bReplace==true a possible previous V value 
    //is replaced by pVal otherwise the previous value is 
    //unchanged and neither pszKey nor pVal is stored.
    //if ppPrev!=NULL: *ppPerv is unchanged if no pervious value.
    //otherwise *ppPrev receives the previous value.
    inline bool insertEx(
        K     pKey, 
        V     pVal, 
        bool  bReplace, 
        V*    ppPrev
    );

    //remove a key-val pair:
    //return true if pszKey was found and removed,false otherwise:
    inline bool remove(K pKey);
    
    //enum the key-val pairs in the natural order of key creation:
    const Pair* firstPair()const;
    const Pair* lastPair()const;
    long      countPairs()const;
    
    //alloc[Key|Val]Array[Reverse]: returns zero terminated arrays
    //of pointers to all key (or value) pointers in natural 
    //(or natural reverse) order. The memory for the array is 
    //allocated with FC_LibNewMemory. 
    //The caller must free the memory with FC_LibFreeMemory. 
    //The arrays returned are only valid as long no 
    //call to remove/insert follows. Be carefull..
    //If pLen!=NULL: *pLen receives the length of the 
    //returned array without the NULL terminator, same as 
    //countPairs() would return.
    K* allocKeyArray(int* pLen)const;
    V* allocValArray(int* pLen)const;
    K* allocKeyArrayReverse(int* pLen)const;
    V* allocValArrayReverse(int* pLen)const;

    //remove all Pairs from the hash:
    inline void clear();

    //if V is a non const pointer deletePtrs() can be used
    //to free memory of V for all values in the hash.
    //usefull for destructor bodies (saves a for-loop)
    //deletePtrs calls also clear():
    inline void deletePtrs();
};


//Wrap FC_Hashtable functions:
//FC_CHashStrTable registers strings and returns a const TCHAR*.
//This is usefull if string data should be strored only once
//and then only a const TCHAR*  pointer is used.
//Note: to be safe this class does only a case insensitive 
//hash operation, hashing ignore causes possibly undesired
//side effects in an application.
class FC_CHashStrTable
{
private:
    FC_HashTable* m_pHash;
public:
    inline FC_CHashStrTable(int initalSize=0, float threshold=0.0);
    inline ~FC_CHashStrTable();
    
    //register a string:
    inline const TCHAR* registerString(const TCHAR* psz)const;
};
#endif //__cplusplus























//--------- Hash table implementation C-Interface:
#ifdef __cplusplus
extern "C"{
#endif

//the key is either a const TCHAR* 0-terminated buffer
//or a (DWORD)pKey is used
typedef struct _FC_HashEnumPairs FC_HashEnumPairs;
struct _FC_HashEnumPairs
{
    const FC_HashEnumPairs*  pNext;
    const FC_HashEnumPairs*  pPrev;
    const TCHAR*             pKey;
    void*                    pVal;
};

typedef enum _FC_HASH_KEY
{
    FC_HASH_KEY_IS_DWORD,//the const TCHAR* pKey is interpreted as DWORD.
                         //the keySize passed to functions must be 0
                         //hash function is simply (DWORD)pKey
    FC_HASH_KEY_IS_PSZ,  //use string hash function 
    FC_HASH_KEY_IS_IPSZ  //use ignore case string hash function 
} FC_HASH_KEY;




//return NULL if out of memory or illegal flags
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
);

void FC_HashDelete(
    FC_HashTable* pHT
);
void FC_HashClear(
    FC_HashTable* pHT
);

//returns the enum pair if found by key or NULL:
const FC_HashEnumPairs* FC_HashFind(
    const FC_HashTable* pHT,  //table where to search 
    const TCHAR*        pKey  //either a DWORD or const TCHAR*
);

//returns false if key already exits, true if new key created:
//if key already exists it's value is replaced
//If out of memory returns -1
int FC_HashInsert(
    FC_HashTable* pHT,     //pointer to hash table 
    const TCHAR*  pKey,    //pointer to key or DWORD
    void*         pVal     //the 32 bit value
);

//returns 1 if key found false if not.
int FC_HashRemove(
    FC_HashTable* pHT,     //pointer to hash table 
    const TCHAR*  pKey     //pointer to key
);


//returns 0 if key already exits, 1 if new key created:
//if key already exists it's value is replaced only if 
//bReplace is ture. 
//If out of memory returns -1
int FC_HashInsertEx(
    FC_HashTable* pHT,     //pointer to hash table 
    const TCHAR*  pKey,    //pointer to key or DWORD
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
                           //it can be used as e.g. a string table.
);


//get a list of key/value pairs, thelist is maintained 
//in natural order of insert operations:
long  FC_HashCountPairs(const FC_HashTable* pHT);
const FC_HashEnumPairs* FC_HashFirstPair(const FC_HashTable* pHT);
const FC_HashEnumPairs* FC_HashLastPair (const FC_HashTable* pHT);
const TCHAR** FC_HashGetNewPZPSZKeys(
    const FC_HashTable* pHT, 
    int*                pLen,
    int                 bReverseOrder
);
const void**  FC_HashGetNewPZPValues(
    const FC_HashTable* pHT, 
    int*                pLen,
    int                 bReverseOrder
);
const void**  FC_HashGetNewPZPKeys(
    const FC_HashTable* pHT, 
    int*                pLen,
    int                 bReverseOrder
);



#ifdef __cplusplus
}
#endif










#ifdef __cplusplus
//----------- C++ member inline functions:
template<class V> inline FC_CHashStr<V>::FC_CHashStr(
   bool bIgnoreCase, bool bAllocKey, int initalSize, float threshold
)
{
    m_pHash = FC_HashCreate(bIgnoreCase ? FC_HASH_KEY_IS_IPSZ : FC_HASH_KEY_IS_PSZ, 
        bAllocKey, initalSize, threshold);
}
template<class V> inline FC_CHashStr<V>::~FC_CHashStr()
{
    FC_HashDelete(m_pHash);
}
template<class V> inline const FC_CHashStr<V>::Pair* FC_CHashStr<V>::getPair(
    const TCHAR* pszKey
)const
{
    return (const Pair*)FC_HashFind(m_pHash, pszKey);
}
template<class V> inline  bool FC_CHashStr<V>::get(
    const TCHAR* pszKey, V* ppVal
)const
{
    const Pair* p = (const Pair*)FC_HashFind(m_pHash, pszKey);
    if(p)
        *ppVal = p->pVal;
    return p!=NULL;

}
template<class V> inline V FC_CHashStr<V>::get(
    const TCHAR* pszKey
)const
{
    const Pair* p = (const Pair*)FC_HashFind(m_pHash, pszKey);
    return  p ? p->pVal : NULL;
}
template<class V> inline bool FC_CHashStr<V>::insert(
    const TCHAR* pszKey, V pVal
)
{
    return FC_HashInsert(m_pHash, pszKey, (void*)pVal)>0;
}
template<class V> inline bool FC_CHashStr<V>::insertEx(
    const TCHAR* pszKey, V pVal, bool bReplace, V* ppPrev
)
{
    return FC_HashInsertEx(m_pHash, pszKey, (void*)pVal, 
                    bReplace, (void**)ppPrev, NULL)>0;
}
template<class V> inline bool FC_CHashStr<V>::remove(
    const TCHAR* pszKey
)
{
    return FC_HashRemove(m_pHash, pszKey)!=0;
}
template<class V> inline const FC_CHashStr<V>::Pair* FC_CHashStr<V>::firstPair()const
{
    return (const Pair*)FC_HashFirstPair(m_pHash);
}
template<class V> inline const FC_CHashStr<V>::Pair* FC_CHashStr<V>::lastPair()const
{
    return (const Pair*)FC_HashLastPair(m_pHash);
}
template<class V> inline long      FC_CHashStr<V>::countPairs()const
{
    return FC_HashCountPairs(m_pHash);
}
template<class V> const TCHAR** FC_CHashStr<V>::allocKeyArray(int* pLen)const
{
    return FC_HashGetNewPZPSZKeys(m_pHash, pLen, 0);
}
template<class V> V* FC_CHashStr<V>::allocValArray(int* pLen)const
{
    return (V*)FC_HashGetNewPZPValues(m_pHash, pLen, 0);
}
template<class V> const TCHAR** FC_CHashStr<V>::allocKeyArrayReverse(int* pLen)const
{
    return FC_HashGetNewPZPSZKeys(m_pHash, pLen, 1);
}
template<class V> V* FC_CHashStr<V>::allocValArrayReverse(int* pLen)const
{
    return (V*)FC_HashGetNewPZPValues(m_pHash, pLen, 1);
}


template<class V> inline void FC_CHashStr<V>::clear()
{
    FC_HashClear(m_pHash);
}
template<class V> inline void FC_CHashStr<V>::deletePtrs()
{
    const Pair *pd,*p = (const Pair*)FC_HashFirstPair(m_pHash);
    while(p)
    {
        p = (pd = p)->pNext;
        delete(pd->pVal);
    }
    FC_HashClear(m_pHash);
}




template <class K, class V> inline 
 FC_CHashPtr<K,V>::FC_CHashPtr(
   int initalSize, float threshold)
{
    m_pHash = FC_HashCreate(FC_HASH_KEY_IS_DWORD, false,
        initalSize, threshold);
}
template <class K, class V> inline 
 FC_CHashPtr<K,V>::~FC_CHashPtr()
{
    FC_HashDelete(m_pHash);
}
template <class K, class V> inline 
 const FC_CHashPtr<K,V>::Pair* 
  FC_CHashPtr<K,V>::getPair(K pKey)const
{
    return (const Pair*)FC_HashFind(m_pHash, (const TCHAR*)pKey);
}
template <class K, class V> inline  
 bool FC_CHashPtr<K,V>::get(K pKey, V* ppVal)const
{
    const Pair* p = (const Pair*)FC_HashFind(m_pHash, (const TCHAR*)pKey);
    if(p)
        *ppVal = p->pVal;
    return p!=NULL;

}
template <class K, class V> inline 
 V FC_CHashPtr<K,V>::get(K pKey)const
{
    const Pair* p = (const Pair*)FC_HashFind(m_pHash, (const TCHAR*)pKey);
    return  p ? p->pVal : NULL;
}
template <class K, class V> inline 
 bool FC_CHashPtr<K,V>::insert(K pKey, V pVal)
{
    return FC_HashInsert(m_pHash, (const TCHAR*)pKey, (void*)pVal)>0;
}
template <class K, class V> inline 
 bool FC_CHashPtr<K,V>::insertEx(
    K pKey, V pVal, bool bReplace, V* ppPrev)
{
    return FC_HashInsertEx(m_pHash, (const TCHAR*)pKey, (void*)pVal, 
                    bReplace, (void**)ppPrev, NULL)>0;
}
template <class K, class V> inline 
 bool FC_CHashPtr<K,V>::remove(K pKey)
{
    return FC_HashRemove(m_pHash, (const TCHAR*)pKey)!=0;
}
template <class K, class V> inline 
 const FC_CHashPtr<K,V>::Pair* 
 FC_CHashPtr<K,V>::firstPair()const
{
    return (const Pair*)FC_HashFirstPair(m_pHash);
}
template <class K, class V> inline 
 const FC_CHashPtr<K,V>::Pair* 
 FC_CHashPtr<K,V>::lastPair()const
{
    return (const Pair*)FC_HashLastPair(m_pHash);
}
template <class K, class V> inline 
 long  FC_CHashPtr<K,V>::countPairs()const
{
    return FC_HashCountPairs(m_pHash);
}
template <class K, class V> inline 
 K* FC_CHashPtr<K,V>::allocKeyArray(int* pLen)const
{
    return (K*)FC_HashGetNewPZPKeys(m_pHash, pLen, 0);
}
template <class K, class V> inline 
 V* FC_CHashPtr<K,V>::allocValArray(int* pLen)const
{
    return (V*)FC_HashGetNewPZPValues(m_pHash, pLen, 0);
}
template <class K, class V> inline 
 K* FC_CHashPtr<K,V>::allocKeyArrayReverse(int* pLen)const
{
    return (K*)FC_HashGetNewPZPKeys(m_pHash, pLen, 1);
}
template <class K, class V> inline 
 V* FC_CHashPtr<K,V>::allocValArrayReverse(int* pLen)const
{
    return (V*)FC_HashGetNewPZPValues(m_pHash, pLen, 1);
}

template <class K, class V> inline 
 void FC_CHashPtr<K,V>::clear()
{
    FC_HashClear(m_pHash);
}
template <class K, class V> inline 
 void FC_CHashPtr<K,V>::deletePtrs()
{
    const Pair* p = (const Pair*)FC_HashFirstPair(m_pHash);
    for( ; p; p = p->pNext)
        delete(p->pVal);
    FC_HashClear(m_pHash);
}





inline FC_CHashStrTable::FC_CHashStrTable(int initalSize, float threshold)
{
    m_pHash = FC_HashCreate(FC_HASH_KEY_IS_PSZ, true, initalSize, threshold);
}
inline FC_CHashStrTable::~FC_CHashStrTable()
{
    FC_HashDelete(m_pHash);
}
inline const TCHAR* FC_CHashStrTable::registerString(const TCHAR* psz)const
{
    const TCHAR* pszReg;
    FC_HashInsertEx(m_pHash, psz, NULL, false, NULL, &pszReg);
    assert(pszReg);
    return pszReg;
}
#endif //__cplusplus


#endif // __FC_HASH_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: fc_hash.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 13  *****************
 * User: Ef           Date: 22.10.01   Time: 11:12
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * bug in inline code
 * 
 * *****************  Version 11  *****************
 * User: Die          Date: 25.04.01   Time: 20:45
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 10  *****************
 * User: Ef           Date: 9.04.01    Time: 10:25
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new function allocKey/ValReverse order
 * 
 * *****************  Version 9  *****************
 * User: Ef           Date: 1.03.01    Time: 20:53
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 8  *****************
 * User: Ef           Date: 12.02.01   Time: 13:34
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * homework changes, removed the debug dump
 * 
 * *****************  Version 7  *****************
 * User: Ef           Date: 6.02.01    Time: 16:23
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 6  *****************
 * User: Ef           Date: 31.01.01   Time: 16:31
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new function fc_cgarray::findLong
 * 
 * *****************  Version 5  *****************
 * User: Ef           Date: 29.01.01   Time: 21:12
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new functions in fc_cgarray
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 29.01.01   Time: 10:47
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * changed FC_StringGetPilePArt so that a pure file name does not 
 * return "", seem much better to work with.
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 23.01.01   Time: 9:57
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * changed oder of bIgnorecase and bAllocKey
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 22.01.01   Time: 11:01
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new fc_System.h, some minor changes
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 15.01.01   Time: 16:22
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 *==
 *----------------------------------------------------------------------------*
*/
