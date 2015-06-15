/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil/CP_GrFramedPanel.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrFramedPanel.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil/CP_GrFramedPanel.java $
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
 * $History: CP_GrFramedPanel.java $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 1  *****************
 * User: Scd          Date: 20.03.01   Time: 9:28
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:13
 * Created in $/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 4  *****************
 * User: Ff           Date: 29.04.98   Time: 12:42
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 3  *****************
 * User: Ff           Date: 23.04.98   Time: 10:16
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 *==
 ******************************************************************************
H<<*/

import java.awt.Panel;
import java.awt.Color;
import java.awt.Insets;
import java.awt.Graphics;
import java.awt.Dimension;
import java.awt.Rectangle;

/**
 * A Panel with a 3D-Frame and adjustable inner and outer borders.
 */
public class CP_GrFramedPanel extends Panel
{	
	//=== class variables to specify frame type
	public final static int NONE   = 0;
	public final static int RAISED = 1;
	public final static int SUNKEN = 2;
	public final static int RIDGE  = 3;
	public final static int GROOVE = 4;

	//=== class variables to specify border type
	public final static int ABSOLUTE = 0;
	public final static int RELATIVE = 1;
	
	//=== object parameters
	protected int m_frameType;
	protected int m_frameWidth;

	protected Color m_lighterColor;
	protected Color m_darkerColor;

	protected int m_xBorderType;
	protected int m_yBorderType;
	protected double m_xBorder;
	protected double m_yBorder;
	
	//=== derived variables
	protected int m_frameWidthPixel;
	
	/**
	 * 	Creates a new framed panel of a specified type.
	 *  Following defaults will be used:
	 *  <UL>
	 *		<LI>frame width: 4
	 *		<LI>lighter color: Color.white
	 *      <LI>darker color: Color.darkgray
	 *		<LI>outer border width: 0
	 *	</UL>
	 */
	public CP_GrFramedPanel(int ft)
	{
		setFrameType(ft);
		setFrameWidth(2);
		setFrameColor(Color.white, Color.gray);
		setXBorderType(0);
		setYBorderType(0);
	}
	
	//=== methods for setting parameters
	public void setXBorderType(int xBorder)
	{
		m_xBorderType = ABSOLUTE;
		m_xBorder = xBorder;
	}

	public void setXBorderType(double xBorderRel)
	{
		m_xBorderType = RELATIVE;
		m_xBorder = xBorderRel;
	}
	
	public void setYBorderType(int yBorder)
	{
		m_yBorderType = ABSOLUTE;
		m_yBorder = yBorder;
	}

	public void setYBorderType(double yBorderRel)
	{
		m_yBorderType = RELATIVE;
		m_yBorder = yBorderRel;
	}
	
	public void setFrameType(int ft)
	{
		//=== set frameType if valid
		//=== default is NONE
		m_frameType = ( ( 0 <= ft && ft <= 4 ) ? ft : NONE );
	}

	public void setFrameColor(Color lc, Color dc)
	{
		m_lighterColor = lc;
		m_darkerColor = dc;
	}
	
	public void setFrameWidth(int fw) 
	{
		//=== set frameWidth if valid
		//=== default is 2
		m_frameWidth = ( ( fw < 0 ) ? 2 : fw );

		switch (m_frameType)
		{
		case NONE:
			m_frameWidthPixel = 0;
			break;
		case RAISED:
			m_frameWidthPixel = m_frameWidth;
			break;
		case SUNKEN:
			m_frameWidthPixel = m_frameWidth;
			break;
		case RIDGE:
			m_frameWidthPixel = 2 * m_frameWidth;
			break;
		case GROOVE:
			m_frameWidthPixel = 2 * m_frameWidth;
		}
	}

	public int getXBorder()
	{
		int xBorder = 0;
		if ( m_xBorderType == ABSOLUTE )
		{
			xBorder = (int)Math.floor(m_xBorder);
		} 
		else
		{
			Dimension d = getSize();			
			xBorder = (int)Math.floor(d.width * m_xBorder);
		}
		xBorder = Math.max(xBorder, m_frameWidthPixel);
		return xBorder;
	}

	public int getYBorder()
	{
		int yBorder = 0;
		if ( m_yBorderType == ABSOLUTE )
		{
			yBorder = (int)Math.floor(m_yBorder);
		} 
		else
		{
			Dimension d = getSize();			
			yBorder = (int)Math.floor(d.height * m_yBorder) ;
		}
		yBorder = Math.max(yBorder, m_frameWidthPixel);
		return yBorder;
	}


	//=== override some methods from class Panel
	public Insets getInsets()
	{
		int xBorder = getXBorder();
		int yBorder = getYBorder();
		return new Insets(yBorder+1, xBorder+1, yBorder+1, xBorder+1);
	}

	public void paint(Graphics g)
	{
		//=== first paint all contained components
		super.paintComponents(g);

		//=== get panel size
		Dimension d = getSize();
		int w = d.width;
		int h = d.height;

		int fw = m_frameWidthPixel;

		int xb = getXBorder();
		int yb = getYBorder();

		int x0 = xb - fw + 1;
		int y0 = yb - fw + 1;
		int x1 = w - ( xb - fw + 1 );
		int y1 = h - ( yb - fw + 1 );

		Rectangle r = new Rectangle(x0, y0, x1 - x0, y1 - y0);

		Color oldColor = g.getColor();
		Color light = m_lighterColor;
		Color dark = m_darkerColor;

		switch ( m_frameType )
		{
		case NONE:
			break;
		case RAISED:
			paintRaisedFrame(g, light, dark, r, m_frameWidth);
			break;
		case SUNKEN:
			paintSunkenFrame(g, light, dark, r, m_frameWidth);
			break;
		case RIDGE:
			paintRidgeFrame(g, light, dark, r, m_frameWidth);
			break;
		case GROOVE:
			paintGrooveFrame(g, light, dark, r, m_frameWidth);
			break;
		}

		g.setColor(oldColor);

	}	

	protected void paintRaisedFrame(Graphics g, Color light, Color dark, Rectangle r, int width)
	{
		Rectangle rect = new Rectangle(r.x, r.y, r.width, r.height);
		for( int i = 0; i < width; i++ )
		{
			paintFrameRect(g, light, dark, rect);
			rect.x += 1;
			rect.y += 1;
			rect.width -= 2;
			rect.height -= 2;
			//rect.grow(-1, -1);
		}

	}

	protected void paintSunkenFrame(Graphics g, Color light, Color dark, Rectangle r, int width)
	{
		Rectangle rect = new Rectangle(r.x, r.y, r.width, r.height);
		for( int i = 0; i < m_frameWidth; i++ )
		{
			paintFrameRect(g, dark, light, rect);
			rect.x += 1;
			rect.y += 1;
			rect.width -= 2;
			rect.height -= 2;
			//rect.grow(-1, -1);
		}
	}

	protected void paintRidgeFrame(Graphics g,  Color light, Color dark, Rectangle r, int width)
	{ 
		Rectangle rect = new Rectangle(r.x, r.y, r.width, r.height);
		paintRaisedFrame(g, light, dark, rect, width);
		rect.grow(-width, -width);
		paintSunkenFrame(g, light, dark, rect, width);
	}

	protected void paintGrooveFrame(Graphics g,  Color light, Color dark, Rectangle r, int width)
	{ 
		Rectangle rect = new Rectangle(r.x, r.y, r.width, r.height);
		paintSunkenFrame(g, light, dark, rect, width);
		rect.grow(-width, -width);
		paintRaisedFrame(g, light, dark, rect, width);
	}

	protected void paintFrameRect(Graphics g, Color c1, Color c2, Rectangle r)
	{
		int west  = r.x;
		int north = r.y;
		int east  = r.x + r.width  - 1;
		int south = r.y + r.height - 1;
		
		g.setColor(c1);
		//=== line from southwest to northwest
		g.drawLine(west, north + 1, west, south);
		//=== line from northwest to northeast
		g.drawLine(west, north, east - 1, north);

		g.setColor(c2);
		//=== line from northeast to southeast
		g.drawLine(east, north, east, south - 1);
		
		//=== line from southeast to southwest
		g.drawLine(west + 1, south, east, south);
		  
	}
}
