import java.awt.*;
import java.awt.event.*;
public class CpContext extends Dialog implements WindowListener, ActionListener
{
	public CpContext(Frame theFrame)
	{
		//super(theFrame, true); //dialog
		super(theFrame); //window
		// super(); //frame
		
		//createEntries();
		addWindowListener(this);
		m_layout=new GridBagLayout();
		setLayout(m_layout);
		setResizable(false); //dialog
	}

	public void addSeparator()
	{
		ContextSeperator sep=new ContextSeperator();
		GridBagConstraints constraint = new GridBagConstraints();
		constraint.weightx=1.0;
		constraint.weighty=0.25;
		constraint.gridwidth = GridBagConstraints.REMAINDER;
		constraint.fill = GridBagConstraints.BOTH;
		m_layout.setConstraints(sep, constraint);
		add(sep);
	}
	
	public Component add(Component component)
	{
		GridBagConstraints constraint = new GridBagConstraints();
		constraint.weightx=1.0;
		constraint.weighty=0.5;
		constraint.gridwidth = GridBagConstraints.REMAINDER;
		constraint.fill = GridBagConstraints.BOTH;
		m_layout.setConstraints(component, constraint);

		return super.add(component);
	}
	
	public void windowActivated(WindowEvent e)
	{
	}
	
	public void windowClosed(WindowEvent e)
	{
	}
	
	public void windowClosing(WindowEvent e)
	{
		dispose();
	}
	
	public void windowDeactivated(WindowEvent e)
	{
		dispose();
	}
	
	public void windowDeiconified(WindowEvent e)
	{
	}
		
	public void windowIconified(WindowEvent e)
	{
	}
	
	public void windowOpened(WindowEvent e)
	{
	}
	
	public void actionPerformed(ActionEvent e)
	{
		dispose();
	}

	protected void createEntries()
	{
		setLayout(new GridLayout(2,1));
		ContextLabel b1;
		CheckboxContextMenu b2;
		b1=new ContextMenu("hallo1");
		b2=new CheckboxContextMenu("hallo2");
		//b1.addActionListener(this);
		//b2.addActionListener(this);
		b2.setState(true);
		add(b1);
		add(b2);
		pack();
	}
	
	protected GridBagLayout m_layout;
}
