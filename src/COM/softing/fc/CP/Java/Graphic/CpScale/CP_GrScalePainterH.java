/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpScale/CP_GrScalePainterH.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrScalePainterH.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpScale/CP_GrScalePainterH.java $
 *
 * =PROJECT             7VH102  ATCMControl
 *
 * =SWKE                CP
 *
 * =COMPONENT           graphic
 *
 * =CURRENT             $Date: 28.02.07 19:00 $
 *                      $Revision: 1 $
 *
 * =ENVIRONMENT         Win32:
 *                      MS Windows NT 4.0
 *                      MSVC++ V5.0 / MFC 4.2 / ATL 2.0 / DAO 3.5
 *                      MS Java 2.0
 *                      MS Visual SourceSafe 5.0
 *
 * =REFERENCES
 *==
 *----------------------------------------------------------------------------*
 *

 

 

 *==
 *
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATIONS
 * $History: CP_GrScalePainterH.java $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpScale
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpScale
 * 
 * *****************  Version 1  *****************
 * User: Scd          Date: 19.03.01   Time: 15:58
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpScale
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:13
 * Created in $/fc/CP/Java/Graphic/CpScale
 * 
 * *****************  Version 8  *****************
 * User: Ff           Date: 17.07.98   Time: 8:22
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 * 
 * *****************  Version 7  *****************
 * User: Ff           Date: 24.06.98   Time: 9:24
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 * 
 * *****************  Version 6  *****************
 * User: Ff           Date: 17.06.98   Time: 16:57
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 * 
 * *****************  Version 5  *****************
 * User: Ff           Date: 9.06.98    Time: 16:58
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 * 
 * *****************  Version 4  *****************
 * User: Ff           Date: 29.04.98   Time: 12:42
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 * 
 * *****************  Version 3  *****************
 * User: Ff           Date: 23.04.98   Time: 10:16
 * Updated in $/fc/CP/lib/Java/Graphic/CPScale
 *==
 ******************************************************************************
H<<*/

import java.awt.*;

//================================================================================
// 
//================================================================================
class CP_GrScalePainterH extends CP_GrScalePainter
{
	//============================================================
	/** Lettering positions.*/
	//============================================================
	private int m_posLettering[];

	//============================================================
	/** Constructs a new horizontal scale painter.*/
	//============================================================
	public CP_GrScalePainterH(boolean alignRight, FontMetrics fm)
	{
		super(alignRight, fm);
	}

	//============================================================
	/** Computes the positions for dashes and letterings.*/
	//============================================================
	public void compute(Rectangle r)
	{
		super.compute(r);

		m_posLettering = computePosLettering(
			m_alignmentPos,
			m_posAxisFrom,
			m_letterings,
			m_posBigDashes, 
			m_fontMetrics
			);
	}

	//============================================================
	/** Compute x-positions for lettering.*/
	//============================================================
	protected int[] computePosLettering(
		boolean alignPos, 
		int posAxisFrom,
		String[] letterings, 
		int[] posBigDash, 
		FontMetrics fm
		)
	{
		int noOfLetterings = Math.min(letterings.length, posBigDash.length);
		int[] pos = new int[noOfLetterings];
		for(int i = 0; i < noOfLetterings; i++)
		{		
			String lettering = letterings[i];
			int offSet = (int)(fm.stringWidth(lettering)/2);
			if ( alignPos )
			{
				pos[i] = posAxisFrom + posBigDash[i] - offSet;
			}
			else
			{
				pos[i] = posAxisFrom + posBigDash[i] - offSet;
			}
		}

		return pos;
	}

	//============================================================
	/** Computes maximum overlapping of lettering texts.*/
	//============================================================
	protected int computeMaxOverlap(
		String[] letterings, int[] pos, int pad, FontMetrics fm)
	{
		if ( 
			pos == null || letterings == null || 
			pos.length != letterings.length ||
			pos.length <= 1
			)
		{
			return 0;
		}

		int maxOverlap = 0;
		for(int i = 1; i < pos.length; i++)
		{
			int len = fm.stringWidth(letterings[i-1]);
			int overlap = pos[i-1] + len + pad - pos[i];
			if ( overlap > maxOverlap )
			{
				maxOverlap = overlap;
			};
		}

		return maxOverlap;
	}

	//============================================================
	/** Paints the scale axis.*/
	//============================================================
	protected void paintAxis(
		Graphics g,
		Color color,
		boolean alignPos,
		int posAxis,
		int posAxisFrom,
		int posAxisTo,
		int axisWidth
		)
	{
		//=== store old color
		Color oldColor = g.getColor();

		//=== set new color
		g.setColor(color);

		//=== draw axis line
		if ( alignPos )
		{
			g.fillRect(
				posAxisFrom,
				posAxis,
				posAxisTo - posAxisFrom + 1,
				axisWidth
				);
		}
		else
		{
			g.fillRect(
				posAxisFrom,
				posAxis - ( axisWidth - 1),
				posAxisTo - posAxisFrom + 1,
				axisWidth
				);
		}

		//=== reset old color
		g.setColor(oldColor);
	}

	//============================================================
	/** Draws a dash.*/
	//============================================================
	protected void paintDash(
		Graphics g,
		boolean alignPos, 
		int posAxis, int posAxisFrom, int posAxisTo, int axisWidth, 
		int pos, int length, int width
		)
	{
		Point upperLeft = new Point(0,0);
		if ( alignPos )
		{
			upperLeft.move(
				posAxisFrom + pos,
				posAxis + axisWidth
				);
		}
		else
		{
			upperLeft.move(
				posAxisFrom + pos,
				posAxis - ( axisWidth + length - 1 )
				);
		}
		g.fillRect(upperLeft.x, upperLeft.y, width, length);
	}
	
	//============================================================
	/** Paints lettering.*/
	//============================================================
	protected void paintLettering(
		Graphics g,
		Color color,
		FontMetrics fm,
		boolean alignPos,
		int posAxis,
		int posAxisFrom,
		int posAxisTo,
		int widthAxis,
		int lenBigDash,
		int padLettering,
		int[] pos,
		String[] letterings
		)
	{
		if ( pos == null || 
			letterings == null ||
			pos.length == 0 ||
			letterings.length == 0 ||
			pos.length != letterings.length
			)
		{
			return;
		}
		
		Color oldColor = g.getColor();
		g.setColor(color);

		for(int i = 0; i < pos.length; i++)
		{		
			String lettering = letterings[i];
			if ( alignPos )
			{
				g.drawString(
					lettering,
					m_posLettering[i],
					posAxis + widthAxis + lenBigDash + padLettering + fm.getAscent()
					);
			}
			else
			{
				g.drawString(
					lettering,
					m_posLettering[i],
					posAxis - widthAxis - lenBigDash - padLettering
					);
			}
		}

		g.setColor(oldColor);		
  }

	//============================================================
	/** Computes the positions for the axis.*/
	//============================================================
	protected void computeAxis(Rectangle r, boolean alignPos)
	{
		m_posAxisFrom = r.x;
		m_posAxisTo   = r.x + r.width - 1;

		//=== positive alignment for a horizontal scale means ...
		if ( alignPos )
		{
			//==== the axis is at the top of the rectangle
			m_posAxis = r.y;
		}
		//=== negative alignment for a horizontal scale means
		else
		{
			//=== the axis is at the bottom of the rectangle
			m_posAxis = r.y + r.height - 1;
		}
	}

	//============================================================
	/** Returns padding between two neighboured lettering.*/
	//============================================================
	protected int getPadLettering(FontMetrics fm)
	{
		//=== between two letterings must fit at least one character
		return fm.stringWidth("0");
	}

	//============================================================
	/** Returns place for string in a given font in pixels.*/
	//============================================================
	protected int getStringSize(FontMetrics fm, String s)
	{
		int size = fm.stringWidth(s);
		return size;
	}

	//============================================================
	/** Get length of scale. This is the length of the axis plus
	  * some lettering space exceeding the axis.
	*/
	//============================================================
	public int getAxisLength()
	{
		int l = m_posAxisTo - m_posAxisTo + 1;
		if ( m_letterings != null && m_letterings.length > 0 )
		{
			int maxOverlap = (int)Math.max(
				m_fontMetrics.stringWidth(m_letterings[0]),
				m_fontMetrics.stringWidth(m_letterings[m_letterings.length-1])
				);
			l += maxOverlap;
		}
		return l;
	}

	//============================================================
	/** Get a guess how much the min and max letterings exceed the 
	*   axis ends.
	*/
	//============================================================
	public int getAxisExceed()
	{
		int exceed = 0;
		// if any lettering is already computed
		// take the maximum of all lettering lengths
		if ( m_letterings != null && m_letterings.length > 0 )
		{
			exceed += (int)Math.ceil(
				Math.max(
					m_fontMetrics.stringWidth(m_letterings[0]),
					m_fontMetrics.stringWidth(m_letterings[m_letterings.length-1])
					)
					/2.0);
		}
		else
		  {
		    exceed += Math.max(
                                       m_fontMetrics.stringWidth(format(m_scaleModel.getMin())),
                                       m_fontMetrics.stringWidth(format(m_scaleModel.getMax()))
				       );
		  }
		
		return exceed;
	}
	//============================================================
	/** Get breadth of scale. This is the place from the axis line
	  * to the bottom of the lettering.
	  * Parameter n is not used.
	*/
	//============================================================
	public int getBreadth()
	{
		return 
			m_widthAxis    + 
			m_lenBigDash   + 
			m_padLettering + 
			m_fontMetrics.getAscent();
	}

}

