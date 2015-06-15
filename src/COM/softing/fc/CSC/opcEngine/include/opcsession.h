//-----------------------------------------------------------------------------
//                                 OPC SESSION                                |
//                                                                            |
//  Filename    : OPCSESSION.H                                                |
//  Version     : 2.01.0.00.release                                           |
//  Date        : 24-June-1999                                               |
//                                                                            |
//  Description : Entry point for Client Toolkit                              |
//				  - OPCSession: OPC session                                   |
//                - ClientCreator: Object factory for toolkit objects         |
//                                                                            |
//  CHANGE_NOTES                                                              |
//                                                                            |
//  date      name      change                                                |
//  ------------------------------------------------------------------------- |
//  02/01/99  RT        added creation data to parameters of creator methods  |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _OPCSESSION_H_
#define _OPCSESSION_H_

#include <opccomn.h>
#include <atlbase.h>
#include "GenTree.h"
#include "GenError.h"
#include "ClientConfig.h"

//-----------------------------------------------------------------------------
// Trace Groups                                                               |
//-----------------------------------------------------------------------------

#define OPCSESSION_TRACE_ALL			0xFFFFFFFF
#define OPCSESSION_TRACE_NOTHING		0x00000000

#define OPCSESSION_TRACE_REF_COUNTS		0x00000001
#define OPCSESSION_TRACE_IO				0x00000700
#define OPCSESSION_TRACE_LOG_OBJECTS	0x10000000

#define OPCSESSION_TRACE_GENERIC		0x00000001
#define OPCSESSION_TRACE_TREE			0x00000002
#define OPCSESSION_TRACE_SESSION		0x00020000
#define OPCSESSION_TRACE_SERVER			0x00000100
#define OPCSESSION_TRACE_GROUP			0x00000200
#define OPCSESSION_TRACE_ITEM			0x00000400
#define OPCSESSION_TRACE_OTHER			0x08000000
#define OPCSESSION_TRACE_LOG_GENOBJ		0x10000000

// object state flags for server, group and item objects
#define OPCSESSION_STATE_FLAG_CONNECTED		0x01
#define OPCSESSION_STATE_FLAG_STARTED		0x02

// common toolbox error codes
#define OPCSESSION_ERR_EXCEPTION					0xE0400000
#define OPCSESSION_ERR_COM_INIT						0xE0400001
#define OPCSESSION_ERR_COM_QI						0xE0400002
#define OPCSESSION_ERR_OUT_OF_MEMORY				0xE0400003
#define OPCSESSION_ERR_PAR_INPUT					0xE0400010
#define OPCSESSION_ERR_PAR_OUTPUT					0xE0400011
#define OPCSESSION_ERR_CONFIGURATION				0xE0400018
#define OPCSESSION_ERR_OPCDA_QI_REQ					0xE0400021
#define OPCSESSION_ERR_OPCDA_QI_REQ_V10				0xE0400022
#define OPCSESSION_ERR_OPCDA_QI_REQ_V20				0xE0400023
#define OPCSESSION_ERR_OPCDA_QI_OPT					0xE0400026
#define OPCSESSION_ERR_DEMO_TIME					0xE0400F00


//-----------------------------------------------------------------------------
// ClientCreator                                                              |
//-----------------------------------------------------------------------------

class ClientItem;
class ClientGroup;
class ClientServer;
class OPCSession;

class OPCENGINE_EXPORT ClientCreator : public GenericObject
{
public:
	ClientCreator();
	virtual ~ClientCreator();
	virtual ClientServer* createServer(IN OPCSession *parent, IN ClientConfigServer *config);
	virtual ClientServer* createServer(IN OPCSession *parent, IN ClientConfigServerData *data);
	virtual ClientGroup* createGroup(IN ClientServer *parent, IN ClientConfigGroup *config);
	virtual ClientGroup* createGroup(IN ClientServer *parent, IN ClientConfigGroupData *data);
	virtual ClientItem* createItem(IN ClientGroup *parent, IN ClientConfigItem *config);
	virtual ClientItem* createItem(IN ClientGroup *parent, IN ClientConfigItemData *data);
}; // ClientCreator




//-----------------------------------------------------------------------------
// OPCSession                                                                 |
//-----------------------------------------------------------------------------

class ClientCreator;
class ClientConfig;

class OPCENGINE_EXPORT OPCSession : 
	public GenericBranch,
	public GenericError
{
public:
	// object states
	enum sessionState
	{
		disconnected = 0,
		connected    = OPCSESSION_STATE_FLAG_CONNECTED,
		started      = OPCSESSION_STATE_FLAG_CONNECTED | OPCSESSION_STATE_FLAG_STARTED
	};

	OPCSession(void);
	virtual ~OPCSession(void);

	BOOL isConnected(void);				// is session connected?
	BOOL isStarted(void);				// is session started?

	BOOL initialize(IN DWORD coInit);	// initialize session
	BOOL terminate(void);				// terminate session
	DWORD getCoInit(void);				// COM init mode

	BOOL start(void);		// start communication with the OPC Servers					
	BOOL connect(void);		// connect to the OPC Servers
	BOOL disconnect(void);	// disconnect from the OPC Servers

	// server creation
	ClientServer *insertServer(IN ClientConfigServer *config);
	ClientServer *addServer(IN LPCTSTR serverName, IN LPCTSTR nodeName, 
			IN REFCLSID classID, IN DWORD clsctx = CLSCTX_ALL);

	// object factory
	void setCreator(IN ClientCreator* pCreator);
	ClientCreator *getCreator(void);

	// configuration object
	BOOL setConfiguration(IN ClientConfig *clientConfig);
	ClientConfig *getConfiguration(void);
	BOOL storeConfiguration(IN OPTIONAL ClientConfig *clientConfig = NULL);

	// tracing
	void setTraceLevel(IN DWORD error, IN DWORD warning, IN DWORD info, 
			IN DWORD debug);
	BOOL setTraceFile(IN LPCTSTR file, 
			IN OPTIONAL LPCTSTR secondFile = NULL, 
			IN OPTIONAL DWORD maxFileSize = 1000000,
			IN OPTIONAL DWORD creationDisposition = CREATE_ALWAYS);

	// tree hierarchy level delimiter
	void setTreeDelimiter(IN TCHAR treeDel);
	TCHAR getTreeDelimiter(void) const;

	virtual	LONG release(void);

	// tree management
	virtual BOOL addLeaf(IN GenericLeaf *newLeaf, IN OPTIONAL int index = -1);
	virtual BOOL addBranch(IN GenericBranch *newBranch, 
			IN OPTIONAL int index = -1);

protected:
	ClientCreator *m_creator;		// creator object
	ClientConfig *m_clientConfig;	// configuration object
	BOOL m_terminateLock;			// termination lock
	BOOL m_initialized;				// initialized flag
	DWORD m_coInit;					// COM init mode
	TCHAR m_delimTree;				// tree delimiter
}; // OPCSession

inline ClientCreator *OPCSession::getCreator()
{ if (m_creator) m_creator->addRef(); return (ClientCreator *)m_creator; }

inline ClientConfig *OPCSession::getConfiguration()
{ if (m_clientConfig) m_clientConfig->addRef(); return (ClientConfig *)m_clientConfig; }

inline DWORD OPCSession::getCoInit(void)
{ return m_coInit; }

inline BOOL OPCSession::isConnected(void)
{ return ((getObjectState() & connected) == connected); }

inline BOOL OPCSession::isStarted(void)
{ return ((getObjectState() & started) == started); }

inline void OPCSession::setTreeDelimiter(IN TCHAR treeDel)
{ m_delimTree = treeDel; }

inline TCHAR OPCSession::getTreeDelimiter(void) const
{ return m_delimTree; }

// get reference to OPC Session object
OPCENGINE_EXPORT OPCSession* getOPCSession(void);

#endif 
