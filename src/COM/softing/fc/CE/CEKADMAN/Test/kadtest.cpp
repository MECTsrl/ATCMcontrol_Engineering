#include "stdafx.h"

#include <objbase.h>

#include "CEKadMan.h"
#include "KadManager.h"

#include "CEKadMan_i.c"
#include "KadManager_i.c"

#define TEST_FUNCTION_STRING(ifc,function)                            \
    hr = ifc->function(&sString);                                     \
    if (hr == S_OK) {                                                 \
        fprintf(logFile,#function ": %S\r\n",sString);                \
        SysFreeString(sString);                                       \
    } else {                                                          \
        fprintf(logFile,#function " failed (%08x)\r\n",hr);           \
    }

#define TEST_FUNCTION_GUID(ifc,function)                              \
    hr = ifc->function(&guid);                                        \
    if (hr == S_OK) {                                                 \
        StringFromCLSID(guid,&sString);                               \
        fprintf(logFile,#function ": %S\r\n",sString);                \
        CoTaskMemFree(sString);                                       \
    } else {                                                          \
        fprintf(logFile,#function " failed (%08x)\r\n",hr);           \
    }

#define TEST_FUNCTION_BOOL(ifc,function)                              \
    hr = ifc->function(&bBool);                                       \
    if (hr == S_OK) {                                                 \
        fprintf(logFile,#function ": %c\r\n",(bBool ? 'T' : 'F'));    \
    } else {                                                          \
        fprintf(logFile,#function " failed (%08x)\r\n",hr);           \
    }

#define TEST_FUNCTION_WORD(ifc,function)                              \
    hr = ifc->function(&wWord);                                       \
    if (hr == S_OK) {                                                 \
        fprintf(logFile,#function ": %x\r\n",wWord);                  \
    } else {                                                          \
        fprintf(logFile,#function " failed (%08x)\r\n",hr);           \
    }

#define TEST_FUNCTION_TYPE(ifc,function)                              \
    hr = ifc->function(&pType);                                       \
    if (hr != S_OK) {                                                 \
        fprintf(logFile,#function " failed (%08x)\r\n",hr);           \
    } else {                                                          \
        fprintf(logFile,#function " succeeded\r\n");                  \
        TEST_FUNCTION_STRING(pType,GetId)                             \
        pType->Release();                                             \
    }


void GeneralTest(IUnknown *pKadManager, FILE *logFile)
{
	HRESULT hr;
	BSTR sString;
    GUID guid;
	IGeneral *pGeneral;

    fprintf(logFile,"====================================================\r\n");
    fprintf(logFile,"IGeneral\r\n");
    fprintf(logFile,"====================================================\r\n");
    hr = pKadManager->QueryInterface(IID_IGeneral,(LPVOID *) &pGeneral);
	if (FAILED(hr)) {
        fprintf(logFile,"QueryInterface failed (IID_IGeneral)\r\n");
	} else {
        TEST_FUNCTION_STRING(pGeneral,GetCompilerPath)
        TEST_FUNCTION_STRING(pGeneral,GetSplashScreenFilename)
        TEST_FUNCTION_STRING(pGeneral,GetDesktopIcon)
        TEST_FUNCTION_GUID(pGeneral,GetAboutBoxHandler)
		pGeneral->Release();
	}
}

void FilesTest(IUnknown *pKadManager, FILE *logFile)
{
    BOOL bDone;
    ULONG ulFiles;
	HRESULT hr;
    IFiles *pFiles;
    IFile *pFile;
    IEnumFiles *pEnumFiles;
    BSTR sString;

    fprintf(logFile,"====================================================\r\n");
    fprintf(logFile,"IFiles\r\n");
    fprintf(logFile,"====================================================\r\n");
    hr = pKadManager->QueryInterface(IID_IFiles,(LPVOID *) &pFiles);
	if (FAILED(hr)) {
        fprintf(logFile,"QueryInterface failed (IID_IFiles)\r\n");
	} else {
        hr = pFiles->GetFileEnumerator(&pEnumFiles);
        if (hr != S_OK) {
            fprintf(logFile,"GetFileEnumerator failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetFileEnumerator succeeded\r\n");
            hr = pEnumFiles->Reset();
            if (hr != S_OK) {
                fprintf(logFile,"Reset failed (%08x)\r\n",hr);
            } else {
                fprintf(logFile,"Reset succeeded\r\n");
                bDone = FALSE;
                while (! bDone) {
                    hr = pEnumFiles->Next(1,&pFile,&ulFiles);
                    if (hr != S_OK) {
                        fprintf(logFile,"Next failed (%08x)\r\n",hr);
                        bDone = TRUE;
                    } else {
                        fprintf(logFile,"Next succeeded\r\n");
                        TEST_FUNCTION_STRING(pFile,GetExtension)
                        pFile->Release();
                    }
                }
            }
            pEnumFiles->Release();
        }
        hr = pFiles->GetFileByExtension(L"pc",&pFile);
        if (hr != S_OK) {
            fprintf(logFile,"GetFileByExtension failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetFileByExtension succeeded\r\n");
            TEST_FUNCTION_STRING(pFile,GetExtension)
            pFile->Release();
        }
		pFiles->Release();
	}
}

void ClassesTest(IUnknown *pKadManager, FILE *logFile)
{
    BOOL bDone;
	HRESULT hr;
    ULONG ulClasses;
    IClasses *pClasses;
    IClass *pClass;
    IType *pType;
    IEnumClasses *pEnumClasses;
    BSTR sString;

    fprintf(logFile,"====================================================\r\n");
    fprintf(logFile,"IClasses\r\n");
    fprintf(logFile,"====================================================\r\n");
    hr = pKadManager->QueryInterface(IID_IClasses,(LPVOID *) &pClasses);
	if (FAILED(hr)) {
        fprintf(logFile,"QueryInterface failed (IID_IClasses)\r\n");
	} else {
        TEST_FUNCTION_STRING(pClasses,GetClassesTypeString)
        TEST_FUNCTION_TYPE(pClasses,GetClassesType)
        hr = pClasses->GetClassEnumerator(&pEnumClasses);
        if (hr != S_OK) {
            fprintf(logFile,"GetClassEnumerator failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetClassEnumerator succeeded\r\n");
            hr = pEnumClasses->Reset();
            if (hr != S_OK) {
                fprintf(logFile,"Reset failed (%08x)\r\n",hr);
            } else {
                fprintf(logFile,"Reset succeeded\r\n");
                bDone = FALSE;
                while (! bDone) {
                    hr = pEnumClasses->Next(1,&pClass,&ulClasses);
                    if (hr != S_OK) {
                        fprintf(logFile,"Next failed (%08x)\r\n",hr);
                        bDone = TRUE;
                    } else {
                        fprintf(logFile,"Next succeeded\r\n");
                        TEST_FUNCTION_STRING(pClass,GetGroupText)
                        pClass->Release();
                    }
                }
            }
            pEnumClasses->Release();
        }
        hr = pClasses->GetClassByType(L"DATA_TYPE_GROUP",&pClass);
        if (hr != S_OK) {
            fprintf(logFile,"GetClassByType failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetClassByType succeeded\r\n");
            TEST_FUNCTION_STRING(pClass,GetGroupText)
            pClass->Release();
        }
		pClasses->Release();
	}
}

void MainMenuTest(IUnknown *pKadManager, FILE *logFile)
{
    BOOL bDone;
	HRESULT hr;
    ULONG ulMenus;
    IMainMenu *pMainMenu;
    IMenu *pMenu;
    IEnumMenus *pEnumMenus;
    BSTR sString;

    fprintf(logFile,"====================================================\r\n");
    fprintf(logFile,"IMainMenu\r\n");
    fprintf(logFile,"====================================================\r\n");
    hr = pKadManager->QueryInterface(IID_IMainMenu,(LPVOID *) &pMainMenu);
	if (FAILED(hr)) {
        fprintf(logFile,"QueryInterface failed (IID_IMainMenu)\r\n");
	} else {
        hr = pMainMenu->GetMenuEnumerator(&pEnumMenus);
        if (hr != S_OK) {
            fprintf(logFile,"GetMenuEnumerator failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetMenuEnumerator succeeded\r\n");
            hr = pEnumMenus->Reset();
            if (hr != S_OK) {
                fprintf(logFile,"Reset failed (%08x)\r\n",hr);
            } else {
                fprintf(logFile,"Reset succeeded\r\n");
                bDone = FALSE;
                while (! bDone) {
                    hr = pEnumMenus->Next(1,&pMenu,&ulMenus);
                    if (hr != S_OK) {
                        fprintf(logFile,"Next failed (%08x)\r\n",hr);
                        bDone = TRUE;
                    } else {
                        fprintf(logFile,"Next succeeded\r\n");
                        TEST_FUNCTION_STRING(pMenu,GetId)
                        pMenu->Release();
                    }
                }
            }
            pEnumMenus->Release();
        }
        hr = pMainMenu->GetMenuById(L"Tools",&pMenu);
        if (hr != S_OK) {
            fprintf(logFile,"GetMenuById failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetMenuById succeeded\r\n");
            TEST_FUNCTION_STRING(pMenu,GetId)
            pMenu->Release();
        }
		pMainMenu->Release();
	}
}

void ToolbarsTest(IUnknown *pKadManager, FILE *logFile)
{
    BOOL bDone;
	HRESULT hr;
    ULONG ulToolbars;
    IToolbars *pToolbars;
    IToolbar *pToolbar;
    IEnumToolbars *pEnumToolbars;
    BSTR sString;

    fprintf(logFile,"====================================================\r\n");
    fprintf(logFile,"IToolbars\r\n");
    fprintf(logFile,"====================================================\r\n");
    hr = pKadManager->QueryInterface(IID_IToolbars,(LPVOID *) &pToolbars);
	if (FAILED(hr)) {
        fprintf(logFile,"QueryInterface failed (IID_IToolbars)\r\n");
	} else {
        hr = pToolbars->GetToolbarEnumerator(&pEnumToolbars);
        if (hr != S_OK) {
            fprintf(logFile,"GetToolbarEnumerator failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetToolbarEnumerator succeeded\r\n");
            hr = pEnumToolbars->Reset();
            if (hr != S_OK) {
                fprintf(logFile,"Reset failed (%08x)\r\n",hr);
            } else {
                fprintf(logFile,"Reset succeeded\r\n");
                bDone = FALSE;
                while (! bDone) {
                    hr = pEnumToolbars->Next(1,&pToolbar,&ulToolbars);
                    if (hr != S_OK) {
                        fprintf(logFile,"Next failed (%08x)\r\n",hr);
                        bDone = TRUE;
                    } else {
                        fprintf(logFile,"Next succeeded\r\n");
                        TEST_FUNCTION_STRING(pToolbar,GetId)
                        pToolbar->Release();
                    }
                }
            }
            pEnumToolbars->Release();
        }
        hr = pToolbars->GetToolbarById(L"TOOLBAR2",&pToolbar);
        if (hr != S_OK) {
            fprintf(logFile,"GetToolbarById failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetToolbarById succeeded\r\n");
            TEST_FUNCTION_STRING(pToolbar,GetId)
            pToolbar->Release();
        }
		pToolbars->Release();
	}
}

void TypesTest(IUnknown *pKadManager, FILE *logFile)
{
    BOOL bDone;
	HRESULT hr;
    ULONG ulTypes;
    ITypes *pTypes;
    IType *pType;
    IEnumTypes *pEnumTypes;
    BSTR sString;

    fprintf(logFile,"====================================================\r\n");
    fprintf(logFile,"ITypes\r\n");
    fprintf(logFile,"====================================================\r\n");
    hr = pKadManager->QueryInterface(IID_ITypes,(LPVOID *) &pTypes);
	if (FAILED(hr)) {
        fprintf(logFile,"QueryInterface failed (IID_ITypes)\r\n");
	} else {
        hr = pTypes->GetTypeEnumerator(&pEnumTypes);
        if (hr != S_OK) {
            fprintf(logFile,"GetTypeEnumerator failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetTypeEnumerator succeeded\r\n");
            hr = pEnumTypes->Reset();
            if (hr != S_OK) {
                fprintf(logFile,"Reset failed (%08x)\r\n",hr);
            } else {
                fprintf(logFile,"Reset succeeded\r\n");
                bDone = FALSE;
                while (! bDone) {
                    hr = pEnumTypes->Next(1,&pType,&ulTypes);
                    if (hr != S_OK) {
                        fprintf(logFile,"Next failed (%08x)\r\n",hr);
                        bDone = TRUE;
                    } else {
                        fprintf(logFile,"Next succeeded\r\n");
                        TEST_FUNCTION_STRING(pType,GetId)
                        pType->Release();
                    }
                }
            }
            pEnumTypes->Release();
        }
        hr = pTypes->GetTypeById(L"RESOURCE",&pType);
        if (hr != S_OK) {
            fprintf(logFile,"GetTypeById failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetTypeById succeeded\r\n");
            TEST_FUNCTION_STRING(pType,GetId)
            pType->Release();
        }
		pTypes->Release();
	}
}

void TargetsTest(IUnknown *pKadManager, FILE *logFile)
{
    BOOL bDone;
	HRESULT hr;
    ULONG ulTargets;
    ITargets *pTargets;
    ITarget *pTarget;
    IType *pType;
    IEnumTargets *pEnumTargets;
    BSTR sString;

    fprintf(logFile,"====================================================\r\n");
    fprintf(logFile,"ITargets\r\n");
    fprintf(logFile,"====================================================\r\n");
    hr = pKadManager->QueryInterface(IID_ITargets,(LPVOID *) &pTargets);
	if (FAILED(hr)) {
        fprintf(logFile,"QueryInterface failed (IID_ITargets)\r\n");
	} else {
        TEST_FUNCTION_STRING(pTargets,GetTargetsTypeString)
        TEST_FUNCTION_TYPE(pTargets,GetTargetsType)
        hr = pTargets->GetTargetEnumerator(&pEnumTargets);
        if (hr != S_OK) {
            fprintf(logFile,"GetTargetEnumerator failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetTargetEnumerator succeeded\r\n");
            hr = pEnumTargets->Reset();
            if (hr != S_OK) {
                fprintf(logFile,"Reset failed (%08x)\r\n",hr);
            } else {
                fprintf(logFile,"Reset succeeded\r\n");
                bDone = FALSE;
                while (! bDone) {
                    hr = pEnumTargets->Next(1,&pTarget,&ulTargets);
                    if (hr != S_OK) {
                        fprintf(logFile,"Next failed (%08x)\r\n",hr);
                        bDone = TRUE;
                    } else {
                        fprintf(logFile,"Next succeeded\r\n");
                        TEST_FUNCTION_STRING(pTarget,GetTypeString)
                        pTarget->Release();
                    }
                }
            }
            pEnumTargets->Release();
        }
        hr = pTargets->GetTargetByType(L"PC",&pTarget);
        if (hr != S_OK) {
            fprintf(logFile,"GetTargetByType failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetTargetByType succeeded\r\n");
            TEST_FUNCTION_STRING(pTarget,GetTypeString)
            pTarget->Release();
        }
		pTargets->Release();
	}
}

void FileTest(IUnknown *pKadManager, FILE *logFile)
{
	HRESULT hr;
    GUID guid;
    BSTR sString;
    IFiles *pFiles;
    IFile *pFile;

    fprintf(logFile,"====================================================\r\n");
    fprintf(logFile,"IFile\r\n");
    fprintf(logFile,"====================================================\r\n");
    hr = pKadManager->QueryInterface(IID_IFiles,(LPVOID *) &pFiles);
	if (FAILED(hr)) {
        fprintf(logFile,"QueryInterface failed (IID_IFiles)\r\n");
	} else {
        hr = pFiles->GetFileByExtension(L"4gr",&pFile);
        if (hr != S_OK) {
            fprintf(logFile,"GetFileByExtension failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetFileByExtension succeeded\r\n");
            TEST_FUNCTION_STRING(pFile,GetExtension)
            TEST_FUNCTION_STRING(pFile,GetIcon)
            TEST_FUNCTION_GUID(pFile,GetParser)
            TEST_FUNCTION_GUID(pFile,GetStandardEditor)
            TEST_FUNCTION_GUID(pFile,GetProgramEditor)
            TEST_FUNCTION_STRING(pFile,GetExeEditor)
            TEST_FUNCTION_GUID(pFile,GetContainer)
            TEST_FUNCTION_GUID(pFile,GetHandler)
            TEST_FUNCTION_STRING(pFile,GetDispatchId)
            TEST_FUNCTION_GUID(pFile,GetConfigurator)
            pFile->Release();
        }
        pFiles->Release();
	}
}

void ClassTest(IUnknown *pKadManager, FILE *logFile)
{
	HRESULT hr;
    IClasses *pClasses;
    IClass *pClass;
    IType *pType;
    BSTR sString;

    fprintf(logFile,"====================================================\r\n");
    fprintf(logFile,"IClass\r\n");
    fprintf(logFile,"====================================================\r\n");
    hr = pKadManager->QueryInterface(IID_IClasses,(LPVOID *) &pClasses);
	if (FAILED(hr)) {
        fprintf(logFile,"QueryInterface failed (IID_IClasses)\r\n");
	} else {
        hr = pClasses->GetClassByType(L"RESOURCE_GROUP",&pClass);
        if (hr != S_OK) {
            fprintf(logFile,"GetClassByType failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetClassByType succeeded\r\n");
            TEST_FUNCTION_STRING(pClass,GetSection)
            TEST_FUNCTION_STRING(pClass,GetGroupText)
            TEST_FUNCTION_STRING(pClass,GetTypeString)
            TEST_FUNCTION_STRING(pClass,GetClassType)
            TEST_FUNCTION_TYPE(pClass,GetType)
            pClass->Release();
        }
        pClasses->Release();
	}
}

void TargetTest(IUnknown *pKadManager, FILE *logFile)
{
	HRESULT hr;
    ITargets *pTargets;
    ITarget *pTarget;
    ITargetOnline *pTargetOnline;
    IType *pType;
    BSTR sString;
    GUID guid;
    WORD wWord;

    fprintf(logFile,"====================================================\r\n");
    fprintf(logFile,"ITarget\r\n");
    fprintf(logFile,"====================================================\r\n");
    hr = pKadManager->QueryInterface(IID_ITargets,(LPVOID *) &pTargets);
	if (FAILED(hr)) {
        fprintf(logFile,"QueryInterface failed (IID_ITargets)\r\n");
	} else {
        hr = pTargets->GetTargetByType(L"PC",&pTarget);
        if (hr != S_OK) {
            fprintf(logFile,"GetTargetByType failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetTargetByType succeeded\r\n");
            TEST_FUNCTION_STRING(pTarget,GetTypeString)
            TEST_FUNCTION_TYPE(pTarget,GetType)
            TEST_FUNCTION_WORD(pTarget,GetFacilityCode)
            hr = pTarget->GetTargetOnline(&pTargetOnline);
            if (hr != S_OK) {
                fprintf(logFile,"GetTargetOnline failed (%08x)\r\n",hr);
            } else {
                fprintf(logFile,"GetTargetOnline succeeded\r\n");
                TEST_FUNCTION_GUID(pTargetOnline,GetCustomConnect)
                pTargetOnline->Release();
            }
            pTarget->Release();
        }
        pTargets->Release();
	}
}

void MenuTest(IUnknown *pKadManager, FILE *logFile)
{
	HRESULT hr;
    BOOL bDone;
    IMainMenu *pMainMenu;
    IMenu *pMenu;
    IMenuItem *pMenuItem;
    BSTR sString;
    GUID guid;
    ULONG ulMenuItems;
    IEnumMenuItems *pEnumMenuItems;

    fprintf(logFile,"====================================================\r\n");
    fprintf(logFile,"IMenu\r\n");
    fprintf(logFile,"====================================================\r\n");
    hr = pKadManager->QueryInterface(IID_IMainMenu,(LPVOID *) &pMainMenu);
	if (FAILED(hr)) {
        fprintf(logFile,"QueryInterface failed (IID_IMainMenu)\r\n");
	} else {
        hr = pMainMenu->GetMenuById(L"Tools",&pMenu);
        if (hr != S_OK) {
            fprintf(logFile,"GetMenuById failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetMenuById succeeded\r\n");
            TEST_FUNCTION_STRING(pMenu,GetId)
            TEST_FUNCTION_STRING(pMenu,GetTitle)
            TEST_FUNCTION_GUID(pMenu,GetHandler)
            hr = pMenu->GetMenuItemEnumerator(&pEnumMenuItems);
            if (hr != S_OK) {
                fprintf(logFile,"GetMenuItemEnumerator failed (%08x)\r\n",hr);
            } else {
                fprintf(logFile,"GetMenuItemEnumerator succeeded\r\n");
                hr = pEnumMenuItems->Reset();
                if (hr != S_OK) {
                    fprintf(logFile,"Reset failed (%08x)\r\n",hr);
                } else {
                    fprintf(logFile,"Reset succeeded\r\n");
                    bDone = FALSE;
                    while (! bDone) {
                        hr = pEnumMenuItems->Next(1,&pMenuItem,&ulMenuItems);
                        if (hr != S_OK) {
                            fprintf(logFile,"Next failed (%08x)\r\n",hr);
                            bDone = TRUE;
                        } else {
                            fprintf(logFile,"Next succeeded\r\n");
                            TEST_FUNCTION_STRING(pMenuItem,GetText)
                            pMenuItem->Release();
                        }
                    }
                }
                pEnumMenuItems->Release();
            }
            hr = pMenu->GetItemByText(L"Field PC Configuration",&pMenuItem);
            if (hr != S_OK) {
                fprintf(logFile,"GetItemByText failed (%08x)\r\n",hr);
            } else {
                fprintf(logFile,"GetItemByText succeeded\r\n");
                TEST_FUNCTION_STRING(pMenuItem,GetText)
                pMenuItem->Release();
            }
		    pMenu->Release();
        }
        pMainMenu->Release();
	}
}

void MenuItemTest(IUnknown *pKadManager, FILE *logFile)
{
	HRESULT hr;
    BOOL bBool;
    IMainMenu *pMainMenu;
    IMenu *pMenu;
    IMenuItem *pMenuItem;
    BSTR sString;
    GUID guid;

    fprintf(logFile,"====================================================\r\n");
    fprintf(logFile,"IMenuItem\r\n");
    fprintf(logFile,"====================================================\r\n");
    hr = pKadManager->QueryInterface(IID_IMainMenu,(LPVOID *) &pMainMenu);
	if (FAILED(hr)) {
        fprintf(logFile,"QueryInterface failed (IID_IMainMenu)\r\n");
	} else {
        hr = pMainMenu->GetMenuById(L"Tools",&pMenu);
        if (hr != S_OK) {
            fprintf(logFile,"GetMenuById failed (%08x)\r\n",hr);
        } else {
            hr = pMenu->GetItemByText(L"Field PC Configuration",&pMenuItem);
            if (hr != S_OK) {
                fprintf(logFile,"GetItemByText failed (%08x)\r\n",hr);
            } else {
                fprintf(logFile,"GetItemByText succeeded\r\n");
                TEST_FUNCTION_STRING(pMenuItem,GetText)
                TEST_FUNCTION_STRING(pMenuItem,GetIcon)
                TEST_FUNCTION_GUID(pMenuItem,GetHandler)
                TEST_FUNCTION_STRING(pMenuItem,GetExeHandler)
                TEST_FUNCTION_STRING(pMenuItem,GetDispatchId)
                TEST_FUNCTION_STRING(pMenuItem,GetStandardOperation)
                TEST_FUNCTION_STRING(pMenuItem,GetStatusBarText)
                TEST_FUNCTION_BOOL(pMenuItem,GetSeparatorFlag)
                TEST_FUNCTION_STRING(pMenuItem,GetKeyMouseEvent)
                TEST_FUNCTION_STRING(pMenuItem,GetEnableExpression)
                TEST_FUNCTION_STRING(pMenuItem,GetDisableExpression)
                pMenuItem->Release();
            }
		    pMenu->Release();
        }
        pMainMenu->Release();
	}
}

void ToolbarTest(IUnknown *pKadManager, FILE *logFile)
{
	HRESULT hr;
    BOOL bDone;
    IToolbars *pToolbars;
    IToolbar *pToolbar;
    IToolbarItem *pToolbarItem;
    GUID guid;
    BSTR sString;
    ULONG ulToolbarItems;
    IEnumToolbarItems *pEnumToolbarItems;

    fprintf(logFile,"====================================================\r\n");
    fprintf(logFile,"IToolbar\r\n");
    fprintf(logFile,"====================================================\r\n");
    hr = pKadManager->QueryInterface(IID_IToolbars,(LPVOID *) &pToolbars);
	if (FAILED(hr)) {
        fprintf(logFile,"QueryInterface failed (IID_IToolbars)\r\n");
	} else {
        hr = pToolbars->GetToolbarById(L"TOOLBAR2",&pToolbar);
        if (hr != S_OK) {
            fprintf(logFile,"GetToolbarById failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetToolbarById succeeded\r\n");
            TEST_FUNCTION_STRING(pToolbar,GetId)
            TEST_FUNCTION_GUID(pToolbar,GetHandler)
            hr = pToolbar->GetToolbarItemEnumerator(&pEnumToolbarItems);
            if (hr != S_OK) {
                fprintf(logFile,"GetToolbarItemEnumerator failed (%08x)\r\n",hr);
            } else {
                fprintf(logFile,"GetToolbarItemEnumerator succeeded\r\n");
                hr = pEnumToolbarItems->Reset();
                if (hr != S_OK) {
                    fprintf(logFile,"Reset failed (%08x)\r\n",hr);
                } else {
                    fprintf(logFile,"Reset succeeded\r\n");
                    bDone = FALSE;
                    while (! bDone) {
                        hr = pEnumToolbarItems->Next(1,&pToolbarItem,&ulToolbarItems);
                        if (hr != S_OK) {
                            fprintf(logFile,"Next failed (%08x)\r\n",hr);
                            bDone = TRUE;
                        } else {
                            fprintf(logFile,"Next succeeded\r\n");
                            TEST_FUNCTION_STRING(pToolbarItem,GetDispatchId)
                            pToolbarItem->Release();
                        }
                    }
                }
                pEnumToolbarItems->Release();
            }
            pToolbar->Release();
        }
        pToolbars->Release();
	}
}

void ToolbarItemTest(IUnknown *pKadManager, FILE *logFile)
{
	HRESULT hr;
    BOOL bBool;
    IToolbars *pToolbars;
    IToolbar *pToolbar;
    IToolbarItem *pToolbarItem;
    BSTR sString;
    GUID guid;
    IEnumToolbarItems *pEnumToolbarItems;
    ULONG ulToolbarItems;

    fprintf(logFile,"====================================================\r\n");
    fprintf(logFile,"IToolbarItem\r\n");
    fprintf(logFile,"====================================================\r\n");
    hr = pKadManager->QueryInterface(IID_IToolbars,(LPVOID *) &pToolbars);
	if (FAILED(hr)) {
        fprintf(logFile,"QueryInterface failed (IID_IToolbars)\r\n");
	} else {
        hr = pToolbars->GetToolbarById(L"TOOLBAR2",&pToolbar);
        if (hr != S_OK) {
            fprintf(logFile,"GetToolbarById failed (%08x)\r\n",hr);
        } else {
            hr = pToolbar->GetToolbarItemEnumerator(&pEnumToolbarItems);
            if (hr != S_OK) {
                fprintf(logFile,"GetToolbarItemEnumerator failed (%08x)\r\n",hr);
            } else {
                fprintf(logFile,"GetToolbarItemEnumerator succeeded\r\n");
                hr = pEnumToolbarItems->Reset();
                if (hr != S_OK) {
                    fprintf(logFile,"Reset failed (%08x)\r\n",hr);
                } else {
                    fprintf(logFile,"Reset succeeded\r\n");
                    hr = pEnumToolbarItems->Next(1,&pToolbarItem,&ulToolbarItems);
                    if (hr != S_OK) {
                        fprintf(logFile,"Next failed (%08x)\r\n",hr);
                    } else {
                        fprintf(logFile,"Next succeeded\r\n");
                        TEST_FUNCTION_STRING(pToolbarItem,GetIcon)
                        TEST_FUNCTION_GUID(pToolbarItem,GetHandler)
                        TEST_FUNCTION_STRING(pToolbarItem,GetDispatchId)
                        TEST_FUNCTION_STRING(pToolbarItem,GetStatusBarText)
                        TEST_FUNCTION_STRING(pToolbarItem,GetToolTip)
                        TEST_FUNCTION_BOOL(pToolbarItem,GetSeparatorFlag)
                        pToolbarItem->Release();
                    }
                }
                pEnumToolbarItems->Release();
            }
		    pToolbar->Release();
        }
        pToolbars->Release();
	}
}

void TypeTest(IUnknown *pKadManager, FILE *logFile)
{
	HRESULT hr;
    BOOL bBool;
    ITypes *pTypes;
    IType *pType;
    IMenu *pMenu;
    IState *pState;
    IEnumStates *pEnumStates;
    BSTR sString;
    BOOL bDone;
    ULONG ulStates;

    fprintf(logFile,"====================================================\r\n");
    fprintf(logFile,"IType\r\n");
    fprintf(logFile,"====================================================\r\n");
    hr = pKadManager->QueryInterface(IID_ITypes,(LPVOID *) &pTypes);
	if (FAILED(hr)) {
        fprintf(logFile,"QueryInterface failed (IID_ITypes)\r\n");
	} else {
        hr = pTypes->GetTypeById(L"PC",&pType);
        if (hr != S_OK) {
            fprintf(logFile,"GetTypeById failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetTypeById succeeded\r\n");
            TEST_FUNCTION_STRING(pType,GetIcon)
            TEST_FUNCTION_STRING(pType,GetId)
            TEST_FUNCTION_BOOL(pType,GetVisibilityFlag)
            TEST_FUNCTION_BOOL(pType,GetProjectListViewFlag)
            hr = pType->GetMenu(&pMenu);
            if (hr != S_OK) {
                fprintf(logFile,"GetMenu failed (%08x)\r\n",hr);
            } else {
                fprintf(logFile,"GetMenu succeeded\r\n");
                TEST_FUNCTION_STRING(pMenu,GetId)
                pMenu->Release();
            }
            hr = pType->GetStateEnumerator(&pEnumStates);
            if (hr != S_OK) {
                fprintf(logFile,"GetStateEnumerator failed (%08x)\r\n",hr);
            } else {
                fprintf(logFile,"GetStateEnumerator succeeded\r\n");
                hr = pEnumStates->Reset();
                if (hr != S_OK) {
                    fprintf(logFile,"Reset failed (%08x)\r\n",hr);
                } else {
                    fprintf(logFile,"Reset succeeded\r\n");
                    bDone = FALSE;
                    while (! bDone) {
                        hr = pEnumStates->Next(1,&pState,&ulStates);
                        if (hr != S_OK) {
                            fprintf(logFile,"Next failed (%08x)\r\n",hr);
                            bDone = TRUE;
                        } else {
                            fprintf(logFile,"Next succeeded\r\n");
                            TEST_FUNCTION_STRING(pState,GetText)
                            pState->Release();
                        }
                    }
                }
                pEnumStates->Release();
            }
            pType->Release();
        }
        pTypes->Release();
	}
}

void StateTest(IUnknown *pKadManager, FILE *logFile)
{
	HRESULT hr;
    ITypes *pTypes;
    IType *pType;
    IState *pState;
    IEnumStates *pEnumStates;
    BSTR sString;
    ULONG ulStates;

    fprintf(logFile,"====================================================\r\n");
    fprintf(logFile,"IState\r\n");
    fprintf(logFile,"====================================================\r\n");
    hr = pKadManager->QueryInterface(IID_ITypes,(LPVOID *) &pTypes);
	if (FAILED(hr)) {
        fprintf(logFile,"QueryInterface failed (IID_ITypes)\r\n");
	} else {
        hr = pTypes->GetTypeById(L"PC",&pType);
        if (hr != S_OK) {
            fprintf(logFile,"GetTypeById failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetTypeById succeeded\r\n");
            hr = pType->GetStateEnumerator(&pEnumStates);
            if (hr != S_OK) {
                fprintf(logFile,"GetStateEnumerator failed (%08x)\r\n",hr);
            } else {
                fprintf(logFile,"GetStateEnumerator succeeded\r\n");
                hr = pEnumStates->Reset();
                if (hr != S_OK) {
                    fprintf(logFile,"Reset failed (%08x)\r\n",hr);
                } else {
                    fprintf(logFile,"Reset succeeded\r\n");
                    hr = pEnumStates->Next(1,&pState,&ulStates);
                    if (hr != S_OK) {
                        fprintf(logFile,"Next failed (%08x)\r\n",hr);
                    } else {
                        fprintf(logFile,"Next succeeded\r\n");
                        TEST_FUNCTION_STRING(pState,GetText)
                        TEST_FUNCTION_STRING(pState,GetIcon)
                        TEST_FUNCTION_STRING(pState,GetOverlay)
                        TEST_FUNCTION_STRING(pState,GetStateValue)
                        pState->Release();
                    }
                }
                pEnumStates->Release();
            }
            pType->Release();
        }
        pTypes->Release();
	}
}

void TargetOnlineTest(IUnknown *pKadManager, FILE *logFile)
{
	HRESULT hr;
    ITargets *pTargets;
    ITarget *pTarget;
    ITargetOnline *pTargetOnline;
    GUID guid;
    BSTR sString;

    fprintf(logFile,"====================================================\r\n");
    fprintf(logFile,"ITargetOnline\r\n");
    fprintf(logFile,"====================================================\r\n");
    hr = pKadManager->QueryInterface(IID_ITargets,(LPVOID *) &pTargets);
	if (FAILED(hr)) {
        fprintf(logFile,"QueryInterface failed (IID_ITargets)\r\n");
	} else {
        hr = pTargets->GetTargetByType(L"PC",&pTarget);
        if (hr != S_OK) {
            fprintf(logFile,"GetTargetByType failed (%08x)\r\n",hr);
        } else {
            fprintf(logFile,"GetTargetByType succeeded\r\n");
            hr = pTarget->GetTargetOnline(&pTargetOnline);
            if (hr != S_OK) {
                fprintf(logFile,"GetTargetOnline failed (%08x)\r\n",hr);
            } else {
                fprintf(logFile,"GetTargetOnline succeeded\r\n");
                TEST_FUNCTION_GUID(pTargetOnline,GetCustomConnect)
                TEST_FUNCTION_GUID(pTargetOnline,GetDownload)
                TEST_FUNCTION_GUID(pTargetOnline,GetDownloadChanges)
                TEST_FUNCTION_GUID(pTargetOnline,GetDownloadSelection)
                pTargetOnline->Release();
            }
            pTarget->Release();
        }
        pTargets->Release();
	}
}

void test()
{
	HRESULT hr;
    FILE *logFile;
    IUnknown *pKadManager;

    logFile = fopen("kadtest.log","wb");
    if (logFile == NULL) {
        ::MessageBox(NULL,"error","failed to open log-file",MB_OK);
    } else {
	    hr = CoInitialize(NULL);
        if ((hr == S_OK) || (hr == S_FALSE)) {    
            hr = CoCreateInstance(CLSID_KadManagerComObj,NULL,
                CLSCTX_INPROC_SERVER,IID_IUnknown,(LPVOID *) &pKadManager);
            if (FAILED(hr)) {
                fprintf(logFile,"CoCreateInstance failed (CLSID_KadManagerComObj,IID_IUnknown)\r\n");
            } else {
                GeneralTest(pKadManager,logFile);
                FilesTest(pKadManager,logFile);
                ClassesTest(pKadManager,logFile);
                MainMenuTest(pKadManager,logFile);
                ToolbarsTest(pKadManager,logFile);
                TypesTest(pKadManager,logFile);
                TargetsTest(pKadManager,logFile);
                FileTest(pKadManager,logFile);
                ClassTest(pKadManager,logFile);
                TargetTest(pKadManager,logFile);
                MenuTest(pKadManager,logFile);
                MenuItemTest(pKadManager,logFile);
                ToolbarTest(pKadManager,logFile);
                ToolbarItemTest(pKadManager,logFile);
                TypeTest(pKadManager,logFile);
                StateTest(pKadManager,logFile);
                TargetOnlineTest(pKadManager,logFile);
                pKadManager->Release();
            }
		    CoUninitialize();
	    } else {
		    ::MessageBox(NULL,"CoInitialize failed","Error",MB_OK);
	    }
        fclose(logFile);
        system("notepad kadtest.log");
    }
}