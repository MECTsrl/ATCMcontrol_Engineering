
//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "ExtParamDB.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CExtParamDefinition



//------------------------------------------------------------------*
CExtParamDefinition::CExtParamDefinition(long lReference)
  : m_dataType(octets),
    m_paramType(CExtParamDefinition::constantValue),
    m_lOffset(0),
    m_lDefaultValue(0),
    m_lMinValue(0),
    m_lMaxValue(0),
    m_lAreaStart(0),
    m_lAreaLength(0)
{
    m_lReference = lReference;
}



/////////////////////////////////////////////////////////////////////////////
// CExtParamDB


//------------------------------------------------------------------*
CExtParamDB::CExtParamDB()
{
}

//------------------------------------------------------------------*
CExtParamDB::~CExtParamDB()
{
    POSITION    pos;
    for (pos = m_mapParamDef.GetStartPosition(); pos != NULL; )
    {
        long lReference;;
        CExtParamDefinition *pItem = NULL;
        m_mapParamDef.GetNextAssoc(pos, lReference, pItem);
        if (pItem != NULL) {
            delete pItem;
        }
    }
    m_mapParamDef.RemoveAll();
}

//------------------------------------------------------------------*
BOOL
    CExtParamDB::GetDescription(long lReference, CString &strDescr)
{
    CExtParamDefinition *pItem = NULL;
    if (m_mapParamDef.Lookup(lReference, pItem)) {
        strDescr = pItem->GetDescription();
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------*
BOOL
    CExtParamDB::GetDefinition(long lReference, CExtParamDefinition *&pItem)
{
    CExtParamDefinition *pLookup = NULL;
    if (m_mapParamDef.Lookup(lReference, pLookup)) {
        pItem = pLookup;
        return TRUE;
    }
    pItem = NULL;
    return FALSE;
}

//------------------------------------------------------------------*
BOOL
    CExtParamDB::AddDefinition(CExtParamDefinition *pItem)
{
    if (pItem == NULL) {
        return FALSE;
    }
    long lReference = pItem->GetReference();
    CExtParamDefinition *pOld = NULL;
    BOOL bExists = m_mapParamDef.Lookup(lReference, pOld);
    if (pOld != NULL) {
        // clean it up before overwriting it!
        delete pOld;
    }

    m_mapParamDef.SetAt(lReference, pItem);
    return bExists;
}


