import java.awt.*;
import java.awt.event.*;
import java.util.*;

public class CheckboxContextMenu extends ContextLabel implements ItemSelectable
{
	public CheckboxContextMenu(String text)
	{
		super(text);
		m_set=false;
	}
	
	public void setState(boolean bSet)
	{
		m_set=bSet;
		//repaint();
	}
	
	public boolean getState()
	{
		return m_set;
	}
	
	public void addItemListener(ItemListener itemListener)
	{
		m_itemListeners.addElement(itemListener);
	}
	
	public void removeItemListener(ItemListener itemListener)
	{
		m_itemListeners.removeElement(itemListener);
	}
	
	public Object[] getSelectedObjects()
	{
		return null;
	}
	
	public void paint(Graphics g)
	{
		super.paint(g);
		if (m_set)
			drawCheck(g);
	}
	
	protected void drawCheck(Graphics g)
	{
		Dimension dd=getSize();
		int x=1;
		int y=(dd.height-10)/2;
		CheckBitmap bm=new CheckBitmap();
		bm.draw(g,x,y-1);
	}

	protected void performActionEvents()
	{
		m_set=!m_set;
		int etype;
		if (m_set)
			etype=ItemEvent.SELECTED;
		else
			etype=ItemEvent.DESELECTED;
		ItemEvent itemEvent=new ItemEvent(this, etype, this, etype);
			
		for (int ii=0; ii<m_itemListeners.size(); ii++)
		{
			ItemListener e=(ItemListener) m_itemListeners.elementAt(ii);
			e.itemStateChanged(itemEvent);
		}
		
		CpContext ct=(CpContext)getParent();
		WindowEvent windowEvent=new WindowEvent(ct,WindowEvent.WINDOW_CLOSING);
		ct.dispatchEvent(windowEvent);					
	}

	private boolean m_set=false;
	private Vector m_itemListeners=new Vector();
}
