
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "KADFileExtInfo.h"

//----  Static Initializations:   ----------------------------------*


CKADFileExtInfo::CKADFileExtInfo()
{
    m_pEditorGUID   = NULL;
    m_bProgEditor   = FALSE;
    m_pParserGUID   = NULL;
    m_bVisible      = TRUE;
    m_bFindInFiles  = TRUE;
    m_bPrint        = FALSE;
}

CKADFileExtInfo::~CKADFileExtInfo()
{
    if(m_pEditorGUID)
    {
        delete m_pEditorGUID;
        m_pEditorGUID = NULL;
    }
    if(m_pParserGUID)
    {
        delete m_pParserGUID;
        m_pParserGUID = NULL;
    }
}

//------------------------------------------------------------------*
/**
 * set extension and make it lower case
 *
 * @param           [in] strExt: extension string
*/
void CKADFileExtInfo::SetExtension(CString strExt)
{
    m_strExt = strExt;
    m_strExt.MakeLower();
}

//------------------------------------------------------------------*
/**
 * create GUID if necessary and copy it into member variable
 *
 * @param           [in] tGuid: GUID to be copied
*/
void CKADFileExtInfo::SetEditorGUID(GUID& tGuid)
{
    if(!m_pEditorGUID)
    {
        m_pEditorGUID = new GUID;
    }
    *m_pEditorGUID = tGuid;
}

//------------------------------------------------------------------*
/**
 * create GUID if necessary and copy it into member variable
 *
 * @param           [in] tGuid: GUID to be copied
*/
void CKADFileExtInfo::SetParserGUID(GUID& tGuid)
{
    if(!m_pParserGUID)
    {
        m_pParserGUID = new GUID;
    }
    *m_pParserGUID = tGuid;
}