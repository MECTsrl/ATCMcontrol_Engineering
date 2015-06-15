//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SODaC                              |
//                                                                            |
//  Filename    : SODaCServer.h                                               |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Server object related classes                               |
//                - SODaCServer: proxy for OPC server object                  |
//                - SODaCServerDefault: default server class for creator      |
//                - SODaCPropertyData: item property data                     |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SODACSERVER_H_
#define _SODACSERVER_H_

#include "SOCltElement.h"
#include "SODaCEntry.h"


#pragma pack(push,4)

//-----------------------------------------------------------------------------
// SODaCPropertyData                                                          |
//-----------------------------------------------------------------------------

class SODAC_EXPORT SODaCPropertyData : public SOCmnData
{
public:
	SODaCPropertyData() 
	{
		m_propertyID = 0;
		m_datatype = VT_EMPTY;
		m_error = S_OK;
	}

	virtual ~SODaCPropertyData()	{}

	DWORD m_propertyID;					// property id
	SOCmnString m_description;	// description
	VARTYPE m_datatype;					// data type
	SOCmnVariant m_value;		    // property value
	SOCmnString m_itemID;		    // item ID of property
	HRESULT	m_error;						// error
};


//-----------------------------------------------------------------------------
// SODaCServer                                                                |
//-----------------------------------------------------------------------------

// server errors

// server property get what parameters
#define SODAC_PROPDATA_VALUE	0x0001
#define SODAC_PROPDATA_ITEMID	0x0002
#define SODAC_PROPDATA_ALL		0xFFFF

// supported opc version
#define SODAC_VERSION_OPC_DAV10A	0x0101
#define SODAC_VERSION_OPC_DAV20		0x0200

class SODaCGroup;
class SOCltShutdown;
class SODaCNameSpaceBrowser;

class SODAC_EXPORT SODaCServer : 
  public SOCltServer, 
  virtual public SOCmnElementListBranch
{
friend SODaCGroup;

public:
	SODaCServer(IN OPTIONAL WORD groupListType = SOCMNLIST_TYPE_ORG_LIST);  

	// OPC server version
	WORD getSupportedOPCVersion(void);
	
	// query COM interface
	virtual HRESULT queryInterface(IN REFIID riid, IN void **interf);

	// namespace browsing
	SODaCNameSpaceBrowser* getNameSpaceBrowser();

	// group creation
	SODaCGroup *addGroup(IN LPCTSTR groupName = NULL,	IN DWORD reqUpdateRate = 1000,	
		IN OPTIONAL FLOAT *deadband = NULL, IN OPTIONAL LONG *timeBias = NULL, 
		IN OPTIONAL DWORD LCID = 0);
	SODaCGroup *getPublicGroupByName(IN LPCTSTR groupName);

	// IOPCServer interface
	HRESULT getErrorString(IN HRESULT error, IN LCID locale, OUT SOCmnString *string);
	HRESULT getStatus(OUT OPCSERVERSTATUS *serverStatus, OUT SOCmnString *vendorInfo);

	// IOPCBrowseServerAddressSpace interface
	HRESULT queryOrganization(OUT OPCNAMESPACETYPE *nameSpaceType);
	HRESULT changeBrowsePosition(IN OPCBROWSEDIRECTION direction, 
		IN OPTIONAL LPCTSTR node = NULL);
	HRESULT browseOPCItemIDs(IN OPCBROWSETYPE browseType, 
		IN LPCTSTR filterCriteria, IN VARTYPE dataTypeFilter, 
		IN DWORD accessRightsFilter, OUT SOCmnStringList &itemIDs);
	SOCmnString getItemID(IN LPCTSTR itemID, OUT OPTIONAL HRESULT *result = NULL);
	HRESULT browseAccessPaths(IN LPCTSTR itemID, OUT SOCmnStringList &accessPaths);

	// IOPCItemProperties interface
	HRESULT queryAvailableProperties(IN LPCTSTR itemID, 
		OUT SOCmnList<SODaCPropertyData> &propteryList);
	HRESULT getPropertiesData(IN LPCTSTR itemID, IN WORD whatData,
		IN OUT SOCmnList<SODaCPropertyData> &propteryList);

	//	IPersistFile
	BOOL isDirty(void);
	HRESULT load(IN LPCTSTR fileName, IN DWORD mode);
	HRESULT save(IN LPCTSTR fileName, IN BOOL remember);
	HRESULT saveCompleted(IN LPCTSTR fileName);
	HRESULT getCurFile(OUT SOCmnString *fileName);

	// tree management
	virtual BOOL addLeaf(IN SOCmnElement *newLeaf);
	virtual BOOL addBranch(IN SOCmnElement *newBranch);

	// serialization
	virtual BOOL serialize(IN OUT SOCltStorage& ar, IN BOOL isStoring);

	// get child object by name
	inline SODaCGroup *getGroup(IN LPCTSTR groupName) { return (SODaCGroup *)findBranch(groupName); };

protected:
	WORD m_version;					     // supported OPC version of the server

  // interfaces of the server object
	SOCltInterfaceAccess<IOPCServer, &_ATL_IIDOF(IOPCServer)> m_server;
	SOCltInterfaceAccess<IOPCServerPublicGroups, &_ATL_IIDOF(IOPCServerPublicGroups)> m_serverPub;	
	SOCltInterfaceAccess<IOPCBrowseServerAddressSpace, &_ATL_IIDOF(IOPCBrowseServerAddressSpace)> m_browse;	
	SOCltInterfaceAccess<IOPCItemProperties, &_ATL_IIDOF(IOPCItemProperties)> m_properties;
	SOCltInterfaceAccess<IPersistFile, &IID_IPersistFile> m_persistFile;	

	// do the state transitions
	virtual HRESULT doStateTransition(BOOL deep);
	virtual HRESULT doUpdate(IN BOOL fromServer, IN OPTIONAL DWORD whatParameters = SOCLT_SRVPAR_ALL);
	virtual HRESULT doDisconnect(void);
 
protected:
  virtual ~SODaCServer();
}; // SODaCServer


inline WORD SODaCServer::getSupportedOPCVersion(void)
{ return m_version; }



//-----------------------------------------------------------------------------
// SODaCServerDefault                                                         |
//-----------------------------------------------------------------------------

class SODAC_EXPORT SODaCServerDefault :  
	public SODaCServer,
	virtual public SOCltElementErrorStore
{
public:
	SODaCServerDefault(IN OPTIONAL WORD groupListType = SOCMNLIST_TYPE_ORG_LIST);  

protected:
	virtual SOCmnObject *getObjectCltElementIError(void);
};

#define OPCItemPropertyData SODaCPropertyData 
#define ClientServer SODaCServerDefault

#define SODAC_SRVPAR_CLIENT_NAME  SOCLT_SRVPAR_CLIENT_NAME	        
#define SODAC_SRVPAR_LCID         SOCLT_SRVPAR_LCID			            
#define SODAC_SRVPAR_ALL          SOCLT_SRVPAR_ALL			            

#pragma pack(pop)
#endif 
