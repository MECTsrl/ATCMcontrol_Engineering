//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOSrv                              |
//                                                                            |
//  Filename    : SOSrvWatch.h                                                |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : OPC Server monitoring classes                               |
//                - SOSrvWatch: watch object                                  |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOSRVWATCH_H_
#define _SOSRVWATCH_H_

#include "SOCmnObject.h"
#include "SOProxyDef.h"
#import "msxml.dll"

#pragma pack(push,4)

//-----------------------------------------------------------------------------
// SOSrvWatchHandler                                                          |
//-----------------------------------------------------------------------------

class SOSrvWatch;

class SOSRV_EXPORT SOSrvWatchHandler : public SOCmnObject
{
friend SOSrvWatch;
public:
	SOSrvWatchHandler(void);  

protected:
	virtual SOCmnObject *getObject(IN LPCTSTR objSpace, IN LPCTSTR objType);
	virtual void getRootObjects(OUT SOCmnList<SOCmnObject> objList);

	// get statistics
	virtual HRESULT getStatistics(IN MSXML::IXMLDOMDocumentPtr pDoc, IN MSXML::IXMLDOMNodePtr pRootNode);

	// DA only
	virtual BOOL hasCacheObject(IN SOCmnObject *tag, IN SOCmnObject *cache, OUT BOOL &uniqueCache);
	virtual BOOL setCollectCacheChanges(IN SOCmnObject *tag, IN BOOL doCollect);
	virtual SOCmnObject *getNameSpaceObjectByItemID(IN LPCTSTR itemID);
}; // SOSrvWatchHandler


//-----------------------------------------------------------------------------
// SOSrvWatchData                                                             |
//-----------------------------------------------------------------------------

class SOSRV_EXPORT SOSrvWatchData : public SOCmnData
{
friend SOSrvWatch;
public:
	SOSrvWatchData(void);  
	virtual ~SOSrvWatchData(void);  

protected:
	SOCmnPointer<SOCmnObject> m_object;
	DWORD m_clientHandle;
	MSXML::IXMLDOMNodePtr m_changeNode;	
}; // SOSrvWatchData


//-----------------------------------------------------------------------------
// SOSrvWatch                                                                 |
//-----------------------------------------------------------------------------

class SOSRV_EXPORT SOSrvWatch : public SOCmnObject
{
public:
	SOSrvWatch(void);  

	virtual SOCmnObject *getObject(IN LPCTSTR object);
	virtual SOCmnObject *getObject(IN MSXML::IXMLDOMNodePtr objectsNode, OPTIONAL OUT DWORD *clientHandle = NULL);

	// interface implementations
	virtual HRESULT getChildren(IN LPCWSTR parent, IN BOOL withAttributes, OUT LPWSTR *children);
	virtual HRESULT collectChanges(IN LPCWSTR objects, IN DWORD doCollect);
	virtual HRESULT getChanges(OUT LPWSTR *changes);
	virtual HRESULT getStatistics(OUT LPWSTR *statistics);
	virtual HRESULT getObject(IN LPCWSTR objectDescription, OUT LPWSTR *object);

	SOSrvWatchHandler *getDAHandler(void) { m_DAHandler.addRef(); return m_DAHandler; } 
	void setDAHandler(IN SOSrvWatchHandler *handler) { m_DAHandler = handler; } 
	SOSrvWatchHandler *getAEHandler(void) { m_AEHandler.addRef(); return m_AEHandler; } 
	void setAEHandler(IN SOSrvWatchHandler *handler) { m_AEHandler = handler; } 
	SOSrvWatchHandler *getCustomHandler(void) { m_customHandler.addRef(); return m_customHandler; } 
	void setCustomHandler(IN SOSrvWatchHandler *handler) { m_customHandler = handler; } 

	SOCmnSync *getStatisticsLock(void) { return &m_getStatisticsLock; }

	static BOOL attributeChange(IN SOCmnObject *pObj, IN LPCTSTR attrName, IN LPCTSTR attrValue);
	virtual BOOL attributeChange(IN SOCmnObject *pObj, IN MSXML::IXMLDOMNodePtr pChange);
	virtual BOOL addObject(IN SOCmnObject *pParent, IN SOCmnObject *pObj);
	virtual BOOL removeObject(IN SOCmnObject *pParent, IN SOCmnObject *pObj);

	BOOL hasDocuments(void) { return ((m_getObjectDoc.GetInterfacePtr()) && (m_getObjDoc.GetInterfacePtr()) && (m_getChildrenDoc.GetInterfacePtr()) && (m_collectChangesDoc.GetInterfacePtr()) && (m_getChangesDoc.GetInterfacePtr()) && (m_getStatisticsDoc.GetInterfacePtr())); }

	static DWORD convertObjectType(LPCTSTR objectType);

	static BOOL createElementText(MSXML::IXMLDOMDocumentPtr pDOMDoc, 
				MSXML::IXMLDOMNodePtr pObjectNode, LPCTSTR element, LPCTSTR text, OPTIONAL OUT MSXML::IXMLDOMNode **pNodeInterface = NULL);
	static BOOL createElementText(MSXML::IXMLDOMDocumentPtr pDOMDoc, 
				MSXML::IXMLDOMNodePtr pObjectNode, LPCTSTR element, DWORD dw, OPTIONAL OUT MSXML::IXMLDOMNode **pNodeInterface = NULL);
	static BOOL getElementText(MSXML::IXMLDOMNodePtr pElement, SOCmnString &string);
	static BOOL getElementText(MSXML::IXMLDOMNodePtr pElement, DWORD &dw);	
	static LPWSTR getXML(MSXML::IXMLDOMNodePtr pNode);

	static MSXML::IXMLDOMDocumentPtr createDocument(void);
	static BOOL cloneRawAttribute(MSXML::IXMLDOMNodePtr pSrcElement, MSXML::IXMLDOMNodePtr pDestElement);	

	static MSXML::IXMLDOMNodePtr getAttributesNode(MSXML::IXMLDOMNodePtr pObjNode);	
	static BOOL getAttribute(LPWSTR attrName, SOCmnString &string, IN MSXML::IXMLDOMNodePtr pObjNode);

protected:
	SOCmnPointer<SOSrvWatchHandler> m_customHandler;
	SOCmnPointer<SOSrvWatchHandler> m_DAHandler;
	SOCmnPointer<SOSrvWatchHandler> m_AEHandler;
	SOCmnList<SOSrvWatchData> m_changesList;
	MSXML::IXMLDOMNodePtr m_changesNode;	
	MSXML::IXMLDOMDocumentPtr m_getObjDoc;
	MSXML::IXMLDOMDocumentPtr m_getChildrenDoc;
	MSXML::IXMLDOMDocumentPtr m_collectChangesDoc;
	MSXML::IXMLDOMDocumentPtr m_getChangesDoc;
	MSXML::IXMLDOMDocumentPtr m_getStatisticsDoc;
	MSXML::IXMLDOMDocumentPtr m_getObjectDoc;
	SOCmnSync m_getObjLock;
	SOCmnSync m_getChildrenLock;
	SOCmnSync m_collectChangesLock;
	SOCmnSync m_getChangesLock;
	SOCmnSync m_getStatisticsLock;
	SOCmnSync m_getObjectLock;
	FILETIME m_statTime;
}; // SOSrvWatch

#pragma pack(pop)
#endif
