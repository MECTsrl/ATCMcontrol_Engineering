
#ifndef POU_H
#define POU_H

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include <string>
#include <iostream>
#include "Global.h"

using namespace std;

#include "IECSRCVisitor.h"
#include "SysComment.h"
#include "TaskConfiguration.h"
#include "ProgramConfiguration.h"
#include "ImportDirective.h"
#include "VarDeclSection.h"

BACKPARSERDLLEXPORT typedef vector<const VarDeclSection*> var_decl_sections_type;
typedef var_decl_sections_type::const_iterator var_decl_sections_citer ;

typedef vector<const ImportDirective*> import_directives_type;
typedef import_directives_type::const_iterator import_directives_citer ;

typedef vector<const TaskConfiguration*> task_configurations_type;
typedef task_configurations_type::const_iterator task_configurations_citer ;

typedef vector<const ProgramConfiguration*> program_configurations_type;
typedef program_configurations_type::const_iterator program_configurations_citer ;

//================================================================================
class POU
//================================================================================
{
public:
    POU(const string& name,
        const StringPosRange& nameRange,
        StringPos lineNo,
        StringPos nextVarSectInsertPos);
    virtual ~POU();

    virtual void setFileSysComment(SysComment* fileSysComment);
    void addVarDeclSect(const VarDeclSection* vds);
    virtual string getPOUType() const = 0;
    virtual void setProjectInfo(BackparserProjectInfo &);

    BACKPARSERDLLEXPORT virtual const StringPosRange& getNameRange() const;
    BACKPARSERDLLEXPORT virtual const string& getName() const;
    virtual void setName(const string& name);
    virtual StringPos getLineNo() const;
    BACKPARSERDLLEXPORT virtual const SysComment* getFileSystemComment() const;
    virtual void invite(const IECSRCVisitor& visitor) const;

    void setNextVarSectInsertPos(StringPos nextVarSeciInsertPos);
    StringPos getNextVarSectInsertPos()
    {
        return m_nextVarSectInsertPos;
    }

    BACKPARSERDLLEXPORT void getVarInsertPos(const string& varSectionType,
                                             const string& varSectionModifier,
                                             StringPos& insertPos,
                                             bool& sectionExists) const;

    BACKPARSERDLLEXPORT const var_decl_sections_type& getVarDeclSections() const;

    void addImportDirective(const ImportDirective* impdir);
    void setNextImpDirInsertPos(StringPos nextImpDirInsertPos);
    BACKPARSERDLLEXPORT StringPos getNextImpDirInsertPos() const;
    BACKPARSERDLLEXPORT const import_directives_type& getImportDirectives() const;

    void addTaskConfiguration(const TaskConfiguration* taskconf);
    void setNextTaskConfInsertPos(StringPos nextTaskConfInsertPos);
    BACKPARSERDLLEXPORT StringPos getNextTaskConfInsertPos() const;
    BACKPARSERDLLEXPORT const task_configurations_type& getTaskConfigurations() const;

    void addProgramConfiguration(const ProgramConfiguration* taskconf);
    void setNextProgConfInsertPos(StringPos nextProgConfInsertPos);
    BACKPARSERDLLEXPORT StringPos getNextProgConfInsertPos() const;
    BACKPARSERDLLEXPORT const program_configurations_type& getProgramConfigurations() const;

    void setErrorText(const string& errorText);
    string getErrorText() const;
    void SuppressError()
    {
        m_bSuppressError = true;
    }
protected:
    string                          m_name;
    StringPosRange                  m_name_range;
    StringPos                       m_lineno;

    SysComment                      *m_filesys_cmnt;

    var_decl_sections_type          m_var_decl_sections;

    StringPos                       m_nextVarSectInsertPos;

    import_directives_type          m_import_directives;
    task_configurations_type        m_task_configurations;
    program_configurations_type     m_program_configurations;
    StringPos                       m_nextImpDirInsertPos;
    StringPos                       m_nextTaskConfInsertPos;
    StringPos                       m_nextProgConfInsertPos;
    string                          m_errorText;
    bool                            m_bSuppressError;
};

#endif

