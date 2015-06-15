import java.awt.*;

//
//
// CPStdOutLine
//
//
class CPStdOutLine extends Panel
{
	private final int HIDE_LED = 0x04;
	private final int HIDE_NAME = 0x02;
	private final int HIDE_VALUE = 0x01;
	private final int FULLSIZE_LED = 0x40;
	private final int FULLSIZE_NAME = 0x20;
	private final int FULLSIZE_VALUE = 0x10;

	private int m_HideMask = 0;
	
	private	CP_Led			m_CompLed = null;
	private	CPStdOutName	m_CompName = null;
	private	CP_ValueOut		m_CompVal = null;

	public CPStdOutLine(
		int HideMask,
		Color BgColor,
		Color BgColorValue,
		Color LinkColor)
	{
		m_HideMask = HideMask;
		
		setBackground(BgColor);

		// we need absolute positioning
		setLayout(null);
		
		m_CompLed = null;
		m_CompName = null;
		m_CompVal = null;
		
		// create LED if neccessary
		if ((m_HideMask & HIDE_LED) == 0)
		{
			m_CompLed = new CP_Led(BgColor, CP_Led.RECT_SUNKEN);
		}
		
		// create name field if neccessary
		if ((m_HideMask & HIDE_NAME) == 0)
		{
			m_CompName = new CPStdOutName(BgColor, LinkColor);
		}

		// create value field if neccessary
		if ((m_HideMask & HIDE_VALUE) == 0)
		{
			m_CompVal = new CP_ValueOut(BgColorValue);
			m_CompVal.setBackground(BgColorValue);
		}
	}

	public void init(
		int LedColor, 
		String VarPath, 
		String VarType, 
		String VarAttribute)
	{
		if (m_CompLed != null)
		{
			m_CompLed.init(LedColor);
			add(m_CompLed);			
		}
		if (m_CompName != null)
		{
			m_CompName.init(VarPath, VarType, VarAttribute);
			add(m_CompName);						
		}
		if (m_CompVal != null)
		{
			m_CompVal.init(VarType);
			add(m_CompVal);			
		}
	}

	public void setSize(
		Dimension size)
	{
		if (size != null)
		{
			super.setSize(size);
			
			//calculate size and position of components
			int w = size.width-4;
			int h = size.height;
			int d = h / 4;

			int x1 = 2;
			int w1 = h - 4;
			Rectangle rLed = new Rectangle(x1, x1, w1, w1);

			int x2 = x1 + w1 + d;
			int w2 = (w - x2 - d) / 2;
			w2 = Math.min(25 * h, w2);
			Rectangle rName = new Rectangle(x2, 0, w2, h);

			int x3 = x2 + w2 + d;
			int w3 = w - x3;
			Rectangle rValue = new Rectangle(x3, 0, w3, h);

			if ((m_HideMask & FULLSIZE_LED) != 0)
			{
				if ((m_HideMask & HIDE_NAME) != 0)
				{
					rLed = rLed.union(rName);
					if ((m_HideMask & HIDE_VALUE) != 0)
						rLed = rLed.union(rValue);
				}
			}

			if ((m_HideMask & FULLSIZE_NAME) != 0)
			{
				if ((m_HideMask & HIDE_LED) != 0)
					rName = rName.union(rLed);
				if ((m_HideMask & HIDE_VALUE) != 0)
					rName = rName.union(rValue);
			}

			if ((m_HideMask & FULLSIZE_VALUE) != 0)
			{
				if ((m_HideMask & HIDE_NAME) != 0)
				{
					rValue = rValue.union(rName);
					if ((m_HideMask & HIDE_LED) != 0)
						rValue = rValue.union(rLed);
				}
			}

			if (m_CompLed != null)
			{
				m_CompLed.setSize(rLed.width, rLed.height);
				m_CompLed.setLocation(rLed.x, rLed.y);			
			}
			
			// size and position of name
			if (m_CompName != null)
			{
				m_CompName.setSize(rName.width, rName.height);
				m_CompName.setLocation(rName.x, rName.y);
			}

				// size and position of out field
			if (m_CompVal != null)
			{
					m_CompVal.setSize(rValue.width, rValue.height);
					m_CompVal.setLocation(rValue.x, rValue.y);
			}
		}
	}

	public void set(
		 boolean bLedValue,
		 int LedQuality,
		 String Value, 
		 int Quality)
	{
		if (m_CompVal != null)
			m_CompVal.set( Value, Quality);
		if (m_CompLed != null)
			m_CompLed.set(bLedValue, LedQuality);			
	}

	public void set(
		 boolean bLedValue,
		 int LedQuality,
		 String Value, 
		 int Quality,
		 int LedColorIndex)
	{
		if (m_CompVal != null)
			m_CompVal.set( Value, Quality);
		if (m_CompLed != null)
			m_CompLed.set(bLedValue, LedQuality, LedColorIndex);			
	}
}

