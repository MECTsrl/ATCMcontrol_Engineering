//******************************************************************************
// CPStdOut.java:	Applet
//
//******************************************************************************
import java.applet.*;
import java.awt.*;
import java.util.*;
import netscape.javascript.*;

//==============================================================================
// Hauptklasse für Applet CPStdOut
//
//==============================================================================
public class CPStdOut extends Applet
{
	// applet parameter
	//--------------------------------------------------------------------------
	private Color m_BgColor = null;
	private Color m_BgColorValue = null;
	private Color m_LinkColor = null;

	//parameter names
	//--------------------------------------------------------------------------
	private final String PARAM_BgColor = "BackgroundColor";
	private final String PARAM_BgColorValue = "BackgroundTextColor";
	private final String PARAM_LinkColor = "HyperlinkColor";

	private String m_JSAppletRef;	
	
	// array of output lines
	//--------------------------------------------------------------------------
	private Vector m_LineList;

	private Dimension m_LineSize = null;
	boolean m_bAdjustAppletSizeDone = false;
	

	// Event handler
	//--------------------------------------------------------------------------
	String m_HandlerEventWriteValue = null; 

	// Constructor
	//--------------------------------------------------------------------------
	public CPStdOut()
	{
	}

	// APPLET-INFO-UNTERSTÜTZUNG:
	//		Die Methode getAppletInfo() gibt eine Zeichenfolge zurück, die Autor/Autorin,
	// Copyright-Datum oder verschiedene andere Informationen des Applets beschreibt
    //--------------------------------------------------------------------------
	public String getAppletInfo()
	{
		return "Name: CPStdOut\r\n" +
		       "Autor/Autorin: Matthias Kramer\r\n" +
		       "Erstellt mit Microsoft Visual J++ Version 1.1";
	}

	// PARAMETER-UNTERSTÜTZUNG
	//		Die Methode getParameterInfo() liefert ein Array von Zeichenfolgen zurück, die
	// die Parameter beschreibt, die dieses Applet versteht.
	//
    // CPStdOut Parameter-Information:
    //  { "Name", "Typ", "Beschreibung" },
    //--------------------------------------------------------------------------
	public String[][] getParameterInfo()
	{
		String[][] info =
		{
			{ PARAM_BgColor, "String", "Background Color" },
			{ PARAM_BgColorValue, "String", "Background Color of Value Field" },
			{ PARAM_LinkColor, "String", "Color for hyper linked variable names" },
		};
		return info;		
	}

	//Die Methode init() wird vom AWT aufgerufen, wenn ein Applet erstmals geladen oder
	//neu geladen wird. Überschreiben Sie diese Methode, um jede Initialisierung auszuführen,
	//die das Applet benötigt (z. B. die Initialisierung von Datenstrukturen, das Laden von Bildern oder
	//Schriftarten, das Erstellen von Rahmenfenstern, das Festlegen des Layout-Managers oder das Hinzufügen von
	//Komponenten der Benutzeroberfläche).
    //--------------------------------------------------------------------------
	public void init()
	{
		m_JSAppletRef = getParameter("ID");
		if ( m_JSAppletRef == null ) m_JSAppletRef = getParameter("NAME");
		
		// BgColor: Background Color
		//----------------------------------------------------------------------
		String param = getParameter(PARAM_BgColor);
		if (param != null)
			m_BgColor = parseColorString(param);
		else
			m_BgColor = new Color(0xC0, 0xC0, 0xC0);

		// BgColor: Background Color of Value Field
		//----------------------------------------------------------------------
		param = getParameter(PARAM_BgColorValue);
		if (param != null)
			m_BgColorValue = parseColorString(param);
		else
			m_BgColorValue = new Color(0xC0, 0xC0, 0xC0);

		// LinkColor: Color for var names which are hyperlinks
		//----------------------------------------------------------------------
		param = getParameter(PARAM_LinkColor);
		if (param != null)
			m_LinkColor = parseColorString(param);
		else
			m_LinkColor = new Color(0x0, 0x0, 0xff);


		m_LineList = new Vector(5, 5);
		m_LineList.removeAllElements();

		setBackground(m_BgColor);
		setLayout(null);
		
		m_bAdjustAppletSizeDone = false;		
		m_LineSize = null;		
		adjustLineSize();
//		System.out.print("StdOut::Init(): Done\n");		
	}

	private final Color parseColorString(String colorString)  
	{
		if(colorString != null && colorString.length()==7) 
		{
			int R = Integer.valueOf(colorString.substring(1,3),16).intValue();
			int G = Integer.valueOf(colorString.substring(3,5),16).intValue();
			int B = Integer.valueOf(colorString.substring(5,7),16).intValue();
			return new Color(R,G,B);
		}
		else return Color.lightGray;
	}

	public void Refresh() 
	{ 
	//	adjustSize(); 
	}

	public void Refresh2() 
	{ 
/*		
		for (int ii = 0; ii < m_LineList.size(); ii++)
		{
			CPStdOutLine LineOut = (CPStdOutLine)m_LineList.elementAt(ii);			
			LineOut.repaint();
		}	
*/		
	}

	private void adjustLineSize()
	{
		if (m_LineSize == null)
		{
			Dimension LineSize = getSize();
			if (LineSize.width > 0)
			{
				FontMetrics fm = getFontMetrics(getFont());
				LineSize.height = fm.getHeight() + 4;
				m_LineSize = LineSize;

//				System.out.print("adjustSize:AppletSize: Sz = " + getSize().toString() + "\n");																											
				for (int ii = 0; ii < m_LineList.size(); ii++)
				{
					CPStdOutLine LineOut = (CPStdOutLine)m_LineList.elementAt(ii);
					LineOut.setSize(m_LineSize);
					LineOut.setLocation(0, ii * (m_LineSize.height+4));		
//					System.out.print("adjustLineSize: Sz = " + LineOut.getSize().toString() + "Pos = " + LineOut.getLocation().toString() + "\n");																							
				}
			}
		}
	}
	private void adjustAppletSize()
	{
		if (m_LineSize != null && !m_bAdjustAppletSizeDone)
		{
			if ( m_JSAppletRef != null && getNumberOfLines() > 0)
			{
				JSObject window = null;
				JSObject applet = null;			
				try { window = JSObject.getWindow(this); }
				catch (JSException e) {};
				if (window != null)
				{
					applet = (JSObject)window.getMember(m_JSAppletRef);
					if (applet != null)
					{
						applet.setMember("height", new Integer((m_LineSize.height+4) * getNumberOfLines()));
						m_bAdjustAppletSizeDone = true;
						repaint();
//						System.out.print("adjustAppletSize: h = " + (m_LineSize.height+4) + "\n");																																		
					}
				}
			}
		}
	}
	

	public void AddLine(
		String VarPath,
		String VarType,
		String VarAttribute,
		int LedColor,
		int HideMask)
	{
		adjustLineSize();
		
		CPStdOutLine LineOut =
			new CPStdOutLine(HideMask, m_BgColor, m_BgColorValue, m_LinkColor);

		LineOut.init(LedColor, VarPath, VarType, VarAttribute);

		m_LineList.addElement(LineOut);		
		add(LineOut);

		if (m_LineSize != null)
		{
			LineOut.setSize(m_LineSize);
			LineOut.setLocation(0, (m_LineList.size()-1) * (m_LineSize.height+4));		
		}
		
		m_bAdjustAppletSizeDone = false;		
		adjustAppletSize();
	}

	public int getNumberOfLines()
	{
		return m_LineList.size();
	}

	
	public void SetValue(
		 int index, 
		 boolean bLedValue,
		 int LedQuality,
		 String Value, 
		 int Quality)
	{
		adjustLineSize();		
		adjustAppletSize();		
		if (index >= 0 && index < m_LineList.size())
		{
			CPStdOutLine LineOut = (CPStdOutLine)m_LineList.elementAt(index);
			LineOut.set(bLedValue, LedQuality, Value, Quality);
		}
	}

	public void SetValue(
		 int index, 
		 boolean bLedValue,
		 int LedQuality,
		 String Value, 
		 int Quality,
		 int LedColorIndex)
	{
		adjustLineSize();
		adjustAppletSize();		
		if (index >= 0 && index < m_LineList.size())
		{
			CPStdOutLine LineOut = (CPStdOutLine)m_LineList.elementAt(index);
			LineOut.set(bLedValue, LedQuality, Value, Quality, LedColorIndex);
		}
	}

	public void setEventWriteValue(String handler) 
	{ 
		if (handler != null && handler.length() > 0)
			m_HandlerEventWriteValue = handler;
		else
			m_HandlerEventWriteValue = null;			
	}

	public void fireEventWriteValue(String ValueName, String ValueType)
	{
		if (m_HandlerEventWriteValue != null)
		{
			try 
			{
				JSObject JavaScriptObject = (JSObject) JSObject.getWindow(this);
				try 
				{            
					JavaScriptObject.eval(m_HandlerEventWriteValue + "('" + ValueName + "', '" + ValueType + "')");
				}
				catch (Exception e) {}				
			}
			catch (Exception e) {}
		}
	}
}
