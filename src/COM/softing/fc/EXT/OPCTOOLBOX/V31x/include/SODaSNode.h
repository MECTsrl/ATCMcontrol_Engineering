//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SODaS                              |
//                                                                            |
//  Filename    : SODaSNode.h                                                 |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Namespace branch classes                                    |
//                - SODaSNode: branch object in namespace tree                |
//                - SODaSNodeDefault: default configuration of node object    |
//                - SODaSNameSpaceRoot: root object of namespace tree         |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SODASNODE_H_
#define _SODASNODE_H_

#include <opcda.h>
#include "SOCmnElement.h"

#pragma pack(push,4)


//-----------------------------------------------------------------------------
// SODaSNode                                                                  |
//-----------------------------------------------------------------------------

class SODaSProperty;

class SODAS_EXPORT SODaSNode : 
	public SOCmnElement,
	virtual public SOCmnElementNameString
{
public:
	SODaSNode();

	virtual BOOL addLeaf(IN SOCmnElement *newLeaf);
	virtual BOOL addBranch(IN SOCmnElement *newBranch);
	// watch overload 
	virtual BOOL setParentBranch(IN SOCmnElement *parent);

	virtual BOOL isUniqueName(void) const;
	virtual BOOL trustUniqueChilds(void);

	// property handling
	virtual SODaSProperty *addProperty(IN DWORD propertyId, IN LPCTSTR propName);
	virtual BOOL addProperty(IN SODaSProperty *prop);

	virtual SOCmnObject *getObjectCmnElementIName(void);

	virtual BOOL handlePropertyRequest(IN SODaSProperty *prop, 
			IN SODaSRequest *request);

	// dynamic created item tag
	virtual BOOL setDynamic(IN BOOL dynamic);
	BOOL getDynamic(void) const;

	// special release method for dynamic created nodes
	virtual LONG release(void);	

	// objects watch data
	virtual void getObjWatchData(IN void *pXMLDocumentInterface, IN void *pXMLParentNodeInterface, 
			IN BOOL withAttributes,	IN LPWSTR objNodeName, OPTIONAL OUT void **pXMLObjectNodeInterface = NULL);

	// device state and state info for watch
	virtual SOCmnString getDeviceState(void);
	virtual SOCmnString getDeviceStateInfo(void);

protected:
	BYTE m_dynamic;
	
	virtual ~SODaSNode(void);
}; // SODaSNode

inline BOOL SODaSNode::getDynamic(void) const
{ return (BOOL)m_dynamic; }

#define GenericNode SODaSNodeDefault



//-----------------------------------------------------------------------------
// SODaSNodeDefault                                                           |
//-----------------------------------------------------------------------------

class SODAS_EXPORT SODaSNodeDefault : 
	public SODaSNode,
	virtual public SOCmnElementListLeafBranchAdditional
{
public:
	SODaSNodeDefault(void);
	SODaSNodeDefault(IN LPCTSTR nodeName);

protected:
	virtual SOCmnObject *getObjectCmnElementIList(void);
}; // SODaSNodeDefault




//-----------------------------------------------------------------------------
// SODaSNameSpaceRoot                                                         |
//-----------------------------------------------------------------------------

class SODaSTag;

class SODAS_EXPORT SODaSNameSpaceRoot : 
	public SOCmnElement,
	virtual public SOCmnElementListLeafBranch
{
public:
	SODaSNameSpaceRoot(); 

	virtual BOOL addLeaf(IN SOCmnElement *newLeaf);
	virtual BOOL addBranch(IN SOCmnElement *newBranch);

	// tree hierarchy level delimiter and not allowed chars
	void setTreeDelimiter(IN TCHAR create, IN LPCTSTR accept, 
			IN LPCTSTR bad);
	virtual TCHAR getTreeDelimiter(OUT LPCTSTR *accept, OUT LPCTSTR *bad);

	// use some tags the same cache object
	BOOL getUseTagLinks(void) const;
	BOOL setUseTagLinks(IN BOOL use);

	// get tag, node object of static namespace tree
	static SODaSTag *getTag(IN LPCTSTR fullName, IN OPTIONAL BYTE serverInstance = 0);
	static SODaSNode *getNode(IN LPCTSTR fullName, IN OPTIONAL BYTE serverInstance = 0);
	static SOCmnElement *getElement(IN LPCTSTR fullName, IN OPTIONAL BYTE serverInstance = 0);

	void setTrustUniqueChilds(IN BOOL trust);

	// objects watch data
	virtual void getObjWatchData(IN void *pXMLDocumentInterface, IN void *pXMLParentNodeInterface, 
			IN BOOL withAttributes,	IN LPWSTR objNodeName, OPTIONAL OUT void **pXMLObjectNodeInterface = NULL);

protected:
	BYTE m_useTagLinks;
	BYTE m_trustUniqueChilds;			// trust unique childs

	// tree delimiters
	TCHAR m_delimNScreate;				// creation tree delimiter
	SOCmnString m_delimNSaccept;		// accepted tree delimiter
	SOCmnString m_badNSchars;			// bad tree characters

	// have to check the uniqueness of the children?
	virtual BOOL trustUniqueChilds(void);

	virtual SOCmnObject *getObjectCmnElementIList(void);
}; // SODaSNameSpaceRoot

inline void SODaSNameSpaceRoot::setTreeDelimiter(IN TCHAR create, 
	IN LPCTSTR accept, IN LPCTSTR bad)
{ m_delimNScreate = create; m_delimNSaccept = accept;m_badNSchars = bad; }

inline BOOL SODaSNameSpaceRoot::getUseTagLinks(void) const
{ return m_useTagLinks; }

inline BOOL SODaSNameSpaceRoot::setUseTagLinks(IN BOOL use)
{ m_useTagLinks = use; return TRUE; }

inline void SODaSNameSpaceRoot::setTrustUniqueChilds(IN BOOL trust)
{ m_trustUniqueChilds = trust; }

#define OPCRootNS SODaSNameSpaceRoot

#pragma pack(pop)
#endif
