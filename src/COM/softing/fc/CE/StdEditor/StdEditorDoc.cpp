
// StdEditorDoc.cpp : implementation of the CStdEditorDoc class
//

#include "stdafx.h"
#include "fc_todebug\fc_assert.h"
#include "utilif.h"
#include "StdEditorIF.h"
#include "StdEditorDoc.h"
#include "CEMainDrivers.h"
#include "CEMsg_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#if _MFC_VER == 0x0421

// helper for reliable and small Release calls
DWORD AFXAPI _AfxRelease(LPUNKNOWN* plpUnknown);
#ifndef _DEBUG
// generate smaller code in release build
#define RELEASE(lpUnk) _AfxRelease((LPUNKNOWN*)&lpUnk)
#else
// generate larger but typesafe code in debug build
#define RELEASE(lpUnk) do \
	{ if ((lpUnk) != NULL) { (lpUnk)->Release(); (lpUnk) = NULL; } } while (0)
#endif

// Note: the following interface is not an actual OLE interface, but is useful
//  for describing an abstract (not typesafe) enumerator.

#undef  INTERFACE
#define INTERFACE   IEnumVOID

DECLARE_INTERFACE_(IEnumVOID, IUnknown)
{
	STDMETHOD(QueryInterface)(REFIID, LPVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)()  PURE;
	STDMETHOD_(ULONG,Release)() PURE;
	STDMETHOD(Next)(ULONG, void*, ULONG*) PURE;
	STDMETHOD(Skip)(ULONG) PURE;
	STDMETHOD(Reset)() PURE;
	STDMETHOD(Clone)(IEnumVOID**) PURE;
};

class CEnumArray : public CCmdTarget
{
// Constructors
public:
	CEnumArray(size_t nSize,
		const void* pvEnum, UINT nCount, BOOL bNeedFree = FALSE);

// Implementation
public:
	virtual ~CEnumArray();

protected:
	size_t m_nSizeElem;     // size of each item in the array
	CCmdTarget* m_pClonedFrom;  // used to keep original alive for clones

	BYTE* m_pvEnum;     // pointer data to enumerate
	UINT m_nCurPos;     // current position in m_pvEnum
	UINT m_nSize;       // total number of items in m_pvEnum
	BOOL m_bNeedFree;   // free on release?

	virtual BOOL OnNext(void* pv);
	virtual BOOL OnSkip();
	virtual void OnReset();
	virtual CEnumArray* OnClone();

// Interface Maps
public:
	BEGIN_INTERFACE_PART(EnumVOID, IEnumVOID)
		INIT_INTERFACE_PART(CEnumArray, EnumVOID)
		STDMETHOD(Next)(ULONG, void*, ULONG*);
		STDMETHOD(Skip)(ULONG);
		STDMETHOD(Reset)();
		STDMETHOD(Clone)(IEnumVOID**);
	END_INTERFACE_PART(EnumVOID)
};

#define GetConnectionPtr(pTarget, pEntry) \
	(LPCONNECTIONPOINT)((char*)pTarget + pEntry->nOffset + \
			offsetof(CConnectionPoint, m_xConnPt))


/////////////////////////////////////////////////////////////////////////////
// CConnectionPoint

CConnectionPoint::CConnectionPoint() :
	m_pUnkFirstConnection(NULL),
	m_pConnections(NULL)
{
}

CConnectionPoint::~CConnectionPoint()
{
	POSITION pos = GetStartPosition();
	while (pos != NULL)
	{
		LPUNKNOWN pUnk = GetNextConnection(pos);
		ASSERT(pUnk != NULL);
		pUnk->Release();
	}

	if (m_pConnections != NULL)
		delete m_pConnections;
}

POSITION CConnectionPoint::GetStartPosition() const
{
	ASSERT(m_pConnections == NULL || m_pUnkFirstConnection == NULL);

	if (m_pUnkFirstConnection != NULL)
		return (POSITION)-1;

	if (m_pConnections == NULL || m_pConnections->GetSize() == 0)
		return NULL;

	return (POSITION)1;
}

LPUNKNOWN CConnectionPoint::GetNextConnection(POSITION& pos) const
{
	ASSERT(pos != NULL);

	if (pos == (POSITION)-1)
	{
		ASSERT(m_pUnkFirstConnection != NULL);
		ASSERT(m_pConnections == NULL);

		pos = NULL;
		return m_pUnkFirstConnection;
	}

	ASSERT(m_pConnections != NULL);
	ASSERT((long)pos > 0 && (long)pos <= m_pConnections->GetSize());

	int nIndex = (long)pos - 1;
	pos = (POSITION)((long)pos + 1);
	if ((long)pos > m_pConnections->GetSize())
		pos = NULL;
	return (LPUNKNOWN)m_pConnections->GetAt(nIndex);
}

const CPtrArray* CConnectionPoint::GetConnections()
{
	ASSERT_VALID(this);
	if (m_pConnections == NULL)
		CreateConnectionArray();

	ASSERT(m_pConnections != NULL);
	return m_pConnections;
}

void CConnectionPoint::OnAdvise(BOOL)
{
	ASSERT_VALID(this);
}

int CConnectionPoint::GetMaxConnections()
{
	ASSERT_VALID(this);

	// May be overridden by subclass.
	return -1;
}

LPCONNECTIONPOINTCONTAINER CConnectionPoint::GetContainer()
{
	CCmdTarget* pCmdTarget = (CCmdTarget*)((BYTE*)this - m_nOffset);
#ifdef _DEBUG
	pCmdTarget->CCmdTarget::AssertValid();
#endif

	LPCONNECTIONPOINTCONTAINER pCPC = NULL;
	if (SUCCEEDED((HRESULT)pCmdTarget->ExternalQueryInterface(
			&IID_IConnectionPointContainer, (LPVOID*)&pCPC)))
	{
		ASSERT(pCPC != NULL);
	}

	return pCPC;
}

void CConnectionPoint::CreateConnectionArray()
{
	ASSERT(m_pConnections == NULL);

	m_pConnections = new CPtrArray;
	if (m_pUnkFirstConnection != NULL)
	{
		m_pConnections->Add(m_pUnkFirstConnection);
		m_pUnkFirstConnection = NULL;
	}

	ASSERT(m_pConnections != NULL);
	ASSERT(m_pUnkFirstConnection == NULL);
}

int CConnectionPoint::GetConnectionCount()
{
	if (m_pUnkFirstConnection != NULL)
		return 1;

	if (m_pConnections == NULL)
		return 0;

	return m_pConnections->GetSize();
}

LPUNKNOWN CConnectionPoint::QuerySinkInterface(LPUNKNOWN pUnkSink)
{
	LPUNKNOWN pUnkReturn = NULL;
	pUnkSink->QueryInterface(GetIID(), reinterpret_cast<void**>(&pUnkReturn));
	return pUnkReturn;
}

STDMETHODIMP_(ULONG) CConnectionPoint::XConnPt::Release()
{
	METHOD_PROLOGUE_EX_(CConnectionPoint, ConnPt)
	return (ULONG)pThis->InternalRelease();
}

STDMETHODIMP_(ULONG) CConnectionPoint::XConnPt::AddRef()
{
	METHOD_PROLOGUE_EX_(CConnectionPoint, ConnPt)
	return (ULONG)pThis->InternalAddRef();
}

STDMETHODIMP CConnectionPoint::XConnPt::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CConnectionPoint, ConnPt)
	ASSERT(AfxIsValidAddress(ppvObj, sizeof(LPVOID), FALSE));

	if (IsEqualIID(iid, IID_IUnknown) ||
		IsEqualIID(iid, IID_IConnectionPoint))
	{
		*ppvObj = this;
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

STDMETHODIMP CConnectionPoint::XConnPt::GetConnectionInterface(IID* pIID)
{
	METHOD_PROLOGUE_EX_(CConnectionPoint, ConnPt)
	ASSERT(AfxIsValidAddress(pIID, sizeof(IID)));

	*pIID = pThis->GetIID();
	return S_OK;
}

STDMETHODIMP CConnectionPoint::XConnPt::GetConnectionPointContainer(
	IConnectionPointContainer** ppCPC)
{
	METHOD_PROLOGUE_EX_(CConnectionPoint, ConnPt)
	ASSERT(AfxIsValidAddress(ppCPC, sizeof(LPCONNECTIONPOINT)));

	if ((*ppCPC = pThis->GetContainer()) != NULL)
		return S_OK;

	return E_FAIL;
}

STDMETHODIMP CConnectionPoint::XConnPt::Advise(
	LPUNKNOWN pUnkSink, DWORD* pdwCookie)
{
	METHOD_PROLOGUE_EX_(CConnectionPoint, ConnPt)
	ASSERT(AfxIsValidAddress(pUnkSink, sizeof(IUnknown), FALSE));
	ASSERT((pdwCookie == NULL) || AfxIsValidAddress(pdwCookie, sizeof(DWORD)));

	if (pUnkSink == NULL)
		return E_POINTER;

	LPUNKNOWN lpInterface;

	int cMaxConn = pThis->GetMaxConnections();
	if ((cMaxConn >= 0) && (pThis->GetConnectionCount() == cMaxConn))
	{
		return CONNECT_E_ADVISELIMIT;
	}

	if ((lpInterface = pThis->QuerySinkInterface(pUnkSink)) != NULL)
	{
		if (pThis->m_pUnkFirstConnection == NULL &&
			pThis->m_pConnections == NULL)
		{
			pThis->m_pUnkFirstConnection = lpInterface;
		}
		else
		{
			if (pThis->m_pConnections == NULL)
				pThis->CreateConnectionArray();

			pThis->m_pConnections->Add(lpInterface);
		}

		pThis->OnAdvise(TRUE);
		if (pdwCookie != NULL)
			*pdwCookie = (DWORD)lpInterface;
		return S_OK;
	}

	return E_NOINTERFACE;
}

STDMETHODIMP CConnectionPoint::XConnPt::Unadvise(DWORD dwCookie)
{
	METHOD_PROLOGUE_EX_(CConnectionPoint, ConnPt)

	if (pThis->m_pUnkFirstConnection != NULL)
	{
		if ((DWORD)pThis->m_pUnkFirstConnection == dwCookie)
		{
			pThis->m_pUnkFirstConnection->Release();
			pThis->m_pUnkFirstConnection = NULL;
			pThis->OnAdvise(FALSE);
			return S_OK;
		}
		else
		{
			return CONNECT_E_NOCONNECTION;
		}
	}

	if (pThis->m_pConnections == NULL)
		return CONNECT_E_NOCONNECTION;

	LPUNKNOWN pUnkSink;
	int cConnections = pThis->m_pConnections->GetSize();
	for (int i = 0; i < cConnections; i++)
	{
		pUnkSink = (LPUNKNOWN)(pThis->m_pConnections->GetAt(i));
		if ((DWORD)pUnkSink == dwCookie)
		{
			pUnkSink->Release();
			pThis->m_pConnections->RemoveAt(i);
			pThis->OnAdvise(FALSE);
			return S_OK;
		}
	}

	return CONNECT_E_NOCONNECTION;
}

/////////////////////////////////////////////////////////////////////////////
// CEnumConnections

class CEnumConnections : public CEnumArray
{
public:
	CEnumConnections(const void* pvEnum, UINT nSize);
	~CEnumConnections();
	void AddConnection(CONNECTDATA* pConn);

protected:
	virtual BOOL OnNext(void* pv);
	virtual CEnumArray* OnClone();

	UINT m_nMaxSize;    // number of items allocated (>= m_nSize)

	DECLARE_INTERFACE_MAP()
};

BEGIN_INTERFACE_MAP(CEnumConnections, CEnumArray)
	INTERFACE_PART(CEnumConnections, IID_IEnumConnections, EnumVOID)
END_INTERFACE_MAP()


CEnumConnections::CEnumConnections(const void* pvEnum, UINT nSize) :
	CEnumArray(sizeof(CONNECTDATA), pvEnum, nSize, TRUE)
{
	m_nMaxSize = 0;
}

CEnumConnections::~CEnumConnections()
{
	if (m_pClonedFrom == NULL)
	{
		UINT iCP;
		CONNECTDATA* ppCP = (CONNECTDATA*)(VOID *)m_pvEnum;
		for (iCP = 0; iCP < m_nSize; iCP++)
			RELEASE(ppCP[iCP].pUnk);
	}
	// destructor will free the actual array (if it was not a clone)
}

BOOL CEnumConnections::OnNext(void* pv)
{
	if (!CEnumArray::OnNext(pv))
		return FALSE;

	// outgoing connection point needs to be AddRef'ed
	//  (the caller has responsibility to release it)

	((CONNECTDATA*)pv)->pUnk->AddRef();
	return TRUE;
}

CEnumArray* CEnumConnections::OnClone()
{
	ASSERT_VALID(this);
	CEnumConnections* pClone;
	pClone = new CEnumConnections(m_pvEnum, m_nSize);
	pClone->m_bNeedFree = FALSE;
	ASSERT(pClone != NULL);
	ASSERT(!pClone->m_bNeedFree);   // clones should never free themselves
	pClone->m_nCurPos = m_nCurPos;

	// finally, return the clone to OLE
	ASSERT_VALID(pClone);
	return pClone;
}

void CEnumConnections::AddConnection(CONNECTDATA* pConn)
{
	ASSERT(m_nSize <= m_nMaxSize);

	if (m_nSize == m_nMaxSize)
	{
		// not enough space for new item -- allocate more
		CONNECTDATA* pListNew = new CONNECTDATA[m_nSize+2];
		m_nMaxSize += 2;
		if (m_nSize > 0)
			memcpy(pListNew, m_pvEnum, m_nSize*sizeof(CONNECTDATA));
		delete m_pvEnum;
#ifdef _WIN32
		m_pvEnum = (BYTE*)pListNew;
#else
		m_pvEnum = (char*)pListNew;
#endif
	}

	// add this item to the list
	ASSERT(m_nSize < m_nMaxSize);
	((CONNECTDATA*)m_pvEnum)[m_nSize] = *pConn;
	pConn->pUnk->AddRef();
	++m_nSize;
}

STDMETHODIMP CConnectionPoint::XConnPt::EnumConnections(LPENUMCONNECTIONS* ppEnum)
{
	METHOD_PROLOGUE_EX(CConnectionPoint, ConnPt)
	CEnumConnections* pEnum = NULL;
	CONNECTDATA cd;

	TRY
	{
		pEnum = new CEnumConnections(NULL, 0);

		if (pThis->m_pUnkFirstConnection != NULL)
		{
			cd.pUnk = pThis->m_pUnkFirstConnection;
			cd.dwCookie = (DWORD)cd.pUnk;
			pEnum->AddConnection(&cd);
		}

		if (pThis->m_pConnections != NULL)
		{
			int cConnections = pThis->m_pConnections->GetSize();
			for (int i = 0; i < cConnections; i++)
			{
				cd.pUnk = (LPUNKNOWN)(pThis->m_pConnections->GetAt(i));
				cd.dwCookie = (DWORD)cd.pUnk;
				pEnum->AddConnection(&cd);
			}
		}
	}
	CATCH (CException, e)
	{
		delete pEnum;
		pEnum = NULL;
	}
	END_CATCH

	if (pEnum != NULL)
	{
		// create and return the IEnumConnectionPoints object
		*ppEnum = (IEnumConnections*)&pEnum->m_xEnumVOID;
	}
	else
	{
		// no connections: return NULL
		*ppEnum = NULL;
	}
	return (pEnum != NULL) ? S_OK : E_OUTOFMEMORY;
}


/////////////////////////////////////////////////////////////////////////////
// CEnumConnPoints

class CEnumConnPoints : public CEnumArray
{
public:
	CEnumConnPoints(const void* pvEnum, UINT nSize);
	~CEnumConnPoints();
	void AddConnPoint(LPCONNECTIONPOINT pConnPt);

protected:
	virtual BOOL OnNext(void* pv);

	UINT m_nMaxSize;    // number of items allocated (>= m_nSize)

	DECLARE_INTERFACE_MAP()
};

BEGIN_INTERFACE_MAP(CEnumConnPoints, CEnumArray)
	INTERFACE_PART(CEnumConnPoints, IID_IEnumConnectionPoints, EnumVOID)
END_INTERFACE_MAP()


CEnumConnPoints::CEnumConnPoints(const void* pvEnum, UINT nSize) :
	CEnumArray(sizeof(LPCONNECTIONPOINT), pvEnum, nSize, TRUE)
{
	m_nMaxSize = 0;
}

CEnumConnPoints::~CEnumConnPoints()
{
	if (m_pClonedFrom == NULL)
	{
		UINT iCP;
		LPCONNECTIONPOINT* ppCP =
			(LPCONNECTIONPOINT*)(VOID *)m_pvEnum;
		for (iCP = 0; iCP < m_nSize; iCP++)
			RELEASE(ppCP[iCP]);
	}
	// destructor will free the actual array (if it was not a clone)
}

BOOL CEnumConnPoints::OnNext(void* pv)
{
	if (!CEnumArray::OnNext(pv))
		return FALSE;

	// outgoing connection point needs to be AddRef'ed
	//  (the caller has responsibility to release it)

	(*(LPCONNECTIONPOINT*)pv)->AddRef();
	return TRUE;
}

void CEnumConnPoints::AddConnPoint(LPCONNECTIONPOINT pConnPt)
{
	ASSERT(m_nSize <= m_nMaxSize);

	if (m_nSize == m_nMaxSize)
	{
		// not enough space for new item -- allocate more
		LPCONNECTIONPOINT* pListNew = new LPCONNECTIONPOINT[m_nSize+2];
		m_nMaxSize += 2;
		if (m_nSize > 0)
			memcpy(pListNew, m_pvEnum, m_nSize*sizeof(LPCONNECTIONPOINT));
		delete m_pvEnum;
#ifdef _WIN32
		m_pvEnum = (BYTE*)pListNew;
#else
		m_pvEnum = (char*)pListNew;
#endif
	}

	// add this item to the list
	ASSERT(m_nSize < m_nMaxSize);
	((LPCONNECTIONPOINT*)m_pvEnum)[m_nSize] = pConnPt;
	pConnPt->AddRef();
	++m_nSize;
}


/////////////////////////////////////////////////////////////////////////////
// COleConnPtContainer

class COleConnPtContainer : public IConnectionPointContainer
{
public:
#ifndef _AFX_NO_NESTED_DERIVATION
	// required for METHOD_PROLOGUE_EX
	size_t m_nOffset;
	COleConnPtContainer::COleConnPtContainer()
		{ m_nOffset = offsetof(CCmdTarget, m_xConnPtContainer); }
#endif

	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();
	STDMETHOD(QueryInterface)(REFIID, LPVOID*);

	STDMETHOD(EnumConnectionPoints)(LPENUMCONNECTIONPOINTS* ppEnum);
	STDMETHOD(FindConnectionPoint)(REFIID iid, LPCONNECTIONPOINT* ppCP);
};

STDMETHODIMP_(ULONG) COleConnPtContainer::AddRef()
{
	METHOD_PROLOGUE_EX_(CCmdTarget, ConnPtContainer)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COleConnPtContainer::Release()
{
	METHOD_PROLOGUE_EX_(CCmdTarget, ConnPtContainer)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP COleConnPtContainer::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CCmdTarget, ConnPtContainer)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP COleConnPtContainer::EnumConnectionPoints(
	LPENUMCONNECTIONPOINTS* ppEnum)
{
	METHOD_PROLOGUE_EX_(CCmdTarget, ConnPtContainer)

	CEnumConnPoints* pEnum = NULL;

	TRY
	{
		pEnum = new CEnumConnPoints(NULL, 0);

		// Add connection points that aren't in the connection map
		CPtrArray ptrArray;
		if (pThis->GetExtraConnectionPoints(&ptrArray))
		{
			for (int i = 0; i < ptrArray.GetSize(); i++)
				pEnum->AddConnPoint((LPCONNECTIONPOINT)ptrArray.GetAt(i));
		}

		// walk the chain of connection maps
		const AFX_CONNECTIONMAP* pMap = pThis->GetConnectionMap();
		const AFX_CONNECTIONMAP_ENTRY* pEntry;

		while (pMap != NULL)
		{
			pEntry = pMap->pEntry;

			while (pEntry->piid != NULL)
			{
				pEnum->AddConnPoint(GetConnectionPtr(pThis, pEntry));
				++pEntry;
			}
#ifdef _AFXDLL
			pMap = (*pMap->pfnGetBaseMap)();
#else
			pMap = pMap->pBaseMap;
#endif
		}
	}
	CATCH (CException, e)
	{
		delete pEnum;
		pEnum = NULL;
	}
	END_CATCH

	if (pEnum != NULL)
	{
		// create and return the IEnumConnectionPoints object
		*ppEnum = (IEnumConnectionPoints*)&pEnum->m_xEnumVOID;
	}
	else
	{
		// no connection points: return NULL
		*ppEnum = NULL;
	}

	return (pEnum != NULL) ? S_OK : CONNECT_E_NOCONNECTION;
}

STDMETHODIMP COleConnPtContainer::FindConnectionPoint(
	REFIID iid, LPCONNECTIONPOINT* ppCP)
{
	METHOD_PROLOGUE_EX_(CCmdTarget, ConnPtContainer)
	ASSERT(ppCP != NULL);

	if ((*ppCP = pThis->GetConnectionHook(iid)) != NULL)
	{
		(*ppCP)->AddRef();
		return S_OK;
	}

	const AFX_CONNECTIONMAP* pMap = pThis->GetConnectionMap();
	const AFX_CONNECTIONMAP_ENTRY* pEntry;

	while (pMap != NULL)
	{
		pEntry = pMap->pEntry;

		while (pEntry->piid != NULL)
		{
			if (IsEqualIID(iid, *(IID*)(pEntry->piid)))
			{
				*ppCP = GetConnectionPtr(pThis, pEntry);
				(*ppCP)->AddRef();
				return S_OK;
			}
			++pEntry;
		}
#ifdef _AFXDLL
		pMap = (*pMap->pfnGetBaseMap)();
#else
		pMap = pMap->pBaseMap;
#endif
	}

	return E_NOINTERFACE;
}


/////////////////////////////////////////////////////////////////////////////
// Wiring CCmdTarget to COleConnPtContainer

// enable this object for OLE connections, called from derived class ctor
void CCmdTarget::EnableConnections()
{
	ASSERT(GetConnectionMap() != NULL);   // must have DECLARE_DISPATCH_MAP

	// construct an COleConnPtContainer instance just to get to the vtable
	COleConnPtContainer cpc;

	// vtable pointer should be already set to same or NULL
	ASSERT(m_xConnPtContainer.m_vtbl == NULL||
		*(DWORD*)&cpc == m_xConnPtContainer.m_vtbl);
	// verify that sizes match
	ASSERT(sizeof(m_xConnPtContainer) == sizeof(COleConnPtContainer));

	// copy the vtable (and other data) to make sure it is initialized
	m_xConnPtContainer.m_vtbl = *(DWORD*)&cpc;
	*(COleConnPtContainer*)&m_xConnPtContainer = cpc;
}

#endif // _MFC_VER == 0x0421


const int CStdEditorDoc::GotoPositionHint = 1000;
const int CStdEditorDoc::GetPositionHint  = 1001;
const int CStdEditorDoc::SetColorHint     = 1002;
const int CStdEditorDoc::SetFontHint      = 1003;
const int CStdEditorDoc::GotoLocationHint = 1004;


/////////////////////////////////////////////////////////////////////////////
// CStdEditorDoc

IMPLEMENT_DYNAMIC(CStdEditorDoc, COleServerDoc)

BEGIN_MESSAGE_MAP(CStdEditorDoc, COleServerDoc)
	//{{AFX_MSG_MAP(CStdEditorDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CStdEditorDoc, COleServerDoc)
	//{{AFX_DISPATCH_MAP(CStdEditorDoc)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


BEGIN_INTERFACE_MAP(CStdEditorDoc, COleServerDoc)
	INTERFACE_PART(CStdEditorDoc, IID_ICEEdit, CEEditObj)
    INTERFACE_PART(CStdEditorDoc, IID_ICEPrint, CEPrintObj)
	INTERFACE_PART(CStdEditorDoc, IID_IConnectionPointContainer, ConnPtContainer)
END_INTERFACE_MAP()

BEGIN_CONNECTION_MAP(CStdEditorDoc, COleServerDoc)
	CONNECTION_PART(CStdEditorDoc, IID_ICEEditNotify, CEEditNotifyConnPt)
	CONNECTION_PART(CStdEditorDoc, IID__ICEMessageEvent, CEMessageEventConnPt)
END_CONNECTION_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStdEditorDoc construction/destruction

CStdEditorDoc::CStdEditorDoc()
: m_lPageWidth (0),
  m_lPageHeight (0),
  m_bForcedReadOnly (false),
  m_bDelayedClose (false),
  m_bReadonlyCached (false),
  m_bReadonlyCache (false),
  m_uiLockCount (0)
{
	m_ClientExtent.cx = 0;
	m_ClientExtent.cy = 0;
}


CStdEditorDoc::~CStdEditorDoc()
{
}


/////////////////////////////////////////////////////////////////////////////
// CStdEditorDoc server implementation


/////////////////////////////////////////////////////////////////////////////
// CStdEditorDoc ActiveX Document server implementation

BOOL CStdEditorDoc::OnOpenDocument (LPCTSTR lpszPathName)
{
	if (!COleServerDoc::OnOpenDocument (lpszPathName))
		return (FALSE);

	if (lpszPathName != NULL)
    {
		m_strDocumentFile = lpszPathName;
        m_strBaseFile = lpszPathName;
    }

	return (TRUE);
}


BOOL CStdEditorDoc::OnSaveDocument (LPCTSTR lpszPathName)
{
	if (!COleServerDoc::OnSaveDocument (lpszPathName))
		return (FALSE);

	if (lpszPathName != NULL)
    {
		m_strDocumentFile = lpszPathName;
        m_strBaseFile = lpszPathName;
    }

	return (TRUE);
}


void CStdEditorDoc::OnCloseDocument ()
{

    // Don't close the document as long the client does not allow
    // the active document to die because of the following scenario.
    // 1) Open ST editor standalone
    // 2) Open engineering
    // 3) Open a project in engineering which contains a 4gr file
    // 4) Open the 4gr file inside engineering
    // 5) Validate that only one STEditor process is running
    // 6) Click into ST part of the split editor window
    // 7) Click into FBD part of the split editor window
    // 8) Close the standalone editor
    // 9) Click into the ST part of the split editor window inside
    //    engineering
    // The effect was that the ST part never became active again
    // because during shutdown the ST editor application detected that
    // the ST part was not inplace active and thus closed the active
    // document which in turn disconnects the active document from the
    // client.Regardless of whether the server continues to execute
    // (because there are inplace active documents) or not (because all
    // documents were closed) the ST part active documents interface
    // pointers at the client side will never become valid again (the
    // object has been disconnected!).
    // The solution (workaround?) is to advise the active document to
    // lock the application as long as some client still refers to it.
    // Advise is done through ICEEdit.Lock. Note that calling OleLock-
    // Running at the client side did not work for whatever reason.

    if (m_uiLockCount != 0)
        return;

    m_bDelayedClose = QueryDelayClose ();

    if (m_bDelayedClose)
    {
        return;
    }

    COleServerDoc::OnCloseDocument ();
}


bool CStdEditorDoc::LoadDocument (const CString &strFileName)
{
	CFile file;
	CFileException fe;

	if (!file.Open (strFileName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		return (false);
	}

	CArchive archive (&file, CArchive::load);


	// Set document file name before calling OnSerializeDocument. This
	// way OnSerializeDocument can make decisions based on the file name.
	// Cannot use m_strPathName because an embedded COleDocument is valid
	// only with m_strPathName == "" (see AssertValid)

	m_strDocumentFile = strFileName;
    m_strBaseFile = strFileName;

	OnSerializeDocument (archive);

	archive.Close ();
	file.Close ();

	if (!IsInPlaceActive ())
	{
		GetEmbeddedItem ()->NotifyChanged ();
	}

	return (true);
}


bool CStdEditorDoc::SaveDocument (const CString &strFileName)
{
	CFile file;
	CFileException fe;


	if (!file.Open (strFileName, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive, &fe))
	{
		return (false);
	}

	CArchive archive (&file, CArchive::store);


	// Set document file name before calling OnSerializeDocument. This
	// way OnSerializeDocument can make decisions based on the file name.
	// Cannot use m_strPathName because an embedded COleDocument is valid
	// only with m_strPathName == "" (see AssertValid)

    if (m_strDocumentFile == m_strBaseFile)
    {
	    m_strDocumentFile = strFileName;
        m_strBaseFile = strFileName;
    }
    else
    {
        m_strDocumentFile = strFileName;
    }


	OnSerializeDocument (archive);

	archive.Close ();
	file.Close ();

	return (true);
}


void CStdEditorDoc::SetModified (bool bModified)
{
	SetModifiedFlag (bModified);
}


bool CStdEditorDoc::GetModified ()
{
	OnUpdateModified ();
	return (m_bModified != FALSE);
}


void CStdEditorDoc::ForceReadOnly (bool bOn)
{
	m_bForcedReadOnly = bOn;
	OnUpdateReadOnly ();
}


void CStdEditorDoc::GotoPosition (CStdEditorPosition position)
{
	UpdateAllViews (NULL, GotoPositionHint, &position);
}


void CStdEditorDoc::GetPosition (CStdEditorPosition &position)
{
	UpdateAllViews (NULL, GetPositionHint, &position);
}


CString CStdEditorDoc::GetDocumentFile ()
{
	return (m_strDocumentFile);
}


CSize CStdEditorDoc::GetClientExtent ()
{
	return (m_ClientExtent);
}


CString CStdEditorDoc::GetPrinterName ()
{
	return (m_strPrinterName);
}


long CStdEditorDoc::GetPageWidth ()
{
	return (m_lPageWidth);
}


long CStdEditorDoc::GetPageHeight ()
{
	return (m_lPageHeight);
}


void CStdEditorDoc::OnUpdateReadOnly ()
{
}


void CStdEditorDoc::OnUpdateModified ()
{
}


void CStdEditorDoc::SetLogicalColor (CStdEditorColor color)
{
	UpdateAllViews (NULL, SetColorHint, &color);
}


void CStdEditorDoc::SetFont (CFont *pFont)
{
	UpdateAllViews (NULL, SetFontHint, pFont);
}


HRESULT CStdEditorDoc::SetClientExtent (CSize size)
{
	m_ClientExtent = size;
	GetEmbeddedItem()->NotifyChanged (); //	m_pEmbeddedItem->NotifyChanged ();
	return (S_OK);
}


HRESULT CStdEditorDoc::GotoLocation (const CString &strLocation)
{
	CString strTemp=strLocation;

	UpdateAllViews (NULL, GotoLocationHint, (CObject *)&strTemp);
	return (S_OK);
}


HRESULT CStdEditorDoc::FindFirst (const CString &strStringToSearch, UINT uiOption, CString &strLocation)
{
	return (E_NOTIMPL);
}


HRESULT CStdEditorDoc::FindNext (CString &strLocation)
{
	return (E_NOTIMPL);
}


HRESULT CStdEditorDoc::SetPrintInfo (const CString &strPrinterName, long lWidth, long lHeight)
{
	m_strPrinterName = strPrinterName;
	m_lPageWidth = lWidth;
	m_lPageHeight = lHeight;

	return (S_OK);
}


HRESULT CStdEditorDoc::GetPageCount (long &lCount)
{
	return (E_NOTIMPL);
}


HRESULT CStdEditorDoc::GetPages (CStringArray &astrPages)
{
	return (E_NOTIMPL);
}

HRESULT CStdEditorDoc::GetPrintOptions(CString& rstrOptions)
{
	return (E_NOTIMPL);
}


HRESULT CStdEditorDoc::SetPrintOptions(const CString& crstrOptions)
{
	return (E_NOTIMPL);
}


HRESULT CStdEditorDoc::ShowPrintOptionsDialog(CString& rstrOptions)
{
	return (E_NOTIMPL);
}


HRESULT CStdEditorDoc::SetBaseFile (const CString &strBaseFile)
{
    m_strBaseFile = strBaseFile;
    m_bReadonlyCached = false;
    return (S_OK);
}


HRESULT CStdEditorDoc::Lock (bool bLock)
{
    if (bLock)
    {
        AfxOleLockApp ();
        ++m_uiLockCount;
    }
    else
    {
        AfxOleUnlockApp ();
        --m_uiLockCount;
    }

    return (S_OK);
}


//------------------------------------------------------------------*
/**
 * get selected text: base class returns empty string.
 *
 * @param           -
 * @return          selected text
 * @exception       -
 * @see             
*/
CString CStdEditorDoc::GetSelectedText()
{
    return CString();
}


//------------------------------------------------------------------*
/**
 * prepare edit.
 *
 *  base class does nothing
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
BOOL CStdEditorDoc::PrepareEdit()
{
    return TRUE;
}


bool CStdEditorDoc::CheckReadonly ()
{
    if (m_bReadonlyCached)
    {
        return (m_bReadonlyCache);
    }

    if (m_strBaseFile.IsEmpty ())
    {
        return (false);
    }

    // look at file
    CFileStatus fileStatus;
    if (!CFile::GetStatus (m_strBaseFile, fileStatus))
    {
        return (false);
    }

    m_bReadonlyCached = true;
    m_bReadonlyCache = (fileStatus.m_attribute & CFile::readOnly) != 0;

    return m_bReadonlyCache;
}


void CStdEditorDoc::InvalidateReadonlyCache()
{
    m_bReadonlyCached = false;
}


/////////////////////////////////////////////////////////////////////////////
// CStdEditorDoc serialization

void CStdEditorDoc::Serialize (CArchive &ar)
{

	// If we are not embedded do not call the base classes
	// version. We just want our special document information
	// to be written (no bells and whistles around). In case
	// of embedding those bells and whistles are necessary
	// for the compound document to work.

	if (m_bEmbedded)
	{
		COleServerDoc::Serialize (ar);
	}


	// Ask subclass to serialize our
	// special document information.

	OnSerializeDocument (ar);
}


void CStdEditorDoc::DeleteContents ()
{
	COleServerDoc::DeleteContents ();

	m_strDocumentFile = "";
    m_strBaseFile = "";
    m_bReadonlyCached = false;
}


void CStdEditorDoc::PositionChanged (const CStdEditorPosition &edp)
{
	POSITION pPosition;

	pPosition = m_xCEEditNotifyConnPt.GetStartPosition();
	while (pPosition != NULL)
	{
		CComQIPtr<ICEEditNotify> pEditNotify;

		pEditNotify = m_xCEEditNotifyConnPt.GetNextConnection (pPosition);
		ASSERT (pEditNotify != NULL);
		if (pEditNotify == NULL)
			continue;

		pEditNotify->OnPositionChanged (edp.GetPosQual(), edp.GetX(), edp.GetY(), edp.GetCX(), edp.GetCY());
	}
}


HRESULT CStdEditorDoc::GotoURL (const CString &strURL)
{
	HRESULT hr;
	CComPtr<IDispatch> pGenEdCont;
	CGenEdContDriver driver;

	hr = GetGenEdCont (pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	}

	ASSERT (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	}

	driver.AttachDispatch (pGenEdCont, FALSE);

	TRY
	{
		driver.ShowURL (strURL);
	}
	CATCH (COleException, e)
	{
		return (COleException::Process (e));
	}
	END_CATCH

	return (S_OK);
}

void CStdEditorDoc::PrintSystemMessage (WORD wFacility, HRESULT hrMessage)
{
	CString strMessage;

	if (!GetMessageText (hrMessage, strMessage))
	{
		return;
	}

    PrintMessage(wFacility,hrMessage,strMessage);
}

void CStdEditorDoc::PrintMessage (WORD wFacility, HRESULT hrMessage, CString &strMessage)
{
	POSITION pPosition = m_xCEMessageEventConnPt.GetStartPosition ();
	while (pPosition != NULL)
	{
		CComQIPtr<_ICEMessageEvent> pICEMessageEvent;

		pICEMessageEvent = m_xCEMessageEventConnPt.GetNextConnection (pPosition);
		ASSERT (pICEMessageEvent != NULL);
		if (pICEMessageEvent == NULL)
			continue;

		pICEMessageEvent->OnNewMessage (CComBSTR (""), wFacility, hrMessage, CComBSTR (strMessage));
	}
}

bool CStdEditorDoc::GetMessageText (HRESULT hr, CString &strMessage)
{
	HINSTANCE hInstance;
	DWORD dwResult;
	LPVOID pBuffer;

	hInstance = AfxGetResourceHandle ();
	ASSERT (hInstance != NULL);
	if (hInstance == NULL)
	{
		return (false);
	}

	dwResult = FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE,
	                          hInstance,
	                          hr,
	                          UTIL_GetInstallLanguage (),
	                          (LPTSTR)&pBuffer,
	                          0,
	                          NULL);

	if (dwResult == 0)
	{
		return (false);
	}

	ASSERT (pBuffer != NULL);
	if (pBuffer == NULL)
	{
		return (false);
	}

	strMessage = (LPCTSTR)pBuffer;

	::LocalFree (pBuffer);

	return (true);
}


HRESULT CStdEditorDoc::GetGenEdItem (CComPtr<IDispatch> &pGenEdItem)
{
	HRESULT hr;

	if (m_lpClientSite == NULL)
	{
		return (E_NOINTERFACE);
	}

	hr = m_lpClientSite->QueryInterface (IID_IDispatch, (LPVOID *)&pGenEdItem);
	if (FAILED (hr))
	{
		return (E_NOINTERFACE);
	}

	ASSERT (pGenEdItem != NULL);
	if (pGenEdItem == NULL)
	{
		return (E_UNEXPECTED);
	}

	return (S_OK);
}


HRESULT CStdEditorDoc::GetGenEdDoc (CComPtr<IDispatch> &pGenEdDoc)
{
	HRESULT hr;
	CComPtr<IDispatch> pGenEdItem;
	CGenEdItemDriver driver;

	hr = GetGenEdItem (pGenEdItem);
	if (FAILED (hr))
	{
		return (hr);
	}

	ASSERT (pGenEdItem != NULL);
	if (pGenEdItem == NULL)
	{
		return (E_UNEXPECTED);
	}

	driver.AttachDispatch (pGenEdItem, FALSE);
	hr = driver.GetDocument (&pGenEdDoc);
	if (FAILED (hr))
	{
		return (hr);
	}

	ASSERT (pGenEdDoc != NULL);
	if (pGenEdDoc == NULL)
	{
		return (E_UNEXPECTED);
	}

	return (S_OK);
}


HRESULT CStdEditorDoc::GetGenEdCont (CComPtr<IDispatch> &pGenEdCont)
{
	HRESULT hr;
	CComPtr<IDispatch> pGenEdDoc;
	CGenEdDocDriver driver;

	hr = GetGenEdDoc (pGenEdDoc);
	if (FAILED (hr))
	{
		return (hr);
	}

	ASSERT (pGenEdDoc != NULL);
	if (pGenEdDoc == NULL)
	{
		return (E_UNEXPECTED);
	}

	driver.AttachDispatch (pGenEdDoc, FALSE);
	hr = driver.GetContainer (&pGenEdCont);
	if (FAILED (hr))
	{
		return (hr);
	}

	ASSERT (pGenEdCont != NULL);
	if (pGenEdCont == NULL)
	{
		return (E_UNEXPECTED);
	}

	return (S_OK);
}


void CStdEditorDoc::OnDocWindowActivate (BOOL bActivate)
{
	// HACK class to get access to protected member m_lpDocFrame
	class myIPFrame : public COleIPFrameWnd
	{
	public:
		LPOLEINPLACEUIWINDOW GetOleDocFrame () { return (m_lpDocFrame); }
	};

	COleServerDoc::OnDocWindowActivate (bActivate);

	// It seems as if MFC forgets to negotiate border space
	// for the document frame on activation of the document
	// window.

	OnResizeBorder (NULL, ((myIPFrame *)m_pInPlaceFrame)->GetOleDocFrame (), FALSE);
}


void CStdEditorDoc::OnIdle ()
{
    COleServerDoc::OnIdle ();

    if (m_bDelayedClose)
    {
        OnCloseDocument ();
    }

    // maybe this will become intersting later
    // at the moment the read only state in the engineering
    // is update only once at the beginning, so to be
    // compatible to this behaviour we do the same here

//    // all 3 seconds: invalidate read only state
    static  CTime statLastTime = CTime::GetCurrentTime();
    CTime   tTimeTmp = CTime::GetCurrentTime();
    CTimeSpan   tTimeSpan(0,0,0,3);
    if((tTimeTmp - tTimeSpan) < statLastTime)
    {
        return;
    }

    statLastTime = CTime::GetCurrentTime();
    InvalidateReadonlyCache();
}


bool CStdEditorDoc::QueryDelayClose ()
{
    return (false);
}


//------------------------------------------------------------------*
/**
 * parse print options.
 *
 *  find print attribute and return value
 *  options string contains attribute=value;attribute=value
 *
 * @param           crstrOptions: options string.
 * @param           crstrAttribute: attribute string
 * @param           rstrValue: value string
 * @return          successful?
 * @exception       -
 * @see             -
*/
bool CStdEditorDoc::ParsePrintOptions(const CString& crstrOptions, const CString& crstrAttribute, CString& rstrValue)
{
    // parse options string
    int iFound = crstrOptions.Find(crstrAttribute);
    if(iFound >= 0)
    {
        CString strTmp;
        int     iStrlen = crstrAttribute.GetLength();

        strTmp = crstrOptions.Mid(iFound + iStrlen);
        if(strTmp[0] == _T('='))
        {
            int iSemicolon = strTmp.Find(_T(';'));
            if(iSemicolon > 0)
            {
                strTmp = strTmp.Mid(1, iSemicolon);
            }
            else
            {
                strTmp = strTmp.Mid(1);
            }
            rstrValue = strTmp;
            return true;
        }
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////
// CStdEditorDoc diagnostics

#ifdef _DEBUG
void CStdEditorDoc::AssertValid() const
{
	COleServerDoc::AssertValid();
}

void CStdEditorDoc::Dump(CDumpContext& dc) const
{
	COleServerDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStdEditorDoc commands

/////////////////////////////////////////////////////////////////////////////
// embedded interface XCEEditObj commands

STDMETHODIMP_(ULONG) CStdEditorDoc::XCEEditObj::AddRef ()
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)
    
	return (pThis->ExternalAddRef ());
}

STDMETHODIMP_(ULONG) CStdEditorDoc::XCEEditObj::Release ()
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	return (pThis->ExternalRelease ());
}

STDMETHODIMP CStdEditorDoc::XCEEditObj::QueryInterface (REFIID iid, LPVOID *ppvObj)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	return ((HRESULT)pThis->ExternalQueryInterface (&iid, ppvObj));
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::LoadDocument (BSTR szDocName)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

    return (pThis->LoadDocument (szDocName) ? S_OK : E_FAIL);
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::SaveDocument(BSTR szDocName)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	return (pThis->SaveDocument (szDocName) ? S_OK : E_FAIL);
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::GetModified ()
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

    return (pThis->GetModified () ? S_OK : S_FALSE);
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::SetModified (BOOL bModified)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	pThis->SetModified (bModified != FALSE);
	return (S_OK);
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::GetPosition (EDP_POS_QUALIFIER* posQual, long* lX, long* lY, long* lCX, long* lCY)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	CStdEditorPosition position;

	pThis->GetPosition (position);
	*posQual = position.GetPosQual();
    *lX =      position.GetX();  
    *lY =      position.GetY();
    *lCX=      position.GetCX();
    *lCX=      position.GetCY();

	return (S_OK);
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::GotoPosition (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	pThis->GotoPosition (CStdEditorPosition (posQual, lX, lY, lCX, lCY));
	return (S_OK);
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::ForceReadOnly (BOOL bOn)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	pThis->ForceReadOnly (bOn != FALSE);
	return (S_OK);
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::SetLogicalColor (unsigned int uiKind, OLE_COLOR color)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	COLORREF cr;
	HRESULT hr;

	hr = ::OleTranslateColor (color, NULL, &cr);
	if (FAILED (hr))
	{
		return (hr);
	}

	pThis->SetLogicalColor (CStdEditorColor ((CStdEditorColor::LogicalColor)uiKind, cr));
	return (S_OK);
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::SetFont (BSTR font, int iSize, int iFlags)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	// INCOMPLETE: Font erzeugen
	// CFont *pFont;
	// pFont = ...
	// pThis->SetFont (pFont);

	pThis->SetFont (NULL);
	return (S_OK);
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::SetClientExtent (long lWidth, long lHeight)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	return (pThis->SetClientExtent (CSize (lWidth, lHeight)));
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::GotoLocation (BSTR szLocation)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	return (pThis->GotoLocation (szLocation));
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::FindFirst (BSTR sStringToSearch, UINT uiOption, BSTR *psLocation)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	HRESULT hr;
	CString strLocation;

	hr = pThis->FindFirst (sStringToSearch, uiOption, strLocation);
	if (SUCCEEDED (hr))
	{
		*psLocation = strLocation.AllocSysString ();
	}

	return (hr);
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::FindNext (BSTR *psLocation)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	HRESULT hr;
	CString strLocation;

	hr = pThis->FindNext (strLocation);
	if (SUCCEEDED (hr))
	{
		*psLocation = strLocation.AllocSysString ();
	}

	return (hr);
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::SetPrintInfo (BSTR sPrinterName, long lWidth, long lHeight)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	HRESULT hr;

	hr = pThis->SetPrintInfo (sPrinterName, lWidth, lHeight);
	return (hr);
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::GetPageCount (long *lCount)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	HRESULT hr;

	hr = pThis->GetPageCount (*lCount);
	return (hr);
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::GetPages (SAFEARRAY **pPages)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	HRESULT hr;
	CStringArray astrPages;
	SAFEARRAYBOUND bound;
	long lIndex;

	hr = pThis->GetPages (astrPages);
	if (FAILED (hr))
	{
		*pPages = NULL;
		return (hr);
	}

	bound.lLbound = 0;
	bound.cElements = astrPages.GetSize ();;

	*pPages = ::SafeArrayCreate (VT_BSTR, 1, &bound);
	ASSERT (*pPages != NULL);
	if (*pPages == NULL)
	{
		//FC_DEBUGPRINT ("STDEDITOR> CStdEditorDoc::XCEEditObj::GetPages: failed to create pages SAFEARRAY\n");
		return (E_FAIL);
	}

	for (lIndex = 0; lIndex < (long)bound.cElements; ++lIndex)
	{
		CString strPage;

		strPage = astrPages.GetAt (lIndex);

		hr = ::SafeArrayPutElement (*pPages, &lIndex, (void *)(BSTR)CComBSTR (strPage));
		if (FAILED (hr))
		{
			//FC_DEBUGPRINT1 ("STDEDITOR> CStdEditorDoc::XCEEditObj::GetPages: failed to put page SAFEARRAY element (hr=0x%08lx)\n", hr);
			::SafeArrayDestroy (*pPages);
			*pPages = NULL;
			return (hr);
		}
	}

	return (S_OK);
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::SetBaseFile (BSTR szBaseFile)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	HRESULT hr;

	hr = pThis->SetBaseFile (szBaseFile);
	return (hr);
}


STDMETHODIMP CStdEditorDoc::XCEEditObj::Lock (BOOL bLock)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

	HRESULT hr;

	hr = pThis->Lock (bLock == TRUE);
	return (hr);
}

STDMETHODIMP CStdEditorDoc::XCEEditObj::GetSelectedText(BSTR* psText)
{
    METHOD_PROLOGUE_EX (CStdEditorDoc, CEEditObj)

    HRESULT hr = S_OK;

    ASSERT(psText);
    if(psText)
    {
        CString strText = pThis->GetSelectedText();
        *psText = strText.AllocSysString();
    }
    else
    {
        hr = E_POINTER;
    }
    
    return (hr);
}


///////////////////////////////////////////////////////////////////
// Overridden methods of CConnectionPoint for ICEEditNotify sinks.

void CStdEditorDoc::XCEEditNotifyConnPt::OnAdvise (BOOL bAdvise)
{
}


/////////////////////////////////////////////////////////////////////
// Overridden methods of CConnectionPoint for _ICEMessageEvent sinks.

void CStdEditorDoc::XCEMessageEventConnPt::OnAdvise (BOOL bAdvise)
{
}



STDMETHODIMP_(ULONG) CStdEditorDoc::XCEPrintObj::AddRef ()
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEPrintObj)
    
	return (pThis->ExternalAddRef ());
}

STDMETHODIMP_(ULONG) CStdEditorDoc::XCEPrintObj::Release ()
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEPrintObj)

	return (pThis->ExternalRelease ());
}

STDMETHODIMP CStdEditorDoc::XCEPrintObj::QueryInterface (REFIID iid, LPVOID *ppvObj)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEPrintObj)

	return ((HRESULT)pThis->ExternalQueryInterface (&iid, ppvObj));
}


STDMETHODIMP CStdEditorDoc::XCEPrintObj::PRLoadDocument(/*[in]*/ BSTR sFilePathName)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEPrintObj)

	return (pThis->LoadDocument (sFilePathName) ? S_OK : E_FAIL);
}


STDMETHODIMP CStdEditorDoc::XCEPrintObj::PRSetPrintInfo(/*[in, string]*/ BSTR sPrinterName, /*[in]*/ long lWidth, /*[in]*/ long lHeight)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEPrintObj)

	HRESULT hr = pThis->SetPrintInfo (sPrinterName, lWidth, lHeight);

	return (hr);
}


STDMETHODIMP CStdEditorDoc::XCEPrintObj::PRGetModuleName(/*[out,string]*/ BSTR* psModuleName)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEPrintObj)

	HRESULT hr = S_OK;

    CString strModuleName = AfxGetAppName();

    *psModuleName = strModuleName.AllocSysString();

	return (hr);
}


STDMETHODIMP CStdEditorDoc::XCEPrintObj::PRGetPrintOptions(/*[out, string]*/ BSTR* psOptions)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEPrintObj)

    CString strPrintOptions;

	HRESULT hr = pThis->GetPrintOptions(strPrintOptions);

    if(hr == S_OK)
    {
        *psOptions = strPrintOptions.AllocSysString();
    }

	return (hr);
}


STDMETHODIMP CStdEditorDoc::XCEPrintObj::PRSetPrintOptions(/*[in, string]*/ BSTR sOptions)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEPrintObj)

	HRESULT hr = pThis->SetPrintOptions(sOptions);

	return (hr);
}


STDMETHODIMP CStdEditorDoc::XCEPrintObj::PRShowPrintOptionsDialog(/*[out, string]*/ BSTR* psOptions)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEPrintObj)

    CString strOptions;
    if(psOptions)
    {
        strOptions = *psOptions;
    }

	HRESULT hr = pThis->ShowPrintOptionsDialog(strOptions);

    if(hr == S_OK)
    {
        ::SysFreeString(*psOptions);
        *psOptions = strOptions.AllocSysString();
    }

	return (hr);
}


STDMETHODIMP CStdEditorDoc::XCEPrintObj::PRGetPageCount(/*[out]*/ long *lCount)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEPrintObj)

	HRESULT hr = pThis->GetPageCount(*lCount);

	return (hr);
}


STDMETHODIMP CStdEditorDoc::XCEPrintObj::PRGetPages(/*[out]*/ SAFEARRAY/*(BSTR)*/ **ppsaPages)
{
	METHOD_PROLOGUE_EX (CStdEditorDoc, CEPrintObj)

	HRESULT         hr;
    CStringArray    astrPages;
	SAFEARRAYBOUND  bound;
	long            lIndex;

	hr = pThis->GetPages (astrPages);
	if (FAILED (hr))
	{
		return (hr);
	}

	bound.lLbound = 0;
	bound.cElements = astrPages.GetSize ();;

	*ppsaPages = ::SafeArrayCreate (VT_BSTR, 1, &bound);
	ASSERT (*ppsaPages != NULL);
	if (*ppsaPages == NULL)
	{
		//FC_DEBUGPRINT ("STDEDITOR> CStdEditorDoc::XCEPrintObj::GetPages: failed to create pages SAFEARRAY\n");
		return (E_FAIL);
	}

	for (lIndex = 0; lIndex < (long)bound.cElements; ++lIndex)
	{
		CString strPage;

		strPage = astrPages.GetAt (lIndex);

		hr = ::SafeArrayPutElement (*ppsaPages, &lIndex, (void *)(BSTR)CComBSTR (strPage));
		if (FAILED (hr))
		{
			//FC_DEBUGPRINT1 ("STDEDITOR> CStdEditorDoc::XCEPrintObj::GetPages: failed to put page SAFEARRAY element (hr=0x%08lx)\n", hr);
			::SafeArrayDestroy (*ppsaPages);
			*ppsaPages = NULL;
			return (hr);
		}
	}

	return (S_OK);
}


STDMETHODIMP CStdEditorDoc::XCEPrintObj::PRGetXMLString(/*[out, string]*/ BSTR* psXMLString)
{
    return E_NOTIMPL;
}
