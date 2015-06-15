
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "CEWindowPlacement.h"
#include "settings.h"

//----  Static Initializations:   ----------------------------------*


//------------------------------------------------------------------*
/**
 * CCEWindowPlacement constructor.
 *
 * @param            [in] pWindowPlacement: pointer to window placement structure
 *                                          may be null, if one is to be created.
 *                                          in this case use InitNew()
 * @return           -
 * @exception        -
 * @see              
*/
CCEWindowPlacement::CCEWindowPlacement(WINDOWPLACEMENT* pWindowPlacement /*= NULL*/)
{
    if(pWindowPlacement)
    {
        m_tWindowPlacement = *pWindowPlacement;
        m_bInitialized = TRUE;
    }
    else
    {
       m_bInitialized = FALSE;
    }
}

//------------------------------------------------------------------*
/**
 * CCEWindowPlacement destructor.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CCEWindowPlacement::~CCEWindowPlacement()
{
}

//------------------------------------------------------------------*
/**
 * init new.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEWindowPlacement::InitNew()
{
    // initialize structure with zeroes
    int iLen = sizeof(m_tWindowPlacement);
    memset(&m_tWindowPlacement, 0, iLen);
    // and set length
    m_tWindowPlacement.length = iLen;
    m_bInitialized = TRUE;
}


//------------------------------------------------------------------*
/**
 * reset.
 *
 *  set window placement member invalid (uninitialized)
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEWindowPlacement::Reset()
{
    m_bInitialized = FALSE;
}

//------------------------------------------------------------------*
/**
 * set window placement.
 *
 * @param           [in] rtWindowPlacement: (valid) window placement structure
 * @return          -
 * @exception       -
 * @see             -
*/
void CCEWindowPlacement::SetWindowPlacement(WINDOWPLACEMENT& rtWindowPlacement)
{
    m_bInitialized = TRUE;
    m_tWindowPlacement = rtWindowPlacement;
}

//------------------------------------------------------------------*
/**
 * get window placement.
 *
 * @param           [out] rtWindowPlacement: window placement structure
 * @return          is structure initialized?
 * @exception       -
 * @see             -
*/
BOOL CCEWindowPlacement::GetWindowPlacement(WINDOWPLACEMENT& rtWindowPlacement)
{
    rtWindowPlacement = m_tWindowPlacement;
    return m_bInitialized;
}

//------------------------------------------------------------------*
/**
 * load.
 *
 *  get attributes from node and set members.
 *
 * @param           [in] rtNode: node containing window placement structure
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CCEWindowPlacement::Load(CXMLNode& rtNode)
{
    CString strTmp;
    BOOL    bReturn = TRUE;

    InitNew();

    // flags
    if(!rtNode.GetAttribute(CE_PSET_WP_FLAGS, strTmp))
    {
        bReturn = FALSE;
    }
    m_tWindowPlacement.flags = atoi(strTmp);

    // showCmd
    if(!rtNode.GetAttribute(CE_PSET_WP_SHOW, strTmp))
    {
        bReturn = FALSE;
    }
    m_tWindowPlacement.showCmd = atoi(strTmp);

    // ptMinPosition
    if(!rtNode.GetAttribute(CE_PSET_WP_MINPOSX, strTmp))
    {
        bReturn = FALSE;
    }
    m_tWindowPlacement.ptMinPosition.x = atoi(strTmp);
    if(!rtNode.GetAttribute(CE_PSET_WP_MINPOSY, strTmp))
    {
        bReturn = FALSE;
    }
    m_tWindowPlacement.ptMinPosition.y = atoi(strTmp);

    // ptMaxPosition
    if(!rtNode.GetAttribute(CE_PSET_WP_MAXPOSX, strTmp))
    {
        bReturn = FALSE;
    }
    m_tWindowPlacement.ptMaxPosition.x = atoi(strTmp);
    if(!rtNode.GetAttribute(CE_PSET_WP_MAXPOSY, strTmp))
    {
        bReturn = FALSE;
    }
    m_tWindowPlacement.ptMaxPosition.y = atoi(strTmp);


    // rcNormalPosition
    if(!rtNode.GetAttribute(CE_PSET_WP_NORMPOSBOTTOM, strTmp))
    {
        bReturn = FALSE;
    }
    m_tWindowPlacement.rcNormalPosition.bottom = atoi(strTmp);
    if(!rtNode.GetAttribute(CE_PSET_WP_NORMPOSLEFT, strTmp))
    {
        bReturn = FALSE;
    }
    m_tWindowPlacement.rcNormalPosition.left = atoi(strTmp);
    if(!rtNode.GetAttribute(CE_PSET_WP_NORMPOSRIGHT, strTmp))
    {
        bReturn = FALSE;
    }
    m_tWindowPlacement.rcNormalPosition.right = atoi(strTmp);
    if(!rtNode.GetAttribute(CE_PSET_WP_NORMPOSTOP, strTmp))
    {
        bReturn = FALSE;
    }
    m_tWindowPlacement.rcNormalPosition.top = atoi(strTmp);

    return bReturn;
}

//------------------------------------------------------------------*
/**
 * save.
 *
 * @param           [in] rtNode: node where window placement info is to be written to
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CCEWindowPlacement::Save(CXMLNode& rtNode)
{
    CString strTmp;
    BOOL    bReturn = TRUE;

    if(!m_bInitialized)
    {
        return FALSE;
    }

    // flags
    strTmp.Format("%d", m_tWindowPlacement.flags);
    if(!rtNode.SetAttribute(CE_PSET_WP_FLAGS, strTmp))
    {
        bReturn = FALSE;
    }

    // showCmd
    strTmp.Format("%d", m_tWindowPlacement.showCmd);
    if(!rtNode.SetAttribute(CE_PSET_WP_SHOW, strTmp))
    {
        bReturn = FALSE;
    }

    // ptMinPosition
    strTmp.Format("%d", m_tWindowPlacement.ptMinPosition.x);
    if(!rtNode.SetAttribute(CE_PSET_WP_MINPOSX, strTmp))
    {
        bReturn = FALSE;
    }
    strTmp.Format("%d", m_tWindowPlacement.ptMinPosition.y);
    if(!rtNode.SetAttribute(CE_PSET_WP_MINPOSY, strTmp))
    {
        bReturn = FALSE;
    }

    // ptMaxPosition
    strTmp.Format("%d", m_tWindowPlacement.ptMaxPosition.x);
    if(!rtNode.SetAttribute(CE_PSET_WP_MAXPOSX, strTmp))
    {
        bReturn = FALSE;
    }
    strTmp.Format("%d", m_tWindowPlacement.ptMaxPosition.y);
    if(!rtNode.SetAttribute(CE_PSET_WP_MAXPOSY, strTmp))
    {
        bReturn = FALSE;
    }


    // rcNormalPosition
    strTmp.Format("%d", m_tWindowPlacement.rcNormalPosition.bottom);
    if(!rtNode.SetAttribute(CE_PSET_WP_NORMPOSBOTTOM, strTmp))
    {
        bReturn = FALSE;
    }
    strTmp.Format("%d", m_tWindowPlacement.rcNormalPosition.left);
    if(!rtNode.SetAttribute(CE_PSET_WP_NORMPOSLEFT, strTmp))
    {
        bReturn = FALSE;
    }
    strTmp.Format("%d", m_tWindowPlacement.rcNormalPosition.right);
    if(!rtNode.SetAttribute(CE_PSET_WP_NORMPOSRIGHT, strTmp))
    {
        bReturn = FALSE;
    }
    strTmp.Format("%d", m_tWindowPlacement.rcNormalPosition.top);
    if(!rtNode.SetAttribute(CE_PSET_WP_NORMPOSTOP, strTmp))
    {
        bReturn = FALSE;
    }

    return bReturn;
}

//------------------------------------------------------------------*
/**
 * is initialized?.
 *
 * @param           -
 * @return          is structure initialized?
 * @exception       -
 * @see             -
*/
BOOL CCEWindowPlacement::IsInitialized()
{
    return m_bInitialized;
}
