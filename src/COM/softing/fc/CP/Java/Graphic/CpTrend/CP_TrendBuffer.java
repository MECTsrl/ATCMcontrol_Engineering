import java.util.*;
import java.awt.*;

//=================================================================
// buffers data for a single trend
//=================================================================
public class CP_TrendBuffer 
{
	//=============================================================
	// private data

	// a vector with trend points
	protected Vector m_points= null;

	protected double m_scaleMin = 0;
	protected double m_scaleMax = 1;

	//==============================================================
	// public data

	//=== style data
	protected int m_ipolStyle     = CP_GrPTrend.LINEAR;
	protected int m_markerStyle   = CP_GrPTrend.RECTANGLE;
	protected Color m_markerColor = Color.black;
	protected Color m_lineColor   = Color.black;

	//===============================================================
	// methods

	//=== constructor
	public CP_TrendBuffer(double scaleMin, double scaleMax)
	{
		m_points   = new Vector();
		m_scaleMin = scaleMin;
		m_scaleMax = scaleMax;
	}
	
	//=== set marker style
	public void setMarkerStyle(int style)
	{
		m_markerStyle = style;
	}
	//=== get marker style
	public int getMarkerStyle()
	{
		return m_markerStyle;
	}

	//=== set marker color
	public void setMarkerColor(Color color)
	{
		m_markerColor = color;
	}
	
	//=== get marker color
	public Color getMarkerColor()
	{
		return m_markerColor;
	}

	//=== set interpolation style
	public void setInterpolationStyle(int ipolStyle)
	{
		m_ipolStyle = ipolStyle;
	}
	
	//=== get interpolation style
	public int getInterpolationStyle()
	{
		return m_ipolStyle;
	}

	//=== set line color
	public void setLineColor(Color lineColor)
	{
		m_lineColor = lineColor;
	}

	//=== get line color
	public Color getLineColor()
	{
		return m_lineColor;
	}

	//=== get scale minimum
	public double getScaleMin()
	{
		return m_scaleMin;
	}

	//=== get scale maximum
	public double getScaleMax()
	{
		return m_scaleMax;
	}

	//=== set scale minimum
	public void setScaleMin(double scaleMin)
	{
		m_scaleMin = scaleMin;
	}

	//=== set scale maximum
	public void setScaleMax(double scaleMax)
	{
		m_scaleMax = scaleMax;
	}

	//=== get size of buffer
	public int countPoints()
	{
		return m_points.size();
	}

	//=== get a point from buffer
	public CP_TrendPoint getPoint(int i)
		throws ArrayIndexOutOfBoundsException
	{
		return ((CP_TrendPoint)m_points.elementAt(i));
	}

	// get a point from buffer but suppress all exceptions
	// for class internal use only
	protected CP_TrendPoint _getPoint(int i)
	{
		try
		{
			return getPoint(i);
		}
		catch(ArrayIndexOutOfBoundsException e)
		{
			return null;
		}
	}

	//=== add a new point
	public void addPoint(long timeStamp, double value, int quality)
	{
		// check timeStamp
		if ( 
			countPoints() > 0 && 
			_getPoint(0).getTimeStamp() > timeStamp
			)
		{
			throw new CP_TimeStampError();
		}

		CP_TrendPoint newPoint = new CP_TrendPoint(timeStamp, value, quality);

		// zero index belongs to the latest point
		m_points.insertElementAt(newPoint, 0);
	}

	//=== get an interpolated value at a given time
	public double getValue(long t, int ipol)
	{
		// if t is younger than the youngest point
		// or older than the oldest point
		if ( 
			getPoint(0).getTimeStamp() > t || 
			getPoint(countPoints()-1).getTimeStamp() < t 
			)
		{
			// then no value can be interpolated
			return Double.NaN;
		}

		// otherwise find the closest point not after t
		int i = closestIndexNotAfter(t);
		CP_TrendPoint p0 = _getPoint(i);
		double t0 = p0.getTimeStamp();
		double v0 = p0.getValue();
		
		// if t is exactly the timestamp of p0
		// or interpolation is last value
		if ( t0 == t || ipol == CP_GrPTrend.LAST )
		{
			// do not interpolate but return stored value
			return v0;
		}
		// otherwise interpolate linear
		else
		{
			CP_TrendPoint p1 = _getPoint(i+1);
			double t1 = p0.getTimeStamp();
			double v1 = p1.getValue();

			double v = v0 + (v1 - v0)*(t - t0)/(t1 - t0);
			return v;
		}
	}
	
	//=== find the index for that point, with the closest timestamp not after "time"
	protected int closestIndexNotAfter(long time)
	{
		int i;
		for(
			i = 0; 
			// while index is in range
			i < countPoints() &&
			// and the timestamp of point i is after time
			_getPoint(i).getTimeStamp() > time;
			// go to the next (older) point
			i++
			)
		{
		}
		
		// if no point was found
		if ( i == countPoints() )
			return -1;
		// otherwise
		return i;
	}

	//=== remove all points before a given timestamp
	public void trimToTime(long timeMillis)
	{
		try 
		{
			for(
				CP_TrendPoint p = (CP_TrendPoint)(m_points.lastElement());
				p.getTimeStamp() < timeMillis;
				p = (CP_TrendPoint)(m_points.lastElement())
					)
				{
					m_points.removeElement(p);
				}
		}
		catch(NoSuchElementException e)
		{
		}
	}
}

class CP_TimeStampError extends Error
{
}