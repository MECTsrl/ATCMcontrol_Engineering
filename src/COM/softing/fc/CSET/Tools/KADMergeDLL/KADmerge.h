/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL/KADmerge.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: KADmerge.h $
 *                       $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL/KADmerge.h $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                4CE
 *
 * =COMPONENT           CSET
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *           $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  01.03.2000  KT      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/

// KADmerge.h: interface for the CKADmerge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KADMERGE_H__DD62679F_1244_11D5_B90E_002018641833__INCLUDED_)
#define AFX_KADMERGE_H__DD62679F_1244_11D5_B90E_002018641833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

#include "FileLogger.h"

#include <string>
#include <map>
using namespace std;

#import <msxml.dll> raw_interfaces_only named_guids
using namespace MSXML;

class CKADmerge : public CFileLogger
{
public:
	CKADmerge();
    CKADmerge(string sFileName);
	virtual ~CKADmerge();
    HRESULT GetKADFilenames(string * sFilenames[], LONG &lLength, string sLogFile);
	HRESULT Merge(string sAddOnFileName, BOOL bOverwrite = TRUE, string sLogFile = "");
    HRESULT RemoveKADFile(string &sFilename, string sLogFile = "");
    HRESULT GetValueByPattern(string sElementPattern,
        string sAttributePattern,
        string & sValue,
        string sLogFile = "");
    HRESULT SetValueByPattern(string sElementPattern,
        string sAttributePattern,
        string sValue,
        string sLogFile = "");

private:
	string ExpandBackslash(string str);
    IXMLDOMDocument * m_pXMLKad;
    string m_sFileName;
    //map<string, string> m_PrimaryKeys;
    map<string, BOOL> m_Overwriteable;
    BOOL m_bIsDirty; // document has been changed

    HRESULT CopyNode(IXMLDOMNode ** pXMLDest, IXMLDOMNode ** pXMLSrc, BOOL bOverwrite, string sPath);
    HRESULT GetRootElement(IXMLDOMDocument * pXMLDoc, IXMLDOMElement ** pXMLElem);
    HRESULT GetRootElement(IXMLDOMDocument * pXMLDoc, IXMLDOMNode ** pXMLNode);
	HRESULT LoadXMLFile(string sFileName, IXMLDOMDocument ** pXMLDoc);
	HRESULT SaveXMLFile(string sFileName, IXMLDOMDocument * pXMLDoc);

    string GetPatternString(IXMLDOMNode * pXMLNode, BOOL bQuery);
    string GetName(IXMLDOMNode * pXMLNode);
    string GetValue(IXMLDOMNode * pXMLNode);
    HRESULT SetValue(IXMLDOMNode * pXMLNode, string sValue);

    BOOL HasNode(IXMLDOMNode * pXMLNode, string sPatternString, IXMLDOMNode ** pXMLTarget);
};

#endif // !defined(AFX_KADMERGE_H__DD62679F_1244_11D5_B90E_002018641833__INCLUDED_)

/*
 *----------------------------------------------------------------------------*
 *  $History: KADmerge.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/KADMergeDLL
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CSET/Tools/KADmergeDLL
 * 
 * *****************  Version 4  *****************
 * User: Kt           Date: 20.06.01   Time: 10:42
 * Updated in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * changed 'HRESULT CopyNode(...)' prototype;
 * changed methods 'GetPatternString()' and 'CopyNode()'
 * no "Primary Keys" anymore
 * 
 * *****************  Version 3  *****************
 * User: Kt           Date: 22.03.01   Time: 17:00
 * Updated in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * included 'Installed4CKernel()' and 'InstalledIECAddOn()'
 * 
 * *****************  Version 2  *****************
 * User: Kt           Date: 21.03.01   Time: 13:57
 * Updated in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * 
 * *****************  Version 1  *****************
 * User: Kt           Date: 15.03.01   Time: 14:09
 * Created in $/4Control/COM/softing/fc/CSET/KADmergeDLL
 * New Project
 * 
 *----------------------------------------------------------------------------*
*/
