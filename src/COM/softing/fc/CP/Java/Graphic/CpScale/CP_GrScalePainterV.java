/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpScale/CP_GrScalePainterV.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrScalePainterV.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpScale/CP_GrScalePainterV.java $
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
 * $History: CP_GrScalePainterV.java $
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
import java.util.*;

//================================================================================
// 
//================================================================================
class CP_GrScalePainterV extends CP_GrScalePainter
{
	//============================================================
	/** The maximum length of all letterings in pixels.*/
	//============================================================
	private int m_maxLetLength;

	//============================================================
	/** The maximum length of all parts left of the dot of letterings in pixels.*/
	//============================================================
	private int m_maxLetPreDotLength;

	CP_GrScalePainterV(boolean alignDown, FontMetrics fm) 
	{
		super(alignDown, fm);

		m_maxLetLength = 0;
		m_maxLetPreDotLength = 0;
	}

	//============================================================
	/** Computes the positions for dashes and letterings.*/
	//============================================================
	public void compute(Rectangle r)
	{
		//=== do all the computations of the baseclass
		super.compute(r);

		//=== compute additionally the maximum lettering length
		computeMaxLettering(m_letterings, m_fontMetrics);
	}
		
	//============================================================
	/** Compute maximal lettering length.
	* NOTE: this not not simply the maximal length of all letterings.
	* Every lettering has a pre-dot-part and a post-dot-part:
	* 123.45 has pre-dot-part 123 and post-dot-part 45.
	* the maximal lettering length is:
	* <br>length of pre-dot-part + length of dot + length of post-dot-part
	*/
	//============================================================
	protected void computeMaxLettering(String[] letterings, FontMetrics fm)
	{
		int maxPreDotLen = 0;
		int maxPostDotLen = 0;
		int dotLen = fm.stringWidth(".");
		for( int i = 0; i < letterings.length; i++ )
		{
			StringTokenizer parts = splitLettering(letterings[i]);
			if ( parts.countTokens() >= 1 )
			{
				String preDotPart = parts.nextToken();
				int preDotLen = fm.stringWidth(preDotPart);
				if (  preDotLen > maxPreDotLen )
				{
					maxPreDotLen = preDotLen;
				}
			}
			if ( parts.countTokens() >= 1 )
			{
				String postDotPart = parts.nextToken();
				int postDotLen = fm.stringWidth(postDotPart);
				if (  postDotLen > maxPostDotLen )
				{
					maxPostDotLen = postDotLen;
				}
			}
		}
		m_maxLetLength = maxPreDotLen + dotLen + maxPostDotLen;
		m_maxLetPreDotLength = maxPreDotLen;
	}
		
	//============================================================
	/** Computes maximum overlapping of lettering texts.*/
	//============================================================
	protected int computeMaxOverlap(
		String[] letterings, int[] pos, int pad, FontMetrics fm)
	{
		return 0;
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
		int widthAxis
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
				posAxis,
				posAxisFrom,
				widthAxis,
				posAxisTo - posAxisFrom + 1
				);
		}
		else
		{
			g.fillRect(
				posAxis - ( widthAxis - 1 ),
				posAxisFrom,
				widthAxis,
				posAxisTo - posAxisFrom + 1
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
		if ( alignPos )
		{
			g.fillRect(
				posAxis + axisWidth,
				posAxisTo - pos - ( width - 1 ),
				length,
				width
				);
		}
		else
		{
			g.fillRect(
				posAxis - ( axisWidth + length - 1),
				posAxisTo - pos - ( width - 1 ),
				length,
				width
				);
		}
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
		int yOffSet = fm.getAscent() / 2;
		int xOffSet = 0;
		int xPos = posAxis + widthAxis + lenBigDash + padLettering;
		int xNeg = posAxis - widthAxis - lenBigDash - padLettering - m_maxLetLength;
		int y = posAxisTo + yOffSet - 1;
		for(int i = 0; i < pos.length; i++)
		{		
			String lettering = letterings[i];
			int preDotLen = fm.stringWidth(splitLettering(lettering).nextToken());
			xOffSet = m_maxLetPreDotLength - preDotLen;
			if ( alignPos )
			{
				g.drawString(lettering,	xPos + xOffSet, y - pos[i]);
			}
			else
			{
				g.drawString(lettering,	xNeg + xOffSet, y - pos[i]);
			}
		}

		g.setColor(oldColor);
	}

	//============================================================
	/** Computes the positions for the axis.*/
	//============================================================
	protected void computeAxis(Rectangle r, boolean alignPos)
	{
		m_posAxisFrom = r.y;
		m_posAxisTo   = r.y + r.height - 1;

		//=== positive alignment for a vertical scale means ...
		if ( alignPos )
		{
			//==== the axis is at the left border of the rectangle
			m_posAxis = r.x;
		}
		//=== negative alignment for a horizontal scale means
		else
		{
			//=== the axis is at the right border of the rectangle
			m_posAxis = r.x + r.width - 1;
		}
	}

	//============================================================
	/** Returns padding between two neighboured lettering.*/
	//============================================================
	protected int getPadLettering(FontMetrics fm)
	{
		//=== between two letterings must fit at least one line
		return fm.getAscent();
	}

	//============================================================
	/** Returns place for string in a given font in pixels.*/
	//============================================================
	protected int getStringSize(FontMetrics fm, String s)
	{
		int size = fm.getAscent();
		return size;
	}

	//============================================================
	/** Splits a string at point.*/
	//============================================================
	protected StringTokenizer splitLettering(String l)
	{
		return new StringTokenizer(l, ".");
	}

	//============================================================
	/** Get length of scale axis.
	*/
	//============================================================
	public int getAxisLength()
	{
		return 
			( m_posAxisFrom - m_posAxisTo + 1 );
	}

	//============================================================
	/** Get a guess how much the min and max letterings exceeds the axis ends.
	*/
	//============================================================
	public int getAxisExceed()
	{
		return (int)Math.ceil(m_fontMetrics.getAscent()/2.0); 
	}

	//============================================================
	/** Get breadth of scale. This is the place from the axis line
	  * to the end of the lettering.
	*/
	//============================================================
	public int getBreadth()
	{
		int b = 
			m_widthAxis    + 
			m_lenBigDash   +
			m_padLettering + 
			m_maxLetLength;
		return b;
	}
}

