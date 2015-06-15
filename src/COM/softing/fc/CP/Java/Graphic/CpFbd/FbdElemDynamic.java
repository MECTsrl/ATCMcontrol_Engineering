/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdElemDynamic.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FbdElemDynamic.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdElemDynamic.java $
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
 * $History: FbdElemDynamic.java $
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
 * User: Scd          Date: 19.03.01   Time: 15:25
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 4  *****************
 * User: So           Date: 23.07.99   Time: 11:18
 * Updated in $/4Control/COM/softing/fc/CP/Java/Graphic/CPFBD
 * awt poup menu replaced by own "context" menu
 * 
 * *****************  Version 5  *****************
 * User: Mkr          Date: 15.07.99   Time: 13:10
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 4  *****************
 * User: Mkr          Date: 14.07.99   Time: 18:22
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 3  *****************
 * User: Mkr          Date: 6.07.99    Time: 19:10
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 2  *****************
 * User: Mkr          Date: 2.07.99    Time: 15:02
 * Updated in $/COM/softing/fc/CP/Java/Graphic/CpFbd
 *==
 ******************************************************************************
H<<*/

import java.awt.*;
import java.awt.event.*;

//*****************************************************************************
public abstract class FbdElemDynamic extends FbdElemBase 
	implements ActionListener, ItemListener, MouseListener
{
	protected int m_id;                 // 4C id of object 		
	protected boolean m_bIsSelectable;  // object can be selected
	protected boolean m_bIsSelected;    // object is selected	
	protected boolean m_bIsBlocked;     // object is blocked
	protected boolean m_bIsForced;      // object is forced
	protected boolean m_bIsActive;      // object is active
	protected int     m_iQuality;       // object's quality	

	protected CpContext m_Menu;				
//*****************************************************************************
	public FbdElemDynamic(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 boolean bIsComment,       // if true, object is displayed as comment
		 int id,				   // 4C id		 
		 boolean bIsSelectable)    // if true, object is selectable		 
//*****************************************************************************		
	{	
		super(iXo, iYo, iWidth, iHeight, strName, bIsComment);
		m_ClassId   |= ClassId_Dynamic;		
		m_iQuality   = FbdPicture.QualityBad;
		m_bIsBlocked = false;
		m_bIsForced  = false;		
		m_bIsActive  = false;
		m_bIsSelected   = false;
		m_bIsSelectable = !bIsComment && bIsSelectable;		
		m_id = id;
		m_Menu = null;				

		if (isSelectable())
			addMouseListener(this);				
	}

//*****************************************************************************			
	public int     getId()        { return m_id;             }	
	public boolean isSelected()	  { return m_bIsSelected;	 }
	public boolean isSelectable() {	return m_bIsSelectable;	 }
	public boolean isForced()     { return m_bIsForced;      }
	public boolean isBlocked()    { return m_bIsBlocked;     }
	public boolean isActive()     { return m_bIsActive;	     }
	public int     getQuality()   { return m_iQuality;       }

	
//*****************************************************************************		
	public void setSelection(boolean bSelect)
//*****************************************************************************		
	{
		if (m_bIsSelectable && bSelect != m_bIsSelected)
		{
			m_bIsSelected = bSelect;
			repaint();
		}
	}
	
//*****************************************************************************		
	public void setStatus(int iQuality, boolean bActive, boolean bForce, boolean bBlock)
//*****************************************************************************		
	{
		if (!isComment())
		{
			boolean bUpdate = false;
			if (iQuality != m_iQuality)
			{
				m_iQuality  = iQuality;
				bUpdate = true;
			}			
			if (m_iQuality != FbdPicture.QualityBad)
			{
				if (bForce != m_bIsForced)
				{
					m_bIsForced  = bForce;
					m_bIsBlocked = m_bIsForced ? false : m_bIsBlocked;							
					bUpdate = true;
				}
				if (bBlock != m_bIsBlocked)
				{
					m_bIsBlocked = bBlock;
					m_bIsForced = m_bIsBlocked ? false : m_bIsForced;
					bUpdate = true;				
				}	

				if (bActive != m_bIsActive)
				{
					m_bIsActive = bActive;
					bUpdate = true;
				}
			}
			
			if (bUpdate)
				update(getGraphics()); 			
		}
	}

//*****************************************************************************		
	protected boolean canOperate()
//*****************************************************************************		
	{
		FbdPicture pic = (FbdPicture)this.getParent();
		return pic.canOperate();
	}

//*****************************************************************************		
	public void PrePaint(Graphics g)
//*****************************************************************************		
	{
		super.PrePaint(g);
	}

//*****************************************************************************		
	public void PostPaint(Graphics g)
//*****************************************************************************		
	{
		// draw selection frame if object is selected
		drawSelectionFrame(g);			
		
		super.PostPaint(g);
	}
	
//*****************************************************************************		
	protected Color getForgroundColor()
//*****************************************************************************		
	{
		if (isComment())
		{
			return FbdPicture.ColorElemComment;
		}
		else
		{
			if (isForced())
				return FbdPicture.ColorElemForced;	
			else if (isBlocked())
				return FbdPicture.ColorElemBlocked;	
			else
				return FbdPicture.ColorElemFgStd;					
		}
			
	}
	
//*****************************************************************************		
	protected Color getBackgroundColor()
//*****************************************************************************		
	{
		if (isAutomatic())
			return FbdPicture.ColorElemDynAutomatic;				
		else
			return FbdPicture.ColorElemDynManual;
	}

//*****************************************************************************		
	protected Color getBackgroundColorCaption()
//*****************************************************************************		
	{
		if (isActive())
			return FbdPicture.ColorElemCaptActive;
		else
			return super.getBackgroundColorCaption();
	}

//*****************************************************************************		
	protected void drawSelectionFrame(Graphics g)
//*****************************************************************************		
	{
		if (isSelected())
		{
			g.setColor(FbdPicture.ColorSelect);

			int ll  = 3;      // length of a line segment
			int ll2 = 2 * ll; 
			int w = getSize().width;
			int h = getSize().height;					
			
			// Draw horizontal dotted lines.
			int y1 = 0;
			int y2 = h - 1;
			for (int x = 0; x < w-ll; x += ll2)
			{
				g.drawLine(x, y1, x+ll, y1);
				g.drawLine(x, y2, x+ll, y2);
			}

			// Draw vertical dotted lines.
			int x1 = 0;
			int x2 = w - 1;
			for (int y = 0; y < h-ll; y += ll2)
			{
				g.drawLine(x1, y, x1, y+ll);
				g.drawLine(x2, y, x2, y+ll);
			}
		}
	}	

//*****************************************************************************		
	protected void drawQuality(Graphics g, int x1, int y1, int x2, int y2)
//*****************************************************************************		
	{
		if ((getId() >= 0) && (m_iQuality != FbdPicture.QualityGood))
		{
			if (m_iQuality == FbdPicture.QualityUncertain)
				g.setColor(FbdPicture.ColorQualityUncertain);
			else
				g.setColor(FbdPicture.ColorQualityBad);

			g.drawLine(x1, y1, x2, y2);
			g.drawLine(x1, y2, x2, y1);						
		}
	}	
		
	public void itemStateChanged(ItemEvent e)
	{
		FbdPicture pic = (FbdPicture)this.getParent();
		CheckboxContextMenu item = (CheckboxContextMenu)e.getSource();
		if (e.getStateChange() == e.SELECTED)
			pic.sendCommand(item.getActionCommand(), new String("" + m_id), "add");
		else
			pic.sendCommand(item.getActionCommand(), new String("" + m_id), "remove");			
	}
	
	public void actionPerformed(ActionEvent e) 
	{
		FbdPicture pic = (FbdPicture)this.getParent();
		pic.sendCommand(e.getActionCommand(), new String("" + m_id), "add");
	}
	
	public void mousePressed(MouseEvent e)
	{
		if ((e.getModifiers() & e.BUTTON1_MASK) != 0)
		{
			FbdPicture pic = (FbdPicture)this.getParent();
			pic.elementClicked(this, e.isControlDown(), e.isShiftDown());
		}
		if ((e.getModifiers() & e.BUTTON3_MASK) != 0)
		{
			showContextMenu(e.getPoint().x, e.getPoint().y);
		}
	}

	public void mouseClicked(MouseEvent e) {;}
	public void mouseReleased(MouseEvent e) {;}
	public void mouseEntered(MouseEvent e) {;}
	public void mouseExited(MouseEvent e) {;}		
	
	protected abstract void showContextMenu(int x, int y);	
}