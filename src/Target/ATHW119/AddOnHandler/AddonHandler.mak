#automatically generated by C:\ATCMControl_SDK\GenWork\GenDir.tmp\4Control_R\gentools\make.pl: Version=$Revision: 1 $

!ifndef PROJECT_ROOT
!MESSAGE check environmemt: environment variable PROJECT_ROOT not defined
!ERROR PROJECT_ROOT must point to the project base directory
!endif

STATIC_RUNTIME = 0
!include <$(PROJECT_ROOT)\gentools\global.inc>
makefirst:$(EXE_DIR)\Engineering\bin\ATHW119
$(EXE_DIR)\Engineering\bin\ATHW119:
  -md $@
doxygen: AddonHandler.doxy
	$(CP) $(PROJECT_ROOT)\gentools\4C_doxy.css .\4C_doxy.css
	doxygen AddonHandler.doxy
AddonHandler.doxy:
	$(CP) $(PROJECT_ROOT)\gentools\Sample.doxy .\AddonHandler.doxy
oleclasses: AddOnHandler.h
AddOnHandler.h: AddOnHandler.idl
	$(MIDL) $(MIDLFLAGS) -I ..\inc -I ..\res -I $(TARGET_BASE)\AddonHandler\inc -I $(TARGET_BASE)\AddonHandler\AddonBase -I $(TARGET_BASE)\Kernel\inc -I $(TARGET_BASE)\Kernel\CE\inc -I $(TARGET_BASE)\ProjectWizardBase -D _ATL_STATIC_REGISTRY -D _ATL_NO_SECURITY -D _AFXDLL -D _MRCEXT_NOFORCELIBS -h AddOnHandler.h AddOnHandler.idl
clean::
	-$(RM) AddOnHandler.h
compile: $(OBJ)\AAddOnObject.obj
$(OBJ)\AAddOnObject.obj: AAddOnObject.cpp $(OBJ)
	@$(CXX) @<<
$(CXXFLAGS) -I..\inc -I..\res -I$(TARGET_BASE)\AddonHandler\inc -I$(TARGET_BASE)\AddonHandler\AddonBase -I$(TARGET_BASE)\Kernel\inc -I$(TARGET_BASE)\Kernel\CE\inc -I$(TARGET_BASE)\ProjectWizardBase -D_ATL_STATIC_REGISTRY -D_ATL_NO_SECURITY -D_AFXDLL -D_MRCEXT_NOFORCELIBS -Fo$(OBJ)\AAddOnObject.obj -c $(MAKEDIR)\AAddOnObject.cpp
<<
clean::
	-$(RM) $(OBJ)\AAddOnObject.obj
compile: $(OBJ)\AAddOnParameter.obj
$(OBJ)\AAddOnParameter.obj: AAddOnParameter.cpp $(OBJ)
	@$(CXX) @<<
$(CXXFLAGS) -I..\inc -I..\res -I$(TARGET_BASE)\AddonHandler\inc -I$(TARGET_BASE)\AddonHandler\AddonBase -I$(TARGET_BASE)\Kernel\inc -I$(TARGET_BASE)\Kernel\CE\inc -I$(TARGET_BASE)\ProjectWizardBase -D_ATL_STATIC_REGISTRY -D_ATL_NO_SECURITY -D_AFXDLL -D_MRCEXT_NOFORCELIBS -Fo$(OBJ)\AAddOnParameter.obj -c $(MAKEDIR)\AAddOnParameter.cpp
<<
clean::
	-$(RM) $(OBJ)\AAddOnParameter.obj
compile: $(OBJ)\AddOnHandler.obj
$(OBJ)\AddOnHandler.obj: AddOnHandler.cpp $(OBJ)
	@$(CXX) @<<
$(CXXFLAGS) -I..\inc -I..\res -I$(TARGET_BASE)\AddonHandler\inc -I$(TARGET_BASE)\AddonHandler\AddonBase -I$(TARGET_BASE)\Kernel\inc -I$(TARGET_BASE)\Kernel\CE\inc -I$(TARGET_BASE)\ProjectWizardBase -D_ATL_STATIC_REGISTRY -D_ATL_NO_SECURITY -D_AFXDLL -D_MRCEXT_NOFORCELIBS -Fo$(OBJ)\AddOnHandler.obj -c $(MAKEDIR)\AddOnHandler.cpp
<<
clean::
	-$(RM) $(OBJ)\AddOnHandler.obj
compile: $(OBJ)\AProjectData.obj
$(OBJ)\AProjectData.obj: AProjectData.cpp $(OBJ)
	@$(CXX) @<<
$(CXXFLAGS) -I..\inc -I..\res -I$(TARGET_BASE)\AddonHandler\inc -I$(TARGET_BASE)\AddonHandler\AddonBase -I$(TARGET_BASE)\Kernel\inc -I$(TARGET_BASE)\Kernel\CE\inc -I$(TARGET_BASE)\ProjectWizardBase -D_ATL_STATIC_REGISTRY -D_ATL_NO_SECURITY -D_AFXDLL -D_MRCEXT_NOFORCELIBS -Fo$(OBJ)\AProjectData.obj -c $(MAKEDIR)\AProjectData.cpp
<<
clean::
	-$(RM) $(OBJ)\AProjectData.obj
compile: $(OBJ)\ATargetData.obj
$(OBJ)\ATargetData.obj: ATargetData.cpp $(OBJ)
	@$(CXX) @<<
$(CXXFLAGS) -I..\inc -I..\res -I$(TARGET_BASE)\AddonHandler\inc -I$(TARGET_BASE)\AddonHandler\AddonBase -I$(TARGET_BASE)\Kernel\inc -I$(TARGET_BASE)\Kernel\CE\inc -I$(TARGET_BASE)\ProjectWizardBase -D_ATL_STATIC_REGISTRY -D_ATL_NO_SECURITY -D_AFXDLL -D_MRCEXT_NOFORCELIBS -Fo$(OBJ)\ATargetData.obj -c $(MAKEDIR)\ATargetData.cpp
<<
clean::
	-$(RM) $(OBJ)\ATargetData.obj
compile: $(OBJ)\ProjectWizardDialog.obj
$(OBJ)\ProjectWizardDialog.obj: ProjectWizardDialog.cpp $(OBJ)
	@$(CXX) @<<
$(CXXFLAGS) -I..\inc -I..\res -I$(TARGET_BASE)\AddonHandler\inc -I$(TARGET_BASE)\AddonHandler\AddonBase -I$(TARGET_BASE)\Kernel\inc -I$(TARGET_BASE)\Kernel\CE\inc -I$(TARGET_BASE)\ProjectWizardBase -D_ATL_STATIC_REGISTRY -D_ATL_NO_SECURITY -D_AFXDLL -D_MRCEXT_NOFORCELIBS -Fo$(OBJ)\ProjectWizardDialog.obj -c $(MAKEDIR)\ProjectWizardDialog.cpp
<<
clean::
	-$(RM) $(OBJ)\ProjectWizardDialog.obj
compile: $(OBJ)\StdAfx.obj
$(OBJ)\StdAfx.obj: StdAfx.cpp $(OBJ)
	@$(CXX) @<<
$(CXXFLAGS) -I..\inc -I..\res -I$(TARGET_BASE)\AddonHandler\inc -I$(TARGET_BASE)\AddonHandler\AddonBase -I$(TARGET_BASE)\Kernel\inc -I$(TARGET_BASE)\Kernel\CE\inc -I$(TARGET_BASE)\ProjectWizardBase -D_ATL_STATIC_REGISTRY -D_ATL_NO_SECURITY -D_AFXDLL -D_MRCEXT_NOFORCELIBS -Fo$(OBJ)\StdAfx.obj -c $(MAKEDIR)\StdAfx.cpp
<<
clean::
	-$(RM) $(OBJ)\StdAfx.obj
clean::
	-$(RM) vc60.idb vc60.pdb
compile: $(OBJ)\AddonHandler.res
$(OBJ)\AddonHandler.res: AddonHandler.rc $(OBJ)
	$(RC) $(RCFLAGS) -i ..\inc -i ..\res -i $(TARGET_BASE)\AddonHandler\inc -i $(TARGET_BASE)\AddonHandler\AddonBase -i $(TARGET_BASE)\Kernel\inc -i $(TARGET_BASE)\Kernel\CE\inc -i $(TARGET_BASE)\ProjectWizardBase -d _ATL_STATIC_REGISTRY -d _ATL_NO_SECURITY -d _AFXDLL -d _MRCEXT_NOFORCELIBS -fo$(OBJ)\AddonHandler.res AddonHandler.rc
clean::
	-$(RM) $(OBJ)\AddonHandler.res
link: $(EXE_DIR)\Engineering\bin\ATHW119\AddonHandler.dll
$(EXE_DIR)\Engineering\bin\ATHW119\AddonHandler.dll: $(OBJ)\AAddOnObject.obj $(OBJ)\AAddOnParameter.obj $(OBJ)\AddOnHandler.obj $(OBJ)\AProjectData.obj $(OBJ)\ATargetData.obj $(OBJ)\ProjectWizardDialog.obj $(OBJ)\StdAfx.obj $(OBJ)\AddonHandler.res $(TARGET_BASE)\Kernel\CE\Lib\CEUtil_XML.lib $(TARGET_BASE)\lib\AddonBase.lib $(TARGET_BASE)\lib\ProjectWizardBase.lib $(TARGET_BASE)\Kernel\lib\fc_util.lib $(TARGET_BASE)\Kernel\lib\fc_tools.lib $(TARGET_BASE)\Kernel\CE\Lib\CEUTIL.lib AddOnHandler.def
	$(LNK) @<<
$(LFLAGS) -OUT:$(EXE_DIR)\Engineering\bin\ATHW119\AddonHandler.dll -SUBSYSTEM:WINDOWS -INCREMENTAL:NO -DLL
-DEF:AddOnHandler.def
$(OBJ)\AAddOnObject.obj $(OBJ)\AAddOnParameter.obj $(OBJ)\AddOnHandler.obj $(OBJ)\AProjectData.obj $(OBJ)\ATargetData.obj $(OBJ)\ProjectWizardDialog.obj $(OBJ)\StdAfx.obj 
$(OBJ)\AddonHandler.res
$(TARGET_BASE)\Kernel\CE\Lib\CEUtil_XML.lib $(TARGET_BASE)\lib\AddonBase.lib $(TARGET_BASE)\lib\ProjectWizardBase.lib $(TARGET_BASE)\Kernel\lib\fc_util.lib $(TARGET_BASE)\Kernel\lib\fc_tools.lib $(TARGET_BASE)\Kernel\CE\Lib\CEUTIL.lib kernel32.lib user32.lib gdi32.lib advapi32.lib 
-LIBPATH:$(TARGET_BASE)\Kernel\lib
-LIBPATH:$(TARGET_BASE)\Kernel\CE\lib
<<
	@$(RM) $(EXE_DIR)\Engineering\bin\ATHW119\AddonHandler.lib $(EXE_DIR)\Engineering\bin\ATHW119\AddonHandler.exp
clean::
	-$(RM) $(EXE_DIR)\Engineering\bin\ATHW119\AddonHandler.dll
	-$(RM) $(EXE_DIR)\Engineering\bin\ATHW119\AddonHandler.pdb