// This is a part of the Stingray Foundation Library.
// Copyright (C) 1995-2000 Rogue Wave Software, Inc.
//
// This source code is only intended as a supplement to
// the Objective Studio Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Studio product.
//
// Author: Daniel Jebaraj
//

//
// These macros are courtesy of the Objective Grid PRO (CCE) product
//

#ifndef _SECMESMACROS_H
#define _SECMESMACROS_H

#define SECRUNTIME_CLASS(class_name) ((CRuntimeClass*)(&class_name::class##class_name))

#ifdef _AFXDLL
#define SECIMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew, act_base) \
	CRuntimeClass* PASCAL class_name::_GetBaseClass() \
		{ return SECRUNTIME_CLASS(act_base); } \
	const AFX_DATADEF CRuntimeClass class_name::class##class_name = { \
		#class_name, sizeof(class class_name), wSchema, pfnNew, \
			&class_name::_GetBaseClass, NULL }; \
	CRuntimeClass* class_name::GetRuntimeClass() const \
		{ return RUNTIME_CLASS(class_name); } \

#define SECIMPLEMENT_DYNCREATE(class_name, base_class_name, act_base) \
	CObject* PASCAL class_name::CreateObject() \
		{ return new class_name; } \
	SECIMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, \
		class_name::CreateObject, act_base)
#else
#define SECIMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew, act_base) \
	const AFX_DATADEF CRuntimeClass class_name::class##class_name = { \
		#class_name, sizeof(class class_name), wSchema, pfnNew, \
			RUNTIME_CLASS(act_base), NULL }; \
	CRuntimeClass* class_name::GetRuntimeClass() const \
		{ return RUNTIME_CLASS(class_name); } \

#define SECIMPLEMENT_DYNCREATE(class_name, base_class_name, act_base) \
	CObject* PASCAL class_name::CreateObject() \
		{ return new class_name; } \
	SECIMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, \
		class_name::CreateObject, act_base)
#endif


#ifdef _AFXDLL
#define SECBEGIN_MESSAGE_MAP(theClass, baseClass) \
	template<class baseClass>\
	const AFX_MSGMAP* PASCAL theClass##<baseClass>::_GetBaseMessageMap() \
		{ return &baseClass::messageMap; } \
	template<class baseClass>\
	const AFX_MSGMAP* theClass##<baseClass>::GetMessageMap() const \
{ return &theClass##<baseClass>::messageMap; } \
	template<class baseClass>\
	AFX_DATADEF const AFX_MSGMAP theClass##<baseClass>::messageMap = \
	{ &theClass::_GetBaseMessageMap, &theClass##<baseClass>::_messageEntries[0] }; \
	template<class baseClass>\
	const AFX_MSGMAP_ENTRY theClass##<baseClass>::_messageEntries[] = \
	{ \

#else
#define SECBEGIN_MESSAGE_MAP(theClass, baseClass) \
	template<class baseClass>\
	const AFX_MSGMAP* theClass##<baseClass>::GetMessageMap() const\
{ return &theClass##<baseClass>::messageMap; } \
	template<class baseClass>\
	AFX_DATADEF const AFX_MSGMAP theClass##<baseClass>::messageMap = \
	{ &baseClass::messageMap, &theClass##<baseClass>::_messageEntries[0] }; \
	template<class baseClass>\
	const AFX_MSGMAP_ENTRY theClass##<baseClass>::_messageEntries[] = \
	{ \

#endif
// use for intermediate base classes (refer designer code for sample).
#ifdef _SECDX_USE_DLL_MAP
#ifdef _AFXDLL
#define SECINTERBEGIN_MESSAGE_MAP(theClass, baseClass, templateClass) \
	template<class templateClass>\
	const AFX_MSGMAP* PASCAL theClass##<templateClass>::_GetBaseMessageMap() \
		{ return &baseClass::messageMap; } \
	template<class templateClass>\
	const AFX_MSGMAP* theClass##<templateClass>::GetMessageMap() const \
{ return &theClass##<templateClass>::messageMap; } \
	template<class templateClass>\
	AFX_DATADEF const AFX_MSGMAP theClass##<templateClass>::messageMap = \
	{ &theClass::_GetBaseMessageMap, &theClass##<templateClass>::_messageEntries[0] }; \
	template<class templateClass>\
	const AFX_MSGMAP_ENTRY theClass##<templateClass>::_messageEntries[] = \
	{ \
#else

#define SECINTERBEGIN_MESSAGE_MAP(theClass, baseClass, templateClass) \
	template<class templateClass>\
	const AFX_MSGMAP* theClass##<templateClass>::GetMessageMap() const \
{ return &theClass##<templateClass>::messageMap; } \
	template<class templateClass>\
	AFX_DATADEF const AFX_MSGMAP theClass##<templateClass>::messageMap = \
	{ &baseClass::messageMap, &theClass##<templateClass>::_messageEntries[0] }; \
	template<class templateClass>\
	const AFX_MSGMAP_ENTRY theClass##<templateClass>::_messageEntries[] = \
	{ \

#endif
#endif //0 excluded

#endif //_SECMESMACROS_H
