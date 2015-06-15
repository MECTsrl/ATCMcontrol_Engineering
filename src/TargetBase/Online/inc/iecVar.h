/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/iecVar.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: iecVar.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/iecVar.h $
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

#ifndef _IECVAR_H_
#define _IECVAR_H_

//------------------------------------------------------------------*
/**
 *  class IEC_Variable
 *
 *  @see    ....
*/
class IEC_Variable :
    public COpcTag
{
// =Construction
public:
                            IEC_Variable();
                                // default ctor
protected:
    virtual                 ~IEC_Variable();
                                // dtor

private:
                            IEC_Variable(IEC_Variable const & other);
                                // copy ctor
    IEC_Variable const &    operator = (IEC_Variable const & other);
                                // assignment

// =Access
public:
    inline void             SetFullName(LPCTSTR pszFullName);
    inline LPCTSTR          GetFullName(void) const;

// =Operations
public:
    HRESULT                 Attach(CSC_VARADDR *pVarAddr, long downloadCounter);
                                // attach to control's object interface

    HRESULT                 SetParent(SRT_Resource *pParent);

// =Overrides
public:
	// handle I/O requests
	virtual HRESULT         updateReadCache(IN GenericCache *cache, IN GenericItem *item);
	virtual HRESULT         writeToVar(IN VARIANT vtNew, IN GenericItem *item);

    HRESULT                 BuildReadRequest(IN GenericCache *cache, IN GenericItem *item, IN OUT BYTE**request, IN OUT UINT *requestLen);
    HRESULT                 updateReadCache(IN GenericCache *cache, IN GenericItem *item, IN BYTE *request, UINT requestLen, UINT *readPos);

    HRESULT                 BuildWriteRequest(IN VARIANT vtNew, IN GenericItem *item, IN OUT BYTE**request, IN OUT UINT *requestLen);

protected:

// =Helpers
private:
    HRESULT                 VariantToFileTime(DATE dt, FILETIME *pFileTime);
    HRESULT                 CheckForNamespaceUpdate();

// =Attributes
protected:

private:
    BOOL                    m_bValid;
    CSC_VARADDR             m_varAddr;
    CString                 m_strFullName;
    SRT_Resource            *m_pParent;
    long                    m_downloadCounter;
};


//------------------------------------------------------------------*
/**
*/
inline void
    IEC_Variable::SetFullName(LPCTSTR pszFullName)
{
    m_strFullName = pszFullName;
}

//------------------------------------------------------------------*
/**
*/
inline LPCTSTR
    IEC_Variable::GetFullName(void) const
{
    return m_strFullName;
}



//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif

/* ---------------------------------------------------------------------------- */
