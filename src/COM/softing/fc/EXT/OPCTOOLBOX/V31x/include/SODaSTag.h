//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SODaS                              |
//                                                                            |
//  Filename    : SODaSTag.h                                                  |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Namespace Tag classes                                       |
//                - SODaSTag: base class for namespace tag objects            |
//                - SODaSTagDefault: default configuration for tag objects    |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SODASTAG_H_
#define _SODASTAG_H_

#include <opcda.h>
#include "SOCmnElement.h"
#include "SODaSItemTag.h"
#include "SODaSProperty.h"

#pragma pack(push,4)


//-----------------------------------------------------------------------------
// SODaSTag                                                                   |
//-----------------------------------------------------------------------------

class SODAS_EXPORT SODaSTag : 
	public SODaSItemTag,
	virtual public SOCmnElementNameString
{
public:
	SODaSTag(); 
	
	virtual BOOL isUniqueName(void) const;

	// watch overload 
	virtual BOOL setParentBranch(IN SOCmnElement *parent);

	// tag properties
	virtual SODaSProperty *addProperty(IN DWORD propertyId, OPTIONAL IN LPCTSTR propName = NULL);
	virtual BOOL addProperty(IN SODaSProperty *prop);

	virtual BOOL handlePropertyRequest(IN SODaSProperty *prop, 
			IN SODaSRequest *request);

	// engineering units information and type
	virtual BOOL getEUInfo(OUT LPVARIANT info);
	virtual OPCEUTYPE getEUType(void);
	virtual BOOL setEUAnalog(IN double low, IN double high);
	virtual BOOL setEUEnumerated(IN SOCmnStringList *enumList, IN OPTIONAL LONG startIndex = 0);
	virtual DWORD getEUEnumeratedPropertyId(void);

	// I/O mode
	virtual BYTE getDeviceIOMode(IN SODaSRequest *req);

	// special release method for dynamic created tags
	virtual LONG release(void);	

	// objects watch data
	virtual void getObjWatchData(IN void *pXMLDocumentInterface, IN void *pXMLParentNodeInterface, 
			IN BOOL withAttributes,	IN LPWSTR objNodeName, OPTIONAL OUT void **pXMLObjectNodeInterface = NULL);

protected:
	virtual ~SODaSTag(); 

	virtual SOCmnObject *getObjectCmnElementIName(void);
}; // SODaSTag

#define GenericTag SODaSTagDefault




//-----------------------------------------------------------------------------
// SODaSTagDefault                                                            |
//-----------------------------------------------------------------------------

class SODAS_EXPORT SODaSTagDefault : 
	public SODaSTag,
	virtual public SOCmnElementListAdditional,
	virtual public SODaSItemTagConnectQueueStore
{
public:
	SODaSTagDefault(void);
	SODaSTagDefault(IN LPCTSTR tagName);

protected:
	virtual SOCmnObject *getObjectCmnElementIList(void);
}; // SODaSTagDefault

#pragma pack(pop)
#endif
