/////////////////////////////////////////////////////////////////////////////
// sflcom.h : Additions to ATL's COM implementation
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//


#ifdef _ATL_DEBUG
#define END_COM_MAP_NO_PURE() {NULL, 0, 0}}; return &_entries[1];}
#else
#define END_COM_MAP_NO_PURE() {NULL, 0, 0}}; return _entries;}
#endif // _ATL_DEBUG




#define ATL_IUNKNOWN_IMPL()	\
public:	\
	STDMETHOD_(ULONG, AddRef)()	\
	{return InternalAddRef();}	\
	STDMETHOD_(ULONG, Release)()	\
	{	\
		ULONG l = InternalRelease();	\
		if (l == 0)	\
			delete this;	\
		return l;	\
	}	\
	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)	\
	{return _InternalQueryInterface(iid, ppvObject);}


/////////////////////////////////////////////////////////
// Special COM implementation for objects
// whose life time will not be controlled by
// the COM reference counting mechanism.
// No assumptions are made about the allocation of the 
// object, owner is still in charge of destroying it
// appropiately when done.

#define ATL_IUNKNOWN_IMPL_NODESTROY()	\
public:	\
	STDMETHOD_(ULONG, AddRef)()	\
	{return InternalAddRef();}	\
	STDMETHOD_(ULONG, Release)()	\
	{	\
		ULONG l = InternalRelease();	\
		return l;	\
	}	\
	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)	\
	{return _InternalQueryInterface(iid, ppvObject);}
