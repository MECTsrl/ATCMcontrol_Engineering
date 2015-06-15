/////////////////////////////////////////////////////////////////////////////
// Application.h : OTL Application
//
// Stingray Software Foundation Library
// Copyright (C) 1999 RogueWave Software Inc,
// All rights reserved.
//
// Author:       Gilberto Araya
// Description:  OTL Basic application logic
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

#include <list>
#include <map>
#include <foundation\sflcommon.h>

#if defined(_MT)
#include <process.h>
#endif

namespace stingray {
namespace foundation {

/////////////////////////////////////////////////////////////////////////////
// Initializer classes

/////////////////////////////
// AutoDuck tag block for CNoopInitialize
//
//@doc CNoopInitializer
//
//@mfunc static void | CNoopInitializer | Init | Initialize the application
//@rdesc void 
//@comm CNoopInitializer's Init does nothing. 
//
//@mfunc static void | CNoopInitializer | Term | Terminate the application
//@rdesc void 
//@comm CNoopInitializer's Term does nothing. 
//
//@class CNoopInitializer | Many applications require initialization code to
// happen. SFL's architecture has initializers that mix this functionality 
// in. This class represents generalized intialization and
// termination points for SFL applications. These two functions include
// <mf CNoopInitializer::Init> and <mf CNoopInitializer::Term>.
//
// See Also <c CComInitializer>, <c COleInitializer>, and <c CCommonControlsInitializer>
//
//
class CNoopInitializer
{
public:
	//@cmember
	/* Initialize the application. CNoopInitializer does nothing.  */
	static void Init() {
	}
	//@cmember
	/* Terminate the application. CNoopInitializer does nothing.  */
	static void Term() {
	}
};

/////////////////////////////
// AutoDuck tag block for CComInitializer
//
//@doc CComInitializer
//
//@mfunc void | CComInitializer | Init | Initialize the application
//@comm CComInitializer's Init calls CoInitializeEx using whatever value 
//  was passed in as the coinit template parameter. 
//
//@mfunc void | CComInitializer | Term | Terminate the application
//@comm CComInitializer's Term calls CoUninitialize
//
//@class CComInitializer | Many applications require initialization code to
// happen. SFL's architecture has initializers that mix this functionality 
// in. CComInitialize class represents generalized intialization and
// termination points for SFL applications that need to use COM
//
// As you might expect, <mf CComInitializer::Init> calls CoInitializeEx for 
//  the application and <mf CComInitializer::Term> calls CoUninitialize for 
//  the application.
//
// See Also <c CNoopInitializer>, <c COleInitializer>, and <c CCommonControlsInitializer>
//
//@tcarg class | _Base = CNoopInitializer | Default initializaton.
//@tcarg DWORD | coinit = COINIT_APARTMENTTHREADED | Default value representing
//  which apartment to enter.
//
//@base public | _Base
//
template <typename _Base = CNoopInitializer, DWORD coinit = COINIT_APARTMENTTHREADED >
class CComInitializer :
	public _Base
{
public:
	//@cmember
	/* Initialize the application. Calls CoInitializeEx with the coinit template parameter  */
	static void Init()
	{
		_Base::Init();
		::CoInitializeEx(NULL, coinit);
	}

	//@cmember
	/* Initialize the application. Calls CoUninitializeEx. */
	static void Term()
	{
		_Base::Term();
		::CoUninitialize();
	}
};

/////////////////////////////
// AutoDuck tag block for COleInitializer
//
//@doc COleInitializer
//
//@mfunc void | COleInitializer | Init | Initialize the application using OleInitilize
//@comm COleInitializer's Init calls OleInitialize
//
//@mfunc void | COleInitializer | Term | Terminate the application using OleUniinitalize
//@comm COleInitializer's Term calls OleUninitialize
//
//@class COleInitializer | Many applications require initialization code to
// happen. SFL's architecture has initializers that mix this functionality 
// in This class represents generalized intialization and
// termination points for SFL applications that need to use OLE (for example, 
// applications that need to use the OLE Clipboard). As you might expect, 
// <mf COleInitializer::Init> calls 
// OleInitialize for the application and <mf COleInitializer::Term> 
// calls OleUninitialize for the application.
//
// See Also <c CNoopInitializer>, <c CComInitializer>, and <c CCommonControlsInitializer>
//
//@tcarg typename | _Base = CNoopInitializer | Default initializaton.
//
template <typename _Base = CNoopInitializer>
class COleInitializer :
	public _Base
{
public:
	//@cmember
	/* Initialize the application. Calls OleInitialize  */
	static void Init()
	{
		_Base::Init();
		::OleInitialize(NULL);
	}

	//@cmember
	/* Terminate the application. Calls OleUninitialize  */
	static void Term()
	{
		_Base::Term();
		::OleUninitialize();
	}
};

/////////////////////////////
// AutoDuck tag block for CCommonControlsInitializer
//
//@doc CCommonControlsInitializer
//
//@mfunc void | CCommonControlsInitializer | Init | Initialize the application
//@comm COleInitializer's Init calls InitCommonControlsEx
//
//@mfunc void | CCommonControlsInitializer | Term | Terminate the application
//@comm CCommonControlsInitializer's Term doesn't do anything.
//
//@class CCommonControlsInitializer | Many applications require initialization code to
// happen. SFL's architecture has initializers that mix this functionality 
// in. This class represents generalized intialization and
// termination points for SFL applications that need to use the common
// controls library.
//
// As you might expect, Init calls OleInitialize for the application.
//
// See Also <c CNoopInitializer>, <c COleInitializer>, and <c CComInitializer>
//
//@tcarg typename | _Base = CNoopInitializer | Default initializaton.
//@tcarg DWORD | dwICC = ICC_ALL | Default constant to pass to common control
// initialization..
//
template <typename _Base = CNoopInitializer, DWORD dwICC = ICC_ALL>
class CCommonControlsInitializer :
	public _Base
{
public:
	//@cmember
	/* Initialize the application. Calls InitCommonControlsEx applying the dwICC template parameter  */
	static void Init()
	{
		_Base::Init();
		// Initialize support for common controls
		INITCOMMONCONTROLSEX iccx = { sizeof(iccx), dwICC};
		InitCommonControlsEx(&iccx); // Normal common controls
	}

	//@cmember
	/* Terminate the application. */
	static void Term()
	{
		_Base::Term();
	}
};


typedef CComInitializer<CCommonControlsInitializer<> > UseCOMAndCommonControls;
typedef COleInitializer<CCommonControlsInitializer<> > UseOLEAndCommonControls;
typedef CCommonControlsInitializer<> UseCommonControls;
typedef CNoopInitializer UseNoopInitializer;

///////////////////////////////////////////////////////////////////////////
//@doc CMessageLoopBase
//
//
//@mfunc int | CMessageLoopBase | Run | Runs the application's message loop. 
//@rdesc returns the final message's WPARAM value
//@comm CMessageLoopBase provides a simple implementation of a message loop,
// calling GetMessage repeatedly until a WM_QUIT message is placed in the
// queue. Run also provides basic message pre-translation services and 
// idle time processing. Run simply calls <mf CMessageLoopBase::RunMessageLoop>.
// 
//@mfunc void | CMessageLoopBase | CreateMainWindow | Creates the application's
// main window. 
//@parm int | nCmdShow | Integer SW_XXX describing the way to show the window. 
//  Comes from the standard Win32 SW_XXX enumeration. 
//@parm void* | lpParam = NULL | Address of a parameter to pass to the window
//  when it's created. 
//@comm CMessageLoopBase provides no implementation of this function. It is 
// pure virtual and must be filled out by the derived class.
//
//@mfunc void | CMessageLoopBase | DestroyMainWindow | Destroys the application's
// main window. 
//@comm CMessageLoopBase provides no implementation of this function. It is 
// pure virtual and must be filled out by the derived class.
//
//@mfunc int | CMessageLoopBase | RunMessageLoop | Actual implementation of running
// the application's message loop. 
//@rdesc returns the final message's WPARAM value
//@comm CMessageLoopBase provides a simple implementation of a message loop,
// calling GetMessage repeatedly until a WM_QUIT message is placed in the
// queue. Run also provides basic message pre-translation services and 
// idle time processing
//
//@mfunc virtual bool | CMessageLoopBase | PreTranslateMessage | Hook for pre-translating messages.
//@rdesc false
//@parm MSG* | pMsg | Address of the Win32 MSG structure being processed
//@comm CMessageLoopBase provides a hook for pre-translating messages. The default implementation
// simply calls the Win32 API TranslateMessage.
//
//@mfunc virtual bool | CMessageLoopBase | OnIdle | Hook for pre-translating messages.
//@rdesc false
//@parm int | nIdleCount | Not used in default.
//@comm CMessageLoopBase provides a hook for handling idle time processing. The default
// implementation does nothing. 

//@mfunc virtual bool | CMessageLoopBase | IsIdleMessage | Hook for filtering
// messages that shouldn't be handled during idle-time processing 
//@rdesc false
//@parm MSG* | pMsg | Not used in default.
//@comm CMessageLoopBase provides a hook for filtering out messages that should
// be let through. the default implementation does nothing.
//
//@class CMessageLoopBase | This class is provided as a standard way 
// to manage an application's message loop. The general structure for 
// most applications is to create a main window, run the message loop
// (via GetMessage/DispatchMessage) and finally destroy the main
// window when the WM_QUIT message is received)
// CMessageLoopBase has two virtual functions for managing the window:
// <mf CMessageLoopBase::CreateMainWindow> and <mf CMessageLoopBase::DestroyMainWindow>. 
// Applications cannot use CMessageLoopBase directly, instead using one
// of CMessageLoopBase's derivatives. 
//
// Member functions include:
//  <mf CMessageLoopBase::CreateMainWindow>, 
//  <mf CMessageLoopBase::DestroyMainWindow>, 
//  <mf CMessageLoopBase::Run>, 
//  <mf CMessageLoopBase::RunMessageLoop>, 
//  <mf CMessageLoopBase::CreateMainWindow>, 
//  <mf CMessageLoopBase::OnIdle>, 
//  <mf CMessageLoopBase::IsIdleMessage>, and 
//  <mf CMessageLoopBase::PreTranslateMessage>
//
class CMessageLoopBase
{
public:
	//@cmember
	/* Creates the main window, runs the message loop, and destroys the main waindow*/
	int Run()
	{
		CreateMainWindow(SW_SHOWDEFAULT);
		int result = RunMessageLoop();
		DestroyMainWindow();
		return result;
	}


protected:
	//@cmember
	/* Create the main window  */
	virtual void CreateMainWindow(int nCmdShow, void* lpParam = NULL) = 0;

	//@cmember
	/* Destroy the main window  */
	virtual void DestroyMainWindow() = 0;

	//@cmember
	/* Run the message loop */
	int RunMessageLoop()
	{
		MSG msg;
		msg.wParam = 0;

		bool bDoIdle = true;
		int nIdleCount = 0;
		bool bQuitMessage = false;

		while (!bQuitMessage) {

			while(!::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) && bDoIdle) {
				if (!OnIdle(nIdleCount++)) {
					bDoIdle = false;
				}
			}

			bQuitMessage = ::GetMessage(&msg, NULL, 0, 0) == 0;

			if (!PreTranslateMessage(&msg)) {
				::DispatchMessage(&msg);
			}

			if (IsIdleMessage(&msg)) {
				bDoIdle = true;
				nIdleCount = 0;
			}
		}

		return msg.wParam;
	}

	//@cmember
	/* Message pre-translation. */
	virtual bool PreTranslateMessage (MSG* pMsg)
	{
		::TranslateMessage(pMsg);
		return false;	// not translated
	}

	//@cmember
	/* Handle Idle time processing */
	virtual bool OnIdle (int nIdleCount)
	{
		nIdleCount;
		return false;	// don't continue
	}

	//@cmember
	/* Is the current message an idle message? */
	static bool IsIdleMessage (MSG* pMsg)
	{
		// Return FALSE if the message just dispatched should _not_
		// cause OnIdle to be run.  
		// As in MFC, messages which do not usually affect the state 
		// of the user interface and happen very often are checked for.
		switch(pMsg->message)
		{
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
		case WM_PAINT:
		case 0x0118:	// WM_SYSTIMER (caret blink)
			return false;
		}

		return true;
	}

};


///////////////////////////////////////////////////////////////////////////
//@doc CCreateWindowMessageLoop
//
//@mfunc void | CCreateWindowMessageLoop | CreateMainWindow | Creates the application's
// main window. 
//@parm int | nCmdShow | Integer SW_XXX describing the way to show the window. 
//  Comes from the standard Win32 SW_XXX enumeration. 
//@parm void* | lpParam = NULL | Address of a parameter to pass to the window
//  when it's created. 
//@comm CCreateWindowMessageLoop provides an implementation of this function. 
//
//@mfunc void | CCreateWindowMessageLoop | DestroyMainWindow | Destroys the application's
// main window. 
//@comm CCreateWindowMessageLoop provides an implementation of this function. 
//
//@mdata  | CCreateWindowMessageLoop | m_pwndMain | This message loop's window 
//
//@class CCreateWindowMessageLoop | This class is provided as a standard way 
// to manage an application's message loop. The general structure for 
// most applications is to create a main window, run the message loop
// (via GetMessage/DispatchMessage) and finally destroy the main
// window when the WM_QUIT message is received). 
// CCreateWindowMessageLoop fills in the two virtual functions for managing the window:
// <mf CCreateWindowMessageLoop::CreateMainWindow> and <mf CCreateWindowMessageLoop::CreateMainWindow>. 
// Applications can use CCreateWindowMessageLoop directly, though you may
// prefer to use CMessageLoopDefaultImpl.
//
// See Also <c CMessageLoopBase>, <c CCreateDialogMessageLoop>, and <c CMessageLoopDefaultImpl>
//
// Member functions include:
//  <mf CCreateWindowMessageLoop::CreateMainWindow>, 
//  <mf CCreateWindowMessageLoop::DestroyMainWindow>, 
//
//@tcarg typename | _WindowClass | Type of window to create and destroy
//@tcarg typename | _Base = CMessageLoopBase | Base class
//
template <typename _WindowClass, typename _Base = CMessageLoopBase>
class CCreateWindowMessageLoop:
	public _Base
{
	typedef _Base _baseClass;
public:
	typedef _WindowClass WindowClass;

	//@cmember
	/* Create the application's main window */
	void CreateMainWindow(int nCmdShow, void* lpParam = NULL);
	//@cmember
	/* Destroy the application's main window */
	void DestroyMainWindow();

protected:
	//@cmember
	/* Pointer to the main window-- notice it's based on the template parameter */
	_WindowClass* m_pwndMain;

};

///////////////////////////////////////////////////////////////////////////
//@doc CCreateDialogMessageLoop
//
//
//@mdata  | CCreateDialogMessageLoop | m_pwndMain | This dialog message loop's window 
//
//@mfunc void | CCreateDialogMessageLoop | CreateMainWindow | Creates the application's
// main window. 
//@parm int | nCmdShow | Integer SW_XXX describing the way to show the window. 
//  Comes from the standard Win32 SW_XXX enumeration. 
//@parm void* | lpParam = NULL | Address of a parameter to pass to the window
//  when it's created. 
//@comm CCreateDialogMessageLoop provides an implementation of this function. 
//
//@mfunc void | CCreateDialogMessageLoop | DestroyMainWindow | Destroys the application's
// main window. 
//@comm CCreateDialogMessageLoop provides an implementation of this function. 
//
//@class CCreateDialogMessageLoop | This class is provided as a standard way 
// to manage a dialog application's message loop. The general structure for 
// most applications is to create a main window, run the message loop
// (via GetMessage/DispatchMessage) and finally destroy the main
// window when the WM_QUIT message is received). 
// CCreateDialogMessageLoop fills in the two virtual functions for managing the window:
// <mf CCreateDialogMessageLoop::CreateMainWindow> and <mf CCreateDialogMessageLoop::CreateMainWindow>. 
//
// See Also <c CMessageLoopBase>, <c CCreateWindowMessageLoop>, and <c CMessageLoopDefaultImpl>
//
//@tcarg typename | _DialogClass | Type of dialog window to create and destroy
//@tcarg typename | _Base = CMessageLoopBase | Base class
//
// Member functions include:
//  <mf CCreateDialogMessageLoop::CreateMainWindow>, 
//  <mf CCreateDialogMessageLoop::DestroyMainWindow>, 
//
template <typename _DialogClass, typename _Base = CMessageLoopBase>
class CCreateDialogMessageLoop:
	public _Base
{
	typedef _Base _baseClass;
public:
	typedef _DialogClass WindowClass;

	//@cmember
	/* Create the main dialog window */
	void CreateMainWindow (
		int nCmdShow,
		void* lpParam = NULL
	);

	//@cmember
	/* Destroy the main dialog window. Default implementation does nothing.  */
	void DestroyMainWindow (
	);

protected:
	//@cmember
	/* Pointer to the main dialog window-- notice it's based on the template parameter */
	_DialogClass* m_pwndMain;
};

/////////////////////////////////////////////////////////////////////////////
// Dynamic message pretranslation and idle processing capabilities
//


///////////////////////////////////////////////////////////////////////////
//@doc IIdleHandler | Interface for handling idle processing
//
//@class IIdleHandler | Interface for handling idle processing.
//
// 
interface IIdleHandler
{
	//@cmember
	/* Handle idle time processing */
	virtual bool OnIdle() = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc IMessagePreTranslator | Interface for handling message pre-translation
//
//@class IMessagePreTranslator | Interface to handle message translation
//
interface IMessagePreTranslator
{
	//@cmember
	/* Pre-process the message */
	virtual bool PreTranslateMessage(MSG* pMsg) = 0;
};

///////////////////////////////////////////////////////////////////////////
//@doc CMessageLoopDefaultImpl
//
//@mfunc void | CMessageLoopDefaultImpl | AddMessagePreTranslator | Add a message translator
//@parm IMessagePreTranslator* | pMsgTrans | pointer to an implementation of IMessagePreTranslator
//@comm AddMessagePreTranslator Adds a message pre-translator to the loop
//
//@mfunc void | CMessageLoopDefaultImpl | RemoveMessagePreTranslator | Remove a message translator
//@parm IMessagePreTranslator* | pMsgTrans | pointer to an implementation of IMessagePreTranslator
//@comm RemoveMessagePreTranslator Fremoves a message pre-translator from the loop
//
//@mfunc void | CMessageLoopDefaultImpl | AddIdleHandler | Add and idle handler 
//@parm IIdleHandler* | pIdleHandler | pointer to an implementation of IIdleHandler
//@comm AddIdleHandler Adds an idle handler to the window message machinery
//
//@mfunc void | CMessageLoopDefaultImpl | RemoveIdleHandler | Add and idle handler 
//@parm IIdleHandler* | pIdleHandler | pointer to an implementation of IIdleHandler
//@comm RemoveIdleHandler Removes an idle Handler from the window message machinery
//
//@mfunc virtual bool | CMessageLoopDefaultImpl | PreTranslateMessage | Give the app an opportunity
// to pre-process a message.
//@rdesc true if the message was processed, false otherwise
//@parm MSG* | pMsg | pointer to an message structure to preprocess
//@comm PreTranslateMessage gives your app's message loop a chance to handle messages
// before they're circulated in the system. 
//
//@mfunc virtual bool | CMessageLoopDefaultImpl | OnIdle | Give the app an opportunity
// to be handle tasks during idle time 
//@rdesc false 
//@parm int | nIdleCount | reserved 
//@comm OnIdle gives your app's message loop a chance handle tasks during idle time
//
//@mdata  | CMessageLoopDefaultImpl | m_lstIdleHandlers| List of idle handlers
//@mdata  | CMessageLoopDefaultImpl | m_lstMessagePreTranslators| List of message translators
//
//@class CMessageLoopDefaultImpl | This class provides a default 
// implementation for an application's message loop. The general structure for 
// most applications is to create a main window, run the message loop
// (via GetMessage/DispatchMessage) and finally destroy the main
// window when the WM_QUIT message is received). CCreateWindowMessageLoop 
// fills in the two virtual functions for managing the window:
// by deriving from a message loop-derived class (CMessageLoopBase by default)
//
// CMessageLoopDefaultImpl is capable of managing several message pre-translators
//  and idle handlers. You add message pre-translators and idle handlers to
//  the message loop using AddMessagePreTranslator and AddIdleHandler. 
//  CCreateWindowMessageLoop runs the message loop by calling the pretranslators
//  and the idle handlers registered with the message loop.
//
// Member functions include:
//  <mf CMessageLoopDefaultImpl::AddMessagePreTranslator>, 
//  <mf CMessageLoopDefaultImpl::RemoveMessagePreTranslator>, 
//  <mf CMessageLoopDefaultImpl::AddIdleHandler>, 
//  <mf CMessageLoopDefaultImpl::RemoveIdleHandler>, 
//  <mf CMessageLoopDefaultImpl::PreTranslateMessage>, 
//  <mf CMessageLoopDefaultImpl::OnIdle>
//
//@tcarg typename | _Base = CMessageLoopBase | Base class
//
//
template <typename _Base = CMessageLoopBase>
class CMessageLoopDefaultImpl :
	public _Base
{
	typedef _Base _baseClass;

protected:

	typedef std::list<IIdleHandler*> IdleHandlersList;
	typedef std::list<IMessagePreTranslator*> MessagePreTranslatorsList;

	//@cmember
	/* List of idle handlers */
	IdleHandlersList m_lstIdleHandlers;

	//@cmember
	/* List of message translators */
	MessagePreTranslatorsList m_lstMessagePreTranslators;

public:
	//@cmember
	/* Add a message translator to the loop */
	void AddMessagePreTranslator(IMessagePreTranslator* pMsgTrans)
	{
		m_lstMessagePreTranslators.push_back(pMsgTrans);
	}

	//@cmember
	/* Remove a message translator from the loop */
	void RemoveMessagePreTranslator(IMessagePreTranslator* pMsgTrans)
	{
		m_lstMessagePreTranslators.remove(pMsgTrans);
	}

	//@cmember
	/* Add an idle handler to the loop */
	void AddIdleHandler(IIdleHandler* pIdleHandler)
	{
		m_lstIdleHandlers.push_back(pIdleHandler);
	}

	//@cmember
	/* Remove an idle handler from the loop */
	void RemoveIdleHandler(IIdleHandler* pIdleHandler)
	{
		m_lstIdleHandlers.remove(pIdleHandler);
	}

public:

	//@cmember
	/* Override message pre-translation */
	virtual bool PreTranslateMessage (
		MSG* pMsg
	)
	{
		bool bTranslated = false;
		for (MessagePreTranslatorsList::iterator pos = m_lstMessagePreTranslators.begin(); !bTranslated && pos != m_lstMessagePreTranslators.end(); ++pos) {
			bTranslated = (*pos)->PreTranslateMessage(pMsg);
		}
		if (!bTranslated) {
			_baseClass::PreTranslateMessage(pMsg);
		}
		return bTranslated;
	}

	//@cmember
	/* Override idle handling */
	virtual bool OnIdle (
		int /*nIdleCount*/
	)
	{
		for (IdleHandlersList::iterator pos = m_lstIdleHandlers.begin(); pos != m_lstIdleHandlers.end(); ++pos) {
			(*pos)->OnIdle();
		}
		return false;	// Do not continue
	}

};

/////////////////////////////////////////////////////////////////////////////
// Default message loop classes

///////////////////////////////////////////////////////////////////////////
//@doc CMessageLoop
//
//@class CMessageLoop | CMessageLoop represents a convenient message loop
// class for mixing into applications. CMessageLoop inherits from 
// <c CCreateWindowMessageLoop> using a template parameter provided by
// the application code as the window to create. 
//
//@tcarg typename | WindowClass | Type of window to create during initialization.
//
//@base public | CCreateWindowMessageLoop<lt>WindowClass, CMessageLoopDefaultImpl<lt><gt> <gt>
//
template <typename WindowClass>
class CMessageLoop :
	public CCreateWindowMessageLoop<WindowClass, CMessageLoopDefaultImpl<> >
{
};

///////////////////////////////////////////////////////////////////////////
//@doc CDialogMessageLoop
//
//@class CDialogMessageLoop | CDialogMessageLoop represents a convenient message loop
// class for mixing into dialog-based applications. CDialogMessageLoop inherits from 
// <c CDialogWindowMessageLoop> using a template parameter provided by
// the application code as the type of dialog to create. 
//
//@tcarg typename | DialogClass | Type of dialog to create
//
//@base public | CCreateDialogMessageLoop<lt>DialogClass, CMessageLoopDefaultImpl<lt><gt> <gt>
//
template <typename DialogClass>
class CDialogMessageLoop :
	public CCreateDialogMessageLoop<DialogClass, CMessageLoopDefaultImpl<> >
{
};


/////////////////////////////////////////////////////////////////////////////
// Default processing implementations

///////////////////////////////////////////////////////////////////////////
//@doc CDialogMessagePreTranslator
//
//
//@mfunc bool | CDialogMessagePreTranslator | PreTranslateMessage | Pre-translate messages
//@rdesc true if the message was translated, false otherwise
//@parm MSG* | pMsg | Message to translate
//@comm Message pre-translation mechanism for diaogs.
//
//@class CDialogMessagePreTranslator | Message pre-translation mechanism
//  for dialog-based apps. 
//
//@tcarg typename | _Derived | Derived class
//
// Member functions include:
//  <mf CDialogMessagePreTranslator::AddMessagePreTranslator>
//
//@base public | IMessagePreTranslator
//
template <typename _Derived>
class CDialogMessagePreTranslator :
	public IMessagePreTranslator
{
public:
	//@cmember
	/* Message pre-translation */
	virtual bool PreTranslateMessage(MSG* pMsg)
	{
		_Derived* pDerived = static_cast<_Derived*>(this);
		BOOL bResult = ::IsDialogMessage(pDerived->m_hWnd, pMsg);
		return bResult? true : false;
	}
};


///////////////////////////////////////////////////////////////////////////
//@doc CMDIMessagePreTranslator
//
//
//@mfunc bool | CMDIMessagePreTranslator | PreTranslateMessage | Message pretranslation
// implementation suited towards MDI applications. 
//@rdesc true if the message was translated, false otherwise
//@parm MSG* | pMsg | Message to translate
//@comm Checks for MDI system accelerators
//@class CMDIMessagePreTranslator | Messag pre Translation for MDI apps
//
//@tcarg typename | _Derived | Message pre-translator for MDI Applications.
//
// Member functions include:
//  <mf CMDIMessagePreTranslator::PreTranslateMessage>, 
//
//@base public | IMessagePreTranslator
//
template <typename _Derived>
class CMDIMessagePreTranslator :
	public IMessagePreTranslator
{
public:
	//@cmember
	/* Message pre-translation */
	virtual bool PreTranslateMessage(MSG* pMsg)
	{
		_Derived* pDerived = static_cast<_Derived*>(this);
		BOOL bResult = ::TranslateMDISysAccel(pDerived->m_wndClient.m_hWnd, pMsg);
		return bResult? true : false;
	}
};

///////////////////////////////////////////////////////////////////////////
//@doc CAcceleratorsTranslator
//
//
//@mfunc bool | CAcceleratorsTranslator | PreTranslateMessage | Pretranslate accelerator messages
//@rdesc true if the message was translated, false otherwise
//@parm MSG* | pMsg | Message to translate
//@comm Pre-translate accelerators
//
//@class CAcceleratorsTranslator | Accelerator translation
//
//@tcarg typename | _Derived | Message pre-translators for managing keyboard accelerators.
//
// Member functions include:
//  <mf CAcceleratorsTranslator::PreTranslateMessage>
//
//@base public | IMessagePreTranslator
//
template <typename _Derived>
class CAcceleratorsTranslator :
	public IMessagePreTranslator
{
public:
	//@cmember
	/* Message pre-translation */
	virtual bool PreTranslateMessage(MSG* pMsg)
	{
		_Derived* pDerived = static_cast<_Derived*>(this);
		int nRes = 0;
		if (pDerived->m_hAccTable) {
			nRes = ::TranslateAccelerator(pDerived->m_hWnd, pDerived->m_hAccTable, pMsg);
		}
		return nRes != 0;
	};
};


/////////////////////////////////////////////////////////////////////////////
// Application classes

///////////////////////////////////////////////////////////////////////////
//@doc CApp
//
//
//@mdata  | CApp | m_aMessageLoop | Message loop for this app
//
//@mfunc HRESULT | CApp  | Init | Initialize the application
//@rdesc S_OK if the application initialized
//@parm int | nShowCmd | Integer from the SW_XXX enumeration indicating how to
// show the window
//@parm _ATL_OBJMAP_ENTRY* | p | pointer to ATL object map
//@parm HINSTANCE | h | Instance handle for the application
//@parm GUID* | plibid = NULL | pointer to the type library ID
//@comm Initialize a regular, single-threaded app. Usually creates a main window. 
//
//@mfunc void | CApp  | Term | Terminate the application
//@comm Terminate the regular, single threaded app
//
//@mfunc int | CApp  | Run | Run the application.
//@rdesc 0
//@comm Run the main message loop for the application.
//
//@mfunc _MessageLoop* | CApp  | GetMessageLoop | Get the app's
// message loop.
//@rdesc Pointer to the application's message loop 
//@comm Get message loop from the app.
//
//@class CApp | SFL's standard application class. Holds global state for 
// an application as well as functions for initializing the app, creating a main window 
// running the message loop, and terminating the application.
//
// See Also <c CMTIApp>
//
//@tcarg typename | _Base = ATL::CComModule | This first parameter must either be ATL's CComModule class, or be derived from ATL's CComModule class. 
//@tcarg typename | _MessageLoop | Message loop class for handling messages
//@tcarg typename | _Initializer = <c CNoopInitializer> | Class for handling initialization
//
// Member functions include:
//  <mf CApp::Init>,  
//  <mf CApp::Term>, and 
//  <mf CApp::Run>
//
//@base public | _Base (default is CComModule)
//
//
template <typename _Base = ATL::CComModule, typename _MessageLoop, typename _Initializer = CNoopInitializer >
class CApp : 
	public _Base
{
public:
	typedef _MessageLoop MessageLoopClass;
	typedef _Initializer Initializer;

	//@cmember
	/* Constructor */
	CApp()
	{
	}

	//@cmember
	/* Destructor */
	~CApp()
	{
	}

	//@cmember
	/* Initialize the application */
	HRESULT Init(int nShowCmd, _ATL_OBJMAP_ENTRY* p, HINSTANCE h, const GUID* plibid = NULL)
	{
		HRESULT hr = _Base::Init(p, h, plibid);
		if (SUCCEEDED(hr)) {
			// Initialize support for common controls
			Initializer::Init();
		}
		return hr;
	}

	//@cmember
	/* Terminate the application */
	void Term()
	{
		Initializer::Term();
		_Base::Term();
	}

	//@cmember
	/* Run the application */
	int Run()
	{
		m_aMessageLoop.Run();
		return 0;
	}

	//@cmember
	/* Get the message loop */
	_MessageLoop* GetMessageLoop()
	{
		return &m_aMessageLoop;
	}


protected:
	//@cmember
	/* Message loop for this app */
	_MessageLoop m_aMessageLoop;

};


///////////////////////////////////////////////////////////////////////////
//@doc CMTIApp
//
//@mdata  | CMTIApp | m_aMessageLoops | List of message loops for this app
//
//@mfunc HRESULT | CMTIApp  | Init | Initialize the MTI app
//@rdesc S_OK if the application initialized
//@parm int | nShowCmd | Integer from the SW_XXX enumeration indicating 
//  how to show the window. 
//@parm _ATL_OBJMAP_ENTRY* | p | ATL's object map entry.
//@parm HINSTANCE | h | Instance of the application
//@parm GUID* | plibid = NULL | Type library ID
//@comm Initialize the MTI-based app
//
//@mfunc void | CMTIApp  | Term | Terminate the app
//@comm Terminate the app
//
//@mfunc int | CMTIApp  | Run | Run the application
//@rdesc 0 if the application ran
//@comm Run the MTI app. Runs a top level window
//
//@mfunc _MessageLoop* | CMTIApp  | GetMessageLoop | Get one of the MTI App's
// message loops.
//@rdesc Pointer to the application's message loop 
//@parm DWORD | dwKey | Key to use to find the message loop
//@comm Get one of the message loops from the app.
//
//@mfunc bool | CMTIApp  | RunTopLevelWindow | Run a new top level window. 
// This funciton is often called in response to a File Open menu selection.
//@rdesc Returns true if the top-level thread ran, false otherwise.
//@parm void* | lpParm | Parameter to pass to the thread function
//@comm Runs a top level window in an MTI application. 
//
//@mfunc bool | CMTIApp  | AddMessageLoop | Add a message loop to the application
//@parm _MessageLoop* | aMessageLoop | Message loop to add
//@comm Adds a message loop to the application
//
//@mfunc bool | CMTIApp  | RemoveMessageLoop | Remove a message loop from the application
//@comm Removes a message loop to the application

//@mfunc static UINT WINAPI | CMTIApp  | ThreadEntryPoint | Static entry
// point for the thread. 
//@rdesc 0
//@parm void* | lpParm = NULL | Parameter being passed to the thread function
//@comm Entry point for the thread in an MTI app. This implementation creates
// a message loop on the thread stack, calls the initializer's init function, 
// adds the message loop to the module, and then runs the message loop.
//
//@class CMTIApp | Application class for MTI applications 
//
//@tcarg typename | _Base = ATL::CComModule | This first parameter must either be ATL's CComModule class, or be derived from ATL's CComModule class. 
//@tcarg typename | _MessageLoop | Message loop class for handling messages
//@tcarg typename | _Initializer = <c CNoopInitializer> | Class for handling initialization
//
// Member functions include:
//  <mf CMTIApp::Init>, 
//  <mf CMTIApp::Term>, 
//  <mf CMTIApp::Run>,
//  <mf CMTIApp::GetMessageLoop>,
//  <mf CMTIApp::RunTopLevelWindow>,
//  <mf CMTIApp::AddMessageLoop>,
//  <mf CMTIApp::RemoveMessageLoop>, and
//  <mf CMTIApp::ThreadEntryPoint>
//
// See Also <c CApp>
//
//@base public | _Base (default is CComModule)
//
template <typename _Base = ATL::CComModule, typename _MessageLoop, typename _Initializer = CNoopInitializer >
class CMTIApp : public _Base
{
public:
	typedef _MessageLoop MessageLoopClass;
	typedef _Initializer Initializer;
	typedef std::map<DWORD, MessageLoopClass*> MessageLoopMap;

public:
	//@cmember
	/* Initialize the MTI application */
	HRESULT Init(int nShowCmd, _ATL_OBJMAP_ENTRY* p, HINSTANCE h, const GUID* plibid = NULL)
	{
		nShowCmd;
		HRESULT hr = _Base::Init(p, h, plibid);
		if (SUCCEEDED(hr)) {
			// Initialize support for common controls
			Initializer::Init();
			m_cs.Init();
			m_hFinish = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		}
		return hr;
	}

	//@cmember
	/* Terminate the MTI application */
	void Term()
	{
		::CloseHandle(m_hFinish);
		m_cs.Term();
		Initializer::Term();
		_Base::Term();
	}

	//@cmember
	/* Run the MTI application */
	int Run()
	{
		// Create an initial UI thread
		RunTopLevelWindow();

		// Wait for all threads to finish, before exiting the process
		ATL::AtlWaitWithMessageLoop(m_hFinish);
		return 0;
	}

	//@cmember
	/* Get a specific message loop */
	_MessageLoop* GetMessageLoop(DWORD dwKey = ::GetCurrentThreadId())
	{
		MessageLoopMap::iterator pos = m_MessageLoops.find(dwKey);
		if (pos != m_MessageLoops.end()) {
			return (*pos).second;
		}
		else {
			return NULL;
		}
	}

	//@cmember
	/* Run the top level window */
	bool RunTopLevelWindow(void* lpParam = NULL)
	{
		// Run the new thread
		UINT nThreadId;
		DWORD hThread = ::_beginthreadex(NULL, 0, ThreadEntryPoint, lpParam, 0, &nThreadId);
		return hThread != NULL;
	}


protected:

	//@cmember
	/* Add a messsage loop to the MTI app */
	void AddMessageLoop(_MessageLoop* aMessageLoop)
	{
		// Add the message loop to the map
		// Use critical section to prevent race conditions, as
		// STL containers are not thread safe
		m_cs.Lock();
		m_MessageLoops.insert(MessageLoopMap::value_type(::GetCurrentThreadId(), aMessageLoop));
		m_cs.Unlock();
	}

	//@cmember
	/* Remove the message loop from the MTI app */
	void RemoveMessageLoop()
	{
		m_cs.Lock();

		// Remove message loop from map
		MessageLoopMap::iterator pos = m_MessageLoops.find(::GetCurrentThreadId());
		if (pos != m_MessageLoops.end()) {
			m_MessageLoops.erase(pos);
		}

		// Notify the main thread
		if (m_MessageLoops.size() == 0) {
			::SetEvent(m_hFinish);
		}
		m_cs.Unlock();
	}

	//@cmember
	/* Thread entry poin for MTI threads */
	static UINT WINAPI ThreadEntryPoint(void* lpParam = NULL)
	{
		_MessageLoop aMessageLoop;
		Initializer::Init();
		_Module.AddMessageLoop(&aMessageLoop);

		aMessageLoop.Run();

		_Module.RemoveMessageLoop();
		Initializer::Term();
		return 0;
	}


protected:
	HANDLE m_hFinish;
	CComCriticalSection m_cs;

	//@cmember
	/* List of message loops for this app */
	MessageLoopMap m_MessageLoops;
};


};	// namespace stingray::foundation
};	// namespace foundation


#ifndef _SFL_NO_PRAGMA_WARNINGS
	#if 1200 <= _MSC_VER
	#pragma warning (pop)
	#endif
#endif