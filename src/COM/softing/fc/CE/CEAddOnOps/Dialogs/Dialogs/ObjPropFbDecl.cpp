
#include "stdafx.h"
#include "resource.h"
#include "ObjPropFbDecl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//------------------------------------------------------------------*
/**
 * Function name    CObjPropFbDecl::CObjPropFbDecl
 * Description      constructor for class CObjPropFbDecl
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @param           int iColumnStrId
 * @param           LPCTSTR pszSClass[]
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropFbDecl::CObjPropFbDecl(DIALOGTYPE DialogType,
                               CPropertySheetBase *PropertySheet,
                               int iColumnStrId,
                               LPCTSTR pszSClass[]) :
    CObjPropDeclBase(DialogType, IDD_OBJPROP_FBDECL, PropertySheet, iColumnStrId)
{
    m_pszSClasses = pszSClass;
}

//------------------------------------------------------------------*
/**
 * Function name    CObjPropFbDecl::~CObjPropFbDecl
 * Description      destructor for class CObjPropFbDecl
 * @return          -
 * @exception       -
 * @see             -
*/
CObjPropFbDecl::~CObjPropFbDecl()
{
    if(m_pMembers)
    {
        ::SafeArrayDestroy(m_pMembers);
        m_pMembers = NULL;
    }
}
