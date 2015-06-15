#ifndef WIZARD_LIST_H
#define WIZARD_LIST_H

class CWizardListEntry {
// this is the information describing one project wizard
    private:
        CLSID m_classId;
        CString m_sName;
        CString m_sDescription;
    public:
        CWizardListEntry(CLSID m_clsid, CString m_sName, 
            CString m_sDescription);
        CLSID GetClassId();
        CString GetName();
        CString GetDescription();
};

class CWizardList : private CPtrList {
// this is the list of all project wizards
// the list is initialized by searching for COM objects
// implementing the project wizard category
    private:
        POSITION position;
        HRESULT AddWizardToList(CLSID classId);
        void SearchWizards();
    public:
        CWizardList();
        ~CWizardList();
        void Init();
        CWizardListEntry *GetFirst();
        CWizardListEntry *GetNext();
        CWizardListEntry *GetEntry(CString sName);
};

#endif
