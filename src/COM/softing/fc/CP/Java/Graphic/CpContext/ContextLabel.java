import java.awt.*;
import java.awt.event.*;
import java.util.*;

public abstract class ContextLabel extends Label implements MouseListener
{
	public ContextLabel(String text)
	{
		super("    " + text);
		addMouseListener(this);
		m_fg=getForeground();
		m_bg=getBackground();
	}
	
	public void setActionCommand(String actionCommand)
	{
		m_actionCommand=actionCommand;
	}
	
	public String getActionCommand()
	{
		return m_actionCommand;
	}

	public void mousePressed(MouseEvent e) {;}
	public void mouseClicked(MouseEvent e)
	{
		performActionEvents();
	}
	
	public void mouseReleased(MouseEvent e) {;}
	public void mouseEntered(MouseEvent e)
	{
		mouseEnter();
	}
	public void mouseExited(MouseEvent e) 
	{
		mouseLeave();
	}
	
	protected void mouseEnter()
	{
		setForeground(Color.white);
		setBackground(new Color(0,0,128));
	}
	
	protected void mouseLeave()
	{
		setForeground(m_fg);
		setBackground(m_bg);		
	}
	
	protected abstract void performActionEvents();
	
	private Color m_fg, m_bg;
	protected String m_actionCommand;
}
