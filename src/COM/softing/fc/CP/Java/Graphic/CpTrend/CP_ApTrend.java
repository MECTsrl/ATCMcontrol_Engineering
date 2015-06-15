import java.applet.*;
import java.awt.*;
import java.util.*;

//================================================================================
// an applet for a trend display
//================================================================================
public class CP_ApTrend extends Applet
{
	//============================================================
	// private data

	protected Dimension offDimension = null;
	protected Image     offImage     = null;
	protected Graphics  offGraphics  = null;

	CP_GrPTrend m_trendDisplay = new CP_GrPTrend();

	//============================================================
	// parameter names
	final static protected String m_PARAM_TRENDS               = "TRENDS";
	final static protected String m_PARAM_BGCOLOR              = "BGCOLOR";
	final static protected String m_PARAM_MARKERSIZE           = "MARKERSIZE";
	final static protected String m_PARAM_UNCERTAINCOLOR       = "UNCERTAINCOLOR";
	final static protected String m_PARAM_UNCERTAINMARKERSIZE  = "UNCERTAINMARKERSIZE";
	final static protected String m_PARAM_BADMARKERCOLOR       = "BADMARKERCOLOR";
	final static protected String m_PARAM_BADMARKERSIZE        = "BADMARKERSIZE";
	final static protected String m_PARAM_FGCOLOR              = "FGCOLOR";
	final static protected String m_PARAM_FONT                 = "FONT";

	private int m_updateCounter = 0;

	//============================================================
	// methods

	//=== constructor
	public CP_ApTrend()
	{
	}

	//=== applet info
	public String getAppletInfo()
	{
		return  "Name: CP_ApTrend\r\n" +
	  	        "Autor/Autorin: Frank Fischer\r\n" + 
				"Update: " + String.valueOf(m_updateCounter) + "\r\n" + 
				"Appletgröße: " + getSize().toString() + "\r\n" +
				"Displaygröße: " + m_trendDisplay.getSize().toString() + "\r\n" +
				"Version: 09";
	}

	//============================================================
	// parameter info
	public String[][] getParameterInfo()
	{
		String[][] info =
		{
			{ m_PARAM_TRENDS,			   "int",   "number of trends to use" },
			{ m_PARAM_BGCOLOR,             "Color", "background color of applet" },
			{ m_PARAM_UNCERTAINCOLOR,      "Color", "color for uncertain quality points and interpolations" },
			{ m_PARAM_UNCERTAINMARKERSIZE, "int",   "size of marker for bad points" },
			{ m_PARAM_BADMARKERCOLOR,      "Color", "color of marker for bad quality points" },
			{ m_PARAM_BADMARKERSIZE,       "int",   "size of marker for bad points" },
			{ m_PARAM_MARKERSIZE,          "int",   "size of point marker for all trends" },
			{ m_PARAM_FGCOLOR,             "Color", "foreground color of applet" },
			{ m_PARAM_FONT,                "Font", "font specification for scales" }
		};
		return info;
	}

	//============================================================
	// applet initialization
	public void init()
	{
		String parValue = null;

		parValue = getParameter(m_PARAM_TRENDS);
		int trends = 0;
		if ( parValue != null )
		{
			try
			{
				trends = Integer.parseInt(parValue);
			}
			catch(NumberFormatException e)
			{
			}
		}
		for(int i = 0; i < trends; i++)
		{
			addTrend(0, 1);
		}

		parValue = getParameter(m_PARAM_BGCOLOR);
		setBgColor(parValue);

		parValue = getParameter(m_PARAM_FGCOLOR);
		setFgColor(parValue);

		parValue = getParameter(m_PARAM_FONT);
		setFont(parValue);

		parValue = getParameter(m_PARAM_UNCERTAINCOLOR);
		setUncertainColor(parValue);

		parValue = getParameter(m_PARAM_UNCERTAINMARKERSIZE);
		if ( parValue != null )
		{
			int s = -1;
			try 
			{
				s = Integer.parseInt(parValue);
			}
			catch(NumberFormatException e)
			{
			}
			setUncertainMarkerSize(s);	
		}

		parValue = getParameter(m_PARAM_BADMARKERCOLOR);
		setBadMarkerColor(parValue);

		parValue = getParameter(m_PARAM_BADMARKERSIZE);
		if ( parValue != null )
		{
			int s = -1;
			try 
			{
				s = Integer.parseInt(parValue);
			}
			catch(NumberFormatException e)
			{
			}
			setBadMarkerSize(s);	
		}

		parValue = getParameter(m_PARAM_MARKERSIZE);
		if ( parValue != null )
		{
			int s = -1;
			try 
			{
				s = Integer.parseInt(parValue);
			}
			catch(NumberFormatException e)
			{
			}
			setMarkerSize(s);	
		}

//		setLayout(null);
		setLayout(new GridLayout());
		add(m_trendDisplay);
	}

	//============================================================
	// destroying applet
	public void destroy()
	{
		// ZU ERLEDIGEN: Platzieren Sie hier Bereinigungscode für das Applet 
	}

	//============================================================
	// starting the applet
	public void start()
	{
		// ZU ERLEDIGEN: Platzieren Sie hier zusätzlichen Code für den Start des Applets
	}
	
	//============================================================
	// stopping the applet
	public void stop()
	{
	}

	//============================================================
	// setting the applet background color
	public void setBgColor(String color)
	{
		if ( color != null )
		{
			Color c = CP_GrColor.getColor(color);
			setBackground(c);
			m_trendDisplay.setTrendBgColor(c);
		}
	}

	//============================================================
	// setting the applet foreground color
	public void setFgColor(String color)
	{
		if ( color != null )
		{
			Color c = CP_GrColor.getColor(color);
			setForeground(c);
			m_trendDisplay.setTrendFgColor(c);
		}
	}

	//============================================================
	// setting the trend display background color
	public void setTrendBgColor(String color)
	{
		if ( color != null )
		{
			Color c = CP_GrColor.getColor(color);
			m_trendDisplay.setTrendBgColor(c);
		}
	}

	//============================================================
	// setting the font
	public void setFont(String fontSpec)
	{
		Font f = null;

		if ( fontSpec != null )
			f = Font.getFont(fontSpec);

		m_trendDisplay.setFont(f);
		super.setFont(m_trendDisplay.getFont());
	}

	//============================================================
	// setting color for a marker
	public void setMarkerColor(int trend, String color)
	{
		try
		{
			if ( color != null )
			{
				Color c = CP_GrColor.getColor(color);
				m_trendDisplay.getTrendBuffer(trend).setMarkerColor(c);
			}
		}
		catch(ArrayIndexOutOfBoundsException e)
		{}
	}

	//============================================================
	// setting style for a marker
	public void setMarkerStyle(int trend, int style)
	{
		try
		{
			if ( 0 <= style && style <= 4 )
			m_trendDisplay.getTrendBuffer(trend).setMarkerStyle(style);
		}
		catch(ArrayIndexOutOfBoundsException e)
		{}
	}

	//============================================================
	// setting color for a line
	public void setLineColor(int trend, String color)
	{
		try
		{
			if ( color != null )
			{
				Color c = CP_GrColor.getColor(color);
				m_trendDisplay.getTrendBuffer(trend).setLineColor(c);
			}
		}
		catch(ArrayIndexOutOfBoundsException e)
		{}
	}

	//============================================================
	// setting the time window in milliseconds
	public void setTimeWindowMillis(long millis)
	{
		m_trendDisplay.setTimeWindowMillis(millis);
	}

	//============================================================
	// setting the grid color
	public void setGridColor(String color)
	{
		if ( color != null )
		{
			Color c = CP_GrColor.getColor(color);
			m_trendDisplay.setGridColor(c);
		}
	}
	
	//============================================================
	// show or hide grid
	public void showGrid(boolean b)
	{
		m_trendDisplay.showGrid(b);
	}

	//============================================================
	// setting the color for the uncertain quality marker
	public void setUncertainColor(String color)
	{
		if ( color != null )
		{
			Color c = CP_GrColor.getColor(color);
			m_trendDisplay.setUncertainColor(c);
		}
	}

	//============================================================
	// setting the color for the bad quality marker
	public void setBadMarkerColor(String color)
	{
		if ( color != null )
		{
			Color c = CP_GrColor.getColor(color);
			m_trendDisplay.setBadMarkerColor(c);
		}
	}

	//============================================================
	// setting interpolation style for points
	public void setInterpolationStyle(int trend, int style)
	{
		try
		{
			m_trendDisplay.getTrendBuffer(trend).setInterpolationStyle(style);
		}
		catch(ArrayIndexOutOfBoundsException e)
		{}
	}
	
	//============================================================
	// setting marker size
	public void setMarkerSize(int s)
	{
		if ( s < 1 )
			s = 3;
		m_trendDisplay.setMarkerSize(s);
	}

	//============================================================
	// setting uncertain quality marker size
	public void setUncertainMarkerSize(int s)
	{
		if ( s < 1 )
			s = 14;
		m_trendDisplay.setUncertainMarkerSize(s);
	}
		
	//============================================================
	// setting bad quality marker size
	public void setBadMarkerSize(int s)
	{
		if ( s < 1 )
			s = 14;
		m_trendDisplay.setBadMarkerSize(s);
	}
		
	//============================================================
	// adding a new trend
	protected int addTrend(double scaleMin, double scaleMax)
	{
		CP_TrendBuffer trendBuf = new CP_TrendBuffer(scaleMin, scaleMax);
		int i = m_trendDisplay.addTrendBuffer(trendBuf);
		return i;
	}

	//============================================================
	// adding a point to a trend
	public void addPointToTrend(
		int i,
		String timeStampStrg,
		double value,
		int quality
		)
	{
		long timeStamp = Long.parseLong(timeStampStrg);
		try
		{
			CP_TrendBuffer trendBuf = m_trendDisplay.getTrendBuffer(i);
			trendBuf.addPoint(timeStamp, value, quality);
		}
		catch(ArrayIndexOutOfBoundsException e)
		{
		}
		catch(NumberFormatException e)
		{
		}
	}

	//============================================================
	// setting quality
	public void setQuality(int q)
	{
		m_trendDisplay.setQuality(q);
	}
	
	//============================================================
	// setting scale minimum
	public void setTrendScaleMin(int i, double scaleMin)
	{
		try
		{
			CP_TrendBuffer trendBuf = m_trendDisplay.getTrendBuffer(i);
			trendBuf.setScaleMin(scaleMin);
		}
		catch(ArrayIndexOutOfBoundsException e)
		{
		}
	}

	//============================================================
	// setting scale maximum
	public void setTrendScaleMax(int i, double scaleMax)
	{
		try
		{
			CP_TrendBuffer trendBuf = m_trendDisplay.getTrendBuffer(i);
			trendBuf.setScaleMax(scaleMax);
		}
		catch(ArrayIndexOutOfBoundsException e)
		{
		}
	}

	//============================================================
	// overriding paint for double buffering
	public void paint(Graphics g)
	{
		update(g);
	}

	//============================================================
	// updating the trend with new current time
	public void advance(String t)
	{
		long tt = Long.parseLong(t);
		m_trendDisplay.setCurrentTime(tt);
		repaint();
	}

	//============================================================
	// overriding update for double buffering
	public void update(Graphics g)
	{
		m_updateCounter++;
		Dimension d = getSize();

		if ( (offGraphics == null) ||
			(d.width != offDimension.width) ||
			(d.height != offDimension.height)
			)
		{
			offDimension = d;
			offImage = createImage(d.width, d.height);
			offGraphics = offImage.getGraphics();
		}

		offGraphics.clearRect(0, 0, d.width, d.height);
		m_trendDisplay.setSize(d);
		m_trendDisplay.paint(offGraphics);
		g.drawImage(offImage, 0, 0, this);
	}
}
