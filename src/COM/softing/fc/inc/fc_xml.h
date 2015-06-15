/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_xml.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_xml.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_xml.h $
 *
 * =PROJECT				CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_xml
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *               $Revision: 1 $
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
 *  26.04.2001  JD      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __FC_XML_H_
#define __FC_XML_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#import <msxml.dll> raw_interfaces_only named_guids rename_namespace("MSXML_4C")
using namespace MSXML_4C;


HRESULT XML_LoadDocument(BSTR sFileName, IXMLDOMDocument **ppDocument);

HRESULT XML_GetFirstSubNode(BSTR sNodeName, IXMLDOMNode *pParentNode,
                            IXMLDOMNode **ppChildNode);

HRESULT XML_GetAttribute(BSTR sAttributeName, IXMLDOMNode *pNode,
	                     BSTR *psAttributeValue);

HRESULT XML_GetNextSiblingNode(IXMLDOMNode *pNode, BSTR sNodeName,
                               IXMLDOMNode **ppSiblingNode);



#endif 


/*
 *----------------------------------------------------------------------------*
 *  $History: fc_xml.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 3  *****************
 * User: Jd           Date: 29.05.01   Time: 16:54
 * Updated in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 2  *****************
 * User: Jd           Date: 26.04.01   Time: 14:58
 * Updated in $/4Control/COM/softing/fc/inc
 * Removed some memory leaks.
 *==
 *----------------------------------------------------------------------------*
*/
