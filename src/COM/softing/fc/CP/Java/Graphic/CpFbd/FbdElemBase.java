/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdElemBase.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FbdElemBase.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdElemBase.java $
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
 * $History: FbdElemBase.java $
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
 * User: Scd          Date: 19.03.01   Time: 15:21
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 7  *****************
 * User: Mkr          Date: 3.08.99    Time: 10:39
 * Updated in $/4Control/COM/softing/fc/CP/Java/Graphic/CPFBD
 * Multiline Comments
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

public class FbdElemBase extends Component 
{
	protected int m_iXo, m_iYo;         // object origin
	protected int m_iWidth, m_iHeight;  // size of object	
	protected boolean m_bIsComment;     // object will be display as comment
	protected String m_strName;         // name of object
	
	private Color m_oldColor;
	protected int m_ClassId;
	
	public final static int ClassId_Base             = 0x00001;	
	public final static int ClassId_Dynamic          = 0x00002;		
	public final static int ClassId_Line             = 0x00004;
	public final static int ClassId_Comment          = 0x00008;	
	public final static int ClassId_Branch           = 0x00010;
	public final static int ClassId_BranchAlt        = 0x00020;	
	public final static int ClassId_Action           = 0x00040;
	public final static int ClassId_ActionBlock      = 0x00080;
	public final static int ClassId_Transition       = 0x00100;
	public final static int ClassId_Step             = 0x00200;
	public final static int ClassId_FunctionBlock    = 0x00400;				
	public final static int ClassId_Function         = 0x00800;				
	public final static int ClassId_VarRead          = 0x01000;				
	public final static int ClassId_VarWrite         = 0x02000;				
	public final static int ClassId_VarFeedback      = 0x04000;				
	public final static int ClassId_Label            = 0x08000;
	public final static int ClassId_Jump             = 0x10000;					
	public final static int ClassId_Return           = 0x20000;					
	public final static int ClassId_Continuation     = 0x40000;					
	public final static int ClassId_Connector        = 0x80000;						
	
	protected final static int TextFrameTypeNo = 0;
	protected final static int TextFrameTypeSingle = 1;
	protected final static int TextFrameTypeDouble = 2;
	protected final static int TextAlignXLeft = 0;
	protected final static int TextAlignXCenter = 1;
	protected final static int TextAlignXRight = 2;	
	protected final static int TextAlignYTop = 0;
	protected final static int TextAlignYCenter = 1;
	
//*****************************************************************************
	public FbdElemBase(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 boolean bIsComment)       // if true, object is displayed as comment
//*****************************************************************************		
	{	
		m_ClassId       = ClassId_Base;
		m_oldColor      = null;		
		m_iXo           = iXo;
		m_iYo           = iYo;
		m_iWidth        = iWidth;
		m_iHeight       = iHeight;
		m_bIsComment    = bIsComment;
		if (strName != null)
			m_strName = new String(strName);
		else
			m_strName = null;
	}
	
//*****************************************************************************		
	public int getClassId()        { return m_ClassId; }
	public boolean isComment()     { return m_bIsComment; }	

//*****************************************************************************		
	protected Color getForgroundColor()
//*****************************************************************************		
	{
		return isComment() ? FbdPicture.ColorElemComment : FbdPicture.ColorElemFgStd;
	}

//*****************************************************************************		
	protected Color getBackgroundColor()
//*****************************************************************************		
	{
		if (isAutomatic())
			return FbdPicture.ColorElemAutomatic;				
		else
			return FbdPicture.ColorElemManual;
	}

//*****************************************************************************		
	protected Color getBackgroundColorCaption()
//*****************************************************************************		
	{
		if (isAutomatic())
			return FbdPicture.ColorElemCaptAutomatic;				
		else
			return FbdPicture.ColorElemCaptManual;
	}

//*****************************************************************************		
	protected int LogicToPixelX(int x)
//*****************************************************************************		
	{
		FbdPicture pic = (FbdPicture)this.getParent();
		return pic.LogicToPixelX(x);
	}

//*****************************************************************************		
	protected int LogicToPixelY(int y)
//*****************************************************************************		
	{
		FbdPicture pic = (FbdPicture)this.getParent();
		return pic.LogicToPixelY(y);
	}

//*****************************************************************************		
	protected boolean isAutomatic()
//*****************************************************************************		
	{
		FbdPicture pic = (FbdPicture)this.getParent();
		return pic.isAutomatic();
	}

//*****************************************************************************		
	protected boolean isOverview() 
//*****************************************************************************		
	{
		FbdPicture pic = (FbdPicture)this.getParent();
		return pic.isOverview();
	}
	
//*****************************************************************************		
	public void PrePaint(Graphics g)
//*****************************************************************************		
	{
		// save previous color
		m_oldColor = g.getColor();
	}

//*****************************************************************************		
	public void PostPaint(Graphics g)
//*****************************************************************************		
	{
		if (m_oldColor != null)
			g.setColor(m_oldColor);	
		m_oldColor = null;
	}
	
//*****************************************************************************		
	protected int getStdCaptionHight(
		Graphics g)
//*****************************************************************************		
	{
		return 2 * LogicToPixelY(1) - 1;		
	}	

//*****************************************************************************		
	protected void drawTextFrame(
		Graphics g,
		Color BgColor,
		Color TextColor,
		Color FrameColor, 				
		int iXo, int iYo, 
		int iWidth, int iHeight,
		String strText,
		int iFrameType,
		int iAlignX,
		int iAlignY)		
//*****************************************************************************		
	{
		int w = iWidth;              
		int h = iHeight;

		// fill area with background color
		g.setColor(BgColor);				
		g.fillRect(iXo, iYo, w, h);

		// draw frame		
		g.setColor(FrameColor);
		int d = 0;
		if (iFrameType > FbdElemBase.TextFrameTypeNo)
		{
			g.drawRect(iXo, iYo, w, h);
			d += 1;
		}
		if (iFrameType > FbdElemBase.TextFrameTypeSingle)
		{
			if (isOverview())
			{
				g.drawRect(iXo+1, iYo+1, w-2, h-2);
				d += 1;										
			}
			else
			{
				g.drawRect(iXo+2, iYo+2, w-4, h-4);
				d += 2;										
			}
		}
		
		// draw text
		drawTextArea(g, TextColor, iXo+d, iYo+d, w-2*d, h-2*d, strText, iAlignX, iAlignY);
	}

//*****************************************************************************		
	private void drawTextArea(
		Graphics g,
		Color TextColor,
		int iXo, int iYo, 
		int iWidth, int iHeight,
		String strText,
		int iAlignX,
		int iAlignY)		
//*****************************************************************************		
	{
		// draw text
		if (strText != null && strText.length() > 0)
		{
			g.setColor(TextColor);			
			int iStart = 0;
			int iEnd = 1;
			int iLine = 0;
			String strLine, strFormat;
			while (iEnd > 0 && iStart < strText.length())
			{
				iEnd = strText.indexOf("\n", iStart);
				if (iEnd > 0)
				{
					if (strText.charAt(iEnd) < 32)
						strLine = strText.substring(iStart, iEnd-1); // remove line feed
					else
						strLine = strText.substring(iStart, iEnd);
					iStart = iEnd + 1;
				}
				else
					strLine = strText.substring(iStart);
				
				strFormat = formatText(g, iWidth, strLine);					

				FontMetrics fm = g.getFontMetrics();
				int ws  = fm.stringWidth(strFormat);
				int hs  = fm.getHeight();
				int des = fm.getDescent();

				int y = iYo;
				int x = iXo;								
				if (iAlignY == FbdElemBase.TextAlignYTop)
					y += iLine * hs + hs;
				else if (iAlignY == FbdElemBase.TextAlignYCenter)
					y += (iHeight + hs + 1) / 2;

				y -= des;

				if (iAlignX == FbdElemBase.TextAlignXLeft)
					x += 0;
				else if (iAlignX == FbdElemBase.TextAlignXCenter)					
					x += (iWidth - ws)/2;
				else if (iAlignX == FbdElemBase.TextAlignXRight)					
					x += iWidth - ws;				
				
				
//				if (y + des <= iYo + iHeight && y + des - hs >= iYo)
					g.drawString(strFormat, x, y);

			iLine++;
			}
		}
	}
//*****************************************************************************		
	protected String formatText(
		Graphics g,
		int iWidth, 
		String strText)
//*****************************************************************************		
	{
		if (strText != null)
		{
			FontMetrics fm = g.getFontMetrics();
			int ws = fm.stringWidth(strText);
			if (ws > iWidth)
			{
				String str;
				int ws2 = fm.stringWidth("..");
				StringBuffer ToLongBuffer = new StringBuffer(strText);
				while (ToLongBuffer.length() > 0 && ws > iWidth)
				{
					ToLongBuffer.setLength(ToLongBuffer.length() - 1);
					ws = fm.stringWidth(ToLongBuffer.toString()) + ws2;
				}
				str = ToLongBuffer.toString() + "..";
				return str;
			}
		}
		return strText;
	}
	
//*****************************************************************************		
	protected void drawArrow(
		Graphics g,
		Color FgColor,
		int iXo, int iYo, 
		int Height,
		boolean bPointRight,
		boolean bDouble)
//*****************************************************************************		
	{
		g.setColor(FgColor);

		int h = Height - 2;
		h += (h + 1) % 2;
		int x1 = iXo + 1;
		int y1 = iYo + 1;
		int x2 = x1 + h / 2;
		int y2 = y1 + h / 2;				
		int y3 = y1 + (h - 1);

		if (!bPointRight)
		{
			int xTmp = x1;
			x1 = x2;
			x2 = xTmp;
		}
		
		int dx = 0;
		g.drawLine(x1+dx, y1, x2+dx, y2);
		g.drawLine(x1+dx, y3, x2+dx, y2);
		if (!isOverview())
		{
			dx += 1;
			g.drawLine(x1+dx, y1, x2+dx, y2);
			g.drawLine(x1+dx, y3, x2+dx, y2);
		}
		if (bDouble)
		{
			dx += 3;
			g.drawLine(x1+dx, y1, x2+dx, y2);
			g.drawLine(x1+dx, y3, x2+dx, y2);
			if (!isOverview())
			{
				dx += 1;
				g.drawLine(x1+dx, y1, x2+dx, y2);
				g.drawLine(x1+dx, y3, x2+dx, y2);
			}
		}
	}
	
//*****************************************************************************		
	public void updatePosition()
	{
		int xo  = LogicToPixelX(m_iXo);
		int yo  = LogicToPixelY(m_iYo);
		int szy = m_iHeight == 0 ? 1 : LogicToPixelY(m_iHeight);
		int szx = m_iWidth == 0  ? 1 : LogicToPixelX(m_iWidth);		

		if (szy < 0)
		{
			yo = yo + szy;
			szy = -szy;
		}

		if (szx < 0)
		{
			xo = xo + szx;
			szx = -szx;
		}

		if ((getClassId() & FbdElemBase.ClassId_Branch) != 0)
		{
			setLocation(xo, yo-2);
			setSize(szx, 5);						
		}
		else if ((getClassId() & FbdElemBase.ClassId_BranchAlt) != 0)
		{
			setLocation(xo, yo-1);
			setSize(szx, 2);									
		}
		else
		{
			setLocation(xo, yo);		
			setSize(szx, szy);			
		}
	}
}