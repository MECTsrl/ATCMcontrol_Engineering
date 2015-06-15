/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrLimits.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrLimits.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrLimits.java $
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
 * $History: CP_GrLimits.java $
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
 * User: Scd          Date: 19.03.01   Time: 14:49
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 2  *****************
 * User: Ff           Date: 16.10.98   Time: 11:34
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:12
 * Created in $/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 4  *****************
 * User: So           Date: 17.07.98   Time: 11:48
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
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
	drawing limit markers of bargraf

*/ 
public abstract class CP_GrLimits extends Component
{
	//== upper or lower limit, lower is default
	public static final int LOWER = 0;
	public static final int UPPER = 1;

	//== property: physical range
	double m_yMin, m_yMax;

	//== property: background color, forground color
	Color m_bgColor, m_fgColor;

	//== property: forground color alarm
	Color m_fgColorAlarm;

	//== property: padLimitMarks, pad to x border in pixel
	int m_PadLimitMarks = 1;

	/**
		set property ( physical ) Range
		@param p_yMin minimal value of bargraf limits
		@param p_yMac maxiaml value of bargraf limits
	*/
	public void setRange(double p_yMin, double p_yMax)
	{
		m_yMin = p_yMin;
		m_yMax = p_yMax;
	}	
	
	/** 
		set minimal value (of range)
		@param p_yMin minimal value
	*/
	public void setMin(double p_yMin)
	{
		if ( m_effectiveQuality.isBad() ) return;
		if (m_yMin != p_yMin)
		{
			m_yMin = p_yMin;
			repaint();
		}
	}

	/** 
		set maximal value (of range)
		@param p_yMin minimal value
	*/
	public void setMax(double p_yMax)
	{
		if ( m_effectiveQuality.isBad() ) return;
		if (m_yMax != p_yMax  )
		{
			m_yMax = p_yMax;
			repaint();
		}
	}

	/**
		set property BgColor
		@param p_Color background color of bargraf
	*/
	public void setBgColor(Color p_Color) 
	{
		if ( m_effectiveQuality.isBad() ) return;
		if ( ! m_bgColor.equals(p_Color)  )
		{
			m_bgColor = p_Color;
			repaint();
		}
	}

	/**
		set property FgColorAlarm for all limit marks, added limit marks heritate this color
		@param forground color of limit marks at alarm state
	*/
	void setFgColorAlarm(Color p_Color) 
	{
		if ( m_effectiveQuality.isBad() ) return;
		
		m_fgColorAlarm = p_Color;
		for (int ii=0;ii<m_FgColorAlarmList.size();ii++)
		{
			Color color = getFgColorAlarmByIndex(ii);
			int quality = getQualityByIndex(ii);
			if ( ! color.equals(p_Color) && quality==0 ) 
			{
				m_FgColorAlarmList.setElementAt(p_Color, ii);
				drawOneMark(getGraphics(), ii, computeArea());
			}
		}
		drawQuality(getGraphics());
	}

	/**
		set property FgColorAlarm for limit mark at spec. position
		@param p_position position of limit mark
		@param p_Color forground color of limit marks at alarm state
	*/
	void setFgColorAlarm(int p_position, Color p_Color) 
	{
		if ( m_effectiveQuality.isBad() ) return;
		int index = findListIndex(p_position);
		if ( index>=0 ) {
			Color color = getFgColorAlarmByIndex(index);
			int quality = getQualityByIndex(index);
			if ( ! color.equals(p_Color) && quality==0 ) 
			{
				m_FgColorAlarmList.setElementAt(p_Color, index);
				drawOneMark(getGraphics(), index, computeArea());
			}
		}
		drawQuality(getGraphics());
	}

	/**
		set property FgColor for all limit marks, added limit marks heritate this color
		@param forground color of limit marks
	*/
	void setFgColor(Color p_Color) 
	{
		if ( m_effectiveQuality.isBad() ) return;
		m_fgColor = p_Color;
		for (int ii=0;ii<m_FgColorList.size();ii++)
		{
			Color color = getFgColorByIndex(ii);
			int quality = getQualityByIndex(ii);
			if ( ! color.equals(p_Color) && quality==0 ) 
			{
				m_FgColorList.setElementAt(p_Color, ii);
				drawOneMark(getGraphics(), ii, computeArea());
			}
		}
		drawQuality(getGraphics());
	}

	/**
		set property FgColor for limit mark at spec. position
		@param p_position position of limit mark
		@param p_Color forground color of limit mark
	*/
	void setFgColor(int p_position, Color p_Color) 
	{
		if ( m_effectiveQuality.isBad() ) return;
		int index = findListIndex(p_position);
		if ( index>=0 ) {
			Color color = getFgColorByIndex(index);
			int quality = getQualityByIndex(index);
			if ( ! color.equals(p_Color) && quality==0 ) 
			{
				m_FgColorList.setElementAt(p_Color, index);
				drawOneMark(getGraphics(),index,computeArea());
			}
		}
		drawQuality(getGraphics());
	}

	/**
		set default values of properties ( except area )
		property					default
		yMin						0.0
		yMax						1.0
		BgColor						gray
		FgColor						green
		FgColorAlarm				red
	*/
	void setDefaults() 
	{
		m_yMin = 0.0;
		m_yMax = 1.0;
		m_bgColor = Color.lightGray;
		m_fgColor = Color.green;
		m_fgColorAlarm = Color.red;
	}
	
	/**
		set the value of the controlled variable, if it is set
		alarm status is shown by different colors of limit marks
		@param p_value value of process variable
	*/
	public void setControlledValue(double p_value)
	{
		if ( m_effectiveQuality.isBad() ) return;

		if ( ! m_ControlledValueInit )
		{
			m_ControlledValueInit = true;
			m_ControlledValueOld = p_value; 
			m_ControlledValue = p_value;
			repaint();
			return;
		}
		
		if ( p_value == m_ControlledValue ) return ;
			
		m_ControlledValueOld = m_ControlledValue;
		m_ControlledValue = p_value;

		boolean alarmOld, alarmNew, init, found = false;
		int style;
		double value;
		Graphics g = getGraphics();
		CP_GrRectAngle area = computeArea();

		// search if colors have to be changed
		int nn=m_positionList.size();
		for (int ii=0;ii<nn;ii++)
		{			
			//==draw only if initialized
			init = ((Boolean) m_StatusList.elementAt(ii)).booleanValue();
			if ( ! init ) continue;
	
			//==get actual value from lists
			value = ((Double) m_ValueList.elementAt(ii)).doubleValue();
			style = ((Integer) m_StyleList.elementAt(ii)).intValue();
			
			//== test if alarm status has changed
			alarmOld = isAlarm(value,m_ControlledValueOld,style);
			alarmNew = isAlarm(value,m_ControlledValue,style);
			
			if ( alarmOld != alarmNew ) 
			{
				/*
				found = true;
				break;
				*/
				drawOneMark(g,ii,area);
			}
		}

		if (found) repaint();
		drawQuality(getGraphics());
	}

	/**
		Add a limit mark to the group at specified position.
		The limit mark heritates the actual properties.
		@param p_position position of limit mark
		@param p_type : LOWER or UPPER
	*/
	public void addLimit(int p_position, int p_type)
	{
		if ( p_type != UPPER) p_type = LOWER;
		addToList( p_position, m_fgColor, m_fgColorAlarm, p_type);
	}

	/**
		set quality for limit mark at spec. position
		@param p_position position of limit mark
		@param p_quality 0 good, 1 uncertain, 2 bad
	*/
	void setQuality(int p_position, int p_quality) 
	{
		if ( m_quality.isBad() ) return;
		int index = findListIndex(p_position);
		if ( index>=0 ) {
			int quality = getQualityByIndex(index);
			if ( quality != p_quality ) 
			{
				m_QualityList.setElementAt(new Integer(p_quality), index);
				m_effectiveQuality.set(getEffectiveQuality());
				paint(getGraphics());
			}
		}
	}

	/**
		set quality for limits as whole id scale min and max
		if quality is set to not good the quality of all limit marks
		changes to not good. if quality changes to good, the state is not 
		inherited to limit marks
		@param p_quality 0 good, 1 uncertain, 2 bad
	*/
	void setQuality(int p_quality) 
	{
		if ( m_quality.get() != p_quality )
		{
			m_quality.set(p_quality);
			m_effectiveQuality.set(getEffectiveQuality());
			paint(getGraphics());
		}
	}

	/**
		update the value of given process variable
		@param p_position position of limit mark
		@p_value actual value of process variable
		(@return true if p_ProcessVariable is contained in group)
	*/
	public void setValue(int p_position,double p_Value)
	{
		if ( m_effectiveQuality.isBad() ) return;
		//== scan list to find process variable and update
		int index;
		int position,quality;
		double value;
		boolean found = false, status;

		//== search in list for position
		index = findListIndex(p_position);
		if (index>=0) { 
			quality = getQualityByIndex(index);
			if (quality != 0) return;
			value = ((Double) m_ValueList.elementAt(index)).doubleValue();
			status = ((Boolean) m_StatusList.elementAt(index)).booleanValue();
			if (value != p_Value || (! status)) {
				found = true;
				//== set new value and status in list
				m_ValueList.setElementAt(new Double(p_Value),index);
				m_StatusList.setElementAt(new Boolean(true),index);
			}
		}

		//== if process variable found and changed draw all limit marks
		if (found)
		{
			/*
			double buffering does not work if component in framed panel
			Dimension d = size();
			if (m_offScreen == null || m_offScreenDimension != d)
			{
				m_offScreen = createImage(d.width,d.height);
				m_offScreenDimension.width = d.width;
				m_offScreenDimension.height = d.height;
			}
			
			Graphics g = m_offScreen.getGraphics();
			drawAllMarks(g);
			g.drawImage(m_offScreen,0,0,this);
			*/
			
			paint(getGraphics());
		}
	}

	/**
		fill background and draw all limit marks
		@param g Graphics object
	*/
	protected void drawAllMarks(Graphics g)
	{
		int nn;
		Color color;
		CP_GrRectAngle Area = computeArea();

		//fill background
		m_Dimension = getSize();
		g.setColor(m_bgColor);
		g.fillRect(0,0,m_Dimension.width,m_Dimension.height);

		// draw all marks
		nn=m_positionList.size();
		for (int ii=0;ii<nn;ii++)
		{
			drawOneMark(g,ii,Area);
		}
	}

	/**
		draw a red or black cross if total quality of values is not good
		@param g Graphics object
	*/
	protected void drawQuality(Graphics g)
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

	/**
		fill background and draw all limit marks
		@param g Graphics object
		@param index index of mark in internal list ( not position )
		@param p_Area, area where to draw limit marks
	*/
	protected abstract void drawOneMark(Graphics g, int index, CP_GrRectAngle p_Area );
	
	/**
		map a physical value to the height/width of the
		rectangle in painting area of bargraf
		@param p_value physical value
		@return y coordinate of pixel
	*/
	protected abstract int physValToPixel(double p_value);

	/**
		Compute the (pixel) area where the limit marks are painted.
		@return area in pixels where to paint bargraf
	*/
	protected abstract CP_GrRectAngle computeArea();

	/**
		adds process variable and properties  to internal list
		@param p_ProcessVariable name of process variable (positiv value)
		@param p_Color color of limit mark
		@param p_ColorAlarm alarm color of limit mark
		@param p_Style style of limit mark
	*/
	protected void addToList(int p_position, Color p_Color, Color p_ColorAlarm, int p_Style)
	{
		if (p_position < 1) return;

		m_positionList.addElement(new Integer(p_position));
		m_ValueList.addElement(new Double(0.0));
		m_FgColorList.addElement(p_Color);
		m_FgColorAlarmList.addElement(p_ColorAlarm);
		m_StyleList.addElement(new Integer(p_Style));
		m_StatusList.addElement(new Boolean(false));
		m_QualityList.addElement(new Integer(0));
	}

	/**
		find internal index of list, return negativ value if not found
		@param p_position position to be searched for
	*/
	protected int findListIndex(int p_position)
	{
		int nn = m_positionList.size();
		int retValue = -1;
		int position;
		
		for (int ii=0;ii<nn;ii++)
		{
			position = ( (Integer) m_positionList.elementAt(ii)).intValue();
			if (position==p_position) {
				retValue = ii;
				break;
			}
		}
		return retValue;
	}

	/**
		get fgColor from internal list
	*/
	protected Color getFgColorByIndex( int index )
	{
		return (Color) m_FgColorList.elementAt(index);
	}

	/**
		get fgColorAlarm from internal list
	*/
	protected Color getFgColorAlarmByIndex( int index )
	{
		return (Color) m_FgColorAlarmList.elementAt(index);
	}

	/**
		get quality from internal list
	*/
	protected int getQualityByIndex( int index )
	{
		return ((Integer) m_QualityList.elementAt(index)).intValue();
	}

	/**
		get status from internal list
	*/
	protected boolean getStatusByIndex( int index )
	{
		return ((Boolean) m_StatusList.elementAt(index)).booleanValue();
	}
	/**
		test for alarm status of value
		@p_limitValue value of limit mark
		@p_controlledValue value of controlled value
		@p_style LOWER of UPPER ( limit )
	*/
	protected boolean isAlarm(double p_limitValue, double p_controlledValue, int p_style)
	{
		if (p_controlledValue < p_limitValue && p_style == LOWER)
			return true;
		if (p_controlledValue > p_limitValue && p_style == UPPER )
			return true;
		
		return false;
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
		// test qualities of limit markers
		//
		CP_GrBarGrafPainter barGraf;
		int nn = m_positionList.size();
		int ii;

		for (ii=0;ii<nn;ii++)
		{
			int limitQuality=getQualityByIndex(ii);
			if ( limitQuality == CP_GrQuality.BAD)
			{
				return CP_GrQuality.BAD;
			}
			else if ( limitQuality == CP_GrQuality.UNCERTAIN)
			{
				quality = limitQuality;
			}
		}
		return quality;
	}

	public void update (Graphics g)
	{
		paint(g);
	}

	public void paint (Graphics g)
	{
		m_Drawn = true;
		drawAllMarks(g);
		drawQuality(g);
	}

	public Dimension getMinimumSize() 
	{
		return new Dimension(5,5);
	}
	

	//== process variable to be controlled
	protected boolean m_ControlledValueInit = false;
	protected double m_ControlledValue = 0.0;
	protected double m_ControlledValueOld = 0.0;

	//== dimension of component
	protected Dimension m_Dimension = new Dimension();

	//== vector of process variable names and related bargrafs
	protected Vector m_positionList = new Vector();
	protected Vector m_FgColorList = new Vector();
	protected Vector m_FgColorAlarmList = new Vector();
	protected Vector m_StyleList = new Vector();
	protected Vector m_ValueList = new Vector();
	protected Vector m_StatusList  = new Vector();
	protected Vector m_QualityList = new Vector();

	protected CP_GrQuality m_quality = new CP_GrQuality(CP_GrQuality.BAD);
	protected CP_GrQuality m_effectiveQuality = new CP_GrQuality(CP_GrQuality.BAD);

	//== flag = true if paint method previously invoked
	protected boolean	m_Drawn = false;

	//== image and image dimension for double buffering
	protected Image m_offScreen = null;
	protected Dimension m_offScreenDimension = new Dimension(0,0);
}