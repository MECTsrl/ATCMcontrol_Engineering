//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : ENUM.H                                                      |   
//  Version     : 2.00.0.00.release                                           |
//  Date        : 29-March-1999                                               |
//  Author      : ATCM                                                 |
//                                                                            |
//  Description : DCOM enumerator classes                                     |
//                - COMStringEnum: string enumerator                          |
//                - COMUnknownEnum: IUnknown enumerator                       |
//                - OPCItemEnum: OPCITEMATTRIBUTES enumerator                 |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _ENUM_H_
#define _ENUM_H_

#include "OPCObj.h"
#include "GenItem.h"
#include <opcda.h>

//-----------------------------------------------------------------------------
// COMStringEnum                                                              |
//-----------------------------------------------------------------------------

class COMStringEnum : public IEnumString,
                      public CComObjectRoot
{
public:
	COMStringEnum();
	~COMStringEnum();

	BEGIN_COM_MAP(COMStringEnum)
		COM_INTERFACE_ENTRY(IEnumString)
	END_COM_MAP()

	STDMETHODIMP Next(IN ULONG celt, OUT LPOLESTR *rgelt, 
			OUT ULONG *pceltFetched);

	STDMETHODIMP Skip(IN ULONG celt);

	STDMETHODIMP Reset(void);

	STDMETHODIMP Clone(OUT IEnumString ** ppEnumString);

	CStringList m_list;

	LPCTSTR getObjTraceId(void);
	BOOL releaseMe(void);

protected:
	POSITION m_curPos;
	CString m_objectTraceId;	// object trace id string 
};

inline LPCTSTR COMStringEnum::getObjTraceId(void)
{ return m_objectTraceId; }

typedef OPCObject<COMStringEnum> COMStringEnumObject;


//-----------------------------------------------------------------------------
// COMUnknownEnum                                                              |
//-----------------------------------------------------------------------------

typedef CTypedPtrList<CPtrList, IUnknown *> IUnknownList;

class COMUnknownEnum : public IEnumUnknown,
                       public CComObjectRoot
{
public:
	COMUnknownEnum();
	~COMUnknownEnum();

	BEGIN_COM_MAP(COMUnknownEnum)
		COM_INTERFACE_ENTRY(IEnumUnknown)
	END_COM_MAP()

	STDMETHODIMP Next(IN ULONG celt, OUT IUnknown **rgelt, 
			OUT ULONG *pceltFetched);

	STDMETHODIMP Skip(IN ULONG celt);

	STDMETHODIMP Reset(void);

	STDMETHODIMP Clone(OUT IEnumUnknown ** ppEnumUnknown);

	IUnknownList m_list;

	LPCTSTR getObjTraceId(void);
	BOOL releaseMe(void);

protected:
	POSITION m_curPos;
	CString m_objectTraceId;	// object trace id string 
};

inline LPCTSTR COMUnknownEnum::getObjTraceId(void)
{ return m_objectTraceId; }

typedef OPCObject<COMUnknownEnum> COMUnknownEnumObject;


//-----------------------------------------------------------------------------
// OPCItemEnum                                                                |
//-----------------------------------------------------------------------------

class OPCItemEnum : public IEnumOPCItemAttributes,
                    public CComObjectRoot
{
public:
	OPCItemEnum();
	~OPCItemEnum();

	BEGIN_COM_MAP(OPCItemEnum)
		COM_INTERFACE_ENTRY(IEnumOPCItemAttributes)
	END_COM_MAP()

	STDMETHODIMP Next(IN ULONG celt, OUT OPCITEMATTRIBUTES **rgelt, 
			OUT ULONG *pceltFetched);

	STDMETHODIMP Skip(IN ULONG celt);

	STDMETHODIMP Reset(void);

	STDMETHODIMP Clone(OUT IEnumOPCItemAttributes ** ppEnumOPCItemAttr);

	ItemList m_list;

	LPCTSTR getObjTraceId(void);
	BOOL releaseMe(void);

protected:
	POSITION m_curPos;
	CString m_objectTraceId;	// object trace id string 
};

inline LPCTSTR OPCItemEnum::getObjTraceId(void)
{ return m_objectTraceId; }

typedef OPCObject<OPCItemEnum> OPCItemEnumObject;

#endif
