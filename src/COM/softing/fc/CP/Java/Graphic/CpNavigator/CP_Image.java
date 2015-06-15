import java.awt.Canvas;
import java.awt.Image;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.MediaTracker;

//
//
// CP_Image
//
//
class CP_Image extends Canvas
{
	Image m_image;
	CP_Image(Image img)
	{
		m_image = img;
		computeSize();
	}

	protected void loadImageSync(Image img)
	{
		MediaTracker mt = new MediaTracker(this);
		mt.addImage(img, 1);

		try
		{
			mt.waitForID(1);
		}
		catch(InterruptedException e)
		{
			return;
		}
		catch(Exception e)
		{
			CP_ApNavigator.out.append("Exception " + e.getMessage() + "\n");
		}
		if ( mt.isErrorAny() )
		{
			CP_ApNavigator.out.append("error: ");
			int status = mt.statusID(1, false);
			int aborted  = status & mt.ABORTED;
			int complete = status & mt.COMPLETE;
			int errored  = status & mt.ERRORED;
			int loading  = status & mt.LOADING;
			if (aborted>0) CP_ApNavigator.out.append("aborted \n");
			if (complete>0) CP_ApNavigator.out.append("complete \n");
			if (errored>0) CP_ApNavigator.out.append("errored \n");
			if (loading>0) CP_ApNavigator.out.append("loading \n");
		}
	}


	protected void computeSize()
	{
		loadImageSync(m_image);
		int width  = m_image.getWidth(this);
		int height = m_image.getHeight(this);
		super.setSize(width, height);
	}

	public Dimension getSize()
	{
		computeSize();
		return super.getSize();
	}

	public Dimension getMinimumSize()
	{
		return getSize();
	}

	public Dimension getPreferredSize()
	{
		return getSize();
	}
	
	public void setSize(int width, int height)
	{
	}

	public void setSize(Dimension d)
	{
	}

	public void paint(Graphics g)
	{
		Dimension d = getSize();
		g.drawImage(m_image, 0, 0, this);
		g.drawLine(0,0, d.width, d.height);
	}

}

