//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOClt                              |
//                                                                            |
//  Filename    : SOCltServerBrowser.h                                        |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : OPCENUM access classes                                      |
//                - SOCltServerBrowser: object for browsing the installed     |
//                  servers on a machine                                      |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOCLTSERVERBROWSER_H_
#define _SOCLTSERVERBROWSER_H_

#include "SOCltElement.h"

#pragma pack(push,4)

//-----------------------------------------------------------------------------
// SOCltServerBrowser                                                         |
//-----------------------------------------------------------------------------

// server browsing whatServers
#define SOCLT_BROWSE_SERVER_REG_DA10A	0x01
#define SOCLT_BROWSE_SERVER_CAT_DA10A	0x02
#define SOCLT_BROWSE_SERVER_CAT_DA20	0x04
#define SOCLT_BROWSE_SERVER_CAT_AE10	0x08

#define SOCLT_BROWSE_SERVER_CAT_ALL	( SOCLT_BROWSE_SERVER_CAT_DA10A | SOCLT_BROWSE_SERVER_CAT_DA20 | SOCLT_BROWSE_SERVER_CAT_AE10)
#define SOCLT_BROWSE_SERVER_DA10A		( SOCLT_BROWSE_SERVER_REG_DA10A | SOCLT_BROWSE_SERVER_CAT_DA10A )
#define SOCLT_BROWSE_SERVER_DA20		( SOCLT_BROWSE_SERVER_CAT_DA20 )
#define SOCLT_BROWSE_SERVER_AE10		( SOCLT_BROWSE_SERVER_CAT_AE10 )
#define SOCLT_BROWSE_SERVER_ALL		  ( SOCLT_BROWSE_SERVER_CAT_ALL | SOCLT_BROWSE_SERVER_REG_DA10A )	

class SOCLT_EXPORT SOCltServerBrowser : 
	public SOCmnObject,
	virtual public SOCltElementErrorStore
{
public:
	SOCltServerBrowser(void);

	void setNodeName(LPCTSTR nodeName);	// set node name of server browser object
	SOCmnString getNodeName(void);			// get node name of server brower object

	// server browsing
	BOOL browseServer(IN DWORD whatServers, OUT SOCmnList<CLSID> &classIdArray);

	// details of server
	BOOL getServerDescription(IN REFCLSID clsid, OUT SOCmnString &descr);
	BOOL getServerProgId(IN REFCLSID clsid, OUT SOCmnString &progId);

protected:

	virtual ~SOCltServerBrowser(void);

	CComPtr<IOPCServerList> m_serverBrowser;	// IOPCServerList interface of the browse node
	SOCmnString m_serverBrowserNode;					// node name of server brower object

	BOOL createServerBrowser();				// creates server browser object
	void releaseServerBrowser(void);	// release server browser object

	BOOL includesCLSID(IN REFCLSID clsid, IN SOCmnList<CLSID> *classIdList);

	virtual SOCmnObject *getObjectCltElementIError(void);
}; // SOCltServerBrowser

inline void SOCltServerBrowser::releaseServerBrowser(void)
{ m_serverBrowser.Release(); }

inline SOCmnString SOCltServerBrowser::getNodeName(void)
{ return m_serverBrowserNode; }

inline void SOCltServerBrowser::setNodeName(LPCTSTR nodeName)
{ 
	m_serverBrowserNode = nodeName; 
	releaseServerBrowser();
}


#pragma pack(pop)
#endif 
