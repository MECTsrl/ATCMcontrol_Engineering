//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SODaC                              |
//                                                                            |
//  Filename    : SODaCBrowse.h                                               |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : DA Browsing objects                                         |
//                - SODaCNameSpaceBrowser: name space browsing root object    |
//                - SODaCNode: node browser object class                      |
//                - SODaCTag: tag browser object class                        |
//                - SODaCProperty: property browser object class              |
//                                                                            |
//-----------------------------------------------------------------------------


#ifndef _SODACBROWSE_H_
#define _SODACBROWSE_H_

#pragma pack(push,4)

class SODaCServer;


//-----------------------------------------------------------------------------
// SODaCNameSpaceBrowser                                                      |
//-----------------------------------------------------------------------------

class SODAC_EXPORT SODaCNameSpaceBrowser : public SOCltBrowseObject
{
protected:
	SODaCNameSpaceBrowser();
	virtual ~SODaCNameSpaceBrowser();
public:

// Overrides
	// SOCMNOBJECT_TYPE_NODE or SOCMNOBJECT_TYPE_TAG
	virtual BOOL expand(IN DWORD type, OUT SOCmnList<SOCltBrowseObject> &objList);
	virtual SOCmnString getDisplayName();
	
	SOCmnString m_filterCriteria;
	VARTYPE m_typeFilter;					// VT_* type 
	DWORD m_accessRights;					// any combination of OPC_READABLE and OPC_WRITEABLE

	SODaCServer* getServer(void);
  CComPtr<IOPCBrowseServerAddressSpace> m_browser;

	BOOL expandHierarchial(IN DWORD type, OUT SOCmnList<SOCltBrowseObject> &objList);
	BOOL expandFlat(IN DWORD type, OUT SOCmnList<SOCltBrowseObject> &objList);

	friend class SODaCServer;	// creates the instance
};

inline SODaCServer* SODaCNameSpaceBrowser::getServer(void)
{
	_ASSERTION( !!m_server, "No server object");
	m_server.addRef();
	return (SODaCServer*)(SOCltServer*)m_server;
}




//-----------------------------------------------------------------------------
// SODaCNode                                                                  |
//-----------------------------------------------------------------------------

class SODAC_EXPORT SODaCNode : public SOCltBrowseObject  
{
public:
	SODaCNode(IN SODaCNameSpaceBrowser* parent, LPCWSTR wszName);
	SODaCNode(IN SODaCNode* parent, LPCWSTR wszName);

	virtual ~SODaCNode();

// Overrides
	// SOCMNOBJECT_TYPE_NODE or SOCMNOBJECT_TYPE_TAG
	virtual BOOL expand(IN DWORD type, OUT SOCmnList<SOCltBrowseObject> &objList);
	virtual SOCmnString getPath();

	SOCmnString m_filterCriteria;
	VARTYPE m_typeFilter;					// VT_* type 
	DWORD m_accessRights;					// any combination of OPC_READABLE and OPC_WRITEABLE

  CComPtr<IOPCBrowseServerAddressSpace> m_browser;
	SOCmnStringList m_path;

	friend class SODaCNameSpaceBrowser;
};




//-----------------------------------------------------------------------------
// SODaCTag                                                                   |
//-----------------------------------------------------------------------------

class SODAC_EXPORT SODaCTag : public SOCltBrowseObject  
{
public:

	SODaCTag(IN SODaCNameSpaceBrowser* parent, LPCWSTR szName);
	SODaCTag(IN SODaCNode* parent, LPCWSTR szName);

	SODaCTag();
	virtual ~SODaCTag();

// Overrides
	virtual BOOL expand(IN DWORD type, OUT SOCmnList<SOCltBrowseObject> &objList);
	virtual SOCmnString getPath();

  CComPtr<IOPCBrowseServerAddressSpace> m_browser;
	SOCmnStringList m_path;
};




//-----------------------------------------------------------------------------
// SODaCProperty                                                              |
//-----------------------------------------------------------------------------

class SODAC_EXPORT SODaCProperty : public SOCltBrowseObject
{
public:
	SODaCProperty(IN SODaCNode* parent, LPCWSTR szName, LPCTSTR szParentID);
	SODaCProperty(IN SODaCTag* parent, LPCWSTR szName, LPCTSTR szParentID);
	virtual ~SODaCProperty();

// Overrides
	virtual BOOL expand(IN DWORD type, OUT SOCmnList<SOCltBrowseObject> &objList);
	virtual SOCmnString getPath();

	DWORD m_id;
	VARTYPE m_dataType;
	SOCmnString m_parentID;
};


#pragma pack(pop)

#endif // _SODACBROWSE_H_
