/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpStdOut/CP_ValueOut.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_ValueOut.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpStdOut/CP_ValueOut.java $
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
 * $History: CP_ValueOut.java $
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
 * User: Scd          Date: 19.03.01   Time: 16:09
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpStdOut
 * 
 * *****************  Version 2  *****************
 * User: Ed           Date: 3.12.99    Time: 11:35
 * Updated in $/4Control/COM/softing/fc/CP/Java/Graphic/CpStdOut
 * Aenderungen von FF eingebaut
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 9.07.99    Time: 10:58
 * Created in $/COM/softing/fc/CP/Java/Graphic/CpStdOut
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:13
 * Created in $/fc/CP/Java/Graphic/CpUtil
 * 
 * *****************  Version 12  *****************
 * User: Mkr          Date: 3.09.98    Time: 17:21
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 11  *****************
 * User: Mkr          Date: 7.08.98    Time: 10:23
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 10  *****************
 * User: Mkr          Date: 29.07.98   Time: 16:08
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 9  *****************
 * User: Mkr          Date: 23.07.98   Time: 17:08
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 8  *****************
 * User: Mkr          Date: 23.07.98   Time: 16:43
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 7  *****************
 * User: Mkr          Date: 14.07.98   Time: 20:21
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 6  *****************
 * User: Mkr          Date: 9.07.98    Time: 11:34
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 5  *****************
 * User: Mkr          Date: 8.07.98    Time: 16:52
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 4  *****************
 * User: Mkr          Date: 8.07.98    Time: 12:59
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 3  *****************
 * User: Mkr          Date: 6.07.98    Time: 15:21
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 2  *****************
 * User: Mkr          Date: 6.07.98    Time: 13:41
 * Updated in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 6.07.98    Time: 13:04
 * Created in $/fc/CP/lib/Java/Graphic/CPUtil
 * 
 *==
 ******************************************************************************
H<<*/

import java.awt.*;
import java.text.DecimalFormat;
import java.util.Locale;

class CP_ValueOut extends Component
{
	//=== class variables to specify the text alignment
	public final static int LEFT   = 0;
	public final static int CENTER = 1;
	public final static int RIGHT  = 2;
	private int m_Align = LEFT;	 

	public final static int TYPE_DONTCARE = 0;
	public final static int TYPE_FLOAT	  = 1;
	public final static int TYPE_STRING	  = 2;
	private int m_VarType = TYPE_DONTCARE;

	private String m_ToLongString = null;
	private String m_Value = null;

	public final static int QUALITY_GOOD = 0;
	public final static int QUALITY_UNCERTAIN = 1;
	public final static int QUALITY_BAD = 2;
	private int m_Quality  = QUALITY_BAD;

	private Color m_BgColor = null;
	/**
		constructor
	*/

	CP_ValueOut(
		Color BgColor)
	{
		m_Value = null;
		m_Quality  = QUALITY_BAD;
		m_ToLongString = null;
	
		if (BgColor != null)
			m_BgColor = BgColor;			
		else
			m_BgColor = Color.lightGray;		
	}

	public void init(
		String VarType)
	{
		m_Value = null;
		m_Quality  = QUALITY_BAD;
		m_ToLongString = null;

		String VarTypeU = VarType.toUpperCase();
		if (VarTypeU.compareTo("WSTRING") == 0 || VarTypeU.compareTo("STRING") == 0)
		{
			m_Align = LEFT;
			m_VarType = TYPE_STRING;
		}
		else if (VarTypeU.compareTo("REAL") == 0 || VarTypeU.compareTo("LREAL") == 0)
		{
			m_Align = RIGHT;
			m_VarType = TYPE_FLOAT;
		}
		else if (VarTypeU.compareTo("BOOL") == 0)
		{
			m_Align = RIGHT;
			m_VarType = TYPE_DONTCARE;
		}
		else
		{
			m_Align = RIGHT;
			m_VarType = TYPE_DONTCARE;
		}
		repaint();
	}

    public void set(String value, int Quality)
	{
		boolean bRepaint = false;
		if (value == null)
			Quality = QUALITY_BAD;

		String value2 = value;
		if (m_VarType == TYPE_FLOAT)
		{
			if (Quality == QUALITY_GOOD || Quality == QUALITY_UNCERTAIN)
			{
				value2 = formatFloat(value);
				if (value2 == null)
					Quality = QUALITY_BAD;
			}
		}

		if (Quality == QUALITY_GOOD || Quality == QUALITY_UNCERTAIN)
		{
			if ((m_Value == null) || (m_Value.compareTo(value2) != 0))
			{
				bRepaint = true;
				m_Value = new String(value2);
			}
		}

		if (m_Quality != Quality)
		{
			bRepaint = true;
			m_Quality = Quality;
		}
		
		if (bRepaint)
			drawText(getGraphics());
	}

	private String formatFloat(String Val)
	{
		if (Val != null && Val.compareTo("") != 0)
		{
			try 
			{
				Double dObj = Double.valueOf(Val);
				if (!dObj.isInfinite())
				{
					double dVal = dObj.doubleValue();	
					double dValAbs  = Math.abs(dVal);	

					if (dValAbs >= 1000000.0)
					{
						return Val;
					}
					else
					{
						DecimalFormat df = (DecimalFormat)DecimalFormat.getInstance(Locale.US); 
						df.setMaximumIntegerDigits(50);
						if (dValAbs < 0.00000001)
							return Val;
						else if (dValAbs >= 10000.0)
							df.applyPattern("#0.00");
						else if (dValAbs >= 1.0)
							df.applyPattern("#0.0000");
						else if (dValAbs >= 0.001)
							df.applyPattern("#0.000000");
						else 
							df.applyPattern("#0.00000000");

						return df.format(dVal);
					}
				}
			}
			catch (NumberFormatException e) 
			{
			}
		}
		return null;
	}
	
	
	public void paint(Graphics g)
	{
		drawFrame(g);
		drawText(g);		
	}
	
	private void drawFrame(Graphics g)
	{
		Dimension d = getSize();
		int w = d.width;
		int h = d.height;

		if (w < 2 || h < 2)
			return;

		g.setColor(Color.gray);
		g.drawLine(0, 0, 0 , h-1);
		g.drawLine(0, 0, w-1, 0);
		g.setColor(Color.white);
		g.drawLine(w-1, h-1, 0, h-1);
		g.drawLine(w-1, h-1, w-1, 0);
	}

	private void drawText(Graphics g)
	{
		Dimension d = getSize();
		int w = d.width;
		int h = d.height;

		if (w < 2 || h < 2)
			return;

		g.setColor(m_BgColor);
		g.fillRect(1, 1, w-2, h-2);

 		if (m_Value != null)
		{
			FontMetrics fm = g.getFontMetrics();
			int ws = fm.stringWidth(m_Value);
			int ws_max = w - 8;

			String str;
			int align = m_Align;
			if (ws > ws_max)
			{
				if (m_VarType == TYPE_STRING)
				{
					ws += fm.stringWidth("...");
					int index = m_Value.length() - 1;
					while (ws > ws_max && index > 0)
					{
						ws -= fm.charWidth(m_Value.charAt(index));
						index--;
					}
					str = m_Value.substring(0, index) + "...";	
				}
				else
				{
					align = CENTER;
					if (m_ToLongString == null)
					{
						int nChars = ws_max / fm.stringWidth("#");
						StringBuffer ToLongBuffer = new StringBuffer(nChars);
						for (int ii = 0; ii < nChars-1; ii++)
							ToLongBuffer.append('#');
						m_ToLongString = ToLongBuffer.toString();
					}

					str = m_ToLongString;
					ws = fm.stringWidth(str);
				}

			}
			else
			{
				str = m_Value;
			}

			int y = h/2 + fm.getHeight()/2 - 1;

			int x = 5;
			if (align == RIGHT)
				x = w - 5 - ws;
			else if (align == CENTER)
				x = w/2 - ws/2;
		

			if (m_Quality == QUALITY_BAD)
				g.setColor(Color.red);
			else
				g.setColor(Color.black);
			
			g.drawString(str, x, y);
		}

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