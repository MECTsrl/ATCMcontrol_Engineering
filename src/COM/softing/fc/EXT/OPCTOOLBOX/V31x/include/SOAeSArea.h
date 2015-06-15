//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOAeS                              |
//                                                                            |
//  Filename    : SOAeSArea.h                                                 |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Area class                                                  |
//                - SOAeSArea: Area class                                     |
//                - SOAeSAreaDefault: Default implementation of area          |
//                - SOAeSAreaSpaceRoot: Root of area space                    |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOAESAREA_H_
#define _SOAESAREA_H_

#pragma pack(push,4)


//-----------------------------------------------------------------------------
// SOAeSArea                                                                  |
//-----------------------------------------------------------------------------

class SOAES_EXPORT SOAeSArea : 
	public SOCmnElement,
	virtual public SOCmnElementNameString
{
public:
	SOAeSArea(void);
	SOAeSArea(IN LPCTSTR areaName);

	virtual BOOL isUniqueName(void) const;
	virtual BOOL addLeaf(IN SOCmnElement *newLeaf);
	virtual BOOL addBranch(IN SOCmnElement *newBranch);

	// objects watch data
	virtual void getObjWatchData(IN void *pXMLDocumentInterface, IN void *pXMLParentNodeInterface, 
			IN BOOL withAttributes,	IN LPWSTR objNodeName, OPTIONAL OUT void **pXMLObjectNodeInterface = NULL);

	// device state and state info for watch
	virtual SOCmnString getDeviceState(void);
	virtual SOCmnString getDeviceStateInfo(void);

protected:
	virtual ~SOAeSArea(void);

	// have to check the uniqueness of the children?
	virtual BOOL trustUniqueChilds(void);

	virtual SOCmnObject *getObjectCmnElementIName(void);
}; // SOAeSArea




//-----------------------------------------------------------------------------
// SOAeSAreaDefault                                                           |
//-----------------------------------------------------------------------------

class SOAES_EXPORT SOAeSAreaDefault : 
	public SOAeSArea,
	virtual public SOCmnElementListLeafBranch
{
public:
	SOAeSAreaDefault();
	SOAeSAreaDefault(IN LPCTSTR areaName);
protected:
	virtual SOCmnObject *getObjectCmnElementIList(void);
}; // SOAeSAreaDefault




//-----------------------------------------------------------------------------
// SOAeSAreaSpaceRoot                                                         |
//-----------------------------------------------------------------------------

class SOAES_EXPORT SOAeSAreaSpaceRoot : 
	public SOCmnElement,
	virtual public SOCmnElementListLeafBranch
{
public:
	SOAeSAreaSpaceRoot(); 

	virtual BOOL addLeaf(IN SOCmnElement *newLeaf);
	virtual BOOL addBranch(IN SOCmnElement *newBranch);

	// tree hierarchy level delimiter and not allowed chars
	void setTreeDelimiter(IN TCHAR create, IN LPCTSTR accept, 
			IN LPCTSTR bad);
	virtual TCHAR getTreeDelimiter(OUT LPCTSTR *accept, OUT LPCTSTR *bad);

	void setTrustUniqueChilds(IN BOOL trust);

	// objects watch data
	virtual void getObjWatchData(IN void *pXMLDocumentInterface, IN void *pXMLParentNodeInterface, 
			IN BOOL withAttributes,	IN LPWSTR objNodeName, OPTIONAL OUT void **pXMLObjectNodeInterface = NULL);

	static void getConditions(IN SOCmnElement *parent, OUT SOCmnList<SOAeSCondition> &condList);
protected:
	// tree delimiters
	TCHAR m_delimAScreate;				// creation tree delimiter
	SOCmnString m_delimASaccept;		// accepted tree delimiter
	SOCmnString m_badASchars;			// bad tree characters
	BYTE m_trustUniqueChilds;			// trust unique childs

	// have to check the uniqueness of the children?
	virtual BOOL trustUniqueChilds(void);

	virtual SOCmnObject *getObjectCmnElementIList(void);
}; // SOAeSAreaSpaceRoot

inline void SOAeSAreaSpaceRoot::setTreeDelimiter(IN TCHAR create, 
	IN LPCTSTR accept, IN LPCTSTR bad)
{ m_delimAScreate = create; m_delimASaccept = accept;m_badASchars = bad; }

inline void SOAeSAreaSpaceRoot::setTrustUniqueChilds(IN BOOL trust)
{ m_trustUniqueChilds = trust; }

#pragma pack(pop)
#endif
