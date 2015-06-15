/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/stlmap.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: stlmap.h $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/stlmap.h $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                INC
 *
 * =COMPONENT           INC
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
 *== Secure thread-safe maps using STL map and list.
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.11.2000  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __STLMAP_H_
#define __STLMAP_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/**
* STL does not compile very cleanly with warning levels higher than 3.
* Thus the warning level will be forced to 3 during STL-Includes
* and restored back to the original level immediately after the include.
* Warning 4786 will be generated often by STL. Microsoft recommends 
* suppressing the warning this way rather than adjusting your warning level.
*/
//----  Aggregate Includes:   --------------------------------------*
#pragma warning( push,3 )
#pragma warning(disable:4786)

#include <map>
#include <list>
#include <algorithm>
using namespace std;

#pragma warning( pop ) 

#include "refobj.h"
//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#ifndef OBJMAN_OK
/**
* @def OBJMAN_OK - define for return value.
*/
#define OBJMAN_OK (0)
#endif

#ifndef OBJSTORE_OK
/**
* @def OBJSTORE_OK - define for return value.
*/
#define OBJSTORE_OK (0)
#endif

#ifndef OBJSTORE_ASSOC
/**
* @def OBJSTORE_ASSOC - define for return value.
*/
#define OBJSTORE_ASSOC (1)
#endif

#ifndef STLMAPASSERT
/**
* @def STLMAPASSERT - for other assert mechanisms replace only the define here.
*/
#define STLMAPASSERT(x) assert((x))
#endif
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

//------------------------------------------------------------------*
/**
 * class CSTLObjMan - maintains a map with a key object and a 
 * value object. The map is based on STL class map and protected by
 * a critical section encapsulated in class CExclusiveSection against
 * concurrent access (see refobj.h).
 * TItem represents the key class. The key for the map is unique.
 * TAssoc represents the value class. Only references of TAssoc will be
 * stored in the map. TAssoc class must support the methods 
 * AddRef()/Release(). A simple base class CRefObj will be provided
 * with refobj.h to facilitate implementation. Hence the template
 * class CSTLObjMan is useful for storing referenced objects, 
 * especially COM-Objects along with a key represented by TItem class.
 *
 * @see     class CRefObj (refobj.h), class CExclusiveSection (refobj.h), STL class
 *          map.
*/
template <class TItem,class TAssoc>class CSTLObjMan
{
private:
    map<TItem,TAssoc*> m_map;   /**< TItem is the key , TAssoc the value*/
    map<TItem,TAssoc*>::iterator m_mapiter; /**< map iterator */
    CExclusiveSection m_Guard;  /**< exclusive section for concurrent access*/
    long m_lObjectsIn;          /**< number of objects in the map*/
public:
    CSTLObjMan();
    virtual ~CSTLObjMan();

    int Assoc(TItem & item, TAssoc *pAssoc);
    TAssoc * Get(TItem & item);
    int Forget(TItem & item);
    void ForgetAll();
    long size();

    TAssoc** GetAssocList(); //get a copy of it as a list
    static void FreeAssocList(TAssoc **assocList); //free the copy

    void EnterCritical();
    void LeaveCritical();
};

/**
 * Ctor of CSTLObjMan.
 * Resets members.
 *        
*/
template <class TItem,class TAssoc>
CSTLObjMan<TItem,TAssoc>::CSTLObjMan(void)
    : m_map() , m_lObjectsIn(0)
{
}

/**
 * Dtor of CSTLObjMan.
 * Removes all objects from the map by calling method ForgetAll().
 *        
*/
template <class TItem,class TAssoc>
CSTLObjMan<TItem,TAssoc>::~CSTLObjMan(void)
{
    ForgetAll();
}

//------------------------------------------------------------------*
/**
 * Associates respectively writes a TAssoc referenced object into 
 * the map. TAssoc referenced object must be addref'd prior to
 * a call of this method. The method will not call AddRef() itself
 * before entering the object into the map. Checks first whether an
 * object of the key "item" is already in the map. In this case
 * the object in the map will be released prior to writing the new
 * object pAssoc into the map. The counterpart method Forget() will
 * remove the object with key "item" from the map.
 *
 * @param       item - TItem class, represents the key.
 * @param       pAssoc - reference to a TAssoc class, represents
 *              the value. TAssoc has to support methods Addref()/Release()
 *              and shall not be null.
 *
 * @see         Forget()
*/
template <class TItem,class TAssoc>
int CSTLObjMan<TItem,TAssoc>::Assoc(TItem & item, TAssoc *pAssoc)
{
    STLMAPASSERT(pAssoc);
    
    m_Guard.enter();
    TAssoc * pIn;
    /*  pAssoc must be AddRef'd
        check first if the item is inside the list
    */
    m_mapiter = m_map.find(item);
    if (m_mapiter != m_map.end()) {//found
        pIn = m_mapiter->second;
        STLMAPASSERT(pIn);
        pIn->Release();
        m_lObjectsIn--;
    }

    m_map[item] = pAssoc;
    m_lObjectsIn++;
    m_Guard.leave();
    return OBJMAN_OK;
}

//------------------------------------------------------------------*
/**
 * Gets the object of class TAssoc with the key "item" from the map.
 * Calls TAssoc::AddRef() prior to returning the TAssoc reference. User
 * has to call TAssoc::Release() after the user has used the TAssoc ref.
 * and doesn't need the reference any more. Returns a null reference
 * if a TAssoc object with the key "item" does not exist in the map. 
 *
 * @param       item - TItem class, represents the key.
 * @return      TAssoc reference, represents the value class or null
 *              reference if no object can be found with that key.
 *
*/
template <class TItem,class TAssoc>
TAssoc * CSTLObjMan<TItem,TAssoc>::Get(TItem & item)
{
    TAssoc *pAssoc = NULL;

    m_Guard.enter();
    m_mapiter = m_map.find(item);
    if (m_mapiter != m_map.end()) {//found
        pAssoc = m_mapiter->second;
        STLMAPASSERT(pAssoc);
        pAssoc->AddRef();
    }

    m_Guard.leave();
    return pAssoc;
}

//------------------------------------------------------------------*
/**
 * Removes the object of class TAssoc with the key "item" from the map.
 * Checks first whether a TAssoc reference with the key "item" can be
 * found in the map. If a TAssoc reference exists the key and object
 * will be removed from the map and the method calls TAssoc::Release()
 * prior to returning. The counterpart method Assoc() adds a new TAssoc
 * object to the map.
 *
 * @param       item - TItem class, represents the key.
 * @see         Assoc()
*/
template <class TItem,class TAssoc>
int CSTLObjMan<TItem,TAssoc>::Forget(TItem & item)
{
    TAssoc *pAssoc=NULL;

    m_Guard.enter();
    
    m_mapiter = m_map.find(item);
    if (m_mapiter != m_map.end()) {//found
        pAssoc = m_mapiter->second;
        STLMAPASSERT(pAssoc);
        m_map.erase(m_mapiter);
        m_lObjectsIn--;
    }
    else {
        pAssoc = NULL;
    }
    
    m_Guard.leave();
    if(pAssoc) {
        pAssoc->Release();
    }
    return OBJMAN_OK;
}

//------------------------------------------------------------------*
/**
 * Removes all objects of class TAssoc from the map. Calls TAssoc::
 * Release() for each object found in the map. Resets number of objects
 * in the map to 0. Will be called by the dtor of CSTLObjMan automatically.
 *
 * @see     ~CSTLObjMan() - dtor of CSTLObjMan
*/
template <class TItem,class TAssoc>
void CSTLObjMan<TItem,TAssoc>::ForgetAll()
{
    TAssoc* pAssoc;

    m_Guard.enter();

    for(m_mapiter = m_map.begin();m_mapiter != m_map.end(); m_mapiter++)
    {
        pAssoc = m_mapiter->second;
        STLMAPASSERT(pAssoc);
        if(pAssoc) {
            pAssoc->Release();
        }
    }

    m_map.clear();
    m_lObjectsIn = 0;
    m_Guard.leave();
}

//------------------------------------------------------------------*
/**
 * Returns the number of TAssoc value objects in the map.
 * 
 * @return      long - number of TAssoc value objects.
*/
template <class TItem,class TAssoc>
long CSTLObjMan<TItem,TAssoc>::size()
{
    m_Guard.enter();
    long l = m_lObjectsIn;
    m_Guard.leave();
    return l;
}

//------------------------------------------------------------------*
/**
 * Allocates an simple array for all TAssoc references of the map
 * from the heap. Calls operator new() to allocate the array. The size
 * of the array is the number of TAssoc references in the map + 1. 
 * The last entry of the array will be set to null to indicate the
 * end of the array. Each TAssoc reference of the map will be stored
 * in the array and TAssoc::AddRef() will be called for each valid
 * TAssoc reference. Use the counterpart method FreeAssocList() to free
 * the array of TAssoc references.
 * Note: This method is userful for implementing Enumerators
 * or getting a copy of the maps TAssoc references in an simple array.<br>
 * <b>For iteration the following code can be facilitated:</b><br>
 *      for (int i=0; array[i]; i++) { <br>
 *          array[i]->dosomething();<br>
 *      }<br>
 * @return      TAssoc** pointer to the array of TAssoc refs.
 *              or null in case of an out of memory situation.
 * @see         FreeAssocList().
*/
template <class TItem,class TAssoc>
TAssoc** CSTLObjMan<TItem,TAssoc>::GetAssocList()
{
    TAssoc** retval;
    TAssoc *pAssoc;
    size_t sizeList = 0;
    size_t sizeCount = 0;

    m_Guard.enter();
    
    sizeList = m_map.size();
    retval = new TAssoc*[sizeList+1];
    STLMAPASSERT(retval);
    if (!retval) {
        m_Guard.leave();
        return NULL;
    }

    for(m_mapiter = m_map.begin();m_mapiter != m_map.end(); m_mapiter++)
    {
        pAssoc = m_mapiter->second;
        STLMAPASSERT(pAssoc);
        if(pAssoc) {
            pAssoc->AddRef();
            retval[sizeCount] = pAssoc;
            sizeCount++;
        }
    }

    STLMAPASSERT(sizeCount == sizeList);
    retval[sizeCount] = NULL;
    m_Guard.leave();
    return retval;
}

//------------------------------------------------------------------*
/**
 * Frees respectively deletes the simple array of TAssoc references allocated
 * with GetAssocList(). Calls TAssoc::Release() for each valid TAssoc ref.
 * in the array. See GetAssocList() for allocation of the array.
 * 
 * @see         GetAssocList()
*/
template <class TItem,class TAssoc>
void CSTLObjMan<TItem,TAssoc>::FreeAssocList(TAssoc **assocList)
{
    STLMAPASSERT(assocList);
    for (int i=0; assocList[i]; i++)
        assocList[i]->Release();
    delete[]assocList;
}

//------------------------------------------------------------------*
/**
 * Enters the critical section used for protection of CSTLObjMan.
 * A direct call to this method is normally not necessary unless the
 * user wants to lock the map to prohibit concurrent access during a
 * larger operation. Don't forget to call LeaveCritical() to unlock the
 * map after the locked operation is finished.
 * 
 * @see LeaveCritical()
*/
template <class TItem,class TAssoc>
void CSTLObjMan<TItem,TAssoc>::EnterCritical()
{
    m_Guard.enter();
}

//------------------------------------------------------------------*
/**
 * Leaves the critical section used for protection of CSTLObjMan and
 * thereby unlocks the map. Concurrent access is possible after the
 * method has been called.
 * A direct call to this method is normally not necessary unless the
 * user wants to lock the map to prohibit concurrent access during a
 * larger operation. Don't forget to call EnterCritical() to lock the
 * map before the locked operation takes place and LeaveCritical() after
 * the locked operation is finished.
 * 
 * @see EnterCritical()
*/
template <class TItem,class TAssoc>
void CSTLObjMan<TItem,TAssoc>::LeaveCritical()
{
    m_Guard.leave();
}


//------------------------------------------------------------------*
/**
 * template class CSTLObjStore - maintains a protected list of  
 * referenced objects. The list is based on STL class list and protected by
 * a critical section encapsulated in class CExclusiveSection against
 * concurrent access (see refobj.h).
 * TItem represents the class stored in the list. 
 * Only references of TItem will be stored in the list. 
 * TItem class must support the methods AddRef()/Release(). A simple base
 * class CRefObj will be provided with refobj.h to facilitate 
 * implementation. Hence the template class CSTLObjStore is useful for
 * storing referenced objects, especially COM-Objects in a list that is
 * protected against concurrent access.
 *
 * @see     class CRefObj (refobj.h),class CExclusiveSection (refobj.h), STL class
 *          list.
 * 
*/
template <class TItem>class CSTLObjStore
{
private:
    list<TItem*> m_list;        /**< list of TItem referenced objects */
    list<TItem*>::iterator m_listiter; /**< list iterator */
    CExclusiveSection m_Guard;  /**< exclusive section for concurrent access*/
public:
    CSTLObjStore();
    virtual ~CSTLObjStore();
    int Add(TItem *item);
    int GetCountOf(const TItem *item);
    int GetCount();
    TItem * GetItem(const TItem *item);
    TItem * operator [] (int nPos);
    int Remove(const TItem *item);
    void RemoveAll();
    TItem **GetItemList();
    static void FreeItemList(TItem **itemList);
    void EnterCritical();
    void LeaveCritical();
};

/**
 * Ctor of CSTLObjStore.
 * Resets members.
 *        
*/
template <class TItem>
CSTLObjStore<TItem>::CSTLObjStore() : m_list()
{
}

/**
 * Dtor of CSTLObjStore.
 * Removes all objects from the map by calling method RemoveAll().
 * 
*/
template <class TItem>
CSTLObjStore<TItem>::~CSTLObjStore()
{
    RemoveAll();
}

//------------------------------------------------------------------*
/**
 * Adds a new entry to the list. item is a reference to the class 
 * stored in the list. The item referenced object must be addref'd 
 * prior to a call of this method. The method will not call TItem::AddRef()
 * itself before adding the object into the list. Call the counterpart
 * method Remove() to remove the object identified by the item reference
 * from the list.
 * 
 * @see     Remove()
*/
template <class TItem>
int CSTLObjStore<TItem>::Add(TItem *item)
{
    STLMAPASSERT(item);
    m_Guard.enter();
    m_list.push_back(item);
    m_Guard.leave();
    return OBJSTORE_OK;
}

//------------------------------------------------------------------*
/**
 * Gets the number of TItem objects in the list.
 * 
 * @return      int - number of objects in the list.
*/
template <class TItem>
int CSTLObjStore<TItem>::GetCount()
{
    return m_list.size();
}

//------------------------------------------------------------------*
/**
 * Gets the number of referenced TItem objects in the list that have
 * the same reference respectively address as the argument item. Hence
 * duplicate objects with the same address can be detected by this 
 * method.
 *
 * @param       item - reference resp. address of an TItem object.
 *              The list will be searched for TItem objects that have
 *              the same address as item.
 * @return      int - number of objects that have the same address as
 *              item.
*/
template <class TItem>
int CSTLObjStore<TItem>::GetCountOf(const TItem *item)
{
    int cnt=0;
    TItem* pItem;

    m_Guard.enter();
    for(m_listiter = m_list.begin(); m_listiter != m_list.end(); m_listiter++) {
        pItem = *m_listiter;
        if(pItem == item) {
            cnt++;
        }
    }
    m_Guard.leave();
    return cnt;
}

//------------------------------------------------------------------*
/**
 * Gets the referenced TItem object identified by the argument
 * reference item. Looks up the list to find a stored TItem object
 * that has the same address (or reference) as the argument item.
 * If a TItem object of the same address can be found the method calls
 * TItem::Addref() an returns the reference to the TItem object in the
 * list. A null reference will be returned if the object cannot be found
 * in the list.
 *
 * @param       item - reference respectively address of an TItem object.
 *              The list will be searched for a TItem object that has
 *              the same address as item.
 * @return      TItem - reference to an object found in the list or
 *              null if no object has been found.
 *
*/
template <class TItem>
TItem * CSTLObjStore<TItem>::GetItem(const TItem *item)
{
    TItem *pItem;

    m_Guard.enter();
    m_listiter = find(m_list.begin(), m_list.end(), item);
    if(m_listiter != m_list.end()) {
        if(*m_listiter == item) {
            pItem = *m_listiter;
            pItem->AddRef();
            m_Guard.leave();
            return pItem;
        }
    }
    m_Guard.leave();
    return NULL;
}

//------------------------------------------------------------------*
/**
 * Index operator to the CSTLObjStore - gets the referenced TItem 
 * object with the index of argument nPos from the list. Calls
 * TItem::Addref() to the referenced object of index nPos before the
 * method returns. A null reference will be returned if the index
 * nPos is out of bounds. Otherwise a reference to the TItem object
 * will be returned. Method can be used to iterate through the list.
 *
 * @param       nPos - index of the TItem object in the list which
 *              should be retrieved.
 * @return      reference to the TItem object of index nPos in the
 *              list or a null reference if nPos is out of bounds.
*/
template <class TItem>
TItem * CSTLObjStore<TItem>::operator [] (int nPos)
{
    TItem *pItem = NULL;
    int iIndex = 0;

    m_Guard.enter();
    if(nPos < 0 || nPos >= m_list.size()) {
        m_Guard.leave();
        return NULL;
    }

    for(m_listiter = m_list.begin(); m_listiter != m_list.end(); m_listiter++) {
        if(iIndex == nPos) {
            pItem = *m_listiter;
            STLMAPASSERT(pItem);
            break;
        }
        iIndex ++;
    }

    if(pItem) {
        pItem->AddRef();
    }

    m_Guard.leave();
    return pItem;
}

//------------------------------------------------------------------*
/**
 * Removes the referenced TItem object identified by the argument
 * reference item. Looks up the list to find a stored TItem object
 * that has the same address (or reference) as the argument item.
 * If a TItem object of the same address can be found the method removes
 * the object from the list and calls TItem::Release() prior to return.
 * An error will be returned if the object cannot be found
 * in the list.
 *
 * @param       item - reference respectively address of an TItem object.
 *              The list will be searched for a TItem object that has
 *              the same address as item.
 * @return      OBJSTORE_OK - success, <br> OBJSTORE_ASSOC if the object 
 *              cannot be found in the list.
 * @see         Add()
*/
template <class TItem>
int CSTLObjStore<TItem>::Remove(const TItem *item)
{
    TItem *removedItem = NULL;

    m_Guard.enter();

    m_listiter = find(m_list.begin(), m_list.end(), item);
    if(m_listiter != m_list.end()) {
        removedItem = *m_listiter;
        STLMAPASSERT(removedItem == item);
        m_list.erase(m_listiter);
    }

    m_Guard.leave();
    
    if (!removedItem) {
        STLMAPASSERT(removedItem);
        return OBJSTORE_ASSOC;
    }
    
    removedItem->Release();
    return OBJSTORE_OK;
}

//------------------------------------------------------------------*
/**
 * Removes all referenced TItem objects from the list.
 * Calls TItem::Release() for each TItem object removed from the list.
 * Will be called by the dtor of CSTLObjStore automatically.
 * 
 * @see     ~CSTLObjStore() - dtor of CSTLObjStore
*/
template <class TItem>
void CSTLObjStore<TItem>::RemoveAll()
{
    TItem* pItem;
    m_Guard.enter();

    for(m_listiter = m_list.begin(); m_listiter != m_list.end(); m_listiter++) {
        pItem = *m_listiter;
        if(pItem) {
            pItem->Release();
        }
    }
    m_list.clear();
    m_Guard.leave();
}

//------------------------------------------------------------------*
/**
 * Allocates an simple array for all TItem references of the list
 * from the heap. Calls operator new() to allocate the array. The size
 * of the array is the number of TItem references in the list + 1. 
 * The last entry of the array will be set to null to indicate the
 * end of the array. Each TItem reference of the list will be stored
 * in the array and TItem::AddRef() will be called for each valid
 * TItem reference. Use the counterpart method FreeItemList() to free
 * the array of TItem references.
 * Note: This method is userful for implementing Enumerators
 * or getting a copy of the lists TItem references in an simple array.<br>
 * <b>For iteration the following code can be facilitated:</b><br>
 *      for (int i=0; array[i]; i++) { <br>
 *          array[i]->dosomething();<br>
 *      }<br>
 * @return      TItem** pointer to the array of TItem refs.
 *              or null in case of an out of memory situation.
 * @see         FreeItemList().
*/
template <class TItem>
TItem **CSTLObjStore<TItem>::GetItemList()
{
    TItem **retval;
    TItem *pItem;
    size_t sizeList = 0;
    size_t sizeCount = 0;

    m_Guard.enter();
    
    sizeList = m_list.size();
    retval = new TItem*[sizeList+1];
    STLMAPASSERT(retval);
    if (!retval) {
        m_Guard.leave();
        return NULL;
    }

    for(m_listiter = m_list.begin(); m_listiter != m_list.end(); m_listiter++) {
        pItem = *m_listiter;
        STLMAPASSERT(pItem);
        if(pItem) {
            pItem->AddRef();
        }
        retval[sizeCount++] = pItem;
    }

    STLMAPASSERT(sizeCount == sizeList);
    retval[sizeCount] = NULL;
    m_Guard.leave();
    return retval;
}

//------------------------------------------------------------------*
/**
 * Frees respectively deletes the simple array of TItem references allocated
 * with GetItemList(). Calls TItem::Release() for each valid TItem ref.
 * in the array. See GetItemList() for allocation of the array.
 * 
 * @see         GetItemList()
*/
template <class TItem>
void CSTLObjStore<TItem>::FreeItemList(TItem **itemList)
{
    STLMAPASSERT(itemList);
    for (int i=0; itemList[i]; i++)
        itemList[i]->Release();
    delete[]itemList;
}

//------------------------------------------------------------------*
/**
 * Enters the critical section used for protection of CSTLObjStore.
 * A direct call to this method is normally not necessary unless the
 * user wants to lock the list to prohibit concurrent access during a
 * larger operation. Don't forget to call LeaveCritical() to unlock the
 * list after the locked operation is finished.
 * 
 * @see LeaveCritical()
*/
template <class TItem>
void CSTLObjStore<TItem>::EnterCritical()
{
    m_Guard.enter();
}

//------------------------------------------------------------------*
/**
 * Leaves the critical section used for protection of CSTLObjStore and
 * thereby unlocks the list. Concurrent access is possible after the
 * method has been called.
 * A direct call to this method is normally not necessary unless the
 * user wants to lock the list to prohibit concurrent access during a
 * larger operation. Don't forget to call EnterCritical() to lock the
 * list before the locked operation takes place and LeaveCritical() after
 * the locked operation is finished.
 * 
 * @see EnterCritical()
*/
template <class TItem>
void CSTLObjStore<TItem>::LeaveCritical()
{
    m_Guard.leave();
}


//------------------------------------------------------------------*
/**
 * class CSTLRefKeyMap - template for the reference object pointer 
 * key map protected against concurrent access with a critical section 
 * (encapsulated in class CExclusiveSection).<br>
 * The key class TItem is a reference pointer to an object with the methods
 * AddRef() and Release(). The value class TAssoc can be any data type. Classes
 * can be used as value as long as they support an assignment operator.<br>
 *
 * <b>1.)Key</b><br>
 * a reference to a class TItem, representing a referenced object.<br>
 * TItem has to support the methods TItem::AddRef() and TItem::Release().<br>
 * <b>2.)Value</b><br>
 * a class TAssoc of any datatype. TAssoc shall support the assignment
 * operator.<p>
 * A simple base class CRefObj will be provided with refobj.h to facilitate implementation.
 * 
 * @see     class CRefObj (refobj.h), class CExclusiveSection (refobj.h), STL class map. 
*/
template <class TItem,class TAssoc>class CSTLRefKeyMap
{
private:
    map<TItem *,TAssoc> m_map;  /**< TItem is the key , TAssoc the value - TItem is a pointer to a reference object*/
    map<TItem *,TAssoc>::iterator m_mapiter; /**< map iterator*/
    CExclusiveSection m_Guard;  /**< exclusive section for protection against concurrent access*/
    long m_lObjectsIn;          /**< number of objects in the map*/
public:
    //------------------------------------------------------------------*
    /**
     * Ctor of CSTLRefKeyMap.
     * Resets members.
     *        
    */
    CSTLRefKeyMap() : m_map() , m_Guard() , m_mapiter()
    {
        m_lObjectsIn = 0L;
    }
    
    //------------------------------------------------------------------*
    /**
     * Dtor of CSTLRefKeyMap.
     * Removes all objects from the map by calling method ForgetAll().
     *        
    */
    virtual ~CSTLRefKeyMap()
    {
        ForgetAll();
    }

    int Assoc(TItem*, TAssoc &);

    bool Get(TItem*,TAssoc &);
    bool Forget(TItem*);
    void ForgetAll();
    
    //------------------------------------------------------------------*
    /**
     * Returns the number of TAssoc value objects in the map.
     * 
     * @return      long - number of TAssoc value objects.
    */
    long size() 
    { return m_lObjectsIn; }


    //------------------------------------------------------------------*
    /**
     * Enters the critical section used for protection of CSTLRefKeyMap.
     * A direct call to this method is normally not necessary unless the
     * user wants to lock the map to prohibit concurrent access during a
     * larger operation. Don't forget to call LeaveCritical() to unlock the
     * map after the locked operation is finished.
     * 
     * @see LeaveCritical()
    */
    void EnterCritical()
    {
        m_Guard.enter();
    }
    
    //------------------------------------------------------------------*
    /**
     * Leaves the critical section used for protection of CSTLRefKeyMap and
     * thereby unlocks the map. Concurrent access is possible after the
     * method has been called.
     * A direct call to this method is normally not necessary unless the
     * user wants to lock the map to prohibit concurrent access during a
     * larger operation. Don't forget to call EnterCritical() to lock the
     * map before the locked operation takes place and LeaveCritical() after
     * the locked operation is finished.
     * 
     * @see EnterCritical()
    */
    void LeaveCritical()
    {
        m_Guard.leave();
    }
};

//------------------------------------------------------------------*
/**
 * Associates respectively writes a TAssoc object into 
 * the map. TItem key object must be addref'd prior to
 * a call of this method. The method will not call TItem::AddRef() itself
 * before entering the object into the map. Checks first whether an
 * object of the key "pItem" is already in the map. In this case
 * the TItem referenced object in the map will be released after
 * writing the new object Assoc into the map. The counterpart method 
 * Forget() will remove the object with key "pItem" from the map.
 *
 * @param       pItem - pointer to referenced TItem class, represents the key.
 *              TItem has to support methods Addref()/Release()
 *              and shall not be null.
 * @param       Assoc - reference to a TAssoc class, represents
 *              the value.
 *
 * @see         Forget()
*/
template <class TItem,class TAssoc>
int CSTLRefKeyMap<TItem,TAssoc>::Assoc(TItem* pItem, TAssoc & Assoc)
{
    bool bFound = false;
    STLMAPASSERT(pItem);

    m_Guard.enter();
    /*  pItem must be AddRef'd
        check first if the item is inside the list
    */
    m_mapiter = m_map.find(pItem);
    if (m_mapiter != m_map.end()) {//found
        bFound = true;
    }

    m_map[pItem] = Assoc;
    if(bFound == true) {
        pItem->Release();
        m_lObjectsIn--;
    }

    m_lObjectsIn++;
    m_Guard.leave();
    return OBJMAN_OK;
}

//------------------------------------------------------------------*
/**
 * Gets the object of class TAssoc with the key "pItem" from the map.
 * Returns false if the TItem pointer could not be found.
 * Hence a TAssoc object with the key "pItem" does not exist in the map. 
 *
 * @param       pItem - TItem class, represents the key.
 * @param       TAssoc reference, represents the value class (out).<br>
 * @return      true - object TAssoc found with key TItem pointer<br>
 * @return      false - object TAssoc not found with key TItem pointer
 *
*/
template <class TItem,class TAssoc>
bool CSTLRefKeyMap<TItem,TAssoc>::Get(TItem* pItem, TAssoc & assoc)
{
    STLMAPASSERT(pItem);

    m_Guard.enter();
    m_mapiter = m_map.find(pItem);
    if (m_mapiter != m_map.end()) {//found
        assoc = m_mapiter->second;
        m_Guard.leave();
        return true;
    }

    m_Guard.leave();
    return false;
}

//------------------------------------------------------------------*
/**
 * Removes the object of class TAssoc with the key "pItem" from the map.
 * Checks first whether a TAssoc reference with the key "pItem" can be
 * found in the map. If a TAssoc reference exists the key and object
 * will be removed from the map and the method calls TItem::Release()
 * for the referenced key object of type TItem prior to returning. 
 * The counterpart method Assoc() adds a new TAssoc object with the 
 * referenced key object TItem to the map.
 *
 * @param       pItem - pointer to TItem class, represents the key.
 * @return      true - TAssoc object with key TItem in the map. TAssoc
 *              successfully erased from the map. TItem object released.<br>
 *              false - TAssoc object with key TItem not in the map.
 * @see         Assoc()
*/
template <class TItem,class TAssoc>
bool CSTLRefKeyMap<TItem,TAssoc>::Forget(TItem* pItem)
{
    bool bFound = false;
    m_Guard.enter();
    
    m_mapiter = m_map.find(pItem);
    if (m_mapiter != m_map.end()) {//found
        m_map.erase(m_mapiter);
        m_lObjectsIn--;
        bFound = true;
    }
    
    m_Guard.leave();
    if(pItem) {
        pItem->Release();
    }

    return bFound;
}

//------------------------------------------------------------------*
/**
 * Removes all objects of class TAssoc and all keys of class TItem 
 * from the map. Calls TItem::Release() for each referenced key object 
 * of type TItem found in the map. Resets number of objects
 * in the map to 0. Will be called by the dtor of CSTLRefKeyMap 
 * automatically.
 *
 * @see     ~CSTLRefKeyMap() - dtor of CSTLRefKeyMap
*/
template <class TItem,class TAssoc>
void CSTLRefKeyMap<TItem,TAssoc>::ForgetAll()
{
    TItem* pItem;

    m_Guard.enter();
    if(!m_lObjectsIn) {//no objects inside
        m_Guard.leave();
        return;
    }

    for(m_mapiter = m_map.begin();m_mapiter != m_map.end(); m_mapiter++)
    {
        pItem = m_mapiter->first;
        STLMAPASSERT(pItem);
        if(pItem) {
            pItem->Release();
        }
    }

    m_map.clear();
    m_lObjectsIn = 0;
    m_Guard.leave();
}

//------------------------------------------------------------------*
/**
 * class CSTLKeyMap - template for a simple key map protected agains
 * concurrent access by a critical section encapsulated in class 
 * CExclusiveSection.
 * <b>1.) Key</b><br>
 * The key TItem can be any data type. Classes
 * can be used as value as long as they support an assignment operator
 * and copy ctor.<br>
 * <b>1.) Value</b><br>
 * The value TAssoc can be any data type. Classes
 * can be used as value as long as they support an assignment operator
 * and copy ctor.<p>
 * @see     class CExclusiveSection (refobj.h), STL class map. 
*/
template <class TItem,class TAssoc>class CSTLKeyMap
{
private:
    map<TItem,TAssoc> m_map;    /**< TItem is the key , TAssoc the value*/
    map<TItem,TAssoc>::iterator m_mapiter;  /**< map iterator*/
    CExclusiveSection m_Guard;  /**< exclusive section for protection against concurrent access*/
    long m_lObjectsIn;          /**< number of objects in the map*/
public:
    //------------------------------------------------------------------*
    /**
     * Ctor of CSTLKeyMap.
     * Resets members.
     *        
    */
    CSTLKeyMap() : m_map() , m_Guard() , m_mapiter()
    {
        m_lObjectsIn = 0L;
    }

    //------------------------------------------------------------------*
    /**
     * Dtor of CSTLKeyMap.
     * Removes all objects from the map by calling method ForgetAll().
     *        
    */
    virtual ~CSTLKeyMap()
    {
        ForgetAll();
    }

    int Assoc(TItem &, TAssoc &);

    bool Get(TItem & ,TAssoc &);
    bool Forget(TItem &);
    void ForgetAll();
    long size();

    //------------------------------------------------------------------*
    /**
     * Enters the critical section used for protection of CSTLKeyMap.
     * A direct call to this method is normally not necessary unless the
     * user wants to lock the map to prohibit concurrent access during a
     * larger operation. Don't forget to call LeaveCritical() to unlock the
     * map after the locked operation is finished.
     * 
     * @see LeaveCritical()
    */
    void EnterCritical()
    {
        m_Guard.enter();
    }

    //------------------------------------------------------------------*
    /**
     * Leaves the critical section used for protection of CSTLKeyMap and
     * thereby unlocks the map. Concurrent access is possible after the
     * method has been called.
     * A direct call to this method is normally not necessary unless the
     * user wants to lock the map to prohibit concurrent access during a
     * larger operation. Don't forget to call EnterCritical() to lock the
     * map before the locked operation takes place and LeaveCritical() after
     * the locked operation is finished.
     * 
     * @see EnterCritical()
    */
    void LeaveCritical()
    {
        m_Guard.leave();
    }
};

//------------------------------------------------------------------*
/**
 * Associates respectively writes a TAssoc object into 
 * the map. Checks first whether an object of the key "Item" is 
 * already in the map. In this case the number of objects in the map 
 * remains the same. Although the entry will be actualized with a the 
 * new Assoc object. The counterpart method Forget() will remove the 
 * object with key "Item" from the map.
 *
 * @param       Item - reference to TItem class, represents the key.
 * @param       Assoc - reference to a TAssoc class, represents
 *              the value.
 * @return      OBJMAN_OK.
 * @see         Forget()
*/
template <class TItem,class TAssoc>
int CSTLKeyMap<TItem,TAssoc>::Assoc(TItem & Item, TAssoc & Assoc)
{
    bool bFound = false;

    m_Guard.enter();
    //check first if the item is inside the list
    m_mapiter = m_map.find(Item);
    if (m_mapiter != m_map.end()) {//found
        bFound = true;
    }

    m_map[Item] = Assoc;
    if(bFound == true) {
        m_lObjectsIn--;
    }

    m_lObjectsIn++;
    m_Guard.leave();
    return OBJMAN_OK;
}

//------------------------------------------------------------------*
/**
 * Gets the object of class TAssoc with the key "Item" from the map.
 * Returns false if the Item key could not be found.
 * Hence a TAssoc object with the key "Item" does not exist in the map. 
 *
 * @param       Item - reference to TItem class, represents the key.
 * @param       TAssoc reference, represents the value class (out).<br>
 * @return      true - object TAssoc found with key TItem<br>
 * @return      false - object TAssoc not found with key TItem.
 *
*/
template <class TItem,class TAssoc>
bool CSTLKeyMap<TItem,TAssoc>::Get(TItem & Item, TAssoc & assoc)
{
    m_Guard.enter();
    m_mapiter = m_map.find(Item);
    if (m_mapiter != m_map.end()) {//found
        assoc = m_mapiter->second;
        m_Guard.leave();
        return true;
    }
    m_Guard.leave();
    return false;
}


//------------------------------------------------------------------*
/**
 * Removes the object of class TAssoc with the key "Item" from the map.
 * Checks first whether a TAssoc reference with the key "Item" can be
 * found in the map. If a TAssoc reference exists the key and object
 * will be removed from the map.
 * The counterpart method Assoc() adds a new TAssoc object with the 
 * key object TItem to the map.
 *
 * @param       Item - pointer to TItem class, represents the key.
 * @return      true - TAssoc object with key TItem in the map. TAssoc
 *              successfully erased from the map.<br>
 *              false - TAssoc object with key TItem not in the map.
 * @see         Assoc()
*/
template <class TItem,class TAssoc>
bool CSTLKeyMap<TItem,TAssoc>::Forget(TItem & Item)
{
    bool bFound = false;
    m_Guard.enter();
    
    m_mapiter = m_map.find(Item);
    if (m_mapiter != m_map.end()) {//found
        m_map.erase(m_mapiter);
        m_lObjectsIn--;
        bFound = true;
    }
    
    m_Guard.leave();
    return bFound;
}

//------------------------------------------------------------------*
/**
 * Removes all objects of class TAssoc and all keys of class TItem 
 * from the map. Resets number of objects in the map to 0. 
 * Will be called by the dtor of CSTLKeyMap automatically.
 *
 * @see     ~CSTLKeyMap() - dtor of CSTLKeyMap
*/
template <class TItem,class TAssoc>
void CSTLKeyMap<TItem,TAssoc>::ForgetAll()
{
    m_Guard.enter();
    if(!m_lObjectsIn) {//no objects inside
        m_Guard.leave();
        return;
    }

    m_map.clear();
    m_lObjectsIn = 0;
    m_Guard.leave();
}

//------------------------------------------------------------------*
/**
 * Returns the number of TAssoc value objects in the map.
 * 
 * @return      long - number of TAssoc value objects.
*/
template <class TItem,class TAssoc>
long CSTLKeyMap<TItem,TAssoc>::size()
{
    m_Guard.enter();
    long l = m_lObjectsIn;
    m_Guard.leave();
    return l;
}

#endif // __STLMAP_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: stlmap.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 12.02.01   Time: 10:05
 * Created in $/4Control/COM/softing/fc/inc
 * Initial rev.
 *==
 *----------------------------------------------------------------------------*
*/
