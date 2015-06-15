/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_cgarray.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_cgarray.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_cgarray.h $
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
 *==FC_CGArray class: template class used for a dynamically growing array of type <T>
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  03.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/
#ifndef FC_CGARRAY_H_
#define FC_CGARRAY_H_

#include <tchar.h>
#include "fc_defines.h"

#if !defined(__cplusplus) && !defined(FC_GARRAY_IMPL_C_)
#error "fc_garray.h can only be used in a C++ source file"
#endif



#ifdef __cplusplus
//FC_CGArray: generic dynamic array, can be used for all 
//types T that have no constructor/destructor.
//uses FC_LibNewMemory/FC_LibResizeMemory/FC_LibFreeMemory,
template <class T> class FC_CGArray
{
    //don't like: 
    FC_CGArray(const FC_CGArray&);
    FC_CGArray operator = (const FC_CGArray&);


    //NOTE:  FC_CGArray shall not be used if T has class type
    //or any construction /copy construction code.
    //FC_CGArray uses maaloc/free/realloc directry.
    //To get a compile time error in case some one tries to 
    //use FC_CGArray with a class the unionT type wraps a T
    //the C++ compiler will complain if T is not an aggregate type.
    typedef union unionT{T aT;} dontUseClasses;

protected:
    struct gaT
    {
        unsigned umax;//allocated elements ((type*)&data)[0..umax-1]
        unsigned used;//used elements ((type*)&data)[0..used-1]
        T        aT[32];//Note: size 32 only for easy debugging!
    } *m_p;

public:
//create/delete a GArray, supply an iPreAllocLen initial gues for performance:
    FC_CGArray(int iPreAllocLen=0);
    ~FC_CGArray();
   
    
//add a value to the end increment m_p->used and realloc if necessaray:
    int add(const T& val);         //returns the index of added elem >=0,
                                   //-1 if out of memory
    int push(const T& val);        //same as add, only other function name
    T*  addZero();                 //adds one elem memset to 0
                                   //returns pointer to this last element
    
//set value, does not resize: 
    void set(int i, const T& val); //set val at position 0<=i<getUsed()


//get array elements as [const] T& or [const] T* or the whole array:
    const T& get(int i)const;       //get const val& from position 0<=i<getUsed()
    T&       get(int i);            //get val& from position 0<=i<getUsed() 
    
    const T& getLast()const;        //get the last elem with index i<getUsed()-1 
    T&       getLast();             //assert if array is empty 
    
    T*       getAddrOf(int i);      //get val* from position 0<=i<getUsed() 
    const T* getAddrOf(int i)const; //get const val* from position 0<=i<getUsed() 
    
    T*       getElements();         //get the whole array content as writable
    const T* getCElements()const;   //get the whole array content as non writable

    T*       getZeroTermArray();    //appends one element memset to 0 without 
                                    //incrementing m_p->used (can be helpful to 
                                    //create zero terminated arrays of pointers)
    T*       getMemCopy()const;     //malloc new memory for used size
                                    //copy used portion into new memory and 
                                    //return the copy. The caller must free the mem.
    T*       getMem();              //same as getMemCopy but clears the array memory
                                    //to have 0 used and 0 alloc elements
    bool     pop(T& val);           //copies the last elem into out param val and 
                                    //decrements used by one, if used==0 returns 
                                    //false and nothing copied.
    
    
    //get/set used and alloc size information:
    int  getUsed()const;    //get the used portion of the array<T>
    void setUsed(int i);    //set the used portion to i<=getUsed() (doesn't realloc)
    void resetUsed();       //same as setUsed(0)
    int  getMaxAlloc()const;//get the currently allocated size  
    bool allocSize(int i);  //use allocSize to avoid realloc for performance.
                            //allocSize asserts if i<m_p->used.
    bool sizeToFit();       //force a realloc to shrink mem to only the used
                            //portion of the array, same as allocSize(used)

//operations for more than one element:
    void copyto(FC_CGArray<T>* pNew)const;        //clone this content to pNew
    void ownmemto(FC_CGArray<T>* pNewOwner);      //copy pointer to mem to pNewOwner, any mem 
                                                  //of pNewOwner is freed before and this will
                                                  //have used=alloc=0.
    void appendElems(const T* pElems, int nElems);//append nElems from const T* pElems
    void append(const FC_CGArray<T>* pOther);     //append content of pOther GArray<T>



//miscellaneous
    //if T is a pointer you may call deletePtrs in order to 
    //delete(aT[i]) for 0<=i<getUsed(). After deletePtrs m_p->used is set to 0.
    void deletePtrs();

    //linear search the array for m_p->aT[i=0..used-1]==l, <T> must be a type 
    //that can be compared with normal '=='  
    //returns the (first) index if found or -1 if not.
    int findElem(const T& val)const;

    //next two are deprecated!
    int findLong(long l);
    int findInt64(__int64 i64);

};


class FC_CStrList : private FC_CGArray<TCHAR*>
{
public:
    FC_CStrList(){}
    inline FC_CStrList(int iPreAllocLen) : FC_CGArray<TCHAR*>(iPreAllocLen){}
    inline ~FC_CStrList();

    inline void clear();

    int getUsed()const{return m_p->used;};

    //return the index of the new item or -1 if out of mem:
    inline int add(const TCHAR* psz);

    //get psz from position 0<=i<getUsed() 
    inline const TCHAR* get(int i)const
    {
        assert(((unsigned)i)<=m_p->used);
        return m_p->aT[i];
    }


    //get index of psz or -1 if not found:
    inline int getIndex(const TCHAR* psz)const;
    //get index of psz ignore case or -1 if not found:
    inline int getIndexI(const TCHAR* psz)const;
    
    //find psz:
    inline bool find(const TCHAR* psz)const;
    //find psz ignore case:
    inline bool findI(const TCHAR* psz)const;

    //if not found (ignorcase) add psz return the index of the new 
    //item or old item, return -1 if out of memory:
    inline int findOrAdd(const TCHAR* psz);
    inline int findOrAddI(const TCHAR* psz);

    //add another list to this list:
    inline bool addList(TCHAR*const pszList[], int iLen);
    inline bool addStrList(const FC_CStrList* pOther);
    
    //sort the list: qsort with _tcs(i)cmp
    inline void qsort();
    inline void qsortI();
};
#endif // __cplusplus





//----------------------------------- inlined members ----------------------------
//next functions should only be used by class FC_GArray<T> and FC_CStrList:
#if defined(__cplusplus) || defined(FC_GARRAY_IMPL_C_)
#ifdef __cplusplus
extern "C"{
#endif
typedef struct _FC_GArrayTYP
{
    unsigned umax;//allocated elements ((type*)&data)[0..umax-1]
    unsigned used;//used elements ((type*)&data)[0..used-1]
    char     data[32];//memblock to be casted to (type)
} FC_GArrayTYP;

FC_TOAPI FC_GArrayTYP* FC_TODECL FC_GArrayNew(
    size_t   elemSize,     //sizeof(type) size of one element
    unsigned uiPreAllocLen
); 
FC_TOAPI FC_GArrayTYP* FC_TODECL FC_GArrayFree(
    FC_GArrayTYP* pga
); 
FC_TOAPI int           FC_TODECL FC_GArrayResize(
    size_t         elemSize, //sizeof one element
    FC_GArrayTYP** ppga, 
    unsigned       umax
); 
FC_TOAPI int           FC_TODECL FC_GArrayAddElem(
    size_t         elemSize, //sizeof one element
    FC_GArrayTYP** ppga,
    const void*    pElem
); 
FC_TOAPI void* FC_TODECL FC_GArrayGetMemCopy(
    size_t         elemSize,
    FC_GArrayTYP** ppga
);
FC_TOAPI void* FC_TODECL FC_GArrayGetMem(
    size_t         elemSize,
    FC_GArrayTYP** ppga
);
FC_TOAPI void  FC_TODECL FC_StrListFree(
    TCHAR*const  ppsz[], 
    unsigned     uisize 
);
FC_TOAPI int  FC_TODECL  FC_StrListAppendList(
    FC_GArrayTYP**     ppga, //<type> must be TCHAR*
    const TCHAR*const  ppszadd[/*uiadd*/], 
    unsigned           uiadd
);
FC_TOAPI int   FC_TODECL FC_StrListAddStrDup(
    FC_GArrayTYP** ppga, //<type> must be TCHAR*
    const TCHAR*   psz
); 
FC_TOAPI int  FC_TODECL FC_StrListFind(
    const TCHAR*const  ppsz[], 
    unsigned           uisize, 
    const TCHAR*       pszSearch 
);
FC_TOAPI int  FC_TODECL FC_StrListFindI(
    const TCHAR*const  ppsz[], 
    unsigned           uisize, 
    const TCHAR*       pszSearch 
);
FC_TOAPI void  FC_TODECL FC_StrListQsort(
    TCHAR*             ppsz[], 
    unsigned           uisize 
);
FC_TOAPI void  FC_TODECL FC_StrListQsortI(
    TCHAR*             ppsz[], 
    unsigned           uisize 
);
#ifdef __cplusplus
}//extern "C"
#endif
#endif //defined(__cplusplus) || defined(FC_GARRAY_IMPL_C_)





#ifdef __cplusplus
//inlined FC_CGArray<T> methods:
template<class T> inline FC_CGArray<T>::FC_CGArray(int iPreAllocLen)
{
     m_p = (gaT*)FC_GArrayNew(sizeof(T), (unsigned)iPreAllocLen);
}
template<class T> inline FC_CGArray<T>::~FC_CGArray()
{
    m_p = (gaT*)FC_GArrayFree((FC_GArrayTYP*)m_p);
}
template<class T> inline bool FC_CGArray<T>::allocSize(int i)
{
    return FC_GArrayResize(sizeof(T), (FC_GArrayTYP**)&m_p, (unsigned)i)!=0;
}
template<class T> inline bool FC_CGArray<T>::sizeToFit()
{
    return FC_GArrayResize(sizeof(T), (FC_GArrayTYP**)&m_p, m_p->used)!=0;
}
template<class T> inline int FC_CGArray<T>::add(const T& val)
{
    return FC_GArrayAddElem(sizeof(T), (FC_GArrayTYP**)&m_p, &val);
}
template<class T> inline int FC_CGArray<T>::push(const T& val)
{
    return FC_GArrayAddElem(sizeof(T), (FC_GArrayTYP**)&m_p, &val);
}
template<class T> inline T* FC_CGArray<T>::addZero()
{
    FC_GArrayAddElem(sizeof(T), (FC_GArrayTYP**)&m_p, NULL);
    return &m_p->aT[m_p->used-1];
}
template<class T> inline void FC_CGArray<T>::set(int i, const T& val)
{
    assert(((unsigned)i)<m_p->used);
    m_p->aT[i] = val;
}
template<class T> inline const T& FC_CGArray<T>::get(int i)const
{
    assert(((unsigned)i)<m_p->used);
    return m_p->aT[i];
}
template<class T> inline T& FC_CGArray<T>::get(int i)
{
    assert(((unsigned)i)<m_p->used);
    return m_p->aT[i];
}
template<class T> inline const T& FC_CGArray<T>::getLast()const
{
    assert(m_p->used>0);
    return m_p->aT[m_p->used-1];
}
template<class T> inline T& FC_CGArray<T>::getLast()
{
    assert(m_p->used>0);
    return m_p->aT[m_p->used-1];
}
template<class T> inline T* FC_CGArray<T>::getAddrOf(int i)
{
    assert(((unsigned)i)<m_p->used);
    return m_p->aT+i;
}
template<class T> inline const T* FC_CGArray<T>::getAddrOf(int i)const
{
    assert(((unsigned)i)<m_p->used);
    return m_p->aT+i;
}
template<class T> inline int FC_CGArray<T>::getUsed()const
{
    return m_p->used;
}
template<class T> inline void FC_CGArray<T>::resetUsed()
{
    m_p->used = 0;
}
template<class T> inline void FC_CGArray<T>::setUsed(int i)
{
    assert(((unsigned)i)<=m_p->umax);
    m_p->used = i;
}
template<class T> inline T* FC_CGArray<T>::getElements()
{
    return m_p->aT;
}
template<class T> inline const T* FC_CGArray<T>::getCElements()const
{
    return m_p->aT;
}
template<class T> inline T* FC_CGArray<T>::getZeroTermArray()
{
    FC_GArrayAddElem(sizeof(T), (FC_GArrayTYP**)&m_p, NULL);
    m_p->used--;
    return m_p->aT;
}
template<class T> inline T* FC_CGArray<T>::getMemCopy()const
{
    return (T*)FC_GArrayGetMemCopy(sizeof(T), (FC_GArrayTYP**)&m_p);
}
template<class T> inline T* FC_CGArray<T>::getMem()
{
    return (T*)FC_GArrayGetMem(sizeof(T), (FC_GArrayTYP**)&m_p);
}
template<class T> inline bool FC_CGArray<T>::pop(T& val)
{
    return m_p->used>0? (val = m_p->aT[--m_p->used]), true : false;
}
template<class T> inline int FC_CGArray<T>::getMaxAlloc()const
{
    return (int)m_p->umax;
}
template<class T> inline void FC_CGArray<T>::copyto(FC_CGArray<T>* pNew)const
{
    assert(pNew != this);
    if(pNew != this)
    {
        if(pNew->m_p->umax >= m_p->umax || pNew->allocSize(m_p->umax))
        {
            pNew->m_p->used = m_p->used;
            memcpy(pNew->m_p->aT, m_p->aT, m_p->used*sizeof(T));
        }
    }
}
template<class T> inline void FC_CGArray<T>::ownmemto(FC_CGArray<T>* pNewOwner)
{
    assert(pNewOwner != this);
    if(pNewOwner != this)
    {
        gaT* pEmpty = pNewOwner->m_p;
        if(pEmpty->used)
            pEmpty = (gaT*)FC_GArrayFree((FC_GArrayTYP*)pEmpty);
        pNewOwner->m_p = this->m_p;
        this->m_p      = pEmpty;
    }
}
template<class T> inline void FC_CGArray<T>::appendElems(const T* pElems, int nElems)
{
    unsigned uiNew = m_p->used+nElems;
    if(m_p->umax >= uiNew || allocSize(uiNew))
    {
        memcpy(m_p->aT+m_p->used, pElems, nElems* sizeof(T));
        m_p->used = uiNew;
    }
}
template<class T> inline void FC_CGArray<T>::append(const FC_CGArray<T>* pOther)
{
    assert(pOther != this);
    if(pOther != this)
        appendElems(pOther->getCElements(), pOther->getUsed());
}
template<class T> inline void FC_CGArray<T>::deletePtrs()
{
    for(int i=m_p->used-1;i>=0;i--)
    {
        delete(m_p->aT[i]);
        m_p->aT[i] = NULL;
    }
    m_p->used = 0;
}
template<class T> inline int FC_CGArray<T>::findLong(long l)
{
    for(unsigned int i=0;i<m_p->used;i++)
    {
        if(m_p->aT[i]==l)
            return i;
    }
    return -1;
}
template<class T> inline int FC_CGArray<T>::findInt64(__int64 i64)
{
    for(unsigned int i=0;i<m_p->used;i++)
    {
        if(m_p->aT[i]==i64)
            return i;
    }
    return -1;
}
template<class T> inline int FC_CGArray<T>::findElem(const T& val)const
{
    for(unsigned int i=0;i<m_p->used;i++)
    {
        if(m_p->aT[i]==val)
            return i;
    }
    return -1;
}






//inlined FC_CStrList methods:
inline FC_CStrList::~FC_CStrList()
{
    FC_StrListFree(m_p->aT, m_p->used);
}
inline void FC_CStrList::clear()
{
    FC_StrListFree(m_p->aT, m_p->used);
    resetUsed();
}
inline int FC_CStrList::add(const TCHAR* psz)
{
    return FC_StrListAddStrDup((FC_GArrayTYP**)&m_p, psz);
}
inline int FC_CStrList::getIndex(const TCHAR* psz)const
{
    return FC_StrListFind(m_p->aT, m_p->used, psz);
}
inline int FC_CStrList::getIndexI(const TCHAR* psz)const
{
    return FC_StrListFindI(m_p->aT, m_p->used, psz);
}
inline bool FC_CStrList::find(const TCHAR* psz)const
{
    return getIndex(psz)>=0;
}
inline bool FC_CStrList::findI(const TCHAR* psz)const
{
    return getIndexI(psz)>=0;
}
inline int FC_CStrList::findOrAdd(const TCHAR* psz)
{
    int idx = getIndex(psz);
    return idx>=0 ? idx : add(psz);
}
inline int FC_CStrList::findOrAddI(const TCHAR* psz)
{
    int idx = getIndexI(psz);
    return idx>=0 ? idx : add(psz);
}
inline bool FC_CStrList::addList(TCHAR*const pszList[], int iLen)
{
    return FC_StrListAppendList((FC_GArrayTYP**)&m_p, pszList, iLen)!=0;
}
inline bool FC_CStrList::addStrList(const FC_CStrList* pOther)
{
    return addList(pOther->m_p->aT, pOther->getUsed());
}
inline void FC_CStrList::qsort()
{
    FC_StrListQsort(m_p->aT, m_p->used);
}
inline void FC_CStrList::qsortI()
{
    FC_StrListQsortI(m_p->aT, m_p->used);
}
#endif //__cplusplus



#endif // FC_CGARRAY_H_

/*
 *----------------------------------------------------------------------------*
 *  $History: fc_cgarray.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 2  *****************
 * User: Wi           Date: 26.09.03   Time: 13:11
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * changes for acyclic tasks
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 14  *****************
 * User: Ef           Date: 20.11.01   Time: 19:06
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * made findElem const member function
 * 
 * *****************  Version 13  *****************
 * User: Ef           Date: 22.10.01   Time: 19:36
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * added getLast method
 * 
 * *****************  Version 12  *****************
 * User: Ef           Date: 26.03.01   Time: 17:45
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * FC_CStrList  must inherti private from FC_CGArray !!
 * 
 * *****************  Version 11  *****************
 * User: Ef           Date: 16.03.01   Time: 9:58
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 10  *****************
 * User: Ef           Date: 26.02.01   Time: 10:48
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * corrected SWKE name
 * 
 * *****************  Version 9  *****************
 * User: Ef           Date: 22.02.01   Time: 14:33
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * added pup/pop methods
 * 
 * *****************  Version 8  *****************
 * User: Ef           Date: 21.02.01   Time: 13:21
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * fixed problem in sizeToFit
 * 
 * *****************  Version 7  *****************
 * User: Ef           Date: 12.02.01   Time: 13:35
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * homework changes
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
 * User: Ef           Date: 22.01.01   Time: 11:09
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * renamed file
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 22.01.01   Time: 11:01
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * new fc_System.h, some minor changes
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 5.01.01    Time: 17:08
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 * from V1.3 tools.dll, and some new features
 *==
 *----------------------------------------------------------------------------*
*/
