// DataSource.h
// Copyright (C) 1999 Stingray Software Inc.
// All Rights Reserved


#pragma once

#include <vector>
#include "DropSource.h"


namespace stingray {
namespace foundation {

///////////////////////////////////////////////////////////////////////////
//@doc _OTL_DATACACHE_ENTRY
//
//@struct _OTL_DATACACHE_ENTRY | This class ties together the FORMATETC structure
// and the STGMEDIUM structure
//
//
struct _OTL_DATACACHE_ENTRY
{
	FORMATETC m_formatEtc;
	STGMEDIUM m_stgMedium;
	DATADIR m_nDataDir;
};

typedef std::vector<_OTL_DATACACHE_ENTRY*> _OTL_DATACACHE;
typedef std::vector<FORMATETC*> _FORMATETCCACHE;


// This is an implementation of IEnumFORMATETC...
class _EnumFORMATETCImpl : public IEnumFORMATETC
{
	bool m_bIsClone;
	ULONG m_nNdx;
	LONG m_cRef;
	_FORMATETCCACHE* m_pFormatETCCache;

	_EnumFORMATETCImpl()
	{
		m_bIsClone = 0;
		m_nNdx = 0;
		m_cRef = 0;
		m_pFormatETCCache = 0;
	}

public:
	_EnumFORMATETCImpl(ULONG nNdx, bool bIsClone)
	{
		m_nNdx = nNdx;
		m_cRef = 0;
		m_bIsClone = bIsClone;
		m_pFormatETCCache = new _FORMATETCCACHE;
	}

	~_EnumFORMATETCImpl()
	{
		if(m_pFormatETCCache)
		{
			std::vector<FORMATETC*>::iterator iter;
			for (iter = m_pFormatETCCache->begin(); 
				iter < m_pFormatETCCache->end(); iter++) 
			{
				FORMATETC* pFormatEtc = *iter;

				if(!m_bIsClone)
				{
					CoTaskMemFree(pFormatEtc->ptd);
				}

				delete (pFormatEtc);
				m_pFormatETCCache->erase(iter);
			}
			delete m_pFormatETCCache;
		}
	}

	void AddFormat(const FORMATETC* lpFormatEtc)
	{
		FORMATETC* pFormat = new FORMATETC;

		pFormat->cfFormat = lpFormatEtc->cfFormat;
		pFormat->ptd = lpFormatEtc->ptd;
			// Note: ownership of lpFormatEtc->ptd is transfered with this call.
		pFormat->dwAspect = lpFormatEtc->dwAspect;
		pFormat->lindex = lpFormatEtc->lindex;
		pFormat->tymed = lpFormatEtc->tymed;
		// add to list
		m_pFormatETCCache->push_back(pFormat);
	}

	STDMETHODIMP QueryInterface(REFIID riid, void** ppv)
	{
		*ppv = NULL;

		if(riid == IID_IUnknown || 
			riid == IID_IEnumFORMATETC)
		{
			*ppv = static_cast<IEnumFORMATETC*>(this);
			((IUnknown*)(*ppv))->AddRef();
		}
		else
		{
			*ppv = NULL;
		}

		if(*ppv)
		{
			return S_OK;
		}
		else
		{
			return E_NOINTERFACE;
		}
	}

	STDMETHODIMP_(ULONG) AddRef()
	{
		return InterlockedIncrement(&m_cRef);
	}

	STDMETHODIMP_(ULONG) Release()
	{
		if(InterlockedDecrement(&m_cRef) == 0)
		{
			delete this;
			return 0;
		}
		else
		{
			return m_cRef;
		}
	}

	STDMETHODIMP Next(ULONG cElt, 
						FORMATETC* rgelt, 
						ULONG * pceltFetched)
	{
		// Fetch the next x elements
		ULONG cActual = 0;

		if(m_pFormatETCCache)
		{
			std::vector<FORMATETC*>::iterator iter;

			iter = m_pFormatETCCache->begin();

			while (m_nNdx < m_pFormatETCCache->size() && cActual < cElt) 
			{
				FORMATETC* pEntry = NULL;
				
				pEntry = m_pFormatETCCache->at(m_nNdx);
				
				memcpy(&rgelt[cActual++], pEntry, sizeof(FORMATETC));
				m_nNdx++;
			}
		}

		if (pceltFetched)
		{
			*pceltFetched = cActual;
		}

		return (cActual == cElt) ? S_OK : S_FALSE;
	}

	STDMETHODIMP Skip(ULONG celt)
	{
		// Skip the next x elements
		ULONG nSize = m_pFormatETCCache->size();

		if(nSize - m_nNdx >= celt) {
			m_nNdx += celt; 
			return S_OK; 
		}
		m_nNdx = nSize;
		return S_FALSE;
	}

	STDMETHODIMP Reset(void)
	{
		// Start over
		m_nNdx = 0;
		return S_OK;
	}

	STDMETHODIMP Clone(IEnumFORMATETC ** ppenum)
	{
		_EnumFORMATETCImpl* pEnumFORMATETC;

		pEnumFORMATETC = new _EnumFORMATETCImpl(m_nNdx, true);

		if(!pEnumFORMATETC)
		{
			*ppenum = NULL;
			return E_OUTOFMEMORY;
		} else
		{
			*ppenum = static_cast<IEnumFORMATETC*>(pEnumFORMATETC);
			return S_OK;
		}
	}
};



/////////////////////////////
// AutoDuck tag block for CDataObject
//
//@doc CDataObject
//
//@mfunc int | CDataObject| CDataObject | Constructor
//@comm Constructor performs initialization
//
//@mfunc ULONG | CDataObject| AddRef | Place a reference count on the object
//@rdesc Returns the current reference count, but you're not supposed to care.
//@comm AddRef is a member of IUnknown 
//
//@mfunc ULONG | CDataObject| Release | Remove a reference count on the object
//@rdesc Returns the current reference count, but you're not supposed to care.
//@comm Release is a member of IUnknown 
//
//@mfunc HRESULT | CDataObject| QueryInterface | Get a new interface from the object
//@rdesc Returns S_OK if succeeded, otherwise, returns E_NOINTERFACE
//@parm REFIID | riid | Interface ID to use 
//@parm void** | ppv | place to put the pointer
//@comm QueryInterface is the standard means by which client code acquires
// new interfaces to a COM object. CDataObject supports IUnknown and IDataObject
//
//@mfunc void | CDataObject| CacheGlobalData | Cache a memory handle into the object
//@parm CLIPFORMAT | cfFormat | Format of data (text, metafile, bitmap, etc. )
//@parm HGLOBAL | hGlobal | Memory handle to cache
//@parm LPFORMATETC | lpFormatEtc = NULL | Pointer to a FORMATETC structure containing
// information about the format of the data. 
//@comm CacheGlobalData adds data held in a global memory handle to the data object cache
//
//@mfunc DWORD | CDataObject| DoDragDrop | Initiate dragging and dropping
//@rdesc Returns the DROPEFFECT mode when the drag drop operation ended
//@parm DWORD | dwEffects = DROPEFFECT_COPY or DROPEFFECT_MOVE or DROPEFFECT_LINK,CLIPFORMAT | Allowable drag drop modes
//@parm LPCRECT | lpRectStartDrag = NULL,| Pointer to a rectangle 
//@parm IDropSource* | pDropSource = NULL| IDropSource necessary for the drag drop operation
// data. 
//@comm Initiate the drag drop operation
//
//@mfunc void | CDataObject| SetClipboard | Put the data on a clipboard
//@comm Put the data on the clipboard.
//
//@mfunc void | CDataObject| CacheData | Cache any format of data into the object
//@parm LPSTGMEDIUM | lpStgMedium | Pointer to a STGMEDIUM structure holding the data to cache
//@parm LPFORMATETC | lpFormatEtc = NULL | Pointer to a FORMATETC structure containing
// information about the format of the data. 
//@comm Cache Global data adds data held in a STGMEDIUM structure to the data object cache
//
//@mfunc void | CDataObject| Empty | Empty any data stored in the cache
//@comm Empty any data stored in the cache
//
//@mfunc HRESULT | CDataObject | GetData | Get the data out of the object. This is a member of
// IDataObject
//@rdesc Returns DV_E_LINDEX if lpFormatEtc->lindex is not == -1. Returns DATA_E_FORMATETC
// if an unavailable format was requested. Returns S_OK otherwise. 
//@parm LPFORMATETC | lpFormatEtc | Pointer to a FORMATETC structure holding the format information of the requested data type. 
//@parm LPSTGMEDIUM | lpStgMedium | Pointer to a STGMEDIUM structure holding the data. 
//@comm GetData is a member of the COM interface IDataObject. Server allocates the STGMEDIUM structure. See the 
// See the Platform SDK documentation about IDataObject for more information
// 
//@mfunc HRESULT | CDataObject | GetDataHere | Get the data out of the object. This is a member of
// IDataObject
//@rdesc Returns DV_E_LINDEX if lpFormatEtc->lindex is not == -1. Returns DATA_E_FORMATETC
// if an unavailable format was requested. Returns S_OK otherwise. 
//@parm LPFORMATETC | lpFormatEtc | Pointer to a FORMATETC structure holding the format information of the requested data type. 
//@parm LPSTGMEDIUM | lpStgMedium | Pointer to a STGMEDIUM structure holding the data. 
//@comm GetData is a member of the COM interface IDataObject. Client allocates the STGMEDIUM structure
// See the Platform SDK documentation about IDataObject for more information
//
//@mfunc HRESULT | CDataObject | QueryGetData | Find out if a certain format of data is available. 
//@rdesc Returns DATA_E_FORMATETC if an unavailable format was requested. Returns S_OK otherwise. 
//@parm LPFORMATETC | lpFormatEtc | Pointer to a FORMATETC structure holding the format information of the requested data type. 
//@comm QueryGetData is a member of the COM interface IDataObject. 
// See the Platform SDK documentation about IDataObject for more information
//
//@mfunc HRESULT | CDataObject | GetCanonicalFormatEtc | Provides a standard FORMATETC structure that is logically equivalent to one that is more complex.
//@parm LPFORMATETC | lpFormatEtc | Pointer to a FORMATETC structure holding the format information of the requested data type. 
//@parm LPFORMATETC | lpFormatEtc | Pointer to a FORMATETC structure holding the format information of the requested data type. 
//@comm GetCanonicalFormatEtc Provides a standard FORMATETC structure that is logically equivalent to one that is more complex. You use this method to determine whether two different FORMATETC structures would return the same data, removing the need for duplicate rendering.
//
//@mfunc HRESULT | CDataObject | SetData | Poke data into the data object
//@parm LPFORMATETC | lpFormatEtc | Pointer to a FORMATETC structure holding the format information of the requested data type. 
//@parm LPSTGMEDIUM | lpStgMedium | Pointer to a STGMEDIUM structure holding the data. 
//@parm BOOL | bRelease | TRUE indicates that the data object owns the data. FALSE indicates the client owns the data.
//@comm CDataObject provides no implementation of this function. 
//
//@mfunc HRESULT | CDataObject | EnumFormatEtc | Get a FORMATETC enumerator 
//@rdesc Returns S_OK if the enumerator is available
//@parm DWORD | dwDirection | Direction of data 
//@parm LPENUMFORMATETC* | ppenumFormatEtc | Place the enumerator here.
//@comm CDataObject provides no implementation of this function
//
//@mfunc HRESULT | CDataObject | DAdvise | Add an advisor to the data object
//@rdesc Returns S_OK if the advise sink hooked up okay 
//@parm LPFORMATETC | lpFormatEtc | Pointer to a FORMATETC structure holding the format information of the requested data type. 
//@parm DWORD | dwFlags | Specifies a group of flags for controlling the advisory connection. 
//@parm LPADVISESINK | pAdviseSink | The Advise Sink
//@parm LPDWROD | pdwConnection | Cookie for unsubscribing 
//@comm CDataObject provides no implementation of this function
//
//@mfunc HRESULT | CDataObject | DAdvise | Add an advisor to the data object
//@rdesc Returns S_OK if the advise sink hooked up okay 
//@parm LPFORMATETC | lpFormatEtc | Pointer to a FORMATETC structure holding the format information of the requested data type. 
//@parm DWORD | dwFlags | Specifies a group of flags for controlling the advisory connection. 
//@parm LPADVISESINK | pAdviseSink | The Advise Sink
//@parm LPDWROD | pdwConnection | Cookie for unsubscribing 
//@comm CDataObject provides no implementation of this function
//
//@mfunc HRESULT | CDataObject | DUnadvise | Unsubscribe
//@rdesc Returns S_OK if the connection is terminated 
//@parm DWORD | dwCookie | DWORD identifying the conneciton 
//@comm CDataObject provides no implementation of this function
//
//@mfunc HREESULT | CDataObject | EnumDAdvise | Get an advise enumerator
//@rdesc Returns S_OK if the enumerator is available. 
//@parm LPENUMSTATDATA* | ppenumAdvise | Advise enumerator 
//@comm CDataObject provides no implementation of this function
//
//@class CDataObject | This class is provided as a standard way 
//
// Member functions include:
//  <mf CDataObject::CDataObject>, 
//  <mf CDataObject::AddRef>, 
//  <mf CDataObject::Release>, 
//  <mf CDataObject::QueryInterface>, 
//  <mf CDataObject::CacheGlobalData>, 
//  <mf CDataObject::DoDragDrop>, 
//  <mf CDataObject::SetClipboard>, 
//  <mf CDataObject::CacheData>, 
//  <mf CDataObject::GetDataHere>, 
//  <mf CDataObject::GetData>, 
//  <mf CDataObject::GetDataHere>, 
//  <mf CDataObject::QueryGetData>, 
//  <mf CDataObject::GetCanonicalFormatEtc>, 
//  <mf CDataObject::SetData>, 
//  <mf CDataObject::EnumFormatEtc>, 
//  <mf CDataObject::DAdvise>, 
//  <mf CDataObject::DUnadvise>, and 
//  <mf CDataObject::EnumDAdvise>
//
//@base public | IDataObject
//
class CDataObject : public IDataObject
{
// State
protected:
	_OTL_DATACACHE_ENTRY* m_pDataCache;  // data cache itself
	UINT m_nMaxSize;    // current allocated size
	UINT m_nSize;       // current size of the cache
	UINT m_nGrowBy;     // number of cache elements to grow by for new allocs
	IDataObject* m_pClipboardSource;
	LONG m_cRef;

// Constructors
public:
	//@cmember
	/* Constructor initializes the data object  */
	CDataObject()
	{
		m_pDataCache = NULL;
		m_nMaxSize = 0;
		m_nSize = 0;
		m_nGrowBy = 1;
		m_pClipboardSource = NULL;
		m_cRef = 0;
	}

	~CDataObject()
	{
		OleSetClipboard(NULL);

		// free the data cache
		Empty();
	}

	//@cmember
	/* AddRef puts a reference count on the object */
	STDMETHODIMP_(ULONG) AddRef()
	{
		return InterlockedIncrement(&m_cRef);
	}

	//@cmember
	/* Release Removesa reference count from the object */
	STDMETHODIMP_(ULONG) Release()
	{
		if(InterlockedDecrement(&m_cRef) == 0)
		{
			return 0;
		} else
		{
			return m_cRef;
		}
	}

	//@cmember
	/* Get a new interface */
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv)
	{
		*ppv = NULL;
		if(riid == IID_IUnknown || riid == IID_IDataObject) {
			*ppv = static_cast<IDataObject*>(this);
		} 
		else {
			*ppv = NULL;
		}

		if(*ppv)
		{
			((IUnknown*)(*ppv))->AddRef();
			return S_OK;
		} else
		{
			return E_NOINTERFACE;
		}
	}

// static utilities
	static LPFORMATETC _FillFormatEtc(LPFORMATETC lpFormatEtc, 
								 CLIPFORMAT cfFormat, 
								 LPFORMATETC lpFormatEtcFill)
	{
		ATLASSERT(lpFormatEtcFill != NULL);
		if (lpFormatEtc == NULL && cfFormat != 0)
		{
			lpFormatEtc = lpFormatEtcFill;
			lpFormatEtc->cfFormat = cfFormat;
			lpFormatEtc->ptd = NULL;
			lpFormatEtc->dwAspect = DVASPECT_CONTENT;
			lpFormatEtc->lindex = -1;
			lpFormatEtc->tymed = (DWORD) -1;
		}
		return lpFormatEtc;
	}	

	static HGLOBAL _CopyGlobalMemory(HGLOBAL hDest, HGLOBAL hSource)
	{
		ATLASSERT(hSource != NULL);

		// make sure destination is suitable. 
		DWORD nSize = ::GlobalSize(hSource);
		if (hDest == NULL)
		{
			hDest = ::GlobalAlloc(GMEM_SHARE|GMEM_MOVEABLE, nSize);
			if (hDest == NULL)
				return NULL;
		}
		else if (nSize > ::GlobalSize(hDest))
		{
			// destination needs engorging. 
			return NULL;
		}

		// copy the bits
		LPVOID lpSource = ::GlobalLock(hSource);
		LPVOID lpDest = ::GlobalLock(hDest);
		ATLASSERT(lpDest != NULL);
		ATLASSERT(lpSource != NULL);
		memcpy(lpDest, lpSource, nSize);
		::GlobalUnlock(hDest);
		::GlobalUnlock(hSource);

		// success -- return hDest
		return hDest;
	}	

	static BOOL _CopyStgMedium(CLIPFORMAT cfFormat, 
								  LPSTGMEDIUM lpDest, 
								  LPSTGMEDIUM lpSource)
	{
		if (lpDest->tymed == TYMED_NULL)
		{
			ATLASSERT(lpSource->tymed != TYMED_NULL);
			switch (lpSource->tymed)
			{
			case TYMED_ENHMF:
			case TYMED_HGLOBAL:
				ATLASSERT(sizeof(HGLOBAL) == sizeof(HENHMETAFILE));
				lpDest->tymed = lpSource->tymed;
				lpDest->hGlobal = NULL;
				break;  // fall through to CopyGlobalMemory case

			case TYMED_ISTREAM:
				lpDest->pstm = lpSource->pstm;
				lpDest->pstm->AddRef();
				lpDest->tymed = TYMED_ISTREAM;
				return TRUE;

			case TYMED_ISTORAGE:
				lpDest->pstg = lpSource->pstg;
				lpDest->pstg->AddRef();
				lpDest->tymed = TYMED_ISTORAGE;
				return TRUE;

			case TYMED_MFPICT:
				{
					// copy LPMETAFILEPICT struct + embedded HMETAFILE
					HGLOBAL hDest = _CopyGlobalMemory(NULL, lpSource->hGlobal);
					if (hDest == NULL)
						return FALSE;
					LPMETAFILEPICT lpPict = (LPMETAFILEPICT)::GlobalLock(hDest);
					ATLASSERT(lpPict != NULL);
					lpPict->hMF = ::CopyMetaFile(lpPict->hMF, NULL);
					if (lpPict->hMF == NULL)
					{
						::GlobalUnlock(hDest);
						::GlobalFree(hDest);
						return FALSE;
					}
					::GlobalUnlock(hDest);
	
					// fill STGMEDIUM struct
					lpDest->hGlobal = hDest;
					lpDest->tymed = TYMED_MFPICT;
				}
				return TRUE;

			case TYMED_GDI:
				lpDest->tymed = TYMED_GDI;
				lpDest->hGlobal = NULL;
				break;

			case TYMED_FILE:
				{
					USES_CONVERSION;
					lpDest->tymed = TYMED_FILE;
					ATLASSERT(lpSource->lpszFileName != NULL);
					UINT cbSrc = ocslen(lpSource->lpszFileName);
					LPOLESTR szFileName = (LPOLESTR)CoTaskMemAlloc(cbSrc*sizeof(OLECHAR));
					lpDest->lpszFileName = szFileName;
					if (szFileName == NULL)
						return FALSE;
					memcpy(szFileName, lpSource->lpszFileName,  (cbSrc+1)*sizeof(OLECHAR));
					return TRUE;
				}

			// unable to create + copy other TYMEDs
			default:
				return FALSE;
			}
		}
		ATLASSERT(lpDest->tymed == lpSource->tymed);

		switch (lpSource->tymed)
		{
		case TYMED_HGLOBAL:
			{
				HGLOBAL hDest = _CopyGlobalMemory(lpDest->hGlobal,
				lpSource->hGlobal);
				if (hDest == NULL)
					return FALSE;
	
				lpDest->hGlobal = hDest;
			}
			return TRUE;

		case TYMED_ISTREAM:
			{
				ATLASSERT(lpDest->pstm != NULL);
				ATLASSERT(lpSource->pstm != NULL);
	
				// get the size of the source stream
				STATSTG stat;
				if (lpSource->pstm->Stat(&stat, STATFLAG_NONAME) != S_OK)
				{
					// unable to get size of source stream
					return FALSE;
				}
				ATLASSERT(stat.pwcsName == NULL);
	
				// always seek to zero before copy
				LARGE_INTEGER zero = { 0, 0 };
				lpDest->pstm->Seek(zero, STREAM_SEEK_SET, NULL);
				lpSource->pstm->Seek(zero, STREAM_SEEK_SET, NULL);

				// copy source to destination
				if (lpSource->pstm->CopyTo(lpDest->pstm, stat.cbSize,
					NULL, NULL) != NULL)
				{
					// copy from source to dest failed
					return FALSE;
				}

				// always seek to zero after copy
				lpDest->pstm->Seek(zero, STREAM_SEEK_SET, NULL);
				lpSource->pstm->Seek(zero, STREAM_SEEK_SET, NULL);
			}
			return TRUE;

		case TYMED_ISTORAGE:
			{
				ATLASSERT(lpDest->pstg != NULL);
				ATLASSERT(lpSource->pstg != NULL);
	
				// just copy source to destination
				if (lpSource->pstg->CopyTo(0, NULL, NULL, lpDest->pstg) != S_OK)
					return FALSE;
			}
			return TRUE;

		case TYMED_FILE:
			{
				USES_CONVERSION;
				ATLASSERT(lpSource->lpszFileName != NULL);
				ATLASSERT(lpDest->lpszFileName != NULL);
				return CopyFile(OLE2T(lpSource->lpszFileName), OLE2T(lpDest->lpszFileName), FALSE);
			}

		case TYMED_ENHMF:
		case TYMED_GDI:
			{
				ATLASSERT(sizeof(HGLOBAL) == sizeof(HENHMETAFILE));

				// with TYMED_GDI cannot copy into existing HANDLE
				if (lpDest->hGlobal != NULL)
					return FALSE;

				// otherwise, use OleDuplicateData for the copy
				lpDest->hGlobal = OleDuplicateData(lpSource->hGlobal, cfFormat, 0);
				if (lpDest->hGlobal == NULL)
					return FALSE;
			}
			return TRUE;

		// other TYMEDs cannot be copied
		default:
			return FALSE;
		}
	}

	static DVTARGETDEVICE* _CopyTargetDevice(DVTARGETDEVICE* ptdSrc)
	{
		if (ptdSrc == NULL)
			return NULL;

		DVTARGETDEVICE* ptdDest =
			(DVTARGETDEVICE*)CoTaskMemAlloc(ptdSrc->tdSize);
		if (ptdDest == NULL)
			return NULL;

		memcpy(ptdDest, ptdSrc, (size_t)ptdSrc->tdSize);
		return ptdDest;
	}

	static void _CopyFormatEtc(LPFORMATETC petcDest, 
									LPFORMATETC petcSrc)
	{
		ATLASSERT(petcDest != NULL);
		ATLASSERT(petcSrc != NULL);

		petcDest->cfFormat = petcSrc->cfFormat;
		petcDest->ptd = _CopyTargetDevice(petcSrc->ptd);
		petcDest->dwAspect = petcSrc->dwAspect;
		petcDest->lindex = petcSrc->lindex;
		petcDest->tymed = petcSrc->tymed;
	}



// Operations
	// CacheData operations similar to ::SetClipboardData
	//@cmember
	/* Cache a global handle in the data object */
	void CacheGlobalData(CLIPFORMAT cfFormat, HGLOBAL hGlobal,
		LPFORMATETC lpFormatEtc = NULL)
	{
		ATLASSERT(hGlobal != NULL);

		// fill in FORMATETC struct
		FORMATETC formatEtc;
		lpFormatEtc = _FillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);
		lpFormatEtc->tymed = TYMED_HGLOBAL;

		// add it to the cache
		_OTL_DATACACHE_ENTRY* pEntry = GetCacheEntry(lpFormatEtc, DATADIR_GET);
		pEntry->m_stgMedium.tymed = TYMED_HGLOBAL;
		pEntry->m_stgMedium.hGlobal = hGlobal;
		pEntry->m_stgMedium.pUnkForRelease = NULL;

	}

	// Clipboard and Drag/Drop access
	//@cmember
	/* Start a new drag drop operation */
	DWORD DoDragDrop(
		DWORD dwEffects = DROPEFFECT_COPY|DROPEFFECT_MOVE|DROPEFFECT_LINK,
		LPCRECT lpRectStartDrag = NULL,
		IDropSource* pDropSource = NULL)
	{
		// use standard drop source implementation if one not provided
		IDropSourceImpl dropSource;
		if (pDropSource == NULL)
		{
			pDropSource = &dropSource;
		}

		if (lpRectStartDrag != NULL)
		{
		}
		else
		{
			// otherwise start with default empty rectangle around current point
			CPoint ptCursor;
			GetCursorPos(&ptCursor);
		}

		// call global OLE api to do the drag drop
		DWORD dwResultEffect = DROPEFFECT_NONE;

		LPDATAOBJECT lpDataObject = static_cast<LPDATAOBJECT>(this);

		HRESULT hr;

		hr = ::DoDragDrop(lpDataObject, pDropSource, dwEffects, &dwResultEffect);

		return dwResultEffect;
	}

	//@cmember
	/* Put the data on the clipboard */
	void SetClipboard()
	{
		LPDATAOBJECT lpDataObject = static_cast<LPDATAOBJECT>(this);
		HRESULT hr;
		
		hr = ::OleSetClipboard(lpDataObject);
	}

	// If you have the wherewithall, you can set up the STGMEDIUM and FORMATETC 
	//  structures yourself...
	//@cmember
	/* Cache the data using the STGMEDIUM and FORMATETC structures yourself-- if you have the wherewithall. */
	void CacheData(CLIPFORMAT cfFormat, 
					LPSTGMEDIUM lpStgMedium,
					LPFORMATETC lpFormatEtc = NULL)
	{
		ATLASSERT(lpStgMedium == NULL || lpStgMedium->tymed != TYMED_NULL);

		// fill in FORMATETC struct
		FORMATETC formatEtc;
		lpFormatEtc = _FillFormatEtc(lpFormatEtc, cfFormat, &formatEtc);

		ATLASSERT(lpStgMedium->tymed != TYMED_GDI ||
			lpFormatEtc->cfFormat == CF_METAFILEPICT ||
			lpFormatEtc->cfFormat == CF_PALETTE ||
			lpFormatEtc->cfFormat == CF_BITMAP);
		lpFormatEtc->tymed = lpStgMedium->tymed;

		// add it to the cache
		_OTL_DATACACHE_ENTRY* pEntry = GetCacheEntry(lpFormatEtc, DATADIR_GET);
		pEntry->m_stgMedium = *lpStgMedium;
	}


public:
	void Empty()
	{
		if (m_pDataCache != NULL)
		{
			ATLASSERT(m_nMaxSize != 0);
			ATLASSERT(m_nSize != 0);

			// release all of the STGMEDIUMs and FORMATETCs
			for (UINT nIndex = 0; nIndex < m_nSize; nIndex++)
			{
				CoTaskMemFree(m_pDataCache[nIndex].m_formatEtc.ptd);
				::ReleaseStgMedium(&m_pDataCache[nIndex].m_stgMedium);
			}

			// delete the cache
			delete[] m_pDataCache;
			m_pDataCache = NULL;
			m_nMaxSize = 0;
			m_nSize = 0;
		}
		ATLASSERT(m_pDataCache == NULL);
		ATLASSERT(m_nMaxSize == 0);
		ATLASSERT(m_nSize == 0);
	}

protected:
	_OTL_DATACACHE_ENTRY* Lookup(
		LPFORMATETC lpFormatEtc, DATADIR nDataDir) const
	{
		_OTL_DATACACHE_ENTRY* pLast = NULL;

		// look for suitable match to lpFormatEtc in cache
		for (UINT nIndex = 0; nIndex < m_nSize; nIndex++)
		{
			// get entry from cache at nIndex
			_OTL_DATACACHE_ENTRY* pCache = &m_pDataCache[nIndex];
			FORMATETC *pCacheFormat = &pCache->m_formatEtc;

			// check for match
			if (pCacheFormat->cfFormat == lpFormatEtc->cfFormat &&
				(pCacheFormat->tymed & lpFormatEtc->tymed) != 0 &&
				(pCache->m_stgMedium.tymed == TYMED_NULL ||
					pCacheFormat->lindex == lpFormatEtc->lindex) &&
				pCacheFormat->dwAspect == lpFormatEtc->dwAspect &&
				pCache->m_nDataDir == nDataDir)
			{
				// for backward compatibility we match even if we never
				// find an exact match for the DVTARGETDEVICE
				pLast = pCache;
				DVTARGETDEVICE* ptd1 = pCacheFormat->ptd;
				DVTARGETDEVICE* ptd2 = lpFormatEtc->ptd;

				if (ptd1 == NULL && ptd2 == NULL)
				{
					// both target devices are NULL (exact match), so return it
					break;
				}
				if (ptd1 != NULL && ptd2 != NULL &&
					ptd1->tdSize == ptd2->tdSize &&
					memcmp(ptd1, ptd2, ptd1->tdSize) == 0)
				{
					// exact match, so return it
					break;
				}
			// continue looking for better match
			}
		}
	
		return pLast;    // not found
	}

	// GetCacheEntry adds the entry to the cache if it's not already there...
	_OTL_DATACACHE_ENTRY* GetCacheEntry(
		LPFORMATETC lpFormatEtc, DATADIR nDataDir)
	{
		_OTL_DATACACHE_ENTRY* pEntry = Lookup(lpFormatEtc, nDataDir);
		if (pEntry != NULL)
		{
			// cleanup current entry and return it
			CoTaskMemFree(pEntry->m_formatEtc.ptd);
			::ReleaseStgMedium(&pEntry->m_stgMedium);
		}
		else
		{
			// allocate space for item at m_nSize (at least room for 1 item)
			if (m_pDataCache == NULL || m_nSize == m_nMaxSize)
			{
				ATLASSERT(m_nGrowBy != 0);
				_OTL_DATACACHE_ENTRY* pCache = new _OTL_DATACACHE_ENTRY[m_nMaxSize+m_nGrowBy];
				m_nMaxSize += m_nGrowBy;
				if (m_pDataCache != NULL)
				{
					memcpy(pCache, m_pDataCache, m_nSize * sizeof(_OTL_DATACACHE_ENTRY));
					delete[] m_pDataCache;
				}
				m_pDataCache = pCache;
			}
			ATLASSERT(m_pDataCache != NULL);
			ATLASSERT(m_nMaxSize != 0);

			pEntry = &m_pDataCache[m_nSize++];
		}

		// fill the cache entry with the format and data direction and return it
		pEntry->m_nDataDir = nDataDir;
		pEntry->m_formatEtc = *lpFormatEtc;
		return pEntry;

	}

public:
	//@cmember
	/* IDataObject method for retrieving data */
	STDMETHODIMP GetData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
	{
		// Toss out bad FORMATETCs
		if(lpFormatEtc->lindex != -1) {
			return DV_E_LINDEX ;
		}

		// attempt to find match in the cache
		_OTL_DATACACHE_ENTRY* pCache = Lookup(lpFormatEtc, DATADIR_GET);
		if (pCache == NULL) {
			return DATA_E_FORMATETC;
		}

		memset(lpStgMedium, 0, sizeof(STGMEDIUM));
		if (pCache->m_stgMedium.tymed != TYMED_NULL) {
			// Copy the cached medium into the lpStgMedium provided by caller.
			if (!_CopyStgMedium(lpFormatEtc->cfFormat, 
									lpStgMedium,
									&pCache->m_stgMedium))
				return DATA_E_FORMATETC;

			// format was supported for copying
			return S_OK;
		}

		HRESULT hr  = DATA_E_FORMATETC;
		return hr;
	}

	//@cmember
	/* IDataObject method for retrieving data. Client supplies storage medium */
	STDMETHODIMP GetDataHere(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
	{
		// these two must be the same
		ATLASSERT(lpFormatEtc->tymed == lpStgMedium->tymed);
		lpFormatEtc->tymed = lpStgMedium->tymed;    // but just in case...

		// attempt to find match in the cache
		_OTL_DATACACHE_ENTRY* pCache = Lookup(lpFormatEtc, DATADIR_GET);
		if (pCache == NULL) {
			return DATA_E_FORMATETC;
		}

		// handle cached medium and copy
		if (pCache->m_stgMedium.tymed != TYMED_NULL) {
			// found a cached format -- copy it to dest medium
			ATLASSERT(pCache->m_stgMedium.tymed == lpStgMedium->tymed);
			if (!_CopyStgMedium(lpFormatEtc->cfFormat, lpStgMedium,
			  &pCache->m_stgMedium))
				return DATA_E_FORMATETC;

			// format was supported for copying
			return S_OK;
		}

		HRESULT hr = DATA_E_FORMATETC;

		return hr;

	}

	//@cmember
	/* IDataObject method for determining availability of data format */
	STDMETHODIMP QueryGetData(LPFORMATETC lpFormatEtc)
	{
		// attempt to find match in the cache
		_OTL_DATACACHE_ENTRY* pCache = Lookup(lpFormatEtc, DATADIR_GET);
		if (pCache == NULL)
			return DATA_E_FORMATETC;

		// it was found in the cache or can be rendered -- success
		return S_OK;
	}

	STDMETHODIMP GetCanonicalFormatEtc(LPFORMATETC, LPFORMATETC)
	{
		return DATA_S_SAMEFORMATETC;
	}

	STDMETHODIMP SetData(LPFORMATETC /*lpFormatEtc*/, LPSTGMEDIUM /*lpStgMedium*/, BOOL /*bRelease*/)
	{
		return E_NOTIMPL;
	}

	//@cmember
	/* IDataObject method for retrieving an enumerator for FORMATETCs */
	STDMETHODIMP EnumFormatEtc(DWORD dwDirection, LPENUMFORMATETC* ppenumFormatEtc)
	{
		*ppenumFormatEtc = NULL;

		_EnumFORMATETCImpl* pFormatList = NULL;
		HRESULT hr = E_OUTOFMEMORY;

		// generate a format list from the cache
		pFormatList = new _EnumFORMATETCImpl(0, false);

		for (UINT nIndex = 0; nIndex < m_nSize; nIndex++)
		{
			_OTL_DATACACHE_ENTRY* pCache = &m_pDataCache[nIndex];
			if ((DWORD)pCache->m_nDataDir & dwDirection)
			{
				// entry should be enumerated -- add it to the list
				FORMATETC formatEtc;
				_CopyFormatEtc(&formatEtc, &pCache->m_formatEtc);
				pFormatList->AddFormat(&formatEtc);
			}
		}
		*ppenumFormatEtc = static_cast<IEnumFORMATETC*>(pFormatList);
		pFormatList->AddRef();
		hr = S_OK;

		return hr;	
	}

	//@cmember
	/* IDataObject method not supported */
	STDMETHODIMP DAdvise(LPFORMATETC, DWORD, LPADVISESINK, LPDWORD pdwConnection)
	{
		*pdwConnection = 0;
		return OLE_E_ADVISENOTSUPPORTED;
	}

	//@cmember
	/* IDataObject method not supported */
	STDMETHODIMP DUnadvise(DWORD)
	{
		return OLE_E_ADVISENOTSUPPORTED;
	}

	//@cmember
	/* IDataObject method not supported */
	STDMETHODIMP EnumDAdvise(LPENUMSTATDATA* ppenumAdvise)
	{
		*ppenumAdvise = NULL;
		return OLE_E_ADVISENOTSUPPORTED;
	}
};


};	// namespace stingray::foundation
};	// namespace stingray