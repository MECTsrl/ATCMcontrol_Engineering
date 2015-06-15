//-----------------------------------------------------------------------------
//                                                                            |



//                                                                            |



//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                           OPC TOOLBOX - SOAeC                              |
//                                                                            |
//  Filename    : SOAeCEntry.h                                                |
//  Version     : 3.10.release                                                |
//  Date        : 4-March-2002                                                |
//                                                                            |
//  Description : Entry point for OPC Data Access Client Toolkit              |
//                - SOAeCEntry: Entry point class                             |
//                - SOAeCCreator: Object factory for toolkit objects          |
//                                                                            |
//-----------------------------------------------------------------------------

#ifndef _SOAECENTRY_H_
#define _SOAECENTRY_H_

#pragma pack(push,4)

class SOAeCEntry;
class SOAeCServer;
class SOAeCEvent;
class SOAeCSubscription;

//-----------------------------------------------------------------------------
// SOAeCCreator                                                               |
//-----------------------------------------------------------------------------

class SOAEC_EXPORT SOAeCCreator : public SOCmnObject
{
public:
	SOAeCCreator();

	virtual SOAeCServer* createServer(IN SOAeCEntry *parent);
	virtual SOAeCSubscription* createSubscription(IN SOAeCServer *parent);
	virtual SOAeCEvent* createEvent(IN SOAeCSubscription *subs, IN const ONEVENTSTRUCT& data);

protected:
	virtual ~SOAeCCreator();
}; // SOAeCCreator




//-----------------------------------------------------------------------------
// SOAeCEntry                                                                 |
//-----------------------------------------------------------------------------

class SOAEC_EXPORT SOAeCEntry : 
	public SOCltEntry,
	virtual public SOCmnElementListBranch,
	virtual public SOCltElementErrorStore
{
public:
	SOAeCEntry(void);

	// server creation
	SOAeCServer *addServer(IN LPCTSTR serverName, IN LPCTSTR nodeName, 
			IN REFCLSID classID, IN DWORD clsctx = CLSCTX_ALL);

	// Overrides
	virtual BOOL terminate(void);

	// object factory
	void setCreator(IN SOAeCCreator* pCreator);
	SOAeCCreator *getCreator(void);

	// tree management
	virtual BOOL addLeaf(IN SOCmnElement *newLeaf);
	virtual BOOL addBranch(IN SOCmnElement *newBranch);

	// serialization
	virtual BOOL serialize(IN OUT SOCltStorage& ar, IN BOOL isStoring);
	DWORD getFileVersion(void) { return m_fileVersion; }

	// get child object by name
	SOAeCServer *getServer(IN LPCTSTR serverName);

protected:
	SOAeCCreator* m_creator;		    // creator object
	DWORD m_fileVersion;						// version of currently loaded file

	virtual SOCmnObject *getObjectCmnElementIList(void);
	virtual SOCmnObject *getObjectCltElementIError(void);
	virtual ~SOAeCEntry(void);
}; // SOAeCEntry

inline SOAeCCreator *SOAeCEntry::getCreator()
{ if (m_creator) m_creator->addRef(); return m_creator; }


//-----------------------------------------------------------------------------
// Globals                                                                    |
//-----------------------------------------------------------------------------

// get reference to SOAeCEntry object
SOAEC_EXPORT SOAeCEntry* getSOAeCEntry(void);

#pragma pack(pop)
#endif 
