/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdElemFunctionBlock.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FbdElemFunctionBlock.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdElemFunctionBlock.java $
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
 * $History: FbdElemFunctionBlock.java $
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
 * User: Scd          Date: 19.03.01   Time: 15:27
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 1  *****************
 * User: Mkr          Date: 14.07.99   Time: 18:37
 * Created in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 *==
 ******************************************************************************
H<<*/
import java.awt.*;

public class FbdElemFunctionBlock extends FbdElemBase
{
	private FbdPinList m_pinList = null;
	private String m_strTypeName = null;
	
//*****************************************************************************
	public FbdElemFunctionBlock(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 boolean bIsComment,       // if true, object is displayed as comment
		 String strTypeName,       // type name of function block
		 FbdPinList pinlist)       // array of pin information (name, position, ... )				 
//*****************************************************************************		
	{	
		super(iXo, iYo, iWidth, iHeight, strName, bIsComment);
		m_ClassId |= ClassId_FunctionBlock;
		m_strTypeName = strTypeName;
		m_pinList = pinlist;		
	}

//*****************************************************************************		
	public void paint(Graphics g)
//*****************************************************************************		
	{
		// base class painting
		PrePaint(g);

		// colors
		Color BgCol  = getBackgroundColorCaption();
		Color BgCol2  = getBackgroundColor();		
		Color FgCol = getForgroundColor();		
	
		// object size
		int w = getSize().width - 1;              
		int h = getSize().height - 1;
		int dx = LogicToPixelX(2); // pin length				

		// object background
		g.setColor(BgCol2);
		g.fillRect(dx, 0, w-2*dx, h);
		
		// object frame
		g.setColor(FgCol);
		g.drawRect(dx, 0, w-2*dx, h);
		
		// caption width name
		if (m_strName != null)
		{
			drawTextFrame(
				g, BgCol,  FgCol, FgCol, 
				dx, 0, w-2*dx, getStdCaptionHight(g), m_strName, 
				FbdElemBase.TextFrameTypeSingle, 
				FbdElemBase.TextAlignXCenter, FbdElemBase.TextAlignYCenter);
		}
		// some coordinates for spaceing of pin labels
		int hp = getStdCaptionHight(g)-1;		
		int dt = 4;
		int wt1 = w - 2 * dx - 2 * dt;		
		int wt2 = (wt1 / 2) - 2;
		int x1 = dx+dt;
		int x2 = w-dx-dt-wt2;		
	
		// draw type name
		int yType = 2;
		if (m_strName != null)
			yType += hp;

		drawTextFrame(
			g, BgCol2,  FgCol, FgCol, 
			x1, yType, wt1, hp, 
			m_strTypeName, 
			FbdElemBase.TextFrameTypeNo, 
			FbdElemBase.TextAlignXCenter, FbdElemBase.TextAlignYCenter);
		
		// draw pins and their labels
		g.setColor(FgCol);
		int nPins = m_pinList.getLength();
		FbdPin p = null;
		for (int ii = 0; ii < nPins; ii++)
		{
			p = m_pinList.getPin(ii);
			int x = LogicToPixelX(p.getX() - m_iXo);
			int y = LogicToPixelY(p.getY() - m_iYo);
			
			if (x == 0)
			{
				drawTextFrame(
					g, BgCol2,  FgCol, FgCol, 
					x1, y - hp/2, wt2, hp, p.getName(), 
					FbdElemBase.TextFrameTypeNo, 
					FbdElemBase.TextAlignXLeft, FbdElemBase.TextAlignYCenter);

				g.drawLine(x, y, x+dx, y); 
			}
			else
			{
				drawTextFrame(
					g, BgCol2,  FgCol, FgCol, 
					x2, y - hp/2, wt2, hp, p.getName(), 
					FbdElemBase.TextFrameTypeNo, 
					FbdElemBase.TextAlignXRight, FbdElemBase.TextAlignYCenter);

				g.drawLine(x, y, x-dx, y); 				
			}
		}

		// base class painting
		PostPaint(g);
	}
}
