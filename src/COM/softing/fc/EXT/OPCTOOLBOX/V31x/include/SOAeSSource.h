//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOAeS                              |
//                                                                            |
//  Filename    : SOAeSSource.h                                               |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Source class                                                |
//                - SOAeSSource: Source class                                 |
//                - SOAeSSourceDefault: Default implementation of source      |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOAESSOURCE_H_
#define _SOAESSOURCE_H_

#pragma pack(push,4)


//-----------------------------------------------------------------------------
// SOAeSSource                                                                |
//-----------------------------------------------------------------------------

class SOAES_EXPORT SOAeSSource : 
	public SOCmnElement,
	virtual public SOCmnElementNameString
{
public:
	SOAeSSource(void);
	SOAeSSource(IN LPCTSTR sourceName);

	virtual BOOL addAdditionalElement(IN SOCmnElement *newEl);
	virtual BOOL isUniqueName(void) const;

	// objects watch data
	virtual void getObjWatchData(IN void *pXMLDocumentInterface, IN void *pXMLParentNodeInterface, 
			IN BOOL withAttributes,	IN LPWSTR objNodeName, OPTIONAL OUT void **pXMLObjectNodeInterface = NULL);

	// device state and state info for watch
	virtual SOCmnString getDeviceState(void);
	virtual SOCmnString getDeviceStateInfo(void);

protected:
	virtual ~SOAeSSource(void);

	virtual SOCmnObject *getObjectCmnElementIName(void);
}; // SOAeSSource




//-----------------------------------------------------------------------------
// SOAeSSourceDefault                                                         |
//-----------------------------------------------------------------------------

class SOAES_EXPORT SOAeSSourceDefault : 
	public SOAeSSource,
	virtual public SOCmnElementListAdditional
{
public:
	SOAeSSourceDefault();
	SOAeSSourceDefault(IN LPCTSTR sourceName);
protected:
	virtual SOCmnObject *getObjectCmnElementIList(void);
}; // SOAeSSourceDefault

#pragma pack(pop)
#endif
