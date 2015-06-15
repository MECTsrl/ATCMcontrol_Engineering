//******************************************************************************
// CP_ApNavigator.java:	Applet
//
//******************************************************************************
import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.net.*;

//==============================================================================
// Hauptklasse für Applet CP_ApNavigator
//
//==============================================================================
public class CP_ApNavigator extends Applet implements CP_ConfiReaderProgress
{
	private CP_Image m_img;
	private Image m_gif;

	static public TextArea out;

	public void progress(int objects)
	{
		System.out.println(String.valueOf(objects) + " objects loaded");
	}

	// CP_ApNavigator Klassen-Konstruktor
	//--------------------------------------------------------------------------
	public CP_ApNavigator()
	{
	}

	// APPLET-INFO-UNTERSTÜTZUNG:
	//		Die Methode getAppletInfo() gibt eine Zeichenfolge zurück, die Autor/Autorin,
	// Copyright-Datum oder verschiedene andere Informationen des Applets beschreibt
    //--------------------------------------------------------------------------
	public String getAppletInfo()
	{
		return "Name: CP_ApNavigator\r\n" +
		       "Autor/Autorin: ATCM\r\n" +
		       "Erstellt mit Microsoft Visual J++ Version 1.1\r\n" +
		       "ATCM";
	}

	public void start()
	{
	}

	//Die Methode init() wird vom AWT aufgerufen, wenn ein Applet erstmals geladen oder
	//neu geladen wird. Überschreiben Sie diese Methode, um jede Initialisierung auszuführen,
	//die das Applet benötigt (z. B. die Initialisierung von Datenstrukturen, das Laden von Bildern oder
	//Schriftarten, das Erstellen von Rahmenfenstern, das Festlegen des Layout-Managers oder das Hinzufügen von
	//Komponenten der Benutzeroberfläche).
    //--------------------------------------------------------------------------
	public void init()
	{
		setLayout(new GridLayout(1, 1));
		Toolkit toolkit = Toolkit.getDefaultToolkit();

		int[] types = { 0, 1, 8, 16, 32, 64 };
		for(int i = 0; i < types.length; i++)
		{
			int t = types[i];
			try
			{
				CP_TreeNodeData.addIcon(
					t,
					"w-ff",
					"nav/CPNAV_type" + String.valueOf(t) + ".gif"
					);
			}
			catch(MalformedURLException e)
			{
				System.out.println(e.getMessage());
			}
		}
		try 
		{
			CP_Tree tree = new CP_Tree();
			CP_ConfiReader reader = new CP_ConfiReader(this, 100);
			reader.open(new URL("http://w-ff/nav/objects.xml"));
			reader.eval(tree);
			reader.close();

			add(tree);
			validate();
		}
		catch(Error e)
		{
			System.out.println(e.getMessage());
		}
		catch(Exception e)
		{
			System.out.println("EXCEPTION: " + e.getMessage());
		}
		catch(Throwable e)
		{
			System.out.println("THROWABLE: " + e.getMessage());
		}
	}

	// CP_ApNavigator Zeichnungsbehandlungsroutine
	//--------------------------------------------------------------------------
/*	public void paint(Graphics g)
	{
		g.drawString("Erstellt mit Microsoft Visual J++ Version 1.1", 10, 20);
	}
*/
}
