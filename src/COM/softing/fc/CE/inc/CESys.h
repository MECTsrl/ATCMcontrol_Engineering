#ifndef __CESYS_H_
#define __CESYS_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

#ifndef CESYS_H
#define CESYS_H

#include "CESysDef.h"

////////////////////////////////////////////////////////////////////////////
// common macros used by 4CE
#ifndef ARRAY_LEN
#define ARRAY_LEN(var) (sizeof(var)/sizeof((var)[0]))
#endif //ARRAY_LEN


/////////////////////////////////////////////////////////////////////////////
// defines the MessageStruct used for data exchange via messages
// the sender of a messages allocates memory for such a


// this structs are used to communicate a notification about an item from the
// navigators tree to its environment

// used with NAVN_DBLCLK
struct NAVDBlClkMsg {
	long	lTreeMode;		// Id of the 'sending' tree
	CString	strName;		// name of the Object to open
	NAVDBlClkMsg(long lMode, LPCTSTR lpszName) {
		lTreeMode	= lMode;
		strName		= lpszName;
		return;
	}
};

// used with NAVN_RCLICK
struct NAVRClickMsg {
	long	lNavTreeMode;	// TreeMode of the navigator
	long	lNavTreeId;		// ImageId off the Tree in the navigator
	long	lObjClass;		// ObjClass of the 'sending' item
							// one off the allowed ATCM Object Class Ids (cedb.h)
                            // not used if PrjListView is the source of the structure
	long	lObjId;			// ObjId of the 'sending' item
							// the Id of the 4CObject table of the database
    long    lInstId;        // InstaneID of the 4CObject, only valid if lNavTreeId is an implicite instance image Id!
                            // the Id of the Instance table of the database
	CPoint	CurrPosition;	// current position of the mouse in screen coordinates
    HTREEITEM    hTreeObj;  // tree item handle of the messages source object 
                            // nItem if PrjListView is the source of the structure
    CString strObjName;     // name of the sending item

	NAVRClickMsg(long lTreeMode, long lTreeId, 
                 long lClass, long lId, const CPoint MousePos,
                 HTREEITEM hObj = NULL, LPCTSTR pszObjName = NULL, long lInstanceId = 0) {
		lNavTreeMode    = lTreeMode;
		lNavTreeId	    = lTreeId;
		lObjClass	    = lClass;
		lObjId		    = lId;
		CurrPosition    = MousePos;
        hTreeObj        = hObj;
        strObjName      = pszObjName;
        lInstId         = lInstanceId;
		return;
	}
	NAVRClickMsg() {
		lNavTreeMode    = 0;
		lNavTreeId	    = 0;
		lObjClass	    = 0;
		lObjId		    = 0;
		CurrPosition    = CPoint(0, 0);
        hTreeObj        = NULL;
        strObjName.Empty();
        lInstId         = 0;
		return;
	}
	NAVRClickMsg(NAVRClickMsg* pMsg) {
		lNavTreeMode    = pMsg->lNavTreeMode;
		lNavTreeId	    = pMsg->lNavTreeId;
		lObjClass	    = pMsg->lObjClass;
		lObjId		    = pMsg->lObjId;
		CurrPosition    = pMsg->CurrPosition;
        hTreeObj        = pMsg->hTreeObj;
        strObjName      = pMsg->strObjName;
        lInstId         = pMsg->lInstId;
		return;
	}
};

// used with SESSION_NOTIFY
struct SessionMsg {
	long	lSessionObjId;	// Id of the 'sending' PC object
    long    lSessionState;  // new State of the PC object
    SessionMsg(long lObjId, long lState) {
        lSessionObjId   = lObjId;
        lSessionState   = lState;
        return;
    }
};

#endif //CESYS_H

#endif // __CESYS_H_