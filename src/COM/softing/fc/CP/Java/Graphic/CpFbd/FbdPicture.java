/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdPicture.java 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FbdPicture.java $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CP/Java/Graphic/CpFbd/FbdPicture.java $
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
 * $History: FbdPicture.java $
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
 * User: Scd          Date: 19.03.01   Time: 15:37
 * Created in $/4Control/COM/softing/fc/CP/Java/Graphic/CpFbd
 * 
 * *****************  Version 8  *****************
 * User: Mkr          Date: 4.08.99    Time: 18:24
 * Updated in $/4Control/COM/softing/fc/CP/Java/Graphic/CPFBD
 * 
 * *****************  Version 7  *****************
 * User: So           Date: 23.07.99   Time: 11:19
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
import java.util.*;

public class FbdPicture extends Panel
	implements ActionListener, ItemListener, MouseListener	
{
	public final static Color ColorPicFgStd          = Color.black;
	public final static Color ColorPicAutomatic      = new Color(255, 255, 200);
	public final static Color ColorPicManual         = new Color(230, 242, 255);

	public final static Color ColorElemRubberline    = new Color(0, 255, 255); 		

	public final static Color ColorElemAutomatic     = new Color(255, 255, 230);	
	public final static Color ColorElemManual        = new Color(245, 250, 255);	
	public final static Color ColorElemDynAutomatic  = ColorPicAutomatic;	
	public final static Color ColorElemDynManual     = ColorPicManual;	
	
	public final static Color ColorElemCaptAutomatic = new Color(230, 230, 175);	
	public final static Color ColorElemCaptManual    = new Color(195, 215, 255);	
	public final static Color ColorElemCaptActive    = new Color(200, 200, 200);
	public final static Color ColorElemForced        = new Color(  0, 128,   0);
	public final static Color ColorElemBlocked       = new Color(255,   0,   0);
	public final static Color ColorElemComment       = new Color(200, 200, 200); 	
	
	public final static Color ColorElemFgStd         = ColorPicFgStd;

	public final static Color ColorQualityUncertain  = Color.black;
	public final static Color ColorQualityBad        = new Color(255,   0,  0);	

	public final static Color ColorSelect            = new Color(  0, 128, 255); 	
	public final static Color ColorError             = new Color(255,   0,   0); 	

	private int m_iZoomFactor;
	private int m_iUnitToPixelX = 10;
	private int m_iUnitToPixelY = 10;
	
	private boolean m_bIsAutomatic;
	
	public final static int QualityGood      = 0;
	public final static int QualityUncertain = 1;
	public final static int QualityBad       = 2;	
	
	private boolean m_bOperate;	
	private int m_iNextIndex;
	
	CpContext m_Menu = null;
	
//*****************************************************************************			
	public FbdPicture(boolean bOperate)
//*****************************************************************************				
	{
		m_iNextIndex = 0;
		m_bOperate = bOperate;
		m_bIsAutomatic = true;
		
		setForeground(ColorPicFgStd);
		setBackground(ColorPicAutomatic);				
		
		setLayout(null);
		m_Menu = null;		
		addMouseListener(this);	
		
		setZoomFactor(100);
	}

//*****************************************************************************			
	public void setZoomFactor(int ZoomFactor)
//*****************************************************************************				
	{
		m_iZoomFactor = ZoomFactor;
		
		Font f = new Font("SansSerif", Font.PLAIN, 12);
		FontMetrics fm = this.getFontMetrics(f);
		int h = (fm.getHeight() +  4) / 2;
		int h_new = (h * m_iZoomFactor) / 100;
		
		if (m_iZoomFactor != 100)
		{
			f = new Font("SansSerif", Font.PLAIN, 2 * h_new - 3);
		}

		m_iUnitToPixelX	= h_new;
		m_iUnitToPixelY	= h_new;

		setFont(f);
		
		int iCount = 0;		
		FbdElemBase elem = null;
		int wx_max = 0;
		int wy_max = 0;
		while ((elem = getNextElement(0xffff, (iCount == 0))) != null) 
		{
			elem.updatePosition();
			int wx = elem.getLocation().x + elem.getSize().width;
			int wy = elem.getLocation().y + elem.getSize().height;	
			wx_max = Math.max(wx, wx_max);
			wy_max = Math.max(wy, wy_max);
			iCount++;
		}

		setSize(wx_max + 10, wy_max + 10);		
	}	

//*****************************************************************************		
	public boolean isAutomatic() {return m_bIsAutomatic; }
	public boolean canOperate() {return m_bOperate; }

//*****************************************************************************		
	protected Color getBackgroundColor()
//*****************************************************************************		
	{
		if (isAutomatic())
			return ColorPicAutomatic;
		else
			return ColorPicManual;			
	}
	
	
//*****************************************************************************			
	private int[][] parseActiveElements(
		String strActiveElements)
				throws FbdUpdateException
//*****************************************************************************			
	{
		int[] ids[]= {null, null};
		
		if (strActiveElements	!= null)
		{
			int nElements = 0; 
			int nTotElements = 0;

			StringTokenizer tokenList = new StringTokenizer(strActiveElements, ";");
			int nTokens = tokenList.countTokens();
			if (nTokens < 3)
				throw new FbdUpdateException(strActiveElements);

			for (int ii = 0; ii < 2; ii++)	  // right now we need the list of active steps and actions only
			{
				try { nElements = Integer.parseInt(tokenList.nextToken()); }
				catch (NumberFormatException e){ throw new FbdUpdateException(strActiveElements); }			
				nTotElements += nElements;
				if (nTokens < nTotElements + 3)
					throw new FbdUpdateException(strActiveElements);

				if (nElements > 0)
				{
					ids[ii] = new int [nElements];			
					for (int jj = 0; jj < nElements; jj++)
					{
						try { ids[ii][jj] = Integer.parseInt(tokenList.nextToken()); }
						catch (NumberFormatException e){ throw new FbdUpdateException(strActiveElements); }
					}
				}
			}
		}
		return ids;
	}

//*****************************************************************************			
	private int[] parseElements(
		String strElements)
				throws FbdUpdateException
//*****************************************************************************			
	{
		int ids[]= null;		
		if (strElements	!= null)
		{
			StringTokenizer tokenList = new StringTokenizer(strElements, ";");
			int nTokens = tokenList.countTokens();
			if (nTokens > 0)
			{
				ids = new int [nTokens];
				for (int jj = 0; jj < nTokens; jj++)
				{
					try { ids[jj] = Integer.parseInt(tokenList.nextToken()); }
					catch (NumberFormatException e){ throw new FbdUpdateException(strElements); }
				}
			}
		}
		return ids;
	}	
//*****************************************************************************			
	public void update(
		int iQuality,					   					   
		boolean bAutomatic,
		String strActiveElementIds,
		String strForcedTransitionIds,
		String strBlockedTransitionIds,
		String strForcedActionIds,
		String strBlockedActionIds) 		
			throws FbdUpdateException
//*****************************************************************************					
	{
		FbdElemDynamic elem = null;
		int iCount = 0;

		if (iQuality == QualityBad)
		{
			while ((elem = getNextDynamicElement(iCount == 0)) != null)
			{
				elem.setStatus(iQuality, false, false, false);			
				iCount++;
			}
		}
		else
		{
			// automatic/manual mode -> different background color
			if (bAutomatic != m_bIsAutomatic)
			{
				m_bIsAutomatic = bAutomatic;
				if (m_bIsAutomatic)
					setBackground(ColorPicAutomatic);		
				else		
					setBackground(ColorPicManual);
			}		
				
			// parse strings containing id lists
			int[][] ids = parseActiveElements(strActiveElementIds);
			int[] ActiveStepIds        = ids[0];

			int[] ActiveActionIds      = ids[1];			
			int[] ForcedActionIds      = parseElements(strForcedActionIds);
			int[] BlockedActionIds     = parseElements(strBlockedActionIds);
		
			int[] ForcedTransitionIds  = parseElements(strForcedTransitionIds);		
			int[] BlockedTransitionIds = parseElements(strBlockedTransitionIds);

			// update graphic elements
			while ((elem = getNextDynamicElement(iCount == 0)) != null)
			{
				boolean bFoundActive  = false;
				boolean bFoundForced  = false;
				boolean bFoundBlocked = false;										
				if ((elem.getClassId() & FbdElemBase.ClassId_Step) != 0)
				{
					if (ActiveStepIds != null)
					{
						for (int jj = 0; jj < ActiveStepIds.length && !bFoundActive; jj++)
						{
							if (ActiveStepIds[jj] == elem.getId())
								bFoundActive = true;
						}
					}
				}
				else if ((elem.getClassId() & FbdElemBase.ClassId_Action) != 0)
				{
					if (ForcedActionIds != null)
					{
						for (int jj = 0; jj < ForcedActionIds.length && !bFoundForced; jj++)
						{
							if (ForcedActionIds[jj] == elem.getId())
								bFoundForced = true;
						}
					}
					if (!bFoundForced && BlockedActionIds != null)
					{
						for (int jj = 0; jj < BlockedActionIds.length && !bFoundBlocked; jj++)
						{
							if (BlockedActionIds[jj] == elem.getId())
								bFoundBlocked = true;
						}
					}
					if (ActiveActionIds != null)
					{
						for (int jj = 0; jj < ActiveActionIds.length && !bFoundActive; jj++)
						{
							if (ActiveActionIds[jj] == elem.getId())
								bFoundActive = true;
						}
					}
				}
				else if ((elem.getClassId() & FbdElemBase.ClassId_Transition ) != 0)
				{
					if (ForcedTransitionIds != null)
					{
						for (int jj = 0; jj < ForcedTransitionIds.length && !bFoundForced; jj++)
						{
							if (ForcedTransitionIds[jj] == elem.getId())
								bFoundForced = true;
						}
					}
					if (!bFoundForced && BlockedTransitionIds != null)
					{
						for (int jj = 0; jj < BlockedTransitionIds.length && !bFoundBlocked; jj++)
						{
							if (BlockedTransitionIds[jj] == elem.getId())
								bFoundBlocked = true;
						}
					}
				}
				elem.setStatus(iQuality, bFoundActive, bFoundForced, bFoundBlocked);			
				iCount++;
			}
		}
	}
	
//*****************************************************************************		
	public void addLine(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 boolean bIsComment,       // if true, object is displayed as comment
		 boolean bIsRubberline)    // if true => rubber line
//*****************************************************************************				
	{
		addFbdElem(new FbdElemLine(iXo, iYo, iWidth, iHeight, bIsComment, bIsRubberline), bIsRubberline);
	}

//*****************************************************************************		
	public void addStep(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 int id,				   // 4C id
		 boolean bIsComment,       // if true, object is displayed as comment
		 boolean bIsInitialStep)   // if true -> initial step
//*****************************************************************************				
	{
		addFbdElem(new FbdElemStep(iXo, iYo, iWidth, iHeight, strName, bIsComment, id, bIsInitialStep), false);
	}
//*****************************************************************************		
	public void addFunctionBlock(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 boolean bIsComment,       // if true, object is displayed as comment
		 String strTypeName,
		 FbdPinList pinlist)       // array of pin information (name, position, ... )		
//*****************************************************************************				
	{
		addFbdElem(new FbdElemFunctionBlock(iXo, iYo, iWidth, iHeight, strName, bIsComment, strTypeName, pinlist), false);
	}
//*****************************************************************************		
	public void addFunction(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 boolean bIsComment,       // if true, object is displayed as comment
		 String strTypeName,
		 FbdPinList pinlist)       // array of pin information (name, position, ... )		
//*****************************************************************************				
	{
		addFbdElem(new FbdElemFunction(iXo, iYo, iWidth, iHeight, bIsComment, strTypeName, pinlist), false);
	}	
//*****************************************************************************		
	public void addJump(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 boolean bIsComment)       // if true, object is displayed as comment
//*****************************************************************************				
	{
		addFbdElem(new FbdElemJump(iXo, iYo, iWidth, iHeight, strName, bIsComment), false);
	}
//*****************************************************************************		
	public void addReturn(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 boolean bIsComment)       // if true, object is displayed as comment
//*****************************************************************************				
	{
		addFbdElem(new FbdElemReturn(iXo, iYo, iWidth, iHeight, "RETURN", bIsComment), false);
	}
//*****************************************************************************		
	public void addConnector(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 boolean bIsComment)       // if true, object is displayed as comment
//*****************************************************************************				
	{
		addFbdElem(new FbdElemConnector(iXo, iYo, iWidth, iHeight, strName, bIsComment), false);
	}
//*****************************************************************************		
	public void addContinuation(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 boolean bIsComment)       // if true, object is displayed as comment
//*****************************************************************************				
	{
		addFbdElem(new FbdElemContinuation(iXo, iYo, iWidth, iHeight, strName, bIsComment), false);
	}
//*****************************************************************************		
	public void addLabel(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 boolean bIsComment)       // if true, object is displayed as comment
//*****************************************************************************				
	{
		addFbdElem(new FbdElemLabel(iXo, iYo, iWidth, iHeight, strName, bIsComment), false);
	}	
//*****************************************************************************		
	public void addVarRead(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 boolean bIsComment)       // if true, object is displayed as comment
//*****************************************************************************				
	{
		addFbdElem(new FbdElemVarRead(iXo, iYo, iWidth, iHeight, strName, bIsComment), false);
	}
//*****************************************************************************		
	public void addVarWrite(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 boolean bIsComment)       // if true, object is displayed as comment
//*****************************************************************************				
	{
		addFbdElem(new FbdElemVarWrite(iXo, iYo, iWidth, iHeight, strName, bIsComment), false);
	}	
//*****************************************************************************		
	public void addVarFeedback(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 boolean bIsComment)       // if true, object is displayed as comment
//*****************************************************************************				
	{
		addFbdElem(new FbdElemVarFeedback(iXo, iYo, iWidth, iHeight, strName, bIsComment), false);
	}		
//*****************************************************************************		
	public void addAction(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 int id,				   // 4C id
		 boolean bIsComment)       // if true, object is displayed as comment
//*****************************************************************************				
	{
		addFbdElem(new FbdElemAction(iXo, iYo, iWidth, iHeight, strName, bIsComment, id), false);
	}

//*****************************************************************************		
	public void addActionBlock(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 int id,				   // 4C id
		 boolean bIsComment,       // if true, object is displayed as comment
		 String strQual)
//*****************************************************************************				
	{
		addFbdElem(new FbdElemActionBlock(iXo, iYo, iWidth, iHeight, strName, bIsComment, id, strQual), false);
	}

//*****************************************************************************		
	public void addTransition(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strName,           // name
		 int id,				   // 4C id
		 boolean bIsComment)       // if true, object is displayed as comment
//*****************************************************************************				
	{
		addFbdElem(new FbdElemTransition(iXo, iYo, iWidth, iHeight, strName, bIsComment, id), false);
	}

//*****************************************************************************		
	public void addBranch(
		int iXo,                  // origin
		int iYo, 
		int iWidth,               // size
		int id,                   // 4C id
		boolean bIsComment,       // if true, object is displayed as comment
		boolean bIsAlternative)   // if true -> alternative divider
                                  // if false -> simultanous diveder 		 
//*****************************************************************************				
	{
		if (bIsAlternative)
			addFbdElem(new FbdElemBranchAlt(iXo, iYo, iWidth, bIsComment), false);		
		else
			addFbdElem(new FbdElemBranch(iXo, iYo, iWidth, bIsComment), false);		
	}

//*****************************************************************************		
	public void addComment(
		 int iXo,                  // origin
		 int iYo, 
		 int iWidth,               // size
		 int iHeight, 
		 String strText,           // comment text
		 int id,				   // 4C id
		 boolean bIsComment)       // if true, object is displayed as comment
//*****************************************************************************				
	{
		addFbdElem(new FbdElemComment(iXo, iYo, iWidth, iHeight, strText, bIsComment), true);
	}
//*****************************************************************************		
	private void addFbdElem(FbdElemBase elem, boolean bTop)
//*****************************************************************************				
	{
		if (bTop)
			add(elem, 0);
		else
			add(elem);
		
		elem.updatePosition();
	}

//*****************************************************************************		
	public void elementClicked(
		FbdElemDynamic elemDyn,
		boolean bIsControlDown,
		boolean bIsShiftDown)
//*****************************************************************************				
	{
		if (bIsControlDown)
		{
			elemDyn.setSelection(!elemDyn.isSelected());
		}
		else
		{
			selectAll(false, null, elemDyn);
			elemDyn.setSelection(true);			
		}
	}

//*****************************************************************************		
	public void selectAll(
		 boolean bSelect, 
		 String strClassFilter, 
		 FbdElemBase NotThisOne )
//*****************************************************************************				
	{
		int iCount = 0;		
		FbdElemDynamic elem = null;
		Class cl = null;
		while ((elem = getNextDynamicElement(iCount == 0)) != null)
		{
			if (NotThisOne == null || NotThisOne != elem)
			{
				cl = elem.getClass();
				if (strClassFilter == null || cl.getName().indexOf(strClassFilter) == 0)
					elem.setSelection(bSelect);
				else
					elem.setSelection(!bSelect);
			}
			iCount++;
		}
	}		
	
//*****************************************************************************				
	public int LogicToPixelX(int x)	{ return x * m_iUnitToPixelX; }
	public int LogicToPixelY(int y)	{ return y * m_iUnitToPixelY; }
	public boolean isOverview()	    { return m_iUnitToPixelY < 8; }		

	public void mousePressed(MouseEvent e)
	{
		if ((e.getModifiers() & e.BUTTON1_MASK) != 0)
		{
			selectAll(false, null, null);
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
	
	private void showContextMenu(int x, int y)
	{
		if (canOperate())
		{
			CpContext m_Menu = new CpContext(new Frame());
			addStdContextMenu(m_Menu);
			
			Point pt = getLocationOnScreen();
			m_Menu.setLocation(pt.x+x,pt.y+y);
			m_Menu.pack();	
			m_Menu.show();			
		}
	}

	public CpContext addStdContextMenu(CpContext menu)
	{
		CheckboxContextMenu mi;
		ContextMenu mi2;
		
		mi2 = new ContextMenu("Force All Transitions");
		mi2.setActionCommand("forceTransitionAll");									
		mi2.setEnabled(true);			
		mi2.addActionListener(this);
		menu.add(mi2);

		mi2 = new ContextMenu("Block All Transitions");
		mi2.setActionCommand("blockTransitionAll");									
		mi2.setEnabled(true);			
		mi2.addActionListener(this);
		menu.add(mi2);
			
		mi2 = new ContextMenu("Revert All Transitions");
		mi2.setActionCommand("revertTransitionAll");									
		mi2.setEnabled(true);			
		mi2.addActionListener(this);
		menu.add(mi2);
		
		menu.addSeparator();
		
		mi2 = new ContextMenu("Block All Actions");
		mi2.setActionCommand("blockActionAll");									
		mi2.setEnabled(true);			
		mi2.addActionListener(this);
		menu.add(mi2);
			
		mi2 = new ContextMenu("Force All Actions");
		mi2.setActionCommand("forceActionAll");									
		mi2.setEnabled(true);			
		mi2.addActionListener(this);
		menu.add(mi2);

		mi2 = new ContextMenu("Revert All Actions");
		mi2.setActionCommand("revertActionAll");									
		mi2.setEnabled(true);			
		mi2.addActionListener(this);
		menu.add(mi2);
		
		menu.addSeparator();
		
		mi = new CheckboxContextMenu("Manual Mode");
		mi.setActionCommand("manualMode");
		mi.setState(!isAutomatic());
		mi.setEnabled(true);
		mi.addItemListener(this);
		menu.add(mi);

		mi2 = new ContextMenu("Single SFC Cycle");
		mi2.setActionCommand("doNext");
		mi2.setEnabled(!isAutomatic());
		mi2.addActionListener(this);
		menu.add(mi2);
			
		menu.addSeparator();
		
		mi = new CheckboxContextMenu("Zoom 100%");
		mi.setActionCommand("zoom100");
		mi.setState(m_iZoomFactor == 100);
		mi.setEnabled(true);
		mi.addItemListener(this);
		menu.add(mi);			

		mi = new CheckboxContextMenu("Zoom 75%");
		mi.setActionCommand("zoom75");
		mi.setState(m_iZoomFactor == 75);
		mi.setEnabled(true);
		mi.addItemListener(this);
		menu.add(mi);			

		mi = new CheckboxContextMenu("Zoom 50%");
		mi.setActionCommand("zoom50");
		mi.setState(m_iZoomFactor == 50);
		mi.setEnabled(true);
		mi.addItemListener(this);
		menu.add(mi);
		
		return menu;
	}
	
	public void itemStateChanged(ItemEvent e)
	{
		CheckboxContextMenu item = (CheckboxContextMenu)e.getSource();
		String command = item.getActionCommand();

		if (command.equals("manualMode"))
		{
			if (e.getStateChange() == e.SELECTED)
				sendCommand(command, "", "1");
			else
				sendCommand(command, "", "0");				
		}
		else if (command.indexOf("zoom") == 0)
		{
			setAppletSize(Integer.parseInt(command.substring(4)));
		}
	}

	public void actionPerformed(ActionEvent e) 
	{
		String cmd = e.getActionCommand();		
		if (cmd.equals("doNext"))
			sendCommand(e.getActionCommand(), null, null);
		else if (cmd.equals("forceTransitionAll"))
			sendCommand(cmd, createElemList(FbdElemBase.ClassId_Transition, false), "");
		else if (cmd.equals("blockTransitionAll"))
			sendCommand(cmd, createElemList(FbdElemBase.ClassId_Transition, false), "");
		else if (cmd.equals("revertTransitionAll"))
			sendCommand(cmd, "", "");
		else if (cmd.equals("blockActionAll"))
			sendCommand(cmd, createElemList(FbdElemBase.ClassId_Action, false), "");
		else if (cmd.equals("forceActionAll"))
			sendCommand(cmd, createElemList(FbdElemBase.ClassId_Action, false), "");
		else if (cmd.equals("revertActionAll"))
			sendCommand(cmd, "", "");		
	}

	public void setAppletSize(int iZoomFactor) 
	{
		FbdApplet applet = (FbdApplet)this.getParent();
		applet.setAppletSize(iZoomFactor);
	}

	public void sendCommand(String cmd, String elemList, String param) 
	{
		FbdApplet applet = (FbdApplet)this.getParent();
		applet.sendCommand(cmd, elemList, param);
	}

	private FbdElemDynamic getNextDynamicElement(boolean bFirst) 
	{
		return (FbdElemDynamic)getNextElement(FbdElemBase.ClassId_Dynamic, bFirst); 
	}	

	private FbdElemBase getNextElement(int ClassFilter, boolean bFirst) 
	{
		if (bFirst)
			m_iNextIndex = 0;
		
		Object obj;
		int iCount = getComponentCount();
		while (m_iNextIndex < iCount)
		{
			obj = getComponent(m_iNextIndex);
			m_iNextIndex++;			
			if (obj.getClass().getName().indexOf("Fbd") >= 0)
			{
				FbdElemBase elem = (FbdElemBase)obj;
				if ((elem.getClassId() & ClassFilter) != 0)
					return elem;
			}
		}
		return null;
	}	
	
	public String createElemList(int ClassFilter, boolean bSelectedOnly) 
	{
		int iCount = 0;
		int iCountSelected = 0;		
		String ElemList = new String("");		
		FbdElemDynamic elem = null;
		while ((elem = getNextDynamicElement(iCount == 0)) != null)
		{
			if ((elem.getId() >= 0) && (elem.getClassId() & ClassFilter) != 0)
			{
				if (!bSelectedOnly || (bSelectedOnly && elem.isSelected()))
				{
					if (iCountSelected > 0)
						ElemList += ";";
					ElemList += elem.getId();
					iCountSelected++;
				}
			}
			iCount++;			
		}
		return ElemList;
	}
}