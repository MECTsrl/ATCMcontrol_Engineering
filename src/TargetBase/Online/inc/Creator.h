/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/Creator.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Creator.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/Creator.h $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase / Online
 *
 * =COMPONENT			OnlLibServ
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

#ifndef _CREATOR_H_
#define _CREATOR_H_

//------------------------------------------------------------------*
/**
 *  class Creator
 *      factory for C++ objects allocated by OPC engine
 *
 *  @see    docu of OPC engine
*/
class Creator :
            public GenericCreator
{
// =Construction
public:
                            Creator(HANDLE event, RequestList *readList, RequestList *writeList, CSyncObject *readSync, CSyncObject *writeSync);
                                // default ctor
    virtual                 ~Creator();
                                // dtor
private:
                            Creator(Creator const & other);
                                // copy ctor
    Creator const &    operator = (Creator const & other);
                                // assignment

// =Access
public:

// =Operations
public:

	virtual GenericServer *createServer(IN BOOL publicGroups);
	virtual GenericGroup *createGroup(IN GenericServer *parent);
//	virtual GenericItem *createItem(IN GenericGroup *parent, IN GenericItemTag *tag);
//	virtual GenericRequest *createRequest(IN GenericServer *server, IN GenericItem *item, 
//			IN GenericItemTag *tag, IN BYTE operation, IN BYTE type, IN BYTE deviceIOMode);
//	virtual GenericCache *createCache(IN GenericItemTag *tag);
	virtual GenericTag *createTag(IN GenericServer *server, IN LPCTSTR itemID);
//	virtual GenericTagProperty *createTagProperty(IN GenericTag *tag, IN DWORD propertyId);

    void    Bless(GenericTag *tag);

protected:

// =Helpers
private:

// =Attributes
protected:

private:
    HANDLE                  m_event;
    RequestList            *m_readList;
    RequestList            *m_writeList;
    CSyncObject            *m_readSync;
    CSyncObject            *m_writeSync;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif

/* ---------------------------------------------------------------------------- */
