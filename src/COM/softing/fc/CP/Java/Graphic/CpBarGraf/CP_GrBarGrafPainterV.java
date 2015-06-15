/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrBarGrafPainterV.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrBarGrafPainterV.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrBarGrafPainterV.java $
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
 * $History: CP_GrBarGrafPainterV.java $
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
 * User: Scd          Date: 19.03.01   Time: 14:29
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:12
 * Created in $/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 5  *****************
 * User: So           Date: 16.07.98   Time: 15:26
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
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
public class CP_GrBarGrafPainterV extends CP_GrBarGrafPainter 
{
	/**
		constructor
		@param p_Rect rectangle, where to paint the bargraf
	*/
	CP_GrBarGrafPainterV(CP_GrRectAngle p_Rect) 
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

		//== draw upper background
		if ( yBg > m_Area.y )
		{
			g.setColor(m_bgColor);
			temp = (yBg <= yOrigin)? yBg-m_Area.y : yOrigin-m_Area.y;
			g.fillRect(m_Area.x,m_Area.y,m_Area.w,temp);
		
		}

		//== draw bargraf
		if ( yBg != yOrigin )
		{
			if ( yBg < yOrigin )
			{
				g.setColor(m_fgColorPos);
				g.fillRect(m_Area.x, m_Area.y+yBg, m_Area.w, yOrigin-yBg);
			}
			else
			{
				g.setColor(m_fgColorNeg);
				g.fillRect(m_Area.x, yOrigin, m_Area.w, yBg - yOrigin);
			}
		}

		//== draw lower background
		if ( yBg < m_Area.y + m_Area.h ) {
			g.setColor(m_bgColor);
			temp = ( yBg <= yOrigin )? yOrigin : yBg; 
			g.fillRect(m_Area.x, temp, m_Area.w, m_Area.y+m_Area.h-temp);

		}
		
		//== draw under/overflow
		drawOverflowArrows(g);

		//== draw quality
		drawQuality(g);
	}

	/**
		draw small arrows at top/bottom of bar graf on over/underflow
		@param g graphics object
	*/
	protected void drawOverflowArrows(Graphics g)
	{
		//== set arrow size
		boolean fill = false;
		int d = m_Area.w/3 - 1;
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

				arrow.fill3D(g,m_Area.x+m_Area.w/2,m_Area.y+5,CP_GrArrow.NORTH,true);
			}
			else
			{
				if (m_yMax > m_yOrigin)
					g.setColor(m_bgColor);
				else
					g.setColor(m_fgColorPos);

				arrow.fill(g,m_Area.x+m_Area.w/2,m_Area.y+5,CP_GrArrow.NORTH);
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

				arrow.fill3D(g,m_Area.x+m_Area.w/2,m_Area.y+m_Area.h-5,CP_GrArrow.SOUTH,true);
			}
			else
			{
				if (m_yMin < m_yOrigin)
					g.setColor(m_bgColor);
				else
					g.setColor(m_fgColorNeg);

				arrow.fill(g,m_Area.x+m_Area.w/2,m_Area.y+m_Area.h-5,CP_GrArrow.SOUTH);
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
		value = ((double) m_Area.h) * ( m_yMax - p_value ) / 
			                          ( m_yMax - m_yMin )    + m_Area.y;

		//== round value
		intValue = (int) (value + 0.5); 

		//== restrict value to painting area
		if ( intValue < m_Area.y ) intValue = 0;
		if ( intValue > m_Area.y + m_Area.h ) intValue = m_Area.h;

		return intValue;
	}
}