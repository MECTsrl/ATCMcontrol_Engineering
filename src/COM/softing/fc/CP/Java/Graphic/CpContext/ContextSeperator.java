import java.awt.*;

public class ContextSeperator extends Component
{
	public ContextSeperator()
	{
		super();
	}

	public Dimension getMininumSize()
	{
		return new Dimension(4,4);
	}
	
	public Dimension getPreferredSize()
	{
		return new Dimension(4,4);
	}
								 
	public void paint(Graphics g)
	{
		Dimension dd=getSize();
		g.setColor(Color.gray);			   
		g.drawLine(0, dd.height/2, dd.width-1, dd.height/2);
		g.setColor(Color.white);			   
		g.drawLine(0, dd.height/2+1, dd.width-1, dd.height/2+1);
	}
}
