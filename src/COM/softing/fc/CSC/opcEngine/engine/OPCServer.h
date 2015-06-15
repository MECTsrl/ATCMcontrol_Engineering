//-----------------------------------------------------------------------------
//                                  OPC ENGINE                                |
//                                                                            |
//  Filename    : OPCSERVER.H                                                 |   
//  Version     : 2.00.0.00.release                                           |
//  Date        : 29-March-1999                                               |
//                                                                            |
//  Description : OPCServer class                                             |
//                                                                            |
//  CHANGE_NOTES                                                              | 
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _OPCSERVER_H_
#define _OPCSERVER_H_

#include <opccomn.h>
#include "globals.h"
#include "OPCObj.h"
#include "OPCGroup.h"
#include "GenNS.h"

//-----------------------------------------------------------------------------
// OPCServer                                                                  |
//-----------------------------------------------------------------------------

class GenericServer;

class OPCServer : public IOPCCommon,
				  public IOPCServer,
				  public IOPCItemProperties,
				  public IOPCBrowseServerAddressSpace,
				  public IOPCServerPublicGroups,
				  public IPersistFile,
				  public IConnectionPointContainerImpl<OPCServer>,
				  public IConnectionPointImpl<OPCServer, &IID_IOPCShutdown, CComDynamicUnkArray>,
				  public CComObjectRoot,
				  public CComCoClass<OPCServer, &g_clsid>
{
public:
	OPCServer();
	~OPCServer();

	BEGIN_COM_MAP(OPCServer)
		COM_INTERFACE_ENTRY(IOPCCommon)
		COM_INTERFACE_ENTRY(IOPCServer)
		COM_INTERFACE_ENTRY(IOPCItemProperties)
		COM_INTERFACE_ENTRY(IOPCServerPublicGroups)
		COM_INTERFACE_ENTRY(IOPCBrowseServerAddressSpace)
		COM_INTERFACE_ENTRY(IPersistFile)
#if _MSC_VER >= 1200
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
#else
		COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
#endif
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(OPCServer)
		CONNECTION_POINT_ENTRY(IID_IOPCShutdown)
	END_CONNECTION_POINT_MAP()
	
	DECLARE_OPCOBJECT_NOT_AGGREGATABLE(OPCServer) 
	DECLARE_REGISTRY_RESOURCEID(g_registerId)

	// IOPCCommon interface
    STDMETHODIMP SetLocaleID(IN LCID dwLcid);
        
    STDMETHODIMP GetLocaleID(OUT LCID *pdwLcid);
        
    STDMETHODIMP QueryAvailableLocaleIDs(OUT DWORD *pdwCount, 
			OUT LCID **pdwLcid);
        
    STDMETHODIMP GetErrorString(IN HRESULT dwError, OUT LPWSTR *ppString);
        
    STDMETHODIMP SetClientName(IN LPCWSTR szName);

	// IOPCServer interface
	STDMETHODIMP AddGroup(IN LPCWSTR szName, IN BOOL bActive, 
			IN DWORD dwRequestedUpdateRate, IN OPCHANDLE hClientGroup,
			IN LONG *pTimeBias, IN FLOAT *pPercentDeadband, IN DWORD dwLCID,
			OUT OPCHANDLE *phServerGroup, OUT DWORD *pRevisedUpdateRate,
			IN REFIID riid, IN LPUNKNOWN * ppUnk);

	STDMETHODIMP GetErrorString(IN HRESULT dwError, IN LCID dwLocale,
			OUT LPWSTR *ppString);

	STDMETHODIMP GetGroupByName(IN LPCWSTR szName, IN REFIID riid,
			OUT LPUNKNOWN *ppUnk);

	STDMETHODIMP GetStatus(OUT OPCSERVERSTATUS **ppServerStatus);

	STDMETHODIMP RemoveGroup(IN OPCHANDLE hServerGroup, IN BOOL bForce);

	STDMETHODIMP CreateGroupEnumerator(IN OPCENUMSCOPE dwScope,
			IN REFIID riid, OUT LPUNKNOWN *ppUnk);

	// IOPCItemProperties interface
	STDMETHODIMP QueryAvailableProperties(IN LPWSTR szItemID, 
			OUT DWORD *pdwCount, OUT DWORD **ppPropertyIDs,
			OUT LPWSTR **ppDescriptions, OUT VARTYPE **ppvtDataTypes);
        
	STDMETHODIMP GetItemProperties(IN LPWSTR szItemID, IN DWORD dwCount,
			IN DWORD *pdwPropertyIDs, OUT VARIANT **ppvData,
			OUT HRESULT **ppErrors);
        
	STDMETHODIMP LookupItemIDs(IN LPWSTR szItemID, IN DWORD dwCount,
			IN DWORD *pdwPropertyIDs, OUT LPWSTR **ppszNewItemIDs,
			OUT HRESULT **ppErrors);

	// IOPCServerPublicGroups interface
	STDMETHODIMP GetPublicGroupByName(IN LPCWSTR szName, IN REFIID riid,
			OUT LPUNKNOWN *ppUnk);
        
	STDMETHODIMP RemovePublicGroup(IN OPCHANDLE hServerGroup, IN BOOL bForce);

	// IOPCBrowseServerAddressSpace interface
	STDMETHODIMP QueryOrganization(OUT OPCNAMESPACETYPE *pNameSpaceType);
        
	STDMETHODIMP ChangeBrowsePosition(IN OPCBROWSEDIRECTION dwBrowseDirection,
			IN LPCWSTR szString);
        
	STDMETHODIMP BrowseOPCItemIDs(IN OPCBROWSETYPE dwBrowseFilterType,
			IN LPCWSTR szFilterCriteria, IN VARTYPE vtDataTypeFilter,
			IN DWORD dwAccessRightsFilter, OUT LPENUMSTRING *ppIEnumString);
        
	STDMETHODIMP GetItemID(IN LPWSTR szItemDataID, OUT LPWSTR *szItemID);
        
	STDMETHODIMP BrowseAccessPaths(IN LPCWSTR szItemID, 
			OUT LPENUMSTRING *ppIEnumString);

	// IPersistFile interface
	STDMETHODIMP GetClassID(OUT CLSID *pClassID);

	STDMETHODIMP IsDirty(void);
        
	STDMETHODIMP Load(IN LPCOLESTR pszFileName, IN DWORD dwMode);
        
	STDMETHODIMP Save(IN LPCOLESTR pszFileName, IN BOOL fRemember);
        
	STDMETHODIMP SaveCompleted(IN LPCOLESTR pszFileName);
        
	STDMETHODIMP GetCurFile(OUT LPOLESTR *ppszFileName);

	// IConnectionPoint
	STDMETHODIMP Advise(IN IUnknown* unknown, OUT DWORD* cookie);

	STDMETHODIMP Unadvise(IN DWORD cookie);

	HRESULT getStandardProperties(IN VARTYPE tagNativeDatatype,
			OUT TagPropertyDataList &propertyList);

	HRESULT isTagProperty(IN LPCTSTR itemId, IN GenericTag *tag,
			IN DWORD propId);

	HRESULT getErrorString(IN HRESULT dwError, IN BOOL useLocale, 
			IN LCID dwLocale, OUT LPWSTR *ppString);

	HRESULT sendShutdownRequest(IN LPCTSTR reason);
	BOOL connectedShutdownInterface(void);
	// check if connection to V2.0 OPC client is OK
	BOOL checkClientConnection(void);

	// object trace id
	LPCTSTR getObjTraceId(void);

	HRESULT createOPCGroup(IN GenericGroup *grp, IN REFIID riid,
			OUT LPUNKNOWN * ppUnk, IN OPTIONAL BOOL doAddRef = TRUE);

	GenericServer *getGenSrv(void);	// get generic server object

	BOOL releaseMe(void);
	void initObject(void);

protected:
	CString m_objectTraceId;	// object trace id string 
	POSITION m_objectTracePos;	// object trace pointer
	GenericServer *m_me;		// generic server object
	CCriticalSection m_advisesSync;		// synchronisation for advises array

	void copyAppStringList(IN CStringList &appList, 
			OUT CStringList &engineList);
}; // OPCServer

inline GenericServer *OPCServer::getGenSrv(void)
{ return m_me; }

#endif
