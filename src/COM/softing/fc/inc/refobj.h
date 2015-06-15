/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/refobj.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: refobj.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/refobj.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                INC
 *
 * =COMPONENT           INC
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *== CRefObj - simple referenced object implementation. 
 *== CExclusiveSection - encapsulates critical section.
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  24.01.2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/
#ifndef __REFOBJ_H_
#define __REFOBJ_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//for other assert mechanisms replace this.
#ifndef REFOBJASSERT
#include <assert.h>
#define REFOBJASSERT(x) assert((x))
#endif
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
//------------------------------------------------------------------*
/**
 * class CRefObj - provides a referenced object implementation.
 * The implementation is thread safe. Derived classes are not forced
 * to implement referencing again. OnFinalRelease can be overwritten
 * by derived classes to perform object cleanup during the last release
*/
class CRefObj
{
protected:
	LONG m_lRef;
public:

	//------------------------------------------------------------------*
	/**
	 * CRefObj ctor - constructs a referenced object with an initial
	 * ref-count 0. Call AddRef after construction to increase reference
	 * count. The object will be deleted using the delete operator. Hence
	 * it is neccessary to create the object facilitating the new operator.
	 * @see AddRef
	*/
	CRefObj(void)
    {
	    m_lRef = 0;
    }
    
	//------------------------------------------------------------------*
	/**
	 * CRefObj dtor - reference count has to be 0.
	*/
	virtual ~CRefObj(void)
    {
    	REFOBJASSERT(!m_lRef);
    }
	
	//------------------------------------------------------------------*
	/**
	 * Increments reference count by one. Returns actual value of 
	 * reference count. Release decrements reference count by one and
	 * additionally deletes object if reference count reaches 0.
	 *
	 * @return			ULONG representing actual reference count.
	 * @see				Release				
	*/
	ULONG AddRef(void)
    {
        REFOBJASSERT(m_lRef >= 0);
        LONG lRef = InterlockedIncrement(&m_lRef);
	    return lRef;
    }
	
	//------------------------------------------------------------------*
	/**
	 * Decrements reference count by one. Returns actual value of
	 * reference count. If the reference count reaches 0 the object 
	 * will be deleted. Hence it is necessary to create the object using 
	 * the new operator. The counterpart AddRef increments the reference 
	 * count by one.
	 * 
	 * @return			ULONG representing actual reference count.
	 * @see				AddRef				
	*/
	ULONG Release(void)
    {
	    LONG lRef = InterlockedDecrement(&m_lRef);
	    if(!lRef) {
            OnFinalRelease();
		    delete this;
		    return 0UL;
	    }
	    return lRef;
    }

	//------------------------------------------------------------------*
	/**
	 * assigns a source CRefObj* to the destination CRefObj*.
	 * The original destination @param pDest will be released after the assingment
	 * The new destination will be addref'd after the assignment if not NULL.
	 * Parameter @param pDest and @param pSrc can be both NULL.
	 * For thread safety it is recommended to guard the function Assign by a
	 * critical section or the like.
	 * @param	pSrc - source CRefObj pointer. Can be NULL.
	 * @param	pDest - destination CRefObj pointer. Can be NULL.
	*/

	static void Assign(CRefObj* &pDest, CRefObj* &pSrc)
	{
		CRefObj* pTemp = NULL;
		if(pDest) {
			pTemp = pDest;
		}
		pDest = pSrc;
		if(pDest) {
			pDest->AddRef();
		}
		if(pTemp) {
			pTemp->Release();
		}
	}

protected:
	//------------------------------------------------------------------*
	/**
	 * OnFinalRelease - will be called when the last reference to the
	 * object derived from CRefObj is released. 
	 * Derived classes can perform class specific cleanup actions in an 
	 * override of OnFinalRelease.
	*/
    virtual void OnFinalRelease()
    {}

};


//------------------------------------------------------------------*
/**
 *  class CExclusiveSection
 *  encapsulated critical section syncronization or protection can be 
 *  realized by another mechanism than an critical section if necessary
*/
class	CExclusiveSection 
{
public:
	//------------------------------------------------------------------*
	/**
	 * Constructs exclusive section object. The underlying critical section
	 * will be initialized.
	 *
	 * @see	SDK - InitializeCriticalSection
	*/
	CExclusiveSection()
    {
        InitializeCriticalSection(&m_lockSection);
    }

	//------------------------------------------------------------------*
	/**
	 * Will be called upon deletion of the exclusive section object. The
	 * underlying critical section will be deleted as well.
	 *
	 * @see SDK - DeleteCriticalSection				
	*/
	~CExclusiveSection()
    {
        DeleteCriticalSection(&m_lockSection);
    }

	//------------------------------------------------------------------*
	/**
	 * enters the critical section. Calls EnterCriticalSection to enter
	 * the synchronized code area. Always call leave() when leaving the
	 * synchronized area
	 *
	 * @see SDK - EnterCriticalSection, leave.
	*/
	DWORD enter	(DWORD timeout = 0)
    {
        EnterCriticalSection(&m_lockSection);
	    return 0;
    }

	//------------------------------------------------------------------*
	/**
	 * leaves the critical section. Calls LeaveCriticalSection to leave
	 * the synchronized code area. Enter has to be called prior to calling
	 * leave.
	 *
	 * @see SDK - LeaveCriticalSection, enter.
	*/
	DWORD leave	()
    {
        LeaveCriticalSection(&m_lockSection);
	    return 0;
    }

private:
    CRITICAL_SECTION    m_lockSection;

};

#endif // __REFOBJ_H_
/*
 *----------------------------------------------------------------------------*
 *  $History: refobj.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 2  *****************
 * User: Su           Date: 2.05.01    Time: 11:46
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Su           Date: 24.01.01   Time: 13:43
 * Created in $/4Control/COM/softing/fc/inc
 * Moved to ATCMControl V2.0 Sourcesafe data base.
 * 
 * *****************  Version 3  *****************
 * User: Su           Date: 24.01.01   Time: 13:39
 * Updated in $/4ControlMesse/COM/softing/fc/inc
 * Added comments and class descriptions
 *==
 *----------------------------------------------------------------------------*
*/
