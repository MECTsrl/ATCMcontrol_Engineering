/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/opcDynGroup.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: opcDynGroup.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/opcDynGroup.h $
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

#ifndef __OPCDYNGROUP_H_
#define __OPCDYNGROUP_H_

#include <gengrp.h>

//------------------------------------------------------------------*
/**
 *  class CDynamicOpcGroup
 *
 *  @see    ....
*/
class CDynamicOpcGroup :
            public GenericGroup
{
// =Serialization
// =Construction
public:
                            CDynamicOpcGroup();
                                // default ctor
    virtual                 ~CDynamicOpcGroup();
                                // dtor
private:
                            CDynamicOpcGroup(CDynamicOpcGroup const & other);
                                // copy ctor
    CDynamicOpcGroup const &    operator = (CDynamicOpcGroup const & other);
                                // assignment

// =Access
public:

// =Operations
public:

// =Overrides
public:
	// IOPCItemMgt
	virtual HRESULT         validateItem(IN LPCTSTR id, IN LPCTSTR accessPath, 
			                    IN VARTYPE reqDataType, OUT VARTYPE *nativeDataType, 
			                    OUT DWORD *accessRights, OUT OPTIONAL GenericItemTag **tag = NULL);

    BOOL changeUpdateRate(IN DWORD updateRate);
    BOOL checkUpdateRate(IN DWORD requestTime);

protected:

// =Helpers
private:
    DWORD dwOrigUpdateRate;
    DWORD dwMaxRequestTime;
    DWORD dwRequestCount;

    DWORD dwLastCheckTime;
// =Attributes
protected:

private:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif

/* ---------------------------------------------------------------------------- */
