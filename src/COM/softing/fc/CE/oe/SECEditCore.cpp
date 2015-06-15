// SECEditCore.cpp
// SECEditCore Implementation File
// All the implementation is in the .inl files

#include "stdafx.h"
#include "SECEdit.h"

#include "SECEditCore.inl"
#include "View_Core.inl"
#include "ViewBackspace.inl"
#include "ViewCopy.inl"
#include "ViewCursor.inl"
#include "ViewCut.inl"
#include "ViewDelete.inl"
#include "ViewInsert.inl"
#include "ViewJoinLines.inl"
#include "ViewPaint.inl"
#include "ViewPaste.inl"
#include "ViewSelect.inl"
#include "ViewSplitLine.inl"
#include "ViewTab.inl"


// Developer studio flags these two lines with the warnings:
// warning C4660: template-class specialization 'SECEditCore<class CWnd>' is already instantiated
// warning C4660: template-class specialization 'SECEditCore<class CView>' is already instantiated
// but if you do not have these two lines, then you will get 100's of unresolved
// externals when your app tries to link. it appears to be a bug in the compiler.
#pragma warning(disable:4660)

	template class SECEditCore<CWnd>;
	template class SECEditCore<CView>;

#pragma warning(default:4660)

PROCESS_LOCAL(SEC_DND_STATE, _secDndState);




