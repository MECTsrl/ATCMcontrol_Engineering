/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdElemBranchAlt.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FbdElemBranchAlt.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdElemBranchAlt.java $
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
 * $History: FbdElemBranchAlt.java $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 1  *****************
 * User: Scd          Date: 19.03.01   Time: 15:23
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 2  *****************
 * User: Mkr          Date: 9.07.99    Time: 13:29
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 6.07.99    Time: 19:10
 * Created in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 *==
 ******************************************************************************
H<<*/


import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;

public class FbdElemBranchAlt extends FbdElemBase
{
//*****************************************************************************
	public FbdElemBranchAlt(
		int iXo,                  // origin
		int iYo, 
		int iWidth,               // size
		boolean bIsComment)       // if true, object is displayed as comment
//*****************************************************************************		
	{	
		super(iXo, iYo, iWidth, 1, null, bIsComment);
		m_ClassId |= ClassId_BranchAlt;						
	}

//*****************************************************************************		
	public void paint(Graphics g)
//*****************************************************************************		
	{
		// base class painting
		PrePaint(g);
		
		int w = getSize().width; 
		g.drawLine(0, 0, w-1, 0);
		if (!isOverview())
			g.drawLine(0, 1, w-1, 1);		

		// base class painting
		PostPaint(g);
	}
	
//*****************************************************************************		
	protected void showContextMenu(int x, int y)
//*****************************************************************************				
	{
	}	
}
