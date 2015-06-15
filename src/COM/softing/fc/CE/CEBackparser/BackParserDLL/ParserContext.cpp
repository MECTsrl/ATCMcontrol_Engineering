
#include "stdafx.h"

#include <stdio.h>

#include "ParserContext.h"
#include "StreamScan.h"

#include "LexVal.h"
#include "POU.h"
#include "SysComment.h"
#include "EmptyFile.h"
#include "GlobVarConstList.h"
#include "Configuration.h"
#include "StResource.h"
#include "TaskConfiguration.h"
#include "Program.h"
#include "FunctionBlock.h"
#include "Function.h"
#include "StStruct.h"
#include "ImportDirective.h"
#include "VarDeclSection.h"
#include "VarDecl.h"

int end_of_last_comment;

#ifdef DEBUG
#define DEBUG_FF(msg) DebugTrace((msg))
void DebugTrace(LPCTSTR psz) 
{
    ::TrcPrint(TRC_BUS_DIAGNOSE,_T("BACKPARSER> %s\n"),psz);
}
#else
#define DEBUG_FF(msg)
#endif


//------------------------------------------------------------------*
/**
 * Function name    ParserContext::ParserContext
 * Description      constructor for class ParserContext
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
ParserContext::ParserContext()
//================================================================================
{
    init();
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::ParserContext
 * Description      destructor for class ParserContext
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
ParserContext::~ParserContext()
//================================================================================
{
    clear();
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::GetPOU
 * Description      returns the created pou
 * @return          POU*
 * @exception       -
 * @see             -
*/
//================================================================================
POU* ParserContext::GetPou() const
//================================================================================
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    return m_pou;
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::clear
 * Description      deletes all dynamically allocated objects
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::clear()
//================================================================================
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    if ( m_syscmnt != 0 )
        delete m_syscmnt;

    if ( m_globvarconstlist != 0 )
        delete m_globvarconstlist;

    if ( m_resource != 0 )
        delete m_resource;

/*
    if ( m_importdirective != 0 )
        delete m_importdirective;
*/
/*
    if ( m_taskconfiguration != 0 )
        delete m_taskconfiguration;
*/
/*  
    if ( m_programconfiguration != 0 )
        delete m_programconfiguration;
*/
    if ( m_program != 0 )
        delete m_program;

    if ( m_fb != 0 )
        delete m_fb;

    if ( m_function != 0 )
        delete m_function;

    if ( m_struct != 0 )
        delete m_struct;

    if ( m_vardeclsect != 0 )
        delete m_vardeclsect;

    if ( m_vardecl != 0 )
        delete m_vardecl;

    init();
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::init
 * Description      initializes the object
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::init()
//================================================================================
{
    m_pou = 0;

    m_syscmnt       = 0;

    m_globvarconstlist     = 0;

    m_resource      = 0;

    m_configuration = 0;

    m_importdirective = 0;

    m_taskconfiguration = 0;

    m_program       = 0;

    m_fb            = 0;

    m_function      = 0;

    m_struct        = 0;

    m_vardeclsect   = 0;

    m_vardecl       = 0;

    m_varlistopened = false;
    m_vardeclsect_found = false;
}


//#define ASSERT(x) if (x) ; else assert(__LINE__, __FILE__);

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::cleanup
 * Description      deletes an SysComment object
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::cleanup(SysComment*& syscmnt)
//================================================================================
{
    if ( syscmnt != 0 )
	{
		delete syscmnt;
		syscmnt = 0;
	}
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::assert
 * Description      throws an assertion
 * @param           int line
 * @param           const char* file
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::assert(int line, const char* file)
//================================================================================
{
    char msg[256];
    sprintf(msg, "assertion failed, in line %d of file %s", line, file);
    throw string(msg);
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::syscmnt_begin
 * Description      allocates a new SysComment object
 * @param           LexVal* lexval
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::syscmnt_begin(LexVal* lexval)
//================================================================================
{

    DEBUG_FF("syscmnt_begin");
    ASSERT(lexval != 0);
    m_syscmnt = new SysComment(lexval->getStartPos(), lexval->getLineNo());
    DEBUG_FF("OK1");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::syscmnt_end
 * Description      sets the end position of the SysComment object and returns a pointer to it
 * @param           LexVal* lexval
 * @return          SysComment*
 * @exception       -
 * @see             -
*/
//================================================================================
SysComment* ParserContext::syscmnt_end(LexVal* lexval)
//================================================================================
{

    DEBUG_FF("syscmnt_end");
    ASSERT(lexval != 0);
    ASSERT(m_syscmnt != 0);
    m_syscmnt->setEndPos(lexval->getEndPos()); 
    SysComment* sc = m_syscmnt;
    syscmnt_clear();
    DEBUG_FF("OK2");
    return sc;
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::syscmnt_add_av
 * Description      adds an attribute value pair to the SysComment object
 * @param           LexVal* a
 * @param           LexVal* v
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::syscmnt_add_av(LexVal* a, LexVal* v)
//================================================================================
// NOTE: v may be NULL, since value is optional even though EQ is required
{
    ASSERT(a != 0);
    ASSERT(m_syscmnt != 0);
    DEBUG_FF("syscmnt_add_av ");

    string value = "";
    if (v != 0)
    {
        value = v->getText();
        // Ignore both "".
        if (value.length() >= 2)
        {
            value = value.substr(1, value.length() - 2);
        }
    }

    m_syscmnt->avm_set(a->getText(), value);
    DEBUG_FF("OK3");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::syscmnt_clear
 * Description      sets the member m_syscmnt to NULL and returns NULL
 * @return          SysComment*
 * @exception       -
 * @see             -
*/
//================================================================================
SysComment* ParserContext::syscmnt_clear()
//================================================================================
{

    DEBUG_FF("syscmnt_clear");
    m_syscmnt = 0; 
    DEBUG_FF("OK4");

    return 0;
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::emptyfile
 * Description      allocates a new EmptyFile object
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::emptyfile()
//================================================================================
{

    DEBUG_FF("emptyfile");
    m_pou = new EmptyFile();
    SetFileSysComment();
    if(m_pou->getNextVarSectInsertPos() <= end_of_last_comment) {
        //file system comment not available or before last comment
        //set insert position behind last comment
        m_pou->setNextVarSectInsertPos(end_of_last_comment);
    }
    //leave it as is if file system comment is after last comment.
    
    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::initializer_begin
 * Description      sets the start position of an initializer
 * @param           LexVal* ib 
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::initializer_begin(LexVal* ib)
//================================================================================
{

    ASSERT(ib != 0);
    DEBUG_FF("initializer ");

    ASSERT(m_vardecl != 0);
    m_vardecl->setInitializerFirst(ib->getRange().getLast() + 1);
    DEBUG_FF("OK");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::initializer_clear
 * Description      sets the end position and the string of an initializer
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::initializer_clear()
//================================================================================
{
    DEBUG_FF("initializer_clear");

    ASSERT(m_vardecl != NULL);

    stream_scan *strscan = getStreamScanner();
    ASSERT(strscan != NULL);

    char *c = strscan->yytext;
    if (*c == ')')
    {   // initializer for structs
        m_vardecl->setInitializerLast(strscan->yypos - 1);
    }
    else
    {
        m_vardecl->setInitializerLast(strscan->yypos - 2);
    }
    
    string s = strscan->getSubString(m_vardecl->getInitializerRange());
    StringPos start = s.find_first_not_of(" \t\r\n");
    string initializer = s.substr(start, s.length() - start);
    m_vardecl->setInitializer(initializer);
    m_vardecl->setInitializerFirst(m_vardecl->getInitializerLast() - initializer.length() + 1);

    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::globvarconstlist_begin
 * Description      allocates a new GlobVarConstList objekt
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::globvarconstlist_begin()
//================================================================================
{

    DEBUG_FF("globvarconstlist_begin");
    m_globvarconstlist = new GlobVarConstList();
    m_pou = m_globvarconstlist;
    SetFileSysComment();
    DEBUG_FF("OK");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::globvarconstlist_end
 * Description      sets the member m_globvarconstlist to NULL
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::globvarconstlist_end()
//================================================================================
{
    DEBUG_FF("globvarconstlist_end");
    if (m_vardeclsect_found == false)
    {
        m_pou->setNextVarSectInsertPos(end_of_last_comment);
    }
    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::globvarconstlist_clear
 * Description      sets the member m_globvarconstlist to NULL
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::globvarconstlist_clear()
//================================================================================
{
    DEBUG_FF("globvarconstlist_clear");
    m_globvarconstlist = 0;
    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::globvarconstlist_set_poutype
 * Description      sets the type of the GlobVarConstList object m_globvarconstlist
 * @param           const string& POUType 
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::globvarconstlist_set_poutype(const string& POUType)
//================================================================================
{

    DEBUG_FF("globvarconstlist_begin ");
    if (m_globvarconstlist != NULL)
    {
        m_globvarconstlist->setPOUType(POUType);
    }
    DEBUG_FF("OK");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::set_next_impdir_insert_pos
 * Description      sets the insert position for the next import directive
 * @param           LexVal* lv
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::set_next_impdir_insert_pos(LexVal* lv)
//================================================================================
{
    ASSERT(lv != 0);
    string s = "set_next_impdir_insert_pos" + lv->getText();

    DEBUG_FF("set_next_impdir_insert_pos");
        m_pou->setNextImpDirInsertPos(lv->getEndPos());
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::configuration_begin
 * Description      allocates a new Configuration object
 * @param           LexVal* configuration 
 * @param           LexVal* name
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::configuration_begin(LexVal* configuration, LexVal* name)
//================================================================================
{
    ASSERT(configuration != 0);
    ASSERT(name != 0);
    DEBUG_FF("configuration_begin");

    m_configuration = new Configuration(name->getText(),
                                        name->getRange(),
                                        configuration->getLineNo(),
						                name->getRange().getLast());
    m_pou = m_configuration;
    SetFileSysComment();
    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::configuration_clear
 * Description      sets the member m_configuration to NULL
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::configuration_clear(LexVal* end)
//================================================================================
{
    DEBUG_FF("configuration_clear");
//    m_resource->setNextProgConfInsertPos(end->getStartPos() - 1);
    m_configuration = 0;
    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::resource_begin
 * Description      allocates a new Resource object
 * @param           LexVal* resource 
 * @param           LexVal* name
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::resource_begin(LexVal* resource, LexVal* name)
//================================================================================
{
    ASSERT(resource != 0);
    ASSERT(name != 0);
    DEBUG_FF("resource_begin");

    m_resource = new Resource(name->getText(),
                              name->getRange(),
                              resource->getLineNo(),
						      name->getRange().getLast());
    m_pou = m_resource;
    SetFileSysComment();
    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::resource_set_target
 * Description      sets the target of the Resource member m_resource
 * @param           LexVal* target
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::resource_set_target(LexVal* target)
//================================================================================
{
    ASSERT(target != 0);
    DEBUG_FF("resource_set_target");

    m_resource->setTarget(target->getText(), target->getRange());
    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::resoure_set_next_taskconf_insert_pos
 * Description      sets the insert position for the next task configuration
 * @param           LexVal* lv
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::resoure_set_next_taskconf_insert_pos(LexVal* lv)
//================================================================================
{
    ASSERT(lv != 0);
    DEBUG_FF("resoure_set_next_taskconf_insert_pos");
    // Calculate pos only for resources.
    if (m_resource != 0)                
    {   
        m_resource->setNextTaskConfInsertPos(lv->getEndPos());
    }
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::resoure_set_next_progconf_insert_pos
 * Description      sets the insert position for the next program configuration
 * @param           LexVal* lv
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::resoure_set_next_progconf_insert_pos(LexVal* lv)
//================================================================================
{
    ASSERT(lv != 0);
    DEBUG_FF("resoure_set_next_progconf_insert_pos");
    // Calculate pos only for resources.
    if (m_resource != 0)                
    {   
        m_resource->setNextProgConfInsertPos(lv->getEndPos());
    }
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::resoure_clear
 * Description      sets the member m_resource to NULL
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::resource_clear(/*LexVal* lv*/)
//================================================================================
{
    DEBUG_FF("resource_clear");
    m_resource = 0;
    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::task_configuration_begin
 * Description      allocates a new TaskConfiguration object
 * @param           LexVal* task_configuration 
 * @param           LexVal* name
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::task_configuration_begin(LexVal* task_configuration, LexVal* name)
//================================================================================
{
    ASSERT(task_configuration != 0);
    ASSERT(name != 0);
    DEBUG_FF("task_configuration_begin");

    m_taskconfiguration = new TaskConfiguration(name->getText(),
                                                name->getRange());
    m_taskconfiguration->setStart(task_configuration->getStartPos());

    ASSERT(m_resource != NULL);
    m_resource->addTaskConfiguration(m_taskconfiguration);

    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::task_configuration_initialisation
 * Description      sets an initialization value
 * @param           LexVal* identifier
 * @param           LexVal* becomes
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::task_configuration_initialisation(LexVal* identifier, LexVal* becomes)
//================================================================================
{
    DEBUG_FF("task_configuration_initialisation");

    ASSERT(identifier != NULL);
    ASSERT(becomes != NULL);

    ASSERT(m_taskconfiguration != NULL);

    stream_scan *strscan = getStreamScanner();
    StringPosRange spr(becomes->getRange().getLast() + 1, strscan->yypos - 2);
    string value = strscan->getSubString(spr);
	
	//EF 20.02.07 d-353 >>
	CString stmp(value.c_str());
	stmp.TrimRight();
	stmp.TrimLeft();
	value = stmp;
	//EF 20.02.07 d-353 <<

    CString id(identifier->getText().c_str());

    m_taskconfiguration->setTaskAttribute(identifier->getText(), value);

    if (id.CompareNoCase("INTERVAL") == 0)
    {
        m_taskconfiguration->setInterval(value);
        m_taskconfiguration->setIntervalRange(spr);
    }

    if (id.CompareNoCase("PRIORITY") == 0)
    {
        m_taskconfiguration->setPriority(value);
        m_taskconfiguration->setPriorityRange(spr);
    }

    if (id.CompareNoCase("AUTOSTART") == 0)
    {
        m_taskconfiguration->setAutostart(value);
        m_taskconfiguration->setAutostartRange(spr);
    }

	// NFTASKHANDLING 19.05.05 SIS >>
    if (id.CompareNoCase("LOADED") == 0)
    {
        m_taskconfiguration->setLoaded(value);
        m_taskconfiguration->setLoadedRange(spr);
    }
	// NFTASKHANDLING 19.05.05 SIS <<

    if (id.CompareNoCase("IOSYNC") == 0)
    {
        m_taskconfiguration->setIosync(value);
        m_taskconfiguration->setIosyncRange(spr);
    }

    if (id.CompareNoCase("FFTASK") == 0)
    {
        m_taskconfiguration->setFftask(value);
        m_taskconfiguration->setFftaskRange(spr);
    }
    
    // SIS: event driven tasks >>
    if (id.CompareNoCase("EVENT") == 0)
    {
        m_taskconfiguration->setEvent(value);
        m_taskconfiguration->setEventRange(spr);
    }
    // SIS: event driven tasks <<

    if (id.CompareNoCase("WATCHDOG") == 0)
    {
        m_taskconfiguration->setWatchdog(value);
        m_taskconfiguration->setWatchdogRange(spr);
    }


    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::task_configuration_initialisation_list_range
 * Description      sets the range of the task configuration initialisation list
 * @param           LexVal* open
 * @param           LexVal* close
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::task_configuration_initialisation_list_range(LexVal* open, LexVal* close)
//================================================================================
{
    DEBUG_FF("task_configuration_initialisation_list_range");

    ASSERT(m_taskconfiguration != NULL);
    ASSERT(open != NULL);
    ASSERT(close != NULL);

    StringPosRange range(open->getRange().getFirst(), close->getRange().getLast());
    m_taskconfiguration->setInitialisationListRange(range);

    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::task_configuration_set_sys_comment
 * Description      sets the system comment of a task configuration
 * @param           SysComment* comment
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::task_configuration_set_sys_comment(SysComment* comment)
//================================================================================
{
    DEBUG_FF("task_configuration_set_sys_comment");

    ASSERT(m_taskconfiguration != NULL);
    m_taskconfiguration->SetSysComment(comment);

    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::task_configuration_clear
 * Description      sets the member m_taskconfiguration to NULL
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::task_configuration_clear(LexVal* end)
//================================================================================
{
    DEBUG_FF("task_configuration_clear");
    m_taskconfiguration->setEnd(end->getEndPos());
    m_taskconfiguration = 0;
    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::program_configuration_begin
 * Description      allocates a new ProgramConfiguration object
 * @param           LexVal* program_configuration 
 * @param           LexVal* name
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::program_configuration_begin(LexVal* program_configuration, LexVal* name)
//================================================================================
{
    ASSERT(program_configuration != 0);
    ASSERT(name != 0);
    DEBUG_FF("program_configuration_begin");

    m_programconfiguration = new ProgramConfiguration(name->getText(),
                                                      name->getRange());
    m_programconfiguration->setStart(program_configuration->getRange().getFirst());

    ASSERT(m_resource != NULL);
    m_resource->addProgramConfiguration(m_programconfiguration);

    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::program_configuration_clear
 * Description      sets the end of the program configuration
 *                  and the member m_programconfiguration to NULL
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::program_configuration_clear(LexVal* semicolon)
//================================================================================
{
    DEBUG_FF("program_configuration_clear");
    m_programconfiguration->setEnd(semicolon->getRange().getLast());
    m_programconfiguration = 0;
    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::program_configuration_set_task
 * Description      sets the task and its range of a program configuration
 * @param           LexVal* task
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::program_configuration_set_task(LexVal* task)
//================================================================================
{
    ASSERT(task != 0);
    DEBUG_FF("program_configuration_set_task");
    m_programconfiguration->setTask(task->getText(), task->getRange());
    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::program_configuration_set_program_class
 * Description      sets the program class and its range of a program configuration
 * @param           LexVal* task
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::program_configuration_set_program_class(LexVal* progClass)
//================================================================================
{
    ASSERT(progClass != 0);
    DEBUG_FF("program_configuration_set_program_class");
    m_programconfiguration->setProgramClass(progClass->getText(), progClass->getRange());
    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::program_configuration_set_sys_comment
 * Description      sets the system comment of a program configuration
 * @param           SysComment* comment
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::program_configuration_set_sys_comment(SysComment* comment)
//================================================================================
{
    DEBUG_FF("program_configuration_set_sys_comment");
    ASSERT(m_programconfiguration != NULL);
    m_programconfiguration->SetSysComment(comment);
    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::program_begin
 * Description      allocates a new Program object
 * @param           LexVal* program
 * @param           LexVal* name
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::program_begin(LexVal* program, LexVal* name)
//================================================================================
{

    ASSERT(program != 0);
    ASSERT(name != 0);
    DEBUG_FF("program_begin");

    m_program = new Program(name->getText(),
                            name->getRange(),
                            program->getLineNo(),
			   			    name->getRange().getLast());
    m_pou = m_program;
    SetFileSysComment();
    DEBUG_FF("OK");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::program_clear
 * Description      sets the member m_program to NULL
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::program_clear()
//================================================================================
{

    DEBUG_FF("program_clear");
    if(m_program) {
        m_program = 0;
    }
    DEBUG_FF("OK");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::fb_begin
 * Description      allocates a new FunctionBlock object
 * @param           LexVal* fb
 * @param           LexVal* name
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::fb_begin(LexVal* fb, LexVal* name)
//================================================================================
{

    ASSERT(fb != 0);
    ASSERT(name != 0);
    DEBUG_FF("fb_begin ");

    m_fb = new FunctionBlock(name->getText(),
                             name->getRange(),
                             fb->getLineNo(),
							 name->getRange().getLast());
    m_pou = m_fb;
    SetFileSysComment();
    DEBUG_FF("OK");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::fb_clear
 * Description      sets the member m_fb to NULL
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::fb_clear()
//================================================================================
{

    DEBUG_FF("fb_clear");
    if(m_fb) {
        m_fb = 0;
    }
    DEBUG_FF("OK");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::function_begin
 * Description      allocates a new Function object
 * @param           LexVal* function
 * @param           LexVal* name
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::function_begin(LexVal* function, LexVal* name)
//================================================================================
{

    ASSERT(function != 0);
    ASSERT(name != 0);
    DEBUG_FF("function_begin ");

    m_function = new Function(name->getText(),
                              name->getRange(),
                              function->getLineNo(),
							  name->getRange().getLast());
    m_pou = m_function;
    SetFileSysComment();
    DEBUG_FF("OK");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::function_clear
 * Description      sets the member m_function to NULL
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::function_clear()
//================================================================================
{

    DEBUG_FF("function_clear");
    if(m_function) {
        m_function = 0;
    }
    DEBUG_FF("OK");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::function_set_function_type
 * Description      sets the return type of an st function
 * @param           LexVal* type
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::function_set_function_type(LexVal* type)
//================================================================================
{

    ASSERT(type != 0);
    DEBUG_FF("function_set_function_type");

    ASSERT(m_function != NULL);
    m_function->setFunctionType(type->getText());
    m_function->setFunctionTypeRange(type->getRange());
    m_function->setNextVarSectInsertPos(type->getRange().getLast());

    DEBUG_FF("OK");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::struct_begin
 * Description      allocates a new StStruct object
 * @param           LexVal* st
 * @param           LexVal* name
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::struct_begin(LexVal* st, LexVal* name)
//================================================================================
{

    ASSERT(st != 0);
    ASSERT(name != 0);
    DEBUG_FF("struct_begin ");

    m_struct = new StStruct(name->getText(),
                            name->getRange(),
                            st->getLineNo(),
						    name->getRange().getLast());
    m_pou = m_struct;
    SetFileSysComment();
    DEBUG_FF("OK");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::struct_clear
 * Description      sets the member m_struct to NULL
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::struct_clear()
//================================================================================
{

    DEBUG_FF("struct_clear");
    if(m_struct) {
        m_struct = 0;
    }
    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::pou
 * Description      sets the system comment of the member m_pou
 * @param           SysComment* sc
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::SetFileSysComment()
//================================================================================
{

    DEBUG_FF("SetFileSysComment");
    if (m_pou)
    {
        if (m_syscmnt) {
            m_pou->setFileSysComment(m_syscmnt);
            //avoid deletion
            m_syscmnt = NULL;
        }
        else
        {
            DEBUG_FF("NO SYSCMNT");
        }
    }
    else
    {
        DEBUG_FF("NO POU");
    }
    DEBUG_FF("OK");

}

void ParserContext::SetFileSysComment(SysComment* p)
{
    m_syscmnt = p;
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::pou_clear
 * Description      sets the member m_pou to NULL
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::pou_clear()
//================================================================================
{

    DEBUG_FF("pou_clear");
    m_pou = 0;
    DEBUG_FF("OK");

}



//------------------------------------------------------------------*
/**
 * Handling of error symbol for pou.
 * @return		int - Return code for YYRETURN. 0 - indicates
 *              YYACCEPT, 1 - indicates YYABORT
*/
int ParserContext::pou_error()
{
    fb_clear();
    program_clear();
    function_clear();
    resource_clear();
    struct_clear();
    globvarconstlist_clear();

    if(m_pou) {
        return 0; // == YYACCEPT
    }
    else return 1; // == YYABORT
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::import_directive
 * Description      allocates a new ImportDirective object
 * @param           LexVal* imp_dir
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::import_directive(LexVal* imp_dir)
//================================================================================
{
    ASSERT(imp_dir != 0);
    ASSERT(m_pou != 0);
    DEBUG_FF("import_directive");

    string type = "UNSPECIFIED";
    if ((m_resource != 0) || 
        (m_program != 0) || 
        (m_fb != 0) || 
        (m_function != 0))
    {
        type = "GLOB_VAR_FILE_REF";
    }
    else if (m_configuration != 0)
    {
        type = "RESOURCE_REF";
    }

    m_importdirective = new ImportDirective(imp_dir->getText(), 
                                            imp_dir->getRange(),
                                            type); 
    m_pou->addImportDirective(m_importdirective);
    DEBUG_FF("OK");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::vardeclsect_begin
 * Description      allocates a new VarDeclSection object
 * @param           LexVal* var
 * @param           LexVal* modifier
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::vardeclsect_begin(LexVal* var, LexVal* modifier)
//================================================================================
{ 

    ASSERT(var != 0);

    DEBUG_FF("var_decl_sect_begin");
 
    m_vardeclsect = new VarDeclSection(var->getText(), 
                                       var->getRange(),
                                       (modifier == NULL ? "" : 
                                                           modifier->getText()),
                                       (modifier == NULL ? StringPosRange(var->getRange().getLast(), 
                                                                          var->getRange().getLast()) : 
                                                           modifier->getRange()),
                                       var->getStartPos());
    m_vardeclsect_found = true;
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::vardeclsect_end
 * Description      sets the end position of the member m_vardeclsect 
 *                  and adds it to the list of variable declatation 
 *                  sections of member m_pou
 * @param           LexVal* end
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::vardeclsect_end(LexVal* end)
//================================================================================
{

    ASSERT(end != 0);
    ASSERT(m_vardeclsect != 0);
    ASSERT(m_pou != 0);
    DEBUG_FF("var_declsect_end");

    m_vardeclsect->setEndPos(end->getEndPos()); 
    m_pou->addVarDeclSect(m_vardeclsect);
    vardeclsect_clear();

    DEBUG_FF("OK6");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::vardeclsect_clear
 * Description      sets the member m_vardeclsect to NULL
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::vardeclsect_clear()
//================================================================================
{

    DEBUG_FF("var_declsect_clear");
    m_vardeclsect = 0;
    DEBUG_FF("OK7");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::vardecl_begin
 * Description      allocates a new VarDecl object
 * @param           LexVal* firstelem
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::vardecl_begin(LexVal* firstelem)
//================================================================================
{

    ASSERT(firstelem != 0);
    ASSERT(! m_varlistopened);

    DEBUG_FF("vardecl_begin");

    m_vardecl = new VarDecl(firstelem->getText(), firstelem->getStartPos());
    m_varlistopened = true;

    DEBUG_FF("OK8");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::vardecl_end
 * Description      sets the end position of the member m_vardecl
 * @param           LexVal* end
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::vardecl_end(LexVal* end)
//================================================================================
{

    DEBUG_FF("var_decl_end");

    ASSERT(end != 0);
    ASSERT(m_vardecl != 0);
    ASSERT(m_vardeclsect != 0);
    ASSERT(m_varlistopened);

    m_vardecl->setEndPos(end->getEndPos());

}
//------------------------------------------------------------------*
/**
 * Function name    ParserContext::vardecl_end_without_varcomment
 * Description      sets the end position of the member m_vardecl
 * @param           LexVal* end
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::vardecl_end_without_varcomment(LexVal* end)
//================================================================================
{

    DEBUG_FF("var_decl_end");

    ASSERT(end != 0);
    ASSERT(m_vardecl != 0);
    ASSERT(m_vardeclsect != 0);
    ASSERT(m_varlistopened);

    m_vardecl->setEndPos(end->getEndPos() + 1);
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::vardecl_type
 * Description      sets the type of the variable declaration
 * @param           LexVal* type
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::vardecl_type(LexVal* type)
//================================================================================
{

    ASSERT(type != 0);
    ASSERT(m_vardecl != 0);

    DEBUG_FF("vardecl_type ");

    m_vardecl->setType(type->getText());
    m_vardecl->setTypeRange(type->getRange());

    m_vardecl->setInitializerFirst(type->getRange().getLast());
    m_vardecl->setInitializerLast(type->getRange().getLast());

    DEBUG_FF("OK10");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::vardecl_edge
 * Description      sets the edge of the variable declaration
 * @param           LexVal* edge
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::vardecl_edge(LexVal* edge)
//================================================================================
{

    ASSERT(edge != 0);
    ASSERT(m_vardecl != 0);

    DEBUG_FF("vardecl_edge");

    m_vardecl->setEdge(edge->getText());
    m_vardecl->setEdgeRange(edge->getRange());

    m_vardecl->setInitializerFirst(edge->getRange().getLast());
    m_vardecl->setInitializerLast(edge->getRange().getLast());

    DEBUG_FF("OK10");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::vardecl_var_comment
 * Description      sets the variable comment of the variable declaration
 * @param           LexVal* var_vomment
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::vardecl_var_comment(LexVal* var_comment)
//================================================================================
{
    ASSERT(var_comment != 0);
    ASSERT(m_vardecl != 0);

    DEBUG_FF("vardecl_var_comment");

    m_vardecl->setVarComment(var_comment->getText());
    m_vardecl->setVarCommentRange(var_comment->getRange());
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::vardecl_clear
 * Description      adds the variable declaration to the variable declaration section
 *                  and sets the member m_vardecl to NULL
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::vardecl_clear()
//================================================================================
{
    DEBUG_FF("var_decl_clear");

    m_vardeclsect->addVarDecl(m_vardecl);

    m_varlistopened = false;
    m_vardecl = 0;

    DEBUG_FF("OK11");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::varlistelem_begin
 * Description      adds a variable name to a variable declaration
 *                  (the variable declaration object is created if it does 
 *                  not already exist)
 * @param           LexVal* name
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::varlistelem_begin(LexVal* name)
//================================================================================
{
    ASSERT(name != 0);

    DEBUG_FF("varlistelem_begin");

    if ( ! m_varlistopened )
    {
        vardecl_begin(name); // does NOT clean up "name" !!!
    }
    else
    {
        ASSERT(m_vardecl != 0);
        m_vardecl->addVarListElem(name->getText(), name->getStartPos());
    }

    DEBUG_FF("OK12");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::varlistelem_end
 * Description      sets the end position of the last element in the variable list
 * @param           LexVal* end
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::varlistelem_end(LexVal* end)
//================================================================================
{
   ASSERT(end != 0);

   DEBUG_FF("varlistelem_end");

   ASSERT(m_vardecl != 0);

   m_vardecl->setLastVarListElemEnd(end->getStartPos());

   DEBUG_FF("OK13");
}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::varlistelem_address
 * Description      sets the address and the range of the address
 *                  of the last element in the variable list
 * @param           LexVal* address
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::varlistelem_address(LexVal* address)
//================================================================================
{

   ASSERT(address != 0);

   DEBUG_FF("varlistelem_address");

   ASSERT(m_vardecl != 0);

   m_vardecl->setLastVarListElemAddress(address->getText());
   m_vardecl->setLastVarListElemAddressRange(address->getRange());

   DEBUG_FF("OK13");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::varlistelem_syscomment
 * Description      sets the system comment of the last element in the variable list
 * @param           SysComment* comment
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::varlistelem_syscomment(SysComment* comment)
//================================================================================
{

   DEBUG_FF("varlistelem_syscomment");

   ASSERT(m_vardecl != 0);

   m_vardecl->setLastVarListElemSysComment(comment);

   DEBUG_FF("OK13");

}

//------------------------------------------------------------------*
/**
 * Function name    ParserContext::extract_array_type
 * Description      gets the array type from the scanner and writes it to the parameter end
 *                  It is done this way in order to avoid parsing the whole array type deklaration.
 * @param           LexVal* start
 * @param           LexVal* end
 * @return          -
 * @exception       -
 * @see             -
*/
//================================================================================
void ParserContext::extract_array_type(LexVal* start, LexVal* end)
//================================================================================
{
    StringPosRange spr(start->getStartPos() - 1, 
                       end->getEndPos() - 1); 
    end->set(getStreamScanner()->getSubString(spr).c_str(), 
             end->getEndPos(), 
             end->getLineNo());

}


void ParserContext::SuppressError()
{
    if(m_pou) {
        m_pou->SuppressError();
    }
}
