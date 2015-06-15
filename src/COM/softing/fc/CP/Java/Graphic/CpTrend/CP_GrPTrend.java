import java.awt.*;
import java.util.*;

//================================================================================
// A graphical component for a trend display
//================================================================================
class CP_GrPTrend extends Component
{
	//============================================================
	// public constants

	//=== may be used for both: marker and interpolation style
	public final static int NONE      = 0;

	//=== constants for marker styles
	public final static int RECTANGLE        = 1;
	public final static int CIRCLE           = 2;
	public final static int RECTANGLE_FILLED = 3;
	public final static int CIRCLE_FILLED    = 4;
	
	//=== constants for interpolation styles
	public final static int LINEAR    = 1;
	public final static int LAST      = 2;

	//============================================================
	// protected data
	protected long m_currentTimeMillis = System.currentTimeMillis();
	
	protected Vector m_trends = null;

	protected long m_timeWindowMillis = 60000;

	protected Color m_uncertainColor = Color.black;
	protected Color m_badMarkerColor = Color.red;

	protected int m_markerRadius = 3;
	protected int m_uncertainMarkerRadius = 10;
	protected int m_badMarkerRadius = 10;
	
	protected Color m_gridColor  = Color.gray;
	protected boolean m_showGrid = true;

	protected Color m_trendBgColor = Color.black;
	protected Color m_trendFgColor = Color.black;

	protected CP_GrScalePainter m_valueScale = null;
	protected CP_GrScalePainter m_timeScale  = null;

	protected CP_GrQuality m_quality = new CP_GrQuality(CP_GrQuality.BAD);
	
	static final protected Font m_defaultFont = new Font("Sans", Font.PLAIN, 10);

	//============================================================
	// methods

	//========================================
	// constructor
	//========================================
	public CP_GrPTrend()
	{
		m_trends = new Vector(10);
		Font f = getFont();
		FontMetrics fm = this.getFontMetrics(f);
		m_valueScale = new CP_GrScalePainterV(true, fm);
		m_timeScale  = new CP_GrScalePainterH(true, fm);
	}

	//========================================
	// set font
	//========================================
	public void setFont(Font f)
	{
		super.setFont(f);
		FontMetrics fm = this.getFontMetrics(getFont());
		// note: fm is NEVER null due to getFont() implementation
		m_valueScale = new CP_GrScalePainterV(true, fm);
		m_timeScale  = new CP_GrScalePainterH(true, fm);
	}

	//========================================
	// get font
	//========================================
	public Font getFont()
	{
		return ( super.getFont() == null ? m_defaultFont : super.getFont() );
	}

	//========================================
	// get a trendbuffer by index
	//========================================
	public CP_TrendBuffer getTrendBuffer(int i)
		throws ArrayIndexOutOfBoundsException
	{
		return (CP_TrendBuffer)(m_trends.elementAt(i));
	}

	//========================================
	// set timewindow
	//========================================
	public void setTimeWindowMillis(long timeWindowMillis)
	{
		m_timeWindowMillis = timeWindowMillis;
	}

	//========================================
	// set uncertain quality color
	//========================================
	public void setUncertainColor(Color c)
	{
		m_uncertainColor = c;
	}
	
	//========================================
	// set bad quality marker color
	//========================================
	public void setBadMarkerColor(Color c)
	{
		m_badMarkerColor = c;
	}
	
	//========================================
	// get timewindow
	//========================================
	public long getTimeWindowMillis()
	{
		return m_timeWindowMillis;
	}

	//========================================
	// set marker size
	//========================================
	public void setMarkerSize(int s)
	{
		m_markerRadius = s;
	}

	//========================================
	// set uncertain quality marker size
	//========================================
	public void setUncertainMarkerSize(int s)
	{
		m_uncertainMarkerRadius = s;
	}

	//========================================
	// set bad quality marker size
	//========================================
	public void setBadMarkerSize(int s)
	{
		m_badMarkerRadius = s;
	}

	//========================================
	// set grid color
	//========================================
	public void setGridColor(Color c)
	{
		m_gridColor = c;
	}

	//========================================
	// get grid color
	//========================================
	public Color getGridColor()
	{
		return m_gridColor;
	}

	//========================================
	// show/hide grid
	//========================================
	public void showGrid(boolean b)
	{
		m_showGrid = b;
	}

	//========================================
	// test if grid is shown
	//========================================
	public boolean showGrid()
	{
		return m_showGrid;
	}

	//========================================
	// set background color for trend display
	//========================================
	public void setTrendBgColor(Color c)
	{
		m_trendBgColor = c;
	}

	//========================================
	// set foreground color for trend display
	//========================================
	public void setTrendFgColor(Color c)
	{
		m_trendFgColor = c;
	}

	//========================================
	// get a trendbuffer by index but suppress
	// all exceptions.
	// for class internal use only
	//========================================
	protected CP_TrendBuffer _getTrendBuffer(int i)
	{
		try
		{
			return getTrendBuffer(i);
		}
		catch(ArrayIndexOutOfBoundsException e)
		{
			return null;
		}
	}	

	//========================================
	// add a trendbuffer to the display
	//========================================
	public int addTrendBuffer(CP_TrendBuffer trendBuf)
	{
		if ( ! m_trends.contains(trendBuf) )
			m_trends.addElement(trendBuf);
		return m_trends.indexOf(trendBuf);
	}

	//========================================
	// set the quality
	//========================================
	public void setQuality(int q)
	{
		m_quality.set(q);
	}

	//========================================
	// set the timestamp for the current time
	//========================================
	public void setCurrentTime(long t)
	{
		m_currentTimeMillis = t;
	}

	//========================================
	// painting
	//========================================
	public void paint(Graphics g)
	{
			// prepare painting
			Dimension d = getSize();
			Color oldColor = g.getColor();
			g.setColor(getBackground());
			g.fillRect(0, 0, d.width, d.height);
			g.setColor(oldColor);

			int  iMax = m_trends.size();
			long tMax = m_currentTimeMillis;
			long tMin = tMax - m_timeWindowMillis;

			// set time scale domain
			m_timeScale.setDomain(-(tMax - tMin)/1000, 0.0);
			// set value scale domain
			m_valueScale.setDomain(0.0, 100.0);		

			// set constant padding values
			int padBot   = 1;
			int padMid   = 1;
			int padRight = 1;

			// compute how much value-scale-letterings will exceed its axis end
			// this is a fixed-value, since time scale is a vertical scale
			int valueScaleExceed  = m_valueScale.getAxisExceed();

			// set top padding
			int padTop   = 1 + valueScaleExceed;

			// compute how broad (y-direction!) the time-scale will be
			// this is a fixed value since the time-scale is horizontal scale
			int timeScaleBreadth = m_timeScale.getBreadth();

			// compute available height for value-scale
			int valueScaleHeightAvail = 
				d.height - padTop - padMid - timeScaleBreadth - padBot;
			
			// compute value scale layout
			m_valueScale.compute(
				new Rectangle(0, padTop, d.width, valueScaleHeightAvail)
				);
			
			// compute how much time-scale-letterings will exceed its axis end
			// this is a scale-layout-dependend value, since value-scale is a horizontal scale
			int timeScaleExceed   = m_timeScale.getAxisExceed();

			// set left padding
			int padLeft  = 0 + Math.max(20, timeScaleExceed);

			// compute value scale breadth
			int valueScaleBreadth = m_valueScale.getBreadth();
			
			// compute available width for time scale
			int timeScaleWidthAvail = 
				d.width - padLeft - padMid - valueScaleBreadth - padRight;

			// compute time scale layout
			m_timeScale.compute(
				new Rectangle(
				padLeft, 
				d.height - padBot - timeScaleBreadth + 1, 
				timeScaleWidthAvail, 
				d.height)
				);
			
			// correct axis position of value scale
			m_valueScale.translateAxis(d.width- valueScaleBreadth);
			
			// paint value scale
			m_valueScale.setColors(m_trendFgColor,m_trendFgColor,m_trendFgColor,m_trendFgColor);
			m_valueScale.paint(g);
			
			// paint time scale
			m_timeScale.setColors(m_trendFgColor,m_trendFgColor,m_trendFgColor,m_trendFgColor);
			m_timeScale.paint(g);
			
			// compute rectangle for trend display
			Rectangle rTrend = new Rectangle(
				padLeft,
				padTop,
				d.width - padLeft - padMid - valueScaleBreadth - padRight,
				d.height - padTop - padMid - timeScaleBreadth - padBot
				);
			
			Image display = createImage(rTrend.width, rTrend.height);
			Graphics gg = display.getGraphics();
			
			// paint trend background
			gg.setColor(m_trendBgColor);
			gg.fillRect(0, 0, rTrend.width, rTrend.height);
			gg.setColor(oldColor);
			
			Rectangle r = new Rectangle(0, 0, rTrend.width, rTrend.height);
			
			// paint grid
			if ( m_showGrid )
			{
				int[] horGridLines = m_valueScale.getPosBigDashes();
				int   horGridTo    = m_valueScale.getPosAxisTo();
				int[] verGridLines = m_timeScale.getPosBigDashes();
				int   verGridTo    = m_timeScale.getPosAxisTo();
				paintGrid(
					gg, 
					r,
					horGridTo, 
					horGridLines,
					verGridTo,
					verGridLines,
					m_gridColor
					);
			}
			
			
			// paint trends
			for( int i = 0; i < iMax; i++ )
			{
				CP_TrendBuffer trendBuf = _getTrendBuffer(i);
				paintTrend(gg, r, trendBuf, tMin, tMax, m_uncertainColor);
				trendBuf.trimToTime(tMax - m_timeWindowMillis);
			}
			
			g.drawImage(display, rTrend.x, rTrend.y, this);

		m_quality.paint(g, new Rectangle(getSize()));
	}	

	//========================================
	// painting the grid
	//========================================
	protected void paintGrid(
		Graphics g,
		Rectangle r,
		int horGridTo, 
		int[] horGridLines,
		int verGridTo, 
		int[] verGridLines,
		Color c
		)
	{
		Color oldColor = g.getColor();
		g.setColor(c);

		for(int i = 0; i < horGridLines.length; i++)
		{
			g.drawLine(
				0, 
				r.height - horGridLines[i] - 1,
				r.width, 
				r.height - horGridLines[i] - 1
				);
}

		for(int j = 0; j < verGridLines.length; j++)
		{
			g.drawLine(
				verGridLines[j], 
				0,
				verGridLines[j], 
				r.height
				);		}

		g.setColor(oldColor);
	}

	//========================================
	// painting a single trend
	//========================================
	protected void paintTrend(
		Graphics g, 
		Rectangle r,
		CP_TrendBuffer tb, 
		long tMin, 
		long tMax,
		Color uncertainColor
		)
	{
		try
		{
			// get interpolation style of trendbuffer
			int ipolStyle = tb.getInterpolationStyle();

			// get scale domain of trendbuffer
			double vMin = tb.getScaleMin();
			double vMax = tb.getScaleMax();	

			// initialize buffer index
			int i = 0;
			int iMax = tb.countPoints();

			// get first trendpoint and evaluate it
			CP_TrendPoint tp = tb.getPoint(i);
			long    t = tp.getTimeStamp();
			double  v = tp.getValue();
			CP_GrQuality q = tp.getQuality();
			Point p = computeScreenPoint(t, v, tMin, tMax, vMin, vMax, r);

			// extract style informations from trendbuffer
			int markerStyle   = tb.getMarkerStyle();
			Color markerColor = tb.getMarkerColor();
			Color lineColor   = tb.getLineColor();

			//!!!NOTE: no markers and no interpolation means
			//         nothing will be painted
			//         especially no bad- or uncertain-markers too
			if ( markerStyle == NONE && ipolStyle == NONE )
				return;

			// store this point as the previous point
			Point pPrev   = p;
			CP_GrQuality qPrev = q;
			// initialize quality of the point previous the previous as GOOD
			CP_GrQuality qPrevPrev = new CP_GrQuality(CP_GrQuality.GOOD);

			// Do the loop:
			for(
				// advance trendbuffer index 
				i++;
	  		    // but stop if maximal index has been reached
				i < iMax;
				// advance index, remember previous point 
				// and quality of the previous previous point
				i++, pPrev = p, qPrevPrev = qPrev, qPrev = q 
				)
			{
				// get actual point from trendbuffer and evaluate it
				tp = tb.getPoint(i);
				t = tp.getTimeStamp();
				v = tp.getValue();
				q = tp.getQuality();
				// transform buffer point into a screen point
				p = computeScreenPoint(t, v, tMin, tMax, vMin, vMax, r);

			    // if the last point had no bad quality ...
				// otherwise previous point will not be interpolated
				// nor marked in anyway!
				if ( qPrev.isNotBad() )
				{
					// ... and the actual point has no bad quality ...
					if ( q.isNotBad() )
					{
						// ... then interpolate them
						if ( qPrev.isUncertain() || q.isUncertain() )
							interpolatePoints(g, pPrev, p, uncertainColor, ipolStyle);
						else
							interpolatePoints(g, pPrev, p, lineColor, ipolStyle);
					}
					// ... if the previous previous point was or the actual is bad ...
					// nonBad-Neighbours of badPoints are marked as bad!
					// NOTE: not points are marked as bad/uncertain but ends of intervals
					if ( qPrevPrev.isBad() || q.isBad() )
					{
						// ... mark the previous point as end of a bad interval
						markPointInvalid(g, pPrev, m_badMarkerRadius, m_badMarkerColor);
					}
					// ... only otherwise 
					// ... if the previous previous point was or the actual is uncertain ...
					// nonUncertain-Neighbours of uncertainPoints are marked as uncertain!
					// NOTE: if a point is end of a bad interval and of an uncertain interval
					//       it is marked as bad
					else if ( qPrevPrev.isUncertain() || q.isUncertain() )
					{
						// is the previous point was good one
						if ( qPrev.isGood() )
							// ... mark the previous point as end of an uncertain interval
							markPointInvalid(g, pPrev, m_uncertainMarkerRadius, m_uncertainColor);
					}

					// draw the point mark for the previous point
					if ( qPrev.isUncertain() )
						markPoint(g, pPrev, markerStyle, m_markerRadius, uncertainColor);
					else
						markPoint(g, pPrev, markerStyle, m_markerRadius, markerColor);
				}
			}
			if ( qPrev.isNotBad() )
			{
				markPoint(g, pPrev, markerStyle, m_markerRadius, markerColor);
			}
		}
		catch(ArrayIndexOutOfBoundsException e)
		{
		}
	}

	//========================================
	// compute screen coordinates
	//========================================
	public Point computeScreenPoint(
		long t,
		double v,
		long tMin,
		long tMax,
		double vMin,
		double vMax,
		Rectangle viewRect
		)
	{
		int x = viewRect.x + (int)(Math.round(viewRect.width  * (t - tMin)/(tMax - tMin)));
		int y = viewRect.y + (int)(Math.round(viewRect.height * (vMax - v)/(vMax - vMin)));
		Point p = new Point(x, y);
		return p;
	}

	//========================================
	// mark a point
	//========================================
	protected void markPoint(
		Graphics g,
		Point p,
		int style,
		int r,
		Color color
		)
	{	
		Color oldColor = g.getColor();
		g.setColor(color);

		int d = 2*r;

		switch(style)
		{
			case CIRCLE:
				g.drawOval(p.x - r - 1, p.y - r - 1, d+2, d+2);
				break;
			case RECTANGLE:
				g.drawRect(p.x - r, p.y - r, d, d);
				break;
			case CIRCLE_FILLED:
				g.fillOval(p.x - r - 1, p.y - r - 1, d+2, d+2);
				break;
			case RECTANGLE_FILLED:
				g.fillRect(p.x - r, p.y - r, d, d);
				break;
			default: //NONE
		}

		g.setColor(oldColor);
	}

	//========================================
	// mark point as invalid
	//========================================
	protected void markPointInvalid(
		Graphics g,
		Point p,
		int r,
		Color c
		)
	{
		Color oldColor = g.getColor();
		g.setColor(c);
		g.drawLine(p.x,     p.y - r, p.x,     p.y + r);
		g.drawLine(p.x + 1, p.y - r, p.x + 1, p.y + r);
		g.drawLine(p.x - 1, p.y - r, p.x - 1, p.y + r);
		g.setColor(oldColor);
	}

	//========================================
	// interpolate points
	//========================================
	protected void interpolatePoints(
		Graphics g,
		Point p0,
		Point p1,
		Color color,
		int ipolStyle
		)
	{
		Color oldColor = g.getColor();
		g.setColor(color);

		switch(ipolStyle)
		{
			case LINEAR:
				g.drawLine(p0.x, p0.y, p1.x, p1.y);
				break;
			case LAST:
				g.drawLine(p1.x, p1.y, p0.x, p1.y);
				g.drawLine(p0.x, p1.y, p0.x, p0.y);
				break;
			default: //NONE
				break;
		}

		g.setColor(oldColor);
	}
}
