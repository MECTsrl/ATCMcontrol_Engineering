/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrLimitsV.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrLimitsV.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrLimitsV.java $
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
 * $History: CP_GrLimitsV.java $
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
 * User: Scd          Date: 19.03.01   Time: 14:51
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
	drawing vertikal limit markers of bargraf

*/
public class CP_GrLimitsV extends CP_GrLimits
{	
	//== left or right limits, left is default 
	public static final int LEFT = 0;
	public static final int RIGHT = 1;
		
	/**
		constructor
		@param p_limitSide, LEFT or RIGHT
	*/
	CP_GrLimitsV(int p_side) 
	{
		setDefaults();
		if ( p_side == RIGHT ) 
			m_LimitSide = RIGHT;
		else
			m_LimitSide = LEFT;
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
			y=physValToPixel(value);
				
			//== get real style of mark
			realStyle = getRealStyle(style);

			//draw mark
			CP_GrLimitMark limitMark = new CP_GrLimitMark(p_Area.w-2*m_PadLimitMarks,realStyle);

			if (m_LimitSide == LEFT)
				x = p_Area.w;
			else
				x = p_Area.x;

			limitMark.fill3D(g,x-m_PadLimitMarks,y,CP_GrPolygon.NORTH,raised);
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
		value = ((double) Area.h) * ( m_yMax - p_value ) / 
			                        ( m_yMax - m_yMin )    + Area.y;

		//== round value
		intValue = (int) (value + 0.5); 

		//== restrict value to painting area
		if ( intValue < Area.y ) intValue = 0;
		if ( intValue > Area.y + Area.h ) intValue = Area.h;

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
		int style = CP_GrLimitMark.LOWERLEFT;
		
		if ( m_LimitSide == LEFT )
		{
			if (p_style == UPPER ) 
				style = CP_GrLimitMark.UPPERLEFT;
			else
				style = CP_GrLimitMark.LOWERLEFT;
		}
		else
		{
			if (p_style == UPPER ) 
				style = CP_GrLimitMark.UPPERRIGHT;
			else
				style = CP_GrLimitMark.LOWERRIGHT;
		}

		return style;
	}

	//== side, where limits are painted 
	protected int m_LimitSide = 0;

}