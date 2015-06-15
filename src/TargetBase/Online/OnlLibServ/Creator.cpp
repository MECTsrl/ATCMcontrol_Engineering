/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/Creator.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Creator.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/Creator.cpp $
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

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "creator.h"
#include "Security.h"
#include "opcItem.h"
#include "opcDynServer.h"
#include "opcDynGroup.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS Creator
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
Creator::Creator(HANDLE event, RequestList *readList, RequestList *writeList, CSyncObject *readSync, CSyncObject *writeSync)
{
    assert(readList);
    assert(writeList);
    assert(readSync);
    assert(writeSync);

    m_event = event;
    m_readList = readList;
    m_writeList = writeList;
    m_readSync = readSync;
    m_writeSync = writeSync;
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
Creator::~Creator()
{
}

//------------------------------------------------------------------*
/**
 * createServer
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
*/
GenericServer *
    Creator::createServer(IN BOOL publicGroups)
{
    return new CDynamicOpcServer(publicGroups);
}

//------------------------------------------------------------------*
/**
 * createGroup
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
*/
GenericGroup *
    Creator::createGroup(IN GenericServer * /*parent*/)
{
    return new CDynamicOpcGroup;
}

//------------------------------------------------------------------*
/**
 * createTag
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
*/
GenericTag *
    Creator::createTag(IN GenericServer *server, IN LPCTSTR itemID)
{
    CDynamicOpcServer *pServer = dynamic_cast<CDynamicOpcServer*>(server);
    if (pServer == NULL)
        return NULL;
    GenericTag *tag = pServer->createDynamicTag(itemID);
    Bless(tag);
    return tag;
}

//------------------------------------------------------------------*
/**
 * Bless
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
*/
void    Creator::Bless(GenericTag *tag)
{
    if (tag != NULL)
    {
        assert(m_readList);
        assert(m_writeList);
        assert(m_readSync);
        assert(m_writeSync);

        tag->setReadEvent(m_event);
        tag->setWriteEvent(m_event);
        tag->setReadQueue(m_readList);
        tag->setWriteQueue(m_writeList);
        tag->setReadQueueLockObj(m_readSync);
        tag->setWriteQueueLockObj(m_writeSync);
    }
}

/* ---------------------------------------------------------------------------- */
