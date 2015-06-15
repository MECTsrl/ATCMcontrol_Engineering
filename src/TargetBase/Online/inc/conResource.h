/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/conResource.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: conResource.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/conResource.h $
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

#ifndef _CON_RESOURCE_H_
#define _CON_RESOURCE_H_

class   IEC_Variable;
class   CSysVarPool;

//------------------------------------------------------------------*
/**
 *  class CON_Resource
 *
 *  @see    ....
*/
class CON_Resource : public IEC_Resource
{
// =Construction
public:
                            CON_Resource(LPCTSTR pszConfigName);
                                // default ctor
protected:
    virtual                 ~CON_Resource();
                                // dtor

private:
                            CON_Resource(CON_Resource const & other);
                                // copy ctor
    CON_Resource const &    operator = (CON_Resource const & other);
                                // assignment

// =Access
public:

// =Operations
public:
// =Overrides
public:
    virtual HRESULT         OpenResourceInstance();
    virtual BOOL            CheckForNamespaceUpdate(void);
    virtual HRESULT         FillChildrenList(OUT CStringList &listChildren,
                                             IN LPCTSTR pszCurrentPath, IN CStringList &parsedPath,
                                             OPCBROWSETYPE BrowseFilterType, LPCTSTR pszFilterCriteria,
                                             VARTYPE DataTypeFilter, DWORD AccessRightsFilter);
    virtual BOOL            isDynamicNode(CStringList &listParsedName);
    virtual BOOL            isDynamicTag(CStringList &listParsedName);
    virtual GenericTag *    createDynamicTag(IN CStringList &parsedPath, LPCTSTR pszFullName);

protected:

// =Helpers
private:


// =Attributes
protected:

private:
    CString                 m_sConfigName;
    // namespace browse support 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif

/* ---------------------------------------------------------------------------- */
