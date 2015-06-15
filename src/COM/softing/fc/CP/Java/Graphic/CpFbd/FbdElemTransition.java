/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdElemTransition.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FbdElemTransition.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdElemTransition.java $
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
 * $History: FbdElemTransition.java $
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
 * User: So           Date: 23.07.99   Time: 11:18
 * Updated in $/4Control/COM/softing/fc/CP/Java/Graphic/CPFBD
 * awt poup menu replaced by own "context" menu
 * 
 * *****************  Version 7  *****************
 * User: Mkr          Date: 15.07.99   Time: 13:10
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
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

public class FbdElemTransition extends FbdElemDynamic
{
	private boolean m_bIsBlocked;
	private boolean m_bIsForced;
	
//*****************************************************************************
	public FbdElemTransition(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 boolean bIsComment,       // if true, object is displayed as comment
		 int id)				   // 4C id
//*****************************************************************************		
	{	
		super(iXo, iYo, iWidth, iHeight, strName, bIsComment, id, true);
		m_ClassId |= ClassId_Transition;								
	}

//*****************************************************************************		
	public void paint(Graphics g)
//*****************************************************************************		
	{
		// base class painting
		PrePaint(g);
		
		// some sizes
		int gx = LogicToPixelX(1); // grid size in pixel
		int w = getSize().width;              // object size
		int h = getSize().height;
		int h2 = h / 2;		
		int hCaption = getStdCaptionHight(g); // caption hight		

		// colors
		Color BgCol = getBackgroundColorCaption();
		Color FgCol = getForgroundColor();		
		
		// draw transition
		g.setColor(FbdPicture.ColorElemFgStd);		
		g.drawLine(2*gx, 0, 2*gx, h-1);		  // pins
		
		g.setColor(FgCol);
		g.drawLine(0 , h2-1, 4*gx, h2-1); // thick horz. line
		g.drawLine(0 , h2  , 4*gx, h2  );						
		g.drawLine(0 , h2+1, 4*gx, h2+1);				

		// name caption		
		drawTextFrame(
			g, BgCol, FgCol, FgCol, 
			4*gx, h2-hCaption/2, w-4*gx, hCaption, m_strName, 
			FbdElemBase.TextFrameTypeNo, 
			FbdElemBase.TextAlignXCenter, FbdElemBase.TextAlignYCenter);

		// cross element if "bad" or "uncertain" quality
		drawQuality(g, 4*gx+1, h2-hCaption/2, w-2, h2+hCaption/2-2);
		
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
			CheckboxContextMenu mi;
			ContextMenu mi2;			
			
			boolean bCanModified = (getId() >= 0);
			
			mi2 = new ContextMenu("Force Transition Once");
			mi2.setActionCommand("forceTransitionOnce");
			mi2.setEnabled(bCanModified && !isForced());
			mi2.addActionListener(this);
			m_Menu.add(mi2);

			mi = new CheckboxContextMenu("Force Transition");
			mi.setActionCommand("forceTransition");
			mi.setState(isForced());
			mi.setEnabled(bCanModified && !isBlocked());
			mi.addItemListener(this);
			m_Menu.add(mi);

			mi = new CheckboxContextMenu("Block Transition");
			mi.setActionCommand("blockTransition");									
			mi.setState(isBlocked());
			mi.setEnabled(bCanModified && !isForced());			
			mi.addItemListener(this);
			m_Menu.add(mi);

			m_Menu.addSeparator();
			
			FbdPicture pic = (FbdPicture)this.getParent();			
			pic.addStdContextMenu(m_Menu);

			Point pt = getLocationOnScreen();
			m_Menu.setLocation(pt.x+x,pt.y+y);
			m_Menu.pack();	
			m_Menu.show();		
		}
	}	
	public void actionPerformed(ActionEvent e) 
	{
		super.actionPerformed(e);
	}	
}	
