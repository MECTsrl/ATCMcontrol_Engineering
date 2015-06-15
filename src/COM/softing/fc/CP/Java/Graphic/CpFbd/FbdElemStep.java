/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdElemStep.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FbdElemStep.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdElemStep.java $
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
 * $History: FbdElemStep.java $
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
 * User: Scd          Date: 19.03.01   Time: 15:31
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 8  *****************
 * User: Mkr          Date: 4.08.99    Time: 18:24
 * Updated in $/4Control/COM/softing/fc/CP/Java/Graphic/CPFBD
 * 
 * *****************  Version 7  *****************
 * User: Mkr          Date: 27.07.99   Time: 15:47
 * Updated in $/4Control/COM/softing/fc/CP/Java/Graphic/CPFBD
 * 
 * *****************  Version 6  *****************
 * User: Mkr          Date: 14.07.99   Time: 18:22
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 5  *****************
 * User: Mkr          Date: 9.07.99    Time: 13:29
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 4  *****************
 * User: Mkr          Date: 6.07.99    Time: 19:10
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 3  *****************
 * User: Mkr          Date: 2.07.99    Time: 15:02
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 2  *****************
 * User: Mkr          Date: 29.06.99   Time: 15:43
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 23.06.99   Time: 17:49
 * Created in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * Initial version
 *==
 ******************************************************************************
H<<*/

import java.awt.*;
import java.awt.event.*;

public class FbdElemStep extends FbdElemDynamic
{
	private boolean m_bIsInitialStep;
	
//*****************************************************************************
	public FbdElemStep(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 boolean bIsComment,       // if true, object is displayed as comment
		 int id,				   // 4C id
		 boolean bIsInitialStep)   // if true -> initial step
//*****************************************************************************		
	{	
		super(iXo, iYo, iWidth, iHeight, strName, bIsComment, id, true);
		m_ClassId |= ClassId_Step;								
		m_bIsInitialStep = bIsInitialStep;
	}

//*****************************************************************************		
	public void paint(Graphics g)
//*****************************************************************************		
	{
		// base class painting
		PrePaint(g);
		
		// some sizes
		int gx = LogicToPixelX(1); // grid size in pixel
		int gy = LogicToPixelY(1);
		int w = getSize().width-1;              // object size
		int h = getSize().height-1;

		// colors
		Color BgCol = getBackgroundColorCaption();
		Color FgCol = getForgroundColor();		
		
		// draw step
		drawTextFrame(
			g, BgCol, FgCol, FgCol, 
			0, gy, w, h - 2 * gy, m_strName, 
			m_bIsInitialStep ? FbdElemBase.TextFrameTypeDouble : FbdElemBase.TextFrameTypeSingle,
			FbdElemBase.TextAlignXCenter, FbdElemBase.TextAlignYCenter);

		//draw pins
		int xCenter = LogicToPixelX((m_iWidth - (m_iWidth % 4)) / 2);
		g.drawLine(xCenter, 0, xCenter, gx);
		g.drawLine(xCenter, h, xCenter, h - gx);		

		drawQuality(g, 1, gy+1, w-2, h-gy-2);
		
		// base class painting
		PostPaint(g);
	}
	
//*****************************************************************************		
	protected void showContextMenu(int x, int y)
//*****************************************************************************				
	{
		if (canOperate())
		{
			m_Menu = new CpContext(new Frame());
			FbdPicture pic = (FbdPicture)this.getParent();			
			pic.addStdContextMenu(m_Menu);

			Point pt = getLocationOnScreen();
			m_Menu.setLocation(pt.x+x,pt.y+y);
			m_Menu.pack();	
			m_Menu.show();		
		}
	}	

}
