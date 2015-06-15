
#include "stdafx.h"
#include "resource.h"
#include "DialogGenPB.h"
#include "ObjPropGenGlobVar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenPB::CDialogGenPB
 * Description      constructor for class CDialogGenPB
 * @param           DIALOGTYPE DialogType
 * @param           CPropertySheetBase *PropertySheet
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenPB::CDialogGenPB(DIALOGTYPE DialogType, 
                           CPropertySheetBase *PropertySheet) :
    m_ObjPropGenPB(DialogType, PropertySheet),
    CDialogGenBase(m_ObjPropGenPB)
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenPB::~CDialogGenPB
 * Description      destructor for class CDialogGenPB
 * @return          -
 * @exception       -
 * @see             -
*/
CDialogGenPB::~CDialogGenPB()
{
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenPB::GetChannelNumber
 * Description      returns the channel number for the profibus
 * @return          CString
 * @exception       -
 * @see             -
*/
CString CDialogGenPB::GetChannelNumber()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	return m_ObjPropGenPB.m_ChannelNumber;
}

//------------------------------------------------------------------*
/**
 * Function name    CDialogGenPB::SetChannelNumber
 * Description      sets the channel nuber for the profibus
 * @param           CString ChannelNumber
 * @return          -
 * @exception       -
 * @see             -
*/
void CDialogGenPB::SetChannelNumber(CString ChannelNumber)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	m_ObjPropGenPB.m_ChannelNumber = ChannelNumber;
}

