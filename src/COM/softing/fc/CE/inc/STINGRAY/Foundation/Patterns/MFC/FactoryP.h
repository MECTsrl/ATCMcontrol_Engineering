/////////////////////////////////////////////////////////////////////////////
// FactoryP.h : Declaration of SEC Factory Patterns
//
// Stingray Software Extension Classes
// Copyright (C) 1998 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Authors:      Mark Isham, Dean Hallman
// Description:  Declaration of SEC Factory Patterns
//

#ifndef __SFL_SECFACTORY_PATTERNS_H__
#define __SFL_SECFACTORY_PATTERNS_H__

#ifdef WIN32

// Header Includes
#include <typeinfo.h>

//
// CMN Extension DLL
// Initialize declaration context
//
#ifdef _SFLDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SFL_DATAEXT
#endif //_SFLDLL

namespace stingray {
namespace foundation {

// Constants

// Macros

// Memory Manager Debug Macros
#if (defined(SEC_OBJECTMGR_DEBUG) && defined(_DEBUG))
#pragma message("Building SEC Object Manager Debug Diagnostics")
#define SEC_OBJMGR_DUMPCOMPLETE(count)							\
	{															\
	CTime time=CTime::GetCurrentTime();							\
	CString strTime=time.Format(_T("%H:%M:%S"));				\
	TRACE(_T("OBJECT MGR: Deleted %d entries total\n"),count);	\
	TRACE(_T("********** Completed %s ********************\n"),strTime);	\
	}
#define SEC_OBJMGR_DUMPDELETEOBJECT(pObject) DumpDeleteObject(pObject)
#else
#define SEC_OBJMGR_DUMPCOMPLETE(count)
#define SEC_OBJMGR_DUMPDELETEOBJECT(pObject)
#endif	// SEC_OBJECTMGR_DEBUG

// Types

// Forward Declarations

/////////////////////////////////////////////////////////////////////////////
// SECClassFactory

//
// Type-Safe Object Creation
//

template <class RTBase>
class SECClassFactory {
public:
    virtual RTBase* CreateObject() const = 0;
};

template <class RTClass, class RTBase>
class SECClassFactory_T : public SECClassFactory<RTBase>
{
public:
    virtual RTBase* CreateObject() const { return new RTClass; }
};

/////////////////////////////////////////////////////////////////////////////
// SECObjectMemoryManager

//
// Type-Safe Memory Allocation Manager
//

template <class RTBase>
class SECObjectMemoryManager_T {
public:
	SECObjectMemoryManager_T() {}
	virtual ~SECObjectMemoryManager_T() { DeleteManagedMemory(); }

	// Make sure object destructors are virtual, else 
	// unpredictable behavior may result on autodelete.
	virtual BOOL ManageMemory(RTBase* pObject);
	virtual BOOL ReleaseManagement(RTBase* pObject);
	virtual BOOL IsManaged(RTBase* pObject);
	virtual int DeleteManagedMemory();
	virtual int GetManagedCount() { return m_listManagedObjects.GetCount(); }
protected:
	CTypedPtrList<CPtrList,RTBase*> m_listManagedObjects;

#ifdef SEC_OBJECTMGR_DEBUG	
	void DumpDeleteObject(RTBase* pObject);
#endif	
};

//
// Inline Implementation
//

// Add an object to the managed memory list. 
// Return FALSE if NULL, or if item already in list.
template <class RTBase>
BOOL SECObjectMemoryManager_T<RTBase>::ManageMemory(RTBase* pObject) {
	if(!pObject) return FALSE;
	if(m_listManagedObjects.Find(pObject)) return FALSE;

	m_listManagedObjects.AddTail(pObject);
	return TRUE;
}

// Remove an object from the managed memory list (does not deallocate). 
// Return FALSE if NULL or item not found in list.
template <class RTBase>
BOOL SECObjectMemoryManager_T<RTBase>::ReleaseManagement(RTBase* pObject) {
	if(!pObject) return FALSE;
	POSITION posFind=m_listManagedObjects.Find(pObject);
	if(!posFind) return FALSE;

	m_listManagedObjects.RemoveAt(posFind);
	return TRUE;
}

// Determine if the object passed in is currently managed by this manager.
template <class RTBase>
BOOL SECObjectMemoryManager_T<RTBase>::IsManaged(RTBase* pObject) {
	if(!pObject) return FALSE;
	POSITION posFind=m_listManagedObjects.Find(pObject);
	return (BOOL)(posFind);
}

// Delete all the managed memory pointers, and reset the list.
// Returns number of objects deleted.
template <class RTBase>
int SECObjectMemoryManager_T<RTBase>::DeleteManagedMemory() {
	int nCount=0;

	// Delete all the cached memory pointers
	RTBase* pObject;
	POSITION pos=m_listManagedObjects.GetHeadPosition();
	while(pos) {
		pObject=m_listManagedObjects.GetNext(pos);				
		SEC_OBJMGR_DUMPDELETEOBJECT(pObject);	// debug only dump
		
		delete pObject;
		++nCount;
		}
	m_listManagedObjects.RemoveAll();
	SEC_OBJMGR_DUMPCOMPLETE(nCount)	
	
	return nCount;
}

#ifdef SEC_OBJECTMGR_DEBUG
template <class RTBase>
void SECObjectMemoryManager_T<RTBase>::DumpDeleteObject(RTBase* pObject) {
	ASSERT(pObject);
	const type_info& ti = typeid(*pObject);
	const char* strClassName=ti.name();
	TRACE(_T("OBJECT MGR: Deleting 0x%x - %s\n"),pObject,strClassName);
}	
#endif	// SEC_OBJECTMGR_DEBUG

};  // namespace stingray::foundation
};  // namespace stingray

/////////////////////////////////////////////////////////////////////////////

//
// CMN Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif	// WIN32

#endif	//__SFL_SECFACTORY_PATTERNS_H__
