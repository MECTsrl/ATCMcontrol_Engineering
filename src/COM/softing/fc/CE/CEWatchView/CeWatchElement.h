#if !defined(AFX_CEWATCHELEMENT_H__51D50B27_E062_11D2_A008_00A024363DFC__INCLUDED_)
#define AFX_CEWATCHELEMENT_H__51D50B27_E062_11D2_A008_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CESysDef.h"
#include "CeWatchType.h"
#include "CeWatchIndices.h"
#include "XMLHelper/XMLNode.h"
#include "XMLHelper/XMLDocument.h"

class CCeWatchBackEnd;
class CCeSymbol;

interface IResourceDBI;
interface IVariableDBI;

#define CEWATCH_MAX_HISTORY 10
#define CEWATCH_EXP_NAME _T("Name=")
#define CEWATCH_EXP_VAL  _T("Value=")
#define CEWATCH_EXP_EQ   _T("=")
#define CEWATCH_EXP_CRLF _T("\n")

// xml definitions
#define CEWATCH_XMLTAG_VARNODE      _T("VARNODE")
#define CEWATCH_XMLATTR_EXPRESSION  _T("EXPRESSION")
#define CEWATCH_XMLATTR_TYPE        _T("TYPE")
#define CEWATCH_XMLATTR_VALUE       _T("VALUE")
#define CEWATCH_XMLATTR_FORMAT      _T("FORMAT")
#define CEWATCH_XMLATTR_EXPANDED    _T("EXPANDED")


class CCeWatchElement  
{
public:
	CCeWatchElement();
	CCeWatchElement(LPCTSTR pName, const CCeWatchType& type);
	CCeWatchElement(CCeSymbol *pSymbol);

	virtual ~CCeWatchElement();

    static bool IsVarKindVisible(ECeWatchVarKind varKind);

	inline const CString& GetName() const { return m_Name; }
	CString GetShortName() const;
	void SetName(LPCTSTR pName) { 
		m_Name = pName; 
		m_Name.TrimLeft();
		m_Name.TrimRight();	}

	inline const CString& GetFormat() const { return m_strFormat; }
	inline void SetFormat(LPCTSTR pValue) { m_strFormat = pValue; }

	void SetType(const CCeWatchType& type);
	inline const CCeWatchType& GetType() const { return m_Type; }

	inline const CString& GetValue() const { return m_strValue; }
	inline void SetValue(LPCTSTR pValue) { m_strValue = pValue; }

	bool IsConnected() const;

	void SetQuality(short Quality);
	inline const CString& GetQualityText() const { return m_strQuality; }
	inline const short GetQuality() const { return m_iQuality; }

	inline const CString& GetTime() const { return m_strTime; }
	inline void SetTime(LPCTSTR pValue) { m_strTime = pValue; }


//  parent and children relation ship
//	every element has one parent (is  NULL for the root element)
//  every element has 0 or n children
	inline CCeWatchElement* GetRoot() const { return m_pRoot; }
	inline CCeWatchElement* GetParent() const { return m_pParent; }

	CCeWatchElement* AddChild(LPCTSTR pChildName, const CCeWatchType& ChildType);
	bool AddChild(CCeWatchElement* pChild);
	bool SetChild(int ii, CCeWatchElement* pChild);
	void AddChildren(CCeSymbol* pSymbol);		// 10.10.06 SIS
	CCeWatchElement* GetChild(int index) const { 
		ASSERT(index >= 0 && index < GetChildrenCount());
		if (index >= 0 && index < GetChildrenCount())
			return m_ChildrenArray.GetAt(index); 
		else
			return NULL; 
	}

	inline int GetChildrenCount() const {return m_ChildrenArray.GetSize();}

	bool DeleteChild(CCeWatchElement* pChild);
	bool DeleteAllChildren();

	// level in tree (
	// root element: 0, 
	// 1st visible level 1, 
	// 1st children level 2
	// ...
	inline unsigned GetLevel() const { return m_Level; }

	// history functions
	void AddToHistory(const CString& value);
	inline int GetHistoryCount() const { return m_HistoryList.GetSize(); }
	inline const CString GetHistory(int index) const { 
		if (index >= 0 && index < m_HistoryList.GetSize())
			return m_HistoryList.GetAt(index); 
		else
			return CString();
	}

// serialization
	bool WriteData(CStdioFile& file) const;
	bool ReadData(CStdioFile& file);

	// 15.12.05 SIS >>
	// consider correct index range
    bool LoadFromNode(CXMLNode& rtNode, CCeSymbol* pRootSymbol);	// 10.10.06 SIS
    bool SaveToNode(CXMLNode& rtNode, CXMLDocument& rtDocument, const CString& crstrIndexRange);
	// 15.12.05 SIS <<

    void SetInitExpanded(bool bInitExpanded) {m_bInitExpanded = bInitExpanded; };
    bool GetInitExpanded() { return m_bInitExpanded; };
	
	// 14.12.05 SIS >>
	CString	GetIndexRange();	// 15.12.05 SIS
	void SetIndexRange(const CString& crsRange);	// 10.10.06 SIS
	// 14.12.05 SIS <<

	// 10.10.06 SIS >>
	BOOL ExtractArrayIndices(const CString& crsIndexRange, int& riStartIndex, int& riStopIndex);
	BOOL CheckArrayIndices();
	// 10.10.06 SIS <<

private:
	void Init();
	void SetParent(CCeWatchElement* pParent);
    void PresetHistory();

private:
// static members
	CString       m_Name;
	CCeWatchType  m_Type;
	CString       m_strFormat;

// dynamic members
	CString       m_strValue;
	CString       m_strTime;
	short         m_iQuality;
	CString       m_strQuality;
    bool          m_bInitExpanded;    // expanded initially?

// relations
	CArray<CCeWatchElement*, CCeWatchElement*> m_ChildrenArray;
	CCeWatchElement*  m_pParent;
	unsigned          m_Level;
	CCeWatchElement*  m_pRoot;

// write history
	CArray<CString,CString> m_HistoryList;

	// 15.12.05 SIS >>
	// Index range is only used to initialize array index range correctly 
	// after load.	It may be incorrect later
	CString			m_strIndexRange;
	// 15.12.05 SIS <<


// debug checking
public :
	unsigned m_DebugId;
};

bool IsValidWatchElement(const CCeWatchElement* pElem);

#endif // !defined(AFX_CEWATCHELEMENT_H__51D50B27_E062_11D2_A008_00A024363DFC__INCLUDED_)
