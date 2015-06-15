/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/SysVar.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: SysVar.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/SysVar.h $
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

#ifndef _SYS_VAR_H_
#define _SYS_VAR_H_

//------------------------------------------------------------------*
/**
 *  class CSysVar
 *
 *  @see    ....
*/
class CSysVar :
    public COpcTag
{
// =Construction
public:
                            CSysVar(LPCTSTR pszName = NULL, DWORD rights = OPC_READABLE, VARTYPE datatype = VT_BSTR);
                                // default ctor
protected:
    virtual                 ~CSysVar();
                                // dtor

private:
                            CSysVar(CSysVar const & other);
                                // copy ctor
    CSysVar const &    operator = (CSysVar const & other);
                                // assignment

// =Access
public:

// =Operations
public:
    virtual HRESULT         Update(LPCTSTR pszNewValue) = 0;
                                // set new value in registry
    virtual HRESULT         Reread() = 0;
                                // reread current value from registry

// =Overrides
// GenericTag
public:
	// handle I/O requests
	virtual HRESULT         updateReadCache(IN GenericCache *cache, IN GenericItem *item) = 0;
	virtual HRESULT         writeToVar(IN VARIANT vtNew, IN GenericItem *item) = 0;

protected:

// =Helpers
private:

// =Attributes
protected:

};

//------------------------------------------------------------------*
/**
 *  class CSysVarRegistry
 *
 *  @see    ....
*/
class CSysVarRegistry : public CSysVar
{
// =Construction
public:
                            CSysVarRegistry(LPCTSTR pszName);
                                // default ctor
protected:
    virtual                 ~CSysVarRegistry();
                                // dtor

private:
                            CSysVarRegistry(CSysVarRegistry const & other);
                                // copy ctor
    CSysVarRegistry const &    operator = (CSysVarRegistry const & other);
                                // assignment

// =Access
public:

// =Operations
public:
    HRESULT                 Update(LPCTSTR pszNewValue);
                                // set new value in registry
    HRESULT                 Reread();
                                // reread current value from registry

// =Overrides
public:
// GenericTag
public:
	// handle I/O requests
	virtual HRESULT         updateReadCache(IN GenericCache *cache, IN GenericItem *item);
	virtual HRESULT         writeToVar(IN VARIANT vtNew, IN GenericItem *item);

protected:

// =Helpers
private:
    HRESULT                 InitFromRegistry(HKEY hkNode);
    HRESULT                 UpdateToRegistry(HKEY hkNode);

// =Attributes
protected:

private:
    CComVariant             m_vtMutexedValue;
};

//------------------------------------------------------------------*
/**
 *  class CSysVarSecurity
 *
 *  @see    ....
*/
class CSysVarSecurity : public CSysVar
{
public:
typedef enum {
    isEnabled,
    Password,
    Rights
} SecurityType;

// =Construction
public:
                            CSysVarSecurity(LPCTSTR pszName, SecurityType type);
                                // default ctor
protected:
    virtual                 ~CSysVarSecurity();
                                // dtor

private:
                            CSysVarSecurity(CSysVarSecurity const & other);
                                // copy ctor
    CSysVarSecurity const &    operator = (CSysVarSecurity const & other);
                                // assignment

// =Access
public:

// =Operations
public:
    HRESULT                 Update(LPCTSTR pszNewValue);
                                // set new value in registry
    HRESULT                 Reread();
                                // reread current value from registry

// =Overrides
// GenericTag
public:
	// handle I/O requests
	virtual HRESULT         updateReadCache(IN GenericCache *cache, IN GenericItem *item);
	virtual HRESULT         writeToVar(IN VARIANT vtNew, IN GenericItem *item);

protected:

// =Helpers
private:
    HRESULT                 UpdateSecurity();

// =Attributes
protected:

private:
    SecurityType            m_securityType;
    BOOL                    m_bIsEnabled;
    CString                 m_strPassword;
    CSecurity::SECURITY_RIGHTS  m_rights;
};

//------------------------------------------------------------------*
/**
 *  class CSysVarDebug
 *
 *  @see    ....
*/
class CSysVarDebug : public CSysVar
{
public:
typedef enum {
    isEnabled,
    startedDate,
    currUser,
    currHost
} DbgType;

// =Construction
public:
                            CSysVarDebug(LPCTSTR pszName, DbgType type);
                                // default ctor
protected:
    virtual                 ~CSysVarDebug();
                                // dtor

private:
                            CSysVarDebug(CSysVarDebug const & other);
                                // copy ctor
    CSysVarDebug const &    operator = (CSysVarDebug const & other);
                                // assignment

// =Access
public:

// =Operations
public:
    HRESULT                 Update(LPCTSTR pszNewValue);
                                // set new value in registry
    HRESULT                 Reread();
                                // reread current value from registry

// =Overrides
// GenericTag
public:
	// handle I/O requests
	virtual HRESULT         updateReadCache(IN GenericCache *cache, IN GenericItem *item);
	virtual HRESULT         writeToVar(IN VARIANT vtNew, IN GenericItem *item);

protected:

// =Helpers
private:

// =Attributes
protected:

private:
    DbgType                 m_debugType;
    BOOL                    m_bIsEnabled;
    CString                 m_strVal;
};


//------------------------------------------------------------------*
/**
 *  class CSysVarResource
 *
 *  @see    ....
*/
class CSysVarResource : public CSysVar
{
public:
typedef enum {
    state,
    dnload
} ResVarType;

typedef enum {
    NoConnect  = -4,
    BadVersion = -3,
    NotRunning = -2,
    error = -1,
    OnCreation  = 0,
    Paused      = 1,
    Running     = 2
} ControlState;


// =Construction
public:
                            CSysVarResource(LPCTSTR pszName, ResVarType type);
                                // default ctor
protected:
    virtual                 ~CSysVarResource();
                                // dtor

private:
                            CSysVarResource(CSysVarResource const & other);
                                // copy ctor
    CSysVarResource const &    operator = (CSysVarResource const & other);
                                // assignment

// =Access
public:

// =Operations
public:
    HRESULT                 Update(LPCTSTR pszNewValue);
                                // set new value in registry
    HRESULT                 Reread();
                                // reread current value from registry

// =Overrides
// GenericTag
public:
	// handle I/O requests
	virtual HRESULT         updateReadCache(IN GenericCache *cache, IN GenericItem *item);
	virtual HRESULT         writeToVar(IN VARIANT vtNew, IN GenericItem *item);

protected:

// =Helpers
private:
    ControlState            GetResourceState();

// =Attributes
protected:

private:
    ResVarType              m_resVarType;
};

#endif

/* ---------------------------------------------------------------------------- */
