//******************************************************************************
// CPStdOut.java:	Applet
//
//******************************************************************************
import java.applet.*;
import java.awt.*;
import java.util.*;


//==============================================================================
// Hauptklasse für Applet CPStdOut
//
//==============================================================================
public class CPApLed extends Applet
{
	//parameter names
	//--------------------------------------------------------------------------
	private final String PARAM_BgColor = "BgColor";
	private final String PARAM_Type = "Type";
	private final String PARAM_ColorId = "ColorId";

	CP_Led m_Led = null;
	// Constructor
	//--------------------------------------------------------------------------
	public CPApLed()
	{
	}

	// APPLET-INFO:
    //--------------------------------------------------------------------------
	public String getAppletInfo()
	{
		return "Name: CPApLed\r\n" +
		       "Autor/Autorin: Matthias Kramer\r\n" +
		       "Erstellt mit Microsoft Visual J++ Version 1.1";
	}

	// PARAMETER Description
    // CPApLed Parameter-Information:
    //  { "Name", "Typ", "Beschreibung" },
    //--------------------------------------------------------------------------
	public String[][] getParameterInfo()
	{
		String[][] info =
		{
			{ PARAM_BgColor, "String", "Background Color" },
			{ PARAM_Type, "String", "LED type" },
			{ PARAM_ColorId, "String", "Color id for LED color (red=0, yellow=1, green=2, blue=3)" },
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
		// BgColor: Background Color
		//----------------------------------------------------------------------
		String param = getParameter(PARAM_BgColor);
		Color BgColor = new Color(0xC0, 0xC0, 0xC0);
		if (param != null)
			BgColor = parseColorString(param);

		// Led type
		//----------------------------------------------------------------------
		param = getParameter(PARAM_Type);
		int LedType = CP_Led.RECT_SUNKEN;
		if (param != null)
		{			
			if(param.compareTo("ArrowNorthSunken") == 0)
			{
				LedType = CP_Led.ARROW_NORTH_SUNKEN;
			}
			else if(param.compareTo("ArrowSouthSunken") == 0)
			{
				LedType = CP_Led.ARROW_SOUTH_SUNKEN;
			}
			else if(param.compareTo("ArrowEastSunken")  == 0)
			{
				LedType = CP_Led.ARROW_EAST_SUNKEN;
			}
			else if(param.compareTo("ArrowWestSunken")  == 0)
			{
				LedType = CP_Led.ARROW_WEST_SUNKEN;
			}
			else
			{
				LedType = CP_Led.RECT_SUNKEN;
			}
		}
		// Led Color Id
		//----------------------------------------------------------------------
		param = getParameter(PARAM_ColorId);
		int ColorIndex = 0;
		if (param != null)
			ColorIndex = Integer.valueOf(param).intValue();

		// create LED
		m_Led = new CP_Led(BgColor, LedType);
		setBackground(BgColor);
		setLayout(null);
		add(m_Led);
		m_Led.setSize(getSize());
		m_Led.setLocation(0, 0);
		m_Led.init(ColorIndex);
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

	public void setValue(
		 boolean bValue,
		 int Quality)
	{
		m_Led.set(bValue, Quality);
	}

}
