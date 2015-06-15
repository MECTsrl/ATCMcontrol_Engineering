import java.awt.*;
import java.awt.event.*;

//
//
// CP_TreeExpandButton
//
//

class CP_TreeExpandButton extends Canvas
{
	private CP_TreeNode m_owner;

	private boolean m_validSize = false;
	private boolean m_expanded  = false;

	private int m_gap = 1;

	CP_TreeExpandButton(CP_TreeNode owner, int width, int height)
	{
		m_owner = owner;
		
		setBackground(Color.white);
		super.setSize(width, height);
		m_validSize = true;

		addMouseListener(
			new MouseAdapter()
		{
			public void mouseClicked(MouseEvent e)
			{	
				toggle();
			}
		}
		);
	}

	public boolean isExpanded()
	{
		return m_expanded;
	}

	protected void setExpanded(boolean expanded)
	{
		m_expanded = expanded;
		m_owner.getOwner().doLayout();
	}

	protected void toggle()
	{
		setExpanded(! isExpanded());
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

	public void paint(Graphics g)
	{
		Dimension d = getSize();
		int top    = 0;
		int left   = 0;
		int bottom = d.height - 1;
		int right  = d.width - 1;

		Color oldColor = g.getColor();
		g.setColor(getForeground());

		g.drawLine(left,  top,    right, top);
		g.drawLine(right, top,    right, bottom);
		g.drawLine(right, bottom, left,  bottom);
		g.drawLine(left,  bottom, left,  top);

		int x1 = left  + m_gap + 1;
		int x2 = right - m_gap - 1;
		int y  = (int)((left+right)/2);
		if ( x2 >= x1 )
		g.drawLine(x1, y, x2, y); 
		if ( !m_expanded )
		{
			int y1 = top    + m_gap + 1;
			int y2 = bottom - m_gap - 1;
			int x  = (int)((top+bottom)/2);
			if ( y2 >= y1 )
			g.drawLine(x, y1, x, y2); 
		}

	
		g.setColor(oldColor);
	}

}

