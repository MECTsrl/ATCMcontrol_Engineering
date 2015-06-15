import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.net.URL;
//
//
// CPStdOutLine
//
//
class CPStdOutName extends Component
	implements MouseListener
{
	private String	m_VarPath = null;
	private String	m_VarName = null;
	private String	m_VarType = null;
	private String	m_VarAttribute = null;

	private Color m_BgColor = null;
	private Color m_LinkColor = null;

	public CPStdOutName(
		Color BgColor,
		Color LinkColor)
	{
		m_VarPath		= null;
		m_VarName		= null;
		m_VarType		= null;
		m_VarAttribute	= null;

		if (BgColor != null)
			m_BgColor = BgColor;			
		else
			m_BgColor = Color.lightGray;		

		if (LinkColor != null)
			m_LinkColor = LinkColor;			
		else
			m_LinkColor = Color.blue;		
	}

	public void init(
		String VarPath, 
		String VarType, 
		String VarAttribute)
	{
		m_VarPath		= new String(VarPath);	
		m_VarType		= new String(VarType);	
		m_VarAttribute	= new String(VarAttribute.toUpperCase());

		// extract var name from var path
		int iStart = m_VarPath.lastIndexOf('.');
		if (iStart > 0 && iStart < m_VarPath.length()-1)
		{
			m_VarName = new String(m_VarPath.substring(iStart+1));
		}
		else
		{
			m_VarName = m_VarPath;
			m_VarPath = "";
		}
		if (!isReadOnly())
			addMouseListener(this);

		repaint();
	}

	public void paint(Graphics g)
	{
		drawText(g);		
	}
	
	private void drawText(Graphics g)
	{
		Dimension d = getSize();
		int w = d.width;
		int h = d.height;

		g.setColor(m_BgColor);
		g.fillRect(0, 0, w, h);

		if (m_VarName != null)
		{
			FontMetrics fm = g.getFontMetrics();
			int ws = fm.stringWidth(m_VarName);

			String str;
			if (ws > w)
			{
				int ws2 = fm.stringWidth("...");
				StringBuffer ToLongBuffer = new StringBuffer(m_VarName);
				while (ToLongBuffer.length() > 0 && ws > w)
				{
					ToLongBuffer.setLength(ToLongBuffer.length() - 1);
					ws = fm.stringWidth(ToLongBuffer.toString()) + ws2;
				}
				str = ToLongBuffer.toString() + "...";
			}
			else
			{
				str = m_VarName;
			}

			int y = h/2 + fm.getHeight()/2 - 1;
			int x = 0;

			if (!isReadOnly())
			{
				g.setColor(m_LinkColor);
				g.drawLine(x, y+1, x+ws, y+1);
			}
			else
				g.setColor(Color.black);
			
			g.drawString(str, x, y);
		}
	}

	public boolean isReadOnly()
	{
		return m_VarAttribute.indexOf('W') < 0;
	}


	public void mousePressed(MouseEvent e)
	{
		if (!isReadOnly())
		{
			CPStdOut ap = (CPStdOut)getParent().getParent();
			ap.fireEventWriteValue(m_VarPath, m_VarType);
		}
	}

	public void mouseClicked(MouseEvent e) {;}
//	public void mousePressed(MouseEvent e) {;}
	public void mouseReleased(MouseEvent e) {;}
	public void mouseEntered(MouseEvent e) {;}
	public void mouseExited(MouseEvent e) {;}
}

