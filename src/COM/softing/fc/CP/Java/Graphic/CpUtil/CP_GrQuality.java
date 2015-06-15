/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil/CP_GrQuality.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrQuality.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpUtil/CP_GrQuality.java $
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
 * $History: CP_GrQuality.java $
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
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:13
 * Created in $/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 5  *****************
 * User: Ff           Date: 15.07.98   Time: 14:18
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 5  *****************
 * User: Ff           Date: 14.07.98   Time: 17:15
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
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

import java.awt.*;

//================================================================================
//================================================================================
public class CP_GrQuality 
{
	//============================================================
	//============================================================
	final static private Color m_qBadColor       = Color.red;	
	final static private Color m_qUncertainColor = Color.black;

	final static public int GOOD      = 0;
	final static public int UNCERTAIN = 1;
	final static public int BAD       = 2;

	//============================================================
	//============================================================
	private int m_quality;
	
	//============================================================
	//============================================================
	public CP_GrQuality(int q)
	{
		set(q);
	}

	//============================================================
	//============================================================
	public void set(int q)
	{
		if ( q == GOOD || q == UNCERTAIN )
			m_quality = q;
		else
			m_quality = BAD;
	}

	//============================================================
	//============================================================
	public int get()
	{
		return m_quality;
	}

	public boolean isGood()
	{
		return m_quality == GOOD;
	}
	
	public boolean isUncertain()
	{
		return m_quality == UNCERTAIN;
	}
	
	public boolean isBad()
	{
		return m_quality == BAD;
	}
	
	public boolean isNotGood()
	{
		return m_quality != GOOD;
	}
	
	public boolean isNotBad()
	{
		return m_quality != BAD;
	}
	
	//============================================================
	//============================================================
	public void paint(Graphics g, Rectangle area)
	{
		if ( m_quality == GOOD )
			return;

		Color oldColor = g.getColor();
		if ( isUncertain() )
			g.setColor(m_qUncertainColor);
		if ( isBad() )
			g.setColor(m_qBadColor);

		//=== draw two diagonal lines
		g.drawLine(area.x, area.y, area.x + area.width - 1, area.y + area.height - 1);
		g.drawLine(area.x + area.width - 1, area.y, area.x, area.y + area.height - 1);
			
		g.setColor(oldColor);
	}
}

