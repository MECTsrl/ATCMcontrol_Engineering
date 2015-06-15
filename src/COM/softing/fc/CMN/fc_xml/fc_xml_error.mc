;/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_xml/fc_xml_error.mc 1     28.02.07 19:00 Ln $
; *----------------------------------------------------------------------------*
; *
; * =FILENAME            $Workfile: fc_xml_error.mc $
; *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_xml/fc_xml_error.mc $
; *
; * =PROJECT				CAK1020  ATCMControl V2.0
; *
; * =SWKE                CE
; *
; * =COMPONENT           FC_XML
; *
; * =CURRENT      $Date: 28.02.07 19:00 $
; *               $Revision: 1 $
; *
; * =REFERENCES
; *
; *----------------------------------------------------------------------------*
; *
;
; 
;
;
;
; *==
; *----------------------------------------------------------------------------*
; * =DESCRIPTION
; *    @DESCRIPTION@
; *==
; *----------------------------------------------------------------------------*
; * =MODIFICATION
; *  15.02.2001  SCD      File created
; *  see history at end of file !
; *==
; *******************************************************************************
;H<<*/

;//standard setting for severity:
SeverityNames=(
               Success=0x0
               Informational=0x1
               Warning=0x2
               Error=0x3
              )

;//predefined system wide facility codes (do not change!). You may
;//insert additional facility codes between the main areas:
FacilityNames=(
		XML = 0x2F3 : E_FACILITY_XML	
              )



;//currently supported languages:
LanguageNames=(
               English=0x409:FcXmlMessagesENG
               German=0x407:FcXmlMessagesDEU
              )

MessageIdTypedef=HRESULT
OutputBase=16



;//---------------
MessageId= 0
Severity = Error
Facility = XML
SymbolicName = E_LOAD_XML_DOCUMENT_FAILED
Language=English
FC_XML: Failed to load XML document
.
Language=German
FC_XML: Laden des XML-Dokuments fehlgeschlagen 
.

;//---------------
MessageId= 1
Severity = Error
Facility = XML
SymbolicName = E_XML_EXCEPTION_OCCURED
Language=English
FC_XML: An exception occurred while accessing XML data.
.
Language=German
FC_XML: Es ist eine Exception beim bearbeiten der XML Daten aufgetreten.
.

;//---------------
MessageId= 2
Severity = Error
Facility = XML
SymbolicName = E_MISSING_PARENT_XML_NODE
Language=English
FC_XML: There is no XML parent node.
.
Language=German
FC_XML: Es existiert kein XML-Vater-Knoten.
.

;//---------------
MessageId= 3
Severity = Success
Facility = XML
SymbolicName = S_XML_NODE_NOT_FOUND
Language=English
FC_XML: A node was not found.
.
Language=German
FC_XML: Ein Knoten wurde nicht gefunden.
.

;/*
; *----------------------------------------------------------------------------*
; *  $History: fc_xml_error.mc $
; * 
; * *****************  Version 1  *****************
; * User: Ln           Date: 28.02.07   Time: 19:00
; * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_xml
; * 
; * *****************  Version 1  *****************
; * User: Ln           Date: 28.08.03   Time: 16:33
; * Created in $/4Control/COM/softing/fc/CMN/fc_xml
; * 
; * *****************  Version 3  *****************
; * User: Scd          Date: 29.03.01   Time: 8:33
; * Updated in $/4Control/COM/softing/fc/CMN/fc_xml
; * 
; * *****************  Version 2  *****************
; * User: Scd          Date: 16.02.01   Time: 9:35
; * Updated in $/4Control/COM/softing/fc/CMN/fc_xml
; * 
; * *****************  Version 1  *****************
; * User: Scd          Date: 15.02.01   Time: 12:58
; * Created in $/4Control/COM/softing/fc/CMN/fc_xml
; *----------------------------------------------------------------------------*
;*/
