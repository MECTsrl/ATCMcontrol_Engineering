
#include "stdafx.h"

// disable warning C4786: symbol greater than 255 character,
// okay to ignore
#pragma warning(disable: 4786)

#include "POU.h"

//------------------------------------------------------------------*
/**
 * Function name    POU::POU
 * Description      constructor for class POU
 * @param           const string& name
 * @param           const StringPosRange nameRange
 * @param           const StringPos lineNo
 * @param           const StringPos nextVarSectInsertPos
 * @return          -
 * @exception       -
 * @see             -
*/
POU::POU(const string& name,
         const StringPosRange& nameRange,
         StringPos lineNo,
         StringPos nextVarSectInsertPos):
    m_name(name),
    m_name_range(nameRange),
    m_lineno(lineNo),
    m_nextVarSectInsertPos(nextVarSectInsertPos),
    m_filesys_cmnt(0)
{
    m_bSuppressError = false;
}

//------------------------------------------------------------------*
/**
 * Function name    POU::~POU
 * Description      destructor for class POU
 * @return          -
 * @exception       -
 * @see             -
*/
POU::~POU()
{
    if ( m_filesys_cmnt != 0 )
        delete m_filesys_cmnt;

    for(var_decl_sections_citer ci_vds = m_var_decl_sections.begin();
        ci_vds != m_var_decl_sections.end();
        ++ci_vds)
    {
        delete (*ci_vds);
    }

    for(import_directives_citer ci_impdir = m_import_directives.begin();
        ci_impdir != m_import_directives.end();
        ++ci_impdir)
    {
        delete (*ci_impdir);
    }

    for(task_configurations_citer ci_taskconf = m_task_configurations.begin();
        ci_taskconf != m_task_configurations.end();
        ++ci_taskconf)
    {
        delete (*ci_taskconf);
    }

    for(program_configurations_citer ci_progconf = m_program_configurations.begin();
        ci_progconf != m_program_configurations.end();
        ++ci_progconf)
    {
        delete (*ci_progconf);
    }
}

//------------------------------------------------------------------*
/**
 * Function name    POU::setFileSysComment
 * Description      sets the file system comment
 * @param           SysComment* fileSysComment
 * @return          -
 * @exception       -
 * @see             -
*/
void POU::setFileSysComment(SysComment* fileSysComment)
{
    m_filesys_cmnt = fileSysComment;
}

//------------------------------------------------------------------*
/**
 * Function name    POU::addVarDeclSect
 * Description      adds a variable declaration section
 * @param           VarDeclSection* vds
 * @return          -
 * @exception       -
 * @see             -
*/
void POU::addVarDeclSect(const VarDeclSection* vds)
{
    if ( vds == 0 )
    {
//        DebugOutput << "VDS IS NULL" << endl;cout.flush();
        return;
    }

    m_var_decl_sections.push_back(vds);
    m_nextVarSectInsertPos = vds->getRange().getLast();
}

//------------------------------------------------------------------*
/**
 * Function name    POU::getNameRange
 * Description      returns the range of the pou name
 * @return          const StringPosRange&
 * @exception       -
 * @see             -
*/
const StringPosRange& POU::getNameRange() const 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_name_range; 
}

//------------------------------------------------------------------*
/**
 * Function name    POU::getName
 * Description      returns the pou name
 * @return          const string&
 * @exception       -
 * @see             -
*/
const string& POU::getName() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_name;       
}

//------------------------------------------------------------------*
/**
 * Function name    POU::setName
 * Description      sets the pou name
 * @param           const string& name
 * @return          -
 * @exception       -
 * @see             -
*/
void POU::setName(const string& name)
{
    m_name = name;       
}

//------------------------------------------------------------------*
/**
 * Function name    POU::getLineNo
 * Description      returns line number of the pou
 * @return          StringPos
 * @exception       -
 * @see             -
*/
StringPos POU::getLineNo() const
{
    return m_lineno;     
}

//------------------------------------------------------------------*
/**
 * Function name    POU::getFileSystemComment
 * Description      returns system comment of the pou
 * @return          SysComment*
 * @exception       -
 * @see             -
*/
const SysComment* POU::getFileSystemComment() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_filesys_cmnt;
}

//------------------------------------------------------------------*
/**
 * Function name    POU::invite
 * Description      calls visit of the visitor for itself
 * @param           const IECSRCVisitor& visitor
 * @return          -
 * @exception       -
 * @see             -
*/
void POU::invite(const IECSRCVisitor& visitor) const
{
    // visit filesystem comment
    if ( m_filesys_cmnt != 0 )
        m_filesys_cmnt->invite(visitor);

    // visit each variable declaration section
    for(var_decl_sections_citer ci_vds = m_var_decl_sections.begin();
        ci_vds != m_var_decl_sections.end();
        ++ci_vds)
    {
        (*ci_vds)->invite(visitor);
    }

    for(import_directives_citer ci_impdir = m_import_directives.begin();
        ci_impdir != m_import_directives.end();
        ++ci_impdir)
    {
        (*ci_impdir)->invite(visitor);
    }

    for(task_configurations_citer ci_taskconf = m_task_configurations.begin();
        ci_taskconf != m_task_configurations.end();
        ++ci_taskconf)
    {

        for(program_configurations_citer ci_progconf = m_program_configurations.begin();
            ci_progconf != m_program_configurations.end();
            ++ci_progconf)
        {
            CString TaskNameOfProg((*ci_progconf)->getTaskName().c_str());
            CString TaskName((*ci_taskconf)->getName().c_str());

            if (TaskNameOfProg.CompareNoCase(TaskName) == 0)
            {
                (*ci_progconf)->invite(visitor, 1);
            }
        }

        (*ci_taskconf)->invite(visitor);
    }

    for(program_configurations_citer ci_progconf = m_program_configurations.begin();
        ci_progconf != m_program_configurations.end();
        ++ci_progconf)
    {
        (*ci_progconf)->invite(visitor);
    }

}

//------------------------------------------------------------------*
/**
 * Function name    POU::getVarInsertPos
 * Description      gets the insert position for a variable
 *                  If a correspondig variable declaration section
 *                  already exists, the parameter sectionExists is set to true.
 *                  Otherwise, it is set to false.
 * @param           const string& varSectionType
 * @param           const string& varSectionModifier
 * @param           StringPos& insertPos
 * @param           bool& sectionExists
 * @return          -
 * @exception       -
 * @see             -
*/
void POU::getVarInsertPos(const string& varSectionType,
                          const string& varSectionModifier,
                          StringPos& insertPos,
                          bool& sectionExists) const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    CString vst(varSectionType.c_str());
    CString vsm(varSectionModifier.c_str());

    CString endMark;
    if (getPOUType() == _CST_FILE)
    {
        endMark = _END_CONST;
    }
    else if (getPOUType() == _STRUCT)
    {
        endMark = _END_STRUCT;
    }
    else
    {
        endMark = _END_VAR;
    }

    for(var_decl_sections_citer ci_vds = m_var_decl_sections.begin();
        ci_vds != m_var_decl_sections.end();
        ++ci_vds)
    {
        CString t((*ci_vds)->getType().c_str());
        CString m((*ci_vds)->getModifier().c_str());
        if ((t.CompareNoCase(vst) == 0) && (m.CompareNoCase(vsm) == 0))
        {
            sectionExists = true;
            insertPos = (*ci_vds)->getRange().getLast() - endMark.GetLength();
            return;
        }
    }

    sectionExists = false;
    insertPos = m_nextVarSectInsertPos;
}

//------------------------------------------------------------------*
/**
 * Function name    POU::addImportDirective
 * Description      adds an import directive
 * @param           const ImportDirective* impdir
 * @return          -
 * @exception       -
 * @see             -
*/
void POU::addImportDirective(const ImportDirective* impdir)
{
    if ( impdir == 0 )
    {
//        DebugOutput << "IMPDIR IS NULL" << endl;cout.flush();
        return;
    }

    m_import_directives.push_back(impdir);
}

//------------------------------------------------------------------*
/**
 * Function name    POU::setNextImpDirInsertPos
 * Description      sets the insert position for the next import directive
 * @param           StringPos nextImpDirInsertPos
 * @return          -
 * @exception       -
 * @see             -
*/
void POU::setNextImpDirInsertPos(StringPos nextImpDirInsertPos)
{
    m_nextImpDirInsertPos = nextImpDirInsertPos;
}

//------------------------------------------------------------------*
/**
 * Function name    POU::getNextImpDirInsertPos
 * Description      returns the insert position for the next import directive
 * @return          StringPos
 * @exception       -
 * @see             -
*/
StringPos POU::getNextImpDirInsertPos() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_nextImpDirInsertPos;
}

//------------------------------------------------------------------*
/**
 * Function name    POU::getImportDirectives
 * Description      returns the import directives
 * @return          const import_directives_type&
 * @exception       -
 * @see             -
*/
const import_directives_type& POU::getImportDirectives() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_import_directives;       
}

//------------------------------------------------------------------*
/**
 * Function name    POU::addTaskConfiguration
 * Description      adds a task configuration
 * @param           const TaskConfiguration* taskconf
 * @return          -
 * @exception       -
 * @see             -
*/
void POU::addTaskConfiguration(const TaskConfiguration* taskconf)
{
    if ( taskconf == 0 )
    {
//        DebugOutput << "TASKCONF IS NULL" << endl;cout.flush();
        return;
    }

    m_task_configurations.push_back(taskconf);
}

//------------------------------------------------------------------*
/**
 * Function name    POU::setNextTaskConfInsertPos
 * Description      sets the insert postion for the next task configuration
 * @param           StringPos nextTaskConfInsertPos
 * @return          -
 * @exception       -
 * @see             -
*/
void POU::setNextTaskConfInsertPos(StringPos nextTaskConfInsertPos)
{
    m_nextTaskConfInsertPos = nextTaskConfInsertPos;
}

//------------------------------------------------------------------*
/**
 * Function name    POU::getNextTaskConfInsertPos
 * Description      returns the insert postion for the next task configuration
 * @return          StringPos
 * @exception       -
 * @see             -
*/
StringPos POU::getNextTaskConfInsertPos() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_nextTaskConfInsertPos;
}

//------------------------------------------------------------------*
/**
 * Function name    POU::getTaskConfigurations
 * Description      returns the task configurations
 * @return          const task_configurations_type&
 * @exception       -
 * @see             -
*/
const task_configurations_type& POU::getTaskConfigurations() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_task_configurations;       
}

//------------------------------------------------------------------*
/**
 * Function name    POU::addProgramConfiguration
 * Description      adds a program configuration
 * @param           const ProgramConfiguration* progconf
 * @return          -
 * @exception       -
 * @see             -
*/
void POU::addProgramConfiguration(const ProgramConfiguration* progconf)
{
    if ( progconf == 0 )
    {
//        DebugOutput << "PROGKCONF IS NULL" << endl;cout.flush();
        return;
    }

    m_program_configurations.push_back(progconf);
}

//------------------------------------------------------------------*
/**
 * Function name    POU::setNextProgConfInsertPos
 * Description      sets the insert postion for the next program configuration
 * @param           StringPos nextProgConfInsertPos
 * @return          -
 * @exception       -
 * @see             -
*/
void POU::setNextProgConfInsertPos(StringPos nextProgConfInsertPos)
{
    m_nextProgConfInsertPos = nextProgConfInsertPos;
}

//------------------------------------------------------------------*
/**
 * Function name    POU::getNextProgConfInsertPos
 * Description      returns the insert postion for the next program configuration
 * @return          StringPos
 * @exception       -
 * @see             -
*/
StringPos POU::getNextProgConfInsertPos() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_nextProgConfInsertPos;
}

//------------------------------------------------------------------*
/**
 * Function name    POU::getProgramConfigurations
 * Description      returns the program configurations
 * @return          const program_configurations_type&
 * @exception       -
 * @see             -
*/
const program_configurations_type& POU::getProgramConfigurations() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_program_configurations;       
}

//------------------------------------------------------------------*
/**
 * Function name    POU::setNextVarSectInsertPos
 * Description      sets the insert postion for the next variable section
 * @param           StringPos nextVarSectInsertPos
 * @return          -
 * @exception       -
 * @see             -
*/
void POU::setNextVarSectInsertPos(StringPos nextVarSectInsertPos)
{
    m_nextVarSectInsertPos = nextVarSectInsertPos;
}

//------------------------------------------------------------------*
/**
 * Function name    POU::getVarDeclSections
 * Description      returns the variable declaration sections
 * @return          const var_decl_sections_type&
 * @exception       -
 * @see             -
*/
const var_decl_sections_type& POU::getVarDeclSections() const
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_var_decl_sections;
}

//------------------------------------------------------------------*
/**
 * Function name    POU::setErrorText
 * Description      sets the error text member
*/
void POU::setErrorText(const string& errorText)
{
    if(m_bSuppressError) {
        //avoid setting error text if suppress error set.
        return;
    }

    m_errorText = errorText;
    // strip \r\n
    int iLastChar = m_errorText.length() - 1;
    if(iLastChar < 0)
    {
        return;
    }

    if(m_errorText[iLastChar] == '\n')
    {
        m_errorText[iLastChar] = 0;
    }
    
    --iLastChar;
    if(iLastChar < 0)
    {
        return;
    }

    if(m_errorText[iLastChar] == '\r')
    {
        m_errorText[iLastChar] = 0;
    }
}

//------------------------------------------------------------------*
/**
 * Function name    POU::getErrorText
 * Description      returns the error text
 * @return          string
*/
string POU::getErrorText() const
{
    return m_errorText;
}



//------------------------------------------------------------------*
/**
 * Set project information into POU object. Method is virtual and 
 * implemented empty within POU object. POU objects of specific
 * type (e.g Configuration etc.) may overwrite this method to set
 * project data into the POU object.
 *
 * @param			BackparserProjectInfo & - reference to project
 *                  information.
 * @see			::doBackparse()
*/
void POU::setProjectInfo(BackparserProjectInfo & info)
{}

