import java.util.*;
import java.net.*;
import java.awt.*;

//
//
// CP_ConfiData
//
//
class CP_ConfiData 
{
	final static private Hashtable m_classNames = new Hashtable();
	final static private Hashtable m_icons      = new Hashtable();
	
	static public void addIcon(int type, String server, String file)
		throws MalformedURLException
	{
		URL iconURL = new URL("http", server, "/"+file);
		Toolkit tk = Toolkit.getDefaultToolkit();
		m_icons.put(new Integer(type), tk.getImage(iconURL));
	}

	private String m_name = null;
	private int    m_type = -1;
	private String m_className = null;
	private String m_server = null;
	private String m_visuFile = null;
	private String m_confiFile = null;

	private boolean m_isRoot = false;

	final private Vector m_childs = new Vector();

	public int countChilds()
	{
		return m_childs.size();
	}
	
	public void addChild(CP_ConfiData d)
	{
		m_childs.addElement(d);
	}

	public CP_ConfiData()
	{
		m_isRoot = true;
	}

	public CP_ConfiData(
		int type,
		String className,
		String name,
		String server,
		String visuFile,
		String confiFile
		)
	{
		m_name       = name;
		m_type       = type;
		m_className  = getClassName(className);
		
		m_server    = server;

		m_visuFile  = visuFile;
		m_confiFile = confiFile;
	}

	static private String getClassName(String className)
	{
		if ( ! m_classNames.containsKey(className) )
			m_classNames.put(className, className);

		return (String)(m_classNames.get(className));
	}

	public boolean isRoot()
	{
		return m_isRoot;
	}
	
	public Image getIcon()
	{
		return (Image)(m_icons.get(new Integer(m_type)));
	}
	
	public String getClassName()
	{
		return m_className;
	}

	public int getType()
	{
		return m_type;
	}

	public String getName()
	{
		return m_name;
	}

	public String getConfiFile()
	{
		return m_confiFile;
	}

	public String getVisuFile()
	{
		return m_visuFile;
	}

	public String getServer()
	{
		return m_server;
	}

	public CP_ConfiData getChild(int i)
	{
		return (CP_ConfiData)m_childs.elementAt(i);
	}
}
