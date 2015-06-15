//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SODaC                              |
//                                                                            |
//  Filename    : SODaCEntry.h                                                |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Entry point for OPC Data Access Client Toolkit              |
//                - SODaCEntry: Entry point class                             |
//                - SODaCCreator: Object factory for toolkit objects          |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SODACENTRY_H_
#define _SODACENTRY_H_

#pragma pack(push,4)

// object state flags for server, group and item objects
#define SODAC_STATE_FLAG_CONNECTED		0x01
#define SODAC_STATE_FLAG_STARTED		0x02

//-----------------------------------------------------------------------------
// SODaCCreator                                                               |
//-----------------------------------------------------------------------------

class SODaCItem;
class SODaCGroup;
class SODaCServer;
class SODaCEntry;

class SODAC_EXPORT SODaCCreator : public SOCmnObject
{
public:
	SODaCCreator();
	virtual SODaCServer* createServer(IN SODaCEntry *parent);
	virtual SODaCGroup* createGroup(IN SODaCServer *parent);
	virtual SODaCItem* createItem(IN SODaCGroup *parent);
protected:
	virtual ~SODaCCreator();
}; // SODaCCreator

#define ClientCreator SODaCCreator




//-----------------------------------------------------------------------------
// SODaCEntry                                                                 |
//-----------------------------------------------------------------------------

class SODaCCreator;
class SODaCConfig;

class SODAC_EXPORT SODaCEntry : 
	public SOCltEntry,
	virtual public SOCmnElementListBranch,
	virtual public SOCltElementErrorStore
{
public:
	SODaCEntry(void);
	virtual ~SODaCEntry(void);

	// Overrides
	virtual BOOL terminate(void);

	// server creation

	SODaCServer *addServer(IN LPCTSTR serverName, IN LPCTSTR nodeName, 
			IN REFCLSID classID, IN DWORD clsctx = CLSCTX_ALL);

	// object factory
	void setCreator(IN SODaCCreator* pCreator);
	SODaCCreator *getCreator(void);

	// tree management
	virtual BOOL addLeaf(IN SOCmnElement *newLeaf);
	virtual BOOL addBranch(IN SOCmnElement *newBranch);

	// serialization
	virtual BOOL serialize(IN OUT SOCltStorage& ar, IN BOOL isStoring);
	DWORD getFileVersion(void) { return m_fileVersion; }

	// get child object by name
	SODaCServer *getServer(IN LPCTSTR serverName);

protected:
	SODaCCreator *m_creator;		// creator object
	DWORD m_fileVersion;				// version of currently loaded file

	virtual SOCmnObject *getObjectCmnElementIList(void);
	virtual SOCmnObject *getObjectCltElementIError(void);

}; // SODaCEntry

inline SODaCCreator *SODaCEntry::getCreator()
{ if (m_creator) m_creator->addRef(); return (SODaCCreator *)m_creator; }

#define OPCSession SODaCEntry




//-----------------------------------------------------------------------------
// Globals                                                                    |
//-----------------------------------------------------------------------------

// get reference to SODaC Entry object
SODAC_EXPORT SODaCEntry* getOPCSession(void);
SODAC_EXPORT SODaCEntry* getSODaCEntry(void);

#pragma pack(pop)
#endif 
