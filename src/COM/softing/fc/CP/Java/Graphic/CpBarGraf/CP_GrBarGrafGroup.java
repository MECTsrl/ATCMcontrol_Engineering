/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrBarGrafGroup.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrBarGrafGroup.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrBarGrafGroup.java $
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
 * $History: CP_GrBarGrafGroup.java $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 1  *****************
 * User: Scd          Date: 19.03.01   Time: 14:24
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:12
 * Created in $/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 4  *****************
 * User: So           Date: 16.07.98   Time: 14:19
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 * andere Darstellung bzgl quality
 * 
 * *****************  Version 3  *****************
 * User: Ff           Date: 29.04.98   Time: 12:42
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 * 
 * *****************  Version 2  *****************
 * User: Ff           Date: 23.04.98   Time: 10:15
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 *==
 ******************************************************************************
H<<*/


import java.awt.* ;
import java.util.* ;

/**
	drawing of bargraf

*/
public abstract class CP_GrBarGrafGroup extends Component
{	
	//== property: physical range, origin 
	double m_yMin, m_yMax, m_yOrigin;

	//== property: background color, forground color positiv, negativ
	Color m_bgColor, m_fgColorPos, m_fgColorNeg;
	
	//== property: set point ( Sollwert );
	boolean m_SetPoint = false;
	Color m_fgColorSetPoint;
	Color m_bgColorSetPoint;

	/**
		constructor
		@param p_BarGrafN number of bargrafs in group
	*/
	CP_GrBarGrafGroup(int p_BarGrafN) 
	{
		setDefaults();
		m_BarGrafN = p_BarGrafN ;
		m_BarGrafList = new Vector();
		for (int ii=0;ii<=m_BarGrafN+1;ii++)
		{
			m_BarGrafList.addElement(null);
		}
	}

	/**
		set property ( physical ) Range
		@param p_yMin minimal value of bargraf
		@param p_yMac maxiaml value of bargraf
		@param p_yOrigin origin of bargraf
	*/
	void setRange(double p_yMin, double p_yMax, double p_yOrigin)
	{
		setMin(p_yMin);
		setMax(p_yMax);
		setOrigin(p_yOrigin);
	}	
	
	/**
		set default values of properties ( except area )
		property					default
		yMin						0.0
		yMax						1.0
		yOrigin						0.0
		BgColor						gray
		FgColorPos					green
		FgColorNeg					green
		FgColorSetValue				darkBlue
	*/
	void setDefaults() 
	{
		m_yMin = 0.0;
		m_yMax = 1.0;
		m_yOrigin = 0.0;
		m_bgColor = Color.lightGray;
		m_fgColorPos = Color.green;
		m_fgColorNeg = Color.green;
		m_fgColorSetPoint = Color.blue;
	}

	/**
		set minimal value of bargraf at position
		@p_position position of bar graf in group, 1 is leftmost bar graf
		@p_value minimal value in bar graf
	*/
	public void setMin(int p_position, double p_value)
	{
		if ( m_effectiveQuality.isBad() ) return;

		CP_GrBarGrafPainter barGraf = getBarGrafPainter(p_position);
		if ( barGraf != null ) {
			barGraf.setMin(p_value,getGraphics());
		}
		paintQuality(getGraphics());
	}

	/**
		set minimal value for all bargrafs
		@p_value minimal value in bar graf
	*/
	public void setMin(double p_value)
	{
		if ( m_effectiveQuality.isBad() ) return;
		m_yMin = p_value;
		int nn = m_BarGrafList.size();
		for (int ii=0;ii<nn;ii++) 
		{
			setMin(ii,p_value);
		}
		paintQuality(getGraphics());
	}

	/**
		set maximal value of bargraf at position
		@p_position position of bar graf in group, 1 is leftmost bar graf
		@p_value maximal value in bar graf
	*/
	public void setMax(int p_position, double p_value)
	{
		if ( m_effectiveQuality.isBad() ) return;
		CP_GrBarGrafPainter barGraf = getBarGrafPainter(p_position);
		if ( barGraf != null ) {
			barGraf.setMax(p_value, getGraphics());
		}		
		paintQuality(getGraphics());
	}

	/**
		set maximal value for all bargrafs
		@p_value maximal value in bar graf
	*/
	public void setMax(double p_value)
	{
		if ( m_effectiveQuality.isBad() ) return;
		m_yMax = p_value;
		int nn = m_BarGrafList.size();
		for (int ii=0;ii<nn;ii++) 
		{
			setMax(ii,p_value);
		}
		paintQuality(getGraphics());
	}

	/**
		set origin value of bargraf at position
		@p_position position of bar graf in group, 1 is leftmost bar graf
		@p_value origin value in bar graf
	*/
	public void setOrigin(int p_position, double p_value)
	{
		if ( m_effectiveQuality.isBad() ) return;
		CP_GrBarGrafPainter barGraf = getBarGrafPainter(p_position);
		if ( barGraf != null ) {
			barGraf.setOrigin(p_value,getGraphics());
		}
		paintQuality(getGraphics());
	}

	/**
		set origin value for all bargrafs
		@p_value minimal value in bar graf
	*/
	public void setOrigin(double p_value)
	{
		if ( m_effectiveQuality.isBad() ) return;
		m_yOrigin = p_value;
		int nn = m_BarGrafList.size();
		for (int ii=0;ii<nn;ii++) 
		{
			setOrigin(ii,p_value);
		}
		paintQuality(getGraphics());
	}

	/**
		set property BgColor for single bargraf at spec. position
		@paran p_position postion of bargraf, 1 is lefmost/upmost bargraf
		@param p_Color background color of bargraf
	*/
	void setBgColor(int p_position, Color p_Color) 
	{
		if ( m_effectiveQuality.isBad() ) return;
		CP_GrBarGrafPainter barGraf = getBarGrafPainter(p_position);
		if ( barGraf != null ) {
			barGraf.setBgColor(p_Color, getGraphics());
		}
		paintQuality(getGraphics());
	}

	/**
		set property BgColor for all bargrafs
		@param p_Color background color of bargraf
	*/
	void setBgColor(Color p_Color) 
	{
		if ( m_effectiveQuality.isBad() ) return;

		int nn = m_BarGrafList.size();
		for (int ii=0;ii<nn;ii++) 
		{
			setBgColor(ii,p_Color);
		}
		if ( p_Color != m_bgColor)
		{
			m_bgColor = p_Color;
			paint(getGraphics());
		}
	}

	/**
		set property FgColorPos for single bargraf at spec. position
		@paran p_position postion of bargraf, 1 is lefmost/upmost bargraf
		@param forground color of 'positiv' ( > yOrigin ) values
	*/
	void setFgColorPos(int p_position, Color p_Color) 
	{
		if ( m_effectiveQuality.isBad() ) return;
		CP_GrBarGrafPainter barGraf = getBarGrafPainter(p_position);
		if ( barGraf != null ) {
			barGraf.setFgColorPos(p_Color, getGraphics());
		}
		paintQuality(getGraphics());
	}

	/**
		set property FgColorPos
		@param forground color of 'positiv' ( > yOrigin ) values
	*/
	void setFgColorPos(Color p_Color) 
	{
		if ( m_effectiveQuality.isBad() ) return;
		m_fgColorPos = p_Color;
		int nn = m_BarGrafList.size();
		for (int ii=0;ii<nn;ii++) 
		{
			setFgColorPos(ii,p_Color);
		}
		paintQuality(getGraphics());
	}

	/**
		set property FgColorNeg for single bargraf at spec. position
		@paran p_position postion of bargraf, 1 is lefmost/upmost bargraf
		@param forground color of 'negativ' ( < yOrigin ) values
	*/
	void setFgColorNeg(int p_position, Color p_Color)
	{
		if ( m_effectiveQuality.isBad() ) return;
		CP_GrBarGrafPainter barGraf = getBarGrafPainter(p_position);
		if ( barGraf != null ) {
			barGraf.setFgColorNeg(p_Color,getGraphics());
		}
		paintQuality(getGraphics());
	}

	/**
		set property FgColorNeg
		@param forground color of 'negativ' ( < yOrigin ) values
	*/
	void setFgColorNeg(Color p_Color)
	{
		if ( m_effectiveQuality.isBad() ) return;
		if (p_Color == m_fgColorNeg) return;
		m_fgColorNeg = p_Color;
		int nn = m_BarGrafList.size();
		for (int ii=0;ii<nn;ii++) 
		{
			setFgColorNeg(ii,p_Color);
		}
		paintQuality(getGraphics());
	}

	/**
		set property BgColorSetPoint
		@param forground color of set point colums
	*/
	void setBgColorSetPoint(Color p_Color)
	{
		if ( m_effectiveQuality.isBad() ) return;
		m_bgColorSetPoint = p_Color;
		CP_GrBarGrafPainter barGraf = getBarGrafPainter(0);
		if ( barGraf != null ) {
			barGraf.setBgColor(p_Color, getGraphics());
		}
		barGraf = getBarGrafPainter(m_BarGrafN+1);
		if ( barGraf != null ) {
			barGraf.setBgColor(p_Color, getGraphics());
		}
		paintQuality(getGraphics());
	}

	/**
		set property FgColorSetPoint
		@param forground color of set point colums
	*/
	void setFgColorSetPoint(Color p_Color)
	{
		if ( m_effectiveQuality.isBad() ) return;
		m_fgColorSetPoint = p_Color;
		CP_GrBarGrafPainter barGraf = getBarGrafPainter(0);
		if ( barGraf != null ) {
			barGraf.setFgColorPos(p_Color, getGraphics());
			barGraf.setFgColorNeg(p_Color, getGraphics());
		}
		barGraf = getBarGrafPainter(m_BarGrafN+1);
		if ( barGraf != null ) {
			barGraf.setFgColorPos(p_Color, getGraphics());
			barGraf.setFgColorNeg(p_Color, getGraphics());
		}
		paintQuality(getGraphics());
	}

	/**
		Add a bargraf to the group at specified position.
		The bargraf heritates the actual properties.
		@param p_Position position (from left to right), 1 is leftmost position
		@return reference to a CP_BarGrafPainter object
	*/
	public abstract CP_GrBarGrafPainter addBarGraf(int p_position);

	/**
		add set point bar graf to group i.e.
		add two half width bar grafs for the set point at left and right
		of the other bargrafs. Only one set point per group is possible.
	*/
	public abstract void addSetPoint();

	/**
		update the value of given position
		@param p_position position of bar graf in group
		@p_value actual value of process variable
		(@return true if p_ProcessVariable is contained in group)
	*/
	public void setValue(int p_position,double p_Value)
	{
		if ( m_effectiveQuality.isBad() ) return;
		//== scan list to find process variable and update
		int nn = m_BarGrafList.size();
		Graphics g = getGraphics();
		CP_GrBarGrafPainter barGraf;

		//== don't allow invalid positions
		if ( p_position < 1 || p_position > m_BarGrafN 
			 || p_position > nn) return;

		barGraf = getBarGrafPainter(p_position);
		if (barGraf != null ) barGraf.setValue(p_Value,g);
		paintQuality(getGraphics());
	}

	public void setSetPointValue(double p_Value)
	{
		if ( m_effectiveQuality.isBad() ) return;
		Graphics g = getGraphics();
		CP_GrBarGrafPainter barGraf;
		barGraf=(CP_GrBarGrafPainter) m_BarGrafList.elementAt(0);
		if (barGraf != null) barGraf.setValue(p_Value,g);
		barGraf=(CP_GrBarGrafPainter) m_BarGrafList.elementAt(m_BarGrafN+1);
		if (barGraf != null) barGraf.setValue(p_Value,g);
		paintQuality(getGraphics());
	}

	/**
		set quality of bar graf, bar graf will be crossed out red if quality is not good 
		@param p_position position of bar graf in group
		@param p_quality 0 good, 1 uncertain, 2 bad
	*/
	public void setQuality(int p_position, int p_quality)
	{
		if ( m_quality.isBad() ) return;
		CP_GrBarGrafPainter barGraf = getBarGrafPainter(p_position);
		if (barGraf != null ) 
		{
			barGraf.setQuality(p_quality,getGraphics());
			int quality = getEffectiveQuality();
			if ( quality != m_effectiveQuality.get() )
			{
				m_effectiveQuality.set(quality);
				paint(getGraphics());
			}
			else
			{
				paintQuality(getGraphics());
			}
		}
	}


	/**
		set quality of bar graf group, i.e. min, max and origin value
		if quality is not good, no values can be displayed and ALL bar grafs are
		crossed out.
	*/
	public void setQuality(int p_quality)
	{
		if ( m_quality.get() != p_quality )
		{
			m_quality.set(p_quality);
			int quality = getEffectiveQuality();

			if ( quality != m_effectiveQuality.get() )
			{
				m_effectiveQuality.set(quality);
				paint(getGraphics());
			}
			else
			{
				paintQuality(getGraphics());
			}
		}
	}

	/**
		set quality of set point, bar graf will be crossed out red if quality is not good 
		@param p_quality 0 good, 1 uncertain, 2 bad
	*/
	public void setSetPointQuality(int p_quality)
	{
		if ( m_quality.isBad() ) return;
		CP_GrBarGrafPainter barGraf=(CP_GrBarGrafPainter) m_BarGrafList.elementAt(0);
		if (barGraf != null ) barGraf.setQuality(p_quality,getGraphics());
		barGraf=(CP_GrBarGrafPainter) m_BarGrafList.elementAt(m_BarGrafN+1);
		if (barGraf != null ) barGraf.setQuality(p_quality,getGraphics());
		paintQuality(getGraphics());
	}

	/**
		Compute the (pixel) area where single bargraf is painted.
		Each bargraf get 1/m_BarGrafN of the width of group.
		@param p_position position of bargraf in group (0 and m_BarGrafN+1
				are reserved for set points
		@return area in pixels where to paint bargraf
	*/
	protected abstract CP_GrRectAngle computeArea(int p_position);

	/**
		pass the actual properties to the CP_BarGrafPainter object
		@param p_BarGraf the CP_BarGrafPainter object
	*/
	protected void passProperties(CP_GrBarGrafPainter p_BarGraf)
	{
		Graphics g = getGraphics();
		p_BarGraf.setRange(m_yMin, m_yMax, m_yOrigin);
		p_BarGraf.setBgColor(m_bgColor,g);
		p_BarGraf.setFgColorPos(m_fgColorPos,g);
		p_BarGraf.setFgColorNeg(m_fgColorNeg,g);
	}
	
	/**
		pass the actual properties for set point to the CP_BarGrafPainter object
		@param p_BarGraf the CP_BarGrafPainter object
	*/
	protected void passSetPointProperties(CP_GrBarGrafPainter p_BarGraf)
	{
		Graphics g = getGraphics();
		p_BarGraf.setRange(m_yMin, m_yMax, m_yOrigin);
		p_BarGraf.setBgColor(m_bgColor,g);
	}

	protected CP_GrBarGrafPainter getBarGrafPainter(int p_position)
	{
		int nn = m_BarGrafList.size();
		
		//== don't allow invalid positions
		if ( p_position < 0 || p_position > m_BarGrafN + 1
			 || p_position > nn) return null;

		CP_GrBarGrafPainter barGraf=(CP_GrBarGrafPainter) m_BarGrafList.elementAt(p_position);
		return barGraf;
	}
	
	/**
		adds process variable and bargraf to internal list
		@param p_position of bar graf in list
		@param p_BarGraf bargraf associated with process variable
	*/
	protected void addToList(CP_GrBarGrafPainter p_BarGraf,int p_position)
	{
		m_BarGrafList.setElementAt(p_BarGraf,p_position);
	}
	
	public void update (Graphics g)
	{
		paint(g);
	}

	public void paint (Graphics g)
	{
		CP_GrBarGrafPainter barGraf;
		CP_GrRectAngle bgArea = new CP_GrRectAngle();
		int nn = m_BarGrafList.size();
		int ii;

		m_Dimension = getSize();
		
		//
		// paint background
		//
		g.setColor(m_bgColor);
		g.fillRect(0,0,m_Dimension.width,m_Dimension.height);

		//
		// paint bar grafs
		//
		for (ii=0;ii<nn;ii++)
		{
			bgArea = computeArea(ii);
			barGraf = (CP_GrBarGrafPainter) m_BarGrafList.elementAt(ii);
			if (barGraf !=null) {
				barGraf.setArea(bgArea);
				barGraf.draw(g);
			}
		}
		//
		// paint (effective) quality
		//
		paintQuality(g);

		m_Drawn = true;
	}

	public void paintQuality(Graphics g)
	{
		m_Dimension = getSize();
		//
		// paint (effective) quality
		//
		if (m_effectiveQuality.isBad() )
		{
			g.setColor(Color.lightGray);
			g.fillRect(0,0,m_Dimension.width, m_Dimension.height);
		}

		Rectangle rect = new Rectangle(m_Dimension);
		m_effectiveQuality.paint(g,rect);
	}

	public Dimension getMinimumSize() 
	{
		return new Dimension(5,5);
	}

	protected int getEffectiveQuality()
	{
		//
		// if scale quality is bad -> return bad
		//
		int quality = m_quality.get();
		if ( quality == CP_GrQuality.BAD)
			return quality;

		//
		// test qualities of bargrafpainters
		//
		CP_GrBarGrafPainter barGraf;
		int nn = m_BarGrafList.size();
		int ii;

		for (ii=0;ii<nn;ii++)
		{
			barGraf = (CP_GrBarGrafPainter) m_BarGrafList.elementAt(ii);
			if (barGraf !=null) {
				int bgPainterQuality=barGraf.getQuality();
				if ( bgPainterQuality == CP_GrQuality.BAD)
				{
					return CP_GrQuality.BAD;
				}
				else if ( bgPainterQuality == CP_GrQuality.UNCERTAIN)
				{
					quality = bgPainterQuality;
				}
			}
		}
		return quality;
	}
	
	//== number of bar grafs
	protected int m_BarGrafN;
	
	//== dimension of component
	protected Dimension m_Dimension = new Dimension();

	//== vector of bargrafs;
	protected Vector m_BarGrafList = new Vector();

	//== quality
	protected CP_GrQuality m_quality = new CP_GrQuality(CP_GrQuality.BAD);

	//== effective quality
	protected CP_GrQuality m_effectiveQuality = new CP_GrQuality(CP_GrQuality.BAD);

	//== 
	protected boolean m_Drawn = false;
}