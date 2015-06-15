/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrLimitsH.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrLimitsH.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrLimitsH.java $
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
 * $History: CP_GrLimitsH.java $
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
 * User: Scd          Date: 19.03.01   Time: 14:50
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:12
 * Created in $/fc/CP/Java/Graphic/CpBarGraf
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
	drawing horizontal limit markers of bargraf

*/
public class CP_GrLimitsH extends CP_GrLimits
{	
	//== top or bottom limits, top is default 
	public static final int TOP = 0;
	public static final int BOTTOM = 1;
		
	/**
		constructor
		@param p_limitSide, TOP or BOTTOM
	*/
	CP_GrLimitsH(int p_side) 
	{
		setDefaults();
		if ( p_side == BOTTOM ) 
			m_LimitSide = BOTTOM;
		else
			m_LimitSide = TOP;
	}

	/**
		fill background and draw all limit marks
		@param g Graphics object
		@param index index of mark in internal list ( not position )
		@param p_Area, area where to draw limit marks
	*/
	protected void drawOneMark(Graphics g, int index, CP_GrRectAngle p_Area )
	{
		boolean init,raised;
		double value;
		int x,y, nn, style, realStyle, quality;
		Color color, colorAlarm;

		nn=m_positionList.size();
		if (index >=0 && index < nn)
		{			
			//==draw only if initialized
			init = ((Boolean) m_StatusList.elementAt(index)).booleanValue();
			if ( ! init ) return;
	
			//==get properties from lists
			value = ((Double) m_ValueList.elementAt(index)).doubleValue();
			quality = getQualityByIndex(index);
			if (quality != 0)
			{
				color = m_bgColor;
				colorAlarm = m_bgColor;
				raised = false;
			}
			else
			{
				color = (Color) m_FgColorList.elementAt(index);
				colorAlarm = (Color) m_FgColorAlarmList.elementAt(index);
				raised = true;
			}
			style = ((Integer) m_StyleList.elementAt(index)).intValue();
			
			//== set color
			g.setColor(color);
			if (m_ControlledValueInit)
			{
				if ( isAlarm(value, m_ControlledValue, style) )
					g.setColor(colorAlarm);
			}

			//== compute y coordinate of mark
			x=physValToPixel(value);
				
			//== get real style of mark
			realStyle = getRealStyle(style);

			//draw mark
			CP_GrLimitMark limitMark = new CP_GrLimitMark(p_Area.h-2*m_PadLimitMarks,realStyle);

			if (m_LimitSide == BOTTOM)
				y = p_Area.y + m_PadLimitMarks;
			else
				y = p_Area.y + p_Area.h - m_PadLimitMarks;

			limitMark.fill3D(g,x,y,CP_GrPolygon.NORTH,raised);
		}
	}

	/**
		map a physical value to the height of the
		rectangle in painting area of bargraf
		@param p_value physical value
		@return y coordinate of pixel
	*/
	protected int physValToPixel(double p_value)
	{
		double value;
		int intValue;
		CP_GrRectAngle Area = computeArea();

		//== map value
		value = ((double) Area.w) * ( p_value - m_yMin) / 
			                        ( m_yMax - m_yMin )    + Area.x;

		//== round value
		intValue = (int) (value + 0.5); 

		//== restrict value to painting area
		if ( intValue < Area.x ) intValue = Area.x;
		if ( intValue > Area.x + Area.w ) intValue = Area.x + Area.w;

		return intValue;
	}

	/**
		Compute the (pixel) area where the limit marks are painted.
		@return area in pixels where to paint bargraf
	*/
	protected CP_GrRectAngle computeArea()
	{
		CP_GrRectAngle area = new CP_GrRectAngle();

		//== get size dynamically
		if ( m_Drawn ) m_Dimension = getSize();
		
		//== x coordinate
		area.x = 0;

		//== y coordinate
		area.y = 0;

		//== heigt
		area.h = m_Dimension.height;

		//== width
		area.w = m_Dimension.width;
		
		return area;
	}

	/**
		get the style of limitmark: UPPERLEFT, UPPERRIGHT, ...
		@param p_style UPPER or LOWER
	*/
	protected int getRealStyle(int p_style)
	{
		int style = CP_GrLimitMark.LOWERTOP;
		
		if ( m_LimitSide == TOP )
		{
			if (p_style == UPPER ) 
				style = CP_GrLimitMark.UPPERTOP;
			else
				style = CP_GrLimitMark.LOWERTOP;
		}
		else
		{
			if (p_style == UPPER ) 
				style = CP_GrLimitMark.UPPERBOTTOM;
			else
				style = CP_GrLimitMark.LOWERBOTTOM;
		}

		return style;
	}

	//== side, where limits are painted 
	protected int m_LimitSide = 0;

}