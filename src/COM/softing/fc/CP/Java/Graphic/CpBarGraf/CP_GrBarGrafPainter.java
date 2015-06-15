/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrBarGrafPainter.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrBarGrafPainter.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrBarGrafPainter.java $
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
 * $History: CP_GrBarGrafPainter.java $
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
 * User: Scd          Date: 19.03.01   Time: 14:27
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
public abstract class CP_GrBarGrafPainter 
{
	//== property: rectangle where to paint the bargraf
	CP_GrRectAngle m_Area;

	//== property: physical range, origin 
	double m_yMin, m_yMax, m_yOrigin;

	//== property: background color, forground color positiv, negativ
	Color m_bgColor, m_fgColorPos, m_fgColorNeg;
	
	/**
		constructor
		@param p_Rect rectangle, where to paint the bargraf
	*/
	CP_GrBarGrafPainter(CP_GrRectAngle p_Rect) 
	{
		setDefaults();
		m_Area = new CP_GrRectAngle(p_Rect);
	}

	/**
		set property area
		@param p_Rect rectangle ( in pixel ) where to paint bargraf
	*/
	void setArea(CP_GrRectAngle p_Rect)
	{
		m_Area = p_Rect;
	}

	/**
		set property ( physical ) Range
		@param p_yMin minimal value of bargraf
		@param p_yMac maxiaml value of bargraf
		@param p_yOrigin origin of bargraf
	*/
	void setRange(double p_yMin, double p_yMax, double p_yOrigin)
	{
		m_yMin = p_yMin;
		m_yMax = p_yMax;
		m_yOrigin = p_yOrigin;
	}

	/**
		set minimal value of bargraf at position
		@p_value minimal value in bar graf
	*/
	public void setMin(double p_value, Graphics g)
	{
		if ( m_yMin != p_value && !isLocked() )
		{
			m_yMin = p_value;
			draw(g);
		}
	}

	/**
		set maximal value of bargraf at position
		@p_value maximal value in bar graf
	*/
	public void setMax(double p_value, Graphics g)
	{
		if ( m_yMax != p_value && !isLocked() )
		{
			m_yMax = p_value;
			draw(g);
		}
	}

	/**
		set origin value of bargraf at position
		@p_value origin value in bar graf
	*/
	public void setOrigin(double p_value, Graphics g)
	{
		if ( m_yOrigin != p_value )
		{
			m_yOrigin = p_value;
			draw(g);
		}
	}

	/**
		set property BgColor
		@param p_Color background color of bargraf
	*/
	void setBgColor(Color p_Color, Graphics g) 
	{
		if ( ! m_bgColor.equals(p_Color) && !isLocked() )
		{
			m_bgColor = p_Color;
			draw(g);
		}
	}

	/**
		set property FgColorPos
		@param forground color of 'positiv' ( > yOrigin ) values
	*/
	void setFgColorPos(Color p_Color, Graphics g) 
	{
		if ( ! m_fgColorPos.equals(p_Color) && !isLocked() )
		{
			m_fgColorPos = p_Color;
			draw(g);
		}
	}

	/**
		set property FgColorNeg
		@param forground color of 'negativ' ( < yOrigin ) values
	*/
	void setFgColorNeg(Color p_Color, Graphics g)
	{
		if ( ! m_fgColorNeg.equals(p_Color) && !isLocked() )
		{
			m_fgColorNeg = p_Color;
			draw(g);
		}
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
	*/
	void setDefaults() 
	{
		m_yMin = 0.0;
		m_yMax = 1.0;
		m_yOrigin = 0.0;
		m_bgColor = Color.lightGray;
		m_fgColorPos = Color.green;
		m_fgColorNeg = Color.green;
	}

	/**
		update the value and draw bargraf
		@param p_value new value to be displayed
		@param g graphics object where to paint
	*/
	public void setValue(double p_value, Graphics g)
	{	
		boolean drawBg = false;
		if ( isLocked() ) return;
		// test if bg has to be drawn, display of bargraf changed
		if ( ! m_actualValueInit ) 
		{
			m_actualValueInit = true;
			m_actualValue = p_value;
			m_actualValueOverflow = isOverflow(p_value);
			m_actualValueUnderflow = isUnderflow(p_value);
			drawBg = true;draw(g);
		}
		else
		{
			if ( p_value != m_actualValue )
			{
				if ( m_actualValueOverflow && isOverflow(p_value) || 
					 m_actualValueUnderflow && isUnderflow(p_value) )
					drawBg = false;
				else
					drawBg = true;
				m_actualValue = p_value;
				m_actualValueOverflow = isOverflow(p_value);
				m_actualValueUnderflow = isUnderflow(p_value);
			}
		}

		//== finally draw bargraf if necessary
		if ( drawBg ) draw(g);
	}

	/**
		set quality of bar graf 0 good, 1 uncertain, 2 bad
		if quality is not good, bar graf is crossed out red
		the default value is 'good'
		@param p_quality quality
	*/
	public void setQuality(int p_quality,Graphics g)
	{
		m_actualValueInit = true;
		if ( p_quality != m_quality.get() )
		{
			m_quality.set(p_quality);
			//draw(g);
		}
	}

	/**
		get quality of bar graf 0 good, 1 uncertain, 2 bad
	*/
	public int getQuality()
	{
		return m_quality.get();
	}

	/**
		draw bar graf
	*/
	public abstract void draw(Graphics g);

	/**
		test for overflow of value
		@param p_value value for bargraf
	*/
	protected boolean isOverflow(double p_value)
	{
		return ( p_value > m_yMax) ? true : false ;
	}

	/**
		test for underflow of value
		@param p_value value for bargraf
	*/
	protected boolean isUnderflow(double p_value)
	{
		return ( p_value < m_yMin) ? true : false ;
	}

	/**
		get the color for the cross if quality is not good
		be shure that cross can be seen in any case
	*/
	protected Color getCrossColor()
	{
		if (! ( m_fgColorPos.equals(Color.red) || 
				m_fgColorNeg.equals(Color.red) ||
				m_bgColor.equals(Color.red) ) )
		{
			return Color.red;
		}
		else if (! ( m_fgColorPos.equals(Color.black) || 
				m_fgColorNeg.equals(Color.black) ||
				m_bgColor.equals(Color.black) ) )
		{
			return Color.black;
		}
		else if (! ( m_fgColorPos.equals(Color.gray) || 
				m_fgColorNeg.equals(Color.gray) ||
				m_bgColor.equals(Color.gray) ) )
		{
			return Color.gray;
		}
		else
			return Color.lightGray;
	}
		
	/**
		draw small arrows at top/bottom of bar graf on over/underflow
		@param g graphics object
	*/
	protected abstract void drawOverflowArrows(Graphics g);

	/**
		draw quality, i.e. a read cross if quality is not good
		@param g graphics object
	*/
	protected void drawQuality(Graphics g)
	{
		if (! m_quality.isGood()) 
		{
			/*
			g.setColor(getCrossColor());
			g.drawLine(m_Area.x, m_Area.y, m_Area.x+m_Area.w-1, m_Area.y+m_Area.h-1);
			g.drawLine(m_Area.x+m_Area.w-1, m_Area.y, m_Area.x, m_Area.y+m_Area.h-1);
			
			Rectangle rect = new Rectangle(m_Area.x, m_Area.y, m_Area.w, m_Area.h);
			m_quality.paint(g,rect);
			*/
		}
	}


	/**
		get lock status of bargraf painter dependent on quality
		@return true, quality is not good, false quality is good
	*/
	protected boolean isLocked()
	{
		if ( m_quality.isBad() )
			return true;
		else
			return false;
	}
	

	/**
		map a physical value to the height of the
		rectangle in painting area of bargraf
		@param p_value physical value
		@return height in pixel of rectangle
	*/
	protected abstract int physValToPixel(double p_value);

	//== actual physical value	
	protected double m_actualValue;
	//== actual quality
	protected CP_GrQuality m_quality=new CP_GrQuality(CP_GrQuality.BAD);
	
	//== state of actual value: false = not initialized
	protected boolean m_actualValueInit = false;
	protected boolean m_actualValueOverflow = false;
	protected boolean m_actualValueUnderflow = false;
}