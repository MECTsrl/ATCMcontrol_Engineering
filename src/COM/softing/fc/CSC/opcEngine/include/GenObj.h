//-----------------------------------------------------------------------------
//                OPC ENGINE              OPC SESSION                         |
//                                                                            |
//  Filename    : GENOBJ.H                                                    |   
//  Version     : 2.00.0.00.release       2.00.0.00.release                   |
//  Date        : 29-March-1999           29-March-1999                       |
//                                                                            |
//  Description : Basic object classes                                        | 
//                - GenericObject: root class for most toolbox objects        |
//                - GenericPointer: smart pointer template class for          |
//                                  GenericObject			                  |
//                - GenericList: list template class                          |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  08/10/98  RT        added session type defines                            |
//  08/14/98  RT        added state support                                   |
//  01/05/99  RT        added GenericList class                               |
//  ------------------------------------------------------------------------- |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _GENOBJ_H_
#define _GENOBJ_H_

#pragma warning( disable : 4251 ) 
#pragma warning( disable : 4786 ) 

#ifndef _LIB
// compile as server or client DLL
#ifdef OPCENGINE
#define OPCENGINE_EXPORT __declspec(dllexport)
#else
#define OPCENGINE_EXPORT __declspec(dllimport)
#endif
#else
#define OPCENGINE_EXPORT 
#endif

//-----------------------------------------------------------------------------
// GenericObject                                                              |
//-----------------------------------------------------------------------------

// possible object types
#define GENOBJ_TYPE_ENGINE		 (0x00010000)
#define GENOBJ_TYPE_CALLBACK	 (0x00020000)
#define GENOBJ_TYPE_REQUEST		 (0x00040000)
#define GENOBJ_TYPE_CACHE		 (0x00080000)
#define GENOBJ_TYPE_TRANSACTION	 (0x00100000)
#define GENOBJ_TYPE_CONFIG		 (0x01000000)
#define GENOBJ_TYPE_OTHER		 (0x08000000)

#define GENOBJ_TYPE_ELEMENT		 (0x80000000)

#define GENOBJ_TYPE_LEAF			(GENOBJ_TYPE_ELEMENT | 0x1000)  
#define GENOBJ_TYPE_BRANCH			(GENOBJ_TYPE_ELEMENT | 0x8000)
#define GENOBJ_TYPE_SERVER			(GENOBJ_TYPE_BRANCH  | 0x0001)
#define GENOBJ_TYPE_GROUP			(GENOBJ_TYPE_BRANCH  | 0x0002)
#define GENOBJ_TYPE_OPC_ROOT		(GENOBJ_TYPE_BRANCH  | 0x0004)
#define GENOBJ_TYPE_NODE			(GENOBJ_TYPE_BRANCH  | 0x0800)
#define GENOBJ_TYPE_NS_ROOT			(GENOBJ_TYPE_BRANCH  | 0x0400)
#define GENOBJ_TYPE_SESSION			(GENOBJ_TYPE_BRANCH  | 0x0010)
#define GENOBJ_TYPE_ITEM_TAG		(GENOBJ_TYPE_LEAF    | 0x0800)
#define GENOBJ_TYPE_TAG				(GENOBJ_TYPE_ITEM_TAG| 0x0400)
#define GENOBJ_TYPE_TAG_PROPERTY	(GENOBJ_TYPE_ITEM_TAG| 0x0200)
#define GENOBJ_TYPE_ITEM			(GENOBJ_TYPE_LEAF    | 0x0001)


class OPCENGINE_EXPORT GenericObject
{
public:
	GenericObject(IN DWORD objType);

	// object type
	DWORD getObjectType(void) const;
	BOOL is(DWORD objectType) const;

	// reference counting
	virtual void addRef(void);	
	virtual LONG release(void);	
	LONG getRefCount(void) const; 

	// object identity
	virtual CString getKey(void);
	virtual BOOL hasKey(LPCTSTR key);

	// object state
	BYTE getObjectState(void) const;
	virtual BOOL setObjectState(IN BYTE state);
	BYTE getTargetObjectState(void) const;
	virtual BOOL setTargetObjectState(IN BYTE state);

	// object trace strings
	virtual LPCTSTR getObjTraceId(void);
	CString getObjTypeString(void);

	// application specific data
	BYTE getUserData(void) const;
	void setUserData(IN BYTE userData);

protected:
	virtual ~GenericObject(void);

	// object state callbacks
	virtual void onSetObjectState(IN BYTE newState);
	virtual void onSetTargetObjectState(IN BYTE newState);

	LONG m_refCount;					// reference count
	DWORD m_objectType;					// object type
	BYTE m_objectState;					// object state
	BYTE m_objectStateTarget;			// target state of the object
	BYTE m_userData;					// application specific data
	CString m_objectTraceId;			// object trace id string 
	POSITION m_objectTracePos;			// object trace object log position
}; // GenericObject


inline DWORD GenericObject::getObjectType(void) const
{ return m_objectType; }

inline BOOL GenericObject::is(DWORD objectType) const
{ return ((m_objectType & objectType) == objectType); }

inline LONG GenericObject::getRefCount(void) const
{ return m_refCount; }

inline BYTE GenericObject::getObjectState(void) const
{ return m_objectState; }

inline BYTE GenericObject::getTargetObjectState(void) const
{ return m_objectStateTarget; }

inline BYTE GenericObject::getUserData(void) const
{ return m_userData; }

inline void GenericObject::setUserData(IN BYTE userData)
{ m_userData = userData; }



//-----------------------------------------------------------------------------
// GenericPointer                                                             |
//-----------------------------------------------------------------------------

template <class T> class GenericPointer
{
public:
	// constructors
	GenericPointer(void) 
	{ p = NULL; }
	GenericPointer(T* p_)
	{
		p = p_;
	}
	GenericPointer(IN const GenericPointer<T>& p_)
	{
		if ((p = p_.p) != NULL)
			p->addRef();
	}
	
	// destructor
	~GenericPointer(void) 
	{ 
		if (p) 
			p->release(); 
	}

	// reference count methods
	LONG release(void) 
	{ 
		LONG ret = -1;
		if (p) 
			ret = p->release(); 
		p = NULL; 
		return ret;
	}
	void addRef(void) 
	{ 
		if (p) 
			p->addRef(); 
	}

	// operators
	operator T*(void) 
	{ return (T*)p; }
	operator T**(void) 
	{ return (T**)&p; }
	T& operator*(void) 
	{ return *p; }
	T** operator&() 
	{ return &p; }
	T* operator->() 
	{ return p; }
	GenericPointer& operator=(IN T* p_) 
	{ 
		if (p)
			p->release(); 
		p = p_;
		return *this; 
	}
	bool operator!() 
	{ return (p == NULL); }

private:
	T* p;
}; // GenericPointer




//-----------------------------------------------------------------------------
// GenericList                                                                |
//-----------------------------------------------------------------------------

#define GENLIST_CREATE_LIST	0x01
#define GENLIST_CREATE_MAP	0x02

template <class T> class GenericList
{
public:
	enum lockState
	{
		unlocked,
		locked,
		notlocking
	};

	// constructors
	GenericList(void) 
	{
		m_list = NULL; 
		m_map = NULL; 
		m_lock = NULL; 
		m_locked = unlocked;
	}

	GenericList(IN const GenericList<T>& list)
	{
		m_list = list.m_list;
		m_map = list.m_map; 
		m_lock = list.m_lock;
		m_locked = unlocked;
	}

	GenericList(IN GenericList<T> *list)
	{
		m_list = list->m_list;
		m_map = list->m_map; 
		m_lock = list->m_lock;
		m_locked = unlocked;
	}

	GenericList(IN CTypedPtrList<CPtrList, T*> *list, IN OPTIONAL CSyncObject *lock = NULL)
	{
		m_list = list;
		m_map = NULL; 
#ifdef _AFXDLL
		m_lock = (CCriticalSection *)lock;
#else
		m_lock = (CMutex *)lock;
#endif
		m_locked = unlocked;
	}

	// destructor
	~GenericList(void) 
	{
		if ((m_lock) && (m_locked == locked))
			unlock();
	}

	// operators
	GenericList& operator=(IN GenericList<T>* list) 
	{ 
		if (list)
		{
			unlock();
			m_list = list->m_list;
			m_map = list->m_map; 
			m_lock = list->m_lock;
			m_locked = unlocked;
		}
		return *this; 
	}

	bool operator!() 
	{ return ((m_list == NULL) && (m_map == NULL)); }

	operator POSITION(void) 
	{ return getStartPosition(); }

	operator T*(void) 
	{ if (m_list) return m_list->GetHead(); else return NULL; }

	operator CTypedPtrList<CPtrList, T*>*(void) 
	{ return m_list; }

	// create list and map
	BOOL create(IN OPTIONAL BYTE what = GENLIST_CREATE_LIST, 
			IN OPTIONAL int blockSize = 10, IN OPTIONAL int mapHashTableSize = 17);

	// free list and map
	void free(void);	

	// free and remove all list and map entries 
	void clear(void);

	// remove all list and map entries
	void removeAll(void)
	{ lock();
	  if (m_map) m_map->RemoveAll(); 
	  if (m_list) m_list->RemoveAll(); 
	}

	// release object
	void release(IN T *obj)
	{ if (m_map) obj->release(); 
	  if (m_list) obj->release(); 
	}

	// get start position
	POSITION getStartPosition(void)
	{ lock();
	  if (m_map) return m_map->GetStartPosition(); 
	  if (m_list) return m_list->GetHeadPosition(); 
	  return NULL; }

	// get entry count
	LONG getCount(void)
	{ if (m_map) return (LONG)m_map->GetCount(); 
	  if (m_list) return (LONG)m_list->GetCount(); 
	  return 0; }

	// check if list or map is empty
	BOOL isEmpty(void)
	{ if (m_map) return m_map->IsEmpty(); 
	  if (m_list) return m_list->IsEmpty(); 
	  return 0; }

	// get next entriy
	T* getNext(IN OUT POSITION& pos);

	// add entry
	BOOL add(IN T *obj, IN OPTIONAL LONG index = -1);

	// find entry
	T *find(IN LPCTSTR key);
	T *find(IN DWORD key)
	{ CString sKey; sKey.Format(_T("%lu"), key); return find((LPCTSTR)sKey); }

	// remove entry
	BOOL remove(IN T* obj);
	BOOL remove(IN LPCTSTR key);
	BOOL remove(IN DWORD key)
	{ CString sKey; sKey.Format(_T("%lu"), key); return remove((LPCTSTR)sKey); }

	// get list
	CTypedPtrList<CPtrList, T*> *getList(void)
	{ return m_list; } 

	// get map
	CTypedPtrMap<CMapStringToPtr, CString, T*> *getMap(void)
	{ return m_map; } 

	// get lock object
	CSyncObject *getLockObject(void)
	{ return m_lock; } 

#ifndef _AFXDLL
	HANDLE getLockHandle(void)
	{ if (m_lock) return (HANDLE)(*m_lock); else return INVALID_HANDLE_VALUE; } 
#endif

	// lock list and map
	void lock(void)
	{ if ((m_lock) && (m_locked == unlocked)) { m_lock->Lock(); m_locked = locked;} }

	// unlock list and map
	void unlock(void)
	{ if ((m_lock) && (m_locked == locked)) {m_lock->Unlock(); m_locked = unlocked;} }

	// prevent locking
	void dontLock(void)
	{ unlock(); m_locked = notlocking; }

private:
	CTypedPtrList<CPtrList, T*> *m_list;				// list
	CTypedPtrMap<CMapStringToPtr, CString, T*> *m_map;	// map

#ifdef _AFXDLL
	CCriticalSection *m_lock;							// lock
#else
	CMutex *m_lock;										// lock
#endif
	BYTE m_locked;										// is locked?
}; // GenericList


//-----------------------------------------------------------------------------
// create
//
// - creates list, map and lock object
//
// returns:
//		TRUE  - all created
//		FALSE - error during creation
//
template <class T> BOOL GenericList<T>::create(
	IN OPTIONAL BYTE what,				// what to create (list?, map?)
	IN OPTIONAL int blockSize,			// block size of list and map
	IN OPTIONAL int mapHashTableSize)	// hash table size of map
{
	BOOL ok = TRUE;

	if (what & GENLIST_CREATE_LIST)
	{
		m_list = new CTypedPtrList<CPtrList, T*>(blockSize);
		ok &= (m_list != NULL);
	}

	if (what & GENLIST_CREATE_MAP)
	{
		m_map = new CTypedPtrMap<CMapStringToPtr, CString, T*>(blockSize);
		if (m_map)
		{
			// set hash table size
			m_map->InitHashTable(mapHashTableSize, FALSE);
		}
		ok &= (m_map != NULL);
	}

#ifdef _AFXDLL
	m_lock = new CCriticalSection();
#else
	m_lock = new CMutex();
#endif
	ok &= (m_lock != NULL);
	return ok;
} // create

 
//-----------------------------------------------------------------------------
// free
//
// - free list, map and lock object
//
template <class T> void GenericList<T>::free(void)
{
	lock();
	clear();
	if (m_list)
	{
		delete m_list;
		m_list = NULL;
	}
	if (m_map)
	{
		delete m_map;
		m_map = NULL;
	}
	unlock();
	if (m_lock)
	{
		delete m_lock;
		m_lock = NULL;
	}		
} // free


//-----------------------------------------------------------------------------
// free
//
// - free all entries of the list and map
// - remove all entries of the list and map
//
template <class T> void GenericList<T>::clear(void)
{
	if (m_list)
	{
		POSITION pos;
		T *obj;

		lock();
		pos = m_list->GetHeadPosition();
		while(pos)
		{
			obj = (T *)m_list->GetNext(pos);		
			obj->release();
		}
		m_list->RemoveAll();
		unlock();
	}
	if (m_map)
	{
		POSITION pos;
		CString key;
		T *obj;

		lock();
		pos = m_map->GetStartPosition();
		while(pos)
		{
			m_map->GetNextAssoc(pos, key, obj);		
			obj->release();
		}
		m_map->RemoveAll();
		unlock();
	}
} // clear


//-----------------------------------------------------------------------------
// add
//
// - add an object to the list and map
//
// returns:
//		TRUE  - added
//		FALSE - can't add object
//
template <class T> BOOL GenericList<T>::add(
	IN T *obj,				// object
	IN OPTIONAL LONG index)	// index to add object into list
{
	BOOL ok = FALSE;
	if (m_list)
	{
		lock();
		obj->addRef();

		if (index >= 0)
		{
			POSITION pos;

			pos = m_list->FindIndex(index);
			if (pos != NULL) 
			{
				pos = m_list->InsertBefore(pos, obj);
				ok = TRUE; 
			}

		}

		if (!ok)
		{
			m_list->AddTail(obj);
			ok = TRUE; 
		}
	}
	if (m_map)
	{
		lock();
		obj->addRef();
		m_map->SetAt(obj->getKey(), obj);
		ok = TRUE; 
	}

	return ok;
} // add


//-----------------------------------------------------------------------------
// getNext
//
// - get the next object of the list or map
//
// returns:
//		pointer to object 
//		NULL - no list and map 
//
template <class T> T* GenericList<T>::getNext(
	IN OUT POSITION& pos)	// current position
{
	if (m_map) 
	{
		CString key;
		T *obj;
		m_map->GetNextAssoc(pos, key, obj);
		return obj;
	}

	if (m_list) 
		return m_list->GetNext(pos); 

	return NULL; 
} // getNext


//-----------------------------------------------------------------------------
// find
//
// - find a object in the list or map
//
// returns:
//		pointer to object 
//		NULL - object not in list and map
//
template <class T> T *GenericList<T>::find(
	IN LPCTSTR key)	// key
{
	if (m_map)
	{
		T *obj = NULL;

		lock();
		if (m_map->Lookup(key, obj))
		{
			obj->addRef();
			return obj;
		}
	}
	if (m_list)
	{
		POSITION pos;
		T *obj;

		lock();
		pos = m_list->GetHeadPosition();
		while(pos)
		{
			obj = (T *)m_list->GetNext(pos);		
			if (obj->hasKey(key))
			{
				obj->addRef();
				return obj;
			}
		}
	}
	return NULL;
} // find


//-----------------------------------------------------------------------------
// remove
//
// - remove object form the list and map
//
// returns:
//		TRUE  - object removed
//		FALSE - can't remove object
//
template <class T> BOOL GenericList<T>::remove(
	IN T* obj)	// object pointer
{
	BOOL ok = FALSE;

	if (m_list)
	{
		POSITION pos;

		lock();

		if (pos = m_list->Find(obj))
		{
			m_list->RemoveAt(pos);
			obj->release();
			ok = TRUE;
		}
	}
	if (m_map)
	{
		ok = remove(obj->getKey());
	}
	return ok;
} // remove


//-----------------------------------------------------------------------------
// remove
//
// - remove object form the list and map
//
// returns:
//		TRUE  - object removed
//		FALSE - can't remove object
//
template <class T> BOOL GenericList<T>::remove(
	IN LPCTSTR key)	// object key
{
	BOOL ok = FALSE;

	if (m_list)
	{
		POSITION pos;
		POSITION actPos;
		T *obj;

		lock();
		pos = m_list->GetHeadPosition();
		while(pos)
		{
			actPos = pos;
			obj = (T *)m_list->GetNext(pos);		
			if (obj->hasKey(key))
			{	
				m_list->RemoveAt(actPos);
				obj->release();
				ok = TRUE;
			}
		}
	}
	if (m_map)
	{
		T *obj = NULL;

		lock();
		if (m_map->Lookup(key, obj))
		{	
			m_map->RemoveKey(key);
			obj->release();
			ok = TRUE;
		}
	}
	return ok;
} // remove

#endif
