/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil/CP_GrPolygon.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrPolygon.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil/CP_GrPolygon.java $
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
 * $History: CP_GrPolygon.java $
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
 * User: Scd          Date: 20.03.01   Time: 9:29
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 2  *****************
 * User: Ff           Date: 16.10.98   Time: 11:32
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:13
 * Created in $/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 3  *****************
 * User: Ff           Date: 29.04.98   Time: 12:42
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 2  *****************
 * User: Ff           Date: 23.04.98   Time: 10:16
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 *==
 ******************************************************************************
H<<*/

import java.awt.*;

/**
	class for drawing polygons
	exspecially raised and sunken polygons
	note: there's somehow a bug or someting undocumented in AWT Polygon class
	a 3d frame can only be drawed once for every instance of the polygon
*/
public class CP_GrPolygon extends Polygon
{

	public final static double NORTH = 0.0;
	public final static double NW	= Math.PI*7/4;
	public final static double WEST = Math.PI*3/2;
	public final static double SW	= Math.PI*5/4;
	public final static double SOUTH = Math.PI;
	public final static double SE	= Math.PI*3/4;
	public final static double EAST	= Math.PI/2;
	public final static double NE	= Math.PI/4;

	/**
		default constructor
	*/
	public CP_GrPolygon()
	{
		super();
	}

	/**
		constructor
		@param p_x[] field of x coordinates of polygon
		@param p_y[] field of y coordinates of plolygon
		@param p_n number of points in polygon
		@param p_centerX local center of polygon ( not yet impl., set to 0 )
		@param p_centerX local center of polygon ( not yet impl., set to 0 )
	*/
	public CP_GrPolygon(int p_x[], int p_y[], int p_n,int p_centerX, int p_centerY)
	{
		super(p_x,p_y,p_n);
		m_centerX = p_centerX;
		m_centerY = p_centerY;
		m_Polygon = new Polygon(p_x,p_y,p_n);
		xpointsd = new double[p_n];
		ypointsd = new double[p_n];
		for (int ii=0;ii<p_n;ii++)
		{
			xpointsd[ii] = p_x[ii];
			ypointsd[ii] = p_y[ii];
		}
	}
	
	/**
		initialize coordinates
		@param p_x[] field of x coordinates of polygon
		@param p_y[] field of y coordinates of plolygon
		@param p_n number of points in polygon
		@param p_centerX local center of polygon ( not yet impl., set to 0 )
		@param p_centerX local center of polygon ( not yet impl., set to 0 )
	*/
	public void init(int p_x[], int p_y[], int p_n,int p_centerX, int p_centerY)
	{
		for (int ii=0;ii<p_n;ii++) addPoint(p_x[ii],p_y[ii]);
		m_centerX = p_centerX;
		m_centerY = p_centerY;
		m_Polygon = new Polygon(p_x,p_y,p_n);
		xpointsd = new double[p_n];
		ypointsd = new double[p_n];
		for (int ii=0;ii<p_n;ii++)
		{
			xpointsd[ii] = p_x[ii];
			ypointsd[ii] = p_y[ii];
		}
	}

	/**
		draw outline of polygon
		note: the polygon is first rotated in it's local coordinate system
		then the center is translated to given position
		@param g AWT Graphics object
		@param p_x x coordinate of position of center of polygon
		@param p_y y coordinate of position of center of polygon
		@param angle angle in radiants 
	*/
	public void draw(Graphics g,int p_x, int p_y,double p_angle)
	{
		rotateTo(p_angle);
		translate(p_x,p_y);
		g.drawPolygon(this);
	}

	/**
		draw sunken 3-d-outline of polygon
		note: the polygon is first rotated in it's local coordinate system
		then the center is translated to given position
		@param g AWT Graphics object
		@param p_x x coordinate of position of center of polygon
		@param p_y y coordinate of position of center of polygon
		@param angle angle in radiants 
	*/
	public void drawSunken(Graphics g,int p_x, int p_y,double p_angle)
	{
		rotateTo(p_angle);
		translate(p_x,p_y);
		draw3DFrame(g, false, Color.white, m_gh);
		expand(-1);
		draw3DFrame(g, false, Color.lightGray, m_gd);
	}

	/**
		draw raised 3-d-outline of polygon
		note: the polygon is first rotated in it's local coordinate system
		then the center is translated to given position
		@param g AWT Graphics object
		@param p_x x coordinate of position of center of polygon
		@param p_y y coordinate of position of center of polygon
		@param angle angle in radiants 
	*/
	public void drawRaised(Graphics g,int p_x, int p_y,double p_angle)
	{
		rotateTo(p_angle);
		translate(p_x,p_y);
		draw3DFrame(g, true, Color.white, m_gh);
		expand(1);
		draw3DFrame(g, true, Color.lightGray, m_gd);
	}

	/**
		fill polygon
		note: the polygon is first rotated in it's local coordinate system
		then the center is translated to given position
		@param g AWT Graphics object
		@param p_x x coordinate of position of center of polygon
		@param p_y y coordinate of position of center of polygon
		@param angle angle in radiants 
	*/
	public void fill(Graphics g, int p_x, int p_y, double p_angle)
	{
		rotateTo(p_angle);
		translate(p_x,p_y);
		g.fillPolygon(this);
		g.drawPolygon(this);
	}

		/**
		fill polygon with sunken 3-d frame
		note: the polygon is first rotated in it's local coordinate system
		then the center is translated to given position
		@param g AWT Graphics object
		@param p_x x coordinate of position of center of polygon
		@param p_y y coordinate of position of center of polygon
		@param angle angle in radiants 
	*/
	public void fillSunken(Graphics g,int p_x, int p_y,double p_angle)
	{
		rotateTo(p_angle);
		translate(p_x,p_y);
		g.fillPolygon(this);
		draw3DFrame(g, false, Color.white, m_gh);
		expand(-1);
		draw3DFrame(g, false, Color.lightGray, m_gd);
	}

	/**
		fill polygon with raised 3-d frame
		note: the polygon is first rotated in it's local coordinate system
		then the center is translated to given position
		@param g AWT Graphics object
		@param p_x x coordinate of position of center of polygon
		@param p_y y coordinate of position of center of polygon
		@param angle angle in radiants 
	*/
	public void fillRaised(Graphics g,int p_x, int p_y,double p_angle)
	{
		rotateTo(p_angle);
		translate(p_x,p_y);
		g.fillPolygon(this);
		draw3DFrame(g, true, Color.white, m_gh);
		expand(1);
		draw3DFrame(g, true, Color.lightGray, m_gd);
	}

	/**
		fill 3d polygon
		note: the polygon is first rotated in it's local coordinate system
		then the center is translated to given position
		@param g AWT Graphics object
		@param p_x x coordinate of position of center of polygon
		@param p_y y coordinate of position of center of polygon
		@param angle angle in radiants
		@param p_raise if true fill a raised else a sunken polygon
	*/
	public void fill3D(Graphics g, int p_x, int p_y, double p_angle, boolean p_raise)
	{
		if (p_raise) 
			fillRaised(g, p_x, p_y, p_angle);
		else
			fillSunken(g, p_x, p_y, p_angle);
	}
	
	/**
		draw an inner darkGray shade
		@param p_raise if true draw frame for raised else for sunken polygon
	*/
	protected void draw3DFrame(Graphics g, boolean p_raise)
	{
		draw3DFrame(g, p_raise, Color.white, Color.gray);
	}

	/**
		draw an inner darkGray shade
		@param p_raise if true draw frame for raised else for sunken polygon
	*/
	protected void draw3DFrame(Graphics g, boolean p_raise, Color sun, Color shadow)
	{
		Color color;
		boolean b;
		boolean sunsite[] = new boolean[npoints];

		//== compute sunsite edges for raised polygon
		for (int ii=0;ii<npoints-1;ii++)
		{
			sunsite[ii] = edgeOnSunsite(xpoints[ii],ypoints[ii],xpoints[ii+1],ypoints[ii+1]);
		}

		//== remember Color
		color = g.getColor();
		
		//== paint all shadowed edges gray
		g.setColor(shadow);
		for (int ii=0;ii<npoints-1;ii++)
		{
			if (  p_raise && sunsite[ii] || !p_raise && !sunsite[ii]  )
			{
				g.drawLine(xpoints[ii],ypoints[ii],xpoints[ii+1],ypoints[ii+1]);
			}
		}

		//== paint all edges on sunsite white 
		g.setColor(sun);
		for (int ii=0;ii<npoints-1;ii++)
		{
			if (  !(p_raise && sunsite[ii] || !p_raise && !sunsite[ii]) )
			{
				g.drawLine(xpoints[ii],ypoints[ii],xpoints[ii+1],ypoints[ii+1]);
			}
		}

		//== set old color
		g.setColor(color);
	}

	/**
		rotate polygon around its local coordinate system to specified angle
		note: all previous rotations/translations´/expansions are overwritten
		@param p_angle angle in radiants
	*/
	protected void rotateTo(double p_angle)
	{
		int ii,cx,cy;

		for (ii=0;ii<npoints;ii++) 
		{
			xpointsd[ii] = m_Polygon.xpoints[ii]*Math.cos(p_angle) - m_Polygon.ypoints[ii]*Math.sin(p_angle);
			ypointsd[ii] =  m_Polygon.xpoints[ii]*Math.sin(p_angle) + m_Polygon.ypoints[ii]*Math.cos(p_angle);
			xpoints[ii] = (int) Math.round(xpointsd[ii]);
			ypoints[ii] = (int) Math.round(ypointsd[ii]);
		}
		cx = (int) Math.round(m_centerX*Math.cos(p_angle) - m_centerY*Math.sin(p_angle) );
		cy = (int) Math.round(m_centerX*Math.sin(p_angle) + m_centerY*Math.cos(p_angle) );
		m_centerX = cx;
		m_centerY = cy;
	}

	/**
		translate polygon 
		already implemented in Polygon class
	*/
	public void translate(int p_deltaX, int p_deltaY)
	{
		int ii;

		for (ii=0;ii<m_Polygon.npoints;ii++) 
		{
			xpointsd[ii] += p_deltaX; 
			xpointsd[ii] += p_deltaX; 
			xpoints[ii] += p_deltaX; 
			ypoints[ii] += p_deltaY;
		}
		m_centerX += p_deltaX;
		m_centerY += p_deltaY;
	}

	/** 
		expand polygon
	*/
	protected void expand(int p_delta)
	{
		int xp[] = new int[m_Polygon.npoints];
		int yp[] = new int[m_Polygon.npoints];
		double ax1,ay1,ax2,ay2,le,c,wx,wy,xdir,ydir;

		for (int ii=1;ii<m_Polygon.npoints;ii++)
		{
			//
			// Winkelhalbierende (wx,wy) berechnen 
			//
			ax1 = xpointsd[ii-1] - xpointsd[ii];
			ay1 = ypointsd[ii-1] - ypointsd[ii];
			if (ii==m_Polygon.npoints-1)
			{
				ax2 = xpointsd[1] - xpointsd[ii];
				ay2 = ypointsd[1] - ypointsd[ii];
			}
			else
			{
				ax2 = xpointsd[ii+1] - xpointsd[ii];
				ay2 = ypointsd[ii+1] - ypointsd[ii];
			}
			int k = -sgn((int)(ax1*ay2 - ay1*ax2))*100;

			le=Math.sqrt(ax1*ax1+ay1*ay1);
			ax1=ax1/le;
			ay1=ay1/le;
			le=Math.sqrt(ax2*ax2+ay2*ay2);
			ax2=ax2/le;
			ay2=ay2/le;

			wx = k*(ax1 + ax2);
			wy = k*(ay1 + ay2);

			//
			// Richtung und Betrag (xdir,ydir) berechnen,
			// in die sich die Ecken bewegen muessen
			//
			if ( Math.abs(wx) <= 1)
			{
				xdir = 0;
				ydir = sgn(wy);
			}
			else if ( Math.abs(wy) <= 1)
			{
				xdir = sgn(wx);
				ydir = 0;
			}
			else
			{
				c = Math.abs((wx/wy));
				if ( c>1 )
				{
					xdir = c*sgn(wx);
					ydir = sgn(wy);
				}
				else
				{
					xdir = sgn(wx);
					ydir = (1/c)*sgn(wy);
				}
			}
			xp[ii] = xpoints[ii] + (int) Math.round(xdir*p_delta);
			yp[ii] = ypoints[ii] + (int) Math.round(ydir*p_delta);
		}

		xp[0] = xp[npoints-1];
		yp[0] = yp[npoints-1];

		for (int ii=0;ii<m_Polygon.npoints;ii++)
		{
			xpoints[ii] = xp[ii];
			ypoints[ii] = yp[ii];
		}
	}

	/**
		returns true if specified edge (of polygon) is on the 'sunny site' of the polygon
		@param p_x1 x coordinate of first point
		@param p_y1 y coordinate of first point
		@param p_x2 x coordinate of second point
		@param p_y2 x coordinate of second point
	*/
	protected boolean edgeOnSunsite(int p_x1, int p_y1, int p_x2, int p_y2) 
	{
		int x,y;
		boolean b;
		
		//== compute centre of edge
		x = (int) Math.round( (double)((p_x2+p_x1))/2.0 );
		y = (int) Math.round( (double)((p_y2+p_y1))/2.0 );

		//== go two pixels to north west (to sun)
		x = x - 2;
		y = y - 2;

		//== test if point is in polygon
		b = contains(x,y);
		return b;
	}
	
	/**
		return true if specified edge is horizontal
		@param p_x1 x coordinate of first point
		@param p_y1 y coordinate of first point
		@param p_x2 x coordinate of second point
		@param p_y2 x coordinate of second point
	*/
	protected boolean isHLine(int p_x1, int p_y1, int p_x2, int p_y2) 
	{
		return (p_y1 == p_y2);
	}
	
	/**
		return true if specified edge is vertical
		@param p_x1 x coordinate of first point
		@param p_y1 y coordinate of first point
		@param p_x2 x coordinate of second point
		@param p_y2 x coordinate of second point
	*/	
	protected boolean isVLine(int p_x1, int p_y1, int p_x2, int p_y2) 
	{
		return (p_x1 == p_x2);
	}

	/**
		return true if specified edge is horizontal or vertical
		@param p_x1 x coordinate of first point
		@param p_y1 y coordinate of first point
		@param p_x2 x coordinate of second point
		@param p_y2 x coordinate of second point
	*/
	protected boolean isHVLine(int p_x1, int p_y1, int p_x2, int p_y2) 
	{
		return (p_y1 == p_y2 || p_x1 == p_x2);
	}

	protected int sgn(int x)
	{
		if (x > 0) return 1;
		if (x < 0) return -1;
		return 0;
	}

	protected double sgn(double x)
	{
		if (x > 0) return 1;
		if (x < 0) return -1;
		return 0;
	}

	//== original polygon
	private Polygon m_Polygon;
	//== coordinates in double density
	private double xpointsd[],ypointsd[];
	//== center of polygon in local coordinate system (center of expansion)
	private int m_centerX=0, m_centerY=0;
	
	//== color for 3d frames
	private static Color m_gh = new Color(150,150,150);
	private static Color m_gd = new Color(100,100,100);
}