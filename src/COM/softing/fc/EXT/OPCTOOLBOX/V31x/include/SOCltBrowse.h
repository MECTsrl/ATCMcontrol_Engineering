//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOClt                              |
//                                                                            |
//  Filename    : SOCltBrowse.h                                               |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Browse base class for Client Toolkits                       |
//                - SOCltBrowseObject: base class for browsing                |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOCLTBROWSE_H_
#define _SOCLTBROWSE_H_

#include "SOCltServer.h"

#pragma pack(push,4)


//-----------------------------------------------------------------------------
// SOCltBrowseObject                                                          |
//-----------------------------------------------------------------------------

class SOCLT_EXPORT SOCltBrowseObject : public SOCmnObject  
{
public:
	SOCltBrowseObject(IN DWORD objType);
	virtual ~SOCltBrowseObject();

	// Returns the children list
	virtual BOOL expand(IN DWORD type, OUT SOCmnList<SOCltBrowseObject> &objList) = 0;
	virtual SOCmnString getDisplayName(void);		// Returns string to be shown in the tree
	virtual SOCmnString getPath(void);					// Returns the fully qualified object's name
	virtual BOOL isExpandable(void);

	SOCmnPointer<SOCltServer> m_server;
	SOCmnString m_name;
};

#pragma pack(pop)

#endif // _SOCLTBROWSE_H_
