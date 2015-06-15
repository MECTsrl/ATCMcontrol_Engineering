import java.awt.*;

public class CheckBitmap
{
	public CheckBitmap()
	{
		m_points[0] = new int[] { 0,0,0,0,0,0,0,0,0,0 };
		m_points[1] = new int[] { 0,0,0,0,0,0,0,0,0,0 };
		m_points[2] = new int[] { 0,0,0,0,0,0,0,0,0,1 };
		m_points[3] = new int[] { 0,0,0,0,0,0,0,0,1,1 };
		m_points[4] = new int[] { 1,0,0,0,0,0,0,1,1,0 };
		m_points[5] = new int[] { 1,1,0,0,0,0,1,1,0,0 };
		m_points[6] = new int[] { 0,1,1,0,0,1,1,0,0,0 };
		m_points[7] = new int[] { 0,0,1,1,1,1,0,0,0,0 };
		m_points[8] = new int[] { 0,0,0,1,1,0,0,0,0,0 };
		m_points[9] = new int[] { 0,0,0,0,1,0,0,0,0,0 };
	}
	
	public void draw(Graphics g, int x, int y)
	{
		//g.setColor(bg);
		//g.fillRect(x,y,10,10);
		//g.setColor(fg);
		for (int ii=0; ii<10; ii++)
		{
			for (int jj=0; jj<9; jj++)
			{
				if (m_points[jj][ii]==1)
				{
					g.drawLine(x+ii,y+jj,x+ii,y+jj);
				}
			}
		}
	}
															 
	protected int[][] m_points = new int[10][10];
}