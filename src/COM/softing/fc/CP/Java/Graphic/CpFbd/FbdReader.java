/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdReader.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FbdReader.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdReader.java $
 *
 * =PROJECT             7VH102  ATCMControl
 *
 * =SWKE                CP
 *
 * =COMPONENT           graphic
 *
 * =CURRENT             $Date: 28.02.07 19:00 $
 *                      $Revision: 1 $
 *
 * =ENVIRONMENT         Win32:
 *                      MS Windows NT 4.0
 *                      MSVC++ V5.0 / MFC 4.2 / ATL 2.0 / DAO 3.5
 *                      MS Java 2.0
 *                      MS Visual SourceSafe 5.0
 *
 * =REFERENCES
 *==
 *----------------------------------------------------------------------------*
 *

 

 

 *==
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATIONS
 * $History: FbdReader.java $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 1  *****************
 * User: Scd          Date: 19.03.01   Time: 15:39
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 3  *****************
 * User: Mkr          Date: 14.07.99   Time: 18:22
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 2  *****************
 * User: Mkr          Date: 2.07.99    Time: 15:02
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 23.06.99   Time: 17:49
 * Created in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * Initial version
 *==
 ******************************************************************************
H<<*/

import java.awt.Point;
import java.net.*;

import com.ms.security.*;
import com.ms.xml.om.*;
import com.ms.xml.parser.*;
import com.ms.xml.util.*;

//================================================================================
class FbdReader
//================================================================================
{
//====================
// public part
//====================
  
//=== constructors ========================================
FbdReader()
{
	super();
}
  
//=== debug methods ========================================
public long getLoadTimeMillis() { return m_loadTimeMillis;     }
public long getEvalTimeMillis() { return m_evalTimeMillis;     }
public long getObjCounter()     { return m_objCounter;         }
  
//=== load an XML file ========================================
public void load(URL url)
throws FbdParseException
{
	startTimer();
	    
	m_doc     = new Document();
	m_docBase = url;
	    
	try { m_doc.load(url); }
	catch(ParseException e)      { parseExc(e); }
	catch(SecurityExceptionEx e) { parseExc(e); }
	catch(Exception e)           { parseExc(e); }
	    
	m_loadTimeMillis = stopTimer();
}
  
//=== evaluate a loaded XML document ==============================
public void eval(FbdPicture pic)
throws FbdParseException
{
	startTimer();
	    
	if ( m_doc == null ) parseExc("no document loaded");
	    
	try { evalFCPOU(m_doc.getRoot(), pic, m_docBase); }
	catch (FbdParseException e) { throw e; }
	    
	m_evalTimeMillis = stopTimer();
	    
	m_doc = null;
}
    
//====================
// private part
//====================
  
//=== xml document data
private Document m_doc;
private URL      m_docBase;
  
//=== error handling
boolean m_errorOccured = false;
String  m_errorMessage = "";
  
//=== debug info
private int  m_objCounter         = 0;
private long m_loadTimeMillis     = 0;
private long m_evalTimeMillis     = 0;
private long m_timer              = 0;

//=== timer methods
private long startTimer() { return (m_timer = System.currentTimeMillis()); }
private long stopTimer()  { return (System.currentTimeMillis() - m_timer); }
  
//==== FCPOU ==================================================
private void evalFCPOU(Element elem, FbdPicture pic, URL docBase)
throws FbdParseException
{
	if ( elem == null ) return; // empty file
	    
	if ( elem.getTagName() != Fbd_XML_4GR_Names.ELEM_FCPOU )
		parseExc("wrong root element " + elem.getTagName() + ", expected FCPOU");
	    
	int nChildren = elem.numElements();
	for (int i = 0; i < nChildren; i++)
	{
		Element child = elem.getChild(i);
		Name tag = child.getTagName();
	      
		if ( tag == null )
		continue;
		else if ( tag == Fbd_XML_4GR_Names.ELEM_BODY )
		evalBody(child, pic);
	}
return; 
}
  
//=== BODY ====================================================
private void evalBody(Element elem, FbdPicture pic)
throws FbdParseException
{
	int nChildren = elem.numElements();
	for (int i = 0; i < nChildren; i++)
	{
		Element child = elem.getChild(i);
		Name tag = child.getTagName();
		      
		if ( tag == null )
			continue;
		else if ( tag == Fbd_XML_4GR_Names.ELEM_ACTION )
			evalAction(child, pic);
		else if ( tag == Fbd_XML_4GR_Names.ELEM_ACTBLCK )
			evalActBlck(child, pic);
		else if ( tag == Fbd_XML_4GR_Names.ELEM_ALTDIV )
			evalAltSimDiv(child, pic, true);
		else if ( tag == Fbd_XML_4GR_Names.ELEM_COMMENT )
			evalComment(child, pic);  
		else if ( tag == Fbd_XML_4GR_Names.ELEM_LINE )
			evalLine(child, pic);
		else if ( tag == Fbd_XML_4GR_Names.ELEM_SIMDIV )
			evalAltSimDiv(child, pic, false);
		else if ( tag == Fbd_XML_4GR_Names.ELEM_STEP )
			evalStep(child, pic);
		else if ( tag == Fbd_XML_4GR_Names.ELEM_TRANS )
			evalTrans(child, pic);
		else if ( tag == Fbd_XML_4GR_Names.ELEM_FB )
			evalPinElement(child, pic, tag);
		else if ( tag == Fbd_XML_4GR_Names.ELEM_FNC )
			evalPinElement(child, pic, tag);		
		else 
			evalSimpleElement(child, pic, tag);
		m_objCounter++;
	}
	return; 
}
  
//==== STEP ==================================================
private void evalStep(Element elem, FbdPicture pic)
throws FbdParseException
{
	//=== get attributes
	String  attrName     = getOptAttr     (elem, Fbd_XML_4GR_Names.ATTR_NAME, "");
	boolean attrInitStep = getOptAttrBool (elem, Fbd_XML_4GR_Names.ATTR_INITSTEP, false);
	Point   attrOrigin   = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_ORIGIN);
	Point   attrSize     = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_SIZE);
	boolean attrComment  = getOptAttrBool (elem, Fbd_XML_4GR_Names.ATTR_COMMENT, false);
	int     attrId       = getOptAttrInt  (elem, Fbd_XML_4GR_Names.ATTR_ID, -1);
	    
	pic.addStep(
		attrOrigin.x, attrOrigin.y, 
		attrSize.x, attrSize.y, 
		attrName, attrId, attrComment, attrInitStep);
	    
	return; 
}
//==== FUNCTION BLOCK =======================================
private void evalPinElement(Element elem, FbdPicture pic, Name elemType)
throws FbdParseException
{
	//=== get attributes
	String  attrType     = getReqAttr     (elem, Fbd_XML_4GR_Names.ATTR_TYPENAME);	
	String  attrName     = getOptAttr     (elem, Fbd_XML_4GR_Names.ATTR_NAME, "");
	Point   attrOrigin   = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_ORIGIN);
	Point   attrSize     = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_SIZE);
	boolean attrComment  = getOptAttrBool (elem, Fbd_XML_4GR_Names.ATTR_COMMENT, false);
	    
	//=== get children
	int nChildren = elem.numElements();
	FbdPinList pinList = null;	
	for (int i = 0; i < nChildren; i++)
	{
		Element child = elem.getChild(i);
		Name tag = child.getTagName();
	      
		if ( tag == null )
			continue;
		else if ( tag == Fbd_XML_4GR_Names.ELEM_PINLIST )
			pinList = getPinList(child);
	}
	if (elemType == Fbd_XML_4GR_Names.ELEM_FB)
		pic.addFunctionBlock(attrOrigin.x, attrOrigin.y, attrSize.x, attrSize.y, attrName, attrComment, attrType, pinList);
	else if (elemType == Fbd_XML_4GR_Names.ELEM_FNC)	
		pic.addFunction(attrOrigin.x, attrOrigin.y, attrSize.x, attrSize.y, attrComment, attrType, pinList);	    
	return; 
}

//==== some simple elements  ==================================================
private void evalSimpleElement(Element elem, FbdPicture pic, Name tag)
throws FbdParseException
{
	//=== get attributes
	String  attrName     = getOptAttr     (elem, Fbd_XML_4GR_Names.ATTR_NAME, "");
	Point   attrOrigin   = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_ORIGIN);
	Point   attrSize     = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_SIZE);
	boolean attrComment  = getOptAttrBool (elem, Fbd_XML_4GR_Names.ATTR_COMMENT, false);
	
	if (tag != null)
	{
		if ( tag == Fbd_XML_4GR_Names.ELEM_VARREAD )
			pic.addVarRead(	attrOrigin.x, attrOrigin.y, attrSize.x, attrSize.y, attrName, attrComment);
		else if ( tag == Fbd_XML_4GR_Names.ELEM_VARWRITE )
			pic.addVarWrite(attrOrigin.x, attrOrigin.y, attrSize.x, attrSize.y, attrName, attrComment);
		else if ( tag == Fbd_XML_4GR_Names.ELEM_VARFEEDBACK )
			pic.addVarFeedback(attrOrigin.x, attrOrigin.y, attrSize.x, attrSize.y, attrName, attrComment);
		else if ( tag == Fbd_XML_4GR_Names.ELEM_JUMP )
			pic.addJump(attrOrigin.x, attrOrigin.y, attrSize.x, attrSize.y, attrName, attrComment);
		else if ( tag == Fbd_XML_4GR_Names.ELEM_RETURN )
			pic.addReturn(attrOrigin.x, attrOrigin.y, attrSize.x, attrSize.y, attrComment);
		else if ( tag == Fbd_XML_4GR_Names.ELEM_CONNECTOR )
			pic.addConnector(attrOrigin.x, attrOrigin.y, attrSize.x, attrSize.y, attrName, attrComment);
		else if ( tag == Fbd_XML_4GR_Names.ELEM_CONTINUATION )
			pic.addContinuation(attrOrigin.x, attrOrigin.y, attrSize.x, attrSize.y, attrName, attrComment);
		else if ( tag == Fbd_XML_4GR_Names.ELEM_LABEL )
			pic.addLabel(attrOrigin.x, attrOrigin.y, attrSize.x, attrSize.y, attrName, attrComment);
	}
	return; 
}

//==== TRANS ==================================================
private void evalTrans(Element elem, FbdPicture pic)
throws FbdParseException
{
	//=== get attributes
	String  attrName     = getOptAttr     (elem, Fbd_XML_4GR_Names.ATTR_NAME, "");
	Point   attrOrigin   = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_ORIGIN);
	Point   attrSize     = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_SIZE);
	boolean attrComment  = getOptAttrBool (elem, Fbd_XML_4GR_Names.ATTR_COMMENT, false);
	int     attrId       = getOptAttrInt  (elem, Fbd_XML_4GR_Names.ATTR_ID, -1);
	    
	pic.addTransition(
		attrOrigin.x, attrOrigin.y, 
		attrSize.x, attrSize.y, 
		attrName, attrId, attrComment);
	    
	return; 
}

//==== ALTDIV ==================================================
private void evalAltSimDiv(Element elem, FbdPicture pic, boolean isAlt)
throws FbdParseException
{
	//=== get attributes
	Point   attrOrigin   = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_ORIGIN);
	Point   attrSize     = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_SIZE);
	boolean attrComment  = getOptAttrBool (elem, Fbd_XML_4GR_Names.ATTR_COMMENT, false);
	int     attrId       = getOptAttrInt  (elem, Fbd_XML_4GR_Names.ATTR_ID, -1);
	    
	pic.addBranch(
		attrOrigin.x, attrOrigin.y, 
		attrSize.x,
		attrId, attrComment, isAlt);
    
return; 
}

//==== COMMENT ==================================================
private void evalComment(Element elem, FbdPicture pic)
throws FbdParseException
{
	//=== get attributes
	Point   attrOrigin   = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_ORIGIN);
	Point   attrSize     = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_SIZE);
	boolean attrComment  = getOptAttrBool (elem, Fbd_XML_4GR_Names.ATTR_COMMENT, false);
	int     attrId       = getOptAttrInt  (elem, Fbd_XML_4GR_Names.ATTR_ID, -1);

	int nChildren = elem.numElements();
	String  strText = new String("");		
	int count = 0;
	final String  strNewLine = new String("\n");			
	for (int i = 0; i < nChildren; i++)
	{
		Element child = elem.getChild(i);
		if (child.getTagName() == Fbd_XML_4GR_Names.ELEM_OBJCOMMENT)
		{
			if (count > 0)
				strText += strNewLine;
			strText += child.getText().trim();
			count++;
		}
	}

	pic.addComment(
		attrOrigin.x, attrOrigin.y, 
		attrSize.x, attrSize.y, 
		strText, attrId, attrComment);
   
	return; 
}

//============================================================
private FbdPinList getPinList(Element elem)	throws FbdParseException
{
	int nChildren = elem.numElements();
	FbdPinList pinList = new FbdPinList();
	    
	for (int i = 0; i < nChildren; i++)
	{
		Element child = elem.getChild(i);
		Name tag = child.getTagName();
		if ( tag == null )
			continue;
		else if ( tag == Fbd_XML_4GR_Names.ELEM_PIN )
			pinList.addPin(getPin(child));
	}
	return pinList;
}
  
//==== ACTION ==================================================
private void evalAction(Element elem, FbdPicture pic)
throws FbdParseException
{
	//=== get attributes
	String  attrName     = getOptAttr     (elem, Fbd_XML_4GR_Names.ATTR_NAME, "");
	Point   attrOrigin   = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_ORIGIN);
	Point   attrSize     = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_SIZE);
	boolean attrComment  = getOptAttrBool (elem, Fbd_XML_4GR_Names.ATTR_COMMENT, false);
	int     attrId       = getOptAttrInt  (elem, Fbd_XML_4GR_Names.ATTR_ID, -1);
	    
	pic.addAction(
		attrOrigin.x, attrOrigin.y, 
		attrSize.x, attrSize.y, 
		attrName, attrId, attrComment);
    
return; 
}

//==== ACTBLCK ==================================================
private void evalActBlck(Element elem, FbdPicture pic)
throws FbdParseException
{
	//=== get attributes
	String  attrName     = getOptAttr     (elem, Fbd_XML_4GR_Names.ATTR_NAME, "");
	Point   attrOrigin   = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_ORIGIN);
	Point   attrSize     = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_SIZE);
	boolean attrComment  = getOptAttrBool (elem, Fbd_XML_4GR_Names.ATTR_COMMENT, false);
	int     attrId       = getOptAttrInt  (elem, Fbd_XML_4GR_Names.ATTR_ID, -1);
	    
	//=== get children
	String qualType = "";
	String qualTime = "";
	int nChildren = elem.numElements();
	for (int i = 0; i < nChildren; i++)
	{
		Element child = elem.getChild(i);
		Name tag = child.getTagName();
			      
		if ( tag == null )
			continue;
		else if ( tag == Fbd_XML_4GR_Names.ELEM_ACTQUAL )
		{
			qualType = getOptAttr(child, Fbd_XML_4GR_Names.ATTR_QUALTYPE, "");
			qualTime = getOptAttr(child, Fbd_XML_4GR_Names.ATTR_QUALTIME, "");
		}
	} 
    
	pic.addActionBlock(
		attrOrigin.x, attrOrigin.y, 
		attrSize.x, attrSize.y, 
		attrName, attrId, attrComment, qualType);
	return; 
}

//==== LINE ==================================================
private void evalLine(Element elem, FbdPicture pic)
throws FbdParseException
{ 
	//=== get attributes
	Point   attrOrigin   = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_ORIGIN);
	Point   attrSize     = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_SIZE);
	boolean attrComment  = getOptAttrBool (elem, Fbd_XML_4GR_Names.ATTR_COMMENT, false);
	boolean attrRubberline = getOptAttrBool (elem, Fbd_XML_4GR_Names.ATTR_RUBBERLINE, false);
	
	pic.addLine(
		attrOrigin.x, attrOrigin.y, 
		attrSize.x, attrSize.y, 
		attrComment, attrRubberline);
	    
	return; 
}

//============================================================
private FbdPin getPin(Element elem)
throws FbdParseException
{
	String  attrName     = getOptAttr     (elem, Fbd_XML_4GR_Names.ATTR_NAME, "");
	Point   attrOrigin   = getReqAttrPoint(elem, Fbd_XML_4GR_Names.ATTR_ORIGIN);
	Name    strType      = Name.create(getReqAttr(elem, Fbd_XML_4GR_Names.ATTR_CLASS));
	
	int iType = -1;
	if (strType == Fbd_XML_4GR_Names.ATTR_VAL_INPUT)
		iType = FbdPin.FbdPinType_In;
	else if (strType == Fbd_XML_4GR_Names.ATTR_VAL_OUTPUT)
		iType = FbdPin.FbdPinType_Out;	
	else if (strType == Fbd_XML_4GR_Names.ATTR_VAL_INOUTPUT)
		iType = FbdPin.FbdPinType_InOut;	
	else if (strType == Fbd_XML_4GR_Names.ATTR_VAL_INEXT)
		iType = FbdPin.FbdPinType_In;	
	else if (strType == Fbd_XML_4GR_Names.ATTR_VAL_OUTEXT)
		iType = FbdPin.FbdPinType_Out;	
	else 
	{
		parseExc(getTxtInvalidPinType(strType.toString()));
	}

	return new FbdPin(attrOrigin.x, attrOrigin.y, iType, attrName);
	
}
  
//============================================================
private boolean getOptAttrBool(Element elem, Name attr, boolean bDefault)
	throws FbdParseException
{
	return getOptAttrInt(elem, attr, bDefault ? 1 : 0) == 0 ? false : true;
}

private int getOptAttrInt(Element elem, Name attr, int iDefault)
	throws FbdParseException
{
	String valStrg = getOptAttr(elem, attr, null);
	if ( valStrg == null )
		return iDefault;

	try { return Integer.parseInt(valStrg); }
	catch (NumberFormatException e)
	{
		parseExc(getTxtInvValElemAttr(valStrg, elem, attr));
	}
	return iDefault;
}
  
//============================================================
private String getTxtInvValElemAttr(String val, Element elem, Name attr)
{
	return getTxtInvVal(val) + " in " + getTxtElemAttr(elem,attr);
}
	private String getTxtMisElemAttr(Element elem, Name attr)
{
	return getTxtMisAttr(attr) + " in " + getTxtElemAttr(elem,attr);
}
private String getTxtElemAttr(Element elem, Name attr)
{
	return "attribute " + attr.toString() + " in element " + elem.getTagName();
}
private String getTxtInvVal(String val)
{
	return "invalid value \"" + val + "\"";
}
private String getTxtMisAttr(Name attr)
{
	return "missing attribute " + attr.toString();
}
private String getTxtInvalidPinType(String strType)
{
	return "invalid pin type: " + strType;
}
  
//============================================================
private Point getReqAttrPoint(Element elem, Name attr)
throws FbdParseException
{
	String val = getReqAttr(elem, attr);
	try
	{
		int splitIndex = val.indexOf(";");
		return  new Point(
			Integer.parseInt(val.substring(0, splitIndex).trim()),
			Integer.parseInt(val.substring(splitIndex+1).trim()));
	}
	catch(StringIndexOutOfBoundsException e)
	{
		parseExc(getTxtInvValElemAttr(val, elem, attr));
	}
	catch(NumberFormatException e)
	{
		parseExc(getTxtInvValElemAttr(val, elem, attr));
	}
	return null;
}
  
//============================================================
private String getReqAttr(Element elem, Name attr)
throws FbdParseException
{
	String val = getOptAttr(elem, attr, null);
	if ( val == null )
		parseExc(getTxtMisElemAttr(elem, attr));
	return val;
}
  
//============================================================
private String getOptAttr(Element elem, Name attr, String defaullt)
{
	Object val = null;
	try
	{
		val = elem.getAttribute(attr);
	}
	catch(Exception e)
	{
	}
	if ( val == null )
		return defaullt;
	return val.toString();
}

//============================================================
private void parseExc(Exception e)
throws FbdParseException
{
	parseExc(e.getMessage());
}
//============================================================
private void parseExc(String msg)
throws FbdParseException
{
	throw new FbdParseException(msg);
}
}