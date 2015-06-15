import java.awt.*;

//
//
// CP_TreeNodeContent
//
//
class CP_TreeNodeContent extends Canvas
{
	private CP_TreeNodeData m_data;
	private CP_ConfiData    m_confiData;

	private boolean m_valid_layout = false;

	static private Font  m_defaultFont       = null;
	static private Color m_defaultForeground = null;

	private int m_mid;
	
	private Image  m_icon;
	private String m_text;

	private int m_hGap = 3;
	private int m_vGap = 1;

	private int m_iconPosX;
	private int m_iconPosY;
	private int m_textPosX;
	private int m_textPosY;
	private int m_linePosFromX;
	private int m_linePosToX;
	private int m_linePosY;

	private int m_horAttach = 0;
	private int m_verAttach = 0;

	public CP_TreeNodeContent(CP_TreeNodeData data)
	{
		m_data = data;
		m_icon = data.getIcon();
		m_text = data.getName();

		setFont(getDefaultFont());
		setForeground(getDefaultForeground());
		setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));

		computeLayout();
	}
	
	public CP_TreeNodeContent(CP_ConfiData data)
	{
		m_confiData = data;
		m_icon = data.getIcon();
		m_text = data.getName();

		setFont(getDefaultFont());
		setForeground(getDefaultForeground());
		setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));

		computeLayout();
	}

	protected void init(Image icon, String text, int horAt, int verAt)
	{
		m_icon = icon;
		m_text = text;
		m_horAttach = horAt;
		m_verAttach = verAt;

		setFont(getDefaultFont());
		setForeground(getDefaultForeground());
		setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));

		computeLayout();
	}
	
	public Dimension getMinimumSize()
	{
		return getSize();
	}

	public Dimension getMaximumSize()
	{
		return getSize();
	}

	public Dimension getPreferredSize()
	{
		return getSize();
	}

	public void computeLayout()
	{
		//=== try to read size if icon image
		int iconWidth  = m_icon.getWidth(this);
		int iconHeight = m_icon.getHeight(this);

		//=== if size is unknown
		if ( iconWidth < 0 || iconHeight < 0 )
		{
			//=== load image synchrounusly
			loadImageSync(m_icon);

			//=== and read size once more
			iconWidth  = m_icon.getWidth (this);
			iconHeight = m_icon.getHeight(this);
		}

		//=== get size of text
		if ( getFont() == null ) CP_ApNavigator.out.append("FONT IS NULL\n");
		FontMetrics fm = getFontMetrics(getFont());
		int textWidth      = fm.stringWidth(m_text);
		int textHeight     = fm.getMaxAscent();
		int underlinedTextHeight = textHeight + m_vGap + 1;
		
		//=== compute size of the whole component
		int width  = iconWidth + textWidth + m_hGap;
		int height = Math.max(iconHeight, underlinedTextHeight);

		//=== compute horizontal positions
		int iconPosX     = 0;
		int textPosX     = iconPosX + iconWidth + m_hGap;
		int linePosFromX = textPosX;
		int linePosToX   = textPosX + textWidth;

		//=== compute vertical positions
		int midPos   = (int)height/2 - 1;
		int iconPosY = midPos - ((int)(iconHeight/2) - 1);
		int textPosY = midPos + (int)(underlinedTextHeight/2) - 1 - m_vGap;
		int linePosY = textPosY + 1 + m_vGap;
		
		//=== set component size
		super.setSize(width, height);

		//=== set positions
		m_iconPosX     = iconPosX;
		m_iconPosY     = iconPosY;
		m_textPosX     = textPosX;
		m_textPosY     = textPosY;
		m_linePosFromX = linePosFromX;
		m_linePosToX   = linePosToX;
		m_linePosY     = linePosY;

		m_mid = midPos;

		m_valid_layout = true;
	}
	public void paint(Graphics g)
	{
		g.drawImage (m_icon, m_iconPosX, m_iconPosY, this);

		Color origColor = g.getColor();
		g.setColor(getForeground());
		g.drawString(m_text, m_textPosX, m_textPosY);
		g.drawLine  (m_linePosFromX, m_linePosY, m_linePosToX, m_linePosY);
		g.setColor(origColor);
	}

	protected int loadImageSync(Image img)
	{
		MediaTracker mt = new MediaTracker(this);
		mt.addImage(img, 1);

		try
		{
			mt.waitForID(1);
		}
		catch(InterruptedException e)
		{
		}

		return mt.statusID(1, false);
		}

		public int getHorAttach()
	{
		return m_horAttach;
	}

	public int getVerAttach()
	{
		return m_verAttach;
	}

	public String getText()
	{
		return m_text;
	}
	
	public CP_TreeNodeData getData()
	{
		return m_data;
	}

	static public Font getDefaultFont()
	{
		if ( m_defaultFont == null )
			m_defaultFont = new Font("SansSerif", Font.PLAIN, 12);
		
		return m_defaultFont;
	}

	static public void setDefaultFont(Font f)
	{
		m_defaultFont = f;
	}

	static public Color getDefaultForeground()
	{
		if ( m_defaultForeground == null )
			m_defaultForeground = Color.blue;

		return m_defaultForeground;
	}

	static public void setDefaultForeground(Color c)
	{
		m_defaultForeground = c;
	}
}

