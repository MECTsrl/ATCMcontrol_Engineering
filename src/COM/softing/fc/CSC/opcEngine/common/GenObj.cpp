//-----------------------------------------------------------------------------
//                OPC ENGINE              OPC SESSION                         |
//                                                                            |
//  Filename    : GENOBJ.CPP                                                  |   
//  Version     : 2.00.0.00.release       2.00.0.00.release                   |
//  Date        : 29-March-1999           29-March-1999                       |
//  Author      : ATCM                                                 |
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

#include "stdafx.h"
#include "GenObj.h"
#include "OPCTrace.h"

//-----------------------------------------------------------------------------
// GenericObject                                                              |
//-----------------------------------------------------------------------------

GenericObject::GenericObject(
	IN DWORD objType)	// object type
{

	m_objectType = objType;
	m_refCount = 0;
	m_objectTracePos = NULL;
	m_objectState = 0;
	m_objectStateTarget = 0;
	m_userData = 0;

	_TRACE(TL_INF, TG_GEN, (_T("create")));
	_TRACE_CREATE_GENOBJ();
	addRef();
}

GenericObject::~GenericObject(void)
{
	_TRACE(TL_INF, TG_GEN, (_T("free")))
	_TRACE_DELETE_GENOBJ();
}


//-----------------------------------------------------------------------------
// addRef
//
// - increment reference counter
//
void GenericObject::addRef(void)
{
	LONG refCount = InterlockedIncrement(&m_refCount);
	_TRACE(TL_DEB, TG_GEN, (_T("addRef %li"), refCount))
} // addRef


//-----------------------------------------------------------------------------
// release
//
// - decrement reference counter
// - free object if no reference left
//
// returns:
//		reference count
//
LONG GenericObject::release(void)
{
	LONG refCount = InterlockedDecrement(&m_refCount);
	_TRACE(TL_DEB, TG_GEN, (_T("release %li"), refCount))
	if (refCount <= 0)
	{
		delete this;	// last reference -> free object
		return 0;
	}
	return refCount;
} // release


//-----------------------------------------------------------------------------
// hasKey
//
// - compare objects unique key with given string
//
// returns:
//		TRUE  - object has the key
//      FALSE - object has not the key
//
BOOL GenericObject::hasKey(
	IN LPCTSTR key)	// object key
{
	DWORD addr = _tcstoul(key, NULL, 0);
	return (addr == (DWORD)this);
} // hasKey


//-----------------------------------------------------------------------------
// getKey
//
// - get objects unique key
//
// returns:
//		object key
//
CString GenericObject::getKey(void)
{ 
	CString addr;
	addr.Format(_T("0x%8.8X"), this);
	return addr; 
} // getKey


//-----------------------------------------------------------------------------
// getObjTraceId
//
// - get objects trace string id
//
// returns:
//		trace id string
//
LPCTSTR GenericObject::getObjTraceId(void)
{ 
	if (m_objectTraceId.IsEmpty())
	{
		m_objectTraceId.Format(_T("[%s|%8.8X|]"), (LPCTSTR)getObjTypeString(), this);
		m_objectTraceId.FreeExtra();
	}

	return m_objectTraceId; 
} // getObjTraceId


//-----------------------------------------------------------------------------
// getObjTypeString
//
// - get objects type string
//
// returns:
//		type string
//
CString GenericObject::getObjTypeString(void)
{ 
	CString objectTypeString;			// object type string 

	switch(m_objectType)
	{
		case GENOBJ_TYPE_CALLBACK: objectTypeString = "CALLB"; break;
		case GENOBJ_TYPE_ENGINE: objectTypeString = "ENG  "; break;
		case GENOBJ_TYPE_REQUEST: objectTypeString = "REQ  "; break;
		case GENOBJ_TYPE_CACHE: objectTypeString = "CACHE"; break;
		case GENOBJ_TYPE_ELEMENT: objectTypeString = "ELEMT"; break;
		case GENOBJ_TYPE_TRANSACTION: objectTypeString = "TRACT"; break;
		case GENOBJ_TYPE_LEAF: objectTypeString = "LEAF "; break;
		case GENOBJ_TYPE_BRANCH: objectTypeString = "BRANC"; break;
		case GENOBJ_TYPE_SERVER: objectTypeString = "SERVR"; break;
		case GENOBJ_TYPE_GROUP: objectTypeString = "GROUP"; break;
		case GENOBJ_TYPE_NODE: objectTypeString = "NODE "; break;
		case GENOBJ_TYPE_TAG: objectTypeString = "TAG  "; break;
		case GENOBJ_TYPE_ITEM: objectTypeString = "ITEM "; break;
		case GENOBJ_TYPE_NS_ROOT: objectTypeString = "RONS "; break;
		case GENOBJ_TYPE_OPC_ROOT: objectTypeString = "ROOPC"; break;
		case GENOBJ_TYPE_SESSION: objectTypeString = "SES  "; break;
		case GENOBJ_TYPE_CONFIG: objectTypeString = "CFG  "; break;
		case GENOBJ_TYPE_OTHER: objectTypeString = "OTHER"; break;
		case GENOBJ_TYPE_TAG_PROPERTY: objectTypeString = "PROP "; break;
		case GENOBJ_TYPE_ITEM_TAG: objectTypeString = "ITAG "; break;
		default: objectTypeString = "?????"; break;
	}

	return objectTypeString;
} // getObjTypeString


//-----------------------------------------------------------------------------
// setObjectState
//
// - set the object state
//
// returns:
//		TRUE
//
BOOL GenericObject::setObjectState(
	IN BYTE state)	// object state
{
	onSetObjectState(state);
	m_objectState = state;
	return TRUE;
} // setObjectState


//-----------------------------------------------------------------------------
// onSetObjectState
//
// - called before the new object state is set
//
void GenericObject::onSetObjectState(
	IN BYTE newState)	// object state
{ } // onSetObjectState


//-----------------------------------------------------------------------------
// setTargetObjectState
//
// - set the target state of the object
//
// returns:
//		TRUE
//
BOOL GenericObject::setTargetObjectState(
	IN BYTE state)	// object state
{
	onSetTargetObjectState(state);
	m_objectStateTarget = state;
	return TRUE;
} // setTargetObjectState


//-----------------------------------------------------------------------------
// onSetTargetObjectState
//
// - called before the new object state is set
//
void GenericObject::onSetTargetObjectState(
	IN BYTE newState)	// object state
{ } // onSetTargetObjectState


