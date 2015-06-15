/////////////////////////////////////////////////////////////////////////////
// LayoutFactory.h : SFL Layout Node Factory
//
// Stingray Software Windows Template Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  SFL Layout Manager node factory definition
//


#pragma once

#if !defined(_SFL)
#define _SFL
#endif


#include <foundation\Patterns\Factory.h>
#include <foundation\Layout\LayoutBase.h>
#include <map>

namespace stingray {
namespace foundation {

//@doc


typedef foundation::ILayoutNode* (*_LayoutNodeCreator)();
typedef void (*_LayoutNodeDestructor)(foundation::ILayoutNode*);
struct LayoutMapEntry
{
	const GUID* pguid;
	_LayoutNodeCreator creator;
	_LayoutNodeDestructor destructor;
};

#define BEGIN_LAYOUT_MAP()	\
	namespace stingray{ namespace foundation {	\
	foundation::LayoutMapEntry __LayoutMap[] = {

#define LAYOUT_MAP_ENTRY(nodeClass)	\
	{&__uuidof(nodeClass), nodeClass::CreateNode, nodeClass::DeleteNode},

#define END_LAYOUT_MAP() \
	{&IID_NULL, 0, 0}};	\
	};};



// Must declare a Layout Map for this symbol to be defined
extern foundation::LayoutMapEntry __LayoutMap[];


//@class CLayoutFactory |
// Helper class responsible for the creation and destruction of the
// layout nodes. Makes use of the layout map to locate the appropiate
// object factory when a node needs to be created.
class CLayoutFactory
{
public:
	CLayoutFactory(): m_pLayoutMap(__LayoutMap) {}
	CLayoutFactory(LayoutMapEntry* pMap): m_pLayoutMap(pMap) {}

public:
	ILayoutNode* CreateLayoutNode(const GUID& guid);
	void DestroyLayoutNode(ILayoutNode* pNode);

protected:
	LayoutMapEntry* m_pLayoutMap;

	LayoutMapEntry* FindEntry(const GUID& guid);
};




};	// namespace stingray::foundation
};	// namespace foundation

