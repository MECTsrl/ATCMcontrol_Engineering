//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOAeC                              |
//                                                                            |
//  Filename    : SOAeCBrowse.h                                               |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : AE Browsing objects                                         |
//                - SOAeCAreaBrowser: area browser root object                |
//                - SOAeCArea: area object browse class                       |
//                - SOAeCSource: source object browse class                   |
//                - SOAeCCondition: condition object browse class             |
//                - SOAeCEventBrowser: event browser root object              |
//                - SOAeCCategory: category object browse class               |
//                - SOAeCAttribute: attribute object browse class             |
//                - SOAeCConditionClass: condition class object browse class  |
//                - SOAeCSubConditionClass: sub condition class browse class  |
//                                                                            |
//-----------------------------------------------------------------------------


#ifndef _SOAECBROWSE_H_
#define _SOAECBROWSE_H_

#pragma pack(push,4)

class SOAeCServer;

//-----------------------------------------------------------------------------
// SOAeCAreaBrowser                                                           |
//-----------------------------------------------------------------------------

class SOAEC_EXPORT SOAeCAreaBrowser : public SOCltBrowseObject
{
protected:
	SOAeCAreaBrowser();
	virtual ~SOAeCAreaBrowser();
public:

// Overrides
	// SOCMNOBJECT_TYPE_AREA or SOCMNOBJECT_TYPE_SOURCE
	virtual BOOL expand(IN DWORD type, OUT SOCmnList<SOCltBrowseObject> &objList);
	virtual SOCmnString getDisplayName();
	
	SOCmnString m_filterCriteria;
	
	SOAeCServer* getServer(void);
  CComPtr<IOPCEventAreaBrowser> m_browser;

	friend class SOAeCServer;	// creates the instance
};

inline SOAeCServer* SOAeCAreaBrowser::getServer(void)
{
	_ASSERTION( !!m_server, "No server object");
	return (SOAeCServer*)(SOCltServer*)m_server;
}




//-----------------------------------------------------------------------------
// SOAeCArea                                                                  |
//-----------------------------------------------------------------------------

class SOAEC_EXPORT SOAeCArea : public SOCltBrowseObject  
{
public:
	SOAeCArea(SOAeCAreaBrowser* parent, LPCWSTR wszName);
	SOAeCArea(SOAeCArea* parent, LPCWSTR wszName);
	virtual ~SOAeCArea();

// Overrides
	// SOCMNOBJECT_TYPE_AREA or SOCMNOBJECT_TYPE_SOURCE
	virtual BOOL expand(IN DWORD type, OUT SOCmnList<SOCltBrowseObject> &objList);
	virtual SOCmnString getPath();

	SOCmnString m_filterCriteria;

  CComPtr<IOPCEventAreaBrowser> m_browser;
	SOCmnStringList m_path;
};




//-----------------------------------------------------------------------------
// SOAeCSource                                                                |
//-----------------------------------------------------------------------------

class SOAEC_EXPORT SOAeCSource : public SOCltBrowseObject  
{
public:

	SOAeCSource(SOAeCAreaBrowser* parent, LPCWSTR wszName);
	SOAeCSource(SOAeCArea* parent, LPCWSTR wszName);

	virtual ~SOAeCSource();

// Overrides
	virtual BOOL expand(IN DWORD type, OUT SOCmnList<SOCltBrowseObject> &objList);
	virtual SOCmnString getPath();

  CComPtr<IOPCEventAreaBrowser> m_browser;
	SOCmnStringList m_path;
};




//-----------------------------------------------------------------------------
// SOAeCCondition                                                             |
//-----------------------------------------------------------------------------

class SOAEC_EXPORT SOAeCCondition : public SOCltBrowseObject
{
public:
	SOAeCCondition(SOAeCSource* parent, LPCWSTR wszName);
	virtual ~SOAeCCondition();

// Overrides
	virtual BOOL expand(IN DWORD type, OUT SOCmnList<SOCltBrowseObject> &objList);
};




//-----------------------------------------------------------------------------
// SOAeCEventBrowser                                                          |
//-----------------------------------------------------------------------------

class SOAEC_EXPORT SOAeCEventBrowser : public SOCltBrowseObject
{
protected:
	SOAeCEventBrowser();
	virtual ~SOAeCEventBrowser();
public:

// Overrides
	// SOCMNOBJECT_TYPE_CATEGORY
	virtual BOOL expand(IN DWORD type, OUT SOCmnList<SOCltBrowseObject> &objList);
	virtual SOCmnString getDisplayName();

	DWORD m_filterCategories;	// any combination of OPC_*_EVENT flags
	
	SOAeCServer* getServer(void);
  CComPtr<IOPCEventServer> m_browser;

	friend class SOAeCServer;	// creates the instance
};

inline SOAeCServer* SOAeCEventBrowser::getServer(void)
{
	_ASSERTION( !!m_server, "No server object");
	return (SOAeCServer*)(SOCltServer*)m_server;
}




//-----------------------------------------------------------------------------
// SOAeCCategory                                                              |
//-----------------------------------------------------------------------------

class SOAEC_EXPORT SOAeCCategory : public SOCltBrowseObject
{
public:
	SOAeCCategory(SOAeCEventBrowser* parent, LPCWSTR szName);
	virtual ~SOAeCCategory();

// Overrides
	virtual BOOL expand(IN DWORD type, OUT SOCmnList<SOCltBrowseObject> &objList);

	DWORD m_id;									// Category ID
	DWORD m_eventType;
};




//-----------------------------------------------------------------------------
// SOAeCAttribute                                                             |
//-----------------------------------------------------------------------------

class SOAEC_EXPORT SOAeCAttribute : public SOCltBrowseObject
{
public:
	SOAeCAttribute(SOAeCCategory* parent, LPCWSTR wszName);
	virtual ~SOAeCAttribute();

// Overrides
	virtual BOOL expand(IN DWORD Attribute, OUT SOCmnList<SOCltBrowseObject> &objList);

	DWORD m_id;									// Attribute ID
	VARTYPE m_type;							// Attribute type
};




//-----------------------------------------------------------------------------
// SOAeCConditionClass                                                        |
//-----------------------------------------------------------------------------

class SOAEC_EXPORT SOAeCConditionClass : public SOCltBrowseObject
{
public:
	SOAeCConditionClass(SOAeCCategory* parent, LPCWSTR wszName);
	virtual ~SOAeCConditionClass();

// Overrides
	virtual BOOL expand(IN DWORD type, OUT SOCmnList<SOCltBrowseObject> &objList);
};




//-----------------------------------------------------------------------------
// SOAeCSubConditionClass                                                     |
//-----------------------------------------------------------------------------

class SOAEC_EXPORT SOAeCSubConditionClass : public SOCltBrowseObject
{
public:
	SOAeCSubConditionClass(SOAeCConditionClass* parent, LPCWSTR szName);
	virtual ~SOAeCSubConditionClass();

// Overrides
	virtual BOOL expand(IN DWORD type, OUT SOCmnList<SOCltBrowseObject> &objList);
};


#pragma pack(pop)

#endif // _SOAECBROWSE_H_
