/* $Header: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/SysVarPool.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: SysVarPool.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/ONLINE/OnlLibServ/SysVarPool.cpp $
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

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "Security.h"
#include "opcItem.h"
#include "SysVar.h"
#include "SysVarPool.h"
#include "NSpaceHolder.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCriticalSection CSysVarPool::m_cs;


/////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CSysVarPool
/////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * constructor
 *
 * @param           void
 * @return          void
 * @exception       -
 */
CSysVarPool::CSysVarPool(LPCTSTR pszSimplePath, LPCTSTR pszOpcNode, poolType type, int iEnumerator /*= 0*/)
{
    CString strNode = pszOpcNode;
    strNode.MakeLower();
    setName(strNode);
    m_strSimplePath = pszSimplePath;
//    m_strSimplePath.MakeLower();
    m_poolType = type;
    m_iEnumerator = iEnumerator;
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @param           void
 * @return          void
 * @exception       -
*/
CSysVarPool::~CSysVarPool()
{
    ClearSysVar();
}




//------------------------------------------------------------------*
void
    CSysVarPool::ClearSysVar()
{
    for (POSITION pos = m_sysVarMap.GetStartPosition(); pos != NULL; )
    {
        CString strName;
        CSysVar *pVar = NULL;
        m_sysVarMap.GetNextAssoc(pos, strName, pVar);
        assert(pVar != NULL);
        removeLeaf(pVar);
        pVar->release();
    }
    m_sysVarMap.RemoveAll();
}

//------------------------------------------------------------------*
/**
 * AddSysVar
 *
 * @param           
 * @return          S_OK; 
 * @exception       -
*/
HRESULT
    CSysVarPool::AddSysVar(CSysVar *pVar)
{
    HRESULT hr;
    if (pVar == NULL) {
        return E_POINTER;
    }
    CSysVar *pVarTest = NULL;
    CString strName = pVar->getName();
    strName.MakeLower();
    if (m_sysVarMap.Lookup(strName, pVarTest)) {
        // already in map, refuse!
        return S_FALSE;
    }
    addLeaf(pVar);
    hr = pVar->Reread();
    if (FAILED(hr)) {
        removeLeaf(pVar);
        return hr;
    }
    pVar->addRef();
    m_sysVarMap.SetAt(strName, pVar);
    return S_OK;
}

//------------------------------------------------------------------*
HRESULT
    CSysVarPool::UpdateValue(LPCTSTR pszName, LPCTSTR pszNewValue) const
{
    CSysVar *pVar = NULL;
    CString strName = pszName;
    strName.MakeLower();
    if (m_sysVarMap.Lookup(strName, pVar)) {
        // found it, do update
        assert(pVar != NULL);
        return pVar->Update(pszNewValue);
    }
    return E_FAIL;
}

//------------------------------------------------------------------*
CSysVar *
    CSysVarPool::LookupSysVar(LPCTSTR pszName)
{
    CSysVar *pVar = NULL;
    CString strName = pszName;
    strName.MakeLower();
    if (m_sysVarMap.Lookup(strName, pVar)) {
        // found it, do update
        assert(pVar != NULL);
        pVar->addRef();
        return pVar;
    }
    return NULL;
}

//------------------------------------------------------------------*
HKEY
    CSysVarPool::OpenSysVarPath(BOOL bWritable)
{
    HKEY hkBase = OpenCommonBasepath(bWritable);
    if (hkBase == NULL) {
        return NULL;
    }

    CRegKey keyBase;
    CRegKey keySRT;
    CRegKey keyRet;

    CString strEnum;
    strEnum.Format(_T("%d"), m_iEnumerator);

    keyBase.Attach(hkBase);    
	switch (m_poolType) {
    case configGlobal:
        keyRet.Attach(keyBase.Detach());
        break;
    case srtGlobal:
        {
            if (bWritable) 
			{
                if (keySRT.Create(keyBase, g_csSoftRT) == ERROR_SUCCESS)
                {
					keyRet.Create(keySRT, strEnum);
				}
            } else {
                if (keySRT.Open(keyBase, g_csSoftRT, KEY_READ) == ERROR_SUCCESS)
				{
					keyRet.Open(keySRT, strEnum, KEY_READ);
				}
            }
        }
        break;
    case consoleGlobal:
        if (bWritable) {
            keyRet.Create(keyBase, g_csConsole);
        } else {
            keyRet.Open(keyBase, g_csConsole, KEY_READ);
        }
        break;
    default:
        assert(0);
        break;
    }
    
	if (!m_strSimplePath.IsEmpty()) 
	{
		if (keyRet.m_hKey == NULL)
		{
			return NULL;
		}

        CRegKey keyPath;
        if (bWritable) {
            keyPath.Create(keyRet, m_strSimplePath);
        } else {
            keyPath.Open(keyRet, m_strSimplePath, KEY_READ);
        }
        keyRet.Close();
        keyRet.Attach(keyPath.Detach());
    }

    return keyRet.Detach();
}

//------------------------------------------------------------------*
HKEY
    CSysVarPool::OpenCommonBasepath(BOOL bWritable)
{
    assert(g_StartParam != NULL);
    CRegKey keySW;
    CRegKey key4C;
    CRegKey keyApp;
    CRegKey keySysVar;
    CRegKey keyResult;
    LONG lRes;
    if (bWritable) {
        lRes = keySW.Open(HKEY_LOCAL_MACHINE, _T("Software"));
    } else {
        lRes = keySW.Open(HKEY_LOCAL_MACHINE, _T("Software"), KEY_READ);
    }
    if (lRes == ERROR_SUCCESS)
    {
        if (bWritable) {
            lRes = key4C.Create(keySW, g_cs4Control);
        } else {
            lRes = key4C.Open(keySW, g_cs4Control, KEY_READ);
        }
        if (lRes == ERROR_SUCCESS)
        {
            if (bWritable) {
                lRes = keyApp.Create(key4C, g_csApp);
            } else {
                lRes = keyApp.Open(key4C, g_csApp, KEY_READ);
            }
            if (lRes == ERROR_SUCCESS)
            {
                if (bWritable) {
                    lRes = keySysVar.Create(keyApp, g_csSysVar);
                } else {
                    lRes = keySysVar.Open(keyApp, g_csSysVar, KEY_READ);
                }
                if (lRes == ERROR_SUCCESS)
                {
                    if (bWritable) {
                        lRes = keyResult.Create(keySysVar, g_StartParam);
                    } else {
                        lRes = keyResult.Open(keySysVar, g_StartParam, KEY_READ);
                    }
                }
            }
        }
    }

    return keyResult.Detach();
}

/* ---------------------------------------------------------------------------- */
