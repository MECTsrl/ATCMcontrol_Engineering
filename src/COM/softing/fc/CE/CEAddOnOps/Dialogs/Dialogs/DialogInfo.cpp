
#include "stdafx.h"
#include "resource.h"
#include "DialogInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogInfo::CDialogInfo
 * Description      constructor for class CDialogInfo
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogInfo::CDialogInfo(DIALOGTYPE DialogType, CPropertySheetBase *PropertySheet) :
    m_ObjPropInfo(DialogType, PropertySheet)
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogInfo::~CDialogInfo
 * Description      destructor for class CDialogInfo
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogInfo::~CDialogInfo()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogInfo::SetUrl
 * Description      sets the url
 * @param           CString Url
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogInfo::SetUrl(CString Url)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    m_ObjPropInfo.m_EditUrl = Url;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogInfo::GetUrl
 * Description      returns the url
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogInfo::GetUrl()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_ObjPropInfo.m_EditUrl;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogInfo::SetHelpUrl
 * Description      sets the help url
 * @param           CString HelpUrl
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogInfo::SetHelpUrl(CString HelpUrl)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    m_ObjPropInfo.m_EditHelpUrl = HelpUrl;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogInfo::GetHelpUrl
 * Description      sets the help url
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogInfo::GetHelpUrl()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_ObjPropInfo.m_EditHelpUrl;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogInfo::SetText1
 * Description      sets the text 1
 * @param           CString Text1
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogInfo::SetText1(CString Text1)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    m_ObjPropInfo.m_EditText1 = Text1;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogInfo::GetText1
 * Description      returns the text 1
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogInfo::GetText1()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_ObjPropInfo.m_EditText1;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogInfo::SetText2
 * Description      sets the text 2
 * @param           CString Text2
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogInfo::SetText2(CString Text2)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    m_ObjPropInfo.m_EditText2 = Text2;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogInfo::GetText2
 * Description      returns the text 2
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogInfo::GetText2()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_ObjPropInfo.m_EditText2;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogInfo::SetText3
 * Description      sets the text 3
 * @param           CString Text3
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogInfo::SetText3(CString Text3)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    m_ObjPropInfo.m_EditText3 = Text3;
} 

//------------------------------------------------------------------*
/**
 * Function name    CDialogInfo::GetText3
 * Description      returns the text 3
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogInfo::GetText3()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
    return m_ObjPropInfo.m_EditText3;
}
