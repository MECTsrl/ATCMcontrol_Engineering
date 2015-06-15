/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdApplet.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FbdApplet.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdApplet.java $
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
 * $History: FbdApplet.java $
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
 * User: Scd          Date: 19.03.01   Time: 15:16
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 6  *****************
 * User: Mkr          Date: 14.07.99   Time: 18:22
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 5  *****************
 * User: Mkr          Date: 9.07.99    Time: 13:29
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 4  *****************
 * User: Mkr          Date: 6.07.99    Time: 19:10
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 3  *****************
 * User: Mkr          Date: 2.07.99    Time: 15:02
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 2  *****************
 * User: Mkr          Date: 29.06.99   Time: 15:43
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 *==
 ******************************************************************************
H<<*/

import java.applet.*;
import java.net.*;
import com.ms.xml.parser.*;
import java.awt.*;
import netscape.javascript.*;
import java.util.*;

//==============================================================================
public class FbdApplet extends Applet implements Runnable
{
	private String m_data;
	private boolean m_bOperate; // applet shows execute commands in context menues
	private String m_JSAppletRef;
	  
	private FbdPicture m_fbdPicture;
	private Thread m_loadThread;
	private FbdReader m_reader;
	  
	private String m_JSEventHandler;
	private JSObject m_JSWindow;

//*****************************************************************************	
	public String getAppletInfo()
//*****************************************************************************		
	{
		return 
			"Name: FbdApplet\r\n" +
			"Autor/Autorin: ATCM\r\n" +
			"ATCM";
	}

//*****************************************************************************	 
	public String[][] getParameterInfo()
//*****************************************************************************	 
	{
		String info[][] =
		{
			{"DATA",    "String","(REQ)The URL of the XML file containing the Fbd data."},
			{"HANDLER", "String","(OPT)The name of a javascript method, taking a debug message as parameter."},
			{"NAME",    "String","(OPT)The applet name. One of ID or NAME is required."},
			{"OPERATE", "String","(OPT) 1: The applet allows operate command"},			
			{"ID",      "String","(OPT)The applet id.   One of ID or NAME is required."}
		};
		return info;
	}
	  
//*****************************************************************************	 
	public void init()
//*****************************************************************************	 
	{
		m_data = null;
		m_bOperate = false;
		m_JSAppletRef = null;
		  
		m_loadThread = null;
		m_reader = null;
		  
		m_JSEventHandler = null;
		m_JSWindow = null;
		
		evalParams();
		try { m_JSWindow = JSObject.getWindow(this); }
		catch (JSException e) {};
	                           
		if ( m_JSWindow == null )
			m_JSEventHandler = null;
	    
		setLayout(null);
		m_fbdPicture = new FbdPicture(m_bOperate);		
		add(m_fbdPicture);
	}
	  
//*****************************************************************************	 
	public void start()
//*****************************************************************************	 
	{
		if ( m_loadThread != null && m_loadThread.isAlive() )
			return;
		     
		m_loadThread = new Thread(this);
		m_loadThread.start();

		return;
	}
	
//*****************************************************************************	 
	public void run()
//*****************************************************************************	 
	{
		URL url;
		URL base = getDocumentBase();
		try {url  = ( base == null ? new URL(m_data) : new URL(base, m_data) ); }
		catch (MalformedURLException e) { error(e); return; }
		
		m_reader = new FbdReader();		
		try { m_reader.load(url); }
		catch(FbdParseException e) { error(e); }
	
		try { m_reader.eval(m_fbdPicture); }
		catch (FbdParseException e) { error(e); }
		validate();
		setAppletSize(100);
		m_reader = null;
	}

//*****************************************************************************	 
	public void setAppletSize(int iZoomFactor)
//*****************************************************************************	 
	{
		if ( m_JSAppletRef == null )
			return;
		    
		m_fbdPicture.setZoomFactor(iZoomFactor);
		int w = m_fbdPicture.getSize().width;
		int h = m_fbdPicture.getSize().height;

		JSObject applet = (JSObject)m_JSWindow.getMember(m_JSAppletRef);
		if ( w > 0 ) applet.setMember("width",  new Integer(w));
		if ( h > 0 ) applet.setMember("height", new Integer(h));
	}

//*****************************************************************************	 
	public void sendCommand(String cmd, String elemList, String param) 
//*****************************************************************************	 
	{
		createEvent(cmd, elemList, param);
	}

//*****************************************************************************	 
	public String joinIdLists(
		String strList1, 
		String strList2, 
		String command) throws FbdUpdateException
//*****************************************************************************	 																					   
	{
		boolean bAdd = command.equals("add");
		
		if (strList2 == null || strList2.length() == 0)
			return strList1;

		if (strList1 == null || strList1.length() == 0)
		{
			if (bAdd)				
				return strList2;
			else
				return strList1;					
		}

		// parse id lists
		StringTokenizer tokenList = new StringTokenizer(strList1, ";");
		int ids1[] = new int [tokenList.countTokens()];
		for (int ii1 = 0; ii1 < ids1.length; ii1++)
		{
			try { ids1[ii1] = Integer.parseInt(tokenList.nextToken()); }
			catch (NumberFormatException e){ throw new FbdUpdateException(strList1); }
		}
		tokenList = new StringTokenizer(strList2, ";");
		int ids2[] = new int [tokenList.countTokens()];
		for (int ii2 = 0; ii2 < ids2.length; ii2++)
		{
			try { ids2[ii2] = Integer.parseInt(tokenList.nextToken()); }
			catch (NumberFormatException e){ throw new FbdUpdateException(strList2); }
		}
		
		// update lists
		for (int ii2 = 0; ii2 < ids2.length; ii2++)
		{
			if (ids2[ii2] >= 0)
			{
				for (int ii1 = 0; ii1 < ids1.length; ii1++)
				{
					if (ids2[ii2] == ids1[ii1])
					{
						if (bAdd)
							ids2[ii2] = -1;	 // id already in list -> delete from list2
						else							
							ids1[ii1] = -1;  // id found -> delete from list1
						break;
					}
				}
			}
		}

		// create new joined list
		String strJoinList;
		if (bAdd)
		{
			// add remaining list2 ids to original list
			strJoinList = new String(strList1);
			for (int ii2 = 0; ii2 < ids2.length; ii2++)
			{
				if (ids2[ii2] >= 0)
					strJoinList += ";" + ids2[ii2];
			}
		}
		else
		{
			// create new list and add remaining list1 ids			
			strJoinList = new String("");
			boolean bFirst = true;			
			for (int ii1 = 0; ii1 < ids1.length; ii1++)
			{
				if (ids1[ii1] >= 0)
				{
					if (bFirst)
						strJoinList += ids1[ii1];
					else
						strJoinList += ";" + ids1[ii1];
					bFirst = false;
				}
			}
		}
		return strJoinList;		
	}
					   
//*****************************************************************************	 
	public void update(
		int iQuality,					   
		boolean bAutomatic,
		String strActiveElements,
		String strForcedTransitions,
		String strBlockedTransitions,
		String strForcedActions,
		String strBlockedActions) 		
//*****************************************************************************	 
	{
		if (m_loadThread == null || !m_loadThread.isAlive())
		{
			try 
			{
				m_fbdPicture.update(
					iQuality,					   									
					bAutomatic,
					strActiveElements,
					strForcedTransitions,
					strBlockedTransitions,
					strForcedActions,
					strBlockedActions);
			}
			catch (FbdUpdateException e) { error(e); }
		}
	}
 	  
	  
//*****************************************************************************	 
	private void error(String msg)
//*****************************************************************************	 
	{
		createEvent("ERROR", msg);		
	}

//*****************************************************************************	 
	private void error(Throwable t)
//*****************************************************************************	 
	{
		createEvent("ERROR", t.toString());				
	}
	  
//*****************************************************************************	 
	private void evalParams()
//*****************************************************************************	 
	{
		String param;
		    
		m_JSEventHandler = getParameter("HANDLER");
		m_bOperate = getParameter("OPERATE").equals("1");		

		if ( (m_data = getParameter("DATA")) == null )error("Missing applet parameter DATA");
		    
		m_JSAppletRef = getParameter("ID");
		if ( m_JSAppletRef == null ) m_JSAppletRef = getParameter("NAME");
		if ( m_JSAppletRef == null ) error("you must specify applet parameter ID or NAME");
	}
	  
	//=== call the JavaScript event handler

//*****************************************************************************	 
	private void createEvent(String event, String param1) { createEvent(event, param1, null); }
	private void createEvent(String event, String param1, String param2)
//*****************************************************************************	 
	{
		//=== ensure that the event handler is available
		if ( m_JSWindow == null || m_JSEventHandler == null )
			return;
		
		if (event == null)
			return;
		
		String expr = event;
		if ( param1 != null ) 
			expr += "', '" + param1;
		else
			expr += "', '" + "";
		
		if ( param2 != null ) 
			expr += "', '" + param2;
		else
			expr += "', '" + "";
			
		expr = m_JSEventHandler + "('" + expr + "')";
		    
		try { m_JSWindow.eval(expr); }
		catch(JSException e) { m_JSEventHandler = null; } //=== ignore, but disable events
		catch(Exception e)   { m_JSEventHandler = null; } //=== ignore, but disable events
		catch(Throwable t)   { m_JSEventHandler = null; } 
	}
}