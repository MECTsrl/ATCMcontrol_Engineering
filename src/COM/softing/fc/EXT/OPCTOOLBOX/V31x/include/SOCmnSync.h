//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOCmn                              |
//                                                                            |
//  Filename    : SOCmnSync.h                                                 |
//  Version     : 3.11.release                                                |
//  Date        : 23-September-2002                                           |
//                                                                            |
//  Description : Synchornisation classes                                     |
//                - SOCmnSync: Standard synchronisation class of toolkits     |
//                - SOCmnMutex: Mutex synchronisation class                   |
//                - SOCmnSingleLock: Template for locking of syncs            |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOCMNSYNC_H_
#define _SOCMNSYNC_H_

#include "SOCmnList.h"

#pragma pack(push,4)


//-----------------------------------------------------------------------------
// SOCmnSync                                                                  |
//-----------------------------------------------------------------------------

class SOCmnObjectLock;

class SOCMN_EXPORT SOCmnSync
{
friend SOCmnObjectLock;
public:
	SOCmnSync();
	~SOCmnSync();

	// gain access to the sync object
	BOOL Lock(void);
	BOOL lock(void);

	// release access to the sync object
	BOOL Unlock(void);
	BOOL unlock(void);

protected:
	LONG m_lockCount;		// current number of locks
	HANDLE m_waitEvent;		// wait event if other thread owns sync
	DWORD m_ownThreadId;	// id of owning thread
	LONG m_recurseCount;	// recurse lock calls of the thread
}; // SOCmnSync

inline BOOL SOCmnSync::Lock(void)
{ return lock(); }

inline BOOL SOCmnSync::Unlock(void)
{ return unlock(); }




//-----------------------------------------------------------------------------
// SOCmnMutex                                                                 |
//-----------------------------------------------------------------------------

class SOCMN_EXPORT SOCmnMutex
{
public:
	SOCmnMutex();
	~SOCmnMutex();

	// gain access to the mutex object
	BOOL lock(IN OPTIONAL DWORD timeout = INFINITE);

	// release access to the mutex object
	BOOL unlock(void);

protected:
	HANDLE m_mutex;	// handle of system mutex
}; // SOCmnMutex




//-----------------------------------------------------------------------------
// SOCmnSingleLock                                                            |
//-----------------------------------------------------------------------------

template <class T> class SOCmnSingleLock
{
public:
	SOCmnSingleLock(IN T *sync)	{ m_sync = sync; lock(); }

	~SOCmnSingleLock() { unlock();	}

	// gain access to the sync object
	BOOL lock(void)	{ if (m_sync) return m_sync->lock(); else return FALSE; }

	// release access to the sync object
	BOOL unlock(void) { if (m_sync) return m_sync->unlock(); else return FALSE; }

protected:
	T *m_sync;
}; // SOCmnSingleLock




//-----------------------------------------------------------------------------
// SOCmnObjectLock                                                            |
//-----------------------------------------------------------------------------

#define SOCMNOBJECTLOCK_ARRAY_MASK 0x01FF

class SOCmnObject;

class SOCMN_EXPORT SOCmnObjectLock
{
public:
	SOCmnObjectLock();
	~SOCmnObjectLock();

	// gain access to the sync object
	static BOOL lock(SOCmnObject *pObj) { return getObjectLock(pObj)->lock(); }

	// release access to the sync object
	static BOOL unlock(SOCmnObject *pObj) { return getObjectLock(pObj)->unlock(); }

	static SOCmnSync *getObjectLock(IN SOCmnObject *pObj);
	static SOCmnSync *getObjectLockV(IN void *pVoid);

	static void releaseObjectLock(IN SOCmnObject *pObj, IN SOCmnSync *sync);
	static void releaseObjectLockV(IN void *pVoid, IN SOCmnSync *sync);

protected:
	SOCmnSync m_lockSync[SOCMNOBJECTLOCK_ARRAY_MASK + 1];
	void *m_lockObject[SOCMNOBJECTLOCK_ARRAY_MASK + 1];
	WORD m_lockRecurse[SOCMNOBJECTLOCK_ARRAY_MASK + 1];
	SOCmnList<SOCmnSync> m_extensionList;
	SOCmnSync *m_lock;

	SOCmnSync *doGetObjectLock(IN SOCmnObject *pObj, IN void *pVoid);
	void doReleaseObjectLock(IN SOCmnObject *pObj, IN void *pVoid, IN SOCmnSync *sync);
}; // SOCmnObjectLock

void createObjectLock(void);
void destroyObjectLock(void);



//-----------------------------------------------------------------------------
// SOCmnSingleObjectLock                                                      |
//-----------------------------------------------------------------------------

template <class T> class SOCmnSingleObjectLock
{
public:
	SOCmnSingleObjectLock(IN SOCmnObject *pObj)	{ m_obj = pObj; m_v = NULL; m_sync = SOCmnObjectLock::getObjectLock(pObj); lock(); }
	SOCmnSingleObjectLock(IN SOCmnSync *sync, IN void *pV)	{ m_v = pV; m_obj = NULL; m_sync = sync; lock(); }

	~SOCmnSingleObjectLock() { unlock(); if (m_obj) SOCmnObjectLock::releaseObjectLock(m_obj, m_sync); else  SOCmnObjectLock::releaseObjectLockV(m_v, m_sync); }

	// gain access to the sync object
	BOOL lock(void)	{ if (m_sync) return m_sync->lock(); else return FALSE; }

	// release access to the sync object
	BOOL unlock(void) { if (m_sync) return m_sync->unlock(); else return FALSE; }

protected:
	T *m_sync;
	SOCmnObject *m_obj;
	void *m_v;
}; // SOCmnSingleObjectLock

#pragma pack(pop)
#endif
