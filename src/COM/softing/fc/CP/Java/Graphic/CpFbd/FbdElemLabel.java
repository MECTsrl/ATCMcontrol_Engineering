/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdElemLabel.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FbdElemLabel.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdElemLabel.java $
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
 * $History: FbdElemLabel.java $
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
 * User: Scd          Date: 19.03.01   Time: 15:28
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 14.07.99   Time: 18:37
 * Created in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 *==
 ******************************************************************************
H<<*/
import java.awt.*;

public class FbdElemLabel extends FbdElemBase
{
//*****************************************************************************
	public FbdElemLabel(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 boolean bIsComment)       // if true, object is displayed as comment
//*****************************************************************************		
	{	
		super(iXo, iYo, iWidth, iHeight, strName, bIsComment);
		m_ClassId |= ClassId_Jump;						
	}

//*****************************************************************************		
	public void paint(Graphics g)
//*****************************************************************************		
	{
		// base class painting
		PrePaint(g);

		// colors
		Color BgCol = getBackgroundColorCaption();
		Color FgCol = getForgroundColor();		
	
		// object size
		int w = getSize().width-1;              
		int h = getSize().height-1;

		// caption width name
		drawTextFrame(
			g, BgCol,  FgCol, FgCol, 
			0, 0, w, h, m_strName, 
			FbdElemBase.TextFrameTypeNo, 
			FbdElemBase.TextAlignXCenter, FbdElemBase.TextAlignYCenter);
		
		// base class painting
		PostPaint(g);
	}
}
