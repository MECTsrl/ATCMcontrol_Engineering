/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpStdOut/CP_Led.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_Led.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpStdOut/CP_Led.java $
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
 * $History: CP_Led.java $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpStdOut
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpStdOut
 * 
 * *****************  Version 1  *****************
 * User: Scd          Date: 19.03.01   Time: 16:08
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpStdOut
 * 
 * *****************  Version 3  *****************
 * User: Jd           Date: 17.08.00   Time: 12:36
 * Updated in $/4Control/COM/softing/fc/CP/Java/Graphic/CpStdOut
 * Changed start value, due to problems in the task-list faceplate.
 * 
 * *****************  Version 2  *****************
 * User: Jd           Date: 3.08.00    Time: 13:25
 * Updated in $/4Control/COM/softing/fc/CP/Java/Graphic/CpStdOut
 * West and east arrow Led direction corrected.
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 9.07.99    Time: 10:58
 * Created in $/COM/softing/fc/CP/Java/Graphic/CpStdOut
 * 
 * *****************  Version 2  *****************
 * User: Mkr          Date: 23.11.98   Time: 18:24
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:13
 * Created in $/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 5  *****************
 * User: Mkr          Date: 3.09.98    Time: 17:21
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 4  *****************
 * User: Mkr          Date: 6.07.98    Time: 16:38
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 2  *****************
 * User: Mkr          Date: 6.07.98    Time: 13:41
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 6.07.98    Time: 13:02
 * Created in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 6.07.98    Time: 12:08
 * Created in $/fc/CP/lib/Java/Graphic/CP_Led
 * 
 *==
 ******************************************************************************
H<<*/

import java.awt.*;

/**
	Component for LEDs
*/

class CP_Led extends Component
{
	//== object parameters

	private int m_ColorIndex = -1;
	private Color m_onColor = null;
	private Color m_offColor = null;
	private Color m_invalidColor = null;
	private boolean m_bValue = false;

	public final static int QUALITY_GOOD = 0;
	public final static int QUALITY_UNCERTAIN = 1;
	public final static int QUALITY_BAD = 2;
	private int m_Quality  = QUALITY_BAD;


	public final static int RECT_SUNKEN = 0;
	public final static int ARROW_NORTH_SUNKEN = 1;
	public final static int ARROW_EAST_SUNKEN  = 2;
	public final static int ARROW_SOUTH_SUNKEN = 3;
	public final static int ARROW_WEST_SUNKEN  = 4;
	public final static int MAX_LED_TYPE = 4;
	private int m_LedType  = RECT_SUNKEN;


	/**
		constructor
	*/

	CP_Led(Color BgColor, int LedType)
	{
		m_bValue = false;
		m_Quality  = QUALITY_BAD;

		if (BgColor != null)
			m_invalidColor = BgColor;			
		else
			m_invalidColor = Color.lightGray;
		
		setColor(0);
		if (LedType >= RECT_SUNKEN && LedType <= MAX_LED_TYPE)
			m_LedType = LedType;
		else
			m_LedType = RECT_SUNKEN;
		repaint();		
	}
	
	public void init(int ColorIndex)
	{
		setColor(ColorIndex);
		repaint();
	}

	private void setColor(int ColorIndex)
	{
		switch (ColorIndex)
		{
		case 0: //red
		default:
			m_onColor = new Color(255, 0, 0);
			m_offColor = new Color(96, 0, 0);
			break;

		case 2: //green
			m_onColor = new Color(0, 255, 0);
			m_offColor = new Color(0, 96, 0);
			break;

		case 1: //yellow
			m_onColor = new Color(255, 255, 0);
			m_offColor = new Color(96, 96, 0);
			break;

		case 3: //blue
			m_onColor = new Color(0, 0, 255);
			m_offColor = new Color(0, 0, 96);
			break;
		}
	}

	public void set(boolean bValue, int Quality, int ColorIndex)
	{
		boolean bRepaint = false;
		
		if (Quality == QUALITY_GOOD || Quality == QUALITY_UNCERTAIN)
		{
			if (bValue != m_bValue)
			{
				m_bValue = bValue;
				bRepaint = true;
			}
			if (m_ColorIndex != ColorIndex)
			{
				m_ColorIndex = ColorIndex;
				setColor(ColorIndex);			
				bRepaint = true;
			}
		}			
		
		if (m_Quality != Quality)
		{
			bRepaint = true;
			m_Quality = Quality;
		}

		if (bRepaint)
			drawLed(getGraphics());		
	}
	
    public void set(boolean bValue, int Quality)
	{
		set(bValue, Quality, m_ColorIndex);
	}

	public void paint(Graphics g)
	{
		drawLed(g);
	}
	
	private void drawLed(Graphics g)
	{
		Dimension d = getSize();
		int w = d.width;
		int h = d.height;
		if (w < 2 || h < 2)
			return;

		//== set led color
		Color LedColor;
		if (m_Quality == QUALITY_GOOD || m_Quality == QUALITY_UNCERTAIN)
		{
			if (m_bValue)
				LedColor = m_onColor;
			else
				LedColor = m_offColor;
		}
		else
		{
			LedColor = m_invalidColor;
		}

		if (m_LedType == ARROW_NORTH_SUNKEN)
		{
			g.setColor(m_invalidColor);
			g.fillRect(0, 0, w, h);

			int xp[] = {0  , w/2, w-1, 0,};
			int yp[] = {h-1, 1  , h-1, h-1,};

			g.setColor(LedColor);
			g.fillPolygon(xp, yp, 4);

			g.setColor(Color.white);
			g.drawPolygon(xp, yp, 4);

			g.setColor(Color.gray);
			g.drawPolygon(xp, yp, 2);
		}
		else if (m_LedType == ARROW_SOUTH_SUNKEN)
		{
			g.setColor(m_invalidColor);
			g.fillRect(0, 0, w, h);
			
			int xp[] = {w-1, w/2, 0, w-1};
			int yp[] = {0  , h-1, 0, 0  };
			g.setColor(LedColor);
			g.fillPolygon(xp, yp, 4);

			g.setColor(Color.gray);
			g.drawPolygon(xp, yp, 4);

			g.setColor(Color.white);
			g.drawPolygon(xp, yp, 2);
		}
		else if (m_LedType == ARROW_WEST_SUNKEN)
		{
			g.setColor(m_invalidColor);
			g.fillRect(0, 0, w, h);
			
			int xp[] = {0  , w-1, w-1, 0  };
			int yp[] = {h/2, 0  , h-1, h/2};

			g.setColor(LedColor);
			g.fillPolygon(xp, yp, 4);

			g.setColor(Color.white);
			g.drawPolygon(xp, yp, 4);

			g.setColor(Color.gray);
			g.drawPolygon(xp, yp, 2);
		}
		else if (m_LedType == ARROW_EAST_SUNKEN)
		{
			g.setColor(m_invalidColor);
			g.fillRect(0, 0, w, h);
			
			int xp[] = {0  , w-1, 0, 0  };
			int yp[] = {h-1, h/2, 0, h-1};
			g.setColor(LedColor);
			g.fillPolygon(xp, yp, 4);

			g.setColor(Color.gray);
			g.drawPolygon(xp, yp, 4);

			g.setColor(Color.white);
			g.drawPolygon(xp, yp, 2);			
		}
		else
		{
			g.setColor(LedColor);
			g.fillRect(1, 1, w-2, h-2);

			g.setColor(Color.gray);
			g.drawLine(0, 0, 0 , h-1);
			g.drawLine(0, 0, w-1, 0);
			g.setColor(Color.white);
			g.drawLine(w-1, h-1, 0, h-1);
			g.drawLine(w-1, h-1, w-1, 0);
		}

		// mark LED if quality is not "GOOD"
		if (m_Quality != QUALITY_GOOD)
		{
			if (m_Quality == QUALITY_UNCERTAIN)
				g.setColor(Color.black);
			else
				g.setColor(Color.red);				
			
			g.drawLine(1, h-2, w-2, 1);
			g.drawLine(1, 1  , w-2, h-2);
		}
	}
}