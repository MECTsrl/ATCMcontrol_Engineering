#ifndef Breakpoint_H
#define Breakpoint_H

#include "StdEdAttr.h"


class CBreakpoint : public CObject
{
public:
	CBreakpoint (const CStdEditorPosition &position)
    : m_Position (position),
      m_bActive (false),
      m_bSubscribed (false)
    {
    }


    const CStdEditorPosition& GetPosition () const
    {
        return m_Position;
    }
	
    bool IsActive () const
    {
        return (m_bActive);
    }
	
    void SetActive (bool bActive)
    {
	    m_bActive = bActive;
    }

    bool IsSubscribed () const
    {
	    return m_bSubscribed;
    }

	void SetSubscribed (bool bSubscribed)
    {
	    m_bSubscribed = bSubscribed;
    }


protected:
	CStdEditorPosition m_Position;
	bool               m_bActive;
	bool               m_bSubscribed;
};

#endif