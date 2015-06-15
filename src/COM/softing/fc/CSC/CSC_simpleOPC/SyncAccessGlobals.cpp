/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/SyncAccessGlobals.cpp 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: SyncAccessGlobals.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC/SyncAccessGlobals.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CSC
 *
 * =COMPONENT           simple sync OPC client
 *
 * =CURRENT      $Date: 28.02.07 19:00 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  07.08.01  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "SyncAccessGlobals.h"

//----  Static Initializations:   ----------------------------------*


//------------------------------------------------------------------*
/**
 * SplitMachineName
 *  isolate a escaped machine name, return pure machine name with
 *  square brackets removed!
 *
 * @param            pszVarName     either a full path variable name or simple machine name
 * @return           
 * @exception        -
 * @see              
*/
HRESULT SplitMachineName(LPCTSTR pszVarName, CString &strPureMachineName, CString &strRemainder)
{
    CString strTmp;
    _ASSERTE(pszVarName != NULL);
    strTmp = pszVarName;
    strRemainder = pszVarName;

    LPTSTR  pszScan = strTmp.GetBuffer(strTmp.GetLength());

    // overread any leading whitespaces
    while ((*pszScan != EOS) && _istspace(*pszScan)) {
        pszScan++;
    }

    // must be the escape char!
    if (*pszScan != s_chMachineNameLeadIn && *pszScan != s_chAltMachineNameLeadIn)
    {
        return E_FAIL;
    }
    pszScan++;
    LPCTSTR pszStart = pszScan; // save ptr

    // now read on until ending escape character is found:
    while ((*pszScan != EOS)
            && (*pszScan != s_chMachineNameLeadOut)
            && (*pszScan != s_chAltMachineNameLeadOut)) {
        pszScan++;
    }
    if (*pszScan == EOS)
    {
        return E_FAIL;
    }
    *pszScan = EOS;

    strPureMachineName = pszStart;
    pszScan++;
    if (*pszScan != EOS) {
        if (*pszScan == _T('.'))
          pszScan++;
        strRemainder = pszScan;
    }
    
    return S_OK;
}

//------------------------------------------------------------------*
/**
 * DecorateConfigName
 *
 * @param            
 * @return           
 * @exception        -
 * @see              
*/
CString DecorateConfigName(LPCTSTR pszMachineName)
{
    CString strConfig;
    strConfig  = s_chMachineNameLeadIn;
    strConfig += pszMachineName;
    strConfig += s_chMachineNameLeadOut;
    return strConfig;
}

//------------------------------------------------------------------*
/**
 *  isolate configuration name from variable name
 *  @return     name of configuration which hosts configuration
*/
CString GetConfigName(LPCTSTR pszVarName, CString& strPureName)
{
    CString strConfig;
    _ASSERTE(pszVarName != NULL);

    // %TBD% in V1.0 expect config name to ALWAYS be surrounded by square brackets,
    // since there is no other way to map to machine name!!

    HRESULT hr = SplitMachineName(pszVarName, strConfig, strPureName);
    if (SUCCEEDED(hr))
    {
        // **** have a decorated machine name as config name !!!
        // construct config name with brackets!!
        strConfig = DecorateConfigName(strConfig);
        
    }
    
    return strConfig;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: SyncAccessGlobals.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSC/CSC_simpleOPC
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * 
 * *****************  Version 1  *****************
 * User: Ca           Date: 8.08.01    Time: 9:59
 * Created in $/4Control/COM/softing/fc/CSC/CSC_simpleOPC
 * copied from V1.3
 *==
 *----------------------------------------------------------------------------*
*/
