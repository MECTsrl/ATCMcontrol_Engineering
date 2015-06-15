#ifndef _TOOLBOX_H_
#define _TOOLBOX_H_

class CHourglass
{
public:
// =Operations:
                            CHourglass(HWND hwndCap = NULL);
                            ~CHourglass();

// =Attributes:
private:
    HWND                    m_hwndCapture;    // window to capture mouse, if any
    HCURSOR                 m_hSaveCursor;    // save cursor handle
};

#endif
