/////////////////////////////////////////////////////////////////////////////
// SplitterLayout.h : SFL Layout Manager
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  Splitter layout
//


#pragma once

#if !defined(_SFL)
#define _SFL
#endif

#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
		#pragma warning(push,3)
	#endif
	#pragma warning(disable:4876)
	#pragma warning(disable:4284) // STL, not a UDT or reference to a UDT. Errors if infix notation used.
#endif


#include <foundation\SflCommon.h>
#include <foundation\Layout\DCNode.h>
#include <deque>



/////////////////////////////////////////////////////////////////////
// Resource identifiers for the special splitter cursors

namespace stingray {
namespace foundation {


//@doc


//@enum SplitterFlags |
// Configuration flags that can be applied to splitter nodes
// to control diverse aspects of their appearance.
enum SplitterFlags {
	SplitterRealtimeDrag = 0x0001,	//@emem Enable real time dragging: windows are moved as the splitter is dragged, 
									//providing immediate feedback to the user.
	SplitterBottomBorder = 0x0002,	//@emem Draw the bottom border of the splitter node.
	SplitterRightBorder = 0x0004,	//@emem Draw the right border of the splitter node.
	SplitterBorders = 0x0006,		//@emem Combination of SplitterBottomBorder and SplitterRightBorder.
	SplitterNoSplitters = 0x0008	//@emem Do not provide draggable splitters. Simulates a grid layout.
};


//@enum SplitterDrawingStyle |
// Style used by the splitter node to draw the splitter bars.
enum SplitterDrawingStyle {
	DrawGlobalSetting = 0x0000,	
	DrawTraditional = 0x0001,		//@emem Traditional splitters, with pronounced 3-D effect. Similar to the MFC splitters.
	DrawFlat = 0x0002,				//@emem Splitter bars with a flatter 3-D effect, similar to the VB or VJ environments.
	DrawBorder = 0x0004				//@emem Splitter bars with no 3-D effect at all, drawn just as spaces separating the splitter cells.
};



//@interface ISplitter |
//  Provides all the accessors and methods to control the operation
//  of a splitter layout node.
//@base  | ILayoutNode
interface __declspec(uuid("8B1647C2-45E5-11d3-9D92-00C04F91E286")) ISplitter
: public ILayoutNode
{
	//@prop,property int | Get/SetSplitterWidth |
	// Sets or retrieves the thickness in pixels of the splitter bars.
	//@@syntax nWidth = pSplitter->GetSplitterWidth();
	//@@syntax pSplitter->SetSplitterWidth(nWidth);
	//@@parm int | nWidth | Thickness in pixels of the splitter bars.
	virtual void SetSplitterWidth(int nNewWidth) = 0;
	virtual int GetSplitterWidth() const = 0;

	//@prop,property DrawingStyle | Get/SetDrawingStyle |
	// Sets or retrieves the drawing style of the splitter. The drawing style
	// determines the appearance with which the splitter bars will be drawn.
	//@@syntax DWORD dwStyle = pSplitter->GetDrawingStyle();
	//@@syntax pSplitter->SetDrawingStyle(dwStyle);
	//@@parm DWORD | dwStyle | Style for drawing the splitter bars.
	//@@comm 
	// Acceptable values for this property are those from the <t SplitterDrawingStyle>
	// enumeration. 
	virtual void SetDrawingStyle(DWORD dwStyle = DrawGlobalSetting) = 0;
	virtual DWORD GetDrawingStyle() const = 0;

	//@prop,property DWORD | Get/SetSplitterFlags | 
	// Sets or retrieves the flags that control certain aspects of how 
	// the splitter behaves.
	//@@syntax DWORD dwFlags = pSplitter->GetSplitterFlags();
	//@@syntax pSplitter->SetSplitterFlags(dwFlags);
	//@@parm DWORD | dwFlags | Set of flags applied to the splitter node.
	//@@comm
	// Acceptable values are any OR-combination of values from the <t SplitterFlags>
	// enumeration.
	virtual DWORD GetSplitterFlags() const = 0;
	virtual void SetSplitterFlags(DWORD dwFlags) = 0;

	//@meth,method CPoint | GetSplitterPosition |
	// Retrieves the position of the splitter bar on the bottom-right
	// of the cell determined by the (iRow, iCol) coordinates.
	//@@syntax CPoint pt = pSplitter->GetSplitterPosition(iRow, iCol);
	//@@parm short | iRow | Row position of the cell, based on 0. Set this parameter to 
	//  -1 if only interested in position of vertical bar.
	//@@parm short | iCol | Column position of the cell, based on 0. Set this parameter to 
	//  -1 if only interested in position of horizontal bar.
	//@@rdesc Position of the splitter bar intersection, in coordinates relative to the master
	// window.
	//@@comm
	// Set iRow to -1 if you are only interested in the x coordinate of the vertical splitter bar.
	// Set iCol to -1 if you are only interested in the y coordinate of the horizontal splitter bar.
	// If either iRow or iCol is -1, the corresponding entry in the returned CPoint should be ignored.
	virtual CPoint GetSplitterPosition(short iRow, short iCol) const = 0;

	//@meth,method void | SetSplitterPosition |
	// Programatically sets the position of the splitter bar on the bottom-right
	// of the cell determined by the (iRow, iCol) coordinates.
	//@@syntax CPoint pt = pSplitter->GetSplitterPosition(iRow, iCol);
	//@@syntax pSplitter->SetSplitterPosition(iRow, iCol, pt);
	//@@parm short | iRow | Row position of the cell, based on 0. Set this parameter to 
	//  -1 if only interested in position of vertical bar.
	//@@parm short | iCol | Column position of the cell, based on 0. Set this parameter to 
	//  -1 if only interested in position of horizontal bar.
	//@@parm CPoint | pt | Position where you want to move the splitter, in coordinates relative
	// to the master window.
	//@@comm 
	// Set iRow to -1 if you are only interested in moving the vertical splitter bar.
	// Set iCol to -1 if you are only interested in moving the horizontal splitter bar.
	// If either iRow or iCol is -1, the corresponding entry in the returned CPoint will be ignored. <nl>
	// The effect of moving the splitter can be achieved with either the SetSplitterPosition method or
	// the <mf ISplitter::MoveSplitter> method. The main difference is that SetSplitterPosition receives
	// the exact position where you want to set the splitter bars, whereas MoveSplitter rather concentrates in
	// the number of units you want to move the splitter from its current position.
	virtual void SetSplitterPosition(short iRow, short iCol, CPoint ptSplitter) = 0;

	//@meth,method CSize | MoveSplitter |
	// Moves a specified splitter bar by a given displacement.
	//@@syntax CSize szActualDisplacement = pSplitter->MoveSplitter(iRow, iCol, szDelta);
	//@@parm short | iRow | Row position of the cell, based on 0. Set this parameter to 
	//  -1 if only interested in moving the vertical bar.
	//@@parm short | iCol | Column position of the cell, based on 0. Set this parameter to 
	//  -1 if only interested in moving the horizontal bar.
	//@@parm CSize | szDelta | Movement desired for the splitter bars specified with the
	// coordinates (iRow, iCol). If either of those parameters is -1, only the value 
	// in szDelta corresponding to the given coordinate is considered.
	//@@rdesc Returns the actual displacement of the splitter. It may be different than the 
	// szDelta parameter if the splitter could not be moved for some reason like out of bounds
	// problems or if the column or row has reached a width of 0.
	//@@comm
	// The effect of moving the splitter can be achieved with either the MoveSplitter method or
	// the <mf ISplitter::SetSplitterPosition> method. The main difference is that SetSplitterPosition receives
	// the exact position where you want to set the splitter bars, whereas MoveSplitter rather concentrates in
	// the number of units you want to move the splitter from its current position.
	virtual CSize MoveSplitter(short iRow, short iCol, CSize szDelta) = 0;

	//@meth,method void | SetGridDimensions |
	// Defines the dimensions of the splitter grid, so sucessive calls to
	// <mf ILayoutNode::AddLayoutNode> can distribute the new nodes in rows
	// and columns.
	//@@syntax pSplitter->SetGridDimensions(nRows, nCols);
	//@@parm short | nRows | Number of rows the grid should have.
	//@@parm short | nCols | Number of columns the grid should have.
	//@@comm
	// SetGridDimensions does not actually expands or contracts the splitter grid.
	// It rather sets the maximum number of columns and rows the splitter will accept.
	virtual void SetGridDimensions(short nRows, short nCols = 1) = 0;

	//@meth,method bool | AddPane |
	// Adds a new pane in the splitter in the position indicated by the coordinates
	// (nRow, nCol) and spanning nRowSpan rows and nColSpan columns. 
	//@@syntax bResult = pSplitter->AddPane
	//@@parm ILayoutNode* | pNode | Layout node that will occupy the area reserved
	// for the pane being created.
	//@@parm short | nRow | Row position of the topmost cell of the pane.
	//@@parm short | nCol | Column position of the leftmost cell of the pane.
	//@@parm short | nRowSpan | Number of rows occupied by the pane. Default is 1.
	//@@parm short | nColSpan | Number of columns occupied by the pane. Default is 1.
	//@@comm
	// A pane is a set of contiguous cells that are all occupied by the same layout node. 
	// No splitter bars go across a pane; it is resized as though it were a single cell.<nl>
	// Use AddPane() instead of <mf ILayoutNode::AddLayoutNode> to take advantage of the 
	// special characteristics of the splitter, like cell spanning and positioning. 
	virtual bool AddPane(ILayoutNode* pNode, short nRow, short nCol, short nRowSpan = 1, short nColSpan = 1) = 0;

	//@meth,method bool | RemovePane | 
	// Removes from the splitter grid the cell indicated by (nRow, nCol) coordinates.
	//@@parm short | nRow | Row position of the cell being removed.
	//@@parm short | nCol | Column position of the cell being removed.
	//@@rdesc true if the cell was removed successfully, false otherwise.
	//@@comm
	// The cells occupied by this pane are left empty, unless one or more rows
	// or columns are left empty, in which case those rows or columns are eliminated.
	virtual bool RemovePane(short nRow, short nCol) = 0;
};



//@class CSplitterLayout |
// Implements splitters and integrates this feature into the layout
// manager architecture.
//@supint ILayoutNode | Basic layout node functionality
//@supint ISplitter | Splitter-specific functionality
//@base | CDCLayoutBase<lt>CSplitterLayout, ISplitter<gt>
//@comm
// The splitter layout, unlike the rest of the layout algorithms, is a dynamic interactive layout
// arrangement. An application user can rearrange windows interactively, using the
// mouse. In the other layout algorithms, the layout recalculation is triggered indirectly
// by operations such as resizing the container window.<nl>
// The splitter layout node offers multiple configuration choices, from choosing the
// style used or drawing the splitter bars, to getting rid of the bars altogether,
// producing non-interactive grid layout.
class __declspec(uuid("8B1647C3-45E5-11d3-9D92-00C04F91E286")) CSplitterLayout: 
	public CDCLayoutBase<CSplitterLayout, ISplitter>
{
public:
	typedef CDCLayoutBase<CSplitterLayout, ISplitter> _LayoutBase;

	// Initialization/Destruction
	CSplitterLayout();
	virtual ~CSplitterLayout() {}

public:

	// ISplitter interface
	virtual void SetSplitterWidth(int nNewWidth) {m_nSplitterWidth = nNewWidth;}
	virtual int GetSplitterWidth() const {return m_nSplitterWidth;}

	virtual void SetDrawingStyle(DWORD dwStyle = DrawGlobalSetting) {m_dwDrawingStyle = dwStyle;}
	virtual DWORD GetDrawingStyle() const {return m_dwDrawingStyle;}

	virtual DWORD GetSplitterFlags() const {return m_dwFlags;}
	virtual void SetSplitterFlags(DWORD dwFlags) {m_dwFlags = dwFlags;}

	virtual void SetSplitterPosition(short iRow, short iCol, CPoint ptSplitter);
	virtual CPoint GetSplitterPosition(short iRow, short iCol) const;
	virtual CSize MoveSplitter(short iRow, short iCol, CSize szDelta);

	virtual void SetGridDimensions(short nRows, short nCols = 1);

	virtual bool AddPane(ILayoutNode* pNode, short nRow, short nCol, short nRowSpan = 1, short nColSpan = 1);
	virtual bool RemovePane(short nRow, short nCol);


public:
	// ILayoutNode interface overrides
	virtual CRect OnRecalcLayout(const CRect& rectDesired, bool bChildNegotiate, bool bStrictRecalc);
	virtual bool OnRealizeNode(CLayoutTransaction& aTransaction);

	virtual int AddLayoutNode (ILayoutNode* pNode);
	virtual int RemoveLayoutNode(ILayoutNode* pNode);


protected:
	// CDCLayoutBase overrides
	virtual void OnPaint(HDC hDC = NULL);
	virtual bool EraseNodeBkgnd(HDC hDC = NULL, bool bApplyToChildren = true);

protected:

	// Event map
	BEGIN_WINDOW_EVENT_MAP(CSplitterLayout)
		HANDLE_WINDOW_EVENT(WM_SETCURSOR, OnSetCursor)
		HANDLE_WINDOW_EVENT(WM_LBUTTONDOWN, OnLButtonDown)
		HANDLE_WINDOW_EVENT(WM_MOUSEMOVE, OnMouseMove)
	END_WINDOW_EVENT_MAP()

	LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


protected:

	// Implementation Member Variables
	int m_nSplitterWidth;
	DWORD m_dwDrawingStyle;
	DWORD m_dwFlags;

	short m_nMaxRow, m_nMaxCol;
	short m_nNextCol, m_nNextRow;


	// For splitter track
	bool m_bTracking;
	CRect m_rcLastRowTracker, m_rcLastColTracker;
	CSize m_szLastRowTracker, m_szLastColTracker;



protected:
	// Pane information

	struct CPaneInfo
	{
		CPaneInfo(ILayoutNode* aNode = 0, short aRow = 0, short aCol = 0, short aRowSpan = 1, short aColSpan = 1):
		m_pPaneNode(aNode),
		nRowOrigin(aRow), nColOrigin(aCol), nRowSpan(aRowSpan), nColSpan(aColSpan)
		{}
		CPaneInfo(const CPaneInfo& aInfo): 
		m_pPaneNode(aInfo.m_pPaneNode),
		nRowOrigin(aInfo.nRowOrigin), nColOrigin(aInfo.nColOrigin), nRowSpan(aInfo.nRowSpan), nColSpan(aInfo.nColSpan)
		{}

		// Data
		ILayoutNode* m_pPaneNode;
		short nRowOrigin;
		short nColOrigin;
		short nRowSpan;
		short nColSpan;

		// Operations
		bool IsEmpty() const {return m_pPaneNode == 0;}
		void Empty() {m_pPaneNode = 0;}
	};

	struct CPanes
	{
		CPanes(): m_nGapSize(0), m_bEndingRowGap(false), m_bEndingColGap(false) {}

		ILayoutNode* GetPaneNode(short nRow, short nCol);

		void Insert(ILayoutNode* pNode, short nRow, short nCol, short nRowSpan = 1, short nColSpan = 1);
		void Remove(short iRow, short iCol);
		bool FindPane(ILayoutNode* pNode, short& nRow, short& nCol);
		CPaneInfo& GetCell(short iRow, short iCol);
		const CPaneInfo& GetCell(short iRow, short iCol) const;
		CRect GetCellRect(short nRow, short nCol) const;
		CRect GetPaneRect(short nRow, short nCol) const;

		short GetRowCount() const {return static_cast<short>(m_RowPoints.size());}
		short GetColCount() const {return static_cast<short>(m_ColPoints.size());}

		void DistributeNewRect(const CRect& rcNewRect, int iGapSize = 0);
		void SetRowHeight(short nRow, int nHeight);
		void SetColWidth(short nCol, int nWidth);
		int GetRowHeight(short nRow) const;
		int GetColWidth(short nCol) const;

		void ApplyRects(bool bNegotiate = true, bool bStrict = true);

		bool GapTest(CPoint ptPos, short& nRow, short& nCol) const;

		CRect GetCurrentRect() const;
	protected:

		CRect GetPaneRect(const CPaneInfo& pane) const;
		void AddColumns(short nPos = -1, short nCount = 1);
		void RemoveColumns(short nPos, short nCount = 1);

		void AddRows(short nPos = -1, short nCount = 1);
		void RemoveRows(short nPos, short nCount = 1);

		bool IsColEmpty(short iColumn) const;
		bool IsRowEmpty(short iRow) const;

		bool InBounds(short iRow, short iCol) const;

		int GetMaxRowHeight(short iRow) const;
		int GetMaxColWidth(short iCol) const;

	private:
		typedef std::deque<float> CoordList; 
		typedef std::deque<CPaneInfo> CellsList; 
		typedef std::deque<CellsList> ColsList; 

		ColsList m_grid;
		CoordList m_RowPoints;
		CoordList m_ColPoints;
		CPoint m_ptOrigin;
		int m_nGapSize; 
		bool m_bEndingRowGap;
		bool m_bEndingColGap;
	};

	friend struct CPanes;
	CPanes m_panes;


	// Drawing information
	struct CSplitterDrawInfo 
	{
		HDC   hdc;
		CRect rcCell;
		DWORD dwDrawingStyle;
		bool  bDrawHSplitter;
		bool  bDrawVSplitter;
		bool  bDrawHConnector;
		bool  bDrawVConnector;

		CSplitterDrawInfo(HDC aDC, CRect aRect, DWORD aStyle, bool bHS = true, bool bVS = true, bool bHC = true, bool bVC = true)
			: hdc(aDC), rcCell(aRect), dwDrawingStyle(aStyle), 
			bDrawHSplitter(bHS), bDrawVSplitter(bVS), bDrawHConnector(bHC), bDrawVConnector(bVC)
		{}
	};

protected:
	// Implementation

	virtual bool SplitterHitTest(CPoint point, short& iRowSplitter, short& iColSplitter);
	CRect GetRowSplitterRect(short iRow) const;
	CRect GetColSplitterRect(short iCol) const;

	virtual void DrawSplitters(HDC hdc);
	virtual void DrawCellSplitter(const CSplitterDrawInfo& sdInfo);
	DWORD ResolveAppropDrawingStyle()
	{
		if (m_dwDrawingStyle == DrawGlobalSetting) {
			return m_dwDrawingStyleAll; 
		}
		else {
			return m_dwDrawingStyle;
		}
	}
	void SetSplitterCursor(CPoint ptPos);

	// Splitter mouse tracking
	virtual void StartTracking(CPoint point, short iRowSplitter, short iColSplitter);
	virtual CSize Track(CPoint point, short iRowSplitter, short iColSplitter);
	virtual bool TrackMessageLoop(HDC hdcTracker, short iRowSplitter, short iColSplitter, CRect& rcRowSplitter, CRect& rcColSplitter);
	virtual void OnTrackMouseMove(HDC hdcTracker, CPoint ptOldPos, CPoint ptNewPos, short iRowSplitter, short iColSplitter, CRect& rcRowSplitter, CRect& rcColSplitter);
	virtual void DrawTrackerRect(HDC hdcTracker, CRect rcTracker, bool bRow, bool bErase = false);
	virtual HDC GetTrackerDC();

	// Called when the drag operation starts
	virtual bool OnSplitterDragStart(CPoint /*point*/, short /*iRowSplitter*/, short /*iColSplitter*/) {return true;}
	// Called when the drag operation finishes
	virtual CSize OnAdjustFinalSplitterRect(CRect& rcSplitter, bool bRow);
	// Called after a moving of the splitter
	virtual void OnSplitterMoved(CSize szDelta);
	void PumpPaintMessages();

public:
	// Default splitter behavior

	static int m_nSplitterWidthAll;
	static DWORD m_dwDrawingStyleAll;

	void InitSplitters();
	static HCURSOR m_hcurVSplitter;
	static HCURSOR m_hcurHSplitter;
	static HCURSOR m_hcurCrossSplitter;

};



};
};

#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
	#pragma warning (pop)
	#endif
#endif