
// Von Klassen-Assistent automatisch erstellte IDispatch-Kapselungsklasse(n). 
/////////////////////////////////////////////////////////////////////////////
// Wrapper-Klasse CGenEdItemDriver 

class CGenEdItemDriver : public COleDispatchDriver
{
public:
	CGenEdItemDriver() {}		// Ruft den Standardkonstruktor für COleDispatchDriver auf
	CGenEdItemDriver(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CGenEdItemDriver(const CGenEdItemDriver& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attribute
public:

// Operationen
public:
	SCODE GetDocument(LPDISPATCH* pDocumentDispatch);
};
/////////////////////////////////////////////////////////////////////////////
// Wrapper-Klasse CGenEdDocDriver 

class CGenEdDocDriver : public COleDispatchDriver
{
public:
	CGenEdDocDriver() {}		// Ruft den Standardkonstruktor für COleDispatchDriver auf
	CGenEdDocDriver(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CGenEdDocDriver(const CGenEdDocDriver& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attribute
public:

// Operationen
public:
	SCODE GetContainer(LPDISPATCH* pContainerDispatch);
	SCODE SetBreakpoint       (long lQualifier, long lX, long lY, long lCX, long lCY);
	SCODE ClearBreakpoint     (long lQualifier, long lX, long lY, long lCX, long lCY);
	SCODE ActivateBreakpoint  (long lQualifier, long lX, long lY, long lCX, long lCY);
	SCODE DeactivateBreakpoint(long lQualifier, long lX, long lY, long lCX, long lCY);
	SCODE ContinueTask();
    //mode=0: single step, mode=1: step over, mode=2: step out 
	SCODE StepTask(long mode);
	SCODE ClearInstanceBreakpoints();
	SCODE ClearAllBreakpoints();
    SCODE OnEdit();
};
/////////////////////////////////////////////////////////////////////////////
// Wrapper-Klasse CGenEdContDriver 

class CGenEdContDriver : public COleDispatchDriver
{
public:
	CGenEdContDriver() {}		// Ruft den Standardkonstruktor für COleDispatchDriver auf
	CGenEdContDriver(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CGenEdContDriver(const CGenEdContDriver& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attribute
public:

// Operationen
public:
	SCODE ShowURL(LPCTSTR szURL);
	SCODE ShowFile(LPCTSTR szFile);
	SCODE SelectFBForCall();
    SCODE ShowVarSelection(LPCTSTR szPouName);
    SCODE SetVarSelectionHint(LPCTSTR szPouName,
                              LPCTSTR szVarNamePrefix,
                              LPCTSTR szDataType);
    SCODE ShowHelp(LPCTSTR szPOU);
    SCODE ShowHeader(LPCTSTR szPOU);
    SCODE ShowHeaderFile(LPCTSTR szFileName);
    
    SCODE GetLRUPous(VARIANT* parrPous);
    SCODE GetPouXML(LPCTSTR szPouName, BSTR* pszPouDef);
    SCODE GetVarList(VARIANT* parrVars);
    SCODE GetPouList(VARIANT* parrPous);
    SCODE GetPouDefinition(LPCTSTR szPouName, BSTR* pszPouDef);
    SCODE GetPouMember(LPCTSTR szPouName, VARIANT* parrMembers);
    SCODE XRefChangeRefPosition(BOOL bForward);
	SCODE XRefSetActiveQuery(LPCTSTR sVariableName);
    SCODE ShowVarSelectionBox(LONG lXPos, LONG lYPos,
                              LPCTSTR szLine, LONG lColumn,
                              LPCTSTR szTypeHint,
                              LPCTSTR szPouName,
                              BSTR FAR* pszSelection,
                              LONG* plStartColumn,
                              LONG* plEndColumn);
    SCODE GetProjectManager(IUnknown** pProjManUnk);

};