/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/iecResource.h 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: iecResource.h $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/inc/iecResource.h $
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

#ifndef _IECRESOURCE_H_
#define _IECRESOURCE_H_

//------------------------------------------------------------------*
/**
 *  class IEC_Resource
 *
 *  @see    ....
*/
class IEC_Resource : public GenericNode
{
// =Construction
public:
                            IEC_Resource();
                                // default ctor
protected:
    virtual                 ~IEC_Resource();
                                // dtor

private:
                            IEC_Resource(IEC_Resource const & other);
                                // copy ctor
    IEC_Resource const &    operator = (IEC_Resource const & other);
                                // assignment

// =Access
public:
    LPCTSTR                 GetSystemName(void) const
                            {
                                return m_strSystemName;
                            }
    BOOL                    IsSystemName(LPCTSTR pszName)
                            {
                                return (m_strSystemName.CompareNoCase(pszName) == 0);
                            }

// =Operations
public:

// =Overrides
public:
    virtual HRESULT         OpenResourceInstance() = 0;
    virtual BOOL            CheckForNamespaceUpdate(void)= 0;
    virtual HRESULT         FillChildrenList(OUT CStringList &listChildren,
                                             IN LPCTSTR pszCurrentPath, IN CStringList &parsedPath,
                                             OPCBROWSETYPE BrowseFilterType, LPCTSTR pszFilterCriteria,
                                             VARTYPE DataTypeFilter, DWORD AccessRightsFilter)= 0;
    virtual BOOL            isDynamicNode(CStringList &listParsedName) = 0;
    virtual BOOL            isDynamicTag(CStringList &listParsedName) = 0;
    virtual GenericTag *    createDynamicTag(IN CStringList &parsedPath, LPCTSTR pszFullName) = 0;

protected:

// =Helpers
private:

// =Attributes
protected:
    CString                 m_strSystemName;

private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif

/* ---------------------------------------------------------------------------- */
