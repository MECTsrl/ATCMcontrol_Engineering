/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/opcItem.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: opcItem.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/opcItem.h $
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

#ifndef __OPCITEM_H_
#define __OPCITEM_H_

class   CDynamicOpcServer;

//------------------------------------------------------------------*
/**
 *  class COpcItem
 *
 *  @see    ....
*/
class COpcTag :
            public GenericTag
{
// =Construction
public:
                            COpcTag();
                                // default ctor
    virtual                 ~COpcTag();
                                // dtor

private:
                            COpcTag(COpcTag const & other);
                                // copy ctor
    COpcTag const &    operator = (COpcTag const & other);
                                // assignment

// =Access
public:

// =Operations
public:
    HRESULT                 CheckAccessRights(DWORD dwReqAccessRights, IN GenericItem *item);
    HRESULT                 SetAccessRights(CSecurity::SECURITY_RIGHTS newRights, IN GenericItem *item);
    CSecurity::SECURITY_RIGHTS  GetCurrentAccessRights(IN GenericItem *item);
    void                    SavePassword(LPCTSTR pszPassword, IN GenericItem *item);

// =Overrides
public:
	virtual HRESULT         updateReadCache(IN GenericCache *cache, IN GenericItem *item) = 0;
	virtual HRESULT         writeToVar(IN VARIANT vtNew, IN GenericItem *item) = 0;

protected:

// =Helpers
private:
    CDynamicOpcServer *     GetCurrentSession(IN GenericItem *item);

// =Attributes
protected:

private:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif

/* ---------------------------------------------------------------------------- */
