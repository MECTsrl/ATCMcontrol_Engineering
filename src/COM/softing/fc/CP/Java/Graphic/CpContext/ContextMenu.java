import java.awt.*;
import java.awt.event.*;
import java.util.*;

public class ContextMenu extends ContextLabel
{
	public ContextMenu(String text)
	{
		super(text);
	}
	
	public void addActionListener(ActionListener actionListener)
	{
		m_actionListeners.addElement(actionListener);
	}
	
	public void removeActionListener(ActionEvent actionListener)
	{
		m_actionListeners.removeElement(actionListener);
	}
	
	protected void performActionEvents()
	{
		ActionEvent actionEvent=new ActionEvent(this, ActionEvent.ACTION_PERFORMED,m_actionCommand);
		for (int ii=0; ii<m_actionListeners.size(); ii++)
		{
			ActionListener e=(ActionListener) m_actionListeners.elementAt(ii);
			e.actionPerformed(actionEvent);
		}
		CpContext ct=(CpContext)getParent();
		WindowEvent windowEvent=new WindowEvent(ct,WindowEvent.WINDOW_CLOSING);
		ct.dispatchEvent(windowEvent);							   
	}
			
	private Vector m_actionListeners=new Vector();
}
