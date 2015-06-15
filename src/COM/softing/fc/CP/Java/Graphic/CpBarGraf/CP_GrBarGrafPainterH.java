/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrBarGrafPainterH.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrBarGrafPainterH.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrBarGrafPainterH.java $
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
 * $History: CP_GrBarGrafPainterH.java $
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
 * User: Scd          Date: 19.03.01   Time: 14:28
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
public class CP_GrBarGrafPainterH extends CP_GrBarGrafPainter
{
	/**
		constructor
		@param p_Rect rectangle, where to paint the bargraf
	*/
	CP_GrBarGrafPainterH(CP_GrRectAngle p_Rect) 
	{
		super(p_Rect);
	}

	public void draw(Graphics g)
	{
		int temp, yBg, yOrigin;

		//== if value not initilized draw background and exit
		if ( !m_actualValueInit ) 
		{
			g.setColor(m_bgColor);
			g.fillRect(m_Area.x,m_Area.y,m_Area.w,m_Area.h);
			//== draw quality
			drawQuality(g);
			return;
		}

		//== compute coordinate of bargraf and origin
		yBg = physValToPixel(m_actualValue);
		yOrigin = physValToPixel(m_yOrigin);

		//== draw right background
		if ( yBg < m_Area.x + m_Area.w )
		{
			g.setColor(m_bgColor);
			temp = (yBg <= yOrigin)? yOrigin: yBg;
			g.fillRect(temp,m_Area.y,m_Area.w-temp,m_Area.h);
		}

		//== draw bargraf
		if ( yBg != yOrigin )
		{
			if ( yBg > yOrigin )
			{
				g.setColor(m_fgColorPos);
				g.fillRect(yOrigin, m_Area.y, yBg-yOrigin, m_Area.h);
			}
			else
			{
				g.setColor(m_fgColorNeg);
				g.fillRect(yBg, m_Area.y, yOrigin-yBg, m_Area.h);
			}
		}

		//== draw left background
		if ( yBg > m_Area.x  ) {
			g.setColor(m_bgColor);
			temp = ( yBg <= yOrigin )? yBg : yOrigin; 
			g.fillRect(m_Area.x, m_Area.y, temp-m_Area.x , m_Area.h);
		}
		
		//== draw under/overflow
		drawOverflowArrows(g);

		//== draw quality
		drawQuality(g);
	}

	/**
		draw small arrows at right/left of bar graf on over/underflow
		@param g graphics object
	*/
	protected void drawOverflowArrows(Graphics g)
	{

		//== set arrow size
		boolean fill = false;
		int d = m_Area.h/3 - 1;
		if ( d < 20 )
		{
			fill = true;
			d++;
		}

		CP_GrArrow arrow = new CP_GrArrow(d/2,d*2,d,d);

		//== draw overflow arrow
		if ( m_actualValueOverflow )
		{
			if ( ! fill )
			{
				if (m_yMax > m_yOrigin)
					g.setColor(m_fgColorPos);
				else
					g.setColor(m_bgColor);

				arrow.fill3D(g, m_Area.x+m_Area.w-5,m_Area.y+m_Area.h/2,CP_GrArrow.EAST,true);
			}
			else
			{
				if (m_yMax > m_yOrigin)
					g.setColor(m_bgColor);
				else
					g.setColor(m_fgColorPos);

				arrow.fill(g, m_Area.x+m_Area.w-5,m_Area.y+m_Area.h/2,CP_GrArrow.EAST);
			}
		}
		

		//== draw underflow arrow
		if ( m_actualValueUnderflow )
		{
			if ( ! fill )
			{
				if (m_yMin < m_yOrigin)
					g.setColor(m_fgColorNeg);
				else
					g.setColor(m_bgColor);

				arrow.fill3D(g,m_Area.x+5, m_Area.y+m_Area.h/2, CP_GrArrow.WEST,true);
			}
			else
			{
				if (m_yMin < m_yOrigin)
					g.setColor(m_bgColor);
				else
					g.setColor(m_fgColorNeg);

				arrow.fill(g,m_Area.x+5, m_Area.y+m_Area.h/2, CP_GrArrow.WEST);
			}
		}
	}


	/**
		map a physical value to the height of the
		rectangle in painting area of bargraf
		@param p_value physical value
		@return height in pixel of rectangle
	*/
	protected int physValToPixel(double p_value)
	{
		double value;
		int intValue;

		//== map value
		value = ((double) m_Area.w) * ( p_value - m_yMin) / 
			                          ( m_yMax - m_yMin )    + m_Area.x;

		//== round value
		intValue = (int) (value + 0.5); 

		//== restrict value to painting area
		if ( intValue < m_Area.x ) intValue = m_Area.x;
		if ( intValue > m_Area.x + m_Area.w ) intValue = m_Area.w;

		return intValue;
	}
}