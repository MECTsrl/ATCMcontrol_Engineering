/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpScale/CP_GrScaleH.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrScaleH.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpScale/CP_GrScaleH.java $
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
 * $History: CP_GrScaleH.java $
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
 * User: Scd          Date: 19.03.01   Time: 15:56
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

//================================================================================
/** A graphical component for horizontal scale. */
//================================================================================
public class CP_GrScaleH extends CP_GrScale
{

	//============================================================
	/** Constructs a new horizontal scale component.*/
	//============================================================
	public CP_GrScaleH(
		double min, double max, int q, 
		boolean alignRight, double relPad
		) 
	{
		Font f = ( getFont() == null ? m_defaultFont : getFont() );
		m_painter = new CP_GrScalePainterH(
			alignRight,
			getFontMetrics(f)
			);
		setMembers(min, max, q, relPad, m_painter);
	}

	//============================================================
	/** Paints this component.*/
	//============================================================
	public void paint(Graphics g)
	{
		//=== get size of this scale component
		Dimension d = getSize();

		//=== fill background with color
		Color oldColor = g.getColor();
		g.setColor(getBackground());
		g.fillRect(0, 0, d.width, d.height);
		g.setColor(oldColor);

		//=== compute padding 
		int pad = (int)(d.width * m_relPad);

		//=== compute bounding rectangle
		Rectangle r = new Rectangle(pad, 0, d.width - 2 * pad, d.height);

		Color c = getForeground();
		m_painter.setColors(c, c, c, c);

		//=== compute scale graphics
		m_painter.compute(r);

		//=== paint scale graphics
		m_painter.paint(g);

		//=== paint quality
		m_quality.paint(g, new Rectangle(0, 0, d.width, d.height));
	}
}

