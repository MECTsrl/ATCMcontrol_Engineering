import java.util.Date;

class CP_TrendPoint 
{
	protected long         m_timeStamp;
	protected double       m_value;
	protected CP_GrQuality m_quality;

	public CP_TrendPoint(long t, double v, int q)
	{
		m_timeStamp = t;
		m_value     = v;
		m_quality   = new CP_GrQuality(q);
	}
	
	public long getTimeStamp()
	{
		return m_timeStamp;
	}

	public double getValue()
	{
		return m_value;
	}

	public CP_GrQuality getQuality()
	{
		return m_quality;
	}
}
