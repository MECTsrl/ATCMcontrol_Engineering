/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/XML_4gr_names.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: XML_4gr_names.h $
 *   $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/XML_4gr_names.h $
 *
 * =PROJECT             CAK1020    ATCMControl V2.0
 *
 * =SWKE                CE
 *
 * =COMPONENT           GREDITOR
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
 *    
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  20.02.01  HA/RW      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef XML_4GR_NAMES_H
#define XML_4GR_NAMES_H

//XML Element and Attribute names used in 4GR Files
//NOTE: all Elemet and Attribute names must be uppercase 
//to be compatible with msxml.dll version 4.
//Attribute values shall be all lower case.

//Element names:
#define XML_4GR_ELEM_HEADER         _T("HEADER")
#define XML_4GR_ELEM_BODY           _T("BODY")
#define XML_4GR_ELEM_EDITINFO       _T("EDITINFO")
#define XML_4GR_ELEM_LINE           _T("LINE")
#define XML_4GR_ELEM_VARREAD        _T("VARREAD")
#define XML_4GR_ELEM_VARWRITE       _T("VARWRITE")
#define XML_4GR_ELEM_VARFEEDBACK    _T("VARFEEDBACK")
#define XML_4GR_ELEM_PRO            _T("PRO")
#define XML_4GR_ELEM_FNC            _T("FNC")
#define XML_4GR_ELEM_FB             _T("FB")
#define XML_4GR_ELEM_PIN            _T("PIN")
#define XML_4GR_ELEM_PINLIST        _T("PINLIST")
#define XML_4GR_ELEM_LABEL          _T("LABEL")
#define XML_4GR_ELEM_JUMP           _T("JUMP")
#define XML_4GR_ELEM_RETURN         _T("RETURN")
#define XML_4GR_ELEM_CONNECTOR      _T("CONNECTOR") 
#define XML_4GR_ELEM_CONTINUATION   _T("CONTINUATION")
#define XML_4GR_ELEM_STEP           _T("STEP")			
#define XML_4GR_ELEM_TRANS          _T("TRANS")			
#define XML_4GR_ELEM_ALTDIV         _T("ALTDIV")			
#define XML_4GR_ELEM_SIMDIV         _T("SIMDIV")			
#define XML_4GR_ELEM_ACTION         _T("ACTION")
#define XML_4GR_ELEM_ACTBLCK        _T("ACTBLCK")
#define XML_4GR_ELEM_ACTQUAL        _T("ACTQUAL")
#define XML_4GR_ELEM_ACTBLCK        _T("ACTBLCK")
#define XML_4GR_ELEM_STACTION       _T("STACTION")
#define XML_4GR_ELEM_COMMENT        _T("COMMENT")
#define XML_4GR_ELEM_POWERRAIL      _T("POWERRAIL")
#define XML_4GR_ELEM_CONTACT        _T("CONTACT")
#define XML_4GR_ELEM_COIL           _T("COIL")


//Attribute names:
#define XML_4GR_ATTR_EDITNAME       _T("EDITNAME")
#define XML_4GR_ATTR_EDITVERSION    _T("EDITVERSION")
#define XML_4GR_ATTR_EDITCATID      _T("EDITCATID")
#define XML_4GR_ATTR_EDITCLASSID    _T("EDITCLASSID")
#define XML_4GR_ATTR_LANG           _T("LANG")
#define XML_4GR_ATTR_SFC_MENU       _T("SFC_MENU")
#define XML_4GR_ATTR_ORIGIN         _T("ORIGIN")
#define XML_4GR_ATTR_INORIGIN       _T("INORIGIN")
#define XML_4GR_ATTR_OUTORIGIN      _T("OUTORIGIN")
#define XML_4GR_ATTR_SIZE           _T("SIZE")
#define XML_4GR_ATTR_NAME           _T("NAME")
#define XML_4GR_ATTR_TYPE           _T("TYPE")
#define XML_4GR_ATTR_TYPENAME       _T("TYPENAME")
#define XML_4GR_ATTR_VERSIONID      _T("VERSIONID")
#define XML_4GR_ATTR_SELECTED       _T("SELECTED")
#define XML_4GR_ATTR_HIDEPINNAMES   _T("HIDEPINNAMES")
#define XML_4GR_ATTR_BORDER         _T("BORDER")
#define XML_4GR_ATTR_CLASS          _T("CLASS")
#define XML_4GR_ATTR_INITVAL        _T("INITVAL")
#define XML_4GR_ATTR_INITSTEP       _T("INITSTEP")
#define XML_4GR_ATTR_RUBBERLINE     _T("RUBBERLINE")
#define XML_4GR_ATTR_NEGATE         _T("NEGATE")
#define XML_4GR_ATTR_HIDDED         _T("HIDDED")  //please use XML_4GR_ATTR_HIDDEN instead!
#define XML_4GR_ATTR_HIDDEN         _T("HIDDEN")
#define XML_4GR_ATTR_CENTRE_X       _T("CENTRE_X")
#define XML_4GR_ATTR_CENTRE_Y       _T("CENTRE_Y")
#define XML_4GR_ATTR_DISABLE        _T("DISABLE")
#define XML_4GR_ATTR_R_EDGE         _T("R_EDGE")
#define XML_4GR_ATTR_F_EDGE         _T("F_EDGE")
#define XML_4GR_ATTR_QUALTYPE       _T("QUALTYPE")
#define XML_4GR_ATTR_QUALTIME       _T("QUALTIME")
#define XML_4GR_ATTR_COMMENT        _T("COMMENT")
#define XML_4GR_ATTR_OBJCOMMENT     _T("OBJCOMMENT")
#define XML_4GR_ATTR_FUNC           _T("FUNC")
	    

//Attribute Values of attribute CLASS
#define XML_4GR_ATTR_VAL_INPUT      _T("input")
#define XML_4GR_ATTR_VAL_OUTPUT     _T("output")
#define XML_4GR_ATTR_VAL_INOUTPUT   _T("inoutput")
#define XML_4GR_ATTR_VAL_INEXT      _T("inext")
#define XML_4GR_ATTR_VAL_OUTEXT     _T("outext")

//Attribute Values of attribute FUNC
#define XML_4GR_ATTR_VAL_NORMAL     _T("normal")
#define XML_4GR_ATTR_VAL_CLOSED     _T("closed")
#define XML_4GR_ATTR_VAL_NEGATIVE   _T("negative")
#define XML_4GR_ATTR_VAL_SET        _T("set")
#define XML_4GR_ATTR_VAL_RESET      _T("reset")
#define XML_4GR_ATTR_VAL_POSTRANS   _T("postrans")
#define XML_4GR_ATTR_VAL_NEGTRANS   _T("negtrans")

//Attribute Values of attribute LANG
#define XML_4GR_ATTR_VAL_FBD        _T("fbd")
#define XML_4GR_ATTR_VAL_LD         _T("ld")
#define XML_4GR_ATTR_VAL_FBD_SFC    _T("fbd_sfc")
#define XML_4GR_ATTR_VAL_LD_SFC     _T("ld_sfc")
#define XML_4GR_ATTR_VAL_ALL        _T("all")


//
//Elements & Attr speciffic for the clibboard format:
//
#define XML_FCCB_DTDTYPE            _T("FCCB")
#define XML_FCCB_DTDFILE            _T("FCCB.DTD")
//Header
#define XML_FCCB_CBHEADER           _T("CBHEADER")
//Attributes header
#define XML_FCCB_ATTR_CBTYPE        _T("CBTYPE")
#define XML_FCCB_ATTR_CBSOURCE      _T("CBSOURCE")
//Const
#define XML_FCCB_ELEM_CONST         _T("CONST")

/*
 *----------------------------------------------------------------------------*
 *  $History: XML_4gr_names.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Rw           Date: 23.02.01   Time: 11:28
 * Created in $/4Control/COM/softing/fc/inc
 * initial version for V2.0
 * 
 *==
 *----------------------------------------------------------------------------*
*/

#endif
