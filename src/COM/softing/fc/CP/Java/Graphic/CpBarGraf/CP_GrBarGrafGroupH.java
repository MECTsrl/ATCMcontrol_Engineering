/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrBarGrafGroupH.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CP_GrBarGrafGroupH.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf/CP_GrBarGrafGroupH.java $
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
 * $History: CP_GrBarGrafGroupH.java $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 1  *****************
 * User: Scd          Date: 19.03.01   Time: 14:26
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 30.09.98   Time: 16:12
 * Created in $/fc/CP/Java/Graphic/CpBarGraf
 * 
 * *****************  Version 3  *****************
 * User: Ff           Date: 29.04.98   Time: 12:42
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 * 
 * *****************  Version 2  *****************
 * User: Ff           Date: 23.04.98   Time: 10:15
 * Updated in $/fc/CP/lib/Java/Graphic/CPBarGraf
 *==
 ******************************************************************************
H<<*/


import java.awt.* ;
import java.util.* ;

/**
	drawing of horzontal bargraf

*/
public class CP_GrBarGrafGroupH extends CP_GrBarGrafGroup
{	
	/**
		contructor
		@param p_BarGrafN number of bargrafs in group
	*/
	CP_GrBarGrafGroupH(int p_BarGrafN) 
	{
		super(p_BarGrafN);
	}

	/**
		Add a bargraf to the group at specified position.
		The bargraf heritates the actual properties.
		@param p_Position position (from left to right), 1 is leftmost position
		@return reference to a CP_BarGrafPainter object
	*/
	public CP_GrBarGrafPainter addBarGraf(int p_position)
	{
		CP_GrBarGrafPainter barGraf;
		CP_GrRectAngle bgArea = new CP_GrRectAngle();

		//== don't allow invalid positions
		if (p_position < 1) p_position = 1;
		if (p_position > m_BarGrafN ) p_position = m_BarGrafN;		

		//== compute area where to paint bargraf
		bgArea = computeArea(p_position);

		//== instatiate bargraf and add to internal list
		barGraf = new CP_GrBarGrafPainterH(bgArea);
		addToList(barGraf,p_position);

		//== pass properties to new bargraf
		passProperties(barGraf);
		
		return barGraf;
	}

	/**
		add set point bar graf to group i.e.
		add two half width bar grafs for the set point at left and right
		of the other bargrafs. Only one set point per group is possible.
	*/
	public void addSetPoint()
	{
		CP_GrBarGrafPainter barGraf;
		CP_GrRectAngle bgArea = new CP_GrRectAngle();

		//== set flag for existing set point
		m_SetPoint = true;

		//== add bar graf left of all bar grafs
		bgArea = computeArea(0);
		barGraf = new CP_GrBarGrafPainterH(bgArea);
		addToList(barGraf,0);
		passSetPointProperties(barGraf);

		//== add bar graf right of all bar grafs
		bgArea = computeArea(m_BarGrafN+1);
		barGraf = new CP_GrBarGrafPainterH(bgArea);
		addToList(barGraf,m_BarGrafN+1);
		passSetPointProperties(barGraf);	
	}

	/**
		Compute the (pixel) area where single bargraf is painted.
		Each bargraf get 1/m_BarGrafN of the width of group.
		@param p_position position of bargraf in group (0 and m_BarGrafN+1
				are reserved for set points
		@return area in pixels where to paint bargraf
	*/
	protected CP_GrRectAngle computeArea(int p_position)
	{
		CP_GrRectAngle area = new CP_GrRectAngle();
		int barGrafN;
		int offset;
		int height;


		//== get size dynamically
		if ( m_Drawn ) m_Dimension = getSize();

		//== don't allow invalid positions
		if (p_position < 0) p_position = 0;
		if (p_position > m_BarGrafN + 1) p_position = m_BarGrafN + 1;
		
		//== easiest first: width is width of group
		area.w = m_Dimension.width;

		//== x coordinate is zero
		area.x = 0;
		

		//== if set point compute new number of bargrafs and offsets
		barGrafN = m_BarGrafN;
		offset = 0;

		if (! m_SetPoint ) 
		{
			//== height of normal bargraf
			height = m_Dimension.height / (m_BarGrafN) ;
		
			//== compute width, lowest bargraf may be higher
			if (p_position == barGrafN)
				area.h = m_Dimension.height - ( barGrafN - 1 ) * height;
			else 
				area.h = height - 1;

			//== compute y coordinate
			area.y = (p_position - 1) *  height  ;
		}
		else
		{
			//== height of normal bargraf
			height = m_Dimension.height / ( m_BarGrafN + 1 );

			//==
			offset = height/2;
		
			//== compute width, lowest bargraf may be higher
			if (p_position == m_BarGrafN +1 )
				area.h = m_Dimension.height - ( m_BarGrafN ) * height - height/2;
			else if (p_position == 0)
				area.h = height/2 - 1; 
			else 
				area.h = height - 1;

			//== compute y coordinate
			if (p_position == 0)
				area.y = 0;
			else
				area.y = (p_position - 1) *  height + offset ;
		}

		return area;
	}
}