/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdElemActionBlock.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FbdElemActionBlock.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdElemActionBlock.java $
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
 * $History: FbdElemActionBlock.java $
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
 * User: Scd          Date: 19.03.01   Time: 15:20
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpFbd
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

import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;

public class FbdElemActionBlock extends FbdElemAction
{
	private String  m_strQual;

//*****************************************************************************
	public FbdElemActionBlock(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 boolean bIsComment,       // if true, object is displayed as comment
		 int id,				   // 4C id
		 String strQual)           // qualifier
//*****************************************************************************		
	{	
		super(iXo, iYo, iWidth, iHeight, strName, bIsComment, id);
		m_ClassId |= ClassId_ActionBlock;						
		m_strQual = strQual;		 
	}

//*****************************************************************************		
	public void paint(Graphics g)
//*****************************************************************************		
	{
		// base class painting
		PrePaint(g);
		
		// some sizes
		int w = getSize().width - 1;              // object size
		int h = getSize().height - 1;
		int hCaption = getStdCaptionHight(g); // caption hight		
		int wQual = 3 * LogicToPixelX(1);  // width of qualifier field
		
		// colors
		Color BgCol = getBackgroundColorCaption();
		Color FgCol = getForgroundColor();		

		// object frame
		g.setColor(FgCol);		
		g.drawRect(0, 0, w, h);
		
		// caption width name
		drawTextFrame(
			g, BgCol, FgCol, FgCol, 
			0, 0, wQual, hCaption, m_strQual, 
			FbdElemBase.TextFrameTypeSingle, 
			FbdElemBase.TextAlignXCenter, FbdElemBase.TextAlignYCenter);		

		// caption width qualifier
		drawTextFrame(
			g, BgCol, FgCol, FgCol, 
			wQual-1, 0, w-wQual+1, hCaption, m_strName, 
			FbdElemBase.TextFrameTypeSingle, 
			FbdElemBase.TextAlignXCenter, FbdElemBase.TextAlignYCenter);		

		// base class painting
		PostPaint(g);
	}
}
