//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : OPCOBJ.H                                                    |   
//  Version     : 2.00.0.00.release                                           |
//  Date        : 29-March-1999                                               |
//                                                                            |
//  Description : IUnknown implementation of COM engine objects               |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _OPCOBJ_H_
#define _OPCOBJ_H_


#include "OPCTrace.h"
#include "OPCEngine.h"

// taken from ATLCOM.H

template <class Base>
class OPCObject : public Base
{
public:
	typedef Base _BaseClass;
	OPCObject(void* = NULL)
	{
		_TRACE(TL_INF, TG_COM, (_T("create")));
		m_internalCnt = 0;
		_Module.Lock();
	}
	// Set refcount to 1 to protect destruction
	~OPCObject()
	{
		m_dwRef = 1L;
		FinalRelease();
		_Module.Unlock();
		_TRACE(TL_INF, TG_COM, (_T("free")));
	}
	//If InternalAddRef or InteralRelease is undefined then your class
	//doesn't derive from CComObjectRoot
	STDMETHOD_(ULONG, AddRef)() 
	{
		ULONG l = InternalAddRef();
		_TRACE(TL_DEB, TG_COM, (_T("AddRef %lu"), l));
		return l;
	}
	ULONG internalAddRef(void) 
	{
		ULONG intCnt;
		intCnt = InterlockedIncrement(&m_internalCnt);
		AddRef();
		return intCnt;
	}
	STDMETHOD_(ULONG, Release)()
	{
		Lock();
		ULONG l = InternalRelease();
		if (l == 0)
		{
			InternalAddRef();
			Unlock();
			if (releaseMe())
			{
				InternalRelease();
				delete this;
			}
			else
				InternalRelease();
		}
		else
			Unlock();

		_TRACE(TL_DEB, TG_COM, (_T("Release %lu"), l));
		return l;
	}
	ULONG internalRelease(OPTIONAL OUT ULONG *refCnt = NULL)
	{
		ULONG intCnt;
		ULONG rCnt;
		intCnt = InterlockedDecrement(&m_internalCnt);
		if (intCnt < 0xFFFFFFF0)
		{
			rCnt = Release();
			if (refCnt)
				*refCnt = rCnt;
		}
		else
		{
			intCnt = 0;
			m_internalCnt = 0;
			if (refCnt)
				*refCnt = m_dwRef;
		}
		return intCnt;
	}
	ULONG getRefCount(void)
	{
		return m_dwRef;
	}
	//if _InternalQueryInterface is undefined then you forgot BEGIN_COM_MAP
	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
	{
		HRESULT res = E_NOINTERFACE;

		if (iid != IID_IPersistFile)
			res = _InternalQueryInterface(iid, ppvObject);
		else
		{
			// check if configuration file handler is set
			GenericPointer<OPCEngine> engine = ::getOPCEngine();
			GenericPointer<GenericConfigFileHandler> cfg = engine->getConfigurationFileHandler();

			// support IPersistFile only if config file handler is set
			if ((GenericConfigFileHandler *)cfg)
			{
				res = _InternalQueryInterface(iid, ppvObject);
			}			
		}

		_TRACE(TL_DEB, TG_COM, (_T("QueryInferface %8.8X-%4.4X-%4.4X-%2.2X%2.2X%2.2X%2.2X%2.2X%2.2X%2.2X%2.2X (0x%X)"), iid.Data1, iid.Data2, iid.Data3, iid.Data4[0], iid.Data4[1], iid.Data4[2], iid.Data4[3], iid.Data4[4], iid.Data4[5], iid.Data4[6], iid.Data4[7], res));
		return res;
	}

	static HRESULT WINAPI CreateInstance(OPCObject<Base>** pp);

	LONG m_internalCnt;

	DECLARE_PROTECT_FINAL_CONSTRUCT();
};

template <class Base>
HRESULT WINAPI OPCObject<Base>::CreateInstance(OPCObject<Base>** pp)
{
	_TRACE(TL_DEB, TG_COM, (_T("CreateInstance")));
	_ASSERTE(pp != NULL);
	HRESULT hRes = E_OUTOFMEMORY;
	OPCObject<Base>* p = NULL;
	ATLTRY(p = new OPCObject<Base>())
	if (p != NULL)
	{
		p->SetVoid(NULL);
		p->InternalFinalConstructAddRef();
		hRes = p->FinalConstruct();
		p->InternalFinalConstructRelease();
		if (hRes != S_OK)
		{
			delete p;
			p = NULL;
		}
	}
	*pp = p;
	return hRes;
}


template <class T1>
class OPCComCreator
{
public:
	static HRESULT WINAPI CreateInstance(void* pv, REFIID riid, LPVOID* ppv)
	{
		ATLASSERT(*ppv == NULL);
		HRESULT hRes = E_OUTOFMEMORY;
		T1* p = NULL;
		ATLTRY(p = new T1(pv))
		if (p != NULL)
		{
			p->InternalFinalConstructAddRef();
			p->initObject();
			p->SetVoid(pv);
			hRes = p->FinalConstruct();
			if (hRes == S_OK)
				hRes = p->QueryInterface(riid, ppv);
			p->InternalFinalConstructRelease();
			if (hRes != S_OK)
				delete p;
		}
		return hRes;
	}
};

#define DECLARE_OPCOBJECT_NOT_AGGREGATABLE(x) public:\
	typedef CComCreator2< OPCComCreator< OPCObject< x > >, CComFailCreator<CLASS_E_NOAGGREGATION> > _CreatorClass;

void lastCOMObjectActions(void);

#endif
